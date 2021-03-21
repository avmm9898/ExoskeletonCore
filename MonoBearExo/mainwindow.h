#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTimer>
#include <QString>
#include <math.h>
#include <wiringPi.h>

#include "chserialport.h"
#include <Definitions.h>
#include "maxonmotor.h"

#include <libsvm/svm.h>
#include <imu/imusvm.h>

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

    void on_btn_manual_setpos_clicked();

    void getpos();

    void on_btn_up_clicked();

    void on_btn_down_clicked();

    void motorCalibrate();

    void on_btn_train_clicked();

    void on_btn_AIassisstant_clicked();

    void on_motor_lift();

signals:
    void sig_motor_lift();

private:
    Ui::MainWindow *ui;

    //motor
    MaxonMotor *m_motor;
    void* keyHandle;
    QTimer *encoder_timer;
    int *pos;

    //max and min pos limitation
    int max_pos_limit=40000;
    int min_pos_limit=-40000;
    float imu_roll=0;
    float imu_gyrX=0;

    //imu port
    CHSerialPort *ch_serialport;
    receive_imusol_packet_t raw_imu_data;

    //svm model
    IMUsvm *m_IMUsvm;


    //collect training data
    QTimer *svm_trainer_timer;

    void collectTrainingData();
    bool saveData2CSV(QStringList data, QString person_profile);
    double stddev(std::vector<double> const & func);


    //for a single svm model
    QString person_name;


    //collect prediction data
    QTimer *AIassistant_timer;
    int collectPredictionData();

    //controlMachine activated by AIassistant_timer
    void controlMachine();
    int control_state=0;


};

#endif // MAINWINDOW_H
