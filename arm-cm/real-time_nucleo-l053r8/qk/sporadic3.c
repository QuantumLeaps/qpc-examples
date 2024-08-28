//$file${.::sporadic3.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: real-time.qm
// File:  ${.::sporadic3.c}
//
// This code has been generated by QM 6.2.1 <www.state-machine.com/qm>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
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
// Please contact Quantum Leaps for commercial licensing options, which
// expressly supersede the GNU General Public License and are specifically
// designed for licensees interested in retaining the proprietary status
// of the generated code.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//
//$endhead${.::sporadic3.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include "qpc.h"               // QP/C real-time embedded framework
#include "bsp.h"               // Board Support Package
#include "app.h"               // Application interface

//$declare${AOs::Sporadic3} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${AOs::Sporadic3} ..........................................................
typedef struct Sporadic3 {
// protected:
    QMActive super;

// public:
} Sporadic3;

extern Sporadic3 Sporadic3_inst;

// protected:
static QState Sporadic3_initial(Sporadic3 * const me, void const * const par);
static QState Sporadic3_active  (Sporadic3 * const me, QEvt const * const e);
static QMState const Sporadic3_active_s = {
    QM_STATE_NULL, // superstate (top)
    Q_STATE_CAST(&Sporadic3_active),
    Q_ACTION_NULL, // no entry action
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
//$enddecl${AOs::Sporadic3} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Q_DEFINE_THIS_MODULE("sporadic3")

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U)%0x2710U))
#error qpc version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Shared::Sporadic3_ctor} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::Sporadic3_ctor} ..................................................
void Sporadic3_ctor(void) {
    Sporadic3 * const me = &Sporadic3_inst;
    QMActive_ctor(&me->super, Q_STATE_CAST(&Sporadic3_initial));
}
//$enddef${Shared::Sporadic3_ctor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Shared::AO_Sporadic3} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::AO_Sporadic3} ....................................................
QActive * const AO_Sporadic3 = (QActive *)&Sporadic3_inst;
//$enddef${Shared::AO_Sporadic3} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${AOs::Sporadic3} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${AOs::Sporadic3} ..........................................................
Sporadic3 Sporadic3_inst;

//${AOs::Sporadic3::SM} ......................................................
static QState Sporadic3_initial(Sporadic3 * const me, void const * const par) {
    //${AOs::Sporadic3::SM::initial}
    Q_UNUSED_PAR(par);

    QS_FUN_DICTIONARY(&Sporadic3_active);

    static QMTranActTable const tatbl_ = { // tran-action table
        &Sporadic3_active_s, // target state
        {
            Q_ACTION_NULL // zero terminator
        }
    };
    return QM_TRAN_INIT(&tatbl_);
}

//${AOs::Sporadic3::SM::active} ..............................................
//${AOs::Sporadic3::SM::active}
static QState Sporadic3_active(Sporadic3 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::Sporadic3::SM::active::SPORADIC_A}
        case SPORADIC_A_SIG: {
            BSP_d4on();
            // Sporadic3 --> Periodic4
            QACTIVE_POST(AO_Periodic4, BSP_getEvtPeriodic4(1U), me);
            BSP_d4off();

            for (uint16_t i = Q_EVT_CAST(SporadicSpecEvt)->toggles;
                 i > 0U; --i)
            {
                BSP_d4on();
                BSP_d4off();
            }
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
//$enddef${AOs::Sporadic3} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
