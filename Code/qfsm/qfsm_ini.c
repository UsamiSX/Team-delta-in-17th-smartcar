#include "qep.h"
#include "qevent.h"


extern QEvent QEP_reservedEvt_[];

void QFsm_init(QFsm *me, QEvent const *e)
{
    (*me->state)(me, e);
    (void)(*me->state)(me , &QEP_reservedEvt_[Q_ENTRY_SIG]);
}



