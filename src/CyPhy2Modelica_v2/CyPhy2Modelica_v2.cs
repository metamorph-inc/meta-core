namespace CyPhy2Modelica_v2
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.IO;
    using System.Linq;
    using System.Runtime.InteropServices;
    using System.Text;
    using System.Windows.Forms;
    using CyPhyGUIs;
    using GME.CSharp;
    using GME.MGA;
    using GME.MGA.Core;
    using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
    using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyPhy2Modelica_v2Interpreter : IMgaComponentEx, IGMEVersionInfo, ICyPhyInterpreter
    {
        /// <summary>
        /// Contains information about the GUI event that initiated the invocation.
        /// </summary>
        [ComVisible(false)]
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

        public void InvokeEx(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, int param)
        {
            if (this.enabled == false)
            {
                return;
            }

            try
            {
                this.Logger = new CyPhyGUIs.GMELogger(project, this.ComponentName);
                // Need to call this interpreter in the same way as the MasterInterpreter will call it.
                // initialize main parameters

                if (currentobj == null)
                {
                    this.Logger.WriteError("CyPhy2Modelica_v2 must be invoked on a Test Bench.");
                    return;
                }

                var parameters = new InterpreterMainParameters()
                {
                    Project = project,
                    CurrentFCO = currentobj,
                    SelectedFCOs = selectedobjs,
                    StartModeParam = param,
                    VerboseConsole = true
                };

                this.mainParameters = parameters;
                parameters.ProjectDirectory = Path.GetDirectoryName(currentobj.Project.ProjectConnStr.Substring("MGA=".Length));

                // set up the output directory
                MgaGateway.PerformInTransaction(delegate
                {
                    string outputDirName = project.Name;
                    if (currentobj != null)
                    {
                        outputDirName = currentobj.Name;
                    }

                    parameters.OutputDirectory = Path.GetFullPath(Path.Combine(
                        parameters.ProjectDirectory,
                        "results",
                        outputDirName));

                    //this.Parameters.PackageName = Modelica.Factory.GetModifiedName(currentobj.Name);
                });

                PreConfigArgs preConfigArgs = new PreConfigArgs();
                preConfigArgs.ProjectDirectory = parameters.ProjectDirectory;

                // call the preconfiguration with no parameters and get preconfig
                var preConfig = this.PreConfig(preConfigArgs);

                // get previous GUI config
                var previousConfig = META.ComComponent.DeserializeConfiguration(
                    parameters.ProjectDirectory,
                    typeof(CyPhy2Modelica_v2Settings),
                    this.ComponentProgID);

                // get interpreter config through GUI
                var config = this.DoGUIConfiguration(preConfig, previousConfig);

                if (config == null)
                {
                    this.Logger.WriteWarning("Operation cancelled by the user.");
                    return;
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
                GC.Collect();
                GC.WaitForPendingFinalizers();
                GC.Collect();
                GC.WaitForPendingFinalizers();
            }
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

        #region Dependent Interpreters

        private bool CallElaborator(
            MgaProject project,
            MgaFCO currentobj,
            MgaFCOs selectedobjs,
            int param,
            bool expand = true)
        {
            bool result = false;
            try
            {
                this.Logger.WriteDebug("Elaborating model...");
                var elaborator = new CyPhyElaborateCS.CyPhyElaborateCSInterpreter();
                elaborator.Initialize(project);
                int verbosity = 128;
                if (this.mainParameters.VerboseConsole)
                {
                    verbosity = 3;
                }
                result = elaborator.RunInTransaction(project, currentobj, selectedobjs, verbosity);

                if (this.result.Traceability == null)
                {
                    this.result.Traceability = new META.MgaTraceability();
                }

                if (elaborator.Traceability != null)
                {
                    elaborator.Traceability.CopyTo(this.result.Traceability);
                }
                this.Logger.WriteDebug("Elaboration is done.");
            }
            catch (Exception ex)
            {
                this.Logger.WriteError("Exception occurred in Elaborator : {0}", ex.ToString());
                result = false;
            }

            return result;
        }

        // From StackOverflow: http://stackoverflow.com/questions/1406808/wait-for-file-to-be-freed-by-process 
        private static bool IsFileReady(String sFilename)
        {
            // If the file can be opened for exclusive access it means that the file
            // is no longer locked by another process.
            try
            {
                using (FileStream inputStream = File.Open(sFilename, FileMode.Open, FileAccess.Read, FileShare.None))
                {
                    if (inputStream.Length > 0)
                    {
                        inputStream.Close();
                        return true;
                    }
                    else
                    {
                        inputStream.Close();
                        return false;
                    }

                }
            }
            catch (Exception)
            {
                return false;
            }
        }

        private bool CallCyberInterpreter(CyPhy.CyberModel cyberModel)
        {
            bool success = true;

            string cyberModelPath = string.Empty; 

            // get the path to the cyber model file
            // first, the right way (using resources)
            CyPhy.UsesResource uses_conn = null;
            CyPhy.Resource res = null;

            CyPhy.Component parent_comp = cyberModel.ParentContainer as CyPhy.Component;

            if (cyberModel.SrcConnections.UsesResourceCollection != null
                && cyberModel.SrcConnections.UsesResourceCollection.Count() != 0)
            {
                uses_conn = cyberModel.SrcConnections.UsesResourceCollection.First();
                res = uses_conn.SrcEnd as CyPhy.Resource;
            }
            else if (cyberModel.DstConnections.UsesResourceCollection != null
                      && cyberModel.DstConnections.UsesResourceCollection.Count() != 0)
            {
                uses_conn = cyberModel.DstConnections.UsesResourceCollection.First();
                res = uses_conn.DstEnd as CyPhy.Resource;
            }

            if (res != null)
            {
                cyberModelPath = Path.Combine(parent_comp.Attributes.Path, res.Attributes.Path);
            }
            else
            {
                uses_conn = null;
            }
            
            if (uses_conn == null)
            {

                // failing that, do it the wrong way by getting the fileref attribute value

                // checks
                if (string.IsNullOrWhiteSpace(cyberModel.Attributes.FileRef))
                {
                    this.Logger.WriteError("[Cyber] Model filename attribute is empty: {0}", cyberModel.ToHyperLink());
                    return false;
                }

                if (Path.IsPathRooted(cyberModel.Attributes.FileRef))
                {
                    cyberModelPath = cyberModel.Attributes.FileRef;
                }
                else
                {
                    cyberModelPath = Path.Combine(this.mainParameters.ProjectDirectory, cyberModel.Attributes.FileRef);
                }
            }

            string cyberModelMgaPath = string.Empty;
            string cyberModelXmePath = string.Empty;
            bool requiresImport = false;

            cyberModelMgaPath = Path.GetFileNameWithoutExtension(cyberModelPath) + ".mga";
            cyberModelXmePath = Path.GetFileNameWithoutExtension(cyberModelPath) + ".xme";

            if (Path.GetExtension(cyberModelPath) == ".mga")
            {
                if (File.Exists(cyberModelMgaPath) == false)
                {
                    requiresImport = true;
                    if (File.Exists(cyberModelXmePath) == false)
                    {
                        this.Logger.WriteError("[Cyber] Model filename does not exist: {0} {1}", cyberModel.ToHyperLink(), cyberModelPath);
                        return false;
                    }
                }
            }
            else if (Path.GetExtension(cyberModelPath) == ".xme")
            {
                requiresImport = true;
                if (File.Exists(cyberModelXmePath) == false)
                {
                    this.Logger.WriteError("[Cyber] Model filename does not exist: {0} {1}", cyberModel.ToHyperLink(), cyberModelPath);
                    return false;
                }
            }
            else
            {
                this.Logger.WriteError("[Cyber] Model filename attribute has unknown extension (valid: [mga|xme]): {0} {1}", cyberModel.ToHyperLink(), Path.GetExtension(cyberModelPath));
                return false;
            }

            MgaProject cyberProject = new MgaProject();

            if (requiresImport)
            {
                // FIXME: this will throw an exception if xme-referenced mga exists and it is being used.
                MgaUtils.ImportXME(cyberModelXmePath, cyberModelMgaPath);
            }

            try
            {
                bool ro_mode;

                // FIXME: any race conditions here???
                // FIXME: for SoT we need to copy the referenced xme/mgas
                cyberProject.Open("MGA=" + cyberModelMgaPath, out ro_mode);
                string cyberComponentPath = "";

                if (cyberModel.Attributes.FilePathWithinResource.Contains('.'))
                {
                    cyberComponentPath = cyberModel.Attributes.FilePathWithinResource.Substring(cyberModel.Attributes.FilePathWithinResource.IndexOf('.')).Replace(".", "/@");
                }

                this.Logger.WriteInfo("[Cyber] {0} --> {1}", cyberModel.Attributes.FilePathWithinResource, cyberComponentPath);

                var terr = cyberProject.BeginTransactionInNewTerr();

                MgaFCO currentObj = cyberProject.ObjectByPath[cyberComponentPath] as MgaFCO;

                cyberProject.AbortTransaction();
                terr.Destroy();

                if (currentObj == null)
                {
                    this.Logger.WriteError("[Cyber] Referenced cyber object was not found in model: {0} {1} {2}", cyberModel.ToHyperLink(), cyberModelPath, cyberModel.Attributes.FilePathWithinResource);
                    return false;
                }

                // Cyber model type and interpreter progid map. Each cyber model type has a different interpreter.
                Dictionary<CyPhyClasses.CyberModel.AttributesClass.ModelType_enum, string> interpreterMap =
                    new Dictionary<CyPhyClasses.CyberModel.AttributesClass.ModelType_enum, string>()
                {
                    //{ CyPhyClasses.CyberModel.AttributesClass.ModelType_enum.ESMoL, ""},
                    //{ CyPhyClasses.CyberModel.AttributesClass.ModelType_enum.SignalFlow, ""},
                    { CyPhyClasses.CyberModel.AttributesClass.ModelType_enum.Simulink, "MGA.Interpreter.Cyber2SLC_CodeGen" }
                };
                // call appropriate Cyber interpreter
                Type tCyber = Type.GetTypeFromProgID(interpreterMap[cyberModel.Attributes.ModelType]);
                if (tCyber == null)
                {
                    this.Logger.WriteError("[Cyber] Cannot instantiate Cyber interpreter: {0} {1}", cyberModel.ToHyperLink(), interpreterMap[cyberModel.Attributes.ModelType]);
                }
                IMgaComponentEx cyberCodeGenerator = Activator.CreateInstance(tCyber) as IMgaComponentEx;

                cyberCodeGenerator.Initialize(cyberProject);
                string cyberOutputDir = Path.Combine(this.mainParameters.OutputDirectory, Modelica.CodeGenerator.MainPackage);
                Directory.CreateDirectory(cyberOutputDir);
                cyberCodeGenerator.ComponentParameter["output_dir"] = cyberOutputDir;
                cyberCodeGenerator.ComponentParameter["automation"] = "true";
                cyberCodeGenerator.ComponentParameter["console_messages"] = "off";
                this.Logger.WriteInfo("Generating code for Cyber [{0}] elements... to directory {1}", cyberModel.Attributes.ModelType, cyberOutputDir);
                System.Windows.Forms.Application.DoEvents();
                MgaFCOs selectedobjs = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
                cyberCodeGenerator.InvokeEx(cyberProject, currentObj, selectedobjs, 128);
                this.Logger.WriteInfo("Cyber [{0}] code generation is done.", cyberModel.Attributes.ModelType);
                System.Windows.Forms.Application.DoEvents();

                // Copy files referred by resource objects
                string cyber_manifest_filename = parent_comp.Name + ".cyber.json";
                Dictionary<string, string> comp_resources = new Dictionary<string, string>();
                foreach (CyPhy.Resource resource in parent_comp.Children.ResourceCollection)
                {
                    FileInfo resource_path = new FileInfo(Path.Combine(parent_comp.Attributes.Path, resource.Attributes.Path));
                    string final_path;

                    if (resource_path.DirectoryName == null || resource_path.DirectoryName == string.Empty)
                    {
                        final_path = "Cyber";
                    }
                    else
                    {
                        final_path = resource_path.DirectoryName.Split(Path.DirectorySeparatorChar).Last();
                    }
                    //DirectoryInfo resource_directory = resource_path.Directory;

                    //string res_path = Path.Combine(cyberModel.Path, resource.Attributes.Path);
                    //string[] path_parts = res_path.Split('\\');
                    //string[] path_only = path_parts.Take(path_parts.Length - 1).ToArray();
                    
                    string new_path = Path.Combine( cyberOutputDir, final_path );
                    Logger.WriteInfo("[Cyber] resource_path.DirectoryName == string.Empty, new_path is {0}", new_path);
                    
                    System.IO.Directory.CreateDirectory(new_path);
                    string dest_filename = Path.Combine(new_path, resource_path.Name);
                    Logger.WriteInfo("[Cyber] dest_filename is {0}", dest_filename);
                    // TODO: Little bit hacky hard-coding the wait time
                    //int counter = 0;
                    //while ((IsFileReady(resource_path.FullName) != true)) && (counter < 10))
                    //{
                    //    System.Threading.Thread.Sleep(2000);
                    //    counter++;
                    //}

                    if (IsFileReady(resource_path.FullName) == true)
                    {
                        Logger.WriteInfo("[Cyber] copying file {0} to file {1}", resource_path.FullName, dest_filename);
                        System.IO.File.Copy(resource_path.FullName, dest_filename, true);
                        comp_resources[resource.Attributes.ID] = Path.Combine(final_path, resource_path.Name );
                    }
                    else
                    {
                        Logger.WriteError("[Cyber] Cannot access file for copying: {0}", dest_filename);
                    }
                    
                }

                string json_resources = Newtonsoft.Json.JsonConvert.SerializeObject(comp_resources);
                System.IO.File.WriteAllText(Path.Combine(cyberOutputDir, cyber_manifest_filename), json_resources);
            }
            catch (Exception ex)
            {
                this.Logger.WriteError("Cyber exception occured: {0}", ex);

                success = false;
            }

            return success;
        }

        private bool CallCyber(MgaProject mgaProject, MgaFCO mgaFCO, MgaFCOs mgaFCOs, int param)
        {
            var filter = mgaProject.CreateFilter();
            filter.Kind = typeof(CyPhy.CyberModel).Name;

            bool success = true;

            var cyberModelFcos = (mgaFCO as MgaModel).GetDescendantFCOs(filter);

            foreach (var cyberModelFco in cyberModelFcos)
            {
                success = success && this.CallCyberInterpreter(CyPhyClasses.CyberModel.Cast(cyberModelFco as MgaObject));
            }

            return success;
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
                this.Logger.WriteError("{0} : FAILED", message);
            }
        }

        /// <summary>
        /// Name of the log file. (It is not a full path)
        /// </summary>
        private string LogFileFilename { get; set; }

        /// <summary>
        /// Full path to the log file.
        /// </summary>
        private string LogFilePath
        {
            get
            {
                return Path.Combine(this.result.LogFileDirectory, this.LogFileFilename);
            }
        }

        public CyPhyGUIs.GMELogger Logger { get; set; }

        /// <summary>
        /// ProgId of the configuration class of this interpreter.
        /// </summary>
        public string InterpreterConfigurationProgId
        {
            get
            {
                return (typeof(CyPhy2Modelica_v2Settings).GetCustomAttributes(typeof(ProgIdAttribute), false)[0] as ProgIdAttribute).Value;
            }
        }

        /// <summary>
        /// Preconfig gets called first. No transaction is open, but one may be opened.
        /// In this function model may be processed and some object ids get serialized
        /// and returned as preconfiguration (project-wise configuration).
        /// </summary>
        /// <param name="preConfigParameters"></param>
        /// <returns>Null if no configuration is required by the DoGUIConfig.</returns>
        public IInterpreterPreConfiguration PreConfig(IPreConfigParameters preConfigParameters)
        {
            var preConfig = new CyPhy2Modelica_v2PreConfiguration()
            {
                ProjectDirectory = preConfigParameters.ProjectDirectory
            };

            return preConfig;
        }

        /// <summary>
        /// Shows a form for the user to select/change settings through a GUI. All interactive 
        /// GUI operations MUST happen within this function scope.
        /// </summary>
        /// <param name="preConfig">Result of PreConfig</param>
        /// <param name="previousConfig">Previous configuration to initialize the GUI.</param>
        /// <returns>Null if operation is cancelled by the user. Otherwise returns with a new
        /// configuration object.</returns>
        public IInterpreterConfiguration DoGUIConfiguration(
            IInterpreterPreConfiguration preConfig,
            IInterpreterConfiguration previousConfig)
        {
            DialogResult ok = DialogResult.Cancel;

            var settings = previousConfig as CyPhy2Modelica_v2Settings;

            if (settings == null)
            {
                settings = new CyPhy2Modelica_v2Settings();
            }

            using (MainForm mf = new MainForm(settings, (preConfig as CyPhy2Modelica_v2PreConfiguration).ProjectDirectory))
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

                this.Logger.WriteInfo("Running CyPhy2Modelica 2.0");
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
            this.mainParameters  = (InterpreterMainParameters) parameters;
            this.Factory = new CyPhyTypeFactory(parameters.Project.RootMeta);
            var configSuccess = this.mainParameters != null;
            if (configSuccess && this.mainParameters.config == null)
            {
                var config = META.ComComponent.DeserializeConfiguration(this.mainParameters.ProjectDirectory, typeof(CyPhy2Modelica_v2Settings), this.ComponentProgID) as
                    CyPhy2Modelica_v2Settings;
                if (config != null)
                {
                    this.mainParameters.config = config;
                }
                else
                {
                    this.mainParameters.config = new CyPhy2Modelica_v2Settings();
                }

                configSuccess = this.mainParameters.config != null;
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

            // If called from InvokeEx logger is already created
            bool disposeLogger = false;
            if (this.Logger == null)
            {
                this.Logger = new CyPhyGUIs.GMELogger(this.mainParameters.Project, this.ComponentName);
                disposeLogger = true;
            }

           // this.Logger.MakeVersionInfoHeaderAsync();

            if (this.mainParameters.VerboseConsole)
            {
                this.Logger.GMEConsoleLoggingLevel = SmartLogger.MessageType_enum.Debug;
                this.Logger.WriteDebug("Components not used in this design will still be generated in the Modelica Package.");
                this.GenerateUnusedComponents = true;
            }
            else
            {
                this.Logger.GMEConsoleLoggingLevel = SmartLogger.MessageType_enum.Info;
                this.Logger.WriteDebug("Components not used in this design will not be generated in the Modelica Package. Use verbose console to enable this option.");
                this.GenerateUnusedComponents = false;
            }

            this.result.Traceability.CopyTo(this.Logger.Traceability);


            this.UpdateSuccess("Configuration", configSuccess);

            this.result.Labels = "OpenModelica && py_modelica12.08";
            this.result.RunCommand = "om_simulate.py";
            //var elaboratorSuccess = this.CallElaboratorOLD(this.mainParameters.Project,
            //    this.mainParameters.CurrentFCO, this.mainParameters.SelectedFCOs, this.mainParameters.StartModeParam);
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
                    this.Logger.WriteInfo("CyPhy2Modelica 2.0 finished successfully.");
                    this.Logger.WriteInfo("Generated files are here: <a href=\"file:///{0}\" target=\"_blank\">{0}</a>", this.mainParameters.OutputDirectory);
                    this.Logger.WriteDebug("[SUCCESS: {0}, Labels: {1}]", this.result.Success, this.result.Labels);
                }
                else
                {
                    this.Logger.WriteError("CyPhy2Modelica 2.0 failed! See error messages above.");
                }
            }
            finally
            {
                if (disposeLogger && this.Logger != null)
                {
                    this.Logger.Dispose();
                    this.Logger = null;
                }
                if (MgaGateway != null && MgaGateway.territory != null)
                {
                    MgaGateway.territory.Destroy();
                }
                MgaGateway = null;
                GC.Collect();
                GC.WaitForPendingFinalizers();
                GC.Collect();
                GC.WaitForPendingFinalizers();
            }

            return this.result;
        }

        private bool CheckGeneratedModelWithDymola(Modelica.ComponentMap modelicaURIMaps)
        {
            var modelicaSettings = this.mainParameters.config as CyPhy2Modelica_v2Settings;
            int secondsToWait = 120;
            var dymolaExe = MainForm.DymolaExe;
            var result = false;
            if (File.Exists(dymolaExe) == false)
            {
                //GMEConsole.Warning.WriteLine("Dymola install was not found.");
                this.Logger.WriteWarning("Dymola install was not found.");
                this.Logger.WriteWarning("The generate code cannot be checked with Dymola.");
                return true;
            }
            else
            {
                this.Logger.WriteDebug("Dymola install was found: " + dymolaExe);
            }

            try
            {
                var tb = CyPhyClasses.TestBench.Cast(this.mainParameters.CurrentFCO);

                // generate script file for model check
                StringBuilder sb = new StringBuilder();
                sb.AppendLine("Advanced.TranslationInCommandLog = true");
                sb.AppendLine("openModel(\"CyPhy\\package.mo\")");
                sb.AppendLine("checkModel(\"" + Modelica.CodeGenerator.MainPackage + ".TestBenches." + tb.Name + "\")");
                sb.AppendLine("savelog(\"checkModelDymola.txt\")");
                sb.AppendLine("exit()");
                string checkModelMos = Path.Combine(this.mainParameters.OutputDirectory, Modelica.CodeGenerator.MainPackage, "checkModelDymola.mos");

                File.WriteAllText(checkModelMos, sb.ToString());

                Process p = new Process();
                p.StartInfo = new ProcessStartInfo()
                {
                    Arguments = " /nowindow " + checkModelMos,
                    FileName = dymolaExe,
                    CreateNoWindow = true,
                    UseShellExecute = false,
                    WorkingDirectory = this.mainParameters.OutputDirectory
                };

                if (p.StartInfo.EnvironmentVariables["MODELICAPATH"] != null)
                {
                    p.StartInfo.EnvironmentVariables.Remove("MODELICAPATH");
                }

                var modelicaPath = Path.GetFullPath(Path.Combine(this.mainParameters.OutputDirectory, "Libraries"));

                p.StartInfo.EnvironmentVariables.Add("MODELICAPATH", modelicaPath);

                this.Logger.WriteInfo("Checking model with Dymola...");
                this.Logger.WriteDebug("DymolaExe : {0}", dymolaExe);
                this.Logger.WriteDebug("MODELICAPATH : {0}", modelicaPath);
                System.Windows.Forms.Application.DoEvents();

                p.Start();
                if (p.WaitForExit(1000 * secondsToWait) == false)
                {
                    p.Kill();
                    this.Logger.WriteError("Dymola checking process did not exit within {0} seconds - the process was killed.", secondsToWait);
                    this.Logger.WriteError("Do you have a Dymola license? If so please leave a Dymola instance open (it will keep the license checked out).");
                    return result;
                }
                // read generated results back

                string s = File.ReadAllText(Path.Combine(this.mainParameters.OutputDirectory, Modelica.CodeGenerator.MainPackage, "checkModelDymola.txt"));

                // display it on the GME console
                if (s.Contains("Check of " + Modelica.CodeGenerator.MainPackage + ".TestBenches." + tb.Name + " successful."))
                {
                    // TODO: maybe check for next line  = true?
                    this.Logger.WriteInfo("Successful Dymola check.");
                    result = true;
                }
                else
                {
                    result = false;
                    this.Logger.WriteError("Failed Dymola check.");
                    var startRecording = false;
                    foreach (var line in s.Split(Environment.NewLine.ToArray()).Where(l => string.IsNullOrWhiteSpace(l) == false))
                    {
                        if (startRecording == false)
                        {
                            if (line.Contains("Advanced.TranslationInCommandLog = true"))
                            {
                                startRecording = true;
                            }
                        }
                        else
                        {
                            if (line.Contains("savelog(\"checkModelDymola.txt\")"))
                            {
                                break;
                            }
                            this.Logger.WriteError(TryConvertErrorStringToHyperLink(line, modelicaURIMaps));
                        }
                    }
                }

            }
            catch (Exception ex)
            {
                //Trace.TraceError(ex.ToString());
                this.Logger.WriteError(ex.ToString());
                result = false;
            }

            return result;
        }

        private bool CheckGeneratedModelWithOpenModelica(Modelica.ComponentMap modelicaURIMaps, ModelConfig modelConfig)
        {
            var modelicaSettings = this.mainParameters.config as CyPhy2Modelica_v2Settings;
            var omcExe = MainForm.OMCExe;
            var result = false;
            if (File.Exists(omcExe) == false)
            {
                this.Logger.WriteWarning("OpenModelica installation was not found.");
                this.Logger.WriteWarning("The generate code cannot be checked with OpenModelica.");
                return true;
            }
            else
            {
                this.Logger.WriteDebug("OpenModelica compiler was found: " + omcExe);
            }

            try
            {
                var tb = CyPhyClasses.TestBench.Cast(this.mainParameters.CurrentFCO);
                var externalLibraries = new List<String>();
                externalLibraries = modelConfig.lib_package_names;
                var mslVersion = modelConfig.MSL_version;
                // generate script file for model check
                StringBuilder sb = new StringBuilder();
                sb.AppendLine("// Checking model in OpenModelica");
                sb.AppendLine(string.Format("loadModel(Modelica, {{\"{0}\"}});", mslVersion));
                foreach (var item in externalLibraries)
                {
                    sb.AppendLine(string.Format("loadModel({0});", item));
                }
                sb.AppendLine("loadFile(\"CyPhy/package.mo\");");
                sb.AppendLine(string.Format("checkStr := checkModel({0}.TestBenches.{1});", Modelica.CodeGenerator.MainPackage, tb.Name));
                sb.AppendLine("writeFile(\"CyPhy/checkModelOpenModelica.txt\", checkStr);");
                string checkModelMos = Path.Combine(this.mainParameters.OutputDirectory, Modelica.CodeGenerator.MainPackage, "checkModelOpenModelica.mos");

                File.WriteAllText(checkModelMos, sb.ToString());

                // call mos file to check model
                Process p = new Process();
                p.StartInfo = new ProcessStartInfo()
                {
                    Arguments = string.Format("+q +s \"{0}\"", checkModelMos),
                    FileName = omcExe,
                    CreateNoWindow = true,
                    UseShellExecute = false,
                    WorkingDirectory = this.mainParameters.OutputDirectory
                };

                if (p.StartInfo.EnvironmentVariables["OPENMODELICALIBRARY"] != null)
                {
                    p.StartInfo.EnvironmentVariables.Remove("OPENMODELICALIBRARY");
                }

                var modelicaPath = Path.Combine(Environment.GetEnvironmentVariable("OPENMODELICAHOME"), "lib", "omlibrary") + ";" 
                    + Path.GetFullPath(Path.Combine(this.mainParameters.OutputDirectory, "Libraries"));

                p.StartInfo.EnvironmentVariables.Add("OPENMODELICALIBRARY", modelicaPath);

                this.Logger.WriteInfo("Checking model with OpenModelica...");
                this.Logger.WriteDebug("OmcExe : {0}", omcExe);
                this.Logger.WriteDebug("OPENMODELICALIBRARY : {0}", modelicaPath);
                System.Windows.Forms.Application.DoEvents();

                p.Start();
                p.WaitForExit();
                // read generated results back

                string s = File.ReadAllText(Path.Combine(this.mainParameters.OutputDirectory, Modelica.CodeGenerator.MainPackage, "checkModelOpenModelica.txt"));

                // display it on the GME console
                if (s.Contains("Check of " + Modelica.CodeGenerator.MainPackage + ".TestBenches." + tb.Name + " completed successfully."))
                {
                    // TODO: maybe check for next line  = true?
                    this.Logger.WriteInfo("Successful OpenModelica check.");
                    result = true;
                }
                else
                {
                    result = false;
                    this.Logger.WriteError("Failed OpenModelica check.");
                    s.Split(Environment.NewLine.ToArray())
                        .Where(l => string.IsNullOrWhiteSpace(l) == false).ToList()
                        .ForEach(x => this.Logger.WriteError(TryConvertErrorStringToHyperLink(x, modelicaURIMaps)));
                }

            }
            catch (Exception ex)
            {
                //Trace.TraceError(ex.ToString());
                this.Logger.WriteError(ex.ToString());
                result = false;
            }

            return result;
        }

        private string TryConvertErrorStringToHyperLink(string errorMsg, Modelica.ComponentMap modelicaURIMaps)
        {
            // Assumption, only checks each line for one mapping!

            var result = errorMsg.Replace("__CyPhy__", "");
            var match = "";
            foreach (var kvp in modelicaURIMaps.ModelMapping)
            {
                if (errorMsg.Contains(kvp.Key) && kvp.Key.Length > match.Length)
                {
                    // Make sure the longest ModelicaURI is used.
                    match = kvp.Key;
                }
            }

            if (string.IsNullOrWhiteSpace(match) == false)
            {
                var cInfo = modelicaURIMaps.ModelMapping[match];
                var mgaComponent = this.mainParameters.Project.GetFCOByID(cInfo.ID);
                if (mgaComponent != null)
                {
                    return errorMsg.Replace(match, GmeConsoleHelper.ToMgaHyperLink(mgaComponent, this.result.Traceability));
                }
                else
                {
                    this.Logger.WriteWarning("Could not obtain {0} from IDMap using {1}", match, cInfo.Path);
                }
            }
            else
            {
                foreach (var kvp in modelicaURIMaps.InstanceMapping)
                {
                    if (errorMsg.Contains(kvp.Key) && kvp.Key.Length > match.Length)
                    {
                        // Make sure the longest ModelicaURI is used.
                        match = kvp.Key;
                    }
                }

                if (string.IsNullOrWhiteSpace(match) == false)
                {
                    var cInfo = modelicaURIMaps.InstanceMapping[match];
                    var mgaComponent = this.mainParameters.Project.GetFCOByID(cInfo.ID);
                    if (mgaComponent != null)
                    {
                        return errorMsg.Replace(match, GmeConsoleHelper.ToMgaHyperLink(mgaComponent, this.result.Traceability));
                    }
                    else
                    {
                        this.Logger.WriteWarning("Could not obtain {0} from IDMap using {1}", match, cInfo.Path);
                    }
                }
            }

            return result;
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

        #region CyPhy2Modelica Specific code

        public CyPhyTypeFactory Factory { get; set; }

        private bool GenerateUnusedComponents { get; set; }

        /// <summary>
        /// This function does the job. CyPhy2Modelica translation.
        /// </summary>
        private void WorkInMainTransaction()
        {
            this.Logger.WriteInfo("{0} [{1}]", this.mainParameters.CurrentFCO.Name, this.mainParameters.CurrentFCO.MetaBase.Name);
            this.result.Success = true;

            // Try to get the original design name for elaboration (used when generating testbench_manifest)
            string originalDesignName = GetOriginalDesignName();

            // 1) try to call dependencies - elaborate
            var elaboratorSuccess = this.CallElaborator(this.mainParameters.Project,
                this.mainParameters.CurrentFCO, this.mainParameters.SelectedFCOs, this.mainParameters.StartModeParam);
            //var elaboratorSuccess = true;
            this.UpdateSuccess("Elaborator", elaboratorSuccess);

            if (elaboratorSuccess == false)
            {
                return;
            }

            // 2) Check rules on Elborated model
            this.Logger.WriteDebug("Checking rules...");
            var checker = new Rules.Checker(this.mainParameters, this.Logger, this.result.Traceability, this.Factory);
            checker.Check();
            checker.PrintDetails();
            this.UpdateSuccess("Model check", checker.Success);

            // if user held the control ignore the checker results ... for debugging purposes ONLY!
            bool controlWasHeld = false;
            int VK_CONTROL = 0x11;
            if ((bool)((GetKeyState(VK_CONTROL) & 0x8000) == 0x8000))
            {
                controlWasHeld = true;
            }

            if (this.result.Success == false)
            {
                if (controlWasHeld)
                {
                    this.Logger.WriteWarning("Control was held, will continue generating code.");
                }
                else
                {
                    return;
                }
            }

            // 3) call cyber
            var cyberSuccess = this.CallCyber(this.mainParameters.Project, this.mainParameters.CurrentFCO, this.mainParameters.SelectedFCOs, this.mainParameters.StartModeParam);

            if (cyberSuccess == false)
            {
                this.Logger.WriteWarning("Cyber code generator failed! Modelica interpreter ignores this failure.");
            }

            // FIXME: add cyber back when it is stable
            //this.UpdateSuccess("Cyber code generator", cyberSuccess);

            // 4) Generate the Modelica package

            var modelicaCodeGenerator = new Modelica.CodeGenerator(this.mainParameters, this.Logger, this.result.Traceability);
            modelicaCodeGenerator.GenerateUnusedComponents = this.GenerateUnusedComponents;
            modelicaCodeGenerator.GenerateModelicaCode();

            this.result.Labels = modelicaCodeGenerator.SolverSettings.ToolSelection + " && py_modelica" + JobManager.Job.LabelVersion;
            var limitDefintion = modelicaCodeGenerator.LimitDefintion;
            var modelicaURIMaps = modelicaCodeGenerator.ModelicaURIMap;

            this.UpdateSuccess("Modelica translation", true);

            // 5) post actions - pre/post processing scripts and test bench manifest
            //Scripts.Generator.GMEConsole = GMEConsole;
            var scripts = new Scripts.Generator(this.mainParameters, this.Logger);
            scripts.LimitDefinition = limitDefintion;
            var successScripts = scripts.Generate(originalDesignName);
            this.result.RunCommand = scripts.RunCommand;
            var modelConfig = scripts.CurrentModelConfig;
            this.UpdateSuccess("Script generation", successScripts);

            var modelicaSettings = this.mainParameters.config as CyPhy2Modelica_v2Settings;

            // 6) Call checkModel from selected tool(s).
            if (this.result.Success == false)
            {
                return;
            }

            if (modelicaSettings.CheckWithDymola)
            {
                this.UpdateSuccess("Dymola Check", this.CheckGeneratedModelWithDymola(modelicaURIMaps));
            }

            if (modelicaSettings.CheckWithOpenModelica)
            {
                this.UpdateSuccess("OpenModelica Check", this.CheckGeneratedModelWithOpenModelica(modelicaURIMaps, modelConfig));
            }

            // TODO: this part needs to be refactored!
            string Parameters = this.mainParameters
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

            META.AnalysisTool.ApplyToolSelection(this.ComponentProgID, workflowParameters, this.result, this.mainParameters);
        }

        /// <summary>
        /// If the context is correct and TopLevelSystemUnderTest points to a component-assembly it returns the name 
        /// of that (first if many) component assembly.
        /// Else it returns null.
        /// </summary>
        /// <returns>String of original design name.</returns>
        private string GetOriginalDesignName()
        {
            string originalDesignName = null;
            if (this.mainParameters.CurrentFCO.MetaBase.Name == typeof(CyPhy.TestBench).Name)
            {
                var testBench = CyPhyClasses.TestBench.Cast(this.mainParameters.CurrentFCO);
                var tlsut = testBench.Children.TopLevelSystemUnderTestCollection.FirstOrDefault();
                if (tlsut != null && tlsut.AllReferred != null && tlsut.AllReferred.Kind == typeof(CyPhy.ComponentAssembly).Name)
                {
                    originalDesignName = tlsut.AllReferred.Name;
                    this.Logger.WriteDebug("Found originalDesignName {0}", originalDesignName);
                }
            }

            return originalDesignName;
        }

        [DllImport("user32.dll")]
        public static extern short GetKeyState(int Key);
        #endregion
    }
}
