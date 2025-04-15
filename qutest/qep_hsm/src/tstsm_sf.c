#include "qpc.h"          // QP/C framework
#include "tstsm.h"        // TstSM well-formed state machine
#include "tstsm_sf.h"     // Test state machines for Safety Functions

Q_DEFINE_THIS_MODULE("tstsm_sf")

//----------------------------------------------------------------------------
typedef struct {
// protected:
    QHsm super;
} TstSM0;

//............................................................................
static TstSM0 TstSM0_inst;
QAsm * const the_TstSM0 = &TstSM0_inst.super.super;

//............................................................................
void TstSM0_ctor(void) {
    TstSM0 *me = &TstSM0_inst;
    QHsm_ctor(&me->super, Q_STATE_CAST(0)); // NULL initial tran
}

//----------------------------------------------------------------------------
typedef struct {
// protected:
    QHsm super;
} TstSM1;

// protected:
static QState TstSM1_initial(TstSM1 * const me, void const * const par);

//............................................................................
static TstSM1 TstSM1_inst;
QAsm * const the_TstSM1 = &TstSM1_inst.super.super;

//............................................................................
void TstSM1_ctor(void) {
    TstSM1 *me = &TstSM1_inst;
    QHsm_ctor(&me->super, Q_STATE_CAST(&TstSM1_initial));
}
//............................................................................
static QState TstSM1_initial(TstSM1 * const me, void const * const par) {
    Q_UNUSED_PAR(me);
    Q_UNUSED_PAR(par);
    return Q_HANDLED(); // missing transition (should be Q_TRAN())
}

//----------------------------------------------------------------------------
typedef struct {
// protected:
    QHsm super;
} TstSM2;

// protected:
static QState TstSM2_initial(TstSM2 * const me, void const * const par);

//............................................................................
static TstSM2 TstSM2_inst;
QAsm * const the_TstSM2 = &TstSM2_inst.super.super;

//............................................................................
void TstSM2_ctor(void) {
    TstSM2 *me = &TstSM2_inst;
    QHsm_ctor(&me->super, Q_STATE_CAST(&TstSM2_initial));
}
//............................................................................
static QState TstSM2_initial(TstSM2 * const me, void const * const par) {
    Q_UNUSED_PAR(me);
    Q_UNUSED_PAR(par);
    return Q_TRAN(0); // NULL tran. target
}

//----------------------------------------------------------------------------
typedef struct {
// protected:
    QHsm super;
} TstSM3;

// protected:
static QState TstSM3_initial(TstSM3 * const me, void const * const par);

//............................................................................
static TstSM3 TstSM3_inst;
QAsm * const the_TstSM3 = &TstSM3_inst.super.super;

//............................................................................
void TstSM3_ctor(void) {
    TstSM3 *me = &TstSM3_inst;
    QHsm_ctor(&me->super, Q_STATE_CAST(&TstSM3_initial));
}
//............................................................................
static QState TstSM3_initial(TstSM3 * const me, void const * const par) {
    Q_UNUSED_PAR(me);
    Q_UNUSED_PAR(par);
    return Q_UNHANDLED(); // wrong return (must be Q_TRAN())
}

//----------------------------------------------------------------------------
typedef struct {
// protected:
    QHsm super;
} TstSM4;

// protected:
static QState TstSM4_initial(TstSM4 * const me, void const * const par);
static QState TstSM4_s(TstSM4 * const me, QEvt const * const e);

//............................................................................
static TstSM4 TstSM4_inst;
QAsm * const the_TstSM4 = &TstSM4_inst.super.super;

//............................................................................
void TstSM4_ctor(void) {
    TstSM4 *me = &TstSM4_inst;
    QHsm_ctor(&me->super, Q_STATE_CAST(&TstSM4_initial));
}
//............................................................................
static QState TstSM4_initial(TstSM4 * const me, void const * const par) {
    Q_UNUSED_PAR(me);
    Q_UNUSED_PAR(par);
    return Q_TRAN(&TstSM4_s);
}
//............................................................................
static QState TstSM4_s(TstSM4 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        default: {
            status_ = Q_SUPER(&TstSM4_s); // circular nesting
            break;
        }
    }
    return status_;
}

//----------------------------------------------------------------------------
typedef struct {
// protected:
    QHsm super;
} TstSM5;

// protected:
static QState TstSM5_initial(TstSM5 * const me, void const * const par);
static QState TstSM5_s1(TstSM5 * const me, QEvt const * const e);
static QState TstSM5_s2(TstSM5 * const me, QEvt const * const e);

