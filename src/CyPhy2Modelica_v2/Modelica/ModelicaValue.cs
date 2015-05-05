using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CyPhy2Modelica_v2.Modelica
{
    public class ModelicaValue
    {
        public ModelicaValue(string value)
        {
            if (string.IsNullOrWhiteSpace(value))
            {
                this.Value = "0";
            }
            else if (value.StartsWith("."))
            {
                this.Value = "0" + value;
            }
            else
            {
                this.Value = value;
            }
        }

        public string ClassName { get; set; }
        public string Value { get; set; }
        public string QUDTIdentifier { get; set; }
    }
}
