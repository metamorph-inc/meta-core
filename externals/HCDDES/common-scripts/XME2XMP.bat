
REM pushd %CD%
REM cd to directory containing this script
REM cd /d %~dp0
CALL XME2MGA.vbs %1.xme %1.mga MetaGME
CALL MGA2XMP.vbs	%1.mga
REM popd
