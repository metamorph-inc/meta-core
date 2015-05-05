using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace ModelicaImporter.Modelica
{
    public class Package
    {

        [JsonProperty(PropertyName = "name")]
        public string Name { get; set; }

        [JsonProperty(PropertyName = "value")]
        public string Value { get; set; }

        public string DefaultValue { get; set; }

        public Package()
        {
            this.DefaultValue = null;
        }
    }
}
