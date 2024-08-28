//$file${.::defer.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: defer.qm
// File:  ${.::defer.c}
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
//$endhead${.::defer.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include "qpc.h"
#include "bsp.h"

#include "safe_std.h" // portable "safe" <stdio.h>/<string.h> facilities

Q_DEFINE_THIS_FILE

//..........................................................................
enum TServerSignals {
    NEW_REQUEST_SIG = Q_USER_SIG, // the new request signal
    RECEIVED_SIG,                 // the request has been received
    AUTHORIZED_SIG,               // the request has been authorized
    TERMINATE_SIG                 // terminate the application
};
//..........................................................................
//$declare${Events::RequestEvt} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Events::RequestEvt} ......................................................
typedef struct {
// protected:
    QEvt super;

// public:
    uint8_t ref_num;
} RequestEvt;
//$enddecl${Events::RequestEvt} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// Active object class -----------------------------------------------------
//$declare${Components::TServer} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Components::TServer} .....................................................
typedef struct TServer {
// protected:
    QActive super;

// private:
    QEQueue requestQueue;
    QEvt const * requestQSto[3];

// public:
    RequestEvt const * activeRequest;
    QTimeEvt receivedEvt;
    QTimeEvt authorizedEvt;
} TServer;

extern TServer TServer_inst;

// public:
static void TServer_ctor(TServer * const me);

// protected:
static QState TServer_initial(TServer * const me, void const * const par);
static QState TServer_idle(TServer * const me, QEvt const * const e);
static QState TServer_busy(TServer * const me, QEvt const * const e);
static QState TServer_receiving(TServer * const me, QEvt const * const e);
static QState TServer_authorizing(TServer * const me, QEvt const * const e);
static QState TServer_final(TServer * const me, QEvt const * const e);
//$enddecl${Components::TServer} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U)%0x2710U))
#error qpc version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${Components::TServer} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Components::TServer} .....................................................
TServer TServer_inst;

//${Components::TServer::ctor} ...............................................
static void TServer_ctor(TServer * const me) {
    QActive_ctor(&me->super, Q_STATE_CAST(&TServer_initial));
    QEQueue_init(&me->requestQueue,
                  me->requestQSto, Q_DIM(me->requestQSto));
    QTimeEvt_ctorX(&me->receivedEvt,   &me->super, RECEIVED_SIG,   0U);
    QTimeEvt_ctorX(&me->authorizedEvt, &me->super, AUTHORIZED_SIG, 0U);
}

//${Components::TServer::SM} .................................................
static QState TServer_initial(TServer * const me, void const * const par) {
    //${Components::TServer::SM::initial}
    (void)par; // unused parameter
    me->activeRequest = (RequestEvt const *)0; // no active request yet


    QS_OBJ_DICTIONARY(&TServer_inst);
    QS_OBJ_DICTIONARY(&TServer_inst.receivedEvt);
    QS_OBJ_DICTIONARY(&TServer_inst.authorizedEvt);
    QS_OBJ_DICTIONARY(&TServer_inst.requestQueue);

    QS_FUN_DICTIONARY(&TServer_idle);
    QS_FUN_DICTIONARY(&TServer_busy);
    QS_FUN_DICTIONARY(&TServer_receiving);
    QS_FUN_DICTIONARY(&TServer_authorizing);
    QS_FUN_DICTIONARY(&TServer_final);

    return Q_TRAN(&TServer_idle);
}

