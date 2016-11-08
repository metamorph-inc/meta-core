using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;
using System.Runtime.InteropServices;
using System.IO;
using GME.CSharp;
using GME.MGA;
using GME;
using GME.MGA.Core;
using META;

using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;

namespace META
{
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class ComponentLibraryManagerAddOn : IMgaComponentEx, IGMEVersionInfo, IMgaEventSink, IDisposable
    {

        private MgaAddOn addon;
        private bool componentEnabled = true;
        private bool handleEvents = true;
        private bool xmeImportInProgress = false;
        CyPhyGUIs.GMELogger Logger { get; set; }
        private Dictionary<String, String> lastObservedPaths = new Dictionary<String, String>();

        public void Dispose()
        {
            if (Logger != null)
            {
                Logger.Dispose();
                Logger = null;
            }
        }

        // Event handlers for addons
        #region MgaEventSink members
        public void GlobalEvent(globalevent_enum @event)
        {
            if (@event == globalevent_enum.GLOBALEVENT_OPEN_PROJECT)
            {
                GetMetaRefs(this.addon.Project);
            }
            if (@event == globalevent_enum.GLOBALEVENT_CLOSE_PROJECT)
            {
                if (Logger != null)
                {
                    Logger.Dispose();
                    Logger = null;
                }
                Marshal.FinalReleaseComObject(addon);
                addon = null;
            }
            if (@event == globalevent_enum.APPEVENT_XML_IMPORT_BEGIN)
            {
                xmeImportInProgress = true;
                addon.EventMask = unchecked((uint)objectevent_enum.OBJEVENT_CREATED);
            }
            else if (@event == globalevent_enum.APPEVENT_XML_IMPORT_END)
            {
                unchecked { addon.EventMask = (uint)ComponentConfig.eventMask; }
                xmeImportInProgress = false;
            }
            else if (@event == globalevent_enum.APPEVENT_LIB_ATTACH_BEGIN)
            {
                addon.EventMask = 0;
                handleEvents = false;
            }
            else if (@event == globalevent_enum.APPEVENT_LIB_ATTACH_END)
            {
                unchecked { addon.EventMask = (uint)ComponentConfig.eventMask; }
                handleEvents = true;
            }
            if (!componentEnabled)
            {
                return;
            }

            // TODO: Handle global events
            // MessageBox.Show(@event.ToString());
        }

        /// <summary>
        /// Called when an FCO or folder changes
        /// </summary>
        /// <param name="subject">the object the event(s) happened to</param>
        /// <param name="eventMask">objectevent_enum values ORed together</param>
        /// <param name="param">extra information provided for cetertain event types</param>
        public void ObjectEvent(MgaObject subject, uint eventMask, object param)
        {
            if (!componentEnabled)
            {
                return;
            }
            if (!handleEvents)
            {
                return;
            }
            uint uOBJEVENT_CREATED = 0;
            unchecked { uOBJEVENT_CREATED = (uint)objectevent_enum.OBJEVENT_CREATED; }
            bool objectCreatedAndNotLibObject = (eventMask & uOBJEVENT_CREATED) != 0 && (eventMask & (uint)objectevent_enum.OBJEVENT_DESTROYED) == 0 && subject.IsLibObject == false;
            if (xmeImportInProgress)
            {
                if (objectCreatedAndNotLibObject && subject.MetaBase.MetaRef == componentAssemblyMetaRef && componentAssemblyPathMetaRef != 0)
                {
                    string path = ((IMgaFCO)subject).StrAttrByName["Path"];
                    if (path == "")
                    {
                        ((IMgaFCO)subject).StrAttrByName["Path"] = GetRandomComponentAssemblyDir();
                    }
                }
                return;
            }

            // TODO: Handle object events (OR eventMask with the members of objectevent_enum)
            // Warning: Only those events are received that you have subscribed for by setting ComponentConfig.eventMask

            // If the event is OBJEVENT_DESTROYED, most operations on subject will fail
            //   Safe operations: getting Project, ObjType, ID, MetaRole, Meta, MetaBase, Name, AbsPath
            //   Operations that will fail: all others, including attribute access and graph navigation
            //     Try handling OBJEVENT_PRE_DESTROYED if these operations are necessary

            // Be careful not to modify Library objects (check subject.IsLibObject)

            // MessageBox.Show(eventMask.ToString());
            // GMEConsole.Out.WriteLine(subject.Name);

            // attribute was changed and object was neither created nor destroyed
            bool attributeChanged = (eventMask & (uint)objectevent_enum.OBJEVENT_ATTR) != 0 &&
                (eventMask & uOBJEVENT_CREATED) == 0 &&
                (eventMask & (uint)objectevent_enum.OBJEVENT_DESTROYED) == 0;

            if (objectCreatedAndNotLibObject)
            {
                MgaObject parent;
                GME.MGA.Meta.objtype_enum objType;
                subject.GetParent(out parent, out objType);
                // Object was created
                if (subject.MetaBase.Name == "Component")
                {
                    // Check that parent is a Components folder
                    if (objType == GME.MGA.Meta.objtype_enum.OBJTYPE_FOLDER
                        && parent.MetaBase.Name == "Components")
                    {
                        Process(CyPhyClasses.Component.Cast(subject));
                    }
                }
                else if (subject.MetaBase.Name == "ComponentAssembly")
                {
                    Process(CyPhyClasses.ComponentAssembly.Cast(subject));
                }
            }
            else if (attributeChanged && subject.IsLibObject == false)
            {
                MgaObject parent;
                GME.MGA.Meta.objtype_enum objType;
                subject.GetParent(out parent, out objType);
                if (subject.MetaBase.Name == "Component")
                {
                    string oldPath = null;
                    // Check that parent is a Components folder
                    if (objType == GME.MGA.Meta.objtype_enum.OBJTYPE_FOLDER
                        && parent.MetaBase.Name == "Components")
                    {
                        if (param != null)
                        {
                            var parameters = ((object[])param).ToList();
                            int pathAttr = parameters.IndexOf("ATTR:Path");
                            if (pathAttr != -1)
                            {
                                oldPath = (string)parameters[pathAttr + 1];
                                RenameComponentDirectory(CyPhyClasses.Component.Cast(subject), oldPath);
                            }
                        }
                    }
                }

            }
        }

        private void RenameComponentDirectory(CyPhy.Component component, string oldPath)
        {
            if (string.IsNullOrEmpty(oldPath) || Path.GetFullPath(oldPath) == Path.GetFullPath(component.Attributes.Path))
            {
                return;
            }

            // Check to see if this "changed" path is the same one we saw last time we were in this function, post-normalization.
            // If so, then don't consider this a rename, and return.
            var componentGuid = component.Guid.ToString();
            String lastPath;
            if (lastObservedPaths.TryGetValue(componentGuid, out lastPath) && lastPath.Equals(component.Attributes.Path))
            {
                return;
            }

            var project = component.Impl.Project;
            var projectRoot = Path.GetFullPath(project.GetRootDirectoryPath());
            if (Directory.Exists(Path.Combine(projectRoot, component.Attributes.Path)))
            {
                var console = GMEConsole.CreateFromProject(project);
                if (console.gme != null)
                {
                    console.Error.WriteLine(String.Format("Cannot move component directory to {0}: this directory already exists.", component.Attributes.Path));
                    Marshal.FinalReleaseComObject(console.gme);
                }
                Marshal.ThrowExceptionForHR(-2023391233); // E_MGA_CONSTRAINT_VIOLATION	= 0x87657fff
            }

            if (Directory.Exists(Path.Combine(projectRoot, oldPath)))
            {
                // Normalize path syntax
                if (component.Attributes.Path.Contains("\\"))
                {
                    component.Attributes.Path = component.Attributes.Path.Replace("\\", "/");
                }
                if (component.Attributes.Path.EndsWith("/") == false)
                {
                    component.Attributes.Path = component.Attributes.Path + "/";
                }
                var pathWithoutTrailingSlash = Path.GetDirectoryName(component.Attributes.Path);

                // Directory.CreateDirectory(Path.Combine(projectRoot, Path.GetDirectoryName(pathWithoutTrailingSlash)));
                try
                {
                    Directory.Move(Path.Combine(projectRoot, oldPath), Path.Combine(projectRoot, pathWithoutTrailingSlash));
                }
                catch (IOException e)
                {
                    var console = GMEConsole.CreateFromProject(project);
                    if (console.gme != null)
                    {
                        string msg = String.Format("Cannot move component directory {0}.", oldPath);
                        const int E_SHARING_VIOLATION = unchecked((int)0x80070020);
                        const int E_ACCESS_DENIED = unchecked((int)0x80070005);
                        if (Marshal.GetHRForException(e) == E_SHARING_VIOLATION || Marshal.GetHRForException(e) == E_ACCESS_DENIED)
                        {
                            msg = msg + " Check that the folder and files it contains are not open in another program.";
                        }
                        console.Info.WriteLine(msg);
                        //Marshal.FinalReleaseComObject(console.gme);
                    }

                    throw;
                }
                {
                    var console = GMEConsole.CreateFromProject(project);
                    if (console.gme != null)
                    {
                        console.Info.WriteLine(String.Format("Moved component directory from {0} to {1}", oldPath, component.Attributes.Path));
                        //Marshal.FinalReleaseComObject(console.gme);
                    }
                }
            }

            lastObservedPaths.Add(componentGuid, component.Attributes.Path);
        }

        private IEnumerable<CyPhy.ComponentAssembly> GetAllComponentAssemblys(IMgaProject project, bool includeLibraries = false)
        {
            var filter = project.CreateFilter();
            filter.Kind = "ComponentAssembly";

            int componentsId = project.RootMeta.RootFolder.GetDefinedFolderByNameDisp("Components", true).MetaRef;
            int componentAssembliesId = project.RootMeta.RootFolder.GetDefinedFolderByNameDisp("ComponentAssemblies", true).MetaRef;

            IEnumerable<MgaFolder> rootFolders = new MgaFolder[] { project.RootFolder };
            if (includeLibraries)
            {
                rootFolders = rootFolders.Concat(
                    project.RootFolder.ChildFolders.Cast<MgaFolder>().Where(f => String.IsNullOrEmpty(f.LibraryName) == false));
            }
            foreach (MgaFolder rootFolder in rootFolders)
            {
                foreach (MgaFolder folder in rootFolder.ChildFolders)
                {
                    int metaRef = folder.MetaBase.MetaRef;
                    if (metaRef == componentsId || metaRef == componentAssembliesId)
                    {
                        foreach (IMgaObject fco in folder.GetDescendantFCOs(filter))
                        {
                            yield return CyPhyClasses.ComponentAssembly.Cast(fco);
                        }
                    }
                }
            }
        }

        public static void CopyDirectory(string sourcePath, string destPath)
        {
            if (!Directory.Exists(destPath))
            {
                Directory.CreateDirectory(destPath);
            }

            foreach (string file in Directory.GetFiles(sourcePath))
            {
                string dest = Path.Combine(destPath, Path.GetFileName(file));
                File.Copy(file, dest);
            }

            foreach (string folder in Directory.GetDirectories(sourcePath))
            {
                string dest = Path.Combine(destPath, Path.GetFileName(folder));
                CopyDirectory(folder, dest);
            }
        }

        private static string GetRandomComponentAssemblyDir()
        {
            return ComponentLibraryManager.GetRandomComponentAssemblyDir();
        }

        private void Process(CyPhy.ComponentAssembly componentAssembly)
        {
            if (Logger == null)
            {
                Logger = new CyPhyGUIs.GMELogger(componentAssembly.Impl.Project, this.ComponentName);
            }

            if (String.IsNullOrWhiteSpace(componentAssembly.Attributes.Path))
            {
                componentAssembly.Attributes.Path = GetRandomComponentAssemblyDir();
            }
            else
            {
                string originalPath = Path.Combine(componentAssembly.Impl.Project.GetRootDirectoryPath(), componentAssembly.Attributes.Path);
                componentAssembly.Attributes.Path = GetRandomComponentAssemblyDir();
                if (Directory.Exists(originalPath))
                {
                    try
                    {
                        CopyDirectory(originalPath, Path.Combine(componentAssembly.Impl.Project.GetRootDirectoryPath(), componentAssembly.Attributes.Path));
                    }
                    catch (Exception ex)
                    {
                        Logger.WriteError("Exception while copying to {0}: {1}", componentAssembly.Attributes.Path, ex);
                    }
                }
            }
        }

        #endregion

        #region Management Functions

        /// <summary>
        /// Determines what action to take if a Component has been created under a Components folder.
        /// The actions are dispatched to other functions.
        /// </summary>
        /// <param name="componentFco"></param>
        private void Process(CyPhy.Component component)
        {
            var componentFco = component.Impl;
            if (Logger == null)
            {
                Logger = new CyPhyGUIs.GMELogger(componentFco.Project, this.ComponentName);
            }

            bool HasAVMID = !String.IsNullOrWhiteSpace(component.Attributes.AVMID);
            bool HasPath = !String.IsNullOrWhiteSpace(component.Attributes.Path);

            META.ComponentLibraryManager.EnsureComponentFolder(component);

            // Get all other components that have a "Components" folder as parent.
            // For the next logic tests, we'll need this list.
            var project = component.Impl.Project;
            MgaFilter filter = project.CreateFilter();
            filter.Kind = "Component";

            var otherComponents = project.AllFCOs(filter)
                                         .Cast<MgaFCO>()
                                         .Where(x => x.ParentFolder != null
                                                  && x.ParentFolder.MetaBase.Name == "Components"
                                                  && x.ID != componentFco.ID)
                                         .Select(x => CyPhyClasses.Component.Cast(x));

            //bool AVMIDCollision = HasAVMID && otherComponents.Where(c => c.Attributes.AVMID == component.Attributes.AVMID).Any();

            bool PathCollision = HasPath;
            if (HasPath)
            {
                PathCollision = false;

                string myFullPath = Path.GetFullPath(component.Attributes.Path);
                foreach (var cFullPath in otherComponents.Where(c => !String.IsNullOrWhiteSpace(c.Attributes.Path))
                                                         .Select(c => Path.GetFullPath(c.Attributes.Path)))
                {
                    if (myFullPath == cFullPath)
                        PathCollision = true;
                }
            }

            //bool PathCollision = HasPath && otherComponents.Where(c => Path.GetFullPath(c.Attributes.Path) == Path.GetFullPath(component.Attributes.Path)).Any();

            var projectPath_Absolute = component.Impl.Project.GetRootDirectoryPath();
            var lastChar = projectPath_Absolute.Last();
            if (lastChar != '\\' && lastChar != '/')
                projectPath_Absolute += '\\';

            if (HasAVMID && HasPath)
            {
                var oldCompPath_Absolute = Path.Combine(projectPath_Absolute, component.Attributes.Path);
                var folderExists = Directory.Exists(oldCompPath_Absolute);

                if (PathCollision || !folderExists)
                {
                    var newCompPath_Absolute = ComponentLibraryManager.CreateComponentFolder(component.Impl.Project);
                    var newCompPath_Relative = ComponentLibraryManager.MakeRelativePath(projectPath_Absolute, newCompPath_Absolute);

                    newCompPath_Relative = newCompPath_Relative.Replace('\\', '/') + '/';
                    var firstChars = newCompPath_Relative.Substring(0, 2);
                    if (firstChars != ".\\" && firstChars != "./")
                        newCompPath_Relative = newCompPath_Relative.Insert(0, "./");
                    component.Attributes.Path = newCompPath_Relative;

                    if (folderExists)
                    {
                        CopyComponentFiles(oldCompPath_Absolute, newCompPath_Absolute);
                    }
                    else if (component.Children.ResourceCollection.Any())
                    {
                        WarnUserAboutMissingFiles(component);
                    }
                }
            }
            else if (HasAVMID)
            {
                var newCompPath_Absolute = ComponentLibraryManager.CreateComponentFolder(component.Impl.Project);
                var newCompPath_Relative = ComponentLibraryManager.MakeRelativePath(projectPath_Absolute, newCompPath_Absolute);

                newCompPath_Relative = newCompPath_Relative.Replace('\\', '/') + '/';
                component.Attributes.Path = newCompPath_Relative;

                if (component.Children.ResourceCollection.Any())
                {
                    WarnUserAboutMissingFiles(component);
                }
            }
            else if (HasPath)
            {
                var oldCompPath_Absolute = Path.Combine(projectPath_Absolute, component.Attributes.Path);
                var folderExists = Directory.Exists(oldCompPath_Absolute);

                if (PathCollision || !folderExists)
                {
                    var newCompPath_Absolute = ComponentLibraryManager.CreateComponentFolder(component.Impl.Project);
                    var newCompPath_Relative = ComponentLibraryManager.MakeRelativePath(projectPath_Absolute, newCompPath_Absolute);

                    newCompPath_Relative = newCompPath_Relative.Replace('\\', '/') + '/';
                    var firstChars = newCompPath_Relative.Substring(0, 2);
                    if (firstChars != ".\\" && firstChars != "./")
                        newCompPath_Relative = newCompPath_Relative.Insert(0, "./");
                    component.Attributes.Path = newCompPath_Relative;

                    if (folderExists)
                    {
                        CopyComponentFiles(oldCompPath_Absolute, newCompPath_Absolute);
                    }
                    else if (component.Children.ResourceCollection.Any())
                    {
                        WarnUserAboutMissingFiles(component);
                    }
                }
            }
            else
            {
                if (component.Children.ResourceCollection.Any())
                {
                    WarnUserAboutMissingFiles(component);
                }
            }
        }

        private void WarnUserAboutMissingFiles(CyPhy.Component component)
        {
            Logger.WriteWarning(string.Format(@"Warning: copied component {0} contains resource children, but
                                                its path is empty or invalid; its backing files are likely
                                                missing and should be copied manually.", component.ToHyperLink()));
        }

        private void CopyComponentFiles(string fromPath, string toPath)
        {
            if (false == (fromPath.EndsWith("/") || fromPath.EndsWith("\\")))
                fromPath += '/';

            // Just copy everything except the original ACM/XML file
            DirectoryInfo oldComponentDir = new DirectoryInfo(fromPath);
            foreach (FileInfo fi in oldComponentDir.GetFiles("*.*", SearchOption.AllDirectories))
            {
                if (fi.Name == "ComponentData.xml"
                    || fi.Name.Contains(".acm"))
                    continue;

                var filePath_RelativeToComponentRoot = META.ComponentLibraryManager.MakeRelativePath(fromPath, fi.FullName);
                var newFilePath = Path.Combine(toPath, filePath_RelativeToComponentRoot);

                try
                {
                    Directory.CreateDirectory(Path.GetDirectoryName(newFilePath));
                    fi.CopyTo(newFilePath);
                }
                catch (Exception ex)
                {
                    Logger.WriteError("Exception while copying {0}: {1}",
                                                                fi.Name,
                                                                ex);
                }
            }
        }

        #endregion

        #region IMgaComponentEx Members

        public void Initialize(MgaProject project)
        {
            // Creating addon
            project.CreateAddOn(this, out addon);
            // Setting event mask (see ComponentConfig.eventMask)
            unchecked
            {
                addon.EventMask = (uint)ComponentConfig.eventMask;
            }

            if (Logger == null)
            {
                Logger = new CyPhyGUIs.GMELogger(project, this.ComponentName);
            }
            GetMetaRefs(project);
        }

        private void GetMetaRefs(MgaProject project)
        {
            var componentAssemblyMeta = project.RootMeta.RootFolder.GetDefinedFCOByNameDisp("ComponentAssembly", true);
            componentAssemblyMetaRef = componentAssemblyMeta.MetaRef;
            try
            {
                componentAssemblyPathMetaRef = componentAssemblyMeta.GetAttributeByNameDisp("Path").MetaRef;
            }
            catch (COMException e)
            {
                componentAssemblyPathMetaRef = 0;
                const int E_NOTFOUND = unchecked((int)0x80731007);
                if (e.ErrorCode != E_NOTFOUND)
                {
                    throw;
                }

            }
        }
        int componentAssemblyMetaRef;
        int componentAssemblyPathMetaRef;

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

        public static void Enable(bool newval, MgaProject project)
        {
            var addon = project.AddOnComponents
                               .Cast<IMgaComponentEx>()
                               .Where(c => c.ComponentName == ComponentConfig.componentName);

            if (addon.FirstOrDefault() != null)
            {
                addon.First().Enable(newval);
            }
            else
            {
                // Silent fail
            }
        }

        public static Boolean GetEnabled(MgaProject project)
        {
            var addon = project.AddOnComponents
                              .Cast<IMgaComponentEx>()
                              .Where(c => c.ComponentName == ComponentConfig.componentName);

            if (addon.FirstOrDefault() != null)
            {
                return (addon.First() as ComponentLibraryManagerAddOn).enabled;
            }
            else
            {
                // Silent fail
                return false;
            }
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
