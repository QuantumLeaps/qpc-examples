//============================================================================
// Product: main function for emWin/uC/GUI, Win32 simulation
// Last updated for version 8.0.0
// Last updated on  2024-09-18
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <www.gnu.org/licenses/>.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"
#include "bsp.h"
#include "dpp.h"

// GUI includes

//Q_DEFINE_THIS_FILE

// Local-scope objects -----------------------------------------------------
static QEvtPtr l_tableQueueSto[N_PHILO];
static QEvtPtr l_philoQueueSto[N_PHILO][N_PHILO];
static QSubscrList   l_subscrSto[MAX_PUB_SIG];

static union SmallEvent {
    void *min_size;
    TableEvt te;
    MouseEvt me;
    // other event types to go into this pool
} l_smlPoolSto[2*N_PHILO]; // storage for the small event pool

//............................................................................
void MainTask(void) {
    QF_init();    // initialize the framework and the underlying RT kernel
    BSP_init();   // initialize the Board Support Package

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

