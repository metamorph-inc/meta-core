REM	The following system environment variables must be defined:
REM	PROE_ISIS_EXTENSIONS	// typically set to	C:\Program Files\Proe ISIS Extensions
REM	CREO_PARAMETRIC_INSTALL_PATH	// typically set to      C:\Progra~1\PTC\CREO1~1.0\PARAME~1\
REM	CREO_PARAMETRIC_COMM_MSG_EXE	// typically set to	 C:\Program Files\PTC\Creo 1.0\Common Files\F000\x86e_win64\obj\pro_comm_msg

echo off

set EXE_FILE_NAME=CADCreoParametricComputeMetrics.exe
set PRO_COMM_MSG_EXE=%CREO_PARAMETRIC_COMM_MSG_EXE%
set EXE="%PROE_ISIS_EXTENSIONS%\bin\%EXE_FILE_NAME%"

set PROE_START_CMD="%CREO_PARAMETRIC_INSTALL_PATH%/bin/parametric.exe -g:no_graphics -i:rpc_input"

set WORKING_DIR="."

set EXIT_PROMPT="YES"

set COMPONENTS_XML_FILE="ComputeMetricsExample.xml"
set LOG_FILE=%COMPONENTS_XML_FILE%.log
%EXE%     %PROE_START_CMD%    "%PROE_ISIS_EXTENSIONS%"     %WORKING_DIR%      %COMPONENTS_XML_FILE%     %LOG_FILE%     %EXIT_PROMPT%
