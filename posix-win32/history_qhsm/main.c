//============================================================================
// Product: History Example, Win32
// Last updated for version 8.0.2
// Last updated on  2024-12-27
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <www.gnu.org/licenses/>.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"
#include "history.h"

#include "safe_std.h" // portable "safe" <stdio.h>/<string.h> facilities
#include <stdlib.h>   // for exit()

//Q_DEFINE_THIS_FILE

//............................................................................
int main() {
    QF_init();
    QF_onStartup();

    PRINTF_S("History state pattern\nQP version: %s\n"
           "Press 'o' to OPEN  the door\n"
           "Press 'c' to CLOSE the door\n"
           "Press 't' to start TOASTING\n"
           "Press 'b' to start BAKING\n"
           "Press 'f' to turn the oven OFF\n"
           "Press ESC to quit...\n",
           QP_VERSION_STR);

    // instantiate the ToastOven HSM and trigger the initial transition
    ToastOven_ctor();
    QASM_INIT(the_oven, (void *)0, 0U);

    for (;;) {
        PRINTF_S("\n", "");
        uint8_t c = (uint8_t)QF_consoleWaitForKey();
        PRINTF_S("%c: ", (c >= ' ') ? c : 'X');

        QSignal sig = 0U;
        switch (c) {
            case 'o':  sig = OPEN_SIG;        break;
            case 'c':  sig = CLOSE_SIG;       break;
            case 't':  sig = TOAST_SIG;       break;
            case 'b':  sig = BAKE_SIG;        break;
            case 'f':  sig = OFF_SIG;         break;
            case 0x1B: sig = TERMINATE_SIG;   break;
            default: PRINTF_S("unrecognized", ""); break;
        }

        // dispatch the event into the state machine
        QEvt e = QEVT_INITIALIZER(sig);
        QASM_DISPATCH(the_oven,  &e, 0U);
    }

    QF_onCleanup();
    return 0;
}

//............................................................................
void QF_onStartup(void) {
    QF_consoleSetup();
}
//............................................................................
void QF_onCleanup(void) {
    QF_consoleCleanup();
}
//............................................................................
void QF_onClockTick(void) {
}

//............................................................................
Q_NORETURN Q_onError(char const * const module, int_t const id) {
    FPRINTF_S(stderr, "ERROR in %s, line %d", module, id);
    QF_onCleanup();
    exit(-1);
}
