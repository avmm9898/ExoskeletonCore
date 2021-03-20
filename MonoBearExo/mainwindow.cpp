#include "mainwindow.h"
#include "ui_mainwindow.h"

#define buttonPin 4
#define a_circle 16384

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    wiringPiSetup();
    pinMode(buttonPin,INPUT);
    pullUpDnControl(buttonPin,PUD_UP);

    ch_serialport = new CHSerialPort(nullptr);

    //get data from ch_serialport class
    //    connect(ch_serialport, SIGNAL(errorOpenPort()), this, SLOT(geterrorOpenPort()));
    //    connect(ch_serialport, SIGNAL(sigPortOpened()), this, SLOT(getsigPortOpened()));
    //    connect(ch_serialport, SIGNAL(sigPortClosed()), this, SLOT(getsigPortClosed()));

    connect(ch_serialport, SIGNAL(sigSendIMU(receive_imusol_packet_t)),
            this, SLOT(getIMUData(receive_imusol_packet_t)), Qt::QueuedConnection);
    //link imu
    ch_serialport->linkCHdevices("/dev/ttyUSB0",115200);


    //motor
    QString str_error;

    pos=new int();
    m_motor=new MaxonMotor();
    bool motor_connected = m_motor->openDevice(&str_error);
    bool motor_set = m_motor->setmotor_Operationmode(&str_error);
    bool enable_state = m_motor->setEnabled(&str_error);

    ui->spinBox->setRange(-a_circle*1000,a_circle*1000);
    motorCalibrate();

    timer =new QTimer(this);
    connect(timer,  SIGNAL(timeout()) ,this, SLOT(getpos()));


    timer->setInterval(10);
    timer->start();

}

MainWindow::~MainWindow()
{
    //close imu port and thread
    ch_serialport->closePort();

    QString str_error;
    bool disable_state = m_motor->setDisabled(&str_error);
    m_motor->closeAllDevice();


    delete ui;
}

void MainWindow::getIMUData(receive_imusol_packet_t imu_data)
{

    imu_roll=imu_data.eul[0];
    //qDebug()<<imu_roll;
}


void MainWindow::on_pushButton_clicked()
{
    long pos = ui->spinBox->value();
    //    pos = 12800*(10/360)*160*(72/102);
    m_motor->moveToPosition(pos);
}

void MainWindow::getpos()
{

    bool err_handle; //1 if success, 0 if error
    int btn_state=2;
    btn_state=digitalRead(buttonPin);

    //qDebug()<<btn_state;
    ui->Labelbtn->setNum(btn_state);

    err_handle=m_motor->getPosition(pos);
    m_motor->getPosition(pos);
    ui->labelpos->setText(tr("POS:%1").arg(*pos));

    if(err_handle){
        if(btn_state!=1){
            if(max_pos_limit>*pos){
                if(m_motor->haltMotor()){
                    qDebug()<<"halt success";
                }
                max_pos_limit=*pos;
                min_pos_limit=max_pos_limit-a_circle*160.0f/360.0f*200.0f;
                ui->label_max->setText(tr("max:%1,min:%2").arg(max_pos_limit).arg(min_pos_limit));

                m_motor->moveToPosition(max_pos_limit);
            }
            else{
                *pos-=a_circle;
                m_motor->moveToPosition(*pos);
            }


        }
        else{
            //
            int imu_pos=round(max_pos_limit-abs(imu_roll)/360 *a_circle*160);
            if(imu_pos>max_pos_limit){
                imu_pos=max_pos_limit;
            }
            else if(imu_pos<min_pos_limit){
                imu_pos=min_pos_limit;
            }

            *pos=imu_pos;
            ui->spinBox->setValue(*pos);

            m_motor->moveToPosition(*pos);
        }
    }


}

void MainWindow::on_btn_up_clicked()
{
    ui->spinBox->setValue(ui->spinBox->value()+16384);
    long pos = ui->spinBox->value();
    //m_motor->moveToPosition(pos);
}

void MainWindow::on_btn_down_clicked()
{
    ui->spinBox->setValue(ui->spinBox->value()-16384);

    //m_motor->moveToPosition(pos);
}

void MainWindow::motorCalibrate()
{

    while(true){
        bool err_handle; //1 if success, 0 if error
        err_handle=m_motor->getPosition(pos);

        int btn_state=2;
        btn_state=digitalRead(buttonPin);

        if(err_handle){
            if(btn_state!=1){

                if(m_motor->haltMotor()){
                    qDebug()<<"halt success";
                }
                max_pos_limit=*pos-a_circle;
                min_pos_limit=max_pos_limit-a_circle*160.0f/360.0f*200.0f;
                ui->label_max->setText(tr("max:%1,min:%2").arg(max_pos_limit).arg(min_pos_limit));

                m_motor->moveToPosition(max_pos_limit);
                break;

            }
            else{

                *pos+=a_circle;
                ui->spinBox->setValue(*pos);

                m_motor->moveToPosition(*pos);
            }
        }
    }

}

void MainWindow::on_btn_train_clicked()
{
    if()
}
