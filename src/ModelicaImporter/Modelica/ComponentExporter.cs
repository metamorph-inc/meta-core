// -----------------------------------------------------------------------
// <copyright file="ComponentExporter.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace ModelicaImporter.Modelica
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using Newtonsoft.Json;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class ExportedComponent
    {
        [JsonProperty(PropertyName="exportedComponentClass")]
        public string ExportedComponentClass { get; set; }

        [JsonProperty(PropertyName = "components")]
        public List<Component> Components { get; set; }

        public ExportedComponent()
        {
            this.Components = new List<Component>();
        }
    }
}
