:: Run PSU RAMD Analysis Tool
@echo off

set WORKING_DIR=%~dp0
set INSTALL_DIR=C:\PSU-RAMD-Tools

IF NOT EXIST %INSTALL_DIR% goto NO_EXIST

cmd /c runCADJob.bat

set NATIVE_LIBS=%INSTALL_DIR%\lib\win64
set PATH=%INSTALL_DIR%\omplapp\bin;%INSTALL_DIR%\Python27;%INSTALL_DIR%\jre7\bin;%NATIVE_LIBS%

pushd "%INSTALL_DIR%\main\RAMD"
java -cp ".;lib\*" -Djava.library.path=%NATIVE_LIBS% -Dlog4j.configurationFile=%INSTALL_DIR%\log4j2.xml -Xmx4g RAMD "%WORKING_DIR%\"
popd

exit 0

:NO_EXIST
echo The RAMD installation folder does not exist!
echo Please download and install the RAMD tool from VehicleForge.
exit 1
