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
using System.ComponentModel;


namespace CyPhyComponentParameterEditor
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyPhyComponentParameterEditorInterpreter : IMgaComponentEx, IGMEVersionInfo
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

            // To use the domain-specific API:
            //  Create another project with the same name as the paradigm name
            //  Copy the paradigm .mga file to the directory containing the new project
            //  In the new project, install the GME DSMLGenerator NuGet package (search for DSMLGenerator)
            //  Add a Reference in this project to the other project
            //  Add "using [ParadigmName] = ISIS.GME.Dsml.[ParadigmName].Classes.Interfaces;" to the top of this file
            // [ParadigmName].[KindName] dsCurrentObj = ISIS.GME.Dsml.[ParadigmName].Classes.[KindName].Cast(currentobj);

            // TODO: find model editor window and open form inside it. Would be nice to find it based on  C++ class.
            var mainHwd = System.Diagnostics.Process.GetCurrentProcess().MainWindowHandle;

            IntPtr mdiClient = IntPtr.Zero;

            foreach (var childHwd in GetChildWindows(mainHwd))
            {
                StringBuilder sb = new StringBuilder(100);
                GetClassName(childHwd, sb, 100);
                if ("MDIClient" == sb.ToString())
                {
                    mdiClient = childHwd;
                    break;
                }
            }

            //GMEConsole.Warning.WriteLine("Name: '{0}' Position: {1} {2} {3} {4}", name, rr.Top, rr.Left, rr.Bottom, rr.Right);

            List<string> supportedKinds = new List<string>()
            {
                typeof(CyPhy.Component).Name,
                typeof(CyPhy.ComponentAssembly).Name,
                typeof(CyPhy.TestComponent).Name,
                typeof(CyPhy.DesignContainer).Name,
                typeof(CyPhy.TestBench).Name,
                typeof(CyPhy.BlastTestBench).Name,
                typeof(CyPhy.BallisticTestBench).Name,
                typeof(CyPhy.CADTestBench).Name,
                typeof(CyPhy.CFDTestBench).Name,
                typeof(CyPhy.TestBenchSuite).Name,
            };

            if (currentobj == null ||
                supportedKinds.Contains(currentobj.Meta.Name) == false)
            {
                GMEConsole.Warning.WriteLine("One of the following object types must be open in the editor: {0}", string.Join(", ", supportedKinds));
                return;
            }

            List<ComponentParameterItem> parameterItems = new List<ComponentParameterItem>();

            if (currentobj.Meta.Name == typeof(CyPhy.Component).Name ||
                currentobj.Meta.Name == typeof(CyPhy.ComponentAssembly).Name)
            {
                var component = CyPhyClasses.DesignElement.Cast(currentobj);

                foreach (var item in component.Children.ParameterCollection)
                {
                    var parameterItem = new ComponentParameterItem(item.Impl as MgaFCO);

                    if (item.SrcConnections.ValueFlowCollection.Where(x => x.ParentContainer.ID == item.ParentContainer.ID).Count() > 0)
                    {
                        // skip derived parameters
                        GMEConsole.Info.WriteLine("{0} is a derived parameter; it is not shown in list.", item.Name);
                        continue;
                    }

                    parameterItems.Add(parameterItem);
                }

                foreach (var item in component.Children.PropertyCollection)
                {
                    var parameterItem = new ComponentParameterItem(item.Impl as MgaFCO);

                    if (item.SrcConnections.ValueFlowCollection.Where(x => x.ParentContainer.ID == item.ParentContainer.ID).Count() > 0)
                    {
                        // skip derived parameters
                        GMEConsole.Info.WriteLine("{0} is a derived parameter; it is not shown in list.", item.Name);
                        continue;
                    }

                    parameterItems.Add(parameterItem);
                }
            }
            else if (currentobj.Meta.Name == typeof(CyPhy.DesignContainer).Name)
            {
                var component = CyPhyClasses.DesignContainer.Cast(currentobj);

                foreach (var item in component.Children.ParameterCollection)
                {
                    var parameterItem = new ComponentParameterItem(item.Impl as MgaFCO);

                    if (item.SrcConnections.ValueFlowCollection.Where(x => x.ParentContainer.ID == item.ParentContainer.ID).Count() > 0)
                    {
                        // skip derived parameters
                        GMEConsole.Info.WriteLine("{0} is a derived parameter; it is not  shown in list.", item.Name);
                        continue;
                    }

                    parameterItems.Add(parameterItem);
                }

                foreach (var item in component.Children.PropertyCollection)
                {
                    var parameterItem = new ComponentParameterItem(item.Impl as MgaFCO);

                    if (item.SrcConnections.ValueFlowCollection.Where(x => x.ParentContainer.ID == item.ParentContainer.ID).Count() > 0)
                    {
                        // skip derived parameters
                        GMEConsole.Info.WriteLine("{0} is a derived parameter; it is not shown in list.", item.Name);
                        continue;
                    }

                    parameterItems.Add(parameterItem);
                }

            }
            else if (currentobj.Meta.Name == typeof(CyPhy.TestComponent).Name)
            {
                var component = CyPhyClasses.TestComponent.Cast(currentobj);

                foreach (var item in component.Children.ParameterCollection)
                {
                    var parameterItem = new ComponentParameterItem(item.Impl as MgaFCO);

                    if (item.SrcConnections.ValueFlowCollection.Where(x => x.ParentContainer.ID == item.ParentContainer.ID).Count() > 0)
                    {
                        // skip derived parameters
                        GMEConsole.Info.WriteLine("{0} is a derived parameter; it is not  shown in list.", item.Name);
                        continue;
                    }

                    parameterItems.Add(parameterItem);
                }

                foreach (var item in component.Children.PropertyCollection)
                {
                    var parameterItem = new ComponentParameterItem(item.Impl as MgaFCO);

                    if (item.SrcConnections.ValueFlowCollection.Where(x => x.ParentContainer.ID == item.ParentContainer.ID).Count() > 0)
                    {
                        // skip derived parameters
                        GMEConsole.Info.WriteLine("{0} is a derived parameter; it is not shown in list.", item.Name);
                        continue;
                    }

                    parameterItems.Add(parameterItem);
                }

                foreach (var item in component.Children.MetricCollection)
                {
                    var parameterItem = new ComponentParameterItem(item.Impl as MgaFCO);

                    if (item.SrcConnections.ValueFlowCollection.Where(x => x.ParentContainer.ID == item.ParentContainer.ID).Count() > 0)
                    {
                        // skip derived parameters
                        //GMEConsole.Info.WriteLine("{0} is a derived parameter; it is not shown in list.", item.Name);
                        //continue;
                    }

                    parameterItems.Add(parameterItem);
                }
            }

            else if (currentobj.Meta.Name == typeof(CyPhy.TestBench).Name ||
                currentobj.Meta.Name == typeof(CyPhy.BlastTestBench).Name ||
                currentobj.Meta.Name == typeof(CyPhy.BallisticTestBench).Name ||
                currentobj.Meta.Name == typeof(CyPhy.CADTestBench).Name ||
                currentobj.Meta.Name == typeof(CyPhy.CFDTestBench).Name)
            {
                var component = CyPhyClasses.TestBenchType.Cast(currentobj);

                foreach (var item in component.Children.ParameterCollection)
                {
                    var parameterItem = new ComponentParameterItem(item.Impl as MgaFCO);

                    if (item.SrcConnections.ValueFlowCollection.Where(x => x.ParentContainer.ID == item.ParentContainer.ID).Count() > 0)
                    {
                        // skip derived parameters
                        GMEConsole.Info.WriteLine("{0} is a derived parameter; it is not  shown in list.", item.Name);
                        continue;
                    }

                    parameterItems.Add(parameterItem);
                }

                foreach (var item in component.Children.PropertyCollection)
                {
                    var parameterItem = new ComponentParameterItem(item.Impl as MgaFCO);

                    if (item.SrcConnections.ValueFlowCollection.Where(x => x.ParentContainer.ID == item.ParentContainer.ID).Count() > 0)
                    {
                        // skip derived parameters
                        GMEConsole.Info.WriteLine("{0} is a derived parameter; it is not shown in list.", item.Name);
                        continue;
                    }

                    parameterItems.Add(parameterItem);
                }

                foreach (var item in component.Children.MetricCollection)
                {
                    var parameterItem = new ComponentParameterItem(item.Impl as MgaFCO);

                    if (item.SrcConnections.ValueFlowCollection.Where(x => x.ParentContainer.ID == item.ParentContainer.ID).Count() > 0)
                    {
                        // skip derived parameters
                        //GMEConsole.Info.WriteLine("{0} is a derived parameter; it is not shown in list.", item.Name);
                        //continue;
                    }

                    parameterItems.Add(parameterItem);
                }
            }
            else if (currentobj.Meta.Name == typeof(CyPhy.TestBenchSuite).Name)
            {
                var component = CyPhyClasses.TestBenchSuite.Cast(currentobj);

                foreach (var item in component.Children.ParameterCollection)
                {
                    var parameterItem = new ComponentParameterItem(item.Impl as MgaFCO);

                    if (item.SrcConnections.ValueFlowCollection.Where(x => x.ParentContainer.ID == item.ParentContainer.ID).Count() > 0)
                    {
                        // skip derived parameters
                        GMEConsole.Info.WriteLine("{0} is a derived parameter; it is not  shown in list.", item.Name);
                        continue;
                    }

                    parameterItems.Add(parameterItem);
                }

                foreach (var item in component.Children.MetricCollection)
                {
                    var parameterItem = new ComponentParameterItem(item.Impl as MgaFCO);

                    if (item.SrcConnections.ValueFlowCollection.Where(x => x.ParentContainer.ID == item.ParentContainer.ID).Count() > 0)
                    {
                        // skip derived parameters
                        //GMEConsole.Info.WriteLine("{0} is a derived parameter; it is not shown in list.", item.Name);
                        //continue;
                    }

                    parameterItems.Add(parameterItem);
                }
            }

            parameterItems.Sort((x, y) => x.Name.CompareTo(y.Name));

            if (parameterItems.Any() == false)
            {
                GMEConsole.Warning.WriteLine("Please insert at least one non-derived Parameter/Property in the model.");
                return;
            }

            using (ComponentParameterForm cpForm = new ComponentParameterForm(currentobj))
            {
                cpForm.dgvParameters.DataSource = new BindingList<ComponentParameterItem>(parameterItems);
                if (mdiClient != IntPtr.Zero)
                {
                    // TODO: would be nice to attach the form to the MDIClient window.
                    var parentWindow = new WindowWrapper(mdiClient);
                    var dialogResult = cpForm.ShowDialog(parentWindow);
                }
                else
                {
                    var dialogResult = cpForm.ShowDialog();
                }
            }
        }

        public class WindowWrapper : System.Windows.Forms.IWin32Window
        {
            public WindowWrapper(IntPtr handle)
            {
                _hwnd = handle;
            }

            public IntPtr Handle
            {
                get { return _hwnd; }
            }

            private IntPtr _hwnd;
        }



        #region Window handles

        [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        static extern int GetClassName(IntPtr hWnd, StringBuilder lpClassName, int nMaxCount);

        [DllImport("user32.dll")]
        public static extern bool GetWindowRect(IntPtr hwnd, ref Rect rectangle);

        [ComVisible(false)]
        public struct Rect
        {
            public int Left { get; set; }
            public int Right { get; set; }
            public int Bottom { get; set; }
            public int Top { get; set; }
        }

        [DllImport("user32.dll")]
        static extern int GetWindowText(IntPtr hWnd, StringBuilder text, int count);

        private string GetWindowTitle(IntPtr handle)
        {
            const int nChars = 256;
            StringBuilder Buff = new StringBuilder(nChars);

            if (GetWindowText(handle, Buff, nChars) > 0)
            {
                return Buff.ToString();
            }
            return null;
        }


        [DllImport("user32")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool EnumChildWindows(IntPtr window, EnumWindowProc callback, IntPtr i);

        /// <summary>
        /// Returns a list of child windows
        /// </summary>
        /// <param name="parent">Parent of the windows to return</param>
        /// <returns>List of child windows</returns>
        [ComVisible(false)]
        public static List<IntPtr> GetChildWindows(IntPtr parent)
        {
            List<IntPtr> result = new List<IntPtr>();
            GCHandle listHandle = GCHandle.Alloc(result);
            try
            {
                EnumWindowProc childProc = new EnumWindowProc(EnumWindow);
                EnumChildWindows(parent, childProc, GCHandle.ToIntPtr(listHandle));
            }
            finally
            {
                if (listHandle.IsAllocated)
                    listHandle.Free();
            }
            return result;
        }

        /// <summary>
        /// Callback method to be used when enumerating windows.
        /// </summary>
        /// <param name="handle">Handle of the next window</param>
        /// <param name="pointer">Pointer to a GCHandle that holds a reference to the list to fill</param>
        /// <returns>True to continue the enumeration, false to bail</returns>
        private static bool EnumWindow(IntPtr handle, IntPtr pointer)
        {
            GCHandle gch = GCHandle.FromIntPtr(pointer);
            List<IntPtr> list = gch.Target as List<IntPtr>;
            if (list == null)
            {
                throw new InvalidCastException("GCHandle Target could not be cast as List<IntPtr>");
            }
            list.Add(handle);
            //  You can modify this to check to see if you want to cancel the operation, then return a null here
            return true;
        }

        /// <summary>
        /// Delegate for the EnumChildWindows method
        /// </summary>
        /// <param name="hWnd">Window handle</param>
        /// <param name="parameter">Caller-defined variable; we use it for a pointer to our list</param>
        /// <returns>True to continue enumerating, false to bail.</returns>
        [ComVisible(false)]
        public delegate bool EnumWindowProc(IntPtr hWnd, IntPtr parameter);

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
