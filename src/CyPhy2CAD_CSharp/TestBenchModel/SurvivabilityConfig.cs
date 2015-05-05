using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;
using System.IO;

namespace CyPhy2CAD_CSharp.TestBenchModel
{
    namespace Survivability
    {
        public class FileLocation
        {
            public enum FileTypeEnum
            {
                CADLibrary,
                STEPFile,
                MetricsFile,
                WorkingDirectory,
                ResultsFile
            }

            public string name { get; set; }
            public string directory { get; set; }

            [JsonConverter(typeof(StringEnumConverter))]
            public FileTypeEnum type { get; set; }
        }

        public enum ReferencePlaneTypeEnum
        {
            None,
            Waterline,
            Ground
        }

        public class SurvivabilityConfigBase
        {
            public List<Survivability.FileLocation> fileLocations { get; set; }

            public SurvivabilityConfigBase()
            {
                fileLocations = new List<Survivability.FileLocation>();
            }

            public void SerializeToJson(string outputDir, 
                                        string filename)
            {
                string reportContent = Newtonsoft.Json.JsonConvert.SerializeObject(this,
                                                                                   Newtonsoft.Json.Formatting.Indented);
                using (StreamWriter writer = new StreamWriter(Path.Combine(outputDir, filename)))
                {
                    writer.WriteLine(reportContent);
                }
            }
        }


    }   // end namespace
}   // end namespace
