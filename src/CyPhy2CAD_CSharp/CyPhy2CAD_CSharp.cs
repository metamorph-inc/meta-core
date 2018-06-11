using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using GME.CSharp;
using GME;
using GME.MGA;
using GME.MGA.Core;
using System.Windows.Forms;
using System.Xml.Serialization;
using System.Xml;
using System.Linq;
using System.Diagnostics;
using Newtonsoft.Json;
using CyPhyGUIs;
using META;

namespace CyPhy2CAD_CSharp
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public partial class CyPhy2CAD_CSharpInterpreter : IMgaComponentEx, IGMEVersionInfo, ICyPhyInterpreter
    {
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

        const string OptionNameConsole = "console_messages";
        const string OptionNameOutputDir = "output_dir";
        const string OptionNameAutomation = "automation";
        const string OptionNameDoConfig = "do_config";
        const string OptionNameRunCmd = "runCommand";
        const string OptionNameCadDir = "cadFile_dir";
        const string OptionNameIFab = "prepIFab";
        const string OptionNameProjectDir = "original_project_file";

        //private CyPhy2CAD_UILib.CADOptions cadoptionHolder;
        private bool Automation;

        public CyPhy2CAD_CSharpInterpreter()
        {
            this.componentParameters = new SortedDictionary<string, object>();
            this.componentParameters[OptionNameConsole] = "on";
            this.componentParameters[OptionNameOutputDir] = ".\\";
            this.componentParameters[OptionNameAutomation] = "false";
            this.componentParameters[OptionNameDoConfig] = "true";
            this.componentParameters[OptionNameRunCmd] = "runCADJob.bat";
            this.componentParameters[OptionNameCadDir] = ".\\";
            this.componentParameters[OptionNameIFab] = "false";
            this.componentParameters[OptionNameProjectDir] = "";

            var resultzip = CyPhy2CAD_CSharp.Properties.Resources.ResultZip;
            this.componentParameters["results_zip_py"] = resultzip.ToString();

            //cadoptionHolder = new CyPhy2CAD_UILib.CADOptions();

            Automation = false;
        }


        /// <summary>
        /// This function is called for each interpreter invocation before Main.
        /// Don't perform MGA operations here unless you open a tansaction.
        /// </summary>
        /// <param name="project">The handle of the project opened in GME, for which the interpreter was called.</param>
        public void Initialize(MgaProject project)
        {
            // TODO: Add your initialization code here...   
            Contract.Requires(project != null);

            GMEConsole = GMEConsole.CreateFromProject(project);
            MgaGateway = new MgaGateway(project);
        }

        /// <summary>
        /// The main entry point of the interpreter. A transaction is already open,
        /// GMEConsole is available. A general try-catch block catches all the exceptions
        /// coming from this function, you don't need to add it. For more information, see InvokeEx.
        /// </summary>
        /// <param name="project">The handle of the project opened in GME, for which the interpreter was called.</param>
        /// <param name="currentobj">The model open in the active tab in GME. Its value is null if no model is open (no GME modeling windows open). </param>
        /// <param name="selectedobjs">
        /// A collection for the selected model elements. It is never null.
        /// If the interpreter is invoked by the context menu of the GME Tree Browser, then the selected items in the tree browser. Folders
        /// are never passed (they are not FCOs).
        /// If the interpreter is invoked by clicking on the toolbar icon or the context menu of the modeling window, then the selected items 
        /// in the active GME modeling window. If nothing is selected, the collection is empty (contains zero elements).
        /// </param>
        /// <param name="startMode">Contains information about the GUI event that initiated the invocation.</param>
        /// <returns>true if successful</returns>
        [ComVisible(false)]
        public bool Main(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, ComponentStartMode startMode)
        {
            // TODO: Add your interpreter code
            //GMEConsole.Clear();
            GMEConsole.Out.WriteLine("Running CyPhy2CAD interpreter...");

            // TODO: show how to initialize DSML-generated classes
            // Get RootFolder
            //IMgaFolder rootFolder = project.RootFolder;
            //GMEConsole.Out.WriteLine(rootFolder.Name);


            // Check Model
            // Grab all components
            // Create Intermediate Data Representations
            // Walk representation + find islands
            // Convert to DAG

            try
            {

                // META-1971: ADM + ACM file export for blast + ballistics
                // ACM
                {
                    CallComponentExporter(project, currentobj);
                }
                result.Success = true;
                ProcessCAD(currentobj);
                Logger.Instance.DumpLog(GMEConsole, LogDir);
                GMEConsole.Out.WriteLine("Finished CyPhy2CAD with " + (result.Success ? "success" : "failure"));
                return result.Success;
            }
            catch (Exception ex)
            {

                Logger.Instance.AddLogMessage(ex.Message, Severity.Error);
                Logger.Instance.DumpLog(GMEConsole, LogDir);
                GMEConsole.Out.WriteLine(ex.StackTrace.ToString());
                GMEConsole.Out.WriteLine("Finished CyPhy2CAD with failure. Exception encountered.");
                return false;
            }
        }

        private string LogDir
        {
            get
            {
                string logDir = Path.Combine(this.settings.OutputDirectory, "log");
                if (!Directory.Exists(logDir))
                    Directory.CreateDirectory(logDir);
                return logDir;
            }
        }

        #region IMgaComponentEx Members

        MgaGateway MgaGateway { get; set; }
        GMEConsole GMEConsole { get; set; }

        public void InvokeEx(
                            MgaProject project,
                            MgaFCO currentobj,
                            MgaFCOs selectedobjs,
                            int param)
        {
            if (!enabled)
            {
                return;
            }

            if (currentobj == null)
            {
                GMEConsole.Error.WriteLine("Please select a CADTestBench, Ballistic Testbench, FEA Testbench, Blast Testbench or CadAssembly.");
                return;
            }

            string currentWorkDir = System.IO.Directory.GetCurrentDirectory();

            try
            {
                var parameters = new InterpreterMainParameters();
                this.mainParameters = parameters;
                parameters.ProjectDirectory = Path.GetDirectoryName(currentobj.Project.ProjectConnStr.Substring("MGA=".Length));

                FetchSettings();

                // Show UI
                using (MainForm mf = new MainForm(settings))
                {
                    mf.ShowDialog();
                    DialogResult ok = mf.DialogResult;
                    if (ok == DialogResult.OK)
                    {
                        settings = mf.ConfigOptions;
                        parameters.OutputDirectory = settings.OutputDirectory;
                        parameters.config = settings;
                    }
                    else
                    {
                        GMEConsole.Warning.WriteLine("Process was cancelled.");
                        return;
                    }
                }

                SaveSettings();

                MgaGateway.PerformInTransaction(delegate
                {
                    Elaborate(project, currentobj, selectedobjs, param);
                    Main(project, currentobj, selectedobjs, Convert(param));
                },
                abort: true);


            }
            finally
            {
                MgaGateway = null;
                project = null;
                currentobj = null;
                selectedobjs = null;
                GMEConsole = null;
                GC.Collect();
                GC.WaitForPendingFinalizers();
            }
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


        private void SaveSettings()
        {
            string projectDirectory = "";
            if (this.mainParameters != null)
                projectDirectory = this.mainParameters.ProjectDirectory;
            var settingsFilename = Path.Combine(projectDirectory, CyPhy2CADSettings.ConfigFilename);
            META.ComComponent.SerializeConfiguration(projectDirectory, settings, this.ComponentProgID);
        }

        private void SaveSettings(string projectDirectory)
        {

            META.ComComponent.SerializeConfiguration(projectDirectory, settings, this.ComponentProgID);
        }

        private void FetchSettings()
        {
            string projectDirectory = "";
            if (mainParameters != null)
                projectDirectory = mainParameters.ProjectDirectory;

            var config = META.ComComponent.DeserializeConfiguration(projectDirectory, typeof(CyPhy2CADSettings), this.ComponentProgID) as CyPhy2CADSettings;
            settings = config;

            if (settings == null)
            {
                settings = new CyPhy2CADSettings();
            }
        }

        private bool Elaborate(
            MgaProject project,
            MgaFCO currentobj,
            MgaFCOs selectedobjs,
            int param)
        {
            try
            {
                bool Expanded = this.componentParameters[OptionNameIFab] as String == "true";
                if (!Expanded)
                {
                    try
                    {
                        var elaborator = new CyPhyElaborateCS.CyPhyElaborateCSInterpreter();
                        elaborator.Logger = new GMELogger(project);
                        elaborator.Logger.AddWriter(Logger.Instance);
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
                        Logger.Instance.AddLogMessage("Elaborator exception occurred: " + e.Message, Severity.Error);
                        throw new Exception(e.Message);
                    }
                }
            }
            catch (Exception ex)
            {
                Logger.Instance.AddLogMessage(ex.ToString(), Severity.Error);
                return false;
            }

            return true;
        }

        private bool CallComponentExporter(MgaProject project,
                                           MgaFCO currentobj
                                           )
        {
            try
            {
                if (currentobj.MetaBase.Name == "BallisticTestBench" || currentobj.MetaBase.Name == "BlastTestBench")
                {
                    // call component exporter to traverse design and build component index
                    CyPhyComponentExporter.CyPhyComponentExporterInterpreter compExport = new CyPhyComponentExporter.CyPhyComponentExporterInterpreter();
                    compExport.Initialize(project);
                    compExport.TraverseTestBenchForComponentExport(currentobj, this.mainParameters.OutputDirectory, this.mainParameters.ProjectDirectory);
                }
            }
            catch (Exception ex)
            {
                Logger.Instance.AddLogMessage("ACM generation exception from ComponentExporter: " + ex.Message, Severity.Error);
                return false;
            }

            return true;
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
        SortedDictionary<string, object> componentParameters = null;

        public object get_ComponentParameter(string Name)
        {
            if (Name == "type")
                return "csharp";

            if (Name == "path")
                return GetType().Assembly.Location;

            if (Name == "fullname")
                return GetType().FullName;

            if (Name == "name")
                return ComponentConfig.componentName;

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

        public IInterpreterPreConfiguration PreConfig(IPreConfigParameters parameters)
        {
            return null;
        }

        public IInterpreterConfiguration DoGUIConfiguration(IInterpreterPreConfiguration preConfig, IInterpreterConfiguration previousConfig)
        {
            if (previousConfig != null)
            {
                settings = (CyPhy2CADSettings)previousConfig;
            }
            // show main form    
            //FetchSettings();
            bool prepIFab = false;

            using (MainForm mf = new MainForm(settings,
                                              true,
                                              prepIFab))
            {
                mf.ShowDialog();
                DialogResult ok = mf.DialogResult;
                if (ok == DialogResult.OK)
                {
                    settings = mf.ConfigOptions;
                    return settings;
                }
                else
                    return null;
            }
        }
        private bool CopySTL = false;
        private CyPhy2CADSettings settings = new CyPhy2CADSettings();
        private InterpreterResult result = new InterpreterResult();
        private InterpreterMainParameters mainParameters;
        public IInterpreterResult Main(IInterpreterMainParameters parameters)
        {
            result.RunCommand = "runCADJob.bat";
            result.Labels = "Creo&&CADCreoParametricCreateAssembly.exev1.4&&" + JobManager.Job.DefaultLabels;
            var ProjectIsNotInTransaction = (parameters.Project.ProjectStatus & 8) == 0;
            if (ProjectIsNotInTransaction)
            {
                parameters.Project.BeginTransactionInNewTerr();
            }
            Dictionary<string, string> workflowParameters = new Dictionary<string, string>();
            var workflowRef = parameters
                .CurrentFCO
                .ChildObjects
                .OfType<MgaReference>()
                .FirstOrDefault(x => x.Meta.Name == "WorkflowRef");
            if (workflowRef != null)
            {
                string Parameters = workflowRef.Referred
                    .ChildObjects
                    .OfType<MgaAtom>()
                    .FirstOrDefault(x => x.Meta.Name == "Task")
                    .StrAttrByName["Parameters"];
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
            }
            META.AnalysisTool.ApplyToolSelection(this.ComponentProgID, workflowParameters, this.result, parameters, modifyLabels: false);
            if (ProjectIsNotInTransaction)
            {
                parameters.Project.AbortTransaction();
            }

            this.CopySTL = workflowParameters.ContainsValue("FreedLinkageAssembler");



            var resultzip = CyPhy2CAD_CSharp.Properties.Resources.ResultZip;
            result.ZippyServerSideHook = Encoding.UTF8.GetString(resultzip);
            result.LogFileDirectory = Path.Combine(parameters.OutputDirectory, "log");

            this.mainParameters = (InterpreterMainParameters)parameters;
            if (this.mainParameters.config == null)
            {
                var config = META.ComComponent.DeserializeConfiguration(this.mainParameters.ProjectDirectory, typeof(CyPhy2CADSettings), this.ComponentProgID) as CyPhy2CADSettings;
                if (config != null)
                {
                    this.mainParameters.config = config;
                    settings = config;
                }
                else
                {
                    this.mainParameters.config = new CyPhy2CADSettings();
                }

            }
            if (this.result.Traceability == null)
            {
                this.result.Traceability = new META.MgaTraceability();
            }

            // getting traceability from caller, like master interpreter
            if (this.mainParameters.Traceability != null)
            {
                this.mainParameters.Traceability.CopyTo(this.result.Traceability);
            }

            //CyPhy2CAD_CSharp.CyPhy2CADSettings configSettings = (CyPhy2CAD_CSharp.CyPhy2CADSettings)parameters.config;
            //settings = configSettings;

            settings = (CyPhy2CAD_CSharp.CyPhy2CADSettings)this.mainParameters.config;
            settings.OutputDirectory = parameters.OutputDirectory;
            Automation = true;
            Logger.Instance.AddLogMessage("Main:Aux Directory is: " + settings.AuxiliaryDirectory, Severity.Info);
            Logger.Instance.AddLogMessage("Output Directory is: " + settings.OutputDirectory, Severity.Info);
            MgaGateway.voidDelegate action = delegate
            {
                if (Elaborate(parameters.Project, parameters.CurrentFCO, parameters.SelectedFCOs, parameters.StartModeParam))
                {
                    result.Success = Main(parameters.Project, parameters.CurrentFCO, parameters.SelectedFCOs, Convert(parameters.StartModeParam));
                }
                else
                {
                    result.Success = false;
                    Logger.Instance.DumpLog(GMEConsole, LogDir);
                }
            };

            if ((parameters.Project.ProjectStatus & 8) == 0)
            {
                MgaGateway.PerformInTransaction(
                    d: action, 
                    mode: transactiontype_enum.TRANSACTION_NON_NESTED, 
                    abort: true);
            }
            else
            {
                action.Invoke();
            }

            return result;
        }


        public string InterpreterConfigurationProgId
        {
            get
            {
                return (typeof(CyPhy2CADSettings).GetCustomAttributes(typeof(ProgIdAttribute), false)[0] as ProgIdAttribute).Value;
            }
        }
    }
}
