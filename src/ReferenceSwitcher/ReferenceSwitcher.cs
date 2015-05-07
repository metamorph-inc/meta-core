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

namespace ReferenceSwitcher
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class ReferenceSwitcherInterpreter : IMgaComponentEx, IGMEVersionInfo
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
        /// GMEConsole is avaliable. A general try-catch block catches all the exceptions
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
            FCOChooser f = new FCOChooser(MgaGateway, GMEConsole);
            bool closed = false;
            f.FormClosed += delegate(object sender, FormClosedEventArgs e) { closed = true; };
            f.Show();
            while (!closed)
            {
                WaitMessage();
                System.Windows.Forms.Application.DoEvents();
            }
            GMEConsole.Out.Write(this.GetType().Name + " finished");
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


        public void SwitchReferences(object[] lib1Objects, object[] lib2Objects)
        {
            Switcher switcher = new Switcher(lib1Objects.Cast<IMgaObject>(), lib2Objects.Cast<IMgaObject>(), null);
            switcher.UpdateSublibrary();
        }

        [ComVisible(false)]
        public void PrintLine(Func<string, string, string> f, IMgaFCO a, IMgaFCO b)
        {
            Console.Out.WriteLine(f(Switcher.getPath(a), Switcher.getPath(b)));
        }
        public void SwitchReference(IMgaFCO to, IMgaReference @ref)
        {
            Switcher.MoveReferenceWithRefportConnections(to, @ref, PrintLine);
        }
    }

    public class Switcher
    {
        List<IMgaObject> lib1Objects;
        List<IMgaObject> lib2Objects;
        GMEConsole GMEConsole;

        public Switcher(IMgaObject fco1Object, IMgaObject fco2Object, GMEConsole GMEConsole)
        {
            this.lib1Objects = new List<IMgaObject>() { fco1Object };
            this.lib2Objects = new List<IMgaObject>() { fco2Object };
            this.GMEConsole = GMEConsole;
        }
        public Switcher(IEnumerable<IMgaObject> fco1Object, IEnumerable<IMgaObject> fco2Object, GMEConsole GMEConsole)
        {
            this.lib1Objects = fco1Object.ToList();
            this.lib2Objects = fco2Object.ToList();
            this.GMEConsole = GMEConsole;
        }

        class ObjectPair
        {
            public IMgaObject o1 { get; set; }
            public IMgaObject o2 { get; set; }
        }

        public void UpdateSublibrary()
        {
            int origPrefs = this.lib1Objects.First().Project.Preferences;
            // Magic word allows us to remove ConnPoints
            this.lib1Objects.First().Project.Preferences = origPrefs | (int)GME.MGA.preference_flags.MGAPREF_IGNORECONNCHECKS
                | (int)GME.MGA.preference_flags.MGAPREF_FREEINSTANCEREFS;
            try
            {
                IEnumerator<IMgaObject> lib2ObjectsEnum = lib2Objects.GetEnumerator();
                foreach (IMgaObject lib1Object in lib1Objects)
                {
                    lib2ObjectsEnum.MoveNext();
                    UpdateSublibrary(lib1Object, lib2ObjectsEnum.Current);
                }
            }
            finally
            {
                lib1Objects.First().Project.Preferences = origPrefs;
            }
        }

        private void UpdateSublibrary(IMgaObject fco1Objec, IMgaObject fco2Objec)
        {
            // fco2Object may be null
            if (fco1Objec is IMgaFCO) // references only refer to FCOs
            {
                IMgaFCO fco1 = (IMgaFCO)fco1Objec;
                IMgaFCO fco2 = fco2Objec as IMgaFCO;
                foreach (IMgaFCO fco in fco1.ReferencedBy)
                {
                    if (fco.IsInstance)
                        continue; // instance references will be updated by their archetype
                    // Don't update references in the old library
                    bool fcoInLib1Objects = false;
                    foreach (IMgaObject lib1Root in this.lib1Objects)
                    {
                        // FIXME: Contains(this.fco1Object) doesn't work
                        if (new ParentChain(fco).Contains(lib1Root, new KeyEqualityComparer<IMgaObject>(x => x.ID)))
                        {
                            fcoInLib1Objects = true;
                        }
                    }
                    if (fcoInLib1Objects)
                        continue;
                    IMgaReference refe = (IMgaReference)fco;
                    if (fco2 != null)
                    {
                        if (refe.UsedByConns.Count != 0)
                        {
                            if (refe.DerivedFrom == null)
                            {
                                try
                                {
                                    MoveReferenceWithRefportConnections(fco2, refe, WriteLine);
                                }
                                catch (Exception e)
                                {
                                    if (GMEConsole != null)
                                        GMEConsole.Error.WriteLine("Could not set reference " + GetLink(refe, refe.Name));
                                    throw new Exception("Could not set reference " + getPath(refe) +
                                        " (" + refe.ID + ")", e);
                                }
                            }
                        }
                        else
                        {
                            try
                            {
                                bool setRef;
                                if (refe.DerivedFrom == null)
                                    setRef = true;
                                else
                                {
                                    short compareToBase;
                                    refe.CompareToBase(out compareToBase);
                                    setRef = compareToBase != 0;
                                }
                                if (setRef)
                                {
                                    // FIXME: can this fail; should we handle it somehow?
                                    refe.Referred = (GME.MGA.MgaFCO)fco2;
                                }
                            }
                            catch (Exception e)
                            {
                                if (GMEConsole != null)
                                    GMEConsole.Error.WriteLine("Could not set reference " + GetLink(refe, refe.Name));
                                throw new Exception("Could not set reference " + getPath(refe) +
                                    " (" + refe.ID + ")", e);
                            }
                        }
                    }
                    else
                    {
                        WriteLine((x, y) => "Couldn't update " + x + ": " + y + " has no counterpart", refe, fco1);
                    }
                }
            }

            List<IMgaObject> fco1Children = getChildren(fco1Objec);
            List<IMgaObject> fco2Children = getChildren(fco2Objec);
            Dictionary<string, ObjectPair> dict = new Dictionary<string, ObjectPair>();
            foreach (IMgaObject o in fco1Children)
            {
                // FIXME: should use an actual data structure
                dict.GetValueOrDefault(o.Name + "xxxFIXMExxx" + o.MetaBase.Name).o1 = o;
            }
            foreach (IMgaObject o in fco2Children)
            {
                dict.GetValueOrDefault(o.Name + "xxxFIXMExxx" + o.MetaBase.Name).o2 = o;
            }
            foreach (KeyValuePair<string, ObjectPair> entry in dict)
            {
                if (entry.Value.o1 != null)
                {
                    UpdateSublibrary(entry.Value.o1, entry.Value.o2);
                }
            }
        }


        public delegate void WriteLineF(Func<string, string, string> f, IMgaFCO a, IMgaFCO b);
        public void WriteLine(Func<string, string, string> f, IMgaFCO a, IMgaFCO b)
        {
            if (GMEConsole != null)
            {
                GMEConsole.Out.WriteLine(f(GetLink(a, a.Name), GetLink(b, b.Name)));
            }
            else
            {
                Console.Out.WriteLine(f(getPath(a), getPath(b)));
            }
        }

        public static List<T> MakeList<T>(T itemOftype)
        {
            List<T> newList = new List<T>();
            return newList;
        }

        /**
         * First we disconnect all connections to refports, then move the reference, then reconnect
         */
        public static void MoveReferenceWithRefportConnections(IMgaFCO targetFCO, IMgaReference reference,
            WriteLineF WriteLine, IMgaReference oldReference=null)
        {
            if (oldReference == null)
            {
                oldReference = reference;
            }
            Queue<IMgaReference> references = new Queue<IMgaReference>();
            references.Enqueue(reference);
            IMgaFCO targetModel = targetFCO;
            while (targetModel is IMgaReference)
            {
                targetModel = ((IMgaReference)targetModel).Referred;
            }
            MgaFCOs fco2ChildFCOs = ((IMgaModel)targetModel).ChildFCOs;
            Dictionary<string, IMgaFCO> newRefeChildren = GetNameMap(fco2ChildFCOs,
                x => { });
            // TODO: warn, but only for refport-connected children
            //GMEConsole.Warning.WriteLine("Warning: " + fco2.Name + " has multiple children named " + x));

            int origPrefs = targetFCO.Project.Preferences;
            // Magic word allows us to remove ConnPoints
            targetFCO.Project.Preferences = origPrefs | (int)GME.MGA.preference_flags.MGAPREF_IGNORECONNCHECKS
                | (int)GME.MGA.preference_flags.MGAPREF_FREEINSTANCEREFS;

            try {

                MgaConnection conn = null;
                var ReconnectList = MakeList( new { ConnRole = "src", Ref = reference, Port = targetFCO, Conn = conn } );
                while( references.Count != 0 ) {
                    IMgaReference refe = references.Dequeue();

                    foreach( IMgaConnPoint connPoint in oldReference.UsedByConns ) {
                        if (connPoint.References[1] != refe)
                        {
                            continue;
                        }
                        IMgaFCO fco2Port;
                        if( newRefeChildren.TryGetValue( connPoint.Target.Name, out fco2Port ) ) {
                            if( fco2Port == null ) {
                                // fco2Port == null => multiple children with the same name
                                // Try matching based on Kind too
                                fco2Port = fco2ChildFCOs.Cast<IMgaFCO>().Where( x => x.Name == connPoint.Target.Name
                                    && x.Meta.MetaRef == connPoint.Target.Meta.MetaRef ).FirstOrDefault();
                            }
                            if( fco2Port != null ) {
                                ReconnectList.Add( new { ConnRole = connPoint.ConnRole, Ref = refe, Port = fco2Port, Conn = connPoint.Owner } );
                                connPoint.Remove();
                            }
                        } else {
                            WriteLine( ( x, y ) => "Can't find corresponding port for " + x
                                + " in " + y, connPoint.Target, targetFCO );
                            connPoint.Owner.DestroyObject();
                        }
                    }
                    foreach( IMgaReference x in refe.ReferencedBy.Cast<IMgaReference>() ) {
                        if( x.ID == reference.ID )
                            throw new Exception( "Circular reference chain starting with " + reference.AbsPath );
                        references.Enqueue( x );
                    }
                }
                reference.Referred = (MgaFCO)targetFCO;
                foreach( var reconnect in ReconnectList ) {
                    if( reconnect.ConnRole == "src" )
                        ( reconnect.Conn as IMgaSimpleConnection ).SetSrc( (MgaFCOs)GetRefChain( reconnect.Ref ), (MgaFCO)reconnect.Port );
                    else
                        ( reconnect.Conn as IMgaSimpleConnection ).SetDst( (MgaFCOs)GetRefChain( reconnect.Ref ), (MgaFCO)reconnect.Port );
                }

            } finally {
                targetFCO.Project.Preferences = origPrefs;
            }

        }

        public static string GetLink(IMgaObject o, string linkText = null)
        {
            if (linkText == null)
            {
                linkText = getPath(o);
            }
            return "<a href=\"mga:" + o.ID + "\">"
                + linkText
                + "</a>";
        }

        private static Dictionary<string, IMgaFCO> GetNameMap(IMgaFCOs fcos,
            Action<string> warnFunc)
        {
            Dictionary<string, IMgaFCO> refeChildren;
            refeChildren = new Dictionary<string, IMgaFCO>();
            foreach (IMgaFCO refeChild in fcos.Cast<IMgaFCO>())
            {
                if (refeChild.ObjType == GME.MGA.Meta.objtype_enum.OBJTYPE_CONNECTION)
                    continue;
                if (refeChildren.ContainsKey(refeChild.Name))
                {
                    warnFunc(refeChild.Name);
                    refeChildren[refeChild.Name] = null;
                }
                else
                    refeChildren.Add(refeChild.Name, refeChild);
            }
            return refeChildren;
        }

        private static IMgaFCOs GetRefChain(IMgaReference reference)
        {
            IMgaFCOs ret = (IMgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
            ret.Append(reference as MgaFCO);
            while (true)
            {
                if (reference.Referred == null)
                {
                    break;
                }
                if (reference.Referred.ObjType == GME.MGA.Meta.objtype_enum.OBJTYPE_REFERENCE)
                {
                    reference = reference.Referred as IMgaReference;
                    ret.Append(reference as MgaFCO);
                }
                else
                {
                    break;
                }
            }
            return ret;
        }

        public static string getPath(IMgaObject fco1Object)
        {
            return String.Join("/", new ParentChain(fco1Object).Select(x => x.Name).Reverse().ToArray());
        }

        private List<IMgaObject> getChildren(IMgaObject fco1Objec)
        {
            List<IMgaObject> fco1Children = new List<IMgaObject>();
            if (fco1Objec == null)
            {
                return fco1Children;
            }
            if (fco1Objec is IMgaFolder)
            {
                fco1Children.AddRange((fco1Objec as IMgaFolder).ChildFolders.Cast<IMgaObject>());
                fco1Children.AddRange((fco1Objec as IMgaFolder).ChildFCOs.Cast<IMgaObject>());
            }
            if (fco1Objec is IMgaModel)
            {
                fco1Children.AddRange((fco1Objec as IMgaModel).ChildFCOs.Cast<IMgaObject>());
            }
            return fco1Children;
        }

    }
}
