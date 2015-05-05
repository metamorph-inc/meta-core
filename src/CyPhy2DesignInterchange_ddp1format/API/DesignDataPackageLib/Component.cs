using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using AVM;
using System.IO;
using System.Xml.Serialization;
using System.Reflection;

namespace AVM
{
    public partial class Component
    {
        static JsonSerializerSettings jss_defaults = new JsonSerializerSettings()
        {
            PreserveReferencesHandling = PreserveReferencesHandling.Objects,
            TypeNameHandling = TypeNameHandling.Objects,
            NullValueHandling = NullValueHandling.Ignore,
            Converters = new List<Newtonsoft.Json.JsonConverter>()
            { 
                new Newtonsoft.Json.Converters.StringEnumConverter()
            }
        };

        /// <param name="fileName">The path to the output file</param>
        public string SerializeToFile(string fileName)
        {
            // TODO: Catch exceptions for serialization function
            // TODO: Check for valid filename and path
            
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

            Dictionary<String, String> d_OldIDToNew = new Dictionary<string, string>();
            json = ProcessJSON.CrawlJSONDictionary( json, 
                                        new ParseSettings(){b_FixIDs = true, operation = Operation.SERIALIZE, b_ManipulateMode = true},
                                        d_OldIDToNew);
            json = ProcessJSON.RedirectRefs(json, d_OldIDToNew);

            return json;
        }

        public static Component Deserialize(string json)
        {
            string json_ToImport = json;

            // Auto-migrate Structural Interfaces
            Dictionary<String, String> d_ToAutoMigrate = new Dictionary<string, string>()
            {
                { "\"AVM.META.StructuralInterface\"", "\"AVM.StructuralInterface\"" },
                { "\"AVM.META.StructuralInterfaceDatum\"", "\"AVM.StructuralInterfaceDatum\"" },
            };

            foreach (KeyValuePair<String, String> kvp in d_ToAutoMigrate)
                json_ToImport = json_ToImport.Replace(kvp.Key, kvp.Value);

            json_ToImport = ProcessJSON.CrawlJSONDictionary(json_ToImport, 
                                                new ParseSettings() { b_FixIDs = true, operation = Operation.DESERIALIZE, b_ManipulateMode = true });
            Component rtn = JsonConvert.DeserializeObject(json_ToImport, typeof(AVM.Component), jss_defaults) as AVM.Component;
            return rtn;
        }

        public static Component DeserializeFromFile(string path)
        {
            System.IO.StreamReader sr = new System.IO.StreamReader(path);
            String json = sr.ReadToEnd();
            sr.Close();

            return Deserialize(json);
        }
    }
}
