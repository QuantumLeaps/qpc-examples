//============================================================================
// "DPP" example: BSP for console target (Windows, Linux, macOS)
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
extern "C" {
#include "qpc.h"          // QP/C real-time event framework
#include "bsp.h"          // Board Support Package
} // extern "C"

//............................................................................
int main(int argc, char *argv[]) {
    QF_init();            // initialize the framework
    BSP_init((argc > 1) ? argv[1] : nullptr); // initialize the BSP
    return QF_run();      // start the AOs and run the framework
}
