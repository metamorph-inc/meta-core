rem set JAVA_HOME=C:\Program Files (x86)\Java\jdk1.7.0_07\

pushd %~dp0

where UdmDll_3_2_VS10.dll

"%ProgramFiles(x86)%\MSBuild\14.0\Bin\MSBuild.exe" make.msbuild /t:Installer /fl /flp:Verbosity=diag;PerformanceSummary /m /nodeReuse:false /p:"VCTargetsPath=C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V140"\;PlatformToolset=v140
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
rem "%ProgramFiles(x86)%\MSBuild\14.0\Bin\MSBuild.exe" make.msbuild /t:source_code_analysis /fl /flp:Verbosity=diag;PerformanceSummary;LogFile=source_code_analysis.log /m /nodeReuse:false
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

echo %TIME%
.\bin\Python311\python.exe copy_pdbs.py
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
echo %TIME%

.\bin\Python311\python.exe run_in_job_object.py "%ProgramFiles(x86)%\MSBuild\14.0\Bin\MSBuild.exe" test\run.msbuild
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
echo %TIME%
