//$file${.::cut_qmsm.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: qmsm.qm
// File:  ${.::cut_qmsm.c}
//
// This code has been generated by QM 6.2.2 <www.state-machine.com/qm>.
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
//$endhead${.::cut_qmsm.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include "qpc.h"       // QP/C framework
#include "cut_qmsm.h"  // Code Under Test (CUT)

Q_DEFINE_THIS_FILE

//$declare${SMs::QMsmTst} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${SMs::QMsmTst} ............................................................
typedef struct QMsmTst {
// protected:
    QMsm super;

// private:
    uint8_t foo;

// public:

// private submachines
    // exit points for submachine ${SMs::QMsmTst::SM::sub}
    struct SM_sub {
        QMState super;
        QActionHandler const xpH; // eXit-Point segment
        QActionHandler const xpG; // eXit-Point segment
    } const *sub_sub;

// private state histories
    QMState const *hist_s;
    QMState const *hist_s1;
    QMState const *hist_s2;
} QMsmTst;

extern QMsmTst QMsmTst_inst;

// protected:
static QState QMsmTst_initial(QMsmTst * const me, void const * const par);
static QState QMsmTst_s  (QMsmTst * const me, QEvt const * const e);
static QState QMsmTst_s_e(QMsmTst * const me);
static QState QMsmTst_s_x(QMsmTst * const me);
static QState QMsmTst_s_i(QMsmTst * const me);
static QMState const QMsmTst_s_s = {
    QM_STATE_NULL, // superstate (top)
    Q_STATE_CAST(&QMsmTst_s),
    Q_ACTION_CAST(&QMsmTst_s_e),
    Q_ACTION_CAST(&QMsmTst_s_x),
    Q_ACTION_CAST(&QMsmTst_s_i)
};
static QState QMsmTst_s1  (QMsmTst * const me, QEvt const * const e);
static QState QMsmTst_s1_e(QMsmTst * const me);
static QState QMsmTst_s1_xpH(QMsmTst * const me);
static QState QMsmTst_s1_xpG(QMsmTst * const me);
static QState QMsmTst_sub_i(QMsmTst * const me); // forward decl.
static struct SM_sub const QMsmTst_s1_s = {
    {
        &QMsmTst_s_s, // superstate
        Q_STATE_CAST(&QMsmTst_s1),
        Q_ACTION_CAST(&QMsmTst_s1_e),
        Q_ACTION_NULL, // no exit action
        Q_ACTION_CAST(&QMsmTst_sub_i) // reuse from submachine
    }
    ,Q_ACTION_CAST(&QMsmTst_s1_xpH)
    ,Q_ACTION_CAST(&QMsmTst_s1_xpG)};
static QState QMsmTst_s2  (QMsmTst * const me, QEvt const * const e);
static QState QMsmTst_s2_e(QMsmTst * const me);
static QState QMsmTst_s2_xpH(QMsmTst * const me);
static QState QMsmTst_s2_xpG(QMsmTst * const me);
static QState QMsmTst_sub_i(QMsmTst * const me); // forward decl.
static struct SM_sub const QMsmTst_s2_s = {
    {
        &QMsmTst_s_s, // superstate
        Q_STATE_CAST(&QMsmTst_s2),
        Q_ACTION_CAST(&QMsmTst_s2_e),
        Q_ACTION_NULL, // no exit action
        Q_ACTION_CAST(&QMsmTst_sub_i) // reuse from submachine
    }
    ,Q_ACTION_CAST(&QMsmTst_s2_xpH)
    ,Q_ACTION_CAST(&QMsmTst_s2_xpG)};
static QState QMsmTst_t  (QMsmTst * const me, QEvt const * const e);
static QState QMsmTst_t_e(QMsmTst * const me);
static QState QMsmTst_t_x(QMsmTst * const me);
static QMState const QMsmTst_t_s = {
    QM_STATE_NULL, // superstate (top)
    Q_STATE_CAST(&QMsmTst_t),
    Q_ACTION_CAST(&QMsmTst_t_e),
    Q_ACTION_CAST(&QMsmTst_t_x),
    Q_ACTION_NULL  // no initial tran.
};

