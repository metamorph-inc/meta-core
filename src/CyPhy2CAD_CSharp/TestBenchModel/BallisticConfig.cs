using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;

namespace CyPhy2CAD_CSharp.TestBenchModel
{
    namespace Survivability
    {
        public class BallisticConfig : SurvivabilityConfigBase
        {
            public class Analysis
            {
                public string suiteName { get; set; }
                public string ID { get; set; }
                public int tier { get; set; }
            }

            public class CriticalComponent
            {
                public enum CriticalityTypeEnum
                {
                    Crew,
                    Mobility,
                    Magazine
                }

                public string componentID { get; set; }

                [JsonConverter(typeof(StringEnumConverter))]
                public CriticalityTypeEnum type { get; set; }
            }

            public class BallisticThreat
            {
                public string name { get; set; }
                public string materialRef { get; set; }
                public double speed_metersPerSec { get; set; }
                public double length_meters { get; set; }
                public double diameter_meters { get; set; }

                public enum BallisticThreatTypeEnum
                {
                    ShapedChargeJet,
                    Ballistic
                }

                public enum ChargeQualityEnum
                {
                    Low,
                    Medium,
                    High
                }

                [JsonConverter(typeof(StringEnumConverter))]
                public BallisticThreatTypeEnum type { get; set; }

                [JsonConverter(typeof(StringEnumConverter))]
                public ChargeQualityEnum chargeQuality { get; set; }
                public double standoff_meters { get; set; }
            };

            public bool ShouldSerializeballisticThreats()
            {
                // don't serialize the ballisticThreats property if its count == 0
                return (ballisticThreats.Count > 0);
            }


            // members
            public Analysis analysis { get; set; }
            //public List<SurvivabilityConfig.FileLocation> fileLocations { get; set; }
            public List<CriticalComponent> criticalComponents { get; set; }
            public List<BallisticThreat> ballisticThreats { get; set; }


            // constructor
            public BallisticConfig() :
                   base()
            {
                analysis = new Analysis();
                //fileLocations = new List<SurvivabilityConfig.FileLocation>();
                criticalComponents = new List<CriticalComponent>();
                ballisticThreats = new List<BallisticThreat>();
            }

            /*
            public void SerializeToJson(string outputDir)
            {
                string reportContent = Newtonsoft.Json.JsonConvert.SerializeObject(this, 
                                                                                   Newtonsoft.Json.Formatting.Indented);
                using (StreamWriter writer = new StreamWriter(Path.Combine(outputDir, "BallisticConfig.json")))
                {
                    writer.WriteLine(reportContent);
                }
            }
            */
        }

    }   // end namespace
}   // end namespace
