//============================================================================
// "DPP" example: BSP for console target (Windows, Linux, macOS)
//
// Copyright (C) 2005-2026 Quantum Leaps, LLC. All rights reserved.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open-source GNU
// General Public License, version 3 or later (GPL-3.0-or-later), or,
// alternatively, under the terms of one of the closed-source Quantum Leaps
// commercial licenses.
//
// Redistributions in source code must retain this top-level comment block.
//
// NOTE:
// The GPL does NOT permit distributing this code as part of proprietary
// (closed-source) programs. For such use, please contact Quantum Leaps for
// a commercial license, which is an alternative to the GPL designed
// explicitly for closed-source distribution.
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
    BSP_init((argc > 1) ? argv[1] : nullptr); // initialize the BSP and instantiate/start the AOs
    return QF_run();      // start the AOs and run the framework
}
