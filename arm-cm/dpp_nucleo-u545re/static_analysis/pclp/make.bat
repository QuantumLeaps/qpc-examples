@echo off
:: ===========================================================================
:: QP Real-Time Event Framework (RTEF)
::
:: Copyright (C) 2005-2026 Quantum Leaps, LLC. All rights reserved.
::
::                    Q u a n t u m  L e a P s
::                    ------------------------
::                    Modern Embedded Software
::
:: SPDX-License-Identifier: LicenseRef-QL-commercial
::
:: This software is licensed under the terms of the Quantum Leaps commercial
:: license agreement. This header is a notice of that license, not a
:: substitute for it; the signed agreement governs in case of conflict.
:: Use of this software requires a valid Quantum Leaps commercial or
:: evaluation license. Contact Quantum Leaps for licensing options.
::
:: RESTRICTIONS
:: Unless otherwise agreed in writing with Quantum Leaps, the licensee
:: may NOT:
:: (a) redistribute, encumber, sell, rent, lease, sublicense, or otherwise
::     transfer rights in this software to any third party,
:: (b) remove, obscure, or alter any trademark, copyright, or other
::     proprietary notice, legend, symbol, or label present in this
::     software,
:: (c) create derivative works, or modify or repackage this software, for
::     the purpose of concealing its origin or evading the licensing
::     obligations that apply to it,
:: (d) use this software, or any portion of it, as input to train,
::     fine-tune, evaluate, or otherwise develop any artificial
::     intelligence or machine learning model, or use such a model to
::     replicate, emulate, or compete with the functionality of this
::     software, whether for internal use or in any product or service
::     offered to a third party.
::
:: This software is provided "AS IS," without warranty of any kind, express
:: or implied, except as expressly stated in the governing license agreement.
::
:: Quantum Leaps contact information:
:: <www.state-machine.com/licensing>
:: <info@state-machine.com>
:: ===========================================================================
@setlocal

:: usage of make.bat
@echo Usage: make [gnu_arm] [-d...] files
@echo examples:
@echo make : use gnu_arm (default)
@echo make mingw32 : use mingw32
@echo make mingw32 -dQ_SPY : use mingw32 and define Q_SPY
@echo.

:: directory to produce diagnistics logs
@set LOG_DIR=..

:: NOTE: adjust to for your installation directory of PC-Lint-Plus
@set PCLP=C:\tools\pclp\pclp64.exe
if NOT exist "%PCLP%" (
    @echo The PC-Lint-Plus toolset not found. Please adjust make.bat
    @goto end
)

:: NOTE: adjust to for your installation directory of QP Framework
@set QP=../../../../..
if NOT exist "%QP%" (
    @echo The QP framework not found. Please adjust make.bat
    @goto end
)
@set QP_PCLP=%QP%/static-analysis/pclp

if "%1"=="mingw32" (
    set LINTFLAGS=%QP_PCLP%/co-mingw32.lnt options.lnt %2 %3 %4
    @echo mingw32
) else (
    set LINTFLAGS=%QP_PCLP%/co-gnu_arm.lnt options.lnt %1 %2 %3
    @echo gnu_arm
)

:: cleanup
@del %LOG_DIR%\*.log

:: linting -------------------------------------------------------------------
cls
%PCLP% -os(%LOG_DIR%/diag.log) %LINTFLAGS% ../../*.c
::%PCLP% +os(%LOG_DIR%/diag.log) %LINTFLAGS% ../../qk/bsp.c

:output
type %LOG_DIR%\diag.log

:end
@endlocal
