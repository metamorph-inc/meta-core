namespace CyPhyCar
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
    [ProgId("ISIS.META.CyPhyCarSettings")]
    [Guid("DD1D90BE-CD1A-41BE-AF25-B325D067B33B")]
    public class CyPhyCarSettings : CyPhyGUIs.IInterpreterConfiguration
    {
        public const string ConfigFilename = "CyPhyCar_config.xml";

        public CyPhyCarSettings()
        {

        }
    }
}