// submachine ${SMs::QMsmTst::SM::sub}
static QState QMsmTst_sub  (QMsmTst * const me, QEvt const * const e);
static QState QMsmTst_sub_e(QMsmTst * const me);
static QState QMsmTst_sub_x(QMsmTst * const me);
static QState QMsmTst_sub_i(QMsmTst * const me);
static QMState const QMsmTst_sub_s = {
    QM_STATE_NULL, // superstate unused
    Q_STATE_CAST(&QMsmTst_sub),
    Q_ACTION_CAST(&QMsmTst_sub_e),
    Q_ACTION_CAST(&QMsmTst_sub_x),
    Q_ACTION_CAST(&QMsmTst_sub_i)
};
static QState QMsmTst_sub_ep1_ep(QMsmTst * const me);
static QState QMsmTst_sub_sub1  (QMsmTst * const me, QEvt const * const e);
static QState QMsmTst_sub_sub1_e(QMsmTst * const me);
static QState QMsmTst_sub_sub1_x(QMsmTst * const me);
static QState QMsmTst_sub_sub1_i(QMsmTst * const me);
static QMState const QMsmTst_sub_sub1_s = {
    &QMsmTst_sub_s, // superstate
    Q_STATE_CAST(&QMsmTst_sub_sub1),
    Q_ACTION_CAST(&QMsmTst_sub_sub1_e),
    Q_ACTION_CAST(&QMsmTst_sub_sub1_x),
    Q_ACTION_CAST(&QMsmTst_sub_sub1_i)
};
static QState QMsmTst_sub_sub11  (QMsmTst * const me, QEvt const * const e);
static QState QMsmTst_sub_sub11_e(QMsmTst * const me);
static QState QMsmTst_sub_sub11_x(QMsmTst * const me);
static QMState const QMsmTst_sub_sub11_s = {
    &QMsmTst_sub_sub1_s, // superstate
    Q_STATE_CAST(&QMsmTst_sub_sub11),
    Q_ACTION_CAST(&QMsmTst_sub_sub11_e),
    Q_ACTION_CAST(&QMsmTst_sub_sub11_x),
    Q_ACTION_NULL  // no initial tran.
};
static QState QMsmTst_sub_sub12  (QMsmTst * const me, QEvt const * const e);
static QState QMsmTst_sub_sub12_e(QMsmTst * const me);
static QState QMsmTst_sub_sub12_x(QMsmTst * const me);
static QMState const QMsmTst_sub_sub12_s = {
    &QMsmTst_sub_sub1_s, // superstate
    Q_STATE_CAST(&QMsmTst_sub_sub12),
    Q_ACTION_CAST(&QMsmTst_sub_sub12_e),
    Q_ACTION_CAST(&QMsmTst_sub_sub12_x),
    Q_ACTION_NULL  // no initial tran.
};
static QState QMsmTst_sub_sub2  (QMsmTst * const me, QEvt const * const e);
static QState QMsmTst_sub_sub2_e(QMsmTst * const me);
static QState QMsmTst_sub_sub2_x(QMsmTst * const me);
static QMState const QMsmTst_sub_sub2_s = {
    &QMsmTst_sub_s, // superstate
    Q_STATE_CAST(&QMsmTst_sub_sub2),
    Q_ACTION_CAST(&QMsmTst_sub_sub2_e),
    Q_ACTION_CAST(&QMsmTst_sub_sub2_x),
    Q_ACTION_NULL  // no initial tran.
};
//$enddecl${SMs::QMsmTst} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U)%0x2710U))
#error qpc version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Shared::the_sm} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::the_sm} ..........................................................
QAsm * const the_sm = &QMsmTst_inst.super.super;
//$enddef${Shared::the_sm} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Shared::QMsmTst_ctor} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::QMsmTst_ctor} ....................................................
void QMsmTst_ctor(void) {
    QMsmTst *me = &QMsmTst_inst;
    QMsm_ctor(&me->super, Q_STATE_CAST(&QMsmTst_initial));
}
//$enddef${Shared::QMsmTst_ctor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Shared::QMsmTst_isIn} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::QMsmTst_isIn} ....................................................
bool QMsmTst_isIn(uint32_t const state_num) {
    QMsm * const me = &QMsmTst_inst.super;
    bool stat = false;
    switch (state_num) {
    case 0:
        stat = QASM_IS_IN(me, Q_STATE_CAST(&QMsmTst_s));
        break;
    case 1:
        stat = QASM_IS_IN(me, Q_STATE_CAST(&QMsmTst_s1));
        break;
    case 2:
        stat = QASM_IS_IN(me, Q_STATE_CAST(&QMsmTst_s2));
        break;
    case 11:
        stat = QASM_IS_IN(me, Q_STATE_CAST(&QMsmTst_sub_sub1));
        break;
    case 111:
        stat = QASM_IS_IN(me, Q_STATE_CAST(&QMsmTst_sub_sub11));
        break;
    case 112:
        stat = QASM_IS_IN(me, Q_STATE_CAST(&QMsmTst_sub_sub12));
        break;
    case 12:
        stat = QASM_IS_IN(me, Q_STATE_CAST(&QMsmTst_sub_sub2));
        break;
    default:
        Q_ERROR();
    }
    return stat;
}
//$enddef${Shared::QMsmTst_isIn} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${SMs::QMsmTst} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${SMs::QMsmTst} ............................................................
QMsmTst QMsmTst_inst;

