//$file${.::periodic1.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: real-time-ms.qm
// File:  ${.::periodic1.c}
//
// This code has been generated by QM 7.0.1 <www.state-machine.com/qm>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// Copyright (c) 2005 Quantum Leaps, LLC. All rights reserved.
//
//                 ____________________________________
//                /                                   /
//               /    GGGGGGG    PPPPPPPP   LL       /
//              /   GG     GG   PP     PP  LL       /
//             /   GG          PP     PP  LL       /
//            /   GG   GGGGG  PPPPPPPP   LL       /
//           /   GG      GG  PP         LL       /
//          /     GGGGGGG   PP         LLLLLLL  /
//         /___________________________________/
//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// This generated code is open-source software licensed under the GNU
// General Public License (GPL) as published by the Free Software Foundation
// (see <https://www.gnu.org/licenses>).
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
//
//$endhead${.::periodic1.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include "qpc.h"               // QP/C real-time event framework
#include "bsp.h"               // Board Support Package
#include "app.h"               // Application interface

//$declare${AOs::Periodic1} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${AOs::Periodic1} ..........................................................
typedef struct Periodic1 {
// protected:
    QMActive super;

// public:
    QTimeEvt te;
    uint16_t toggles;
} Periodic1;

extern Periodic1 Periodic1_inst;

// protected:
static QState Periodic1_initial(Periodic1 * const me, void const * const par);
static QState Periodic1_active  (Periodic1 * const me, QEvt const * const e);
static QMState const Periodic1_active_s = {
    QM_STATE_NULL, // superstate (top)
    Q_STATE_CAST(&Periodic1_active),
    Q_ACTION_NULL, // no entry action
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
//$enddecl${AOs::Periodic1} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Q_DEFINE_THIS_MODULE("periodic1")

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U)%0x2710U))
#error qpc version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Shared::Periodic1_ctor} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::Periodic1_ctor} ..................................................
void Periodic1_ctor(void) {
    Periodic1 * const me = &Periodic1_inst;
    QMActive_ctor(&me->super, Q_STATE_CAST(&Periodic1_initial));
    QTimeEvt_ctorX(&me->te, (QActive *)me, TIMEOUT_SIG, 0U);
}
//$enddef${Shared::Periodic1_ctor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Shared::AO_Periodic1} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::AO_Periodic1} ....................................................
QActive * const AO_Periodic1 = (QActive *)&Periodic1_inst;
//$enddef${Shared::AO_Periodic1} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${AOs::Periodic1} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${AOs::Periodic1} ..........................................................
Periodic1 Periodic1_inst;

//${AOs::Periodic1::SM} ......................................................
static QState Periodic1_initial(Periodic1 * const me, void const * const par) {
    //${AOs::Periodic1::SM::initial}
    // the initial event must be provided and must be WORKLOAD_SIG
    QEvt const *e =  (QEvt const *)par;

    Q_REQUIRE_ID(300, (e != (QEvt const *)0)
                 && (e->sig == PERIODIC_SPEC_SIG));

    QTimeEvt_armX(&me->te,
        Q_EVT_CAST(PeriodicSpecEvt)->ticks,
        Q_EVT_CAST(PeriodicSpecEvt)->ticks);
    me->toggles = Q_EVT_CAST(PeriodicSpecEvt)->toggles;

    QS_FUN_DICTIONARY(&Periodic1_active);

    static QMTranActTable const tatbl_ = { // tran-action table
        &Periodic1_active_s, // target state
        {
            Q_ACTION_NULL // zero terminator
        }
    };
    return QM_TRAN_INIT(&tatbl_);
}

//${AOs::Periodic1::SM::active} ..............................................
//${AOs::Periodic1::SM::active}
static QState Periodic1_active(Periodic1 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::Periodic1::SM::active::TIMEOUT}
        case TIMEOUT_SIG: {
            for (uint16_t i = me->toggles; i > 0U; --i) {
                BSP_d6on();
                BSP_d6off();
            }

            #ifdef USE_SCHED_DISABLE
            QV_schedDisable(1U); // <== disable scheduler up to given prio.
            #endif
            status_ = QM_HANDLED();
            break;
        }
        //${AOs::Periodic1::SM::active::PERIODIC_SPEC}
        case PERIODIC_SPEC_SIG: {
            BSP_d6on();
            QTimeEvt_disarm(&me->te);
            QTimeEvt_armX(&me->te,
                Q_EVT_CAST(PeriodicSpecEvt)->ticks,
                Q_EVT_CAST(PeriodicSpecEvt)->ticks);
            me->toggles = Q_EVT_CAST(PeriodicSpecEvt)->toggles;
            BSP_d6off();
            status_ = QM_HANDLED();
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
//$enddef${AOs::Periodic1} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
