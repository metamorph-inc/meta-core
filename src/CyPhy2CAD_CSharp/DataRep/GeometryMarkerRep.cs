using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;


namespace CyPhy2CAD_CSharp.DataRep
{
    public class GeometryMarkerRep
    {
        public double x;
        public double y;
        public double z;
        public double i;
        public double j;
        public double k;
        public double pi;
        public string ComponentID;

        public GeometryMarkerRep(string marker)
        {
            if (!String.IsNullOrEmpty(marker))
            {
                marker = Regex.Replace(marker, @"\t|\n|\r", " ");
                string[] words = marker.Split(' ');
                foreach (string str in words)
                {
                    string[] field = str.Split(':');
                    if (field.Count() == 2)
                    {
                        string type = field[0];
                        if (type == "x")
                            x = Convert2Double(field[1]);
                        else if (type == "y")
                            y = Convert2Double(field[1]);
                        else if (type == "z")
                            z = Convert2Double(field[1]);
                        else if (type == "i")
                            i = Convert2Double(field[1]);
                        else if (type == "j")
                            j = Convert2Double(field[1]);
                        else if (type == "k")
                            k = Convert2Double(field[1]);
                        else if (type == "pi")
                            pi = Convert2Double(field[1]);
                    }
                }
            }
        }

        private double Convert2Double(string sValue)
        {
            try
            {
                double dValue = Convert.ToDouble(sValue);
                return dValue;
            }
            catch (FormatException)
            {
                return 0;
            }
            catch (OverflowException)
            {
                return 0;
            }
        }

        public CAD.GeometryMarkerType ToCADXml()
        {
            CAD.GeometryMarkerType marker = new CAD.GeometryMarkerType();
            marker._id = UtilityHelpers.MakeUdmID();
            marker.ComponentID = ComponentID;
            marker.x = x;
            marker.y = y;
            marker.z = z;
            marker.i = i;
            marker.j = j;
            marker.k = k;
            marker.pi = pi;
            return marker;
        }
    }
}
