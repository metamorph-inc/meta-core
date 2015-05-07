# Analysis Tools #

Analysis tools are run as part of a CyPhy test bench. Each tool defines
which artifacts should be extracted from the CyPhy model and what the 
entry point of the tool is.

## Creating a new tool ##

1. Create a new directory here. (`META tool install location\analysis_tools`, e.g. `C:\Program Files (x86)\META\analysis_tools`)
2. Add `analysis_tool.manifest.json` file based on `ExamplePython\analysis_tool.manifest.json`.
Note: one directory can contain multiple analysis tools.
3. Change the values accordingly to your needs in your `analysis_tool.manifest.json` file
	1. `key` - name of your tool
	2. `version` - version number of your tool
	3. `outputDirectory` - content of this directory will be copied to __every__ test bench output directory where your tool is selected as the analysis tool. Note: this could override some generated artifacts, pick file names carefully.
	4. `runCommand` - main entry point to your tool. It is _strongly_ recommended that you use a batch file and then call python/java or other tools.
	5. `requiredInterpreter` - defines the ProgID of the interpreter that has to generate the input artifacts for your analysis tool. Examples
		1. `MGA.Interpreter.CyPhy2CAD_CSharp` - Generates input files for CAD processing (Assembly, FEA, CFD, Thermal, Kinematic, etc.)
		2. `MGA.Interpreter.CyPhyCADAnalysis` - generates CAD files.
		3. `MGA.Interpreter.CyPhy2Modelica_v2` - generates composed Modelica models.
4. Register all tools by running the `add_registry_entries.py` script from the META Python virtual environment as an administrator: `..\bin\Python27\Scripts\python.exe add_registry_entries.py`. If it runs successfully then `Analysis tool registration is done.` is the last message in the console.
5. Create a test bench that uses the required interpreter. Double click on the `Task` object in the `CyPhy (GME) workflow` definition and select your tool from the drop down list.
6. Run the test bench using the `MasterInterpreter` and the `JobManager` configured for local execution.