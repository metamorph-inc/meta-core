@ECHO OFF
REM CLS


set MAKE_TARGET=SchedResults
    echo Rebuilding %MAKE_TARGET% (verbose)
    if EXIST make-chkenv.cmd CALL make-chkenv.cmd
    if ERRORLEVEL 1 goto FAILED
    msbuild.exe %MAKE_TARGET%.sln /t:Rebuild /p:Configuration=Release /verbosity:diagnostic
    if ERRORLEVEL 1 goto FAILED


:SUCCEDED
	set MAKE_TARGET=
	exit /b 0
:FAILED
	echo ERROR: Rebuilding %MAKE_TARGET% (verbose) failed
	set MAKE_TARGET=
	exit /b 1
