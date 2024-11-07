//$file${src::qf::qf_qeq.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: qpc.qm
// File:  ${src::qf::qf_qeq.c}
//
// This code has been generated by QM 7.0.0 <www.state-machine.com/qm>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// The QP/C software is dual-licensed under the terms of the open-source GNU
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
//
//$endhead${src::qf::qf_qeq.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#define QP_IMPL           // this is QP implementation
#include "qp_port.h"      // QP port
#include "qp_pkg.h"       // QP package-scope interface
#include "qsafe.h"        // QP Functional Safety (FuSa) Subsystem
#ifdef Q_SPY              // QS software tracing enabled?
    #include "qs_port.h"  // QS port
    #include "qs_pkg.h"   // QS facilities for pre-defined trace records
#else
    #include "qs_dummy.h" // disable the QS software tracing
#endif // Q_SPY

Q_DEFINE_THIS_MODULE("qf_qeq")

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U)%0x2710U))
#error qpc version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${QF::QEQueue} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${QF::QEQueue} .............................................................

//${QF::QEQueue::init} .......................................................
//! @public @memberof QEQueue
void QEQueue_init(QEQueue * const me,
    struct QEvt const  * * const qSto,
    uint_fast16_t const qLen)
{
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    #if (QF_EQUEUE_CTR_SIZE == 1U)
    Q_REQUIRE_INCRIT(100, qLen < 0xFFU);
    #endif

    me->frontEvt = (QEvt *)0; // no events in the queue
    me->ring     = qSto;      // the beginning of the ring buffer
    me->end      = (QEQueueCtr)qLen;
    if (qLen > 0U) {
        me->head = 0U;
        me->tail = 0U;
    }
    me->nFree    = (QEQueueCtr)(qLen + 1U); // +1 for frontEvt

    #ifndef Q_UNSAFE
    me->frontEvt_dis = (uintptr_t)~Q_PTR2UINT_CAST_(me->frontEvt);
    me->head_dis  = (QEQueueCtr)~me->head;
    me->tail_dis  = (QEQueueCtr)~me->tail;
    me->nFree_dis = (QEQueueCtr)~me->nFree;
    me->nMin      = me->nFree;
    #endif

    QF_MEM_APP();
    QF_CRIT_EXIT();
}

