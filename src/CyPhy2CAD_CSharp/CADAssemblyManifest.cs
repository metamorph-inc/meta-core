using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace CyPhy2CAD_CSharp
{
    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    /// 
    public class CADAssemblyManifest
    {
        public class ManifestComponent
        {
            [JsonIgnore]
        	public string RelativePrefix { get; set; }

            [JsonIgnore]
		    public string ModelURI  { get; set; }

            [JsonIgnore]
		    public string ComponentCadFolder  { get; set; }

            [JsonIgnore]
            public string InstanceGUID { get; set; }

            public string avmid { get; set; }
            public int revision { get; set; }
            public int version { get; set; }
        }


        public Dictionary<string, ManifestComponent> CADAssemblyManifestList { get; set; }

        public CADAssemblyManifest()
        {
            CADAssemblyManifestList = new Dictionary<string, ManifestComponent>();
        }
    }
}
