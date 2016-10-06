using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using GME.CSharp;
using GME.MGA;
using GME.MGA.Core;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using System.Drawing;
using System.IO;

namespace CyPhyComponentAuthoring
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>


    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    //[IsCATModule(ContainsCATmethod = true)]
    [ComVisible(true)]
    public class CyPhyComponentAuthoringInterpreter : IMgaComponentEx, IGMEVersionInfo
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

        private MgaProject StashProject { get; set; }
        private MgaFCO StashCurrentObj { get; set; }
        private CyPhy.Component StashCurrentComponent { get; set; }

        [ComVisible(false)]
        public void Main(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, ComponentStartMode startMode)
        {
            this.Logger.WriteInfo("Running Component Authoring interpreter.");

            // verify we are running in a component and that it is not an instance or library
            string return_msg;
            if (!CheckPreConditions(currentobj, out return_msg))
            {
                this.Logger.WriteFailed(return_msg);
                return;
            }

            // assuming a component is open
            // stash off the project, currentobj and CurrentComponent parameters for use in the event handlers
            StashProject = project;
            StashCurrentObj = currentobj;
            StashCurrentComponent = CyPhyClasses.Component.Cast(currentobj);

            // use reflection to populate the dialog box objects
            PopulateDialogBox();

            // To use the domain-specific API:
            //  Create another project with the same name as the paradigm name
            //  Copy the paradigm .mga file to the directory containing the new project
            //  In the new project, install the GME DSMLGenerator NuGet package (search for DSMLGenerator)
            //  Add a Reference in this project to the other project
            //  Add "using [ParadigmName] = ISIS.GME.Dsml.[ParadigmName].Classes.Interfaces;" to the top of this file
            // if (currentobj.Meta.Name == "KindName")
            // [ParadigmName].[KindName] dsCurrentObj = ISIS.GME.Dsml.[ParadigmName].Classes.[KindName].Cast(currentobj);			
        }

        public bool CheckPreConditions(MgaFCO currentobj, out string message)
        {
            // check if the context is a Component
            if (currentobj == null ||
                currentobj.Meta.Name != typeof(CyPhy.Component).Name)
            {
                // this is a really bad situation we must return
                message = string.Format("Please open a Component and run this tool from that context.");
                return false;
            }
            else if (currentobj.IsInstance)
            {
                // there is no reason to run in an instance since it cannot be modified
                message = string.Format("This component cannot be modified because it is an instance.");
                return false;
            }
            else if (currentobj.IsLibObject)
            {
                // there is no reason to run on a library object since it cannot be modified
                message = string.Format("This component cannot be modified because it is a library object.");
                return false;
            }
            message = string.Format("Pre-Conditions met for running CAT");
            return true;
        }

        Dictionary<string, Tuple<Type, MethodInfo>> dictofCATDnDMethods = new Dictionary<string, Tuple<Type, MethodInfo>>();
        void DragNDropHandler(string filename)
        {
            Tuple<Type, MethodInfo> method;
            if (dictofCATDnDMethods.TryGetValue(Path.GetExtension(filename), out method))
            {
                CATModule newinst = CreateCATModule(method.Item1);

                method.Item2.Invoke(newinst, new object[] { filename });

                GMEConsole console = GMEConsole.CreateFromProject(this.StashProject);
                console.Info.WriteLine("Processed " + filename);
                Marshal.ReleaseComObject(console.gme);
            }
        }


        public void PopulateDialogBox(bool testonly = false)
        {
            List<Tuple<CATName, Type, MethodInfo>> listofCATmethods = new List<Tuple<CATName, Type, MethodInfo>>();

            // create the dialog box
            using (CyPhyComponentAuthoringToolGUI CATgut = new CyPhyComponentAuthoringToolGUI(DragNDropHandler))
            {
                // get the current assembly
                Assembly thisAssembly = Assembly.GetExecutingAssembly();

                // scan each class in the current assembly
                foreach (Type classtype in thisAssembly.GetTypes())
                {
                    send_to_logger(String.Format("Found class {0}", classtype.ToString()), testonly);

                    // Linq query to get all CATModules
                    var CATModulesQuery = classtype.GetCustomAttributes(true)
                                                   .OfType<IsCATModule>()
                                                   .Where(x => x.ContainsCATmethod)
                                                   .Distinct();

                    foreach (var CATMod in CATModulesQuery)
                    {
                        send_to_logger(String.Format("Found CATModule {0}", CATMod.ToString()), testonly);

                        // get a list of all the methods in the class
                        // scan each method in the class for custom attributes
                        foreach (MethodInfo meth in classtype.GetMethods())
                        {
                            send_to_logger(String.Format("Found method {0}", meth.ToString()), testonly);

                            // Get the custom attributes for this method.
                            // If it has the CATName attribute, we'll add it as a button.
                            foreach (CATName attr in meth.GetCustomAttributes(typeof(CATName), true))
                            {
                                listofCATmethods.Add(new Tuple<CATName, Type, MethodInfo>(attr, classtype, meth));
                            }

                            foreach (CATDnD attr in meth.GetCustomAttributes(typeof(CATDnD), true))
                            {
                                dictofCATDnDMethods.Add(attr.Extension.ToLowerInvariant(), new Tuple<Type, MethodInfo>(classtype, meth));
                            }
                        }
                    }
                }

                foreach (Tuple<CATName, Type, MethodInfo> item in listofCATmethods.OrderBy(i => i.Item1.RoleVal))
                {
                    AddButtontoDialogBox(CATgut, item.Item1, item.Item2, item.Item3);
                }

                // META-2679 Set the start position of CAT dialog box to the center of the screen.
                CATgut.StartPosition = FormStartPosition.CenterScreen;
                // save the dialog box ref for testing
                ThisDialogBox = CATgut;
                // start up the dialog box
                if (!testonly)
                {
                    CATgut.ShowDialog();
                }
            }
        }

        public void send_to_logger(string str2write, bool testmode)
        {
            // extracted writing to the log because it made the automated test crash
            if (!testmode)
            {
                Logger.WriteDebug(str2write);
            }
        }


        public void close_dialog_box(object sender, EventArgs e)
        {
            // close the CAT dialog box.  Called as a secondary event handler delegate
            string ags = e.ToString();

            if (ags == "done")
            {
                ThisDialogBox.Close();
            }
        }

        private const int CAT_BUTTON_WIDTH = 130;
        private const int CAT_BUTTON_HEIGHT = 50;
        private const int CAT_DESCRIPTION_WIDTH = 510;
        private const int CAT_DESCRIPTION_HEIGHT = 50;

        public void AddButtontoDialogBox(CyPhyComponentAuthoringToolGUI CATgut, CATName attr, Type classtype, MethodInfo meth)
        {
            // add the Event Handler to the Button List
            CATName a = (CATName)attr;

            // setup button properties
            Button button = new Button();
            button.Name = a.NameVal;
            button.Text = a.NameVal;
            button.Anchor = AnchorStyles.Top;

            // set the button size
            button.Width = CAT_BUTTON_WIDTH;
            button.Height = CAT_BUTTON_HEIGHT;

            // make clicking the button an event we can handle
            Type buttonType = typeof(Button);
            EventInfo ev = buttonType.GetEvent("Click");
            Delegate handler = null;

            // create a delegate event handler for the button
            if (classtype.Name == typeof(CyPhyComponentAuthoringInterpreter).Name)
            {
                // local methods don't require a new instance of CATModule
                handler = Delegate.CreateDelegate(ev.EventHandlerType, this, meth);
            }
            else
            {
                CATModule newinst = CreateCATModule(classtype);
                if (newinst == null)
                {
                    return;
                }

                // create a delegate for the dialog button to call to invoke the method
                handler = Delegate.CreateDelegate(ev.EventHandlerType, newinst, meth);
            }
            // associate the event handler with the button click
            ev.AddEventHandler(button, handler);

            // Add a second event handler to each button to close the CAT dialog box after executing the first event handler
            Delegate closer = Delegate.CreateDelegate(ev.EventHandlerType, this, this.GetType().GetMethod("close_dialog_box"));
            ev.AddEventHandler(button, closer);

            // add another row to the table - each "row" is actually a smaller 2 column table
            TableLayoutPanel new_mini_table = new TableLayoutPanel();
            new_mini_table.AutoSize = true;
            new_mini_table.ColumnCount = 2;
            new_mini_table.RowCount = 1;

            // create the button
            new_mini_table.Controls.Add(button, 0, 0);

            //setup description properties
            Label description = new Label();
            description.Text = a.DescriptionVal;
            description.MinimumSize = new System.Drawing.Size(CAT_DESCRIPTION_WIDTH, CAT_DESCRIPTION_HEIGHT);
            description.Anchor = AnchorStyles.Top;
            description.TextAlign = ContentAlignment.MiddleCenter;
            
            // set the description
            new_mini_table.Controls.Add(description, 1, 0);

            // add the new CAT module to the dialog box
            CATgut.tableLayoutPanel0.Controls.Add(new_mini_table, 1, ++CATgut.tableLayoutPanel0.RowCount);
        }

        private CATModule CreateCATModule(Type classtype)
        {
            // create a new CATModule class instance 
            CATModule newinst = Activator.CreateInstance(classtype) as CATModule;
            if (newinst == null)
            {
                // problem
                this.Logger.WriteFailed("Unable to create a new CATModule class instance. CreateInstance call failed.");
                return null;
            }

            // set the current component for use by the new class instance
            newinst.SetCurrentComp(StashCurrentComponent);
            newinst.CurrentProj = StashProject;
            newinst.CurrentObj = StashCurrentObj;

            return newinst;
        }

        // Classes which contain one or more CAT methods should apply this attribute with the bool set to true
        [AttributeUsage(AttributeTargets.Class, AllowMultiple = true)]
        public class IsCATModule : System.Attribute
        {
            public bool ContainsCATmethod;
        }

        // Event handlers wishing to be added to the CAT dialog box should add this attribute to the method
        // valid values for RoleVal are Construct, Modify, Share, or Publish
        public enum Role
        {
            Construct = 1,
            Modify = 2,
            Share = 3,
            Publish = 4
        };

        [AttributeUsage(AttributeTargets.Method, AllowMultiple = true)]
        public class CATName : System.Attribute
        {
            public string NameVal;
            public string DescriptionVal;
            public Role RoleVal;
        }

        [AttributeUsage(AttributeTargets.Method, AllowMultiple = true)]
        public class CATDnD : System.Attribute
        {
            public string Extension;
        }

        #region IMgaComponentEx Members

        MgaGateway MgaGateway { get; set; }
        public CyPhyGUIs.GMELogger Logger { get; set; }

        public Form ThisDialogBox { get; set; }

        public void InvokeEx(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, int param)
        {
            if (!enabled)
            {
                return;
            }

            try
            {
                MgaGateway = new MgaGateway(project);
                this.Logger = new CyPhyGUIs.GMELogger(project, this.GetType().Name);

                MgaGateway.PerformInTransaction(delegate
                {
                    Main(project, currentobj, selectedobjs, Convert(param));
                }, abort: false);
            }
            finally
            {
                if (Logger != null)
                {
                    Logger.Dispose();
                }
                MgaGateway = null;
                Logger = null;
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
