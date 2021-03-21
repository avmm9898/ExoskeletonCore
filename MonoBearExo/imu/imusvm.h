#ifndef IMUSVM_H
#define IMUSVM_H
#include <QDebug>
#include <QFile>
#include <math.h>
#include <QFileInfo>

#include "libsvm/svm.h"

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

class IMUsvm
{
public:
    IMUsvm();
    int svmPredict(QString data, QString t_person_filename);
    bool trainSVM(QString filename);

private:
    bool fileExists(QString path);
    void initSVM();
    bool read_problem(QString filename);
    bool predict_file(QString filename);
    QString person_name;

    struct svm_parameter param;		// set by parse_command_line
    struct svm_problem prob;		// set by read_problem
    struct svm_model *model;
    QList<svm_node*> train_node_list;


};

#endif // IMUSVM_H
