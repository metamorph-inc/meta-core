@ECHO OFF
REM CLS


set MAKE_TARGET=3rd-Party
    echo Packaging %MAKE_TARGET%
    pushd deploy
REM --deprecated--    "C:\Program Files\Inno Setup 5\Compil32.exe" /cc %MAKE_TARGET%.iss
    if ERRORLEVEL 1 goto FAILED
    popd
set MAKE_TARGET=HCDDES
    echo Packaging %MAKE_TARGET%
    pushd deploy
    "C:\Program Files\Inno Setup 5\ISCC.exe" /cc %MAKE_TARGET%.iss
    if ERRORLEVEL 1 goto FAILED
    popd


:SUCCEEDED
	set MAKE_TARGET=
	exit /b 0
:FAILED
	echo ERROR: Packaging %MAKE_TARGET% failed
	set MAKE_TARGET=
	exit /b 1