//${Components::TServer::SM::idle} ...........................................
static QState TServer_idle(TServer * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::TServer::SM::idle}
        case Q_ENTRY_SIG: {
            PRINTF_S("%s\n", "idle-ENTRY;");

            // recall the oldest deferred request...
            if (QActive_recall(&me->super, &me->requestQueue)) {
                PRINTF_S("%s\n", "Request recalled");
            }
            else {
                PRINTF_S("%s\n", "No deferred requests");
            }
            status_ = Q_HANDLED();
            break;
        }
        //${Components::TServer::SM::idle::NEW_REQUEST}
        case NEW_REQUEST_SIG: {
            // create and save a new reference to the request event so that
            // this event will be available beyond this RTC step and won't be
            // recycled.
            Q_NEW_REF(me->activeRequest, RequestEvt);

            PRINTF_S("Processing request #%d\n",
                   (int)me->activeRequest->ref_num);
            status_ = Q_TRAN(&TServer_receiving);
            break;
        }
        //${Components::TServer::SM::idle::TERMINATE}
        case TERMINATE_SIG: {
            status_ = Q_TRAN(&TServer_final);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

//${Components::TServer::SM::busy} ...........................................
static QState TServer_busy(TServer * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::TServer::SM::busy}
        case Q_EXIT_SIG: {
            PRINTF_S("busy-EXIT; done processing request #%d\n",
                   (int)me->activeRequest->ref_num);

            // delete the reference to the active request because
            // it is now processed.
            Q_DELETE_REF(me->activeRequest);
            status_ = Q_HANDLED();
            break;
        }
        //${Components::TServer::SM::busy::NEW_REQUEST}
        case NEW_REQUEST_SIG: {
            // defer the new request event...
            if (QActive_defer(&me->super, &me->requestQueue, e)) {
                PRINTF_S("Request #%d deferred;\n",
                       (int)Q_EVT_CAST(RequestEvt)->ref_num);
            }
            else { // deferred queue full
                // option1: ignore the new request and do nothing here

                // option2:
                // flush the oldest request to make room for the new one
                QEvt const *old_evt = QEQueue_get(&me->requestQueue, 0U);
                Q_ASSERT(old_evt != (QEvt *)0);
                PRINTF_S("Previous request #%d DISCARDED;\n",
                         (int)((RequestEvt*)old_evt)->ref_num);
                QF_gc(old_evt); // explicitly recycle old

                // repeat the defer request after making room in the queue
                if (!QActive_defer(&me->super, &me->requestQueue, e)) {
                    Q_ERROR(); // now it must succeed
                }
            }
            status_ = Q_HANDLED();
            break;
        }
        //${Components::TServer::SM::busy::TERMINATE}
        case TERMINATE_SIG: {
            status_ = Q_TRAN(&TServer_final);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

//${Components::TServer::SM::busy::receiving} ................................
static QState TServer_receiving(TServer * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::TServer::SM::busy::receiving}
        case Q_ENTRY_SIG: {
            // inform about the first stage of processing of the request...
            PRINTF_S("receiving-ENTRY; active request: #%d\n",
                   (int)me->activeRequest->ref_num);

            // one-shot timeout in 1 second
            QTimeEvt_armX(&me->receivedEvt, BSP_TICKS_PER_SEC, 0U);
            status_ = Q_HANDLED();
            break;
        }
        //${Components::TServer::SM::busy::receiving}
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->receivedEvt);
            status_ = Q_HANDLED();
            break;
        }
        //${Components::TServer::SM::busy::receiving::RECEIVED}
        case RECEIVED_SIG: {
            status_ = Q_TRAN(&TServer_authorizing);
            break;
        }
        default: {
            status_ = Q_SUPER(&TServer_busy);
            break;
        }
    }
    return status_;
}

//${Components::TServer::SM::busy::authorizing} ..............................
static QState TServer_authorizing(TServer * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::TServer::SM::busy::authorizing}
        case Q_ENTRY_SIG: {
            // inform about the second stage of processing of the request..
            PRINTF_S("authorizing-ENTRY; active request: #%d\n",
                   (int)me->activeRequest->ref_num);

            // one-shot timeout in 2 seconds
            QTimeEvt_armX(&me->authorizedEvt, 2U*BSP_TICKS_PER_SEC, 0U);
            status_ = Q_HANDLED();
            break;
        }
        //${Components::TServer::SM::busy::authorizing}
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->authorizedEvt);
            status_ = Q_HANDLED();
            break;
        }
        //${Components::TServer::SM::busy::authorizing::AUTHORIZED}
        case AUTHORIZED_SIG: {
            status_ = Q_TRAN(&TServer_idle);
            break;
        }
        default: {
            status_ = Q_SUPER(&TServer_busy);
            break;
        }
    }
    return status_;
}

//${Components::TServer::SM::final} ..........................................
static QState TServer_final(TServer * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Components::TServer::SM::final}
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
//$enddef${Components::TServer} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// test harness ============================================================

// Local-scope objects -------------------------------------------------------
static QEvt const *l_tserverQSto[10]; // Event queue storage for TServer
static QF_MPOOL_EL(RequestEvt) l_smlPoolSto[20]; // storage for small pool

//..........................................................................
int main(int argc, char *argv[]) {
    PRINTF_S("Deferred Event state pattern\nQP version: %s\n"
           "Press 'n' to generate a new request\n"
           "Press ESC to quit...\n",
           QP_VERSION_STR);

    QF_init(); // initialize the framework and the underlying RTOS/OS

    BSP_init(argc, argv); // initialize the BSP

    // publish-subscribe not used, no call to QF_psInit()

    // initialize event pools...
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));

    QS_SIG_DICTIONARY(NEW_REQUEST_SIG, (void *)0); // global signals
    QS_SIG_DICTIONARY(RECEIVED_SIG,    (void *)0);
    QS_SIG_DICTIONARY(AUTHORIZED_SIG,  (void *)0);
    QS_SIG_DICTIONARY(TERMINATE_SIG,   (void *)0);

    // start the active objects...
    TServer_ctor(&TServer_inst);
    QActive_start((QActive *)&TServer_inst,
                  1U,
                  l_tserverQSto, Q_DIM(l_tserverQSto),
                  (void *)0, 0U, (void *)0);

    return QF_run(); // run the QF application
}
//..........................................................................
void BSP_onKeyboardInput(uint8_t key) {
    switch (key) {
        case 'n': {  // 'n': new request?
            static uint8_t reqCtr = 0; // count the requests
            RequestEvt *e = Q_NEW(RequestEvt, NEW_REQUEST_SIG);
            e->ref_num = (++reqCtr); // set the reference number
            // post directly to TServer active object
            QACTIVE_POST((QActive *)&TServer_inst, (QEvt *)e, (void *)0);
            break;
        }
        case '\33': { // ESC pressed?
            static QEvt const terminateEvt = QEVT_INITIALIZER(TERMINATE_SIG);
            QACTIVE_POST((QActive *)&TServer_inst, &terminateEvt, (void *)0);
            break;
        }
    }
}
