@ECHO OFF
REM CLS

if EXIST make-chkenv.cmd CALL make-chkenv.cmd
if ERRORLEVEL 1 GOTO FAILED


echo Rebuilding Stage1
msbuild Stage1.sln /t:Rebuild /p:Configuration=Release /clp:ErrorsOnly;PerformanceSummary;Summary /nologo
if ERRORLEVEL 1 GOTO FAILED


:SUCCEEDED
	exit /b 0
:FAILED
	echo Rebuilding Stage1 failed
	exit /b 1
