                         CADCreoParametricCreateAssembly.exe 
	  	
Supported Creo Versions
-----------------------
Creo Parametric 3.x

Deprecated Versions
-------------------
Pro/E not supported
Pro/E assemble_ptc.exe Version: v1.0.008 not supported as of Jan. 2012
Creo 1.0 and 2.0 not supported.

Pre-Conditions
--------------

1. This program only runs on 64 bit machines/OSs.

2. A supported version of Creo (see Supported Creo Versions above) must be installed and must work 
   properly on your machine.
   
3. Typically this program would be invoked on a computer that has the OpenMETA tool suite installed.  If this
   is not the case, the x64 version of UDM must be installed.  This is needed because this program uses UDM to 
   parse xml files.  You can download UDM from http://repo.isis.vanderbilt.edu/downloads/.  Make sure you 
   install the 64 bit versions (e.g. UDM_x64-....msi).    

Manual Install of CADCreoParametricCreateAssembly.exe 
--------------------------------------------------------
NOTE - Normally there is no need to perform a manual install.  The OpenMETA installer 
       performs of all the necessary setup.  Only do the following steps if OpenMETA is 
	   not installed and you wish to run CADCreoParametricCreateAssembly.exe as a 
	   standalone program.

If you would like to install manually, perform the following steps:

1.  Look in the registry for 
	HKEY_LOCAL_MACHINE\SOFTWARE\META\META_PATH
	This key would be set if OpenMETA had been installed and would typically be set to:
	C:\Program Files (x86)\META
	If it is not present in the registry, you will need to create it in the registry and 
	set it to the directory of your choosing.  
	This directory will be refered to as <META_PATH>

2.  Copy the meta-core\bin\CAD\Creo directory from the repository (i.e. GitHub) to <META_PATH>\bin\CAD\

3.  Copy CADCreoParametricCreateAssembly.exe to <META_PATH>\bin\CAD\Creo\bin
	Note - CADCreoParametricCreateAssembly.exe would have been compiled by
	the Visual Studio solution CADCreoParametricCreateAssembly.sln.

   
Manual Creo version selection
--------------------------------------------------------
1.  CADCreoParametricCreateAssembly.exe will automatically choose the highest supported version of 
    Creo (see Supported Creo Versions above) that is on your computer.  If you have 
    multiple versions of Creo on your machine, then you may want to use the environment variables 
    to specify the version you would like to use.  To specify a particular version of Creo, 
    set the environment variables as follows:

	a) CREO_PARAMETRIC_USE_ENVIR_VARS   TRUE       
	   Note - TRUE in the above line is case insensitive.  "TrUe" will work equally as well.  Any
	   other setting (e.g. False, None, Null) will be treated as false.
	b) CREO_PARAMETRIC_INSTALL_PATH	C:\Progra~1\PTC\CREO2~1.0\PARAME~1\
	c) CREO_PARAMETRIC_COMM_MSG_EXE Template: C:\Program Files\PTC\Creo 2.0\Common Files\<version number>\x86e_win64\obj\pro_comm_msg
					Example:  C:\Program Files\PTC\Creo 2.0\Common Files\M030\x86e_win64\obj\pro_comm_msg


Revision History:
----------------
v1.0.000  05/16/2011 	Initial Release

v1.0.001  05/17/2011 	Suppressed the “File not found” message when attempting to delete an 
	   		assembly that does not exist.

v1.0.002  05/18/2011	Added the input argument EXIT_PROMPT.  If set to “NO” (case insensitive),
			assemble_ptc.exe will exit without prompting with “Type Enter to exit.” 

v1.0.003  05/22/2011	Corrected a problem with the routine that searches for parts in an assembly 
			structure.  The search was only checking the first part at a particular level 
			in the tree structure.  The error was in the function ComponentIDChildOf.  
			See ISISContraints.cpp for more details.

v1.0.004  06/06/2011	1) Added support for assemblies as a leaf node.  If an assembly has no children 
			in the xml file, then it is treated in the same manner as a part.  For this 
			case, the assembly must exist in the directory with the other parts.  
	
			2) Added code in ApplyModelConstraints.cpp (function Apply_CADDatum_ModelConstraints)
 			to check the status of ProSolidRegenerate.  If the regeneration failed, a warning
 			message would be displayed in the console and the log file.  The warning message 
			would indicate the corrective action that should be taken.
	
			3) Added a check for all FeatureInterfaceTypes being the same within a Constraint.
  			For example, if FeatureInterfaceType="CAD_DATUM" for one Constraint Pair, then 
			they it must be CAD_DATUM for all Pairs within the Constraint.
	
			4) Changed the units for the assembly template from English units to Metric units.  
			All assemblies created by assemble_ptc.exe will have the units of 
			Millimeter-Newton-Second.  

			5) Corrected memory leaks for the case where the memory was allocated via 
			Pro…Alloc(…) functions.  Called Pro…Free(…) where appropriate. 

v1.0.005 - 06/24/2011	1) Added the capability to modify Pro/E parameters.  See ParametricParameters.h  
			for a description of the function ApplyParametricParameters.  In general the 
			following parameter types are supported.

				XML File		Pro/E Model
				-----------		------------
				FLOAT			Real Number
				BOOLEAN			Yes No
				INTEGER			Integer

			2) Changed the code to accommodate the schema correction that 
			makes <xsd:element name="Constraint" a cardinality of 0 or 1.

v1.0.006 - 07/07/2011	Added code to use the WORKING_DIR, which is set in the .bat file 
			(e.g. set WORKING_DIR="C:\Temp\Demos\3_Sub-Assemblies, GME")


v1.0.007 - 07/15/2011
			Added SIZE_TO_FIT capability.  Made changes to support multiple constraints 
			on a single component.

v1.0.008 - 07/18/2011	Stopped regenerating with the addition/constraint of each part/assembly.  Now,
			only regenerate prior to saving the assembly.  


Start Version 1.1... - Works only with Creo-Parametric
------------------------------------------------------

v1.1.0.0 - 8/12/2011	Starting with v1.1,0,0, switched to Visual Studio 2010 and Creo-Parametric 1.0.  
			Note – this version and subsequent v1.1… versions will only work with Creo-Parametric.
  			Changed the executable name from “assemble_ptc.exe” to “CADCreoParametricCreateAssembly.exe”.
   			“assemble_ptc.exe” will still be used for v1.0… versions, which will support only 
			Pro/E (i.e. not Creo).  Modified the internals such that the XML input file is loaded 
			into structures internal to this application (i.e. CADCreoParametricCreateAssembly.exe).
  			This was necessary to make the SIZE_TO_FIT code serviceable/maintainable.  Added version
 			details to CADCreoParametricCreateAssembly.exe (i.e. right click on 
			CADProECreateAssembly.exe, select Properties, select Details tab).   Added 
			checks to make sure the Creo-Parametric environment variables are set.  	

v1.1.1.0 - 9/9/2011	This version adds the creation of a metrics file each time an assembly is built.  
			The metrics information is stored in a file that has a suffix of “…metrics.xml”.  
			The metrics file is located in the same directory (i.e. working directory) as the 
			log file and the Creo models.  To support this addition, the input file schema 
			changed; therefore, this version will only work with CyPhy2CAD.dll version 1.2.0.0 
			or later.  To determine the version of CyPhy2CAD.dll, right click on CyPhy2CAD.dll, 
			select Properties, and select the Details tab.  

v1.2.0.0 - 3/7/2012	Added FEA Support – Polygon loads/constraints, mesh generation, use of material library, 
			and the generation of the bat file to invoke Abaqus.
			Replaced CodeSynthesis with UDM.
			Modified the creation of the metrics file to throw an exception if materials were not assigned 
			to all parts.

v1.2.1.0 - 5/29/2012	Extended the FEA capabilites.  Added support for 1) new constraint types (Pin and Ball), 
			2) geometry types (circle, concentric circles, cylinder, and box), and 3) support for 
			Calculix.  

v1.2.2.0 - 6/1/2012     Removed the need to define the environment variables CREO_PARAMETRIC_INSTALL_PATH and 
			CREO_PARAMETRIC_COMM_MSG_EXE.  The program now reads the registry to determine 
			the installation location of Creo.
******
v1.2.3.0 - 6/21/2012  	Modified the creation of the <assembly name>_metrics.xml such that the file is not
			created if any errors occur.  An example error would be that mass properties were
			not defined for one or more of the Creo parts. Switched from passing 
			post-processing parameters (e.g. ComponentID, FEAElementID, Materialx Strengths, 
			MetricIds...) via Abaqus bat file parameters to passing the parameters via an XML file.
			The XML file is created per FEAPostProcessingParameters.xsd.  With this change 
			FEAResultAnalyzer.exe is no longer used.  ABQ_CompletePostProcess.py supplants
			FEAResultAnalyzer.exe.

v1.3.0.0 - 6/25/2012	Support of the new schema for custom geometry for identifying FEA mesh grid points.  This release
			supports the new schema but does not implement the custom geometry functionality. It also does not
			implement interior only, boundary only, interior and boundary, surface only, and interior and
			surface functionality.  Added support for datum points when assembling parts/sub-assemblies. 	


