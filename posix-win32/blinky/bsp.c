//============================================================================
// "Blinky" example: BSP for console target (Windows, Linux, macOS)
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
#include "qpc.h"  // QP/C real-time event framework
#include "bsp.h"  // Board Support Package

#include "safe_std.h" // portable "safe" <stdio.h>/<string.h> facilities
#include <stdlib.h> // for exit()

//Q_DEFINE_THIS_FILE

#ifdef Q_SPY
    #error Simple Blinky does not support Spy build configuration
#endif

//--------------------------------------------------------------------------
// main function
int main() {
    QF_init();  // initialize the framework
    BSP_init(); // initialize the Board Support Package
    return QF_run(); // run the QF application
}

//--------------------------------------------------------------------------
// BSP functions
void BSP_init(void) {
    PRINTF_S("QP version: %s\nPress ^C to terminate\n", QP_VERSION_STR);
}
//..........................................................................
void BSP_ledOff(void) {
    PRINTF_S("%s\n", "LED OFF");
}
//..........................................................................
void BSP_ledOn(void) {
    PRINTF_S("%s\n", "LED ON");
}

//--------------------------------------------------------------------------
// QF functions
void QF_onStartup(void) {
    Blinky_ctor(); // explicitly call the "constructor"
    static QEvtPtr blinky_queueSto[10]; // event queue buffer
    QActive_start(AO_Blinky, // Active Object instance
        1U,                  // Active Object priority
        blinky_queueSto, Q_DIM(blinky_queueSto), // event queue
        (void *)0, 0U,       // no stack
        (void *)0);          // no initialization parameter
}
//..........................................................................
void QF_onCleanup(void) {}
//..........................................................................
void QF_onClockTick(void) {
    QTIMEEVT_TICK_X(0U, (void *)0);  // clock tick processing
}

//--------------------------------------------------------------------------
// QP error handler
Q_NORETURN Q_onError(char const * const module, int_t const id) {
    FPRINTF_S(stderr, "ERROR in %s:%d", module, id);
    exit(-1);
}
