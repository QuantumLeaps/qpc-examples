//============================================================================
// BSP for the "reminder2" example
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
    PRINTF_S("Reminder state pattern\nQP version: %s\n"
           "Press 'e' to echo the current value...\n"
           "Press ESC to quit...\n",
           QP_VERSION_STR);

    // initialize event pool(s) for dynamic events...
    static QF_MPOOL_EL(ReminderEvt) smlPoolSto[20]; // storage for small pool
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // publish-subscribe not used, no call to QActive_psInit()
}
//............................................................................
void BSP_showMsg(char const * const msg, double x) {
    PRINTF_S(msg, x);
}

//============================================================================
void QF_onStartup(void) {
    // instantiate and start the active objects...
    Cruncher_ctor();
    static QEvtPtr cruncherQSto[10]; // event queue storage for Cruncher AO
    QActive_start(AO_Cruncher,
        1U,
        cruncherQSto, Q_DIM(cruncherQSto),
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
            case 'e': { // 'e' pressed -> post the ECHO event
                // NOTE:
                // Posting the ECHO event would be best handled with an
                // immutable event, as it is illustrated in the next case
                // of the TERMINATE event below (case '\033').
                //
                // However, for the sake of demonstrating the Q_NEW_X()
                // facility, the ECHO event is allocated dynamically, which
                // is allowed to fail because the ECHO event is not
                // considered critical.
#ifdef QEVT_PAR_INIT
                QEvt const * const echoEvt = Q_NEW_X(QEvt, 2U, ECHO_SIG, 0U);
#else
                QEvt * const echoEvt = Q_NEW_X(QEvt, 2U, ECHO_SIG);
#endif
                if (echoEvt != (QEvt *)0) { // event allocated successfully?
                    QACTIVE_POST_X(AO_Cruncher, echoEvt, 2U, (void *)0);
                }
                break;
            }
            case '\033': { // ESC pressed?
                static QEvt const terminateEvt = QEVT_INITIALIZER(TERMINATE_SIG);
                QACTIVE_POST(AO_Cruncher, &terminateEvt, (void *)0);
                break;
            }
        }
    }
}
