//============================================================================
// Product: Console-based BSP
// Last Updated for Version: 5.3.1
// Date of the Last Update:  2014-10-15
//
//                   Q u a n t u m     L e a P s
//                   ---------------------------
//                   innovating embedded systems
//
// Copyright (C) 2005 Quantum Leaps, LLC. www.state-machine.com.
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
// Web:   <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#ifndef BSP_H_
#define BSP_H_

void BSP_init(int argc, char *argv[]);
void BSP_onKeyboardInput(uint8_t key); // process the keyboard scan code

#define BSP_TICKS_PER_SEC    100U

void BSP_showMsg(char const *str);
void BSP_showTime12H(char const *str, uint32_t time, uint32_t base);
void BSP_showTime24H(char const *str, uint32_t time, uint32_t base);

#endif // BSP_H_
