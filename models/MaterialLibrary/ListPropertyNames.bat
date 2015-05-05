set SCRIPTLOCATION="%PUBLIC%/Documents/Meta Documents/MaterialLibrary/ListPropertyNames.py"

FOR /F "skip=2 tokens=2,*" %%A IN ('%SystemRoot%\SysWoW64\REG.exe query "HKLM\software\META" /v "META_PATH"') DO "%%B\bin\Python27\Scripts\Python.exe" %SCRIPTLOCATION%


pause