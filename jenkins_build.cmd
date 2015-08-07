rem set JAVA_HOME=C:\Program Files (x86)\Java\jdk1.7.0_07\

pushd %~dp0

where UdmDll_3_2_VS10.dll

c:\Windows\Microsoft.NET\Framework\v4.0.30319\msbuild make.msbuild /t:Installer /fl /flp:Verbosity=diag;PerformanceSummary /m /nodeReuse:false
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
c:\Windows\Microsoft.NET\Framework\v4.0.30319\msbuild make.msbuild /t:source_code_analysis /fl /flp:Verbosity=diag;PerformanceSummary;LogFile=source_code_analysis.log /m /nodeReuse:false
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

echo %TIME%
.\bin\Python27\Scripts\python.exe copy_pdbs.py
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
echo %TIME%

.\bin\Python27\Scripts\python.exe run_in_job_object.py c:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe test\run.msbuild
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
echo %TIME%
