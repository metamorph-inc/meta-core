using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using GmeCommon = ISIS.GME.Common;
using GME.MGA;

namespace CyPhy2CAD_CSharp
{
    // META-947: Use Connectors and CADDatums instead of StructuralInterface
    class CommonTraversal
    {
        private List<string> visitedPorts;
        private List<string> visitedConns;
        private string startNodeID;
        CyPhy.ComponentAssembly topAssembly;
        public List<GmeCommon.Interfaces.FCO> FoundConnectedNodes { get; set; }

        public CommonTraversal(GmeCommon.Interfaces.FCO start, CyPhy.ComponentAssembly topassembly)
        {
            Initialize();
            startNodeID = start.ID;

            this.topAssembly = topassembly;

            if (start is CyPhy.Connector)
            {
                VisitConnector(start as CyPhy.Connector, (MgaFCO)start.ParentContainer.Impl);
            }
            else if (start is CyPhy.CADDatum)
            {
                VisitCADDatum(start as CyPhy.CADDatum);
            }
        }

        public CommonTraversal(CyPhy.ConnectorComposition connection, CyPhy.ComponentAssembly topassembly)
        {
            Initialize();
            startNodeID = "";

            this.topAssembly = topassembly;

            VisitConnectorComposition(connection);
        }

        private void Initialize()
        {
            visitedPorts = new List<string>();
            visitedConns = new List<string>();
            FoundConnectedNodes = new List<GmeCommon.Interfaces.FCO>();
        }

        private MgaFCO GetRefportOrParent(MgaConnection conn, string role = "src")
        {
            MgaConnPoint connPoint = conn.ConnPoints.Cast<MgaConnPoint>().Where(cp => cp.ConnRole == role).First();
            if (connPoint.References != null && connPoint.References.Count > 0)
            {
                return connPoint.References[1];
            }
            return connPoint.Target;
        }

        private bool IsParent(CyPhy.ConnectorComposition conn, CyPhy.ComponentAssembly topassembly)
        {
            GmeCommon.Interfaces.Container container = conn.ParentContainer;
            while (container != null)
            {
                if (container.Guid == topAssembly.Guid)
                {
                    return true;
                }

                container = container.ParentContainer;
            }
            return false;
        }

        private void VisitConnector(CyPhy.Connector connector, MgaFCO parent)
        {
            if (!visitedPorts.Contains(connector.ID + "_" + parent.ID))
            {
                visitedPorts.Add(connector.ID + "_" + parent.ID);

                bool parentIsComponent = (connector.ParentContainer is CyPhy.Component);
                bool isStart = (connector.ID == startNodeID);

                if (!isStart && parentIsComponent)
                {
                    FoundConnectedNodes.Add(connector);
                }

                foreach (CyPhy.ConnectorComposition conn in connector.SrcConnections.ConnectorCompositionCollection)
                {
                    if (topAssembly != null && !IsParent(conn, topAssembly))
                    {
                        continue;
                    }

                    if (parent.ObjType != GME.MGA.Meta.objtype_enum.OBJTYPE_REFERENCE ||
                       GetRefportOrParent((MgaConnection)conn.Impl, "src").ID == parent.ID)
                    {
                        VisitConnector(conn.SrcEnds.Connector, parent);
                    }
                }

                foreach (CyPhy.ConnectorComposition conn in connector.DstConnections.ConnectorCompositionCollection)
                {
                    if (topAssembly != null && !IsParent(conn, topAssembly))
                    {
                        continue;
                    }

                    if (parent.ObjType != GME.MGA.Meta.objtype_enum.OBJTYPE_REFERENCE ||
                       GetRefportOrParent((MgaConnection)conn.Impl, "dst").ID == parent.ID)
                    {
                        VisitConnector(conn.DstEnds.Connector, parent);
                    }
                }
            }
        }

