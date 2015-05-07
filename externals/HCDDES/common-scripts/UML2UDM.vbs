' USAGE:  UML2UDM.vbs  infile outfile
'	Imports a GME/UML paradigm definition and converts to UDM's XML format
'
' Arguments:
'	1. infile : input GME/UML meta-model 
'	2. outfile: export meta to UDM's required XML format
'
infile  = WScript.arguments.item(0)
outfile = WScript.arguments.item(1)

Set mga  	= CreateObject("Mga.MgaProject")
Set launcher	= CreateObject("Mga.MgaLauncher")
    interpreter = "MGA.Interpreter.UML2XML"
REGISTER	= CLng(128)
DONT_REGISTER	= CLng(0)

mga.Open "MGA="&infile

launcher.RunComponent interpreter,mga, Nothing, Nothing,REGISTER 

mga.Close


