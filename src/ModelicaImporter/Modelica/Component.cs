using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace ModelicaImporter.Modelica
{
    public class Component
    {

        [JsonProperty(PropertyName="fullName")]
        public string FullName { get; set; }

        [JsonProperty(PropertyName = "parameters")]
        public List<Parameter> Parameters { get; set; }

        [JsonProperty(PropertyName = "imports")]
        public List<Import> Imports { get; set; }

        [JsonProperty(PropertyName = "connectors")]
        public List<Connector> Connectors { get; set; }

        [JsonProperty(PropertyName = "extends")]
        public List<Extend> Extends { get; set; }

        [JsonProperty(PropertyName = "redeclare_parameters")]
        public List<RedeclareParameter> RedeclareParameters { get; set; }

        [JsonProperty(PropertyName = "packages")]
        public List<Package> Packages { get; set; }

        public Component()
        {
            this.Parameters = new List<Parameter>();
            this.Imports = new List<Import>();
            this.Connectors = new List<Connector>();
            this.Extends = new List<Extend>();
            this.RedeclareParameters = new List<RedeclareParameter>();
            this.Packages = new List<Package>();
        }

    }
}
