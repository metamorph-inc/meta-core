REM Dependencies: SPP_PATH, CVS_PATH
REM --------------------------------

set CVS_RSH=%CVS_PATH%/plink.exe
cd /d %VCP_PATH%/../
"%CVS_PATH%/cvs.exe" -d :ext:repo:/project/ECSLDP-repository co VCP
IF ERRORLEVEL 1 GOTO ErrorHandler
GOTO Success

:ErrorHandler
	echo ******************************************************************************** 
	echo * 	cvs checkout FAILED at %DATE% %TIME% >> %VCP_LOGF%
	SET ERRORLEV=1
	GOTO End
:Success
	echo ******************************************************************************** 
	echo * 	cvs checkout SUCCEEDED at %DATE% %TIME%  >>  %VCP_LOGF%
:End
	cd /d %VCP_PATH%\autobuild
