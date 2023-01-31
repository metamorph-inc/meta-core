set SCRIPTLOCATION="%PUBLIC%/Documents/Meta Documents/MaterialLibrary/ListAllMaterials.py"

FOR /F "skip=2 tokens=2,*" %%A IN ('%SystemRoot%\SysWoW64\REG.exe query "HKLM\software\META" /v "META_PATH"') DO "%%B\bin\Python311\Python.exe" %SCRIPTLOCATION%



pause