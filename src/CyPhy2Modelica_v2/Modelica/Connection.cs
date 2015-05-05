using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CyPhy2Modelica_v2.Modelica
{
    public class Connection : IComparable<Connection>
    {
        public Connection(string srcName, string dstName)
        {
            // TODO: Should these be empty strings or just left as null? see usage in ModelicaConnection.ToString()
            SrcConnectorName = srcName;
            DstConnectorName = dstName;

            SrcInstanceName = "";
            DstInstanceName = "";
        }
        public string SrcInstanceName { get; set; }
        public string SrcConnectorName { get; set; }
        public string DstInstanceName { get; set; }
        public string DstConnectorName { get; set; }
        public string Type { get; set; }
        public string Annotation { 
            get 
            { 
                var thickness = "";
                var color = "";
                if (Type.EndsWith(".FluidPort_a") || Type.EndsWith(".FluidPort_b"))
                {
                    thickness = ",thickness=0.5";
                    color = ",color={0,127,255}"; //Light Blue
                }
                else if (Type.EndsWith(".PositivePin") || Type.EndsWith(".NegativePin") || Type.EndsWith(".Pin"))
                {
                    color = ",color={0,0,255}"; //Blue
                }
                else if (Type.EndsWith(".HeatPort_a") || Type.EndsWith(".HeatPort_b") || Type.EndsWith(".HeatPort"))
                {
                    color = ",color={191,0,0}"; //Red
                }
                else if (Type.EndsWith(".Hydraulic_Port"))
                {
                    color = ",color={255,0,128}"; //Pink
                }
                else if (Type.EndsWith(".FlangeWithBearing"))
                {
                    color = ",color={95,95,95}";
                    thickness = ",thickness=0.5";
                }
                else if (Type.EndsWith(".Frame_a") || Type.EndsWith(".Frame_b") || Type.EndsWith(".Frame"))
                {
                    color = ",color={175,175,175}";
                    thickness = ",thickness=0.5";
                }
                
                return string.Format("annotation(Line(points = {{{{0.0,0.0}},{{0.1,0.1}}}}{0}{1}))", color, thickness);
            } 
        }
        public string JointName { 
            get
            {
                string srcFullName;
                if (string.IsNullOrWhiteSpace(this.SrcInstanceName))
                {
                    srcFullName = this.SrcConnectorName;
                }
                else
                {
                    srcFullName = this.SrcInstanceName + "." + this.SrcConnectorName;
                }

                string dstFullName;
                if (string.IsNullOrWhiteSpace(this.DstInstanceName))
                {
                    dstFullName = this.DstConnectorName;
                }
                else
                {
                    dstFullName = this.DstInstanceName + "." + this.DstConnectorName;
                }

                if (srcFullName.CompareTo(dstFullName) > 0)
                {
                    return dstFullName + ", " + srcFullName;
                }
                else
                {
                    return srcFullName + ", " + dstFullName;
                }
            }
        }

        public override string ToString()
        {
                return string.Format("  connect({0}) {1};", this.JointName, this.Annotation);
        }

        public int CompareTo(Connection other)
        {
            return this.JointName.CompareTo(other.JointName);
        }
    }

}
