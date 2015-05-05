@echo OFF

setlocal ENABLEEXTENSIONS
set KEY_NAME=HKLM\SOFTWARE\Wow6432Node\META
set VALUE_NAME=META_PATH

FOR /F "usebackq skip=2 tokens=1-5" %%A IN (`REG QUERY %KEY_NAME% /v %VALUE_NAME% 2^>nul`) DO (
    set ValueName=%%A
    set ValueType=%%B
    REM works only with two spaces in the path or 0 spaces
    set META_PATH=%%C
    if NOT "%%E"=="" (set META_PATH="%%C %%D %%E")
)

if defined ValueName (
    REM @echo Value Name = %ValueName%
    REM @echo Value Type = %ValueType%
    @echo META_PATH = %META_PATH%
	call %META_PATH%bin\Python27\Scripts\activate.bat
	python __main__.py -x
) else (
    @echo %KEY_NAME%\%VALUE_NAME% not found.
)