                       CADCreoParametricMetaLink.exe 

Supported Creo Versions
-----------------------
Creo Parametric 2.x and 3.x

Pre-Conditions
--------------

1. This program only runs on 64 bit machines/OSs.

2. A supported version of Creo (see Supported Creo Versions above) must be installed and must work 
   properly on your machine.

CADCreoParametricMetaLink.exe Manual Install Instructions 
---------------------------------------------------------------------------------
NOTE - Normally there is no need to perform a manual install.  The META installer 
       performs of all the necessary setup.

If you would like to install manually, the install instructions for CADCreoParametricMetaLink.exe 
are the same as those for CADCreoParametricCreateAssembly.exe,  
See ...\src\CADAssembler\CADCreoParametricCreateAssembly\src\cadassembler\CADCreoParametricCreateAssembly
for the instructions.


Revision History:
----------------
v1.5.3.0  12/02/2015 	GitHub Branch: CAD_002_FEA_METALink_Improvements

			Performance improvements:	
				a) Modified SetCADModelParameters.cpp to only regen (up to 5 times) for parametric 
				   parts.  Previously the regen was happening for all parts, but non-parametric
				   parts should have been regened before saving.  If that is not the case, 
				   this program would have no way of correcting modeling errors.
				b) Modified AssemblyEditingViaLink.cpp to remove a superfluous regenerate.
			R.O.





