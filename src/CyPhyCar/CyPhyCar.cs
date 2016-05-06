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
// ICyPhyInterpreter and DSML
using CyPhyGUIs;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhyCar
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyPhyCarInterpreter : IMgaComponentEx, IGMEVersionInfo, ICyPhyInterpreter
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
        /// Don't perform MGA operations here unless you open a transaction.
        /// </summary>
        /// <param name="project">The handle of the project opened in GME, for which the interpreter was called.</param>
        public void Initialize(MgaProject project)
        {
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
        [ComVisible(false)]
        public void Main(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, ComponentStartMode startMode)
        {


        }

        #region CyPhyGUIs ICyPhyInterpreter

        /// <summary>
        /// ProgId of the configuration class of this interpreter.
        /// </summary>
        public string InterpreterConfigurationProgId
        {
            get
            {
                return (typeof(CyPhyCarSettings).GetCustomAttributes(typeof(ProgIdAttribute), false)[0] as ProgIdAttribute).Value;
            }
        }

        /// <summary>
        /// Called once per MasterInterpreter run (or equivalent)
        /// </summary>
        /// <param name="parameters"></param>
        /// <returns></returns>
        public IInterpreterPreConfiguration PreConfig(IPreConfigParameters parameters)
        {
            return null;
        }

        /// <summary>
        /// Called once only if running in interactive (non-automated) mode. Interpreters should not access the model during this method.
        /// </summary>
        /// <returns>null iff the user selects Cancel in the dialog</returns>
        public IInterpreterConfiguration DoGUIConfiguration(IInterpreterPreConfiguration preConfig, IInterpreterConfiguration previousConfig)
        {
            return new CyPhyCarSettings();
        }

        public IInterpreterResult Main(IInterpreterMainParameters parameters)
        {
            var result = new InterpreterResult();
            result.RunCommand = "runCarSim.bat";
            try
            {

                MgaGateway.PerformInTransaction(delegate
                {
                    if (parameters.CurrentFCO.MetaBase.Name == "CarTestBench")
                    {
                        try
                        {
                            var elaborator = new CyPhyElaborateCS.CyPhyElaborateCSInterpreter();
                            elaborator.Logger = new GMELogger(parameters.Project);
                            //elaborator.Logger.AddWriter(Logger.Instance);
                            var elaboratorresult = elaborator.RunInTransaction(parameters.Project, parameters.CurrentFCO, parameters.SelectedFCOs, parameters.StartModeParam);
                            if (elaboratorresult == false)
                            {
                                throw new ApplicationException("see elaborator log");
                            }

                        }
                        catch (Exception e)
                        {
                            //Logger.Instance.AddLogMessage("Elaborator exception occurred: " + e.Message, Severity.Error);
                            throw new Exception(e.Message);
                        }
                        var testBench = CyPhyClasses.CarTestBench.Cast(parameters.CurrentFCO);

                        TestBenchProcessor processor = new TestBenchProcessor() { GMEConsole = GMEConsole, OutputDir = parameters.OutputDirectory, ProjectDir = parameters.ProjectDirectory };
                        processor.Process(testBench);
                    }
                    else
                    {
                        GMEConsole.Error.WriteLine("Not a Car Test Bench.");
                        return;
                    }
                },
                transactiontype_enum.TRANSACTION_NON_NESTED, abort: true);
                result.Success = true;
            }
            catch (Exception ex)
            {
                GMEConsole.Error.WriteLine("Exception was raised: {0}", ex.ToString());
                result.Success = false;
            }
            finally
            {
//                parameters.Project.AbortTransaction();
                MgaGateway = null;
                GC.Collect();
                GC.WaitForPendingFinalizers();
            }

            return result;
        }

        #endregion


        #region IMgaComponentEx Members

        MgaGateway MgaGateway { get; set; }
        GMEConsole GMEConsole { get; set; }

        public void InvokeEx(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, int param)
        {
            if (!enabled)
            {
                return;
            }

            try
            {
                if (currentobj == null)
                {
                    GMEConsole.Error.WriteLine("No model opened.");
                    return;
                }

                var parameters = new InterpreterMainParameters()
                {
                    Project = project,
                    CurrentFCO = currentobj,
                    SelectedFCOs = selectedobjs,
                    VerboseConsole = true
                };

                parameters.ProjectDirectory = Path.GetDirectoryName(currentobj.Project.ProjectConnStr.Substring("MGA=".Length));

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
                });

                PreConfigArgs preConfigArgs = new PreConfigArgs();
                preConfigArgs.ProjectDirectory = parameters.ProjectDirectory;

                // call the preconfiguration with no parameters and get preconfig
                var preConfig = this.PreConfig(preConfigArgs);

                // get previous GUI config
                var previousConfig = META.ComComponent.DeserializeConfiguration(parameters.ProjectDirectory,
                    typeof(CyPhyCarSettings),
                    this.ComponentProgID);

                // get interpreter config through GUI
                var config = this.DoGUIConfiguration(preConfig, previousConfig);

                // if config is valid save it and update it on the file system
                META.ComComponent.SerializeConfiguration(parameters.ProjectDirectory, config, this.ComponentProgID);

                // assign the new configuration to mainParameters
                parameters.config = config;


                // call the main (ICyPhyComponent) function
                this.Main(parameters);


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


    }
}
