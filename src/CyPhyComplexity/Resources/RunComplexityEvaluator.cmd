@echo off
set PYTHON=%META_PATH%\bin\Python311\python.exe
%PYTHON% ComplexityMain.py Components.csv Connections.csv 1
pause
