using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CyPhy2CAD_CSharp
{
    /// <summary>
    /// Stores Meta-Link related data which needs to be transferred to Creo
    /// </summary>
    public class MetaLinkData
    {
        public class Component
        {
            public String ID { get; set; }
            public String AvmID { get; set; }
            public List<Connector> Connectors = new List<Connector>();
        }
        public class Connector
        {
            public String ID { get; set; }
            public String DisplayName { get; set; }
            public List<Datum> Datums = new List<Datum>();
        }
        public class Datum
        {
            public String ID { get; set; }
            public String DisplayName { get; set; }
        }
        public List<Component> Components = new List<Component>();
    }
}
