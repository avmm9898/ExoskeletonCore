#include "imusvm.h"

IMUsvm::IMUsvm()
{

}

int IMUsvm::trainSVM(QString filename)
{
    char input_file_name[50]=filename.toLocal8Bit().data();
    char model_file_name[50]="svm.model";
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


    //model=svm_load_model(model_file_name);
    model = svm_train(&prob,&param);
    if(svm_save_model(model_file_name,model))
    {
        fprintf(stderr, "can't save model to file %s\n", model_file_name);
        exit(1);
    }
    predict_file(testfile);


    svm_free_and_destroy_model(&model);

    svm_destroy_param(&param);
    free(prob.y);
    free(prob.x);
    free(x_space);
    free(line);
}

void IMUsvm::initSVM()
{
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

void IMUsvm::read_problem(const char *filename)
{

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    }
    QString line=file.readLine();
    QStringList a_row_data=line.split(',');
    while (!file.atEnd()){


    }

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

void IMUsvm::predict_file(QString filename)
{

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
        }

        a_node[7].index = -1;

        int *labels=(int *) malloc(nr_class*sizeof(int));
        svm_get_labels(model,labels);



        double label = svm_predict(model, a_node);
        label=round(label);

        if(actual_label!=label)
            wrong++;

        row_counter++;

    }
    //round(wrong/row_counter*100);

}

char *IMUsvm::readline(FILE *input)
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
