@ECHO OFF
REM CLS

msbuild.exe VCP.sln /t:Clean /p:Configuration=Debug   /clp:ErrorsOnly;PerformanceSummary;Summary
msbuild.exe VCP.sln /t:Clean /p:Configuration=Release /clp:ErrorsOnly;PerformanceSummary;Summary
