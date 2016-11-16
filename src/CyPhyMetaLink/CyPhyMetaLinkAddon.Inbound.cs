using System;
using System.Collections.Generic;
using System.Collections;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Diagnostics;
using System.Collections.Concurrent;
using System.Threading;
using System.Xml.Serialization;

using GME.CSharp;
using GME.MGA;
using GME.MGA.Meta;
using GME;
using GME.MGA.Core;

using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyMLClasses = ISIS.GME.Dsml.CyPhyML.Classes;

using ProtoBuf;
using edu.vanderbilt.isis.meta;
using MetaLinkProtobuf = edu.vanderbilt.isis.meta;
using System.Security;
using Newtonsoft.Json;
using System.Reflection;
using System.Xml;
using System.Xml.XPath;
using META;

namespace CyPhyMetaLink
{
    /// <summary>
    /// This file contains functionality related to business logic handling incoming messages (from CAD)
    /// </summary>
    [Guid(ComponentConfig_Addon.guid),
    ProgId(ComponentConfig_Addon.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public partial class CyPhyMetaLinkAddon : IMgaComponentEx, IGMEVersionInfo, IMgaEventSink
    {
        // String format for message topics
        public static readonly String SearchPathStr = "SearchPath";

        private readonly String CadAssemblyOrigin = "CAD";
        private readonly String GMEOrigin = "GME";

        private readonly CreoStartupDialog StartupDialog = new CreoStartupDialog();
        private Dictionary<string, int> interests = new Dictionary<string, int>();
        private Dictionary<string, int> InstanceIDToConstraint_Table = new Dictionary<string, int>();

        private readonly HashSet<string> datumKinds = new HashSet<string>()
            {
                "Datum", // abstract
                "Axis",
                "CoordinateSystem",
                "Point",
                "Surface"
            };

        private readonly Dictionary<string, string> datumKindsMap = new Dictionary<string, string>()
        {
            { "Datum", "DATUM" },
            { "Axis", "AXIS" },
            { "CoordinateSystem", "CSYS" },
            { "Point", "POINT" },
            { "Surface", "SURFACE" },
        };

        #region Process Incoming Messages
        /// <summary>
        /// The main dispatch handler for incoming messages
        /// </summary>
        /// <param name="message"></param>
        private void ProcessEditMessage(MetaLinkProtobuf.Edit message)
        {
            if (GMEConsole == null)
            {
                GMEConsole = GMEConsole.CreateFromProject(addon.Project);
            }
            // This is the first message from Creo, hide the CAD Startup Dialog
            if (StartupDialog.Visible && message.origin.Contains(CadAssemblyOrigin) &&
                message.editMode == Edit.EditMode.INTEREST && message.topic.Count == 1 &&
                message.topic[0] == LastStartedInstance.InstanceId)
            {
                ShowStartupDialog(false);
            }
            // Drop a message to the console with the acknowledgement of a previous message
            if (message.actions.Count > 0)
            {
                foreach (var action in message.actions)
                {
                    foreach (var notice in action.notices)
                    {
                        if (notice.noticeMode == Notice.NoticeMode.DONE)
                        {
                            GMEConsole.Info.WriteLine("Done response from Meta-Link: " + notice.msg);
                        }
                        else if (notice.noticeMode == Notice.NoticeMode.ACK)
                        {
                            GMEConsole.Info.WriteLine("Ack response from Meta-Link: " + notice.msg);
                        }
                        else if (notice.noticeMode == Notice.NoticeMode.WARN)
                        {
                            GMEConsole.Warning.WriteLine("Warn response from Meta-Link: " + notice.msg);
                        }
                    }
                }
            }
            if (!TestMode && message.mode.SequenceEqual(new Edit.EditMode[] { Edit.EditMode.INTEREST, MetaLinkProtobuf.Edit.EditMode.NOTICE }))
            {
                // Start Creo in design editing mode
                if (message.notices.Count == 1 && message.notices[0].noticeMode == Notice.NoticeMode.ACK)
                {
                    SyncedComponentData cdata = GetSyncedCompDataByInstanceId(message.topic[0]);
                    if (cdata == null)
                    {
                        GMEConsole.Error.WriteLine("Fatal error: can't find component data for synced component: " + message.topic[0]);
                    }
                    else if (cdata.Type == SyncedComponentData.EditType.Design)
                    {
                        // message.topic[1] is the assembly GUID
                        string assemblyGuid = Guid.Parse(cdata.Id).ToString();

                        string exeparams = "";
                        addon.Project.BeginTransactionInNewTerr();
                        try
                        {
                            exeparams = GetStartupParams(assemblyGuid);
                        }
                        finally
                        {
                            addon.Project.AbortTransaction();
                        }

                        StartAssemblyExe(CreoOpenMode.OPEN_COMPONENT, assemblyGuid, true, exeparams);
                    }
                    else
                    {
                        Action<MetaLinkProtobuf.Edit> action;
                        if (noticeActions.TryGetValue(message.guid, out action))
                        {
                            noticeActions.Remove(message.guid);
                            action(message);
                        }
                    }
                }
            }

            // Write message to console if it's an error response
            if (message.mode.Count > 0 && message.mode.Last() == MetaLinkProtobuf.Edit.EditMode.NOTICE)
            {
                foreach (var notice in message.actions.SelectMany(a => a.notices)
                    .Where(n => n.noticeMode == Notice.NoticeMode.FAULT || n.noticeMode == Notice.NoticeMode.FAIL))
                {
                    GMEConsole.Error.WriteLine("Meta-Link error: " + notice.msg);
                }
            }

            // ignore messages from ourselves
            if (message.origin.Contains(GMEOrigin))
            {
                return;
            }

            // DISINTEREST: The user closed Creo (component)
            if (message.mode.SequenceEqual(new Edit.EditMode[] { Edit.EditMode.DISINTEREST }))
            {
                SyncedComponentData cdata = GetSyncedCompDataByInstanceId(message.topic[0]);
                if (cdata.Type == SyncedComponentData.EditType.Component)
                {
                    String componentAVMID = cdata.Id;
                    //componentEditMessages.Remove(componentAVMID);
                    syncedComponents.Remove(componentAVMID);
                    SendDisinterest(false, cdata.InstanceId);
                    addon.Project.BeginTransactionInNewTerr();
                    try
                    {
                        var component = CyphyMetaLinkUtils.GetComponentByAvmId(addon.Project, componentAVMID);
                        if (component != null)
                        {
                            HighlightInTree(component, 0);
                        }
                    }
                    finally
                    {
                        addon.Project.AbortTransaction();
                    }
                }
                else if (cdata.Type == SyncedComponentData.EditType.Empty)
                {
                    syncedComponents.Remove(""); // CAD has been started in an empty mode
                }

                else
                {
                    Guid componentAssemblyID = Guid.Parse(cdata.Id);

                    syncedComponents.Remove(componentAssemblyID.ToString());
                    SendDisinterest(true, cdata.InstanceId);
                    addon.Project.BeginTransactionInNewTerr();
                    try
                    {
                        CyPhyML.ComponentAssembly assembly = CyphyMetaLinkUtils.GetComponentAssemblyByGuid(addon.Project, AssemblyID);
                        if (assembly != null)
                        {
                            HighlightInTree(assembly, 0);
                        }
                    }
                    finally
                    {
                        addon.Project.AbortTransaction();
                    }
                    AssemblyID = null;
                }
            }
            else
                // Create analysis point
                if (message.mode.SequenceEqual(new Edit.EditMode[] { Edit.EditMode.POST }) &&
                    message.actions.Count >= 1 && message.actions[0].actionMode == MetaLinkProtobuf.Action.ActionMode.CREATE_ANALYSIS_POINTS)
            {
                foreach (var action in message.actions)
                {
                    ProcessAnalysisPointMessage(action.subjectID, action);
                }
            }
            else
                // Design edit message (insert/select/discard) 
                if (message.mode.SequenceEqual(new Edit.EditMode[] { Edit.EditMode.POST }) && message.actions.Count >= 1 &&
                    (message.actions[0].actionMode == MetaLinkProtobuf.Action.ActionMode.ADD_COMPONENT_TO_CYPHY_DESIGN ||
                    message.actions[0].actionMode == MetaLinkProtobuf.Action.ActionMode.SELECT_CYPHY_COMPONENT ||
                    message.actions[0].actionMode == MetaLinkProtobuf.Action.ActionMode.REMOVE_CYPHY_DESIGN_COMPONENT))
            {
                foreach (var action in message.actions)
                {
                    if (action.actionMode == MetaLinkProtobuf.Action.ActionMode.ADD_COMPONENT_TO_CYPHY_DESIGN && action.payload != null)
                    {
                        // Add an AVM component
                        ProcessEditInsert(action.subjectID, action);
                    }
                    else if (action.actionMode == MetaLinkProtobuf.Action.ActionMode.SELECT_CYPHY_COMPONENT && action.payload != null)
                    {
                        // Select a component
                        ProcessEditSelect(action.subjectID, action);
                    }
                    else if (action.actionMode == MetaLinkProtobuf.Action.ActionMode.REMOVE_CYPHY_DESIGN_COMPONENT)
                    {
                        // Remove a component
                        ProcessEditDiscard(action.subjectID, action);
                    }
                    else
                    {
                        GMEConsole.Warning.WriteLine("Unknown action: " + message.ToString());
                    }
                }
            }
            else
                // Re-sync
                if (message.mode.SequenceEqual(new Edit.EditMode[] { Edit.EditMode.POST }) &&
                    message.actions.Count >= 1 && message.actions[0].actionMode == MetaLinkProtobuf.Action.ActionMode.RESYNC)
            {
                ProcessResync(message.actions[0].subjectID);
            }
            else
                // Send assembly/component info to the client
                if (message.mode.SequenceEqual(new Edit.EditMode[] { Edit.EditMode.INTEREST }))
            {
                SyncedComponentData cdata = GetSyncedCompDataByInstanceId(message.topic[0]);
                if (cdata.Type == SyncedComponentData.EditType.Component)
                {
                    addon.Project.BeginTransactionInNewTerr();
                    try
                    {
                        CyPhyML.Component avmcomp = CyphyMetaLinkUtils.FindAVMComponent(addon.Project, cdata.Id, null);

                        if (avmcomp == null)
                        {
                            throw new Exception(String.Format("Can't find AVM component to open with id {0}.", cdata.Id));
                        }

                        var outmessage = CreateComponentEditMessage(cdata.InstanceId, avmcomp, CyphyMetaLinkUtils.FindCADModelObject(avmcomp));

                        if (outmessage != null)
                        {
                            //SendInterest(null, ResyncTopic, message.topic[1]);
                            bridgeClient.SendToMetaLinkBridge(outmessage);
                        }
                        else
                        {
                            GMEConsole.Warning.WriteLine("MetaLink: don't know about component " + cdata.Id);
                        }
                        // This is a recently created component
                        if (!syncedComponents.ContainsKey(avmcomp.Attributes.AVMID))
                        {
                            SyncedComponentData syncedCompData = new SyncedComponentData()
                            {
                                Type = SyncedComponentData.EditType.Component,
                                Id = avmcomp.Attributes.AVMID
                            };
                            syncedComponents.Add(avmcomp.Attributes.AVMID, syncedCompData);
                            HighlightInTree(avmcomp, 1);
                        }
                    }
                    finally
                    {
                        addon.Project.AbortTransaction();
                    }
                }
                else
                {
                    string designId = cdata.Id;
                    string xml;
                    if (designIdToCadAssemblyXml.TryGetValue(designId, out xml))
                    {
                        addon.Project.BeginTransactionInNewTerr();
                        try
                        {
                            SendCadAssemblyXml(message.topic[0], xml, designId, false);
                            SaveCadAssemblyXml(xml, designId);
                        }
                        finally
                        {
                            addon.Project.AbortTransaction();
                        }
                    }
                    else
                    {
                        GMEConsole.Warning.WriteLine("MetaLink: unknown assembly " + designId);
                    }
                }
            }
            else
                // Request component list message from Creo
                if (message.mode.SequenceEqual(new Edit.EditMode[] { Edit.EditMode.POST }) &&
                    message.actions.Count >= 1 && message.actions[0].actionMode == MetaLinkProtobuf.Action.ActionMode.REQUEST_COMPONENT_LIST)
            {
                addon.Project.BeginTransactionInNewTerr();
                try
                {
                    SendComponentList(message.topic[0]);
                }
                finally
                {
                    addon.Project.AbortTransaction();
                }
            }
            else
                // Update AVM Component with the information from Creo
                if (message.mode.SequenceEqual(new Edit.EditMode[] { Edit.EditMode.POST }) &&
                    message.actions.Count == 1 && message.actions[0].actionMode == MetaLinkProtobuf.Action.ActionMode.UPDATE_CYPHY_COMPONENT &&
                    message.actions[0].alien != null && message.actions[0].alien.encodingMode == Alien.EncodingMode.XML)
            {
                string componentId = message.actions[0].subjectID;
                string component_xml = Encoding.UTF8.GetString(message.actions[0].alien.encoded);
                ProcessAVMComponentUpdate(componentId, component_xml);
            }
            else
                // Create a connector in component
                if (message.mode.SequenceEqual(new Edit.EditMode[] { Edit.EditMode.POST }) &&
                    message.actions.Count >= 1 && message.actions[0].actionMode == MetaLinkProtobuf.Action.ActionMode.ADD_CONNECTOR_TO_COMPONENT)
            {
                string componentId = message.actions[0].subjectID;
                ProcessInsertComponentConnector(componentId, message.actions[0]);
            }
            else
                // Create new CyPhy component
                if (message.mode.SequenceEqual(new Edit.EditMode[] { Edit.EditMode.POST }) &&
                    message.actions.Count == 1 && message.actions[0].actionMode == MetaLinkProtobuf.Action.ActionMode.CREATE_CYPHY_COMPONENT &&
                    message.actions[0].alien != null && message.actions[0].alien.encodingMode == Alien.EncodingMode.XML)
            {
                string component_xml = Encoding.UTF8.GetString(message.actions[0].alien.encoded);
                ProcessAVMComponentCreate(component_xml, message.topic[0]);
            }
            else
                // Connect component within a design
                if (message.mode.SequenceEqual(new Edit.EditMode[] { Edit.EditMode.POST }) &&
                    message.actions.Count >= 1 && message.actions[0].actionMode == MetaLinkProtobuf.Action.ActionMode.CONNECT_COMPONENTS_CYPHY)
            {
                try
                {
                    string assemblyID = message.actions[0].payload.components[0].ComponentID;
                    string conn1 = message.actions[0].payload.components[1].ComponentID.Substring(0, message.actions[0].payload.components[1].ComponentID.IndexOf('_'));
                    string ref1 = message.actions[0].payload.components[1].ComponentID.Substring(message.actions[0].payload.components[1].ComponentID.IndexOf('_') + 1);
                    string conn2 = message.actions[0].payload.components[2].ComponentID.Substring(0, message.actions[0].payload.components[2].ComponentID.IndexOf('_'));
                    string ref2 = message.actions[0].payload.components[2].ComponentID.Substring(message.actions[0].payload.components[2].ComponentID.IndexOf('_') + 1);
                    ProcessConnect(assemblyID, conn1, ref1, conn2, ref2);
                }
                catch (Exception ex)
                {
                    GMEConsole.Warning.WriteLine("Error during processing connect message: " + ex.Message);
                }
            }
            else
            {
                GMEConsole.Warning.WriteLine("Message left unprocessed: " + message.ToString());
            }
        }

        /// <summary>
        /// Create analysis point(s)
        /// </summary>
        /// <param name="avmid"></param>
        /// <param name="action"></param>
        private void ProcessAnalysisPointMessage(string avmid, MetaLinkProtobuf.Action action)
        {
            addon.Project.BeginTransactionInNewTerr();
            CyPhyML.Component component = CyphyMetaLinkUtils.GetComponentByAvmId(addon.Project, avmid);
            if (component == null)
            {
                GMEConsole.Error.WriteLine("Create connector: can't find component with AVMID " + avmid);
                addon.Project.AbortTransaction();
            }
            else
            {
                try
                {
                    CyPhyML.CADModel cadmodel = CyphyMetaLinkUtils.FindCADModelObject(component);
                    if (cadmodel == null)
                    {
                        GMEConsole.Error.WriteLine("Can't find CADModel in component: " + component.ToHyperLink());
                        addon.Project.AbortTransaction();
                        return;
                    }
                    foreach (var datum in action.payload.datums)
                    {
                        CyPhyML.Point point = CyPhyMLClasses.Point.Create(component);
                        point.Name = datum.name;
                        point.Attributes.ID = datum.name;
                        var connectdatum = cadmodel.Children.PointCollection.Where(p => p.ID == datum.name);
                        CyPhyML.Point datumPoint = null;
                        if (!connectdatum.Any())
                        {
                            datumPoint = CyPhyMLClasses.Point.Create(cadmodel);
                            datumPoint.Name = datumPoint.Attributes.DatumName = datum.name;
                        }
                        else
                        {
                            datumPoint = connectdatum.First();
                        }
                        CyPhyMLClasses.PortComposition.Connect(point, datumPoint, parent: component);
                    }
                }
                catch (Exception ex)
                {
                    GMEConsole.Error.WriteLine("Error during creating analysis points: " + ex.Message);
                    addon.Project.AbortTransaction();
                    return;
                }
                addon.Project.CommitTransaction();
            }
        }

        /// <summary>
        /// Resync: re-send the whole assembly information again
        /// </summary>
        /// <param name="id"></param>
        private void ProcessResync(string id)
        {
            try
            {
                addon.Project.BeginTransactionInNewTerr();
                CyPhyML.ComponentAssembly assembly = CyphyMetaLinkUtils.GetComponentAssemblyByGuid(addon.Project, id);
                if (assembly == null)
                {
                    CyPhyML.Component comp = CyphyMetaLinkUtils.GetComponentByAvmId(addon.Project, id);
                    if (comp == null)
                    {
                        GMEConsole.Error.WriteLine("Resync: Can't find design/component for id: " + id);
                        addon.Project.AbortTransaction();
                        return;
                    }
                    else
                    {
                        var cadModel = CyphyMetaLinkUtils.FindCADModelObject(comp);
                        //var message = CreateComponentEditMessage(comp, cadModel);
                        //bridgeClient.SendToMetaLinkBridge(message);
                    }
                }
                RestartAssemblySyncAtEndOfTransaction(assembly);
                addon.Project.AbortTransaction();
            }
            catch (Exception ex)
            {
                GMEConsole.Error.WriteLine("Resync failed with exception: " + ex.Message);
                addon.Project.AbortTransaction();
            }
        }

        /// <summary>
        /// Create a new or update an existing connector
        /// </summary>
        /// <param name="component"></param>
        /// <param name="datumList"></param>
        /// <param name="datumNames"></param>
        /// <param name="datumAligns"></param>
        /// <param name="connector"></param>
        /// <param name="connectorName"></param>
        private void CreateOrUpdateConnector(CyPhyML.Component component, List<CyPhyML.CADDatum> datumList, List<string> datumNames, List<ConnectorDatumType.AlignType> datumAligns, CyPhyML.Connector connector = null, String connectorName = null)
        {
            if (connector == null)
            {
                connector = CyPhyMLClasses.Connector.Create(component);
                if (connector == null)
                {
                    throw new Exception("Unable to create connector.");
                }
                connector.Name = connectorName;
            }
            int i = 0;
            foreach (var datum in datumList)
            {
                MgaFCO copy = ((MgaModel)connector.Impl).CopyFCODisp((MgaFCO)datum.Impl, ((MgaMetaModel)connector.Impl.MetaBase).GetRoleByNameDisp(datum.Kind));
                CyPhyML.CADDatum dscopy = CyPhyMLClasses.CADDatum.Cast(copy);
                dscopy.Name = datumNames[i];
                if (dscopy.Kind == "Surface")
                {
                    CyPhyMLClasses.Surface.Cast(dscopy.Impl).Attributes.Alignment = datumAligns[i] == ConnectorDatumType.AlignType.ALIGN ? CyPhyMLClasses.Surface.AttributesClass.Alignment_enum.ALIGN : CyPhyMLClasses.Surface.AttributesClass.Alignment_enum.MATE;
                }
                ((MgaModel)connector.Impl).MoveFCODisp((MgaFCO)dscopy.Impl, ((MgaMetaModel)connector.Impl.MetaBase).GetRoleByNameDisp(dscopy.Kind));
                CyPhyMLClasses.PortComposition.Connect(datum, dscopy, parent: (CyPhyML.DesignElement)null);
                i++;
            }
        }

        /// <summary>
        /// Add a new AVM component to an existing assembly
        /// </summary>
        /// <param name="avmid"></param>
        /// <param name="action"></param>
        private void ProcessInsertComponentConnector(string avmid, MetaLinkProtobuf.Action action)
        {
            try
            {
                addon.Project.BeginTransactionInNewTerr();
                CyPhyML.Component component = CyphyMetaLinkUtils.GetComponentByAvmId(addon.Project, avmid);
                if (component == null)
                {
                    GMEConsole.Error.WriteLine("Create connector: can't find component with AVMID " + avmid);
                    addon.Project.AbortTransaction();
                }
                else
                {
                    CyPhyML.CADModel cadModel = CyphyMetaLinkUtils.FindCADModelObject(component);
                    if (cadModel == null)
                    {
                        GMEConsole.Warning.WriteLine("Create connector: can't find cadmodel within component " + component.Name + " , skipping...");
                        addon.Project.AbortTransaction();
                    }
                    else
                    {
                        // Insert new connectors
                        foreach (ConnectorType actionConnector in action.payload.connectors)
                        {
                            CyPhyML.Connector connector = null;
                            if (!String.IsNullOrEmpty(actionConnector.ID))
                            {
                                connector = CyphyMetaLinkUtils.FindConnector(component, actionConnector.ID);
                                if (connector == null)
                                {
                                    GMEConsole.Warning.WriteLine("Create connector: can't find connector with id " + actionConnector.ID + " , skipping...");
                                    continue;
                                }
                            }
                            List<CyPhyML.CADDatum> datumList = new List<CyPhyML.CADDatum>();
                            List<string> datumNamesList = new List<string>();
                            List<ConnectorDatumType.AlignType> datumAlignList = new List<ConnectorDatumType.AlignType>();
                            bool success = true;
                            foreach (var datum in actionConnector.Datums)
                            {
                                CyPhyML.CADDatum cadDatum = CyphyMetaLinkUtils.FindDatum(cadModel, datum.ID);
                                if (cadDatum == null)
                                {
                                    GMEConsole.Warning.WriteLine("Create connector: can't find datum with name: " + datum.ID + " , skipping...");
                                    success = false;
                                    break;
                                }
                                datumList.Add(cadDatum);
                                datumNamesList.Add(datum.DisplayName);
                                datumAlignList.Add(datum.Alignment);
                            }
                            if (!success)
                            {
                                continue;
                            }

                            CreateOrUpdateConnector(component, datumList, datumNamesList, datumAlignList, connector, connector == null ? actionConnector.DisplayName : null);
                        }
                        addon.Project.CommitTransaction();
                    }
                }
            }
            catch (Exception ex)
            {
                addon.Project.AbortTransaction();
                GMEConsole.Error.WriteLine("AVM Component Insert, exception caught: " + ex.Message);
            }
        }

        /// <summary>
        /// Connect 2 components
        /// </summary>
        /// <param name="assemblyguid"></param>
        /// <param name="connectorguid1"></param>
        /// <param name="comprefguid1"></param>
        /// <param name="connectorguid2"></param>
        /// <param name="comprefguid2"></param>
        private void ProcessConnect(string assemblyguid, string connectorguid1, string comprefguid1, string connectorguid2, string comprefguid2)
        {
            addon.Project.BeginTransactionInNewTerr();
            CyPhyML.ComponentAssembly assembly = null;
            try
            {
                assembly = CyphyMetaLinkUtils.GetComponentAssemblyByGuid(addon.Project, AssemblyID);
                if (assembly == null)
                {
                    throw new Exception();
                }
                CyPhyML.ComponentRef ref1 = CyphyMetaLinkUtils.GetComponentRefInAssemblyById(assembly, comprefguid1);
                CyPhyML.Connector conn1 = CyphyMetaLinkUtils.FindConnector(ref1, connectorguid1);
                if (conn1 == null)
                {
                    GMEConsole.Error.WriteLine("ProcessConnect: Unable to find connector (guid {0}) for assembly {1}", connectorguid1, assembly.Name);
                    addon.Project.AbortTransaction();
                    return;
                }
                CyPhyML.ComponentRef ref2 = CyphyMetaLinkUtils.GetComponentRefInAssemblyById(assembly, comprefguid2);
                if (ref2 == null)
                {
                    GMEConsole.Error.WriteLine("ProcessConnect: Unable to find component ref (guid {0}) for assembly {1}", comprefguid2, assembly.Name);
                    addon.Project.AbortTransaction();
                    return;
                }
                CyPhyML.Connector conn2 = CyphyMetaLinkUtils.FindConnector(ref2, connectorguid2);
                if (conn2 == null)
                {
                    GMEConsole.Error.WriteLine("ProcessConnect: Unable to find connector (guid {0}) for assembly {1}", connectorguid2, assembly.Name);
                    addon.Project.AbortTransaction();
                    return;
                }
                try
                {
                    // FIXME this will fail if ref1 and ref2 are across hierarchies
                    CyPhyMLClasses.ConnectorComposition.Connect(conn1, conn2, ref1, ref2, parent: (CyPhyML.DesignElement)null);
                }

                catch (Exception ex)
                {
                    GMEConsole.Warning.WriteLine("Unable to connect connector {0} <{1}> in {2} <{3}> to {4} <{5}> in {6} <{7}>, error is: {8}", conn1.Name, conn1.Guid, ref1.Name, ref1.Guid, conn2.Name, conn2.Guid, ref2.Name, ref2.Guid, ex.Message);
                    addon.Project.AbortTransaction();
                    return;
                }
            }
            catch (Exception ex)
            {
                GMEConsole.Error.WriteLine("Error during processing connect message " + ex.Message);
                addon.Project.AbortTransaction();
                return;
            }
            addon.Project.CommitTransaction();
        }

        /// <summary>
        /// Remove a component from a design
        /// </summary>
        /// <param name="assemblyId"></param>
        /// <param name="action"></param>
        private void ProcessEditDiscard(string assemblyId, MetaLinkProtobuf.Action action)
        {
            try
            {
                handleEvents = false;
                addon.Project.BeginTransactionInNewTerr(transactiontype_enum.TRANSACTION_NON_NESTED);
                CyPhyML.ComponentAssembly assembly = CyphyMetaLinkUtils.GetComponentAssemblyByGuid(addon.Project, assemblyId);
                foreach (var component in action.payload.components)
                {
                    CyPhyML.ComponentRef cref = CyphyMetaLinkUtils.GetComponentRefInAssemblyById(assembly, component.ComponentID);
                    if (cref == null)
                    {
                        GMEConsole.Warning.WriteLine("Remove component: Can't find component ref with id: " + component.ComponentID);
                    }
                    else
                    {
                        cref.Delete();
                    }
                }
                RestartAssemblySyncAtEndOfTransaction(assembly);
                addon.Project.CommitTransaction();
                handleEvents = true;
            }
            catch (Exception ex)
            {
                GMEConsole.Warning.WriteLine("Remove component: An exception happened while processing the request: " + ex.Message);
                addon.Project.AbortTransaction();
                handleEvents = true;
            }
        }

        /// <summary>
        /// Select a component within a design
        /// </summary>
        /// <param name="assemblyId"></param>
        /// <param name="action"></param>
        private void ProcessEditSelect(string assemblyId, MetaLinkProtobuf.Action action)
        {
            CyPhyML.ComponentRef compRef = null;
            if (action.payload.components.Count > 0 && String.IsNullOrEmpty(action.payload.components[0].ComponentID) == false)
            {
                addon.Project.BeginTransactionInNewTerr(transactiontype_enum.TRANSACTION_NON_NESTED);
                try
                {
                    CyPhyML.ComponentAssembly assembly = CyphyMetaLinkUtils.GetComponentAssemblyByGuid(addon.Project, assemblyId);
                    if (assembly != null)
                    {
                        compRef = CyphyMetaLinkUtils.GetComponentRefInAssemblyById(assembly, action.payload.components[0].ComponentID);
                    }
                }
                finally
                {
                    addon.Project.CommitTransaction();
                }
            }
            if (compRef != null)
            {
                GMEConsole.gme.ShowFCO((MgaFCO)compRef.Impl, true);
            }
        }

        /// <summary>
        /// Add AVM component to design
        /// </summary>
        /// <param name="componentAssemblyGuid"></param>
        /// <param name="action"></param>
        private void ProcessEditInsert(string componentAssemblyGuid, MetaLinkProtobuf.Action action)
        {
            addon.Project.BeginTransactionInNewTerr();
            try
            {
                CyPhyML.ComponentAssembly topicAssembly = CyphyMetaLinkUtils.GetComponentAssemblyByGuid(addon.Project, componentAssemblyGuid);
                foreach (var messageComponent in action.payload.components)
                {
                    CyPhyML.Component referencedComponent = CyphyMetaLinkUtils.FindAVMComponent(addon.Project, messageComponent.AvmComponentID, null);
                    if (referencedComponent == null)
                    {
                        GMEConsole.Warning.WriteLine("Add component: can't find AVM component with id: " + messageComponent.AvmComponentID);
                    }
                    else if (topicAssembly != null) // TODO: log unknown component
                    {
                        //MgaFCO newcompfco = ((MgaModel)assembly.Impl).DeriveChildObject((MgaFCO)referencedComponent.Impl, ((MgaMetaModel)assembly.Impl.MetaBase).RoleByName["Component"], true);
                        // MgaFCO newcompfco = ((MgaModel)assembly.Impl).CopyFCODisp((MgaFCO)referencedComponent.Impl, ((MgaMetaModel)assembly.Impl.MetaBase).RoleByName["Component"]);
                        var newcomp = CyPhyMLClasses.ComponentRef.Create(topicAssembly);
                        newcomp.Referred.Component = referencedComponent;
                        newcomp.Attributes.InstanceGUID = newcomp.Guid.ToString("D");
                        if (String.IsNullOrEmpty(messageComponent.Name))
                        {
                            newcomp.Name = referencedComponent.Name;
                        }
                        else
                        {
                            newcomp.Name = messageComponent.Name;
                        }
                    }
                    else
                    {
                        GMEConsole.Warning.WriteLine("Add component: can't find design assembly with id: " + componentAssemblyGuid);
                    }
                }
                addon.Project.CommitTransaction();
            }
            catch (Exception e)
            {
                addon.Project.AbortTransaction();
                GMEConsole.Out.WriteLine("Add component: Exception during inserting new component: {0}", System.Security.SecurityElement.Escape(e.ToString()));
            }
        }

        /// <summary>
        /// Update an existing component
        /// </summary>
        /// <param name="componentId"></param>
        /// <param name="component_xml"></param>
        private void ProcessAVMComponentUpdate(string componentId, string component_xml)
        {
            try
            {
                File.WriteAllText(Path.Combine(Path.GetTempPath(), "CyPhyMLPropagate.log"), component_xml);
            }
            catch (IOException)
            {
            }
            addon.Project.BeginTransactionInNewTerr();
            try
            {
                CyPhyML.Component cyPhyComponent = CyphyMetaLinkUtils.GetComponentByAvmId(addon.Project, componentId);

                if (!cyPhyComponent.Children.CADModelCollection.Any())
                {
                    throw new Exception(String.Format("No CADModel found inside component under update: {1}" + cyPhyComponent.ToHyperLink()));
                }

                string absModelPath = GetCadModelPath(cyPhyComponent);
                string absModelDir = null;
                if (absModelPath != null)
                {
                    absModelDir = Path.GetDirectoryName(GetCadModelPath(cyPhyComponent));
                }
                else
                {
                    absModelDir = Path.Combine(GetProjectDir(), cyPhyComponent.GetDirectoryPath(), "CAD");
                }

                if (!Directory.Exists(absModelDir))
                {
                    Directory.CreateDirectory(absModelDir);
                }

                XmlDocument doc = new XmlDocument();
                doc.LoadXml(component_xml);

                XmlNamespaceManager manager = new XmlNamespaceManager(doc.NameTable);
                manager.AddNamespace("avm", "avm");
                manager.AddNamespace("cad", "cad");
                manager.AddNamespace("modelica", "modelica");
                manager.AddNamespace("cyber", "cyber");
                manager.AddNamespace("manufacturing", "manufacturing");

                List<string> notcopied = new List<string>();

                //string cadmodelpath = Path.GetDirectoryName(GetCadModelPath(cyPhyComponent));

                CopyResources(absModelDir, doc, manager, notcopied, true);

                avm.Component component = CyPhyComponentImporter.CyPhyComponentImporterInterpreter.DeserializeAvmComponentXml(new StringReader(doc.OuterXml));

                avm.Component oldComponent = CyPhy2ComponentModel.Convert.CyPhyML2AVMComponent(cyPhyComponent);

                // find replace corresponding DomainModels in oldComponent, and find the corresponding CyPhy CADModels
                Dictionary<avm.DomainModel, CyPhyML.CADModel> avmDomainModelToCyPhyCADModel = new Dictionary<avm.DomainModel, CyPhyML.CADModel>();
                var cadmodel = cyPhyComponent.Children.CADModelCollection.Where(cadModel => cadModel.Attributes.FileFormat == CyPhyMLClasses.CADModel.AttributesClass.FileFormat_enum.Creo).First();
                // FIXME possible out-of-bounds access
                avmDomainModelToCyPhyCADModel.Add(component.DomainModel[0], cadmodel);


                foreach (var models in avmDomainModelToCyPhyCADModel)
                {
                    var builder = new AVM2CyPhyML.CyPhyMLComponentBuilder(CyPhyMLClasses.RootFolder.GetRootFolder(models.Value.Impl.Project));
                    var newCADModel = builder.process((avm.cad.CADModel)models.Key, cyPhyComponent);
                    newCADModel.Attributes.FileType = models.Value.Attributes.FileType; // META-1680 this is not set by the Component Importer. Assume it hasn't changed
                    // wire new Datums to CyPhy model, delete old CADModel
                    Func<MgaFCO, string> mapping = x =>
                    {
                        if (datumKinds.Contains(x.MetaBase.Name))
                        {
                            return CyPhyMLClasses.CADDatum.Cast(x).Attributes.DatumName.ToUpper() + "__ __" + x.Meta.Name;
                        }
                        return x.Name.ToUpper() + "__ __" + x.Meta.Name;
                    };
                    Dictionary<string, MgaFCO> nameMap = ((MgaModel)newCADModel.Impl).ChildFCOs.Cast<MgaFCO>().ToDictionary(mapping, fco => fco);

                    foreach (var connPoint in models.Value.Impl.ChildObjects.Cast<MgaFCO>()
                        .SelectMany(child => child.PartOfConns.Cast<MgaConnPoint>().Where(cp => cp.Owner.ParentModel.ID == cyPhyComponent.ID)))
                    {
                        MgaFCO mappedFCO;
                        if (nameMap.TryGetValue(mapping.Invoke(connPoint.Target), out mappedFCO))
                        {
                            if (connPoint.ConnRole == "dst")
                            {
                                ((MgaSimpleConnection)connPoint.Owner).SetDst(connPoint.References, mappedFCO);
                            }
                            else
                            {
                                ((MgaSimpleConnection)connPoint.Owner).SetSrc(connPoint.References, mappedFCO);
                            }
                        }
                        else
                        {
                            GMEConsole.Warning.WriteLine("Could not find connection target " + connPoint.Target.Meta.Name + " '" + connPoint.Target.Name + "'. Existing connection will be removed from model.");
                            connPoint.Owner.DestroyObject();
                        }
                    }
                    foreach (MgaConnPoint connPoint in ((MgaFCO)models.Value.Impl).PartOfConns)
                    {
                        if (connPoint.ConnRole == "dst")
                        {
                            ((MgaSimpleConnection)connPoint.Owner).SetDst(connPoint.References, (MgaFCO)newCADModel.Impl);
                        }
                        else
                        {
                            ((MgaSimpleConnection)connPoint.Owner).SetSrc(connPoint.References, (MgaFCO)newCADModel.Impl);
                        }
                    }
                    foreach (MgaPart part in ((MgaFCO)newCADModel.Impl).Parts)
                    {
                        string icon;
                        int x, y;
                        ((MgaFCO)models.Value.Impl).PartByMetaPart[part.Meta].GetGmeAttrs(out icon, out x, out y);
                        part.SetGmeAttrs(icon, x, y);
                    }

                    List<string> existing = new List<string>();

                    // Refresh resources
                    List<CyPhyML.Resource> removeList = new List<CyPhyML.Resource>();
                    foreach (CyPhyML.Resource res in cyPhyComponent.Children.ResourceCollection)
                    {
                        if (CyphyMetaLinkUtils.IsCADResource(res))
                        {
                            if (!notcopied.Where(s => res.Attributes.Path.ToLower().EndsWith(s.ToLower())).Any())
                            {
                                removeList.Add(res);
                            }
                            else
                            {
                                existing.Add(res.Attributes.Path);
                            }
                        }
                    }
                    foreach (CyPhyML.Resource res in removeList)
                    {
                        res.Delete();
                    }

                    foreach (var res in component.ResourceDependency)
                    {
                        if (!existing.Where(s => s.ToLower().EndsWith(res.Name.ToLower())).Any())
                        {
                            CyPhyML.Resource newRes = CyPhyMLClasses.Resource.Create(cyPhyComponent);
                            newRes.Attributes.ID = res.ID;
                            newRes.Name = res.Name;
                            string cmpath = Path.Combine(absModelDir, res.Name).Substring(cyPhyComponent.GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE).Length);
                            cmpath = cmpath.Trim(new char[] { '\\', '/' });
                            newRes.Attributes.Path = cmpath;
                        }
                    }

                    CyPhyML.Resource usesRes = null;
                    try
                    {
                        usesRes = cyPhyComponent.Children.ResourceCollection.Where(res => res.Attributes.ID.Equals(models.Key.UsesResource)).First();
                        if (!usesRes.SrcConnections.UsesResourceCollection.Any())
                        {
                            CyPhyMLClasses.UsesResource.Connect(newCADModel, usesRes);
                        }
                    }
                    catch (Exception)
                    {
                        GMEConsole.Warning.WriteLine(String.Format("Unable to set main resource for CADModel: {0}", newCADModel.ToHyperLink()));
                    }


                    try
                    {
                        string mainResourcePath = component.ResourceDependency.Where(resdep => resdep.ID == component.DomainModel[0].UsesResource).First().Path;

                        // Handle manufacturing files
                        string manufacturingPath = Path.Combine(Path.GetDirectoryName(mainResourcePath), "..", "Manufacturing");

                        RefreshManufacturingResources(cyPhyComponent, manufacturingPath);
                    }
                    catch (Exception)
                    {
                        GMEConsole.Warning.WriteLine(String.Format("Error during adding manufacturing resources to Component: {0}", cyPhyComponent.ToHyperLink()));
                    }



                    GMEConsole.Out.WriteLine(String.Format("Updated CADModel definition for <a href=\"mga:{0}\">{1}</a>", newCADModel.ID,
                        SecurityElement.Escape(newCADModel.ParentContainer.Name + "/" + newCADModel.Name)));
                    models.Value.Delete();
                }

                CyPhy2ComponentModel.CyPhyComponentAutoLayout.LayoutComponent(cyPhyComponent);

                string acmPath;
                if (ComponentLibraryManager.TryGetOriginalACMFilePath(cyPhyComponent, out acmPath, ComponentLibraryManager.PathConvention.ABSOLUTE))
                {
                    avm.Component newComponent = CyPhy2ComponentModel.Convert.CyPhyML2AVMComponent(cyPhyComponent);
                    CyPhyComponentExporter.CyPhyComponentExporterInterpreter.SerializeAvmComponent(oldComponent, acmPath);
                }
            }
            finally
            {
                addon.Project.CommitTransaction();
            }
        }

        /// <summary>
        /// Create a new component
        /// </summary>
        /// <param name="component_xml"></param>
        /// <param name="topic"></param>
        private void ProcessAVMComponentCreate(string component_xml, string instanceId)
        {
            /*try
            {
                File.WriteAllText(Path.Combine(Path.GetTempPath(), "CyPhyMetaLink_ComponentCreate.xml"), component_xml);
            }
            catch (IOException)
            {
            }*/

            XmlDocument doc = new XmlDocument();
            doc.LoadXml(component_xml);

            XmlNamespaceManager manager = new XmlNamespaceManager(doc.NameTable);
            manager.AddNamespace("avm", "avm");
            manager.AddNamespace("cad", "cad");

            avm.Component component = CyPhyComponentImporter.CyPhyComponentImporterInterpreter.DeserializeAvmComponentXml(new StringReader(doc.OuterXml));

            addon.Project.BeginTransactionInNewTerr();
            CyPhyML.Component createdComp = null;
            string createdavmid = null;
            try
            {
                var rf = CyPhyMLClasses.RootFolder.GetRootFolder(addon.Project);

                CyPhyML.Components importedComponents = CyphyMetaLinkUtils.GetImportedComponentsFolder(addon.Project);
                if (importedComponents == null)
                {
                    importedComponents = CyPhyMLClasses.Components.Create(rf);
                    importedComponents.Name = CyPhyComponentImporter.CyPhyComponentImporterInterpreter.ImportedComponentsFolderName;
                }

                var avmidComponentMap = CyPhyComponentImporter.CyPhyComponentImporterInterpreter.getCyPhyMLComponentDictionary_ByAVMID(rf);
                createdComp = CyPhy2ComponentModel.Convert.AVMComponent2CyPhyML(importedComponents, component, true, GMEConsole);

                // Ricardo requirement: Classification must not be empy
                if (createdComp.Attributes.Classifications.Length == 0)
                {
                    createdComp.Attributes.Classifications = "Unknown";
                }

                List<string> notcopied = new List<string>();
                CopyResources(Path.Combine(GetProjectDir(), createdComp.GetDirectoryPath(), "CAD\\"), doc, manager, notcopied, false);

                CyPhyML.CADModel cadModel = createdComp.Children.CADModelCollection.First();

                // Ricardo requirement: ID must be cad.path
                if (cadModel.SrcConnections.UsesResourceCollection.Any() && cadModel.SrcConnections.UsesResourceCollection.First().DstEnds.Resource != null)
                {
                    cadModel.SrcConnections.UsesResourceCollection.First().DstEnds.Resource.Attributes.ID = "cad.path";
                }
                else if (cadModel.DstConnections.UsesResourceCollection.Any() && cadModel.DstConnections.UsesResourceCollection.First().DstEnds.Resource != null)
                {
                    cadModel.DstConnections.UsesResourceCollection.First().DstEnds.Resource.Attributes.ID = "cad.path";
                }

                foreach (var res in createdComp.Children.ResourceCollection)
                {
                    res.Attributes.Path = Path.Combine("CAD", res.Name);
                }

                CyphyMetaLinkUtils.SetCADModelTypesFromFilenames(createdComp);

                RefreshManufacturingResources(createdComp, Path.Combine(createdComp.GetDirectoryPath(), "Manufacturing"));

                createdavmid = createdComp.Attributes.AVMID;

                if (createdavmid != null)
                {
                    SyncedComponentData cdata = GetSyncedCompDataByInstanceId(instanceId);
                    if (cdata.Type == SyncedComponentData.EditType.Empty)
                    {
                        cdata.Type = SyncedComponentData.EditType.Component;
                        cdata.Id = createdavmid;
                    }
                    HighlightInTree(createdComp, 1);

                    var outmessage = CreateComponentEditMessage(cdata.InstanceId, createdComp, CyphyMetaLinkUtils.FindCADModelObject(createdComp));

                    if (outmessage != null)
                    {
                        //SendInterest(null, ResyncTopic, message.topic[1]);
                        bridgeClient.SendToMetaLinkBridge(outmessage);
                    }
                }
            }
            finally
            {
                addon.Project.CommitTransaction();
            }
        }
        #endregion

        // Event handlers for addons
        #region MgaEventSink members
        public void GlobalEvent(globalevent_enum @event)
        {
            if (@event == globalevent_enum.GLOBALEVENT_CLOSE_PROJECT)
            {
                bridgeClient.CloseConnection();
                // clear the queue. no more messages are coming, since the receive thread is dead
                Edit _;
                while (queuedMessages.TryDequeue(out _))
                    ;
                if (GMEConsole != null)
                {
                    if (GMEConsole.gme != null)
                    {
                        Marshal.FinalReleaseComObject(GMEConsole.gme);
                    }
                    GMEConsole = null;
                }
                Marshal.FinalReleaseComObject(addon);
                // potential race: SyncControl.BeginInvoke was called before the receive thread was killed but after CLOSE_PROJECT
                SyncControl.BeginInvoke((System.Action)delegate
                {
                    SyncControl.Dispose();
                    SyncControl = null;
                });
                addon = null;
                metalinkBridge = null;
            }
            if (@event == globalevent_enum.APPEVENT_XML_IMPORT_BEGIN)
            {
                handleEvents = false;
                addon.EventMask = 0;
            }
            else if (@event == globalevent_enum.APPEVENT_XML_IMPORT_END)
            {
                unchecked { addon.EventMask = (uint)ComponentConfig_Addon.eventMask; }
                handleEvents = true;
            }
            else if (@event == globalevent_enum.APPEVENT_LIB_ATTACH_BEGIN)
            {
                addon.EventMask = 0;
                handleEvents = false;
            }
            else if (@event == globalevent_enum.APPEVENT_LIB_ATTACH_END)
            {
                unchecked { addon.EventMask = (uint)ComponentConfig_Addon.eventMask; }
                handleEvents = true;
            }
            else if (@event == globalevent_enum.GLOBALEVENT_COMMIT_TRANSACTION
                || @event == globalevent_enum.GLOBALEVENT_ABORT_TRANSACTION)
            {
                if (GMEConsole == null)
                {
                    GMEConsole = GMEConsole.CreateFromProject(addon.Project);
                }
                RestartAssemblySync();
                // n.b. MgaProject is in tx now, post a message so we can start our own
                SyncControl.BeginInvoke((System.Action)delegate
                {
                    if (addon != null)
                    {
                        if ((addon.Project.ProjectStatus & 8) != 0) // in tx
                        {
                        }
                        else
                        {
                            ProcessQueuedEditMessages();
                        }
                    }
                });
            }
            if (!componentEnabled)
            {
                return;
            }

            // TODO: Handle global events
            // MessageBox.Show(@event.ToString());
        }



        private HashSet<CyPhyML.ComponentAssembly> assembliesToRestart = new HashSet<CyPhyML.ComponentAssembly>();
        private void RestartAssemblySyncAtEndOfTransaction(CyPhyML.ComponentAssembly design)
        {
            if (design != null)
            {
                assembliesToRestart.Add(design);
            }
        }

        private void RestartAssemblySync()
        {
            foreach (var design in assembliesToRestart)
            {
                SyncControl.BeginInvoke((System.Action)delegate
                {
                    if ((addon.Project.ProjectStatus & 8) != 0) // in tx
                    {
                        return;
                    }
                    addon.Project.BeginTransactionInNewTerr();
                    try
                    {
                        try
                        {
                            handleEvents = false;
                            GenerateCADAssemblyXml(addon.Project, (MgaFCO)design.Impl, 0);
                            SyncedComponentData cdata = syncedComponents[design.Guid.ToString()];
                            SendCadAssemblyXml(cdata.InstanceId, design.Guid.ToString(), true);
                        }
                        catch (Exception ex)
                        {
                            AssemblyID = null;
                            GMEConsole.Error.WriteLine("Unable to generate model. Error: " + ex.Message);
                        }
                    }
                    finally
                    {
                        addon.Project.AbortTransaction();
                        handleEvents = true;
                    }
                });
            }
            assembliesToRestart.Clear();
        }

        #endregion

        private void StartComponentEditAction(MetaLinkProtobuf.Edit message)
        {
            // message.topic[1] is the component AVMID
            String avmid = GetSyncedCompDataByInstanceId(message.topic[0]).Id;
            StartComponentEdit(avmid);
        }

        private void StartComponentEdit(string avmid, bool transaction = true)
        {
            string componentCADDirectory = null;
            CyPhyML.Component component = null;
            if (transaction)
            {
                addon.Project.BeginTransactionInNewTerr();
            }

            try
            {
                component = CyphyMetaLinkUtils.GetComponentByAvmId(addon.Project, avmid);
                componentCADDirectory = Path.GetDirectoryName(GetCadModelPath(component));
                if (componentCADDirectory == null)
                {
                    componentCADDirectory = Path.Combine(component.GetDirectoryPath(), "CAD");
                    Directory.CreateDirectory(componentCADDirectory);
                    GMEConsole.Info.WriteLine("No CAD directory found, created one.");
                }
            }
            finally
            {
                if (transaction)
                {
                    addon.Project.CommitTransaction();
                }
            }
            StartAssemblyExe(CreoOpenMode.OPEN_COMPONENT, avmid, false, "", workingDir: componentCADDirectory);
        }

        private void SaveCadAssemblyXml(string xml, string assemblyGuid)
        {
            try
            {
                File.WriteAllText(Path.Combine(Path.GetTempPath(), "CyPhyMLPropagate_" + assemblyGuid + ".log"), xml);
            }
            catch (IOException)
            {
            }
            designIdToCadAssemblyXml[assemblyGuid] = xml;
        }

        private string GetProjectDir()
        {
            string workingDir = Path.GetTempPath();
            if (addon.Project.ProjectConnStr.StartsWith("MGA="))
            {
                workingDir = Path.GetDirectoryName(addon.Project.ProjectConnStr.Substring("MGA=".Length));
            }
            return workingDir;
        }

        private string GetStartupParams(string assemblyGUID)
        {
            CyPhyML.ComponentAssembly asm = CyphyMetaLinkUtils.GetComponentAssemblyByGuid(addon.Project, assemblyGUID);
            if (asm == null)
            {
                return "";
            }

            var param = asm.Children.ParameterCollection.Where(p => p.Name.ToUpper() == "CADEXEPARAMS");
            if (!param.Any())
            {
                return "";
            }

            return param.First().Attributes.Value;
        }

        /// <summary>
        /// Copy the resources to the directory of the created/updated component
        /// </summary>
        /// <param name="cadResourcesAbsPath"></param>
        /// <param name="doc"></param>
        /// <param name="manager"></param>
        /// <param name="notcopied"></param>
        /// <param name="update"></param>
        private void CopyResources(string cadResourcesAbsPath, XmlDocument doc, XmlNamespaceManager manager, List<string> notcopied, bool update)
        {
            XPathNavigator navigator = doc.CreateNavigator();

            var resourceDependencies = navigator.Select("/avm:Component/avm:ResourceDependency", manager).Cast<XPathNavigator>()
                // KMS: some XML contains <ResourceDependency ... xmlns="" /> which I think is an error, but we'll try to be robust
                .Concat(navigator.Select("/avm:Component/ResourceDependency", manager).Cast<XPathNavigator>());
            foreach (XPathNavigator node in resourceDependencies)
            {
                string path = node.GetAttribute("Path", "avm");
                if (String.IsNullOrWhiteSpace(path))
                {
                    path = node.GetAttribute("Path", "");
                }
                string resname = node.GetAttribute("Name", "avm");
                if (String.IsNullOrWhiteSpace(resname))
                {
                    resname = node.GetAttribute("Name", "");
                }
                try
                {
                    // Add the drive to the path is it's not present
                    path = Path.GetFullPath(path);
                    // Since this function is used in update as well, skip resources which are already in the
                    // component path
                    if (!path.StartsWith(cadResourcesAbsPath))
                    {
                        Directory.CreateDirectory(cadResourcesAbsPath);
                        if (String.IsNullOrWhiteSpace(path) == false)
                        {
                            // Path may be absolute or relative to ProjectDir
                            if (Path.IsPathRooted(path) == false)
                            {
                                path = Path.Combine(GetProjectDir(), path);
                            }
                            if (File.Exists(path))
                            {
                                try
                                {
                                    File.Copy(path, Path.Combine(cadResourcesAbsPath, Path.GetFileName(path)), true);
                                }
                                catch (Exception ex)
                                {
                                    GMEConsole.Warning.WriteLine(String.Format("Error during copying resource file {0} to {1}: {2}", path, cadResourcesAbsPath, ex.Message));
                                }
                            }
                            else if (Directory.Exists(Path.GetDirectoryName(path)))
                            {
                                // CAD files end in .1 .2 etc. Pick the latest ones
                                IEnumerable<string> allFiles = Directory.EnumerateFiles(Path.GetDirectoryName(path), resname + ".*");

                                var latestFile = allFiles.Select(Path.GetFileName)
                                .Select(filename => new { basename = filename.Substring(0, filename.LastIndexOf('.')), version = filename.Substring(filename.LastIndexOf('.') + 1) })
                                .Where(p => { int val = 0; return Int32.TryParse(p.version, out val); })
                                .OrderByDescending(p => Int32.Parse(p.version)).ToArray();

                                // If there's no versioned version of the filename (e.g. XY.PRT.1, try the unversioned one e.g. XY.PRT)
                                if (!latestFile.Any())
                                {
                                    allFiles = Directory.EnumerateFiles(Path.GetDirectoryName(path), resname);
                                    latestFile = allFiles.Select(Path.GetFileName).Select(filename => new { basename = filename, version = "0" }).ToArray();
                                }

                                foreach (var basename in latestFile.Select(p => p.basename).Distinct())
                                {
                                    var latest = latestFile.Where(p => p.basename == basename).FirstOrDefault();
                                    if (latest != null)
                                    {
                                        string latestFilename = latest.basename + ((latest.version != "0") ? ("." + latest.version) : "");
                                        string source = Path.Combine(Path.GetDirectoryName(path), latestFilename);
                                        string dest = Path.Combine(cadResourcesAbsPath, latestFilename);
                                        try
                                        {
                                            File.Copy(source, dest, true);
                                        }
                                        catch (Exception ex)
                                        {
                                            GMEConsole.Warning.WriteLine(String.Format("Error during copying resource file {0} to {1}: {2}", source, dest, ex.Message));
                                        }
                                    }
                                }
                            }
                            else
                            {
                                GMEConsole.Warning.WriteLine("Could not find component directory '" + path + "'");
                            }
                        }
                    }
                    else
                    {
                        notcopied.Add(resname);
                    }
                }
                catch (PathTooLongException ex)
                {
                    throw new Exception(String.Format("Error processing component resource, resource: {0}, path: {1}, message: {2}", resname, path, ex.Message));
                }
            }
        }

        private void RefreshManufacturingResources(CyPhyML.Component component, string path)
        {
            List<CyPhyML.Resource> newResources = new List<CyPhyML.Resource>();

            CyPhyML.ManufacturingModel mmodel = null;


            if (Directory.Exists(path))
            {
                var allFiles = Directory.EnumerateFiles(path, "*.xml")
                    .Select(Path.GetFileName);

                foreach (var res in allFiles)
                {
                    if (mmodel == null)
                    {
                        if (!component.Children.ManufacturingModelCollection.Any())
                        {
                            mmodel = CyPhyMLClasses.ManufacturingModel.Create(component);
                        }
                        else
                        {
                            mmodel = component.Children.ManufacturingModelCollection.First();
                        }
                    }
                    if (!component.Children.ResourceCollection.Where(resource => resource.Attributes.Path.Equals(Path.Combine("Manufacturing", res))).Any())
                    {
                        CyPhyML.Resource newRes = CyPhyMLClasses.Resource.Create(component);
                        newRes.Name = res;
                        newRes.Attributes.Path = Path.Combine("Manufacturing", res);
                        newResources.Add(newRes);
                        CyPhyMLClasses.UsesResource.Connect(mmodel, newRes);
                    }
                }
            }
        }
    }
}
