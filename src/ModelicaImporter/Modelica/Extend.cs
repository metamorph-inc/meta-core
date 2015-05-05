using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace ModelicaImporter.Modelica
{
    public class Extend
    {
        [JsonProperty(PropertyName="fullName")]
        public string FullName { get; set; }

        [JsonProperty(PropertyName = "modifiers")]
        public Dictionary<string, string> Modifiers { get; set; }

        [JsonProperty(PropertyName = "parameters")]
        public List<Parameter> Parameters { get; set; }

        [JsonProperty(PropertyName = "redeclare_parameters")]
        public List<RedeclareParameter> RedeclareParameters { get; set; }


        public Extend()
        {
            this.Modifiers = new  Dictionary<string,string>();
            this.Parameters = new List<Parameter>();
            this.RedeclareParameters = new List<RedeclareParameter>();
        }
    }
}
