using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace CyPhy2CAD_CSharp
{
    public struct CadManifestObject
    {
        public string AvmID;
        public string Version;

        [JsonIgnore]
        public string LocalFolderPath;

        [JsonIgnore]
        public string RemoteMETAProPath;

        //[JsonIgnore]      // META-2009: commented out for this tix
        public string RemoteFolderPath;
    }

    public class CadManifestObjectList
    {
        public List<CadManifestObject> CADAssemblyManifestList { get; set; }

        public CadManifestObjectList()
        {
            CADAssemblyManifestList = new List<CadManifestObject>();
        }
    }

//     Newtonsoft.Json.JsonConvert.SerializeObject(
//./CyPhy2Modelica.Process.cs:                    Newtonsoft.Json.Formatting.Indented);

}
