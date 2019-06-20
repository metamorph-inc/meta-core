namespace CyPhyMasterInterpreter
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.IO;
    using System.Linq;
    using System.Runtime.InteropServices;
    using System.Text;
    using GME.MGA;
    using META;
    using Rules;
    using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
    using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
    using System.Reflection;
    using System.Security;
    using System.Windows.Forms;
    using CyPhyGUIs;

    /// <summary>
    /// Implements full master interpreter functionality.
    /// </summary>
    [Guid("FBA52D8A-F04F-40FD-B186-817427C8D148"),
    ProgId("CyPhyMasterInterpreter.CyPhyMasterInterpreterAPI"),
    ClassInterface(ClassInterfaceType.AutoDual),
    ComVisible(true)]
    public class CyPhyMasterInterpreterAPI : IDisposable
    {
        private Dictionary<string, CyPhyGUIs.IInterpreterConfiguration> interpreterConfigurations =
            new Dictionary<string, CyPhyGUIs.IInterpreterConfiguration>();

        // saves 10% in execution time without calling interpreters: 44 sec vs. 40 sec.
        private HashSet<string> addonNames = new HashSet<string>()
        {
            "ConstraintManager",
            "CyPhyAddOn",
            "CyPhyDecoratorAddon",
            "CyPhyMdaoAddOn",
            "CyPhySignalBlocksAddOn"
        };

        private string JobCollectionID = new Guid().ToString("D");
        public void SetJobCollectionID(string id)
        {
            JobCollectionID = id;
        }

        private bool SendJobCollectionDone = true;
        public void SetSendJobCollectionDone(bool SendJobCollectionDone)
        {
            this.SendJobCollectionDone = SendJobCollectionDone;
        }

        public void Initialize(MgaProject project, CyPhyGUIs.GMELogger logger = null)
        {
            this.IsCancelled = false;

            if (logger == null)
            {
                // dispose our responsibliity
                this.LoggerDisposeRequired = true;
                this.Logger = new CyPhyGUIs.GMELogger(project, this.GetType().Assembly.GetName().Name);
            }
            else
            {
                // dispose caller's responsibility
                this.LoggerDisposeRequired = false;
                this.Logger = logger;
            }

            if (project == null)
            {
                throw new ArgumentNullException("project");
            }

            this.Project = project;

            AppDomain.CurrentDomain.AssemblyResolve += LoadFromSameFolder;
        }

        // MasterInterpreter is loaded via COM with the LoadFrom context. When loading GACed JobManagerLib,
        // we switch to the standard context (fuslogvw shows "Switch from LoadFrom context to default context"),
        // which loses the MasterIntepreter dll path.
        // Tell the AppDomain to look in this folder still
        static string folderPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
        static Assembly LoadFromSameFolder(object sender, ResolveEventArgs args)
        {
            string assemblyPath = Path.Combine(folderPath, new AssemblyName(args.Name).Name + ".dll");
            if (!File.Exists(assemblyPath))
            {
                return null;
            }
            // assume this dll is the right version
            Assembly assembly = Assembly.LoadFrom(assemblyPath);
            return assembly;
        }

        public CyPhyMasterInterpreterAPI()
        {
            this.IsCancelled = false;
        }

        public void CreateLogger()
        {
            if (this.Project != null)
            {
                this.LoggerDisposeRequired = true;
                this.Logger = new CyPhyGUIs.GMELogger(this.Project, this.GetType().Assembly.GetName().Name);
            }
        }

        /// <summary>
        /// Initializes a new instance of <see cref="CyPhyMasterInterpreterAPI"/>.
        /// </summary>
        /// <param name="project">Master interpreter runs on part of this project.</param>
        public CyPhyMasterInterpreterAPI(MgaProject project)
            : this(project, null)
        {
        }

        /// <summary>
        /// Initializes a new instance of <see cref="CyPhyMasterInterpreterAPI"/>.
        /// </summary>
        /// <param name="project">Master interpreter runs on part of this project.</param>
        /// <param name="logger">If logger is give it will be used.</param>
        public CyPhyMasterInterpreterAPI(MgaProject project, CyPhyGUIs.GMELogger logger)
        {
            this.Initialize(project, logger);
        }

        public event Action<ProgressCallbackEventArgs> SingleConfigurationProgress;

        public event Action<ProgressCallbackEventArgs> MultipleConfigurationProgress;

        public bool IsInteractive { get; set; }

        public bool IsCancelled { get; set; }

        public MasterInterpreterResult[] Results { get; private set; }

        /// <summary>
        /// Logs messages to GMEConsole, or Console, and/or to a log file.
        /// </summary>
        [ComVisible(false)]
        public CyPhyGUIs.GMELogger Logger { get; private set; }

        private AVM.DDP.MetaAvmProject ProjectManifest { get; set; }

        /// <summary>
        /// Project on which the execution will happen.
        /// </summary>
        private MgaProject Project { get; set; }

        private JobManagerDispatch Manager { get; set; }

        Dictionary<string, AVM.DDP.MetaTBManifest.DesignType> Designs = new Dictionary<string, AVM.DDP.MetaTBManifest.DesignType>();
        private void Designs_Add(string name, AVM.DDP.MetaTBManifest.DesignType design)
        {
            try
            {
                Designs.Add(name, design);
            }
            catch (ArgumentException e)
            {
                throw new ApplicationException(String.Format("Error: duplicate design name '{0}'", name), e);
            }
        }

        /// <summary>
        /// True if this class should dispose the logger.
        /// </summary>
        private bool LoggerDisposeRequired { get; set; }
        [ComVisible(false)]
        public List<string> ConfigurationNames { get; private set; }

        public string ResultsSubdirectoryName { get; set; }

        [ComVisible(false)]
        public IEnumerable<GMELightObject> UnselectedConfigurationGroups;

        /// <summary>
        /// Disposes the logger if it needs to be disposed.
        /// </summary>
        public void Dispose()
        {
            if (this.LoggerDisposeRequired &&
                this.Logger != null)
            {
                this.Logger.Dispose();
            }
            AppDomain.CurrentDomain.AssemblyResolve -= LoadFromSameFolder;
        }

        public IMgaFCOs GetConfigurations(IMgaProject project, string gmeId)
        {
            if (project == null)
            {
                throw new ArgumentNullException();
            }

            if (string.IsNullOrWhiteSpace(gmeId))
            {
                throw new ArgumentNullException();
            }

            var context = project.GetObjectByID(gmeId);
            return this.GetConfigurations(context as IMgaModel);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="context">TestBench, PET, PCC, or SoT</param>
        /// <returns>CWCs (if configurations are not exported) and ComponentAssembly elements</returns>
        public IMgaFCOs GetConfigurations(IMgaModel context)
        {
            if (context == null)
            {
                throw new ArgumentNullException();
            }

            this.Logger.WriteDebug("Getting configurations for context");

            IMgaFCOs configurations = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
            CyPhy.DesignContainer designContainer;
            foreach (var config in this.GetConfigurationGroups(context, out designContainer).SelectMany(x => x.Configurations.Cast<MgaFCO>()))
            {
                configurations.Append(config);
            }

            this.Logger.WriteDebug("Got {0} configurations for context", configurations.Count);

            return configurations;
        }

        /// <summary>
        /// Shows the master interpreter configuration form, where users can select designs that have to be processed.
        /// Tip: Call the context check first before this function is called.
        /// </summary>
        /// <param name="context"></param>
        /// <returns>Selected configurations and options by the user.</returns>
        /// <exception cref="ArgumentNullException" />
        /// <exception cref="NullReferenceException" />
        /// <exception cref="ExecutionCanceledByUserException" />
        [ComVisible(false)]
        public ConfigurationSelection ShowConfigurationSelectionForm(IMgaModel context, bool enableDebugging = false)
        {
            if (context == null)
            {
                throw new ArgumentNullException();
            }

            ConfigurationSelection results = new ConfigurationSelection()
            {
                Context = context
            };

            CyPhy.DesignContainer designContainer;
            string contextID = null;

            Func<ConfigurationSelectionInput> getInput = () =>
            {
                var configurationGroups = this.GetConfigurationGroups(context, out designContainer);
                ConfigurationSelectionInput configurationSelectionInput = new ConfigurationSelectionInput();
                configurationSelectionInput.designContainer = designContainer;

                AnalysisModelProcessor analysisModelProcessor = null;
                this.ExecuteInTransaction(context, () =>
                {
                    contextID = context.ID;
                    analysisModelProcessor = AnalysisModelProcessor.GetAnalysisModelProcessor(context);
                    configurationSelectionInput.Context = GMELightObject.GetGMELightFromMgaObject(context);
                    configurationSelectionInput.Groups = configurationGroups.Select(x => x.ConvertToLight()).ToArray();
                    configurationSelectionInput.IsDesignSpace = configurationGroups.Any(x => x.Owner == null) == false;

                    configurationSelectionInput.OutputDirectory = analysisModelProcessor.GetResultsDirectory();

                    if (analysisModelProcessor.OriginalSystemUnderTest == null)
                    {
                        configurationSelectionInput.OperationModeInformation = String.Format("{0} - PET",
                            SplitCamelCase(analysisModelProcessor.GetInvokedObject().MetaBase.Name));
                        return;
                    }

                    configurationSelectionInput.OperationModeInformation = string.Format("{0} - {1}",
                        SplitCamelCase(analysisModelProcessor.GetInvokedObject().MetaBase.Name),
                        SplitCamelCase(analysisModelProcessor.OriginalSystemUnderTest.Referred.DesignEntity.Kind));
                });

                var interpreters = analysisModelProcessor.GetWorkflow();
                configurationSelectionInput.InterpreterNames = interpreters.Select(x => x.Name).ToArray();
                return configurationSelectionInput;
            };

            using (ConfigurationSelectionForm selectionForm = new ConfigurationSelectionForm(getInput, enableDebugging))
            {
                System.Windows.Forms.DialogResult dialogResult = System.Windows.Forms.DialogResult.None;
                if (this.IsInteractive)
                {
                    dialogResult = selectionForm.ShowDialog(new NativeWindowWrapper(System.Diagnostics.Process.GetCurrentProcess().MainWindowHandle));
                    this.Logger.GMEConsoleLoggingLevel = selectionForm.ConfigurationSelectionResult.VerboseLogging ?
                        CyPhyGUIs.GMELogger.MessageType_enum.Debug :
                        CyPhyGUIs.SmartLogger.MessageType_enum.Info;
                }
                else
                {
                    selectionForm.SaveSettingsAndResults();
                    dialogResult = System.Windows.Forms.DialogResult.OK;
                    var selectionResult = selectionForm.ConfigurationSelectionResult;
                    selectionResult.KeepTemporaryModels = false;
                    selectionResult.PostToJobManager = false;
                }

                if (dialogResult == System.Windows.Forms.DialogResult.OK)
                {
                    var selectionResult = selectionForm.ConfigurationSelectionResult;
                    results.KeepTemporaryModels = selectionResult.KeepTemporaryModels;
                    results.PostToJobManager = selectionResult.PostToJobManager;
                    results.SelectedConfigurations = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));

                    this.ExecuteInTransaction(context, () =>
                    {
                        foreach (var selectedId in selectionResult.SelectedConfigurations.Select(x => x.GMEId))
                        {
                            var selectedGmeObject = context.Project.GetObjectByID(selectedId);
                            if (selectedGmeObject == null)
                            {
                                throw new NullReferenceException("Selected configuration was not found in the project tree.");
                            }

                            results.SelectedConfigurations.Append(selectedGmeObject as MgaFCO);
                        }

                        this.ConfigurationNames = selectionResult.ConfigurationGroups.SelectMany(group => group.Configurations).Select(fco => fco.Name).ToList();
                        this.UnselectedConfigurationGroups = selectionResult.UnselectedConfigurations;
                    });
                }
                else if (dialogResult == System.Windows.Forms.DialogResult.Yes)
                {
                    var selectionResult = selectionForm.ConfigurationSelectionResult;
                    string ids;
                    if (selectionResult.UnselectedConfigurations.Count() != 0)
                    {
                        ids = "--ids " + String.Join(",", selectionResult.SelectedConfigurations.Select(x => x.GMEId));
                    }
                    else
                    {
                        ids = "--configuration-id " + selectionResult.ConfigurationGroups.Select(fco => fco.Owner.GMEId).Single();
                    }


                    ProcessStartInfo info = new ProcessStartInfo()
                    {
                        FileName = META.VersionInfo.PythonVEnvExe,
                        Arguments = String.Format("\"{0}\" {3} \"{1}\" {2}", Path.Combine(META.VersionInfo.MetaPath, "bin", "parallel_mi.py"),
                                this.Project.ProjectConnStr.Substring("MGA=".Length),
                                contextID,
                                // FIXME will fail if command-line length exceeds 32k
                                ids),
                        RedirectStandardError = true,
                        RedirectStandardInput = true,
                        RedirectStandardOutput = true,
                        UseShellExecute = false,
                        CreateNoWindow = true
                    };
                    Process process = new Process()
                    {
                        StartInfo = info,
                        EnableRaisingEvents = true
                    };
                    GME.CSharp.GMEConsole console = GME.CSharp.GMEConsole.CreateFromProject(this.Project);
                    int streamsOpen = 2;
                    Action streamClosed = () =>
                    {
                        lock (this)
                        {
                            streamsOpen--;
                            if (streamsOpen == 0)
                            {
                                process.Dispose();
                            }
                        }
                    };
                    process.ErrorDataReceived += (sender, data) =>
                    {
                        if (String.IsNullOrEmpty(data.Data) == false)
                        {
                            console.Error.WriteLine(SecurityElement.Escape(data.Data));
                        }
                        else
                        {
                            streamClosed();
                        }
                    };
                    process.OutputDataReceived += (sender, data) =>
                    {
                        if (String.IsNullOrEmpty(data.Data) == false)
                        {
                            console.Info.WriteLine(SecurityElement.Escape(data.Data));
                        }
                        else
                        {
                            streamClosed();
                        }
                    };
                    process.Exited += (s, o) =>
                    {
                        // TODO: check process.ExitCode?
                        // process.Dispose();
                    };
                    process.Disposed += (s, o) =>
                    {
                        Marshal.ReleaseComObject(console.gme);
                        console.gme = null;
                    };
                    process.Start();
                    process.StandardInput.Close();
                    process.BeginErrorReadLine();
                    process.BeginOutputReadLine();
                }
                else
                {
                    throw new ExecutionCanceledByUserException("Main form was canceled.");
                }
            }

            return results;
        }

        public MasterInterpreterResult[] RunInTransactionWithConfigLight(ConfigurationSelectionLight configLight)
        {
            return this.RunInTransactionWithConfig(configLight.GetConfigurationSelection(this.Project));
        }

        [ComVisible(false)]
        public MasterInterpreterResult[] RunInTransactionWithConfig(ConfigurationSelection config)
        {
            return this.RunInTransactionOnMultipleConfigs(
                config.Context,
                config.SelectedConfigurations,
                config.PostToJobManager,
                config.KeepTemporaryModels);
        }

        public MasterInterpreterResult[] RunInTransactionOnOneConfig(
            IMgaModel context,
            IMgaFCO configuration,
            bool postToJobManager = false,
            bool keepTempModels = false)
        {
            MgaFCOs configurations = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
            configurations.Append(configuration as MgaFCO);
            return this.RunInTransactionOnMultipleConfigs(context, configurations, postToJobManager, keepTempModels);
        }

        /// <summary>
        /// All RunInTransaction public functions should end up calling this function.
        /// It checks all constraints and initializes the variables.
        /// <para><remarks>Execute outside of transactions.</remarks></para>
        /// </summary>
        /// <param name="context">Master interpreter runs on this model.</param>
        /// <param name="configurations">Configurations to run the context with.</param>
        /// <param name="postToJobManager">True posts all jobs to the job manager.</param>
        /// <param name="keepTempModels">The interpreter will keep all temporary models.</param>
        /// <returns>Results that represent, which configurations where successful and which ones failed.</returns>
        /// <exception cref="ArgumentNullException" />
        /// <exception cref="InvalidOperationException" />
        /// <exception cref="ExecutionCanceledByUserException" />
        public MasterInterpreterResult[] RunInTransactionOnMultipleConfigs(
            IMgaModel context,
            IMgaFCOs configurations,
            bool postToJobManager = false,
            bool keepTempModels = false)
        {
            if (context == null ||
                configurations == null)
            {
                throw new ArgumentNullException();
            }

            this.Logger.WriteDebug("Start main method for master interpreter.");

            if (this.ProjectManifest == null)
            {
                this.ExecuteInTransaction(context, () =>
                {
                    this.ProjectManifest = this.CreateOrUpdateProjectManifest(context.Project);
                });
            }

            string projectPath = this.ProjectManifest.OutputDirectory;
            this.Logger.WriteDebug("Project directory: {0}", projectPath);

            // delete all previous interpreter configurations
            this.interpreterConfigurations.Clear();

            var results = new List<MasterInterpreterResult>();

            IMgaFCOs resolvedConfigurations = this.ResolveConfigurations(configurations);

            if (resolvedConfigurations.Count == 0)
            {
                this.Logger.WriteInfo("Configurations were not specified. Use all configurations for the given context.");
                resolvedConfigurations = this.GetConfigurations(context);
            }

            var count = 0;
            //this.ExecuteInTransaction(context, () =>
            //{
            count = resolvedConfigurations.Count;
            //});

            var currentNumber = 0;

            this.Logger.WriteInfo("Number of configurations to execute: {0}", count);

            this.ExecuteInTransaction(context, () =>
            {
                this.OnMultipleConfigurationProgress(new ProgressCallbackEventArgs()
                {
                    Percent = 0,
                    Context = context.Name,
                    Title = "Starting"
                });
            });

            int successes = 0;
            int failures = 0;

            foreach (MgaFCO configuration in resolvedConfigurations)
            {
                if (this.IsCancelled)
                {
                    this.Logger.WriteWarning("Execution was cancelled.");
                    break;
                }

                this.Logger.WriteDebug("Processing {0} out of {1} [{2} success(es) and {3} failure(s)]", currentNumber + 1, count, successes, failures);

                this.ExecuteInTransaction(context, () =>
                {
                    this.Logger.WriteDebug("Configuration {0}", configuration.AbsPath);

                    this.OnMultipleConfigurationProgress(new ProgressCallbackEventArgs()
                    {
                        Percent = currentNumber * 99 / count,
                        Context = context.Name,
                        Configuration = configuration.Name,
                        Title = string.Format("Processing {0} out of {1} [{2} success(es) and {3} failure(s)]", currentNumber + 1, count, successes, failures)
                    });
                });

                this.Logger.WriteDebug("Starting analysis model processor");
                MasterInterpreterResult thisResult = null;
                if (keepTempModels == true)
                {
                    this.ExecuteInTransaction(context, () =>
                    {
                        thisResult = this.RunAnalysisModelProcessors(context, configuration, postToJobManager, keepTempModels: true,
                            passTraceability: false);
                    }, abort: false);
                }
                else
                {
                    this.ExecuteInTransaction(context, () =>
                    {
                        thisResult = this.RunAnalysisModelProcessors(context, configuration, postToJobManager, keepTempModels: true /* dont bother deleting tmp models; we are aborting tx */,
                            passTraceability: true);
                    }, abort: true);
                }

                this.Logger.WriteDebug("Analysis model processor is done");

                results.Add(thisResult);
                currentNumber++;

                if (thisResult.Success)
                {
                    successes++;
                }
                else
                {
                    failures++;
                }
            }

            this.ExecuteInTransaction(context, () =>
            {
                if (this.Manager != null && this.UnselectedConfigurationGroups != null)
                {
                    foreach (var configObj in UnselectedConfigurationGroups)
                    {
                        var configuration = this.Project.GetFCOByID(configObj.GMEId);
                        if (configuration.MetaBase.Name == typeof(CyPhy.CWC).Name)
                        {
                            //this.Logger.WriteDebug("{0} was specified as configuration. Start expanding it. {1}", configuration.MetaBase.Name, configuration.AbsPath);
                            //analysisModelProcessor.Expand(CyPhyClasses.CWC.Cast(configuration));
                            //this.Logger.WriteDebug("Expand finished for {0}", configuration.AbsPath);

                            var configs = (CyPhy.Configurations)CyPhyClasses.CWC.Cast(configuration).ParentContainer;
                            var designContainer = (CyPhy.DesignContainer)configs.ParentContainer;
                            var ddpDesign = GetSelectedDesign(CyPhyClasses.CWC.Cast(configuration), designContainer);
                            var ddpDesignName = configuration.Name;
                            Designs_Add(ddpDesignName, ddpDesign);
                        }
                        // TODO else if (configuration.MetaBase.Name == typeof(CyPhy.ComponentAssembly).Name)

                    }
                    if (this.Manager != null && this.Manager.Started)
                    {
                        this.Manager.JobCollection.Designs = Designs;
                    }
                }

                this.OnMultipleConfigurationProgress(new ProgressCallbackEventArgs()
                {
                    Percent = 100,
                    Context = context.Name,
                    Title = "Finished"
                });
            });
            if (this.SendJobCollectionDone && this.Manager != null && this.Manager.Started)
            {
                Manager.Done();
            }

            this.Results = results.ToArray();

            return this.Results;
        }

        public ContextCheckerResult[] CheckContext(IMgaModel context)
        {
            if (context == null)
            {
                throw new ArgumentNullException();
            }

            this.Logger.WriteDebug("Entering Context checker");
            List<ContextCheckerResult> results = new List<ContextCheckerResult>();

            this.ExecuteInTransaction(context, () =>
            {
                if (context == null)
                {
                    throw new ArgumentNullException("Context is null.");
                }

                this.Logger.WriteDebug("Getting context checker for {0}", context.MetaBase.Name);
                var contextChecker = ContextChecker.GetContextChecker(context);

                if (contextChecker == null)
                {
                    this.Logger.WriteDebug("Failed to get context checker for {0}", context.MetaBase.Name);
                    throw new ArgumentOutOfRangeException("Context is invalid.");
                }

                this.Logger.WriteDebug("Got context checker {0} for context {1}", contextChecker.GetType().Name, context.MetaBase.Name);

                this.Logger.WriteDebug("Checking context");
                contextChecker.Check();
                this.Logger.WriteDebug("Context is checked");

                results = contextChecker.Details;
            });

            return results.ToArray();
        }

        public void WriteSummary(MasterInterpreterResult[] results)
        {
            // Print Summary
            this.Logger.WriteInfo(" Summary ");

            if (results == null)
            {
                this.Logger.WriteInfo("No results to show");
            }
            else
            {
                // print sussess messages for the user
                var currentLevel = this.Logger.GMEConsoleLoggingLevel;

                try
                {
                    this.Logger.GMEConsoleLoggingLevel = CyPhyGUIs.SmartLogger.MessageType_enum.Success;

                    this.ExecuteInTransaction(this.Project, () =>
                    {
                        // print failure/success statistics
                        // user master interpreter's result set in case user canceled the execution
                        foreach (var result in results)
                        {
                            string message = string.Format(" {0} {1} {2} {3}",
                                CyPhyCOMInterfaces.TraceabilityExtensions.ToMgaHyperLink(result.Context, this.Logger.Traceability, true),
                                CyPhyCOMInterfaces.TraceabilityExtensions.ToMgaHyperLink(result.Configuration, this.Logger.Traceability, true),
                                SmartLogger.GetGMEConsoleFileLink(result.OutputDirectory, "results folder") ,
                                SecurityElement.Escape(result.Message));

                            if (result.Success)
                            {
                                this.Logger.WriteSuccess(message);
                            }
                            else
                            {
                                this.Logger.WriteFailed(message);
                                this.Logger.WriteDebug(result.Exception);
                            }
                        }
                    }, type: transactiontype_enum.TRANSACTION_READ_ONLY);
                }
                finally
                {
                    // change level back what ever it was
                    this.Logger.GMEConsoleLoggingLevel = currentLevel;
                }
            }
        }

        public bool TryCheckContext(IMgaModel context, out ContextCheckerResult[] results)
        {
            if (context == null)
            {
                throw new ArgumentNullException();
            }

            this.Logger.WriteDebug("Entering TryCheckContext checker");
            var success = true;
            List<ContextCheckerResult> checkerResult = null;
            this.ExecuteInTransaction(context, () =>
            {
                if (context == null)
                {
                    success = false;
                }

                if (success)
                {
                    this.Logger.WriteDebug("Getting context checker for {0}", context.MetaBase.Name);
                    var contextChecker = ContextChecker.GetContextChecker(context);
                    if (contextChecker != null)
                    {
                        this.Logger.WriteDebug("Got context checker {0} for context {1}", contextChecker.GetType().Name, context.MetaBase.Name);

                        this.Logger.WriteDebug("Checking context");
                        success = contextChecker.TryCheck();
                        this.Logger.WriteDebug("Context is checked");

                        checkerResult = contextChecker.Details;
                    }
                    else
                    {
                        success = false;
                    }
                }
            });

            results = checkerResult.ToArray();

            return success;
        }

        public void Cancel()
        {
            this.IsCancelled = true;
        }

        Dictionary<dynamic, Tuple<Action<ProgressCallbackEventArgs>, Action<ProgressCallbackEventArgs>>> progressCallbacks = new Dictionary<dynamic, Tuple<Action<ProgressCallbackEventArgs>, Action<ProgressCallbackEventArgs>>>();
        /*
        cb should be IDispatch equivalent of:
        public interface ProgressCallback
        {
            void SingleConfigurationProgress(ProgressCallbackEventArgs e);
            void MultipleConfigurationProgress(ProgressCallbackEventArgs e);
        }
        */
        public void AddProgressCallback(dynamic cb)
        {
            var cbs = new Tuple<Action<ProgressCallbackEventArgs>, Action<ProgressCallbackEventArgs>>((a) => cb.SingleConfigurationProgress(a),
                (a) => cb.MultipleConfigurationProgress(a));
            SingleConfigurationProgress += cbs.Item1;
            MultipleConfigurationProgress += cbs.Item2;
            progressCallbacks[cb] = cbs;
        }

        public void RemoveProgressCallback(dynamic cb)
        {
            SingleConfigurationProgress -= progressCallbacks[cb].Item1;
            MultipleConfigurationProgress -= progressCallbacks[cb].Item2;
            progressCallbacks.Remove(cb);
        }

        protected virtual void OnSingleConfigurationProgress(ProgressCallbackEventArgs e)
        {
            this.SingleConfigurationProgress?.Invoke(e);
        }

        protected virtual void OnMultipleConfigurationProgress(ProgressCallbackEventArgs e)
        {
            this.MultipleConfigurationProgress?.Invoke(e);
        }

        private static void DirectoryCopy(
            string sourceDirName,
            string destinationDirName,
            bool copySubDirs)
        {
            DirectoryInfo sourceDir = new DirectoryInfo(sourceDirName);
            DirectoryInfo[] dirs = sourceDir.GetDirectories();

            if (sourceDir.Exists == false)
            {
                throw new DirectoryNotFoundException(
                    "Source directory does not exist or could not be found: "
                    + sourceDirName);
            }

            if (Directory.Exists(destinationDirName) == false)
            {
                Directory.CreateDirectory(destinationDirName);
            }

            FileInfo[] files = sourceDir.GetFiles();
            foreach (FileInfo file in files)
            {
                string temppath = Path.Combine(destinationDirName, file.Name);
                file.CopyTo(temppath, true);
            }

            if (copySubDirs)
            {
                foreach (DirectoryInfo subdir in dirs)
                {
                    string temppath = Path.Combine(destinationDirName, subdir.Name);
                    DirectoryCopy(subdir.FullName, temppath, copySubDirs);
                }
            }
        }

        private static string SplitCamelCase(string input)
        {
            return System.Text.RegularExpressions.Regex.Replace(input, "([A-Z])", " $1", System.Text.RegularExpressions.RegexOptions.Compiled).Trim();
        }

        /// <summary>
        /// Creates or updates the project manifest file.
        /// <para><remarks>Execute within transaction.</remarks></para>
        /// </summary>
        /// <param name="project"></param>
        /// <returns></returns>
        private AVM.DDP.MetaAvmProject CreateOrUpdateProjectManifest(MgaProject project)
        {
            if (project == null)
            {
                throw new ArgumentNullException();
            }

            this.Logger.WriteDebug("Generate project manifest.");

            this.ProjectManifest = AVM.DDP.MetaAvmProject.Create(project);

            this.Logger.WriteDebug("Project manifest was generated {0}", this.ProjectManifest.m_filename);

            return this.ProjectManifest;
        }

        private ConfigurationGroup[] GetConfigurationGroups(IMgaModel context, out CyPhy.DesignContainer designContainer)
        {
            if (context == null)
            {
                throw new ArgumentNullException();
            }
            CyPhy.DesignContainer designContainer_ = null;

            List<ConfigurationGroup> configurationGroups = new List<ConfigurationGroup>();
            IMgaFCOs configurations = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));

            this.ExecuteInTransaction(context, () =>
            {
                CyPhy.TestBenchType testBenchType = null;

                if (context.MetaBase.Name == typeof(CyPhy.ParametricExploration).Name)
                {
                    var tbRef = CyPhyClasses.ParametricExploration.Cast(context).Children.TestBenchRefCollection.FirstOrDefault();
                    if (tbRef != null)
                    {
                        testBenchType = tbRef.Referred.TestBenchType;
                    }
                    else
                    {
                        configurations = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
                        configurations.Append(context as MgaFCO);
                        configurationGroups.Add(new ConfigurationGroup() { Configurations = configurations });
                        return;
                    }
                }
                else if (context.MetaBase.Name == typeof(CyPhy.TestBenchSuite).Name)
                {
                    testBenchType = CyPhyClasses.TestBenchSuite.Cast(context).Children.TestBenchRefCollection.FirstOrDefault().Referred.TestBenchType;
                }
                else
                {
                    // test bench type
                    testBenchType = CyPhyClasses.TestBenchType.Cast(context);
                }

                if (testBenchType != null)
                {
                    var referredSut = testBenchType.Children.TopLevelSystemUnderTestCollection.FirstOrDefault().Referred.DesignEntity;

                    if (referredSut is CyPhy.ComponentAssembly)
                    {
                        configurations = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
                        configurations.Append(referredSut.Impl as MgaFCO);
                        configurationGroups.Add(new ConfigurationGroup() { Configurations = configurations });
                    }
                    else if (referredSut is CyPhy.DesignContainer)
                    {
                        designContainer_ = (CyPhy.DesignContainer)referredSut;
                        var configItems = designContainer_
                            .Children
                            .ConfigurationsCollection;

                        foreach (CyPhy.Configurations configItem in configItems)
                        {
                            configurations = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));

                            foreach (var cwc in configItem.Children.CWCCollection)
                            {
                                if (cwc.DstConnections.Config2CACollection.Any())
                                {
                                    foreach (var componentAssemblyRef in cwc.DstConnections.Config2CACollection.Select(x => x.DstEnds.ComponentAssemblyRef))
                                    {
                                        if (componentAssemblyRef.Referred.ComponentAssembly != null)
                                        {
                                            configurations.Append(componentAssemblyRef.Referred.ComponentAssembly.Impl as MgaFCO);
                                        }
                                        else
                                        {
                                            this.Logger.WriteWarning("Null reference to generated component assembly for {0} within {1}.", cwc.Name, configItem.Name);
                                        }
                                    }
                                }
                                else
                                {
                                    // CA exporter will be required to call
                                    configurations.Append(cwc.Impl as MgaFCO);
                                }
                            }

                            configurationGroups.Add(new ConfigurationGroup()
                            {
                                Configurations = configurations,
                                IsDirty = configItem.Attributes.isDirty,
                                Owner = configItem.Impl as MgaModel
                            });
                        }
                    }
                    else
                    {
                        throw new NotImplementedException();
                    }
                }
            });

            designContainer = designContainer_;
            return configurationGroups.ToArray();
        }

        /// <summary>
        /// Resolves all possible variations of configuration specification.
        /// </summary>
        /// <param name="configurations">Contains a set of Component Assembly, CWC, Configurations.</param>
        /// <returns>List of CWC and Component Assembly</returns>
        private MgaFCOs ResolveConfigurations(IMgaFCOs configurations)
        {
            MgaFCOs results = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));

            foreach (MgaFCO configuration in configurations)
            {
                foreach (MgaFCO config in this.ResolveConfiguration(configuration))
                {
                    results.Append(config);
                }
            }

            return results;
        }

        private MgaFCOs ResolveConfiguration(MgaFCO configuration)
        {
            MgaFCOs results = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));

            this.ExecuteInTransaction(configuration, () =>
            {
                if (configuration.MetaBase.Name == typeof(ISIS.GME.Dsml.CyPhyML.Interfaces.Configurations).Name)
                {
                    MgaFilter filter = configuration.Project.CreateFilter();
                    filter.Kind = typeof(ISIS.GME.Dsml.CyPhyML.Interfaces.CWC).Name;

                    foreach (MgaFCO cwc in (configuration as MgaModel).GetDescendantFCOs(filter))
                    {
                        MgaFCO found = null;

                        foreach (MgaSimpleConnection conn in cwc.PartOfConns)
                        {
                            if (conn.MetaBase.Name == typeof(ISIS.GME.Dsml.CyPhyML.Interfaces.Config2CA).Name)
                            {
                                if ((conn.Dst as MgaReference).Referred != null)
                                {
                                    // pick the first non null exported configuration
                                    found = conn.Dst;
                                    break;
                                }
                            }
                        }

                        if (found != null)
                        {
                            results.Append(found);
                        }
                        else
                        {
                            results.Append(cwc);
                        }
                    };
                }
                else if (configuration.MetaBase.Name == typeof(ISIS.GME.Dsml.CyPhyML.Interfaces.CWC).Name)
                {
                    MgaFCO found = null;

                    foreach (MgaSimpleConnection conn in configuration.PartOfConns.Cast<MgaConnPoint>().Select(cp => cp.Owner))
                    {
                        if (conn.MetaBase.Name == typeof(ISIS.GME.Dsml.CyPhyML.Interfaces.Config2CA).Name)
                        {
                            if ((conn.Dst as MgaReference).Referred != null)
                            {
                                // pick the first non null exported configuration
                                found = (conn.Dst as MgaReference).Referred;
                                break;
                            }
                        }
                    }

                    if (found != null)
                    {
                        results.Append(found);
                    }
                    else
                    {
                        results.Append(configuration);
                    }
                }
                else if (configuration.MetaBase.Name == typeof(ISIS.GME.Dsml.CyPhyML.Interfaces.ComponentAssembly).Name)
                {
                    results.Append(configuration);
                }
                else if (configuration.MetaBase.Name == typeof(CyPhy.ParametricExploration).Name)
                {
                    results.Append(configuration);
                }
                else
                {
                    this.Logger.WriteWarning("Not supported configuration type {0} [{1}]", configuration.Name, configuration.MetaBase.Name);
                }

                StringBuilder sbResolvedConfigs = new StringBuilder();

                foreach (MgaFCO config in results)
                {
                    sbResolvedConfigs.AppendFormat(" {0} [{1}]", config.Name, config.MetaBase.Name);
                }

                this.Logger.WriteDebug("Configuration {0} [{1}] was resolved to: {2}", configuration.Name, configuration.MetaBase.Name, sbResolvedConfigs);
            });

            return results;
        }

        private string GetIndexFilename()
        {
            return Path.GetFullPath(Path.Combine(this.ProjectManifest.OutputDirectory, "index.html"));
        }

        private void ExecuteInTransaction(IMgaObject context, Action doWork, bool abort = false, transactiontype_enum type = transactiontype_enum.TRANSACTION_NON_NESTED)
        {
            if (context == null ||
                doWork == null)
            {
                throw new ArgumentNullException();
            }

            this.ExecuteInTransaction(context.Project, doWork, abort, type);
        }

        private void ExecuteInTransaction(MgaProject project, Action doWork, bool abort = false, transactiontype_enum type = transactiontype_enum.TRANSACTION_NON_NESTED)
        {
            if (project == null ||
                doWork == null)
            {
                throw new ArgumentNullException();
            }

            bool inTx = (project.ProjectStatus & 8) != 0;
            if (inTx)
            {
                doWork();
                return;
            }
            project.BeginTransactionInNewTerr(type);

            try
            {
                doWork();

                if (abort)
                {
                    project.AbortTransaction();
                }
                else
                {
                    project.CommitTransaction();
                }
                project.FlushUndoQueue();
            }
            catch (Exception)
            {
                try
                {
                    project.AbortTransaction();
                }
                catch
                {
                }

                throw;
            }
        }

        DateTime MasterInterpreterStartTime = DateTime.Now;

        private MasterInterpreterResult RunAnalysisModelProcessors(
            IMgaModel context,
            IMgaFCO configuration,
            bool postToJobManager = false,
            bool keepTempModels = false,
            bool passTraceability = true)
        {
            if (context == null ||
                configuration == null)
            {
                throw new ArgumentNullException();
            }

            this.Logger.WriteDebug("Preparing analysis model processor");
            var result = new MasterInterpreterResult();
            result.Context = context;
            result.Configuration = configuration;
            result.Success = true;

            string contextName = string.Empty;
            string configurationName = string.Empty;

            AnalysisModelProcessor analysisModelProcessor = null;

            Exception exceptionToThrow = null;

            IAsyncResult createJobManagerResult = null;
            try
            {
                this.Logger.WriteDebug("Turning off addons for performance reasons: {0}", string.Join(", ", this.addonNames));
                this.TurnOffAddons(context);

                Action createJobManager = () =>
                {
                    this.Manager = new JobManagerDispatch(JobCollectionID);
                    this.Manager.StartJobManager(MgaExtensions.MgaExtensions.GetProjectDirectoryPath(this.Project));
                    this.Manager.JobCollection.ConfigurationNames = this.ConfigurationNames;
                };

                AVM.DDP.MetaTBManifest.DesignType ddpDesign = null;
                string ddpDesignName = null;
                this.ExecuteInTransaction(context, () =>
                {
                    contextName = context.Name;
                    configurationName = configuration.Name;

                    this.OnSingleConfigurationProgress(new ProgressCallbackEventArgs()
                    {
                        Percent = 0,
                        Context = contextName,
                        Configuration = configuration.AbsPath,
                        Title = "Initializing"
                    });

                    // expand context for configuration
                    this.Logger.WriteDebug("Getting analysis model processor instance for {0} type", context.MetaBase.Name);
                    analysisModelProcessor = AnalysisModelProcessor.GetAnalysisModelProcessor(context);
                    analysisModelProcessor.ResultsSubdirectoryName = ResultsSubdirectoryName;
                    this.Logger.WriteDebug("Got {0} for {1} {2}", analysisModelProcessor.GetType().Name, context.MetaBase.Name, context.AbsPath);

                    if (this.IsInteractive)
                    {
                        analysisModelProcessor.ShowInterpreterConfigs(this.interpreterConfigurations);
                        this.Logger.WriteDebug("Execution is interactive, showing interpreter configuration forms.");
                    }
                    else
                    {
                        analysisModelProcessor.ShowInterpreterConfigs(this.interpreterConfigurations, interactive: false);
                        this.Logger.WriteDebug("Execution is non interactive, not showing interpreter configuration forms. Using settings from files.");
                    }

                    if (postToJobManager && this.Manager == null)
                    {
                        createJobManagerResult = createJobManager.BeginInvoke(null, null);
                    }

                    this.OnSingleConfigurationProgress(new ProgressCallbackEventArgs()
                    {
                        Percent = 25,
                        Context = contextName,
                        Configuration = configurationName,
                        Title = "Expanding"
                    });

                    if (configuration.MetaBase.Name == typeof(CyPhy.CWC).Name)
                    {
                        this.Logger.WriteDebug("{0} was specified as configuration. Start expanding it. {1}", configuration.MetaBase.Name, configuration.AbsPath);
                        analysisModelProcessor.Expand(CyPhyClasses.CWC.Cast(configuration));
                        this.Logger.WriteDebug("Expand finished for {0}", configuration.AbsPath);

                        var configs = (CyPhy.Configurations)CyPhyClasses.CWC.Cast(configuration).ParentContainer;
                        var designContainer = (CyPhy.DesignContainer)configs.ParentContainer;
                        ddpDesign = GetSelectedDesign(CyPhyClasses.CWC.Cast(configuration), designContainer);
                        ddpDesignName = configuration.Name;
                    }
                    else if (configuration.MetaBase.Name == typeof(CyPhy.ComponentAssembly).Name)
                    {
                        this.Logger.WriteDebug("{0} was specified as configuration. Start expanding it. {1}", configuration.MetaBase.Name, configuration.AbsPath);
                        analysisModelProcessor.Expand(CyPhyClasses.ComponentAssembly.Cast(configuration));
                        // FIXME: get corresponding CWC and assign ddpDesign = GetSelectedDesign(cwc)
                        this.Logger.WriteDebug("Expand finished for {0}", configuration.AbsPath);
                    }
                    else if (configuration.MetaBase.Name == typeof(CyPhy.ParametricExploration).Name)
                    {
                        // standalone PET
                        this.Logger.WriteDebug("{0} was specified as configuration. Start expanding it. {1}", configuration.MetaBase.Name, configuration.AbsPath);
                        analysisModelProcessor.Expand(CyPhyClasses.ParametricExploration.Cast(configuration));
                        this.Logger.WriteDebug("Expand finished for {0}", configuration.AbsPath);
                    }
                    else
                    {
                        this.Logger.WriteDebug("{0} expand is not supported {1}", configuration.MetaBase.Name, configuration.AbsPath);
                        throw new AnalysisModelContextNotSupportedException("Configuration type is not supported. Use CWC or Component Assembly.");
                    }

                    // TODO: give progress update about exporters & their success

                    // design space might be saved multiple times

                    if (analysisModelProcessor.OriginalSystemUnderTest == null)
                    {
                        this.Logger.WriteDebug("[SKIP] Calling design space exporter, since standalone PET does not have a TestBench");
                    }
                    else if (analysisModelProcessor.OriginalSystemUnderTest.AllReferred is CyPhy.DesignContainer)
                    {
                        this.Logger.WriteDebug("Calling design space exporter");
                        bool successDesignSpaceExport = analysisModelProcessor.SaveDesignSpace(this.ProjectManifest);

                        // FIXME: this would case the entire test bench to fail if it is a single configuration.
                        // result.Success = result.Success && successDesignSpaceExport;

                        if (successDesignSpaceExport)
                        {
                            this.Logger.WriteDebug("Design space exporter succeeded.");
                        }
                        else
                        {
                            this.Logger.WriteWarning("Design space exporter failed.");
                        }
                    }
                    else
                    {
                        this.Logger.WriteDebug("[SKIP] Calling design space exporter, since the test bench is defined over a design and not a design space.");
                    }

                    this.Logger.WriteDebug("Calling design exporter");
                    bool successDesignExport = analysisModelProcessor.SaveDesign(this.ProjectManifest);
                    // result.Success = result.Success && successDesignExport;

                    if (successDesignExport)
                    {
                        this.Logger.WriteDebug("Design exporter succeeded.");
                    }
                    else
                    {
                        this.Logger.WriteWarning("Design exporter failed.");
                    }

                    this.Logger.WriteDebug("Saving test bench definition");
                    bool successTestBench = analysisModelProcessor.SaveTestBench(this.ProjectManifest);
                    // result.Success = result.Success && successTestBench;

                    if (successTestBench)
                    {
                        this.Logger.WriteDebug("Saving test bench definition succeeded.");
                    }
                    else
                    {
                        this.Logger.WriteWarning("Saving test bench definition failed.");
                    }

                    this.Logger.WriteDebug("Saving test bench manifest");
                    analysisModelProcessor.SaveTestBenchManifest(this.ProjectManifest, configuration.Name, MasterInterpreterStartTime);

                    var manifest = AVM.DDP.MetaTBManifest.OpenForUpdate(analysisModelProcessor.OutputDirectory);
                    manifest.Design = ddpDesign;
                    manifest.Serialize(analysisModelProcessor.OutputDirectory);

                    foreach (var testbenchRef in context.ChildFCOs.Cast<MgaFCO>().Where(fco => fco.Meta.Name == typeof(CyPhy.WorkflowRef).Name))
                    {
                        var tbRef = CyPhyClasses.WorkflowRef.Cast(testbenchRef);
                        if (tbRef.Referred.Workflow != null)
                        {
                            var projectDir = Path.GetDirectoryName(Path.GetFullPath(tbRef.Impl.Project.ProjectConnStr.Substring("MGA=".Length)));
                            CopyFiles(tbRef.Referred.Workflow.Children.CopyFilesCollection, projectDir, analysisModelProcessor.OutputDirectory);
                        }
                    }

                    this.Logger.WriteDebug("Serializing Project manifest file with updates.");
                    this.ProjectManifest.Serialize();
                    this.Logger.WriteDebug("Project manifest was successfully saved.");
                });

                this.OnSingleConfigurationProgress(new ProgressCallbackEventArgs()
                {
                    Percent = 45,
                    Context = contextName,
                    Configuration = configurationName,
                    Title = "Model is ready for interpreters"
                });

                analysisModelProcessor.InterpreterProgress += (object sender, InterpreterProgressEventArgs e) =>
                {
                    var interpreterName = e.Interpreter == null ? string.Empty : e.Interpreter.Name;
                    this.OnSingleConfigurationProgress(new ProgressCallbackEventArgs()
                    {
                        Percent = 50 + (e.Percent / 5), // we have 20 percent room on the main progress
                        Context = contextName,
                        Configuration = configurationName,
                        Title = string.Format("Running interpreters {0}% {1} success status: {2}", e.Percent, interpreterName, e.Success)
                    });
                };

                this.Logger.WriteDebug("Running interpreters. Keeping temp models: {0}", keepTempModels);

                bool isVerbose = this.Logger.GMEConsoleLoggingLevel == CyPhyGUIs.SmartLogger.MessageType_enum.Debug;

                result.OutputDirectory = analysisModelProcessor.OutputDirectory;
                analysisModelProcessor.RunInterpreters(passTraceability, isVerbose);

                this.Logger.WriteDebug("Interpreters finished.");

                this.OnSingleConfigurationProgress(new ProgressCallbackEventArgs()
                {
                    Percent = 70,
                    Context = contextName,
                    Configuration = configurationName,
                    Title = "All interpreters successfully finished"
                });

                this.ExecuteInTransaction(context, () =>
                {
                    if (postToJobManager)
                    {
                        this.OnSingleConfigurationProgress(new ProgressCallbackEventArgs()
                        {
                            Percent = 80,
                            Context = contextName,
                            Configuration = configurationName,
                            Title = "Posting to Job Manager"
                        });

                        if (createJobManagerResult != null)
                        {
                            this.Logger.WriteDebug("Job manager instance is null. Initializing one.");
                            createJobManager.EndInvoke(createJobManagerResult);
                            this.Logger.WriteDebug("Job manager dispatch is ready to receive jobs");
                        }

                        this.Logger.WriteDebug("Posting to the job manager");
                        var postedToJobManager = analysisModelProcessor.PostToJobManager(this.Manager);
                        result.Success = result.Success && postedToJobManager;

                        if (ddpDesign != null)
                        {
                            Designs_Add(ddpDesignName, ddpDesign);
                        }

                        if (postedToJobManager)
                        {
                            this.Logger.WriteInfo("Job posted to the job manager: {0}", context.Name);
                        }
                        else
                        {
                            this.Logger.WriteError("Job was not posted to the job manager: {0}", context.Name);
                        }

                        this.OnSingleConfigurationProgress(new ProgressCallbackEventArgs()
                        {
                            Percent = 89,
                            Context = contextName,
                            Configuration = configurationName,
                            Title = "Posted to Job Manager"
                        });
                    }
                    else
                    {
                        this.Logger.WriteDebug("Not posting to the job manager");

                        this.OnSingleConfigurationProgress(new ProgressCallbackEventArgs()
                        {
                            Percent = 89,
                            Context = contextName,
                            Configuration = configurationName,
                            Title = "Skip posting to Job Manager"
                        });
                    }
                });
            }
            catch (AnalysisModelInterpreterConfigurationFailedException ex)
            {
                this.Logger.WriteDebug(ex.ToString());
                result.Success = false;
                result.Message = ex.Message;
                result.Exception = ex.ToString();
                exceptionToThrow = ex;
            }
            catch (AnalysisModelInterpreterException ex)
            {
                this.Logger.WriteDebug(ex.ToString());
                result.Success = false;
                result.Message = ex.Message;
                result.Exception = ex.ToString();
            }
            catch (AnalysisModelExpandFailedException ex)
            {
                this.Logger.WriteDebug(ex.ToString());
                result.Success = false;
                result.Message = ex.Message;
                result.Exception = ex.ToString();
            }
            catch (Exception ex)
            {
                this.Logger.WriteDebug(ex.ToString());
                result.Success = false;
                result.Message = ex.Message;
                if (ex.InnerException != null)
                {
                    result.Message = string.Format("{0} ---> {1}", ex.Message, ex.InnerException.Message);
                }

                result.Exception = ex.ToString();
            }
            finally
            {
                if (createJobManagerResult != null)
                {
                    createJobManagerResult.AsyncWaitHandle.WaitOne();
                }
                // clean up if interpreter is canceled
                this.ExecuteInTransaction(context, () =>
                {
                    // destroy objects if keepTempModels == false
                    if (keepTempModels == false)
                    {
                        this.OnSingleConfigurationProgress(new ProgressCallbackEventArgs()
                        {
                            Percent = 90,
                            Context = contextName,
                            Configuration = configurationName,
                            Title = "Removing temporary models"
                        });

                        this.Logger.WriteDebug("Removing temporary models.");

                        analysisModelProcessor.DeleteTemporaryObjects();

                        this.Logger.WriteDebug("Temporary models are removed.");

                        this.OnSingleConfigurationProgress(new ProgressCallbackEventArgs()
                        {
                            Percent = 100,
                            Context = contextName,
                            Configuration = configurationName,
                            Title = "Removing temporary models"
                        });
                    }
                    else
                    {
                        this.OnSingleConfigurationProgress(new ProgressCallbackEventArgs()
                        {
                            Percent = 100,
                            Context = contextName,
                            Configuration = configurationName,
                            Title = "Keeping temporary models"
                        });
                    }
                });

                this.Logger.WriteDebug("Turning addons back on: {0}", string.Join(", ", this.addonNames));
                this.TurnOnAddons(context);
            }

            if (exceptionToThrow != null)
            {
                this.Logger.WriteDebug(exceptionToThrow.ToString());
                throw exceptionToThrow;
            }

            return result;
        }

        private AVM.DDP.MetaTBManifest.DesignType GetSelectedDesign(CyPhy.CWC cwc, CyPhy.DesignContainer designContainer)
        {
            HashSet<string> selectedIDs = new HashSet<string>();
            foreach (var fco in cwc.Children.DesignEntityRefCollection)
            {
                if (fco.GenericReferred != null)
                {
                    selectedIDs.Add(fco.GenericReferred.ID);
                }
            }

            var rootDesign = new AVM.DDP.MetaTBManifest.DesignType();
            Queue<Tuple<CyPhy.DesignContainer, AVM.DDP.MetaTBManifest.DesignType>> designs = new Queue<Tuple<CyPhy.DesignContainer, AVM.DDP.MetaTBManifest.DesignType>>();
            designs.Enqueue(new Tuple<CyPhy.DesignContainer, AVM.DDP.MetaTBManifest.DesignType>(designContainer, rootDesign));
            while (designs.Count > 0)
            {
                var design = designs.Dequeue();
                var cyPhyDesign = design.Item1;
                design.Item2.Name = cyPhyDesign.Name;
                design.Item2.Type = cyPhyDesign.Attributes.ContainerType.ToString();
                design.Item2.Children = new List<AVM.DDP.MetaTBManifest.DesignType>();
                foreach (var child in cyPhyDesign.Children.DesignContainerCollection)
                {
                    var childDdpDesign = new AVM.DDP.MetaTBManifest.DesignType();
                    designs.Enqueue(new Tuple<CyPhy.DesignContainer, AVM.DDP.MetaTBManifest.DesignType>(child, childDdpDesign));
                    design.Item2.Children.Add(childDdpDesign);
                }
                foreach (var child in cyPhyDesign.Children.ComponentRefCollection.Concat<ISIS.GME.Common.Interfaces.FCO>(cyPhyDesign.Children.ComponentCollection))
                {
                    var childDesign = new AVM.DDP.MetaTBManifest.DesignType();
                    childDesign.Name = child.Name;
                    childDesign.Type = "Component";
                    childDesign.Selected = selectedIDs.Contains(child.ID);
                    design.Item2.Children.Add(childDesign);
                }
            }
            return rootDesign;
        }

        private void TurnOnAddons(IMgaModel context)
        {
            this.ExecuteInTransaction(context, () =>
            {
                var addons = context.Project.AddOnComponents.Cast<IMgaComponentEx>().Where(x => this.addonNames.Contains(x.ComponentName));
                foreach (var addon in addons)
                {
                    try
                    {
                        addon.Enable(true);
                    }
                    catch (Exception ex)
                    {
                        // if one fails keep trying the other ones.
                        this.Logger.WriteDebug("Turing on addon failed: {0} --> {1}", addon.ComponentName, ex.ToString());
                    }
                }
            });
        }

        private void TurnOffAddons(IMgaObject context)
        {
            this.ExecuteInTransaction(context, () =>
            {
                var addons = context.Project.AddOnComponents.Cast<IMgaComponentEx>().Where(x => this.addonNames.Contains(x.ComponentName));
                foreach (var addon in addons)
                {
                    try
                    {
                        addon.Enable(false);
                    }
                    catch (Exception ex)
                    {
                        // if one fails keep trying the other ones.
                        this.Logger.WriteDebug("Turing off addon failed: {0} --> {1}", addon.ComponentName, ex.ToString());
                    }
                }
            });
        }

        public static void CopyDirectory(string sourcePath, string destPath)
        {
            if (!Directory.Exists(destPath))
            {
                Directory.CreateDirectory(destPath);
            }

            foreach (string file in Directory.GetFiles(sourcePath))
            {
                string dest = Path.Combine(destPath, Path.GetFileName(file));
                File.Copy(file, dest);
            }

            foreach (string folder in Directory.GetDirectories(sourcePath))
            {
                string dest = Path.Combine(destPath, Path.GetFileName(folder));
                CopyDirectory(folder, dest);
            }
        }

        public static void CopyFiles(IEnumerable<CyPhy.CopyFiles> copyFilesCollection, string projectDir, string outputDirectory)
        {
            foreach (var copyFiles in copyFilesCollection)
            {
                string strip;
                IEnumerable<string> entries;
                if (Path.IsPathRooted(copyFiles.Attributes.Source))
                {
                    entries = Directory.EnumerateFileSystemEntries(Path.GetFullPath(Path.GetDirectoryName(copyFiles.Attributes.Source)), Path.GetFileName(copyFiles.Attributes.Source));
                    strip = Path.GetFullPath(Path.GetDirectoryName(copyFiles.Attributes.Source));
                }
                else
                {
                    entries = Directory.EnumerateFileSystemEntries(projectDir, copyFiles.Attributes.Source);
                    strip = Path.GetFullPath(Path.GetDirectoryName(Path.Combine(projectDir, copyFiles.Attributes.Source)));
                }

                string dest = Path.Combine(outputDirectory, copyFiles.Attributes.Destination);
                Directory.CreateDirectory(dest);
                foreach (string entry in entries)
                {
                    if ((File.GetAttributes(entry) & FileAttributes.Directory) == FileAttributes.Directory)
                    {
                        string finalDestination;
                        if (entry == strip)
                        {
                            finalDestination = dest;
                        }
                        else
                        {
                            finalDestination = Path.Combine(dest, entry.Substring(strip.Length + 1));
                        }


                        CopyDirectory(entry, finalDestination);
                    }
                    else
                    {
                        File.Copy(entry, Path.Combine(dest, Path.GetFileName(entry)));
                    }

                }
            }
        }
    }
}