v1.3.1.0 - 6/27/2012	Modified the code to allow the first component to be added into the assembly even if the added
			component does not have the default datums (front, top, and right).  If the component added first does
			not have said datums, then it will be positioned with its coordinate system aligned to the assembly's
			coordinate system.  Corrected problems with the integration of the new FEA schema.   Allowed 
			CADUnitsAngle to be N/A.  Corrected a problem with the Nastran coordinate system ID not being 
			initialized properly  

v1.3.2.0 - 6/28/2012	Used Beta Version of UDM. 
			(http://build.isis.vanderbilt.edu/job/UDM_x64_msi/label=build-slave6/lastSuccessfulBuild/artifact/trunk/WiX/Udm_x64.msi)


v1.3.3.0 - 6/29/2012	Link against UDM 3.2.11, which is an official release.

v1.3.4.0 - 7/23/2012	Changed the way the environment variables are read and interpreted so that the bat file could be simplified. 
			This was follow-up work to the changes that were made to read the location of Creo from the Windows registry. 
			This version supports both the old bat file format
			(%EXE% %PROE_START_CMD% "%PROE_ISIS_EXTENSIONS%\." %WORKING_DIR% %ASSEMBLY_XML_FILE% %LOG_FILE% %EXIT_PROMPT%) 
			and the new bat file format 
			(%EXE%  %WORKING_DIR%  %ASSEMBLY_XML_FILE%  %LOG_FILE%  %EXIT_PROMPT%).	


v1.3.5.0 - 7/27/2012	For a parametric part that occurs more than once, copied the instances beyond the first instance to unique names.
			Added bounding box information to the metrics file.  By default the bounding box information will be created in 
			the metrics file; however, if the input xml contains a construct such as 
			“<BoundingBox ComponentID="100000445"  MetricID="987654321"/> “, then the MetricID will 
			be associated to the bounding box information in the metrics file.  If the input xml contains <DataExchange …> 
			entries, the STEP files per the DataExchange instructions will be created.


v1.3.6.0 - 7/27/2012	Added the capability to set %PARTS_DIR% 
			(e.g.    %EXE%     %WORKING_DIR%   %PARTS_DIR%   %ASSEMBLY_XML_FILE%     %LOG_FILE%     %EXIT_PROMPT%) in the 
			.bat file.   With this change, two formats of the .bat file are supported.  The new version containing the 
			%PARTS_DIR% and the old version (e.g. %EXE%     %PROE_START_CMD%    "%PROE_ISIS_EXTENSIONS%\."     
			%WORKING_DIR%      %ASSEMBLY_XML_FILE%     %LOG_FILE%     %EXIT_PROMPT%).   There was an intermediate version,
			which was not implemented by CyPh2CAD but was implemented by CADCreoParametricCreateAssembly.exe.  The 
			intermediate version is deprecated.   

v1.3.7.0 - 8/9/2012     Added a message indicating that STEP files are being created.  Added the .stp suffix to the created
			STEP files.  Modified bounding box types to have MetricIDs for Length, Width, and Height.  Previously,
			there was only MetricID for the entire bounding box.  With each run that requests computations, output
			the file ComputedValues.xml.

v1.3.8.0 - 8/16/2012    If an old version of ComputedValues.xml exists, then this program deletes the old version.  This was
			necessary to prevent the old version from being used as current/valid computed values.  Added the 
			capability to identify geometry for FEA via CAD_INTERIOR_ONLY, CAD_BOUNDARY_ONLY, and 
			CAD_INTERIOR_AND_BOUNDARY and for cylinders via CAD_INCLUDE_END_CAPS and CAD_EXCLUDE_END_CAPS. Corrected
			the reading of the log file name from the bat file.  Stopped supporting two formats for the bat file
			invocation of this program.  Now only support 
			%EXE%     %WORKING_DIR%   %PARTS_DIR%   %ASSEMBLY_XML_FILE%     %LOG_FILE%     %EXIT_PROMPT%

v1.3.9.0 - 8/16/2012    Corrected a problem with %PARTS_DIR% containing spaces.  Double quoted this string prior writing it to 
			the config options.
			   I
v1.3.10.0 - 8/17/2012   Corrected the problem that the ComputedValues.xml file did not contain all the values requested in 
			the input xml file via the <Computations>...</Computations> construct.

v1.3.11.0 - 8/21/2012   Added support for computations for center-of-gravity and named-points.  Updated to the new 
			CADPostProcessingParams.xsd.

v1.3.12.0 - 8/22/2012   Corrected a problem with the assembly structures not being initialized properly when
			computations were not present. 

v1.3.13.0 - 8/30/2012 	Added the export to STEP for the unused (i.e. unassembled) components. Create a config.pro and 
			search_META.pro files in the working directory.  This provides for locating the CAD files residing
			in a library.

v1.3.14.0 - 9/14/2012	For ComputedValues.xml, grouped metrics based on component ID.  Previously, the 
			<Component ComponentID="100000142" FEAElementID="" _id="id2a3e"> tag could appear 
			multiple times for the same ComponentID.  Now all metrics for a particular ComponentID
			are grouped under one tag.
		
				
v1.3.15.0 - 10/22/2012	Corrected a bug in function IfFileExists_DeleteFile that was displaying a "File Not Found" 
			message if the file did not exist.

v1.3.16.0 - 10/30/2012	Added "set PYTHONPATH=%PROE_ISIS_EXTENSIONS%bin" to the Abaqus bat file created by this program.
			This was necessary because ABQ_CompletePostProcess.py imports ComputedMetricsSummary.py, and
 			PYTHONPATH defines the path to ComputedMetricsSummary.py.


v1.3.17.0 - 11/6/2012	Added support for specifying the working dir as "set WORKING_DIR="."".  Previously, the
			full path to the working dir was required.

v1.3.18.0 - 11/7/2012   Modified config.pro to a have a relative path to search_META.pro.  config.pro now has the
			line "search_path_file .\search_META.pro".

v1.3.19.0 - 11/15/2012  Added support for assembling (i.e. constraining) via coordinate systems.  This
			was necessary to support positional constraints.

v1.3.20.0 - 11/28/2012  When a parametric part is used more than once in an assembly, a copy must be created 
			so that the parameters for each instance could be saved with the particular instance.
   			This was being done; however, for the case where the original was in a library, the copy
 			failed to locate the original.  This revision forces the original to be saved in the
 			working directory before performing the copy.   

v1.3.21.0 - 11/29/2012	Revision v1.3.20.0 was saving the source parametric model to the working directory 
			each time for multiple instances of the parametric part.  This modification assures that
 			the source model would be saved only one time.	

v1.3.22.0 - 12/7/2012	Added support for editing existing string parameters in the Creo model.  Previously,
			only real-numbers, booleans, and integers were supported. 

v1.3.23.0 - 12/10/2012	Stored the ComponentID as a parameter(DDP_COMPONENT_INSTANCE_ID) in the assembly feature
			for each added part/sub-assembly.  Modified the STEP output process to write the
			parameters to the STEP files. DDP_COMPONENT_INSTANCE_ID parameter will appear in  
			AP203_E2_SEPERATE_PART_FILES, AP203_E2_SINGLE_FILE, AP214_SEPERATE_PART_FILES, and 
			AP214_SINGLE_FILE.  It will not appear in AP203_SINGLE_FILE. Modified the way regeneration
			works.  If the first regeneration fails, regeneration is tried a second time. This
			was necessary because when using Creo interactive, in some cases, the first regeneration
			attempt fails, and the second attempt succeeds.


			BACKWARD COMPATIBILITY - From this point on all v1.3 versions must be backward
			compatible to v1.3.24.0 

v1.3.24.0 - 12/20/2012  1) Changed the default template for creating assemblies from zzz_template_assy_mmns_creo.asm.1
			to zzz_template_assy_mmKs_creo.asm.1.  This was because Ricardo models, by default, use
			Millimeter Kilogram Sec (mmKs) as the units.  This was needed to address the defect
			"Erroneous Mass Properties (Models with Different Units)" described under "Known Defects"
			below.  
			2) Modified the metrics file (metrics file version v1.1.0.0) as follows: 
				a) Modified the metrics schema (i.e. CADMetrics_Uml.xme) to include Anomalies and changed
				the cardinalities of Scalars (now 0..1), InertiaTensor(now 0..2), and 
      				PrincipleMomentsOfInertia (now 0..1). 
				b) The metrics output file previously contained the inertia tensor at the default coordinate
				system.  Added the intertia tensor at the center-of-gravity. 
			3) Modified the creation of the metrics file to always create the metrics files.  Before,
			   if one or more of the Creo models did not contain material properties, then the 
			   metrics file was not created.  Now the metrics file is always created; however, if 
			   material properties are missing, then the errors are reported in the the Anomalies section
			   of the metrics xml file.  Also, the error is logged and displayed to the console screen. 


