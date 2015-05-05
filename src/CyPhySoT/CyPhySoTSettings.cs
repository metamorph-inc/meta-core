using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace CyPhySoT
{
    [Serializable]
    [ComVisible(true)]
    [ProgId("ISIS.META.CyPhySoTSettings")]
    [Guid("B08E33CA-6FAF-4528-B3A0-23318DC1F093")]
    public class CyPhySoTSettings : CyPhyGUIs.IInterpreterConfiguration
    {
        public const string ConfigFilename = "CyPhySoTSettings_config.xml";
        public string OriginalProjectFileName { get; set; }
    }
}
