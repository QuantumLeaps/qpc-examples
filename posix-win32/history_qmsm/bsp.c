//============================================================================
// BSP for the "history" example
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
    PRINTF_S("History state pattern\nQP version: %s\n"
           "Press 'o' to OPEN  the door\n"
           "Press 'c' to CLOSE the door\n"
           "Press 't' to start TOASTING\n"
           "Press 'b' to start BAKING\n"
           "Press 'f' to turn the oven OFF\n"
           "Press ESC to quit...\n",
           QP_VERSION_STR);

    // dynamic events not used, no need to call QF_poolInit()
    // publish-subscribe not used, no call to QActive_psInit()
}
//............................................................................
void BSP_showMsg(char const * const msg) {
    PRINTF_S("%s\n", msg);
}

//============================================================================
void QF_onStartup(void) {
    // instantiate the ToastOven HSM and trigger the initial transition
    ToastOven_ctor();
    QASM_INIT(SM_ToastOven, (void *)0, 0U);

    for (;;) {
        PRINTF_S("\n", "");
        uint8_t c = (uint8_t)QF_consoleWaitForKey();
        PRINTF_S("%c: ", (c >= ' ') ? c : 'X');

        QSignal sig = 0U;
        switch (c) {
            case 'o':  sig = OPEN_SIG;        break;
            case 'c':  sig = CLOSE_SIG;       break;
            case 't':  sig = TOAST_SIG;       break;
            case 'b':  sig = BAKE_SIG;        break;
            case 'f':  sig = OFF_SIG;         break;
            case 0x1B: sig = TERMINATE_SIG;   break;
            default: PRINTF_S("unrecognized", ""); break;
        }

        // dispatch the event into the state machine
        QEvt e = QEVT_INITIALIZER(sig); // automatic event (on the stack)
        QASM_DISPATCH(SM_ToastOven,  &e, 0U); // synchronously dispatch
    }
}
//............................................................................
void QF_onCleanup(void) {
    PRINTF_S("\n%s\n", "Bye! Bye!");
    QF_consoleCleanup();
}
//............................................................................
void QF_onClockTick(void) {
}
