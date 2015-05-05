using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CyPhyGUIs
{
    public class PCCInputDistribution
    {
        public string ID { get; set; }
        public string Name { get; set; }
        public List<string> TestBenchParameterNames { get; set; }
        public string Distribution { get; set; }
        public double Param1 { get; set; }
        public double Param2 { get; set; }
        public double Param3 { get; set; }
        public double Param4 { get; set; }

        public PCCInputDistribution()
        {
            TestBenchParameterNames = new List<string>();
        }
    }
}
