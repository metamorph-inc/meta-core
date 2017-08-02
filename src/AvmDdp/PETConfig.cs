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
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public string units;
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public object value;
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
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public string units;
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
        public Dictionary<string, SubProblem> subProblems;
        public List<Recorder> recorders;

        public class Recorder
        {
            public string type;
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public string filename;
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public string run_id;
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public bool? include_id;
        }

        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public string GeneratedConfigurationModel;
        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public List<string> SelectedConfigurations;
        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public string PETName;
        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public string MgaFilename;
    }

    public class SubProblem
    {
        public class ProblemInput
        {
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public string[] outerSource; //Source of this ProblemInput from outside the subproblem (i.e. one level up), relative to the enclosing problem
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public string[] innerSource; //Source of this ProblemInput from inside the subproblem (i.e. from the driver), relative to this problem
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public string value;
            public bool pass_by_obj;
        }

        public Dictionary<string, ProblemInput> problemInputs; //Map of problem inputs (by name) to their sources
        public Dictionary<string, string[]> problemOutputs; //Map of problem outputs (by name) to their source (relative to this subproblem)

        public Dictionary<string, PETConfig.Component> components;
        public Dictionary<string, PETConfig.Driver> drivers;
        public Dictionary<string, SubProblem> subProblems;
    }
}