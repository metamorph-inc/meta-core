using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace ISIS.Web
{
    public class DiscreteFilterParameter : ComponentLibraryFilterParameter
    {
        [JsonProperty(PropertyName = "values")]
        public List<string> Values { get; set; }

        public DiscreteFilterParameter(string name)
        {
            this.Name = name;
            this.Values = null;
            this.filterType = "string";
        }
    }
}
