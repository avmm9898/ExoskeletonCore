#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Definitions.h>
#include <QTimer>
#include <QString>
#include <libsvm/svm.h>

#include <wiringPi.h>
#include "chserialport.h"
#include "maxonmotor.h"
#include <math.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void getIMUData(receive_imusol_packet_t imu_data);

    void on_pushButton_clicked();

    void getpos();

    void on_btn_up_clicked();

    void on_btn_down_clicked();

    void motorCalibrate();



    void on_btn_train_clicked();

private:
    Ui::MainWindow *ui;

    //motor
    MaxonMotor *m_motor;
    void* keyHandle;
    QTimer *timer;
    int *pos;

    //imu port
    CHSerialPort *ch_serialport;

    //max and min pos limitation
    int max_pos_limit=40000;
    int min_pos_limit=-40000;
    float imu_roll=0;



};

#endif // MAINWINDOW_H
