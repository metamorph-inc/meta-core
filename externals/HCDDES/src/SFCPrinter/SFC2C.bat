:: Generate C source file and associated C header file
@echo off
setlocal

if "%ProgramFiles(x86)%" == "" (
  set "PF=%ProgramFiles%"
) else (
  set "PF=%ProgramFiles(x86)%"
)

if "%MICTES_PATH%" == "" (
  set "MICTES_PATH=%PF%\ISIS\ESMoL"
)

"%MICTES_PATH%\bin\SFCPrinter.exe" "%1"

endlocal
