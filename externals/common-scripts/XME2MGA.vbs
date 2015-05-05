' USAGE:  XME2MGA.vbs  infile outfile  paradigm
'	Imports an ASCII, GME .xme model and copies it to a binary .mga version
'
' Arguments:
'	1. infile	:  XME model to import
'	2. outfile	:  save GME model as .mga file
'	3. paradigm	:  paradigm name of the model (e.g. MetaGME)
'
infile	= WScript.arguments.item(0)
outfile	= WScript.arguments.item(1)
paradigm= WScript.arguments.item(2)

Set mga	= CreateObject("Mga.MgaProject")
Set xme	= CreateObject("Mga.MgaParser" )

mga.Create "MGA="&outfile, paradigm
xme.ParseProject mga, infile
mga.Close

