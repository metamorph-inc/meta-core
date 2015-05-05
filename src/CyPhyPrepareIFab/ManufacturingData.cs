
namespace CyPhyPrepareIFab
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.IO;
    using System.Reflection;
    using System.Xml.Serialization;
    using System.Text.RegularExpressions;
    using System.Diagnostics;

    public class ComponentManufacturingData
    {
        public string Name  { get; set; }
        public string AVMID { get; set; }
        public string RevID { get; set; }
        public string VerID { get; set; }
        public string GUID  { get; set; }
        public string FileFormat { get; set; }
        public string Location   { get; set; }
        public string ID         { get; set; }
        public string NewLocation { get; set; }

        public Dictionary<string, string> ManufacturingParamters { get; set; }

        public ComponentManufacturingData()
        {
            ManufacturingParamters = new Dictionary<string,string>();
        }

        public void UpdateManufacturingSpec()
        {
            // [1] check xml exists
            // [2] deserialize xml file to a manufacturingDetail object
            // [3] update manufacturingDetail object with ManufacturingParamters objects
            // [4] serialize manufacturingDetail object
            char[] charsToTrim = {'{', '}'};
            string fileName = this.GUID.Trim(charsToTrim) + ".xml";
            if (!File.Exists(Location))
            {
                Trace.TraceError("Error: " + Location + " not found!");
                return;
            }

            if (!Directory.Exists(this.NewLocation))
                Directory.CreateDirectory(this.NewLocation);

            if (ManufacturingParamters.Count() > 0)             // update xml
            {
                XmlSerializer mfgDetails_Serializer = new XmlSerializer(typeof(Part.manufacturingDetails));
                Part.manufacturingDetails mfdetail = Part.manufacturingDetails.Deserialize(Location);
                StringWriter sw = new StringWriter();
                sw.NewLine = "";
                mfgDetails_Serializer.Serialize(sw, mfdetail);
                string xmldatastring = sw.ToString();

                try
                {
                    foreach (KeyValuePair<string, string> item in ManufacturingParamters)
                    {
                        xmldatastring = ReplaceParameters(item.Key, item.Value, xmldatastring);
                    }

                    var strreader = new StringReader(xmldatastring);
                    Part.manufacturingDetails mfdetails_revised = (Part.manufacturingDetails)mfgDetails_Serializer.Deserialize(strreader);

                    if (!Directory.Exists(this.NewLocation))
                        Directory.CreateDirectory(this.NewLocation);
                    StreamWriter myWriter = new StreamWriter(Path.Combine(this.NewLocation, fileName));
                    mfgDetails_Serializer.Serialize(myWriter, mfdetails_revised);


                    Trace.TraceInformation("Info: Updated manufacturing xml with manufacturing parameter from model. " + this.NewLocation + "\\" + fileName);
                }
                catch (System.InvalidOperationException ex)
                {
                    Console.WriteLine("Exception occured: {0}", ex.ToString());
                }

            }
            else                                   // copy the xml
            {
                File.Copy(Location, Path.Combine(this.NewLocation, fileName), true);
                Trace.TraceInformation("Info: Copied manufacturing xml. " + this.NewLocation + "\\" + fileName);
            }
        }

        private string ReplaceParameters(string parameter, string value, string xmlstring)
        {
            Trace.TraceInformation("Info: RegexMatch for [" + parameter + "]");
            string pattern = @">[0-9,a-z,A-Z.-_]*</\b(" + parameter + @")\b\>";
            string replacement = ">" + value + "</" + parameter + ">";
            Regex rgx = new Regex(pattern);
            string modified = rgx.Replace(xmlstring, replacement);

            MatchCollection mc = rgx.Matches(xmlstring);
            foreach (Match m in mc)
            {
                Trace.TraceInformation("Info: found = " + m.Value);
            }

            return modified;
        }
    }



    public class DesignManufactureManifest
    {
        public class ComponentManufactureManifestData
        {
            public string id { get; set; }
            public string ManufacturingModel { get; set; }

            public ComponentManufactureManifestData(string guid, 
                                                    string file)
            {
                id = guid;
                ManufacturingModel = file;
            }
        }

        public string DesignID { get; set; }
        public string Name { get; set; }
        public List<ComponentManufactureManifestData> ComponentManufactureList;

        public DesignManufactureManifest()
        {
            ComponentManufactureList = new List<ComponentManufactureManifestData>();
        }
    }

}   // end namespace
