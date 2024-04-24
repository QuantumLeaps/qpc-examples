//$file${.::reminder2.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: reminder2.qm
// File:  ${.::reminder2.c}
//
// This code has been generated by QM 6.1.1 <www.state-machine.com/qm>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
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
// Alternatively, this generated code may be distributed under the terms
// of Quantum Leaps commercial licenses, which expressly supersede the GNU
// General Public License and are specifically designed for licensees
// interested in retaining the proprietary status of their code.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//
//$endhead${.::reminder2.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include "qpc.h"
#include "bsp.h"

#include "safe_std.h" // portable "safe" <stdio.h>/<string.h> facilities
#include <stdlib.h>

//Q_DEFINE_THIS_FILE

//..........................................................................
enum ReminderSignals {
    CRUNCH_SIG = Q_USER_SIG, // the invented reminder signal
    ECHO_SIG,                // check the responsiveness of the system
    TERMINATE_SIG            // terminate the application
};

//$declare${Events::ReminderEvt} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Events::ReminderEvt} .....................................................
typedef struct {
// protected:
    QEvt super;

// public:

    // the next iteration to perform
    uint32_t iter;
} ReminderEvt;

// public:

#ifdef QEVT_DYN_CTOR
static inline ReminderEvt * ReminderEvt_ctor(ReminderEvt * const me,
    uint32_t iter)
{
    if (me != (ReminderEvt *)0) {
        // don't call QEvt_ctor() because the initialization of all
        // QEvt attributes is already done in QF_newX_()
        me->iter = iter;
    }
    return me;
}
#endif // def QEVT_DYN_CTOR
//$enddecl${Events::ReminderEvt} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//..........................................................................
//$declare${Components::Cruncher} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Components::Cruncher} ....................................................
typedef struct {
// protected:
    QActive super;

// private:

    // internal variable
    double sum;
} Cruncher;

// public:
static void Cruncher_ctor(Cruncher * const me);

// protected:
static QState Cruncher_initial(Cruncher * const me, void const * const par);
static QState Cruncher_processing(Cruncher * const me, QEvt const * const e);
static QState Cruncher_final(Cruncher * const me, QEvt const * const e);
//$enddecl${Components::Cruncher} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$define${Components::Cruncher} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Components::Cruncher} ....................................................

//${Components::Cruncher::ctor} ..............................................
static void Cruncher_ctor(Cruncher * const me) {
    QActive_ctor(&me->super, Q_STATE_CAST(&Cruncher_initial));
}

//${Components::Cruncher::SM} ................................................
static QState Cruncher_initial(Cruncher * const me, void const * const par) {
    //${Components::Cruncher::SM::initial}
    (void)par; // unused parameter

    QS_FUN_DICTIONARY(&Cruncher_processing);
    QS_FUN_DICTIONARY(&Cruncher_final);

    return Q_TRAN(&Cruncher_processing);
}