        private void VisitCADDatum(CyPhy.CADDatum datum)
        {
            if (!visitedPorts.Contains(datum.ID))
            {
                visitedPorts.Add(datum.ID);

                bool parentIsComponent = (datum.ParentContainer is CyPhy.Component);
                bool isStart = (datum.ID == startNodeID);


                if (!isStart && parentIsComponent)
                {
                    FoundConnectedNodes.Add(datum);
                }

                foreach (CyPhy.PortComposition conn in datum.SrcConnections.PortCompositionCollection)
                {
                    visitedConns.Add(conn.ID);
                    VisitCADDatum(conn.SrcEnds.CADDatum);
                }

                foreach (CyPhy.PortComposition conn in datum.DstConnections.PortCompositionCollection)
                {
                    visitedConns.Add(conn.ID);
                    VisitCADDatum(conn.DstEnds.CADDatum);
                }
            }
        }

        private void VisitConnectorComposition(CyPhy.ConnectorComposition connection)
        {
            CyPhy.Connector dstconnector = connection.DstEnds.Connector;
            CyPhy.Connector srcconnector = connection.SrcEnds.Connector;
            if (dstconnector != null)
            {
                VisitConnector(dstconnector, GetRefportOrParent((MgaConnection)connection.Impl, "dst"));
            }

            if (srcconnector != null)
            {
                VisitConnector(srcconnector, GetRefportOrParent((MgaConnection)connection.Impl, "src"));
            }
        }

        #region Deprecated
        // META-947: deprecated
        /*
        private void VisitStructuralInterface(CyPhy.StructuralPortType port)
        {
            if (!visitedPorts.Contains(port.ID))
            {
                if (port.Kind == "StructuralInterface")
                {
                    if (startStructuralInterfaceID != port.ID)
                        FoundStructuralInterface.Add(port as CyPhy.StructuralInterface);
                }

                visitedPorts.Add(port.ID);
                foreach (CyPhy.JoinStructures js in port.DstConnections.JoinStructuresCollection)
                {
                    visitedConns.Add(js.ID);
                    CyPhy.StructuralPortType ptype = js.DstEnds.StructuralPortType;
                    VisitStructuralInterface(ptype);
                }

                foreach (CyPhy.JoinStructures js in port.SrcConnections.JoinStructuresCollection)
                {
                    visitedConns.Add(port.ID);
                    CyPhy.StructuralPortType ptype = js.SrcEnds.StructuralPortType;
                    VisitStructuralInterface(ptype);
                }
            }
        }
        */
        #endregion

        public int GetVisitedConnectionCount()
        {
            return visitedConns.Count();
        }
    }   // end class


    class ReferenceCoordinateSystemTraversal
    {
        public List<string> referenceCoordComponents { get; set; }
        private List<string> visitedIDs;

        public ReferenceCoordinateSystemTraversal(CyPhy.ReferenceCoordinateSystem port)
        {
            visitedIDs = new List<string>();
            referenceCoordComponents = new List<string>();

            VisitReferenceCoordinateSystem(port);
        }

        private void VisitReferenceCoordinateSystem(CyPhy.ReferenceCoordinateSystem port)
        {
            if (!visitedIDs.Contains(port.ID))
            {
                visitedIDs.Add(port.ID);

                if (port.ParentContainer.Kind == "Component")
                {
                    referenceCoordComponents.Add(port.ParentContainer.ID);
                }

                foreach (var item in port.DstConnections.RefCoordSystem2RefCoordSystemCollection)
                {
                    VisitReferenceCoordinateSystem(item.DstEnds.ReferenceCoordinateSystem);
                }

                foreach (var item in port.SrcConnections.RefCoordSystem2RefCoordSystemCollection)
                {
                    VisitReferenceCoordinateSystem(item.SrcEnds.ReferenceCoordinateSystem);
                }
            }
        }
    }   // end class


    class PointMetricTraversal
    {
        public List<CyPhy.Port> portsFound { get; set; }
        private List<string> visitedItems;
        private string portKind;

        public PointMetricTraversal(CyPhy.Port port)
        {
            portsFound = new List<CyPhy.Port>();
            visitedItems = new List<string>();
            if (port.Kind == "OrdinalPoint")
            {
                portKind = "Point";
            }
            else
            {
                portKind = port.Kind;
            }

            VisitPort(port);
        }

