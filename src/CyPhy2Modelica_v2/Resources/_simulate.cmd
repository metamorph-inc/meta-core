:: Modelica TestBench simulation
echo off
pushd %~dp0
%SystemRoot%\SysWoW64\REG.exe query "HKLM\software\META" /v "META_PATH"

SET QUERY_ERRORLEVEL=%ERRORLEVEL%

IF %QUERY_ERRORLEVEL% neq 0 (
    echo on
    echo "META tools not installed." >> _FAILED.txt
    echo "See Error Log: _FAILED.txt"
    exit %QUERY_ERRORLEVEL%
)

FOR /F "skip=2 tokens=2,*" %%A IN ('%SystemRoot%\SysWoW64\REG.exe query "HKLM\software\META" /v "META_PATH"') DO SET META_PATH=%%B
SET META_PYTHON_EXE="%META_PATH%\bin\Python27\Scripts\Python.exe"
%META_PYTHON_EXE% scripts\simulate.py --tool TOOL_TO_BE_REPLACED
IF %ERRORLEVEL% neq 0 (
    echo on
    echo "Modelica simulation failed." >> _FAILED.txt
    exit %ERRORLEVEL%
)
popd