v1.3.25.0 - 01/10/2013  Added model name to the logged mass-property-errors. Deleted old metrics file (if it exists) prior  
			to creating a new metrics file.  No longer create search_META.pro.  This file is now created 
			by CyPhy2CAD.

v1.3.26.0 - 01/11/2013  Corrected a problem with the new search path approach.  The search_META.pro contents were 
			not being loaded into the Creo session.

v1.3.27.0 - 01/18/2013	Corrected a problem that a second Creo assembly regeneration was not being invoked if the 
			first attempt fails.  Now, two attempts will be made before an error message would be
			reported.  If both attempts fail, then ComputedValues.xml file, STEP files, and the metrics 
			file would not be created.  The error messages and logging were improved for anomalies that occur
			during regeneration. 

v1.3.28.0 - 01/18/2013  For part mass properties, removed the requirment that density!=1.00.  If density==1.00, it 
			would indicate that mass properties have not been set and cannot be trusted for inclusion 
			in the metrics file.   Later, a better check should be implemented to validate the integrity 
			of the mass properties. 

v1.3.29.0 - 01/23/2013  Updated manufacturing.manifest.json with the name of the STEP file for each component
			instance for the case where AP203_E2_SEPERATE_PART_FILES and/or 
			AP214_SEPERATE_PART_FILES were requested. If only the SINGLE_FILE options were requested, 					then  manufacturing.manifest.json would not be updated.  If .\manufacturing.manifest.json					does not exist, then logging would indicate that no action occurred.	

v1.3.30.0 - 01/25/2013  Modified the update to manufacturing.manifest.json to include the STEP file directory 
			(e.g WAS: "STEPModel": "DAMPER.stp"
			     NOW: "STEPModel": "AP203_E2_SEPERATE_PART_FILES/DAMPER.stp" ).  
			Corrected the error that a \/ was written to manufacturing.manifest.json where only / 
			should have been written.

v1.3.31.0 - 02/4/2013	Modified the update to manufacturing.manifest.json to name the STEP files with a _prt 
			prior to the .stp suffix.  When Creo outputs STEP files with separate part-files option, 
			only the part files are created separately and are named with _prt. The sub-assemblies are 
			not created.  The typical hierarchy consisting of an assembly with multiply layers of 
			sub-assemblies and then detail parts is represented with a top assembly STEP file and 
			detail part STEP files.  The assembly hierarchy consisting of the top assembly with a  
			hierarchy of sub-assemblies is represented in the top-level assembly STEP file.
			(e.g WAS: "STEPModel": "AP203_E2_SEPERATE_PART_FILES/DAMPER.stp"
			     NOW: "STEPModel": "AP203_E2_SEPERATE_PART_FILES/DAMPER_prt.stp" ). 

v1.3.32.0 - 02/8/2013   If the user has META Tools x+0, CAD jobs sent to an x+4 server will fail. They fail 
			because starting with X+2, the Creo search path file (search_META.pro) was created 
			via CyPhy2CAD. With x+0, the search path file was created via 
			CADCreoParametricCreateAssembly.exe. With the combination client x+0 and server x+4, 
			the search path is never created. This results in CADCreoParametricCreateAssembly.exe 
			not being able to find the CAD models. The correction in this release is for the 
			case where search_META.pro does not exists, then CADCreoParametricCreateAssembly.exe 
			would create it.  The create search path is composed of the bat file PARTS_DIR
			and .\Cad_Directory if this directory exists.
			X+0 Meta 13.0, released 1/14/2013
			X+2 Meta 13.2  released 1/28/2013
			X+4 Meta 13.4  released 2/11/2013

			
v1.3.33.0 - 02/20/2013  Added the creation of _FAILED.txt if an error occurred.  Populated _FAILED.txt with the 
			error code and instructions to see the log file.  When an error occurred, wrote the 
			error to std::err instead of std::cout. Moved the log file to the .\log directory.
			Corrected an erroneous message that the ComputedValues.xml was being created even
			when compute values were not requested.


v1.3.34.0 - 02/25/2013	Added supported for computing mass.  The mass value is written to ComputedValues.xml. 
			Improved the error message for isis_ProModelitemByNameInit to include the model name 
			in the error message if the named feature was not found.

v1.3.35.0 - 03/06/2013	Prefixed the log directory with CAD_.  When adding the STEP part file locations to 
			manufacturing.manifest.json, converted the STEP file name to lower case.  This
			is because Creo always creates the STEP part file names with lower case names.

v1.3.36.0 - 03/13/2013	Removed the error condition if there were no assemblies defined the input xml file.  
			Now, if there are no assemblies, then that condition is logged.  If there are no
			assemblies and no unconnected parts/sub-assemblies then that is considered 
			an error and is logged and _FAILED.txt updated.  Added checks to assure that 
			model names and feature names do not exceed 31 characters.  Modified the names
			for the created STEP files to have a suffix (i.e. _asm.stp/_prt.stp) that indicates
			if the STEP file represents a part or assembly.  Added an error check if a component ID
			was used more than once.  Added logging of the xml input information for C.G., 
			Mass, Bounding Box, and Point metrics.
			
v1.3.37.0 - 03/18/2013	Added checks for ComponentID=”” and FeatureName"".  For initial errors, changed the
			starting message from "std::exception" to ERROR. For ProMdlRetrieve and ProModelitemByNameInit
			function calls, added additional information (Component ID, Model-Name/Type) to the error messages.
			Added notes to the top of the log file that explain the mapping of ComponentIDs to CyPhy and
			AVM IDs.  If two regeneration attempts fail, then regenerate the immediate subordinate parts and
			log the success/failure.  This gives a way for the user to know which parts would not
			regenerate. Treated the case where a regeneration fails as a _FAILED type error.  This
			will provide the correct status to the job manager.


v1.3.38.0 - 05/13/2013  Added the class MultiFormatString, which verifies that the Creo string length requirements are
			not violated.  Modified the isis_ptc_toolkit_functions.h/.cpp functions to pass strings as const
			and cast away constantness at the call to the actual Creo function. Error messages that result 
			from non-zero return codes from Creo functions now display the non-zero number and the associated
			enum string. Added support for ballistic analysis via adding reference plane and shot lines to
  			BallisticConfig.json.  Currently, only support a ground plane reference plane.  Will add
			waterline reference plane later.  Modified the template models ( Proe ISIS Extensions\templates\...)
			to be commercial models.  This should allow sites with commercial Creo licenses to run our tools.
			Change ComponentID in the in the metrics file to ComponentInstanceID.
			 
v1.3.39.0 - 06/17/2013 	Added support for Creo 2.0.  Both Creo 1.0 and 2.0 are supported.  Support for Creo 1.0 will be 
			dropped in the future.  If both Creo 1.0 and 2.0 are on the same machine, Creo 2.0 will be used.

v1.4.0.0 - 06/20/2013	Upping the version number to 1.4 to indicate that Creo 2.0 is supported.  Should have made 
			the version number change to 1.4 with the previous revision(i.e. 1.3.39.0).  Added support
			for switches ( -i <input xml file> -l <log file>...) on the input arguments to 
			CADCreoParametricCreateAssembly.exe.  Changed the string ComponentID to ComponentInstanceID in 
			the log file.

v1.4.1.0 - 07/05/2013	Added support for Meta-Link.  Supported commands are AddComponentToAssembly, ConstrainComponent,
			and ModifyParameters.

v1.4.2.0 - 07/23/2013	Added the creation of the ComputedValues.xml for CFD runs.  Added the creation of 
			Stereolithography(STL) files.  Modified the three points created by the function
			ComputeVehicleGroundPlane such that the Vector ( Pt_0 to Pt_1 ) X Vector ( Pt_0 to Pt_2 )   					defines the upward direction for a vehicle.  For the ComputedValues.xml, file changed 
			ComponentID to ComponentInstanceID.  Added the creation of a ground plane in
			ComputedValues.xml.  The ground plane is always computed for now.  Later, CyPhy2CAD will 
			request the ground plane and at that time the code that automatically creates the ground
			plane will be removed.  Switched to linking against the Creo 2.0 SDK libraries instead of the 
			Creo 1.0 libraries.  

v1.4.3.0 - 07/26/2013	Added support for blast analysis.  Currently support writing the reference plane 
			(i.e. ground plane) to BlastConfig.json.  Later will add better ways to position the 
			blast source location.  

v1.4.4.0 - 07/30/2013	BlastConfig file's reference plane section is slightly different than BallisticConfig file.
 			Modified code to generate correct reference plane for BlastConfig. BlastConfig should not 
			contain any shotLines tag. Fixed a bug where empty shotLines tag was added to BlastConfig.
	
v1.4.5.0 - 08/03/2013	Added MetaLink capablity this includes the expression of INTEREST in CADASSEMBLY posts 
			and the ability to receive those posts. 

