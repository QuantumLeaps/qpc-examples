//============================================================================
// Test fixture for QUTEST
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// SPDX-License-Identifier: LicenseRef-QL-commercial
//
// This software is licensed under the terms of the Quantum Leaps commercial
// licenses. Please contact Quantum Leaps for more information about the
// available licensing options.
//
// RESTRICTIONS
// You may NOT:
// (a) redistribute, encumber, sell, rent, lease, sublicense, or otherwise
//     transfer rights in this software,
// (b) remove or alter any trademark, logo, copyright or other proprietary
//     notices, legends, symbols or labels present in this software,
// (c) plagiarize this software to sidestep the licensing obligations.
//
// Quantum Leaps contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"          // QP/C framework
#include "tstsm.h"        // TstSM well-formed state machine
#include "tstsm_sf.h"     // Test state machines for Safety Functions

Q_DEFINE_THIS_MODULE("test_fixture")

enum {
    COMMAND     = QS_USER0,
    BSP_DISPLAY = QS_USER1,
};
enum CmdEnum {
    QP_VERSION_CMD,
    QHSM_GET_STATE_HANDLER_CMD,
    TSTSM_IS_IN_CMD,
    TSTSM6_IS_IN_CMD,
    TSTSM6_CHILD_CMD,
    DISP_NULL_EVT_CMD,
    DISP_CORR_EVT_CMD,
};

static QSpyId const test_fixture = { 0U };

//----------------------------------------------------------------------------
int main() {
    QF_init(); // initialize the framework

    TstSM_ctor(); // instantiate the TstSM passive object

    // test state machines for Safety Functions
    TstSM0_ctor();
    TstSM1_ctor();
    TstSM2_ctor();
    TstSM3_ctor();
    TstSM4_ctor();
    TstSM5_ctor();
    TstSM6_ctor();

    // initialize the QS software tracing
    if (QS_INIT((void *)0) == 0U) {
        Q_ERROR();
    }

    // initialize event pools...
    static QF_MPOOL_EL(QEvt) smlPoolSto[10]; // small pool
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // dictionaries...
    QS_OBJ_DICTIONARY(&test_fixture);
    QS_OBJ_DICTIONARY(the_TstSM);
    QS_OBJ_DICTIONARY(the_TstSM0);
    QS_OBJ_DICTIONARY(the_TstSM1);
    QS_OBJ_DICTIONARY(the_TstSM2);
    QS_OBJ_DICTIONARY(the_TstSM3);
    QS_OBJ_DICTIONARY(the_TstSM4);
    QS_OBJ_DICTIONARY(the_TstSM5);
    QS_OBJ_DICTIONARY(the_TstSM6);

    QS_SIG_DICTIONARY(A_SIG, (void *)0);
    QS_SIG_DICTIONARY(B_SIG, (void *)0);
    QS_SIG_DICTIONARY(C_SIG, (void *)0);
    QS_SIG_DICTIONARY(D_SIG, (void *)0);
    QS_SIG_DICTIONARY(E_SIG, (void *)0);
    QS_SIG_DICTIONARY(F_SIG, (void *)0);
    QS_SIG_DICTIONARY(G_SIG, (void *)0);
    QS_SIG_DICTIONARY(H_SIG, (void *)0);
    QS_SIG_DICTIONARY(I_SIG, (void *)0);

    QS_USR_DICTIONARY(BSP_DISPLAY);
    QS_USR_DICTIONARY(COMMAND);
    QS_ENUM_DICTIONARY(QP_VERSION_CMD,             QS_CMD);
    QS_ENUM_DICTIONARY(QHSM_GET_STATE_HANDLER_CMD, QS_CMD);
    QS_ENUM_DICTIONARY(TSTSM_IS_IN_CMD,            QS_CMD);
    QS_ENUM_DICTIONARY(TSTSM6_IS_IN_CMD,           QS_CMD);
    QS_ENUM_DICTIONARY(TSTSM6_CHILD_CMD,           QS_CMD);
    QS_ENUM_DICTIONARY(DISP_NULL_EVT_CMD,          QS_CMD);
    QS_ENUM_DICTIONARY(DISP_CORR_EVT_CMD,          QS_CMD);

    // QS settings...
    QS_setCurrObj(SM_OBJ, the_TstSM);

    return QF_run(); // run the tests
}

//----------------------------------------------------------------------------
void BSP_display(char const *msg) {
    QS_BEGIN_ID(BSP_DISPLAY, 0U) // app-specific record
        QS_STR(msg);
    QS_END()
}
//............................................................................
void BSP_terminate(int16_t const result) {
    Q_UNUSED_PAR(result);
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
    Q_UNUSED_PAR(param1);
    Q_UNUSED_PAR(param2);
    Q_UNUSED_PAR(param3);

    //PRINTF_S("<TARGET> Command id=%d param=%d\n", (int)cmdId, (int)param);
    switch (cmdId) {
        case QP_VERSION_CMD: {
            QS_BEGIN_ID(COMMAND, 0U) // app-specific record
                QS_ENUM(QS_CMD, cmdId);
                QS_STR(QP_versionStr);
            QS_END()
            break;
        }
        case QHSM_GET_STATE_HANDLER_CMD: {
            QStateHandler const state = QHsm_getStateHandler_(the_TstSM);
            QS_BEGIN_ID(COMMAND, 0U) // app-specific record
                QS_ENUM(QS_CMD, cmdId);
                QS_FUN(state);
            QS_END()
            break;
        }
        case TSTSM_IS_IN_CMD: {
            bool const ret = TstSM_isIn(param1);
            QS_BEGIN_ID(COMMAND, 0U) // app-specific record
                QS_ENUM(QS_CMD, cmdId);
                QS_U8(0U, ret ? 1 : 0);
                QS_U8(0U, (uint8_t)param1);
            QS_END()
            break;
        }
        case TSTSM6_IS_IN_CMD: {
            bool const ret = TstSM6_isIn(param1);
            QS_BEGIN_ID(COMMAND, 0U) // app-specific record
                QS_ENUM(QS_CMD, cmdId);
                QS_U8(0U, ret ? 1 : 0);
                QS_U8(0U, (uint8_t)param1);
            QS_END()
            break;
        }
        case TSTSM6_CHILD_CMD: {
            QStateHandler const child = TstSM6_child(param1);
            QS_BEGIN_ID(COMMAND, 0U) // app-specific record
                QS_ENUM(QS_CMD, cmdId);
                QS_FUN(child);
            QS_END()
            break;
        }
        case DISP_NULL_EVT_CMD: {
            QASM_DISPATCH(the_TstSM6, (QEvt *)0, 0U);
            break;
        }
        case DISP_CORR_EVT_CMD: {
            static QEvt const corr_evt = {A_SIG, 0U, 0U, 0U, 0U, 0U };
            QASM_DISPATCH(the_TstSM6, &corr_evt, 0U);
            break;
        }
        default:
            Q_ERROR();
            break;
    }
}

//............................................................................
// callback function to "massage" the event, if necessary
void QS_onTestEvt(QEvt *e) {
    Q_UNUSED_PAR(e);
#ifdef Q_HOST  // is this test compiled for a desktop Host computer?
#else // this test is compiled for an embedded Target system
#endif
}
//............................................................................
// callback function to output the posted QP events (not used here)
void QS_onTestPost(void const *sender, QActive *recipient,
                   QEvt const *e, bool status)
{
    Q_UNUSED_PAR(sender);
    Q_UNUSED_PAR(recipient);
    Q_UNUSED_PAR(e);
    Q_UNUSED_PAR(status);
}
