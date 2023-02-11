#include "qep.h"
#include "qevent.h"
#include "SEEKFREE_18TFT.h"
#include "PID.h"
#include "SEEKFREE_MT9V03X.h"
#include "qfsm_user.h"


#define X_START 8
#define Y_START 1



extern pid_position_struct motor;

extern int numRotary;
uint8 signParmSetting;
uint8 signParmDisplay;
uint8 signImgDisplay;




QState ParmSetting(QFsm *me, QEvent *e)
{
    switch(e->sig)
    {
        case Q_ENTRY_SIG:
        {
            SettingPage();
            signParmSetting = TRUE;
            return Q_HANDLED();
        }
        case Btn0Prsd:
        {
            return Q_IGNORED();
        }
        case Btn1Prsd:
        {
            return Q_TRAN(ParmDisplay);
        }
        case Btn2Prsd:
        {

            return Q_TRAN(ImgDisplay);
        }
        case Q_EXIT_SIG:
        {
            signParmSetting = FALSE;
            return Q_HANDLED();
        }
    }
    return Q_IGNORED();
}


QState ParmDisplay(QFsm *me, QEvent *e)
{
    switch(e->sig)
    {
        case Q_ENTRY_SIG:
        {
            DisplayPage();
            signParmDisplay = TRUE;
            return Q_HANDLED();
        }
        case Btn0Prsd:
        {
            return Q_TRAN(ParmSetting);
        }
        case Btn1Prsd:
        {
            return Q_IGNORED();
        }
        case Btn2Prsd:
        {
            return Q_TRAN(ImgDisplay);
        }
        case Q_EXIT_SIG:
        {
            signParmDisplay = FALSE;
            return Q_HANDLED();
        }

    }
    return Q_IGNORED();
}

QState ImgDisplay(QFsm *me, QEvent *e)
{
    switch(e->sig)
    {
        case Q_ENTRY_SIG:
        {
            lcd_clear(WHITE);
            signImgDisplay = TRUE;
            return Q_HANDLED();
        }
        case Btn0Prsd:
        {
            return Q_TRAN(ParmSetting);
        }
        case Btn1Prsd:
        {

            return Q_TRAN(ParmDisplay);
        }
        case Btn2Prsd:
        {

            return Q_IGNORED();
        }
        case Q_EXIT_SIG:
        {
            signImgDisplay = FALSE;
            return Q_HANDLED();
        }
    }
    return Q_IGNORED();
}

void SettingPage()
{
    lcd_clear(BLACK);
    lcd_showstr(16, 0, "value0:");
    lcd_showfloat(9 * 8, 0, value0,2,2);
    lcd_showstr(16, 1, "value1:");
    lcd_showfloat(9 * 8, 1, value1,1,3);
    lcd_showstr(16, 2, "value2:");
    lcd_showfloat(9 * 8, 2, value2,2,2);
    lcd_showstr(16, 3, "value3:");
    lcd_showfloat(9 * 8, 3, value3,2,2);
    lcd_showstr(16, 4, "value4:");
    lcd_showfloat(9 * 8, 4, value4,2,2);
    lcd_showstr(16, 5, "value5:");
    lcd_showfloat(9 * 8, 5, value5,1,3);
    lcd_showstr(16, 6, "value6:");
    lcd_showfloat(9 * 8, 6, value6,1,3);
}

void DisplayPage()
{
    lcd_clear(BLACK);
    lcd_showstr(16, 0, "v:");
    lcd_showstr(16, 1, "speed:");
    lcd_showstr(16, 2, "angle:");
    lcd_showstr(16, 3, "RES0:");
    lcd_showstr(16, 4, "RES1:");
    lcd_showstr(16, 5, "RES2:");
    lcd_showstr(16, 6, "RES3:");
}