v1.4.6.0 - 08/13/2013	Changed the transformations matrices in the metrics file from transforming the part
			coordinates into the assembly coordinates to the assembly coordinates into the part 
			coordinates.  The new approach provides the transformation matrix for positioning a part
			the assembly. Changed MetaLink support to no longer initialize the assembly via an XML file.
  			Now, the only supported option is to send a message from the bridge, where the message 
			contains the XML that defines the assembly.  For MetaLink, added support for subscribing 
			with the bridge based on a DesignID.  Modified the creation of STEP file so that STEP files are
 			created for the top assembly and all sub-assemblies.  Previously, STEP files were only created 
			for the top assembly.  In the metrics file, added ComponentInstanceID attribute to the 
			ChildMetric tag.  For ballistics analysis, if there were no shotlines (typical for defining
			predefined suites), then modified the code to not include a sholines construct in the 
			BallisticConfig.json.  For MetaLinkAssemblyEditor::AddComponentToAssembly, if 								in_ParentAssemblyInstanceID could not be found in the internal data structures, added
			a |1 suffix to in_ParentAssemblyInstanceID and tried again to locate in_ParentAssemblyInstanceID.
			When in the MetaLink mode, the top assembly will always be refered to with a |1 suffix.
			A |2 suffix would indicate that there were islands in the the CyPhy assembly, and islands
			are not allowed when connecting to CyPhy via MetaLink. In the top assembly, in MetaLink mode
			added parameter CYPHY_DESIGN_ID to the top assembly. In MetaLinkHandler::process_ComponentAdded,
			added a check for in_component.componentid() being zero length.  Ignored messages with 
			zero length componentids.
			

v1.4.7.0 - 08/14/2013	Modified the code such that MetaLinkAssemblyEditor::AddComponentToAssembly always adds the
			component to the top assembly.  This is a workaround because there is not a good way to 
			map the assembly instance ID in CyPhy to the Creo model.

v1.4.8.0 - 08/14/2013	In MetaLink mode, added support for setting search paths. 
	
v1.4.8.1 - 08/20/2013	Converted from make files to Visual Studio projects.

v1.4.9.0 -08/27/2013	Corrected the "Data Network Error".  This was caused by calling CloseNoUpdate() on a DataNetwork
			that was not open.  Added support for the new schema for representing requests for metrics.
			The old schema used AnalysisComponents tag for FEA, Computations tag for CFD, ballistics and 						blast previously did not not have requested metrics, and assembly metrics (e.g. bounding box,point....)
			used specific tags such as BoundingBox.  For field-of-view, a refrence plane is always created.
			This needs to be changed to request a reference plane.  The new approach uses a Metrics tag for
			all cases. Corrected a problem that the units for the points defining the reference plane for
 			ballistics and blast were not being converted to meters. 

v1.4.10.0 -09/5/2013	Changed from reading the material properties (e.g. density, tensile strength,...)
			from the CyPhy2CAD output xml to the materials library. Changed the Abaqus
			bat file (in directory Analysis\Abaqus) name to runAnalysis.bat.

v1.4.11.0 -09/10/2013	Added ABAQUS_DECK_BASED and ABAQUS_MODEL_BASED to AnalysisSolverType_enum.  Removed the
			hard coded location of the materials library and replace with a call to 
			isis_CADCommon::GetPublicDocuments.  Removed the line "set /p ExitPrompt= Type Enter to exit"
			from the Abaqus runAnalysis.bat. 

v1.4.12.0 -09/10/2013	Added code to delete the directory Analysis\Abaqus if it exists prior to creating the
			bat and xml files necessary to run an Abaqus job.  This was necessary because if a
			Abaqus job-manager job was resubmitted then the old Abaqus files (db, deck...) were
			causing a prompt message to delete the old files before continuing. This prompt was
			hanging the job.

v1.4.13.2 -09/12/2013	Modified MetaLink such that it will not save the assembly upon creation of the 
			assembly. In function MetaLinkAssemblyEditor::ClearAssembly(), called 
			isis_ProMdlEraseNotDisplayed(). 

v1.4.14.0 -09/13/2013	Corrected a problem with setting the unit types in structure AnalysisMaterialProperties.
			Also, corrected an error intializing strength_fatigue property from the material library.

v1.4.15.0 -09/16/2013	Per v1.4.6.0, STEP files are being generated for sub-assemblies.  With this change, 
			manufacturing.manifest.json is now being updated to show the location of the 
			sub-assembly STEP files.  

v1.4.16.0 -09/23/2013	When the CreateAssembly code was refactored to support MetaLink the initialization of the 					UniqueNameIndex variable was omitted.  Added back that initialization 
			(i.e. int UniqueNameIndex = 1;) in file CADCreoParametricCreateAssembly.cpp.

v1.4.17.0 -10/02/2013	Updated the metrics file to complete the information for the subordinate
			parts/sub-assemblies for assemblies that are leafs in CyPhy. Also, updated the 
			metrics file to have the ComponentDefinitionSource attribute 
			(e.g. <MetricComponent ComponentDefinitionSource="CYPHY" ).  This attribute can have the
			values of CYPHY or CAD_MODEL.  Corrected a problem with FEA where acceleration loads
			were resulting in a error. The error was caused by the function 								GetGridPointsWithinAnalysisGeometry being called for acceleration loads, but acceleration 
			loads apply to the entire assembly, and thus do not have grid points.  Corrected the
			spelling "Seperate" spelling error.  Now the STEP directories 
			(e.g. AP203_E2_SEPARATE_PART_FILES) are spelled correctly.  Transposed the rotation matrix
			in the metrics file.  This results in the same transformation matrix that is returned
			via retrieving the transformation matrix (assembly to part) via the Creo UI.

v1.4.18.0 -10/04/2013	For ballistics, CyPhy2Cad was modified to add the ShotLineName to the xml created by
			CyPhy2Cad. This program was modified to read the ShotLineName and persist it in 
			BallisticConfig.json.  The data exchange code (e.g. STEP, STL...) was modified to only 
			export a model one time.  For example if aaa.prt appeared 20 times in an assembly, now it will
			only be exported one time.
			  

v1.4.19.0 -10/07/2013  	The return statement in CombineCreoModelNameAndSuffix was inside a switch block.  It 
			should be after the switch block.  

v1.4.20.0 -10/08/2013	Changed the code to only complete the metrics file, for the case where an assembly is
			a leaf in CyPHy, for ballistic and blast analysis.  The other analyses 
			(CFD, FEA, field-of-view) do not need the extra information.  In the function
			ComputationType_enum, added CenterOfGravity and PointCoordinates.  These are 
			other ways of specifying CG and Point.

v1.4.21.0 -10/15/2013   Previously, parametric parts that occurred more than one time in the assembly were 
			copied to new names.  This was necessary because the parameters apply per model name.  
			This change copies assemblies was done previously for parts.  There was a problem
			copying family tables that will require further investigation. 

v1.4.22.0 -10/25/2013   Modified the code to try three regens before giving up.  Previously, only two 
			regens were tried.  Corrected a problem with outputting the name of the output 
			file in the function isis_ProOutputFileWrite when writing the output file failed.   
			Added the function isis_ProSolidMassPropertyGet_WithDescriptiveErrorMsg and 
			called it when getting mass properties.  This function provides a better error 
			message.  Added a check for when the Possion’s ratio keys in the Nastran deck
			could not be found.  If a key was not found a descriptive error message would 
			be returned.  The key not found condition would probably be because Creo Simulate 
			was used to define material properties.    

v1.4.23.0 -10/29/2013   Refactored the projects so that MetaLink is in a separate project.  Reverted back
			to using clog.

v1.4.24.0 -11/4/2013   	For ABAQUS_MODEL_BASED FEA, stopped producing a Nastran deck.  Added partial support for 
			Nastran.  The release does not create the bat file to invoke the Nastran analysis.  Added
			support for Inventor format and requesting that the metrics file be completely filled for
			the case where a CyPhy assembly is a leaf.

v1.4.25.0 -11/6/2013	Previously, the material information was retrieved from two sources: 1) CyPhy material 
			specification as conveyed by the CyPhy2CAD XML and 2) Creo model material setting.  
			This change makes the Creo part material setting the master and the sole specification 
			of the material for a part.  If the CyPhy material differs from the Creo part setting, 
			then a warning would be issued and the anomalies logged.

v1.4.26.0		No change, incremented for build purposes.

v1.4.27.0		Previosly, .\Analysis\Nastran\runAnalysis.bat was just a shell (i.e. no contents).  
			Added to this bat file the instructions to invoke the Nastran scripts.  The Nastran
			scripts invoke the Nastran solver and post processing.  Post-processing produces
			the stress contour plots and the update of summary.testresults.json.  Updated
			.\Analysis\Nastran\runrunAnalysis.bat to use the version of Python installed by the
			Meta tools.  The manufacturing.manifest.json format changed. The instance ID was 
			previously identified by "id":.  Now it is identified by InstanceGUID.  Updated
			the function AddStepFileMappingToManufacturingManifest to reflect this change. 
			Added the routine SearchForKeyMatch for finding if the Poisson's ratio key
			*_Nas_org.nas matches the keys in this program.  The keys 
			could not match because a string comparison would yield that -.1112 was not 
			equal to .11120.  Converting the two strings to a double and then comparing yields
			the correct results.