//............................................................................
static TstSM5 TstSM5_inst;
QAsm * const the_TstSM5 = &TstSM5_inst.super.super;

//............................................................................
void TstSM5_ctor(void) {
    TstSM5 *me = &TstSM5_inst;
    QHsm_ctor(&me->super, Q_STATE_CAST(&TstSM5_initial));
}
//............................................................................
static QState TstSM5_initial(TstSM5 * const me, void const * const par) {
    Q_UNUSED_PAR(me);
    Q_UNUSED_PAR(par);
    return Q_TRAN(&TstSM5_s2);
}
//............................................................................
static QState TstSM5_s1(TstSM5 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        default: {
            status_ = Q_SUPER(0); // nesting in NULL
            break;
        }
    }
    return status_;
}
//............................................................................
static QState TstSM5_s2(TstSM5 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        default: {
            status_ = Q_SUPER(&TstSM5_s1);
            break;
        }
    }
    return status_;
}

//----------------------------------------------------------------------------
typedef struct {
// protected:
    QHsm super;
} TstSM6;

// protected:
static QState TstSM6_initial(TstSM6 * const me, void const * const par);

static QState TstSM6_s1(TstSM6 * const me, QEvt const * const e);
static QState TstSM6_s2(TstSM6 * const me, QEvt const * const e);
static QState TstSM6_s3(TstSM6 * const me, QEvt const * const e);
static QState TstSM6_s4(TstSM6 * const me, QEvt const * const e);
static QState TstSM6_s5(TstSM6 * const me, QEvt const * const e);
static QState TstSM6_s6(TstSM6 * const me, QEvt const * const e);
static QState TstSM6_s7(TstSM6 * const me, QEvt const * const e);
static QState TstSM6_s8(TstSM6 * const me, QEvt const * const e);

static QState TstSM6_t1(TstSM6 * const me, QEvt const * const e);
static QState TstSM6_t2(TstSM6 * const me, QEvt const * const e);
static QState TstSM6_t3(TstSM6 * const me, QEvt const * const e);
static QState TstSM6_t4(TstSM6 * const me, QEvt const * const e);
static QState TstSM6_t5(TstSM6 * const me, QEvt const * const e);
static QState TstSM6_t6(TstSM6 * const me, QEvt const * const e);

static QState TstSM6_u1(TstSM6 * const me, QEvt const * const e);
static QState TstSM6_u2(TstSM6 * const me, QEvt const * const e);
static QState TstSM6_u3(TstSM6 * const me, QEvt const * const e);

//............................................................................
static TstSM6 TstSM6_inst;
QAsm * const the_TstSM6 = &TstSM6_inst.super.super;

