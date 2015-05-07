set DBGTOOLS=%ProgramFiles%\Debugging Tools for Windows (x86)
IF "%PROCESSOR_ARCHITECTURE%" == "AMD64" set DBGTOOLS=%ProgramFiles%\Debugging Tools for Windows (x64)
IF "%PROCESSOR_ARCHITEW6432%" == "AMD64" set DBGTOOLS=%ProgramW6432%\Debugging Tools for Windows (x64)

call "%DBGTOOLS%\srcsrv\svnindex.cmd" /debug /Ini="%~dp0srcsrv.ini" /source="%CD%" /symbols="%CD%"
IF %ERRORLEVEL% NEQ 0 exit /b

set SYMBOLS_DIR="%CD%"

set PATH=%PATH%;c:\Python27
where Python.exe
if %ERRORLEVEL% equ 0 Python.exe "%~dp0copy_pdbs.py" -s \\kms1.isis.vanderbilt.edu\symbolstmp -t "%JOB_NAME%" -v "%BUILD_NUMBER%_%NODE_NAME%" & goto :EOF

"%DBGTOOLS%\symstore" add /r /f "%CD%" /s \\kms1.isis.vanderbilt.edu\symbolstmp /t "%JOB_NAME%" /v "%BUILD_NUMBER%_%NODE_NAME%"
