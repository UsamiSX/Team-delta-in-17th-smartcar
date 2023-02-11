#ifndef QEP_H
#define QEP_H

#include "common.h"
#include "qevent.h"

typedef uint8 QState;

typedef QState (*QStateHandler) (void *me, QEvent const *e);

typedef struct QFsmTag
{
    QStateHandler state;
} QFsm;

enum QReservedSignals
{
    Q_ENTRY_SIG = 0,
    Q_EXIT_SIG,
    Q_INIT_SIG,
    Q_USER_SIG
};


#define QFsm_ctor(me_, initial_) ((me_)->state = (initial_))
void QFsm_init (QFsm *me, QEvent const *e);
void QFsm_dispatch(QFsm *me, QEvent const *e);
#define Q_RET_HANDLED ((QState)0)
#define Q_RET_IGNORED ((QState)1)
#define Q_RET_TRAN    ((QState)2)
#define Q_HANDLED() (Q_RET_HANDLED)
#define Q_IGNORED() (Q_RET_IGNORED)
#define Q_TRAN(target_) \
(((QFsm *)me)->state = (QStateHandler)(target_), Q_RET_TRAN)



#endif

