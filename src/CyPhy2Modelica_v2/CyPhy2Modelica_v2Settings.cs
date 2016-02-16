// -----------------------------------------------------------------------
// <copyright file="CyPhy2ModelicaSettings.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace CyPhy2Modelica_v2
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
    [ProgId("ISIS.META.CyPhy2Modelica_v2Settings")]
    [Guid("4B678D98-3921-4969-85B8-C9DE47D15835")]
    public class CyPhy2Modelica_v2Settings : CyPhyGUIs.IInterpreterConfiguration
    {
        public const string ConfigFilename = "CyPhy2Modelica_v2_config.xml";
        //public List<ModelicaLibrary> Libraries { get; set; }

        public List<string> IncludeDirectoryPath { get; set; }
        public List<string> NonCheckedIncludeDirPaths { get; set; }

        public bool CheckWithDymola { get; set; }
        public bool CheckWithOpenModelica { get; set; }
        public bool GenerateConstrainedBys { get; set; }

        public CyPhy2Modelica_v2Settings()
        {
            this.NonCheckedIncludeDirPaths = new List<string>();
            this.IncludeDirectoryPath = new List<string>();
        }

        [CyPhyGUIs.WorkflowConfigItem]
        public string simulationTargetTool { get; set; }
    }
}
