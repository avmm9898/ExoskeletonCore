#include "maxonmotor.h"


MaxonMotor::MaxonMotor()
{
    keyHandle=0;
}


bool MaxonMotor::openDevice(QString *str_ErrorCode)
{
    unsigned int errorCode = 0;
    closeAllDevice();
    char deviceName[6]="EPOS4";
    char protocalStackName[16] = "MAXON SERIAL V2";
    char interfaceName[4] = "USB";
    char *portName= "USB0";
    keyHandle = VCS_OpenDevice(deviceName,protocalStackName,interfaceName,portName,&errorCode);
    if(keyHandle == 0){
        //        *str_ErrorCode = "0x" + QString::number(errorCode,16);
        qDebug()<<keyHandle;
        return false;
    }
    qDebug()<<"success";
    return true;
}

bool MaxonMotor::closeAllDevice()
{
    unsigned int errorCode = 0;
    if(!VCS_CloseAllDevices(&errorCode))
    {
        return false;
    }
    qDebug()<<"close motor";
    return true;
}

bool MaxonMotor::setmotor_Operationmode(QString *str_ErrorCode)
{
    unsigned int errorCode = 0;
    if(!VCS_SetOperationMode(keyHandle,node_ID,1,&errorCode)){
        return false;
    }
    VCS_ActivateProfilePositionMode(keyHandle,node_ID,&errorCode);
    if(!VCS_SetPositionProfile(keyHandle,node_ID,3600,500,500,&errorCode)){  //Velocity, Acceleration, Deceleration
        qDebug()<<"set success";
    }
    return true;

}

bool MaxonMotor::moveToPosition(long position_rpm)
{
    unsigned int errorCode = 0;
    long targetPosition = position_rpm;
    if(!VCS_MoveToPosition(keyHandle,node_ID,targetPosition,1,1,&errorCode)){
        qDebug()<<"something wrong";
        return false;
    }
    //qDebug()<<"move to"<<targetPosition;
    return true;

}


bool MaxonMotor::haltMotor(){
    unsigned int errorCode = 0;
    if(!VCS_HaltPositionMovement(keyHandle,node_ID,&errorCode)){
        qDebug()<<"halt wrong";
        return false;
    }

    return true;
}
bool MaxonMotor::setEnabled(QString *str_ErrorCode)
{
    unsigned int errorCode = 0;
    if(!VCS_SetEnableState(keyHandle,node_ID,&errorCode))
    {
        qDebug()<<"enable wrong";
        return false;
    }
    qDebug()<<"enable success";
    return true;

}

bool MaxonMotor::setDisabled(QString *str_ErrorCode)
{
    unsigned int errorCode = 0;
    if(!VCS_SetDisableState(keyHandle,node_ID,&errorCode))
    {
        qDebug()<<"disable wrong";
        return false;
    }
    qDebug()<<"disable success";
    return true;
}

bool MaxonMotor::getCurrentValue(short current_value)
{
    unsigned int errorCode = 0;
    if(!VCS_GetCurrentIs(keyHandle,node_ID,&current_value,&errorCode))
    {
        return false;
    }
    return true;

}
bool MaxonMotor::getPosition(int *position_value)
{
    unsigned int errorCode = 0;
    if(!VCS_GetPositionIs(keyHandle, node_ID, position_value, &errorCode))
    {
        return false;
    }
    //qDebug()<<position_value;
    return true;

}
