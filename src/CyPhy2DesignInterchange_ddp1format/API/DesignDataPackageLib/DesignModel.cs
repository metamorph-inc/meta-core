using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using Newtonsoft.Json.Serialization;
using AVM;

namespace AVM.META.Design
{
    public partial class DesignModel
    {
        private class SortedContractResolver : DefaultContractResolver
        {
            protected override System.Collections.Generic.IList<JsonProperty> CreateProperties(System.Type type, MemberSerialization memberSerialization)
            {
                return base.CreateProperties(type, memberSerialization).OrderByDescending(p => p.PropertyName).ToList();
            }
        }

        /// <param name="fileName">The path to the output file</param>
        public string SerializeToFile(string fileName)
        {
            string jsonString = this.Serialize();
            using (System.IO.StreamWriter sw = new System.IO.StreamWriter(fileName)) { sw.WriteLine(jsonString); }
            return jsonString;
        }

        public string Serialize()
        {
            this.DDPSpecVersion = "1.2.1";

            string json = JsonConvert.SerializeObject(
                this,
                Formatting.Indented,
                jss_defaults
            );

            // TODO: Catch exceptions for serialization function
            // TODO: Check for valid filename and path
            
            Dictionary<String, String> d_OldIDToNew = new Dictionary<string, string>();
            json = ProcessJSON.CrawlJSONDictionary(json,
                                       new ParseSettings() { b_FixIDs = true, b_ManipulateMode = true, operation = Operation.SERIALIZE },
                                       d_OldIDToNew);
            json = ProcessJSON.RedirectRefs(json, d_OldIDToNew);

            return json;
        }

        public static DesignModel Deserialize(string json)
        {
            string json_processed = ProcessJSON.CrawlJSONDictionary(json,
                                                                    new ParseSettings() { b_FixIDs = true, b_ManipulateMode = true, operation = Operation.DESERIALIZE });

            DesignModel rtn = JsonConvert.DeserializeObject(json_processed, 
                                                            typeof(DesignModel), 
                                                            jss_defaults) 
                                            as DesignModel;
            return rtn;
        }

        public static DesignModel DeserializeFromFile(string path)
        {
            System.IO.StreamReader sr = new System.IO.StreamReader(path);
            String json = sr.ReadToEnd();
            return Deserialize(json);
        }

        private static JsonSerializerSettings jss_defaults = new JsonSerializerSettings()
        {
            PreserveReferencesHandling = PreserveReferencesHandling.Objects,
            TypeNameHandling = TypeNameHandling.Objects,
            NullValueHandling = NullValueHandling.Ignore,
            ContractResolver = new SortedContractResolver(),
            Converters = new List<Newtonsoft.Json.JsonConverter>()
            { 
                new Newtonsoft.Json.Converters.StringEnumConverter()
            }
        };
    }        
}
