//$file${.::tstsm.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: tstsm.qm
// File:  ${.::tstsm.c}
//
// This code has been generated by QM 7.0.1 <www.state-machine.com/qm>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// SPDX-License-Identifier: LicenseRef-QL-cert
//
// This software is part of the QP/C Certification Kit, which is licensed
// under the terms of the Quantum Leaps Certification Kit licenses. Please
// contact Quantum Leaps for more information about the available licensing
// options.
//
// RESTRICTIONS
// You may NOT:
// (a) redistribute, encumber, sell, rent, lease, sublicense, or otherwise
//     transfer rights in this software,
// (b) remove or alter any trademark, logo, copyright or other proprietary
//     notices, legends, symbols or labels present in this software,
// (c) plagiarize this software to sidestep the licensing obligations.
//
// Quantum Leaps contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//
//$endhead${.::tstsm.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include "qpc.h"     // QP/C framework
#include "tstsm.h"   // TstSM state machine

Q_DEFINE_THIS_FILE

//$declare${SMs} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${SMs::TstSM} ..............................................................
typedef struct TstSM {
// protected:
    QHsm super;

// private:
    uint8_t foo;

// public:

// private state histories
    QStateHandler hist_s;
    QStateHandler hist_s1;
    QStateHandler hist_s2;
} TstSM;

extern TstSM TstSM_inst;

// protected:
static QState TstSM_initial(TstSM * const me, void const * const par);
static QState TstSM_s(TstSM * const me, QEvt const * const e);
static QState TstSM_s1(TstSM * const me, QEvt const * const e);
static QState TstSM_s11(TstSM * const me, QEvt const * const e);
static QState TstSM_s111(TstSM * const me, QEvt const * const e);
static QState TstSM_s112(TstSM * const me, QEvt const * const e);
static QState TstSM_s12(TstSM * const me, QEvt const * const e);
static QState TstSM_s2(TstSM * const me, QEvt const * const e);
static QState TstSM_s21(TstSM * const me, QEvt const * const e);
static QState TstSM_s211(TstSM * const me, QEvt const * const e);
static QState TstSM_s212(TstSM * const me, QEvt const * const e);
static QState TstSM_s22(TstSM * const me, QEvt const * const e);
static QState TstSM_t(TstSM * const me, QEvt const * const e);
//$enddecl${SMs} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U)%0x2710U))
#error qpc version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Shared} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::the_TstSM} .......................................................
QAsm * const the_TstSM = &TstSM_inst.super.super;

//${Shared::TstSM_ctor} ......................................................
void TstSM_ctor(void) {
    TstSM *me = &TstSM_inst;
    QHsm_ctor(&me->super, Q_STATE_CAST(&TstSM_initial));
}

//${Shared::TstSM_isIn} ......................................................
bool TstSM_isIn(uint32_t const state_num) {
    bool stat = false;
    switch (state_num) {
    case 0:
        stat = QASM_IS_IN(the_TstSM, Q_STATE_CAST(&TstSM_s));
        break;
    case 1:
        stat = QASM_IS_IN(the_TstSM, Q_STATE_CAST(&TstSM_s1));
        break;
    case 11:
        stat = QASM_IS_IN(the_TstSM, Q_STATE_CAST(&TstSM_s11));
        break;
    case 111:
        stat = QASM_IS_IN(the_TstSM, Q_STATE_CAST(&TstSM_s111));
        break;
    case 112:
        stat = QASM_IS_IN(the_TstSM, Q_STATE_CAST(&TstSM_s112));
        break;
    case 12:
        stat = QASM_IS_IN(the_TstSM, Q_STATE_CAST(&TstSM_s12));
        break;
    case 2:
        stat = QASM_IS_IN(the_TstSM, Q_STATE_CAST(&TstSM_s2));
        break;
    case 21:
        stat = QASM_IS_IN(the_TstSM, Q_STATE_CAST(&TstSM_s21));
        break;
    case 211:
        stat = QASM_IS_IN(the_TstSM, Q_STATE_CAST(&TstSM_s211));
        break;
    case 212:
        stat = QASM_IS_IN(the_TstSM, Q_STATE_CAST(&TstSM_s212));
        break;
    case 22:
        stat = QASM_IS_IN(the_TstSM, Q_STATE_CAST(&TstSM_s22));
        break;
    default:
        Q_ERROR();
    }
    return stat;
}
//$enddef${Shared} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${SMs} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${SMs::TstSM} ..............................................................
TstSM TstSM_inst;

