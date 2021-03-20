#ifndef IMUSVM_H
#define IMUSVM_H
#include <QDebug>
#include <QFile>
#include <math.h>

#include "libsvm/svm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

class IMUsvm
{
public:
    IMUsvm();
    int svmPredict(QString);
    int trainSVM(QString);

private:
    void initSVM();
    void read_problem(const char *filename);
    void predict_file(QString);

    char * readline(FILE *input);

    struct svm_parameter param;		// set by parse_command_line
    struct svm_problem prob;		// set by read_problem
    struct svm_model *model;
    struct svm_node *x_space;
    QList<svm_node*> node_list;

    int cross_validation;
    int nr_fold;

    char *line = nullptr;
    int max_line_len;

};

#endif // IMUSVM_H
