using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
using GME.CSharp;
using GME;
using GME.MGA;
using GME.MGA.Core;
using System.Linq;
using Microsoft.Win32;

using Common = ISIS.GME.Common;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using ISIS.GME.Common.Interfaces;
using System.Windows.Forms;
using CyPhySignalBlocksAddOn;



namespace CyPhySignalBlocksAddOn
{
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyPhySignalBlocksAddOnAddon : IMgaComponentEx, IGMEVersionInfo, IMgaEventSink
    {

        private MgaAddOn addon;
        private bool componentEnabled = true;
        private bool firstTime = false;

        private LibraryInfo QudtLibraryInfo;
        private LibraryInfo PortLibraryInfo;
        private LibraryInfo MaterialLibraryInfo;
        private LibraryInfo CADResourceLibraryInfo;

        private string metaPath;
        private List<string> libraryPaths = new List<string>();

        private GMEConsole GMEConsole { get; set; }
        private MgaProject project;


        private delegate void TimerLogicDelegate();

        // test code
        private class LibraryInfo
        {
            public bool attachedLibrary = false;
            private string displayName;
            public string DisplayName { get { return displayName; } }
            private string mgaName;
            public string MgaName { get { return mgaName; } }
            private LibraryTimer timer;
            public void TimerGo()
            {
                timer.go();
            }

            public LibraryInfo(string mga, string display, TimerLogicDelegate f, IMgaProject project)
            {
                mgaName = mga;
                displayName = display;
                timer = new LibraryTimer(mgaName,
                                         f,
                                         project);
            }
        }

        private class LibraryTimer
        {

            private string storeName;
            public string Name { get { return storeName; } }
            private bool setupTimer = false;
            private System.Windows.Forms.Timer timer = new System.Windows.Forms.Timer();
            private TimerLogicDelegate callback;
            private IMgaProject project;

            public LibraryTimer(string name, TimerLogicDelegate f, IMgaProject project)
            {
                callback = f;
                storeName = name;
                this.project = project;
            }

            public void TimerEventProcessor(Object obj, EventArgs evtArgs)
            {
                if ((project.ProjectStatus & 8) == 0) // if not in transaction
                {
                    timer.Stop();
                    callback();
                }
            }

            public void go()
            {
                if (!setupTimer)
                {
                    timer.Tick += new EventHandler(TimerEventProcessor);
                    timer.Interval = 1;
                    timer.Start();
                    setupTimer = true;
                }
                //else
                //{
                //timer.Enabled = true; // restart the timer -- unused right now because library refresh is not useful
                //}
            }
        }

        public CyPhySignalBlocksAddOnAddon()
        {

        }

        // Event handlers for addons
        #region MgaEventSink members
        public void GlobalEvent(globalevent_enum @event)
        {
            if (@event == globalevent_enum.GLOBALEVENT_CLOSE_PROJECT)
            {
                Marshal.FinalReleaseComObject(addon);
                addon = null;
            }
            if (!componentEnabled)
            {
                return;
            }
            if (@event == globalevent_enum.GLOBALEVENT_OPEN_PROJECT)
            {
                if (Environment.Is64BitProcess)
                {
                    MessageBox.Show("You're running GME (64 bit). Many CyPhyML components do not work with GME (64 bit). Please start GME instead.");
                }
            }
            if (@event == (globalevent_enum)11 /* GLOBALEVENT_OPEN_PROJECT_FINISHED */)
            {
                // Store all library path names that will be attached
                // Nothing will be attached for these projects based on http://escher.isis.vanderbilt.edu/JIRA/browse/META-1559
                // Don't attach QUDT or PortLib to themselves
                libraryPaths.Add(Path.Combine(metaPath, QudtLibraryInfo.MgaName + ".mga"));
                libraryPaths.Add(Path.Combine(metaPath, PortLibraryInfo.MgaName +  ".mga"));
                libraryPaths.Add(Path.Combine(metaPath, MaterialLibraryInfo.MgaName + ".mga"));
                libraryPaths.Add(Path.Combine(metaPath, CADResourceLibraryInfo.MgaName + ".mga"));

                TriggerQudtRefreshIfNeeded();
            }
            if (@event == globalevent_enum.GLOBALEVENT_COMMIT_TRANSACTION)
            {
                CollapseFoldersWithSameNames();
            }

            if (@event == globalevent_enum.APPEVENT_XML_IMPORT_END)
            {
                // System.Diagnostics.Trace.WriteLine("XML import end event");

                needFolderCollapse = true;
                CollapseFoldersWithSameNames();
            }

            // System.Diagnostics.Trace.WriteLine(@event.ToString());
            // TODO: Handle global events
            // MessageBox.Show(@event.ToString());
        }

        private bool needFolderCollapse = false;

        private void CollapseFoldersWithSameNames()
        {
            if (needFolderCollapse == false)
            {
                return;
            }

            if (addon == null)
            {
                // this method can be called as a delegate after the timer expires
                // if the xml import fails addon will be null so we have nothing to 
                // do in that case.

                // this prevents a null reference exception on addon.Project line below
                return;
            }


            if ((addon.Project.ProjectStatus & 8) != 0)
            {
                System.Windows.Forms.Timer timer = new System.Windows.Forms.Timer();
                timer.Tick += (sender, args) =>
                {
                    this.CollapseFoldersWithSameNames();
                    timer.Stop();
                    timer.Dispose();
                };
                timer.Interval = 1;
                timer.Start();
                return;
            }

            addon.Project.BeginTransactionInNewTerr();
            try
            {
                needFolderCollapse = false;
                GMEConsole.Info.WriteLine("Collapse duplicate folder names into one...");
                this.CollapseFolders(this.project.RootFolder);
                this.CollapseLibraries(this.project.RootFolder);
                GMEConsole.Info.WriteLine("Removing empty duplicated folders...");
                this.RemoveEmptyDuplicatedFolders(this.project.RootFolder);
                GMEConsole.Info.WriteLine("Done.");
            }
            catch (Exception e)
            {
                GMEConsole.Info.WriteLine("Exception occurred while collapsing folders: " + e.Message);
                addon.Project.AbortTransaction();
                return;
            }
            addon.Project.CommitTransaction();
        }

        private void CollapseLibraries(MgaFolder mgaFolder)
        {
            LibraryInfo[] infos = new LibraryInfo[] { this.QudtLibraryInfo, this.PortLibraryInfo, this.MaterialLibraryInfo, this.CADResourceLibraryInfo};
            var libraries = mgaFolder
                .ChildFolders
                .Cast<MgaFolder>()
                .Where(x => string.IsNullOrEmpty(x.LibraryName) == false)
                .GroupBy(lib => infos.Where(inf => lib.Name.Contains(inf.DisplayName)).FirstOrDefault())
                .ToList();

            foreach (IGrouping<LibraryInfo, MgaFolder> group in libraries.Where(gr => gr.Key != null))
            {
                var orderedLibraries = group.OrderBy(f => f.RelID);
                var library = orderedLibraries.First();
                foreach (var dup in orderedLibraries.Skip(1))
                {
                    ReferenceSwitcher.Switcher sw = new ReferenceSwitcher.Switcher(dup, library, null);
                    sw.UpdateSublibrary();
                    dup.DestroyObject();
                }
            }
        }

        private void RemoveEmptyDuplicatedFolders(MgaFolder mgaFolder)
        {
            foreach (MgaFolder childFolder in mgaFolder.ChildFolders)
            {
                if (childFolder.IsLibOrLibObject() == false)
                {
                    this.RemoveEmptyDuplicatedFolders(childFolder);
                }
            }

            var uniqueNames = mgaFolder
                .ChildFolders
                .Cast<MgaFolder>()
                .Where(x => x.IsLibOrLibObject() == false)
                .Select(x => x.Name)
                .Distinct()
                .ToList();

            foreach (var uniqueFolderName in uniqueNames)
            {
                var firstObj = mgaFolder.ChildFolders.Cast<MgaFolder>().Where(x => x.IsLibOrLibObject() == false).Where(x => x.Name == uniqueFolderName).FirstOrDefault();
                var foldersToDelete = mgaFolder.ChildFolders.Cast<MgaFolder>().Where(x => x.IsLibOrLibObject() == false).Where(x => x != firstObj).Where(x => x.Name == uniqueFolderName).Where(x => x.ChildObjects.Count == 0).ToList();

                foldersToDelete.ForEach(x => x.DestroyObject());
            }
        }

        private void CollapseFolders(MgaFolder mgaFolder)
        {
            Func<IMgaObject, Tuple<string, string>> nameAndKind = x => new Tuple<string, string>(x.Name, x.MetaBase.Name);
            var uniqueNames = mgaFolder
                .ChildFolders
                .Cast<MgaFolder>()
                .Where(x => x.IsLibOrLibObject() == false)
                .Where(x => x.MetaBase.Name != "RootFolder")
                .GroupBy(x => nameAndKind(x))
                .ToList();

            foreach (var group in uniqueNames.Where(x => x.Any()))
            {
                var firstObj = group.First();
                var foldersToMove = group.Skip(1).SelectMany(x => x.ChildFolders.Cast<MgaFolder>()).Where(x => x.IsLibOrLibObject() == false);
                var fcosToMove = group.Skip(1).SelectMany(x => x.ChildFCOs.Cast<MgaFCO>()).Where(x => x.IsLibObject == false);

                //GMEConsole.Info.WriteLine(" -- {0}", firstObj.AbsPath);

                foreach (var item in foldersToMove)
                {
                    if (item.IsLibOrLibObject())
                    {
                        GMEConsole.Error.WriteLine(" - Lib object! {0}", item.AbsPath);
                        continue;
                    }
                    var moved = firstObj.MoveFolderDisp(item);
                    //GMEConsole.Info.WriteLine(" ----- {0} -> {1}", item.AbsPath, moved.AbsPath);
                }

                foreach (var item in fcosToMove)
                {
                    if (item.IsLibObject)
                    {
                        GMEConsole.Error.WriteLine(" - Lib object! {0}", item.AbsPath);
                        continue;
                    }
                    var moved = firstObj.MoveFCODisp(item);
                    //GMEConsole.Info.WriteLine(" ----- {0} -> {1}", item.AbsPath, moved.AbsPath);
                }

            }

            foreach (MgaFolder childFolder in mgaFolder.ChildFolders)
            {
                if (childFolder.IsLibOrLibObject() == false)
                {
                    this.CollapseFolders(childFolder);
                }
            }
        }

        // Look up the value of the META_PATH variable
        public string GetMetaPathValue()
        {
            const string keyName = "Software\\META";

            RegistryKey metaKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry32)
                .OpenSubKey(keyName, false);

            using (metaKey)
            {
                string metaPath = "C:\\Program Files (x86)\\META";
                if (metaKey != null)
                {
                    metaPath = (string)metaKey.GetValue("META_PATH", metaPath);
                }

                return metaPath;
            }
        }


