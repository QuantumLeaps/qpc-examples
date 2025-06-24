#include "qpc.h"
#include "bsp.h"
#include "qhsmtst.h"

//Q_DEFINE_THIS_FILE

//..........................................................................
int main() {
    QF_init();  // initialize the framework and the underlying RT kernel
    BSP_init(); // initialize the Board Support Package

    // QS dictionaries...
    QS_SIG_DICTIONARY(A_SIG, (void*)0);
    QS_SIG_DICTIONARY(B_SIG, (void*)0);
    QS_SIG_DICTIONARY(C_SIG, (void*)0);
    QS_SIG_DICTIONARY(D_SIG, (void*)0);
    QS_SIG_DICTIONARY(E_SIG, (void*)0);
    QS_SIG_DICTIONARY(F_SIG, (void*)0);
    QS_SIG_DICTIONARY(G_SIG, (void*)0);
    QS_SIG_DICTIONARY(H_SIG, (void*)0);
    QS_SIG_DICTIONARY(I_SIG, (void*)0);
    QS_SIG_DICTIONARY(X_SIG, (void*)0);
    QS_OBJ_DICTIONARY(AO_QHsmTst);

    // QS filters...
    QS_GLB_FILTER(QS_GRP_ALL);

    // publish-subscribe not used, no call to QActive_psInit()
    // dynamic event allocation not used, no call to QF_poolInit()

    // instantiate and start the active objects...
    QHsmTst_ctor();
    static QEvt const *l_qhsmtstQSto[10]; // event queue storage
    QActive_start(AO_QHsmTst, // AO pointer to start
        2U,             // QF-priority
        l_qhsmtstQSto,   // storage for the AO's queue
        Q_DIM(l_qhsmtstQSto), // length of the queue [entries]
        (void *)0,      // stack storage (not used)
        0U,             // stack size [bytes] (not used)
        (void *)0);     // initial param (or 0)

    return QF_run(); // run the QF application
}