v1.4.28.0 12/4/2013	Added support for FEA analysis of assemblies that are leaf assemblies in CyPhy.  
			Now loads and constraint can applied to assemblies.  Updated AnalysisMetaData.xml
			to express the assembly structure so that FEA post-processing can return metrics
			for an entire assembly.
			For CADUnitsAngle_enum support DEGREES.  Previously, DEGREE was supported but not
			DEGREES.  For CADUnitsDistance_enum support MM. Previously, MILLIMETER was 
			supported but not MM.  Forced all parametric parts/sub-assemblies to have a suffix
			of the form _1Z, _2Z, _3Z...  Originally the first occurrence of the parametric 
			part/sub-assembly did not have the suffix; however, because that part/sub-assembly 
			may be used in a sub-assembly that is a leaf in CyPhy, we must make all known
			occurrences have the suffix.  Also, note that the suffixes are of the form 
			_<number>Z, where previously they were of the form Z<number>Z.
 			Added a switch on the function BuildAssembly to 
			allow unconstrained components in assemblies.  This switch will be set by 
			MetaLink but not other uses of this program.  Starting with this release, an
			exception will be thrown if any components are unconstrained and the previously
			mentioned switch is not set. Initialized specialInstruction(CAD_SPECIAL_INSTRUCTION_NONE)
			in the constructor for struct CADComponentData.  For the creation of STL files,
			added the config option intf3d_out_prop_chord_heights set to yes.  When reading the
			material library, changed the units for density from "kg/m3" to "kg/m^3".  The
			material library now has entries for properties that have no values.  If the
			property should have a unit, but does not, now this program assumes that the 
			property is not defined. 


v1.4.29.0 12/6/2013	Added CAD_FEATURE_GEOMETRY_FACE e_CADFeatureGeometryType to support FACE features for 
			model-based Abaqus FEA. Added the new Patran_PP.py script, which supports
			post processing assemblies that are leafs in CyPhy.

v1.4.30.0 12/9/2013	For model-based Abaqus FEA, created AnalysisMetaData.xml and corrected the problem that
			the entire set of operations (i.e. completely filled out metrics file and all 
			parts/sub-assemblies renamed to unque names) was not being completed
			for model-based Abaqus FEA.  Corrected the problem that the FACE FEA geoemtry type for 
			model-based Abaqus was being checked against FeatureGeometryType instead of GeometryType.


v1.4.31.0 12/18/2013 	Added support for simplified reps.  Added the attribute Representation 
			(e.g.  Representation="Defeatured_Rep")to the input xml.  If the simplified
			rep is not present in the Creo model, then the active rep would be used.  It
			is not a fatal error if the rep is not present.  The not present condition would
			be logged.  The supported rep types follow:
			Model Type	Reps Supported
			----------  	--------------
			Part		Master Rep
			Part		<user defined (e.g. Featured_Rep, Defeatured_Rep, My_rep...)>
			Assembly	Default Envelope Rep
			Assembly	<user defined (e.g. Featured_Rep, Defeatured_Rep, My_rep...)>
			
			Note: "Master Rep" for assemblies is not supported. 
			
			Initialized more of the attributes in the CommonStructures.h structures via the
			constructors.
			

v1.4.32.0 12/20/2013	Modified the function PopulateMetricsInformation to skip computation types of 
			Modes and Dynamics. These types are currently only supported for model-based FEA.
			They will be added later to deck-based FEA.  Modified the metrics file such that
			a particular component only appears one time.  Due to a previous bug, if there
			were multiple instance of the component, the component information would appear
			multiple times.  For this release, CyPhy2CAD CADAssembly.xml supports hydrostatic
			attributes, but for now this program will ignore that information.


v1.4.33.0 01/06/2014	In the function ModifyToHaveAUniqueName_ForEach_PartAndOrAssembly, added a check
			for the case where the component information did not contain a Creo model name.
			This could happen if a metric was requested, via in the input xml, on a 
			ComponentInstanceID where the ComponentInstanceID did not actually exist. Changed
			from tying to regenerate the assembly from three tries to five tries.  This
			was necessary because some assemblies required four tries before successfully 
			regenerating the assembly.  log4cpp was added to all of CreateAssembly functions.
			Added the creation of the <assembly name>_inteference.txt file.  This file would
			be created each time the assembly program runs; however, for large assemblies, this
			takes a considerable amount of time.  In the future, we may only want to run this
			on demand.  Also, for some large assemblies, the interference analysis fails.  This
			failure is logged and does not stop the rest of the program to complete.  Added the
			computation of the waterline for the case where the input XML file contains
			the tag <HydrostaticsSolverSettings...>.  The waterline information is added to 
			the file hydrostatics.json.

v1.4.34.0 01/09/2014	For MaterialType_enum, added support for fluid, composite fiber, hard_plastic, 
			stainless, and fuel.  Added a more descriptive error messages when reading the
			material library results in errors.  Added a more descriptive error message when
			retrieving model units (i.e.ProMdlPrincipalunitsystemGet) results in an error.
			Changed the hard-coded value for the density of fresh and saltwater to be in the
			units of kg/mm^3.  Previously the units were kg/m^3.  Corrected the sign of the
			waterline computation. If the assembly would sink, set the waterline to the 
			maximum z coordinate of the bounding box.

v1.4.35.0 01/13/2014	Changed from computing the bounding box via isis_ProSolidOutlineGet to 
			isis_ProSolidOutlineCompute. The bounding box for a ComponentInstanceID
			is computed once and stored in the CADComponentData structure.  Subsequent 
			bounding box requests use the previously computed values.

v1.4.36.0 01/15/2014	Turn off interference checks.  Will add it back in later on a per requested
			basis.

v1.4.37.0 01/20/2014	Removed	CADHydrostaticParameters.vehicleVelocity from the internal data structures.
			This parameter was not used by this program.  Added the invocation of interference
			checking based on <Analyses> <Interference> tag existing in CADAssembly.xml.
			Improved the error when a bounding box computation fails.

v1.4.38.0 01/23/2014	When computing the bounding box, an exception was occurring if the model contained
			no solid geometry.  A use case would be a model that only contains sketches, which
			might be used to define an interface. If the exception occurs, a check of the volume 
			would be conducted, and if the volume was zero then the bounding box dimensions would 
			be set to zero and processing would continue. The zero volume/bounding box condition 
			would be logged. See AssembleUtils.cpp:RetrieveBoundingBox_ComputeFirstIfNotAlreadyComputed.
			Made two changes when filling out the subordinate information to .asms that are leafs 
			in CyPhy: 
			1) Treated the case where a subordinate .prt/.asm was not in memory as a normal 
			   (i.e. non-error condition). See function CommonFeatureUtils.cpp::user_action. It 
			   would not be in memory because it was not included in a simplified rep. 
			2) When filling out the subordinates to a CyPhy-leaf .asm, ignore .prt/.asm 
			   that have been exclude by a simplified rep.  The contents of the simplified rep 
			   table (i.e. include/exclude) are filtered if the .prt/.asm was set to exclude. 
			   See AssemblyUtils.cpp::ForEachLeafAssemblyInTheInputXML_AddInformationAboutSubordinates.  
			   This is necessary because if a .prt/.asm that was included in the simplified rep references
			   an excluded .prt/.asm, the exclude .prt/.asm would be pulled into memory and thus 
			   would be found when gathering information on the child .prts/.asms.
			When exporting to STEP and STL, retrieved all models (including models subordinate
			to the top assembly) with the simplified representation for the particular.prt/.asm.

			Bug introduced this release:  The following error occurs if a simplified rep excludes
			models (.prts/.asms) that do not have references to the included models.  The problem
			is that the bounding box computation is trying to reference the excluded models, which
			are not in memory.  The error follows:
				application error: Failed to retrieve bounding box information:
   				Model Name:            TwoPlates
   				Model Type:            PRO_MDL_ASSEMBLY
   				Component Instance ID: 3_1
   				Exception Message: exception : ProSolidOutlineCompute returned ProError: 
								PRO_TK_GENERAL_ERROR(-1)

v1.4.39.0 01/24/2014	Modified the code to use the less-accurate bounding-box calculation (ProSolidOutlineGet)
			if the more accurate computation (ProSolidOutlineCompute) fails.  For model-based FEA, 
			anomalies that occur when looking up material properties from the material library
			are no longer considered fatal.  This is because model-based FEA might not need those
			material properties.  If the .prt/.asm was treated as a rigid body, then the material
			properties would not be needed for model-based FEA.  If model-based FEA does need the 
			anomalous material properties, then model-based FEA would fail and log the condition.


v1.4.40.0 01/24/2014 	For model-based FEA, also treated lack of material properties (e.g. no yield strength)
			as a non fatal condition.  The condition is logged.


v1.4.41.0 01/30/2014 	Added pro_material_dir to config.pro.  An example follows  
			pro_material_dir C:\Users\Public\Documents\META Documents\MaterialLibrary\MATERIALS_CREO_MTL.

