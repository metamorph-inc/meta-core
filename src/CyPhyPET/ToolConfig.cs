// -----------------------------------------------------------------------
// <copyright file="ToolConfig.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace CyPhyPET
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class ToolConfig
    {
        public string tool_name { get; set; }
        public string dymola_home { get; set; }
        public string om_home { get; set; }


        public ToolConfig()
        {
            dymola_home = "";
        }
    }
}
