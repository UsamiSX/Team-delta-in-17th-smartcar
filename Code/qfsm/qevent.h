#ifndef QEVENT_H
#define QEVENT_H

#include "common.h"
#include "qep.h"

typedef uint16 QSignal;

typedef struct QEventTag
{
    QSignal sig;
    uint8 dynamic_;
}QEvent;





#endif
