//============================================================================
// APP example
// Last updated for version 7.4.0
// Last updated on  2024-05-31
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. <state-machine.com>
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
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
extern "C" {
#include "qpc.h"          // QP/C real-time embedded framework
#include "dpp.h"          // DPP Application interface
#include "bsp.h"          // Board Support Package
} // extern "C"

//............................................................................
int main(int argc, char *argv[]) {
    QF_init();            // initialize the framework
    BSP_init(argc, argv); // initialize the BSP
    BSP_start();          // start the AOs/Threads
    return QF_run();      // run the QF application
}
