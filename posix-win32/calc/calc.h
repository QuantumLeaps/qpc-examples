//$file${.::calc.h} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: calc.qm
// File:  ${.::calc.h}
//
// This code has been generated by QM 7.0.1 <www.state-machine.com/qm>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// Copyright (c) 2005 Quantum Leaps, LLC. All rights reserved.
//
//                 ____________________________________
//                /                                   /
//               /    GGGGGGG    PPPPPPPP   LL       /
//              /   GG     GG   PP     PP  LL       /
//             /   GG          PP     PP  LL       /
//            /   GG   GGGGG  PPPPPPPP   LL       /
//           /   GG      GG  PP         LL       /
//          /     GGGGGGG   PP         LLLLLLL  /
//         /___________________________________/
//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// This generated code is open-source software licensed under the GNU
// General Public License (GPL) as published by the Free Software Foundation
// (see <https://www.gnu.org/licenses>).
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
//
//$endhead${.::calc.h} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#ifndef CALC_H_
#define CALC_H_

enum CalcSignals {
    C_SIG = Q_USER_SIG,
    CE_SIG,
    DIGIT_0_SIG,
    DIGIT_1_9_SIG,
    POINT_SIG,
    OPER_SIG,
    EQUALS_SIG,
    OFF_SIG
};

//$declare${Events::CalcEvt} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Events::CalcEvt} .........................................................
typedef struct {
// protected:
    QEvt super;

// public:
    uint8_t key_code;
} CalcEvt;
//$enddecl${Events::CalcEvt} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$declare${SMs::Calc_ctor} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${SMs::Calc_ctor} ..........................................................
void Calc_ctor(void);
//$enddecl${SMs::Calc_ctor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

extern QHsm * const the_calc;  // "opaque" pointer to calculator HSM

#endif // CALC1_H_
