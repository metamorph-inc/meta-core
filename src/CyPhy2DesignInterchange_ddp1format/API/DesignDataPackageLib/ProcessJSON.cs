using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace AVM
{
    internal enum Operation
    {
        SERIALIZE,
        DESERIALIZE
    };

    internal struct ParseSettings
    {
        public Boolean b_ManipulateMode;
        public Boolean b_FixIDs;
        public Operation operation;
    };

    internal class ProcessJSON
    {
        class KeySortComp : IComparer<String>
        {
            public int Compare(String x, String y)
            {
                // $type must always be first
                if (x == "$type")
                    return -1;
                if (y == "$type")
                    return 1;

                // If neither one was $type, then $id comes first
                if (x == "$id")
                    return -1;
                if (y == "$id")
                    return 1;

                // The keys "Associations" and "Connectors" must always be last.
                if (x == "Associations")
                    return 1;
                if (y == "Associations")
                    return -1;
                if (x == "Connectors")
                    return 1;
                if (y == "Connectors")
                    return -1;

                // No other orders matter. So we'll sort alphabetically. Ignore case.
                return String.Compare(x, y, true);
            }
        };

        public static String CrawlJSONDictionary(string json, ParseSettings ps, Dictionary<String, String> d_OldIDToNew = null)
        {
            JToken jt = JToken.Parse(json);

            // JObjects are collections of key-value pairs
            if (jt is JObject)
            {
                JObject jo = jt as JObject;

                // ID field manipulation -- remove $id field
                if (ps.b_FixIDs)
                {
                    if (ps.operation == Operation.DESERIALIZE)
                    {
                        // If we're deserializing, we want to take the "id" field and move it to "$id" for the JSON parser.
                        // However, keep the original "id" field so that users of the library can access the field.
                        JToken jt_val;
                        if (jo.TryGetValue("id", out jt_val))
                        {
                            jo["$id"] = jt_val;
                        }
                    }
                    else if (ps.operation == Operation.SERIALIZE)
                    {
                        // If we're serializing, we want to find "id" (accessible id field) and use it to replace "$id" (inaccessible id field).
                        // However, since "$ref" objects point to "$id", we must track the map so we can adjust the $refs.
                        JToken jt_val;
                        if (jo.TryGetValue("id", out jt_val))
                        {
                            d_OldIDToNew.Add(jo["$id"].ToString(), jt_val.ToString());
                        }
                        jo.Remove("$id");
                    }
                }

                // If we are deserializing, check for missing $type objects
                if (ps.operation == Operation.DESERIALIZE)
                {
                    #region Ensure $type field exists
                    JToken jt_DollarTypeVal;
                    JToken jt_CapitalTypeVal;
                    if (!jo.TryGetValue("$type", out jt_DollarTypeVal) && jo.TryGetValue("Type", out jt_CapitalTypeVal))
                    {
                        jo["$type"] = (jt_CapitalTypeVal.ToString()).Split(':').First();
                    }
                    #endregion
                }

                foreach (KeyValuePair<string, JToken> kvp in jo)
                {
                    JToken jt_val = kvp.Value;
                    if (jt_val is JValue)
                    {
                        #region Manipulate $type field
                        // If we're serializing, strip out the DLL name
                        if (ps.b_ManipulateMode && kvp.Key == "$type")
                        {
                            String val = kvp.Value.ToString();
                            if (ps.operation == Operation.SERIALIZE)
                            {
                                val = val.Substring(0, val.IndexOf(", "));
                            }
                            else if (ps.operation == Operation.DESERIALIZE)
                            {
                                if (val.IndexOf(", DesignDataPackageLib") == -1)
                                {
                                    StringBuilder sb = new StringBuilder(val);
                                    sb.Append(", DesignDataPackageLib");
                                    val = sb.ToString();
                                }
                            }
                            jo[kvp.Key] = val;
                        }
                        #endregion
                    }
                    else
                    {
                        jo[kvp.Key] = JToken.Parse(CrawlJSONDictionary(kvp.Value.ToString(), ps, d_OldIDToNew));
                    }
                }

                // If we are deserializing, make sure $type is first among the properties
                if (ps.operation == Operation.DESERIALIZE)
                {
                    #region Reorder properties
                    SortedDictionary<String, JToken> sd_jo = new SortedDictionary<string, JToken>(jo, new KeySortComp());
                    jo.RemoveAll();
                    foreach (KeyValuePair<String, JToken> kvp in sd_jo)
                    {
                        jo[kvp.Key] = kvp.Value;
                    }
                    #endregion
                }
            }
            else if (jt is JArray)
            {
                JArray jarray = jt as JArray;
                for (int i = 0; i < jarray.Count; i++)
                {
                    JToken jt_iter = jarray[i];

                    if (jt_iter is JObject ||
                        jt_iter is JArray)
                    {
                        jarray[i] = JToken.Parse(CrawlJSONDictionary(jt_iter.ToString(), ps, d_OldIDToNew));
                    }
                }
            }

            return jt.ToString();
        }

        public static string RedirectRefs(string json, Dictionary<String, String> d_OldIDToNew)
        {
            // Wherever we see a $ref value, check the ID and replace if there's a match.
            JToken jt = JToken.Parse(json);

            // JObjects are collections of key-value pairs
            if (jt is JObject)
            {
                JObject jo = jt as JObject;
                foreach (KeyValuePair<string, JToken> kvp in jo)
                {
                    JToken jt_val = kvp.Value;
                    if (jt_val is JValue)
                    {
                        if (kvp.Key == "$ref")
                        {
                            String s_ReferredID = kvp.Value.ToString();
                            String s_NewID;
                            if (d_OldIDToNew.TryGetValue(s_ReferredID, out s_NewID))
                            {
                                jo[kvp.Key] = s_NewID;
                            }
                        }
                    }
                    else
                    {
                        jo[kvp.Key] = JToken.Parse(RedirectRefs(kvp.Value.ToString(), d_OldIDToNew));
                    }
                }
            }
            else if (jt is JArray)
            {
                JArray jarray = jt as JArray;
                for (int i = 0; i < jarray.Count; i++)
                {
                    JToken jt_iter = jarray[i];

                    if (jt_iter is JObject ||
                        jt_iter is JArray)
                    {
                        jarray[i] = JToken.Parse(RedirectRefs(jt_iter.ToString(), d_OldIDToNew));
                    }
                }
            }

            return jt.ToString();
        }
    }
}
