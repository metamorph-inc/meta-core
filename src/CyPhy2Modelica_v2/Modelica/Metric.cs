using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2Modelica_v2.Modelica
{
    public class Metric : IComparable<Metric>
    {
        public string Name { get; set; }
        public string Description { get; set; }
        public bool PostProcessing { get; set; }

        public int CompareTo(Metric other)
        {
            return this.Name.CompareTo(other.Name);
        }
    }
}

