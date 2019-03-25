using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using GmeCommon = ISIS.GME.Common;
using GME.CSharp;

namespace CyPhy2CAD_CSharp.DataRep
{
    public enum DatumType
    {
        CSYS,
        Point,
        Surface,
        Axis
    }

    public class Datum
    {

        public string DatumName { get; set; }
        public DatumType Type { get; set; }
        public string Orientation { get; set; }
        public string Alignment { get; set; }
        public string ComponentID { get; set; }                     // InstanceGUID
        public bool Guide { get; set; }
        public string DatumID { get; set; }                         // UDM ID
        public GeometryMarkerRep Marker { get; set; }

        public Datum(string name, string type, string compid, bool guide)
        {
            DatumName = name;
            if (type == "CoordinateSystem")
                Type = DatumType.CSYS;
            else if (type == "Point")
                Type = DatumType.Point;
            else if (type == "Surface")
                Type = DatumType.Surface;
            else if (type == "Axis")
                Type = DatumType.Axis;

            ComponentID = compid;
            DatumID = UtilityHelpers.MakeUdmID();

            Guide = guide;
        }

        public Datum(CyPhy.CADDatum datum,
                     string compid, bool guide)
        {
            DatumName = datum.Attributes.DatumName;
            if (DatumName == "")
            {
                DatumName = datum.Name;
            }
            if (datum.Kind == "CoordinateSystem")
                Type = DatumType.CSYS;
            else if (datum.Kind == "Point")
                Type = DatumType.Point;
            else if (datum.Kind == "Surface")
                Type = DatumType.Surface;
            else if (datum.Kind == "Axis")
                Type = DatumType.Axis;

            ComponentID = compid;
            DatumID = datum.ID;
            Guide = guide;
            if (!String.IsNullOrEmpty(datum.Attributes.GeometricMarker))
                Marker = new GeometryMarkerRep(datum.Attributes.GeometricMarker);
        }
    }


}
