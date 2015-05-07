@ECHO ON
REM CLS


if EXIST make-chkenv.cmd CALL make-chkenv.cmd
if ERRORLEVEL 1 goto FAILED

msbuild.exe VCP.sln /t:Rebuild /p:Configuration=Release /verbosity:diagnostic
if ERRORLEVEL 1 goto FAILED


:SUCCEEDED
	exit /b 0
:FAILED
	echo "ERROR: Rebuild of VCP (verbose) failed"
	exit /b 1

