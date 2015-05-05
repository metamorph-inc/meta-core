// -----------------------------------------------------------------------
// <copyright file="CyPhyReliabilityAnalysisSettings.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace CyPhyReliabilityAnalysis
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
    [ProgId("ISIS.META.CyPhyReliabilityAnalysisSettings")]
    [Guid("93451D74-B099-41B4-BD9B-D14F4861F798")]
    public class CyPhyReliabilityAnalysisSettings : CyPhyGUIs.IInterpreterConfiguration
    {
        public const string ConfigFilename = "CyPhyReliabilityAnalysisSettings_config.xml";
        public string Material = "Steel";
        public int Size = 1;
        public double AngularThickness = 10.16;
        public double GussetThickness = 3.81;

    }
}
