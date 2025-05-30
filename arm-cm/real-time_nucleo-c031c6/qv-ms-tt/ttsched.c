//$file${.::ttsched.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: real-time-ms-tt.qm
// File:  ${.::ttsched.c}
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
//$endhead${.::ttsched.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include "qpc.h"               // QP/C real-time event framework
#include "bsp.h"               // Board Support Package
#include "app.h"               // Application interface

//$declare${Components::TTSched} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Components::TTSched} .....................................................
typedef struct TTSched {
// protected:
    QMsm super;

// public:
} TTSched;

extern TTSched TTSched_inst;

// protected:
static QState TTSched_initial(TTSched * const me, void const * const par);
static QState TTSched_active  (TTSched * const me, QEvt const * const e);
static QMState const TTSched_active_s = {
    QM_STATE_NULL, // superstate (top)
    Q_STATE_CAST(&TTSched_active),
    Q_ACTION_NULL, // no entry action
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
static QState TTSched_m0tick0  (TTSched * const me, QEvt const * const e);
static QMState const TTSched_m0tick0_s = {
    &TTSched_active_s, // superstate
    Q_STATE_CAST(&TTSched_m0tick0),
    Q_ACTION_NULL, // no entry action
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
static QState TTSched_m0tick1  (TTSched * const me, QEvt const * const e);
static QMState const TTSched_m0tick1_s = {
    &TTSched_active_s, // superstate
    Q_STATE_CAST(&TTSched_m0tick1),
    Q_ACTION_NULL, // no entry action
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
static QState TTSched_m0tick2  (TTSched * const me, QEvt const * const e);
static QMState const TTSched_m0tick2_s = {
    &TTSched_active_s, // superstate
    Q_STATE_CAST(&TTSched_m0tick2),
    Q_ACTION_NULL, // no entry action
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
static QState TTSched_m0tick3  (TTSched * const me, QEvt const * const e);
static QMState const TTSched_m0tick3_s = {
    &TTSched_active_s, // superstate
    Q_STATE_CAST(&TTSched_m0tick3),
    Q_ACTION_NULL, // no entry action
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
static QState TTSched_m1tick0  (TTSched * const me, QEvt const * const e);
static QMState const TTSched_m1tick0_s = {
    &TTSched_active_s, // superstate
    Q_STATE_CAST(&TTSched_m1tick0),
    Q_ACTION_NULL, // no entry action
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
static QState TTSched_m1tick1  (TTSched * const me, QEvt const * const e);
static QMState const TTSched_m1tick1_s = {
    &TTSched_active_s, // superstate
    Q_STATE_CAST(&TTSched_m1tick1),
    Q_ACTION_NULL, // no entry action
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
static QState TTSched_m1tick2  (TTSched * const me, QEvt const * const e);
static QMState const TTSched_m1tick2_s = {
    &TTSched_active_s, // superstate
    Q_STATE_CAST(&TTSched_m1tick2),
    Q_ACTION_NULL, // no entry action
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
static QState TTSched_m1tick3  (TTSched * const me, QEvt const * const e);
static QMState const TTSched_m1tick3_s = {
    &TTSched_active_s, // superstate
    Q_STATE_CAST(&TTSched_m1tick3),
    Q_ACTION_NULL, // no entry action
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
static QState TTSched_m1tick4  (TTSched * const me, QEvt const * const e);
static QMState const TTSched_m1tick4_s = {
    &TTSched_active_s, // superstate
    Q_STATE_CAST(&TTSched_m1tick4),
    Q_ACTION_NULL, // no entry action
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
static QState TTSched_m1tick5  (TTSched * const me, QEvt const * const e);
static QMState const TTSched_m1tick5_s = {
    &TTSched_active_s, // superstate
    Q_STATE_CAST(&TTSched_m1tick5),
    Q_ACTION_NULL, // no entry action
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
//$enddecl${Components::TTSched} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//Q_DEFINE_THIS_MODULE("ttsched")

static QEvt const runEvt = QEVT_INITIALIZER(RUN_SIG);

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U)%0x2710U))
#error qpc version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Shared::TTSched_ctor} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::TTSched_ctor} ....................................................
void TTSched_ctor(void) {
    TTSched * const me = &TTSched_inst;
    QMsm_ctor(&me->super, Q_STATE_CAST(&TTSched_initial));
    QASM_INIT((QAsm *)me, (void *)0, 0U);
}
//$enddef${Shared::TTSched_ctor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Shared::TTSched_tick} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::TTSched_tick} ....................................................
void TTSched_tick(void) {
    static QEvt const tickEvt = QEVT_INITIALIZER(TICK_SIG);
    QASM_DISPATCH((QAsm *)&TTSched_inst, &tickEvt, 0U);
}
//$enddef${Shared::TTSched_tick} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Shared::TTSched_mode} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Shared::TTSched_mode} ....................................................
void TTSched_mode(uint8_t mode) {
    static QEvt const modeEvt[2] = {
        QEVT_INITIALIZER(MODE0_SIG),
        QEVT_INITIALIZER(MODE1_SIG),
    };
    QASM_DISPATCH((QAsm *)&TTSched_inst, &modeEvt[mode], 0U);
}
//$enddef${Shared::TTSched_mode} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Components::TTSched} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Components::TTSched} .....................................................
TTSched TTSched_inst;

//${Components::TTSched::SM} .................................................
static QState TTSched_initial(TTSched * const me, void const * const par) {
    //${Components::TTSched::SM::initial}
    Q_UNUSED_PAR(par);
    static QMTranActTable const tatbl_ = { // tran-action table
        &TTSched_m0tick0_s, // target state
        {
            Q_ACTION_NULL // zero terminator
        }
    };
    return QM_TRAN_INIT(&tatbl_);
}

//${Components::TTSched::SM::active} .........................................
//${Components::TTSched::SM::active}
static QState TTSched_active(TTSched * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::TTSched::SM::active::MODE0}
        case MODE0_SIG: {
            static QMTranActTable const tatbl_ = { // tran-action table
                &TTSched_m0tick0_s, // target state
                {
                    Q_ACTION_NULL // zero terminator
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        //${Components::TTSched::SM::active::MODE1}
        case MODE1_SIG: {
            static QMTranActTable const tatbl_ = { // tran-action table
                &TTSched_m1tick0_s, // target state
                {
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

//${Components::TTSched::SM::active::m0tick0} ................................
//${Components::TTSched::SM::active::m0tick0}
static QState TTSched_m0tick0(TTSched * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::TTSched::SM::active::m0tick0::TICK}
        case TICK_SIG: {
            QACTIVE_POST(AO_Periodic4, &runEvt, 0U);
            static QMTranActTable const tatbl_ = { // tran-action table
                &TTSched_m0tick1_s, // target state
                {
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

//${Components::TTSched::SM::active::m0tick1} ................................
//${Components::TTSched::SM::active::m0tick1}
static QState TTSched_m0tick1(TTSched * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::TTSched::SM::active::m0tick1::TICK}
        case TICK_SIG: {
            static QMTranActTable const tatbl_ = { // tran-action table
                &TTSched_m0tick2_s, // target state
                {
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

//${Components::TTSched::SM::active::m0tick2} ................................
//${Components::TTSched::SM::active::m0tick2}
static QState TTSched_m0tick2(TTSched * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::TTSched::SM::active::m0tick2::TICK}
        case TICK_SIG: {
            QACTIVE_POST(AO_Periodic4, &runEvt, 0U);
            static QMTranActTable const tatbl_ = { // tran-action table
                &TTSched_m0tick3_s, // target state
                {
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

//${Components::TTSched::SM::active::m0tick3} ................................
//${Components::TTSched::SM::active::m0tick3}
static QState TTSched_m0tick3(TTSched * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::TTSched::SM::active::m0tick3::TICK}
        case TICK_SIG: {
            QACTIVE_POST(AO_Periodic1, &runEvt, 0U);
            static QMTranActTable const tatbl_ = { // tran-action table
                &TTSched_m0tick0_s, // target state
                {
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

//${Components::TTSched::SM::active::m1tick0} ................................
//${Components::TTSched::SM::active::m1tick0}
static QState TTSched_m1tick0(TTSched * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::TTSched::SM::active::m1tick0::TICK}
        case TICK_SIG: {
            QACTIVE_POST(AO_Periodic4, &runEvt, 0U);
            QACTIVE_POST(AO_Sporadic3, &runEvt, 0U);
            static QMTranActTable const tatbl_ = { // tran-action table
                &TTSched_m1tick1_s, // target state
                {
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

//${Components::TTSched::SM::active::m1tick1} ................................
//${Components::TTSched::SM::active::m1tick1}
static QState TTSched_m1tick1(TTSched * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::TTSched::SM::active::m1tick1::TICK}
        case TICK_SIG: {
            QACTIVE_POST(AO_Periodic4, &runEvt, 0U);
            QACTIVE_POST(AO_Sporadic3, &runEvt, 0U);
            static QMTranActTable const tatbl_ = { // tran-action table
                &TTSched_m1tick2_s, // target state
                {
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

//${Components::TTSched::SM::active::m1tick2} ................................
//${Components::TTSched::SM::active::m1tick2}
static QState TTSched_m1tick2(TTSched * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::TTSched::SM::active::m1tick2::TICK}
        case TICK_SIG: {
            QACTIVE_POST(AO_Periodic4, &runEvt, 0U);
            QACTIVE_POST(AO_Sporadic2, &runEvt, 0U);
            static QMTranActTable const tatbl_ = { // tran-action table
                &TTSched_m1tick3_s, // target state
                {
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

//${Components::TTSched::SM::active::m1tick3} ................................
//${Components::TTSched::SM::active::m1tick3}
static QState TTSched_m1tick3(TTSched * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::TTSched::SM::active::m1tick3::TICK}
        case TICK_SIG: {
            QACTIVE_POST(AO_Periodic4, &runEvt, 0U);
            QACTIVE_POST(AO_Sporadic3, &runEvt, 0U);
            static QMTranActTable const tatbl_ = { // tran-action table
                &TTSched_m1tick4_s, // target state
                {
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

//${Components::TTSched::SM::active::m1tick4} ................................
//${Components::TTSched::SM::active::m1tick4}
static QState TTSched_m1tick4(TTSched * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::TTSched::SM::active::m1tick4::TICK}
        case TICK_SIG: {
            QACTIVE_POST(AO_Periodic4, &runEvt, 0U);
            QACTIVE_POST(AO_Periodic1, &runEvt, 0U);
            static QMTranActTable const tatbl_ = { // tran-action table
                &TTSched_m1tick5_s, // target state
                {
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

//${Components::TTSched::SM::active::m1tick5} ................................
//${Components::TTSched::SM::active::m1tick5}
static QState TTSched_m1tick5(TTSched * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::TTSched::SM::active::m1tick5::TICK}
        case TICK_SIG: {
            QACTIVE_POST(AO_Periodic4, &runEvt, 0U);
            QACTIVE_POST(AO_Sporadic3, &runEvt, 0U);
            static QMTranActTable const tatbl_ = { // tran-action table
                &TTSched_m1tick0_s, // target state
                {
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
//$enddef${Components::TTSched} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
