REM Dependencies: SPP_PATH, UDM_PATH, UDM_3RDPARTY_PATH, GREAT_PATH, ANTLR_PATH, BOOST_PATH, OTIF_PATH, VCP_PATH, VCP_LOGF
REM --------------------------------------------------------------------------------------------------
   
        if "%VCP_LOGF%"=="" GOTO Usage

        if "%VCP_PATH%"=="" GOTO Usage

	SET LIB=%LIB%;%UDM_3RDPARTY_PATH%\zlib;%UDM_3RDPARTY_PATH%\xerces-c_2_5_0\lib;%UDM_3RDPARTY_PATH%\xalan-c_1_8_0\lib
	call vsvars32.bat
	
	cd /d %VCP_PATH%
	devenv vcp.sln /rebuild Debug /useenv  >>  %VCP_LOGF%
	IF ERRORLEVEL 1 GOTO BuildError
	devenv vcp.sln /rebuild Release /useenv >>  %VCP_LOGF%
	IF ERRORLEVEL 1 GOTO BuildError

:BuildSuccess
	echo ******************************************************************************** >>  %VCP_LOGF%
	echo * VCP build SUCCEEDED at %DATE% %TIME% >>  %VCP_LOGF%
	GOTO End
:BuildError
	echo ******************************************************************************** >>  %VCP_LOGF%
	echo * VCP build FAILED at %DATE% %TIME% >>  %VCP_LOGF%
	SET ERRORLEV=1
	GOTO End

:Usage
        echo off
        echo *******************************************************************************
        echo * The environment variables VCP_PATH and VCP_LOGF must be set in 
        echo * order to use vcp_build.bat.
        echo * The log file will be written to the VCP_LOGF filename and path.
        echo.
        SET ERRORLEV=1
        GOTO AfterEnd

:End
	cd /d %VCP_PATH%\autobuild
:AfterEnd

	
