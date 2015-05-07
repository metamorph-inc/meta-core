@ECHO OFF
REM CLS
setlocal


if EXIST make-chkenv.cmd CALL make-chkenv.cmd
if ERRORLEVEL 1 goto FAILED

msbuild Stage2.sln /t:Rebuild /p:Configuration=Release /verbosity:diagnostic
if ERRORLEVEL 1 goto FAILED


:SUCCEEDED
	exit /b 0
;FAILED
	echo Rebuilding Stage2 (verbose) failed
	exit /b 1