        /// <summary>
        /// Called when an FCO or folder changes
        /// </summary>
        /// <param name="subject">the object the event(s) happened to</param>
        /// <param name="eventMask">objectevent_enum values ORed together</param>
        /// <param name="param">extra information provided for cetertain event types</param>
        public void ObjectEvent(MgaObject subject, uint eventMask, object param)
        {
            // TODO: Handle object events (OR eventMask with the members of objectevent_enum)
            // Warning: Only those events are received that you have subscribed for by setting ComponentConfig.eventMask
            if (GMEConsole == null)
            {
                GMEConsole = GMEConsole.CreateFromProject(project);
            }
            TriggerQudtRefreshIfNeeded();
        }

        private void TriggerQudtRefreshIfNeeded()
        {
            if (!firstTime)
            {
                // put the arguments into private variables where they can be reached by the timer code
                if (GMEConsole == null)
                {
                    GMEConsole = GMEConsole.CreateFromProject(project);
                }
#if DEBUG
                GMEConsole.Info.WriteLine("CyPhySignalBlocksAddOn starting up...");
#endif

                //GMEConsole.Info.WriteLine(eventMask.ToString());
                if (!componentEnabled)
                {
                    GMEConsole.Info.WriteLine("CyPhySignalBlocksAddOn not enabled...");
                    return;
                }

                // First, check to see whether the libraries have already been loaded
                //CyPhy.RootFolder rootFolder = ISIS.GME.Common.Utils.CreateObject<CyPhyClasses.RootFolder>(rf as MgaObject);

                IMgaFolder rootFolder = project.RootFolder;
                IMgaFolders subFolders = rootFolder.ChildFolders;

                // META-1320: refactored some
                // Run this on any event, but only once (if not already loaded)
                if (!QudtLibraryInfo.attachedLibrary)   //if (!attachedQudtLibrary)
                {
                    QudtLibraryInfo.TimerGo();
                }

                if (!PortLibraryInfo.attachedLibrary)   //if (!attachedPortLibrary)
                {
                    PortLibraryInfo.TimerGo(); // portLibTimer.go();
                }

                if (!MaterialLibraryInfo.attachedLibrary)   // if (!attachedMaterialLibrary)
                {
                    MaterialLibraryInfo.TimerGo();
                }
                if (!CADResourceLibraryInfo.attachedLibrary)
                {
                    CADResourceLibraryInfo.TimerGo();
                }

                firstTime = true;
            }
        }
        
