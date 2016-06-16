SET META_INSTRUMENT=True
"%ProgramFiles(x86)%\MSBuild\14.0\Bin\MSBuild.exe" make.msbuild /t:CyPhyML /m /nodeReuse:false /maxcpucount:1
if %errorlevel% neq 0 exit /b %errorlevel%
copyinstrumented.cmd