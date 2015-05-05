using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace ModelicaImporter.Modelica
{
    public class Parameter
    {

        [JsonProperty(PropertyName = "fullName")]
        public string FullName { get; set; }

        [JsonProperty(PropertyName = "modifiers")]
        public Dictionary<string, string> Modifiers { get; set; }

        [JsonProperty(PropertyName = "name")]
        public string Name { get; set; }

        [JsonProperty(PropertyName = "value")]
        public object Value { get; set; }

        public object DefaultValue { get; set; }

        [JsonProperty(PropertyName = "description")]
        public string Description { get; set; }

        [JsonProperty(PropertyName = "isPublic")]
        public bool isPublic { get; set; }

        [JsonProperty(PropertyName = "dimension")]
        public int Dimension { get; set; }

        public IEnumerable<KeyValuePair<string, string>> Modifications { get { return this.Modifiers.Where(x => x.Key != "modifications"); } }


        public Parameter()
        {
            this.Modifiers = new Dictionary<string, string>();
            this.DefaultValue = string.Empty;
        }
    }
}
