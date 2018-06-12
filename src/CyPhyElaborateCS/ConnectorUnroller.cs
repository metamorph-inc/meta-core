using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using GME.MGA;
using CyPhyCOMInterfaces;
using CyPhyGUIs;
using META;
using GME.MGA.Meta;
using GME.CSharp;

namespace CyPhyElaborateCS
{
    public class Unroller : IDisposable
    {
        public Unroller(MgaProject proj, IMgaTraceability Traceability = null, GMELogger Logger = null)
        {
            this.ConnectorsProcessed = new List<MgaModel>();
            this.ConnectorToStandalonePortMap = new Dictionary<MgaModel, List<PortWrapper>>();

            SupportedPortTypesMeta = new HashSet<int>();
            foreach (var kind in SupportedPortTypes)
            {
                SupportedPortTypesMeta.Add(proj.RootMeta.RootFolder.DefinedFCOByName[kind, true].MetaRef);
            }

            MetaRef = new Dictionary<String, int>();
            MetaRef.Add("Component", proj.RootMeta.RootFolder.DefinedFCOByName["Component", true].MetaRef);
            MetaRef.Add("ComponentAssembly", proj.RootMeta.RootFolder.DefinedFCOByName["ComponentAssembly", true].MetaRef);
            MetaRef.Add("Connector", proj.RootMeta.RootFolder.DefinedFCOByName["Connector", true].MetaRef);
            MetaRef.Add("TestBench", proj.RootMeta.RootFolder.DefinedFCOByName["TestBench", true].MetaRef);
            MetaRef.Add("TestComponent", proj.RootMeta.RootFolder.DefinedFCOByName["TestComponent", true].MetaRef);

            if (Logger == null)
            {
                this.Logger = new GMELogger(proj, "ConnectorUnroller");
                myLogger = true;
                this.Logger.LoggingLevel = SmartLogger.MessageType_enum.Warning;
            }
            else
            {
                this.Logger = Logger;
                myLogger = false;
            }

            if (Traceability == null)
            {
                this.Traceability = new META.MgaTraceability();
            }
            else
            {
                this.Traceability = Traceability;
            }
        }

        ~Unroller()
        {
            Dispose();
        }

        public void Dispose()
        {
            if (myLogger && Logger != null)
            {
                Logger.Dispose();
                Logger = null;
            }
        }

        public IMgaTraceability Traceability { get; private set; }
        private Boolean myLogger;
        public GMELogger Logger { get; private set; }

        /// <summary>
        /// For a given Connector (the key), the list contains all the standalone ports created from it.
        /// </summary>
        private Dictionary<MgaModel, List<PortWrapper>> ConnectorToStandalonePortMap;

        /// <summary>
        /// A running list of all Connectors processed since the Unroller was instantiated.
        /// Used during the cleanup phase to delete empty connectors and port "originals".
        /// </summary>
        private List<MgaModel> ConnectorsProcessed;

