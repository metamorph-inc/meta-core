using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

using GME.CSharp;
using GME;
using GME.MGA;
using GME.MGA.Core;

using GmeCommon = ISIS.GME.Common;
using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyMLClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using META;
using MetaLinkBridge = CyPhyMetaLinkBridgeClient;
using MetaLinkProtobuf = edu.vanderbilt.isis.meta;
using System.Diagnostics;
using System.Threading;
using System.Security;
using System.Drawing;
using Microsoft.Win32;


namespace CyPhyMetaLink
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig_Intf.guid),
    ProgId(ComponentConfig_Intf.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyPhyMetalinkInterpreter : IMgaComponentEx, IGMEVersionInfo
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

        [DllImport("user32.dll")]
        static extern IntPtr SendMessage(IntPtr hWnd, int wMsg, IntPtr wParam, IntPtr lParam);

        /// <summary>
        /// This function is called for each interpreter invocation before Main.
        /// Don't perform MGA operations here unless you open a tansaction.
        /// </summary>
        /// <param name="project">The handle of the project opened in GME, for which the interpreter was called.</param>
        public void Initialize(MgaProject project)
        {
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
            GMEConsole.Error.WriteLine("This interpreter doesn't run in console mode.");
        }

        private CyPhyML.ComponentAssembly FindTopLevelAssembly(GmeCommon.Interfaces.FCO componentasm)
        {
            CyPhyML.ComponentAssembly topassembly = null;
            if (componentasm.ParentContainer.Kind == "ComponentAssemblies")             // Top Level Assembly
            {
                topassembly = (componentasm as CyPhyML.ComponentAssembly);
            }
            else if (componentasm.ParentContainer.Kind == "ComponentAssembly")
            {
                topassembly = FindTopLevelAssembly(componentasm.ParentContainer as CyPhyML.ComponentAssembly);
            }

            return topassembly;
        }

        #region IMgaComponentEx Members

        public MgaGateway MgaGateway { get; set; }
        public GMEConsole GMEConsole { get; set; }
        private CyPhyMetaLink.CyPhyMetaLinkAddon propagateAddon;

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


                propagateAddon = GetPropagateAddon(project);
                if (propagateAddon == null)
                {
                    GMEConsole.Error.WriteLine("CyPhyMLSync: Unable to contact CyPhyMLPropagate.");
                    return;
                }
                if (propagateAddon.bridgeClient.IsConnectedToBridge() == false)
                {
                    ConnectToMetaLinkBridge(project, param);
                }

                string currentobjKind = null;
                MgaFCO selectedCADModel = null;
                string componentVersion = null;
                MgaGateway.PerformInTransaction(delegate
                {
                    if (currentobj != null)
                    {
                        currentobjKind = currentobj.Meta.Name;
                        if (currentobjKind == "CADModel")
                        {
                            selectedCADModel = currentobj;
                            currentobj = (MgaFCO)currentobj.ParentModel;
                            currentobjKind = currentobj.Meta.Name;
                        }
                        else
                        {
                            Func<MgaFCO, bool> isCADCreoModel = fco => fco.Meta.Name == "CADModel"
                                && CyPhyMLClasses.CADModel.Cast(fco).Attributes.FileFormat == CyPhyMLClasses.CADModel.AttributesClass.FileFormat_enum.Creo;
                            selectedCADModel = (selectedobjs ?? (System.Collections.IEnumerable)(new List<MgaFCO>())).Cast<MgaFCO>()
                                .FirstOrDefault(isCADCreoModel);
                            if (selectedCADModel == null && currentobj.ObjType == GME.MGA.Meta.objtype_enum.OBJTYPE_MODEL)
                            {
                                // TODO: show dialog instead of picking one at random
                                selectedCADModel = ((MgaModel)currentobj).ChildFCOs.Cast<MgaFCO>().FirstOrDefault(isCADCreoModel);
                            }
                        }
                        if (currentobj != null && currentobj.Meta.Name == typeof(CyPhyMLClasses.Component).Name)
                        {
                            componentVersion = CyPhyMLClasses.Component.Cast(currentobj).Attributes.Version;
                        }
                    }
                }, transactiontype_enum.TRANSACTION_GENERAL, abort: false);
                if (currentobjKind != null && currentobjKind != "Component" && currentobjKind != "CADModel" && currentobjKind != "ComponentAssembly")
                {
                    System.Windows.Forms.MessageBox.Show("Please open a Component or a Component Assembly");
                    return;
                }
                if (currentobjKind == "Component")
                {
                    CyPhyML.Component component = CyPhyMLClasses.Component.Cast(currentobj);
                    if (selectedCADModel == null)
                    {
                        GMEConsole.Error.WriteLine("This component has no CADModels to edit.");
                        return;
                    }
                    bool connected = true;
                    if (propagateAddon.bridgeClient.IsConnectedToBridge() == false)
                    {
                        connected = ConnectToMetaLinkBridge(project, param);
                    }
                    if (connected)
                    {
                        LinkComponent(component, selectedCADModel);
                    }
                    return;
                }

                if (currentobjKind == null)
                {
                    propagateAddon.StartCreoEmpyMode();
                    return;
                }

                //GMEConsole.Out.WriteLine("Running CyPhySync interpreter...");

                if (propagateAddon.AssemblyID != null)
                {
                    GMEConsole.Warning.WriteLine("A ComponentAssembly is already synced");
                }
                else
                {
                    bool connected = propagateAddon.bridgeClient.IsConnectedToBridge();
                    if (connected == false)
                    {
                        GMEConsole.Info.WriteLine("Connecting to MetaLink Bridge ...");

                        connected = ConnectToMetaLinkBridge(project, param);
                    }

                    if (connected)
                    {
                        StartAssemblySync(project, currentobj, param);
                    }
                }

                //GMEConsole.Out.WriteLine("End of CyPhySync interpreter...");
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


        private void LinkComponent(CyPhyML.Component component, MgaFCO selectedCADModel)
        {
            try
            {
                bool createNewComponent = false;
                CyPhyMetaLink.CyPhyMetaLinkAddon.CreoOpenMode creoMode = CyPhyMetaLink.CyPhyMetaLinkAddon.CreoOpenMode.OPEN_EMPTY;
                var createOrEdit = new CreateOrEditComponent();
                createOrEdit.btnCreateNewComponent.Click += (o, e) =>
                    {
                        createNewComponent = true;
                        creoMode = CyPhyMetaLink.CyPhyMetaLinkAddon.CreoOpenMode.OPEN_COMPONENT;
                        createOrEdit.DialogResult = DialogResult.OK;
                        createOrEdit.Close();
                    };
                createOrEdit.btnEditExistingComponent.Click += (o, e) =>
                    {
                        createNewComponent = false;
                        creoMode = CyPhyMetaLink.CyPhyMetaLinkAddon.CreoOpenMode.OPEN_COMPONENT;
                        createOrEdit.DialogResult = DialogResult.OK;
                        createOrEdit.Close();
                    };
                createOrEdit.btnEmptyCreoOpen.Click += (o, e) =>
                {
                    createNewComponent = false;
                    creoMode = CyPhyMetaLink.CyPhyMetaLinkAddon.CreoOpenMode.OPEN_EMPTY;
                    createOrEdit.DialogResult = DialogResult.OK;
                    createOrEdit.Close();
                };
                DialogResult result = createOrEdit.ShowDialog();
                if (result != DialogResult.Cancel)
                {
                    if (creoMode == CyPhyMetaLink.CyPhyMetaLinkAddon.CreoOpenMode.OPEN_COMPONENT)
                    {
                        propagateAddon.StartEditingComponent(component, selectedCADModel, createNewComponent);
                    }
                    else
                    {
                        propagateAddon.StartCreoEmpyMode();
                    }
                }
            }
            catch (IOException e) // e.g. manifest.project.json not found
            {
                GMEConsole.Error.WriteLine(e.ToString());
            }
            return;
        }

        public static string GetJavaInstallationPath()
        {

            foreach (var javaKey in new[] {
                new { key = "SOFTWARE\\JavaSoft\\Java Runtime Environment", view = RegistryView.Registry64 },
                new { key = "SOFTWARE\\JavaSoft\\Java Runtime Environment", view = RegistryView.Registry32 },
                new { key = "SOFTWARE\\JavaSoft\\Java Development Kit", view = RegistryView.Registry64 },
                new { key = "SOFTWARE\\JavaSoft\\Java Development Kit", view = RegistryView.Registry32 },
            })
            {
            try
            {
                    using (var baseKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, javaKey.view).OpenSubKey(javaKey.key))
                {
                    String currentVersion = baseKey.GetValue("CurrentVersion").ToString();
                    using (var homeKey = baseKey.OpenSubKey(currentVersion))
                        return homeKey.GetValue("JavaHome").ToString();
                }
            }
            catch (Exception)
            {
                // no 64-bit Java was found. Will try 64-bit
            }
            }

            return null;
        }

        public bool ConnectToMetaLinkBridge(MgaProject project, int param)
        {
            bool connected = false;

            if (propagateAddon == null)
            {
                propagateAddon = GetPropagateAddon(project);
            }
            if (propagateAddon.EstablishConnection())
            {
                connected = true;
            }
            else
            {
                //GMEConsole.Error.WriteLine("Failed to establish connection to MetaLink Bridge.");
                if (param != 128)
                {
                    string metaLinkPath = Path.Combine(META.VersionInfo.MetaPath, @"src\MetaLink\meta-bridge\java-server\target\metalink-java-server-1.1.0.jar"); // dev machine
                    if (!File.Exists(metaLinkPath))
                    {
                        metaLinkPath = Path.Combine(META.VersionInfo.MetaPath, @"bin\metalink-java-server-1.1.0.jar"); // installed machine
                    }
                    if (File.Exists(metaLinkPath))
                    {
                        string java_path = GetJavaInstallationPath();
                        if (java_path == null)
                        {
                            throw new ApplicationException("Could not find java.exe. Is Java installed?");
                        }
                        string java_exe = Path.Combine(java_path, "bin\\java.exe");

                        //DialogResult res = MessageBox.Show("Could not connect to MetaLink.\n\nDo you want to start MetaLink?", "MetaLink", MessageBoxButtons.YesNo);
                        //if (res == DialogResult.Yes)
                        {
                            ProcessStartInfo startInfo = new ProcessStartInfo()
                            {
                                FileName = java_exe,
                                // Arguments = String.Format("-jar \"{0}\" -r \"{1}\"", metaLinkPath, Path.GetTempFileName()),
                                Arguments = String.Format("-jar \"{0}\"", metaLinkPath),
                                WindowStyle = ProcessWindowStyle.Minimized,
                                UseShellExecute = false,
                            };
#if DEBUG
                            // startInfo.Arguments += String.Format(" -r \"{0}\"", Path.Combine(Path.GetTempPath, "metalink_recordedmessages.mlp");
#endif
                            Process metalink = new Process();
                            metalink.StartInfo = startInfo;
                            metalink.Start();
#if DEBUG
                            metalink.BeginErrorReadLine();
                            metalink.BeginOutputReadLine();
#endif

                            for (int i = 0; i < 100; i++)
                            {
                                if (propagateAddon.EstablishConnection())
                                {
                                    connected = true;
                                    break;
                                }
                                Thread.Sleep(100);
                            }
                            int WM_SETICON = 0x80;
                            int ICON_SMALL = 0;
                            int ICON_BIG = 1;
                            Icon icon = Properties.Resources.CyPhyMLSync;
                            IntPtr result = SendMessage(metalink.MainWindowHandle, WM_SETICON, (IntPtr)ICON_SMALL, icon.Handle);
                            result = SendMessage(metalink.MainWindowHandle, WM_SETICON, (IntPtr)ICON_BIG, icon.Handle);
                            int WM_SYSCOMMAND = 0x0112;
                            var SC_MINIMIZE = (IntPtr)0xF020;
                            SendMessage(metalink.MainWindowHandle, WM_SYSCOMMAND, SC_MINIMIZE, IntPtr.Zero);
                            if (!connected)
                            {
                                GMEConsole.Error.WriteLine("Failed to start MetaLink Bridge.");
                                metalink.Dispose();
                            }
                            else
                            {
                                propagateAddon.metalinkBridge = metalink;
                            }
                        }
                    }
                    else
                    {
                        GMEConsole.Error.WriteLine("Meta-Link Bridge executable is missing. Please check that Meta-Link Bridge is installed.");
                    }
                }
            }
            if (connected)
            {
                GMEConsole.Info.WriteLine("Established connection to MetaLink Bridge.");
            }
            return connected;
        }

        public void StartAssemblySync(MgaProject project, MgaFCO currentobj, int param)
        {
            propagateAddon.Enable(true);

            CyPhyML.ComponentAssembly topasm = null;
            MgaGateway.PerformInTransaction(delegate
            {
                topasm = Run(project, currentobj, param);
                if (topasm != null)
                {
                    propagateAddon.AssemblyID = topasm.Guid.ToString();
                }
            });

            if (topasm == null)
            {
                propagateAddon.AssemblyID = null;
                GMEConsole.Error.WriteLine("No top level assembly can be found. Open an assembly and try again.");
                return;
            }
            try
            {
                propagateAddon.CallCyPhy2CADWithTransaction(project,
                                (topasm.Impl as MgaFCO),
                                param);
            }
            catch (Exception e)
            {
                propagateAddon.syncedComponents.Remove(propagateAddon.AssemblyID);
                propagateAddon.AssemblyID = null;
                GMEConsole.Error.Write(e.Message);
            }
        }

        private static CyPhyMetaLink.CyPhyMetaLinkAddon GetPropagateAddon(MgaProject project)
        {
            CyPhyMetaLink.CyPhyMetaLinkAddon cyPhyMLSyncComponent = null;
            MgaComponents mgaComponents = project.AddOnComponents;
            foreach (IMgaComponent iMgaComponent in mgaComponents)
            {
                if (iMgaComponent.ComponentName == "CyPhyMetaLinkAddon")
                {
                    cyPhyMLSyncComponent = (CyPhyMetaLink.CyPhyMetaLinkAddon)iMgaComponent;
                    break;
                }
            }
            return cyPhyMLSyncComponent;
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
                return ComponentConfig_Intf.progID;
            }
        }

        public componenttype_enum ComponentType
        {
            get { return ComponentConfig_Intf.componentType; }
        }
        public string Paradigm
        {
            get { return ComponentConfig_Intf.paradigmName; }
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
            Registrar.RegisterInterpreter();
        }

        [ComUnregisterFunctionAttribute]
        public static void GMEUnRegister(Type t)
        {
            Registrar.UnregisterInterpreter();
        }

        #endregion

        private CyPhyML.ComponentAssembly Run(MgaProject project,
                                              MgaFCO currentobj,
                                              int param)
        {
            CyPhyML.ComponentAssembly topasm = null;
            if (currentobj == null || currentobj.MetaBase.Name != "ComponentAssembly")
            {
                MessageBox.Show("Please open an assembly!");
                return topasm;
            }

            // [1] Find top-level assembly
            // [2] Create folder name
            // [3] MainFrame
            // [4] CyPhy2CAD_CSharp


            // [1]
            topasm = FindTopLevelAssembly(CyPhyMLClasses.ComponentAssembly.Cast(currentobj));
            if (topasm == null)
            {
                return topasm;
            }

#if DEBUG
            if (topasm != null)
            {
                GMEConsole.Out.WriteLine("TopAsm:" + topasm.Name);
            }
#endif

            ProjectDirectory = Path.GetDirectoryName(currentobj.Project.ProjectConnStr.Substring("MGA=".Length));
            string workingDir = "MetaLink_" + DateTime.Now.ToString("MMddHHmmss");
            CyPhyMetaLink.SyncedComponentData cdata = new CyPhyMetaLink.SyncedComponentData()
            {
                Type = SyncedComponentData.EditType.Design,
                WorkingDir = Path.Combine(ProjectDirectory, workingDir),
                Id = topasm.Guid.ToString(),
                InstanceId = (CyPhyMetaLinkAddon.IdCounter++).ToString()
            };
            if (!Directory.Exists(cdata.WorkingDir))
            {
                Directory.CreateDirectory(cdata.WorkingDir);
            }

            // [3]
            DialogResult ok = DialogResult.Cancel;

            if (param != 128)
            {
                using (MainForm mf = new MainForm(cdata.WorkingDir))
                {
                    ok = mf.ShowDialog();
                    cdata.AuxDir = mf.AuxiliaryDir;

                    if (ok != DialogResult.OK)
                    {
                        return null;
                    }
                }
            }

            propagateAddon.syncedComponents.Add(cdata.Id, cdata);

            return topasm;
        }

        string ProjectDirectory;
    }
}
