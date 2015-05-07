@ECHO OFF
REM CLS

if EXIST make-chkenv.cmd CALL make-chkenv.cmd
if ERRORLEVEL 1 goto FAILED

set MAKE_TARGET=VCP
	echo Rebuilding %MAKE_TARGET%
	msbuild.exe %MAKE_TARGET%.sln /t:Rebuild /p:Configuration=Release /clp:ErrorsOnly;PerformanceSummary;Summary /nologo
	if %ERRORLEVEL% NEQ 0 goto FAILED


set MAKE_TARGET=ESMoLUpdate
	echo Rebuilding %MAKE_TARGET%
	pushd src\%MAKE_TARGET%
	msbuild.exe %MAKE_TARGET%.sln /t:Rebuild /p:Configuration=Release /clp:ErrorsOnly;PerformanceSummary;Summary /nologo
	if %ERRORLEVEL% NEQ 0 goto FAILED
	popd


:SUCCEEDED
	exit /b 0
:FAILED
	echo "ERROR: Rebuild of %MAKE_TARGET% failed"
	exit /b 1
