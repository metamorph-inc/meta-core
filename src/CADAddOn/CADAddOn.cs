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

namespace CADAddOn
{
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CADAddOnAddon : IMgaComponentEx, IGMEVersionInfo, IMgaEventSink
    {

        private MgaAddOn addon;
        private bool componentEnabled = true;


        private bool handleEvents = true;


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

            // TODO: Handle global events
            // MessageBox.Show(@event.ToString());
            if (@event == globalevent_enum.APPEVENT_XML_IMPORT_BEGIN)
            {
                handleEvents = false;
                addon.EventMask = 0;
            }
            else if (@event == globalevent_enum.APPEVENT_XML_IMPORT_END)
            {
                unchecked { addon.EventMask = (uint)ComponentConfig.eventMask; }
                handleEvents = true;
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
            // TODO: Handle object events (OR eventMask with the members of objectevent_enum)
            // Warning: Only those events are received that you have subscribed for by setting ComponentConfig.eventMask

            // MessageBox.Show(eventMask.ToString());
            uint uOBJEVENT_REGISTRY = 0;
            uint uOBJEVENT_CREATED = 0;
            uint uOBJEVENT_ATTR = 0;
            uint uOBJEVENT_PRE_DESTROYED = 0;
            unchecked { uOBJEVENT_REGISTRY = (uint)objectevent_enum.OBJEVENT_REGISTRY; }
            unchecked { uOBJEVENT_CREATED = (uint)objectevent_enum.OBJEVENT_CREATED; }
            unchecked { uOBJEVENT_ATTR = (uint)objectevent_enum.OBJEVENT_ATTR; }
            unchecked { uOBJEVENT_PRE_DESTROYED = (uint)objectevent_enum.OBJEVENT_PRE_DESTROYED; }


            if ((eventMask & uOBJEVENT_CREATED) != 0)
            {
                //subject.CurrentAssociation
                if (subject.MetaBase.Name == "ValueFlow")
                {
                    bool srcCADProp = false;
                    string variableName ="";
                    string variableValue ="";

                    IMgaConnection valueFlowConn = subject as IMgaConnection;
                    foreach (MgaConnPoint cp in valueFlowConn.ConnPoints.Cast<IMgaConnPoint>().Where(x => x.ConnRole == "src")
                        .Where(cp => cp.Target.Meta.Name == "CADProperty"))
                    {
                        MgaFCO propParent;

                        if (cp.References.Count == 0)
                            propParent = cp.Target;
                        else
                        {
                            propParent = cp.References[1];
                            //propParent = cp.Target.ParentModel as MgaFCO;
                        }


                        if (propParent.Meta.Name == "ComponentRef")
                        {
                            variableName = cp.Target.get_StrAttrByName("ParameterName");

                            // go through registry nodes
                            string regPath = "CADPROPERTIES/" + variableName + "/Value";
                            MgaRegNode regNode = propParent.get_RegistryNode(regPath);
                            if (regNode != null)
                            {
                                variableValue = regNode.Value;
                            }

                        }
                        else
                        {
                            // directly get value
                            variableValue = propParent.get_StrAttrByName("Value");
                        }

                    }

                    if (!string.IsNullOrEmpty(variableValue))
                    {
                        foreach (MgaConnPoint cp in valueFlowConn.ConnPoints)
                        {
                            // assign Value and Unit attribute values
                            if (cp.ConnRole == "dst")
                            {
                                if (cp.Target.Meta.Name == "Property")
                                {
                                    if (string.IsNullOrEmpty(variableValue) == false)
                                        cp.Target.set_StrAttrByName("Value", variableValue);
                                }
                            }
                        }
                    }
                }
#if comment
                else if (subject.MetaBase.Name == "CADProperty")
                {
                    // find existing CADProperty
                    MgaFCO fcoSubj = subject as MgaFCO;
                    if (string.IsNullOrEmpty(fcoSubj.get_StrAttrByName("ParameterName")))
                    {
                        MgaModel parentModel = fcoSubj.ParentModel;
                        if ((parentModel.MetaBase.Name == "Component" || parentModel.MetaBase.Name == "ComponentAssembly" || parentModel.MetaBase.Name == "TestBench") && (!parentModel.IsInstance))
                        {
                            List<MgaFCO> cadProperties = parentModel.ExKindChildren("CADProperty").Cast<MgaFCO>().ToList();
                            List<string> existingProp = new List<string>();
                           
                            if (cadProperties.Count > 1)
                            {                         

                                for (int i = 0; i < cadProperties.Count; i++)
                                {
                                    string paramName = cadProperties[i].get_StrAttrByName("ParameterName");

                                    if (!string.IsNullOrEmpty(paramName))
                                        existingProp.Add(paramName);
                                }                                
                            }

                            CADPropertySelection dialog = new CADPropertySelection();
                            dialog.SetInitItemToBeChecked(existingProp);
                            dialog.ShowDialog();

                            string selectedProperty = dialog.selectedProperty;

                            if (!string.IsNullOrEmpty(selectedProperty))
                            {
                                fcoSubj.set_StrAttrByName("ParameterName", selectedProperty);
                                fcoSubj.Name = selectedProperty;
                            }
                        }
                    }

                }
#endif
            }
            else if ((eventMask & uOBJEVENT_REGISTRY) != 0)
            {
                MgaFCO compRef = subject as MgaFCO;
                if (compRef.MetaBase.Name == "ComponentRef")
                {
                    MgaReference rref = compRef as MgaReference;
                    foreach (IMgaConnPoint cp in rref.UsedByConns.Cast<IMgaConnPoint>().Where(x => x.ConnRole == "src")
                        .Where(cp => cp.Target.Meta.Name == "CADProperty"))
                    {
                        MgaFCO property = cp.References[1];
                        string variableName = cp.Target.get_StrAttrByName("ParameterName");
                        string variableValue = "";

                        string regPath = "CADPROPERTIES/" + variableName + "/Value";
                        MgaRegNode regNode = compRef.get_RegistryNode(regPath);
                        if (regNode != null)
                        {
                            variableValue = regNode.Value;
                        }

                        if (!string.IsNullOrEmpty(variableValue))
                        {
                            MgaConnection conn = cp.Owner;
                            foreach (IMgaConnPoint dstCP in conn.ConnPoints.Cast<IMgaConnPoint>().Where(x => x.ConnRole == "dst"))
                            {
                                dstCP.Target.set_StrAttrByName("Value", variableValue);
                            }
                        }
                    }

                }
                
            }
            else if ((eventMask & uOBJEVENT_ATTR) != 0)
            {
                if (subject.MetaBase.Name == "CADProperty")
                {
                    MgaFCO cadProp = subject as MgaFCO;
                    foreach (IMgaConnPoint cp in cadProp.PartOfConns)
                    {
                        if (cp.ConnRole == "src")
                        {
                            MgaSimpleConnection simple = cp.Owner as MgaSimpleConnection;
                            MgaFCO dstProp = simple.Dst;                           
                            dstProp.set_StrAttrByName("Value", cadProp.get_StrAttrByName("Value"));
                        }
                    }
                }

            }
            else if ((eventMask & uOBJEVENT_PRE_DESTROYED) != 0)
            {
           
                if (subject.MetaBase.Name == "ValueFlow")
                {
                    IMgaConnection valueFlowConn = subject as IMgaConnection;
                    foreach (MgaConnPoint cp in valueFlowConn.ConnPoints.Cast<IMgaConnPoint>().Where(x => x.ConnRole == "dst"))
                    {
                        cp.Target.set_StrAttrByName("Value", "");
                    }
                }
            
            }
        }

        #endregion


        #region IMgaComponentEx Members

        public void Initialize(MgaProject p)
        {
            // Creating addon
            p.CreateAddOn(this, out addon);
            // Setting event mask (see ComponentConfig.eventMask)
            unchecked
            {
                addon.EventMask = (uint)ComponentConfig.eventMask;
            }
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

    public static class Extensions
    {
        public static IMgaFCO GetParentOrRefport(this IMgaConnPoint p)
        {
            string role = p.ConnRole;
            if (p.References.Count == 0)
            {
                return p.Target;
                //return p.Target.ParentFolder;
            }
            else
            {
                return p.Target.ParentModel;
                //return p.References[1];
            }
        }

        public static IEnumerable<MgaObject> ExKindChildren<T>(
        this T subject,
        string Role)
        where T : IMgaObject
        {
            foreach (MgaObject child in subject.ChildObjects)
            {
                if (child.MetaBase.Name == Role)
                {
                    yield return child;
                }
            }
        }

    }
}
