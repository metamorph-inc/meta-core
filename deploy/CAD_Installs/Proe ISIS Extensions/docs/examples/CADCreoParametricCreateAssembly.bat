REM	This file was created by CyPhy2CAD.dll 1. 2. 0. 5
REM 	Date:Tue June 20 14:59:28 2013

REM
REM	The following system environment variable must be set:
REM	    PROE_ISIS_EXTENSIONS	// typically set to C:\Program Files\META\Proe ISIS Extensions
REM
REM	See "C:\Program Files\META\Proe ISIS Extensions\0Readme - CreateAssembly.txt" for the complete setup instructions.

set PARTS_DIR=".\Cad_Components_Directory"
set WORKING_DIR="C:\Temp\scratch\2012_08_14\A_Arm"


Rem ****************************
REM Create Creo Assembly
Rem ****************************

set EXE_FILE_NAME=CADCreoParametricCreateAssembly.exe
set EXE="%PROE_ISIS_EXTENSIONS%\bin\%EXE_FILE_NAME%"

set ASSEMBLY_XML_FILE="A_Arm_TestBench_Cad.xml"
set LOG_FILE=%ASSEMBLY_XML_FILE%.log
set EXIT_PROMPT="YES"

if exist %EXE% goto  :EXE_FOUND
@echo off
echo		Error: Could not find %EXE_FILE_NAME%.
echo		Your system is not properly configured to run %EXE_FILE_NAME%.
echo		Please see For instructions on how to configure your system, please see "0Readme - CreateAssembly.txt"
echo		which is typically located at "C:\Program Files\META\Proe ISIS Extensions"
pause
exit
:EXE_FOUND

%EXE%     -w %WORKING_DIR%  -a %PARTS_DIR% -i %ASSEMBLY_XML_FILE%    -l %LOG_FILE%  -p
REM Other examples
REM %EXE%     -w %WORKING_DIR%  -i %ASSEMBLY_XML_FILE%    -l %LOG_FILE%  -p
REM %EXE%     -w %WORKING_DIR%  -i %ASSEMBLY_XML_FILE%    -l %LOG_FILE%  
