@ECHO ON
REM CLS
setlocal

set MAKE_TARGET=SchedTool
    echo Rebuilding %MAKE_TARGET%
    if EXIST make-chkenv.cmd CALL make-chkenv.cmd
    if ERRORLEVEL 1 goto FAILED
    msbuild.exe %MAKE_TARGET%.sln /t:Rebuild /p:Configuration=Release /verbosity:diagnostic
    if ERRORLEVEL 1 goto FAILED


:SUCCEDED
	set MAKE_TARGET=
	exit /b 0
:FAILED
	echo ERROR: Rebuilding %MAKE_TARGET% failed
	set MAKE_TARGET=
	exit /b 1
