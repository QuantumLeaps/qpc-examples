//============================================================================
// BSP for comp example
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
    PRINTF_S("Orthogonal Component pattern\nQP version: %s\n"
           "Press 'o' to turn the Alarm ON\n"
           "Press 'f' to turn the Alarm OFF\n"
           "Press '0'..'9' to set the Alarm time\n"
           "Press 'a' to set the Clock in 12-hour mode\n"
           "Press 'b' to set the Clock in 24-hour mode\n"
           "Press ESC to quit...\n",
           QP_VERSION_STR);

    // initialize event pools...
    static QF_MPOOL_EL(TimeEvt) smlPoolSto[10]; // storage for small pool
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // publish-subscribe not used, no call to QActive_psInit()
}
//............................................................................
void BSP_showMsg(char const *str) {
    PRINTF_S("%s\n", str);
    fflush(stdout);
}
//............................................................................
void BSP_showTime24H(char const *str, uint32_t time, uint32_t base) {
    PRINTF_S("%s, %02d:%02d\n", str, (int)(time / base), (int)(time % base));
    fflush(stdout);
}
//............................................................................
void BSP_showTime12H(char const *str, uint32_t time, uint32_t base) {
    uint32_t h = time / base;

    PRINTF_S("%s %02d:%02d %s\n", str, (h % 12) ? (h % 12) : 12,
           time % base, (h / 12) ? "PM" : "AM");
    fflush(stdout);
}

//============================================================================
void QF_onStartup(void) {
    // instantiate and start AOs...
    AlarmClock_ctor();
    static QEvtPtr alarmClockQSto[10]; // queue storage for AlarmClock
    QActive_start(APP_alarmClock, 1U,
                  alarmClockQSto, Q_DIM(alarmClockQSto),
                  (void *)0, 0U, (QEvt *)0);

    QF_setTickRate(BSP_TICKS_PER_SEC, 50U); // desired tick rate/ticker-prio
}
//............................................................................
void QF_onCleanup(void) {
    PRINTF_S("\n%s\n", "Bye! Bye!");
    QF_consoleCleanup();
}
//............................................................................
void QF_onClockTick(void) {
    QTIMEEVT_TICK_X(0U, (void *)0); // perform clock tick processing

    int key = QF_consoleGetKey();
    if (key != 0) { // any key pressed?
        switch (key) {
            case 'o': { // 'o': Alarm on event?
                QACTIVE_POST(APP_alarmClock,
                             Q_NEW(QEvt, ALARM_ON_SIG), (void *)0);
                break;
            }
            case 'f': { // 'f': Alarm off event?
                QACTIVE_POST(APP_alarmClock,
                             Q_NEW(QEvt, ALARM_OFF_SIG), (void *)0);
                break;
            }
            case '1':   // '1'
            case '2':   // '2'
            case '3':   // '3'
            case '4':   // '4'
            case '5':   // '5'
            case '6':   // '6'
            case '7':   // '7'
            case '8':   // '8'
            case '9': { // '9'
                SetEvt *e = Q_NEW(SetEvt, ALARM_SET_SIG);
                e->digit = (uint8_t)(key - '0');
                QACTIVE_POST(APP_alarmClock, (QEvt *)e, (void *)0);
                break;
            }
            case '0': { // '0'
                SetEvt *e = Q_NEW(SetEvt, ALARM_SET_SIG);
                e->digit = 0;
                QACTIVE_POST(APP_alarmClock, (QEvt *)e, (void *)0);
                break;
            }
            case 'a': { // 'a': Clock 12H event?
                QACTIVE_POST(APP_alarmClock,
                             Q_NEW(QEvt, CLOCK_12H_SIG), (void *)0);
                break;
            }
            case 'b': { // 'b': Clock 24H event?
                QACTIVE_POST(APP_alarmClock,
                             Q_NEW(QEvt, CLOCK_24H_SIG), (void *)0);
                break;
            }
            case '\33': { // ESC pressed?
                QACTIVE_POST(APP_alarmClock,
                             Q_NEW(QEvt, TERMINATE_SIG), (void *)0);
                break;
            }
        }
    }
}
