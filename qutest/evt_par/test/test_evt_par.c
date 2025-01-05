//============================================================================
// Purpose: example event QUTEST fixture
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
#include "qpc.h"    // QUTest interface
#include "my_app.h" // My Application

Q_DEFINE_THIS_FILE

//----------------------------------------------------------------------------
#ifdef Q_HOST
int main(int argc, char *argv[])
#else
int main(void)
#endif
{
    QF_init();  // initialize the framework

    // initialize the QS software tracing
#ifdef Q_HOST
    if (QS_INIT((argc > 1) ? argv[1] : (void*)0) == 0U) {
        Q_ERROR();
    }
#else
    if (QS_INIT((void*)0) == 0U) {
        Q_ERROR();
    }
#endif

    // object dictionaries...
    QS_OBJ_DICTIONARY(AO_MyAO);

    QS_TEST_PAUSE();

    // initialize publish-subscribe...
    static QSubscrList subscrSto[MAX_PUB_SIG];
    QActive_psInit(subscrSto, Q_DIM(subscrSto));

    // initialize event pools...
    static QF_MPOOL_EL(MyEvt3) smlPoolSto[10];
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // start active objects...
    MyAO_ctor(); // instantiate the MyAO active object
    static QEvtPtr myAoQueueSto[10];
    QActive_start(AO_MyAO,             // AO to start
                  (uint_fast8_t)1,      // QP priority of the AO
                  myAoQueueSto,         // event queue storage
                  Q_DIM(myAoQueueSto),  // queue length [events]
                  (void *)0,            // stack storage (not used)
                  0U,                   // size of the stack [bytes]
                  (QEvt *)0);           // initialization event

    return QF_run(); // run the QF application
}

//----------------------------------------------------------------------------
enum AppSpecRecords {
    MY_EVT0 = QS_USER,
    MY_EVT1,
    MY_EVT2,
    MY_EVT3
};

//----------------------------------------------------------------------------
void QS_onTestSetup(void) {
    QS_USR_DICTIONARY(MY_EVT0);
    QS_USR_DICTIONARY(MY_EVT1);
    QS_USR_DICTIONARY(MY_EVT2);
    QS_USR_DICTIONARY(MY_EVT3);
}
//............................................................................
void QS_onTestTeardown(void) {
}

//............................................................................
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    (void)param1; // unused parameter
    (void)param2; // unused parameter
    (void)param3; // unused parameter

    switch (cmdId) {
        case 0: {
            break;
        }
        default:
            break;
    }
}
//............................................................................
//! callback function to "massage" the injected QP events (not used here)
void QS_onTestEvt(QEvt *e) {

    switch (e->sig) {
        case MY_EVT0_SIG:
            QS_BEGIN_ID(MY_EVT0, 0U) // app-specific record
            QS_END()
            break;
        case MY_EVT1_SIG:
            QS_BEGIN_ID(MY_EVT1, 0U) // app-specific record
                QS_U32(0, Q_EVT_CAST(MyEvt1)->u32);
            QS_END()
            break;
        case MY_EVT2_SIG:
            QS_BEGIN_ID(MY_EVT2, 0U) // app-specific record
                QS_U32(0, Q_EVT_CAST(MyEvt2)->u32);
                QS_U32(0, Q_EVT_CAST(MyEvt2)->u16);
            QS_END()
            break;
        case MY_EVT3_SIG:
            QS_BEGIN_ID(MY_EVT3, 0U) // app-specific record
                QS_U32(0, Q_EVT_CAST(MyEvt3)->u32);
                QS_U32(0, Q_EVT_CAST(MyEvt3)->u16);
                QS_U32(0, Q_EVT_CAST(MyEvt3)->u8);
            QS_END()
            break;
    }

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
    (void)recipient;
    (void)e;
    (void)status;
}