//${QF::QEQueue::post} .......................................................
//! @public @memberof QEQueue
bool QEQueue_post(QEQueue * const me,
    struct QEvt const * const e,
    uint_fast16_t const margin,
    uint_fast8_t const qsId)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qsId);
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(200, e != (QEvt *)0);

    QEQueueCtr tmp = me->nFree; // get volatile into temporary

    #ifndef Q_UNSAFE
    Q_INVARIANT_INCRIT(201, QEvt_verify_(e));
    QEQueueCtr dis = (QEQueueCtr)~me->nFree_dis;
    Q_INVARIANT_INCRIT(202, tmp == dis);
    #endif // ndef Q_UNSAFE

    // test-probe#1 for faking queue overflow
    QS_TEST_PROBE_DEF(&QEQueue_post)
    QS_TEST_PROBE_ID(1,
        tmp = 0U; // fake no free events
    )

    // required margin available?
    bool status;
    if (((margin == QF_NO_MARGIN) && (tmp > 0U))
        || (tmp > (QEQueueCtr)margin))
    {
        // is it a mutable event?
        if (QEvt_getPoolNum_(e) != 0U) {
            QEvt_refCtr_inc_(e); // increment the reference counter
        }

        --tmp; // one free entry just used up

        me->nFree = tmp; // update the original
    #ifndef Q_UNSAFE
        me->nFree_dis = (QEQueueCtr)~tmp;

        if (me->nMin > tmp) {
            me->nMin = tmp; // update minimum so far
        }
    #endif // ndef Q_UNSAFE

        QS_BEGIN_PRE(QS_QF_EQUEUE_POST, qsId)
            QS_TIME_PRE();        // timestamp
            QS_SIG_PRE(e->sig);   // the signal of the event
            QS_OBJ_PRE(me);       // this queue object
            QS_2U8_PRE(QEvt_getPoolNum_(e), e->refCtr_);
            QS_EQC_PRE(tmp);      // # free entries
    #ifndef Q_UNSAFE
            QS_EQC_PRE(me->nMin); // min # free entries
    #else
            QS_EQC_PRE(0U);       // min # free entries
    #endif
        QS_END_PRE()

        if (me->frontEvt == (QEvt *)0) { // is the queue empty?
            me->frontEvt = e; // deliver event directly
    #ifndef Q_UNSAFE
            Q_INVARIANT_INCRIT(211, me->frontEvt_dis
                            == (uintptr_t)~Q_PTR2UINT_CAST_((QEvt *)0));
            me->frontEvt_dis = (uintptr_t)~Q_PTR2UINT_CAST_(e);
    #endif // ndef Q_UNSAFE
        }
        else { // queue was not empty, insert event into the ring-buffer
            tmp = me->head; // get volatile into temporary
    #ifndef Q_UNSAFE
            dis = (QEQueueCtr)~me->head_dis;
            Q_INVARIANT_INCRIT(212, tmp == dis);
    #endif // ndef Q_UNSAFE
            me->ring[tmp] = e; // insert e into buffer

            if (tmp == 0U) { // need to wrap the head?
                tmp = me->end;
            }
            --tmp; // advance head (counter-clockwise)

            me->head = tmp; // update the original
    #ifndef Q_UNSAFE
            me->head_dis = (QEQueueCtr)~tmp;
    #endif // ndef Q_UNSAFE
        }
        status = true; // event posted successfully
    }
    else { // event cannot be posted
        // dropping events must be acceptable
        Q_ASSERT_INCRIT(210, margin != QF_NO_MARGIN);

        QS_BEGIN_PRE(QS_QF_EQUEUE_POST_ATTEMPT, qsId)
            QS_TIME_PRE();       // timestamp
            QS_SIG_PRE(e->sig);  // the signal of this event
            QS_OBJ_PRE(me);      // this queue object
            QS_2U8_PRE(QEvt_getPoolNum_(e), e->refCtr_);
            QS_EQC_PRE(tmp);     // # free entries
            QS_EQC_PRE(margin);  // margin requested
        QS_END_PRE()

        status = false; // event not posted
    }

    QF_MEM_APP();
    QF_CRIT_EXIT();

    return status;
}

//${QF::QEQueue::postLIFO} ...................................................
//! @public @memberof QEQueue
void QEQueue_postLIFO(QEQueue * const me,
    struct QEvt const * const e,
    uint_fast8_t const qsId)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qsId);
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(300, e != (QEvt *)0);

    QEQueueCtr tmp = me->nFree; // get volatile into temporary

    #ifndef Q_UNSAFE
    Q_INVARIANT_INCRIT(301, QEvt_verify_(e));
    QEQueueCtr dis = (QEQueueCtr)~me->nFree_dis;
    Q_INVARIANT_INCRIT(302, tmp == dis);
    #endif // ndef Q_UNSAFE

    // test-probe#1 for faking queue overflow
    QS_TEST_PROBE_DEF(&QEQueue_postLIFO)
    QS_TEST_PROBE_ID(1,
        tmp = 0U; // fake no free events
    )

    // must be able to LIFO-post the event
    Q_REQUIRE_INCRIT(310, tmp != 0U);

    if (QEvt_getPoolNum_(e) != 0U) { // is it a mutable event?
        QEvt_refCtr_inc_(e); // increment the reference counter
    }

    --tmp; // one free entry just used up

    me->nFree = tmp; // update the original
    #ifndef Q_UNSAFE
    me->nFree_dis = (QEQueueCtr)~tmp;

    if (me->nMin > tmp) {
        me->nMin = tmp; // update minimum so far
    }
    #endif // ndef Q_UNSAFE

    QS_BEGIN_PRE(QS_QF_EQUEUE_POST_LIFO, qsId)
        QS_TIME_PRE();        // timestamp
        QS_SIG_PRE(e->sig);   // the signal of this event
        QS_OBJ_PRE(me);       // this queue object
        QS_2U8_PRE(QEvt_getPoolNum_(e), e->refCtr_);
        QS_EQC_PRE(tmp);      // # free entries
    #ifndef Q_UNSAFE
        QS_EQC_PRE(me->nMin); // min # free entries
    #else
        QS_EQC_PRE(0U);       // min # free entries
    #endif
    QS_END_PRE()

    QEvt const * const frontEvt = me->frontEvt;
    me->frontEvt = e; // deliver the event directly to the front
    #ifndef Q_UNSAFE
    me->frontEvt_dis = (uintptr_t)~Q_PTR2UINT_CAST_(e);
    #endif // ndef Q_UNSAFE

    if (frontEvt != (QEvt *)0) { // was the queue NOT empty?
        tmp = me->tail; // get volatile into temporary;
    #ifndef Q_UNSAFE
        dis = (QEQueueCtr)~me->tail_dis;
        Q_INVARIANT_INCRIT(311, tmp == dis);
    #endif // ndef Q_UNSAFE
        ++tmp;
        if (tmp == me->end) { // need to wrap the tail?
            tmp = 0U; // wrap around
        }
        me->tail = tmp;
    #ifndef Q_UNSAFE
        me->tail_dis = (QEQueueCtr)~tmp;
    #endif
        me->ring[tmp] = frontEvt;
    }

    QF_MEM_APP();
    QF_CRIT_EXIT();
}

