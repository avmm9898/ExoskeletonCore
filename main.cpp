#include "mainwindow.h"
#include <QApplication>
#include <QMetaType>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //register type for passing value between signal/slot
    qRegisterMetaType<receive_imusol_packet_t>("receive_imusol_packet_t");

    MainWindow w;
    w.show();

    return a.exec();
}
