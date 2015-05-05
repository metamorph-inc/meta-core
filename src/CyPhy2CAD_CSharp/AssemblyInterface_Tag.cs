using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CAD
{
    public partial class AssembliesType
    {
        [System.Xml.Serialization.XmlAttributeAttribute(Namespace = "http://www.w3.org/2001/XMLSchema-instance")]
        public virtual string noNamespaceSchemaLocation
        {
            get { return "AssemblyInterface.xsd"; }
            set { }
        }
    }
}
