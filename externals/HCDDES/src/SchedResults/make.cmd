@ECHO OFF
REM CLS


set MAKE_TARGET=SchedResults
    echo Rebuilding %MAKE_TARGET%
    if EXIST make-chkenv.cmd CALL make-chkenv.cmd
    if ERRORLEVEL 1 goto FAILED
    msbuild.exe %MAKE_TARGET%.sln /t:Rebuild /p:Configuration=Release /clp:ErrorsOnly;PerformanceSummary;Summary /nologo
    if ERRORLEVEL 1 goto FAILED


:SUCCEDED
	set MAKE_TARGET=
	exit /b 0
:FAILED
	echo ERROR: Rebuilding %MAKE_TARGET% failed
	set MAKE_TARGET=
	exit /b 1
