using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CyPhyMetaLink
{
    /// <summary>
    /// Class contains data about the synced entity (design/component)
    /// </summary>
    public class SyncedComponentData
    {
        public enum EditType
        {
            Empty,
            Component,
            Design
        }
        public EditType Type { get; set; }
        public string WorkingDir { get; set; } // Directory where the cad files, configuration, etc stored for this session
        public string AuxDir { get; set; } // Auxiliary dir. for additional CAD files
    }
}