        public void QudtTimerHandler()
        {
            AttachLibrary(QudtLibraryInfo);
        }

        public void PortLibTimerHandler()
        {
            AttachLibrary(PortLibraryInfo);
        }

        public void MaterialLibTimerHandler()
        {
            AttachLibrary(MaterialLibraryInfo);
        }

        public void CADResourceLibTimerHandler()
        {
            AttachLibrary(CADResourceLibraryInfo);
        }

        private readonly int PROJECT_STATUS_OPEN = 0x1;

        private void AttachLibrary(LibraryInfo libraryInfo)
        {
            string mgaPath = metaPath + "\\" + libraryInfo.MgaName + ".mga";

            if (!libraryInfo.attachedLibrary &&
                (project.ProjectStatus & PROJECT_STATUS_OPEN) == PROJECT_STATUS_OPEN)
            {
                if (!File.Exists(mgaPath))
                {
                    GMEConsole.Error.WriteLine("Path '" + mgaPath + "' does not exist. Cannot attach " + libraryInfo.MgaName);
                    return;
                }

                project.Notify(globalevent_enum.APPEVENT_LIB_ATTACH_BEGIN);
                try
                {
                    project.BeginTransaction(project.ActiveTerritory);

                    IMgaFolder oldLibFolder = project.RootFolder.ChildFolders.Cast<IMgaFolder>()
                        .Where(x => string.IsNullOrWhiteSpace(x.LibraryName) == false && (x.Name.Contains(libraryInfo.DisplayName) || x.Name.Contains(libraryInfo.DisplayName))).FirstOrDefault();

                    bool needAttach;
                    if (oldLibFolder == null)
                    {
                        needAttach = true;

                        if (libraryPaths.Contains(Path.GetFullPath(project.ProjectConnStr.Substring("MGA=".Length))))
                        {
                            // Don't attach libraries to themselves
                            needAttach = false;
                        }
                    }
                    else
                    {
                        DateTime oldModTime;
                        long loldModTime;
                        if (long.TryParse(oldLibFolder.RegistryValue["modtime"], out loldModTime))
                        {
                            oldModTime = DateTime.FromFileTimeUtc(loldModTime);
                        }
                        else
                        {
                            oldModTime = DateTime.MinValue;
                        }
                        needAttach = File.GetLastWriteTimeUtc(mgaPath).CompareTo(oldModTime) > 0;
                        if (!needAttach)
                        {
                            GMEConsole.Info.WriteLine("Library is up-to-date: embedded library modified " + oldModTime.ToString() +
                                ", " + libraryInfo.MgaName + " modified " + File.GetLastWriteTimeUtc(mgaPath).ToString());
                        }
                    }

                    if (needAttach)
                    {
                        MgaProject proj = (MgaProject) Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaProject"));
                        int mgaVersion;
                        string paradigmName;
                        string paradigmVersion;
                        object paradigmGuid;
                        bool readOnly;
                        proj.QueryProjectInfo("MGA=" + mgaPath, out mgaVersion, out paradigmName, out paradigmVersion, out paradigmGuid, out readOnly);
                        
                        Guid guidP1 = ConvertToGUID(paradigmGuid);
                        Guid guidP2 = ConvertToGUID(project.RootMeta.GUID);
                        bool guidsEqual = guidP1.Equals(guidP2);

                        if (paradigmName != project.MetaName || !guidsEqual)
                        {
                            GMEConsole.Info.WriteLine("Skipping refresh of " + libraryInfo.DisplayName + " because it uses a different metamodel version than the current project.");
                            throw new Exception();
                        }

                        GMEConsole.Info.WriteLine("Attaching library " + mgaPath);
                        RootFolder newLibFolder = Common.Classes.RootFolder.GetRootFolder(project).AttachLibrary("MGA=" + mgaPath);
                        DateTime modtime = File.GetLastWriteTimeUtc(mgaPath);
                        ((newLibFolder as ISIS.GME.Common.Classes.RootFolder).Impl as GME.MGA.IMgaFolder).RegistryValue["modtime"] =
                             modtime.ToFileTimeUtc().ToString();

                        if (oldLibFolder != null)
                        {
                            ReferenceSwitcher.Switcher sw = new ReferenceSwitcher.Switcher(oldLibFolder, newLibFolder.Impl, null);
                            sw.UpdateSublibrary();
                            oldLibFolder.DestroyObject();
                        }
                        ((newLibFolder as ISIS.GME.Common.Classes.RootFolder).Impl as GME.MGA.IMgaFolder).LibraryName = libraryInfo.DisplayName;
                        GMEConsole.Info.WriteLine((oldLibFolder == null ? "Attached " : "Refreshed ") + libraryInfo.MgaName + ".mga library.");
                    }
                    project.CommitTransaction();
                }
                catch
                {
                   project.AbortTransaction();
                }
                finally
                {
                    project.Notify(globalevent_enum.APPEVENT_LIB_ATTACH_END);
                }

                libraryInfo.attachedLibrary = true;
            }
        }
        

