Title: 	 CreateCsysFromStep.py

Description
-----------

This program reads an input STEP file, searches for instances of coordinate system widgets, and outputs 
an XML file that describes the coordinate systems based on the coordinate-system widget locations.  With each 
invocation of CreateCsysFromStep.py, the output XMl is overwritten.  The output XML file is compliant with the 
schema defined by CADDatumEditor_UML.xme(located in META_SVN\trunk\meta\CAD). 

The coordinate system widgets are identified by their dimensional characteristics as follows:

Coordinate   X-Axis   Y-Axis   Z-Axis 
System       Length   Length   Length
Name         (mm)     (mm)     (mm)
---------    ------   ------   ------
CSys_1       60.001   45.001   30.001
CSys_2       60.002   45.002   30.002   
CSys_3       60.003   45.003   30.003
  .
  .
  .
CSys_99      60.099   45.099   30.099


Pre-Conditions:
--------------

1. Phython 2.7.2 or later must be installed.

2. The input STEP file must have dimensional units of mm.	

Install Instructions:
--------------------

The META installer installs the necessary files.  They are typically installed in the bin directory at the location defined
by the environment variable PROE_ISIS_EXTENSIONS, which is typically set to "C:\Program Files\META\Proe ISIS Extensions".
The installed files are:

   CreateCsysFromStep.py
   StepFileUtility.py
   Utils.py   


Example Invocation:
------------------- 	

python "%PROE_ISIS_EXTENSIONS%\"bin\CreateCsysFromStep.py –i <input_step_file> –m <model__step_file> –t <model_type>

   where:
   
      <input_step_file> - name of the input STEP File (e.g. bracket_CSys.STEP) that contains the part/sub-assembly model
                          and the widget coordinate systems placed at the correct locations relative to the  
                          part/sub-assembly model. 

      <model_step_file> - name of the model STEP File (e.g. bracket.STEP).  This STEP file contains only the 
                          part/sub-assembly model.

      <model_type> -      This field should be set to Assembly (e.g. -t Assembly) or Part (e.g. -t Part) in accordance
			  with the model type of <model_step_file>.  The terms Assembly/Part are case insensitive.

e.g. python "%PROE_ISIS_EXTENSIONS%\"bin\CreateCsysFromStep.py –i bracket_CSys.STEP –m  bracket.STEP –t Part

The above example would result in the creation of bracket_CSys.xml.  If bracket_CSys.xml already exists, then 
the existing bracket_CSys.xml would be renamed to ORIGINAL_bracket_CSys.xml, and a new version of bracket_CSys.xml 
would be created.  The new version would have no information from the prior version.

Revision History:
----------------

v0.1.0.0 - 11/15/2012	Initial release.

v0.2.0.0 - 11/29/2012	Formatted xml to improve the appearance via using minidom.
                        Bug fix, <AddCoordinateSystems> is before <AddDatums>.
                        Bug fix, <Origin> is before <XVector> and <YVector>.

v0.3.0.0 - 12/11/2012   Schema change: Added Type, Format and File attributes to <CADComponent>.
                        Changed cmd line arguments to -i, -m, and -t. 

v0.4.0.0 - 12/20/2012 	Changed DirectoryPath attribute to "." from "C:\Temp\scratch\2012_10_02\DeleteDatums".



Known Defects
-------------

None