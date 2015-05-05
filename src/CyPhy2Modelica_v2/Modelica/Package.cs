using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CyPhy2Modelica_v2.Modelica
{
    public class Package : IComparable<Package>
    {
        public Package()
        {
            IndentLevel = 0;
            Children = new SortedSet<Package>();
            Models = new SortedSet<Component>();
        }

        public string Name { get; set; }
        public string FullName { get; set; }
        public SortedSet<Package> Children { get; set; }
        public SortedSet<Component> Models { get; set; }
        public int IndentLevel { get; set; }

        //public override string ToString()
        //{
        //    var sb = new StringBuilder();
        //    sb.Append(' ', IndentLevel * 2);
        //    sb.AppendLine(string.Format("package {0}", Name));
        //    sb.Append(' ', IndentLevel * 2);
        //    sb.AppendLine("extends Modelica.Icons.Package;");
        //    sb.Append(' ', IndentLevel * 2);
        //    sb.AppendLine(string.Format("end {0};", Name));
        //    return sb.ToString();
        //}

        public int CompareTo(Package other)
        {
            return this.Name.CompareTo(other.Name);
        }
    }
}
