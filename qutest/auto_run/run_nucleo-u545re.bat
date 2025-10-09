@setlocal
@set HOMEDIR=%CD%

::@echo off

@set TRG=nucleo-u545re
@echo Target    : %TRG%
@echo Target    : %TRG% > log_%TRG%.txt

@if "%1"=="" (
@echo usage  : run_%TRG%.bat USB-NUCLEO-BOARD
@echo example: run_%TRG%.bat f:
exit /b
)

@set TESTDIR=%HOMEDIR%\..
@set LOGDIR=%HOMEDIR%
@set MAKEFILE=%TRG%.mak
@set LOGEXT=log
@set LOGSEP=%LOGDIR%\log_sect_sep.txt

:: unit tests ================================================================
set TEST=blinky
cd %TESTDIR%\%TEST%\test
make -j8 -f %MAKEFILE% LOG=. OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\TUN_%TEST%-%TRG%.%LOGEXT%

set TEST=dpp
cd %TESTDIR%\%TEST%\test_dpp
make -j8 -f %MAKEFILE% LOG=. OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\TUN_%TEST%_dpp-%TRG%.%LOGEXT%

set TEST=dpp
cd %TESTDIR%\%TEST%\test_philo
make -j8 -f %MAKEFILE% LOG=. OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\TUN_%TEST%_philo-%TRG%.%LOGEXT%

set TEST=dpp
cd %TESTDIR%\%TEST%\test_table
make -j8 -f %MAKEFILE% LOG=. OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\TUN_%TEST%_table-%TRG%.%LOGEXT%

set TEST=dpp-comp
cd %TESTDIR%\%TEST%\test_dpp
make -j8 -f %MAKEFILE% LOG=. OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\TUN_%TEST%_dpp-%TRG%.%LOGEXT%

set TEST=dpp-comp
cd %TESTDIR%\%TEST%\test_philo
make -j8 -f %MAKEFILE% LOG=. OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\TUN_%TEST%_philo-%TRG%.%LOGEXT%

set TEST=dpp-comp
cd %TESTDIR%\%TEST%\test_table
make -j8 -f %MAKEFILE% LOG=. OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\TUN_%TEST%_table-%TRG%.%LOGEXT%

set TEST=evt-par
cd %TESTDIR%\%TEST%\test
make -j8 -f %MAKEFILE% LOG=. OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\TUN_%TEST%-%TRG%.%LOGEXT%

set TEST=start_seq
cd %TESTDIR%\%TEST%\test
make -j8 -f %MAKEFILE% LOG=. OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\TUN_%TEST%-%TRG%.%LOGEXT%

set TEST=unity_basic
cd %TESTDIR%\%TEST%\qtest
make -j8 -f %MAKEFILE% LOG=. OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\TUN_%TEST%-%TRG%.%LOGEXT%

set TEST=unity_mock
cd %TESTDIR%\%TEST%\qtest
make -j8 -f %MAKEFILE% LOG=. OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\TUN_%TEST%-%TRG%.%LOGEXT%

set TEST=unity_strlen
cd %TESTDIR%\%TEST%\qtest
make -j8 -f %MAKEFILE% LOG=. OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\TUN_%TEST%-%TRG%.%LOGEXT%

set TEST=unity_ledbar
cd %TESTDIR%\%TEST%\qtest
make -j8 -f %MAKEFILE% LOG=. OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\TUN_%TEST%-%TRG%.%LOGEXT%

:: integration tests =========================================================
set TEST=integration_tests
cd %TESTDIR%\%TEST%\test_qk
make -j8 -f %MAKEFILE% LOG=. OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\TIN_qk-%TRG%.%LOGEXT%

set TEST=integration_tests
cd %TESTDIR%\%TEST%\test_qxk
make -j8 -f %MAKEFILE% LOG=. OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.log %LOGDIR%\TIN_qxk-%TRG%.%LOGEXT%

:cleanup
@echo Final cleanup...
cd %TESTDIR%
@for /d /r . %%d in (build_nucleo-u545re) do @if exist "%%d" rd /s /q "%%d"
@echo OK

@cd /d %HOMEDIR%
@del log_%TRG%.txt
exit /b

:err
@chdir /d %HOMEDIR%
@echo %DIR% Has Test Errors... 1>&2

@endlocal
