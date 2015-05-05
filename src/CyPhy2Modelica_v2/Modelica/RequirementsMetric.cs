using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json.Linq;

namespace CyPhy2Modelica_v2.Modelica
{
    class RequirementMetric
    {
        public RequirementMetric(string name, string successCriterion, double threshold, double objective)
        {
            this.MetricName = name;
            this.SuccessCriterion = successCriterion;
            if (threshold < objective)
            {
                this.BiggerIsBetter = true;
                this.Min = threshold;
                this.Max = objective;
            }
            else
            {
                this.BiggerIsBetter = false;
                this.Min = objective;
                this.Max = threshold;
            }
        }

        public string MetricName { get; set; }
        public string SuccessCriterion { get; set; }
        public double Min { get; set; }
        public double Max { get; set; }
        public bool BiggerIsBetter {get;set;}
    }
}
