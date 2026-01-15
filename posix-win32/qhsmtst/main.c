//============================================================================
// QP/C main function (qhsmtst example)
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
#include "qpc.h"          // QP/C real-time event framework
#include "app.h"          // Application

#include "safe_std.h"     // portable "safe" <stdio.h>/<string.h> facilities
#include <stdlib.h>       // for exit()

Q_DEFINE_THIS_FILE

// local objects -----------------------------------------------------------
static FILE *l_outFile = (FILE *)0;
static void dispatch(QSignal sig);

//............................................................................
int main(int argc, char *argv[]) {

    QHsmTst_ctor(); // instantiate the QHsmTst object

    if (argc > 1) { // file name provided?
        FOPEN_S(l_outFile, argv[1], "w");
    }

    if (l_outFile == (FILE *)0) { // interactive version?
        l_outFile = stdout; // use the stdout as the output file

        PRINTF_S("QHsmTst example, built on %s at %s,\n"
               "QP: %s.\nEnter x or X to quit...\n",
               __DATE__, __TIME__, QP_VERSION_STR);

        QASM_INIT(the_sm, (void *)0, 0U); // the top-most initial tran.

        for (;;) { // event loop
            PRINTF_S("\n%s", ">>>");
            char inp[4];
            scanf("%1s", inp); // input the event

            QSignal sig;
            if ('a' <= inp[0] && inp[0] <= 'i') { // in range?
                sig = (QSignal)(inp[0] - 'a' + A_SIG);
            }
            else if ('A' <= inp[0] && inp[0] <= 'I') { // in range?
                sig = (QSignal)(inp[0] - 'A' + A_SIG);
            }
            else if ((inp[0] == 'x') || (inp[0] == 'X')) { // x or X?
                sig = TERMINATE_SIG; // terminate the interactive test
            }
            else {
                sig = IGNORE_SIG;
            }
            QEvt e = QEVT_INITIALIZER(sig);

            QASM_DISPATCH(the_sm, &e, 0U); // dispatch the event
        }
    }
    else { // batch version
        PRINTF_S("QHsmTst example, built on %s at %s, QP %s\n"
               "output saved to %s\n",
               __DATE__, __TIME__, QP_VERSION_STR,
               argv[1]);

        FPRINTF_S(l_outFile, "QHsmTst example, QP %s\n",
                  QP_VERSION_STR);

        QASM_INIT(the_sm, (void *)0, 0U); // the top-most initial tran.

        // testing of dynamic transitions...
        dispatch(A_SIG);
        dispatch(B_SIG);
        dispatch(D_SIG);
        dispatch(E_SIG);
        dispatch(I_SIG);
        dispatch(F_SIG);
        dispatch(I_SIG);
        dispatch(I_SIG);
        dispatch(F_SIG);
        dispatch(A_SIG);
        dispatch(B_SIG);
        dispatch(D_SIG);
        dispatch(D_SIG);
        dispatch(E_SIG);
        dispatch(G_SIG);
        dispatch(H_SIG);
        dispatch(H_SIG);
        dispatch(C_SIG);
        dispatch(G_SIG);
        dispatch(C_SIG);
        dispatch(C_SIG);

        fclose(l_outFile);
    }

    return 0;
}
//............................................................................
Q_NORETURN Q_onError(char const * const module, int_t const id) {
    FPRINTF_S(stderr, "ERROR in %s:%d", module, id);
    exit(-1);
}
//............................................................................
static void dispatch(QSignal sig) {
    Q_REQUIRE((A_SIG <= sig) && (sig <= I_SIG));
    QEvt e = QEVT_INITIALIZER(sig);
    FPRINTF_S(l_outFile, "\n%c:", 'A' + sig - A_SIG);
    QASM_DISPATCH(the_sm, &e, 0U); // dispatch the event
}
//............................................................................
void BSP_display(char const *msg) {
    FPRINTF_S(l_outFile, "%s", msg);
}
//............................................................................
void BSP_terminate(int16_t const result) {
    PRINTF_S("\n%s\n", "Bye, Bye!");
    exit(result);
}
