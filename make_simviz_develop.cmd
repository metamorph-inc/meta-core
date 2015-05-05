msbuild make.msbuild /t:Install_simviz_develop
cd bin\Python27\Scripts
call activate.bat
cd ..\..\..\src\Python27Packages\simviz
paster setup-app development.ini
paster serve --reload development.ini