REM *****
REM These should be edited for users' environment.
REM cmc.py is a script from PRISMATIC.
REM *****
set CMC_BIN=c:\Program Files (x86)\META\bin\cmc.py

REM *****
REM These should be edited for the particular model
REM *****
set MODEL_DIR="E:\rccar-tags\Release_12.8.14\CyPhy\Whole_RC_Car"
set MODEL_NAME=CyPhy_RC
set META_NAME=CyPhyML

REM *****
REM These should not need editing.
REM *****
set PRISMATIC_BIN=c:\Program Files (x86)\META\bin\prismatic.py

cd %MODEL_DIR%
UdmCopy %MODEL_NAME%.mga %MODEL_NAME%.xml c:\Program Files (x86)\META\meta\CyPhyML.xml c:\Program Files (x86)\META\meta\CyPhyML.xsd
if %errorlevel% neq 0 exit /b %errorlevel%
python %CMC_BIN% %MODEL_NAME%.xml
if %errorlevel% neq 0 exit /b %errorlevel%
python %PRISMATIC_BIN%
if %errorlevel% neq 0 exit /b %errorlevel%
REM more prismatic-results.json
REM if %errorlevel% neq 0 exit /b %errorlevel%

