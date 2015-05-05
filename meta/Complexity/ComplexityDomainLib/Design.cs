using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace Complexity
{
    public partial class Design
    {
        /// <param name="fileName">The path to the output file</param>
        public string SerializeToFile(string fileName)
        {
            string jsonString = this.Serialize();
            using (System.IO.StreamWriter sw = new System.IO.StreamWriter(fileName)) { sw.WriteLine(jsonString); }
            return jsonString;
        }

        public string Serialize()
        {
            string jsonString = JsonConvert.SerializeObject(
                this,
                Formatting.Indented,
                new JsonSerializerSettings
                {
                    PreserveReferencesHandling = PreserveReferencesHandling.Objects,
                    TypeNameHandling = TypeNameHandling.None,
                    NullValueHandling = NullValueHandling.Ignore,
                    // TODO: ...
                }
            );

            // TODO: Catch exceptions for serialization function
            // TODO: Check for valid filename and path

            return jsonString;
        }

        public Dictionary<string,string> SerializeToCSVFormat()
        {
            Dictionary<string, string> d_rtn = new Dictionary<string, string>();

            Dictionary<ComponentInstance, int> d_oneIndexIDs = new Dictionary<ComponentInstance, int>();
            
            List<String> ls_compEntries = new List<String>();
            foreach (ComponentInstance ci in this.ComponentInstances)
            {
                int i_id = d_oneIndexIDs.Count + 1;
                d_oneIndexIDs[ci] = i_id;
                switch (ci.DistributionType)
                {
                    case DistributionTypeEnum.None:
                        ls_compEntries.Add(
                            String.Format("{0},{1},{2},{3}", i_id, ci.AVMID, ci.Name, ci.Complexity));
                        break;
                    default:
                        String s_entry = String.Format("{0},{1},{2},-1,{3}", i_id, ci.AVMID, ci.Name, ci.DistributionType.ToString());
                        foreach (double d in ci.DistributionParameters)
                        {
                            s_entry += String.Format(",{0}", d);
                        }
                        ls_compEntries.Add(s_entry);
                        break;
                }
            }
            String csv_comp = "";
            foreach (String s_entry in ls_compEntries)
            {
                csv_comp += s_entry + "\n";
            }
            d_rtn["Components"] = csv_comp;

            List<String> ls_connEntries = new List<String>();
            foreach (Connection c in this.Connections)
            {
                switch (c.DistributionType)
                {
                    case DistributionTypeEnum.None:
                        ls_connEntries.Add(
                            String.Format("{0},{1},{2}", d_oneIndexIDs[c.src], d_oneIndexIDs[c.dst], c.Complexity));
                        break;
                    default:
                        String s_entry = String.Format("{0},{1},-1,{2}", d_oneIndexIDs[c.src], d_oneIndexIDs[c.dst], c.DistributionType.ToString());
                        foreach (double d in c.DistributionParameters)
                        {
                            s_entry += String.Format(",{0}", d);
                        }
                        ls_connEntries.Add(s_entry);
                        break;
                }
            }
            String csv_conn = "";
            foreach (String s_entry in ls_connEntries)
            {
                csv_conn += s_entry + "\n";
            }
            d_rtn["Connections"] = csv_conn;

            return d_rtn;
        }
    }
}
