using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace CyPhy2Modelica_v2
{
    [ComVisible(true)]
    [ProgId("ISIS.META.CyPhy2Modelica_v2PreConfiguration")]
    [Guid("D93F6F50-8E52-451D-AC4D-2C7AA1522154")]
    public class CyPhy2Modelica_v2PreConfiguration : CyPhyGUIs.IInterpreterPreConfiguration
    {
        public string ProjectDirectory { get; set; }
    }
}
