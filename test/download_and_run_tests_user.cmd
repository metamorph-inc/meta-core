pushd "%~dp0"

..\bin\Python27\Scripts\python.exe update_meta_tools.py %*
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
echo %TIME%

rem with runas, writing to HKLM will fail. Previously, it would write COM registration (though it shouldn't) and we would have both Installer dlls and SVN dlls registered
.\exec_unelevated.exe "%ProgramFiles(x86)%\MSBuild\14.0\Bin\MSBuild.exe" build_tests_user.msbuild /m /nodeReuse:false
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
echo %TIME%

.\exec_unelevated.exe "%ProgramFiles(x86)%\MSBuild\14.0\Bin\MSBuild.exe" build_tests_user.msbuild /t:DynamicsTeamSimulations /m /nodeReuse:false
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
echo %TIME%
