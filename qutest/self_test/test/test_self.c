//============================================================================
// Purpose: Fixture for QUTEST self-test
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
#include "qpc.h" // for QUTEST

Q_DEFINE_THIS_MODULE("test_qutest")

//----------------------------------------------------------------------------
static uint8_t buffer[100];
static uint32_t myFun(void);

enum UsrEnum {
    FIXTURE_SETUP = QS_USER,
    FIXTURE_TEARDOWN,
    COMMAND,
};

enum CmdEnum {
    CMD_A,
    CMD_B,
    CMD_C,
    CMD_X,
    CMD_Y,
    CMD_Z
};

enum UsrSig {
    MYSIG_SIG = Q_USER_SIG,
};

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

    // global filter
    QS_GLB_FILTER(QS_ALL_RECORDS); // enable all QS records

    // dictionaries...
    QS_OBJ_DICTIONARY(buffer);
    QS_FUN_DICTIONARY(&myFun);
    QS_SIG_DICTIONARY(MYSIG_SIG, (void *)0);

    QS_USR_DICTIONARY(FIXTURE_SETUP);
    QS_USR_DICTIONARY(FIXTURE_TEARDOWN);
    QS_USR_DICTIONARY(COMMAND);

    QS_ENUM_DICTIONARY(CMD_A, QS_CMD);
    QS_ENUM_DICTIONARY(CMD_B, QS_CMD);
    QS_ENUM_DICTIONARY(CMD_C, QS_CMD);
    QS_ENUM_DICTIONARY(CMD_X, QS_CMD);
    QS_ENUM_DICTIONARY(CMD_Y, QS_CMD);
    QS_ENUM_DICTIONARY(CMD_Z, QS_CMD);

    return QF_run(); // run the tests
}

//----------------------------------------------------------------------------
void QS_onTestSetup(void) {
    QS_BEGIN_ID(FIXTURE_SETUP, 0U)
    QS_END()
}
//............................................................................
void QS_onTestTeardown(void) {
    QS_BEGIN_ID(FIXTURE_TEARDOWN, 0U)
    QS_END()
}

//............................................................................
//! callback function to execute user commands
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    switch (cmdId) {
        case CMD_A: {
            Q_ASSERT_ID(100, param1 != 0U);
            QS_BEGIN_ID(COMMAND, 0U) // app-specific record
                QS_ENUM(QS_CMD, cmdId);
                QS_U32(0, param1);
            QS_END()
            break;
        }
        case CMD_B: {
            QS_BEGIN_ID(COMMAND, 0U) // app-specific record
                QS_ENUM(QS_CMD, cmdId);
                QS_U8(0, param1);
                QS_STR("BAR");
                QS_U16(0, param2);
                QS_STR("FOO");
                QS_U32(0, param3);
                QS_F64(param1, -6.02214076E23);
            QS_END()
            break;
        }
        case CMD_C: {
            // all supported data elements
            QS_BEGIN_ID(COMMAND, 0U) // app-specific record
                QS_ENUM(QS_CMD, cmdId);
                QS_I8(10,  param1);
                QS_U8(10,  param1);
                QS_I16(10, param2);
                QS_U16(10, param2);
                QS_I32(10, param3);
                QS_U32(10, param3);
            QS_END()
            QS_BEGIN_ID(COMMAND, 0U) // app-specific record
                QS_I64(10, param3);
                QS_U64(10, param3);
                QS_F32(param1, -6.02214076E23);
                QS_F64(param1, -6.02214076E23);
            QS_END()
            QS_BEGIN_ID(COMMAND, 0U) // app-specific record
                QS_OBJ(buffer);
                QS_FUN(&myFun);
                QS_SIG(MYSIG_SIG, (void *)0);
                QS_MEM(&buffer[6], 16);
            QS_END()
            break;
        }
        case CMD_X: {
            uint32_t x = myFun();
            QS_BEGIN_ID(COMMAND, 0U) // app-specific record
                QS_ENUM(QS_CMD, cmdId);
                QS_U32(0, x);
                // ...
            QS_END()
            break;
        }
        case CMD_Y: {
            QS_BEGIN_ID(COMMAND, 0U) // application-specific record
                QS_ENUM(QS_CMD, cmdId);
                QS_FUN(&myFun);
                QS_MEM(buffer, param1);
                QS_STR((char const *)&buffer[33]);
            QS_END()
            break;
        }
        case CMD_Z: {
            float32_t f32 = (float32_t)((int32_t)param2/(float32_t)param3);
            float64_t f64 = -6.02214076E23;
            QS_BEGIN_ID(COMMAND, 0U) // app-specific record
                QS_ENUM(QS_CMD, cmdId);
                QS_F32(param1, f32);
                QS_F64(param1, f64);
            QS_END()
            break;
        }
        default:
            break;
    }
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

//----------------------------------------------------------------------------
static uint32_t myFun(void) {
    QS_TEST_PROBE_DEF(&myFun)
    QS_TEST_PROBE(
        return qs_tp_;
    )
    return 0;
}

