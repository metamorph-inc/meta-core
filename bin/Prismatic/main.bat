REM *****
REM These should be edited for users' environment.
REM cmc.py is a script from PRISMATIC.
REM *****
set CMC_BIN=E:\cmc.py

REM *****
REM These should be edited for the particular model
REM *****
set MODEL_DIR="E:\rccar-tags\Release_12.8.14\CyPhy\Whole_RC_Car"
set MODEL_NAME=CyPhy_RC
set META_DIR="C:\Program Files (x86)\META\meta"
set META_NAME=CyPhyML

REM *****
REM These should not need editing.
REM *****
set PRISMATIC_BIN=prismatic.py

cd %MODEL_DIR%
UdmCopy %MODEL_NAME%.mga %MODEL_NAME%.xml %META_DIR%\%META_NAME%_udm.xml %META_DIR%\%META_NAME%.xsd
if %errorlevel% neq 0 exit /b %errorlevel%
python %CMC_BIN% %MODEL_NAME%.xml
if %errorlevel% neq 0 exit /b %errorlevel%
python %PRISMATIC_BIN%
if %errorlevel% neq 0 exit /b %errorlevel%
more prismatic-results.json
if %errorlevel% neq 0 exit /b %errorlevel%
