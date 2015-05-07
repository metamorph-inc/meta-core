using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;
using GME.MGA.Core;
using GME.CSharp;
using GME;
using GME.MGA;
using System.Linq;

namespace SubTreeMerge
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class SubTreeMergeInterpreter : IMgaComponentEx, IGMEVersionInfo
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

        [DllImport("user32.dll")]
        public static extern bool WaitMessage();

        /// <summary>
        /// The main entry point of the interpreter. A transaction is already open,
        /// gmeConsole is avaliable. A general try-catch block catches all the exceptions
        /// coming from this function, you don't need to add it. For more information, see InvokeEx.
        /// </summary>
        /// <param name="project">The handle of the project opened in GME, for which the interpreter was called.</param>
        /// <param name="currentobj">The model open in the active tab in GME. Its value is null if no model is open (no GME modeling windows open). </param>
        /// <param name="selectedobjs">
        /// A collection for the selected  model elements. It is never null.
        /// If the interpreter is invoked by the context menu of the GME Tree Browser, then the selected items in the tree browser. Folders
        /// are never passed (they are not FCOs).
        /// If the interpreter is invoked by clicking on the toolbar icon or the context menu of the modeling window, then the selected items 
        /// in the active GME modeling window. If nothing is selected, the collection is empty (contains zero elements).
        /// </param>
        /// <param name="startMode">Contains information about the GUI event that initiated the invocation.</param>
        [ComVisible(false)]
        public void Main(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, ComponentStartMode startMode)
        {
            // TODO: Add your interpreter code
            gmeConsole.Out.WriteLine("Running Subtree Merge Utility ...");

            string[] FileNames = null;
            DialogResult dr;
            using (OpenFileDialog ofd = new OpenFileDialog()) {
                ofd.CheckFileExists = true;
                ofd.DefaultExt = "mga";
                ofd.Multiselect = false;
                ofd.Filter = "mga files (*.mga)|*.mga|All files (*.*)|*.*";

                dr = ofd.ShowDialog();
                if (dr == DialogResult.OK) {
                    FileNames = ofd.FileNames;
                }
            }
            if (dr == DialogResult.OK) {
                MgaGateway.PerformInTransaction(delegate {
                    SubTreeMerge subTreeMerge = new SubTreeMerge();
                    subTreeMerge.gmeConsole = gmeConsole;
                    subTreeMerge.merge(currentobj, FileNames[0]);
                }, transactiontype_enum.TRANSACTION_NON_NESTED);
                return;
            } else {
                gmeConsole.Warning.WriteLine("Subtree Merge Utility cancelled");
                return;
            }
        }

        #region IMgaComponentEx Members

        MgaGateway MgaGateway { get; set; }
        public FlexConsole gmeConsole { get; set; }

        public MgaProject currentMgaProject { get; set; }
        public void InvokeEx(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, int param)
        {
            if (!enabled)
            {
                return;
            }

            if (currentobj == null) {
                MessageBox.Show("Please select the subtree to be replaced (via merge) so that it is displayed in the main window.");
                return;
            }

            currentMgaProject = project;

            try
            {
                gmeConsole = new FlexConsole(FlexConsole.ConsoleType.GMECONSOLE, project);
                MgaGateway = new MgaGateway(project);
                project.CreateTerritoryWithoutSink(out MgaGateway.territory);

                Main(project, currentobj, selectedobjs, Convert(param));
            }
            finally
            {
                if (MgaGateway.territory != null)
                {
                    MgaGateway.territory.Destroy();
                }
                MgaGateway = null;
                project = null;
                currentobj = null;
                selectedobjs = null;
                gmeConsole = null;
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

    public enum MessageType { OUT, ERROR, WARNING, INFO };

    class NullTextWriter : TextWriter {

        public override Encoding Encoding {
            get { throw new NotImplementedException(); }
        }
        public override void Close() { }
        public override void Flush() { }
        public override void Write(string format, params object[] args) { }
        public override void WriteLine(string format, params object[] args) { }

    }

    public class FlexConsole {

        GMEConsole _gmeConsole = null;

        public enum ConsoleType { NONE, GMECONSOLE, CONSOLE };

        private TextWriter _info = null;
        public TextWriter Info { get { return _info; } }

        private TextWriter _warning = null;
        public TextWriter Warning { get { return _warning; } }

        private TextWriter _error = null;
        public TextWriter Error { get { return _error; } }

        private TextWriter _out = null;
        public TextWriter Out { get { return _out; } }

        public FlexConsole(ConsoleType consoleType, MgaProject mgaProject = null) {

            switch (consoleType) {
                case ConsoleType.NONE:
                    _info = _warning = _error = _out = new NullTextWriter();
                    break;
                case ConsoleType.CONSOLE:
                    _info = _warning = _error = Console.Error;
                    _out = Console.Out;
                    break;
                case ConsoleType.GMECONSOLE:
                    _gmeConsole = GMEConsole.CreateFromProject(mgaProject);
                    _info = _gmeConsole.Info;
                    _warning = _gmeConsole.Warning;
                    _error = _gmeConsole.Error;
                    _out = _gmeConsole.Out;
                    break;
            }

        }
    }

    
    public class SubTreeMerge {

        FlexConsole _gmeConsole = new FlexConsole(FlexConsole.ConsoleType.NONE);

        public enum Errors { NoError = 0, FileError = 0x02, PathError = 0x04, GMEError = 0x08, DuplicateChildNameError = 0x10, MissingChildError = 0x20 }
        private Errors _exitStatus = 0;
        public Errors exitStatus { get { return _exitStatus; } } 

        public FlexConsole gmeConsole {
            get { return _gmeConsole; }
            set { _gmeConsole = value; }
        }

        private MgaProject GetProject(String filename, String metaName) {
            MgaProject result = null;

            if (filename != null && filename != "") {
                if (Path.GetExtension(filename) == ".mga") {
                    result = new MgaProject();
                    if (System.IO.File.Exists(filename)) {
                        gmeConsole.Out.Write("Opening {0} ... ", filename);
                        result.OpenEx("MGA=" + filename, metaName, null);
                    } else {
                        gmeConsole.Out.Write("Creating {0} ... ", filename);
                        result.Create("MGA=" + filename, metaName);
                    }
                    gmeConsole.Out.WriteLine("Done.");
                } else {
                    gmeConsole.Error.WriteLine("{0} file must be an mga project.", filename);
                    _exitStatus |= Errors.FileError;
                }
            } else {
                gmeConsole.Error.WriteLine("Please specify an Mga project.");
                _exitStatus |= Errors.FileError;
            }

            return result;
        }

        private Dictionary<IMgaFCO, IMgaFCO> _fcoMap = new Dictionary<IMgaFCO, IMgaFCO>();

        private IMgaFCO getCorrespondingIMgaFCO(IMgaFCO iMgaFCO) {

            IMgaFCO correspondingIMgaFCO = null;
            if (_fcoMap.ContainsKey(iMgaFCO)) {
                correspondingIMgaFCO = _fcoMap[iMgaFCO];
            }
            if (correspondingIMgaFCO == null) {
                correspondingIMgaFCO = _currentMgaProject.get_ObjectByPath(iMgaFCO.AbsPath) as IMgaFCO;
                if (correspondingIMgaFCO != null) {
                    _fcoMap[iMgaFCO] = correspondingIMgaFCO;
                }
            }

            return correspondingIMgaFCO;
        }

        MgaProject _currentMgaProject = null;
        string _projectFilename = null;
        public void merge(MgaFCO currentObject, string filename) {

            _currentMgaProject = currentObject.Project;

            MgaProject mgaProject = GetProject(filename, currentObject.Project.MetaName);

            if (mgaProject == null)
                return;
            try
            {

                _projectFilename = filename;

                MgaGateway mgaGateway = new MgaGateway(mgaProject);

                mgaProject.CreateTerritoryWithoutSink(out mgaGateway.territory);
                mgaGateway.PerformInTransaction(delegate
                {

                    // "DO" LOOP IS ONLY TO ALLOW "break" TO TERMINATE THIS INTERPRETER
                    do
                    {

                        int origPrefs = _currentMgaProject.Preferences;
                        // Magic word allows us to remove ConnPoints
                        _currentMgaProject.Preferences = origPrefs | (int)GME.MGA.preference_flags.MGAPREF_IGNORECONNCHECKS | (int)GME.MGA.preference_flags.MGAPREF_FREEINSTANCEREFS;

                        try
                        {
                            // GET FCO TO BE MERGED FROM OTHER MGA FILE
                            IMgaFCO otherCurrentObject = mgaProject.get_ObjectByPath(currentObject.AbsPath) as IMgaFCO;
                            if (otherCurrentObject == null)
                            {
                                gmeConsole.Error.WriteLine("Could not perform merge:  could not find object of path \"" + currentObject.AbsPath + "\" in file \"" + filename + "\"");
                                _exitStatus |= Errors.PathError;
                                break;
                            }

                            recordConnections(otherCurrentObject);

                            // GET PARENT (IN CURRENT MODEL) OF THE FCO TO BE MERGED INTO THE CURRENT MODEL
                            MgaObject currentParentMGAObject = null;
                            GME.MGA.Meta.objtype_enum currentParentObjTypeEnum;
                            currentObject.GetParent(out currentParentMGAObject, out currentParentObjTypeEnum);


                            // THE ROOT OF THE MERGED FCO
                            IMgaFCO newCurrentObject = null;

                            IMgaFCO otherArchetype = otherCurrentObject.ArcheType;
                            if (otherArchetype != null)
                            {

                                MgaFCO newArchetype = mgaProject.get_ObjectByPath(otherArchetype.AbsPath) as MgaFCO;
                                if (newArchetype == null)
                                {
                                    gmeConsole.Error.WriteLine("Could not find object of path \"" + otherArchetype.AbsPath + "\" (archetype of \"" + otherCurrentObject.AbsPath + "\" in file \"" + filename + "\") in current model.");
                                    _exitStatus |= Errors.PathError;
                                    break;
                                }

                                if (currentParentObjTypeEnum == GME.MGA.Meta.objtype_enum.OBJTYPE_FOLDER)
                                {
                                    newCurrentObject = (currentParentMGAObject as MgaFolder).DeriveRootObject(newArchetype, otherCurrentObject.IsInstance);
                                }
                                else if (currentParentObjTypeEnum == GME.MGA.Meta.objtype_enum.OBJTYPE_MODEL)
                                {
                                    newCurrentObject = (currentParentMGAObject as MgaModel).DeriveChildObject(newArchetype, otherCurrentObject.MetaRole, otherCurrentObject.IsInstance);
                                }
                                else
                                {
                                    gmeConsole.Error.WriteLine("Unable to merge \"" + otherCurrentObject.AbsPath + "\" of file \"" + filename + "\":  prospective parent neither a folder nor a model.");
                                    _exitStatus |= Errors.GMEError;
                                    break;
                                }

                                attributesAndRegistryCopy(newCurrentObject, otherCurrentObject);
                                connectionCopy();

                            }
                            else
                            {

                                if (currentParentObjTypeEnum == GME.MGA.Meta.objtype_enum.OBJTYPE_FOLDER)
                                {
                                    newCurrentObject = (currentParentMGAObject as MgaFolder).CreateRootObject(currentObject.Meta);
                                }
                                else if (currentParentObjTypeEnum == GME.MGA.Meta.objtype_enum.OBJTYPE_MODEL)
                                {
                                    newCurrentObject = (currentParentMGAObject as MgaModel).CreateChildObject(currentObject.MetaRole);
                                }
                                else
                                {
                                    gmeConsole.Error.WriteLine("Unable to merge \"" + otherCurrentObject.AbsPath + "\" of file \"" + filename + "\":  prospective parent neither a folder nor a model.");
                                    _exitStatus |= Errors.GMEError;
                                    break;
                                }

                                subTreeCopy(newCurrentObject, otherCurrentObject);
                                instanceCopy();
                                attributesAndRegistryCopy(newCurrentObject, otherCurrentObject);
                                referenceCopy();
                                connectionCopy();
                                //referenceConnectionCopy();
                            }
                        }
                        finally
                        {
                            _currentMgaProject.Preferences = origPrefs;
                        }


                    } while (false);

                }, transactiontype_enum.TRANSACTION_NON_NESTED);

                if (mgaGateway.territory != null)
                {
                    mgaGateway.territory.Destroy();
                }

                currentObject.DestroyObject();
            }
            finally
            {
                mgaProject.Close(true);
            }
        }


        public class InstanceInfo {
            private IMgaFCO _otherInstance;
            private IMgaFCO _newInstanceParent;

            public InstanceInfo(IMgaFCO otherInstance, IMgaFCO newInstanceParent) {
                _otherInstance = otherInstance;
                _newInstanceParent = newInstanceParent;
            }

            public IMgaFCO getOtherInstance() { return _otherInstance; }
            public IMgaFCO getNewInstanceParent() { return _newInstanceParent; }
        }

        public List<InstanceInfo> _instanceInfoList = new List<InstanceInfo>();


        public class ReferenceInfo {
            private IMgaReference _otherReference;
            private IMgaReference _newReference;

            public ReferenceInfo(IMgaReference otherReference, IMgaReference newReference) {
                _otherReference = otherReference;
                _newReference = newReference;
            }

            public IMgaReference getOtherReference() { return _otherReference; }
            public IMgaReference getNewReference() { return _newReference; }
        }

        public List<ReferenceInfo> _referenceInfoList = new List<ReferenceInfo>();

        public List<IMgaFCO> _hasConnectionList = new List<IMgaFCO>();
        public HashSet<IMgaConnection> _iMgaConnectionSet = new HashSet<IMgaConnection>();

        // RECORD ALL CONNECTIONS AND ALL OBJECTS THAT HAVE CONNECTIONS
        public void recordConnections(IMgaFCO otherCurrentObject) {

            if (otherCurrentObject.PartOfConns.Count != 0) {
                _hasConnectionList.Add(otherCurrentObject);
            }
            if (otherCurrentObject is IMgaReference)
            {
                IMgaReference otherReference = (IMgaReference)otherCurrentObject;
                if (otherReference.UsedByConns.Count > 0)
                {
                    _hasConnectionList.Add(otherReference);
                }
            }

            if (otherCurrentObject.ObjType != GME.MGA.Meta.objtype_enum.OBJTYPE_MODEL) {
                return;
            }
            foreach (MgaObject otherChildMgaObject in otherCurrentObject.ChildObjects) {

                if (otherChildMgaObject.MetaBase.ObjType == GME.MGA.Meta.objtype_enum.OBJTYPE_CONNECTION) {
                    _iMgaConnectionSet.Add(otherChildMgaObject as IMgaConnection);
                    continue;
                }

                recordConnections(otherChildMgaObject as IMgaFCO);
            }
        }

        public void subTreeCopy(IMgaFCO newCurrentObject, IMgaFCO otherCurrentObject) {

            _fcoMap[otherCurrentObject] = newCurrentObject;

            // NAME
            newCurrentObject.Name = otherCurrentObject.Name;

            // CHILDREN
            if (otherCurrentObject.ObjType != GME.MGA.Meta.objtype_enum.OBJTYPE_MODEL) {
                return;
            }

            foreach (MgaObject otherChildMgaObject in otherCurrentObject.ChildObjects) {

                // CONNECTIONS ARE PUT OFF UNTIL THE END, WHEN ALL NEEDED OBJECTS ARE CREATED
                if (otherChildMgaObject.MetaBase.ObjType == GME.MGA.Meta.objtype_enum.OBJTYPE_CONNECTION) {
                    continue;
                }

                IMgaFCO otherChildIMgaFCO = otherChildMgaObject as IMgaFCO;

                // INSTANCE ARCHETYPE MAY NOT HAVE BEEN CREATED (I.E. COPIED OVER) YET.  POSTPONE INSTANCE CREATION UNTIL LATER.
                if (otherChildIMgaFCO.ArcheType != null) {
                    _instanceInfoList.Add(new InstanceInfo(otherChildIMgaFCO, newCurrentObject));
                    continue;
                }

                MgaFCO newChildMgaFCO = (newCurrentObject as MgaModel).CreateChildObject(otherChildIMgaFCO.MetaRole);

                if (newChildMgaFCO is IMgaReference) {
                    _fcoMap[otherChildIMgaFCO] = newChildMgaFCO;
                    newChildMgaFCO.Name = otherChildIMgaFCO.Name;
                    _referenceInfoList.Add(new ReferenceInfo(otherChildIMgaFCO as IMgaReference, newChildMgaFCO as IMgaReference));
                    continue;
                }

                subTreeCopy(newChildMgaFCO, otherChildIMgaFCO);
            }

        }

        public void instanceCopy() {

            foreach( InstanceInfo instanceInfo in _instanceInfoList ) {

                MgaFCO newArchetype = getCorrespondingIMgaFCO(instanceInfo.getOtherInstance().ArcheType) as MgaFCO;
                if (newArchetype == null) {
                    gmeConsole.Error.WriteLine("Could not find object of path \"" + instanceInfo.getOtherInstance().ArcheType.AbsPath + "\" (archetype of \"" + instanceInfo.getOtherInstance().AbsPath + "\" in file \"" + _projectFilename + "\") in current model.");
                    _exitStatus |= Errors.PathError;
                    continue;
                }

                IMgaFCO iMgaFCO = (instanceInfo.getNewInstanceParent() as MgaModel).DeriveChildObject(newArchetype, instanceInfo.getOtherInstance().MetaRole, instanceInfo.getOtherInstance().IsInstance);
                iMgaFCO.Name = instanceInfo.getOtherInstance().Name;
            }
        }


        public void copyRegistry(IMgaFCO newCurrentObject, IMgaFCO otherCurrentObject) {
            MgaRegNodes otherMgaRegNodes = otherCurrentObject.Registry;
            foreach (MgaRegNode otherMgaRegNode in otherMgaRegNodes) {
                MgaRegNode newMgaRegNode = newCurrentObject.get_RegistryNode(otherMgaRegNode.Path);
                copyRegistry(newMgaRegNode, otherMgaRegNode);
            }
        }

        public void copyRegistry(IMgaRegNode newRegNode, IMgaRegNode otherRegNode) {
            int otherRegNodeStatus = 0;
            otherRegNode.Status(out otherRegNodeStatus);
            if (newRegNode.Value != otherRegNode.Value || otherRegNodeStatus == (int)GME.MGA.attstatus_enum.ATTSTATUS_HERE) {
                newRegNode.Value = otherRegNode.Value;
            }

            foreach (IMgaRegNode iMgaRegNode in otherRegNode.get_SubNodes(true) ) {
                copyRegistry(newRegNode.SubNodeByName[iMgaRegNode.Name], iMgaRegNode);
            }
        }

        public void attributesAndRegistryCopy(IMgaFCO newCurrentObject, IMgaFCO otherCurrentObject) {

            // AT THIS POINT, INSTANCE FCO'S HAVEN'T YET MADE IT INTO THE _fcoMap.  AS ALL FCO'S ARE TRAVERSED
            // TO SET ATTRIBUTE AND REGISTRY ENTRIES, A GOOD PLACE TO PUT THEM INTO THE _fcoMap IS HERE.
            if (!_fcoMap.ContainsKey(otherCurrentObject)) {
                _fcoMap[otherCurrentObject] = newCurrentObject;
            }

            foreach (IMgaReference otherReferencedBy in otherCurrentObject.ReferencedBy) {

                // THIS CHECK PREVENTS REFERENCES THAT ARE IN THE MERGED MODEL FROM BEING PROCESSED HERE (AND DESTROYED AS A RESULT)
                if (!_fcoMap.ContainsKey(otherReferencedBy)) {

                    MgaObject otherReferencedByParent = null;
                    GME.MGA.Meta.objtype_enum otherReferencedByParentObjTypeEnum;
                    otherReferencedBy.GetParent(out otherReferencedByParent, out otherReferencedByParentObjTypeEnum);

                    MgaObject newReferencedByParent = _currentMgaProject.get_ObjectByPath(otherReferencedByParent.AbsPath);
                    if (newReferencedByParent == null) {
                        gmeConsole.Error.WriteLine("Unable to make new reference in \"" + otherReferencedByParent.AbsPath + "\" to \"" + otherCurrentObject.AbsPath + "\": prospective parent path not found.");
                        _exitStatus |= Errors.PathError;
                        continue;
                    }
                    MgaObject newReferencedBy = newCurrentObject.Project.get_ObjectByPath(otherReferencedBy.AbsPath);
                    if (newReferencedByParent == null)
                    {
                        gmeConsole.Error.WriteLine("Unable to redirect reference in \"" + otherReferencedByParent.AbsPath + "\" to \"" + otherCurrentObject.AbsPath + "\": reference not found");
                        _exitStatus |= Errors.PathError;
                        continue;
                    }
                    GME.MGA.Meta.objtype_enum newReferencedByParentObjTypeEnum = newReferencedByParent.ObjType;

                    /*
                    IMgaReference newReferencedBy = null;
                    if (newReferencedByParentObjTypeEnum == GME.MGA.Meta.objtype_enum.OBJTYPE_FOLDER) {
                        newReferencedBy = (newReferencedByParent as MgaFolder).CreateRootObject(otherReferencedBy.Meta) as IMgaReference;
                    } else if (newReferencedByParentObjTypeEnum == GME.MGA.Meta.objtype_enum.OBJTYPE_MODEL) {
                        newReferencedBy = (newReferencedByParent as MgaModel).CreateChildObject(otherReferencedBy.MetaRole) as IMgaReference;
                    } else {
                        gmeConsole.Error.WriteLine("Unable to make new reference in \"" + newReferencedByParent.AbsPath + "\" to \"" + newCurrentObject.AbsPath + "\": prospective parent neither a folder nor a model.");
                        _exitStatus |= Errors.PathError;
                        continue;
                    }

                    newReferencedBy.Referred = newCurrentObject as MgaFCO;
                    // KMS this will be destroyed at the end
                    otherReferencedBy.DestroyObject();
                    */


                    ReferenceSwitcher.Switcher.MoveReferenceWithRefportConnections(newCurrentObject as MgaFCO, (MgaReference)newReferencedBy, (x, y, z) => {
                        Console.WriteLine(x + " ; " + y + " ; " + z);
                    });

                    //_fcoMap[otherReferencedBy] = newReferencedBy;
                }
            }

            // ATTRIBUTES
            MgaAttributes otherMgaAttributes = otherCurrentObject.Attributes;
            foreach (MgaAttribute otherMgaAttribute in otherMgaAttributes) {
                //IMgaAttribute newMgaAttribute = newCurrentObject.get_Attribute( otherMgaAttribute.Meta ); // THROWS 0x87650080 ERROR --- E_META_INVALIDATTR
                IMgaAttribute newMgaAttribute = newCurrentObject.Attributes.Cast<IMgaAttribute>().FirstOrDefault(x => x.Meta.Name == otherMgaAttribute.Meta.Name);
                if (newMgaAttribute.Value != otherMgaAttribute.Value || otherMgaAttribute.Status == (int)GME.MGA.attstatus_enum.ATTSTATUS_HERE) {
                    newMgaAttribute.Value = otherMgaAttribute.Value;
                }
            }

            // REGISTRY
            copyRegistry(newCurrentObject, otherCurrentObject);

            // RECURSE
            if (otherCurrentObject.ObjType != GME.MGA.Meta.objtype_enum.OBJTYPE_MODEL) {
                return;
            }
            foreach (MgaObject otherChildMgaObject in otherCurrentObject.ChildObjects) {

                if (otherChildMgaObject.MetaBase.ObjType == GME.MGA.Meta.objtype_enum.OBJTYPE_CONNECTION) {
                    continue;
                }

                IMgaFCO otherChildIMgaFCO = otherChildMgaObject as IMgaFCO;

                IMgaFCO newChildIMgaFCO = null;
                if (_fcoMap.ContainsKey(otherChildIMgaFCO)) {
                    newChildIMgaFCO = _fcoMap[otherChildIMgaFCO];
                } else {
                    IEnumerable< MgaFCO > newChildMgaFCOs = (newCurrentObject as MgaModel).ChildObjects.Cast<MgaFCO>().Where(x => x.Name == otherChildIMgaFCO.Name);
                    if (newChildMgaFCOs.Count() > 1) {
                        gmeConsole.Error.WriteLine("Error:  FCO of path \"" + newCurrentObject.AbsPath + "\" has more than one child named \"" + otherChildIMgaFCO.Name + "\" -- cannot discern objects, skipping");
                        _exitStatus |= Errors.DuplicateChildNameError;
                        continue;
                    } else if ( newChildMgaFCOs.Count() == 0 ) {
                        gmeConsole.Error.WriteLine("Error:  FCO of path \"" + newCurrentObject.AbsPath + "\" has no child named \"" + otherChildIMgaFCO.Name + "\", but corresponding object in file does.  This shouldn't happen, skipping");
                        _exitStatus |= Errors.MissingChildError;
                        continue;
                    }
                    newChildIMgaFCO = newChildMgaFCOs.First();
                    _fcoMap[otherChildIMgaFCO] = newChildIMgaFCO;
                }

                attributesAndRegistryCopy(newChildIMgaFCO, otherChildIMgaFCO);
            }

        }

        public void referenceCopy() {

            foreach (ReferenceInfo referenceInfo in _referenceInfoList) {

                if (referenceInfo.getOtherReference().Referred == null) continue;

                MgaFCO target = getCorrespondingIMgaFCO(referenceInfo.getOtherReference().Referred) as MgaFCO;
                if (target == null) {
                    gmeConsole.Error.WriteLine("Could not find object of path \"" + referenceInfo.getOtherReference().Referred.AbsPath + "\" (reference of \"" + referenceInfo.getOtherReference().AbsPath + "\" in file \"" + _projectFilename + "\") in current model.");
                    _exitStatus |= Errors.PathError;
                    continue;
                }
                referenceInfo.getNewReference().Referred = target;
            }
        }


        public void connectionCopy() {

            HashSet<IMgaConnection> otherIMgaConnectionSet = new HashSet<IMgaConnection>();

            foreach (IMgaFCO otherIMgaFCO in _hasConnectionList) {

                IEnumerable<IMgaConnPoint> otherIMgaConnPoints = otherIMgaFCO.PartOfConns.Cast<IMgaConnPoint>();
                if (otherIMgaFCO is IMgaReference)
                {
                    otherIMgaConnPoints = otherIMgaConnPoints.Concat(((IMgaReference)otherIMgaFCO).UsedByConns
                        .Cast<IMgaConnPoint>().Where(cp => cp.References[1] == otherIMgaFCO));
                }

                IMgaFCO newIMgaFCO = getCorrespondingIMgaFCO(otherIMgaFCO);
                if (newIMgaFCO == null) {
                    gmeConsole.Error.WriteLine("Could not make any connections to \"" + otherIMgaFCO.AbsPath + "\" in main model during merge:  FCO not found.");
                    _exitStatus |= Errors.PathError;
                    continue;
                }

                // AT THIS POINT, NO CONNECTIONS SHOULD EXIST EXCEPT FOR CONNECTIONS THAT ARE INSTANCE/SUBTYPES
                foreach (IMgaConnPoint otherIMgaConnPoint in otherIMgaConnPoints) {

                    if (otherIMgaConnPoint.References.Count > 0)
                    {
                        if (!_fcoMap.ContainsKey(otherIMgaConnPoint.References[1]))
                        {
                            continue;
                        }
                    }
                    // GET ACTUAL CONNECTION
                    IMgaConnection otherIMgaConnection = otherIMgaConnPoint.Owner;

                    // ANY CONNECTIONS THAT ARE INSTANCES/SUBTYPES IN THE "OTHER" MODEL (I.E. ORIGINAL MODEL IN THE OTHER FILE)
                    // MUST BE INSTANCES/SUBTYPES IN "NEW" MODEL.  I.E. THEY ALREADY EXIST IN THE "NEW" MODEL, SO SKIP
                    if (otherIMgaConnection.ArcheType != null) {
                        continue;
                    }

                    // IF CONNECTION ALREADY PROCESSED, SKIP
                    if (otherIMgaConnectionSet.Contains(otherIMgaConnection)) {
                        continue;
                    }
                    otherIMgaConnectionSet.Add(otherIMgaConnection);
                    _iMgaConnectionSet.Remove(otherIMgaConnection);

                    IMgaModel otherIMgaConnectionParent = otherIMgaConnection.ParentModel;

                    IMgaModel newIMgaConnectionParent = getCorrespondingIMgaFCO(otherIMgaConnectionParent) as IMgaModel;
                    if (newIMgaConnectionParent == null) {
                        gmeConsole.Error.WriteLine( "Could not make connection to \"" + otherIMgaFCO.AbsPath + "\" in main model:  Could not access parent (path: \"" + otherIMgaConnectionParent.AbsPath + "\")." );
                        _exitStatus |= Errors.PathError;
                        continue;
                    }

                    IMgaConnection newIMgaConnection = newIMgaConnectionParent.CreateChildObject(otherIMgaConnection.MetaRole) as IMgaConnection;
                    foreach (IMgaConnPoint otherIMgaConnPointToCopy in otherIMgaConnection.ConnPoints) {

                        IMgaFCO otherTarget = otherIMgaConnPointToCopy.Target;
                        MgaFCO newTarget = getCorrespondingIMgaFCO( otherTarget ) as MgaFCO;

                        if ( newTarget == null ) {
                            gmeConsole.Error.WriteLine( "Could not find connection target of path \"" + otherTarget.AbsPath + "\" and role \"" + otherIMgaConnPointToCopy.ConnRole + "\" for connection of path \"" + newIMgaConnection.AbsPath + "\"" );
                            newIMgaConnection.DestroyObject();
                            newIMgaConnection = null;
                            _exitStatus |= Errors.PathError;
                            break;
                        }

                        MgaFCOs newMgaFCOs = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
                        bool referenceError = false;
                        foreach( IMgaFCO otherReferenceIMgaFCO in otherIMgaConnPointToCopy.References ) {
                            IMgaFCO newReferenceIMgaFCO = getCorrespondingIMgaFCO( otherReferenceIMgaFCO );
                            if ( newReferenceIMgaFCO == null ) {
                                gmeConsole.Error.WriteLine("Could not make connection reference \"" + otherReferenceIMgaFCO.AbsPath + "\" for connection \"" + newIMgaConnection.AbsPath + "\", role \"" + otherIMgaConnPointToCopy.ConnRole + "\".");
                                newIMgaConnection.DestroyObject();
                                newIMgaConnection = null;
                                referenceError = true;
                                _exitStatus |= Errors.PathError;
                                break;
                            }
                            newMgaFCOs.Append(newReferenceIMgaFCO as MgaFCO);
                        }
                        if ( referenceError ) break;

                        MgaConnPoint newMgaConnPoint = null;
                        newIMgaConnection.AddConnPoint(otherIMgaConnPointToCopy.ConnRole, 1, newTarget, newMgaFCOs, out newMgaConnPoint);
                    }
                    if (newIMgaConnection != null) {
                        attributesAndRegistryCopy(newIMgaConnection, otherIMgaConnection);
                    }
                }

            }
        }

        void referenceConnectionCopy() {

            foreach( IMgaConnection otherIMgaConnection in _iMgaConnectionSet ) {

                IMgaModel otherIMgaConnectionParent = otherIMgaConnection.ParentModel;

                IMgaModel newIMgaConnectionParent = getCorrespondingIMgaFCO(otherIMgaConnectionParent) as IMgaModel;
                if (newIMgaConnectionParent == null) {

                    Dictionary< string, IMgaFCO > nameTargetMap = new Dictionary<string,IMgaFCO>();
                    foreach (IMgaConnPoint otherIMgaConnPoint in otherIMgaConnection.ConnPoints) {
                        nameTargetMap[ otherIMgaConnPoint.ConnRole ] = otherIMgaConnPoint.Target;
                    }
                    gmeConsole.Error.Write( "Could not make connection between " );
                    bool first = true;
                    foreach( string key in nameTargetMap.Keys ) {
                        if ( first ) first = false;
                        else gmeConsole.Error.Write( ", " );
                        gmeConsole.Error.Write( "\"" + key + "\"=\"" + nameTargetMap[ key ].AbsPath + "\"" );
                    }
                    
                    gmeConsole.Error.Write( " in main model:  Could not access parent (path: \"" + otherIMgaConnectionParent.AbsPath + "\")");
                    _exitStatus |= Errors.PathError;
                    continue;
                }

                IMgaConnection newIMgaConnection = newIMgaConnectionParent.CreateChildObject(otherIMgaConnection.MetaRole) as IMgaConnection;
                foreach (IMgaConnPoint otherIMgaConnPointToCopy in otherIMgaConnection.ConnPoints) {

                    IMgaFCO otherTarget = otherIMgaConnPointToCopy.Target;
                    MgaFCO newTarget = getCorrespondingIMgaFCO(otherTarget) as MgaFCO;

                    if ( newTarget == null ) {
                        gmeConsole.Error.WriteLine( "Could not find connection target of path \"" + otherTarget.AbsPath + "\" and role \"" + otherIMgaConnPointToCopy.ConnRole + "\" for connection of path \"" + newIMgaConnection.AbsPath + "\"" );
                        newIMgaConnection.DestroyObject();
                        newIMgaConnection = null;
                        _exitStatus |= Errors.PathError;
                        break;
                    }

                    MgaFCOs newMgaFCOs = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
                    bool referenceError = false;
                    foreach( IMgaFCO otherReferenceIMgaFCO in otherIMgaConnPointToCopy.References ) {
                        IMgaFCO newReferenceIMgaFCO = getCorrespondingIMgaFCO( otherReferenceIMgaFCO );
                        if ( newReferenceIMgaFCO == null ) {
                            gmeConsole.Error.WriteLine("Could not make connection reference \"" + otherReferenceIMgaFCO.AbsPath + "\" for connection \"" + newIMgaConnection.AbsPath + "\", role \"" + otherIMgaConnPointToCopy.ConnRole + "\".");
                            newIMgaConnection.DestroyObject();
                            newIMgaConnection = null;
                            referenceError = true;
                            _exitStatus |= Errors.PathError;
                            break;
                        }
                        newMgaFCOs.Append(newReferenceIMgaFCO as MgaFCO);
                    }
                    if ( referenceError ) break;

                    MgaConnPoint newMgaConnPoint = null;
                    newIMgaConnection.AddConnPoint(otherIMgaConnPointToCopy.ConnRole, 1, newTarget, newMgaFCOs, out newMgaConnPoint);
                }
                if (newIMgaConnection != null) {
                    attributesAndRegistryCopy(newIMgaConnection, otherIMgaConnection);
                }
            }
        }

    }

}
