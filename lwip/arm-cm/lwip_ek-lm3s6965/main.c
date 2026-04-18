//============================================================================
// Product: DPP with lwIP application
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
#include "qpc.h"   // QP/C API
#include "bsp.h"   // Board Support Package
#include "app.h"   // application events and active objects

//Q_DEFINE_THIS_FILE

// Local-scope objects -----------------------------------------------------
static QEvtPtr l_tableQueueSto[N_PHILO];
static QEvtPtr l_philoQueueSto[N_PHILO][N_PHILO];
static QEvtPtr l_lwIPMgrQueueSto[10];
static QSubscrList l_subscrSto[MAX_PUB_SIG];

static QF_MPOOL_EL(TableEvt) l_smlPoolSto[20]; // small event pool
static QF_MPOOL_EL(TextEvt)  l_medPoolSto[4];  // med.  event pool

//............................................................................
int main(void) {
    uint8_t n;

    Philo_ctor();   // instantiate all Philosopher active objects
    Table_ctor();   // instantiate the Table active object
    LwIPMgr_ctor(); // instantiate all LwIP-Manager active object

    BSP_init((void *)0);     // initialize the Board Support Package

    QF_init(); // initialize the framework and the underlying RT kernel

    // object dictionaries...
    QS_OBJ_DICTIONARY(l_smlPoolSto);
    QS_OBJ_DICTIONARY(l_medPoolSto);
    QS_OBJ_DICTIONARY(l_lwIPMgrQueueSto);
    QS_OBJ_DICTIONARY(l_philoQueueSto[0]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[1]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[2]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[3]);
    QS_OBJ_DICTIONARY(l_philoQueueSto[4]);
    QS_OBJ_DICTIONARY(l_tableQueueSto);

    // init publish-subscribe
    QActive_psInit(l_subscrSto, Q_DIM(l_subscrSto));

    // initialize event pools...
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));
    QF_poolInit(l_medPoolSto, sizeof(l_medPoolSto), sizeof(l_medPoolSto[0]));

    QActive_start(AO_LwIPMgr, 1,
                  l_lwIPMgrQueueSto, Q_DIM(l_lwIPMgrQueueSto),
                  (void *)0, 0, (QEvt *)0);
    for (n = 0; n < N_PHILO; ++n) { // start the active objects...
        QActive_start(AO_Philo[n], (uint8_t)(n + 2),
                      l_philoQueueSto[n], Q_DIM(l_philoQueueSto[n]),
                      (void *)0, 0, (QEvt *)0);
    }
    QActive_start(AO_Table, (uint8_t)(N_PHILO + 2),
                  l_tableQueueSto, Q_DIM(l_tableQueueSto),
                  (void *)0, 0, (QEvt *)0);

    return QF_run(); // run the QF application
}
