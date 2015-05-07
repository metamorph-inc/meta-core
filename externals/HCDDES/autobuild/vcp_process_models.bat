REM Dependencies: SPP_PATH, GREAT_PATH, UDM_PATH
REM -----------------------------------------------------------

REM ESMoL GME paradigm
cd /d %VCP_PATH%\ESMoL
"%VCP_PATH%\autobuild\CreateMGA.vbs" ESMoL-meta.xme ESMoL.mga MetaGME
"%VCP_PATH%\autobuild\CreateXmp.vbs" ESMoL.mga
"%GREAT_PATH%\bin\MetaGME2UML.exe" ESMoL.mga ESMoL_uml.mga
"%UDM_PATH%\etc\CreateUdmXml.vbs" ESMoL_uml.mga

REM ESM2SFC
cd /d %VCP_PATH%\SF_CodeGen\src
"%VCP_PATH%\autobuild\CreateMGA.vbs" ESM2SFC\ESM2SFC.xme ESM2SFC.mga UMLModelTransformer
REM "%VCP_PATH%\autobuild\CreateUdmXml.vbs" ESM2SFC.mga   
"%UDM_PATH%\etc\CreateUdmXml.vbs" ESM2SFC.mga   
"%VCP_PATH%\autobuild\RunGReATMasterInt.vbs" %VCP_PATH%\SF_CodeGen\src\ESM2SFC.mga
mkdir Gen
"%GREAT_PATH%\bin\CG.exe" Gen\GenESM2SFC Config.mga -t

cd /d %VCP_PATH%\autobuild

