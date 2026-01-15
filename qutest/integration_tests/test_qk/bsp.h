//============================================================================
// Product: BSP for system-testing
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
#ifndef BSP_H_
#define BSP_H_

void BSP_init(void const * const arg);
void BSP_terminate(int16_t result);

// for testing...
void BSP_trace(QActive const *thr, char const *msg);
void BSP_wait4PB1(void);
void BSP_ledOn(void);
void BSP_ledOff(void);
void BSP_trigISR(void);

uint32_t BSP_romRead(int32_t offset, uint32_t fromEnd);
void BSP_romWrite(int32_t offset, uint32_t fromEnd, uint32_t value);

uint32_t BSP_ramRead(int32_t offset, uint32_t fromEnd);
void BSP_ramWrite(int32_t offset, uint32_t fromEnd, uint32_t value);

enum TestSignals {
    TEST0_SIG = Q_USER_SIG,
    TEST1_SIG,
    TEST2_SIG,
    TEST3_SIG,
    MAX_PUB_SIG,    // the last published signal

    MAX_SIG         // the last signal
};

#endif // BSP_H_
