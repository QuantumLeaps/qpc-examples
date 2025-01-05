//============================================================================
// Product: DPP example, BSP for QUTest
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
#include "dpp.h"
#include "bsp.h"

Q_DEFINE_THIS_MODULE("bsp")

// Local objects -------------------------------------------------------------
static uint32_t l_rnd; // random seed

enum {
    BSP_CALL = QS_USER,
};

//............................................................................
void BSP_init(void) {
    BSP_randomSeed(1234U);

    QS_FUN_DICTIONARY(&BSP_displayPaused);
    QS_FUN_DICTIONARY(&BSP_displayPhilStat);
    QS_FUN_DICTIONARY(&BSP_random);
    QS_FUN_DICTIONARY(&BSP_randomSeed);
    QS_ONLY(produce_sig_dict());
    QS_USR_DICTIONARY(BSP_CALL);
}
//............................................................................
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    QS_BEGIN_ID(BSP_CALL, 0U) // app-specific record
        QS_FUN(&BSP_displayPhilStat);
        QS_U8(0, n);
        QS_STR(stat);
    QS_END()
}
//............................................................................
void BSP_displayPaused(uint8_t paused) {
    QS_TEST_PROBE_DEF(&BSP_displayPaused)

    QS_TEST_PROBE(
        Q_ASSERT_ID(100, 0);
    )
    QS_BEGIN_ID(BSP_CALL, 0U) // app-specific record
        QS_FUN(&BSP_displayPaused);
        QS_U8(0, paused);
    QS_END()
}
//............................................................................
uint32_t BSP_random(void) { // a fake random-number generator
    QS_TEST_PROBE_DEF(&BSP_random)
    uint32_t rnd = 123U;

    QS_TEST_PROBE(
        rnd = qs_tp_;
    )
    QS_BEGIN_ID(BSP_CALL, 0U) // app-specific record
        QS_FUN(&BSP_random);
        QS_U32(0, rnd);
    QS_END()
    return rnd;
}
//............................................................................
void BSP_randomSeed(uint32_t seed) {
    QS_TEST_PROBE_DEF(&BSP_randomSeed)

    QS_TEST_PROBE(
        seed = qs_tp_;
    )
    l_rnd = seed;
    QS_BEGIN_ID(BSP_CALL, 0U) // app-specific record
        QS_FUN(&BSP_randomSeed);
        QS_U32(0, seed);
    QS_END()
}
