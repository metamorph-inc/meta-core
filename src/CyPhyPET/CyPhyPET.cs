using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using GME.CSharp;
using GME;
using GME.MGA;
using GME.MGA.Core;
using System.Linq;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using System.Diagnostics;
using CyPhyGUIs;
using Newtonsoft.Json;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.Threading;

namespace CyPhyPET
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyPhyPETInterpreter : IMgaComponentEx, IGMEVersionInfo, ICyPhyInterpreter
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

        /// <summary>
        /// This function is called for each interpreter invocation before Main.
        /// Don't perform MGA operations here unless you open a tansaction.
        /// </summary>
        /// <param name="project">The handle of the project opened in GME, for which the interpreter was called.</param>
        public void Initialize(MgaProject project)
        {
            //GMEConsole = GMEConsole.CreateFromProject(project);
            MgaGateway = new MgaGateway(project);
        }

        #region IMgaComponentEx Members

        private MgaGateway MgaGateway { get; set; }
        //private GMEConsole GMEConsole { get; set; }
        public CyPhyGUIs.GMELogger Logger { get; set; }

        public void InvokeEx(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, int param)
        {
            if (!enabled)
            {
                return;
            }

            try
            {
                // Create a new instance of the logger
                this.Logger = new CyPhyGUIs.GMELogger(project, this.ComponentName);

                if (currentobj == null)
                {
                    this.Logger.WriteFailed("CyPhyPET must be called from a Parametric Exploration.");
                    return;
                }

                // Need to call this interpreter in the same way as the MasterInterpreter will call it.
                // initialize main parameters
                var parameters = new InterpreterMainParameters()
                {
                    Project = project,
                    CurrentFCO = currentobj,
                    SelectedFCOs = selectedobjs,
                    StartModeParam = param,
                    VerboseConsole = true
                };

                this.mainParameters = parameters;
                parameters.ProjectDirectory = Path.GetDirectoryName(project.ProjectConnStr.Substring("MGA=".Length));


                // Check if the PCCPropertyInputEditor should be opend.
                // If so return after this transaction
                bool doReturn = false;

                int VK_CONTROL = 0x11;
                // If control was held, try to open GUI.
                if ((bool)((GetKeyState(VK_CONTROL) & 0x8000) == 0x8000) &&
                    (this.mainParameters.StartModeParam & 128) != 128)
                {
                    MgaGateway.PerformInTransaction(delegate
                    {
                        doReturn = this.TryOpenPCCPropertyInputEditor();
                    }, abort: false);
                }

                if (doReturn)
                {
                    return;
                }

                List<string> interpreterProgIDs = new List<string>();

                // Set up the output directory and check kind of currentObj.
                string kindName = string.Empty;
                MgaGateway.PerformInTransaction(delegate
                {
                    string outputDirName = project.Name;
                    if (currentobj != null)
                    {
                        outputDirName = currentobj.Name;
                        kindName = currentobj.MetaBase.Name;
                    }

                    parameters.OutputDirectory = Path.GetFullPath(Path.Combine(
                        parameters.ProjectDirectory,
                        "results",
                        outputDirName));
                    foreach (var testBenchRef in currentobj.ChildObjects.Cast<MgaFCO>().Where(c => c.MetaRole.Name == typeof(CyPhyClasses.TestBenchRef).Name))
                    {
                        var tbRef = CyPhyClasses.TestBenchRef.Cast(testBenchRef);
                        if (tbRef.AllReferred is CyPhy.TestBench)
                        {
                            var testBench = CyPhyClasses.TestBenchRef.Cast(testBenchRef).Referred.TestBench;
                            interpreterProgIDs.Add(Rules.Global.GetInterpreterProgIDFromTestBench(testBench));
                        }
                        else if (tbRef.AllReferred is CyPhy.TestBenchType)
                        {
                            // Assume CyPhy2CAD_CSharp for all other types of test-benches for now.
                            interpreterProgIDs.Add("MGA.Interpreter.CyPhy2CAD_CSharp");
                        }
                    }
                });

                if (string.IsNullOrEmpty(kindName) == false && kindName != typeof(CyPhyClasses.ParametricExploration).Name)
                {
                    this.Logger.WriteFailed("CyPhyPET must be called from a Parametric Exploration.");
                    return;
                }

                PreConfigArgs preConfigArgs = new PreConfigArgs();
                preConfigArgs.ProjectDirectory = parameters.ProjectDirectory;

                // call the preconfiguration with no parameters and get preconfig
                var preConfig = this.PreConfig(preConfigArgs);

                // get previous GUI config
                var previousConfig = META.ComComponent.DeserializeConfiguration(
                    parameters.ProjectDirectory,
                    typeof(CyPhyPET.CyPhyPETSettings),
                    this.ComponentProgID);

                // get interpreter config through GUI
                var config = this.DoGUIConfiguration(preConfig, previousConfig);
                if (config == null)
                {
                    this.Logger.WriteWarning("Operation cancelled by the user.");
                    return;
                }
                META.ComComponent.SerializeConfiguration(parameters.ProjectDirectory, config, this.ComponentProgID);
                // assign the new configuration to mainParameters
                parameters.config = config;

                // External Interpreter configuration
                foreach (var interpreter in interpreterProgIDs.Distinct().Select(progid => new META.ComComponent(progid)))
                {
                    if (interpreter.InterpreterConfig != null)
                    {
                        interpreter.InterpreterConfig = META.ComComponent.DeserializeConfiguration(parameters.ProjectDirectory, interpreter.InterpreterConfig.GetType(), interpreter.ProgId);
                        if (interpreter.DoGUIConfiguration(parameters.ProjectDirectory) == false)
                        {
                            this.Logger.WriteWarning("Operation cancelled by the user.");
                            return;
                        }

                        // if config is valid save it and update it on the file system
                        META.ComComponent.SerializeConfiguration(parameters.ProjectDirectory, config, interpreter.ProgId);
                    }
                }

                // call the main (ICyPhyComponent) function
                this.Main(parameters);

            }
            finally
            {
                if (this.Logger != null)
                {
                    this.Logger.Dispose();
                    this.Logger = null;
                }
                MgaGateway = null;
                project = null;
                currentobj = null;
                selectedobjs = null;
                //GMEConsole = null;
                GC.Collect();
                GC.WaitForPendingFinalizers();
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

        #region CyPhyGUIs

        /// <summary>
        /// Result of the latest run of this interpreter.
        /// </summary>
        private InterpreterResult result = new InterpreterResult();

        /// <summary>
        /// Parameter of this run.
        /// </summary>
        private InterpreterMainParameters mainParameters { get; set; }

        /// <summary>
        /// Output directory where all files must be generated
        /// </summary>
        private string OutputDirectory
        {
            get
            {
                return this.mainParameters.OutputDirectory;
            }
        }

        private void UpdateSuccess(string message, bool success)
        {
            this.result.Success = this.result.Success && success;
            this.runtime.Enqueue(new Tuple<string, TimeSpan>(message, DateTime.Now - this.startTime));
            if (success)
            {
                this.Logger.WriteDebug("{0} : OK", message);
            }
            else
            {
                this.Logger.WriteDebug("{0} : FAILED", message);
            }
        }

        /// <summary>
        /// Name of the log file. (It is not a full path)
        /// </summary>
        //private string LogFileFilename { get; set; }

        /// <summary>
        /// Full path to the log file.
        /// </summary>
        //private string LogFilePath
        //{
        //    get
        //    {
        //        return Path.Combine(this.result.LogFileDirectory, this.LogFileFilename);
        //    }
        //}

        /// <summary>
        /// ProgId of the configuration class of this interpreter.
        /// </summary>
        public string InterpreterConfigurationProgId
        {
            get
            {
                // TODO: if PET configuration is needed, encapsulate the CyPhy2ModelicaSettings
                return (typeof(CyPhyPET.CyPhyPETSettings).GetCustomAttributes(typeof(ProgIdAttribute), false)[0] as ProgIdAttribute).Value;

            }
        }

        /// <summary>
        /// Preconfig gets called first. No transaction is open, but one may be opened.
        /// In this function model may be processed and some object ids get serialized
        /// and returned as preconfiguration (project-wise configuration).
        /// </summary>
        /// <param name="preConfigParameters"></param>
        /// <returns>Null if no configuration is required by the DoGUIConfig.</returns>
        public IInterpreterPreConfiguration PreConfig(IPreConfigParameters parameters)
        {
            // TODO: configure CyPhy2Modelica only if needed (requires a MI change too)
            // CyPhy.ParametricExploration pet = ISIS.GME.Common.Utils.CreateObject<CyPhyClasses.ParametricExploration>(parameters.TestBench as MgaObject);
            //throw new NotImplementedException();

            return null;
        }

        public IInterpreterConfiguration DoGUIConfiguration(IInterpreterPreConfiguration preConfig, IInterpreterConfiguration previousConfig)
        {
            var settings = previousConfig as CyPhyPET.CyPhyPETSettings;

            if (settings == null)
            {
                settings = new CyPhyPET.CyPhyPETSettings();
            }

            return settings;
        }

        private Queue<Tuple<string, TimeSpan>> runtime = new Queue<Tuple<string, TimeSpan>>();
        private DateTime startTime = DateTime.Now;

        /// <summary>
        /// No GUI and interactive elements are allowed within this function.
        /// </summary>
        /// <param name="parameters">Main parameters for this run and GUI configuration.</param>
        /// <returns>Result of the run, which contains a success flag.</returns>
        public IInterpreterResult Main(IInterpreterMainParameters parameters)
        {
            bool disposeLogger = false;
            try
            {
                if (this.Logger == null)
                {
                    this.Logger = new CyPhyGUIs.GMELogger(parameters.Project, this.ComponentName);
                    disposeLogger = true;
                }
                this.Logger.WriteInfo("Running CyPhyPET");
                System.Windows.Forms.Application.DoEvents();

                var asyncResult = this.Logger.LoggingVersionInfo.BeginInvoke(parameters.Project, null, null);
                var header = this.Logger.LoggingVersionInfo.EndInvoke(asyncResult);
                this.Logger.WriteDebug(header);

                MainThrows(parameters);
            }
            catch (Exception ex)
            {
                this.Logger.WriteError("Exception was thrown : {0}", ex.ToString().Replace("\n", "<br>"));
                this.result.Success = false;
            }
            finally
            {
                if (disposeLogger && this.Logger != null)
                {
                    this.Logger.Dispose();
                    this.Logger = null;
                }
            }

            return this.result;
        }

        /// <summary>
        /// Actual implementation of Main, does not catch general exceptions.
        /// </summary>
        /// <param name="parameters">Main parameters for this run and GUI configuration.</param>
        /// <returns>Result of the run, which contains a success flag.</returns>
        public IInterpreterResult MainThrows(IInterpreterMainParameters parameters)
        {
            this.runtime.Clear();

            this.mainParameters = (InterpreterMainParameters)parameters;
            var configSuccess = this.mainParameters != null;
            if (configSuccess && this.mainParameters.config == null)
            {
                var config = META.ComComponent.DeserializeConfiguration(this.mainParameters.ProjectDirectory, typeof(CyPhyPETSettings), this.ComponentProgID) as
                    CyPhyPETSettings;
                if (config != null)
                {
                    this.mainParameters.config = config;
                }
                else
                {
                    this.mainParameters.config = new CyPhyPETSettings();
                }

                configSuccess = this.mainParameters.config != null;
            }

            bool disposeLogger = false;
            if (this.Logger == null)
            {
                this.Logger = new CyPhyGUIs.GMELogger(this.mainParameters.Project, this.ComponentName);
                disposeLogger = true;
            }
            this.Logger.MakeVersionInfoHeader();
            if (this.mainParameters.VerboseConsole)
            {
                this.Logger.GMEConsoleLoggingLevel = SmartLogger.MessageType_enum.Debug;
            }
            else
            {
                this.Logger.GMEConsoleLoggingLevel = SmartLogger.MessageType_enum.Info;
            }

            //this.result.Traceability.CopyTo(this.Logger.Traceability);

            this.UpdateSuccess("Configuration", configSuccess);

            this.result.Labels = "";
            this.result.RunCommand = "run_PCC.cmd";

            //this.result.LogFileDirectory = Path.Combine(this.mainParameters.ProjectDirectory, "log");
            //this.LogFileFilename = this.ComponentName + "." + System.Diagnostics.Process.GetCurrentProcess().Id + ".log";
            //META.Logger.AddFileListener(this.LogFilePath, this.ComponentName, parameters.Project);

            try
            {
                MgaGateway.PerformInTransaction(delegate
                {
                    this.WorkInMainTransaction();
                },
                transactiontype_enum.TRANSACTION_NON_NESTED, abort: true);

                this.PrintRuntimeStatistics();
                if (this.result.Success)
                {
                    this.Logger.WriteInfo("CyPhyPET finished successfully.");
                    this.Logger.WriteInfo("Generated files are here: <a href=\"file:///{0}\" target=\"_blank\">{0}</a>", this.mainParameters.OutputDirectory);
                    this.Logger.WriteDebug("[SUCCESS: {0}, Labels: {1}]", this.result.Success, this.result.Labels);
                }
                else
                {
                    this.Logger.WriteError("PET Interpreter failed! See error messages above.");
                }
            }
            catch (ApplicationException e)
            {
                this.result.Success = false;
                this.Logger.WriteError(String.Format("PET Interpreter failed: {0}", e.Message));
            }
            finally
            {
                if (disposeLogger && this.Logger != null)
                {
                    this.Logger.Dispose();
                    this.Logger = null;
                }
                MgaGateway = null;
                //GMEConsole = null; 
                GC.Collect();
                GC.WaitForPendingFinalizers();
                GC.Collect();
                GC.WaitForPendingFinalizers();
            }

            //META.Logger.RemoveFileListener(this.ComponentName);

            return this.result;
        }

        private void PrintRuntimeStatistics()
        {
            this.Logger.WriteDebug("======================================================");
            this.Logger.WriteDebug("Start time: {0}", this.startTime);
            foreach (var time in this.runtime)
            {
                this.Logger.WriteDebug("{0} = {1}", time.Item1, time.Item2);
            }

            this.Logger.WriteDebug("======================================================");
        }

        #endregion

        #region CyPhyPET Specific code

        private void WorkInMainTransaction()
        {
            this.Logger.WriteInfo("{0} [{1}]", this.mainParameters.CurrentFCO.Name, this.mainParameters.CurrentFCO.MetaBase.Name);
            this.result.Success = true;
            // 1) check model, if fails return success = false
            this.Logger.WriteDebug("Checking rules...");
            var checker = new Rules.Checker(this.mainParameters, this.Logger, this.result.Traceability);
            checker.Check();
            var succesRules = checker.Success;
            this.UpdateSuccess("PET check", checker.Success);
            checker.PrintDetails();

            if (checker.Success == false)
            {
                return;
            }

            var petGenerator = new PET(this.mainParameters, this.Logger);

            // 2) Get the type of test-bench and call any dependent interpreters
            //var graph = CyPhySoT.CyPhySoTInterpreter.UpdateDependency((MgaModel)this.mainParameters.CurrentFCO, this.Logger);

            foreach (var testBenchRef in CyPhyClasses.ParametricExploration.Cast(this.mainParameters.CurrentFCO).Children.TestBenchRefCollection.OrderBy(x => x.ID))
            {
                var outputFolder = "TestBench_" + testBenchRef.Name;
                bool interpreterSuccess = false;
                if (testBenchRef != null &&
                    testBenchRef.AllReferred != null
                    && testBenchRef.AllReferred is CyPhy.TestBench)
                {
                    var testBench = testBenchRef.AllReferred as CyPhy.TestBench;

                    var interpreterProgID = Rules.Global.GetInterpreterProgIDFromTestBench(testBench);
                    switch (interpreterProgID)
                    {
                        case "MGA.Interpreter.CyPhy2CAD_CSharp":
                            interpreterSuccess = this.CallCyPhy2CAD_CSharp(testBench, outputFolder);
                            break;

                        case "MGA.Interpreter.CyPhyFormulaEvaluator":
                            interpreterSuccess = true;
                            break;

                        case "MGA.Interpreter.CyPhyPython":
                            interpreterSuccess = this.CallCyPhyPython(testBench, outputFolder);
                            break;

                        default:
                            interpreterSuccess = this.CallIntererpreterAndAddRunCommandToManifest(interpreterProgID, testBench, outputFolder);
                            break;
                    }
                    this.UpdateSuccess("Interpreter successful : ", interpreterSuccess);
                }
                else if (testBenchRef != null && testBenchRef.AllReferred != null && testBenchRef.AllReferred is CyPhy.TestBenchType)
                {
                    interpreterSuccess = this.CallCyPhy2CAD_CSharp(testBenchRef.AllReferred as CyPhy.TestBenchType, outputFolder);
                    this.UpdateSuccess("Assuming call to CAD successful : ", true);
                }
                else
                {
                    throw new NotSupportedException(string.Format(@"Error: {0} must contain one non-null reference to a TestBench",
                        this.mainParameters.CurrentFCO.ToMgaHyperLink()));
                }

                if (interpreterSuccess == false)
                {
                    Logger.WriteError("TestBench {0} failed", testBenchRef.Name);
                }
                else
                {
                    petGenerator.GenerateCode(testBenchRef, outputFolder);
                }
            }

            foreach (var excel in CyPhyClasses.ParametricExploration.Cast(this.mainParameters.CurrentFCO).Children.ExcelWrapperCollection.OrderBy(x => x.ID))
            {
                petGenerator.GenerateCode(excel);
            }
            foreach (var matlab in CyPhyClasses.ParametricExploration.Cast(this.mainParameters.CurrentFCO).Children.MATLABWrapperCollection.OrderBy(x => x.ID))
            {
                petGenerator.GenerateCode(matlab);
            }
            foreach (var python in CyPhyClasses.ParametricExploration.Cast(this.mainParameters.CurrentFCO).Children.PythonWrapperCollection.OrderBy(x => x.ID))
            {
                petGenerator.GenerateCode(python);
            }

            if (this.result.Success)
            {
                // 3) Generate scripts
                petGenerator.GenerateDriverCode();
                this.result.RunCommand = petGenerator.RunCommand;
                this.result.Labels += petGenerator.Label;
            }
        }

        private bool TryOpenPCCPropertyInputEditor()
        {
            bool doReturn = false;
            var currentobj = this.mainParameters.CurrentFCO;
            if (currentobj == null)
            {
                //this.Logger.WriteError("Please run CyPhyPET on either or a test-bench or component.");
                doReturn = true;
            }
            else if (currentobj.MetaBase.Name == typeof(CyPhy.Component).Name)
            {
                this.Logger.WriteInfo("CyPhyPET called on a component.");
                var component = CyPhyClasses.Component.Cast(currentobj);
                if (component.Children.ModelicaModelCollection.Any())
                {
                    this.Logger.WriteInfo("Opening PCC Property Input Editor");
                    var pccPropertyGUI = new PCCPropertyInputDistributionEditor(this.mainParameters, this.Logger);
                    if (pccPropertyGUI.IsDisposed == false)
                    {
                        pccPropertyGUI.ShowDialog();
                    }
                }
                else
                {
                    this.Logger.WriteError("There is no ModelicaModel in the component.");
                    this.Logger.WriteError("This is required in order to open up the PCC Property Input Editor.");
                }

                doReturn = true;
            }

            return doReturn;
        }

        private bool CallIntererpreterAndAddRunCommandToManifest(string progid, CyPhy.TestBench testBench, string outputDirName)
        {
            string name = progid.Split('.').Last();
            var interpreter = CallInterpreter(testBench, progid, name, outputDirName, initializeManifest: true);
            if (interpreter.result.Success && string.IsNullOrEmpty(interpreter.result.RunCommand) == false)
            {
                // if RunCommand is specified, add it to the manifest
                var tbManifest = AVM.DDP.MetaTBManifest.OpenForUpdate(interpreter.MainParameters.OutputDirectory);

                tbManifest.AddAllTasks(testBench, new META.ComComponent[] { interpreter });
                tbManifest.Serialize(interpreter.MainParameters.OutputDirectory);

                return true;

            }
            return interpreter.result.Success;
        }

        private bool CallCyPhy2CAD_CSharp(CyPhy.TestBenchType testBench, string outputDirName)
        {
            var cyPhy2CAD = CallInterpreter(testBench, "MGA.Interpreter.CyPhy2CAD_CSharp", "CyPhy2CAD_CSharp", outputDirName);
            if (cyPhy2CAD.result.Success)
            {
                var tbManifest = AVM.DDP.MetaTBManifest.OpenForUpdate(cyPhy2CAD.MainParameters.OutputDirectory);

                tbManifest.AddAllTasks(testBench, new META.ComComponent[] { cyPhy2CAD });

                tbManifest.Steps.Insert(0, new AVM.DDP.MetaTBManifest.Step()
                {
                    Status = AVM.DDP.MetaTBManifest.StatusEnum.UNEXECUTED,
                    Invocation = String.Format("\"{0}\" -E -m run_mdao.cad.update_parameters", META.VersionInfo.PythonVEnvExe),
                    Description = "Update parameters in CADAssembly.xml"
                });

                tbManifest.Serialize(cyPhy2CAD.MainParameters.OutputDirectory);

                return true;

            }
            return cyPhy2CAD.result.Success;
        }

        private META.ComComponent CallInterpreter(CyPhy.TestBenchType testBench, string progid, string name, string outputDirName, bool initializeManifest = false)
        {
            var interpreter = new META.ComComponent(progid);

            if (interpreter.InterpreterConfig != null)
            {
                interpreter.InterpreterConfig = META.ComComponent.DeserializeConfiguration(
                    this.mainParameters.ProjectDirectory,
                    interpreter.InterpreterConfig.GetType(),
                    interpreter.ProgId);
            }

            CyPhy.Task task = null;
            string result = string.Empty;
            if (testBench.Children.WorkflowRefCollection.Count() == 1)
            {
                var workflowRef = testBench.Children.WorkflowRefCollection.FirstOrDefault();
                if (workflowRef != null &&
                    workflowRef.Referred.Workflow != null)
                {
                    var workflow = workflowRef.Referred.Workflow;
                    if (workflow.Children.TaskCollection.Count() == 1)
                    {
                        task = workflow.Children.TaskCollection.FirstOrDefault();
                    }
                }
            }

            if (task != null)
            {
                string parameters = task.Attributes.Parameters;
                try
                {
                    interpreter.WorkflowParameters = (Dictionary<string, string>)JsonConvert.DeserializeObject(parameters, typeof(Dictionary<string, string>));
                    if (interpreter.WorkflowParameters == null)
                    {
                        interpreter.WorkflowParameters = new Dictionary<string, string>();
                    }
                }
                catch (JsonException ex)
                {
                    throw new ApplicationException(String.Format("Could not parse Parameters for '{0}'", task.Name), ex);
                }
            }

            interpreter.SetWorkflowParameterValues();

            interpreter.Initialize(this.mainParameters.Project);

            var diTestBenchOutputDir = Directory.CreateDirectory(Path.Combine(OutputDirectory, outputDirName));

            interpreter.MainParameters.OutputDirectory = diTestBenchOutputDir.FullName;
            interpreter.MainParameters.CurrentFCO = (MgaFCO)testBench.Impl;
            // FIXME: do we need to set interpreter.WorkflowParameters ?
            interpreter.MainParameters.Project = this.mainParameters.Project;
            interpreter.MainParameters.ProjectDirectory = this.mainParameters.ProjectDirectory;
            interpreter.MainParameters.SelectedFCOs = this.mainParameters.SelectedFCOs;
            interpreter.MainParameters.StartModeParam = this.mainParameters.StartModeParam;
            interpreter.MainParameters.VerboseConsole = this.mainParameters.VerboseConsole;

            if (initializeManifest)
            {
                AVM.DDP.MetaTBManifest tbManifest = new AVM.DDP.MetaTBManifest();
                tbManifest.MakeManifest(testBench, diTestBenchOutputDir.FullName);
                tbManifest.Serialize(diTestBenchOutputDir.FullName);
            }
            try
            {
                interpreter.Main();
            }
            catch (META.InterpreterException)
            {
                //Trace.TraceError(ex.ToString());
                this.Logger.WriteError(name + " failed!");
                // cyPhy2CAD.result.Success = false;
                return interpreter;
            }

            if (interpreter.result.Success)
            {
                this.result.Labels += (this.result.Labels.Length > 0 ? " && " : "") + interpreter.result.Labels;
                return interpreter;
            }
            else
            {
                return interpreter;
            }
        }

        private bool CallCyPhyPython(CyPhy.TestBenchType testBench, string outputDirName)
        {
            var cyPhyPython = new META.ComComponent("MGA.Interpreter.CyPhyPython");

            //cyPhyPython.InterpreterConfig = META.ComComponent.DeserializeConfiguration(
            //    this.mainParameters.ProjectDirectory,
            //   typeof(CyPhyPython.CyPhyPythonSettings),
            //    cyPhyPython.ProgId);

            cyPhyPython.Initialize(this.mainParameters.Project);

            var workflowRef = testBench.Children.WorkflowRefCollection.FirstOrDefault();
            if (workflowRef == null || workflowRef.Referred.Workflow == null)
            {
                throw new ApplicationException("CyPhyPython testbench must have a WorkflowRef");
            }
            var task = workflowRef.Referred.Workflow.Children.TaskCollection.Where(t => t.Attributes.COMName == "MGA.Interpreter.CyPhyPython").FirstOrDefault();
            if (task == null)
            {
                throw new ApplicationException("CyPhyPython testbench must have Task");
            }
            try
            {
                cyPhyPython.WorkflowParameters = (Dictionary<string, string>)JsonConvert.DeserializeObject(task.Attributes.Parameters, typeof(Dictionary<string, string>));
                if (cyPhyPython.WorkflowParameters == null)
                {
                    cyPhyPython.WorkflowParameters = new Dictionary<string, string>();
                }
            }
            catch (JsonException ex)
            {
                throw new ApplicationException(String.Format("Could not parse Parameters for '{0}'", task.Name), ex);
            }
            cyPhyPython.DoGUIConfiguration(this.mainParameters.ProjectDirectory, false);

            var diTestBenchOutputDir = Directory.CreateDirectory(Path.Combine(OutputDirectory, outputDirName));

            cyPhyPython.MainParameters.OutputDirectory = diTestBenchOutputDir.FullName;
            cyPhyPython.MainParameters.CurrentFCO = (MgaFCO)testBench.Impl;

            cyPhyPython.MainParameters.Project = this.mainParameters.Project;
            cyPhyPython.MainParameters.ProjectDirectory = this.mainParameters.ProjectDirectory;
            cyPhyPython.MainParameters.SelectedFCOs = this.mainParameters.SelectedFCOs;
            cyPhyPython.MainParameters.StartModeParam = this.mainParameters.StartModeParam;
            cyPhyPython.MainParameters.VerboseConsole = this.mainParameters.VerboseConsole;

            AVM.DDP.MetaTBManifest tbManifest = new AVM.DDP.MetaTBManifest();
            tbManifest.MakeManifest(testBench, cyPhyPython.MainParameters.OutputDirectory);

            tbManifest.Serialize(cyPhyPython.MainParameters.OutputDirectory);

            bool success;
            try
            {
                cyPhyPython.Main();
                success = cyPhyPython.result.Success;
            }
            catch (COMException e)
            {
                Logger.WriteDebug("CyPhyPython on '{0}' failed: {1}", testBench.Name, e.ToString());
                success = false;
            }

            if (success)
            {
                this.result.Labels += cyPhyPython.result.Labels;

                if (string.IsNullOrWhiteSpace(cyPhyPython.result.RunCommand) == false)
                {
                    tbManifest = AVM.DDP.MetaTBManifest.OpenForUpdate(cyPhyPython.MainParameters.OutputDirectory);

                    tbManifest.AddAllTasks(testBench, new META.ComComponent[] { cyPhyPython });
                    tbManifest.Serialize(cyPhyPython.MainParameters.OutputDirectory);
                }

                return true;
            }
            else
            {
                return false;
            }
        }

        public void EditButtonClicked(object fco)
        {
            DecoratorCallback(fco, () =>
            {
                string filename;
                if (((IMgaFCO)fco).Meta.Name == "ExcelWrapper")
                {
                    // ISIS.GME.Dsml.CyPhyML.Interfaces.ExcelWrapper ew;
                    // ew.Attributes.ExcelFilename
                    filename = ((IMgaFCO)fco).GetStrAttrByNameDisp("ExcelFilename");
                }
                else if (((IMgaFCO)fco).Meta.Name == "MATLABWrapper")
                {
                    // ISIS.GME.Dsml.CyPhyML.Interfaces.MATLABWrapper wrapper;
                    // wrapper.Attributes.MFilename
                    filename = ((IMgaFCO)fco).GetStrAttrByNameDisp("MFilename");
                }
                else if (((IMgaFCO)fco).Meta.Name == "PythonWrapper")
                {
                    // ISIS.GME.Dsml.CyPhyML.Interfaces.PythonWrapper wrap;
                    // wrap.Attributes.PyFilename
                    filename = ((IMgaFCO)fco).GetStrAttrByNameDisp("PyFilename");
                }
                else
                {
                    return;
                }
                string editor = "notepad.exe";
                GME.Util.IMgaRegistrar reg = new GME.Util.MgaRegistrar();
                foreach (var mode in new GME.Util.regaccessmode_enum[] { GME.Util.regaccessmode_enum.REGACCESS_USER, GME.Util.regaccessmode_enum.REGACCESS_SYSTEM })
                {
                    if (reg.ExternalEditorEnabled[mode])
                    {
                        editor = reg.ExternalEditor[mode];
                        break;
                    }
                }
                Regex argParser = new Regex("(\"[^\"]*\"|[^\" \t]*)(?:[ \t](.*))?");
                /*
                argParser.Match("notepad.exe").Groups
                argParser.Match("notepad.exe 12 123").Groups
                argParser.Match("\"notepad.exe\"").Groups
                argParser.Match("\"C:\\Program Files\\notepad++.exe\" -multiInst").Groups
                */
                var match = argParser.Match(editor);
                var editorFilename = match.Groups[1].Value;
                if (editorFilename[0] == '\"')
                {
                    editorFilename = editorFilename.Substring(1, editorFilename.Length - 2);
                }
                var args = match.Groups[2].Value;

                Process p = new Process();
                p.StartInfo.FileName = editorFilename;
                p.StartInfo.Arguments = String.Format("{0} \"{1}\"", args, filename);
                p.StartInfo.WorkingDirectory = Path.GetDirectoryName(((IMgaFCO)fco).Project.ProjectConnStr.Substring("MGA=".Length));
                using (p)
                {
                    p.Start();
                    /*
                     * TODO: automatically refresh wrapper after user is done editing
                    p.WaitForInputIdle();
                    Thread.Sleep(2000);
                    if (p.HasExited == false)
                    {
                        // assume user is editing file, editor will exit when done
                        // TODO: pop up a dialog
                        p.WaitForExit();
                    }
                    */
                }
            });
        }

        public void RefreshButtonClicked(object fco)
        {
            DecoratorCallback(fco, () =>
            {
                if (((IMgaFCO)fco).Meta.Name == "ExcelWrapper")
                {
                    CreateParametersAndMetricsForExcel(fco);
                }
                else if (((IMgaFCO)fco).Meta.Name == "MATLABWrapper")
                {
                    var wrapper = CyPhyClasses.MATLABWrapper.Cast((IMgaFCO)fco);
                    string filename = CreateParametersAndMetricsForOpenMDAOComponent("MATLAB file|*.m|All Files (*.*)|*.*", ".m", wrapper.Attributes.MFilename, wrapper,
                        (filename_, model) => GetParamsAndUnknownsFromPythonExe(filename_, "matlab_wrapper", model),
                        () => CyPhyClasses.Metric.Create(wrapper), () => CyPhyClasses.Parameter.Create(wrapper));
                    if (filename != null)
                    {
                        wrapper.Attributes.MFilename = filename;
                    }
                }
                else if (((IMgaFCO)fco).Meta.Name == "PythonWrapper")
                {
                    var wrapper = CyPhyClasses.PythonWrapper.Cast((IMgaFCO)fco);
                    string filename = CreateParametersAndMetricsForOpenMDAOComponent("Python file|*.py;*.pyd|All Files (*.*)|*.*", ".py", wrapper.Attributes.PyFilename, wrapper, GetParamsAndUnknownsForPythonOpenMDAO,
                        () => CyPhyClasses.Metric.Create(wrapper), () => CyPhyClasses.Parameter.Create(wrapper));
                    if (filename != null)
                    {
                        wrapper.Attributes.PyFilename = filename;
                    }
                }
            });
        }

        private void DecoratorCallback(object fco, Action f)
        {
            GMEConsole console = GMEConsole.CreateFromProject(((IMgaFCO)fco).Project);
            try
            {
                f();
            }
            catch (Exception e)
            {
                console.Error.WriteLine(e.Message);
            }
            finally
            {
                GC.Collect();
                GC.WaitForPendingFinalizers();
                GC.Collect();
                GC.WaitForPendingFinalizers();
            }
        }

        private string CreateParametersAndMetricsForOpenMDAOComponent(string openFileFilter, string defaultExt, string oldFilename, CyPhy.ParametricTestBench tb,
            Func<string, ISIS.GME.Common.Interfaces.Model, Dictionary<string, Dictionary<string, Dictionary<string, object>>>> GetParamsAndUnknowns, Func<CyPhy.Metric> metricCreate, Func<CyPhy.Parameter> paramCreate)
        {
            string mgaDir = Path.GetDirectoryName(Path.GetFullPath(tb.Impl.Project.ProjectConnStr.Substring("MGA=".Length)));
            oldFilename = Path.Combine(mgaDir, oldFilename);
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.DefaultExt = defaultExt;
            dialog.CheckFileExists = true;
            if (String.IsNullOrEmpty(oldFilename) == false
                && Directory.Exists(Path.GetDirectoryName(oldFilename)))
            {
                dialog.InitialDirectory = Path.GetDirectoryName(oldFilename);
                if (File.Exists(oldFilename))
                {
                    dialog.FileName = Path.GetFileName(oldFilename);
                    dialog.ShowHelp = true; // https://connect.microsoft.com/VisualStudio/feedback/details/525070/openfiledialog-show-part-of-file-name-in-win7
                }
            }
            else
            {
                dialog.InitialDirectory = mgaDir;
            }
            dialog.Filter = openFileFilter;
            if (dialog.ShowDialog() != DialogResult.OK)
            {
                return null;
            }

            var valueFlow = ((GME.MGA.Meta.IMgaMetaModel)tb.Impl.MetaBase).AspectByName["ValueFlowAspect"];

            Dictionary<string, Dictionary<string, Dictionary<string, object>>> paramsAndUnknowns = GetParamsAndUnknowns(dialog.FileName, tb);

            HashSet<ISIS.GME.Common.Interfaces.FCO> metricsAndParameters = new HashSet<ISIS.GME.Common.Interfaces.FCO>(new DsmlFCOComparer());
            foreach (var metricOrParameter in tb.Children.MetricCollection.Concat<ISIS.GME.Common.Interfaces.FCO>(tb.Children.ParameterCollection))
            {
                metricsAndParameters.Add(metricOrParameter);
            }

            Action<ISIS.GME.Common.Interfaces.FCO, Dictionary<string, object>> setUnit = (fco, metadata) =>
            {
                object gme_unit_id;
                if (metadata.TryGetValue("gme_unit_id", out gme_unit_id))
                {
                    var unit = tb.Impl.Project.GetFCOByID((string)gme_unit_id);
                    ((IMgaReference)fco.Impl).Referred = unit;
                }
                else
                {
                    if (metadata.ContainsKey("units") == false)
                    {
                        ((IMgaReference)fco.Impl).Referred = null;
                    }
                    else
                    {
                        GMEConsole console = GMEConsole.CreateFromProject(((IMgaFCO)fco.Impl).Project);
                        console.Warning.WriteLine(String.Format("Couldn't find CyPhy unit for OpenMDAO unit \"{0}\" for \"{1}\"", metadata["units"], fco.Name));
                    }
                }

            };

            int i = 1;
            foreach (var item in paramsAndUnknowns["unknowns"].OrderBy(x => x.Key))
            {
                string name = item.Key;
                var metric = tb.Children.MetricCollection.Where(m => m.Name == name).FirstOrDefault();
                if (metric == null)
                {
                    metric = metricCreate();
                    metric.Name = name;
                    // metric.Attributes.Description =
                }
                else
                {
                    metricsAndParameters.Remove(metric);
                }
                setUnit(metric, item.Value);
                ((IMgaFCO)metric.Impl).GetPartDisp(valueFlow).SetGmeAttrs(null, 800, i * 65);
                i++;
            }
            i = 1;
            foreach (var item in paramsAndUnknowns["params"].OrderBy(x => x.Key))
            {
                string name = item.Key;
                var param = tb.Children.ParameterCollection.Where(m => m.Name == name).FirstOrDefault();
                if (param == null)
                {
                    param = paramCreate();
                    param.Name = name;
                    // param.Attributes.Description =
                }
                else
                {
                    metricsAndParameters.Remove(param);
                }
                setUnit(param, item.Value);
                ((IMgaFCO)param.Impl).GetPartDisp(valueFlow).SetGmeAttrs(null, 20, i * 65);
                i++;
            }
            foreach (var metricOrParameter in metricsAndParameters)
            {
                // no longer in MATLAB function declaration
                metricOrParameter.Delete();
            }
            var filename = dialog.FileName;
            // make relative if filename is in same directory as GME project
            if (filename.StartsWith(mgaDir + "\\"))
            {
                filename = filename.Substring((mgaDir + "\\").Length);
            }
            return filename;
        }

        public static Dictionary<string, Dictionary<string, Dictionary<string, object>>> GetParamsAndUnknownsForPythonOpenMDAO(string filename, ISIS.GME.Common.Interfaces.Model obj)
        {

            var cyPhyPython = (IMgaComponentEx)Activator.CreateInstance(Type.GetTypeFromProgID("MGA.Interpreter.CyPhyPython"));
            // cyPhyPython.ComponentParameter["script_file"] = Path.Combine(META.VersionInfo.MetaPath, "bin", "CyPhyPET_unit_setter.py");
            cyPhyPython.ComponentParameter["script_file"] = "CyPhyPET_unit_matcher.py";

            var fcos = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));

            cyPhyPython.ComponentParameter["openmdao_py"] = filename;

            cyPhyPython.InvokeEx(obj.Impl.Project, (MgaFCO)obj.Impl, fcos, 128);

            var value = (string)cyPhyPython.ComponentParameter["ret"];
            return Newtonsoft.Json.JsonConvert.DeserializeObject<Dictionary<string, Dictionary<string, Dictionary<string, object>>>>(value);
        }

        private static Dictionary<string, Dictionary<string, Dictionary<string, object>>> GetParamsAndUnknownsFromPythonExe(string filename, string pythonModule, ISIS.GME.Common.Interfaces.Model obj)
        {
            Process getParamsAndUnknowns = new Process();
            getParamsAndUnknowns.StartInfo = new ProcessStartInfo(META.VersionInfo.PythonVEnvExe)
            {
                CreateNoWindow = true,
                UseShellExecute = false,
                RedirectStandardError = true,
                RedirectStandardOutput = true,
                Arguments = String.Format("-m {0} \"{1}\"", pythonModule, filename),
            };

            getParamsAndUnknowns.Start();
            // n.b. assume buffers will not fill up and deadlock us
            string stdout = getParamsAndUnknowns.StandardOutput.ReadToEnd();
            string stderr = getParamsAndUnknowns.StandardError.ReadToEnd();
            getParamsAndUnknowns.WaitForExit();
            if (getParamsAndUnknowns.ExitCode != 0)
            {
                throw new ApplicationException(stderr);
            }

            // e.g. { "unknowns": { "m": { "shape": 1, "val": 0.0, "size": 1}, "s": { "shape": 1, "val": 0.0, "size": 1} }, "params": { "a": { "shape": 1, "val": 0.0, "size": 1}, "b": { "shape": 1, "val": 0.0, "size": 1}, "c": { "shape": 1, "val": 0.0, "size": 1} } }
            return Newtonsoft.Json.JsonConvert.DeserializeObject<Dictionary<string, Dictionary<string, Dictionary<string, object>>>>(stdout.Replace("\r", "").Split('\n').Last(line => string.IsNullOrWhiteSpace(line) == false));
        }

        private static void CreateParametersAndMetricsForExcel(object fco)
        {
            var excel = CyPhyClasses.ExcelWrapper.Cast((IMgaFCO)fco);
            string mgaDir = Path.GetDirectoryName(Path.GetFullPath(excel.Impl.Project.ProjectConnStr.Substring("MGA=".Length)));
            var filename = excel.Attributes.ExcelFilename;
            filename = Path.Combine(mgaDir, filename);
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.DefaultExt = ".xlsx";
            dialog.CheckFileExists = true;
            if (String.IsNullOrEmpty(filename) == false
                && Directory.Exists(Path.GetDirectoryName(filename)))
            {
                dialog.InitialDirectory = Path.GetDirectoryName(filename);

                if (File.Exists(filename))
                {
                    dialog.FileName = Path.GetFileName(filename);
                    dialog.ShowHelp = true; // https://connect.microsoft.com/VisualStudio/feedback/details/525070/openfiledialog-show-part-of-file-name-in-win7
                }

            }
            dialog.Filter = "Excel File|*.xlsx;*.xlsm;*.xlsb;*.xls|All Files (*.*)|*.*";
            if (dialog.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            excel.Attributes.ExcelFilename = dialog.FileName;
            // make relative if possible
            if (excel.Attributes.ExcelFilename.StartsWith(mgaDir + "\\"))
            {
                excel.Attributes.ExcelFilename = excel.Attributes.ExcelFilename.Substring((mgaDir + "\\").Length);
            }

            HashSet<ISIS.GME.Common.Interfaces.FCO> metricsAndParameters = new HashSet<ISIS.GME.Common.Interfaces.FCO>(new DsmlFCOComparer());
            foreach (var metricOrParameter in excel.Children.MetricCollection.Concat<ISIS.GME.Common.Interfaces.FCO>(excel.Children.ParameterCollection))
            {
                metricsAndParameters.Add(metricOrParameter);
            }

            var valueFlow = ((GME.MGA.Meta.IMgaMetaModel)excel.Impl.MetaBase).AspectByName["ValueFlowAspect"];
            ExcelInterop.GetExcelInputsAndOutputs(dialog.FileName, (string name, string refersTo) =>
            {
                var metric = excel.Children.MetricCollection.Where(m => m.Name == name).FirstOrDefault();
                if (metric == null)
                {
                    metric = CyPhyClasses.Metric.Create(excel);
                    metric.Name = name;
                    metric.Attributes.Description = refersTo;
                    ((IMgaFCO)metric.Impl).GetPartDisp(valueFlow).SetGmeAttrs(null, 800, 300);
                }
                else
                {
                    metricsAndParameters.Remove(metric);
                }
            }, (string name, string refersTo) =>
            {
                var param = excel.Children.ParameterCollection.Where(m => m.Name == name).FirstOrDefault();
                if (param == null)
                {
                    param = CyPhyClasses.Parameter.Create(excel);
                    param.Name = name;
                    param.Attributes.Description = refersTo;
                }
                else
                {
                    metricsAndParameters.Remove(param);
                }
            }, () =>
            {
                foreach (var metricOrParameter in metricsAndParameters)
                {
                    // no longer in Excel doc
                    metricOrParameter.Delete();
                }
            });
        }

        [DllImport("user32.dll")]
        public static extern short GetKeyState(int Key);
        #endregion
    }

    public class DsmlFCOComparer : IEqualityComparer<ISIS.GME.Common.Interfaces.FCO>
    {
        public bool Equals(ISIS.GME.Common.Interfaces.FCO x, ISIS.GME.Common.Interfaces.FCO y)
        {
            return x.Impl.Project == y.Impl.Project && x.ID == y.ID;
        }

        public int GetHashCode(ISIS.GME.Common.Interfaces.FCO obj)
        {
            return obj.ID.GetHashCode();
        }
    }

}