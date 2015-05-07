' RegParadigmXmp.vbs <xmp_file>
'	registers the paradigm
'
' Arguments:
'	1. <xmp_file>: GME paradigm 
'
xmp_file= WScript.arguments.item( 0)
Set oGME= CreateObject("Mga.MgaRegistrar")
oGME.RegisterParadigmFromData "XML="+xmp_file, "", 1

