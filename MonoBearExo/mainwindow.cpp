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

    encoder_timer = new QTimer(this);
    connect(encoder_timer,  SIGNAL(timeout()) ,this, SLOT(getpos()));

    encoder_timer->setInterval(10);
    encoder_timer->start();

    //svm
    m_IMUsvm=new IMUsvm();
    svm_trainer_timer = new QTimer(this);
    connect(svm_trainer_timer,  SIGNAL(timeout()) ,this, SLOT(collectTrainingData()));
    svm_trainer_timer->setInterval(10);

    //start au assist
    AIassistant_timer = new QTimer(this);
    connect(AIassistant_timer,  SIGNAL(timeout()) ,this, SLOT(controlMachine()));
    AIassistant_timer->setInterval(10);

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
    static QList<float> imu_roll_3mean;
    static QList<float> imu_gyrX_3mean;

    imu_roll_3mean.append(imu_data.eul[0]);
    imu_gyrX_3mean.append(imu_data.acc[0]);
    if(imu_roll_3mean.length()>3){
        imu_roll_3mean.pop_front();
        imu_gyrX_3mean.pop_front();
    }

    imu_roll=(imu_roll_3mean.at(0)+imu_roll_3mean.at(1)+imu_roll_3mean.at(2))/3.0f;
    imu_gyrX=(imu_gyrX_3mean.at(0)+imu_gyrX_3mean.at(1)+imu_gyrX_3mean.at(2))/3.0f;

    raw_imu_data=imu_data;

}

void MainWindow::collectTrainingData()
{
    static QStringList motion_data;
    static int windowsize_counter=0;
    static std::vector<double> vec_accX,vec_accY,vec_accZ,
            vec_gyrX,vec_gyrY,vec_roll,vec_pitch;

    const uint training_amount=12;

    uint num_motion=motion_data.length();

    //define bending area of angles
    bool bending_area=(imu_roll<75 &&imu_roll>0);

    if(bending_area){

        //true until motion.data.length() >= training amount

        if(windowsize_counter>=0 && windowsize_counter<=100){

            vec_accX.push_back(raw_imu_data.acc[0]);
            vec_accY.push_back(raw_imu_data.acc[1]);
            vec_accZ.push_back(raw_imu_data.acc[2]);
            vec_gyrX.push_back(raw_imu_data.gyr[0]);
            vec_gyrY.push_back(raw_imu_data.gyr[1]);
            vec_roll.push_back(raw_imu_data.eul[0]);
            vec_pitch.push_back(raw_imu_data.eul[1]);
            windowsize_counter++;

        }else if(windowsize_counter>100){

            int label=0;
            if(num_motion<training_amount/2)
                label=1;//fast motion
            else if(num_motion>=training_amount/2 && num_motion<training_amount)
                label=0;//lifting motion

            double std_accX=stddev(vec_accX);
            double std_accY=stddev(vec_accY);
            double std_accZ=stddev(vec_accZ);
            double std_gyrX=stddev(vec_gyrX);
            double std_gyrY=stddev(vec_gyrY);
            double std_roll=stddev(vec_roll);
            double std_pitch=stddev(vec_pitch);
            QString a_row_data=QString("%1,%2,%3,%4,%5,%6,%7,%8\n").arg(label).arg(QString::number(std_accX, 'f', 3))
                    .arg(QString::number(std_accY, 'f', 3)).arg(QString::number(std_accZ, 'f', 3))
                    .arg(QString::number(std_gyrX, 'f', 3)).arg(QString::number(std_gyrY, 'f', 3))
                    .arg(QString::number(std_roll, 'f', 3)).arg(QString::number(std_pitch, 'f', 3));

            motion_data.append(a_row_data);
            ui->textBrowser->append(tr("A row %1 has been recorded: %2").arg(num_motion).arg(a_row_data));

            //stop pushing new data until leaving bending area;
            windowsize_counter=-1;}

    }else{//not in bending area

        //restart collect a window of data
        if(windowsize_counter!=0){
            windowsize_counter=0;
            vec_accX.clear();
            vec_accY.clear();
            vec_accZ.clear();
            vec_gyrX.clear();
            vec_gyrY.clear();
            vec_roll.clear();
            vec_pitch.clear();}}

    if(num_motion>=training_amount){
        bool err=saveData2CSV(motion_data, person_name);

        if(err){
            ui->textBrowser->append(person_name+".csv can't be opened");
        }else{
            bool err1=m_IMUsvm->trainSVM(person_name);

            if(err1){
                ui->textBrowser->append("train svm error!");
            }else{
                ui->textBrowser->append(person_name+".model"+" is trained");}
        }

        //stop training
        windowsize_counter=0;
        vec_accX.clear();
        vec_accY.clear();
        vec_accZ.clear();
        vec_gyrX.clear();
        vec_gyrY.clear();
        vec_roll.clear();
        vec_pitch.clear();
        motion_data.clear();
        svm_trainer_timer->stop();}

}



