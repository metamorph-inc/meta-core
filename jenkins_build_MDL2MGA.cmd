rem set JAVA_HOME=C:\Program Files (x86)\Java\jdk1.7.0_07\

pushd %~dp0

where UdmDll_3_2_VS10.dll

"%ProgramFiles(x86)%\MSBuild\14.0\Bin\MSBuild.exe" make_MDL2MGA.msbuild /t:All;Push_All_NuGet /fl /flp:diag;PerformanceSummary /m /nodeReuse:false
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
