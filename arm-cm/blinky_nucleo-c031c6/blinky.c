//============================================================================
// "Blinky" example
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
#include "qpc.h"               // QP/C real-time embedded framework
#include "blinky.h"            // Blinky Application interface
#include "bsp.h"               // Board Support Package

//Q_DEFINE_THIS_FILE

// NOTES:
// 1. This is a simple "Blinky" Active Object (AO) coded manually;
// 2. Code like this can be generated automatically from state diagrams
//    by the QM modeling tool;
// 3. For more complex AOs automatic code generation avoids mistakes.
//
//............................................................................
// Blinky Active Object (AO) class...
typedef struct {
    QActive super; // QActive superclass (inheritance)

// data members added to the QActive superclass...
    QTimeEvt timeEvt; // private time event generator
    //...
} Blinky;
extern Blinky Blinky_inst; // Blinky instance declaration

// Blinky state machine declaration...
//
// top-most initial transition:
static QState Blinky_initial(Blinky * const me, void const * const par);
// states:
static QState Blinky_off(Blinky * const me, QEvt const * const e);
static QState Blinky_on(Blinky * const me, QEvt const * const e);

//----------------------------------------------------------------------------
Blinky Blinky_inst; // the Blinky AO instance definition
QActive * const AO_Blinky = &Blinky_inst.super; // global opaque pointer

//............................................................................
// Blinky "constructor"
void Blinky_ctor(void) {
    Blinky * const me = &Blinky_inst; // 'me' points to the Blinky instance

    // call the superclass' constructor
    QActive_ctor(&me->super, Q_STATE_CAST(&Blinky_initial));

    // call the members' constructors
    QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0U);
    //...
}

//----------------------------------------------------------------------------
// Blinky state machine definition...
//
//        +--------------------+             +--------------------+
// O----->|        off         |---TIMEOUT-->|        on          |
//        +--------------------+             +--------------------+
//        |entry: BSP_ledOff() |             |entry: BSP_ledOn()  |
//        |                    |<--TIMEOUT---|                    |
//        +--------------------+             +--------------------+

//............................................................................
// top-most initial transition:
QState Blinky_initial(Blinky * const me, void const * const par) {
    Q_UNUSED_PAR(par); // initialization parameter unused in this case

    // arm the time event to expire in half a second and every half second
    QTimeEvt_armX(&me->timeEvt, BSP_TICKS_PER_SEC/2U, BSP_TICKS_PER_SEC/2U);

    // QS software tracing instrumentation (active only when Q_SPY is defined)
    QS_OBJ_DICTIONARY(AO_Blinky);
    QS_OBJ_DICTIONARY(&Blinky_inst.timeEvt);
    QS_SIG_DICTIONARY(TIMEOUT_SIG, me);

    QS_FUN_DICTIONARY(&Blinky_initial);
    QS_FUN_DICTIONARY(&Blinky_off);
    QS_FUN_DICTIONARY(&Blinky_on);

    return Q_TRAN(&Blinky_off); // transition to "off"
}
//............................................................................
// the "off" state
QState Blinky_off(Blinky * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {       // state entry action
            BSP_ledOff();         // action to execute
            status = Q_HANDLED(); // entry action handled
            break;
        }
        case TIMEOUT_SIG: { // TIMEOUT event
            status = Q_TRAN(&Blinky_on); // transition to "on"
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top); // superstate of this state
            break;
        }
    }
    return status;
}
//............................................................................
// the "on" state
QState Blinky_on(Blinky * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {       // state entry action
            BSP_ledOn();          // action to execute
            status = Q_HANDLED(); // entry action handled
            break;
        }
        case TIMEOUT_SIG: { // TIMEOUT event
            status = Q_TRAN(&Blinky_off); // transition to "off"
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top); // superstate of this state
            break;
        }
    }
    return status;
}