        #endregion

        #region IMgaComponentEx Members

        private static Guid ConvertToGUID(object guidObj)
        {
            var array = (Array)guidObj;
            byte[] bytearray = new byte[16];
            array.CopyTo(bytearray, 0);

            var guid = new Guid(bytearray);
            return guid;
        }
        

        public void Initialize(MgaProject p)
        {
            // Creating addon
            p.CreateAddOn(this, out addon);
            // Setting event mask (see ComponentConfig.eventMask)
            unchecked
            {
                addon.EventMask = (uint)ComponentConfig.eventMask;
            }
            this.project = p;
            if (metaPath == null)
            {
                metaPath = Path.Combine(GetMetaPathValue(), "meta");
            }

            if (!Directory.Exists(metaPath))
            {
                throw new ApplicationException(metaPath + " doesn't exist");
            }
            //qudtTimer = new LibraryTimer("QudtTimer", new TimerLogicDelegate(QudtTimerHandler), project);
            //portLibTimer = new LibraryTimer("PortLibTimer", new TimerLogicDelegate(PortLibTimerHandler), project);
            QudtLibraryInfo = new LibraryInfo("CyPhyMLQudt", "UnitLibrary QUDT", new TimerLogicDelegate(QudtTimerHandler), project);
            PortLibraryInfo = new LibraryInfo("CyPhy_PortLib", "PortLibrary CyPhy_PortLib", new TimerLogicDelegate(PortLibTimerHandler), project);        
            MaterialLibraryInfo = new LibraryInfo("CyPhy_MaterialLib", "MaterialLibrary CyPhy_MaterialLib", new TimerLogicDelegate(MaterialLibTimerHandler), project);
            CADResourceLibraryInfo = new LibraryInfo("CyPhy_CADResourceLib", "CADResourceLibrary", new TimerLogicDelegate(CADResourceLibTimerHandler), project);
        }

        public void InvokeEx(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, int param)
        {
            throw new NotImplementedException(); // Not called by addon
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

    public static class MgaObjectLibrary
    {
        public static bool IsLibOrLibObject(this IMgaObject o)
        {
            if (o is IMgaFolder)
            {
                return string.IsNullOrEmpty(((IMgaFolder)o).LibraryName) == false || o.IsLibObject;
            }
            else
            {
                return o.IsLibObject;
            }
        }

        public static bool IsLibOrLibObject(this MgaFolder o)
        {
            return IsLibOrLibObject(((IMgaObject)o));
        }
    }
}
