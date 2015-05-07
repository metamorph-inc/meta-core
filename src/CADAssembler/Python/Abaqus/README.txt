AbaqusMain.py README

1.0.0   - Initial version.
        - Combines standalone FEA and Adams2Abaqus program into one script.
        - META-3438: No longer relies on uniquely named parts
        - META-3338: Allows user to specify maximum element size
        - META-3027: Includes mesh statistics for full assembly and
                     individual parts (MeshQuality.csv)
        - META-2754: Reports ending condition (convergence, model error, etc)

1.1.0   - META-3040: Add thermal capabilities
        - Coupled static thermal/structural displacement analysis
        - If no structural elements specified, thermal analysis only will run.
        
1.2.0   - Resolves issues with ADAMS2Abaqus runs that came about from merging scripts.
        - Removes adaptive flag. Adaptivity process is now determined by checking
          CADAssembly.xml: if maxNumberIter > 1, then an adaptive process is run.
            - Dynamic + Adaptivity does NOT work. Regular dynamic analysis will run.
        - Checks provided to ensure analysis options are compatible.
        - META-3385: Added Explicit Dynamic analysis step for ADAMS-to-Abaqus runs.
            - Explicit Dynamics -> ADAMS2Abaqus
            - Implicit Dynamics -> Abaqus only (already present in script)
            - Checks whether type of dynamic analysis matches with test scenario.
        - META-2956: FACE constructs can be applied to curved surfaces.
        
1.3.0   - META-3415: Allows for children of sub-assemblies to be merged into one part.

1.4.0   - META-3502: Bug related to camel-cased point datums in GME not being picked up.
            - Bug where metrics weren't reported in GME CompAsm of only one component,
                of which was an assembly.
            - Program now searches for ADAMS LOD files with name LT_<CID>.lod

2.0.0   - META-3505: Merging of Deck-Based/Model-Based PostProcessing solutions
        - CreateLOADSBCPNG() moved to AbaqusCAE.py.
            - Not related to post processing & only done with CAE.
        - CreateOverlapPNG() moved to AbaqusDataCheck.py
            - Only utilized with failed data checks, so this is the appropriate location.
        - Analysis now occurs in Analysis\Abaqus directory. Directory is changed after parsing
          assembly related XML files.
        - CopyOrDeleteResults() moved to utility_functions.py.
            - All DataCheck related artifacts are deleted regardless of CopyResults flag.
        - Thermal post processing now handled with rest of post processing scripts
            - Formerly was in AbaqusThermal.py.
 
2.1.0   - META-3523: Remove restriction of FEA:DynamicImplicit & A2A:DynamicExplicit.

2.1.1   - META-3520: All errors are now reported to _FAILED.txt.
            - Begin using cad_library
            
2.2.0   - META-3533: Material properties now retrieved through cad_library python package.
            - References to the material library are removed.
            - Conversion dictionary and unitLength functions moved to cad_library\unit_utils.
            - ABQ/Patran PP updated to grab stress allowables from cad_library instead of
                AnalysisMetaData.xml. Material name is still taken from AnalysisMetaData.xml.

2.3.0   - META-3546: Switch from parsing metrics file to utilizing cad_library.AssemblyInfo() class.
            - Remove superRef and metricRef (unused).
          META-3540: Allow coupled analysis with Implicit and Explicit Dynamic steps.
            - MeshInstances() now done after loads and BCs are applied. This allows for
                case where GME constructs refer to faces that need to be partitioned.
                Otherwise when face is partitioned, the mesh is deleted.
            - Catch UnboundLocalError for Abaqus read_msg function. Still return failed,
                but avoid test bench returning python error.
2.4.0   - META-3543: Switch unique import numbering convention from breadth-first to
                depth-first to accomodate case of sub-assemblies containing sub-assemblies.

2.4.1   - META-3563: Resolve bug that had HeatGeneration not being applied correctly to subassembly
                of a subassembly. Also improve code readability and style for PyCharm and PyLint.
                
2.4.2   - META-3570: Add initialize() for declaring string constants. Readability changes based
                on DARPA code review feedback. Add units to contour plots.
                
2.4.3   - META-3578: Load type changed from ConvectionHeat to Convection.

2.5.0   - META-3565: Add ability to apply moment for Circle and Face constructs.

