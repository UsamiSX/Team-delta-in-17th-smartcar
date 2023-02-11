#include "qep.h"
#include "qevent.h"
#include "qfsm_user.h"


extern QEvent QEP_reservedEvt_[];


void QFsm_dispatch(QFsm *me, QEvent const *e)
{
    QStateHandler s = me->state;
    QState r = (*s)(me, e);
    if (r == Q_RET_TRAN)
    {
        (void)(*s)(me, &QEP_reservedEvt_[Q_EXIT_SIG]);
        (void)(*me->state)(me, &QEP_reservedEvt_[Q_ENTRY_SIG]);
    }
}


