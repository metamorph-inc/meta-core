using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;

namespace CyPhyMetaLink
{
    public class MetaLinkConfiguration
    {
        public string ConfigProFile { get; set; }
        public MetaLinkConfiguration()
        {
            ConfigProFile = "";
        }
        public void SerializeToJson(string filepath)
        {
            string reportContent = Newtonsoft.Json.JsonConvert.SerializeObject(this, Newtonsoft.Json.Formatting.Indented);
            using (StreamWriter writer = new StreamWriter(filepath))
            {
                writer.WriteLine(reportContent);
            }
        }

        public static MetaLinkConfiguration Create(string filepath)
        {
            if (File.Exists(filepath))
            {
                string sjson = "{}";
                using (StreamReader reader = new StreamReader(filepath))
                {
                    sjson = reader.ReadToEnd();
                    return JsonConvert.DeserializeObject<MetaLinkConfiguration>(sjson);
                }
            }
            else
            {
                return new MetaLinkConfiguration();
            }
        }
    }
}