//............................................................................
void TstSM6_ctor(void) {
    TstSM6 *me = &TstSM6_inst;
    QHsm_ctor(&me->super, Q_STATE_CAST(&TstSM6_initial));
}
//............................................................................
static QState TstSM6_initial(TstSM6 * const me, void const * const par) {
    Q_UNUSED_PAR(me);
    Q_UNUSED_PAR(par);
    QS_OBJ_DICTIONARY(&TstSM6_inst);

    QS_FUN_DICTIONARY(&TstSM6_s1);
    QS_FUN_DICTIONARY(&TstSM6_s2);
    QS_FUN_DICTIONARY(&TstSM6_s3);
    QS_FUN_DICTIONARY(&TstSM6_s4);
    QS_FUN_DICTIONARY(&TstSM6_s5);
    QS_FUN_DICTIONARY(&TstSM6_s6);
    QS_FUN_DICTIONARY(&TstSM6_s7);
    QS_FUN_DICTIONARY(&TstSM6_s8);

    QS_FUN_DICTIONARY(&TstSM6_t1);
    QS_FUN_DICTIONARY(&TstSM6_t2);
    QS_FUN_DICTIONARY(&TstSM6_t3);
    QS_FUN_DICTIONARY(&TstSM6_t4);
    QS_FUN_DICTIONARY(&TstSM6_t5);
    QS_FUN_DICTIONARY(&TstSM6_t6);

    QS_FUN_DICTIONARY(&TstSM6_u1);
    QS_FUN_DICTIONARY(&TstSM6_u2);
    QS_FUN_DICTIONARY(&TstSM6_u3);

    return Q_TRAN(&TstSM6_s6);
}
//............................................................................
static QState TstSM6_s1(TstSM6 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&TstSM6_s8);
            break;
        }
        case A_SIG: {
            status_ = Q_TRAN(&TstSM6_s7);
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
static QState TstSM6_s2(TstSM6 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&TstSM6_s3);
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM6_s1);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState TstSM6_s3(TstSM6 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&TstSM6_s4);
            break;
        }
        case C_SIG: {
            status_ = Q_TRAN(&TstSM6_u2);
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM6_s2);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState TstSM6_s4(TstSM6 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&TstSM6_s5);
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM6_s3);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState TstSM6_s5(TstSM6 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&TstSM6_s6);
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM6_s4);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState TstSM6_s6(TstSM6 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&TstSM6_s7);
            break;
        }
        case B_SIG: {
            status_ = Q_TRAN(&TstSM6_s8);
            break;
        }
        case D_SIG: {
            QASM_DISPATCH((QAsm *)me, e, 0U); // RTC violation
            status_ = Q_HANDLED();
            break;
        }
        case E_SIG: {
            status_ = Q_TRAN(&TstSM6_t1);
            break;
        }
        case G_SIG: {
            status_ = Q_TRAN(0); // tran, to NULL
            break;
        }
        case H_SIG: {
            status_ = Q_TRAN(&TstSM6_u1);
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM6_s5);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState TstSM6_s7(TstSM6 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&TstSM6_s8);
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM6_s6);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState TstSM6_s8(TstSM6 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case F_SIG: {
            status_ = Q_TRAN(&TstSM6_t5);
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM6_s7);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState TstSM6_t1(TstSM6 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case D_SIG: {
            status_ = Q_TRAN(&TstSM6_u3);
            break;
        }
        case E_SIG: {
            status_ = Q_TRAN(&TstSM6_s7);
            break;
        }
        case F_SIG: {
            status_ = Q_TRAN(&TstSM6_s1);
            break;
        }
        case G_SIG: {
            status_ = Q_TRAN(&TstSM6_s2);
            break;
        }
        case H_SIG: {
            status_ = Q_TRAN(&TstSM6_t6);
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
static QState TstSM6_t2(TstSM6 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        default: {
            status_ = Q_SUPER(&TstSM6_t1);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState TstSM6_t3(TstSM6 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        default: {
            status_ = Q_SUPER(&TstSM6_t2);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState TstSM6_t4(TstSM6 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        default: {
            status_ = Q_SUPER(&TstSM6_t3);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState TstSM6_t5(TstSM6 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case F_SIG: {
            status_ = Q_TRAN(&TstSM6_s5);
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM6_t4);
            break;
        }
    }
    return status_;
}
//............................................................................
static QState TstSM6_t6(TstSM6 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_INIT_SIG: {
            status_ = Q_TRAN(0); // initial tran. to NULL
            break;
        }
        default: {
            status_ = Q_SUPER(&TstSM6_t5);
            break;
        }
    }
    return status_;
}

//............................................................................
static QState TstSM6_u1(TstSM6 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&TstSM6_u2);
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
static QState TstSM6_u2(TstSM6 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        default: {
            status_ = Q_HANDLED(); // should be Q_SUPER()
            Q_UNUSED_PAR(me); // avoid compiler warning
            break;
        }
    }
    return status_;
}
//............................................................................
static QState TstSM6_u3(TstSM6 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        default: {
            status_ = Q_SUPER(0); // NULL superstate
            break;
        }
    }
    return status_;
}

//----------------------------------------------------------------------------
bool TstSM6_isIn(uint32_t const state_num) {
    bool stat = false;
    switch (state_num) {
    case 1:
        stat = QASM_IS_IN(the_TstSM6, Q_STATE_CAST(&TstSM6_s1));
        break;
    case 2:
        stat = QASM_IS_IN(the_TstSM6, Q_STATE_CAST(&TstSM6_s2));
        break;
    case 3:
        stat = QASM_IS_IN(the_TstSM6, Q_STATE_CAST(&TstSM6_s3));
        break;
    default:
        Q_ERROR();
    }
    return stat;
}
//----------------------------------------------------------------------------
QStateHandler TstSM6_child(uint32_t const  state_num) {
    QStateHandler child;
    switch (state_num) {
    case 1:
        child = QHsm_childState(&TstSM6_inst.super, Q_STATE_CAST(&TstSM6_s1));
        break;
    case 2:
        child = QHsm_childState(&TstSM6_inst.super, Q_STATE_CAST(&TstSM6_s2));
        break;
    case 3:
        child = QHsm_childState(&TstSM6_inst.super, Q_STATE_CAST(&TstSM6_s3));
        break;
    default:
        Q_ERROR();
    }
    return child;
}
