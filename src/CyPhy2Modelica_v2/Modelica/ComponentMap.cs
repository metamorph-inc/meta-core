using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CyPhy2Modelica_v2.Modelica
{
    public class ComponentMap
    {
        public string TestBench { get; set; }
        public string VerifTestBench { get; set; }
        public Dictionary<string, ComponentInfo> InstanceMapping { get; set; }
        public Dictionary<string, ComponentInfo> ModelMapping { get; set; }
        public Dictionary<string, ComponentInfo> ExtendsMapping { get; set; }
    }
}
