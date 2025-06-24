//============================================================================
// Purpose: Fixture for QUTEST
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open-source GNU
// General Public License (GPL) or under the terms of one of the closed-
// source Quantum Leaps commercial licenses.
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// NOTE:
// The GPL does NOT permit the incorporation of this code into proprietary
// programs. Please contact Quantum Leaps for commercial licensing options,
// which expressly supersede the GPL and are designed explicitly for
// closed-source distribution.
//
// Quantum Leaps contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"
#include "bsp.h"
#include "dpp.h"

//#include "safe_std.h" // portable "safe" <stdio.h>/<string.h> facilities

Q_DEFINE_THIS_FILE

//============================================================================
// instantiate Table AO as a dummy collaborator
static QActiveDummy Table_dummy;
QActive * const AO_Table = &Table_dummy.super;

//----------------------------------------------------------------------------
#ifdef Q_HOST
int main(int argc, char *argv[]) { // host takes command-line arguments
    void * const qs_arg = ((argc > 1) ? argv[1] : (void*)0);
#else // embedded target
int main(void) { // embedded target takes no command-line arguments
    void * const qs_arg = (void*)0;
#endif

    QF_init(); // initialize the framework

    if (!QS_INIT(qs_arg)) { // initialize the QS software tracing
        Q_ERROR(); // QS initialization must succeed
    }

    BSP_init(); // initialize the Board Support Package

    // object dictionaries...
    QS_OBJ_DICTIONARY(&Table_dummy);

    // pause execution of the test and wait for the test script to continue
    QS_TEST_PAUSE();

    // initialize event pools...
    static QF_MPOOL_EL(TableEvt) smlPoolSto[2*N_PHILO]; // small pool
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // initialize publish-subscribe...
    static QSubscrList subscrSto[MAX_PUB_SIG];
    QActive_psInit(subscrSto, Q_DIM(subscrSto));

    // start the active objects...
    static QEvtPtr philoQueueSto[N_PHILO][10];
    uint8_t n = 2U;
    Philo_ctor(n); // instantiate all Philosopher active objects
    QActive_start(AO_Philo[n], // AO to start
        n + 1U,                // QP priority of the AO
        philoQueueSto[n],      // event queue storage
        Q_DIM(philoQueueSto[n]), // queue length [events]
        (void *)0,             // stack storage (not used)
        0U,                    // size of the stack [bytes]
        (void *)0);            // initialization param

    // instantiate Table AO as a dummy collaborator
    QActiveDummy_ctor(&Table_dummy);
    QActive_start(&Table_dummy.super,
        N_PHILO + 1U, // QP priority of the dummy
        (QEvtPtr *)0, 0U, (void *)0, 0U, (void *)0);

    return QF_run(); // run the QF application
}

//============================================================================
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
}

//============================================================================
// Host callback function to "massage" the event, if necessary
void QS_onTestEvt(QEvt *e) {
    (void)e;
#ifdef Q_HOST  // is this test compiled for a desktop Host computer?
#else // embedded Target
#endif // embedded Target
}
//............................................................................
// callback function to output the posted QP events...
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
