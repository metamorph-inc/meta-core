
namespace CyPhyPrepareIFab
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
    public partial class CyPhyPrepareIFabInterpreter : IMgaComponentEx, IGMEVersionInfo, ICyPhyInterpreter
    {
        public CyPhyPrepareIFabInterpreter()
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
        }

        public string OutputDirectory { get; set; }
        public string AssemblyName { get; set; }
        public string TestBenchName { get; set; }
        public string ArtifactScriptName { get { return "AppendIFabArtifact.py"; } }
        public string RunScriptName { get { return "PrepareIFab.bat"; } }
        public string ProjectRootDirectory { get; set; }
        private List<ComponentManufacturingData> ComponentManufacturingDataList { get; set; }
        private DesignManufactureManifest ManufacturingManifestData { get; set; }

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
                GMEConsole = GMEConsole.CreateFromProject(project);
                MgaGateway = new MgaGateway(project);
                this.Logger = new CyPhyGUIs.GMELogger(project, this.ComponentName);

                if (currentobj == null)
                {
                    this.Logger.WriteError("Invalid context. This interpreter can only be run if open in the correct context (E.g., test bench).");
                    return;
                }

                GMEConsole.Out.WriteLine(DateTime.Now.ToString() + " running CyPhyPrepIFab Interpreter");

                //InitLogger();

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
                        Logger.WriteFailed("CyPhyPrepIFAB must be called from a TestBench.");
                        return;
                    }

                    ElaborateModel(project, currentobj, selectedobjs, param);                       // elaborate model        
                    CallCAD(project, currentobj, selectedobjs, param);                              // CyPhy2CAD
                    ManufacturingGeneration(currentobj);                                            // DDP, Manufacture XML, Manufacture Manifest

                },
                transactiontype_enum.TRANSACTION_NON_NESTED);


                GenerateAppendArtifactScript();                                                     // AppendArtifact.py                                               
                GenerateBOMGenScript();                                                             // DesignModel1BOM.py                                                 
                GenerateRunBatFile();                                                               // main run bat file                                            

                GMEConsole.Out.WriteLine("CyPhyPrepIFab Interpreter Finished!");
            }
            catch (Exception)
            {
                Logger.WriteError("{0} has finished with critical errors. Please see above.", this.ComponentName);   
            }

            finally
            {
                //Trace.Close();
                MgaGateway = null;
                if (Logger != null) Logger.Dispose();
                project = null;
                currentobj = null;
                selectedobjs = null;
                GMEConsole = null;
                GC.Collect();
                GC.WaitForPendingFinalizers();
            }
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
            // TODO cyPhy2CAD.MainParameters.config.prepIFab = true;
            //cyPhy2CAD.MainParameters.ConsoleMessages = ;
            cyPhy2CAD.MainParameters.Project = project;
            cyPhy2CAD.MainParameters.CurrentFCO = currentobj;
            cyPhy2CAD.MainParameters.SelectedFCOs = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
            cyPhy2CAD.MainParameters.StartModeParam = param;
            cyPhy2CAD.MainParameters.OutputDirectory = this.OutputDirectory;
            cyPhy2CAD.MainParameters.ProjectDirectory = this.ProjectRootDirectory;
            cyPhy2CAD.Main();


            this.componentParameters["results_zip_py"] = cyPhy2CAD.result.ZippyServerSideHook;
        }

        /*
        private void GenerateDDP(MgaFCO currentobj)
        {
            string type = currentobj.MetaBase.Name;

            if (type == "TestBench")
            {
                CyPhy.TestBench tb = CyPhyClasses.TestBench.Cast(currentobj);
                var tlsut = tb.Children.TopLevelSystemUnderTestCollection.FirstOrDefault();
                if (tlsut == null)
                {
                    throw new Exception("There is no top level system under test in the model!");
                }
                if (tlsut.Referred.ComponentAssembly == null)
                {
                    throw new Exception("Top level system under test does not reference a component assembly!");
                }

                var design = CyPhy2DesignInterchange.CyPhy2DesignInterchange.Convert(tlsut.Referred.ComponentAssembly);
                this.TestBenchName = tb.Name;
                this.AssemblyName = design.Name;
                string d = design.SerializeToFile(Path.Combine(this.OutputDirectory, this.TestBenchName + ".metadesign.json"));
            }
            else if (type == "ComponentAssembly")
            {
                CyPhy.ComponentAssembly assembly = CyPhyClasses.ComponentAssembly.Cast(currentobj);
                var design = CyPhy2DesignInterchange.CyPhy2DesignInterchange.Convert(assembly);
                this.AssemblyName = design.Name;
                this.TestBenchName = design.Name;
                string d = design.SerializeToFile(Path.Combine(this.OutputDirectory, this.TestBenchName + ".metadesign.json"));                
            }
            else
                throw new NotImplementedException("");
        }
        */

        private void GenerateRunBatFile()
        {
            // [1] Invoke .bat to create Metrics.xml and assembly.STEP
            // [2] Invoke BOM_Generator.py
            StreamWriter file = new StreamWriter(Path.Combine(this.OutputDirectory, this.RunScriptName));
            string batfilename = ReplaceSpecialChars(this.TestBenchName);
            file.WriteLine("cmd /c runCADJob.bat");
            file.WriteLine("python DesignModel2BOM.py ");               //file.WriteLine("python DesignModel2BOM.py " + this.AssemblyName + ".metadesign.json " + this.AssemblyName + ".bom.json");
            file.WriteLine("python " + this.ArtifactScriptName);        //file.WriteLine("python " + this.ArtifactScriptName + " " + this.AssemblyName);
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
            // call elaborator and expand the references
            Type t = Type.GetTypeFromProgID("MGA.Interpreter.CyPhyElaborate");
            IMgaComponentEx elaborator = Activator.CreateInstance(t) as IMgaComponentEx;
            elaborator.Initialize(project);
            elaborator.ComponentParameter["automated_expand"] = "true";
            elaborator.ComponentParameter["console_messages"] = "off";

            elaborator.InvokeEx(project, currentobj, selectedobjs, param);
        }

        CyPhyGUIs.GMELogger Logger { get; set; }

        private void InitLogger()
        {
            string logDir = Path.Combine(this.OutputDirectory, "log");
            if (!Directory.Exists(logDir))
                Directory.CreateDirectory(logDir);

            string loggingFileName = Path.Combine(logDir, "CyPhyPrepareIFab.log");

            var fs = new FileStream(loggingFileName, FileMode.Create);
            TraceListener fileTL = new TextWriterTraceListener(fs)
            {
                Name = "CyPhyPrepareIFab",
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
                return (typeof(PrepareIFabConfig).GetCustomAttributes(typeof(ProgIdAttribute), false)[0] as ProgIdAttribute).Value;
            }
        }

        public IInterpreterPreConfiguration PreConfig(IPreConfigParameters parameters)
        {
            var config = new PrepareIFabConfig();
            config.ProjectDirectory = parameters.ProjectDirectory;
            return config;
        }

        public IInterpreterConfiguration DoGUIConfiguration(IInterpreterPreConfiguration preConfig, IInterpreterConfiguration previousConfig)
        {
            var config = (PrepareIFabConfig)preConfig;
            ComComponent cyPhy2CAD = new ComComponent("MGA.Interpreter.CyPhy2CAD_CSharp");
            cyPhy2CAD.DoGUIConfiguration(config.ProjectDirectory);
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

            result.RunCommand = "PrepareIFab.bat";
            result.Labels = "Creo&&CADCreoParametricCreateAssembly.exev1.4&&" + JobManager.Job.DefaultLabels;

            PrepareIFabConfig config = (PrepareIFabConfig)parameters.config;
            cadSettings.AuxiliaryDirectory = config.AuxiliaryDirectory;
            cadSettings.StepFormats = config.StepFormats;
            cadSettings.OtherDataFormat.STLAscii = true;
            InvokeEx(parameters.Project, parameters.CurrentFCO, parameters.SelectedFCOs, parameters.StartModeParam);

            MgaGateway.PerformInTransaction(() =>
            {

                // TODO: this part needs to be refactored!
                string Parameters = parameters
                    .CurrentFCO
                    .ChildObjects
                    .OfType<MgaReference>()
                    .FirstOrDefault(x => x.Meta.Name == "WorkflowRef")
                    .Referred
                    .ChildObjects
                    .OfType<MgaAtom>()
                    .FirstOrDefault(fco => fco.Meta.Name == typeof(CyPhy.Task).Name
                        && String.Equals(CyPhyClasses.Task.Cast(fco).Attributes.COMName, this.ComponentProgID, StringComparison.InvariantCultureIgnoreCase))
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

                META.AnalysisTool.ApplyToolSelection(this.ComponentProgID, workflowParameters, this.result, parameters);
            }, abort: true);

            result.Success = true;
            return result;
        }
    }

    [Serializable]
    [ComVisible(true)]
    [ProgId("ISIS.META.PrepareIFabConfigSettings")]
    [Guid("70A46D21-441A-495E-BEC0-62F10205D5DE")]
    public class PrepareIFabConfig : IInterpreterConfiguration, IInterpreterPreConfiguration
    {
        public string ProjectDirectory { get; set; }
        public string AuxiliaryDirectory { get; set; }
        public List<string> StepFormats { get; set; }
    }
}
