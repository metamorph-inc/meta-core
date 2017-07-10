rem set JAVA_HOME=C:\Program Files (x86)\Java\jdk1.7.0_07\

pushd %~dp0

where UdmDll_3_2_VS10.dll

C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe make_CAD.msbuild /t:All;Pack_NuGet /fl /flp:diag;PerformanceSummary /m /nodeReuse:false
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

run_cadunittests

set DBGTOOLS=%ProgramFiles%\Debugging Tools for Windows (x86)
IF "%PROCESSOR_ARCHITECTURE%" == "AMD64" set DBGTOOLS=%ProgramFiles%\Debugging Tools for Windows (x64)
IF "%PROCESSOR_ARCHITEW6432%" == "AMD64" set DBGTOOLS=%ProgramW6432%\Debugging Tools for Windows (x64)
IF NOT EXIST "%DBGTOOLS%" set DBGTOOLS=%ProgramFiles(x86)%\Debugging Tools for Windows (x86)

echo %TIME%
.\bin\Python27\Scripts\python.exe copy_pdbs.py
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
echo %TIME%
