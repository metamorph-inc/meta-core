REM TODO: for now, this just a placeholder for documenting the process; 
REM       this script doesn't actually work yet!

CALL XME2MGA.vbs 	%1.xme 		%1.mga 		MetaGME
REM  Meta2UML.vbs 	%1.mga 		%1_uml.mga
CALL MetaGME2UML.exe 	%1.mga 		%1_uml.mga
CALL UML2UDM.vbs	%1_uml.mga	%1_udm.xml
REM  UDM2CPP.cmd	%1_udm.xml	GENERATED_CODE_DIR
CALL UDM2CPP.cmd	%1_udm.xml
CALL MGA2XMP.vbs	%1.mga
CALL MGA2XME.vbs	%1.mga		%1.xme
