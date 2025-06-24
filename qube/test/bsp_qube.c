#include "qpc.h"
#include "bsp.h"
#include "qhsmtst.h"

//Q_DEFINE_THIS_FILE

//..........................................................................
void BSP_init(void) {
    //Qube_setAO(AO_QHsmTst); /* register the AO with Qube */
}
//..........................................................................
void BSP_exit(void) {
    QF_stop();
}
//..........................................................................
void BSP_display(char const *msg) {
    //PRINTF_S("%s", msg);
    // application-specific record
    QS_BEGIN_ID(QS_USER, 0U)
        QS_STR(msg);
    QS_END()
}
//..........................................................................
// callback function to generate an event for this application
QEvt const* Qube_onGenEvt(QSignal sig, char const* params) {
    Q_UNUSED_PAR(params); // QHsmTst does not use params

    static QEvt evt;
    QEvt_ctor(&evt, sig);
    return &evt;
}
