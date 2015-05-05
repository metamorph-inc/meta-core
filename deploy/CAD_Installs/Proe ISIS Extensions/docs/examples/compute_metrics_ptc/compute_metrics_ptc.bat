REM	The following system environment variables must be defined:
REM	PROE_ISIS_EXTENSIONS	// typically set to	C:\Program Files\Proe ISIS Extensions
REM	PROE_INSTALL_PATH	// typically set to	C:\Progra~1\proeWi~1.0\

echo off
set EXE="%PROE_ISIS_EXTENSIONS%\bin\compute_metrics_ptc.exe"
set PROE_START_CMD="%PROE_INSTALL_PATH%/bin/proe.exe -g:no_graphics -i:rpc_input"
set WORKING_DIR="."

set EXIT_PROMPT="YES"

set COMPONENTS_XML_FILE="bracket_plate.xml"
set LOG_FILE=%COMPONENTS_XML_FILE%.log
%EXE%     %PROE_START_CMD%    "%PROE_ISIS_EXTENSIONS%"     %WORKING_DIR%      %COMPONENTS_XML_FILE%     %LOG_FILE%     %EXIT_PROMPT%
