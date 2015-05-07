echo off
if "%VCP_PATH%" EQU "" (
  echo Variable %%VCP_PATH%% is not set
  goto end
)

if /i "%1" EQU "" (
  echo Usage: %0 filename
  goto end
)

if not exist %1 (
  echo %1 does not exist
  goto end
)

setlocal

rem BEWARE VARIABLE EXPANSION IN COMPOUND-STATEMENTS
rem THIS IS WHY A "goto" IS USED IN THE FOLLOWING IF'S

set OLDFILENAME=%1
if /i "%OLDFILENAME:~-4%" NEQ ".mdl" goto notmdl
  set NEWFILENAME=%OLDFILENAME:~0,-4%.notypes.xml
  %VCP_PATH%\bin\mdl2mga.exe %OLDFILENAME% %NEWFILENAME%
  set OLDFILENAME=%NEWFILENAME%
:notmdl

if /i "%OLDFILENAME:~-11%" NEQ "notypes.xml" goto notnotype
  set NEWFILENAME=%OLDFILENAME:~0,-12%.xml
  copy %OLDFILENAME% %NEWFILENAME%
  set OLDFILENAME=%NEWFILENAME%
:notnotype

%VCP_PATH%\bin\addTypes.exe   %OLDFILENAME%
%VCP_PATH%\bin\SL_CodeGen.exe %OLDFILENAME%
%VCP_PATH%\bin\SF_CodeGen.exe %OLDFILENAME%

endlocal

:end
echo done.
