@ echo off
setlocal

if "%ProgramFiles(x86)%" == "" (
  set "PF=%ProgramFiles%"
) else (
  set "PF=%ProgramFiles(x86)%")


if "%MICTES_PATH%" == "" (
  set "MICTES_PATH=%PF%\ISIS\ESMoL"
)

set mdlfile=%1
set esmolfile=%mdlfile:.mdl=.xml%

echo %MICTES_PATH%\bin\MDL2MGA.exe %mdlfile% %esmolfile%
"%MICTES_PATH%\bin\MDL2MGA.exe" "%mdlfile%" "%esmolfile%"
if not ERRORLEVEL == 0 (
  echo MDL2MGA incurred an error.
  exit 1
)

echo %MICTES_PATH%\bin\addTypes.exe %mdlfile% %esmolfile%
"%MICTES_PATH%\bin\addTypes.exe" "%esmolfile%"
if not ERRORLEVEL == 0 (
  echo addTypes incurred an error.
  exit 2
)

echo %MICTES_PATH%\bin\SL_CodeGen.exe %esmolfile%
"%MICTES_PATH%\bin\SL_CodeGen.exe" "%esmolfile%" > nul 2>&1
if not ERRORLEVEL == 0 (
  echo SL_CodeGen incurred an error.
  exit 3
)

echo %MICTES_PATH%\bin\SF_CodeGen.exe %esmolfile%
"%MICTES_PATH%\bin\SF_CodeGen.exe" "%esmolfile%" > nul 2>&1
if not ERRORLEVEL == 0 (
  echo SF_CodeGen incurred an error.
  exit 4
)

for /f %%F in ('dir /b *_SFC.xml') do (
  echo %MICTES_PATH%\bin\SFCPrinter -j %%F
  "%MICTES_PATH%\bin\SFCPrinter" -j %%F
  if not ERRORLEVEL == 0 (
    echo SFCPrinter incurred an error.
  )
)
endlocal
