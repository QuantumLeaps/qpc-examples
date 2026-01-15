//============================================================================
// Product: Console-based BSP
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

#include "safe_std.h" // portable "safe" <stdio.h>/<string.h> facilities
#include <stdlib.h>   // for exit()

//Q_DEFINE_THIS_FILE

#ifdef Q_SPY
    #error This application does not provide Spy build configuration
#endif

//============================================================================
Q_NORETURN Q_onError(char const * const module, int_t const id) {
    FPRINTF_S(stderr, ">>>> ERROR in %s:%d <<<<", module, id);
    QF_onCleanup();
    exit(-1);
}

//============================================================================
// BSP functions...

void BSP_init(void const * const arg) {
    Q_UNUSED_PAR(arg);

    QF_consoleSetup();
    PRINTF_S("Deferred Event state pattern\nQP version: %s\n"
           "Press ESC to quit...\n",
           QP_VERSION_STR);

    // dynamic events not used, no need to call QF_poolInit()
    // publish-subscribe not used, no call to QActive_psInit()
}
//............................................................................
void BSP_showMsg(char const * const msg, uint8_t const num) {
    PRINTF_S(msg, num);
}

//============================================================================
void QF_onStartup(void) {
    // instantiate and start the active objects...
    Sensor_ctor();
    static QEvtPtr sensorQSto[10]; // event queue storage for Sensor AO
    QActive_start(AO_Sensor,
        1U,
        sensorQSto, Q_DIM(sensorQSto),
        (void *)0, 0U, (void *)0);

    QF_setTickRate(BSP_TICKS_PER_SEC, 50U); // desired tick rate/ticker-prio
}
//............................................................................
void QF_onCleanup(void) {
    PRINTF_S("\n%s\n", "Bye! Bye!");
    QF_consoleCleanup();
}
//............................................................................
void QF_onClockTick(void) {
    QTIMEEVT_TICK_X(0U, (void *)0); // clock tick processing for rate 0

    int key = QF_consoleGetKey();
    if (key != 0) { // any key pressed?
        switch (key) {
            case '\033': { // ESC pressed?
                static QEvt const terminateEvt = QEVT_INITIALIZER(TERMINATE_SIG);
                QACTIVE_POST(AO_Sensor, &terminateEvt, (void *)0);
                break;
            }
        }
    }
}
