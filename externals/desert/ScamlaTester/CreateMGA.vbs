' CreateMGA.vbs <xme_infile> <mga_outfile> <paradigm>
'	1. Imports an GME .xme file
'	2. saves it in a .mga file 
'	3. Interprete and register the paradigm
'
' Arguments:
'	1. <xme_infile>: GME model to import
'	2. <mga_outfile>: save GME model 
'	3. <paradigm>: paradigm of the model
'
xme_infile= WScript.arguments.item( 0)
mga_outfile= WScript.arguments.item( 1)
paradigm= WScript.arguments.item( 2)
Set oMGAProj= CreateObject( "Mga.MgaProject")
oMGAProj.Create "MGA=" & mga_outfile, paradigm
Set oMGAParser= CreateObject( "Mga.MgaParser")
oMGAParser.ParseProject oMGAProj, xme_infile
oMGAProj.Close


