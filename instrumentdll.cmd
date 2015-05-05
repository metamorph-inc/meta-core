if NOT DEFINED META_INSTRUMENT GOTO end
if NOT %META_INSTRUMENT% == True GOTO end
rmdir /s /q %1\instrumented
mkdir %1\instrumented
vsinstr /coverage %1\%2 /outputpath:%1\instrumented
if NOT "%~3" == "" sn.exe /Ra %1\instrumented\%2 %3
:end