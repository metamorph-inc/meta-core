' CreateXMP.vbs <mga_file>
'	Invokes the MetaGME 2004 Interpreter on 'mga_file'
'	registers the paradigm, and produces an output .xmp 
'	file in that file's folder
'
' Arguments:
'	1. <mga_file>: GME model in the MetaGME paradigm 
'
mga_file= WScript.arguments.item( 0)
Set oMGAProj= CreateObject( "Mga.MgaProject")
oMGAProj.Open "MGA=" & mga_file
'MsgBox mga_file
Set launcher= CreateObject( "Mga.MgaLauncher")
launcher.RunComponent "MGA.Interpreter.MetaGME", oMGAProj, Nothing, Nothing, CLng( 128)
oMGAProj.Close

