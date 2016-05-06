using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using GME.CSharp;
using GME;
using GME.MGA;
using GME.MGA.Core;
using System.Diagnostics;
using CyPhyGUIs;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using System.Windows.Forms;

namespace CyPhyReliabilityAnalysis
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyPhyReliabilityAnalysisInterpreter : IMgaComponentEx, IGMEVersionInfo, ICyPhyInterpreter
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
            MgaGateway = new MgaGateway(project);
        }

        public CyPhyGUIs.GMELogger Logger { get; set; }

        #region IMgaComponentEx Members

        MgaGateway MgaGateway { get; set; }

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
                    this.Logger.WriteFailed("CyPhyReliabilityAnalysis must be called from a Test Bench.");
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
                });

                if (string.IsNullOrEmpty(kindName) == false && kindName != "TestBench")
                {
                    this.Logger.WriteFailed("CyPhyReliabilityAnalysis must be called from a Test Bench.");
                    return;
                }

                PreConfigArgs preConfigArgs = new PreConfigArgs();
                preConfigArgs.ProjectDirectory = parameters.ProjectDirectory;

                // call the preconfiguration with no parameters and get preconfig
                var preConfig = this.PreConfig(preConfigArgs);

                // get previous GUI config
                var previousConfig = META.ComComponent.DeserializeConfiguration(
                    parameters.ProjectDirectory,
                    typeof(CyPhyReliabilityAnalysisSettings),
                    this.ComponentProgID);

                // get interpreter config through GUI
                var config = this.DoGUIConfiguration(preConfig, previousConfig);
                if (config == null)
                {
                    this.Logger.WriteWarning("Operation cancelled by the user.");
                    return;
                }

                //#region CyPhy2Modelica_v2Configuration

                //var cyPhy2Modelica_v2 = new META.ComComponent("MGA.Interpreter.CyPhy2Modelica_v2");
                //if (cyPhy2Modelica_v2.DoGUIConfiguration(parameters.ProjectDirectory) == false)
                //{
                //    this.Logger.WriteWarning("Operation cancelled by the user.");
                //    return;
                //}
                //#endregion

                // TODO: put here any other interpreters that we have to call.

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
        SortedDictionary<string, object> componentParameters = null;

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

        public void OpenWebPageWithChromeOrDefaultBrowser(string webPageUrl)
        {
            // try to get this value better HKLM + 32 vs 64bit...

            // chrome is installed for all users
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ee872121(v=vs.85).aspx#app_exe
            string keyName = @"HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\chrome.exe";

            string chromePath = (string)Microsoft.Win32.Registry.GetValue(
                keyName,
                "Path",
                "ERROR: " + keyName + " InstallLocation does not exist!");

            if (chromePath == null || chromePath.StartsWith("ERROR"))
            {
                // Installed only for this user
                keyName = @"HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\chrome.exe";

                chromePath = (string)Microsoft.Win32.Registry.GetValue(
                    keyName,
                    "Path",
                    "ERROR: " + keyName + " InstallLocation does not exist!");
            }

            if (chromePath == null || chromePath.StartsWith("ERROR"))
            {
                this.Logger.WriteInfo("Opening {0} using default browser...", webPageUrl);
                Process.Start(webPageUrl);
            }
            else
            {
                string chromeExe = Path.Combine(chromePath, "chrome.exe");
                this.Logger.WriteInfo("Opening {0} using Chrome at {1} ...", webPageUrl, chromeExe);
                Process p = new Process();
                p.StartInfo = new ProcessStartInfo()
                {
                    Arguments = webPageUrl,
                    FileName = chromeExe,
                    UseShellExecute = false
                };

                p.Start();
            }
        }

        #region CyPhyGUIs

        /// <summary>
        /// Result of the latest run of this interpreter.
        /// </summary>
        private InterpreterResult result = new InterpreterResult();

        /// <summary>
        /// Parameter of this run.
        /// </summary>
        private InterpreterMainParameters mainParameters { get; set; }

        public string InterpreterConfigurationProgId
        {
            get
            {
                return (typeof(CyPhyReliabilityAnalysisSettings).GetCustomAttributes(typeof(ProgIdAttribute), false)[0] as ProgIdAttribute).Value;
            }
        }

        public IInterpreterPreConfiguration PreConfig(IPreConfigParameters parameters)
        {
            return null;
        }

        public IInterpreterConfiguration DoGUIConfiguration(IInterpreterPreConfiguration preConfig, IInterpreterConfiguration previousConfig)
        {
            DialogResult ok = DialogResult.Cancel;

            var settings = previousConfig as CyPhyReliabilityAnalysisSettings;

            if (settings == null)
            {
                settings = new CyPhyReliabilityAnalysisSettings();
            }

            using (MainForm mf = new MainForm(settings))
            {
                // show main form
                ok = mf.ShowDialog();
            }

            if (ok == DialogResult.OK)
            {
                return settings;
            }

            return null;
        }

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
                this.Logger.WriteInfo("Running CyPhyReliabilityAnalysis");
                System.Windows.Forms.Application.DoEvents();

                var asyncResult = this.Logger.LoggingVersionInfo.BeginInvoke(parameters.Project, null, null);
                var header = this.Logger.LoggingVersionInfo.EndInvoke(asyncResult);
                this.Logger.WriteDebug(header);

                this.MainThrows(parameters);
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

        private void MainThrows(IInterpreterMainParameters parameters)
        {
            // TODO: remove this line, since this is the old implementation
            //this.OpenWebPageWithChromeOrDefaultBrowser("http://fame-deploy.parc.com:2040/");

            this.mainParameters = parameters as InterpreterMainParameters;

            this.result.Labels = "";
            this.result.RunCommand = "";
            var config = META.ComComponent.DeserializeConfiguration(
                this.mainParameters.ProjectDirectory,
                typeof(CyPhyReliabilityAnalysisSettings),
                this.ComponentProgID) as CyPhyReliabilityAnalysisSettings;

            if (config != null)
            {
                this.mainParameters.config = config;
            }
            else
            {
                this.mainParameters.config = new CyPhyReliabilityAnalysisSettings();
            }

            if (Directory.Exists(this.mainParameters.OutputDirectory) == false)
            {
                Directory.CreateDirectory(this.mainParameters.OutputDirectory);
            }

            using (StreamWriter writer1 = new StreamWriter(Path.Combine(this.mainParameters.OutputDirectory, "bracket_config.json")))
            {
                writer1.WriteLine(Newtonsoft.Json.JsonConvert.SerializeObject(
                    new BracketConfig(this.mainParameters.config as CyPhyReliabilityAnalysisSettings),
                    Newtonsoft.Json.Formatting.Indented));
            }

            // TODO: call all dependent interpreter

            MgaGateway.PerformInTransaction(() =>
            {
                this.WorkInMainTransaction();
            }, transactiontype_enum.TRANSACTION_NON_NESTED);

            //this.PrintRuntimeStatistics();
            if (this.result.Success)
            {
                this.Logger.WriteInfo("CyPhyReliabilityAnalysis 1.0 finished successfully.");
                this.Logger.WriteInfo("Generated files are here: <a href=\"file:///{0}\" target=\"_blank\">{0}</a>", this.mainParameters.OutputDirectory);
                this.Logger.WriteDebug("[SUCCESS: {0}, Labels: {1}]", this.result.Success, this.result.Labels);
            }
            else
            {
                this.Logger.WriteError("CyPhyReliabilityAnalysis 1.0 failed! See error messages above.");
            }
        }

        private void WorkInMainTransaction()
        {
            // FIXME: this will raise an exception if workflow is not defined since we have no checks...
            // TODO: this part needs to be refactored!
            string Parameters = this.mainParameters
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

            META.AnalysisTool.ApplyToolSelection(this.ComponentProgID, workflowParameters, this.result, this.mainParameters);

            // TODO: this is bogus use real success values
            this.result.Success = true;
        }

        #endregion
    }
}
