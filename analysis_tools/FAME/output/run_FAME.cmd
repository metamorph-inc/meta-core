:: Reliability Analysis using FAME
echo off
FOR /F "skip=2 tokens=2,*" %%A IN ('reg.exe query "HKLM\software\META" /v "META_PATH" /reg:64') DO "%%B\bin\Python27\Scripts\Python.exe" scripts\FAME_runner.py