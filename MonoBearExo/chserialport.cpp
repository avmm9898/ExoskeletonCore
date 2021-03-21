#include "chserialport.h"

CHSerialPort::CHSerialPort(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();

    CH_serial = new QSerialPort();

    //move those to second thread
    this->moveToThread(m_thread);
    CH_serial->moveToThread(m_thread);

    //cross threads signals
    connect(m_thread, SIGNAL(started()), this, SLOT(on_thread_started()),Qt::QueuedConnection);
    connect(m_thread, SIGNAL(finished()), this, SLOT(on_thread_stopped()),Qt::QueuedConnection);

    //be able to close port and thread from baseform
    connect(this, SIGNAL(sigCloseThreadAndPort()), this, SLOT(closeThreadAndPort()));
}
CHSerialPort::~CHSerialPort()
{
    sigCloseThreadAndPort();
}
void CHSerialPort::initThreadReading()
{
    if(m_thread->isRunning()){
        m_thread->quit();
        m_thread->wait();
    }
    m_thread->start();

}

int CHSerialPort::openSerialport(QString port_name, int baudrate)
{

    CH_serial->setPortName(port_name);
    CH_serial->setBaudRate(baudrate);
    CH_serial->setDataBits(QSerialPort::Data8);
    CH_serial->setParity(QSerialPort::NoParity);
    CH_serial->setStopBits(QSerialPort::OneStop);
    CH_serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!CH_serial->open(QIODevice::ReadWrite)) {
        return -1;
    }
    return 0;
}

void CHSerialPort::closePort(){
    emit sigCloseThreadAndPort();
}

void CHSerialPort::closeThreadAndPort()
{
    while(1){
        if(CH_serial->isOpen()) {
            CH_serial->disconnect();
            CH_serial->close();
        }
        else {
            emit sigPortClosed();
            break;
        }
    }
}
void CHSerialPort::quitmThread(){
    m_thread->quit();
    m_thread->wait();
    qDebug()<<"Port and thread are closed";
}



void CHSerialPort::linkCHdevices(QString port_name, int baudrate)
{
    imu_data_decode_init();

    m_port_name=port_name;
    m_baudrate=baudrate;
    initThreadReading();
}

void CHSerialPort::on_thread_started()
{

    connect(CH_serial, SIGNAL(readyRead()), this, SLOT(handleData()), Qt::QueuedConnection);

    int ret=openSerialport(m_port_name, m_baudrate);
    if(ret==-1){
        closePort();
        emit errorOpenPort();
    }
    else{
        emit sigPortOpened();
        qDebug() << "serial port thread is:" << QThread::currentThreadId();
    }
}

void CHSerialPort::on_thread_stopped()
{
    receive_gwsol.tag=0;
    receive_gwsol.n=0;
}
void CHSerialPort::handleData()
{

    if(CH_serial->bytesAvailable() > 0 && CH_serial->isReadable())
    {
        long long NumberOfBytesToRead=CH_serial->bytesAvailable();

        QByteArray arr = CH_serial->readAll();


        for (int i=0;i<NumberOfBytesToRead;i++) {
            uint8_t c=arr[i];
            packet_decode(c);
        }

        mutex_writing.lock();

        if(m_frame_received!=frame_count){

            emit sigSendIMU(receive_imusol);
        }

        m_frame_received=frame_count;
        mutex_writing.unlock();
    }


}



