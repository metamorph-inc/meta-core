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
using System.Threading.Tasks;
using System.ComponentModel;
using CyPhyGUIs;

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
        private CyPhyMetaLink.CyPhyMetaLinkAddon metalinkAddon;
        Task<bool> ConnectToMetaLinkBridgeTask;

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
                ProjectDirectory = Path.GetDirectoryName(project.ProjectConnStr.Substring("MGA=".Length));


                metalinkAddon = GetMetaLinkAddon(project);
                if (metalinkAddon == null)
                {
                    GMEConsole.Error.WriteLine("MetaLink: Unable to find CyPhyMetaLinkAddon. Was it disabled under Tools>Register Components?");
                    return;
                }
                ConnectToMetaLinkBridge(project, param);

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
                    ConnectToMetaLinkBridgeTask.Wait();
                    connected = ConnectToMetaLinkBridgeTask.Result;
                    if (connected)
                    {
                        LinkComponent(component, selectedCADModel);
                    }
                    return;
                }

                if (currentobjKind == null)
                {
                    bool connected = true;
                    ConnectToMetaLinkBridgeTask.Wait();
                    connected = ConnectToMetaLinkBridgeTask.Result;
                    if (connected)
                    {
                        metalinkAddon.StartCreoEmpyMode();
                    }
                    return;
                }

                //GMEConsole.Out.WriteLine("Running CyPhySync interpreter...");

                if (metalinkAddon.AssemblyID != null)
                {
                    GMEConsole.Warning.WriteLine("A ComponentAssembly is already synced");
                }
                else
                {
                    StartAssemblySync(project, currentobj, param);
                }

                //GMEConsole.Out.WriteLine("End of CyPhySync interpreter...");
            }
            catch (AggregateException e)
            {
                foreach (var exception in e.InnerExceptions)
                {
                    GMEConsole.Error.WriteLine(exception.Message);
                }
                throw new ApplicationException(e.InnerException.Message, e.InnerException);
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
                        metalinkAddon.StartEditingComponent(component, selectedCADModel, createNewComponent);
                    }
                    else
                    {
                        metalinkAddon.StartCreoEmpyMode();
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
                new { key = "SOFTWARE\\JavaSoft\\JRE", view = RegistryView.Registry64 },
                new { key = "SOFTWARE\\JavaSoft\\JRE", view = RegistryView.Registry32 },
                new { key = "SOFTWARE\\JavaSoft\\Java Runtime Environment", view = RegistryView.Registry64 },
                new { key = "SOFTWARE\\JavaSoft\\Java Runtime Environment", view = RegistryView.Registry32 },
                new { key = "SOFTWARE\\JavaSoft\\Java Development Kit", view = RegistryView.Registry64 },
                new { key = "SOFTWARE\\JavaSoft\\Java Development Kit", view = RegistryView.Registry32 },
                new { key = "SOFTWARE\\JavaSoft\\JDK", view = RegistryView.Registry64 },
                new { key = "SOFTWARE\\JavaSoft\\JDK", view = RegistryView.Registry32 },
            })
            {
                try
                {
                    using (var baseKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, javaKey.view).OpenSubKey(javaKey.key))
                    {
                        if (baseKey == null)
                        {
                            continue;
                        }
                        object currentVersion = baseKey.GetValue("CurrentVersion");
                        if (currentVersion == null)
                        {
                            continue;
                        }
                        using (var homeKey = baseKey.OpenSubKey(currentVersion.ToString()))
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


        public Task<bool> ConnectToMetaLinkBridge(MgaProject project, int param)
        {
            ConnectToMetaLinkBridgeTask = Task.Run(() => _ConnectToMetaLinkBridge(project, param));
            return ConnectToMetaLinkBridgeTask;
        }

        private async Task<bool> _ConnectToMetaLinkBridge(MgaProject project, int param)
        {
            bool connected = false;

            if (metalinkAddon == null)
            {
                metalinkAddon = GetMetaLinkAddon(project);
            }
            if (await metalinkAddon.EstablishConnection())
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
                                WindowStyle = ProcessWindowStyle.Hidden,
                                CreateNoWindow = true,
                                UseShellExecute = false,
                                RedirectStandardError = true,
                                RedirectStandardOutput = true,
                            };
#if DEBUG
                            // startInfo.Arguments += String.Format(" -r \"{0}\"", Path.Combine(Path.GetTempPath, "metalink_recordedmessages.mlp");
#endif
                            Process metalink = new Process();
                            metalink.StartInfo = startInfo;


                            string logPath = Path.Combine(ProjectDirectory, "log", "MetaLinkBridge_" + Process.GetCurrentProcess().Id.ToString() + ".log");
                            Directory.CreateDirectory(Path.GetDirectoryName(logPath));
                            var log = new StreamWriter(logPath);
                            int streamsClosed = 0;
                            TaskCompletionSource<bool> bridgeListening = new TaskCompletionSource<bool>();

                            DataReceivedEventHandler handler = (sender, e) =>
                            {
                                lock (log)
                                {
                                    if (e.Data == null)
                                    {
                                        streamsClosed += 1;
                                        if (streamsClosed == 2)
                                        {
                                            log.Close();
                                            log.Dispose();
                                        }
                                        return;
                                    }
                                    if (bridgeListening.Task.IsCompleted == false && e.Data.Contains("AssemblyDesignBridgeServer started and listening"))
                                    {
                                        bridgeListening.SetResult(true);
                                    }
                                    log.WriteLine(e.Data);
                                    // log.Flush();
                                }
                            };
                            metalink.OutputDataReceived += handler;
                            metalink.ErrorDataReceived += handler;

                            metalink.Start();
                            metalink.BeginErrorReadLine();
                            metalink.BeginOutputReadLine();

                            if (Task.WaitAny(new Task[] { bridgeListening.Task }, 20 * 1000) == -1)
                            {
                                // throw new TimeoutException();
                            }
                            // n.b. ensure that even if GME is killed or crashes, MetaLinkBridge will exit
                            // this is important because we are reading its output, and it may block when we are not around to read it
                            IntPtr job = JobObjectPinvoke.AssignProcessToKillOnCloseJob(metalink);

                            for (int i = 0; i < 100; i++)
                            {
                                if (await metalinkAddon.EstablishConnection())
                                {
                                    connected = true;
                                    break;
                                }
                                await Task.Delay(100);
                            }
                            if (!connected)
                            {
                                if (GMEConsole != null)
                                {
                                    GMEConsole.Error.WriteLine("Failed to start MetaLink Bridge.");
                                }
                                if (metalink.HasExited)
                                {
                                    metalink.Kill();
                                }
                                metalink.Dispose();
                                JobObjectPinvoke.CloseHandle(job);
                            }
                            else
                            {
                                metalinkAddon._metalinkBridge = metalink;
                                metalinkAddon._metalinkBridgeJob = job;
                            }
                        }
                    }
                    else
                    {
                        if (GMEConsole != null)
                        {
                            GMEConsole.Error.WriteLine("Meta-Link Bridge executable is missing. Please check that Meta-Link Bridge is installed.");
                        }
                    }
                }
            }
            if (connected)
            {
                if (GMEConsole != null)
                {
                    GMEConsole.Info.WriteLine("Established connection to MetaLink Bridge.");
                }
            }
            return connected;
        }

        public void StartAssemblySync(MgaProject project, MgaFCO currentobj, int param)
        {
            // FIXME: possible race between sending MetaLink Bridge INTEREST and it registering it, and Creo sending its INTEREST
            // n.b. the tests call StartAssemblySync directly
            ProjectDirectory = Path.GetDirectoryName(project.ProjectConnStr.Substring("MGA=".Length));
            if (ConnectToMetaLinkBridgeTask == null)
            {
                ConnectToMetaLinkBridge(project, param);
            }
            metalinkAddon.Enable(true);

            CyPhyML.ComponentAssembly topasm = null;
            MgaGateway.PerformInTransaction(delegate
            {
                topasm = Run(project, currentobj, param);
                if (topasm != null)
                {
                    metalinkAddon.AssemblyID = topasm.Guid.ToString();
                }
            });

            ConnectToMetaLinkBridgeTask.Wait();
            if (ConnectToMetaLinkBridgeTask.Result == false)
            {
                return;
            }

            if (topasm == null)
            {
                metalinkAddon.AssemblyID = null;
                GMEConsole.Error.WriteLine("No top level assembly can be found. Open an assembly and try again.");
                return;
            }
            try
            {
                metalinkAddon.CallCyPhy2CADWithTransaction(project,
                                (topasm.Impl as MgaFCO),
                                param);
            }
            catch (Exception e)
            {
                metalinkAddon.syncedComponents.Remove(metalinkAddon.AssemblyID);
                // FIXME: propagateAddon.LastStartedInstance.Remove
                metalinkAddon.AssemblyID = null;
                GMEConsole.Error.Write(e.Message);
            }
        }

        private static CyPhyMetaLink.CyPhyMetaLinkAddon GetMetaLinkAddon(MgaProject project)
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

            string workingDir = "MetaLink_" + DateTime.Now.ToString("MMddHHmmss");
            CyPhyMetaLink.SyncedComponentData cdata = new CyPhyMetaLink.SyncedComponentData()
            {
                Type = SyncedComponentData.EditType.Design,
                WorkingDir = Path.Combine(ProjectDirectory, "metalink",  workingDir),
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

            metalinkAddon.syncedComponents.Add(cdata.Id, cdata);

            return topasm;
        }

        string ProjectDirectory;
    }
}
