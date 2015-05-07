@ECHO OFF
REM CLS


set MAKE_TARGET=VCP
    echo Rebuilding %MAKE_TARGET% (debug)
    if EXIST make-chkenv.cmd CALL make-chkenv.cmd
    if ERRORLEVEL 1 goto FAILED
    msbuild.exe %MAKE_TARGET%.sln /t:Rebuild /p:Configuration=Debug /clp:ErrorsOnly;PerformanceSummary;Summary /nologo
    if ERRORLEVEL 1 goto FAILED


:SUCCEEDED
	set MAKE_TARGET=
	exit /b 0
:FAILED
    	echo ERROR: Rebuilding %MAKE_TARGET% (debug) failed
	set MAKE_TARGET=
	exit /b 1
