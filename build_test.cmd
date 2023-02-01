pushd %~dp0

cmd /c register_interpreters.cmd

"%ProgramFiles(x86)%\MSBuild\14.0\Bin\MSBuild.exe" make.msbuild /t:All /fl /flp:Verbosity=diag;PerformanceSummary /m /nodeReuse:false /p:"VCTargetsPath=C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V140"\;PlatformToolset=v140
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

rem bin\Python311\python.exe deploy\build_msi.py

rem exit /b 1

bin\Python311\python.exe run_in_job_object.py bin\Python311\python.exe test\run_tests_console_output_xml_parallel.py --start-failed
echo rerun with: bin\Python311\python.exe run_in_job_object.py bin\Python311\python.exe test\run_tests_console_output_xml_parallel.py --start-failed --rerun-failed
IF %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%
