Title: 	  CADCreoParametricCreateAssembly.exe (Creo Parametric)
	  assemble_ptc.exe (Pro/E) no longer supported

Creo-Parametric 	CADCreoParametricCreateAssembly.exe v1.4.15.0
Pro/E 		 	assemble_ptc.exe Version: v1.0.008  // Not supported as of Jan. 2012

Supported Creo Versions: Creo 2.x, no other versions currently supported

Pre-Conditions:
--------------

1. This program only runs on 64 bit machines/OSs.

2. Pro/E (i.e. “Pro/E Wildfire 5.0”) Not Supported as of Jan. 2012

3. For Creo-Parametric, must have “Creo Paramtric 2.x” installed and configured such that Creo runs properly.
   Creo 1.0 not supported as of July, 2013.  	

4. Typically this program would be invoked on a computer that has the META tool suite installed.  If this
   is no the case, the x64 version of UDM must be installed.  This is needed because this program uses UDM to 
   parse xml files.  You can download UDM from http://repo.isis.vanderbilt.edu/downloads/.  Make sure you 
   install the 64 bit versions (e.g. UDM_x64-....msi). 

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



Example Bat Files:
-----------------
Example bat files used to invoke assemble_ptc.exe and CADProECreateAssembly.exe follow:

	...\Proe ISIS Extensions\docs\examples\CADProECreateAssembly.bat, example for v1.3.7.0 and later.
 	

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

Known Defects
-------------

1. 	Title: 			Error with Multiple Instances of a Parametric Family Table Part
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





