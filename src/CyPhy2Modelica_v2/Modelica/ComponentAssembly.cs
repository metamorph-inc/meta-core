using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2Modelica_v2.Modelica
{
    public class ComponentAssembly : ModelBase<CyPhy.ComponentAssembly>
    {
        public ComponentAssembly(CyPhy.ComponentAssembly impl)
            : base(impl)
        {
            ComponentAssemblyInstances = new SortedSet<ComponentAssemblyInstance>();
            ComponentInstances = new SortedSet<ComponentInstance>();
            Connectors = new SortedSet<Connector>();
            Connections = new SortedSet<Connection>();
            Parameters = new SortedSet<UnitParameter>();
            Metrics = new SortedSet<Metric>();
            Environments = new SortedSet<Environment>();
            HasInnerCAs = false;
        }
        public string FullName { get; set; }
        public SortedSet<ComponentAssemblyInstance> ComponentAssemblyInstances { get; set; }
        public SortedSet<ComponentInstance> ComponentInstances { get; set; }
        public SortedSet<Connector> Connectors { get; set; }
        public SortedSet<Connection> Connections { get; set; }
        public SortedSet<Environment> Environments { get; set; }
        public SortedSet<UnitParameter> Parameters { get; set; }
        public SortedSet<Metric> Metrics { get; set; }
        public int CanvasXMax { get; set; }
        public int CanvasYMax { get; set; }
        public ComponentAssembly ParentComponentAssembly { get; set; }
        public bool HasInnerCAs { get; set; }
        public bool ConstrainedBys { get; set; }

        public override string ToString()
        {
            var sb = new StringBuilder();

            sb.AppendLine(string.Format("model {0}", Name));
            sb.AppendLine("  //Parameters");
            foreach (var parameter in Parameters)
            {
                sb.AppendLine(string.Format("  parameter {0} {1}{2}={3};", parameter.Value.ClassName, parameter.Name, parameter.Modifier, parameter.Value.Value));
            }
            sb.AppendLine();
            sb.AppendLine("  //Metrics");
            foreach (var metric in Metrics)
            {
                //sb.AppendLine(string.Format("  Real {0} \"{2}\";", metric.Name, metric.Description));
                sb.AppendLine("  // Not supported yet!");
            }
            sb.AppendLine("  //Environments");
            int cnt = 0;
            foreach (var environment in Environments)
            {
                var annotation = string.Format("annotation(Placement(transformation(origin={{{0},40}}, extent={{{{-20,-20}},{{20,20}}}})))",
                        20 + 80 * cnt);
                sb.AppendLine(string.Format("  outer {0} {1} {2};", environment.URI, environment.Name, annotation));
                cnt++;
            }
            sb.AppendLine();
            sb.AppendLine("  //ComponentAssemblies");
            foreach (var componentAssemblyInstance in ComponentAssemblyInstances)
            {
                sb.AppendLine(string.Format("  {0} {1} {2};",
                    componentAssemblyInstance.InstanceOf.FullName, componentAssemblyInstance.ToString(), componentAssemblyInstance.Annotation));
            }
            sb.AppendLine();
            sb.AppendLine("  //Components");
            foreach (var componentInstance in ComponentInstances)
            {
                if (this.ConstrainedBys == true)
                {
                    sb.AppendLine("  replaceable");
                    sb.AppendLine(string.Format("    {0}", componentInstance.InstanceOf.FullName));
                    sb.AppendLine(string.Format("    {0} constrainedby", componentInstance.ToString()));
                    sb.AppendLine(string.Format("    {0}", componentInstance.InstanceOf.Extends));
                    sb.AppendLine(string.Format("    {0};", componentInstance.Annotation));
                }
                else
                {
                    sb.AppendLine(string.Format("  {0} {1} {2};", componentInstance.InstanceOf.FullName, componentInstance.ToString(), componentInstance.Annotation));
                }
            }
            sb.AppendLine();
            sb.AppendLine("  //Connectors");

            foreach (var connector in Connectors)
            {
                sb.Append(string.Format("  {0} {1}", connector.ModelicaURI, connector.Name));
                int n = connector.Parameters.Count;
                int i = 1;
                foreach (var parameter in connector.Parameters)
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

                sb.AppendLine(string.Format(" {0};", connector.Annotation));
            }

            sb.AppendLine("equation");
            foreach (var connection in Connections)
            {
                sb.AppendLine(connection.ToString());
            }

            this.Icon(sb);

            sb.AppendLine(string.Format("end {0};", Name));

            return sb.ToString();
        }

        private void Icon(StringBuilder sb)
        {
            int xMin = 0;
            int xMax = CanvasXMax / CodeGenerator.ScaleFactor;
            int yMin = -CanvasYMax / CodeGenerator.ScaleFactor;
            int yMax = 0;

            int xMiddle = (xMax + xMin) / 2;
            int yMiddle = (yMax + yMin) / 2;

            int width = Math.Min(yMax - yMin, xMax - xMin);
            int height = Math.Min(yMax - yMin, xMax - xMin);


            sb.AppendLine();
            sb.AppendLine(" // Annotations");
            sb.AppendLine("annotation (Icon(coordinateSystem(preserveAspectRatio=true,  extent={{" + xMin + "," + yMin + "},{" + xMax + "," + yMax + "}}),");
            sb.AppendLine("graphics={");
            // main rectangle - background
            sb.AppendLine("Rectangle(extent={{" + xMin + "," + yMax + "},{" + xMax + "," + yMin + "}},");
            sb.AppendLine("lineColor={0,0,0},");
            sb.AppendLine("fillColor={250,250,255},");
            sb.AppendLine("fillPattern=FillPattern.Solid),");
            // triangle corner
            sb.AppendLine("Polygon(points={{-40,-40},{-40,0},{0,0},{-40,-40}},");
            sb.AppendLine("smooth=Smooth.None,");
            sb.AppendLine("fillColor={0,127,0},");
            sb.AppendLine("fillPattern=FillPattern.Solid,");
            sb.AppendLine("pattern=LinePattern.None,");
            sb.AppendLine("origin={" + xMin + "," + (yMax + 40) + "},");
            sb.AppendLine("rotation=0,");
            sb.AppendLine("lineColor={0,127,0}),");
            // triangle corner
            sb.AppendLine("Polygon(points={{-40,-40},{-40,0},{0,0},{-40,-40}},");
            sb.AppendLine("smooth=Smooth.None,");
            sb.AppendLine("fillColor={0,127,0},");
            sb.AppendLine("fillPattern=FillPattern.Solid,");
            sb.AppendLine("pattern=LinePattern.None,");
            sb.AppendLine("origin={" + xMax + "," + (yMin - 40) + "},");
            sb.AppendLine("rotation=180,");
            sb.AppendLine("lineColor={0,127,0}),");
            // green rectangle for name
            sb.AppendLine("Rectangle(");
            sb.AppendLine("extent={{" + (xMin - 40) + "," + (yMax + 50) + "},{" + (xMax + 40) + "," + (yMax + 90) + "}},");
            sb.AppendLine("lineColor={0,127,0},");
            sb.AppendLine("fillColor={0,127,0},");
            sb.AppendLine("fillPattern=FillPattern.Solid),");
            // name text in green rectangle
            sb.AppendLine("Text(");
            sb.AppendLine("extent={{" + (xMin - 40) + "," + (yMax + 50) + "},{" + (xMax + 40) + "," + (yMax + 90) + "}},");
            sb.AppendLine("lineColor={255,255,255},");
            sb.AppendLine("textString=\"%name\",");
            sb.AppendLine("fontName=\"Comic Sans MS\",");
            sb.AppendLine("textStyle={TextStyle.Bold})}),");

            sb.AppendLine("Diagram(coordinateSystem(");
            sb.AppendLine("preserveAspectRatio=true,  extent={{" + xMin + "," + yMin + "},{" + xMax + "," + yMax + "}})));");

        }
    }

}
