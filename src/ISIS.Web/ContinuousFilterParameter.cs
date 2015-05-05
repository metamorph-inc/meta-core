using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace ISIS.Web
{
    public class ContinuousFilterParameter : ComponentLibraryFilterParameter
    {
        [JsonProperty(PropertyName = "minimum")]
        public string MinValue { get; set; }
        [JsonProperty(PropertyName = "maximum")]
        public string MaxValue { get; set; }
        [JsonProperty(PropertyName = "unit")]
        public string Unit { get; set; }

        public ContinuousFilterParameter(string name)
        {
            this.Name = name;
            this.filterType = "range";
            this.MinValue = "-Infinity";
            this.MaxValue = "Infinity";
            this.Unit = null;
        }
    }
}
