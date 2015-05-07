@ECHO OFF
REM CLS
ECHO Deploying the generated files to target directory for cross-compilation
set TARGET_DIR=build


ECHO. 
ECHO 1 of 2) Copying Robostix components  (5 files)
copy ..\..\tests\Generated\target-frodo\InnerLoop_sl.c 	"%TARGET_DIR%\robostix-frodo\files"
if %ERRORLEVEL% NEQ 0 GOTO FAIL
copy ..\..\tests\Generated\target-frodo\InnerLoop_sl.h 	"%TARGET_DIR%\robostix-frodo\files"
if %ERRORLEVEL% NEQ 0 GOTO FAIL
copy ..\..\tests\Generated\target-frodo\DataHandling.c 	"%TARGET_DIR%\robostix-frodo\files"
if %ERRORLEVEL% NEQ 0 GOTO FAIL
copy ..\..\tests\Generated\target-frodo\DataHandling.h 	"%TARGET_DIR%\robostix-frodo\files"
if %ERRORLEVEL% NEQ 0 GOTO FAIL
copy ..\..\tests\Generated\target-frodo\RS.c 		"%TARGET_DIR%\robostix-frodo\files\app\app.c"
if %ERRORLEVEL% NEQ 0 GOTO FAIL

ECHO. 
ECHO 2 of 2) Copying Gumstix components  (3 files) 
copy ..\..\tests\Generated\target-frodo\OuterLoop_sl.h 	"%TARGET_DIR%\gumstix-frodo\files"
if %ERRORLEVEL% NEQ 0 GOTO FAIL
copy ..\..\tests\Generated\target-frodo\OuterLoop_sl.c 	"%TARGET_DIR%\gumstix-frodo\files"
if %ERRORLEVEL% NEQ 0 GOTO FAIL
copy ..\..\tests\Generated\target-frodo\GS.c 		"%TARGET_DIR%\gumstix-frodo\files\app\app.c"
if %ERRORLEVEL% NEQ 0 GOTO FAIL


:SUCCEED
        ECHO.
        ECHO SUCCEEDED 
        ECHO.
	exit /b 0
:FAIL
	ECHO ********************  F A I L E D  ********************
	exit /b 1


