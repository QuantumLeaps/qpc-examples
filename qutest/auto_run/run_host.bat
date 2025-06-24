@setlocal
@set HOMEDIR=%CD%

::@echo off

@set TESTDIR=%HOMEDIR%\..
@set LOGDIR=%HOMEDIR%
@set MAKEFILE=Makefile
@set LOGEXT=host
@set LOGSEP=%LOGDIR%\log_sect_sep.txt

:: goto start

:: unit tests ================================================================
set TEST=blinky
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%.%LOGEXT%

set TEST=dpp
cd %TESTDIR%\%TEST%\test_dpp
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%_dpp.%LOGEXT%

set TEST=dpp
cd %TESTDIR%\%TEST%\test_philo
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%_philo.%LOGEXT%

set TEST=dpp
cd %TESTDIR%\%TEST%\test_table
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%_table.%LOGEXT%

set TEST=dpp-comp
cd %TESTDIR%\%TEST%\test_dpp
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%_dpp.%LOGEXT%

set TEST=dpp-comp
cd %TESTDIR%\%TEST%\test_philo
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%_philo.%LOGEXT%

set TEST=dpp-comp
cd %TESTDIR%\%TEST%\test_table
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%_table.%LOGEXT%

set TEST=evt_par
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%.%LOGEXT%

set TEST=start_seq
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%.%LOGEXT%

set TEST=unity_basic
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
@echo NOTE: 2 errors expected!
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%.%LOGEXT%

set TEST=unity_basic
cd %TESTDIR%\%TEST%\qutest
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
@echo 2 errors expected!
:: if %ERRORLEVEL% neq 2 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%.%LOGEXT%

set TEST=unity_basic
cd %TESTDIR%\%TEST%\qutest
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
@echo NOTE: 2 errors expected!
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%.%LOGEXT%

set TEST=unity_ledbar
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
@echo NOTE: 1 error expected!
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%_unity.%LOGEXT%

set TEST=unity_ledbar
cd %TESTDIR%\%TEST%\qutest
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
@echo NOTE: 1 error expected!
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%.%LOGEXT%

set TEST=unity_ledbar
cd %TESTDIR%\%TEST%\printf
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%_printf.%LOGEXT%

set TEST=unity_ledbar2
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
@echo NOTE: 1 error expected!
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%_unity.%LOGEXT%

set TEST=unity_ledbar2
cd %TESTDIR%\%TEST%\qutest
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
@echo NOTE: 3 errors expected!
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%.%LOGEXT%

set TEST=unity_mock
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%_unity.%LOGEXT%

set TEST=unity_mock
cd %TESTDIR%\%TEST%\qutest
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%.%LOGEXT%

set TEST=unity_strlen
cd %TESTDIR%\%TEST%\test
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%_unity.%LOGEXT%

set TEST=unity_strlen
cd %TESTDIR%\%TEST%\qutest
del *.log *.cov
make -j8 -f %MAKEFILE% LOG=. OPT=c
if %ERRORLEVEL% neq 0 goto err
copy /b/y *.cov + %LOGSEP% + *.log + %LOGSEP% + *.c.gcov %LOGDIR%\TST_%TEST%.%LOGEXT%

:cleanup
@echo Final cleanup...
cd %TESTDIR%
@for /d /r . %%d in (build_host) do @if exist "%%d" rd /s /q "%%d"
@echo OK

@cd /d %HOMEDIR%
exit /b

:err
@chdir /d %HOMEDIR%
@echo %DIR% Has Test Errors... 1>&2

@endlocal
