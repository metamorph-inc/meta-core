using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace JobManager.Jenkins.StatusInfo
{
    /// <summary>
    /// Jenkins worker nodes detailed status through VF API
    /// </summary>
    public class Node
    {
        public string status { get; set; }
        public int available { get; set; }
        public int busy { get; set; }
        public string name { get; set; }
        public string description { get; set; }
    }

    /// <summary>
    /// VF response structure for /testbench/info.json
    /// </summary>
    public class StatusInfo
    {
        /// <summary>
        /// Possible values: OK, BUSY, DOWN.
        /// Note: we intentionally do not use enumerations.
        /// </summary>
        public string status { get; set; }
        public List<Node> nodes { get; set; }
        public int totalBusy { get; set; }
        public int totalAvailable { get; set; }
        public int highPriorityRemaining { get; set; }
    }

    /*
    public enum StatusEnum
    {
        OK,
        BUSY,
        DOWN
    }
    */
}