        private void VisitPort(CyPhy.Port port)
        {
            if (!visitedItems.Contains(port.ID))
            {
                visitedItems.Add(port.ID);

                foreach (CyPhy.PortComposition conn in port.SrcConnections.PortCompositionCollection)
                {
                    CyPhy.Port feature = conn.SrcEnds.Port;
                    if (feature != null)
                    {
                        if (feature.ParentContainer.Kind == "CADModel" && feature.Kind == portKind)
                        {
                            portsFound.Add(feature);
                        }
                        else
                        {
                            VisitPort(feature);
                        }
                    }
                }

                foreach (CyPhy.PortComposition conn in port.DstConnections.PortCompositionCollection)
                {
                    CyPhy.Port feature = conn.DstEnds.Port;
                    if (feature != null)
                    {
                        if (feature.ParentContainer.Kind == "CADModel" && feature.Kind == portKind)
                        {
                            portsFound.Add(feature);
                        }
                        else
                        {
                            VisitPort(feature);
                        }
                    }
                }

                if (port.Kind == "Surface")
                {
                    foreach (CyPhy.SurfaceReverseMap conn in (port as CyPhy.Surface).SrcConnections.SurfaceReverseMapCollection)
                    {
                        CyPhy.Surface surface = conn.SrcEnds.Surface;
                        if (surface != null)
                        {
                            if (surface.ParentContainer.Kind == "CADModel")
                            {
                                portsFound.Add(surface);
                            }
                        }
                        else
                        {
                            VisitPort(surface);
                        }
                    }

                    foreach (CyPhy.SurfaceReverseMap conn in (port as CyPhy.Surface).DstConnections.SurfaceReverseMapCollection)
                    {
                        CyPhy.Surface surface = conn.DstEnds.Surface;
                        if (surface != null)
                        {
                            if (surface.ParentContainer.Kind == "CADModel")
                            {
                                portsFound.Add(surface);
                            }
                        }
                        else
                        {
                            VisitPort(surface);
                        }
                    }
                }
            }
        }
    }   // end class


    public class CadDatumTraversal
    {
        public List<CyPhy.CADDatum> datumFound { get; set; }
        private List<string> visitedItems;
        public bool ReverseMap { get; set; }
        private string CadModelID;
        private string DatumKind;

        public CadDatumTraversal(CyPhy.CADDatum datum, string cadmodelID)
        {
            datumFound = new List<CyPhy.CADDatum>();
            visitedItems = new List<string>();
            ReverseMap = false;
            CadModelID = cadmodelID;
            DatumKind = datum.Kind;

            VisitCadDatum(datum);
        }

        private void VisitCadDatum(CyPhy.CADDatum datum_1)
        {
            if (!visitedItems.Contains(datum_1.ID))
            {
                visitedItems.Add(datum_1.ID);

                List<CyPhy.CADDatum> caddatum_list = new List<CyPhy.CADDatum>();
                foreach (CyPhy.PortComposition conn in datum_1.SrcConnections.PortCompositionCollection)
                {
                    caddatum_list.Add(conn.SrcEnds.CADDatum);
                }

                foreach (CyPhy.PortComposition conn in datum_1.DstConnections.PortCompositionCollection)
                {
                    caddatum_list.Add(conn.DstEnds.CADDatum);
                }

                if (datum_1.Kind == "Surface")
                {
                    foreach (CyPhy.SurfaceReverseMap conn in (datum_1 as CyPhy.Surface).SrcConnections.SurfaceReverseMapCollection)
                    {
                        ReverseMap = true;
                        caddatum_list.Add(conn.SrcEnds.Surface);
                    }

                    foreach (CyPhy.SurfaceReverseMap conn in (datum_1 as CyPhy.Surface).DstConnections.SurfaceReverseMapCollection)
                    {
                        ReverseMap = true;
                        caddatum_list.Add(conn.DstEnds.Surface);
                    }
                }

                foreach (CyPhy.CADDatum datum_2 in caddatum_list)
                {
                    if (datum_2.ParentContainer.ID == CadModelID)
                    {
                        if (datum_2.Kind == DatumKind)
                        {
                            datumFound.Add(datum_2);
                        }
                    }
                }
            }
        }
    }

    /// <summary>
    /// Searches the originating connector of a connector interface
    /// </summary>
    public class OriginatingConnectorTraversal
    {
        public CyPhy.Connector result;
        private List<string> visitedItems;

