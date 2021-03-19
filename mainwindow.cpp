#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_cross_validation()
{
    int i;
    int total_correct = 0;
    double total_error = 0;
    double sumv = 0, sumy = 0, sumvv = 0, sumyy = 0, sumvy = 0;
    double *target = Malloc(double,prob.l);

    svm_cross_validation(&prob,&param,nr_fold,target);
    if(param.svm_type == EPSILON_SVR ||
            param.svm_type == NU_SVR)
    {
        for(i=0;i<prob.l;i++)
        {
            double y = prob.y[i];
            double v = target[i];
            total_error += (v-y)*(v-y);
            sumv += v;
            sumy += y;
            sumvv += v*v;
            sumyy += y*y;
            sumvy += v*y;
        }
        printf("Cross Validation Mean squared error = %g\n",total_error/prob.l);
        printf("Cross Validation Squared correlation coefficient = %g\n",
               ((prob.l*sumvy-sumv*sumy)*(prob.l*sumvy-sumv*sumy))/
               ((prob.l*sumvv-sumv*sumv)*(prob.l*sumyy-sumy*sumy))
               );
    }
    else
    {
        for(i=0;i<prob.l;i++)
            if(target[i] == prob.y[i])
                ++total_correct;
        printf("Cross Validation Accuracy = %g%%\n",100.0*total_correct/prob.l);
    }
    free(target);
}

void MainWindow::initSVM()
{
    // default values
    // default values
    param.svm_type = NU_SVC;
    param.kernel_type = RBF;
    param.degree = 3;
    param.gamma = 0.14;	// 1/num_features
    param.coef0 = 0;
    param.nu = 0.5;
    param.cache_size = 200;
    param.C = 0.1;
    param.eps = 0.001;
    param.p = 0.1;
    param.shrinking = 1;
    param.probability = 0;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;
    cross_validation = 0;

}

// read in a problem (in svmlight format)

void MainWindow::read_problem(const char *filename)
{
    int max_index, inst_max_index, i;
    size_t elements, j;
    FILE *fp = fopen(filename,"r");
    double label;

    if(fp == NULL)
    {
        fprintf(stderr,"can't open input file %s\n",filename);
        exit(1);
    }

    prob.l = 0;
    elements = 0;

    max_line_len = 1024;
    line = Malloc(char,max_line_len);
    while(readline(fp)!=NULL)
    {

        char *p = strtok(line,","); // label

        // features
        while(1)
        {
            p = strtok(NULL,",");

            if(p == NULL || *p == '\n') // check '\n' as ' ' may be after the last feature
                break;
            ++elements;
        }
        ++elements;
        ++prob.l;
    }
    rewind(fp);

    prob.y = Malloc(double,prob.l);
    prob.x = Malloc(struct svm_node *,prob.l);

    //x_space = Malloc(struct svm_node,elements);


    node_list.clear();

    max_index = 0;
    j=0;
    for(i=0;i<prob.l;i++)
    {
        inst_max_index = -1; // strtol gives 0 if wrong format, and precomputed kernel has <index> start from 0
        readline(fp);

        svm_node *a_node;
        a_node = Malloc(struct svm_node,8);

        QStringList a_row_data= QString(line).split(QLatin1Char(','));
        label=a_row_data.first().toInt();
        prob.y[i] = label;

        for(int feature=1;feature<a_row_data.length();feature++){
            a_node[feature-1].index=feature;
            a_node[feature-1].value=a_row_data.at(feature).toDouble();

            //ui->textBrowser->append(tr("train-index:%1,value:%2,j=%3").arg(x_space[j].index).arg(x_space[j].value).arg(j));
        }



        a_node[7].index = -1;

        node_list.append(a_node);

        prob.x[i] = node_list[i];

    }


    fclose(fp);
}

void MainWindow::predict_file(QString filename)
{
    double *prob_estimates=NULL;
    int svm_type=svm_get_svm_type(model);
    int nr_class=svm_get_nr_class(model);



    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    }


    QStringList a_row_data;
    int row_counter=0;

    double wrong=0;
    while (!file.atEnd()) {

        QString line = file.readLine();
        a_row_data=line.split(',');


        svm_node a_node[8];

        int actual_label=a_row_data.takeFirst().toInt();

        for(int feature=0;feature<a_row_data.length();feature++){
            a_node[feature].index=feature+1;
            a_node[feature].value=a_row_data.at(feature).toDouble();
            //ui->textBrowser->append(tr("test-index:%1,value:%2,feature=%3").arg(a_node[feature].index).arg(a_node[feature].value).arg(feature));
        }

        a_node[7].index = -1;

        int *labels=(int *) malloc(nr_class*sizeof(int));
        svm_get_labels(model,labels);
        prob_estimates = (double *) malloc(nr_class*sizeof(double));


        double label = svm_predict_probability(model,a_node,prob_estimates);
        label=round(label);

        if(actual_label!=label)
            wrong++;
        //double label = svm_predict(model, a_node);

        ui->textBrowser->append(tr("row:%1,label:%2").arg(row_counter).arg(label));
        row_counter++;

    }
    ui->textBrowser->append(tr("Error= %1 %").arg(round(wrong/row_counter*100)));


}

char* MainWindow::readline(FILE *input)
{
    int len;

    if(fgets(line,max_line_len,input) == NULL)
        return NULL;

    while(strrchr(line,'\n') == NULL)
    {
        max_line_len *= 2;
        line = (char *) realloc(line,max_line_len);
        len = (int) strlen(line);
        if(fgets(line+len,max_line_len-len,input) == NULL)
            break;
    }
    return line;
}



void MainWindow::on_BTN_Read_clicked()
{
    char input_file_name[1024]="5_train_1s.csv";
    char model_file_name[1024]="svm.model";
    QString testfile="5_test_1s.csv";

    const char *error_msg;

    initSVM();
    read_problem(input_file_name);
    error_msg = svm_check_parameter(&prob,&param);

    if(error_msg)
    {
        fprintf(stderr,"ERROR: %s\n",error_msg);
        exit(1);
    }

    if(0)
    {
        do_cross_validation();
    }
    else
    {
        //model=svm_load_model(model_file_name);
        model = svm_train(&prob,&param);
        if(svm_save_model(model_file_name,model))
        {
            fprintf(stderr, "can't save model to file %s\n", model_file_name);
            exit(1);
        }
        predict_file(testfile);


        svm_free_and_destroy_model(&model);
    }
    svm_destroy_param(&param);
    free(prob.y);
    free(prob.x);
    free(x_space);
    free(line);
}
