:: Generate Java source file
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

"%MICTES_PATH%\bin\SFCPrinter.exe" -j "%1"

endlocal
