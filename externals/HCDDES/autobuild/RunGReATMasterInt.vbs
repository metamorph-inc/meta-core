' RunGreatMasterInt.vbs <mga_file>
'	Invokes the Great Master Interpreter on 'mga_file'
'	In silent mode, Phase I is run taking all the defaults
'       the Config.mga, the GR.xml, and the .udm file are produced
'       with the default names in the default locations for this
'       particular UMT .mga file
'
' Arguments:
'	1. <mga_file>: GME model in the UMT paradigm 
'                      note: mga_file must be the filename including
'                            the full path for the GReAT Master
'                            interpreter to work correctly
mga_file= WScript.arguments.item( 0)
Set oMGAProj= CreateObject( "Mga.MgaProject")
oMGAProj.Open "MGA=" & mga_file
Set launcher= CreateObject( "Mga.MgaLauncher")
Set selectedobjs= CreateObject( "Mga.MgaFCOs")
launcher.RunComponent "MGA.Interpreter.GReAT Master Interpreter VC10", oMGAProj, Nothing, selectedobjs, CLng( 128)
oMGAProj.Close(1)

