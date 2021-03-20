#ifndef CHSERIALPORT_H
#define CHSERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QString>
#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QMessageBox>
#include <QTimer>
#include <QMutex>

#include "imu/packet.h"
#include "imu/imu_data_decode.h"

class CHSerialPort : public QObject
{
    Q_OBJECT


public:
    explicit CHSerialPort(QObject *parent = nullptr);
    ~CHSerialPort();

    int openSerialport(QString, int);

    QSerialPort *CH_serial = nullptr;

    receive_imusol_packet_t *IMU_data=&receive_imusol;

    QByteArray CH_rawmsg="";

public slots:
    void closePort();
    void linkCHdevices(QString, int);
    void quitmThread();

signals:
    //emit data to baseform
    void sigSendIMU(receive_imusol_packet_t);

    //port status handle
    void errorOpenPort();
    void sigPortOpened();
    void sigPortClosed();
    void sigCloseThreadAndPort();



private:

    QMutex mutex_writing;
    QString m_port_name;
    QThread *m_thread;
    int m_baudrate;

    uint m_frame_received=0;

private slots:
    void on_thread_started();
    void on_thread_stopped();
    void initThreadReading();
    void closeThreadAndPort();

    //handle all data from serial
    void handleData();


};

#endif // CHSERIALPORT_H
