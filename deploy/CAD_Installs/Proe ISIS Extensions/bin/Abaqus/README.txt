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