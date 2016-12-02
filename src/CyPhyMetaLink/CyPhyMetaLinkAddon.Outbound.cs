using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using GME.MGA.Core;
using GME.MGA;

using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyMLClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using MetaLinkBridge = CyPhyMetaLinkBridgeClient;
using MetaLinkProtobuf = edu.vanderbilt.isis.meta;
using Newtonsoft.Json;
using GME.MGA.Meta;
using CAD;
using edu.vanderbilt.isis.meta;
using System.Runtime.InteropServices;
using META;

namespace CyPhyMetaLink
{
    public struct CADParameter
    {
        public string Name;
        public string Type;
        public string Value;

        public CADParameter(string name, string type, string value)
        {
            Name = name;
            Type = type;
            Value = value;
        }
    }


    public partial class CyPhyMetaLinkAddon
    {
        private string previouslySelectedParent = null;
        HashSet<string> selectedFCOIDs = new HashSet<string>();
        [ComVisible(true)] // called by CPMDecorator
        public void SetSelected(IMgaFCO fco, bool selected)
        {
            try
            {
                // GMEConsole.Out.WriteLine(fco.Name + " selected");
                if (fco.ParentModel == null)
                {
                    return;
                }
                if (previouslySelectedParent != null && previouslySelectedParent != fco.ParentModel.ID)
                {
                    selectedFCOIDs = new HashSet<string>();
                }
                previouslySelectedParent = fco.ParentModel.ID;
                string topicGuid = null;
                MgaModel ancestor = fco.ParentModel;
                while (ancestor != null)
                {
                    if (ancestor.Meta.Name == "ComponentAssembly")
                    {
                        var assembly = CyPhyMLClasses.ComponentAssembly.Cast(ancestor);
                        if (designIdToCadAssemblyXml.ContainsKey(assembly.Guid.ToString()))
                        {
                            topicGuid = assembly.Guid.ToString();
                            break;
                        }
                    }
                    else if (ancestor.Meta.Name == "CADModel")
                    {
                        /* var cadmodel = CyPhyMLClasses.CADModel.Cast(ancestor);
                         if (assemblyExesStarted.Contains(cadmodel) || assemblyExesStarted.Contains(component.Guid.ToString()))
                         {
                         }*/

                    }
                    else if (ancestor.Meta.Name == "Component")
                    {
                        var component = CyPhyMLClasses.Component.Cast(ancestor);
                        if (syncedComponents.Count > 0 && ((topicGuid != null && syncedComponents.ContainsKey(topicGuid)) || syncedComponents.ContainsKey(component.Attributes.AVMID)))
                        {
                            topicGuid = component.Attributes.AVMID;
                        }
                        break;
                    }
                    else
                    {
                        return;
                    }
                    ancestor = ancestor.ParentModel;
                }
                if (topicGuid == null)
                {
                    return;
                }
                if (selected)
                {
                    selectedFCOIDs.Add(fco.ID);
                }
                else
                {
                    selectedFCOIDs.Remove(fco.ID);
                }
                selectedFCOIDs.RemoveWhere(id => addon.Project.GetFCOByID(id).Status != (int)objectstatus_enum.OBJECT_EXISTS);
                var edit = new Edit();
                edit.origin.Add(GMEOrigin);
                edit.editMode = Edit.EditMode.POST;
                edit.mode.Add(Edit.EditMode.POST);
                edit.actions.Add(new MetaLinkProtobuf.Action());
                edit.actions[0].actionMode = MetaLinkProtobuf.Action.ActionMode.SELECT;
                edit.actions[0].payload = new Payload();
                var selectedFCOs = selectedFCOIDs.Select(id => addon.Project.GetFCOByID(id));
                edit.actions[0].payload.components.AddRange(
                    selectedFCOs.Where(f => f.Meta.Name == "ComponentRef")
                        .Select(CyPhyMLClasses.ComponentRef.Cast)
                        .Select(component =>
                            new edu.vanderbilt.isis.meta.CADComponentType()
                            {
                                ComponentID = component.Attributes.InstanceGUID
                            }));
                edit.actions[0].payload.datums.AddRange(
                    selectedFCOs.Where(f => datumKinds.Contains(f.Meta.Name))
                        .Select(CyPhyMLClasses.CADDatum.Cast)
                        .Where(datum => datum.ParentContainer.Impl.MetaBase.Name == typeof(CyPhyML.CADModel).Name
                            && datum.ParentContainer.ParentContainer.Impl.MetaBase.Name == typeof(CyPhyML.Component).Name)
                        .Select(datum => new DatumType()
                        {
                            name = datum.Attributes.DatumName,
                            type = datumKindsMap[datum.Impl.MetaBase.Name],
                            componentId = CyPhyMLClasses.Component.Cast(datum.ParentContainer.ParentContainer.Impl).Attributes.AVMID
                        }));
                SyncedComponentData cdata = syncedComponents[topicGuid];
                edit.topic.Add(cdata.InstanceId);
                bridgeClient.SendToMetaLinkBridge(edit);
            }
            catch (Exception e)
            {
                GMEConsole.Warning.WriteLine("An exception has been thrown during selection: " + e.Message);
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
            // 6-28-2013: Harmon added an enable property that gets set from CyPhyMLSync. That code is currently commented out in CyPhyMLSync.
            if (!enabled || !componentEnabled || !handleEvents)
            {
                return;
            }

            //if (!componentEnabled || !handleEvents) {
            //    return;
            //}

            if (GMEConsole == null)
            {
                GMEConsole = GME.CSharp.GMEConsole.CreateFromProject(subject.Project);
            }


            uint uOBJEVENT_ATTR = 0;
            uint uOBJEVENT_CONNECTED = 0;
            uint uOBJEVENT_CREATED = 0;
            uint uOBJEVENT_PROPERTIES = 0;
            uint uOBJEVENT_OPENMODEL = 0;
            uint uOBJEVENT_CLOSEMODEL = 0;
            uint uOBJEVENT_NEWCHILD = 0;
            uint uOBJEVENT_PRE_STATUS = 0;
            uint uOBJEVENT_PRE_DESTROYED = 0;

            unchecked { uOBJEVENT_ATTR = (uint)objectevent_enum.OBJEVENT_ATTR; }
            unchecked { uOBJEVENT_CONNECTED = (uint)objectevent_enum.OBJEVENT_CONNECTED; }
            unchecked { uOBJEVENT_CREATED = (uint)objectevent_enum.OBJEVENT_CREATED; }
            unchecked { uOBJEVENT_PROPERTIES = (uint)objectevent_enum.OBJEVENT_PROPERTIES; }
            unchecked { uOBJEVENT_OPENMODEL = (uint)objectevent_enum.OBJEVENT_OPENMODEL; }
            unchecked { uOBJEVENT_CLOSEMODEL = (uint)objectevent_enum.OBJEVENT_CLOSEMODEL; }
            unchecked { uOBJEVENT_NEWCHILD = (uint)objectevent_enum.OBJEVENT_NEWCHILD; }
            unchecked { uOBJEVENT_PRE_STATUS = (uint)objectevent_enum.OBJEVENT_PRE_STATUS; }
            unchecked { uOBJEVENT_PRE_DESTROYED = (uint)objectevent_enum.OBJEVENT_PRE_DESTROYED; }
            uint uOBJEVENT_REFERENCED = unchecked((uint)objectevent_enum.OBJEVENT_REFERENCED);
            uint uOBJEVENT_DESTROYED = unchecked((uint)objectevent_enum.OBJEVENT_DESTROYED);
            uint uOBJEVENT_RELATION = unchecked((uint)objectevent_enum.OBJEVENT_RELATION);


            // TODO: Handle object events (OR eventMask with the members of objectevent_enum)
            // Warning: Only those events are received that you have subscribed for by setting ComponentConfig.eventMask

            // If the event is OBJEVENT_DESTROYED, most operations on subject will fail
            //   Safe operations: getting Project, ObjType, ID, MetaRole, Meta, MetaBase, Name, AbsPath
            //   Operations that will fail: all others, including attribute access and graph navigation
            //     Try handling OBJEVENT_PRE_DESTROYED if these operations are necessary

            // Be careful not to modify Library objects (check subject.IsLibObject)

            // MessageBox.Show(eventMask.ToString());
            // GMEConsole.Out.WriteLine(subject.Name);

#if DEBUG
            GMEConsole.Info.WriteLine("EventID: " + eventMask.ToString());
#endif

            if ((eventMask & uOBJEVENT_PRE_STATUS) != 0)
            {
                if (param != null)
                {
                    string paramString = param as string;
                    string[] paramData = paramString.Split(',');

                    if (paramData[0] == "ATTR" && paramData[2] == "Cleared")
                    {
                        string attributeName = paramData[1];
                        MgaFCO mgaFCO = subject as MgaFCO;
                        MgaMetaAttribute mgaMetaAttribute = mgaFCO.Meta.get_AttributeByName(attributeName);
                        GMEConsole.Error.WriteLine("Attribute \"" + attributeName + "\" cleared, default value = \"" + mgaMetaAttribute.get_DefaultValue().ToString() + "\"");
                    }
                }
            }

            if (AssemblyID != null
                && (((eventMask & uOBJEVENT_RELATION) != 0 && (eventMask & uOBJEVENT_DESTROYED) == 0 && (eventMask & uOBJEVENT_CREATED) == 0)
                    || (eventMask & uOBJEVENT_PRE_DESTROYED) != 0 && (eventMask & uOBJEVENT_CREATED) == 0)
                && subject.Status == (int)objectstatus_enum.OBJECT_EXISTS)
            {
                if (CyphyMetaLinkUtils.IsValidComponentRef(subject))
                {
                    CyPhyML.ComponentRef componentRef = CyPhyMLClasses.ComponentRef.Cast(subject);
                    var parent = componentRef.ParentContainer;
                    if (parent.Kind == typeof(CyPhyML.ComponentAssembly).Name)
                    {
                        foreach (CyPhyML.ComponentAssembly assembly in CyphyMetaLinkUtils.GetContainingAssemblies(CyPhyMLClasses.ComponentAssembly.Cast(parent.Impl)))
                        {
                            if (assembly.Guid == Guid.Parse(AssemblyID))
                            {
                                RestartAssemblySyncAtEndOfTransaction(assembly);
                            }
                        }
                    }
                }
                else if (subject.MetaBase.Name == "ConnectorComposition")
                {
                    CyPhyML.ConnectorComposition connector = CyPhyMLClasses.ConnectorComposition.Cast(subject);
                    var parent = connector.ParentContainer;
                    if (parent.Kind == typeof(CyPhyML.ComponentAssembly).Name)
                    {
                        foreach (CyPhyML.ComponentAssembly assembly in CyphyMetaLinkUtils.GetContainingAssemblies(CyPhyMLClasses.ComponentAssembly.Cast(parent.Impl)))
                        {
                            if (assembly.Guid == Guid.Parse(AssemblyID))
                            {
                                RestartAssemblySyncAtEndOfTransaction(assembly);
                            }
                        }
                    }
                }

                // If the user adds or deletes an FCO contained in a Component that is contained in the synced design, restart the sync (META-1811)
                if (subject is MgaFCO)
                {
                    CyPhyML.Component component = null;
                    MgaModel model = (subject as MgaFCO).ParentModel;
                    while (model != null)
                    {
                        if (model.Meta.Name == "Component")
                        {
                            component = CyPhyMLClasses.Component.Cast(model);
                            break;
                        }
                        model = model.ParentModel;
                    }
                    if (component != null)
                    {
                        foreach (CyPhyML.ComponentAssembly asm in CyphyMetaLinkUtils.GetContainingAssemblies(component))
                        {
                            if (asm.Guid == Guid.Parse(AssemblyID))
                            {
                                var syncedAssembly = CyphyMetaLinkUtils.GetComponentAssemblyByGuid(subject.Project, AssemblyID);
                                RestartAssemblySyncAtEndOfTransaction(syncedAssembly);
                            }
                        }
                    }
                }
            }

            if ((eventMask & uOBJEVENT_PROPERTIES) != 0 && param != null && (eventMask & uOBJEVENT_DESTROYED) == 0)
            {
                // Change of a parameter on componentref
                if (CyphyMetaLinkUtils.IsComponentRef(subject))
                {
                    if (!syncedComponents.ContainsKey(CyPhyMLClasses.Property.Cast(subject).ParentContainer.Guid.ToString()))
                    {
                        return;
                    }

                    string paramName = null;
                    object[] paramData = param as object[];
                    string p = paramData[0].ToString();
                    if (p.StartsWith("PROPERTIES:"))
                    {
                        try
                        {
                            paramName = p.Substring(p.IndexOf(':') + 1);
                        }
                        catch (Exception)
                        {
                            // Fail silently
                        }
                        if (paramName != null)
                        {
                            string paramValue = paramData[1].ToString();
                            if (paramName == "Name")
                            {
                                MetaLinkProtobuf.Edit editMsg = UpdateComponentNameProtoBufMsg(CyPhyMLClasses.ComponentRef.Cast(subject), paramValue);
                                bridgeClient.SendToMetaLinkBridge(editMsg);
                            }
                        }
                    }
                }
            }

            if ((eventMask & uOBJEVENT_ATTR) != 0 && param != null && (eventMask & uOBJEVENT_DESTROYED) == 0)
            {
                // GMEConsole.Info.WriteLine("CyPhyMLPropagate OBJEVENT_ATTR");

                object[] paramData = param as object[];

                string attributeName = "";   //((string)paramData[0]).Substring(((string)paramData[0]).IndexOf(':') + 1);
                foreach (var item in paramData)
                {
                    string s = item.ToString();
                    if (!string.IsNullOrEmpty(s))
                    {
                        if (s.Contains("ATTR:"))
                        {
                            string tmp = s.Substring(s.IndexOf(':') + 1);
                            if (tmp == "Value" || tmp == "InstanceGUID")
                            {
                                attributeName = tmp;
                            }
                        }
                    }
                }

                if (attributeName == "Value")
                {
                    MgaFCO mgaFCO = subject as MgaFCO;
                    MgaMetaAttribute mgaMetaAttribute = mgaFCO.Meta.get_AttributeByName(attributeName);

#if DEBUG
                    GMEConsole.Error.WriteLine("Attribute change detected, name = \"" + attributeName + "\", old value = \"" + paramData[1].ToString() + "\", new value = \"" + mgaFCO.get_Attribute(mgaMetaAttribute).Value.ToString() + "\"");
#endif

                    // A property has been modified
                    string kind = subject.MetaBase.Name;
                    if (kind == "Property" ||
                        kind == "Parameter" ||
                        kind == "Metric")
                    {
                        // Does this parameter belong to the Meta-Linked assembly?
                        if (kind == "Property")
                        {
                            if (!syncedComponents.ContainsKey(CyPhyMLClasses.Property.Cast(subject).ParentContainer.Guid.ToString()))
                            {
                                return;
                            }
                        }
                        else if (kind == "Parameter")
                        {
                            if (!syncedComponents.ContainsKey(CyPhyMLClasses.Parameter.Cast(subject).ParentContainer.Guid.ToString()))
                            {
                                return;
                            }
                        }
                        else if (kind == "Metric")
                        {
                            if (!syncedComponents.ContainsKey(CyPhyMLClasses.Metric.Cast(subject).ParentContainer.Guid.ToString()))
                            {
                                return;
                            }
                        }
                        // If any parameter changes, the whole assembly needs to be re-built
                        // The elaborator can't trace back the original references
                        // So it can't be used to change the Creo model partially
                        // This can be changed when the elaborator is replaced by some more fine-tailored solution
                        CyPhyML.ComponentAssembly assembly = CyphyMetaLinkUtils.GetComponentAssemblyByGuid(addon.Project, AssemblyID);
                        RestartAssemblySyncAtEndOfTransaction(assembly);
                        /*CyPhyML.ValueFlowTarget vft = CyPhyMLClasses.ValueFlowTarget.Cast(subject);
                        if (vft != null)
                        {
                            ValueFlowTraverse traverser = new ValueFlowTraverse();
                            traverser.TraverseValueFlow(vft);

                            if (traverser.Found.Count > 0)
                            {
                                handleEvents = false;
                                MgaModel model = (subject as MgaFCO).ParentModel;
                                RunFormulaEvaluator(CyPhyMLClasses.ValueFlowTarget.Cast(subject),
                                                    model as MgaFCO,
                                                    subject.Project);
                                handleEvents = true;
                                foreach (var item in traverser.Found)
                                {
                                    ModifyCADParameter(item);
                                }
                            }
                        }*/

                    }
                }

                else if (attributeName == "InstanceGUID")
                {
                    MgaFCO mgaFCO = subject as MgaFCO;
                    if (CyphyMetaLinkUtils.IsValidComponentRef(subject))
                    {
                        AddComponent(subject);
                    }
                }
            }       // end if (eventMask)


            if ((eventMask & uOBJEVENT_PROPERTIES) != 0)
            {
                if (CyphyMetaLinkUtils.IsComponent(subject))
                {
                    if (param != null)
                    {
                        object[] paramData = param as object[];
                        string parameterName = ((string)paramData[0]).Substring(((string)paramData[0]).IndexOf(':') + 1);

                        if (parameterName != "Name")
                        {
                            return;
                        }

                        GMEConsole.Error.WriteLine("Name change detected, old name = \"" + ((string)paramData[1]) + "\", new name = \"" + subject.Name + "\"");

                        CyPhyML.Component cyPhyMLComponent = CyPhyMLClasses.Component.Cast(subject);

                        /*
                        Message message = new Message();
                        message.cadComponent = new CADComponentType();

                        message.cadComponent.ComponentID = cyPhyMLComponent.Attributes.InstanceGUID;
                        message.cadComponent.Name = cyPhyMLComponent.Name;


                        message.type = Message.MessageType.UPDATE;

                        _socketQueue.enQueue(message);
                        */
                    }
                }
            }

            if ((eventMask & uOBJEVENT_OPENMODEL) != 0)
            {
                //GMEConsole.Info.WriteLine("OBJEVENT_OPENMODEL");
            }

            // Component or Constraint has been created
            if ((eventMask & uOBJEVENT_CREATED) != 0 && (eventMask & uOBJEVENT_DESTROYED) == 0 && (subject is MgaFCO) && ((MgaFCO)subject).ParentModel != null)
            {
#if DEBUG
                GMEConsole.Info.WriteLine("OBJEVENT_CREATED");
#endif

                /*if (CyphyMetaLinkUtils.IsComponentRef(subject))
                {
                    AddComponent(subject);
                }*/


                if (subject.MetaBase.Name == "ConnectorComposition")
                {
                    AddConnection(subject);
                }
            }
        }

        /// <summary>
        /// A component has been added.
        /// </summary>
        /// <param name="mgaComponentRef"></param>
        private void AddComponent(MgaObject mgaComponentRef)
        {
            // [1] Find ModelName
            // [2] Find ModelType
            // [3] Find URL - search path
            // [4] Find Instance GUID
            // [5] CADParameters
            // [6] Create Protobuf Message
            // [7] Send message

            CyPhyML.ComponentRef componentRef = CyPhyMLClasses.ComponentRef.Cast(mgaComponentRef);

            // Does this component belong to the Meta-Linked assembly?
            if (!syncedComponents.ContainsKey(componentRef.ParentContainer.Guid.ToString()) && !syncedComponents.ContainsKey(componentRef.Guid.ToString()))
            {
                return;
            }

            CyPhyML.Component component = componentRef.Referred.Component;
            if (component != null)
            {
                // [6] Create Protobuf message
                MetaLinkProtobuf.Edit message = AddComponentProtoBufMsg(componentRef);

                if (message != null)
                {
                    // [6.5] Add SearchPath to message
                    try
                    {
                        var env = new MetaLinkProtobuf.Environment
                        {
                            name = SearchPathStr,
                        };

                        AddSearchPathToEnvironment(component, env);

                        if (env.value.Count > 0)
                        {
                            message.actions[0].environment.Add(env);
                        }
                    }
                    catch (IOException)
                    {
                        // XXX
                    }

                    // [7] Send message
                    bridgeClient.SendToMetaLinkBridge(message);
                }
            }
        }

        private void AddSearchPathToEnvironment(CyPhyML.Component component, MetaLinkProtobuf.Environment env)
        {
            string path = Path.GetDirectoryName(GetCadModelPath(component));
            if (path == null)
            {
                // not everything needs a CAD model
                // GMEConsole.Warning.WriteLine(String.Format("MetaLink warning: component '{0}' has no CAD model", component.Name));
            }
            else
            {
                env.value.Add(path);
            }
        }

        private string GetCadModelPath(CyPhyML.Component component)
        {
            string modelPath = component.GetDirectoryPath();
            if (modelPath == null)
            {
                return null;
            }
            else
            {
                foreach (var cadModel in component.Children.CADModelCollection)
                {
                    string cadModelAbsolutePath;
                    if (cadModel.TryGetResourcePath(out cadModelAbsolutePath, META.ComponentLibraryManager.PathConvention.ABSOLUTE))
                    {
                        return cadModelAbsolutePath;
                    }
                }
            }
            return null;
        }

        /// <summary>
        /// A connection has been added.
        /// </summary>
        /// <param name="subject"></param>
        private void AddConnection(MgaObject subject)
        {
            // [1] Traverse a ConnectorComposition connection
            // [2] if (2 ends of a ConnectorComposition ends inside a Component)
            //              Find matching datums
            //              Create a constraint message
            //              Send message
            // [3] Create Protobuf message
            // [4] Send message

            CyPhyML.ConnectorComposition connection = CyPhyMLClasses.ConnectorComposition.Cast(subject);


            if (connection != null)
            {
                // Is this an event from a Meta-Linked assembly?
                if (!syncedComponents.ContainsKey(connection.ParentContainer.Guid.ToString()))
                {
                    return;
                }

                // snyako: removing and adding that connection back doesn't always work as connection doesn't always map the same way in Creo. Need to investigate this
                // Currently just re-sync the whole assembly
                CyPhyML.ComponentAssembly assembly = CyphyMetaLinkUtils.GetComponentAssemblyByGuid(addon.Project, AssemblyID);
                RestartAssemblySyncAtEndOfTransaction(assembly);

                /*
                // [1]
                //CyPhyML.Connector startconnector = connection.SrcEnds.Connector;
                //CyPhy2CAD_CSharp.CommonTraversal traverser = new CyPhy2CAD_CSharp.CommonTraversal(startconnector);
                CyPhy2CAD_CSharp.CommonTraversal traverser = new CyPhy2CAD_CSharp.CommonTraversal(connection, CyphyMetaLinkUtils.GetComponentAssemblyByGuid(addon.Project, AssemblyID));
                if (traverser.FoundConnectedNodes.Count != 2)
                {
#if DEBUG
                    GMEConsole.Warning.WriteLine("Unable to create constraint because >2 or <2 Connectors are connected together [" + connection.Path + "]");
#endif
                    return;
                }


                // [2]
                if (traverser.FoundConnectedNodes.Count == 2)
                {
                    if (traverser.FoundConnectedNodes[0].ParentContainer.Kind == "Component" && traverser.FoundConnectedNodes[1].ParentContainer.Kind == "Component")
                    {
                        MgaFCOs dstRefs = ((IMgaSimpleConnection)connection.Impl).DstReferences;
                        MgaReference dstRef = null;
                        if (dstRefs != null)
                        {
                            dstRef = dstRefs.Cast<MgaReference>().FirstOrDefault();
                        }
                        MgaFCOs srcRefs = ((IMgaSimpleConnection)connection.Impl).SrcReferences;
                        MgaReference srcRef = null;
                        if (srcRefs != null)
                        {
                            srcRef = srcRefs.Cast<MgaReference>().FirstOrDefault();
                        }
                        if (srcRef == null || dstRef == null)
                        {
                            return;
                        }

                        string aInstanceID;
                        string bInstanceID;
                        if (traverser.FoundConnectedNodes[0].ParentContainer.ID == dstRef.Referred.ID)
                        {
                            aInstanceID = CyPhyMLClasses.ComponentRef.Cast(dstRef).Attributes.InstanceGUID;
                            bInstanceID = CyPhyMLClasses.ComponentRef.Cast(srcRef).Attributes.InstanceGUID;
                        }
                        else
                        {
                            aInstanceID = CyPhyMLClasses.ComponentRef.Cast(srcRef).Attributes.InstanceGUID;
                            bInstanceID = CyPhyMLClasses.ComponentRef.Cast(dstRef).Attributes.InstanceGUID;
                        }

                        //= (traverser.FoundConnectedNodes[0].ParentContainer as CyPhyML.Component).Attributes.InstanceGUID;
                        //= (traverser.FoundConnectedNodes[1].ParentContainer as CyPhyML.Component).Attributes.InstanceGUID;
                        CyPhyML.CADModel acadmodel = CyphyMetaLinkUtils.FindCADModelObject(traverser.FoundConnectedNodes[0].ParentContainer as CyPhyML.Component);
                        CyPhyML.CADModel bcadmodel = CyphyMetaLinkUtils.FindCADModelObject(traverser.FoundConnectedNodes[1].ParentContainer as CyPhyML.Component);
                        if (acadmodel != null && bcadmodel != null)
                        {
                            CyPhy2CAD_CSharp.DataRep.StructuralInterfaceConstraint asirep = new CyPhy2CAD_CSharp.DataRep.StructuralInterfaceConstraint(traverser.FoundConnectedNodes[0] as CyPhyML.Connector,
                                                                                                                                                       traverser.FoundConnectedNodes[0].ParentContainer.ID,
                                                                                                                                                       aInstanceID);
                            asirep.PopulateStructuralInterface(traverser.FoundConnectedNodes[0] as CyPhyML.Connector,
                                                               acadmodel);

                            CyPhy2CAD_CSharp.DataRep.StructuralInterfaceConstraint bsirep = new CyPhy2CAD_CSharp.DataRep.StructuralInterfaceConstraint(traverser.FoundConnectedNodes[1] as CyPhyML.Connector,
                                                                                                                                                       traverser.FoundConnectedNodes[1].ParentContainer.ID,
                                                                                                                                                       bInstanceID);
                            bsirep.PopulateStructuralInterface(traverser.FoundConnectedNodes[1] as CyPhyML.Connector,
                                                               bcadmodel);


                            List<string> Errors_L = new List<string>();
                            List<Tuple<CyPhy2CAD_CSharp.DataRep.Datum, CyPhy2CAD_CSharp.DataRep.Datum>> constraintPairs = new List<Tuple<CyPhy2CAD_CSharp.DataRep.Datum, CyPhy2CAD_CSharp.DataRep.Datum>>();
                            CyPhy2CAD_CSharp.DataRep.StructuralInterfaceConstraint.MatchStructuralInterfaceDatums(asirep,
                                                                                            bsirep,
                                                                                            constraintPairs,
                                                                                            Errors_L);

                            // [] Figure out which component to add the constraint to
                            int aConstraints = 0;
                            int bConstraints = 0;
                            if (!InstanceIDToConstraint_Table.TryGetValue(aInstanceID, out aConstraints))
                            {
                                // root component
                                InstanceIDToConstraint_Table.Add(aInstanceID, 1);
                                aConstraints = 1;
                            }

                            if (!InstanceIDToConstraint_Table.TryGetValue(bInstanceID, out bConstraints))
                            {
                                // root component
                                InstanceIDToConstraint_Table.Add(bInstanceID, 1);
                                bConstraints = 1;
                            }

                            string constraintComponentID;
                            if (aConstraints > bConstraints)
                            {
                                constraintComponentID = bInstanceID;
                                InstanceIDToConstraint_Table[bInstanceID] = InstanceIDToConstraint_Table[bInstanceID] + 1;
                            }
                            else
                            {
                                constraintComponentID = aInstanceID;
                                InstanceIDToConstraint_Table[aInstanceID] = InstanceIDToConstraint_Table[aInstanceID] + 1;
                            }


                            // [3] Create Protobuf Message
                            MetaLinkProtobuf.Edit message = CreateAddConstraintProtoBufMsg(constraintComponentID,
                                                                                           constraintPairs);

                            // [4] Send message
                            bridgeClient.SendToMetaLinkBridge(message);
                        }
                    }
                 */
            }
        }

#if false
        /// <summary>
        /// A parameter has been modified
        /// </summary>
        /// <param name="cyphycadparam"></param>
        private void ModifyCADParameter(CyPhyML.CADParameter cyphycadparam)
        {
            // [1] Find ComponentInstanceID
            // [2] Create Protobuf Message
            // [3] Send message

            if (cyphycadparam != null)
            {
                CADParameter cadparam = new CADParameter((cyphycadparam.Attributes.ParameterName == "") ? cyphycadparam.Name : cyphycadparam.Attributes.ParameterName,
                                                            cyphycadparam.Attributes.CADParameterType.ToString(),
                                                            (cyphycadparam.Attributes.Value == "") ? (cyphycadparam.Attributes.DefaultValue == "") ? "0" : cyphycadparam.Attributes.DefaultValue : cyphycadparam.Attributes.Value);


                // [1] Find ComponentInstanceID
                if (cyphycadparam.ParentContainer != null && cyphycadparam.ParentContainer.ParentContainer != null)
                {
                    if (cyphycadparam.ParentContainer.Kind=="ComponentRef" && cyphycadparam.ParentContainer.ParentContainer.Kind == "Component")
                    {
                        string componentInstanceID = (cyphycadparam.ParentContainer as CyPhyML.ComponentRef).Attributes.InstanceGUID;


                        // [2] Create Protobuf Message
                        MetaLinkProtobuf.Edit message = ModifyCADParameterProtoBufMsg(componentInstanceID,
                                                                                        cadparam);

                        // [3] Send message
                        bridgeClient.SendToMetaLinkBridge(message);
                    }
                    else
                    {
                        GMEConsole.Warning.WriteLine("Parent container for parameter {0} is not a ComponentRef." + cyphycadparam.Name);
                    }
                }
                else
                {
                    GMEConsole.Warning.WriteLine("Can't find parent container for parameter: " + cyphycadparam.Name);
                }


            }
        }       // end function
#endif 

        private bool SendInterest(Action<MetaLinkProtobuf.Edit> noticeaction, params string[] topic)
        {
            MetaLinkProtobuf.Edit message = new MetaLinkProtobuf.Edit
            {
                editMode = MetaLinkProtobuf.Edit.EditMode.INTEREST,
                guid = Guid.NewGuid().ToString(),
                //sequence = 0,
            };
            message.origin.Add(GMEOrigin);
            message.topic.AddRange(topic.ToList());
            if (noticeaction != null)
            {
                noticeActions.Add(message.guid, noticeaction);
            }
            string topicstr = String.Join("_", topic);
            // The bridge will send us the same message multiple times if we express interest multiple times
            if (!interests.ContainsKey(topicstr))
            {
                interests.Add(topicstr, 1);
                bridgeClient.SendToMetaLinkBridge(message);
                return true;
            }
            else
            {
                interests[topicstr]++;
            }
            return false;
        }

        private bool SendDisinterest(bool removeInterest, params string[] topic)
        {
            MetaLinkProtobuf.Edit message = new MetaLinkProtobuf.Edit
            {
                editMode = MetaLinkProtobuf.Edit.EditMode.DISINTEREST,
                guid = Guid.NewGuid().ToString(),
                //sequence = 0,
            };
            message.origin.Add(GMEOrigin);
            message.topic.AddRange(topic.ToList());
            string topicstr = String.Join("_", topic);
            if (interests.ContainsKey(topicstr))
            {
                if (!removeInterest)
                {
                    interests[topicstr]--;
                }
                else
                {
                    interests[topicstr] = 0;
                }

                if (interests[topicstr] == 0)
                {
                    interests.Remove(topicstr);
                    bridgeClient.SendToMetaLinkBridge(message);
                    return true;
                }
            }
            else
            {
                GMEConsole.Warning.WriteLine("Sending disinterest for topic without interest entry");
                bridgeClient.SendToMetaLinkBridge(message);
            }
            return false;
        }

        private void SendComponentList(string instanceId)
        {
            MetaLinkProtobuf.Edit manifestMessage = new MetaLinkProtobuf.Edit
            {
                editMode = Edit.EditMode.POST,
            };
            manifestMessage.topic.Add(instanceId);
            var manifestAction = new MetaLinkProtobuf.Action();
            manifestMessage.actions.Add(manifestAction);
            manifestAction.actionMode = MetaLinkProtobuf.Action.ActionMode.REQUEST_COMPONENT_LIST;
            var rf = CyPhyMLClasses.RootFolder.GetRootFolder(addon.Project);
            Queue<CyPhyML.Components> componentsQueue = new Queue<CyPhyML.Components>();
            foreach (var childComponents in rf.Children.ComponentsCollection)
            {
                componentsQueue.Enqueue(childComponents);
            }
            while (componentsQueue.Count > 0)
            {
                var components = componentsQueue.Dequeue();
                manifestAction.manifest.Add(new ComponentManifestNode()
                {
                    nodeMode = ComponentManifestNode.NodeMode.FOLDER,
                    guid = components.Guid.ToString("D"),
                    name = components.Name
                });
                foreach (var childComponents in components.Children.ComponentsCollection)
                {
                    componentsQueue.Enqueue(childComponents);
                }
                foreach (var component in components.Children.ComponentCollection)
                {
                    manifestAction.manifest.Add(new ComponentManifestNode()
                    {
                        nodeMode = ComponentManifestNode.NodeMode.COMPONENT,
                        guid = component.Attributes.AVMID,
                        cyphyParentId = components.Guid.ToString("D"),
                        name = component.Name
                    });
                }
            }
            bridgeClient.SendToMetaLinkBridge(manifestMessage);
        }

        private void SendCadAssemblyXml(string instanceId, string assemblyGuid, bool clear)
        {
            SendCadAssemblyXml(instanceId, designIdToCadAssemblyXml[assemblyGuid], assemblyGuid, clear);
        }

        private void SendCadAssemblyXml(string instanceId, string xml, string assemblyGuid, bool clear)
        {
            MetaLinkProtobuf.Edit message = new MetaLinkProtobuf.Edit
            {
                editMode = Edit.EditMode.POST,
            };
            message.topic.Add(instanceId);
            if (clear)
            {
                message.actions.Add(new edu.vanderbilt.isis.meta.Action()
                {
                    actionMode = MetaLinkProtobuf.Action.ActionMode.CLEAR
                });
            }
            var action = new MetaLinkProtobuf.Action();
            message.actions.Add(action);
            action.actionMode = MetaLinkProtobuf.Action.ActionMode.CREATE_CYPHY_DESIGN;
            MetaLinkProtobuf.Alien alien = new MetaLinkProtobuf.Alien();
            action.alien = alien;
            alien.encoded = Encoding.UTF8.GetBytes(xml);
            alien.encodingMode = Alien.EncodingMode.XML;

            CyPhyML.ComponentAssembly targetAssembly = CyphyMetaLinkUtils.GetComponentAssemblyByGuid(addon.Project, assemblyGuid);
            if (targetAssembly != null)
            {
                var filter = targetAssembly.Impl.Project.CreateFilter();
                filter.Kind = "ComponentRef";
                try
                {
                    var env = new MetaLinkProtobuf.Environment
                    {
                        name = SearchPathStr,
                    };

                    foreach (var component in CyphyMetaLinkUtils.CollectComponentsRecursive(targetAssembly))
                    {
                        if (component is CyPhyML.Component)
                        {
                            AddSearchPathToEnvironment(component as CyPhyML.Component, env);
                        }
                    }

                    if (env.value.Count > 0)
                    {
                        action.environment.Add(env);
                    }
                }
                catch (IOException)
                {
                    // XXX
                }
            }

            bridgeClient.SendToMetaLinkBridge(message);
        }


        #region PROTOBUF
#if false
        protected MetaLinkProtobuf.Edit ModifyCADParameterProtoBufMsg(string in_ComponentInstanceID,
                                                                   CADParameter in_CADParameter)
        {
            MetaLinkProtobuf.ParametricParametersType ParametricParameters = new MetaLinkProtobuf.ParametricParametersType()
            {
                ComponentID = in_ComponentInstanceID
            };



            MetaLinkProtobuf.CADParameterType CADParameter_msg = new MetaLinkProtobuf.CADParameterType
            {
                Name = in_CADParameter.Name,
                Type = in_CADParameter.Type,
                Value = in_CADParameter.Value
            };

            ParametricParameters.CADParameter.Add(CADParameter_msg);


            MetaLinkProtobuf.Payload Payload_msg = new MetaLinkProtobuf.Payload();
            Payload_msg.parameters.Add(ParametricParameters);

            MetaLinkProtobuf.Action Action_msg = new MetaLinkProtobuf.Action
            {
                actionMode = MetaLinkProtobuf.Action.ActionMode.REPLACE,
                payload = Payload_msg
            };

            MetaLinkProtobuf.Edit Edit_msg = new MetaLinkProtobuf.Edit
            {
                editMode = MetaLinkProtobuf.Edit.EditMode.POST,
                guid = Guid.NewGuid().ToString(),
                sequence = 0,
            };
            Edit_msg.origin.Add(GMEOrigin);

            Edit_msg.actions.Add(Action_msg);
            Edit_msg.topic.Add(CadAssemblyTopic);
            Edit_msg.topic.Add(AssemblyID);

            return Edit_msg;
        }
#endif
        public MetaLinkProtobuf.Edit UpdateComponentNameProtoBufMsg(CyPhyML.ComponentRef componentRef, string newName)
        {
            MetaLinkProtobuf.CADComponentType Component_msg = new MetaLinkProtobuf.CADComponentType
            {
                ComponentID = componentRef.Attributes.InstanceGUID,
                DisplayName = componentRef.Name
            };

            MetaLinkProtobuf.Payload Payload_msg = new MetaLinkProtobuf.Payload();
            Payload_msg.components.Add(Component_msg);

            MetaLinkProtobuf.Action Action_msg = new MetaLinkProtobuf.Action
            {
                actionMode = MetaLinkProtobuf.Action.ActionMode.UPDATE_CYPHY_DESIGN,
                payload = Payload_msg
            };

            MetaLinkProtobuf.Edit Edit_msg = new MetaLinkProtobuf.Edit
            {
                editMode = MetaLinkProtobuf.Edit.EditMode.POST,
                guid = Guid.NewGuid().ToString(),
                sequence = 0,
            };

            Edit_msg.origin.Add(GMEOrigin);

            Edit_msg.actions.Add(Action_msg);

            SyncedComponentData cdata = syncedComponents[componentRef.ParentContainer.Guid.ToString()];
            Edit_msg.topic.Add(cdata.InstanceId);

            return Edit_msg;
        }

        public MetaLinkProtobuf.Edit AddComponentProtoBufMsg(CyPhyML.ComponentRef componentRef)
        {
            CyPhyML.Component component = componentRef.Referred.Component;
            CyPhyML.CADModel creocadmodel = null;
            string creomodelname = "";
            string creomodeltype = "";
#if DEBUG
            GMEConsole.Info.WriteLine("AVM: " + component.Attributes.AVMID + " InstanceGUID: " + component.Attributes.InstanceGUID + " Rev: " + component.Attributes.Revision + " Ver: " + component.Attributes.Version);
#endif

            creocadmodel = CyphyMetaLinkUtils.FindCADModelObject(component);

            if (creocadmodel == null)
            {
                GMEConsole.Warning.WriteLine("[" + component.Name + "] is missing a Creo CADModel object!");
                return null;
            }

            // [1]
            List<CyPhyML.Resource> Resource_L = new List<CyPhyML.Resource>();
            foreach (var item in creocadmodel.DstConnections.UsesResourceCollection)
                Resource_L.Add(item.DstEnds.Resource);
            foreach (var item in creocadmodel.SrcConnections.UsesResourceCollection)
                Resource_L.Add(item.SrcEnds.Resource);

            if (Resource_L.Count > 0)
            {
                creomodelname = Resource_L.First().Attributes.Path;
            }
            else
            {
                creomodelname = "";
#if DEBUG
                GMEConsole.Warning.WriteLine("[" + component.Name + "] is not connected to a Resource, therefore does not have a Creo model name.");
#endif
            }

            if (creomodelname != "")
            {
                creomodelname = Path.GetFileNameWithoutExtension(creomodelname);
            }


            // [2]
            creomodeltype = creocadmodel.Attributes.FileType.ToString().ToUpper();

            MetaLinkProtobuf.CADComponentType Component_msg = new MetaLinkProtobuf.CADComponentType
            {
                ComponentID = componentRef.Attributes.InstanceGUID,
                AvmComponentID = component.Attributes.AVMID,
                Type = creomodeltype,
                Name = creomodelname,
                ParametricParameters = new MetaLinkProtobuf.ParametricParametersType(),
                DisplayName = componentRef.Name
            };

            // [4] 
            InstanceIDToConstraint_Table[componentRef.Attributes.InstanceGUID] = 0;

            // [5]
            foreach (var param in creocadmodel.Children.CADParameterCollection)
            {
                MetaLinkProtobuf.CADParameterType CADParameter_msg = new MetaLinkProtobuf.CADParameterType
                {
                    Name = (param.Attributes.ParameterName == "") ? param.Name : param.Attributes.ParameterName,
                    Type = param.Attributes.CADParameterType.ToString(),
                    Value = (param.Attributes.Value == "") ? (param.Attributes.DefaultValue == "") ? "0" : param.Attributes.DefaultValue : param.Attributes.Value
                };

                Component_msg.ParametricParameters.CADParameter.Add(CADParameter_msg);
            }

            foreach (var connector in component.Children.ConnectorCollection)
            {
                Component_msg.Connectors.Add(new MetaLinkProtobuf.ConnectorType() { ID = connector.Guid.ToString(), DisplayName = connector.Name });
            }


            MetaLinkProtobuf.Payload Payload_msg = new MetaLinkProtobuf.Payload();
            Payload_msg.components.Add(Component_msg);

            MetaLinkProtobuf.Action Action_msg = new MetaLinkProtobuf.Action
            {
                actionMode = MetaLinkProtobuf.Action.ActionMode.ADD_COMPONENT_TO_CAD_DESIGN,
                payload = Payload_msg
            };

            MetaLinkProtobuf.Edit Edit_msg = new MetaLinkProtobuf.Edit
            {
                editMode = MetaLinkProtobuf.Edit.EditMode.POST,
                guid = Guid.NewGuid().ToString(),
                sequence = 0,
            };

            Edit_msg.origin.Add(GMEOrigin);

            Edit_msg.actions.Add(Action_msg);

            SyncedComponentData cdata = syncedComponents[componentRef.ParentContainer.Guid.ToString()];
            Edit_msg.topic.Add(cdata.InstanceId);

            return Edit_msg;
        }

#if false
        protected MetaLinkProtobuf.Edit CreateAddConstraintProtoBufMsg(string in_ComponentInstanceID,
                                                                    List<Tuple<CyPhy2CAD_CSharp.DataRep.Datum, CyPhy2CAD_CSharp.DataRep.Datum>> constraintPairs)
        {
            MetaLinkProtobuf.Payload Payload_msg = new MetaLinkProtobuf.Payload();

            MetaLinkProtobuf.ConstraintType Constraint_msg = new MetaLinkProtobuf.ConstraintType()
            {
                ComponentID = in_ComponentInstanceID
            };

            foreach (Tuple<CyPhy2CAD_CSharp.DataRep.Datum, CyPhy2CAD_CSharp.DataRep.Datum> element in constraintPairs)
            {
                MetaLinkProtobuf.PairType PairType_msg = new MetaLinkProtobuf.PairType
                {
                    ConstraintFeatureA = new MetaLinkProtobuf.ConstraintFeatureType
                    {
                        ComponentID = element.Item1.ComponentID,
                        FeatureName = element.Item1.DatumName,
                        FeatureOrientationType = element.Item1.Orientation
                    },
                    ConstraintFeatureB = new MetaLinkProtobuf.ConstraintFeatureType
                    {
                        ComponentID = element.Item2.ComponentID,
                        FeatureName = element.Item2.DatumName,
                        FeatureOrientationType = element.Item2.Orientation
                    },
                    FeatureAlignmentType = element.Item1.Alignment,
                    FeatureGeometryType = element.Item1.Type.ToString().ToUpper(), // need to add namespace to DatumType
                    FeatureInterfaceType = "CAD_DATUM"
                };
                Constraint_msg.Pair.Add(PairType_msg);
            }

            Payload_msg.constraints.Add(Constraint_msg);

            MetaLinkProtobuf.Action Action_msg = new MetaLinkProtobuf.Action
            {
                actionMode = MetaLinkProtobuf.Action.ActionMode.REPLACE,
                payload = Payload_msg
            };

            MetaLinkProtobuf.Edit Edit_msg = new MetaLinkProtobuf.Edit
            {
                editMode = MetaLinkProtobuf.Edit.EditMode.POST,
                guid = Guid.NewGuid().ToString(),
                sequence = 0,
            };
            Edit_msg.origin.Add(GMEOrigin);

            Edit_msg.actions.Add(Action_msg);
            Edit_msg.topic.Add(CadAssemblyTopic);
            Edit_msg.topic.Add(AssemblyID);

            return Edit_msg;
        }
#endif
        #endregion
    }   // end class
}   // end namespace
