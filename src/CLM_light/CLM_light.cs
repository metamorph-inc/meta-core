using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using GME.CSharp;
using GME;
using GME.MGA;
using GME.MGA.Core;
using System.Linq;
using System.Diagnostics.Contracts;

namespace CLM_light
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CLM_lightInterpreter : IMgaComponentEx, IGMEVersionInfo
    {
        public const string ContextErrorMessage = "Run CLM_Light in an 'Alternative' or 'Optional' DesignContainer.";
        public const string SelectedObjErrorMessage = "Please Select One ComponentRef.";

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

            GMEConsole = GMEConsole.CreateFromProject(project);
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
            GMEConsole.Clear();
            System.Windows.Forms.Application.DoEvents();

            IMgaFCO selectedObj = null;

            bool isContextValid = CheckForValidContext(currentobj);

            if (isContextValid == false)
            {
                return;
            }

            // get all the ComponentRefs in the DesignContainer
            var crefs = (currentobj as IMgaModel).
                ChildFCOs.
                Cast<IMgaFCO>().
                Where(x => x.MetaBase.Name == "ComponentRef");

            // If there is exactly '1', select it
            if (crefs.Count() == 1)
            {
                selectedObj = crefs.FirstOrDefault();
            }

            if (selectedObj == null)
            {
                if (selectedobjs.Count != 1)
                {
                    GMEConsole.Error.WriteLine(SelectedObjErrorMessage);
                    return;
                }
                selectedObj = selectedobjs.Cast<IMgaFCO>().FirstOrDefault();
            }

            if (selectedObj == null)
            {
                GMEConsole.Error.WriteLine(SelectedObjErrorMessage);
                return;
            }

            if (selectedObj.MetaBase.Name != "ComponentRef")
            {
                GMEConsole.Error.WriteLine(SelectedObjErrorMessage);
                return;
            }

            if ((selectedObj as IMgaReference).Referred == null)
            {
                GMEConsole.Error.WriteLine("Selected ComponentRef is a null reference.");
                return;
            }

            GMEConsole.Info.WriteLine("Running CLM_light...");

            // everything is checked we can operate on the objects
            IMgaFCO designContainer = currentobj;
            IMgaFCO componentRef = selectedObj;
            IMgaModel component = (selectedObj as IMgaReference).Referred as IMgaModel;

            GMEConsole.Info.WriteLine("Discovering components.");

            //GMEConsole.Info.WriteLine("{0} components were found.", components.Count);

            // TODO: filter detected components if needed
            using (ComponentSelectionForm csf = new ComponentSelectionForm(component, currentobj as IMgaModel, GMEConsole.Error, GMEConsole.Info))
            {
                var dialogresult = csf.ShowDialog();

                if (dialogresult == System.Windows.Forms.DialogResult.OK)
                {
                    GMEConsole.Info.WriteLine("Inserting new components.");

                    List<IMgaFCO> components = new List<IMgaFCO>();

                    foreach (var item in csf.dgvSelector.SelectedRows)
                    {
                        var dgvr = item as System.Windows.Forms.DataGridViewRow;
                        var cli = dgvr.DataBoundItem as ComponentListItem;
                        components.Add(cli.Fco);
                    }

                    GMEConsole.Info.WriteLine("{0} components were selected.", components.Count);

                    List<KeyValuePair<IMgaFCO, string>> messages = new List<KeyValuePair<IMgaFCO, string>>();

                    var insertedComponents = InsertComponents(designContainer, componentRef, components, messages);
                }
            }
            GMEConsole.Info.WriteLine("Done.");
        }

        public bool CheckForValidContext(MgaFCO currentobj)
        {
            bool contextValid = true;

            // check if there is an object open
            if (currentobj == null)
            {
                GMEConsole.Error.WriteLine(ContextErrorMessage);
                contextValid = false;
            }

            // check if the currently opened model is a DesignContainer
            if (currentobj.MetaBase.Name != "DesignContainer")
            {
                GMEConsole.Error.WriteLine(ContextErrorMessage);
                contextValid = false;
            }

            // check if the currently opened model is a Library object
            if (currentobj.IsLibObject)
            {
                GMEConsole.Error.WriteLine("Cannot modify a Library object. Please select a valid DesignContainer.");
                contextValid = false;
            }

            // check if the currently opened model is an Instance
            if (currentobj.IsInstance)
            {
                GMEConsole.Error.WriteLine("Cannot modify an Instance. Please open a non-Instance DesignContainer.");
                contextValid = false;
            }

            // check if the currently opened model is an Alternative- or Optional-type DC
            if (currentobj.StrAttrByName["ContainerType"] != "Alternative" &&
                currentobj.StrAttrByName["ContainerType"] != "Optional") //added per META-2555
            {
                GMEConsole.Error.WriteLine(ContextErrorMessage);
                contextValid = false;
            }

            return contextValid;
        }   

        [ComVisible(false)]
        public List<IMgaFCO> InsertComponents(
            IMgaFCO designContainer,
            IMgaFCO componentRef,
            List<IMgaFCO> components,
            List<KeyValuePair<IMgaFCO, string>> messages)
        {
            Contract.Requires(designContainer as IMgaModel != null);
            Contract.Requires(componentRef as IMgaReference != null);
            Contract.Requires((componentRef as IMgaReference).Referred != null);
            Contract.Requires(components != null);

            List<IMgaFCO> result = new List<IMgaFCO>();

            IMgaModel container = designContainer as IMgaModel;
            IMgaReference compRef = componentRef as IMgaReference;

            var childComps = container.
                ChildFCOs.
                Cast<IMgaFCO>().
                Where(x => x.MetaBase.Name == "ComponentRef").
                Cast<IMgaReference>().
                Select(x => x.Referred).
                ToList();

            // get all connections which has the componentRef as an endpoint
            var childConnections = container.
                ChildFCOs.
                Cast<IMgaFCO>().
                Where(x => x is IMgaSimpleConnection).
                Cast<IMgaSimpleConnection>().
                ToList();


            // ith new component
            int iNewComponent = 0;

            foreach (var compToCreate in components)
            {
                if (childComps.Contains(compToCreate))
                {
                    // If the component already exists this function will not create it again
                    // skip
                    messages.Add(new KeyValuePair<IMgaFCO, string>(compToCreate, "Component already exists."));
                    continue;
                }

                // create reference
                var newRef = container.CreateReference(componentRef.MetaRole, compToCreate as MgaFCO);
                newRef.Name = compToCreate.Name;

                ////////

                //VehicleForge.VFSession session = null;
                //VFComponentExchange results = session.SendGetRequest<VFComponentExchange>("");
                //VFComponentExchange resultjson = Newtonsoft.Json.JsonConvert.DeserializeObject<VFComponentExchange>("");
                ////////


                result.Add(newRef);

                bool compatible = true;

                // create connections
                foreach (var connectionToCreate in childConnections)
                {
                    if (SafeMgaObjectCompare(connectionToCreate.SrcReferences.Cast<IMgaFCO>().FirstOrDefault(), compRef) ||
                        SafeMgaObjectCompare(connectionToCreate.DstReferences.Cast<IMgaFCO>().FirstOrDefault(), compRef))
                    {
                        try
                        {
                            var connRole = connectionToCreate.MetaRole;
                            var connSrc = connectionToCreate.Src;
                            var connDst = connectionToCreate.Dst;
                            var connSrcReference = connectionToCreate.SrcReferences.Cast<MgaFCO>().FirstOrDefault();
                            var connDstReference = connectionToCreate.DstReferences.Cast<MgaFCO>().FirstOrDefault();

                            // overwrite the new endpoints
                            if (SafeMgaObjectCompare(connectionToCreate.SrcReferences.Cast<IMgaFCO>().FirstOrDefault(), compRef))
                            {
                                connSrcReference = newRef;

                                // Check for objects with same name and same type
                                var srcCandidates = compToCreate.ChildObjects.OfType<MgaFCO>().Where(x => 
                                    x.Name == connSrc.Name &&
                                    x.Meta.Name == connSrc.Meta.Name);

                                MgaFCO srcCandidate = srcCandidates.FirstOrDefault();

                                if (srcCandidates.Count() > 1)
                                {
                                    messages.Add(new KeyValuePair<IMgaFCO, string>(
                                        compToCreate,
                                        "Not Inserted. It has more than one matching object named: " + connSrc.Name));
                                    compatible = false;
                                }

                                if (srcCandidate == null)
                                {
                                    messages.Add(new KeyValuePair<IMgaFCO, string>(
                                        compToCreate,
                                        "Not Inserted. It does not have a port with name: " + connSrc.Name));
                                    compatible = false;
                                }

                                connSrc = srcCandidate;
                            }

                            if (SafeMgaObjectCompare(connectionToCreate.DstReferences.Cast<IMgaFCO>().FirstOrDefault(), compRef))
                            {
                                connDstReference = newRef;

                                //var dstCandidate = compToCreate.ObjectByPath[connDst.Name] as MgaFCO;

                                var dstCandidates = compToCreate.ChildObjects.OfType<MgaFCO>().Where(x => 
                                    x.Name == connDst.Name &&
                                    x.Meta.Name == connDst.Meta.Name);

                                MgaFCO dstCandidate = dstCandidates.FirstOrDefault();

                                if (dstCandidates.Count() > 1)
                                {
                                    messages.Add(new KeyValuePair<IMgaFCO, string>(
                                        compToCreate,
                                        "Not Inserted. It has more than one matching object named: " + connDst.Name));
                                    compatible = false;
                                }

                                if (dstCandidate == null)
                                {
                                    messages.Add(new KeyValuePair<IMgaFCO, string>(
                                        compToCreate,
                                        "Not Inserted. It does not have port with name: " + connDst.Name));
                                    compatible = false;
                                }

                                connDst = dstCandidate;
                            }

                            // check end points
                            if (connSrc != null &&
                                connDst != null)
                            {
                                var newConnection = container.CreateSimpleConnDisp(
                                    connRole,
                                    connSrc,
                                    connDst,
                                    connSrcReference,
                                    connDstReference);
                            }
                        }
                        catch (Exception ex)
                        {
                            System.Diagnostics.Trace.TraceError(ex.ToString());
                            System.Diagnostics.Trace.TraceError("Probably some ports do not match.");
                            compatible = false;
                        }
                    }
                }

                if (compatible)
                {
                    iNewComponent = iNewComponent + 1;

                    foreach (IMgaPart part in newRef.Parts)
                    {
                        int x = 0;
                        int y = 0;
                        string icon;

                        componentRef.PartByMetaPart[part.Meta].GetGmeAttrs(out icon, out x, out y);

                        part.SetGmeAttrs(icon, x, y + 80 * iNewComponent);
                    }

                }
                else
                {
                    //messages.Add(new KeyValuePair<IMgaFCO, string>(compToCreate, "Component was skipped: " + compToCreate.Name));
                    result.Remove(newRef);
                    newRef.DestroyObject();
                }

            }

            foreach (var item in messages)
            {
                GMEConsole.Error.WriteLine("{0}: {1}", item.Key.ToMgaHyperLink(), item.Value);
            }

            GMEConsole.Info.WriteLine("{0} components were inserted. Detailed list as follows:", result.Count);

            foreach (var item in result)
            {
                GMEConsole.Info.WriteLine("- {0}", item.ToMgaHyperLink());
            }

            return result;
        }

        private bool SafeMgaObjectCompare(IMgaObject obj1, IMgaObject obj2)
        {
            if (obj1 == obj2)
            {
                return true;
            }
            else if (obj1 != null && obj2 != null)
            {
                return obj1.ID == obj2.ID;
            }
            else
            {
                return false;
            }
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
