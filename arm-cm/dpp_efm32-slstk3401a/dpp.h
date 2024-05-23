//$file${.::dpp.h} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: dpp.qm
// File:  ${.::dpp.h}
//
// This code has been generated by QM 6.1.2 <www.state-machine.com/qm>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// This generated code is open source software: you can redistribute it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation.
//
// This code is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// NOTE:
// Alternatively, this generated code may be distributed under the terms
// of Quantum Leaps commercial licenses, which expressly supersede the GNU
// General Public License and are specifically designed for licensees
// interested in retaining the proprietary status of their code.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//
//$endhead${.::dpp.h} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#ifndef DPP_H_
#define DPP_H_

//$declare${Shared} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::AppSignals} ......................................................
enum AppSignals {
    EAT_SIG = Q_USER_SIG, // published by Table to let a Philo eat
    DONE_SIG,       // published by Philo when done eating
    PAUSE_SIG,      // published by BSP to pause the application
    SERVE_SIG,      // published by BSP to serve re-start serving forks
    TEST_SIG,       // published by BSP to test the application
    MAX_PUB_SIG,    // the last published signal

    TIMEOUT_SIG,    // posted by time event to Philo
    HUNGRY_SIG,     // posted by hungry Philo to Table
    MAX_SIG         // the last signal
};

//${Shared::produce_sig_dict} ................................................
#ifdef Q_SPY
static inline void produce_sig_dict(void) {
    QS_SIG_DICTIONARY(EAT_SIG,     (void *)0);
    QS_SIG_DICTIONARY(DONE_SIG,    (void *)0);
    QS_SIG_DICTIONARY(PAUSE_SIG,   (void *)0);
    QS_SIG_DICTIONARY(SERVE_SIG,   (void *)0);
    QS_SIG_DICTIONARY(TEST_SIG,    (void *)0);

    QS_SIG_DICTIONARY(TIMEOUT_SIG, (void *)0);
    QS_SIG_DICTIONARY(HUNGRY_SIG,  (void *)0);
}
#endif // def Q_SPY

//${Shared::N_PHILO} .........................................................
#define N_PHILO ((uint8_t)5U)

//${Shared::TableEvt} ........................................................
typedef struct {
// protected:
    QEvt super;

// public:
    uint8_t philoId;
} TableEvt;

// public:
static inline TableEvt * TableEvt_init(TableEvt * const me,
    uint8_t id)
{
    me->philoId = id;
    return me;
}

//${Shared::AO_Philo[N_PHILO]} ...............................................
extern QActive * const AO_Philo[N_PHILO];

//${Shared::Philo_ctor} ......................................................
void Philo_ctor(uint_fast8_t const id);

//${Shared::AO_Table} ........................................................
extern QActive * const AO_Table;

//${Shared::Table_ctor} ......................................................
void Table_ctor(void);
//$enddecl${Shared} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#ifdef QXK_H_

extern QXThread * const TH_XThread1;
void XThread1_ctor(void);

extern QXThread * const TH_XThread2;
void XThread2_ctor(void);

extern QXSemaphore TH_sema;
extern QXMutex TH_mutex;

#endif // QXK_H_

#endif // DPP_H_