        /// <summary>
        /// A handy empty array of MgaFCOs
        /// </summary>
        private MgaFCOs EmptyArray = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));

        /// <summary>
        /// A list of MetaRef values for concrete Port subtypes that are supported by the Unroller.
        /// </summary>
        private HashSet<int> SupportedPortTypesMeta;

        /// <summary>
        /// For each model kind (the key), the MetaRef value of that kind.
        /// </summary>
        private Dictionary<String, int> MetaRef;

        /// <summary>
        /// A list of the kind names of concrete Port subtypes that are supported by the Unroller.
        /// </summary>
        public static List<String> SupportedPortTypes = new List<String>()
        {
            "AbstractPort",
            "BondGraphPort",
            "ModelicaConnector",
            "SimulinkPort",
            "GenericDomainModelPort"
        };

        /// <summary>
        /// Used to track the relationships between "unrolled" standalone ports and their original ports/connectors.
        /// </summary>
        private class PortWrapper
        {
            /// <summary>
            /// The connector from which the port was unrolled.
            /// </summary>
            public MgaModel SourceConnector { get; set; }

            /// <summary>
            /// The "role" of the source port within the connector.
            /// In this case, it's the name of the port within the connector.
            /// </summary>
            public String SourcePortRole { get; set; }

            /// <summary>
            /// The original port, within the original connector.
            /// </summary>
            public MgaFCO SourcePort { get; set; }

            /// <summary>
            /// The new port "unrolled" from the connector.
            /// </summary>
            public MgaFCO StandalonePort { get; set; }
        }

        /// <summary>
        /// Iterate over all processed connectors.
        /// Delete any ports that have been broken out into standalone ports.
        /// If no ports remain, delete the whole connector.
        /// </summary>
        private void DeleteConnectorsAndPorts()
        {
            Logger.WriteDebug("DeleteConnectorsAndPorts");

            foreach (var connector in ConnectorsProcessed.Where(c => c.IsInstance == false && c.ArcheType == null))
            {
                foreach (MgaFCO port in connector.ChildObjects)
                {
                    if (port.ArcheType != null)
                    {
                        break;
                    }
                    if (IsSupported(port))
                    {
                        port.DestroyObject();
                    }
                }

                if (false == connector.ChildObjects.Count > 0)
                {
                    connector.DestroyObject();
                }
            }
        }

        /// <summary>
        /// Given a port, check if its kind is supported by the Unroller.
        /// </summary>
        /// <param name="port"></param>
        /// <returns>True if supported</returns>
        private bool IsSupported(MgaFCO port)
        {
            return SupportedPortTypesMeta.Contains(port.MetaBase.MetaRef);
        }

        /// <summary>
        /// Given an FCO, take each attribute and explicitly assign it.
        /// This gets around a problem where attribute values are wiped out
        /// if they were inherited, and then inheritance was broken.
        /// </summary>
        /// <param name="fco"></param>
        public static void SetAttributesLocally(MgaFCO fco)
        {
            if (fco is MgaModel)
            {
                MgaModel model = fco as MgaModel;
                foreach (MgaFCO child in model.ChildFCOs)
                {
                    SetAttributesLocally(child);
                }
            }

            foreach (MgaAttribute attr in fco.Attributes)
            {
                if (attr.Status > (int)attstatus_enum.ATTSTATUS_HERE)
                {
                    attr.Value = attr.Value;
                }
            }
        }

        public void UnrollComponentAssembly(MgaModel ca)
        {
            Logger.WriteDebug("UnrollComponentAssembly: {0}", ca.AbsPath);

            VisitComponentAssembly(ca);
            DeleteConnectorsAndPorts();
        }

        public void UnrollTestBench(MgaModel tb)
        {
            Logger.WriteDebug("UnrollTestBench: {0}", tb.AbsPath);

            VisitTestBench(tb);
            DeleteConnectorsAndPorts();
        }

        public void UnrollComponent(MgaModel comp, Boolean cleanup = true)
        {
            Logger.WriteDebug("UnrollComponent: {0}", comp.AbsPath);

            VisitComponent(comp);
            if (cleanup)
            {
                DeleteConnectorsAndPorts();
            }
        }

        HashSet<MgaModel> componentsProcessed = new HashSet<MgaModel>();
        private void VisitComponent(MgaModel comp)
        {
            #region Check arguments
            if (comp == null)
            {
                throw new ArgumentNullException("comp");
            }
            if (comp.MetaBase.MetaRef != MetaRef["Component"])
            {
                throw new ArgumentException("Input parameter was not a component.", "comp");
            }
            #endregion
            Logger.WriteDebug("VisitComponent: {0}", comp.AbsPath);

            if (componentsProcessed.Contains(comp))
            {
                Logger.WriteDebug("VisitComponent: Skipping (already processed) {0}", comp.AbsPath);

                return;
            }

            // Is this an instance?
            // If so, process the archetype only.
            if (comp.ArcheType != null)
            {
                Logger.WriteDebug("VisitComponent: Processing Archetype instead {0}", comp.AbsPath);

                var archetype = comp.ArcheType as MgaModel;
                VisitComponent(archetype);

                return;
            }

            foreach (MgaModel connector in comp.GetChildrenOfKind("Connector"))
            {
                VisitConnector(connector);
            }
        }

        private void VisitTestComponent(MgaModel testComp)
        {
            #region Check arguments
            if (testComp == null)
            {
                throw new ArgumentNullException("testComp");
            }
            if (testComp.MetaBase.MetaRef != MetaRef["TestComponent"])
            {
                throw new ArgumentException("Input parameter was not a Test Component.", "testComp");
            }
            #endregion
            Logger.WriteDebug("VisitTestComponent: {0}", testComp.AbsPath);

            if (testComp.ArcheType != null)
            {
                Logger.WriteDebug("VisitTestComponent: Has an archetype, detaching {0}", testComp.AbsPath);

                SetAttributesLocally(testComp as MgaFCO);
                testComp.DetachFromArcheType();
            }

            foreach (MgaModel connector in testComp.GetChildrenOfKind("Connector"))
            {
                VisitConnector(connector);
            }
        }

        private void VisitComponentAssembly(MgaModel assembly)
        {
            #region Check arguments
            if (assembly == null)
            {
                throw new ArgumentNullException("assembly");
            }
            if (assembly.MetaBase.MetaRef != MetaRef["ComponentAssembly"])
            {
                throw new ArgumentException("Input parameter was not a component assembly.", "assembly");
            }
            #endregion
            Logger.WriteDebug("VisitComponentAssembly: {0}", assembly.AbsPath);

            foreach (MgaModel asm in assembly.GetChildrenOfKind("ComponentAssembly"))
            {
                VisitComponentAssembly(asm);
            }

            foreach (MgaModel component in assembly.GetChildrenOfKind("Component"))
            {
                VisitComponent(component as MgaModel);
            }

            foreach (MgaModel connector in assembly.GetChildrenOfKind("Connector"))
            {
                VisitConnector(connector);
            }

            ExpandConnectorCompositionChildren(assembly);
        }

        /// <summary>
        /// For a given connector, see what connections involve its constituent ports.
        /// For each such connection, modify it to terminate at the new standalone port.
        /// </summary>
        /// <param name="connector">The CyPhy Connector to be processed</param>
        private void RedirectConnectionsToConstituentPorts(MgaModel connector)
        {
            #region Check arguments
            if (connector == null)
            {
                throw new ArgumentNullException("connector");
            }
            if (connector.MetaBase.MetaRef != MetaRef["Connector"])
            {
                throw new ArgumentException("Input parameter was not a connector.", "connector");
            }
            #endregion
            Logger.WriteDebug("RedirectConnectionsToConstituentPorts: {0}", connector.AbsPath);

            foreach (var port in ConnectorToStandalonePortMap[connector])
            {
                foreach (MgaConnPoint connPoint in port.SourcePort.PartOfConns)
                {
                    var connection = connPoint.Owner as MgaSimpleConnection;
                    var parent = connection.ParentModel;

                    if (connection.Src == port.SourcePort)
                    {
                        var objMetaRole = (parent.Meta as MgaMetaModel).RoleByName["PortComposition"];
                        parent.CreateSimplerConnDisp(objMetaRole, port.StandalonePort, connection.Dst);

                        //connection.SetSrc(null, port.StandalonePort);
                    }
                    else if (connection.Dst == port.SourcePort)
                    {
                        var objMetaRole = (parent.Meta as MgaMetaModel).RoleByName["PortComposition"];
                        parent.CreateSimplerConnDisp(objMetaRole, connection.Src, port.StandalonePort);

                        //connection.SetDst(null, port.StandalonePort);
                    }

                    connection.DestroyObject();
                }
            }
        }

        /// <summary>
        /// Given a container, find all ConnectorComposition connections, and make 
        /// new connections between the new "standalone" ports that have been created.
        /// </summary>
        /// <param name="container"></param>
        private void ExpandConnectorCompositionChildren(MgaModel container)
        {
            Logger.WriteDebug("ExpandConnectorCompositionChildren: {0}", container.AbsPath);

            // Find PortComposition role for this parent type
            GME.MGA.Meta.MgaMetaRole role = null;
            foreach (GME.MGA.Meta.MgaMetaRole roleItem in (container.Meta as GME.MGA.Meta.MgaMetaModel).Roles)
            {
                if (roleItem.Name == "PortComposition")
                {
                    role = roleItem;
                    break;
                }
            }

            // For each ConnectorComposition, create new connections between the new "standalone" ports.
            // Since we did depth-first recursion in modifying the connectors, they should all be "expanded"
            // and ready to go.
            foreach (MgaSimpleConnection connectorComposition in container.GetChildrenOfKind("ConnectorComposition"))
            {
                MgaModel connector1 = connectorComposition.Src as MgaModel;
                MgaModel connector2 = connectorComposition.Dst as MgaModel;

                var portsConn1 = ConnectorToStandalonePortMap[connector1];
                var portsConn2 = ConnectorToStandalonePortMap[connector2];

                // For each port, find the analogue from the other connector.
                foreach (var port1 in portsConn1)
                {
                    var kindPort1 = port1.SourcePort.MetaBase.Name;

                    // Try to match by role & kind.
                    var port2 = portsConn2.FirstOrDefault(p2 => p2.SourcePortRole == port1.SourcePortRole
                                                             && p2.SourcePort.MetaBase.Name == kindPort1);

                    // Nominal match case failed. Try alternatives.
                    if (port2 == null)
                    {
                        // If we failed to match by the methods above, we'll try another method.
                        // Try to see if each port's kind is unique to its connector (e.g.: They are the only ModelicaConnectors in their parent Connectors).
                        // If so, then we will go ahead and match them, but yield a warning.

                        Boolean port1KindIsUnique = portsConn1.Where(pc => kindPort1 == pc.SourcePort.MetaBase.Name).Count() == 1;

                        var port2KindMatches = portsConn2.Where(pc => kindPort1 == pc.SourcePort.MetaBase.Name);
                        Boolean port2KindIsUnique = port2KindMatches.Count() == 1;

                        if (port1KindIsUnique && port2KindIsUnique)
                        {
                            // Match anyway based on unique kinds.
                            port2 = port2KindMatches.First();

                            Logger.WriteWarning("Non-name match: " +
                                                "Port {0} in Connector {1} and Port {2} in Connector {3}",
                                                TraceabilityExtensions.ToMgaHyperLink(port1.SourcePort, Traceability),
                                                TraceabilityExtensions.ToMgaHyperLink(port1.SourceConnector, Traceability),
                                                TraceabilityExtensions.ToMgaHyperLink(port2.SourcePort, Traceability),
                                                TraceabilityExtensions.ToMgaHyperLink(port2.SourceConnector, Traceability));
                        }
                        else
                        {
                            // Kinds were not unique, so we can't guess.
                            Logger.WriteWarning("NO MATCH found for Port {0} of Connector {1} with any Port within Connector {2}",
                                                TraceabilityExtensions.ToMgaHyperLink(port1.SourcePort, Traceability),
                                                TraceabilityExtensions.ToMgaHyperLink(port1.SourceConnector, Traceability),
                                                TraceabilityExtensions.ToMgaHyperLink(connector2, Traceability));
                            continue;
                        }
                    }

                    var conn = container.CreateChildObject(role) as MgaSimpleConnection;
                    conn.SetSrc(EmptyArray, port1.StandalonePort);
                    conn.SetDst(EmptyArray, port2.StandalonePort);
                }
            }
        }

        // What do we have to do here now?
        // Well, if this connector has instances, we need to add port maps for those things too.
        // Then those connectors need to be visited to connections can be adjusted.
        private void VisitConnector(MgaModel connector)
        {
            #region Check arguments
            if (connector == null)
            {
                throw new ArgumentNullException("connector");
            }
            if (connector.MetaBase.MetaRef != MetaRef["Connector"])
            {
                throw new ArgumentException("Input parameter was not a connector.", "connector");
            }
            if (ConnectorsProcessed.Contains(connector))
            {
                return;
            }
            #endregion
            Logger.WriteDebug("VisitConnector: {0}", connector.AbsPath);

            ConnectorToStandalonePortMap.Add(connector, new List<PortWrapper>());
            var parent = connector.ParentModel;
            
            Boolean connectorIsArchetype = (connector.ArcheType == null);
            Boolean parentIsArchetype = (parent.ArcheType == null);
            if (parentIsArchetype)
            {
                Logger.WriteDebug("VisitConnector: Processing with parent as Archetype");

                if (connectorIsArchetype)
                {
                    Logger.WriteDebug("VisitConnector: Processing connector as Archetype");
                }
                else
                {
                    Logger.WriteDebug("VisitConnector: Processing connector as Derived");
                    Logger.WriteDebug("VisitConnector: Setting connector's local attributes and detaching");

                    SetAttributesLocally(connector as MgaFCO);
                    connector.DetachFromArcheType();
                }

                // I'm an archetype.
                // Go through each supported port, clone it, and create a wrapper.
                // Add it to traceability.
                // Redirect connections to constituent ports.
                foreach (MgaFCO port in connector.ChildFCOs)
                {
                    if (false == SupportedPortTypesMeta.Contains(port.MetaBase.MetaRef))
                    {
                        Logger.WriteDebug("VisitConnector: Skipping unsupported port {0}", port.AbsPath);

                        continue;
                    }

                    var newPort = ClonePort(parent, port);
                    newPort.Name = String.Format("{0}__{1}", connector.Name, port.Name);

                    var wrapper = new PortWrapper()
                    {
                        SourceConnector = connector,
                        SourcePortRole = port.Name,
                        SourcePort = port,
                        StandalonePort = newPort
                    };
                    ConnectorToStandalonePortMap[connector].Add(wrapper);

                    // See if the derived guy is itself in the Traceability.
                    String lastGood = port.ID;
                    String iter = null;
                    while (Traceability.TryGetMappedObject(lastGood, out iter) 
                           && lastGood != iter)
                    {
                        lastGood = iter;
                    }
                    Traceability.AddItem(newPort.ID, lastGood);
                }

                RedirectConnectionsToConstituentPorts(connector);
            }
            else
            {
                Logger.WriteDebug("VisitConnector: Processing as Derived");

                var connectorArchetype = connector.ArcheType as MgaModel;

                // I'm derived. What we need to do is go through each
                // port, and create a new wrapper for each one.
                foreach (MgaFCO derivedPort in connector.ChildFCOs)
                {
                    if (false == SupportedPortTypesMeta.Contains(derivedPort.MetaBase.MetaRef))
                    {
                        Logger.WriteDebug("VisitConnector: Skipping unsupported derived port {0}", derivedPort.AbsPath);

                        continue;
                    }

                    // Find the wrapper for these guys
                    var orgWrapper = ConnectorToStandalonePortMap[connectorArchetype]
                                                    .FirstOrDefault(w => w.SourceConnector == connectorArchetype
                                                                      && w.SourcePort == derivedPort.ArcheType);

                    if (orgWrapper == null)
                    {
                        Logger.WriteDebug("VisitConnector: orgWrapper is null for {0}", derivedPort.AbsPath);
                        throw new ArgumentNullException("orgWrapper is null for " + derivedPort.AbsPath);
                    }

                    // Now guess at the standalone port.
                    MgaFCO standalonePort = null;
                    foreach (MgaFCO brokenOutPort in parent.ChildFCOs)
                    {
                        if (brokenOutPort.ArcheType != null
                            && brokenOutPort.ArcheType == orgWrapper.StandalonePort)
                        {
                            standalonePort = brokenOutPort;
                            break;
                        }
                    }

                    if (standalonePort == null)
                    {
                        Logger.WriteDebug("VisitConnector: standalonePort is null for {0}", derivedPort.AbsPath);
                        throw new ArgumentNullException("standalonePort is null for " + derivedPort.AbsPath);
                    }

                    var wrapper = new PortWrapper()
                    {
                        SourceConnector = connector,
                        SourcePortRole = derivedPort.Name,
                        SourcePort = derivedPort,
                        StandalonePort = standalonePort
                    };
                    ConnectorToStandalonePortMap[connector].Add(wrapper);

                    // See if the derived guy is itself in the Traceability.
                    String lastGood = derivedPort.ID;
                    String iter = null;
                    while (Traceability.TryGetMappedObject(lastGood, out iter)
                           && lastGood != iter)
                    {
                        lastGood = iter;
                    }
                    Traceability.AddItem(standalonePort.ID, lastGood);
                }
            }

            foreach (MgaModel derivedConnector in connector.DerivedObjects)
            {
                Logger.WriteDebug("VisitConnector: Visiting derived connector {0}", derivedConnector.AbsPath);
                VisitConnector(derivedConnector);
            }

            ConnectorsProcessed.Add(connector);
        }

        private void VisitTestBench(MgaModel testbench)
        {
            #region Check arguments
            if (testbench == null)
            {
                throw new ArgumentNullException("testbench");
            }
            if (testbench.MetaBase.MetaRef != MetaRef["TestBench"])
            {
                throw new ArgumentException("Input parameter was not a connector.", "testbench");
            }
            #endregion
            Logger.WriteDebug("VisitTestBench: {0}", testbench.AbsPath);

            foreach (MgaModel asm in testbench.GetChildrenOfKind("ComponentAssembly"))
            {
                VisitComponentAssembly(asm);
            }

            foreach (MgaModel component in testbench.GetChildrenOfKind("Component"))
            {
                VisitComponent(component);
            }

            foreach (MgaModel testComp in testbench.GetChildrenOfKind("TestComponent"))
            {
                VisitTestComponent(testComp);
            }

            ExpandConnectorCompositionChildren(testbench);
        }

        private MgaFCO ClonePort(MgaModel parent, MgaFCO oldPort)
        {
            Logger.WriteDebug("ClonePort: {0}", oldPort.AbsPath);

            GME.MGA.Meta.MgaMetaRole role = null;

            foreach (GME.MGA.Meta.MgaMetaRole roleItem in (parent.Meta as GME.MGA.Meta.MgaMetaModel).Roles)
            {
                if (roleItem.Kind.MetaRef == oldPort.MetaBase.MetaRef)
                {
                    role = roleItem;
                    break;
                }
            }

            var newPortFCO = parent.CopyFCODisp(oldPort, role);

            String lastGood = oldPort.ID;
            String iter = null;
            while (Traceability.TryGetMappedObject(lastGood, out iter)
                   && lastGood != iter)
            {
                lastGood = iter;
            }
            Traceability.AddItem(newPortFCO.ID, lastGood);

            return newPortFCO;
        }
    }
}
