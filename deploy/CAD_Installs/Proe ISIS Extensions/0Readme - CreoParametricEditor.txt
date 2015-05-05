Title: 	  CADCreoParametricEditor.exe (Creo Parametric)


CADCreoParametricEditor.exe  v1.0.4.0


Pre-Conditions:
--------------

1. This program only runs on 64 bit machines/OSs.

2. For Creo-Parametric, must have “Creo Paramtric 1.x or 2.x” installed and configured such that Creo runs properly.
   Creo 2.x is prefered.  Creo 1.x will be deprecated in the future.		

Install Instructions:
--------------------

The META installer typically performs the install steps; however, the install can be done manually as follows:

1.  Copy the directory "Proe ISIS Extensions" to a local drive.  Typically, "C:\Program Files"

2.  Setup the following system environment variable:  

	   Environment Variable		Typically Set To
	   ---------------------	----------------
	a) PROE_ISIS_EXTENSIONS 	C:\Program Files\Proe ISIS Extensions
	
3.  No environment variables other than PROE_ISIS_EXTENSIONS are necessary.
    CADCreoParametricEditor.exe will automatically choose the highest version of the supported versions 
    of Creo (see Supported Creo Versions section above) that is on your machine.  However, there 
    are cases where you may want to use the environment variables.  For example, if you have 
    multiple versions of Creo on your machine, then you may want to use the environment variables 
    to specify the version you would like to use.  To specify a particular version of Creo, 
    set the environment variables as follows:

	a) CREO_PARAMETRIC_USE_ENVIR_VARS   TRUE       
	   Note - TRUE in the above line is case insensitive.  "TrUe" will work equally as well.  Any
	   other setting (e.g. False, None, Null) will be treated as false.
	b) CREO_PARAMETRIC_INSTALL_PATH	C:\Progra~1\PTC\CREO2~1.0\PARAME~1\
	c) CREO_PARAMETRIC_COMM_MSG_EXE Template: C:\Program Files\PTC\Creo 2.0\Common Files\<version number>\x86e_win64\obj\pro_comm_msg
					Example:  C:\Program Files\PTC\Creo 2.0\Common Files\M030\x86e_win64\obj\pro_comm_msg


Example Invocation:
-------------------

"%PROE_ISIS_EXTENSIONS%"\bin\CADCreoParametricEditor.exe -<edit_switch> <input_file> <log_file> -<prompt_switch>

   where
       <edit_switch>:  Values edit/check
          -check verifies that the input xml file is syntactically/semantically correct.  It does not
          perform the actions in the input xml file.

         -edit first performs the syntax/semantic checks and if no errors are found, then the Creo
          models are edited per the input-xml-file instructions.  Errors can still occur at this
          phase.  An example of an error that could still occur would be if a Creo model could not be found.

      <input_file>: Name of the input xml file that describes the edits to the Creo model. The XML file must
          be compliant with the schema defined by CADDatumEditor_UML.xme(located in META_SVN\trunk\meta\CAD).

      <log_file>: Name of the log file.  This can contain only the name of the log file or the path and name.

      <prompt_switch>: Values exit_prompt/no_exit_prompt
          -exit_prompt -    Prompt before exiting.
          -no_exit_prompt - Do not prompt before exiting.	

e.g. "%PROE_ISIS_EXTENSIONS%"\bin\CADCreoParametricEditor.exe -edit DatumEditorInput.xml DatumEditorInput.xml.log -exit_prompt 	

Display Help:
------------
"%PROE_ISIS_EXTENSIONS%"\bin\CADCreoParametricEditor.exe -help


Revision History:
----------------

v1.1.0.0 - 10/16/2012	Initial release. Only create datum and point working.

v1.0.1.0 - 10/19/2012	Improved error checking.  Added create axis and better error checking.

v1.0.2.0 - 12/05/2012   Reworked the schema to accommodate Add and Delete functions. Added 1) create coordinate 
			systems and 2)Delete coordinate systems, planes, and points. Delete of axes not 
			supported yet.

v1.0.3.0 - 12/11/2012   Added the capability to use a STEP file as the source model.  Previously, only a
			Creo model could be the initial model. 

v1.0.4.0 - 06/17/2012   Added support for Creo 2.0.  Both Creo 1.0 and 2.0 are supported.  Support for Creo 1.0 will be 
			dropped in the future.  If both Creo 1.0 and 2.0 are on the same machine, Creo 2.0 will be used.

Known Defects
-------------

1. 	Title: 			Memory Leak
   	Date Found: 		12/05/2012
	Found in Version: 	v1.0.2.0
	Description: 		Most of the allocated memory is not deleted.  This should be cleaned up; however,
				the leaks will not cause a problem due to the short duration of typical runs
				of this program.


2. 	Title: 			Delete Axis Not Supported 
   	Date Found: 		12/05/2012
	Found in Version: 	v1.0.2.0
	Description: 		If the input XMl requests the delete of an axis, an error would be returned and
				the program exited.
				
3.	Title: 			pm_is_portmapper_port_busy Warning Message
	Date Found: 		6/17/2013
	Found in Version: 	v1.0.4.0
	Description: 		Creo 2.0 support was added with version v1.0.4.0.  When Creo 2.0 is invoked, the following
			`	warning message is displayed:
					pm_is_portmapper_port_busy: bind failed, error = 10013
					pm_is_portmapper_port_busy: bind failed, error = 10013
				The program works properly; however, the warning message is disconcerting. 