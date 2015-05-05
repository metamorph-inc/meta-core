// -----------------------------------------------------------------------
// <copyright file="VFOntologyTree.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace ISIS.VehicleForge
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class VFOntologyTreeNode
    {
        public string _id { get; set; }
        public string master_id { get; set; }
        public List<VFOntologyTreeNode> children { get; set; }
        public string label { get; set; }
    }
}
