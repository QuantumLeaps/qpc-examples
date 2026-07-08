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

//----------------------------------------------------------------------------
QXSemaphore TH_sema;
QXMutex TH_mutex;

//----------------------------------------------------------------------------
typedef struct XThread2 {
// protected:
    QXThread super;

// private:
    // NOTE: data needed by this thread should be members of
    // the thread class. That way they are in the memory region
    // accessible from this thread.
    uint8_t foo;

// public:
} XThread2;

// private:
static void XThread2_run(QXThread * const thr);
extern XThread2 XThread2_inst;

//............................................................................
QXThread * const TH_XThread2 = &XThread2_inst.super;

//............................................................................
void XThread2_ctor(void) {
    XThread2 *me = &XThread2_inst;
    QXThread_ctor(&me->super, &XThread2_run, 0U);
}

//............................................................................
XThread2 XThread2_inst;

//............................................................................
static void XThread2_run(QXThread * const thr) {
    Q_UNUSED_PAR(thr);

    QS_OBJ_DICTIONARY(TH_XThread2);
    QS_OBJ_DICTIONARY(&TH_XThread2->timeEvt);
    QS_OBJ_DICTIONARY(&TH_sema);
    QS_OBJ_DICTIONARY(&TH_mutex);

    // downcast the generic thr pointer to the specific thread
    //XThread2 *me = (XThread2 *)thr;

    // initialize the semaphore before using it
    // NOTE: Here the semaphore is initialized in the highest-priority thread
    // that uses it. Alternatively, the semaphore can be initialized
    // before any thread runs.
    QXSemaphore_init(&TH_sema,
                0U,  // count==0 (signaling semaphore)
                1U); // max_count==1 (binary semaphore)

    // initialize the mutex before using it
    // NOTE: Here the mutex is initialized in the highest-priority thread
    // that uses it. Alternatively, the mutex can be initialized
    // before any thread runs.
    QXMutex_init(&TH_mutex, N_PHILO + 6U); // priority-ceiling mutex
    //QXMutex_init(&TH_mutex, 0U); // alternatively: priority-ceiling NOT used

    for (;;) {
        // wait on a semaphore (BLOCK indefinitely)
        QXSemaphore_wait(&TH_sema, QXTHREAD_NO_TIMEOUT);

        QXMutex_lock(&TH_mutex, QXTHREAD_NO_TIMEOUT); // lock the mutex
        QXThread_delay(5U);  // wait more (BLOCK)
        QXMutex_unlock(&TH_mutex);
    }
}
