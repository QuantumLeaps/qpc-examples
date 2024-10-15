@setlocal

@set HOMEDIR=%CD%
set LOG=%HOMEDIR%

::goto skip

set DIR=self_test\test
cd %HOMEDIR%\..\%DIR%
make -j8 -f nucleo-u545re.mak LOG=%LOG% OPT=c USB=%1 flash
if %ERRORLEVEL% neq 2 goto err

set DIR=blinky\test
cd %HOMEDIR%\..\%DIR%
make -j8 -f nucleo-u545re.mak LOG=%LOG% OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err

set DIR=dpp\test_philo
cd %HOMEDIR%\..\%DIR%
make -j8 -f nucleo-u545re.mak LOG=%LOG% OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err

set DIR=dpp\test_table
cd %HOMEDIR%\..\%DIR%
make -j8 -f nucleo-u545re.mak LOG=%LOG% OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err

set DIR=dpp\test_dpp
cd %HOMEDIR%\..\%DIR%
make -j8 -f nucleo-u545re.mak LOG=%LOG% OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err

set DIR=dpp-comp\test_philo
cd %HOMEDIR%\..\%DIR%
make -j8 -f nucleo-u545re.mak LOG=%LOG% OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err

set DIR=dpp-comp\test_table
cd %HOMEDIR%\..\%DIR%
make -j8 -f nucleo-u545re.mak LOG=%LOG% OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err

set DIR=dpp-comp\test_dpp
cd %HOMEDIR%\..\%DIR%
make -j8 -f nucleo-u545re.mak LOG=%LOG% OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err

set DIR=evt_par\test
cd %HOMEDIR%\..\%DIR%
make -j8 -f nucleo-u545re.mak LOG=%LOG% OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err

set DIR=qhsm\test
cd %HOMEDIR%\..\%DIR%
make -j8 -f nucleo-u545re.mak LOG=%LOG% OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err

set DIR=qmsm\test
cd %HOMEDIR%\..\%DIR%
make -j8 -f nucleo-u545re.mak LOG=%LOG% OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err

set DIR=start_seq\test
cd %HOMEDIR%\..\%DIR%
make -j8 -f nucleo-u545re.mak LOG=%LOG% OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err

set DIR=unity_basic\qutest
cd %HOMEDIR%\..\%DIR%
make -j8 -f nucleo-u545re.mak LOG=%LOG% OPT=c USB=%1 flash
if %ERRORLEVEL% neq 2 goto err

set DIR=unity_mock\qutest
cd %HOMEDIR%\..\%DIR%
make -j8 -f nucleo-u545re.mak LOG=%LOG% OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err

set DIR=unity_strlen\qutest
cd %HOMEDIR%\..\%DIR%
make -j8 -f nucleo-u545re.mak LOG=%LOG% OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err

set DIR=integration_tests\test_qk
cd %HOMEDIR%\..\%DIR%
make -j8 -f nucleo-u545re.mak LOG=%LOG% OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err

set DIR=integration_tests\test_qxk
cd %HOMEDIR%\..\%DIR%
make -j8 -f nucleo-u545re.mak LOG=%LOG% OPT=c USB=%1 flash
if %ERRORLEVEL% neq 0 goto err

:cleanup
@echo Cleanup...
cd %HOMEDIR%\..
for /d /r %%i in (build_nucleo-u545re) do @rmdir /s/q "%%i"
@echo OK

@chdir /d %HOMEDIR%
exit /b

:err
@chdir /d %HOMEDIR%
@echo %DIR% Has Test Errors... 1>&2

@endlocal
