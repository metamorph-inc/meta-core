
namespace CyPhyCADAnalysis
{
    using System;
    using System.Collections.Generic;
    using System.IO;
    using System.Runtime.InteropServices;
    using System.Text;
    using System.Linq;
    using System.Diagnostics;

    using GME.CSharp;
    using GME;
    using GME.MGA;
    using GME.MGA.Core;
    using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
    using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
    using Newtonsoft.Json;
    using CyPhyGUIs;
    using META;

    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public partial class CyPhyCADAnalysisInterpreter : IMgaComponentEx, IGMEVersionInfo, ICyPhyInterpreter
    {


        public CyPhyCADAnalysisInterpreter()
        {
            OutputDirectory = Path.Combine(Environment.CurrentDirectory, "results");
        }

        /// <summary>
        /// Contains information about the GUI event that initiated the invocation.
        /// </summary>
        public enum ComponentStartMode
        {
            GME_MAIN_START = 0, 		// Not used by GME
            GME_BROWSER_START = 1,      // Right click in the GME Tree Browser window
            GME_CONTEXT_START = 2,		// Using the context menu by right clicking a model element in the GME modeling window
            GME_EMBEDDED_START = 3,		// Not used by GME
            GME_MENU_START = 16,		// Clicking on the toolbar icon, or using the main menu
            GME_BGCONTEXT_START = 18,	// Using the context menu by right clicking the background of the GME modeling window
            GME_ICON_START = 32,		// Not used by GME
            GME_SILENT_MODE = 128 		// Not used by GME, available to testers not using GME
        }

        /// <summary>
        /// This function is called for each interpreter invocation before Main.
        /// Don't perform MGA operations here unless you open a tansaction.
        /// </summary>
        /// <param name="project">The handle of the project opened in GME, for which the interpreter was called.</param>
        public void Initialize(MgaProject project)
        {
            this.ManufacturingManifestData = new DesignManufactureManifest();
            this.ComponentManufacturingDataList = new List<ComponentManufacturingData>();
            this.AVMComponentList = new List<AVM.DDP.MetaAvmProject.Component>();
        }

        public string OutputDirectory { get; set; }
        public string AssemblyName { get; set; }
        public string TestBenchName { get; set; }
        public string ArtifactScriptName { get { return "AppendIFabArtifact.py"; } }
        public string RunScriptName { get { return "CyPhyCADAnalysis.bat"; } }
        public string ProjectRootDirectory { get; set; }
        private List<ComponentManufacturingData> ComponentManufacturingDataList { get; set; }
        private DesignManufactureManifest ManufacturingManifestData { get; set; }
        private List<AVM.DDP.MetaAvmProject.Component> AVMComponentList { get; set; }
        #region IMgaComponentEx Members

        MgaGateway MgaGateway { get; set; }
        GMEConsole GMEConsole { get; set; }

        public void InvokeEx(MgaProject project,
                            MgaFCO currentobj,
                            MgaFCOs selectedobjs,
                            int param)
        {
            if (!enabled)
            {
                return;
            }

            try
            {
                //ComponentIndex ci = new ComponentIndex();
                GMEConsole = GMEConsole.CreateFromProject(project);
                MgaGateway = new MgaGateway(project);
                project.CreateTerritoryWithoutSink(out MgaGateway.territory);
                this.Logger = new CyPhyGUIs.GMELogger(project, this.ComponentName);

                if (currentobj == null)
                {
                    this.Logger.WriteError("Invalid context. This interpreter can only be run if open in the correct context (E.g., test bench).");
                    return;
                }

                GMEConsole.Out.WriteLine(DateTime.Now.ToString() + " running CyPhyCADAnalysis Interpreter");

                //InitLogger();

                // [1] create avmproj
                string projectName = "";
                MgaGateway.PerformInTransaction(delegate
                {
                    projectName = project.Name;
                },
                transactiontype_enum.TRANSACTION_NON_NESTED);

                // META-3080: use passed in project directory, especially in SOTs where .mga file is in a different directory than manifest.project.json
                if (String.IsNullOrEmpty(this.ProjectRootDirectory))
                    this.ProjectRootDirectory = Path.GetDirectoryName(project.ProjectConnStr.Substring("MGA=".Length));
                string avmProjFileName = Path.Combine(this.ProjectRootDirectory, "manifest.project.json");
                //string avmProjFileName = Path.Combine(Path.GetDirectoryName(project.ProjectConnStr.Substring("MGA=".Length)), "manifest.project.json");
                //this.ProjectRootDirectory = Path.GetDirectoryName(avmProjFileName);
                AVM.DDP.MetaAvmProject avmProj = new AVM.DDP.MetaAvmProject();

                if (File.Exists(avmProjFileName))
                {
                    string sjson = "{}";
                    using (StreamReader reader = new StreamReader(avmProjFileName))
                    {
                        sjson = reader.ReadToEnd();
                        avmProj = JsonConvert.DeserializeObject<AVM.DDP.MetaAvmProject>(sjson);
                        this.AVMComponentList = avmProj.Project.Components;
                    }
                }
                // end create avmproj


                // [1] CyPhy2CAD                                 
                // [2] Export DDP, Manufacture XML, Manufacture Manifest
                // [3] Generate AppendArtifact.py - script to append artifacts to testbench_manifest.json files
                // [4] Generate DesignModel1BOM.py - script to generate .bom.json from ddp file
                // [5] Generate main run bat file

                //CallCAD(project, currentobj, selectedobjs, param);                              // CyPhy2CAD                  
                MgaGateway.PerformInTransaction(delegate
                {
                    string kindName = string.Empty;
                    if (currentobj != null)
                    {
                        kindName = currentobj.MetaBase.Name;
                    }

                    if (string.IsNullOrEmpty(kindName) == false && kindName != typeof(CyPhyClasses.TestBench).Name)
                    {
                        Logger.WriteFailed("CyPhyCADAnalysis must be called from a TestBench.");
                        return;
                    }


                    ElaborateModel(project, currentobj, selectedobjs, param);                       // elaborate model        
                    CallCAD(project, currentobj, selectedobjs, param);                              // CyPhy2CAD
                    CallComponentExporter(project, currentobj, selectedobjs, param);                // JS: 7-15-13

                    ManufacturingGeneration(currentobj);                                            // DDP, Manufacture XML, Manufacture Manifest
                },
                transactiontype_enum.TRANSACTION_NON_NESTED);


                GenerateAppendArtifactScript();                                                     // AppendArtifact.py                                               
                GenerateBOMGenScript();                                                             // DesignModel1BOM.py                                                 
                //GenerateAnalysisFilesScript(wkflow_param);

                GenerateRunBatFile();                                                               // main run bat file                                            

                GMEConsole.Out.WriteLine("CyPhyCADAnalysis Interpreter Finished!");
            }

            catch (Exception ex)
            {
                Logger.WriteError("{0} has finished with critical errors. Please see above.", this.ComponentName);
                Logger.WriteError("Error Message: {0}", ex.Message);
            }
            finally
            {
                if (MgaGateway.territory != null)
                {
                    MgaGateway.territory.Destroy();
                }
                if (Logger!=null) Logger.Dispose();
                MgaGateway = null;
                project = null;
                currentobj = null;
                selectedobjs = null;
                GMEConsole = null;
                GC.Collect();
                GC.WaitForPendingFinalizers();
            }
            Trace.Flush();
            Trace.Close();
        }

        private void CallCAD(
            MgaProject project,
            MgaFCO currentobj,
            MgaFCOs selectedobjs,
            int param)
        {
            Type tCAD = Type.GetTypeFromProgID("MGA.Interpreter.CyPhy2CAD_CSharp");
            if (tCAD == null)
            {
                GMEConsole.Info.WriteLine("CyPhy2CAD is not installed on your machine.");
                return;
            }
            ComComponent cyPhy2CAD = new ComComponent("MGA.Interpreter.CyPhy2CAD_CSharp");
            //cyPhy2CAD.WorkflowParameters["prepIFab"] = "true";
            cyPhy2CAD.Initialize(project);
            cyPhy2CAD.InterpreterConfig = cadSettings;
            // TODO cyPhy2CAD.MainParameters.config.CADAnalysis = true;
            //cyPhy2CAD.MainParameters.ConsoleMessages = ;
            cyPhy2CAD.MainParameters.Project = project;
            cyPhy2CAD.MainParameters.CurrentFCO = currentobj;
            cyPhy2CAD.MainParameters.SelectedFCOs = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
            cyPhy2CAD.MainParameters.StartModeParam = param;
            cyPhy2CAD.MainParameters.OutputDirectory = this.OutputDirectory;
            cyPhy2CAD.MainParameters.ProjectDirectory = this.ProjectRootDirectory;
            cyPhy2CAD.MainParameters.Traceability = (META.MgaTraceability) this.result.Traceability;
            cyPhy2CAD.Main();


            this.componentParameters["results_zip_py"] = cyPhy2CAD.result.ZippyServerSideHook;
        }

        // JS: 7/15/13
        private void CallComponentExporter(
            MgaProject project,
            MgaFCO currentobj,
            MgaFCOs selectedobjs,
            int param)
        {
            Type tCAD = Type.GetTypeFromProgID("MGA.Interpreter.CyPhyComponentExporter");
            if (tCAD == null)
            {
                GMEConsole.Info.WriteLine("CyPhyComponentExporter is not installed on your machine.");
                return;
            }
            ComComponent cyPhyCompExp = new ComComponent("MGA.Interpreter.CyPhyComponentExporter");
            cyPhyCompExp.Initialize(project);

            // call component exporter to traverse design and build component index
            CyPhyComponentExporter.CyPhyComponentExporterInterpreter compExport = new CyPhyComponentExporter.CyPhyComponentExporterInterpreter();
            compExport.Initialize(project);
            compExport.TraverseTestBenchForComponentExport(currentobj, this.OutputDirectory, this.ProjectRootDirectory);
        }

        private void GenerateRunBatFile()
        {
            // [1] Invoke .bat to create Metrics.xml and assembly.STEP
            // [2] Invoke BOM_Generator.py
            StreamWriter file = new StreamWriter(Path.Combine(this.OutputDirectory, this.RunScriptName));
            string batfilename = ReplaceSpecialChars(this.TestBenchName);
            file.WriteLine("cmd /c runCADJob.bat");
            file.WriteLine("python DesignModel2BOM.py ");               //file.WriteLine("python DesignModel2BOM.py " + this.AssemblyName + ".metadesign.json " + this.AssemblyName + ".bom.json");
            file.WriteLine("python " + this.ArtifactScriptName);        //file.WriteLine("python " + this.ArtifactScriptName + " " + this.AssemblyName);
            file.WriteLine("call " + Tool_bat);
            file.Close();
        }

        private string ReplaceSpecialChars(string old)
        {
            char[] chars = new char[] { ',', '.', '=', '(', ')', '!', '@', '#', '$', '%', '&', '~', '`', '+', ' ', '^', '*', '[', ']', '{', '}', '/', '?', ':', ';', '<', '>', '|' };
            string newStr = chars.Aggregate(old, (c1, c2) => c1.Replace(c2, '_'));
            return newStr;
        }

        private void GenerateTestResultsJson(MgaFCO currentobj)
        {
            if ((currentobj.MetaBase.Name == "TestBench"))
            {
                /*
                CyPhy.Tes     tBench tb = CyPhyClasses.TestBench.Cast(currentobj);
                AVM.DDP.MetaTBReport report = new AVM.DDP.MetaTBReport();

                report.GenerateSummary(
                    tb.Name,
                    tb,
                    "summary.testresults.json",
                    this.OutputDirectory);
                */

                // Put new TB manifest here
            }
        }

        private void GenerateAppendArtifactScript()
        {
            Templates.AppendIFabArtifact appendscript = new Templates.AppendIFabArtifact()
            {
                AssemblyName = ReplaceSpecialChars(this.AssemblyName),
                TestBenchName = this.TestBenchName,
                TestBenchName_Cleaned = ReplaceSpecialChars(this.TestBenchName),
            };
            using (StreamWriter writer = new StreamWriter(Path.Combine(this.OutputDirectory, this.ArtifactScriptName)))
            {
                writer.WriteLine(appendscript.TransformText());
            }
        }

        private void GenerateBOMGenScript()
        {
            Templates.DesignModel2BOM bomscript = new Templates.DesignModel2BOM()
            {
                DDPFile = this.TestBenchName + ".metadesign.json",
                BOMFile = this.TestBenchName + ".bom.json",
            };
            using (StreamWriter writer = new StreamWriter(Path.Combine(this.OutputDirectory, "DesignModel2BOM.py")))
            {
                writer.WriteLine(bomscript.TransformText());
            }
        }


        private void ElaborateModel(MgaProject project,
                                    MgaFCO currentobj,
                                    MgaFCOs selectedobjs,
                                    int param)
        {
            try
            {
                var elaborator = new CyPhyElaborateCS.CyPhyElaborateCSInterpreter();
                elaborator.Logger = new GMELogger(project);
                //elaborator.Logger.AddWriter(Logger.Instance);
                var result = elaborator.RunInTransaction(project, currentobj, selectedobjs, param);
                if (result == false)
                {
                    throw new ApplicationException("see elaborator log");
                }
                if (this.result.Traceability == null)
                {
                    this.result.Traceability = new META.MgaTraceability();
                }
                if (elaborator.Traceability != null)
                {
                    elaborator.Traceability.CopyTo(this.result.Traceability);
                }
            }
            catch (Exception e)
            {
                //Logger.Instance.AddLogMessage("Elaborator exception occurred: " + e.Message, Severity.Error);
                throw new Exception(e.Message);
            }
        }

        CyPhyGUIs.GMELogger Logger { get; set; }

        private void InitLogger()
        {
            string logDir = Path.Combine(this.OutputDirectory, "log");
            if (!Directory.Exists(logDir))
                Directory.CreateDirectory(logDir);

            string loggingFileName = Path.Combine(logDir, "CyPhyCADAnalysis.log");

            var fs = new FileStream(loggingFileName, FileMode.Create);
            TraceListener fileTL = new TextWriterTraceListener(fs)
            {
                Name = "CyPhyCADAnalysis",
            };

            Trace.AutoFlush = true;
            Trace.Listeners.Add(fileTL);
            Trace.TraceInformation("{0} trace file listener was created.", loggingFileName);
        }

        private ComponentStartMode Convert(int param)
        {
            switch (param)
            {
                case (int)ComponentStartMode.GME_BGCONTEXT_START:
                    return ComponentStartMode.GME_BGCONTEXT_START;
                case (int)ComponentStartMode.GME_BROWSER_START:
                    return ComponentStartMode.GME_BROWSER_START;

                case (int)ComponentStartMode.GME_CONTEXT_START:
                    return ComponentStartMode.GME_CONTEXT_START;

                case (int)ComponentStartMode.GME_EMBEDDED_START:
                    return ComponentStartMode.GME_EMBEDDED_START;

                case (int)ComponentStartMode.GME_ICON_START:
                    return ComponentStartMode.GME_ICON_START;

                case (int)ComponentStartMode.GME_MAIN_START:
                    return ComponentStartMode.GME_MAIN_START;

                case (int)ComponentStartMode.GME_MENU_START:
                    return ComponentStartMode.GME_MENU_START;
                case (int)ComponentStartMode.GME_SILENT_MODE:
                    return ComponentStartMode.GME_SILENT_MODE;
            }

            return ComponentStartMode.GME_SILENT_MODE;
        }

        #region Component Information
        public string ComponentName
        {
            get { return GetType().Name; }
        }

        public string ComponentProgID
        {
            get
            {
                return ComponentConfig.progID;
            }
        }

        public componenttype_enum ComponentType
        {
            get { return ComponentConfig.componentType; }
        }
        public string Paradigm
        {
            get { return ComponentConfig.paradigmName; }
        }
        #endregion

        #region Enabling
        bool enabled = true;
        public void Enable(bool newval)
        {
            enabled = newval;
        }
        #endregion

        #region Interactive Mode
        protected bool interactiveMode = true;
        public bool InteractiveMode
        {
            get
            {
                return interactiveMode;
            }
            set
            {
                interactiveMode = value;
            }
        }
        #endregion

        #region Custom Parameters
        SortedDictionary<string, object> componentParameters = new SortedDictionary<string, object>();

        public object get_ComponentParameter(string Name)
        {
            if (Name == "type")
                return "csharp";

            if (Name == "path")
                return GetType().Assembly.Location;

            if (Name == "fullname")
                return GetType().FullName;

            object value;
            if (componentParameters != null && componentParameters.TryGetValue(Name, out value))
            {
                return value;
            }

            return null;
        }

        public void set_ComponentParameter(string Name, object pVal)
        {
            if (componentParameters == null)
            {
                componentParameters = new SortedDictionary<string, object>();
            }

            componentParameters[Name] = pVal;
        }
        #endregion

        #region Unused Methods
        // Old interface, it is never called for MgaComponentEx interfaces
        public void Invoke(MgaProject Project, MgaFCOs selectedobjs, int param)
        {
            throw new NotImplementedException();
        }

        // Not used by GME
        public void ObjectsInvokeEx(MgaProject Project, MgaObject currentobj, MgaObjects selectedobjs, int param)
        {
            throw new NotImplementedException();
        }

        #endregion

        #endregion

        #region IMgaVersionInfo Members

        public GMEInterfaceVersion_enum version
        {
            get { return GMEInterfaceVersion_enum.GMEInterfaceVersion_Current; }
        }

        #endregion

        #region Registration Helpers

        [ComRegisterFunctionAttribute]
        public static void GMERegister(Type t)
        {
            Registrar.RegisterComponentsInGMERegistry();

        }

        [ComUnregisterFunctionAttribute]
        public static void GMEUnRegister(Type t)
        {
            Registrar.UnregisterComponentsInGMERegistry();
        }

        #endregion


        public string InterpreterConfigurationProgId
        {
            get
            {
                return (typeof(CADAnalysisConfig).GetCustomAttributes(typeof(ProgIdAttribute), false)[0] as ProgIdAttribute).Value;
            }
        }

        public IInterpreterPreConfiguration PreConfig(IPreConfigParameters parameters)
        {
            var config = new CADAnalysisConfig();
            config.ProjectDirectory = parameters.ProjectDirectory;
            return config;
        }

        public IInterpreterConfiguration DoGUIConfiguration(IInterpreterPreConfiguration preConfig, IInterpreterConfiguration previousConfig)
        {
            var config = (CADAnalysisConfig)preConfig;
            ComComponent cyPhy2CAD = new ComComponent("MGA.Interpreter.CyPhy2CAD_CSharp");
            bool cyPhy2CADConfigWasSuccessful =  cyPhy2CAD.DoGUIConfiguration(config.ProjectDirectory);
            if (cyPhy2CADConfigWasSuccessful == false)
            {
                return null;
            }
            // TODO: when CyPhy2CAD implements ICyPhyInterpreter, save its config in ours
            CyPhy2CAD_CSharp.CyPhy2CADSettings cadSettings = (CyPhy2CAD_CSharp.CyPhy2CADSettings)(cyPhy2CAD.InterpreterConfig);
            config.AuxiliaryDirectory = cadSettings.AuxiliaryDirectory;
            config.StepFormats = cadSettings.StepFormats;

            return config;
        }

        private CyPhy2CAD_CSharp.CyPhy2CADSettings cadSettings = new CyPhy2CAD_CSharp.CyPhy2CADSettings();
        private InterpreterResult result = new InterpreterResult();
        public IInterpreterResult Main(IInterpreterMainParameters parameters)
        {
            this.OutputDirectory = parameters.OutputDirectory;
            this.ProjectRootDirectory = parameters.ProjectDirectory;
            if (this.result.Traceability == null)
            {
                this.result.Traceability = new META.MgaTraceability();
            }

            result.RunCommand = "CyPhyCADAnalysis.bat";
            result.Labels = "Creo&&CADCreoParametricCreateAssembly.exev1.4";

            parameters.Project.BeginTransactionInNewTerr();

            string Parameters = parameters
                .CurrentFCO
                .ChildObjects
                .OfType<MgaReference>()
                .FirstOrDefault(x => x.Meta.Name == "WorkflowRef")
                .Referred
                .ChildObjects
                .OfType<MgaAtom>()
                .FirstOrDefault()
                .StrAttrByName["Parameters"];
            Dictionary<string, string> workflowParameters = new Dictionary<string, string>();

            try
            {
                workflowParameters = (Dictionary<string, string>)Newtonsoft.Json.JsonConvert.DeserializeObject(Parameters, typeof(Dictionary<string, string>));
                if (workflowParameters == null)
                {
                    workflowParameters = new Dictionary<string, string>();
                }
            }
            catch (Newtonsoft.Json.JsonReaderException)
            {
            }
            META.AnalysisTool.ApplyToolSelection(this.ComponentProgID, workflowParameters, this.result, parameters, modifyLabels: false);
            this.result.Labels += " && CyPhyCADAnalysis" + JobManager.Job.LabelVersion; // META-2405
            parameters.Project.AbortTransaction();

            List<string> Inventor_Tests = new List<string>() {"closures", "field_of_view", "field_of_fire", "ergonomics", "ingress_egress", "transportability"};
            List<string> Leaf_Metrics = new List<string>() { "conceptual_mfg", "detailed_mfg", "completeness", "corrosion" };
            bool b = Inventor_Tests.Any(workflowParameters.Values.Contains);
            bool c = Leaf_Metrics.Any(workflowParameters.Values.Contains);

            CADAnalysisConfig config = (CADAnalysisConfig)parameters.config;
            cadSettings.AuxiliaryDirectory = config.AuxiliaryDirectory;
            cadSettings.StepFormats = config.StepFormats;
            if (b)
                cadSettings.OtherDataFormat.Inventor = true;
            if (c)
                cadSettings.SpecialInstructions.LeafAssembliesMetric = true;
            InvokeEx(parameters.Project, parameters.CurrentFCO, parameters.SelectedFCOs, parameters.StartModeParam);

            result.Success = true;
            return result;
        }
       
        public string wkflow_param { get; set; }
        public string Tool_bat { get; set; }
    }

    [Serializable]
    [ComVisible(true)]
    [ProgId("ISIS.META.CADAnalysisConfig")]
    [Guid("B8347593-FE34-4F1D-B2C0-8E97C41B23D4")]
    public class CADAnalysisConfig : IInterpreterConfiguration, IInterpreterPreConfiguration
    {
        public string ProjectDirectory { get; set; }
        public string AuxiliaryDirectory { get; set; }
        public List<string> StepFormats { get; set; }
    }
}
