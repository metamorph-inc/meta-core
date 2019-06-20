namespace CyPhyCADAnalysis
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
        public string Name { get; set; }
        public string AVMID { get; set; }
        public string RevID { get; set; }
        public string VerID { get; set; }
        public string GUID { get; set; }
        public string FileFormat { get; set; }
        public string Location { get; set; }
        public string ID { get; set; }
        public string NewLocation { get; set; }

        public Dictionary<string, string> ManufacturingParamters { get; set; }

        public ComponentManufacturingData()
        {
            ManufacturingParamters = new Dictionary<string, string>();
        }

    }



    public class DesignManufactureManifest
    {
        public class ComponentManufactureManifestData
        {
            public string InstanceGUID { get; set; }
            public string id { get; set; }
            public string ManufacturingModel { get; set; }

            public ComponentManufactureManifestData(string guid,
                                                    string mfid,
                                                    string file)
            {
                InstanceGUID = guid;
                id = mfid;
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
