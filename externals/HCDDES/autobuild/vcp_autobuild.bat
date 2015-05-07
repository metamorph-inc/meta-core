REM Create the VCP build Log file
	for /F "tokens=1-4 delims=/- " %%A in ('date/T') do set VCP_LOGF=%VCP_PATH%\autobuild\VCP_Build_Log\VCPBuildLog_%%B_%%C.txt

	echo ******************************************************************************** >  %VCP_LOGF%
	echo * VCP build process started %DATE% %TIME% >> %VCP_LOGF%
	echo ******************************************************************************** >>  %VCP_LOGF%

REM Set enviroment variable
	echo * Step 1: call VCP_set_environment.bat at %DATE% %TIME% >>  %VCP_LOGF%
	call vcp_set_environment.bat

REM Check out CVS tree
	echo * Step 2: call VCP_cvs_checkout.bat at %DATE% %TIME% >>  %VCP_LOGF%
	call vcp_cvs_checkout.bat
	IF "%ERRORLEV%" == "1" GOTO ErrorHandler

REM Process VCP models
	echo * Step 3: call VCP_process_models.bat at %DATE% %TIME% >>  %VCP_LOGF%
	call vcp_process_models.bat
	
REM Build VCP projects
	echo * Step 4: call VCP_build.bat at %DATE% %TIME% >>  %VCP_LOGF%
	call VCP_build.bat
	IF "%ERRORLEV%" == "1" GOTO ErrorHandler

REM Create License.txt
	call CreateLicense.vbs
	GOTO End

:ErrorHandler
	echo ******************************************************************************** >>  %VCP_LOGF%
	echo * VCP Build FAILED at %DATE% %TIME% >>  %VCP_LOGF%
REM	call "%VCP_PATH%\autobuild\sendmail.vbs"
	GOTO End
:End
	echo * VCP build process finished at %DATE% %TIME% >> %VCP_LOGF%	
REM call copy %VCP_LOGF% \\Atlantis\project\Escher\VCP_Build_Log /y
	call copy %VCP_LOGF% %LOG_PATH%\VCP_Build_Log.txt /y
