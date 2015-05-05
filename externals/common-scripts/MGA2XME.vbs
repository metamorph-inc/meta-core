' USAGE:  MGA2XME.vbs  infile.mga  outfile.xme 
'	Imports a binary, GME .mga model and copies it to an XML, .xme encoding
'
' Arguments:
'	1. infile : input MGA model 
'	2. outfile: export model to XML
'
infile  = WScript.arguments.item(0)
outfile = WScript.arguments.item(1)

Set mga  = CreateObject("Mga.MgaProject")
Set xme  = CreateObject("Mga.MgaDumper")

mga.Open         "MGA="&infile
xme.DumpProject  mga, outfile
mga.Close