//${SMs::QMsmTst::SM} ........................................................
static QState QMsmTst_initial(QMsmTst * const me, void const * const par) {
    //${SMs::QMsmTst::SM::initial}
    Q_UNUSED_PAR(par);
    me->foo = 0U;
    BSP_display("top-INIT;");

    QS_SIG_DICTIONARY(A_SIG, (void *)0);
    QS_SIG_DICTIONARY(B_SIG, (void *)0);
    QS_SIG_DICTIONARY(C_SIG, (void *)0);
    QS_SIG_DICTIONARY(D_SIG, (void *)0);
    QS_SIG_DICTIONARY(E_SIG, (void *)0);
    QS_SIG_DICTIONARY(F_SIG, (void *)0);
    QS_SIG_DICTIONARY(G_SIG, (void *)0);
    QS_SIG_DICTIONARY(H_SIG, (void *)0);
    QS_SIG_DICTIONARY(I_SIG, (void *)0);
    // state history attributes
    me->hist_s = &QMsmTst_s1_s.super;
    me->hist_s1 = &QMsmTst_sub_sub11_s;
    me->hist_s2 = &QMsmTst_sub_sub11_s;

    QS_FUN_DICTIONARY(&QMsmTst_s);
    QS_FUN_DICTIONARY(&QMsmTst_s1);
    QS_FUN_DICTIONARY(&QMsmTst_s2);
    QS_FUN_DICTIONARY(&QMsmTst_t);
    QS_FUN_DICTIONARY(&QMsmTst_sub_sub1);
    QS_FUN_DICTIONARY(&QMsmTst_sub_sub11);
    QS_FUN_DICTIONARY(&QMsmTst_sub_sub12);
    QS_FUN_DICTIONARY(&QMsmTst_sub_sub2);
    QS_FUN_DICTIONARY(&QMsmTst_sub);

    static struct {
        QMState const *target;
        QActionHandler act[4];
    } const tatbl_ = { // tran-action table
        &QMsmTst_sub_s, // target submachine
        {
            Q_ACTION_CAST(&QMsmTst_s_e), // entry
            Q_ACTION_CAST(&QMsmTst_s2_e), // entry
            Q_ACTION_CAST(&QMsmTst_sub_i), // initial tran.
            Q_ACTION_NULL // zero terminator
        }
    };
    return QM_TRAN_INIT(&tatbl_);
}

