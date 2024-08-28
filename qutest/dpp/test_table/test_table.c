//============================================================================
// Product: QUTEST fixture for the DPP components
// Last updated for version 7.4.0
// Last updated on  2024-07-31
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC <state-machine.com>.
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open source GNU
// General Public License version 3 (or any later version), or alternatively,
// under the terms of one of the closed source Quantum Leaps commercial
// licenses.
//
// The terms of the open source GNU General Public License version 3
// can be found at: <www.gnu.org/licenses/gpl-3.0>
//
// The terms of the closed source Quantum Leaps commercial licenses
// can be found at: <www.state-machine.com/licensing>
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// Contact information:
// <www.state-machine.com>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"
#include "bsp.h"
#include "dpp.h"

//#include "safe_std.h" // portable "safe" <stdio.h>/<string.h> facilities

//Q_DEFINE_THIS_FILE

// instantiate dummy collaborator AOs...
static QActiveDummy Philo_dummy[N_PHILO];
QActive * const AO_Philo[N_PHILO] = {
    &Philo_dummy[0].super,
    &Philo_dummy[1].super,
    &Philo_dummy[2].super,
    &Philo_dummy[3].super,
    &Philo_dummy[4].super
};

//============================================================================
int main(int argc, char* argv[]) {
    QF_init();       // initialize the framework and the underlying RT kernel
    BSP_init(argc, argv); // initialize the Board Support Package

    for (uint8_t n = 0U; n < N_PHILO; ++n) {
       QS_OBJ_ARR_DICTIONARY(&Philo_dummy[n], n);
    }

    // pause execution of the test and wait for the test script to continue
    QS_TEST_PAUSE();

    // initialize publish-subscribe...
    static QSubscrList subscrSto[MAX_PUB_SIG];
    QActive_psInit(subscrSto, Q_DIM(subscrSto));

    // initialize event pools...
    static QF_MPOOL_EL(TableEvt) smlPoolSto[2*N_PHILO]; // small pool
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // start and setup dummy AOs...
    // NOTE: You need to start dummy AOs, if you wish to subscribe
    //      them to events.
    //
    for (uint8_t n = 0U; n < N_PHILO; ++n) {
        QActiveDummy_ctor(&Philo_dummy[n]);
        QActive_start(&Philo_dummy[n],
                     (uint_fast8_t)(n + 1U), // priority
                     (QEvt const **)0, 0U, (void *)0, 0U,
                     (QEvt const *)0);
        QActive_subscribe(AO_Philo[n], EAT_SIG);
    }


    // start the active objects...
    Table_ctor(); // instantiate the Table active object
    static QEvt const *tableQueueSto[N_PHILO];
    QActive_start(AO_Table,           // AO to start
                  (uint_fast8_t)(N_PHILO + 1), // QP priority of the AO
                  tableQueueSto,        // event queue storage
                  Q_DIM(tableQueueSto), // queue length [events]
                  (void *)0,            // stack storage (not used)
                  0U,                   // size of the stack [bytes]
                  (void *)0);           // initialization param

    return QF_run(); // run the QF application
}

//............................................................................
void QS_onTestSetup(void) {
    //PRINTF_S("%s\n", "QS_onTestSetup");
}
void QS_onTestTeardown(void) {
    //PRINTF_S("%s\n", "QS_onTestTeardown");
}

//............................................................................
//! callback function to execute user commands
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    (void)cmdId;
    (void)param1;
    (void)param2;
    (void)param3;

    switch (cmdId) {
       case 0U: {
           QEvt const e = QEVT_INITIALIZER( PAUSE_SIG);
           QASM_DISPATCH(AO_Table, &e, AO_Table->prio);
           break;
       }
       case 1U: {
           QEvt const e = QEVT_INITIALIZER(SERVE_SIG);
           QASM_DISPATCH(AO_Table, &e, AO_Table->prio);
           break;
       }
       default:
           break;
    }
}

//============================================================================
//! Host callback function to "massage" the event, if necessary
void QS_onTestEvt(QEvt *e) {
    (void)e;
#ifdef Q_HOST  // is this test compiled for a desktop Host computer?
#else // embedded Target
#endif // embedded Target
}
//............................................................................
//! callback function to output the posted QP events (not used here)
void QS_onTestPost(void const *sender, QActive *recipient,
                   QEvt const *e, bool status)
{
    (void)sender;
    (void)status;
    switch (e->sig) {
        case EAT_SIG:
        case DONE_SIG:
        case HUNGRY_SIG:
            QS_BEGIN_ID(QUTEST_ON_POST, 0U) // app-specific record
                QS_SIG(e->sig, recipient);
                QS_U8(0, Q_EVT_CAST(TableEvt)->philoId);
            QS_END()
            break;
        default:
            break;
    }
}
