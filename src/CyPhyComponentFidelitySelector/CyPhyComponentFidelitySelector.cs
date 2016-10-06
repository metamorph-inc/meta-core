using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Linq;
using GME.CSharp;
using GME;
using GME.MGA;
using GME.MGA.Core;

namespace CyPhyComponentFidelitySelector
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyPhyComponentFidelitySelectorInterpreter : IMgaComponentEx, IGMEVersionInfo
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
            if (currentobj == null)
            {
                GMEConsole.Error.WriteLine("Please open a TestBench to run {0}.", ComponentName);
                return;
            }
            else if (currentobj.Meta.Name != "TestBench")
            {
                GMEConsole.Error.WriteLine("Please open a TestBench to run {0}.", ComponentName);
                return;
            }

            GMEConsole.Info.WriteLine("Running {0} on TestBench '{1}'", ComponentName, currentobj.Name);

            // get root folder
            IMgaFolder rootFolder = project.RootFolder;
            
            // create a filter for components
            MgaFilter filter = project.CreateFilter();
            filter.Kind = "Component";

            // get all components
            var components = project.AllFCOs(filter).OfType<IMgaModel>().Cast<IMgaModel>().ToList();

            // store components that may have an option (e.g. MM1, MM2)
            List<IMgaModel> componentsToShow = new List<IMgaModel>();

            // iterate through all components
            // select only those which has more than one modelica model in it
            foreach (var component in components)
            {
                var modelicaModels = component.ChildFCOs.Cast<IMgaFCO>().Where(x => x.Meta.Name == "ModelicaModel");
                if (modelicaModels.Count() > 1)
                {
                    componentsToShow.Add(component);
                }
            }

            using (FidelitySelectorForm fsf = new FidelitySelectorForm())
            {

                // show the form for the user
                foreach (var component in componentsToShow)
                {
                    if (fsf.componentItems.FirstOrDefault(
                        x => x.Classification == component.StrAttrByName["Classifications"]) == null)
                    {
                        fsf.componentItems.Add(new ComponentItem(component));
                    }
                }

                // get the current fidelity settings from the Testbench registry
                fsf.FidelitySettings = currentobj.RegistryValue["FidelitySettings"];

                fsf.PopulateDgv();
                fsf.ShowDialog();

                foreach (string msg in fsf.consoleMessages)
                {
                    GMEConsole.Info.WriteLine(msg);
                }

                // Check where the new fidelity settings should be applied:
                if (fsf.rbThisTestbench.Checked)
                {
                    // Write the updated fidelity settings back to the Testbench registry
                    currentobj.RegistryValue["FidelitySettings"] = fsf.FidelitySettings;
                }
                else if (fsf.rbThisFolder.Checked)
                {
                    // Get all testbenches in the same folder as currentobj
                    var thisTestingFolder = currentobj.ParentFolder;
                    var siblingTBs =
                        thisTestingFolder.ChildFCOs.Cast<IMgaFCO>().Where(x => x.Meta.Name == "TestBench").ToList();

                    GMEConsole.Info.WriteLine(
                        "Applying these FidelitySettings to all TestBenches in folder '{0}':", thisTestingFolder.Name);

                    // Go through each one and set the fidelity settings
                    foreach (MgaFCO testbench in siblingTBs)
                    {
                        testbench.RegistryValue["FidelitySettings"] = fsf.FidelitySettings;

                        GMEConsole.Info.WriteLine("=> {0}", testbench.Name); 
                    }
                }
                else if (fsf.rbThisProject.Checked)
                {
                    // Get all testbenches in the entire project
                    MgaFilter testbenchFilter = project.CreateFilter();
                    testbenchFilter.Kind = "TestBench";

                    // get all testbenches
                    var testbenches = project.AllFCOs(testbenchFilter).OfType<IMgaModel>().Cast<IMgaModel>().ToList();

                    GMEConsole.Info.WriteLine(
                        "Applying these FidelitySettings to all TestBenches in project '{0}':", project.Name);

                    // Go through each one and set the fidelity settings
                    foreach (MgaFCO testbench in testbenches)
                    {
                        testbench.RegistryValue["FidelitySettings"] = fsf.FidelitySettings;

                        GMEConsole.Info.WriteLine("=> {0}", testbench.Name); 
                    }
                }
            }


            GMEConsole.Info.WriteLine("{0} finished.", ComponentName);
        }

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
                GMEConsole = GMEConsole.CreateFromProject(project);
                MgaGateway = new MgaGateway(project);

                MgaGateway.PerformInTransaction(delegate
                {
                    Main(project, currentobj, selectedobjs, Convert(param));
                }, abort: false);
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
