//============================================================================
// Product: calc Example
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
#include "bsp.h"
#include "calc.h"

#include "safe_std.h" // portable "safe" <stdio.h>/<string.h> facilities

//Q_DEFINE_THIS_FILE

//............................................................................
int main() {
    QF_init();
    QF_onStartup();

    PRINTF_S("Calculator example, QP version: %s\n"
           "Press '0' .. '9'     to enter a digit\n"
           "Press '.'            to enter the decimal point\n"
           "Press '+'            to add\n"
           "Press '-'            to subtract or negate a number\n"
           "Press '*'            to multiply\n"
           "Press '/'            to divide\n"
           "Press '=' or <Enter> to get the result\n"
           "Press 'c' or 'C'     to Cancel\n"
           "Press 'e' or 'E'     to Cancel Entry\n"
           "Press <Esc>          to quit.\n\n",
           QP_VERSION_STR);

    Calc_ctor(); // explicitly instantiate the calculator object
    QASM_INIT(the_calc, (void *)0, 0U); // trigger initial transition

    for (;;) { // event loop

        BSP_show_display(); // show the display

        PRINTF_S("%s"," : ");
        fflush(stdout);
        uint8_t key_code = (uint8_t)QF_consoleWaitForKey();
        PRINTF_S("%c ", (key_code >= ' ') ? key_code : 'X');

        QSignal sig;
        switch (key_code) {
            case 'c': // intentionally fall through
            case 'C': {
                sig = C_SIG;
                break;
            }
            case 'e': // intentionally fall through
            case 'E': {
                sig = CE_SIG;
                break;
            }
            case '0': {
                sig = DIGIT_0_SIG;
                break;
            }
            case '1': // intentionally fall through
            case '2': // intentionally fall through
            case '3': // intentionally fall through
            case '4': // intentionally fall through
            case '5': // intentionally fall through
            case '6': // intentionally fall through
            case '7': // intentionally fall through
            case '8': // intentionally fall through
            case '9': {
                sig = DIGIT_1_9_SIG;
                break;
            }
            case '.': {
                sig = POINT_SIG;
                break;
            }
            case '+': // intentionally fall through
            case '-': // intentionally fall through
            case '*': // intentionally fall through
            case '/': {
                sig = OPER_SIG;
                break;
            }
            case '=': // intentionally fall through
            case '\r': { // Enter key
                sig = EQUALS_SIG;
                break;
            }
            case '\33': { // ESC key
                sig = OFF_SIG;
                break;
            }
            default: {
                sig = 0; // invalid event
                break;
            }
        }

        if (sig != 0) {  // valid event generated?
            CalcEvt e = { QEVT_INITIALIZER(sig), key_code}; // Calculator event
            QASM_DISPATCH(the_calc, &e.super, 0U); // dispatch event
        }
    }

    QF_onCleanup();
    return 0;
}