bool MainWindow::saveData2CSV(QStringList data, QString person_profile)
{
    QFile file(person_profile+".csv");

    if (!file.open(QFile::WriteOnly | QFile::Truncate |QIODevice::Append)) {
        qDebug() << file.errorString();
        return 1;
    }else{
        QTextStream stream(&file);

        for (int i=0; i<data.length(); i++) {
            stream << data.at(i);
        }}

    file.close();
    return 0;
}

int MainWindow::collectPredictionData()
{
    static int windowsize_counter=0;
    static std::vector<double> vec_accX,vec_accY,vec_accZ,
            vec_gyrX,vec_gyrY,vec_roll,vec_pitch;

    //define bending area of angles
    bool bending_area=(imu_roll<75 &&imu_roll>-90);

    if(bending_area){
        if(windowsize_counter>=0 && windowsize_counter<=100){

            vec_accX.push_back(raw_imu_data.acc[0]);
            vec_accY.push_back(raw_imu_data.acc[1]);
            vec_accZ.push_back(raw_imu_data.acc[2]);
            vec_gyrX.push_back(raw_imu_data.gyr[0]);
            vec_gyrY.push_back(raw_imu_data.gyr[1]);
            vec_roll.push_back(raw_imu_data.eul[0]);
            vec_pitch.push_back(raw_imu_data.eul[1]);
            windowsize_counter++;

        }else if(windowsize_counter>100){

            double std_accX=stddev(vec_accX);
            double std_accY=stddev(vec_accY);
            double std_accZ=stddev(vec_accZ);
            double std_gyrX=stddev(vec_gyrX);
            double std_gyrY=stddev(vec_gyrY);
            double std_roll=stddev(vec_roll);
            double std_pitch=stddev(vec_pitch);
            QString a_row_data=QString("%1,%2,%3,%4,%5,%6,%7").arg(QString::number(std_accX, 'f', 3))
                    .arg(QString::number(std_accY, 'f', 3)).arg(QString::number(std_accZ, 'f', 3))
                    .arg(QString::number(std_gyrX, 'f', 3)).arg(QString::number(std_gyrY, 'f', 3))
                    .arg(QString::number(std_roll, 'f', 3)).arg(QString::number(std_pitch, 'f', 3));

            int rst=m_IMUsvm->svmPredict(a_row_data, ui->line_person_name->text());
            windowsize_counter=-1;
            return rst;}


    }else{//not in bending area

        //restart collect a window of data
        if(windowsize_counter!=0){
            windowsize_counter=0;
            vec_accX.clear();
            vec_accY.clear();
            vec_accZ.clear();
            vec_gyrX.clear();
            vec_gyrY.clear();
            vec_roll.clear();
            vec_pitch.clear();}}

    return -2;//normally ignoring
}

void MainWindow::controlMachine()
{
    /*
     * timer run this loop every 10ms
     * (while bend to lowest)-90< imu_roll <90(while standing)
     * imu_gyrX>0 while lifting, imu_gyrX<0 while bending
    */


    //define bending area of angles
    bool bending_area=(imu_roll<75 &&imu_roll>-90);

    static int counter_for_lockmode;


    switch (this->control_state) {

    case 0:{

        /*rst=-2 while not in bending area or haven't receive a window of datas(1000ms)
         *When user enter into bending area "<75 degrees", it'll collect a window of datas(1000ms) to predict motion.
         *rst=-1 if couldn't find model
         *When a window of datas(1000ms) is collected, rst will ==0 or 1, which is the prediction result.
         *
        */
        int rst=collectPredictionData();

        if(rst==-1){
            ui->textBrowser->append(ui->line_person_name->text()+".model is not exists");
        }else if(rst==0){//change to lifting motion
            control_state=1;
        }else if(rst==1){//user picking fast, doesn't need motor assistant, so it change to lock detection mode.
            control_state=2;
        }else{
            control_state=0;}

        break;}

    case 1:{//enter into motor lifting state

        if(bending_area){
            //send signal once. It'll shorten the wire, help lifting
            m_motor->moveToPosition(max_pos_limit-10*a_circle);

            //change to lock detection mode.
            this->control_state=2;
        }else{

            //release all wire to free mode
            m_motor->moveToPosition(min_pos_limit);
            this->control_state=0;
            counter_for_lockmode=0;}

        break;}


    case 2:{//lock detection mode. check if body is static.

        if(bending_area){
            //while user keep body at a posture for more than 2000ms, will change state into lock mode.
            if(imu_gyrX<10){
                counter_for_lockmode++;
            }else{
                counter_for_lockmode=0;}

            if(counter_for_lockmode>200){
                this->control_state=3;}

        }else{//not in bending area

            //release all wire to free mode
            m_motor->moveToPosition(min_pos_limit);
            this->control_state=0;
            counter_for_lockmode=0;}

        break;}


    case 3:{//enter into lock mode
        if(bending_area){
            //((standing angle)-imu_roll)/360 *(a_circle of encoder)/(reduction ratio=1/160)))
            int imu_pos=round(max_pos_limit-(90-imu_roll)/360 *a_circle*160);
            if(imu_pos>max_pos_limit){
                imu_pos=max_pos_limit;
            }else if(imu_pos<min_pos_limit){
                imu_pos=min_pos_limit;}

            *pos=imu_pos;
            ui->spinBox->setValue(*pos);

            m_motor->moveToPosition(*pos);
        }else{

            //release all wire to free mode
            m_motor->moveToPosition(min_pos_limit);
            this->control_state=0;
            counter_for_lockmode=0;}

        break;}

    default:{

        //release all wire to free mode
        m_motor->moveToPosition(min_pos_limit);
        this->control_state=0;
        counter_for_lockmode=0;
        break;}}



}


