//============================================================================
// Product: DPP example, BSP for QUTest
// Last updated for version 8.0.0
// Last updated on  2024-09-30
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC <state-machine.com>.
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open source GNU
// General Public License version 3 (or any later version), or alternatively,
// under the terms of one of the closed source Quantum Leaps commercial
// licenses.
//
// The terms of the open source GNU General Public License version 3
// can be found at: <www.gnu.org/licenses/gpl-3.0>
//
// The terms of the closed source Quantum Leaps commercial licenses
// can be found at: <www.state-machine.com/licensing>
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// Contact information:
// <www.state-machine.com>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

Q_DEFINE_THIS_MODULE("bsp")

static uint32_t l_rnd;      // random seed

enum {
    BSP_CALL = QS_USER,
};

//............................................................................
void BSP_init(void) {
    BSP_randomSeed(1234U);

    // initialize the QS software tracing
    if (!QS_INIT((void*)0)) {
        Q_ERROR();
    }

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