v1.4.42.0 02/05/2014	When writing the metrics file, logged an error if a non-positive-definite inertia
			tensor was retrieved from the Creo model for the inertia tensor at the default coordinate
			system or the inertia tensor at the center-of-gravity.

v1.4.43.0 02/25/2014	Added support for using parametric.psf when starting Creo.  The algorithm follows:
			if ( parametric.psf exist)
				Add "<Creo Install Dir>\bin\parametric.psf %*" to the end of the Creo start command.
			An example follows for the non-graphics mode (Note-MetaLink uses the graphics mode):
			if ( C:\PROGRA~1\PTC\CREO2~1.0\PARAME~1\bin\parametric.psf exists)
				C:\PROGRA~1\PTC\CREO2~1.0\PARAME~1\bin\parametric.exe -g:no_graphics -i:rpc_input C:\PROGRA~1\PTC\CREO2~1.0\PARAME~1\bin\parametric.psf %*
			else
				C:\PROGRA~1\PTC\CREO2~1.0\PARAME~1\bin\parametric.exe -g:no_graphics -i:rpc_input				


v1.4.44.0 02/25/2014	Modified NastranDeck class to parse input to allow modification. Previously, the

			input was stored as strings.

v1.4.45.0 03/07/2014    Added support for FEA FREE and FIXED displacement and rotation constraints.


v1.4.46.0 04/2/2014	Added support for a Guide constraint.  This is a constraint that would be applied for the
			purposes of defining an initial position and then removed.  This was necessary for kinematic
			joins so that an orientation could be selected for the case where there would be multiple
			possible configurations, or for the case where the part could be positioned close to
			a viable position so that the Creo constraint engine could successfully compute the orientation
			of the models.  Modified the sort for determining the order of adding .prts/.asms into 
			the assembly so that .prts/.asms with a guide constraint would be given precedence. Added 
			a INFORMATION: prefix to the log message for the case where the manufacturing manifest
			was not created. In ApplyModelConstraints.cpp added a call to ProAsmcompconstraintAttributesSet 
			with argument PRO_ASM_CONSTR_ATTR_FORCE|PRO_ASM_CONSTR_ATTR_IGNORE.  This was per 
			PTC support to force under-constrained assembly models to be treated as kinematic 
			joints. 

v1.4.47.0 04/22/2014 	Added Joints to the CAD_Uml.xme.

v1.4.48.0 05/14/2014 	Changed from calling Creo toolkit functions to constrain a .prt/.asm to creating a 
			feature tree that describes the constraints.  Creating the feature tree supports the 
			full functionality for specifying the constraint information.  Previously, we were
			limited to the constraint functionality available through Creo toolkit SDK. 
			creating a constraint.  Treated an axis and plane constraint as a revolute (i.e. pin) 
			joint and defined the constraint as a revolute joint in the feature tree.  Currently, 
			not checking if the plane was orthogonal to the axis.  Will add that check later.

v1.4.49.0 05/21/2014 	Replaced the function that renames sub-assemblies/sub-parts in an assembly with a function
			written by Creo support.  The new function works across a broader set of scenarios.
			Added additional error checking to the feature redefine function that adds constraints.
			
v1.4.50.0 06/02/2014 	Added debug options to control/debug assembly creation. Regenerating intermediate assemblies up to 5 times.

v1.4.51.0 06/13/2014 	Added joint information to the metrics file.  Invoked the Joint.h routines to compute the joint
			type and location.   Modified isis::ComponentVistorBuildListOfComponentIDs 
			to support omitting the top-assembly.  Modified the SetConstraints function to take
			a structure that groups the constraints by set ID. Exported the joint information to the
			metrics file. R.O

v1.4.52.0 06/30/2014 	Modified the CreateAssembly program to be more instruction based and thus less dependent on 
			the type of analysis. To that end, added support for ProcessingInstructions (e.g. 								COMPLETE_THE_HIERARCHY_FOR_LEAF_ASSEMBLIES, UNIQUELY_NAME_ALL_CAD_MODEL_INSTANCES, and 
			OUTPUT_JOINT_INFORMATION ) in CADAssembly.xml.  Added the DatumRefResolver class, 
			which determines the owner (.prt/.asm) of a datum feature.  This function currently only works 
			with datum planes, axes, and coordinates system.  In the future, it will be expanded to support
			more types. For CyPhy leaf assemblies when OUTPUT_JOINT_INFORMATION was set, determined the type
			of joints between subordinate .prts/.asms and added that joint information to the metrics file.  
			Currently, this approach does not work for many use cases.  The most salient limitation is that 
			every part in the leaf assembly must be constrained to another part.  This is often not the case 
			for the first .prt added into an assembly, which would often be constrained to the parent .asm 
			but not to other .prts.  In the future, this case must be handled. With this release, kinematic 
			joints are always inferred (inferred from the constraining datums) and are used when applying 
			Creo constraints to assemble a .prt/asm.  For example, if the joint type was determined to be 
			a revolute joint, then the constraint set would be created as a revolute (i.e. pin) constraint 
			set.  This will cause problems with legacy models, which probably have erroneous types of 
			constraints that were not a problem in the past because the constraints were treated as 
			user-defined constraint sets.   S.N, R.O.

v1.4.53.0 07/15/2014	Refactored the code so the BuildAssembly does not by default determine the assembled-to references. 
			Those computations now only occur if OUTPUT_JOINT_INFORMATION is specified in CADAssembly.xml.  Added
			VALIDATE_JOINT_INFORMATION as a secondary processing instruction to OUTPUT_JOINT_INFORMATION.  
			Moved all the joint validity checking from being dispersed in multiple functions to one function, 
			which is only invoked if VALIDATE_JOINT_INFORMATION was specified.  In 
			PopulateMap_with_JunctionInformation_SingleJunction changed the order of constraints so that
			component_comp_constr_ref is always first.  This was necessary to assure the computation of the
			location of a joint was always relative to the assembled model (i.e. not the assemble-to model).
			Added support for the LEAF_ASSEMBLY_NO_KINEMATIC_JOINTS-TREAT_AS_ONE_BODY instruction for leaf
 			assemblies in CADAssembly.xml. For
			TREAT_AS_ONE_BODY assemblies, do not 
				1) output to the metrics file joint information between
				   parts internal to the leaf assembly 
				2) do not check connectivity between parts internal to
				   the leaf assembly 
				3) for kinematic joints internal to the leaf assembly do not verify that
				   the kinematic joints are well formed (e.g. only between two parts).  Do verify 
				   that at least one part in the leaf assembly is a constrained-to part by a 
				   part external to the leaf assembly. 
			Skipped computing the junctions and checking the integrity of junctions for leaf-assembly
			subordinates that do not have a leaf-assembly ancestor with the HAS_KINEMATIC_JOINT flag set.
			R.O.

v1.4.54.0 07/24/2014	Throw an exception for the case where a .prt/.asm is not constrained via datums or features.
			If it was not constrained, it would just be fixed in space.  Previously, this condition was
			just logged. R.O.

v1.4.55.0 08/12/2014	Ordered the metrics file such that the order/hierarchy of the Assemblies section would be congruent
			with the order for the Creo model-tree.  Modified the function OutputCADMetricsToXML such that
			it does not compute the bound box and mass properties.  These computations are now done 
			prior to calling OutputCADMetricsToXML and are persisted in CADComponentData_map. R.O.

v1.4.56.0 09/18/2014	Stopped outputting the ProcessingParameters xml file.  Deck-based Abaqus was the only
			consumer of the ProcessingParameters xml file, and it was modified to use 
			AnalysisMetaData.xml.  Modified Nastran\runAnalysis.bat to be compatible with 
			directory references that contain a parenthesis. R.O.

v1.4.57.0 10/14/2014	Added support for deck-based thermal.  This is sufficient for an end-to-end test, but needs
			more work to be ready for general use.  Issues are: 1) the thermal material properties should be read
			from the material library.  They are currently be sourced from the Creo model. 2) the CyPhy model does 
			not have ambient temperature for convection coefficient. and 3)heat generation is not supported.

v1.4.58.0 10/30/2014    Added support for volumetric heat generation (QVOL Nastran Card).  For convection, changed the source
			of the external temperature to the value in the following CADAssembly.xml entry:
 			<ThermalElement LoadType="AmbientTemperature" Value="243.99" Unit="C" _id="id112">.  R.O.
			
v1.4.59.0 04/10/2015	Added support for InterferenceCount CADComputationComponent.  This entailed changing the format
			of CADAssembly_interference.txt to have the string "InterferenceCount: ??", where ?? would be
			the number of interferences.  Also, modified UpdateReportJson_CAD.py to read 
			CADAssembly_interference.txt, if it exists, and update testbench_manifest.json.
			Corrected an unit error in ComputedMetricsSummary.py.  The stress allowables were in PA
			and should have been in MPa. R.O.
			Added CarTestBench and interperter CyPhyCar. S.N.


