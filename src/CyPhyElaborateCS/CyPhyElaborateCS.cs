namespace CyPhyElaborateCS
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Runtime.InteropServices;
    using CyPhyGUIs;
    using GME.CSharp;
    using GME.MGA;
    using GME.MGA.Core;

    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.Guid),
    ProgId(ComponentConfig.ProgID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyPhyElaborateCSInterpreter : IMgaComponentEx, IGMEVersionInfo, CyPhyGUIs.ICyPhyInterpreter
    {
        /// <summary>
        /// Contains information about the GUI event that initiated the invocation.
        /// </summary>
        public enum ComponentStartMode
        {
            /// <summary>
            /// Not used by GME
            /// </summary>
            GME_MAIN_START = 0,

            /// <summary>
            /// Right click in the GME Tree Browser window
            /// </summary>
            GME_BROWSER_START = 1,

            /// <summary>
            /// Using the context menu by right clicking a model element in the GME modeling window
            /// </summary>
            GME_CONTEXT_START = 2,

            /// <summary>
            /// Not used by GME
            /// </summary>
            GME_EMBEDDED_START = 3,

            /// <summary>
            /// Clicking on the toolbar icon, or using the main menu
            /// </summary>
            GME_MENU_START = 16,

            /// <summary>
            /// Using the context menu by right clicking the background of the GME modeling window
            /// </summary>
            GME_BGCONTEXT_START = 18,

            /// <summary>
            /// Not used by GME
            /// </summary>
            GME_ICON_START = 32,

            /// <summary>
            /// Not used by GME, available to testers not using GME
            /// </summary>
            GME_SILENT_MODE = 128
        }

        /// <summary>
        /// This function is called for each interpreter invocation before Main.
        /// Don't perform MGA operations here unless you open a transaction.
        /// </summary>
        /// <param name="project">The handle of the project opened in GME, for which the interpreter was called.</param>
        public void Initialize(MgaProject project)
        {
            // initialize collections
            if (this.componentParameters == null)
            {
                // this is not a constructor, if somebody writes the parameters before calling the initialize
                // then we do not want to overwrite their saved results. Therefore initialize the collection only
                // if it is not initialized already.
                this.componentParameters = new SortedDictionary<string, object>();
            }
        }

        public Boolean UnrollConnectors = false;
        string[] numericLeafNodes;

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
        /// <returns>True if run was successful otherwise false.</returns>
        /// <exception cref="ElaboratorException">If elaborator fails.</exception>
        [ComVisible(false)]
        public bool Main(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, ComponentStartMode startMode)
        {
            this.Logger.WriteInfo("CyPhyElaborate 2.0 started.");
            System.Windows.Forms.Application.DoEvents();

            bool success = this.Check(currentobj);
            if (success == false)
            {
                this.Logger.WriteError("CyPhyElaborate 2.0 context is invalid.");
                return success;
            }

            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();
            try
            {
                this.ElaborateContext(currentobj);
            }
            catch (ElaboratorException ex)
            {
                this.Logger.WriteError(ex.Message);
                this.Logger.WriteError("CyPhyElaborate 2.0 finished with errors.");
                System.Windows.Forms.Application.DoEvents();

                // make sure we abort the transaction
                throw ex;
            }
            catch (NotSupportedException ex)
            {
                this.Logger.WriteError(ex.Message);
                this.Logger.WriteError("CyPhyElaborate 2.0 finished with errors.");
                System.Windows.Forms.Application.DoEvents();

                throw ex;
            }

            sw.Stop();
            this.Logger.WriteDebug("Time: {0}", sw.Elapsed.ToString("c"));

            sw.Restart();
            this.Logger.WriteInfo("CyPhyFormulaEvaluator 1.0 started");
            try
            {
                // create formula evaluator type
                // FIXME: calling the elaborator is faster than calling the formula evaluator
                Type typeFormulaEval = Type.GetTypeFromProgID("MGA.Interpreter.CyPhyFormulaEvaluator");
                IMgaComponentEx formulaEval = Activator.CreateInstance(typeFormulaEval) as IMgaComponentEx;

                // empty selected object set
                Type typeMgaFCOs = Type.GetTypeFromProgID("Mga.MgaFCOs");
                MgaFCOs selectedObjs = Activator.CreateInstance(typeMgaFCOs) as MgaFCOs;

                // initialize formula evauator
                formulaEval.Initialize(project);

                // automation means no UI element shall be shown by the interpreter
                formulaEval.ComponentParameter["automation"] = "true";

                // do not write to the console
                formulaEval.ComponentParameter["console_messages"] = "off";

                // do not expand nor collapse the model
                formulaEval.ComponentParameter["expanded"] = "true";

                // do not generate the post processing python scripts
                // FIXME: Why should we generate them ???
                formulaEval.ComponentParameter["do_not_generate_post_processing"] = "true";

                // call the formula evaluator and update all parameters starting from the current object
                try
                {
                    formulaEval.InvokeEx(project, currentobj, selectedObjs, 128);
                    numericLeafNodes = (string[])formulaEval.ComponentParameter["numericLeafNodes"];
                    this.Logger.WriteInfo("CyPhyFormulaEvaluator 1.0 finished");
                }
                catch (COMException e)
                {
                    success = false;
                    this.Logger.WriteError(e.Message);
                    this.Logger.WriteError("CyPhyFormulaEvaluator 1.0 finished with errors");
                }
            }
            catch (Exception ex)
            {
                this.Logger.WriteInfo("CyPhyFormulaEvaluator 1.0 failed");
                this.Logger.WriteDebug(ex.ToString());
            }

            sw.Stop();
            this.Logger.WriteDebug("Formula evaluator runtime: {0}", sw.Elapsed.ToString("c"));


            if (UnrollConnectors)
            {
                sw.Restart();
                this.Logger.WriteInfo("ConnectorUnroller started");
                try
                {
                    var kindCurrentObj = currentobj.MetaBase.Name;
                    if (kindCurrentObj == "ComponentAssembly")
                    {
                        using (Unroller unroller = new Unroller(currentobj.Project, Traceability, Logger))
                        {
                            unroller.UnrollComponentAssembly(currentobj as MgaModel);
                        }
                    }
                    else if (kindCurrentObj == "TestBench")
                    {
                        using (Unroller unroller = new Unroller(currentobj.Project, Traceability, Logger))
                        {
                            unroller.UnrollTestBench(currentobj as MgaModel);
                        }
                    }

                    this.Logger.WriteInfo("ConnectorUnroller finished");
                }
                catch (Exception ex)
                {
                    this.Logger.WriteError("ConnectorUnroller failed. Check log for details.");
                    this.Logger.WriteDebug(ex.ToString());
                    success = false;
                }
                sw.Stop();
                this.Logger.WriteDebug("ConnectorUnroller runtime: {0}", sw.Elapsed.ToString("c"));
            }

            this.Logger.WriteInfo("CyPhyElaborate 2.0 finished.");
            System.Windows.Forms.Application.DoEvents();

            return success;
        }

        #region Temporary Testing Code
        private void CallOldElaborator(MgaProject project, MgaFCO currentobj)
        {
            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();

            this.Logger.WriteInfo("CyPhyElaborator 1.0 started");
            try
            {
                Type typeElaborator = Type.GetTypeFromProgID("MGA.Interpreter.CyPhyElaborate");
                IMgaComponentEx oldElaborator = Activator.CreateInstance(typeElaborator) as IMgaComponentEx;

                Type typeMgaFCOs = Type.GetTypeFromProgID("Mga.MgaFCOs");
                MgaFCOs selectedObjs = Activator.CreateInstance(typeMgaFCOs) as MgaFCOs;

                oldElaborator.Initialize(project);
                oldElaborator.ComponentParameter["automation"] = "true";
                oldElaborator.ComponentParameter["console_messages"] = "off";
                oldElaborator.ComponentParameter["expanded"] = "true";

                sw.Start();
                oldElaborator.InvokeEx(project, currentobj, selectedObjs, 128);
                this.Logger.WriteInfo("CyPhyElaborator 1.0 finished");
            }
            catch (Exception ex)
            {
                this.Logger.WriteInfo("CyPhyElaborator 1.0 failed");
                this.Logger.WriteDebug(ex.ToString());
            }
            finally
            {
                sw.Stop();
                this.Logger.WriteDebug("CyPhyElaborator 1.0 runtime: {0}", sw.Elapsed.ToString("c"));
            }
        }

        private static MgaFCO GenerateTestModel(MgaProject project, MgaFCO currentobj)
        {
            MgaFolder folderComponentAssemblies = project.RootFolder.CreateFolder(project.RootMeta.RootFolder.DefinedFolderByName["ComponentAssemblies", true]);
            folderComponentAssemblies.Name = "00 ComponentAssemblies";

            MgaModel ca = folderComponentAssemblies.CreateRootObject(project.RootMeta.RootFolder.DefinedFCOByName["ComponentAssembly", true]) as MgaModel;
            ca.Name = string.Format("00 Generated Component Assembly {0}", DateTime.Now);

            MgaFolder folderComponents = project.RootFolder.CreateFolder(project.RootMeta.RootFolder.DefinedFolderByName["Components", true]);
            folderComponents.Name = "00 Components";

            MgaModel leaf = folderComponents.CreateRootObject(project.RootMeta.RootFolder.DefinedFCOByName["Component", true]) as MgaModel;
            leaf.Name = string.Format("00 Generated Component {0}", DateTime.Now);

            // generate syntetic model
            GenerateAssemblies(folderComponentAssemblies, ca, 0, leaf);

            currentobj = ca as MgaFCO;
            return currentobj;
        }

        private static void GenerateAssemblies(MgaFolder componentAssemblyFolder, MgaModel assembly, int level, MgaModel leaf)
        {
            var elementsOnEachLevel = 2;
            var numberOfLevels = 6;

            if (level <= numberOfLevels)
            {
                System.Console.Out.WriteLine(level);

                MgaFolder componentAssemblyChildFolder = componentAssemblyFolder.CreateFolder(componentAssemblyFolder.Project.RootMeta.RootFolder.DefinedFolderByName["ComponentAssemblies", true]);

                componentAssemblyChildFolder.Name = string.Format("ComponentAssembly Child folder {0}", level);

                for (int i = 0; i < elementsOnEachLevel; i++)
                {
                    var childAssembly = componentAssemblyChildFolder.CreateRootObject(assembly.Meta) as MgaModel;
                    childAssembly.Name = string.Format("ChildAssembly {0} {1}", level, i);

                    var cref = assembly.CreateChildObject((assembly.Meta as GME.MGA.Meta.MgaMetaModel).RoleByName["ComponentRef"]) as MgaReference;
                    cref.Name = string.Format("ComponentRef {0} {1}", level, i);

                    if (level < numberOfLevels)
                    {
                        cref.Referred = childAssembly as MgaFCO;
                        GenerateAssemblies(componentAssemblyFolder, childAssembly, level + 1, leaf);
                    }
                    else
                    {
                        cref.Referred = leaf as MgaFCO;
                    }
                }
            }
        }
        #endregion

        private bool Check(MgaFCO currentobj)
        {
            if (currentobj == null)
            {
                this.Logger.WriteError("Must execute it from a Test bench or Component Assembly.");
                return false;
            }

            if ((currentobj is MgaModel) == false)
            {
                this.Logger.WriteError("Must execute it from a Test bench or Component Assembly.");
                return false;
            }

            //if (currentobj.MetaBase.Name != "TestBench" &&
            //    currentobj.MetaBase.Name != "CADTestBench" &&
            //    currentobj.MetaBase.Name != "BlastTestBench" &&
            //    currentobj.MetaBase.Name != "BallisticTestBench" &&
            //    currentobj.MetaBase.Name != "CFDTestBench" &&
            //    currentobj.MetaBase.Name != "ComponentAssembly")
            //{
            //    this.Logger.WriteError("Must execute it from a Test bench or Component Assembly.");
            //    return false;
            //}

            //foreach (MgaReference tlsut in (currentobj as MgaModel).GetChildrenOfKind("TopLevelSystemUnderTest"))
            //{
            //    var referedTLSUT = tlsut.Referred;
            //    if (referedTLSUT != null && referedTLSUT.MetaBase.Name == "DesignContainer")
            //    {

            //    }
            //}
            // TODO: is readonly
            // TODO: is lib element
            // TODO: is instance
            // TODO: is subtype

            //// check everything else on demand

            return true;
        }

        /// <summary>
        /// Runs the elaborator based on a given context.
        /// </summary>
        /// <param name="currentobj">Current context on which the elaborator should run.</param>
        /// <returns>True if the elaboration was successful</returns>
        /// <exception cref="ElaboratorException">If any problem occurred during elaboration.</exception>
        public bool ElaborateContext(MgaFCO currentobj)
        {
            // initialize traceability map with a new instance
            this.Traceability = new META.MgaTraceability();

            // expose traceability for other interpreters
            this.set_ComponentParameter("traceability", this.Traceability);

            // count all descendant objects before elaboration
            var beforeElaboration = (currentobj as MgaModel).GetDescendantFCOs(currentobj.Project.CreateFilter()).Count;

            // get an elaborator for the current context
            var elaborator = Elaborator.GetElaborator(currentobj as MgaModel, this.Logger, UnrollConnectors);

            // elaborate the entire model starting from the current object
            elaborator.Elaborate();

            // update traceability for this object.
            // TODO: would be nice to have a function on MgaTraceability class like update map and we can pass a dictionary.
            foreach (var item in elaborator.Traceability)
            {
                this.Traceability.AddItem(item.Key, item.Value);
            }

            // statistics about the traceability
            this.Logger.WriteDebug("Traceability map size: {0}", elaborator.Traceability.Count);

            // count all descendant objects after elaboration
            var afterElaboration = (currentobj as MgaModel).GetDescendantFCOs(currentobj.Project.CreateFilter()).Count;
            this.Logger.WriteDebug("Before: {0}, After: {1}", beforeElaboration, afterElaboration);

            // verify traceability
            if (afterElaboration == elaborator.Traceability.Count)
            {
                this.Logger.WriteDebug("Traceability map has traces for all objects within the current context.");
            }
            else
            {
                this.Logger.WriteWarning("Traceability map does not have traces for all objects. {0} are missing in the map.", elaborator.Traceability.Count - afterElaboration);
            }

            return true;
        }

        public CyPhyCOMInterfaces.IMgaTraceability Traceability { get; set; }

        [ComVisible(false)]
        public CyPhyGUIs.GMELogger Logger { get; set; }

        #region IMgaComponentEx Members

        private MgaGateway MgaGateway { get; set; }

        public void InvokeEx(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, int param)
        {
            if (!this.enabled)
            {
                return;
            }

            this.RunInTransaction(project, currentobj, selectedobjs, param);
        }

        public bool RunInTransaction(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, int param, string outputDirectory=null)
        {
            bool success = false;
            bool shouldDisposeLogger = false;

            try
            {
                MgaGateway = new MgaGateway(project);

                // populate a test model
                /*MgaGateway.PerformInTransaction(delegate
                {
                    currentobj = GenerateTestModel(project, currentobj);
                }, transactiontype_enum.TRANSACTION_NON_NESTED);*/

                if (this.Logger == null)
                {
                    this.Logger = new CyPhyGUIs.GMELogger(project, this.ComponentName);
                    this.Logger.LoggingLevel = this.Logger.GMEConsoleLoggingLevel = this.Convert(param) == ComponentStartMode.GME_SILENT_MODE ?
                        CyPhyGUIs.SmartLogger.MessageType_enum.Warning :
                        CyPhyGUIs.SmartLogger.MessageType_enum.Info;
                    shouldDisposeLogger = true;
                }

                HashSet<string> m_addonNames = new HashSet<string>()
                {
                    "ConstraintManager".ToLowerInvariant(),
                    "CyPhyAddOn".ToLowerInvariant(),
                    "CyPhyDecoratorAddon".ToLowerInvariant(),
                    "CyPhyMdaoAddOn".ToLowerInvariant(),
                    "CyPhySignalBlocksAddOnAddon".ToLowerInvariant(),
                    "ComponentLibraryManagerAddOn".ToLowerInvariant()
                };

                // turn off all addons specified above
                var addons = project.AddOnComponents.Cast<IMgaComponentEx>();
                foreach (var addon in addons)
                {
                    if (addon.ComponentName.ToLowerInvariant() == "CyPhyAddOn".ToLowerInvariant())
                    {
                        addon.ComponentParameter["DontAssignGuidsOnNextTransaction".ToLowerInvariant()] = true;
                    }

                    if (m_addonNames.Contains(addon.ComponentName.ToLowerInvariant()))
                    {
                        try
                        {
                            addon.Enable(false);
                        }
                        catch (Exception ex)
                        {
                            // if one fails keep trying the other ones.
                            this.Logger.WriteError("Turing off addon failed: {0} --> {1}", addon.ComponentName, ex.ToString());
                        }
                    }
                }

                try
                {
                    MgaGateway.PerformInTransaction(delegate
                    {
                        success = this.Main(project, currentobj, selectedobjs, this.Convert(param));

                        if (String.IsNullOrEmpty(outputDirectory) == false)
                        {
                            UpdateMetricsInTestbenchManifest(currentobj, outputDirectory);
                        }
                    },
                    transactiontype_enum.TRANSACTION_NON_NESTED);

                    // call old elaborator
                    ////this.CallOldElaborator(project, currentobj);
                }
                catch (Exception ex)
                {
                    this.Logger.WriteDebug(ex.ToString());
                    success = false;
                }
                finally
                {
                    foreach (var addon in addons)
                    {
                        if (m_addonNames.Contains(addon.ComponentName.ToLowerInvariant()))
                        {
                            try
                            {
                                addon.Enable(true);
                            }
                            catch (Exception ex)
                            {
                                // if one fails keep trying the other ones.
                                this.Logger.WriteError("Turing off addon failed: {0} --> {1}", addon.ComponentName, ex.ToString());
                            }
                        }
                    }
                }
            }
            finally
            {
                if (shouldDisposeLogger &&
                    this.Logger != null)
                {
                    this.Logger.Dispose();
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

            return success;
        }

        public void UpdateMetricsInTestbenchManifest(MgaFCO currentobj, string outputDirectory)
        {
            var tbManifest = AVM.DDP.MetaTBManifest.OpenForUpdate(outputDirectory);
            Dictionary<string, AVM.DDP.MetaTBManifest.Metric> metrics = tbManifest.Metrics.ToDictionary(metric => metric.Name);
            foreach (MgaFCO metricFco in ((MgaModel)currentobj).ChildFCOs.Cast<MgaFCO>()
                .Where(fco => fco.Meta.Name == "Metric"))
            {
                AVM.DDP.MetaTBManifest.Metric metric;
                if (metrics.TryGetValue(metricFco.Name, out metric))
                {
                    if (numericLeafNodes.Contains(metricFco.Name))
                    {
                        metric.Value = Double.Parse(metricFco.GetStrAttrByNameDisp("Value"));
                    }
                    else
                    {
                        metric.Value = metricFco.GetStrAttrByNameDisp("Value");
                    }
                }
            }

            tbManifest.Serialize(outputDirectory);
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

        /// <summary>
        /// Gets component name
        /// </summary>
        public string ComponentName
        {
            get { return GetType().Name; }
        }

        /// <summary>
        /// Gets the Programmatic Identifier of the component.
        /// </summary>
        public string ComponentProgID
        {
            get
            {
                return ComponentConfig.ProgID;
            }
        }

        /// <summary>
        /// Gets the type of the component, e.g. interpreter or AddOn.
        /// </summary>
        public componenttype_enum ComponentType
        {
            get { return ComponentConfig.ComponentType; }
        }

        /// <summary>
        /// Gets the paradigm name to which the interpreter is registered to.
        /// </summary>
        public string Paradigm
        {
            get { return ComponentConfig.ParadigmName; }
        }
        #endregion

        #region Enabling
        /// <summary>
        /// True if component enabled.
        /// </summary>
        private bool enabled = true;

        /// <summary>
        /// Enables or disables this component
        /// </summary>
        /// <param name="newval">True enables the component, false disables.</param>
        public void Enable(bool newval)
        {
            this.enabled = newval;
        }
        #endregion

        #region Interactive Mode
        /// <summary>
        /// Indicates whether the component is running in interactive mode or not.
        /// </summary>
        protected bool interactiveMode = true;

        /// <summary>
        /// Gets or sets a value indicating whether UI elements may show up and require user interactions.
        /// </summary>
        public bool InteractiveMode
        {
            get
            {
                return this.interactiveMode;
            }

            set
            {
                this.interactiveMode = value;
            }
        }
        #endregion

        #region Custom Parameters
        private SortedDictionary<string, object> componentParameters = null;
        private IInterpreterMainParameters parameters;

        /// <summary>
        /// Gets a parameter by name.
        /// </summary>
        /// <param name="Name">Name of the parameter</param>
        /// <returns>Value of the parameter if found, otherwise null.</returns>
        public object get_ComponentParameter(string Name)
        {
            if (Name == "type")
            {
                return "csharp";
            }

            if (Name == "path")
            {
                return GetType().Assembly.Location;
            }

            if (Name == "fullname")
            {
                return GetType().FullName;
            }

            object value;
            if (this.componentParameters != null &&
                this.componentParameters.TryGetValue(Name, out value))
            {
                return value;
            }

            return null;
        }

        /// <summary>
        /// Sets a component parameter by name.
        /// </summary>
        /// <param name="Name">Parameter name</param>
        /// <param name="pVal">Parameter value</param>
        public void set_ComponentParameter(string Name, object pVal)
        {
            if (this.componentParameters == null)
            {
                this.componentParameters = new SortedDictionary<string, object>();
            }

            this.componentParameters[Name] = pVal;
        }

        #endregion

        #region Unused Methods
        /// <summary>
        /// Old interface, it is never called for <see cref="MgaComponentEx"/> interfaces
        /// </summary>
        /// <param name="Project">The handle of the project opened in GME, for which the interpreter was called.</param>
        /// <param name="selectedobjs">
        /// A collection for the selected model elements. It is never null.
        /// If the interpreter is invoked by the context menu of the GME Tree Browser, then the selected items in the tree browser. Folders
        /// are never passed (they are not FCOs).
        /// If the interpreter is invoked by clicking on the toolbar icon or the context menu of the modeling window, then the selected items 
        /// in the active GME modeling window. If nothing is selected, the collection is empty (contains zero elements).
        /// </param>
        /// <param name="param">Contains information about the GUI event that initiated the invocation.</param>
        public void Invoke(MgaProject Project, MgaFCOs selectedobjs, int param)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Not used by GME
        /// </summary>
        /// <param name="Project">The handle of the project opened in GME, for which the interpreter was called.</param>
        /// <param name="currentobj">The model open in the active tab in GME. Its value is null if no model is open (no GME modeling windows open). </param>
        /// <param name="selectedobjs">
        /// A collection for the selected model elements. It is never null.
        /// If the interpreter is invoked by the context menu of the GME Tree Browser, then the selected items in the tree browser. Folders
        /// are never passed (they are not FCOs).
        /// If the interpreter is invoked by clicking on the toolbar icon or the context menu of the modeling window, then the selected items 
        /// in the active GME modeling window. If nothing is selected, the collection is empty (contains zero elements).
        /// </param>
        /// <param name="param">Contains information about the GUI event that initiated the invocation.</param>
        public void ObjectsInvokeEx(MgaProject Project, MgaObject currentobj, MgaObjects selectedobjs, int param)
        {
            throw new NotImplementedException();
        }

        #endregion
        #endregion

        #region IMgaVersionInfo Members

        /// <summary>
        /// Gets the GME interface version.
        /// </summary>
        public GMEInterfaceVersion_enum version
        {
            get { return GMEInterfaceVersion_enum.GMEInterfaceVersion_Current; }
        }

        public string InterpreterConfigurationProgId
        {
            get
            {
                return null;
            }
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

        public IInterpreterPreConfiguration PreConfig(IPreConfigParameters parameters)
        {
            return null;
        }

        public IInterpreterConfiguration DoGUIConfiguration(IInterpreterPreConfiguration preConfig, IInterpreterConfiguration previousConfig)
        {
            return null;
        }

        public IInterpreterResult Main(IInterpreterMainParameters parameters)
        {
            var result = new InterpreterResult();
            this.parameters = parameters;
            result.Success = this.RunInTransaction(parameters.CurrentFCO.Project, parameters.CurrentFCO, parameters.SelectedFCOs, 128, parameters.OutputDirectory);
            result.RunCommand = "cmd.exe /c \"\"";

            return result;
        }

        #endregion
    }
}