        public OriginatingConnectorTraversal(CyPhy.Connector conn)
        {
            List<CyPhy.ConnectorComposition> visited = new List<CyPhy.ConnectorComposition>();
            CyPhy.Connector current = conn;
            CyPhy.Connector last = conn;
            while (current != null)
            {
                List<CyPhy.ConnectorComposition> nextroute = new List<CyPhy.ConnectorComposition>();
                var connections = current.SrcConnections.ConnectorCompositionCollection.Union(conn.DstConnections.ConnectorCompositionCollection);
                foreach (var connection in connections)
                {
                    if (visited.Contains(connection))
                    {
                        continue;
                    }

                    visited.Add(connection);
                    if (connection.ParentContainer != current.ParentContainer)
                    {
                        nextroute.Add(connection);
                    }
                }
                if (nextroute.Count == 0)
                {
                    result = current;
                    return;
                }
                last = current;
                current = nextroute.First().SrcEnds.Connector;
                if (current.Guid == last.Guid || current == null)
                {
                    current = nextroute.First().DstEnds.Connector;
                }
                nextroute.Clear();
            }
        }
    }

    public class GeometryTraversal
    {
        public List<MgaFCO> geometryFound { get; set; }
        private List<string> visitedConns;
        private List<string> geometryTypes;

        public GeometryTraversal()
        {
            geometryFound = new List<MgaFCO>();
            visitedConns = new List<string>();
            geometryTypes = new List<string>();
            geometryTypes.Add("Cylinder");
            geometryTypes.Add("Sphere");
            geometryTypes.Add("Circle");
            geometryTypes.Add("Extrusion");
            geometryTypes.Add("Polygon");
            geometryTypes.Add("CustomGeometry");
            geometryTypes.Add("Face");
        }

        public void TraverseGeometry(MgaFCO start)
        {
            FindGeometry(start);

            if (!geometryFound.Any())
            {
                if (IsAGeometry(start.MetaBase.Name))
                {
                    geometryFound.Add(start);
                }
            }
        }

        private void FindGeometry(MgaFCO start)
        {
            var conns = start
                .PartOfConns
                .Cast<IMgaConnPoint>()
                .Select(x => x.Owner)
                .Cast<IMgaSimpleConnection>()
                .Where(x => (x.MetaBase.Name == "Geometry2Forwarder" || x.MetaBase.Name == "Sphere2Forwarder" || x.MetaBase.Name == "Cylinder2Forwarder") && !visitedConns.Contains(x.ID));

            if (conns.Any())
            {
                foreach (var connection in conns)
                {
                    MgaFCO end = null;
                    if (connection.Src == start)
                    {
                        end = connection.Dst;
                    }
                    else if (connection.Dst == start)
                    {
                        end = connection.Src;
                    }

                    visitedConns.Add(connection.ID);

                    FindGeometry(end);
                }
            }
            else
            {
                // already found a geometry
                if (IsAGeometry(start.MetaBase.Name))
                {
                    geometryFound.Add(start);
                }
            }
        }

        private bool IsAGeometry(string type)
        {
            return (geometryTypes.Contains(type));
        }
    }

    public class GeneralTraversal
    {
        public string ParentType { get; set; }
        public List<string> Visited;
        public List<MgaFCO> Found { get; set; }
        public List<string> ConnectionTypes { get; set; }

        public GeneralTraversal(List<string> connectionTypes, string parentType)
        {
            ParentType = parentType;
            Visited = new List<string>();
            Found = new List<MgaFCO>();
            ConnectionTypes = connectionTypes;
        }

        public void FindConnectedFCOs(MgaFCO start)
        {
            var conns = start
                .PartOfConns
                .Cast<IMgaConnPoint>()
                .Select(x => x.Owner)
                .Cast<IMgaSimpleConnection>()
                .Where(x => ConnectionTypes.Contains(x.MetaBase.Name) == true && !Visited.Contains(x.ID));

            if (conns.Any())
            {
                foreach (var conn in conns)
                {
                    MgaFCO end = null;
                    if (conn.Src == start)
                    {
                        end = conn.Dst;
                    }
                    else if (conn.Dst == start)
                    {
                        end = conn.Src;
                    }

                    Visited.Add(conn.ID);
                    FindConnectedFCOs(end);
                }
            }
            else
            {
                if (start.ParentModel.MetaBase.Name == ParentType)
                {
                    Found.Add(start);
                }
            }
        }
    }
}   // end namespace
