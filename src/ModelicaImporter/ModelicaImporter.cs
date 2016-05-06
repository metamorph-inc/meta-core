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

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using System.Diagnostics;
using System.Windows.Forms;


namespace ModelicaImporter
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class ModelicaImporterInterpreter : IMgaComponentEx, IGMEVersionInfo
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
            // TODO: Add your initialization code here...
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
            // Check if OpenModelica is installed properly
            if (string.IsNullOrWhiteSpace(Environment.GetEnvironmentVariable("OPENMODELICAHOME")))
            {
                this.Logger.WriteFailed(
                    "OpenModelica is probably not installed. OPENMODELICAHOME environment variable needs to be set. <a href=\"https://openmodelica.org\">https://openmodelica.org</a>");

                return;
            }

            // nothing is open, import multiple components to folder 'ModelicaImports'
            if (currentobj == null)
            {
                MessageBox.Show(
                    "New Components/TestComponents will be created in the folder 'ModelicaImports'.",
                    "For Your Information",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Information);

                this.Logger.WriteInfo("New Components/TestComponents will be created in the folder 'ModelicaImports'.");

                // call ModelicaModelPicker with selectMultiple = true
                using (ModelicaModelPicker modelpicker = new ModelicaModelPicker(true, project, this.Logger))
                {
                    var dialogResult = modelpicker.ShowDialog();
                    if (dialogResult != System.Windows.Forms.DialogResult.OK)
                    {
                        this.Logger.WriteInfo("Modelica import was cancelled by the user.");
                        return;
                    }
                }
            }
            else
            {
                if ((currentobj.Meta.Name != typeof(CyPhy.Component).Name &&
                    currentobj.Meta.Name != typeof(CyPhy.TestComponent).Name))
                {
                    this.Logger.WriteError("Please open a Component or TestComponent and try again.");
                    return;
                }

                if (currentobj.Meta.Name == typeof(CyPhy.TestComponent).Name)
                {
                    var childObjects = currentobj.ChildObjects;

                    foreach (MgaObject child in childObjects)
                    {
                        if (child.MetaBase.Name == "ModelicaModel")
                        {
                            this.Logger.WriteError("TestComponent {0} already has a ModelicaModel. Only one is allowed.", currentobj.Name);
                            return;
                        }
                    }
                }

                CyPhy.ComponentType cyphyComponent = null;
                if (currentobj.Meta.Name == typeof(CyPhy.Component).Name)
                {
                    cyphyComponent = CyPhyClasses.Component.Cast(currentobj);
                }
                else if (currentobj.Meta.Name == typeof(CyPhy.TestComponent).Name)
                {
                    cyphyComponent = CyPhyClasses.TestComponent.Cast(currentobj);
                }

                // per META-2674
                if (cyphyComponent.IsLib)
                {
                    this.Logger.WriteError("Cannot modify a model in an attached Library; please select a valid Component or TestComponent.");
                    return;
                }

                // per META-2673
                if (cyphyComponent.IsInstance)
                {
                    this.Logger.WriteError("Cannot modify an Instance; please select a non-Instance Component or TestComponent.");
                    return;
                }

                string message = string.Format("All selected Modelica models will be added to {0}.", currentobj.Name);

                MessageBox.Show(
                    message,
                    "For Your Information",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Information);

                this.Logger.WriteInfo(message);

                // call ModelicaModelPicker with selectMultiple = false
                using (ModelicaModelPicker modelpicker = new ModelicaModelPicker(false, project, this.Logger, cyphyComponent))
                {
                    var dialogResult = modelpicker.ShowDialog();
                    if (dialogResult != System.Windows.Forms.DialogResult.OK)
                    {
                        this.Logger.WriteInfo("Modelica import was cancelled by the user.");
                        return;
                    }
                }
            }
        }

        #region IMgaComponentEx Members

        MgaGateway MgaGateway { get; set; }
        private CyPhyGUIs.GMELogger Logger { get; set; }

        public void InvokeEx(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, int param)
        {
            if (!enabled)
            {
                return;
            }

            try
            {
                this.Logger = new CyPhyGUIs.GMELogger(project, this.ComponentName);

                MgaGateway = new MgaGateway(project);

                MgaGateway.PerformInTransaction(delegate
                {
                    Main(project, currentobj, selectedobjs, Convert(param));
                }, abort: false);
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
