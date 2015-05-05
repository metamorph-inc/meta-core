' USAGE:  Meta2UML.vbs  infile outfile
'	Imports a MetaGME paradigm definition and converts to GME-UML (for UDM)
'
' Arguments:
'	1. infile : input MGA model 
'	2. outfile: export meta to UML format
'
infile  = WScript.arguments.item(0)
outfile = WScript.arguments.item(1)

Set mga  	= CreateObject("Mga.MgaProject")
Set launcher	= CreateObject("Mga.MgaLauncher")
    interpreter = "MGA.Interpreter.MetaGME2Uml"
HEADLESS_MODE	= CLng(128)
GUI_MODE	= CLng(0)

mga.Open "MGA="&infile

launcher.RunComponent interpreter,mga, Nothing, Nothing, HEADLESS_MODE 

mga.Close


