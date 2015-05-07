@ECHO OFF
REM CLS


REM ==========================================================================
REM VCP
REM ==========================================================================
CALL make
IF ERRORLEVEL 1 GOTO FAIL

REM ==========================================================================
REM Stage #1
REM ==========================================================================
pushd src\Stage1
CALL make
IF ERRORLEVEL 1 GOTO FAIL
popd

REM ==========================================================================
REM Stage #2
REM ==========================================================================
pushd src\Stage2
CALL make
IF ERRORLEVEL 1 GOTO FAIL
popd

REM ==========================================================================
REM SchedTool
REM ==========================================================================
pushd src\SchedTool
CALL make
IF ERRORLEVEL 1 GOTO FAIL
popd

REM ==========================================================================
REM SchedResults
REM ==========================================================================
pushd src\SchedResults
CALL make
IF ERRORLEVEL 1 GOTO FAIL
popd

REM ==========================================================================
REM SchedViz
REM ==========================================================================
pushd src\SchedViz
CALL make
IF %ERRORLEVEL% NEQ 0 GOTO FAIL
popd

REM ==========================================================================
REM Python Dependencies
REM ==========================================================================
pushd py2exe
CALL generate_dist.cmd
IF %ERRORLEVEL% NEQ 0 GOTO FAIL
popd




:SUCCEED
	exit /b 0
:FAIL
	echo ERROR: Rebuilding all HCDDES products failed
	exit /b 1