v1.4.60.0 05/01/2015	Modified ApplyModelConstraints.cpp to be a general constraint solution.  Supported FIXED_JOINT
 			and kinematic joints (REVOLUTE_JOINT, SPHERICAL_JOINT, PRISMATIC_JOINT, CYLINDRICAL_JOINT,
			and PLANAR_JOINT). 
			There are many geometry constructs that could be used to define kinematic joints; however, 
			Creo only supports a subset of those geometries.   For example, a point on a plane defines 
			a revolute joint, but Creo does not support that set of geometries for defining a revolute 
			joint.  Creo supports an axis and a plane/point to define a revolute joint.  The following 
			table shows the constructs supported by Creo and thus supported by this program. R.O.

 			Creo Joint Type	Other Name		Creo Required Geometry
 			---------------	---------------		---------------------------------------
			Pin		Revolute		Axis, Point or Plane
			Cylinder	Cylindrical		Axis
			Slider		Prismatic		Axis, Plane	
			Planar		Planar			Plane (Note- Creo supports further
								restrictions (i.e. additional planes) but
								we will assume the classical definition
								(3 degrees of freedom) of a planar constraint.
								Additional, planes/geometry will result in a
								user defined constraint.
			Ball		Spherical		Point (Creo supports other geometry types,
								but we will only support a point.) 		
			


v1.5.00.0 05/06/2015	Support Creo 3.0.  Previous to this change Creo 2.0 and 1.0 were supported.  With this
			change Creo 3.0 and 2.0 are supported.  Creo 1.0 is no longer supported. R.O.

v1.5.1.0 06/03/2015	Switched from log4cpp to Boost loging. R.O. 
 
v1.5.2.0 06/15/2015	Corrections so the NuGet would work on the build server.  With this change, 
			the following boost libraries are used:
   				"boost" version="1.55.0.16" 
  				"boost_atomic-vc100" version="1.55.0.15" 
  				"boost_chrono-vc100" version="1.55.0.15" 
  				"boost_date_time-vc100" version="1.55.0.15" 
  				"boost_filesystem-vc100" version="1.55.0.15" 
  				"boost_program_options-vc100" version="1.55.0.15" 
  				"boost_regex-vc100" version="1.55.0.15" 
  				"boost_system-vc100" version="1.55.0.15" 
  				"boost_thread-vc100" version="1.55.0.15" 
  				"boost_log_setup-vc100" version="1.55.0.15" 
  				"boost_log-vc100" version="1.55.0.15"     
    			R.O.

v1.5.3.0 06/25/2015	Modify the sorting (order that models are added to the assembly and constrained in 
			the assembly) to give precedence to models that are constrained to more than one
		        other models.
					       * *E* *
					      *
					     C
					    *
				     * *B* *
				    *     *
				   A   D
				  *  *	 
				 *				
			As an example of why this change is needed, consider models A, B, C, D, E; 
			wherein, B is connected to A via a revolute joint, C is connected to B via a 
			revolute joint, D is connected to A and B via revolute joints, and E is 
			connected to C via a revolute joint.  If E has a guide that positions it 
			relative to C, then E should be added after D.  This is because applying D probably 
			moves/rotates C and would invalidate the guide that positions E.  The solution 
			is to fix the position of C by applying D before E is added/constrained.

			Made corrections to how constraints are applied such that the resulting constraint
			feature trees would agree with feature trees dumped from constraints created via the UI.
			See the following document for more information.
			https://svn.isis.vanderbilt.edu/META/sandbox/rowens/Documents/Creo%20Constraint-Feature-Tree%20Contents%20to%20Toolkit%20Constructs.docx
			R.O.


No New Vers. 10/12/2015	A change was made to the CyPhy2CAD interpreter that affects the contents of CADAssembly.xml 
			and thus the behavior of the CreateAssembly program.
			A description of the change follows:
			For both loads and constraints, a metric should have been requested in CADAssembly.xml
                        for the location of the points used in defining the loads and constraints.  This was done 			for constraints but not loads.  FEATestBench.cs lines were modified from 
			AddGeometry2Load(feaforceRep,geometry.Impl as MgaFCO,tipContextPath)  
			to AddGeometry2Load(feaforceRep,geometry.Impl as MgaFCO,tipContextPath, true);  
			The "true" in the previous line means to addcomputations. 
			R.O.

			
v1.5.4.0 12/02/2015	GitHub Branch: CAD_002_FEA_METALink_Improvements 

			For FEA, added:
				a) Support for pressure loads for deck-based Nastran and deck-based Abaqus.
				   Note - will add support for pressure loads for Calculix later.
				b) Support for Face construct (i.e. adding loads/constraints to surfaces) for
				   deck-based Nastran, Abaqus, and Calculix.
				c) Support for extrusions for identifying geometry
				d) The "Structural FEA" Test Bench has a dropdown to specify SolverType. Modified
				   src\CyPhyMasterInterpreter\Rules\StructuralFEATestBenchChecker.cs to allow Face 
				   constructs to be used with all SolverTypes, not just "Abaqus Model Based".

			Performance improvements:	
				a) Modified SetCADModelParameters.cpp to only regen (up to 5 times) for parametric 
				   parts.  Previously the regen was happening for all parts, but non-parametric
				   parts should have been regened before saving.  If that is not the case, 
				   this program would have no way of correcting modeling errors.
				b) Modified AssemblyEditingViaLink.cpp to remove a superfluous regenerate.
			R.O.				




v1.5.5.0 04/04/2016	GitHub Branch: CAD_003_Support_FEA_CAD_GEOMETRY_NONE

			The function ValidateFEAAnalysisInputs checks for a load/constraint having the 
			proper number of geometry features.  For example, a force load must be applied 
			to geometry identified by geometry features (i.e. datum points).  There are some 
			loads (e.g. acceleration and ambient-temperature) that do not require geometry features.
			ValidateFEAAnalysisInputs was handling acceleration and ambient-temperature incorrectly.  
			This resulted in error messages such as “For a polygon load, there must at least three 
			features (i.e. Datum Points)”.  This defect was corrected to support acceleration and 
			ambient temperature loads correctly.
			R.O.

			
v1.5.6.0 04/13/2016	GitHub Branch: CAD_004_Correct_FEA_Treatment_of_Assembly_Surfaces
	
			The Face FEA construct identifies surfaces for placing loads/constraints.  For 
			deck-based FEA this worked correctly for single parts, but not for assemblies 
			of parts.  For assemblies, parts are offset from the assembly global coordinate 
			system and special transformations were needed for the Face construct to work 
			properly for assemblies.
			R.O.


v1.5.7.0 04/26/2016	GitHub Branch: CAD_005_Update_TO_Creo_3_VS2012

			Updated to use Creo 3.0 and Visual Studio 2012.  Added a check that will
			only allow this program to run if Creo 3.0 is installed.  
			Modified String_To_ProBoolean to handle the case of 1/0 where 1 means True 
			and 0 means false. Previously, only handled True/False.  
			Patran_PP.py was modified to handle the case where not all
			test-injection-points have a StructuralFEAComputation.

	
v1.5.8.0 05/9/2016	GitHub Branch: CAD_006_Support_Patran_Solid_Models

			For supporting Patran_Nastran, Modified CreateAssembly program to 
				a) recognize Patran_Nastran switch 
				b) create Parsolid files when Patran_Nastran switch is set, 
				c) create AnalysisMetaData.xml when Patran_Nastran switch is set, and 
				d) added CreatePatranModel.pcl and CreatePatranModel.ses.
			Added parsing the switch InfiniteCycle="true".  Acting on this
			switch via the CreateAssembly program has not been implemented yet.
			Added parsing
		     		<MeshParameters Max_Global_Length="0.101" 
				Max_Curv_Delta_Div_Edge_Len="0.102" 
				Ratio_Min_Edge_To_Max_Edge="0.201" Match_Face_Proximity_Tol="0.0501" />
			MeshParameters are not used by the CreateAssembly program.  They are used
			by CreatePatranModel.pcl.


v1.5.9.0 06/29/2016	GitHub Branch: CAD_007_AnalysisMetaData_FEAElementID_For_Patran

			Modified the creation of AnalysisMetaData.xml to enter the correct value for
			FEAElementID in the Patran_Nastran mode.
			Modifed vSolid.pcl to set the solid part name to psolid.1, psolid.2 etc.  This
			was necessary so that the values in Nastran_mod_out.txt would appear the
			same as those for deck-based FEA; otherwise, post processing would fail.
	
v1.5.10.0  07/01/2016	GitHub Branch: CAD_008_AnalysisMeshType_Support_Surface

			Corrected the bug: application error: Function AnalysisMeshType_enum was 
			passed SURFACE which is an erroneous type. This is necessary to support surface 
			FEA models.
			
v1.5.11.0  07/01/2016	GitHub Branch: CAD_010_AnalysisShellElementType_Support_Plate

			Corrected application error: application error: Function AnalysisShellElementType_enum 
			was passed Plate4 which is an erroneous type. This is necessary to support surface FEA models.

	
