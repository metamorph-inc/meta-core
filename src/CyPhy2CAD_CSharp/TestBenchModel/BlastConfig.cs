using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;

namespace CyPhy2CAD_CSharp.TestBenchModel
{
    namespace Survivability
    {
        public class BlastConfig : SurvivabilityConfigBase
        {
            public class Analysis
            {
                public string suiteName { get; set; }
                public string ID { get; set; }
                public int tier { get; set; }
                public double duration_seconds { get; set; }
                public string upDirection { get; set; }
                public double numDisplayTimeSteps { get; set; }

                public Analysis()
                {
                    upDirection = "+Y";
                }
            }

            public class Shape
            {
                public enum ShapeTypeEnum
                {
                    Cylinder,
                    Sphere
                }

                public double radius_meters;
                public double height_meters;

                [JsonConverter(typeof(StringEnumConverter))]
                public ShapeTypeEnum type;
            }

            public class ChargeLocation
            {
                public double x_meters;
                public double y_meters;
                public double z_meters;
            }

            public enum BlastThreatTypeEnum
            {
                HemisphericalSurface,
                SphericalFreeAir
            }

            public class BlastThreat
            {
                public double chargeSize_kgOfTNT;
                public ChargeLocation chargeLocation;
                public double burialDepth_meters;

                [JsonConverter(typeof(StringEnumConverter))]
                public BlastThreatTypeEnum type;
                public Shape shape;
            }

            public bool ShouldSerializeblastThreat()
            {
                // don't serialize the ballisticThreats property if its count == 0
                return (blastThreat != null);
            }

            // members
            public Analysis analysis { get; set; }
            public BlastThreat blastThreat { get; set; }

            public BlastConfig()
            {
                analysis = new Analysis();
            }
        }

    }   // end namespace
}   // end namespace
