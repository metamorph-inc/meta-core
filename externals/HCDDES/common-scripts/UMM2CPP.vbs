' USAGE:  UMM2CPP.vbs  mcl.mga  
'	Runs the UMM code generator within the specified MCL.mga project
'
' Arguments:
'	1. mcl : the GME MCL spec
'
mcl 		= WScript.arguments.item(0)
Set mga  	= CreateObject("Mga.MgaProject")
Set launcher 	= CreateObject("Mga.MgaLauncher")
interpreter     = "MGA.Interpreter.UMMCG"
DEFAULT_FLAG    = CLng(128)


mga.Open "MGA="&mcl
launcher.RunComponent interpreter,mga, Nothing,Nothing,DEFAULT_FLAG
mga.Close


