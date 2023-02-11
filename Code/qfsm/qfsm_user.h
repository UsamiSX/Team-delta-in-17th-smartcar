#ifndef QFSM_USER_H
#define QFSM_USER_H

#include "motor.h"
#include "pid.h"



#define value0 angleSpeed.kp
#define value1 angleSpeed.ki
#define value2 angleSpeed.kd
#define value3 angle.kp
#define value4 brushlessSpeed.kp
#define value5 brushlessSpeed.ki
#define value6 brushlessSpeed.kd


enum userSignal
{
    Btn0Prsd = Q_USER_SIG,
    Btn1Prsd,
    Btn2Prsd,
};


extern uint8 signParmSetting;
QState ParmSetting(QFsm *me, QEvent *e);
QState ParmDisplay(QFsm *me, QEvent *e);
QState ImgDisplay(QFsm *me, QEvent *e);
void SettingPage();
void DisplayPage();




#endif
