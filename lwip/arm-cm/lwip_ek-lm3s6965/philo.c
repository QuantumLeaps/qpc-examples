//============================================================================
// DPP example
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
#include "qpc.h"     // QP/C header file
#include "bsp.h"     // Board Support Package header file
#include "app.h"     // application events and active objects

Q_DEFINE_THIS_FILE

// Active object class -----------------------------------------------------
typedef struct PhiloTag {
    QActive super;
    QTimeEvt timeEvt; // for timing out thinking or eating
} Philo;

static QState Philo_initial (Philo *me, QEvt const *e);
static QState Philo_thinking(Philo *me, QEvt const *e);
static QState Philo_hungry  (Philo *me, QEvt const *e);
static QState Philo_eating  (Philo *me, QEvt const *e);

// Local objects -----------------------------------------------------------
static Philo l_philo[N_PHILO]; // storage for all Philos

#define THINK_TIME  (BSP_TICKS_PER_SEC/2)
#define EAT_TIME    (BSP_TICKS_PER_SEC/5)

// helper macro to provide the ID of Philo "me_"
#define PHILO_ID(me_)    ((uint8_t)((me_) - l_philo))

enum InternalSignals { // internal signals
    TIMEOUT_SIG = MAX_SIG
};

// Global objects ----------------------------------------------------------
QActive * const AO_Philo[N_PHILO] = {     // "opaque" pointers to Philo AO
    (QActive *)&l_philo[0],
    (QActive *)&l_philo[1],
    (QActive *)&l_philo[2],
    (QActive *)&l_philo[3],
    (QActive *)&l_philo[4]
};

//............................................................................
void Philo_ctor(void) { // instantiate all Philo objects
    uint8_t n;
    Philo *me;
    for (n = 0; n < N_PHILO; ++n) {
        me = &l_philo[n];
        QActive_ctor(&me->super, (QStateHandler)&Philo_initial);
        QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0U);
    }
}
//............................................................................
QState Philo_initial(Philo *me, QEvt const *e) {
    static uint8_t registered; // starts off with 0, per C-standard
    (void)e; // suppress the compiler warning about unused parameter
    if (!registered) {
        QS_OBJ_DICTIONARY(&l_philo[0]);
        QS_OBJ_DICTIONARY(&l_philo[0].timeEvt);
        QS_OBJ_DICTIONARY(&l_philo[1]);
        QS_OBJ_DICTIONARY(&l_philo[1].timeEvt);
        QS_OBJ_DICTIONARY(&l_philo[2]);
        QS_OBJ_DICTIONARY(&l_philo[2].timeEvt);
        QS_OBJ_DICTIONARY(&l_philo[3]);
        QS_OBJ_DICTIONARY(&l_philo[3].timeEvt);
        QS_OBJ_DICTIONARY(&l_philo[4]);
        QS_OBJ_DICTIONARY(&l_philo[4].timeEvt);

        QS_FUN_DICTIONARY(&Philo_initial);
        QS_FUN_DICTIONARY(&Philo_thinking);
        QS_FUN_DICTIONARY(&Philo_hungry);
        QS_FUN_DICTIONARY(&Philo_eating);

        registered = (uint8_t)1;
    }
    QS_SIG_DICTIONARY(HUNGRY_SIG, me);  // signal for each Philos
    QS_SIG_DICTIONARY(TIMEOUT_SIG, me); // signal for each Philos

    QActive_subscribe((QActive *)me, EAT_SIG);

    return Q_TRAN(&Philo_thinking); // top-most initial transition
}
//............................................................................
QState Philo_thinking(Philo *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            QTimeEvt_armX(&me->timeEvt, THINK_TIME, 0U);
            return Q_HANDLED();
        }
        case TIMEOUT_SIG: {
            return Q_TRAN(&Philo_hungry);
        }
        case EAT_SIG:  // intentionally fall-through
        case DONE_SIG: {
            // EAT or DONE must be for other Philos than this one
            Q_ASSERT(((TableEvt const *)e)->philoNum != PHILO_ID(me));
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
//............................................................................
QState Philo_hungry(Philo *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            TableEvt *pe = Q_NEW(TableEvt, HUNGRY_SIG);
            pe->philoNum = PHILO_ID(me);
            QACTIVE_POST(AO_Table, (QEvt *)pe, me);
            return Q_HANDLED();
        }
        case EAT_SIG: {
            if (((TableEvt const *)e)->philoNum == PHILO_ID(me)) {
                return Q_TRAN(&Philo_eating);
            }
            break;
        }
        case DONE_SIG: {
            // DONE must be for other Philos than this one
            Q_ASSERT(((TableEvt const *)e)->philoNum != PHILO_ID(me));
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
//............................................................................
QState Philo_eating(Philo *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            QTimeEvt_armX(&me->timeEvt, EAT_TIME, 0U);
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            TableEvt *pe = Q_NEW(TableEvt, DONE_SIG);
            pe->philoNum = PHILO_ID(me);
            QACTIVE_PUBLISH((QEvt *)pe, &me->super);
            return Q_HANDLED();
        }
        case TIMEOUT_SIG: {
            return Q_TRAN(&Philo_thinking);
        }
        case EAT_SIG: // intentionally fall-through
        case DONE_SIG: {
            // EAT or DONE must be for other Philos than this one
            Q_ASSERT(((TableEvt const *)e)->philoNum != PHILO_ID(me));
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}

