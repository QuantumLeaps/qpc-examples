//============================================================================
// QP/C main function for ThreadX
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
#include "qpc.h"     // QP/C real-time event framework
#include "bsp.h"     // Board Support Package

//............................................................................
int main() {
    QF_init();       // initialize the framework and the underlying RT kernel
    BSP_init((void *)0); // initialize the BSP
    tx_kernel_enter(); // enter ThreadX RTOS
    return 0;        // tx_kernel_enter() does not return
}
//............................................................................
void tx_application_define(void *first_unused_memory) {
    Q_UNUSED_PAR(first_unused_memory);
    (void)QF_run();  // call
}
