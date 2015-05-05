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
            project.CreateTerritoryWithoutSink(out MgaGateway.territory);
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
                if ((bool)((GetKeyState(VK_CONTROL) & 0x8000) == 0x8000))
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

                string interpreterProgID = "";

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
                    var testBenchRef = currentobj.ChildObjects.Cast<MgaFCO>().Where(c => c.MetaRole.Name == typeof(CyPhyClasses.TestBenchRef).Name).FirstOrDefault();
                    if (testBenchRef != null)
                    {
                        var tbRef = CyPhyClasses.TestBenchRef.Cast(testBenchRef);
                        if (tbRef.AllReferred is CyPhy.TestBench)
                        {
                            var testBench = CyPhyClasses.TestBenchRef.Cast(testBenchRef).Referred.TestBench;
                            interpreterProgID = Rules.Global.GetInterpreterProgIDFromTestBench(testBench);
                        }
                        else if (tbRef.AllReferred is CyPhy.TestBenchType)
                        {
                            // Assume CyPhy2CAD_CSharp for all other types of test-benches for now.
                            interpreterProgID = "MGA.Interpreter.CyPhy2CAD_CSharp";
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

                // External Interpreter configuration
                if (interpreterProgID == "MGA.Interpreter.CyPhy2Modelica_v2")
                {
                    var cyPhy2Modelica_v2 = new META.ComComponent("MGA.Interpreter.CyPhy2Modelica_v2");
                    if (cyPhy2Modelica_v2.DoGUIConfiguration(parameters.ProjectDirectory) == false)
                    {
                        this.Logger.WriteWarning("Operation cancelled by the user.");
                        return;
                    }
                }
                else if (interpreterProgID == "MGA.Interpreter.CyPhy2CAD_CSharp")
                {
                    var cyPhy2CAD_CSharp = new META.ComComponent("MGA.Interpreter.CyPhy2CAD_CSharp");
                    if (cyPhy2CAD_CSharp.DoGUIConfiguration(parameters.ProjectDirectory) == false)
                    {
                        this.Logger.WriteWarning("Operation cancelled by the user.");
                        return;
                    }
                }

                // if config is valid save it and update it on the file system
                META.ComComponent.SerializeConfiguration(parameters.ProjectDirectory, config, this.ComponentProgID);

                // assign the new configuration to mainParameters
                parameters.config = config;

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
                if (MgaGateway != null &&
                    MgaGateway.territory != null)
                {
                    MgaGateway.territory.Destroy();
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

        #region Possibly Dependent Interpreters

        private bool CallCyPhy2Modelica_v2(
            MgaProject project,
            MgaFCO currentobj,
            MgaFCOs selectedobjs,
            int param)
        {
            bool result = false;

            return result;
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
                this.Logger.WriteError("Exception was thrown : {0}", ex.ToString());
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
            finally
            {
                if (disposeLogger && this.Logger != null)
                {
                    this.Logger.Dispose();
                    this.Logger = null;
                }
                if (MgaGateway != null &&
                    MgaGateway.territory != null)
                {
                    MgaGateway.territory.Destroy();
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

        /// <summary>
        /// This function does the job. CyPhy2Modelica translation.
        /// </summary>
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

            // 2) Get the type of test-bench and call any dependent interpreters
            var testBenchRef = CyPhyClasses.ParametricExploration.Cast(this.mainParameters.CurrentFCO)
                .Children.TestBenchRefCollection.FirstOrDefault();
            bool interpreterSuccess = false;
            if (testBenchRef != null &&
                testBenchRef.AllReferred != null
                && testBenchRef.AllReferred is CyPhy.TestBench) 
            {
                var testBench = testBenchRef.AllReferred as CyPhy.TestBench;

                var interpreterProgID = Rules.Global.GetInterpreterProgIDFromTestBench(testBench);
                switch (interpreterProgID)
                {
                    case "MGA.Interpreter.CyPhy2Modelica_v2":
                        interpreterSuccess = this.CallCyPhy2Modelica_v2(testBench);
                        this.UpdateSuccess("External Interpreter Called : ", interpreterSuccess);
                        break;

                    case "MGA.Interpreter.CyPhy2CAD_CSharp":
                        interpreterSuccess = this.CallCyPhy2CAD_CSharp(testBench);
                        this.UpdateSuccess("Assuming call to CAD successful : ", true);
                        break;

                    case "MGA.Interpreter.CyPhyFormulaEvaluator":
                        interpreterSuccess = true;
                        break;
                }
            }
            else if (testBenchRef != null && testBenchRef.AllReferred != null && testBenchRef.AllReferred is CyPhy.TestBenchType)
            {
                interpreterSuccess = this.CallCyPhy2CAD_CSharp(testBenchRef.AllReferred as CyPhy.TestBenchType);
                this.UpdateSuccess("Assuming call to CAD successful : ", true);
            }
            else
            {
                throw new NotSupportedException(string.Format(@"Error: {0} must contain one non-null reference to a TestBench",
                    this.mainParameters.CurrentFCO.ToMgaHyperLink()));
            }

            // 3) Generate scripts
            //PET.GMEConsole = GMEConsole;
            var petGenerator = new PET(this.mainParameters, this.Logger);
            var successGenerateScripts = petGenerator.GenerateCode();
            this.result.RunCommand = petGenerator.RunCommand;
            this.result.Labels += petGenerator.Label;
            this.UpdateSuccess("PET Scripts generation", successGenerateScripts);
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

        private bool CallCyPhy2Modelica_v2(CyPhy.TestBench testBench)
        {
            var cyPhy2Modelica_v2 = new META.ComComponent("MGA.Interpreter.CyPhy2Modelica_v2");

            cyPhy2Modelica_v2.InterpreterConfig = META.ComComponent.DeserializeConfiguration(this.mainParameters.ProjectDirectory, typeof(CyPhy2Modelica_v2.CyPhy2Modelica_v2Settings), cyPhy2Modelica_v2.ProgId);

            cyPhy2Modelica_v2.Initialize(this.mainParameters.Project);

            var diTestBenchOutputDir = Directory.CreateDirectory(Path.Combine(OutputDirectory, "TestBench"));

            cyPhy2Modelica_v2.MainParameters.OutputDirectory = diTestBenchOutputDir.FullName;
            cyPhy2Modelica_v2.MainParameters.CurrentFCO = (MgaFCO)testBench.Impl;

            cyPhy2Modelica_v2.MainParameters.Project = this.mainParameters.Project;
            cyPhy2Modelica_v2.MainParameters.ProjectDirectory = this.mainParameters.ProjectDirectory;
            cyPhy2Modelica_v2.MainParameters.SelectedFCOs = this.mainParameters.SelectedFCOs;
            cyPhy2Modelica_v2.MainParameters.StartModeParam = this.mainParameters.StartModeParam;
            cyPhy2Modelica_v2.MainParameters.VerboseConsole = this.mainParameters.VerboseConsole;

            try
            {
                cyPhy2Modelica_v2.Main();
            }
            catch (META.InterpreterException)
            {
                //Trace.TraceError(ex.ToString());
                this.Logger.WriteError("CyPhy2Modelica failed");
                return false;
            }

            if (cyPhy2Modelica_v2.result.Success)
            {
                this.result.Labels += cyPhy2Modelica_v2.result.Labels;
                return true;
            }
            else
            {
                return false;
            }
        }

        private bool CallCyPhy2CAD_CSharp(CyPhy.TestBenchType testBench)
        {
            var cyPhy2CAD = new META.ComComponent("MGA.Interpreter.CyPhy2CAD_CSharp");

            cyPhy2CAD.InterpreterConfig = META.ComComponent.DeserializeConfiguration(
                this.mainParameters.ProjectDirectory,
                typeof(CyPhy2CAD_CSharp.CyPhy2CADSettings),
                cyPhy2CAD.ProgId);

            cyPhy2CAD.Initialize(this.mainParameters.Project);

            var diTestBenchOutputDir = Directory.CreateDirectory(Path.Combine(OutputDirectory, "TestBench"));

            cyPhy2CAD.MainParameters.OutputDirectory = diTestBenchOutputDir.FullName;
            cyPhy2CAD.MainParameters.CurrentFCO = (MgaFCO)testBench.Impl;

            cyPhy2CAD.MainParameters.Project = this.mainParameters.Project;
            cyPhy2CAD.MainParameters.ProjectDirectory = this.mainParameters.ProjectDirectory;
            cyPhy2CAD.MainParameters.SelectedFCOs = this.mainParameters.SelectedFCOs;
            cyPhy2CAD.MainParameters.StartModeParam = this.mainParameters.StartModeParam;
            cyPhy2CAD.MainParameters.VerboseConsole = this.mainParameters.VerboseConsole;

            try
            {
                cyPhy2CAD.Main();
            }
            catch (META.InterpreterException)
            {
                //Trace.TraceError(ex.ToString());
                this.Logger.WriteError("CyPhy2CAD_CSharp failed!");
                return false;
            }

            if (cyPhy2CAD.result.Success)
            {
                this.result.Labels += cyPhy2CAD.result.Labels;
                return true;
            }
            else
            {
                return false;
            }
        }

        [DllImport("user32.dll")]
        public static extern short GetKeyState(int Key);
        #endregion
    }
}