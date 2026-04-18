//============================================================================
// Example QUTEST fixture for the basic example from Unity
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
#include "ProductionCode.h" // CUT interface

Q_DEFINE_THIS_FILE

//----------------------------------------------------------------------------
// sometimes you may want to get at local data in a module.
// for example: If you plan to pass by reference, this could be useful
// however, it should often be avoided
extern int Counter;

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

    // dictionaries...
    QS_FUN_DICTIONARY(&FindFunction_WhichIsBroken);
    QS_FUN_DICTIONARY(&FunctionWhichReturnsLocalVariable);
    QS_OBJ_DICTIONARY(&Counter);

    // filter setup...
    QS_GLB_FILTER(QS_GRP_ALL);

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
        case 0: { // call the CUT: FindFunction_WhichIsBroken
            int ret = FindFunction_WhichIsBroken((int)param1);
            QS_BEGIN_ID(QS_USER + cmdId, 0U) // app-specific record
                QS_FUN(&FindFunction_WhichIsBroken); // function called
                QS_I32(0, (int32_t)ret); // returned value
                QS_I16(0, (int16_t)param1); // parameter
            QS_END()
            break;
        }
        case 1: { // call the CUT: FunctionWhichReturnsLocalVariable
            int ret = FunctionWhichReturnsLocalVariable();
            QS_BEGIN_ID(QS_USER + cmdId, 0U) // app-specific record
                QS_FUN(&FunctionWhichReturnsLocalVariable); // function called
                QS_U32(QS_HEX_FMT, (uint32_t)ret); // returned value
            QS_END()
             break;
        }
        default:
            break;
    }

    // unused parameters...
    //Q_UNUSED_PAR(param1);
    Q_UNUSED_PAR(param2);
    Q_UNUSED_PAR(param3);
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
    Q_UNUSED_PAR(sender);
    Q_UNUSED_PAR(recipient);
    Q_UNUSED_PAR(e);
    Q_UNUSED_PAR(status);
}
