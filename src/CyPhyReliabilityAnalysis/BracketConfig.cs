// -----------------------------------------------------------------------
// <copyright file="BracketConfig.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace CyPhyReliabilityAnalysis
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using Newtonsoft.Json;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class BracketConfig
    {
        public BracketConfig(CyPhyReliabilityAnalysisSettings settings)
        {
            this.Material = settings.Material;
            this.Size = settings.Size;
            this.AngularThickness = settings.AngularThickness;
            this.GussetThickness = settings.GussetThickness;
        }

        public string Material { get; set; }
        public int Size { get; set; }
        [JsonProperty("Angular Thickness")]
        public double AngularThickness { get; set; }
        [JsonProperty("Gusset Thickness")]
        public double GussetThickness { get; set; }
    }
}
