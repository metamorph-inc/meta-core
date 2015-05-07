' RegParadigmXmp.vbs <xmp_file>
'	registers the paradigm
'
' Arguments:
'	1. <xmp_file>: GME paradigm 
'
REG_USER     = 1
REG_SYSTEM   = 2
REG_BOTH 	 = 3

xmp_file= WScript.arguments.item( 0)
Set oGME= CreateObject("Mga.MgaRegistrar")
oGME.RegisterParadigmFromData "XML="+xmp_file, "", REG_SYSTEM

