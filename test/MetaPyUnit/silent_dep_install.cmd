REM download wget for windows
REM add wget bin directory to the path

echo off
set INSTALL_DIR=install_dependencies

echo on
mkdir %INSTALL_DIR%
cd %INSTALL_DIR%

REM install Python 2.7
wget -c http://www.python.org/ftp/python/2.7.3/python-2.7.3.msi
start /wait msiexec /I python-2.7.3.msi /qb /Lv* python-2.7.3.install.log ALLUSERS=1

REM install OpenModelica 1.9
wget -c --no-check-certificate https://build.openmodelica.org/omc/builds/windows/releases/1.9.0/OpenModelica-1.9.0-BETA-revision-12739.exe
start /wait OpenModelica-1.9.0-BETA-revision-12739.exe

REM chrome...
REM wget -c https://dl.google.com/tag/s/appguid%3D%7B8A69D345-D564-463C-AFF1-A69D9E530F96%7D%26iid%3D%7B4A7A6D94-3689-2BDB-5B71-98ECEACCCFA3%7D%26lang%3Den%26browser%3D3%26usagestats%3D0%26appname%3DGoogle%2520Chrome%26needsadmin%3Dfalse%26brand%3DCHMA%26installdataindex%3Ddefaultbrowser/update2/installers/ChromeSetup.exe

cd ..