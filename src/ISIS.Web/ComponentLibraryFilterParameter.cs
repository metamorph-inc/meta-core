using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace ISIS.Web
{
    // this class is currently geared toward a VF search, but should be "abstracted" if possible
    public class ComponentLibraryFilterParameter
    {
        [JsonIgnore]
        public string Name { get; set; }
        [JsonIgnore]
        protected string filterType { get; set; }

        [JsonProperty(PropertyName = "type")]
        public string Type
        {
            get
            {
                return filterType;
            }
        }
    }
}
