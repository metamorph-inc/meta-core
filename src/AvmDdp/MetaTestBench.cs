using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AVM.DDP
{
    public class MetaTestBench
    {
        public abstract class ValueWithUnit
        {
            public string Name { get; set; }
            public string Unit { get; set; }
            public string Value { get; set; }
        }

        public class Parameter : ValueWithUnit
        {
        }

        public class Metric : ValueWithUnit
        {
            public Requirement Requirement { get; set; }
        }

        public class Requirement
        {
            public string Name { get; set; }
            public string SourceRepository { get; set; }
            public string ID { get; set; }
            public string Text { get; set; }
        }

        public string Name { get; set; }

        public List<Metric> Metrics { get; set; }
        public List<Parameter> Parameters { get; set; }
        public List<Requirement> Requirements { get; set; }

        public MetaTestBench()
        {
            Metrics = new List<Metric>();
            Parameters = new List<Parameter>();
            Requirements = new List<Requirement>();
        }
    }
}
