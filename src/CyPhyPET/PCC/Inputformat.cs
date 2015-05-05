using System.Collections.Generic;
using CyPhyGUIs;

namespace CyPhyPET.PCC
{

    //public class InputDistribution
    //{
    //    public string ID { get; set; }
    //    public string Name { get; set; }
    //    public List<string> TestBenchParameterNames { get; set; }
    //    public string Distribution { get; set; }
    //    public double Param1 { get; set; }
    //    public double Param2 { get; set; }
    //    public double Param3 { get; set; }
    //    public double Param4 { get; set; }

    //    public InputDistribution()
    //    {
    //        TestBenchParameterNames = new List<string>();
    //    }
    //}

    public class StochasticInputs
    {
        public List<PCCInputDistribution> InputDistributions { get; set; }

        public StochasticInputs()
        {
            InputDistributions = new List<PCCInputDistribution>();
        }
    }

    public class Limits
    {
        public double Min { get; set; }
        public double Max { get; set; }
        public string op { get; set; }
        public string minrange { get; set; }
        public string maxrange { get; set; }
    }

    public class PCCMetric
    {
        public string ID { get; set; }
        public string Name { get; set; }
        public string TestBenchMetricName { get; set; }
        public double PCC_Calc { get; set; }
        public double PCC_Spec { get; set; }
        public Limits Limits { get; set; }

        public PCCMetric()
        {
            Limits = new Limits();
        }
    }

    public class PCCInputArguments
    {
        [System.Xml.Serialization.XmlArray]
        [System.Xml.Serialization.XmlArrayItem(ElementName = "OutputID")]
        public List<string> OutputIDs { get; set; }
        [System.Xml.Serialization.XmlArray]
        [System.Xml.Serialization.XmlArrayItem(ElementName = "InputID")]
        public List<string> InputIDs { get; set; }
        public StochasticInputs StochasticInputs { get; set; }
        public List<PCCMetric> PCCMetrics { get; set; }
        public List<int> Methods { get; set; }
    }

    public class Part
    {
        public string ModelConfigFileName { get; set; }
        public string ToolConfigFileName { get; set; }
    }

    public class Configuration
    {
        public string Feasible { get; set; }
        public string ID { get; set; }
        public string Name { get; set; }
        public List<Part> Parts { get; set; }
        public PCCInputArguments PCCInputArguments { get; set; }
    }

    public class Configurations
    {
        public Configuration Configuration { get; set; }
    }

    public class RootObject
    {
        public Configurations Configurations { get; set; }
    }
}