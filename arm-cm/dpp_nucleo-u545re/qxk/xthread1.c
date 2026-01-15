//============================================================================
// QXThread example
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
#include "qpc.h"                 // QP/C real-time event framework
#include "bsp.h"                 // Board Support Package
#include "app.h"                 // Application

//----------------------------------------------------------------------------
//Q_DEFINE_THIS_FILE

//............................................................................
typedef struct XThread1 {
// protected:
    QXThread super;

// private:
    // NOTE: data needed by this thread should be members of
    // the thread class. That way they are in the memory region
    // accessible from this thread.
    uint8_t foo;

// public:
} XThread1;

// private:
static void XThread1_run(QXThread * const thr);
extern XThread1 XThread1_inst;

//............................................................................
QXThread * const TH_XThread1 = &XThread1_inst.super;

//............................................................................
void XThread1_ctor(void) {
    XThread1 *me = &XThread1_inst;
    QXThread_ctor(&me->super, &XThread1_run, 0U);
}

//............................................................................
XThread1 XThread1_inst;

//............................................................................
static void XThread1_run(QXThread * const thr) {
    QS_OBJ_DICTIONARY(TH_XThread1);
    QS_OBJ_DICTIONARY(&TH_XThread1->timeEvt);

    // downcast the generic thr pointer to the specific thread
    XThread1 *me = (XThread1 *)thr;

    // subscribe to the EAT signal (from the application)
    QActive_subscribe(&me->super.super, EAT_SIG);

    for (;;) {
        QEvt const *e = QXThread_queueGet(BSP_TICKS_PER_SEC/4U);
        if (e) {
            QXSemaphore_signal(&TH_sema); // signal Thread2
            QF_gc(e); // must explicitly recycle the received event!
        }

        QXMutex_lock(&TH_mutex, QXTHREAD_NO_TIMEOUT); // lock the mutex
        BSP_ledOn();
        if (QXMutex_tryLock(&TH_mutex)) { // exercise the mutex
            // some floating point code to exercise the VFP...
            float volatile x = 1.4142135F;
            x = x * 1.4142135F;
            QXThread_delay(10U);  // BLOCK while holding a mutex
            QXMutex_unlock(&TH_mutex);
        }
        QXMutex_unlock(&TH_mutex);
        BSP_ledOff();
    }
}
