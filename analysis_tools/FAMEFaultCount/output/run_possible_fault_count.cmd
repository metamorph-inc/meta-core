:: Modelica TestBench Dymola simulation
echo off
pushd %~dp0
IF EXIST _FAILED.txt DEL /Q _FAILED.txt
IF EXIST FAME_Possible_Faults.testbench.json DEL /Q FAME_Possible_Faults.testbench.json
%SystemRoot%\SysWoW64\REG.exe query "HKLM\software\META" /v "META_PATH"

SET QUERY_ERRORLEVEL=%ERRORLEVEL%

IF %QUERY_ERRORLEVEL% == 0 (
    FOR /F "skip=2 tokens=2,*" %%A IN ('%SystemRoot%\SysWoW64\REG.exe query "HKLM\software\META" /v "META_PATH"') DO SET META_PATH=%%B)
    SET META_PYTHON_EXE="%META_PATH%\bin\Python27\Scripts\Python.exe"
    IF NOT EXIST scripts\possibleFault.json (
         %META_PYTHON_EXE% scripts\CyPhy_FAME_runner.py 1>> output 2>>&1
         %META_PYTHON_EXE% scripts\postprocess.py ) 
    %META_PYTHON_EXE% scripts\CyPhy_FAME_runner.py 1>> output 2>>&1
    %META_PYTHON_EXE% scripts\postprocess.py
)
IF %QUERY_ERRORLEVEL% == 1 (
    echo on
    echo "META tools not installed." >> _FAILED.txt
    echo "See Error Log: _FAILED.txt"
    exit /b %QUERY_ERRORLEVEL%
)
popd