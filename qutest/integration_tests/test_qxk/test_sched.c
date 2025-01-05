//============================================================================
// Product: System test fixture for QXK kernel
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

//Q_DEFINE_THIS_FILE

#ifdef Q_HOST
    #error This integration test is not designed to run on the HOST
#endif

//==========================================================================
// AO ObjB
enum { NUM_B = 3 };

typedef struct {
    QActive super;
} ObjB;

QState ObjB_initial(ObjB * const me, void const * const par);
QState ObjB_active(ObjB * const me, QEvt const * const e);

void ObjB_ctor(ObjB * const me) {
    QActive_ctor(&me->super, Q_STATE_CAST(&ObjB_initial));
}

//..........................................................................
QState ObjB_initial(ObjB * const me, void const * const par) {
    Q_UNUSED_PAR(par);
    QActive_subscribe(&me->super, TEST1_SIG);
    QActive_subscribe(&me->super, TEST2_SIG);
    QS_FUN_DICTIONARY(&ObjB_active);
    return Q_TRAN(&ObjB_active);
}
//..........................................................................
QState ObjB_active(ObjB * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case TEST0_SIG: {
            BSP_trace(&me->super, "TEST0 1of2");
            BSP_trigISR();
            BSP_trace(&me->super, "TEST0 2of2");
            status_ = Q_HANDLED();
            break;
        }
        case TEST1_SIG: {
            BSP_trace(&me->super, "TEST1 1of1");
            status_ = Q_HANDLED();
            break;
        }
        case TEST2_SIG: {
            BSP_trace(&me->super, "TEST2 1of1");
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
//..........................................................................
ObjB aoB[NUM_B];

//==========================================================================
enum { NUM_X = 3 };

enum UserCommands {
    MEM_READ, MEM_WRITE,
    ROM_READ, ROM_WRITE,
    RAM_READ, RAM_WRITE,
};

//..........................................................................
void ThrX_run(QXThread * const me) {
    QActive_subscribe(&me->super, TEST1_SIG);
    QActive_subscribe(&me->super, TEST2_SIG);

    for (;;) {
        QEvt const *e = QXThread_queueGet(QXTHREAD_NO_TIMEOUT);
        switch (e->sig) {
            case TEST0_SIG: {
                BSP_trace(&me->super, "TEST0 1of2");
                BSP_trigISR();
                BSP_trace(&me->super, "TEST0 2of2");
                break;
            }
            case TEST1_SIG: {
                static QEvt const t2 = QEVT_INITIALIZER(TEST2_SIG);
                BSP_trace(&me->super, "TEST1 1of2");
                QACTIVE_PUBLISH(&t2, &me->super);
                BSP_trace(&me->super, "TEST1 2of2");
                break;
            }
            case TEST2_SIG: {
                BSP_trace(&me->super, "TEST2 1of1");
                break;
            }
            default: {
                break;
            }
        }
    }
}

//..........................................................................
QXThread thrX[NUM_X];

//==========================================================================
int main() {
    QF_init();  // initialize the framework and the underlying QXK kernel
    BSP_init(); // initialize the Board Support Package

    // initialize publish-subscribe...
    static QSubscrList subscrSto[MAX_PUB_SIG];
    QActive_psInit(subscrSto, Q_DIM(subscrSto));

    // initialize event pools...
    static QF_MPOOL_EL(QEvt) smlPoolSto[10]; // small pool
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // dictionaries
    QS_SIG_DICTIONARY(TEST0_SIG,  (void *)0);
    QS_SIG_DICTIONARY(TEST1_SIG,  (void *)0);
    QS_SIG_DICTIONARY(TEST2_SIG,  (void *)0);
    QS_SIG_DICTIONARY(TEST3_SIG,  (void *)0);

    QS_ENUM_DICTIONARY(MEM_READ,  QS_CMD);
    QS_ENUM_DICTIONARY(MEM_WRITE, QS_CMD);
    QS_ENUM_DICTIONARY(ROM_READ,  QS_CMD);
    QS_ENUM_DICTIONARY(ROM_WRITE, QS_CMD);
    QS_ENUM_DICTIONARY(RAM_READ,  QS_CMD);
    QS_ENUM_DICTIONARY(RAM_WRITE, QS_CMD);

    // priority specifications for ObjBs...
    static QPrioSpec pspecB[NUM_B];
    QS_OBJ_DICTIONARY(pspecB);

    // priority specifications for ThrXs...
    static QPrioSpec pspecX[NUM_X];
    QS_OBJ_DICTIONARY(pspecX);

    uint8_t n;
    for (n = 0U; n < NUM_B; ++n) {
        QS_OBJ_ARR_DICTIONARY(&aoB[n], n);
        pspecB[n] = n + 1U;
    }
    for (n = 0U; n < NUM_X; ++n) {
        QS_OBJ_ARR_DICTIONARY(&thrX[n], n);
        pspecX[n] = NUM_B + 1U + n;
    }

    // pause execution of the test and wait for the test script to continue
    // NOTE:
    // this pause gives the test-script a chance to poke pspecB and pspecX
    // variables to start the threads with the desired prio-specifications.
    QS_TEST_PAUSE();

    static QEvtPtr aoB_queueSto[NUM_B][5];
    for (n = 0U; n < NUM_B; ++n) {
        if (pspecB[n] != 0U) {
            ObjB_ctor(&aoB[n]);          // instantiate the AO
            QActive_start(&aoB[n].super,// AO to start
                pspecB[n],               // QF-prio/p-thre.
                aoB_queueSto[n],         // event queue storage
                Q_DIM(aoB_queueSto[n]),  // event length [events]
                (void *)0,               // no stack storage
                0U,                      // zero stack size [bytes]
                (void *)0);              // initialization param
        }
    }

    static QEvtPtr thrX_queueSto[NUM_X][5];
    static uint64_t thrXStackSto[NUM_X][32];
    for (n = 0U; n < NUM_X; ++n) {
    QXThread_ctor(&thrX[n], &ThrX_run, 0U); // instantiate the thread
        if (pspecX[n] != 0U) {
            QXThread_start(&thrX[n],    // thread to start
                pspecX[n],               // QF-prio/p-thre.
                thrX_queueSto[n],        // event queue storage
                Q_DIM(thrX_queueSto[n]), // event length [events]
                thrXStackSto[n],         // stack storage
                sizeof(thrXStackSto[n]), // stack size [bytes]
                (void *)0);              // initialization param
        }
    }
    return QF_run(); // run the QF application
}

//==========================================================================
void QS_onTestSetup(void) {
}
//..........................................................................
void QS_onTestTeardown(void) {
}
//..........................................................................
//! callback function to execute user commands
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    uint32_t volatile value;

    switch (cmdId) {
        case MEM_READ: { // read MEM (can trip the MPU)
            value = *(uint32_t volatile *)(param1 + param2);
            QS_BEGIN_ID(QS_USER, 0U)
                QS_ENUM(QS_CMD, cmdId);
                QS_U32(0, value);
            QS_END()
            break;
        }
        case MEM_WRITE: { // write MEM (can trip the MPU)
            *(uint32_t volatile *)(param1 + param2) = param3;
            QS_BEGIN_ID(QS_USER, 0U)
                QS_ENUM(QS_CMD, cmdId);
                QS_U32(QS_HEX_FMT , param1);
                QS_U32(QS_HEX_FMT , param2);
                QS_U32(0 , param3);
            QS_END()
            break;
        }
        case ROM_READ: { // read ROM (can trip the MPU)
            value = BSP_romRead((int32_t)param1, param2);
            QS_BEGIN_ID(QS_USER, 0U)
                QS_ENUM(QS_CMD, cmdId);
                QS_U32(0, value);
            QS_END()
            break;
        }
        case ROM_WRITE: { // write ROM (can trip the MPU)
            BSP_romWrite(param1, param2, param3);
            QS_BEGIN_ID(QS_USER, 0U)
                QS_ENUM(QS_CMD, cmdId);
                QS_U32(QS_HEX_FMT , param1);
                QS_U32(QS_HEX_FMT , param2);
                QS_U32(0 , param3);
            QS_END()
            break;
        }
        case RAM_READ: { // read RAM (can trip the MPU)
            value = BSP_ramRead(param1, param2);
            QS_BEGIN_ID(QS_USER, 0U)
                QS_ENUM(QS_CMD, cmdId);
                QS_U32(0, value);
            QS_END()
            break;
        }
        case RAM_WRITE: { // write RAM (can trip the MPU)
            BSP_ramWrite(param1, param2, param3);
            QS_BEGIN_ID(QS_USER, 0U)
                QS_ENUM(QS_CMD, cmdId);
                QS_U32(QS_HEX_FMT , param1);
                QS_U32(QS_HEX_FMT , param2);
                QS_U32(0, param3);
            QS_END()
            break;
        }
        default: {
            break;
        }
    }
}

//****************************************************************************
//! Host callback function to "massage" the event, if necessary
void QS_onTestEvt(QEvt *e) {
    Q_UNUSED_PAR(e);
}
//..........................................................................
//! callback function to output the posted QP events (not used here)
void QS_onTestPost(void const *sender, QActive *recipient,
                   QEvt const *e, bool status)
{
    Q_UNUSED_PAR(sender);
    Q_UNUSED_PAR(status);
}

