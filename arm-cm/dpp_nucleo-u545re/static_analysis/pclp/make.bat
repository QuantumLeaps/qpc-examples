@echo off
:: ===========================================================================
:: QP Real-Time Event Framework (RTEF)
::
:: Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
::
::                    Q u a n t u m  L e a P s
::                    ------------------------
::                    Modern Embedded Software
::
:: SPDX-License-Identifier: LicenseRef-QL-commercial
::
:: This software is licensed under the terms of the Quantum Leaps commercial
:: licenses. Please contact Quantum Leaps for more information about the
:: available licensing options.
::
:: RESTRICTIONS
:: You may NOT :
:: (a) redistribute, encumber, sell, rent, lease, sublicense, or otherwise
::     transfer rights in this software,
:: (b) remove or alter any trademark, logo, copyright or other proprietary
::     notices, legends, symbols or labels present in this software,
:: (c) plagiarize this software to sidestep the licensing obligations.
::
:: Quantum Leaps contact information :
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
@del *.log

:: linting -------------------------------------------------------------------
cls
%PCLP% -os(diag.log) %LINTFLAGS% ../../*.c

:output
type diag.log

:end
@endlocal