//${QF::QEQueue::get} ........................................................
//! @public @memberof QEQueue
struct QEvt const * QEQueue_get(QEQueue * const me,
    uint_fast8_t const qsId)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qsId);
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    QEvt const * const e = me->frontEvt; // always remove evt from the front
    #ifndef Q_UNSAFE
    Q_INVARIANT_INCRIT(411, Q_PTR2UINT_CAST_(e)
                            == (uintptr_t)~me->frontEvt_dis);
    #endif // ndef Q_UNSAFE

    if (e != (QEvt *)0) { // was the queue not empty?
        QEQueueCtr tmp = me->nFree; // get volatile into temporary
    #ifndef Q_UNSAFE
        QEQueueCtr const dis = (QEQueueCtr)~me->nFree_dis;
        Q_INVARIANT_INCRIT(412, tmp == dis);
    #endif // ndef Q_UNSAFE

        ++tmp; // one more free event in the queue

        me->nFree = tmp; // update the # free
    #ifndef Q_UNSAFE
        me->nFree_dis = (QEQueueCtr)~tmp;
    #endif // ndef Q_UNSAFE

        // any events in the ring buffer?
        if (tmp <= me->end) {

            QS_BEGIN_PRE(QS_QF_EQUEUE_GET, qsId)
                QS_TIME_PRE();      // timestamp
                QS_SIG_PRE(e->sig); // the signal of this event
                QS_OBJ_PRE(me);     // this queue object
                QS_2U8_PRE(QEvt_getPoolNum_(e), e->refCtr_);
                QS_EQC_PRE(tmp);    // # free entries
            QS_END_PRE()

            tmp = me->tail; // get volatile into temporary
            QEvt const * const frontEvt = me->ring[tmp];
    #ifndef Q_UNSAFE
            Q_ASSERT_INCRIT(421, frontEvt != (QEvt *)0);
            me->frontEvt_dis = (uintptr_t)~Q_PTR2UINT_CAST_(frontEvt);
    #endif // ndef Q_UNSAFE
            me->frontEvt = frontEvt; // update the original

            if (tmp == 0U) { // need to wrap the tail?
                tmp = me->end;
            }
            --tmp; // advance the tail (counter-clockwise)
            me->tail = tmp; // update the original
    #ifndef Q_UNSAFE
            me->tail_dis = (QEQueueCtr)~tmp;
    #endif // ndef Q_UNSAFE
        }
        else {
            me->frontEvt = (QEvt *)0; // queue becomes empty
    #ifndef Q_UNSAFE
            me->frontEvt_dis = (uintptr_t)~Q_PTR2UINT_CAST_((QEvt *)0);
    #endif // ndef Q_UNSAFE

            // all entries in the queue must be free (+1 for frontEvt)
            Q_INVARIANT_INCRIT(440, tmp == (me->end + 1U));

            QS_BEGIN_PRE(QS_QF_EQUEUE_GET_LAST, qsId)
                QS_TIME_PRE();      // timestamp
                QS_SIG_PRE(e->sig); // the signal of this event
                QS_OBJ_PRE(me);     // this queue object
                QS_2U8_PRE(QEvt_getPoolNum_(e), e->refCtr_);
            QS_END_PRE()
        }
    }

    QF_MEM_APP();
    QF_CRIT_EXIT();

    return e;
}
//$enddef${QF::QEQueue} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^