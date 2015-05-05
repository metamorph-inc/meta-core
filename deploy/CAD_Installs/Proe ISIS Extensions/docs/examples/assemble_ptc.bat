REM	The following system environment variables must be defined:
REM	PROE_ISIS_EXTENSIONS	// typically set to	C:\Program Files\Proe ISIS Extensions
REM	PROE_INSTALL_PATH	// typically set to	C:\Progra~1\proeWi~1.0\
REM	PRO_COMM_MSG_EXE	// typically set to C:\Program Files\proeWildfire 5.0\x86e_win64\obj\pro_comm_msg
REM	See "Meta SVN\sandbox\CAD\Installs\Proe ISIS Extensions\0ReadMe.txt" for the complete setup instructions.

set EXE_FILE_NAME=assemble_ptc.exe
set EXE="%PROE_ISIS_EXTENSIONS%\bin\%EXE_FILE_NAME%"
set PROE_START_CMD="%PROE_INSTALL_PATH%/bin/proe.exe -g:no_graphics -i:rpc_input"
set WORKING_DIR="C:\Users\rowens\Documents\Meta SVN\sandbox\seisele\Parts\CAD_TEST_3"


if exist %EXE% goto  :EXE_FOUND
@echo off
echo Error: Could not find %EXE_FILE_NAME%.
echo    Your system is not properly configured to run %EXE_FILE_NAME%.
echo    Please see "Meta SVN\sandbox\CAD\Installs\Proe ISIS Extensions\0ReadMe.txt"
echo    for instructions on how to configure your system.
pause
exit
:EXE_FOUND


set EXIT_PROMPT="YES"

set ASSEMBLY_XML_FILE="IFV_cfg37_hierarchical_Cad.xml"
set LOG_FILE=%ASSEMBLY_XML_FILE%.log

%EXE%     %PROE_START_CMD%    "%PROE_ISIS_EXTENSIONS%"     %WORKING_DIR%      %ASSEMBLY_XML_FILE%     %LOG_FILE%     %EXIT_PROMPT%

