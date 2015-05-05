using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CyPhy2Modelica_v2.Modelica
{
    public class LimitDefinition
    {
        public string PathToMatFile { get; set; }

        public List<LimitCheck> LimitChecks { get; set; }

        public LimitDefinition()
        {
            LimitChecks = new List<LimitCheck>();
        }
    }
}
