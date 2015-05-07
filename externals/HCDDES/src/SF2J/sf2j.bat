:: Generate Java language file directly from ESMoL stateflow model
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

set ESMoL_exists=false
if exist ESMoL.xsd (
  set ESMoL_exists=true
) else (
  copy "%MICTES_PATH%\Meta\ESMoL.xsd" . > copy.out
  del copy.out
)

"%MICTES_PATH%\bin\UDMOclPat.exe" "%1" "%MICTES_PATH%\Meta\ESMoL_udm.xml" "%MICTES_PATH%\Java\sf2j.pat"

if "%ESMoL_exists%" == "false" (
  del ESMoL.xsd
)
endlocal