//${SMs::TstSM::SM} ..........................................................
static QState TstSM_initial(TstSM * const me, void const * const par) {
    //${SMs::TstSM::SM::initial}
    Q_UNUSED_PAR(par);
    me->foo = 0U;
    BSP_display("top-INIT;");

    QS_OBJ_DICTIONARY(&TstSM_inst);
    // state history attributes
    me->hist_s = Q_STATE_CAST(&TstSM_s1);
    me->hist_s1 = Q_STATE_CAST(&TstSM_s11);
    me->hist_s2 = Q_STATE_CAST(&TstSM_s211);

    QS_FUN_DICTIONARY(&TstSM_s);
    QS_FUN_DICTIONARY(&TstSM_s1);
    QS_FUN_DICTIONARY(&TstSM_s11);
    QS_FUN_DICTIONARY(&TstSM_s111);
    QS_FUN_DICTIONARY(&TstSM_s112);
    QS_FUN_DICTIONARY(&TstSM_s12);
    QS_FUN_DICTIONARY(&TstSM_s2);
    QS_FUN_DICTIONARY(&TstSM_s21);
    QS_FUN_DICTIONARY(&TstSM_s211);
    QS_FUN_DICTIONARY(&TstSM_s212);
    QS_FUN_DICTIONARY(&TstSM_s22);
    QS_FUN_DICTIONARY(&TstSM_t);

    return Q_TRAN(&TstSM_s2);
}

