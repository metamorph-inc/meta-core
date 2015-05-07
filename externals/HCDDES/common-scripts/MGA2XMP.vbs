' USAGE:  MGA2XMP.vbs  meta.mga  
'	Creates a registerable .xmp file from specified MetaGME paradigm defn 
'
' Arguments:
'	1. meta : input meta-model in MetaGME.MGA format
'
meta 		= WScript.arguments.item(0)
Set mga  	= CreateObject("Mga.MgaProject")
Set launcher 	= CreateObject("Mga.MgaLauncher")
interpreter     = "MGA.Interpreter.MetaInterpreter"
REGISTER        = CLng(128)
DONT_REGISTER   = CLng(0)

mga.Open "MGA="&meta
launcher.RunComponent interpreter,mga, Nothing,Nothing,REGISTER
mga.Close


