//============================================================================
// Product: main function for emWin/uC/GUI, Win32 simulation
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

//Q_DEFINE_THIS_FILE

// Local-scope objects -----------------------------------------------------
static QEvtPtr l_tableQueueSto[N_PHILO];
static QEvtPtr l_philoQueueSto[N_PHILO][N_PHILO];
static QSubscrList l_subscrSto[MAX_PUB_SIG];

static union SmallEvent {
    void *min_size;
    TableEvt te;
    MouseEvt me;
    // other event types to go into this pool
} l_smlPoolSto[2*N_PHILO]; // storage for the small event pool

//............................................................................
void MainTask(void) {
    QF_init(); // initialize the framework and the underlying RT kernel
    BSP_init((void *)0);   // initialize the Board Support Package

    // object dictionaries...
    QS_OBJ_DICTIONARY(l_smlPoolSto);
    QS_OBJ_DICTIONARY(l_tableQueueSto);
    QS_OBJ_DICTIONARY(l_philoQueueSto[0]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[1]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[2]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[3]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[4]);

    // init publish-subscribe
    QActive_psInit(l_subscrSto, Q_DIM(l_subscrSto));

    // initialize event pools...
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));

    Philo_ctor(); // instantiate all Philosopher active objects
    for (uint8_t n = 0U; n < N_PHILO; ++n) { // start the active objects...
        QActive_start(AO_Philo[n], (n + 1U),
                      l_philoQueueSto[n], Q_DIM(l_philoQueueSto[n]),
                      (void *)0, 1024, (QEvt *)0); // 1K of stack
    }

    Table_ctor(); // instantiate the Table active object
    QActive_start(AO_Table, (N_PHILO + 1U),
                  l_tableQueueSto, Q_DIM(l_tableQueueSto),
                  (void *)0, 1024, (QEvt *)0); // 1K of stack

    QF_run(); // run the QF application
}
