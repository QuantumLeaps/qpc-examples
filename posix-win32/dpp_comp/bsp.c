//============================================================================
// Product: BSP for DPP example (console)
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
#include "app.h"                 // Application interface

#include "safe_std.h" // portable "safe" <stdio.h>/<string.h> facilities
#include <stdlib.h>   // for exit()

Q_DEFINE_THIS_FILE

#if (QP_VERSION < 800U)
#error this application requires qpc version 8.0.0 or higher
#endif

// local variables -----------------------------------------------------------
static QTicker l_Ticker0_inst; // "ticker" AO for tick rate 0
QTicker * const the_Ticker0 = &l_Ticker0_inst;

static uint32_t l_rnd; // random seed

#ifdef Q_SPY
    enum AppRecords { // application-specific trace records
        PHILO_STAT = QS_USER,
        PAUSED_STAT,
    };

    // QSpy source IDs
    static QSpyId const l_clock_tick = { QS_ID_AP };
#endif

//============================================================================
Q_NORETURN Q_onError(char const * const module, int_t const id) {
    QS_ASSERTION(module, id, 10000U); // report assertion to QS
    FPRINTF_S(stderr, ">>>> ERROR in %s:%d <<<<", module, id);
    QF_onCleanup();
    QS_EXIT();
    exit(-1);
}

//============================================================================
void BSP_init(void const * const arg) {
    Q_UNUSED_PAR(arg); // when Q_SPY not defined

    QF_consoleSetup();
    PRINTF_S("Dining Philosophers Problem example"
           "\nQP %s\n"
           "Press 'p' to pause\n"
           "Press 's' to serve\n"
           "Press ESC to quit...\n",
           QP_VERSION_STR);

    // initialize the QS software tracing...
    if (!QS_INIT(arg)) {
        Q_ERROR();
    }

    QS_OBJ_DICTIONARY(&l_clock_tick);
    QS_OBJ_DICTIONARY(the_Ticker0);

    QS_USR_DICTIONARY(PHILO_STAT);
    QS_USR_DICTIONARY(PAUSED_STAT);

    QS_ONLY(produce_sig_dict());

    // setup the QS filters...
    QS_GLB_FILTER(QS_GRP_ALL);
    QS_GLB_FILTER(-QS_QF_TICK); // exclude the tick record
    QS_LOC_FILTER(-(N_PHILO + 4)); // exclude the "ticker" prio

    // initialize event pools
    static QF_MPOOL_EL(TableEvt) smlPoolSto[2*N_PHILO];
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // initialize publish-subscribe
    static QSubscrList subscrSto[MAX_PUB_SIG];
    QActive_psInit(subscrSto, Q_DIM(subscrSto));

    BSP_randomSeed(1234U);
}
//............................................................................
void BSP_terminate(int16_t const result) {
    Q_UNUSED_PAR(result);
    QF_stop(); // stop the main "ticker thread"
}
//............................................................................
void BSP_displayPhilStat(uint8_t const n, char const * const stat) {
    PRINTF_S("Philosopher %2d is %s\n", (int)n, stat);

    // application-specific record
    QS_BEGIN_ID(PHILO_STAT, AO_Table->prio)
        QS_U8(1, n);  // Philosopher number
        QS_STR(stat); // Philosopher status
    QS_END()
}
//............................................................................
void BSP_displayPaused(uint8_t const paused) {
    PRINTF_S("Paused is %s\n", paused ? "ON" : "OFF");
}
//............................................................................
uint32_t BSP_random(void) { // a very cheap pseudo-random-number generator
    // "Super-Duper" Linear Congruential Generator (LCG)
    // LCG(2^32, 3*7*11*13*23, 0, seed)
    //
    uint32_t rnd = l_rnd * (3U*7U*11U*13U*23U);
    l_rnd = rnd;
    return rnd >> 8U;
}
//............................................................................
void BSP_randomSeed(uint32_t const seed) {
    l_rnd = seed;
}

//============================================================================
void QF_onStartup(void) {
    // instantiate and start AOs...
    QTicker_ctor(the_Ticker0, 0U);   // "ticker" AO for tick rate 0
    QActive_start(&the_Ticker0->super,
        N_PHILO + 4U,   // QP priority
        (QEvtPtr*)0, 0U, // no queue
        (void *)0, 0U,  // no stack storage
        (void *)0);     // no init. event

    static QEvtPtr tableQueueSto[N_PHILO];
    Table_ctor();
    QActive_start(AO_Table,
        N_PHILO + 7U,            // QP prio. of the AO
        tableQueueSto,           // event queue storage
        Q_DIM(tableQueueSto),    // queue length [events]
        (void *)0, 0U,           // no stack storage
        (void *)0);              // no initialization param

    QF_setTickRate(BSP_TICKS_PER_SEC, 50U); // desired tick rate/ticker-prio
}
//............................................................................
void QF_onCleanup(void) {
    PRINTF_S("\n%s\n", "Bye! Bye!");
    QF_consoleCleanup();
}
//............................................................................
void QF_onClockTick(void) {
    //QTIMEEVT_TICK_X(0U, &l_clock_tick); // process time events at rate 0
    QTICKER_TRIG(the_Ticker0, &l_clock_tick); // trigger ticker AO

    QS_RX_INPUT(); // handle the QS-RX input
    QS_OUTPUT();   // handle the QS output

    switch (QF_consoleGetKey()) {
        case '\33': { // ESC pressed?
            BSP_terminate(0);
            break;
        }
        case 'p': {
            static QEvt const pauseEvt = QEVT_INITIALIZER(PAUSE_SIG);
            QACTIVE_PUBLISH(&pauseEvt, &l_clock_tick);
            break;
        }
        case 's': {
            static QEvt const serveEvt = QEVT_INITIALIZER(SERVE_SIG);
            QACTIVE_PUBLISH(&serveEvt, &l_clock_tick);
            break;
        }
        default: {
            break;
        }
    }
}

//============================================================================
#ifdef Q_SPY

//............................................................................
void QS_onCommand(uint8_t cmdId,
    uint32_t param1, uint32_t param2, uint32_t param3)
{
    Q_UNUSED_PAR(cmdId);
    Q_UNUSED_PAR(param1);
    Q_UNUSED_PAR(param2);
    Q_UNUSED_PAR(param3);
}

#endif // Q_SPY