double stddev(std::vector<double> const & func)
{
    double mean = std::accumulate(func.begin(), func.end(), 0.0) / func.size();
    double sq_sum = std::inner_product(func.begin(), func.end(), func.begin(), 0.0,
                                       [](double const & x, double const & y) { return x + y; },
    [mean](double const & x, double const & y) { return (x - mean)*(y - mean); });
    return sqrt(sq_sum / func.size());
}



void MainWindow::getpos()
{

    bool err_handle; //1 if success, 0 if error
    int btn_state=2;
    btn_state=digitalRead(buttonPin);

    //qDebug()<<btn_state;
    ui->Labelbtn->setNum(btn_state);

    err_handle=m_motor->getPosition(pos);

    ui->label_enc_pos->setText(tr("Current Pos= %1").arg(*pos));

    if(err_handle){

        //while limitation btn is pressed
        if(btn_state!=1){
            //ui->label_limitbtn->setText("Motor Reach Up Limitation!");
            if(max_pos_limit>*pos){
                if(m_motor->haltMotor()){
                    qDebug()<<"halt success";}

                //max_pos_limit:the wire is shortest, min_pos_limit:the wire is released
                //while reach max, reset the max_pos_limit, and move it to see if it's still touch limitation btn
                max_pos_limit=*pos-a_circle;

                //the free position is: a degree=16384*(reduction ratio)/360, we set to 200 bending degrees of body.
                min_pos_limit=max_pos_limit-a_circle*160.0f/360.0f*200.0f;
                ui->label_pos_limitation->setText(tr("MaxPos=%1,MinPos=%2").arg(max_pos_limit).arg(min_pos_limit));

                m_motor->moveToPosition(max_pos_limit);

            }else{//if (max_pos_limit <= current position)
                *pos-=a_circle;
                m_motor->moveToPosition(*pos);
            }

        }else{
            //btn is not pressed
            //ui->label_limitbtn->setText("Motor is In Save Area");
        }
    }


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
                //ui->label_limitbtn->setText("Motor Reach Up Limitation!");

                if(m_motor->haltMotor()){
                    qDebug()<<"halt success";}

                //calibrate the max and min limitation here;
                max_pos_limit=*pos-a_circle;
                min_pos_limit=max_pos_limit-a_circle*160.0f/360.0f*200.0f;
                ui->label_pos_limitation->setText(tr("MaxPos=%1,MinPos=%2").arg(max_pos_limit).arg(min_pos_limit));

                m_motor->moveToPosition(max_pos_limit);
                break;

            }else{
                //ui->label_limitbtn->setText("Motor is In Save Area");
                *pos+=a_circle;
                ui->spinBox->setValue(*pos);

                m_motor->moveToPosition(*pos);}}}

}

void MainWindow::on_btn_train_clicked()
{ 
    person_name=ui->line_person_name->text();
    svm_trainer_timer->start();
}

void MainWindow::on_btn_AIassisstant_clicked()
{
    if(!AIassistant_timer->isActive()){
        AIassistant_timer->start();
        ui->btn_AIassistant->setText("AI is working");
    }else{
        AIassistant_timer->stop();

        //release wire
        m_motor->moveToPosition(min_pos_limit);

        ui->btn_AIassistant->setText("AI is sleeping");}

}

void MainWindow::on_btn_manual_setpos_clicked()
{
    long pos = ui->spinBox->value();
    //    pos = 12800*(10/360)*160*(72/102);
    m_motor->moveToPosition(pos);
}
void MainWindow::on_btn_up_clicked()
{
    ui->spinBox->setValue(ui->spinBox->value()+a_circle);
    long pos = ui->spinBox->value();
    //m_motor->moveToPosition(pos);
}

void MainWindow::on_btn_down_clicked()
{
    ui->spinBox->setValue(ui->spinBox->value()-a_circle);

    //m_motor->moveToPosition(pos);
}