//${SMs::TstSM::SM::s} .......................................................
static QState TstSM_s(TstSM * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::TstSM::SM::s}
        case Q_ENTRY_SIG: {
            BSP_display("s-ENTRY;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s}
        case Q_EXIT_SIG: {
            BSP_display("s-EXIT;");
            // save shallow history
            me->hist_s =
                QHsm_childState(Q_HSM_UPCAST(me),
                                Q_STATE_CAST(&TstSM_s));
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::initial}
        case Q_INIT_SIG: {
            BSP_display("s-INIT;");
            status_ = Q_TRAN(&TstSM_s1);
            break;
        }
        //${SMs::TstSM::SM::s::E}
        case E_SIG: {
            BSP_display("s-E;");
            status_ = Q_TRAN(&TstSM_s21);
            break;
        }
        //${SMs::TstSM::SM::s::I}
        case I_SIG: {
            BSP_display("s-I;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::C}
        case C_SIG: {
            BSP_display("s-C;");
            status_ = Q_TRAN(&TstSM_t);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

//${SMs::TstSM::SM::s::s1} ...................................................
static QState TstSM_s1(TstSM * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::TstSM::SM::s::s1}
        case Q_ENTRY_SIG: {
            BSP_display("s1-ENTRY;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s1}
        case Q_EXIT_SIG: {
            BSP_display("s1-EXIT;");
            // save shallow history
            me->hist_s1 =
                QHsm_childState(Q_HSM_UPCAST(me),
                                Q_STATE_CAST(&TstSM_s1));
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s1::initial}
        case Q_INIT_SIG: {
            BSP_display("s1-INIT;");
            status_ = Q_TRAN(&TstSM_s111);
            break;
        }
        //${SMs::TstSM::SM::s::s1::B}
        case B_SIG: {
            BSP_display("s1-B;");
            status_ = Q_TRAN(&TstSM_s12);
            break;
        }
        //${SMs::TstSM::SM::s::s1::F}
        case F_SIG: {
            BSP_display("s1-F;");
            status_ = Q_TRAN(&TstSM_s21);
            break;
        }
        //${SMs::TstSM::SM::s::s1::I}
        case I_SIG: {
            //${SMs::TstSM::SM::s::s1::I::[me->foo]}
            if (me->foo) {
                me->foo = 0U;
                BSP_display("s1-I;");
                status_ = Q_HANDLED();
            }
            else {
                status_ = Q_UNHANDLED();
            }
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM_s);
            break;
        }
    }
    return status_;
}

//${SMs::TstSM::SM::s::s1::s11} ..............................................
static QState TstSM_s11(TstSM * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::TstSM::SM::s::s1::s11}
        case Q_ENTRY_SIG: {
            BSP_display("s11-ENTRY;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s1::s11}
        case Q_EXIT_SIG: {
            BSP_display("s11-EXIT;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s1::s11::initial}
        case Q_INIT_SIG: {
            BSP_display("s11-INIT;");
            status_ = Q_TRAN(&TstSM_s111);
            break;
        }
        //${SMs::TstSM::SM::s::s1::s11::A}
        case A_SIG: {
            BSP_display("s11-A;");
            status_ = Q_TRAN(&TstSM_s11);
            break;
        }
        //${SMs::TstSM::SM::s::s1::s11::D}
        case D_SIG: {
            //${SMs::TstSM::SM::s::s1::s11::D::[!me->foo]}
            if (!me->foo) {
                me->foo = 1U;
                BSP_display("s11-D;");
                status_ = Q_TRAN(&TstSM_s1);
            }
            else {
                status_ = Q_UNHANDLED();
            }
            break;
        }
        //${SMs::TstSM::SM::s::s1::s11::G}
        case G_SIG: {
            BSP_display("s11-G->H*-s2;");
            status_ = Q_TRAN_HIST(me->hist_s2);
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM_s1);
            break;
        }
    }
    return status_;
}

//${SMs::TstSM::SM::s::s1::s11::s111} ........................................
static QState TstSM_s111(TstSM * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::TstSM::SM::s::s1::s11::s111}
        case Q_ENTRY_SIG: {
            BSP_display("s111-ENTRY;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s1::s11::s111}
        case Q_EXIT_SIG: {
            BSP_display("s111-EXIT;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s1::s11::s111::C}
        case C_SIG: {
            BSP_display("s111-C;");
            status_ = Q_TRAN(&TstSM_s112);
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM_s11);
            break;
        }
    }
    return status_;
}

//${SMs::TstSM::SM::s::s1::s11::s112} ........................................
static QState TstSM_s112(TstSM * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::TstSM::SM::s::s1::s11::s112}
        case Q_ENTRY_SIG: {
            BSP_display("s112-ENTRY;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s1::s11::s112}
        case Q_EXIT_SIG: {
            BSP_display("s112-EXIT;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s1::s11::s112::H}
        case H_SIG: {
            BSP_display("s112-H;");
            status_ = Q_TRAN(&TstSM_s);
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM_s11);
            break;
        }
    }
    return status_;
}

//${SMs::TstSM::SM::s::s1::s12} ..............................................
static QState TstSM_s12(TstSM * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::TstSM::SM::s::s1::s12}
        case Q_ENTRY_SIG: {
            BSP_display("s12-ENTRY;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s1::s12}
        case Q_EXIT_SIG: {
            BSP_display("s12-EXIT;");
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM_s1);
            break;
        }
    }
    return status_;
}

//${SMs::TstSM::SM::s::s2} ...................................................
static QState TstSM_s2(TstSM * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::TstSM::SM::s::s2}
        case Q_ENTRY_SIG: {
            BSP_display("s2-ENTRY;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s2}
        case Q_EXIT_SIG: {
            BSP_display("s2-EXIT;");
            // save deep history
            me->hist_s2 = QHsm_state(Q_HSM_UPCAST(me));
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s2::initial}
        case Q_INIT_SIG: {
            BSP_display("s2-INIT;");
            status_ = Q_TRAN(&TstSM_s211);
            break;
        }
        //${SMs::TstSM::SM::s::s2::B}
        case B_SIG: {
            BSP_display("s2-B;");
            status_ = Q_TRAN(&TstSM_s22);
            break;
        }
        //${SMs::TstSM::SM::s::s2::F}
        case F_SIG: {
            BSP_display("s2-F;");
            status_ = Q_TRAN(&TstSM_s11);
            break;
        }
        //${SMs::TstSM::SM::s::s2::I}
        case I_SIG: {
            //${SMs::TstSM::SM::s::s2::I::[me->foo]}
            if (me->foo) {
                me->foo = 0U;
                BSP_display("s2-I;");
                status_ = Q_HANDLED();
            }
            else {
                status_ = Q_UNHANDLED();
            }
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM_s);
            break;
        }
    }
    return status_;
}

//${SMs::TstSM::SM::s::s2::s21} ..............................................
static QState TstSM_s21(TstSM * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::TstSM::SM::s::s2::s21}
        case Q_ENTRY_SIG: {
            BSP_display("s21-ENTRY;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s2::s21}
        case Q_EXIT_SIG: {
            BSP_display("s21-EXIT;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s2::s21::initial}
        case Q_INIT_SIG: {
            BSP_display("s21-INIT;");
            status_ = Q_TRAN(&TstSM_s211);
            break;
        }
        //${SMs::TstSM::SM::s::s2::s21::A}
        case A_SIG: {
            BSP_display("s21-A;");
            status_ = Q_TRAN(&TstSM_s21);
            break;
        }
        //${SMs::TstSM::SM::s::s2::s21::D}
        case D_SIG: {
            //${SMs::TstSM::SM::s::s2::s21::D::[!me->foo]}
            if (!me->foo) {
                me->foo = 1U;
                BSP_display("s21-D;");
                status_ = Q_TRAN(&TstSM_s2);
            }
            else {
                status_ = Q_UNHANDLED();
            }
            break;
        }
        //${SMs::TstSM::SM::s::s2::s21::G}
        case G_SIG: {
            BSP_display("s21-G->H*-s1;");
            status_ = Q_TRAN_HIST(me->hist_s1);
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM_s2);
            break;
        }
    }
    return status_;
}

//${SMs::TstSM::SM::s::s2::s21::s211} ........................................
static QState TstSM_s211(TstSM * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::TstSM::SM::s::s2::s21::s211}
        case Q_ENTRY_SIG: {
            BSP_display("s211-ENTRY;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s2::s21::s211}
        case Q_EXIT_SIG: {
            BSP_display("s211-EXIT;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s2::s21::s211::C}
        case C_SIG: {
            BSP_display("s211-C;");
            status_ = Q_TRAN(&TstSM_s212);
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM_s21);
            break;
        }
    }
    return status_;
}

//${SMs::TstSM::SM::s::s2::s21::s212} ........................................
static QState TstSM_s212(TstSM * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::TstSM::SM::s::s2::s21::s212}
        case Q_ENTRY_SIG: {
            BSP_display("s212-ENTRY;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s2::s21::s212}
        case Q_EXIT_SIG: {
            BSP_display("s212-EXIT;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s2::s21::s212::H}
        case H_SIG: {
            BSP_display("s212-H;");
            status_ = Q_TRAN(&TstSM_s);
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM_s21);
            break;
        }
    }
    return status_;
}

//${SMs::TstSM::SM::s::s2::s22} ..............................................
static QState TstSM_s22(TstSM * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::TstSM::SM::s::s2::s22}
        case Q_ENTRY_SIG: {
            BSP_display("s22-ENTRY;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::s::s2::s22}
        case Q_EXIT_SIG: {
            BSP_display("s22-EXIT;");
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM_s2);
            break;
        }
    }
    return status_;
}

//${SMs::TstSM::SM::t} .......................................................
static QState TstSM_t(TstSM * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::TstSM::SM::t}
        case Q_ENTRY_SIG: {
            BSP_display("t-ENTRY;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::t}
        case Q_EXIT_SIG: {
            BSP_display("t-EXIT;");
            status_ = Q_HANDLED();
            break;
        }
        //${SMs::TstSM::SM::t::C}
        case C_SIG: {
            BSP_display("t-C->H-s;");
            status_ = Q_TRAN_HIST(me->hist_s);
            break;
        }
        //${SMs::TstSM::SM::t::E}
        case E_SIG: {
            BSP_display("t-E->H*-s2;");
            status_ = Q_TRAN_HIST(me->hist_s2);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
//$enddef${SMs} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
