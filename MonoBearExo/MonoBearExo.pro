#-------------------------------------------------
#
# Project created by QtCreator 2020-12-19T17:26:43
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MonoBearExo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    maxonmotor.cpp \
    imu/imu_data_decode.cpp \
    imu/packet.cpp \
    chserialport.cpp \
    libsvm/svm.cpp \
    imu/imusvm.cpp

HEADERS += \
        mainwindow.h \
    maxonmotor.h \
    imu/imu_data_decode.h \
    imu/packet.h \
    chserialport.h \
    libsvm/svm.h \
    imu/imusvm.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L$$PWD/../EPOS_Linux_Library/lib/arm/v7/ -lEposCmd.6.6.2.0\
        -L$$PWD/../EPOS_Linux_Library/lib/arm/v7/ -lftd2xx.1.4.8\
        -lwiringPi


INCLUDEPATH += $$PWD/../EPOS_Linux_Library/include
DEPENDPATH += $$PWD/../EPOS_Linux_Library/include
