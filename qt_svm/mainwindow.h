#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFile>

#include "libsvm/svm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_BTN_Read_clicked();

private:
    Ui::MainWindow *ui;
    void exit_input_error(int line_num)
    {
        fprintf(stderr,"Wrong input format at line %d\n", line_num);
        exit(1);
    }

    void initSVM();
    bool read_problem(const char *filename);

    bool predict_file(QString);

    void do_cross_validation();

    struct svm_parameter param;		// set by parse_command_line
    struct svm_problem prob;		// set by read_problem
    struct svm_model *model;
    QList<svm_node*> node_list;



};
#endif // MAINWINDOW_H