v1.5.12.0  08/17/2016	GitHub Branch: Patran_010_Orient_Surfaces_Properly

			Added support for parsing the CADAssembly.xml entries of TSAI_WU_FAILURE, 
			HILL_FAILURE, HOFFMAN_FAILURE, MAXIMUM_FAILURE.


v1.5.13.0  08/19/2016	Add support for Tetra4 and Tetra10 in CADAssembly.xml. 
			R.O.


v1.5.14.0  04/27/2017	Started the refactoring to support other CAD systems.  In CommonStructures.h 
			replaced ProAsmcompConstrType with e_CADAssemblyConstraintType and 
			ProType with e_CADFeatureGeometryType.  Created the file CADStringToEnumConversions.h/.cpp 
			to hold the enums common to all CAD systems.  Renamed StringToEnumConversions.h/.cpp to 
			CreoStringToEnumConversions.h/.cpp to clearly signify that these are Creo enums.
			CAD_025_Updates_for_Generic_CAD_Interface
			R.O.

v1.5.15.0  05/11/2017	Added support for exporting DXF format.  Exporting Wavefront is not supported by the toolkit.
			DXF exports only work with Solid models.  Surface models will result in a DXF file that is empty.
 			Switched from using the Creo function ProOutputFileWrite (deprecated as of Creo 3.0) to ProOutputFileMdlnameWrite.
			R.O.	


v1.5.16.0  05/18/2017	Add code (src\CADAssembler\CodeGenerationTools\enums) to programmatically generate enums and link 
			CAD-specific enums to generic enums.  The generic enums will be the common interface between all CAD systems.
	
v1.5.17.0		Someone else changed the version number. Don't know if any changes were actually made.

v1.5.18.0  07/16/2017 Modified CommonStructures.h so that it only depends on generic CAD enums, with the exception of 
			Creo pointers (ProMdl, ProSolid) and assembled features (ProAsmcomp).  Will deal with those anomalies later. 
			Added error checking to CreateCADEnums.py.  Added CodeGenerationTools.
			Branch: CAD_030_CommonStructures_Remove_Creo_References_Except_ptrs
			
v1.5.19.0 	Removed the dependency on PROE_ISIS_EXTENSIONS.  Now the code is only dependent on the registry 
			entry META_PATH.
			
			
v1.5.20.0 	8/29/2017 Remove all references to Creo from the common structures.  Reorganize the code to move files 
			as appropriate to the CADCommonFunctions directory.

v1.5.21.0 	9/13/2017 CAD_032_Add_Some_of_the_Generic_CAD_Interfaces, Refactored the code such that most of the common 
		files are in CADCommonFuncdtions and name with a prefix cc_.  Passed CadFactoryAbstract to 
		CreateAssemblyViaInputFile.  Other general refactoring.  

v1.5.22.0 	11/9/2017 CAD_033_Refactor_and_Add_Some_of_the_Generic_CAD_Interfaces, Reworked the argument parsers from
		a single parser for CreateAssembly, MetaLink, and Extract_ACM to separate parsers with only the arguments
		that apply to the particular application. Created IEnvironment::setupCADEnvironment functions.
		Corrected an error with constraining parts via coordinates systems.

v1.5.23.0 	1/2/2018 CAD_034_Add_More_Generic_CAD_Interfaces, Added more abstract interfaces.  Moved more functions
		to cc_AssemblyUtilities.h/.cpp and other cc_* .h/.cpp.


v1.5.24.0 	2/8/2018 CAD_035_Add_More_Generic_CAD_Interfaces
		Made the CadFactory a global singleton. Added abstract functions:
		retrievePointCoordinates
		retrieveMaterialName

v1.5.25.0 	3/20/2018 CAD_036_Add_More_Generic_CAD_Interfaces
		Changed the naming of exported DXF files.  Added the _asm, _prt (e.g. <model_name>_asm.dxf,
		<model_name>_asm.dxf).  Discussed the DXF naming change with the developer that used 
		dxf files, and he said Test Benches that exported DXF file are not not being used going forward. 
		
		Added the abstract functions:
			addModelsToAssembly
			applySingleModelConstraints
			dataExchangeFormatSupported
			exportDataExchangeFile_STEP
			exportDataExchangeFile_Stereolithography
			exportDataExchangeFile_Inventor
			exportDataExchangeFile_DXF
			exportDataExchangeFile_Parasolid

v1.5.26.0 	3/21/2018
		Convert values from CyPhy for Creo parameters that have units defined.  Kevin S. made this change.

v1.5.27.0 	4/6/2018
		For Creo parameters that don't have units defined, convert value from the XML unit to the part's default unit system.
                If the XML input doesn't have a unit, throw an error if the part is not mmKs.
		
v1.5.27.0 	4/20/2018 CAD_037_Add_More_Generic_CAD_Interfaces     // Should have incremented the version to v1.5.28.0 
		Added abstract functions:
			computePartInterferences
			computeVehicleGroundPlane
			
		Added function:
			WriteInterferenceReport

		Made in_CADComponentData_map constant in the following functions:
			cADModelSave
			retrieveTranformationMatrix_Assembly_to_Child	// also made const in_ChildComponentID
			retrieveTranformationMatrix_Assembly_to_Child   // also made const in_ChildComponentPaths
			retrieveMassProperties
			retrievePointCoordinates
			findPartsReferencedByFeature
			retrieveCADModelUnits

		Replaced retrieveBoundingBox_ComputeFirstIfNotAlreadyComputed with retrieveBoundingBox.
		Removed the need to have an abstract function computeVehicleGroundPlane.  This is handled
		via a cad common computeVehicleGroundPlane function that calls the abstract function computeVehicleGroundPlane.
	

v1.5.28.0 	5/23/2018 CAD_038_Add_More_Generic_CAD_Interfaces
		Modified ApplyParametricParameters to make in_CADComponentData_map const
		Modified MetaLinkAssemblyEditor::UpdateComponentName to assure data.displayName.size() < MAX_STRING_PARAMETER_LENGTH

		Added abstract functions:
			isParameterDefinedInCADModel
			retrieveParameterUnits
			unitConversionFactorsComputation
			setParameter
			
		Logged:
			Units for each model
			Units for each parameter
			
		Added:
		cc_ParametricParameters.h/.cpp
		
		isis::BuildAssembly returns errorList.  If errorList contained any values, then no actions were taken. For 
		CreateAssemblyViaInputFile changed this to throw an exception for fatal errors and log warnings. For  
		CreateAssemblyViaString logged fatal errors and warnings but did not throw an exception.

Known Defects
-------------

1. 	Title: 			Error with Multiple Insta Parametric Family Table Part
   	Date Found: 		7/27/2012
	Found in Version: 	v1.3.5.0
	Description: 		Multiple instances of a parametric family table part results in the following error:
				Created Assembly: Chassis_Damper_FEA_cfg139
   				Assembly: Chassis_Damper_FEA_cfg139   Added: Chassis_8_Wheel<Chassis>.prt
				isis:application_exception: ProMdlRetrieve returned ProError: -4, Model Name: 								Chassis_8_Wheel<ChassisZ1Z>
 

2. 	Title: 			Displacemnt Contraint Applied to a Cylinder Error 
   	Date Found: 		8/8/2012
	Found in Version: 	v1.3.6.0
	Description: 		Within FEA, when a displacement constraint is applied to cylinder geometry type,
				then CADCreoParametricCreateAssembly.exe exits with an error.  Normally, Pin constraints
				would be applied to cylinder geometry. 


3. 	Title: 			Erroneous Mass Properties (Models with Different Units)
   	Date Found: 		12/18/2012
	Found in Version: 	v1.3.23.0
	Description: 		If the top-assembly model has different units than some/all of the 
	`			detail-parts/sub-assemblies, then the mass property information (exported to the metrics xml)
 				for the top assembly would contain erroneous values.    The Creo SDK function 
				ProSolidMassPropertyGet() appears to not do the unit conversions correctly.  A defect will 
				be entered with PTC. 

4.	Title: 			Regeneration Failure Diagnostics Only Covers the Immediate Subordinate Parts/Sub-assemblies
   	Date Found: 		3/18/2013
	Found in Version: 	v1.3.37.0
	Description: 		With v1.3.37.0, functionality was added to help the user determine which
				models would not generate.  If two regeneration attempts failed then 
				regeneration of subordinate parts/sub-assemblies was attempted with 
				logging of which ones failed/succeeded. This regeneration of subordinate 
				parts only goes to the immediate children.  It should extend to the entire tree.

5.	Title: 			pm_is_portmapper_port_busy Warning Message
	Date Found: 		6/17/2013
	Found in Version: 	v1.3.39.0
	Description: 		Creo 2.0 support was added with version v1.3.39.0.  When Creo 2.0 is invoked, the following
			`	warning message is displayed:
					pm_is_portmapper_port_busy: bind failed, error = 10013
					pm_is_portmapper_port_busy: bind failed, error = 10013
				The program works properly; however, the warning message is disconcerting. 





