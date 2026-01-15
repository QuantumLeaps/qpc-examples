//============================================================================
// qhsmtst example
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
#include "qpc.h"
#include "bsp.h"
#include "qhsmtst.h"

typedef struct {
    // protected:
    QActive super; // inherit QActive
} QHsmTst;

// protected:
static QState QHsmTst_initial(QHsmTst* const me, void const* const par);
static QState QHsmTst_s1(QHsmTst* const me, QEvt const* const e);
static QState QHsmTst_s2(QHsmTst* const me, QEvt const* const e);
static QState QHsmTst_s3(QHsmTst* const me, QEvt const* const e);
static QState QHsmTst_s4(QHsmTst* const me, QEvt const* const e);
static QState QHsmTst_s5(QHsmTst* const me, QEvt const* const e);
static QState QHsmTst_s6(QHsmTst* const me, QEvt const* const e);
static QState QHsmTst_s7(QHsmTst* const me, QEvt const* const e);
static QState QHsmTst_s8(QHsmTst* const me, QEvt const* const e);
static QState QHsmTst_t(QHsmTst* const me, QEvt const* const e);

//............................................................................
static QHsmTst QHsmTst_inst;
QActive * const AO_QHsmTst = &QHsmTst_inst.super;

//............................................................................
void QHsmTst_ctor(void) {
    QHsmTst* me = &QHsmTst_inst;
    QActive_ctor(&me->super, Q_STATE_CAST(&QHsmTst_initial));
}
//............................................................................
static QState QHsmTst_initial(QHsmTst* const me, void const* const par) {
    Q_UNUSED_PAR(me);
    Q_UNUSED_PAR(par);
    QS_OBJ_DICTIONARY(&QHsmTst_inst);

    QS_FUN_DICTIONARY(&QHsmTst_s1);
    QS_FUN_DICTIONARY(&QHsmTst_s2);
    QS_FUN_DICTIONARY(&QHsmTst_s3);
    QS_FUN_DICTIONARY(&QHsmTst_s4);
    QS_FUN_DICTIONARY(&QHsmTst_s5);
    QS_FUN_DICTIONARY(&QHsmTst_s6);
    QS_FUN_DICTIONARY(&QHsmTst_s7);
    QS_FUN_DICTIONARY(&QHsmTst_s8);
    QS_FUN_DICTIONARY(&QHsmTst_t);

    return Q_TRAN(&QHsmTst_t);
}
//............................................................................
static QState QHsmTst_s1(QHsmTst* const me, QEvt const* const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&QHsmTst_s7);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState QHsmTst_s2(QHsmTst* const me, QEvt const* const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsmTst_s1);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState QHsmTst_s3(QHsmTst* const me, QEvt const* const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsmTst_s2);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState QHsmTst_s4(QHsmTst* const me, QEvt const* const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&QHsmTst_s7);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsmTst_s3);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState QHsmTst_s5(QHsmTst* const me, QEvt const* const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsmTst_s4);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState QHsmTst_s6(QHsmTst* const me, QEvt const* const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsmTst_s5);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState QHsmTst_s7(QHsmTst* const me, QEvt const* const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case I_SIG: {
            QASM_DISPATCH(&me->super.super, e, 0U);
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsmTst_s6);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState QHsmTst_s8(QHsmTst* const me, QEvt const* const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsmTst_s7);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState QHsmTst_t(QHsmTst* const me, QEvt const* const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case A_SIG: {
            status_ = Q_TRAN(&QHsmTst_s5);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
