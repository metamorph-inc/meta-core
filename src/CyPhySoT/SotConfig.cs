// -----------------------------------------------------------------------
// <copyright file="SotConfig.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace CyPhySoT
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class SotConfig
    {
        public string ProjectFileName { get; set; }
        public string OriginalProjectFileName { get; set; }
        public string SoTID { get; set; }
        public bool MultiJobRun { get; set; }

        public SotConfig()
        {
            MultiJobRun = false;
        }

    }
}
