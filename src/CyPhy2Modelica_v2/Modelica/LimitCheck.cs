using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CyPhy2Modelica_v2.Modelica
{
    public class LimitCheck
    {
        public string ParentKind { get; set; }
        public string VariableFullPath { get; set; }
        public string VariableName { get; set; }
        public string LimitFullPath { get; set; }
        public string LimitName { get; set; }
        public bool? LimitExceeded { get; set; }
        public Dictionary<string, object> Attributes { get; set; }
        public string Type { get; set; }
        public double Value { get; set; }
        public string ModelicaRecordName { get; set; }
        public LimitCheck()
        {
            Attributes = new Dictionary<string, object>();
        }

    }
}
