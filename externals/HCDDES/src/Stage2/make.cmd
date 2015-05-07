@ECHO OFF
REM CLS


if EXIST make-chkenv.cmd CALL make-chkenv.cmd
if ERRORLEVEL 1 goto FAILED

echo Rebuilding Stage2
msbuild Stage2.sln /t:Rebuild /p:Configuration=Release /clp:ErrorsOnly;PerformanceSummary;Summary /nologo
if ERRORLEVEL 1 goto FAILED


:SUCCEEDED
	exit /b 0
;FAILED
	echo Rebuilding Stage2 failed
	exit /b 1
