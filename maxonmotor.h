#ifndef MAXONMOTOR_H
#define MAXONMOTOR_H
#include <Definitions.h>
#include <QString>
#include <QDebug>


class MaxonMotor
{
public:
    MaxonMotor();
    bool openDevice(QString *str_ErrorCode);
    bool closeAllDevice();
    bool init(int node_ID,double MaxProfileVelocity,double MaxFollowingError,double MaxAcceleration);
    bool setmotor_Operationmode(QString *str_ErrorCode);
    bool moveToPosition(long position_rpm);
    bool haltMotor();


    bool setEnabled(QString *str_ErrorCode);
    bool setDisabled(QString *str_ErrorCode);
    bool getCurrentValue(short current_value);
    bool getPosition(int *position_value);

private:
    void* keyHandle;
    unsigned short node_ID = 1;

};

#endif // MAXONMOTOR_H