//${SMs::QMsmTst::SM::s} .....................................................
//${SMs::QMsmTst::SM::s}
static QState QMsmTst_s_e(QMsmTst * const me) {
    BSP_display("s-ENTRY;");
    Q_UNUSED_PAR(me);
    return QM_ENTRY(&QMsmTst_s_s);
}
//${SMs::QMsmTst::SM::s}
static QState QMsmTst_s_x(QMsmTst * const me) {
    BSP_display("s-EXIT;");
    // save shallow history
    me->hist_s =
        QMsm_childStateObj(Q_MSM_UPCAST(me), &QMsmTst_s_s);
    return QM_EXIT(&QMsmTst_s_s);
}
//${SMs::QMsmTst::SM::s::initial}
static QState QMsmTst_s_i(QMsmTst * const me) {
    //${SMs::QMsmTst::SM::s::initial}
    BSP_display("s-INIT;");
    static struct {
        QMState const *target;
        QActionHandler act[3];
    } const tatbl_ = { // tran-action table
        &QMsmTst_sub_s, // target submachine
        {
            Q_ACTION_CAST(&QMsmTst_s1_e), // entry
            Q_ACTION_CAST(&QMsmTst_sub_i), // initial tran.
            Q_ACTION_NULL // zero terminator
        }
    };
    return QM_TRAN_INIT(&tatbl_);
}
//${SMs::QMsmTst::SM::s}
static QState QMsmTst_s(QMsmTst * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::QMsmTst::SM::s::E}
        case E_SIG: {
            BSP_display("s-E;");
            static struct {
                QMState const *target;
                QActionHandler act[3];
            } const tatbl_ = { // tran-action table
                &QMsmTst_sub_s, // target submachine
                {
                    Q_ACTION_CAST(&QMsmTst_s1_e), // entry
                    Q_ACTION_CAST(&QMsmTst_sub_ep1_ep), // EP
                    Q_ACTION_NULL // zero terminator
                }
            };
            status_ = QM_TRAN_EP(&tatbl_);
            break;
        }
        //${SMs::QMsmTst::SM::s::I}
        case I_SIG: {
            BSP_display("s-I;");
            status_ = QM_HANDLED();
            break;
        }
        //${SMs::QMsmTst::SM::s::C}
        case C_SIG: {
            BSP_display("s-C;");
            static struct {
                QMState const *target;
                QActionHandler act[3];
            } const tatbl_ = { // tran-action table
                &QMsmTst_t_s, // target state
                {
                    Q_ACTION_CAST(&QMsmTst_s_x), // exit
                    Q_ACTION_CAST(&QMsmTst_t_e), // entry
                    Q_ACTION_NULL // zero terminator
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}

//${SMs::QMsmTst::SM::s::s1} .................................................
//${SMs::QMsmTst::SM::s::s1}
static QState QMsmTst_s1_e(QMsmTst * const me) {
    me->sub_sub = &QMsmTst_s1_s; // attach submachine
    return QMsmTst_sub_e(me); // enter submachine
}
//${SMs::QMsmTst::SM::s::s1}
static QState QMsmTst_s1_xpG(QMsmTst * const me) {
    static struct {
        QMState const *target;
        QActionHandler act[3];
    } const tatbl_ = { // tran-action table
        &QMsmTst_sub_s, // target submachine
        {
            Q_ACTION_CAST(&QMsmTst_sub_x), // submachine exit
            Q_ACTION_CAST(&QMsmTst_s2_e), // entry
            Q_ACTION_NULL // zero terminator
        }
    };
    return QM_TRAN_HIST(me->hist_s2, &tatbl_);
}
//${SMs::QMsmTst::SM::s::s1}
static QState QMsmTst_s1_xpH(QMsmTst * const me) {
    static struct {
        QMState const *target;
        QActionHandler act[3];
    } const tatbl_ = { // tran-action table
        &QMsmTst_s_s, // target state
        {
            Q_ACTION_CAST(&QMsmTst_sub_x), // submachine exit
            Q_ACTION_CAST(&QMsmTst_s_i), // initial tran.
            Q_ACTION_NULL // zero terminator
        }
    };
    Q_UNUSED_PAR(me);
    return QM_TRAN(&tatbl_);
}
//${SMs::QMsmTst::SM::s::s1}
static QState QMsmTst_s1(QMsmTst * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::QMsmTst::SM::s::s1::F}
        case F_SIG: {
            BSP_display("s1-F;");
            static struct {
                QMState const *target;
                QActionHandler act[3];
            } const tatbl_ = { // tran-action table
                &QMsmTst_sub_s, // target submachine
                {
                    Q_ACTION_CAST(&QMsmTst_s2_e), // entry
                    Q_ACTION_CAST(&QMsmTst_sub_ep1_ep), // EP
                    Q_ACTION_NULL // zero terminator
                }
            };
            status_ = QM_TRAN_EP(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}

//${SMs::QMsmTst::SM::s::s2} .................................................
//${SMs::QMsmTst::SM::s::s2}
static QState QMsmTst_s2_e(QMsmTst * const me) {
    me->sub_sub = &QMsmTst_s2_s; // attach submachine
    return QMsmTst_sub_e(me); // enter submachine
}
//${SMs::QMsmTst::SM::s::s2}
static QState QMsmTst_s2_xpH(QMsmTst * const me) {
    static struct {
        QMState const *target;
        QActionHandler act[3];
    } const tatbl_ = { // tran-action table
        &QMsmTst_s_s, // target state
        {
            Q_ACTION_CAST(&QMsmTst_sub_x), // submachine exit
            Q_ACTION_CAST(&QMsmTst_s_i), // initial tran.
            Q_ACTION_NULL // zero terminator
        }
    };
    Q_UNUSED_PAR(me);
    return QM_TRAN(&tatbl_);
}
//${SMs::QMsmTst::SM::s::s2}
static QState QMsmTst_s2_xpG(QMsmTst * const me) {
    static struct {
        QMState const *target;
        QActionHandler act[3];
    } const tatbl_ = { // tran-action table
        &QMsmTst_sub_s, // target submachine
        {
            Q_ACTION_CAST(&QMsmTst_sub_x), // submachine exit
            Q_ACTION_CAST(&QMsmTst_s1_e), // entry
            Q_ACTION_NULL // zero terminator
        }
    };
    return QM_TRAN_HIST(me->hist_s1, &tatbl_);
}
//${SMs::QMsmTst::SM::s::s2}
static QState QMsmTst_s2(QMsmTst * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::QMsmTst::SM::s::s2::F}
        case F_SIG: {
            BSP_display("s2-F;");
            static struct {
                QMState const *target;
                QActionHandler act[3];
            } const tatbl_ = { // tran-action table
                &QMsmTst_sub_s, // target submachine
                {
                    Q_ACTION_CAST(&QMsmTst_s1_e), // entry
                    Q_ACTION_CAST(&QMsmTst_sub_ep1_ep), // EP
                    Q_ACTION_NULL // zero terminator
                }
            };
            status_ = QM_TRAN_EP(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}

//${SMs::QMsmTst::SM::t} .....................................................
//${SMs::QMsmTst::SM::t}
static QState QMsmTst_t_e(QMsmTst * const me) {
    BSP_display("t-ENTRY;");
    Q_UNUSED_PAR(me);
    return QM_ENTRY(&QMsmTst_t_s);
}
//${SMs::QMsmTst::SM::t}
static QState QMsmTst_t_x(QMsmTst * const me) {
    BSP_display("t-EXIT;");
    (void)me; // unused parameter
    return QM_EXIT(&QMsmTst_t_s);
}
//${SMs::QMsmTst::SM::t}
static QState QMsmTst_t(QMsmTst * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::QMsmTst::SM::t::C}
        case C_SIG: {
            BSP_display("t-C->H-s;");
            static struct {
                QMState const *target;
                QActionHandler act[3];
            } const tatbl_ = { // tran-action table
                &QMsmTst_s_s, // target state
                {
                    Q_ACTION_CAST(&QMsmTst_t_x), // exit
                    Q_ACTION_CAST(&QMsmTst_s_e), // entry
                    Q_ACTION_NULL // zero terminator
                }
            };
            status_ = QM_TRAN_HIST(me->hist_s, &tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}

//${SMs::QMsmTst::SM::sub} ...................................................
//${SMs::QMsmTst::SM::sub}
static QState QMsmTst_sub_e(QMsmTst * const me) {
    BSP_display("sub-ENTRY;");
    Q_UNUSED_PAR(me);
    return QM_ENTRY(&QMsmTst_sub_s);
}
//${SMs::QMsmTst::SM::sub}
static QState QMsmTst_sub_x(QMsmTst * const me) {
    BSP_display("sub-EXIT;");
    return QM_SM_EXIT(&me->sub_sub->super);
}
//${SMs::QMsmTst::SM::sub::initial}
static QState QMsmTst_sub_i(QMsmTst * const me) {
    //${SMs::QMsmTst::SM::sub::initial}
    BSP_display("sub-INIT;");
    static struct {
        QMState const *target;
        QActionHandler act[3];
    } const tatbl_ = { // tran-action table
        &QMsmTst_sub_sub11_s, // target state
        {
            Q_ACTION_CAST(&QMsmTst_sub_sub1_e), // entry
            Q_ACTION_CAST(&QMsmTst_sub_sub11_e), // entry
            Q_ACTION_NULL // zero terminator
        }
    };
    return QM_TRAN_INIT(&tatbl_);
}
//${SMs::QMsmTst::SM::sub}
static QState QMsmTst_sub(QMsmTst * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::QMsmTst::SM::sub::E}
        case E_SIG: {
            BSP_display("sub-E;");
            static struct {
                QMState const *target;
                QActionHandler act[2];
            } const tatbl_ = { // tran-action table
                &QMsmTst_sub_sub2_s, // target state
                {
                    Q_ACTION_CAST(&QMsmTst_sub_sub2_e), // entry
                    Q_ACTION_NULL // zero terminator
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        //${SMs::QMsmTst::SM::sub::I}
        case I_SIG: {
            //${SMs::QMsmTst::SM::sub::I::[me->foo]}
            if (me->foo) {
                me->foo = 0U;
                BSP_display("sub-I;");
                status_ = QM_HANDLED();
            }
            else {
                status_ = QM_SUPER_SUB(&me->sub_sub->super);
            }
            break;
        }
        default: {
            status_ = QM_SUPER_SUB(&me->sub_sub->super);
            break;
        }
    }
    return status_;
}
//${SMs::QMsmTst::SM::sub::EP-ep1}
static QState QMsmTst_sub_ep1_ep(QMsmTst * const me) {
    BSP_display("sub-ep1;");
    static struct {
        QMState const *target;
        QActionHandler act[3];
    } const tatbl_ = { // tran-action table
        &QMsmTst_sub_sub1_s, // target state
        {
            Q_ACTION_CAST(&QMsmTst_sub_sub1_e), // entry
            Q_ACTION_CAST(&QMsmTst_sub_sub1_i), // initial tran.
            Q_ACTION_NULL // zero terminator
        }
    };
    return QM_TRAN_EP(&tatbl_);
}

//${SMs::QMsmTst::SM::sub::sub1} .............................................
//${SMs::QMsmTst::SM::sub::sub1}
static QState QMsmTst_sub_sub1_e(QMsmTst * const me) {
    BSP_display("sub1-ENTRY;");
    Q_UNUSED_PAR(me);
    return QM_ENTRY(&QMsmTst_sub_sub1_s);
}
//${SMs::QMsmTst::SM::sub::sub1}
static QState QMsmTst_sub_sub1_x(QMsmTst * const me) {
    BSP_display("sub1-EXIT;");
    (void)me; // unused parameter
    return QM_EXIT(&QMsmTst_sub_sub1_s);
}
//${SMs::QMsmTst::SM::sub::sub1::initial}
static QState QMsmTst_sub_sub1_i(QMsmTst * const me) {
    //${SMs::QMsmTst::SM::sub::sub1::initial}
    BSP_display("sub1-INIT;");
    static struct {
        QMState const *target;
        QActionHandler act[2];
    } const tatbl_ = { // tran-action table
        &QMsmTst_sub_sub11_s, // target state
        {
            Q_ACTION_CAST(&QMsmTst_sub_sub11_e), // entry
            Q_ACTION_NULL // zero terminator
        }
    };
    return QM_TRAN_INIT(&tatbl_);
}
//${SMs::QMsmTst::SM::sub::sub1}
static QState QMsmTst_sub_sub1(QMsmTst * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::QMsmTst::SM::sub::sub1::A}
        case A_SIG: {
            BSP_display("sub1-A;");
            static struct {
                QMState const *target;
                QActionHandler act[4];
            } const tatbl_ = { // tran-action table
                &QMsmTst_sub_sub1_s, // target state
                {
                    Q_ACTION_CAST(&QMsmTst_sub_sub1_x), // exit
                    Q_ACTION_CAST(&QMsmTst_sub_sub1_e), // entry
                    Q_ACTION_CAST(&QMsmTst_sub_sub1_i), // initial tran.
                    Q_ACTION_NULL // zero terminator
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        //${SMs::QMsmTst::SM::sub::sub1::B}
        case B_SIG: {
            BSP_display("sub1-B;");
            static struct {
                QMState const *target;
                QActionHandler act[2];
            } const tatbl_ = { // tran-action table
                &QMsmTst_sub_sub11_s, // target state
                {
                    Q_ACTION_CAST(&QMsmTst_sub_sub11_e), // entry
                    Q_ACTION_NULL // zero terminator
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        //${SMs::QMsmTst::SM::sub::sub1::D}
        case D_SIG: {
            //${SMs::QMsmTst::SM::sub::sub1::D::[!me->foo]}
            if (!me->foo) {
                me->foo = 1U;
                BSP_display("sub1-D;");
                static struct {
                    QMState const *target;
                    QActionHandler act[3];
                } const tatbl_ = { // tran-action table
                    &QMsmTst_sub_s, // target submachine
                    {
                        Q_ACTION_CAST(&QMsmTst_sub_sub1_x), // exit
                        Q_ACTION_CAST(&QMsmTst_sub_i), // initial tran.
                        Q_ACTION_NULL // zero terminator
                    }
                };
                status_ = QM_TRAN(&tatbl_);
            }
            else {
                status_ = QM_UNHANDLED();
            }
            break;
        }
        //${SMs::QMsmTst::SM::sub::sub1::G}
        case G_SIG: {
            BSP_display("sub1-G->xpG;");
            static struct {
                QMState const *target;
                QActionHandler act[2];
            } const tatbl_ = { // tran-action table
                &QMsmTst_sub_s, // target submachine
                {
                    Q_ACTION_CAST(&QMsmTst_sub_sub1_x), // exit
                    Q_ACTION_NULL // zero terminator
                }
            };
            status_ = QM_TRAN_XP(me->sub_sub->xpG, &tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}

//${SMs::QMsmTst::SM::sub::sub1::sub11} ......................................
//${SMs::QMsmTst::SM::sub::sub1::sub11}
static QState QMsmTst_sub_sub11_e(QMsmTst * const me) {
    BSP_display("sub11-ENTRY;");
    Q_UNUSED_PAR(me);
    return QM_ENTRY(&QMsmTst_sub_sub11_s);
}
//${SMs::QMsmTst::SM::sub::sub1::sub11}
static QState QMsmTst_sub_sub11_x(QMsmTst * const me) {
    BSP_display("sub11-EXIT;");
    (void)me; // unused parameter
    return QM_EXIT(&QMsmTst_sub_sub11_s);
}
//${SMs::QMsmTst::SM::sub::sub1::sub11}
static QState QMsmTst_sub_sub11(QMsmTst * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::QMsmTst::SM::sub::sub1::sub11::C}
        case C_SIG: {
            BSP_display("sub11-C;");
            static struct {
                QMState const *target;
                QActionHandler act[3];
            } const tatbl_ = { // tran-action table
                &QMsmTst_sub_sub12_s, // target state
                {
                    Q_ACTION_CAST(&QMsmTst_sub_sub11_x), // exit
                    Q_ACTION_CAST(&QMsmTst_sub_sub12_e), // entry
                    Q_ACTION_NULL // zero terminator
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}

//${SMs::QMsmTst::SM::sub::sub1::sub12} ......................................
//${SMs::QMsmTst::SM::sub::sub1::sub12}
static QState QMsmTst_sub_sub12_e(QMsmTst * const me) {
    BSP_display("sub12-ENTRY;");
    Q_UNUSED_PAR(me);
    return QM_ENTRY(&QMsmTst_sub_sub12_s);
}
//${SMs::QMsmTst::SM::sub::sub1::sub12}
static QState QMsmTst_sub_sub12_x(QMsmTst * const me) {
    BSP_display("sub12-EXIT;");
    (void)me; // unused parameter
    return QM_EXIT(&QMsmTst_sub_sub12_s);
}
//${SMs::QMsmTst::SM::sub::sub1::sub12}
static QState QMsmTst_sub_sub12(QMsmTst * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${SMs::QMsmTst::SM::sub::sub1::sub12::H}
        case H_SIG: {
            BSP_display("sub12-H->xpH;");
            static struct {
                QMState const *target;
                QActionHandler act[3];
            } const tatbl_ = { // tran-action table
                &QMsmTst_sub_s, // target submachine
                {
                    Q_ACTION_CAST(&QMsmTst_sub_sub12_x), // exit
                    Q_ACTION_CAST(&QMsmTst_sub_sub1_x), // exit
                    Q_ACTION_NULL // zero terminator
                }
            };
            status_ = QM_TRAN_XP(me->sub_sub->xpH, &tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}

//${SMs::QMsmTst::SM::sub::sub2} .............................................
//${SMs::QMsmTst::SM::sub::sub2}
static QState QMsmTst_sub_sub2_e(QMsmTst * const me) {
    BSP_display("sub2-ENTRY;");
    Q_UNUSED_PAR(me);
    return QM_ENTRY(&QMsmTst_sub_sub2_s);
}
//${SMs::QMsmTst::SM::sub::sub2}
static QState QMsmTst_sub_sub2_x(QMsmTst * const me) {
    BSP_display("sub2-EXIT;");
    (void)me; // unused parameter
    return QM_EXIT(&QMsmTst_sub_sub2_s);
}
//${SMs::QMsmTst::SM::sub::sub2}
static QState QMsmTst_sub_sub2(QMsmTst * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    Q_UNUSED_PAR(me);
    return status_;
}
//$enddef${SMs::QMsmTst} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
