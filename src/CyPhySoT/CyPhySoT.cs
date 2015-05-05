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
using System.Diagnostics;
using CyPhyGUIs;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhySoT
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyPhySoTInterpreter : IMgaComponentEx, IGMEVersionInfo, ICyPhyInterpreter
    {
        [ComVisible(false)]
        public const string RunCommand = "";
        [ComVisible(false)]
        public string OutputBaseDir { get; set; }
        [ComVisible(false)]
        public string ProjectFilename { get; set; }

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
            // set up the default component parameters
            this.componentParameters = new SortedDictionary<string, object>();
            this.componentParameters.Add("labels", "");
            this.componentParameters.Add("runCommand", RunCommand);
            this.componentParameters.Add("output_dir", string.Empty);
            this.componentParameters.Add("automation", "false");
            this.componentParameters.Add("do_config", "true");
            this.componentParameters.Add("configuration", string.Empty);
            this.componentParameters.Add("test_bench_name", string.Empty);

            // Mimic CyPET and CyPhy2Modelica_v2
            //GMEConsole = GMEConsole.CreateFromProject(project);
            MgaGateway = new MgaGateway(project);
            project.CreateTerritoryWithoutSink(out MgaGateway.territory);
        }

        [ComVisible(false)]
        public void SetupOutputDirectory(MgaProject project, MgaFCO currentobj)
        {
            var projectDir = Path.GetDirectoryName(project.ProjectConnStr.Substring("MGA=".Length));
            //this.OutputBaseDir = Path.Combine(projectDir, "results", currentobj.Name);
            this.OutputBaseDir = this.mainParameters.OutputDirectory;

            if (Directory.Exists(this.OutputBaseDir) == false)
            {
                Directory.CreateDirectory(this.OutputBaseDir);
            }
        }

        private bool CheckModel(MgaFCO currentobj)
        {
            bool error = false;

            // checks
            // checks are done
            return error;
        }

        class CycleException : Exception
        {
            public CycleException(string message)
                : base(message)
            {
            }
        }

        [ComVisible(false)]
        public Dictionary<MgaFCO, List<MgaFCO>> DependencyGraph { get; set; }

        delegate void dfs_visit(MgaFCO fco, ref List<MgaFCO> out_);
        /// <summary>
        /// 
        /// </summary>
        /// <returns>True if dependency cannot be determined.</returns>
        private bool UpdateDependency(MgaModel testBenchSuite)
        {
            this.Logger.WriteDebug("Updating test bench dependencies ...");

            HashSet<MgaFCO> toProcess = new HashSet<MgaFCO>(new MgaObjectEqualityComparor<MgaFCO>());
            // Find all FCOs with no outgoing connections
            foreach (MgaFCO fco in testBenchSuite.ChildFCOs.Cast<MgaFCO>().Where(fco => fco.ObjType != GME.MGA.Meta.objtype_enum.OBJTYPE_CONNECTION))
            {
                if (fco.PartOfConns.Cast<MgaConnPoint>().Where(connpoint => connpoint.ConnRole == "src" && connpoint.References.Count == 0).Count() == 0)
                {
                    if (fco.ObjType == GME.MGA.Meta.objtype_enum.OBJTYPE_REFERENCE)
                    {
                        MgaReference reference = (MgaReference)fco;
                        if (reference.UsedByConns.Cast<MgaConnPoint>().Where(connpoint => connpoint.ConnRole == "src").Count() == 0)
                        {
                            toProcess.Add(fco);
                        }
                    }
                    else
                    {
                        toProcess.Add(fco);
                    }
                }
            }

            var DependencyGraph = new Dictionary<MgaFCO, List<MgaFCO>>(new MgaObjectEqualityComparor<MgaFCO>());

            HashSet<MgaFCO> tempMarked = new HashSet<MgaFCO>(new MgaObjectEqualityComparor<MgaFCO>());
            Dictionary<MgaFCO, List<MgaFCO>> resident = new Dictionary<MgaFCO, List<MgaFCO>>(new MgaObjectEqualityComparor<MgaFCO>());

            dfs_visit Visit = null;
            Visit = delegate(MgaFCO fco, ref List<MgaFCO> out_)
            {
                if (tempMarked.Contains(fco))
                {
                    throw new CycleException("Cycle involving " + fco.Name);
                }
                List<MgaFCO> previousOut;
                if (!resident.TryGetValue(fco, out previousOut))
                {
                    //GMEConsole.Out.WriteLine("" + fco.Name);
                    tempMarked.Add(fco);
                    IEnumerable<MgaConnPoint> connections = fco.PartOfConns.Cast<MgaConnPoint>().Where(cp => cp.ConnRole != "src");
                    if (fco.ObjType == GME.MGA.Meta.objtype_enum.OBJTYPE_REFERENCE)
                    {
                        MgaReference reference = (MgaReference)fco;
                        connections = connections.Concat(reference.UsedByConns.Cast<MgaConnPoint>().Where(cp => cp.ConnRole != "src" && cp.References[1] == reference));
                    }
                    foreach (MgaConnPoint connpoint in connections)
                    {
                        MgaSimpleConnection conn = (MgaSimpleConnection)connpoint.Owner;
                        MgaFCO other = conn.Src;
                        if (conn.SrcReferences.Count > 0)
                        {
                            other = (connpoint.Owner as MgaSimpleConnection).SrcReferences[1];
                        }
                        // GMEConsole.Out.WriteLine("&nbsp; " + other.Name);
                        Visit(other, ref out_);
                        List<MgaFCO> dependents;
                        if (DependencyGraph.TryGetValue(fco, out dependents) == false)
                        {
                            dependents = DependencyGraph[fco] = new List<MgaFCO>();
                        }
                        if (other.Meta.Name == typeof(CyPhy.TestBenchRef).Name)
                        {
                            dependents.Add(other);
                        }
                    }
                    tempMarked.Remove(fco);
                    out_.Add(fco);
                    resident.Add(fco, out_);
                }
                else
                {
                    if (previousOut != out_)
                    {
                        foreach (MgaFCO fco2 in out_)
                        {
                            resident[fco2] = previousOut;
                        }
                        previousOut.AddRange(out_);
                        out_ = previousOut;
                    }
                }
            };

            if (toProcess.Count == 0)
            {
                this.Logger.WriteError("Cycle in TestBench dependencies detected");
                return true;
            }
            while (toProcess.Count > 0)
            {
                var before = toProcess.Count;

                List<MgaFCO> out_ = new List<MgaFCO>();
                MgaFCO fco = toProcess.First();
                try
                {
                    Visit(fco, ref out_);
                }
                catch (CycleException ex)
                {
                    this.Logger.WriteError(ex.Message);
                    this.Logger.WriteError(ex.ToString());
                    return true;
                }

                foreach (MgaFCO fco2 in out_)
                {
                    toProcess.Remove(fco2);
                }

                var after = toProcess.Count;
                if (after >= before)
                {
                    string msg = "Determining dependency is not progressing.";
                    this.Logger.WriteError(msg);
                    return true;
                }
            }

            this.DependencyGraph = DependencyGraph.ToDictionary(kvp => kvp.Key, kvp => kvp.Value.Distinct().ToList());
            foreach (KeyValuePair<MgaFCO, List<MgaFCO>> fcos in DependencyGraph)
            {
                this.Logger.WriteDebug("Dependency chain {0}: {1}", fcos.Key.Name, string.Join(" ", fcos.Value.Select(x => x.Name)));
            }

            return false;
        }

        private SotConfig sotConfig = new SotConfig();

        [ComVisible(false)]
        public List<TestBench> TestBenches = new List<TestBench>();

        [ComVisible(false)]
        public List<TestBench> GetTestbenchesFromModel(MgaProject project, MgaModel currentobj, string outputBaseDir)
        {
            componentParameters = null;
            //this.logFileName = Path.Combine(
            //    Path.GetDirectoryName(project.ProjectConnStr.Substring("MGA=".Length)),
            //    "log",
            //    this.ComponentName + "." + System.Diagnostics.Process.GetCurrentProcess().Id + ".log");
            //META.Logger.AddFileListener(this.logFileName, this.ComponentName, project);

            // GMEConsole = GMEConsole.CreateFromProject(project);
            bool disposeLogger = false;
            if (this.Logger == null)
            {
                this.Logger = new GMELogger(project, this.ComponentName);
                disposeLogger = true;
            }
            MgaGateway = new MgaGateway(project);
            MgaGateway.PerformInTransaction(delegate
            {
                this._GetTestbenchesFromModel(currentobj, outputBaseDir);
            });

            if (this.Logger != null && disposeLogger)
            {
                this.Logger.Dispose();
                this.Logger = null;
            }
            //META.Logger.RemoveFileListener(this.ComponentName);

            return this.TestBenches;
        }

        private void _GetTestbenchesFromModel(MgaModel sot, string outputBaseDir)
        {
            var tbRefs = sot
                .ChildObjects
                .OfType<MgaReference>()
                .Where(x => x.Meta.Name == typeof(CyPhy.TestBenchRef).Name);

            foreach (var tbRef in tbRefs)
            {
                var task = tbRef
                    .Referred
                    .ChildObjects
                    .OfType<MgaReference>()
                    .FirstOrDefault(x => x.Meta.Name == typeof(CyPhy.WorkflowRef).Name)
                    .Referred
                    .ChildObjects
                    .OfType<MgaAtom>()
                    .FirstOrDefault();

                var parameters = task.StrAttrByName["Parameters"];
                var parametersDict = new Dictionary<string, string>();
                if (string.IsNullOrWhiteSpace(parameters) == false)
                {
                    try
                    {
                        parametersDict = Newtonsoft.Json.JsonConvert.DeserializeObject<Dictionary<string, string>>(parameters);
                    }
                    catch (Exception ex)
                    {
                        //Console.WriteLine(ex);
                        //Trace.TraceWarning(ex.ToString());
                        this.Logger.WriteWarning(ex.ToString());
                    }
                }

                var testbench = new TestBench();
                testbench.OutputDirectory = GetRandomTBDir(outputBaseDir);
                if (Directory.Exists(testbench.OutputDirectory) == false)
                {
                    Directory.CreateDirectory(testbench.OutputDirectory);
                }
                testbench.Project = tbRef.Project;
                testbench.CurrentObj = tbRef.Referred;
                testbench.ParamInvoke = paramInvoked;

                testbench.Name = string.Format("{0}__{1}", tbRef.Referred.RegistryValue["TestBenchUniqueName"], tbRef.Referred.Name);
                testbench.ProgId = task.StrAttrByName["COMName"];
                testbench.WorkflowParametersDict = parametersDict;

                TestBenches.Add(testbench);
            }

            UpdateDependency(sot);
            if (this.DependencyGraph == null)
            {
                this.Logger.WriteWarning("Dependency was not determined running all test benches without dependencies.");
                return;
            }

            // Update Testbench upstream and downstream dependencies based on the MgaFCO dependency map/list
            foreach (var chain in this.DependencyGraph.Where(x => x.Key.Meta.Name == typeof(CyPhy.TestBenchRef).Name))
            {
                this.TestBenches.First(x => x.CurrentObj.ID == ((MgaReference)chain.Key).Referred.ID).UpstreamTestBenches.AddRange(
                    chain.Value.Select(x => this.TestBenches.First(y => y.CurrentObj.ID == ((MgaReference)x).Referred.ID)));
            }
            foreach (var tb in this.TestBenches)
            {
                foreach (var upstream in tb.UpstreamTestBenches)
                {
                    upstream.DownstreamTestBenches.Add(tb);
                }
                tb.CollectDeps();
            }
        }

        private string GetRandomTBDir(string outputBaseDir)
        {
            string randomFolderName = Path.GetFileNameWithoutExtension(Path.GetRandomFileName());
            List<char> illegalStartChars = new List<char>()
                    {
                        '0','1','2','3','4','5','6','7','8','9'
                    };

            string OutputSubDir = Path.Combine(
                outputBaseDir,
                randomFolderName);

            int maxFolders = 0;

            while (illegalStartChars.Contains(randomFolderName.FirstOrDefault()) ||
                File.Exists(OutputSubDir) ||
                Directory.Exists(OutputSubDir))
            {
                if (maxFolders++ > 2000000)
                {
                    throw new Exception(
                        string.Format("Number of tries ({0}) to create an output folder exceeded in {0}. ",
                            maxFolders,
                            outputBaseDir));
                }

                randomFolderName = Path.GetFileNameWithoutExtension(Path.GetRandomFileName());

                OutputSubDir = Path.Combine(
                    outputBaseDir,
                    randomFolderName);
            }

            return OutputSubDir;
        }

        private void WriteSoTManifest()
        {
            using (StreamWriter writer = new StreamWriter(Path.Combine(OutputBaseDir, "manifest.sot.json")))
            {
                writer.WriteLine(Newtonsoft.Json.JsonConvert.SerializeObject(sotConfig, Newtonsoft.Json.Formatting.Indented));
            }
        }

        internal int paramInvoked = 0;

        #region IMgaComponentEx Members

        private MgaGateway MgaGateway { get; set; }
        //private GMEConsole GMEConsole { get; set; }

        string logFileName { get; set; }

        public void InvokeEx(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, int param)
        {
            this.paramInvoked = param;
            this.ProjectFilename = project.ProjectConnStr.Substring("MGA=".Length);
            sotConfig.OriginalProjectFileName = this.ProjectFilename;

            if (!enabled)
            {
                return;
            }

            string currentWorkDir = System.IO.Directory.GetCurrentDirectory();

            try
            {
                this.Logger = new CyPhyGUIs.GMELogger(project, this.ComponentName);
                this.Logger.GMEConsoleLoggingLevel = SmartLogger.MessageType_enum.Debug;

                if (currentobj == null)
                {
                    this.Logger.WriteFailed("CyPhySOT must be called from a TestBenchSuite.");
                    return;
                }

                this.Logger.WriteInfo("Starting CyPhySoT...");
                System.Windows.Forms.Application.DoEvents();

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
                parameters.ProjectDirectory = Path.GetDirectoryName(currentobj.Project.ProjectConnStr.Substring("MGA=".Length));

                string kindName = string.Empty;

                // Set up the output directory and check Kind of currentObj.
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

                    //this.Parameters.PackageName = Modelica.Factory.GetModifiedName(currentobj.Name);
                });

                if (string.IsNullOrEmpty(kindName) == false && kindName != typeof(CyPhyClasses.TestBenchSuite).Name)
                {
                    this.Logger.WriteFailed("CyPhySOT must be called from a TestBenchSuite.");
                    return;
                }

                PreConfigArgs preConfigArgs = new PreConfigArgs();
                preConfigArgs.ProjectDirectory = parameters.ProjectDirectory;

                // call the preconfiguration with no parameters and get preconfig
                var preConfig = this.PreConfig(preConfigArgs);

                // get previous GUI config
                var previousConfig = META.ComComponent.DeserializeConfiguration(
                    parameters.ProjectDirectory,
                    typeof(CyPhySoTSettings),
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
            catch (Exception ex)
            {
                this.Logger.WriteError("{0} has finished with critical errors see log file {1}.", this.ComponentName, this.logFileName);
                this.Logger.WriteError(ex.ToString());
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
                
                GC.Collect();
                GC.WaitForPendingFinalizers();
                GC.Collect();
                GC.WaitForPendingFinalizers();
            }
        }

        /// <summary>
        /// Helper function for copying entire common folder for post processing
        /// </summary>
        /// <param name="source">Source directory</param>
        /// <param name="target">Destination directory</param>
        private static void CopyFilesRecursively(
            DirectoryInfo source,
            DirectoryInfo target)
        {
            foreach (DirectoryInfo dir in source.GetDirectories())
            {
                CopyFilesRecursively(dir, target.CreateSubdirectory(dir.Name));
            }

            foreach (FileInfo file in source.GetFiles())
            {
                file.CopyTo(Path.Combine(target.FullName, file.Name), true);
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

        public CyPhyGUIs.GMELogger Logger { get; set; }
        ///// <summary>
        ///// Name of the log file. (It is not a full path)
        ///// </summary>
        //private string LogFileFilename { get; set; }

        ///// <summary>
        ///// Full path to the log file.
        ///// </summary>
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
                return (typeof(CyPhySoT.CyPhySoTSettings).GetCustomAttributes(typeof(ProgIdAttribute), false)[0] as ProgIdAttribute).Value;
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
            return null;
        }

        public IInterpreterConfiguration DoGUIConfiguration(IInterpreterPreConfiguration preConfig, IInterpreterConfiguration previousConfig)
        {
            var settings = previousConfig as CyPhySoT.CyPhySoTSettings;

            if (settings == null)
            {
                settings = new CyPhySoT.CyPhySoTSettings();
            }

            return settings;
        }

        private Queue<Tuple<string, TimeSpan>> runtime = new Queue<Tuple<string, TimeSpan>>();
        private DateTime startTime = DateTime.Now;

        public IInterpreterResult Main(IInterpreterMainParameters parameters)
        {
            this.runtime.Clear();

            this.mainParameters = (InterpreterMainParameters)parameters;
            this.result.Success = true;
            var configSuccess = this.mainParameters != null;

            this.paramInvoked = this.mainParameters.StartModeParam;
            this.ProjectFilename = this.mainParameters.Project.ProjectConnStr.Substring("MGA=".Length);
            this.OutputBaseDir = this.mainParameters.OutputDirectory;

            sotConfig.OriginalProjectFileName = this.ProjectFilename;
            bool disposeLogger = false;
            if (this.Logger == null)
            {
                this.Logger = new CyPhyGUIs.GMELogger(this.mainParameters.Project, this.ComponentName);
                this.Logger.WriteInfo("Running CyPhySoT");
                System.Windows.Forms.Application.DoEvents();
                disposeLogger = true;
            }
            if (this.mainParameters.VerboseConsole)
            {
                this.Logger.GMEConsoleLoggingLevel = SmartLogger.MessageType_enum.Debug;
            }
            else
            {
                this.Logger.GMEConsoleLoggingLevel = SmartLogger.MessageType_enum.Info;
            }

            CyPhy.TestBenchSuite tbs = CyPhyClasses.TestBenchSuite.Cast(parameters.CurrentFCO);

//            MgaGateway.PerformInTransaction(delegate
//            {
//                foreach (var testBenchRef in tbs.Children.TestBenchRefCollection)
//                {
//                    if (testBenchRef.Referred == null)
//                    {
//                        continue;
//                    }

//                    var testBench = testBenchRef.Referred.TestBenchType;
//                    if (testBench != null &&
//                        testBench.Impl.MetaBase.Name != (typeof(CyPhy.TestBench)).Name)
//                    {
//                        this.Logger.WriteError(string.Format(@"Test benches must currently
//                        be of type TestBench in a CyPhySoT. Please see {0}.",
//                            testBenchRef.ToHyperLink()));

//                        this.result.Success = false;

//                        object automation;
//                        if (this.componentParameters != null &&
//                            this.componentParameters.TryGetValue("automation", out automation))
//                        {
//                            if ((string)automation == "true")
//                            {
//                                throw new NotSupportedException(@"Error: only test benches of 
//                                type TestBench are currently supported in a CyPhySoT!");
//                            }
//                        }
//                    }
//                }
//            });

            if (this.result.Success == false)
            {
                return this.result;
            }

            var asyncResult = this.Logger.LoggingVersionInfo.BeginInvoke(parameters.Project, null, null);
            var header = this.Logger.LoggingVersionInfo.EndInvoke(asyncResult);
            this.Logger.WriteDebug(header);

            this.UpdateSuccess("Configuration", configSuccess);

            this.result.Labels = "";
            //this.result.LogFileDirectory = Path.Combine(this.mainParameters.ProjectDirectory, "log");
            //this.LogFileFilename = this.ComponentName + "." + System.Diagnostics.Process.GetCurrentProcess().Id + ".log";
            //META.Logger.AddFileListener(this.LogFilePath, this.ComponentName, parameters.Project);

            try
            {
                MgaGateway.PerformInTransaction(delegate
                {
                    this.WorkInMainTransaction();
                },
                abort: true);

                this.WorkOutsideOfMainTransaction();

                this.PrintRuntimeStatistics();
                if (this.result.Success)
                {
                    this.Logger.WriteInfo("CyPhySoT finished successfully.");
                    this.Logger.WriteInfo("Generated files are here: <a href=\"file:///{0}\" target=\"_blank\">{0}</a>", this.mainParameters.OutputDirectory);
                    this.Logger.WriteDebug("[SUCCESS: {0}]", this.result.Success);
                }
                else
                {
                    this.Logger.WriteError("CyPhySoT failed! See error messages above.");
                }
            }
            finally
            {
                if (this.Logger != null && disposeLogger)
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

        private void WorkInMainTransaction()
        {
            // 1) check model, if fails return success = false
            this.Logger.WriteDebug("Checking rules...");
            var checker = new Rules.Checker(this.mainParameters, this.Logger);
            var successRules = checker.Check(this.result.Traceability);
            this.UpdateSuccess("SoT check", successRules);

            if (successRules == false)
            {
                checker.PrintDetails();
                return;
            }

            // 1.5) TODO : Integrate the checks here to checker
            if (UpdateDependency(this.mainParameters.CurrentFCO as MgaModel))
            {
                this.UpdateSuccess("UpdateDependency - failed", false);
                return;
            }

            // 2) Setup the output directory
            sotConfig.SoTID = this.mainParameters.CurrentFCO.ID;
            //this.SetupOutputDirectory(this.mainParameters.Project, this.mainParameters.CurrentFCO);
            //this.UpdateSuccess("SetupOutputDirectory", true);

        }

        private void WorkOutsideOfMainTransaction()
        {
            if (Directory.Exists(this.OutputBaseDir) == false)
            {
                Directory.CreateDirectory(this.OutputBaseDir);
            }
            // 3) Copy mga project file to output directory
            string outputFile = Path.Combine(this.OutputBaseDir, Path.GetFileName(this.ProjectFilename));
            this.mainParameters.Project.Save("MGA=" + outputFile, true);
            sotConfig.ProjectFileName = Path.GetFileName(outputFile);

            this.CleanUpTempMgaProject(outputFile);

            this.UpdateSuccess("Copy mga project", true);

            // 4) Copy Interpreter configuration files

            var projectDir = Path.GetDirectoryName(this.ProjectFilename);

            foreach (var configXML in Directory.GetFiles(Path.Combine(projectDir, "config"), "*.xml"))
            {
                var newConfigXML = configXML.Replace(projectDir, this.OutputBaseDir);
                if (Directory.Exists(Path.GetDirectoryName(newConfigXML)) == false)
                {
                    Directory.CreateDirectory(Path.GetDirectoryName(newConfigXML));
                }

                File.Copy(configXML, newConfigXML, true);
            }

            // CyPhy2CAD stores its config in an xml file instead of in the project

            //string CADConfigXml = Path.Combine(Path.GetDirectoryName(projectFile), "CyPhy2CAD_config.xml");
            //if (File.Exists(CADConfigXml))
            //{
            //    string CADConfigXmlCopy = Path.Combine(OutputBaseDir, "CyPhy2CAD_config.xml");
            //    File.Copy(CADConfigXml, CADConfigXmlCopy, true);
            //}

            //string ModelicaConfigXml = Path.Combine(Path.GetDirectoryName(projectFile),
            //    "CyPhy2Modelica_config_v2.xml");

            //if (File.Exists(ModelicaConfigXml))
            //{
            //    string ModelicaConfigXmlCopy = Path.Combine(OutputBaseDir,
            //        "CyPhy2Modelica_config_v2.xml");

            //    File.Copy(ModelicaConfigXml, ModelicaConfigXmlCopy, true);
            //}

            this.UpdateSuccess("Copied interpreter configs", true);

            // 5) Write out the manifest.json file
            WriteSoTManifest();
            this.UpdateSuccess("WriteSoTManifest", true);
        }

        private void CleanUpTempMgaProject(string outputFile)
        {
            var newProject = new MgaProject();
            bool ro_mode;
            // open the copied temporary mga file
            newProject.Open("MGA=" + outputFile, out ro_mode);
            IMgaTerritory terr = null;

            try
            {
                // create a new transaction
                terr = newProject.BeginTransactionInNewTerr();

                // get the sot object on which the job manager will operate
                var sot = newProject.GetObjectByID(sotConfig.SoTID);

                if (sot != null)
                {
                    // if the sot object exists in the project
                    GME.MGA.Meta.objtype_enum objtype;
                    MgaObject parent;

                    // get the parent folder of which this SoT belongs to.
                    sot.GetParent(out parent, out objtype);

                    if (objtype == GME.MGA.Meta.objtype_enum.OBJTYPE_FOLDER)
                    {
                        // if it is really a folder cast it
                        var parentSoTFolder = parent as MgaFolder;

                        // get all SoT objects in the folder except the one we operate on
                        // mark them for deletion
                        var sotsToDelete = parentSoTFolder
                            .ChildFCOs
                            .Cast<MgaFCO>()
                            .Where(x => x.ID != sotConfig.SoTID)
                            .ToList();

                        try
                        {
                            // destroy all SoTs that are not used by this sot run
                            sotsToDelete.ForEach(x => x.DestroyObject());
                        }
                        catch
                        {
                            // ok
                        }

                        // get all not null test bench objects that this sot run uses.
                        var referredTBsInSot = sot
                            .ChildObjects
                            .OfType<MgaReference>()
                            .Where(x => x.Meta.Name == typeof(CyPhy.TestBenchRef).Name && x.Referred != null)
                            .Select(x => x.Referred)
                            .Cast<MgaModel>()
                            .ToList();

                        foreach (var tb in referredTBsInSot)
                        {
                            GME.MGA.Meta.objtype_enum tbParentObjtype;
                            MgaObject tbParent;

                            // get the parent folder where the referenced test bench located
                            tb.GetParent(out tbParent, out tbParentObjtype);
                            if (tbParentObjtype == GME.MGA.Meta.objtype_enum.OBJTYPE_FOLDER)
                            {
                                var parentTBFolder = tbParent as MgaFolder;

                                // get all test benches that are not used by the sot
                                // WARNING: sometimes COM object comparison can fail this code is not the safest.
                                var tbsToDelete = parentTBFolder
                                    .ChildFCOs
                                    .OfType<MgaModel>()
                                    .Where(x => referredTBsInSot.Contains(x) == false)
                                    .ToList();

                                try
                                {
                                    // delete test benches that sot does not use
                                    tbsToDelete.ForEach(x => x.DestroyObject());
                                }
                                catch
                                {
                                    // ok
                                }
                            }
                        }
                    }
                }

                // commit changes if everything is successful
                newProject.CommitTransaction();
            }
            catch
            {
                // abort all changes if exception occured
                newProject.AbortTransaction();
            }
            finally
            {
                if (terr != null)
                {
                    // destroy the territory we were working in if it is not null
                    terr.Destroy();
                }
            }

            // close the project AND save all changes
            newProject.Close();
        }
    }

    class MgaObjectEqualityComparor<T> : EqualityComparer<T> where T : IMgaObject
    {
        public override bool Equals(T x, T y)
        {
            return x.ID.Equals(y.ID);
        }

        public override int GetHashCode(T obj)
        {
            return obj.ID.GetHashCode();
        }
    }
}
