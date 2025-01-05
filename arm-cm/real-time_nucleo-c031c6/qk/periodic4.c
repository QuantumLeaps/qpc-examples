//$file${.::periodic4.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: real-time.qm
// File:  ${.::periodic4.c}
//
// This code has been generated by QM 7.0.0 <www.state-machine.com/qm>.
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
//$endhead${.::periodic4.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include "qpc.h"               // QP/C real-time event framework
#include "bsp.h"               // Board Support Package
#include "app.h"               // Application interface

//$declare${AOs::Periodic4} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${AOs::Periodic4} ..........................................................
typedef struct Periodic4 {
// protected:
    QMActive super;

// public:
    QTimeEvt te;
    uint16_t toggles;
} Periodic4;

extern Periodic4 Periodic4_inst;

// protected:
static QState Periodic4_initial(Periodic4 * const me, void const * const par);
static QState Periodic4_active  (Periodic4 * const me, QEvt const * const e);
static QMState const Periodic4_active_s = {
    QM_STATE_NULL, // superstate (top)
    Q_STATE_CAST(&Periodic4_active),
    Q_ACTION_NULL, // no entry action
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
//$enddecl${AOs::Periodic4} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Q_DEFINE_THIS_MODULE("periodic4")

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U)%0x2710U))
#error qpc version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Shared::Periodic4_ctor} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::Periodic4_ctor} ..................................................
void Periodic4_ctor(void) {
    Periodic4 * const me = &Periodic4_inst;
    QMActive_ctor(&me->super, Q_STATE_CAST(&Periodic4_initial));
    QTimeEvt_ctorX(&me->te, (QActive *)me, TIMEOUT_SIG, 0U);
}
//$enddef${Shared::Periodic4_ctor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Shared::AO_Periodic4} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::AO_Periodic4} ....................................................
QActive * const AO_Periodic4 = (QActive *)&Periodic4_inst;
//$enddef${Shared::AO_Periodic4} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${AOs::Periodic4} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${AOs::Periodic4} ..........................................................
Periodic4 Periodic4_inst;

//${AOs::Periodic4::SM} ......................................................
static QState Periodic4_initial(Periodic4 * const me, void const * const par) {
    //${AOs::Periodic4::SM::initial}
    // the initial event must be provided and must be WORKLOAD_SIG
    QEvt const *e =  (QEvt const *)par;

    Q_REQUIRE_ID(300, (e != (QEvt const *)0)
                 && (e->sig == PERIODIC_SPEC_SIG));

    QTimeEvt_armX(&me->te,
        Q_EVT_CAST(PeriodicSpecEvt)->ticks,
        Q_EVT_CAST(PeriodicSpecEvt)->ticks);
    me->toggles = Q_EVT_CAST(PeriodicSpecEvt)->toggles;

    QS_FUN_DICTIONARY(&Periodic4_active);

    static QMTranActTable const tatbl_ = { // tran-action table
        &Periodic4_active_s, // target state
        {
            Q_ACTION_NULL // zero terminator
        }
    };
    return QM_TRAN_INIT(&tatbl_);
}

//${AOs::Periodic4::SM::active} ..............................................
//${AOs::Periodic4::SM::active}
static QState Periodic4_active(Periodic4 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::Periodic4::SM::active::TIMEOUT}
        case TIMEOUT_SIG: {
            for (uint16_t i = me->toggles; i > 0U; --i) {
                BSP_d3on();
                BSP_d3off();
            }
            status_ = QM_HANDLED();
            break;
        }
        //${AOs::Periodic4::SM::active::PERIODIC_SPEC}
        case PERIODIC_SPEC_SIG: {
            BSP_d3on();
            QTimeEvt_disarm(&me->te);
            QTimeEvt_armX(&me->te,
                Q_EVT_CAST(PeriodicSpecEvt)->ticks,
                Q_EVT_CAST(PeriodicSpecEvt)->ticks);
            me->toggles = Q_EVT_CAST(PeriodicSpecEvt)->toggles;
            BSP_d3off();
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
//$enddef${AOs::Periodic4} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
