rem set JAVA_HOME=C:\Program Files (x86)\Java\jdk1.7.0_07\

pushd %~dp0

where UdmDll_3_2_VS10.dll

"%ProgramFiles(x86)%\MSBuild\14.0\Bin\MSBuild.exe" make_MDL2MGA.msbuild /t:All;Push_All_NuGet /fl /flp:diag;PerformanceSummary /m /nodeReuse:false
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

set DBGTOOLS=%ProgramFiles%\Debugging Tools for Windows (x86)
IF "%PROCESSOR_ARCHITECTURE%" == "AMD64" set DBGTOOLS=%ProgramFiles%\Debugging Tools for Windows (x64)
IF "%PROCESSOR_ARCHITEW6432%" == "AMD64" set DBGTOOLS=%ProgramW6432%\Debugging Tools for Windows (x64)
IF NOT EXIST "%DBGTOOLS%" set DBGTOOLS=%ProgramFiles(x86)%\Debugging Tools for Windows (x86)

echo %TIME%
pushd externals\HCDDES
call "%DBGTOOLS%\srcsrv\svnindex.cmd" /debug /Ini="%CD%\..\common-scripts\srcsrv.ini" /source="%CD%" /symbols="%CD%"
popd
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
echo %TIME%
copy_pdbs.py
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
echo %TIME%
