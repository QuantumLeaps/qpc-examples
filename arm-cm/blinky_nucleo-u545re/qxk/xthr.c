//============================================================================
// "Blinky" example
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
#include "qpc.h"               // QP/C real-time embedded framework
#include "blinky.h"            // Blinky Application interface
#include "bsp.h"               // Board Support Package

//Q_DEFINE_THIS_FILE

// NOTES:
// 1. This is an example of a an eXtened thread (with blocking) for the
//    QXK kernel.
// 2. The QXK dual-mode kernel component is NOT provided in the open-source
//    QP/C distribution. Please contact Quantum Leaps for free evaluation
//    of the QXK kernel.
// 3. The QS software tracing instrumentation is active only in the
//    "spy" build configuration (Q_SPY macro defined). This build
//    configuration requires QS component, which is NOT provided in
//    the open source QP/C distribution. Please contact Quantum Leaps
//    for free evaluation of the softare tracing component.

//----------------------------------------------------------------------------
// eXtended Thread class declaration...
typedef struct {
    QXThread super; // QXThread superclass (inheritance)

// data members added to the QActive superclass...
    //...
} XThr;
extern XThr XThr_inst; // XThr instance declaration

// XThr thread function declaration...
static void XThr_run(QXThread * const thr);

//----------------------------------------------------------------------------
// XThr eXtended thread class definition...

XThr XThr_inst; // the Blinky AO instance definition
QXThread * const XT_thr = &XThr_inst.super; // global opaque pointer

//............................................................................
// XThr "constructor"
void XThr_ctor(void) {
    XThr * const me = &XThr_inst; // 'me' points to the XThr instance

    // call the superclass' constructor
    QXThread_ctor(&me->super,
        &XThr_run, // pointer to the thread function
        0U);       // tick rate for the timeouts

    // call the members' constructors
    //...
}

//............................................................................
static void XThr_run(QXThread * const thr) {
    // QS software tracing instrumentation (active only when Q_SPY is defined)
    QS_OBJ_DICTIONARY(XT_thr);

    for (;;) { // for-ever loop (must have at least one BLOCKING call)

        QXThread_delay(BSP_TICKS_PER_SEC);  // delay (BLOCK) for 1 second

        // QS software tracing output (active only when Q_SPY is defined)
        QS_BEGIN_ID(QS_USER, 0U)
            QS_OBJ(XT_thr);
            QS_STR("woken");
        QS_END()
    }
}

