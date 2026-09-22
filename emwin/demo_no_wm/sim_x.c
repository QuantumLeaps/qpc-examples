//============================================================================
// Initialization for emWin/uC/GUI, Win32 simulation VC++
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
#include "GUI.h"
#include "LCD_SIM.h"

void SIM_X_Init() {
    if (((LCD_GetDevCap(LCD_DEVCAP_XSIZE) * LCD_GetXMag()) == 320)
        && ((LCD_GetDevCap(LCD_DEVCAP_YSIZE) * LCD_GetYMag()) == 240)
        && (LCD_GetNumLayers() == 1))
    {
        SIM_GUI_SetLCDPos(132, 46); // position of the LCD in the bitmap
        SIM_GUI_SetTransColor(0xFF0000);       // transparent color
        SIM_GUI_SetLCDColorBlack(0, 0x000000); // "black" color
        SIM_GUI_SetLCDColorWhite(0, 0xC0C0C0); // "white" color
    }
}
