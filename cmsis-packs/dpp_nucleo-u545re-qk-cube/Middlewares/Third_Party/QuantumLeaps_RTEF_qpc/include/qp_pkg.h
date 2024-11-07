//$file${include::qp_pkg.h} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: qpc.qm
// File:  ${include::qp_pkg.h}
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
//$endhead${include::qp_pkg.h} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#ifndef QP_PKG_H_
#define QP_PKG_H_

//$declare${QF::QF-pkg} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${QF::QF-pkg::Attr} ........................................................
//! @class QF_Attr
typedef struct {
// private:

#if (QF_MAX_EPOOL > 0U)
    //! @private @memberof QF_Attr
    QF_EPOOL_TYPE_ ePool_[QF_MAX_EPOOL];
#endif //  (QF_MAX_EPOOL > 0U)

#if (QF_MAX_EPOOL > 0U)
    //! @private @memberof QF_Attr
    uint_fast8_t maxPool_;
#endif //  (QF_MAX_EPOOL > 0U)

#if (QF_MAX_EPOOL == 0U)
    //! @private @memberof QF_Attr
    uint8_t dummy;
#endif //  (QF_MAX_EPOOL == 0U)
} QF_Attr;

//${QF::QF-pkg::priv_} .......................................................
//! @static @private @memberof QF
extern QF_Attr QF_priv_;

//${QF::QF-pkg::bzero_} ......................................................
//! @static @private @memberof QF
void QF_bzero_(
    void * const start,
    uint_fast16_t const len);
//$enddecl${QF::QF-pkg} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// Bitmasks are for the QTimeEvt::flags attribute
#define QTE_FLAG_IS_LINKED      (1U << 7U)
#define QTE_FLAG_WAS_DISARMED   (1U << 6U)

//! @private @memberof QEvt
static inline void QEvt_refCtr_inc_(QEvt const *me) {
    uint8_t rc = me->refCtr_ + 1U;
    ((QEvt *)me)->refCtr_ = rc; // cast away 'const'
#ifndef Q_UNSAFE
    ((QEvt *)me)->evtTag_ = (me->evtTag_ & 0xF0U) | ((~rc) & 0x0FU);
#endif // ndef Q_UNSAFE
}

//! @private @memberof QEvt
static inline void QEvt_refCtr_dec_(QEvt const *me) {
    uint8_t rc = me->refCtr_ - 1U;
    ((QEvt *)me)->refCtr_ = rc; // cast away 'const'
#ifndef Q_UNSAFE
    ((QEvt *)me)->evtTag_ = (me->evtTag_ & 0xF0U) | ((~rc) & 0x0FU);
#endif // ndef Q_UNSAFE
}

#define QACTIVE_CAST_(ptr_)    ((QActive *)(ptr_))
#define Q_PTR2UINT_CAST_(ptr_) ((uintptr_t)(ptr_))

#endif // QP_PKG_H_