SET META_INSTRUMENT=True
msbuild make.msbuild /t:CyPhyML /m /nodeReuse:false /maxcpucount:1
if %errorlevel% neq 0 exit /b %errorlevel%
copyinstrumented.cmd