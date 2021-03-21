#include "imusvm.h"

IMUsvm::IMUsvm()
{

}

int IMUsvm::svmPredict(QString line, QString t_person_name)
{
    if(fileExists(t_person_name+".model")){
        if(this->person_name!=t_person_name){
            this->person_name=t_person_name;
            char *c_savefilename=(t_person_name+".model").toLocal8Bit().data();
            model=svm_load_model(c_savefilename);
        }

        //int svm_type=svm_get_svm_type(model);
        int nr_class=svm_get_nr_class(model);


        //ex:"accx,accy,accz,gyrx,gyry,roll,pitch"
        QStringList a_row_data=line.split(',');

        //-1 is because there is no label at first
        svm_node a_node[8];

        for(int feature=0;feature<7;feature++){
            a_node[feature].index=feature+1;
            a_node[feature].value=a_row_data.at(feature).toDouble();
        }

        a_node[7].index = -1;

        int *labels=(int *) malloc(nr_class*sizeof(int));
        svm_get_labels(model,labels);

        double label = svm_predict(model, a_node);
        label=round(label);

        return int(label);
    }else{
        return -1;
    }
}

bool fileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

bool IMUsvm::trainSVM(QString t_person_name)
{

    initSVM();
    read_problem(t_person_name+".csv");


    //train model
    char *c_savefilename=(t_person_name+".model").toLocal8Bit().data();
    const char *error_msg;

    error_msg = svm_check_parameter(&prob,&param);

    if(error_msg)
    {
        qDebug()<<QString("parameter error: %s").arg(error_msg);
        return 1;
    }

    model = svm_train(&prob,&param);
    if(svm_save_model(c_savefilename,model))
    {
        qDebug()<<QString("can't save model to file %s\n").arg(c_savefilename);
        return 1;
    }

    svm_free_and_destroy_model(&model);
    svm_destroy_param(&param);
    free(prob.y);
    free(prob.x);
    qDebug()<<QString("%s is saved").arg(c_savefilename);

    return 0;
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

    person_name="";
}

bool IMUsvm::read_problem(QString filename)
{

    QStringList line_list;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return 1;
    }

    //read all line
    while (!file.atEnd()){
        line_list.append(file.readLine());
    }

    //check if any line exists
    if(line_list.length()<=0){
        qDebug() << "no line in file";
        return 1;
    }

    //empty row
    if(line_list.last().indexOf(',')==-1)
        line_list.pop_back();

    prob.l=line_list.length();
    prob.y = Malloc(double,prob.l);
    prob.x = Malloc(struct svm_node *,prob.l);


    for(int i=0;i<prob.l;i++)
    {

        QStringList a_row_data= line_list.at(i).split(QLatin1Char(','));

        //check length
        if(a_row_data.length()<=0){
            qDebug() << "no feature in a line";
            return 1;
        }


        svm_node *a_node;
        a_node = Malloc(struct svm_node,8);

        double label;
        label=a_row_data.first().toInt();
        prob.y[i] = label;

        for(int feature=0;feature<7;feature++){
            a_node[feature].index=feature+1;
            a_node[feature].value=a_row_data.at(feature+1).toDouble();

        }

        a_node[7].index = -1;

        train_node_list.append(a_node);
        prob.x[i] = train_node_list[i];
    }
    file.close();
    return 0;
}

bool IMUsvm::predict_file(QString filename)
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
    qDebug()<<QString("Error=%1%").arg(round(wrong/row_counter*100));
    return 0;
}

