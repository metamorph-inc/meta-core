using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2Modelica_v2.Modelica
{
    public class ComponentAssemblyInstance : ModelBase<CyPhy.ComponentAssembly>
    {
        public ComponentAssemblyInstance(CyPhy.ComponentAssembly impl)
            : base(impl)
        {
            Parameters = new SortedSet<UnitParameter>();
        }
        public ComponentAssembly InstanceOf { get; set; }
        public SortedSet<UnitParameter> Parameters { get; set; }
        public string Annotation
        {
            get
            {
                return string.Format("annotation(Placement(transformation(origin={{{0},-{1}}}, extent={{{{-{2},-{3}}},{{{2},{3}}}}})))",
                        CanvasX / CodeGenerator.ScaleFactor, CanvasY / CodeGenerator.ScaleFactor, CodeGenerator.ComponentIconSize, CodeGenerator.ComponentIconSize);
            }
        }

        public override string ToString()
        {
            var sb = new StringBuilder();
            sb.Append(Name);
            int n = Parameters.Count;
            int i = 1;
            foreach (var parameter in Parameters)
            {
                if (i == 1)
                {
                    sb.AppendLine("(");
                }

                if (i != n)
                {
                    sb.AppendLine(string.Format("    {0}={1},", parameter.Name, parameter.Value.Value));
                }
                else if (i == n)
                {
                    if (n == 1)
                    {
                        sb.Append(string.Format("{0}={1})", parameter.Name, parameter.Value.Value));
                    }
                    else
                    {
                        sb.Append(string.Format("    {0}={1})", parameter.Name, parameter.Value.Value));
                    }
                }
                i++;
            }

            return sb.ToString();
        }
    }
}
