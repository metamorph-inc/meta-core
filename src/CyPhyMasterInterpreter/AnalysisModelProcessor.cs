namespace CyPhyMasterInterpreter
{
    using System;
    using System.Collections.Generic;
    using System.IO;
    using System.Linq;
    using System.Text;
    using GME.MGA;
    using META;
    using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
    using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

    /// <summary>
    /// Provides the following functionalities: expand a context [design space -> configurations],
    /// run interpreters, post jobs to job manager.
    /// </summary>
    public abstract class AnalysisModelProcessor
    {
        /// <summary>
        /// Suffix string for the Temporary folders in GME project.
        /// </summary>
        protected const string TemporaryFolderSuffix = " TemporaryFolder";

        /// <summary>
        /// Contains further model processors, which must expand their contexts, and provide interpreter configuration
        /// setting forms for the user.
        /// </summary>
        private Stack<AnalysisModelProcessor> innerExpanders = new Stack<AnalysisModelProcessor>();

        /// <summary>
        /// Interpreters which will be called on the expanded context.
        /// </summary>
        private Queue<ComComponent> interpreters;

        /// <summary>
        /// Directory where the GME project lives in the file system. Absolute path is preferred.
        /// </summary>
        private string projectDirectory = string.Empty;

        /// <summary>
        /// Temporary objects that have to be deleted after model is processed.
        /// </summary>
        private Stack<IMgaObject> temporaryObjects = new Stack<IMgaObject>();

        /// <summary>
        /// Provides a traceability map for the analysis interpreters between original context and temporary objects
        /// that might be deleted at the end of the execution. Helps to provide valid hyperlinks to the user to 
        /// identify the root cause of the problem.
        /// </summary>
        private META.MgaTraceability traceability = new MgaTraceability();

        /// <summary>
        /// Callback is called, while RunInterpreters method is running all the interpreters.
        /// </summary>
        public event EventHandler<InterpreterProgressEventArgs> InterpreterProgress;

        protected string OriginalCurrentFCOName;

        /// <summary>
        /// Gets the interpreters which will be called on the expanded context.
        /// </summary>
        public Queue<ComComponent> Interpreters
        {
            get
            {
                if (this.interpreters == null)
                {
                    this.interpreters = this.GetWorkflow();
                }

                return this.interpreters;
            }
        }

        /// <summary>
        /// Gets or sets the original top level system under test object, which is inferred from context based on the context type.
        /// </summary>
        public CyPhy.TopLevelSystemUnderTest OriginalSystemUnderTest { get; protected set; }

        /// <summary>
        /// Gets or sets the configuration on which the analysis interpreters should run within the given context.
        /// </summary>
        /// <remarks>Original top level system under test will be redirected to this object.</remarks>
        public CyPhy.ComponentAssembly Configuration { get; protected set; }

        public CyPhy.CWC DesignConfiguration { get; protected set; }

        /// <summary>
        /// Gets a traceability map for the analysis interpreters between original context and temporary objects
        /// that might be deleted at the end of the execution. Helps to provide valid hyperlinks to the user to 
        /// identify the root cause of the problem.
        /// </summary>
        public META.MgaTraceability Traceability
        {
            get
            {
                return this.traceability;
            }
        }

        /// <summary>
        /// Gets a set of interpreters that have to be configured.
        /// </summary>
        public virtual Queue<ComComponent> InterpretersToConfiguration
        {
            get
            {
                return this.Interpreters;
            }
        }

        /// <summary>
        /// Gets further model processors, which must expand their contexts, and provide interpreter configuration
        /// setting forms for the user.
        /// </summary>
        protected Stack<AnalysisModelProcessor> InnerExpanders
        {
            get
            {
                return this.innerExpanders;
            }
        }

        /// <summary>
        /// Gets or sets the output directory in which the interpreters will generate the files, translated models.
        /// </summary>
        public string OutputDirectory { get; protected set; }

        /// <summary>
        /// Gets the directory where the GME project lives in the file system. Assumes that the project directory is not 
        /// changing, i.e. project will not be saved in a way that the Project Connection String changes.
        /// </summary>
        protected string ProjectDirectory
        {
            get
            {
                if (string.IsNullOrWhiteSpace(this.projectDirectory))
                {
                    this.projectDirectory = MgaExtensions.MgaExtensions.GetProjectDirectoryPath(this.GetInvokedObject().Project);
                }

                return this.projectDirectory;
            }
        }

        /// <summary>
        /// Factory method to get a new analysis model processor instance based on a given context.
        /// </summary>
        /// <param name="context">Model context</param>
        /// <returns>New instance of an analysis model processor.</returns>
        /// <exception cref="ArgumentNullException">If context is null.</exception>
        /// <exception cref="AnalysisModelContextNotSupportedException">If context is not supported.</exception>
        public static AnalysisModelProcessor GetAnalysisModelProcessor(IMgaModel context)
        {
            if (context == null)
            {
                throw new ArgumentNullException("context");
            }

            AnalysisModelProcessor analysisModelProcessor = null;
            if (context.MetaBase.Name == typeof(CyPhy.ParametricExploration).Name)
            {
                analysisModelProcessor = new ParametricExplorationProcessor(CyPhyClasses.ParametricExploration.Cast(context));
            }
            else if (context.MetaBase.Name == typeof(CyPhy.TestBenchSuite).Name)
            {
                analysisModelProcessor = new TestBenchSuiteProcessor(CyPhyClasses.TestBenchSuite.Cast(context));
            }
            else if (TestBenchTypeProcessor.SupportedTestBenchTypes.Contains(context.MetaBase.Name))
            {
                if (context.Meta.Name == typeof(CyPhy.CFDTestBench).Name)
                {
                    string[] openFoamKinds = new string[] {
                        typeof(CyPhy.CalmWaterSolverSettings).Name,
                        typeof(CyPhy.CorrelationSettings).Name,
                        typeof(CyPhy.WaveResistanceSolverSettings).Name };
                    if (context.ChildFCOs.Cast<MgaFCO>().Any(fco => openFoamKinds.Contains(fco.Meta.Name)))
                    {
                        analysisModelProcessor = new MultiJobRunProcessor(CyPhyClasses.CFDTestBench.Cast(context));
                    }
                    else
                    {
                        analysisModelProcessor = new TestBenchTypeProcessor(CyPhyClasses.TestBenchType.Cast(context));
                    }
                }
                else if (context.Meta.Name == typeof(CyPhy.BlastTestBench).Name)
                {
                    var blastTestBench = CyPhyClasses.BlastTestBench.Cast(context);
                    int tierLevel = (int)blastTestBench.Attributes.BlastTier;
                    int tierThresholdForMuliJobRun = (int)CyPhyClasses.BlastTestBench.AttributesClass.BlastTier_enum._2;

                    if (tierLevel > tierThresholdForMuliJobRun)
                    {
                        // use MultiJobRunProcessor if the current tier level is greater than the defined threshold
                        analysisModelProcessor = new MultiJobRunProcessor(blastTestBench);
                    }
                    else
                    {
                        analysisModelProcessor = new TestBenchTypeProcessor(blastTestBench);
                    }
                }
                else if (context.Meta.Name == typeof(CyPhy.BallisticTestBench).Name)
                {
                    var ballisticTestBench = CyPhyClasses.BallisticTestBench.Cast(context);
                    int tierLevel = (int)ballisticTestBench.Attributes.Tier;
                    int tierThresholdForMuliJobRun = (int)CyPhyClasses.BallisticTestBench.AttributesClass.Tier_enum._2;

                    if (tierLevel > tierThresholdForMuliJobRun)
                    {
                        // use MultiJobRunProcessor if the current tier level is greater than the defined threshold
                        analysisModelProcessor = new MultiJobRunProcessor(ballisticTestBench);
                    }
                    else
                    {
                        analysisModelProcessor = new TestBenchTypeProcessor(ballisticTestBench);
                    }
                }
                else
                {
                    // test bench type
                    analysisModelProcessor = new TestBenchTypeProcessor(CyPhyClasses.TestBenchType.Cast(context));
                }
            }
            else
            {
                throw new AnalysisModelContextNotSupportedException(string.Format("{0} does not supported", context.MetaBase.Name));
            }

            analysisModelProcessor.OriginalCurrentFCOName = context.Name;
            return analysisModelProcessor;
        }

        /// <summary>
        /// Saves the test bench manifest file to interpreter's output directory and indexes the newly generated file 
        /// in the project manifest.
        /// </summary>
        /// <param name="projectManifest">Manifest object of the project.</param>
        /// <returns>True if the manifest was saved and indexed successfully, otherwise false.</returns>
        public abstract bool SaveTestBenchManifest(AVM.DDP.MetaAvmProject projectManifest, string configurationName, DateTime analysisStartTime);

        /// <summary>
        /// Updates the execution steps in the test bench manifest file based on the Tasks and Execution tasks in the
        /// workflow.
        /// </summary>
        /// <returns>True if all Tasks were successfully added, false otherwise.</returns>
        public abstract bool UpdateTestBenchManifestExecutionSteps(AVM.DDP.MetaTBManifest manifest);

        /// <summary>
        /// Saves the test bench descriptor file to the test bench directory and indexes the newly generated file 
        /// in the project manifest.
        /// </summary>
        /// <param name="projectManifest">Manifest object of the project.</param>
        /// <returns>True if the manifest was saved and indexed successfully, otherwise false.</returns>
        public abstract bool SaveTestBench(AVM.DDP.MetaAvmProject projectManifest);

        /// <summary>
        /// Gets the object from which the analysis model processor was instantiated.
        /// </summary>
        /// <returns>Model context</returns>
        public abstract MgaModel GetInvokedObject();

        /// <summary>
        /// Gets the expanded version of the invoked object.
        /// </summary>
        /// <returns>Expanded model context</returns>
        public abstract MgaModel GetExpandedObject();

        /// <summary>
        /// Gets the set of Interpreters form the workflow.
        /// </summary>
        /// <returns>Interpreters that need to be run on the expanded object.</returns>
        public abstract Queue<ComComponent> GetWorkflow();

        /// <summary>
        /// Expands the current context and sets the expanded object.
        /// </summary>
        /// <param name="configuration">Top level system under test will be redirected to this object.</param>
        public abstract void Expand(CyPhy.ComponentAssembly configuration);

        public virtual void Expand(CyPhy.ParametricExploration parametricExploration)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Posts the generated analysis packages to the JobManager for execution.
        /// </summary>
        /// <param name="manager">JobManager instance to post the job to.</param>
        /// <returns>True if job was posted successfully, otherwise false</returns>
        public abstract bool PostToJobManager(JobManagerDispatch manager);

        /// <summary>
        /// Deletes all temporary objects in the right order.
        /// </summary>
        public void DeleteTemporaryObjects()
        {
            var innerExpander = this.InnerExpanders.Any() ? this.InnerExpanders.Pop() : null;

            var prevoiusSize = -1;

            while (innerExpander != null && prevoiusSize != this.InnerExpanders.Count)
            {
                prevoiusSize = this.InnerExpanders.Count;
                try
                {
                    innerExpander.DeleteTemporaryObjects();
                }
                catch
                {
                    // TODO: ignore it... LOG IT!
                }

                innerExpander = this.InnerExpanders.Any() ? this.InnerExpanders.Pop() : null;
            }

            var temporaryObject = this.temporaryObjects.Any() ? this.temporaryObjects.Pop() : null;

            prevoiusSize = -1;

            while (temporaryObject != null && prevoiusSize != this.temporaryObjects.Count)
            {
                prevoiusSize = this.temporaryObjects.Count;
                try
                {
                    temporaryObject.DestroyObject();
                }
                catch
                {
                    // TODO: ignore it... LOG IT!
                }

                temporaryObject = this.temporaryObjects.Any() ? this.temporaryObjects.Pop() : null;
            }
        }

        /// <summary>
        /// Expands the given context.
        /// </summary>
        /// <param name="configuration">CWC object to expand to.</param>
        public void Expand(CyPhy.CWC configuration)
        {
            if (configuration == null)
            {
                throw new ArgumentNullException("configuration");
            }
            this.DesignConfiguration = configuration;

            if (configuration.DstConnections.Config2CACollection.Any())
            {
                this.Expand(configuration
                    .DstConnections
                    .Config2CACollection
                    .FirstOrDefault()
                    .DstEnds
                    .ComponentAssemblyRef
                    .Referred
                    .ComponentAssembly);
            }
            else
            {
                var exportedConfigurations = this.RunCAExporter(new List<MgaFCO>() { configuration.Impl as MgaFCO });
                if (exportedConfigurations.Any() == false)
                {
                    throw new AnalysisModelExpandFailedException(string.Format("Failed to export configuration {0}.", configuration.Name));
                }

                // TODO: if CaRefsToProcess is empty then CAExporter failed.
                CyPhy.ComponentAssemblyRef componentAssemblyRef = CyPhyClasses.ComponentAssemblyRef.Cast(exportedConfigurations.FirstOrDefault());
                if (componentAssemblyRef.AllReferred == null)
                {
                    throw new AnalysisModelExpandFailedException(string.Format("Exported configuration {0} yielded null reference to component assembly.", configuration.Name));
                }

                temporaryObjects.Push(componentAssemblyRef.Referred.ComponentAssembly.Impl);
                this.Expand(componentAssemblyRef.Referred.ComponentAssembly);
            }
        }

        /// <summary>
        /// Runs the Component Assembly exporter. Exports CWC configurations into Component Assemblies.
        /// </summary>
        /// <param name="cwcs">Set of CWCs to export.</param>
        /// <returns>Exported Component Assembly references.</returns>
        public IEnumerable<MgaReference> RunCAExporter(IEnumerable<MgaFCO> cwcs)
        {
            if (cwcs == null)
            {
                throw new ArgumentNullException("cwcs");
            }

            List<MgaReference> exportedConfigurations = new List<MgaReference>();

            if (cwcs.Any())
            {
                IMgaComponentEx componentAssemblyExporter = (IMgaComponentEx)Activator.CreateInstance(Type.GetTypeFromProgID("MGA.Interpreter.CyPhyCAExporter"));

                MgaFCOs selected = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
                foreach (MgaFCO cwc in cwcs)
                {
                    selected.Append(cwc);
                }

                componentAssemblyExporter.Initialize(cwcs.First().Project);
                componentAssemblyExporter.InvokeEx(cwcs.First().Project, cwcs.First().ParentModel as MgaFCO, selected, 128);
                foreach (MgaFCO cwc in cwcs)
                {
                    foreach (MgaConnPoint cp in cwc.PartOfConns)
                    {
                        MgaSimpleConnection configuration2ComponentAssembly = cp.Owner as MgaSimpleConnection;
                        exportedConfigurations.Add(configuration2ComponentAssembly.Dst as MgaReference);
                    }
                }
            }

            return exportedConfigurations;
        }

        /// <summary>
        /// Exports the design space into design interchange format AVM Design Model and indexes the file
        /// in the project manifest.
        /// </summary>
        /// <param name="projectManifest">Given project manifest object.</param>
        /// <returns>True if exporting and indexing are successful, otherwise false.</returns>
        public bool SaveDesignSpace(AVM.DDP.MetaAvmProject projectManifest)
        {
            if (projectManifest == null)
            {
                throw new ArgumentNullException("projectManifest");
            }

            this.ThrowIfNotExpanded();

            if (this.OriginalSystemUnderTest == null)
            {
                return true;
            }
            else if (this.OriginalSystemUnderTest.AllReferred is CyPhy.DesignContainer)
            {
                bool success = false;
                try
                {
                    success = projectManifest.SaveDesign(this.OriginalSystemUnderTest.Referred.DesignEntity);
                }
                catch (Exception ex)
                {
                    throw new AnalysisModelProcessorException("Saving design space model failed.", ex);
                }

                return success;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// Exports the design into design interchange format AVM Design Model and indexes the file
        /// in the project manifest.
        /// </summary>
        /// <param name="projectManifest">Given project manifest object.</param>
        /// <returns>True if exporting and indexing are successful, otherwise false.</returns>
        public virtual bool SaveDesign(AVM.DDP.MetaAvmProject projectManifest)
        {
            if (projectManifest == null)
            {
                throw new ArgumentNullException("projectManifest");
            }

            this.ThrowIfNotExpanded();

            bool success = false;
            try
            {
                success = projectManifest.SaveDesign(this.Configuration);
            }
            catch (Exception ex)
            {
                throw new AnalysisModelProcessorException("Saving design model failed.", ex);
            }

            return success;
        }

        /// <summary>
        /// Shows all configuration dialog boxes for all interpreters that have to be configured. <see cref="InterpretersToConfiguration"/>
        /// </summary>
        /// <param name="interpreterConfigurations">Map that stores the configurations.</param>
        /// <param name="firstTimeOnly">If true shows the configurations at the first time only, otherwise it shows always.</param>
        public void ShowInterpreterConfigs(
            Dictionary<string, CyPhyGUIs.IInterpreterConfiguration> interpreterConfigurations,
            bool firstTimeOnly = true,
            bool interactive = true)
        {
            if (interpreterConfigurations == null)
            {
                throw new ArgumentNullException("interpreterConfigurations");
            }

            List<ComComponent> configurationFailed = new List<ComComponent>();

            Action<ComComponent> showAndSaveConfiguration = new Action<ComComponent>((interpreter) =>
            {
                var configSuccess = interpreter.DoGUIConfiguration(this.ProjectDirectory, interactive);

                if (configSuccess == false)
                {
                    configurationFailed.Add(interpreter);
                }

                interpreterConfigurations[interpreter.ProgId] = interpreter.InterpreterConfig;
            });

            foreach (var interpreter in this.InterpretersToConfiguration)
            {
                if (interpreterConfigurations.ContainsKey(interpreter.ProgId) == false)
                {
                    showAndSaveConfiguration(interpreter);
                }
                else
                {
                    // save some file IO interactions
                    // (also this will use the configuration that the user defined)
                    // if we would deserialize the config everytime and the user would open
                    // a new project in the same directory there could be problems:
                    // - sharing read/write violations on the configuration files
                    // - invalid configuration would be used by the interpreter
                    if (firstTimeOnly)
                    {
                        interpreter.InterpreterConfig = interpreterConfigurations[interpreter.ProgId];
                    }
                    else
                    {
                        showAndSaveConfiguration(interpreter);
                    }
                }
            }

            foreach (var innerExpanders in this.InnerExpanders)
            {
                innerExpanders.ShowInterpreterConfigs(interpreterConfigurations, firstTimeOnly, interactive: interactive);
            }

            if (configurationFailed.Any())
            {
                string message = string.Format(
                    "Configuration failed or canceled for interpreter(s): {0}",
                    string.Join(", ", configurationFailed.Select(x => x.Name)));

                throw new AnalysisModelInterpreterConfigurationFailedException(message);
            }
        }

        /// <summary>
        /// Runs all interpreters on the expanded context.
        /// </summary>
        /// <param name="passTraceability">If true passes traceability information for the interpreters.</param>
        /// <param name="verboseConsole">Passes value to the interpreter if they can write verbose (debug) messages to the console or not.</param>
        public void RunInterpreters(bool passTraceability, bool verboseConsole)
        {
            this.ThrowIfNotExpanded();

            if (string.IsNullOrWhiteSpace(this.OutputDirectory))
            {
                throw new InvalidOperationException("Call CreateManifest method first to set up output directory.");
            }

            // TODO: review this code
            bool success = true;

            Dictionary<Exception, ComComponent> interpreterFailed = new Dictionary<Exception, ComComponent>();

            this.OnInterpreterProgress(new InterpreterProgressEventArgs()
            {
                Percent = 1,
                Success = success
            });

            var interpreterCount = this.Interpreters.Count;

            if (this.Interpreters.Count == 0)
            {
                success = false;

                this.OnInterpreterProgress(new InterpreterProgressEventArgs()
                {
                    Percent = 100,
                    Success = success
                });

                throw new AnalysisModelInterpreterException("No interpreters were defined.");
            }

            int idx = 0;

            var singleInterpreterRange = interpreterCount / 100;

            // call the interpreters
            foreach (ComComponent interpreter in this.Interpreters)
            {
                this.OnInterpreterProgress(new InterpreterProgressEventArgs()
                {
                    Percent = ((idx * 100) / interpreterCount) + (singleInterpreterRange * 1),
                    Success = success,
                    Interpreter = interpreter
                });

                idx++;

                DateTime startTime = DateTime.Now;

                this.OnInterpreterProgress(new InterpreterProgressEventArgs()
                {
                    Percent = ((idx * 100) / interpreterCount) + (singleInterpreterRange * 25),
                    Success = success,
                    Interpreter = interpreter
                });

                interpreter.Initialize(this.GetExpandedObject().Project);

                interpreter.MainParameters.Project = this.GetExpandedObject().Project;
                interpreter.MainParameters.CurrentFCO = this.GetExpandedObject() as MgaFCO;
                interpreter.MainParameters.SelectedFCOs = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
                interpreter.MainParameters.StartModeParam = 128;
                interpreter.MainParameters.ConsoleMessages = false;
                interpreter.MainParameters.ProjectDirectory = MgaExtensions.MgaExtensions.GetProjectDirectoryPath(this.GetExpandedObject().Project);
                interpreter.MainParameters.OutputDirectory = this.OutputDirectory;
                interpreter.MainParameters.VerboseConsole = verboseConsole;
                interpreter.MainParameters.OriginalCurrentFCOName = OriginalCurrentFCOName;
                if (this.DesignConfiguration != null)
                {
                    interpreter.MainParameters.GeneratedConfigurationModel = this.DesignConfiguration.ParentContainer.Name;
                    interpreter.MainParameters.SelectedConfig = this.DesignConfiguration.Name;
                }

                if (passTraceability)
                {
                    interpreter.MainParameters.Traceability = this.Traceability;
                }

                this.OnInterpreterProgress(new InterpreterProgressEventArgs()
                {
                    Percent = ((idx * 100) / interpreterCount) + (singleInterpreterRange * 50),
                    Success = success,
                    Interpreter = interpreter
                });

                try
                {
                    interpreter.Main();
                }
                catch (InterpreterException ex)
                {
                    // TODO: log error.
                    // Trace.TraceError(ex.ToString());
                    // ErrorWriter.WriteLine(String.Format("{0} failed: {1}", interpreter.Name, ex.Message));
                    success = false;
                    interpreterFailed.Add(ex, interpreter);
                    continue;
                }

                success = success && interpreter.result.Success;

                this.OnInterpreterProgress(new InterpreterProgressEventArgs()
                {
                    Percent = ((idx * 100) / interpreterCount) + (singleInterpreterRange * 75),
                    Success = success,
                    Interpreter = interpreter
                });

                this.OnInterpreterProgress(new InterpreterProgressEventArgs()
                {
                    Percent = ((idx * 100) / interpreterCount) + (singleInterpreterRange * 100),
                    Success = success,
                    Interpreter = interpreter
                });
            }

            if (success == false ||
                interpreterFailed.Any())
            {
                string message = string.Format(
                    "Interpreter(s) failed: {0}",
                    string.Join(", ", interpreterFailed.Select(x => x.Value.Name)));

                // TODO: log all exceptions here to file
                throw new AnalysisModelInterpreterException(message);
            }
            else
            {
                var manifest = AVM.DDP.MetaTBManifest.OpenForUpdate(this.OutputDirectory);
                this.UpdateTestBenchManifestExecutionSteps(manifest);
                manifest.Serialize(this.OutputDirectory);
            }
        }

        /// <summary>
        /// Gets the results directory. Project directory/results/ All interpreters will get a subdirectory inside this one.
        /// </summary>
        /// <returns>Full path to the output directory.</returns>
        public string GetResultsDirectory()
        {
            string projectPath = MgaExtensions.MgaExtensions.GetProjectDirectoryPath(this.GetInvokedObject().Project);
            string outputDir = Path.Combine(projectPath, "results");
            return Path.GetFullPath(outputDir);
        }

        /// <summary>
        /// Gets the temporary folder name for a given object in GME.
        /// </summary>
        /// <param name="originalObject">Object to get the temporary folder name.</param>
        /// <returns>Temporary folder name</returns>
        protected static string GetTemporaryFolderName(IMgaObject originalObject)
        {
            return originalObject.Name + TemporaryFolderSuffix;
        }

        /// <summary>
        /// Gets the temporary object name for a given object (context and configuration pair) in GME.
        /// </summary>
        /// <param name="originalObject">Object to get the temporary object name.</param>
        /// <param name="configuration">Configuration generated from design space.</param>
        /// <returns>Temporary object name</returns>
        protected static string GetTemporaryObjectName(IMgaObject originalObject, IMgaObject configuration)
        {
            return configuration.Name;
        }

        /// <summary>
        /// Checks if the original model is expanded or not.
        /// </summary>
        /// <exception cref="InvalidOperationException">If Expand was not called before.</exception>
        protected void ThrowIfNotExpanded()
        {
            if (this.GetExpandedObject() == null)
            {
                throw new InvalidOperationException("Model is not expanded call Expand");
            }
        }

        /// <summary>
        /// Adds an object ID pair to the traceability map and adds the copied object to the temporary object list keeping track of 
        /// the object to be deleted.
        /// </summary>
        /// <param name="copied">Copy of the original object</param>
        /// <param name="original">Original object</param>
        /// <param name="recursive">Adds traceability recursively, but it does not add to the temporary objects recursively.</param>
        protected void AddToTraceabilityAndTemporary(IMgaObject copied, IMgaObject original, bool recursive = true)
        {
            // save copied object in temporary object.
            this.temporaryObjects.Push(copied);

            this.AddToTraceability(copied, original, recursive);
        }

        /// <summary>
        /// Helper function to execute an action within a transaction.
        /// </summary>
        /// <param name="doWork">Work that has to be done within a transaction.</param>
        /// <param name="flushUndoQueue">Flushes undo queue to keep the memory usage down.</param>
        /// <param name="abort">Aborts the transaction.</param>
        protected void ExecuteInTransaction(Action doWork, bool flushUndoQueue = true, bool abort = false)
        {
            if (doWork == null)
            {
                // nothing to be done
                return;
            }

            var invokedObject = this.GetInvokedObject();
            if (invokedObject == null)
            {
                throw new ArgumentNullException();
            }

            var project = invokedObject.Project;

            bool inTx = (project.ProjectStatus & 8) != 0;
            if (inTx)
            {
                doWork();
                return;
            }

            var terr = project.BeginTransactionInNewTerr(transactiontype_enum.TRANSACTION_NON_NESTED);

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

                if (flushUndoQueue)
                {
                    project.FlushUndoQueue();
                }
            }
            catch (Exception)
            {
                try
                {
                    project.AbortTransaction();

                    // TODO: log exception
                }
                catch
                {
                }

                throw;
            }
        }

        /// <summary>
        /// Called when run interpreters make progress, like get configuration, initialize, etc.
        /// </summary>
        /// <param name="eventArgs">Interpreter event arguments.</param>
        protected virtual void OnInterpreterProgress(InterpreterProgressEventArgs eventArgs)
        {
            EventHandler<InterpreterProgressEventArgs> handler = this.InterpreterProgress;
            if (handler != null)
            {
                handler(this, eventArgs);
            }
        }

        /// <summary>
        /// Ensures that output directory is not null and creates it if it does not exist already.
        /// </summary>
        protected void EnsureOutputDirectory()
        {
            if (string.IsNullOrWhiteSpace(this.OutputDirectory))
            {
                this.OutputDirectory = this.CreateOutputDirectory();
            }
        }

        /// <summary>
        /// Gets a randomly named subdirectory inside a given output directory.
        /// </summary>
        /// <remarks>The new directory will not start with a number.</remarks>
        /// <param name="outputDir">In which directory generate a new subdirectory</param>
        /// <returns>Full absolute path to the newly created directory.</returns>
        private static string GetRandomSubDirectory(string outputDir)
        {
            // TODO: review this code
            string outputSubDir = string.Empty;
            string randomFolderName;
            int maxFolders = 0;

            do
            {
                if (maxFolders++ > 2000000)
                {
                    throw new Exception(
                        string.Format(
                            "Number of tries ({0}) to create an output folder exceeded in {0}. ",
                            maxFolders,
                            outputDir));
                }

                randomFolderName = "r" + DateTime.Now.ToString("yyyy-MM-dd--HH-mm-ss") + "_" +
                    Path.GetFileNameWithoutExtension(Path.GetRandomFileName());

                outputSubDir = Path.Combine(outputDir, randomFolderName);

            } while (File.Exists(outputSubDir) ||
                Directory.Exists(outputSubDir));

            return Path.GetFullPath(outputSubDir);
        }

        /// <summary>
        /// Adds an object ID pair to the traceability map.
        /// </summary>
        /// <param name="copied">Copy of the original object</param>
        /// <param name="original">Original object</param>
        /// <param name="recursive">Adds traceability recursively</param>
        private void AddToTraceability(IMgaObject copied, IMgaObject original, bool recursive = true)
        {
            this.traceability.AddItem(copied.ID, original.ID);

            if (recursive)
            {
                // recursive
                IEnumerable<MgaObject> copiedChildren = null;

                if (copied is MgaModel && original is MgaModel)
                {
                    copiedChildren = (copied as MgaModel).ChildObjects.Cast<MgaObject>();
                }
                else if (copied is MgaFolder && original is MgaFolder)
                {
                    copiedChildren = (copied as MgaFolder).ChildObjects.Cast<MgaObject>();
                }

                if (copiedChildren != null)
                {
                    // container model or folder
                    // find pair of child objects (do NOT follow references!)
                    foreach (var copiedChild in copiedChildren)
                    {
                        long relID = copiedChild.RelID;

                        // FIXME: this is NOT the correct implementation below! We only use copies.
                        /*if (copiedChild is MgaFCO)
                        {
                            bool derived = (copiedChild as MgaFCO).IsInstance || (copiedChild as MgaFCO).ArcheType != null;
                            if (derived)
                            {
                                relID = relID | (RELID_BASE_MAX + 1);
                            }
                            else
                            {
                                relID = relID & ~(RELID_BASE_MAX + 1);
                            }
                        }*/

                        var originalChild = original.get_ChildObjectByRelID((int)relID);
                        if (originalChild != null)
                        {
                            if (copied == copiedChild ||
                                original == originalChild)
                            {
                                // we would run into an infinite loop avoiding it here
                            }
                            else
                            {
                                this.AddToTraceability(copiedChild, originalChild, recursive);
                            }
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Create the output directory if it does not exist yet.
        /// </summary>
        /// <returns>Full path to the created directory</returns>
        private string CreateOutputDirectory()
        {
            string outputSubDir = string.Empty;
            string projectPath = MgaExtensions.MgaExtensions.GetProjectDirectoryPath(this.GetInvokedObject().Project);
            string outputDir = this.GetResultsDirectory();

            Directory.CreateDirectory(outputDir);

            // output dir %root%/%currentobjInvoked%/%this object%/%interpreter name%/
            // We need shorter path due windows' path limitations
            outputSubDir = GetRandomSubDirectory(outputDir);

            // set up base directory
            Directory.CreateDirectory(outputSubDir);

            return Path.GetFullPath(outputSubDir);
        }
    }
}
