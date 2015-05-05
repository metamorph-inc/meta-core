echo off
set INSTALL_DIR=install

REM for /f "tokens=1-5 delims=/ " %%d in ("%date%") do set INSTALL_DIR=%INSTALL_DIR%_%%g_%%e_%%f_%%d
REM for /f "tokens=1-5 delims=:" %%d in ("%time%") do set INSTALL_DIR=%INSTALL_DIR%_%%d_%%e_%%f

echo on
mkdir %INSTALL_DIR%
cd %INSTALL_DIR%

REM install GME
wget -c http://build.isis.vanderbilt.edu/job/GME_x64_msi/label=build-slave6/lastSuccessfulBuild/artifact/trunk/Install/GME_x64.msi
start /wait msiexec /I GME_x64.msi /qn /Lv* GME_x64.install.log ALLUSERS=1

REM install META
wget -c http://build.isis.vanderbilt.edu/job/META_core/lastSuccessfulBuild/artifact/deploy/META_x64_Core.msi
start /wait msiexec /I META_x64_Core.msi /qn /Lv* META_x64_Core.install.log ALLUSERS=1

cd ..