using System.Collections.Generic;
using Newtonsoft.Json;

namespace AVM.DDP
{
    public class PETConfig
    {
        public class Parameter
        {
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public string[] source;
        }
        public class Constraint
        {
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public string[] source;
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public double? RangeMin;
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public double? RangeMax;
        }
        public class Component
        {
            public Dictionary<string, Parameter> parameters;
            public Dictionary<string, Parameter> unknowns;
            public Dictionary<string, string> details;
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public string type;
        }
        public class DesignVariable
        {
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public string type;
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public double? RangeMin;
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public double? RangeMax;
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public List<object> items;
        }
        public class Driver
        {
            public string type;
            public Dictionary<string, DesignVariable> designVariables;
            public Dictionary<string, Parameter> objectives;
            public Dictionary<string, Constraint> constraints;
            public Dictionary<string, Parameter> intermediateVariables;
            public Dictionary<string, object> details;
        }
        public Dictionary<string, Component> components;
        public Dictionary<string, Driver> drivers;

        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public string GeneratedConfigurationModel;
        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public List<string> SelectedConfigurations;
        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public string PETName;
        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public string MgaFilename;
    }
}