//${Components::Cruncher::SM::processing} ....................................
static QState Cruncher_processing(Cruncher * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::Cruncher::SM::processing}
        case Q_ENTRY_SIG: {
            #ifdef QEVT_DYN_CTOR
            ReminderEvt *reminder = Q_NEW(ReminderEvt, CRUNCH_SIG, 0U);
            #else
            ReminderEvt *reminder = Q_NEW(ReminderEvt, CRUNCH_SIG);
            reminder->iter = 0U;
            #endif

            QACTIVE_POST(&me->super, &reminder->super, me);
            me->sum = 0.0;
            status_ = Q_HANDLED();
            break;
        }
        //${Components::Cruncher::SM::processing::CRUNCH}
        case CRUNCH_SIG: {
            uint32_t i = Q_EVT_CAST(ReminderEvt)->iter;
            uint32_t n = i;
            i += 100U;
            for (; n < i; ++n) {
                if ((n & 1) == 0) {
                    me->sum += 1.0/(2*n + 1);
                }
                else {
                    me->sum -= 1.0/(2*n + 1);
                }
            }
            //${Components::Cruncher::SM::processing::CRUNCH::[i<0x07000000U]}
            if (i < 0x07000000U) {
                #ifdef QEVT_DYN_CTOR
                ReminderEvt *reminder = Q_NEW(ReminderEvt, CRUNCH_SIG, i);
                #else
                ReminderEvt *reminder = Q_NEW(ReminderEvt, CRUNCH_SIG);
                reminder->iter = i;
                #endif

                QACTIVE_POST(&me->super, &reminder->super, me);
                status_ = Q_HANDLED();
            }
            //${Components::Cruncher::SM::processing::CRUNCH::[else]}
            else {
                PRINTF_S("pi=%16.14f\n", 4.0 * me->sum);
                status_ = Q_TRAN(&Cruncher_processing);
            }
            break;
        }
        //${Components::Cruncher::SM::processing::ECHO}
        case ECHO_SIG: {
            PRINTF_S("Echo! pi=%16.14f\n", 4.0 * me->sum);
            status_ = Q_HANDLED();
            break;
        }
        //${Components::Cruncher::SM::processing::TERMINATE}
        case TERMINATE_SIG: {
            status_ = Q_TRAN(&Cruncher_final);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

//${Components::Cruncher::SM::final} .........................................
static QState Cruncher_final(Cruncher * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::Cruncher::SM::final}
        case Q_ENTRY_SIG: {
            PRINTF_S("%s\n", "final-ENTRY;");
                     QF_stop(); // terminate the application
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
//$enddef${Components::Cruncher} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// test harness ============================================================

// Local-scope objects -----------------------------------------------------
static Cruncher l_cruncher; // the Cruncher active object
QEvt const *l_cruncherQSto[10]; // Event queue storage for Cruncher AO
static QF_MPOOL_EL(ReminderEvt) l_smlPoolSto[20]; // storage for small pool

//..........................................................................
int main(int argc, char *argv[]) {
    PRINTF_S("Reminder state pattern\nQP version: %s\n"
           "Press 'e' to echo the current value...\n"
           "Press ESC to quit...\n",
           QP_VERSION_STR);
    fflush(stdout);

    Cruncher_ctor(&l_cruncher);

    BSP_init(argc, argv); // initialize the BSP

    QF_init();// initialize the framework and the underlying RT kernel

    // publish-subscribe not used, no call to QF_psInit()

    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));

    // instantiate and start the active objects...
    QACTIVE_START(&l_cruncher.super,
                  1U,
                  l_cruncherQSto, Q_DIM(l_cruncherQSto),
                  (void *)0, 0U, (QEvt *)0);

    return QF_run(); // run the QF application
}
//..........................................................................
void BSP_onKeyboardInput(uint8_t key) {
    switch (key) {
        case 'e': { // echo the progress so far
            // NOTE:
            // The following Q_NEW_X() allocation might potentially fail
            // but this is acceptable because the "ECHO" event is not
            // considered critical. This code illustrates the Q_NEW_X()
            // API and its use.
            #ifdef QEVT_DYN_CTOR
            QEvt const *echoEvt = Q_NEW_X(QEvt, 2U, ECHO_SIG, QEVT_DYNAMIC);
            #else
            QEvt const *echoEvt = Q_NEW_X(QEvt, 2U, ECHO_SIG);
            #endif
            if (echoEvt != (QEvt *)0) { // event allocated successfully?
                QACTIVE_POST((QActive *)&l_cruncher, echoEvt, (void *)0);
            }
            break;
        }
        case '\033': { // ESC pressed?
            // NOTE: this constant event is statically pre-allocated.
            // It can be posted/published as any other event.
            static QEvt const terminateEvt = QEVT_INITIALIZER(TERMINATE_SIG);
            QACTIVE_POST((QActive *)&l_cruncher, &terminateEvt, (void *)0);
            break;
        }
    }
}
