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
        public string Id; // Id of the component (AVMID for component and assemblyId for design)
        public EditType Type { get; set; }
        public string WorkingDir { get; set; } // Directory where the cad files, configuration, etc stored for this session
        public string AuxDir { get; set; } // Auxiliary dir. for additional CAD files
        public string InstanceId; // ID of the CAD program instance
    }
}
