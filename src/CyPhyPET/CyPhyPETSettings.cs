// -----------------------------------------------------------------------
// <copyright file="CyPhyPETSettings.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace CyPhyPET
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Runtime.InteropServices;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    [Serializable]
    [ComVisible(true)]
    [ProgId("ISIS.META.CyPhyPETSettings")]
    [Guid("93554F4A-EEB4-4E93-B532-23CD5EC06250")]
    public class CyPhyPETSettings : CyPhyGUIs.IInterpreterConfiguration
    {
        public const string ConfigFilename = "CyPhyPETSettings_config.xml";
    }
}
