using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;


namespace CyPhy2Modelica_v2.Modelica
{
    /// <summary>
    /// Corresponds to a Component in the project tree in CyPhyML. If all conditions are fulfilled it will be generated
    /// as a Modelica model under CyPhy/Components/%RootFolderName%/%AbsPath% in the generated package.
    /// Components that are used in the design will be instantiated in the ComponentAssembly.
    /// </summary>
    /// <seealso cref="ComponentInstance"/>
    public class Component : ModelBase<CyPhy.ComponentType>
    {
        /// <summary>
        /// Populates the CyPhyParameters based on all Parameters in the Component.
        /// </summary>
        /// <param name="impl">Wrapped CyPhy Component</param>
        public Component(CyPhy.ComponentType impl)
            : base(impl)
        {
            this.Parameters = new SortedSet<UnitParameter>();
            this.CyPhyParameters = new SortedSet<UnitParameter>();
            //foreach (var cyPhyParameter in impl.Children.ParameterCollection)
            //{
            //    var parameter_mo = new UnitParameter()
            //    {
            //        Name = CodeGenerator.ParameterPrefix + cyPhyParameter.Name,
            //        Value = CodeGenerator.GetParameterModelicaValue(cyPhyParameter),
            //        Modifier = CodeGenerator.GetParameterModifier(cyPhyParameter)
            //    };

            //    this.CyPhyParameters.Add(parameter_mo);
            //}
        }
        /// <summary>
        /// Modelica model URI to extend from.
        /// </summary>
        /// <example>
        /// Given a component named "Mass" and with Extend "Modelica.Translational.Components.Mass"
        /// and a CyPhyParameter named "mass".
        /// <para>
        /// <code>
        /// <pre>
        /// model Mass
        ///   extends Modelica.Translational.Components.Mass(m=__CyPhy__mass);
        ///   ...
        /// end model;
        /// </pre>
        /// </code>
        /// </para>
        /// </example>
        /// 
        /// <seealso cref="CyPhyParameters"/>
        public string Extends { get; set; }
        /// <summary>
        /// ModelicaURI
        /// </summary>
        public string FullName { get; set; }
        /// <summary>
        /// Parameters that will be overwritten in the extended model.
        /// </summary>
        public SortedSet<UnitParameter> Parameters { get; set; }
        /// <summary>
        /// Parameters that will be defined in the Modelica model.
        /// Their name will be appended with __CyPhy__ to avoid conflicts with parameters in the extended model.
        /// </summary>
        public SortedSet<UnitParameter> CyPhyParameters { get; set; }
        
        /// <summary>
        /// Calls ToStringIndented with default indentLevel(=0)
        /// </summary>
        /// <returns>The components representation in Modelica code.</returns>
        public override string ToString()
        {
            return ToStringIndented();
        }

        /// <summary>
        /// Tranforms the Component into indented Modelica model code.
        /// </summary>
        /// <param name="indentLevel">Which level of indentation the model should be written at. (Number of spaces = <paramref name="indentLevel"> * 2.)</param>
        /// <returns>The components representation in Modelica code indented <paramref name="indentLevel">.</returns>
        public string ToStringIndented(int indentLevel = 0)
        {
            var sb = new StringBuilder();
            var package = this.FullName.Substring(0, this.FullName.Length - this.Name.Length).TrimEnd('.');
            if (indentLevel == 0)
            {
                sb.AppendLine(string.Format("within {0};", package));
            }
            sb.Append(' ', indentLevel * 2);
            sb.AppendLine(string.Format("model {0}", this.Name));

            foreach (var cyPhyParameter in this.CyPhyParameters)
            {
                sb.Append(' ', indentLevel * 2);
                sb.AppendLine(string.Format("  parameter {0} {1}{2}={3};", 
                    cyPhyParameter.Value.ClassName, cyPhyParameter.Name, cyPhyParameter.Modifier, cyPhyParameter.Value.Value));
            }

            sb.Append(' ', indentLevel * 2);
            sb.Append(string.Format("  extends {0}", this.Extends));
            int n = Parameters.Count;
            int i = 1;
            foreach (var parameter in this.Parameters)
            {
                if (i == 1)
                {
                    sb.AppendLine("(");
                }
                sb.Append(' ', indentLevel * 2);
                sb.Append(string.Format("    {0}={1}", parameter.Name, parameter.Value.Value));
                if (i == n)
                {
                    sb.Append(')');
                }
                else
                {
                    sb.AppendLine(",");
                }
                i++;
            }
            sb.AppendLine(";");

            sb.Append(' ', indentLevel * 2);
            sb.AppendLine(string.Format("end {0};", Name));
            return sb.ToString();
        }
    }
}
