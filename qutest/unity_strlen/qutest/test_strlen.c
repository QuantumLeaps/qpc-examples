//============================================================================
// Purpose: example QUTEST fixture for the mock example from Unity
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
#include "qpc.h"  // QUTest interface
#include "my_strlen.h" // CUT interface

Q_DEFINE_THIS_FILE

//----------------------------------------------------------------------------
static char string[128];

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

    // dictionaries...
    QS_FUN_DICTIONARY(&my_strlen);
    QS_OBJ_DICTIONARY(string);

    // filter setup...
    QS_GLB_FILTER(QS_ALL_RECORDS);
    QS_setCurrObj(AP_OBJ, string);

    return QF_run(); // run the tests
}

//----------------------------------------------------------------------------
void QS_onTestSetup(void) {
}
//............................................................................
void QS_onTestTeardown(void) {
}

//............................................................................
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    switch (cmdId) {
        case 0: { // call the CUT: my_strlen(string)
            uint16_t ret = my_strlen(string);
            QS_BEGIN_ID(QS_USER + cmdId, 0U) // app-specific record
                QS_FUN(&my_strlen); // function called
                QS_U16(0, ret); // value returned
                QS_STR(string); // the current string
            QS_END()
            break;
        }
        default:
            break;
    }

    // unused parameters...
    (void)param1;
    (void)param2;
    (void)param3;
}
//............................................................................
// host callback function to "massage" the event, if necessary
void QS_onTestEvt(QEvt *e) {
    (void)e;
#ifdef Q_HOST  // is this test compiled for a desktop Host computer?
#else // this test is compiled for an embedded Target system
#endif
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
