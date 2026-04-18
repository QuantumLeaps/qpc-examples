//============================================================================
// Example QUTEST "spy" test double implementation
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
#include "qpc.h"  // QUTest interface
#include "Led.h"  // original interface

//Q_DEFINE_THIS_FILE

enum {
    LED_MOD = QS_USER1 // QS app-specific record for the LED module
};

static uint32_t led_power[MAX_LED] = {
    10, 20, 10, 20, 10
};

//----------------------------------------------------------------------------
void Led_DICTIONARY(void) {
    QS_FUN_DICTIONARY(&Led_on);
    QS_FUN_DICTIONARY(&Led_off);
    QS_USR_DICTIONARY(LED_MOD);
    QS_OBJ_DICTIONARY(&led_power);
}

//----------------------------------------------------------------------------
// turns a given LED off
void Led_off(uint8_t index) {
    QS_BEGIN_ID(LED_MOD, 0U) // app-specific record
        QS_FUN(&Led_off); // function called
        QS_U8 (0, index); // parameter
    QS_END()
}

// turns a given LED on and returns the power drawn by it in uW
uint32_t Led_on(uint8_t index) {
    QS_TEST_PROBE_DEF(&Led_on)
    uint32_t ret = led_power[index]; // assume

    // tweak the returned power draw from the test probe
    QS_TEST_PROBE(
        ret = (uint32_t)qs_tp_;
    )

    QS_BEGIN_ID(LED_MOD, 0U) // app-specific record
        QS_FUN(&Led_on);  // function called
        QS_U32(0, ret);   // value returned
        QS_U8 (0, index); // parameter
    QS_END()

    return ret;
}

