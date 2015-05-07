:: Calculate cost and lead time metrics using Python
echo off
cmd /c runCADJob.bat
FOR /F "skip=2 tokens=2,*" %%A IN ('C:\Windows\SysWoW64\REG.exe query "HKLM\software\META" /v "META_PATH"') DO set META_PATH=%%B
set META_PYTHON_PATH="%META_PATH%\bin\Python27\Scripts\Python.exe"
%META_PYTHON_PATH% scripts\cltestbench.py || goto :ERROR_SECTION

exit 0

:ERROR_SECTION
set ERROR_CODE=%ERRORLEVEL%
set ERROR_MSG="Encountered error during execution, error level is %ERROR_CODE%"
echo %ERROR_MSG% >>_FAILED.txt
echo ""
echo "See Error Log: _FAILED.txt"
ping -n 8 127.0.0.1 > nul
exit /b %ERROR_CODE%