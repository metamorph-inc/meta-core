using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Xml.Schema;
using Xunit;

namespace CADTeamTest
{
    public class AsmXmlChecker
    {
        private XDocument doc;
        public AsmXmlChecker(string path)
        {
            XmlSchemaSet schemas = new XmlSchemaSet();
            schemas.Add("", @"..\..\..\..\src\CyPhy2CAD_CSharp\AssemblyInterface.xsd");

            doc = XDocument.Load(path);
            string msg = "";
            doc.Validate(schemas, (o, e) =>
            {
                // TODO: line number would be nice, but it seems e.Exception.LineNumber doesn't work
                msg += e.Message + Environment.NewLine;
            });
            Assert.True(msg == "", msg);
        }

        public AsmXmlChecker CheckNodes(string[] nodes, bool exists = true)
        {
            foreach (string n in nodes)
            {
                var d = doc.Root.Descendants(n);
                if (exists)
                    Assert.True(d.Count() > 0, "CadAssembly.xml is missing node " + n);
                else
                    Assert.False(d.Count() > 0, "CadAssembly.xml contains node " + n);
            }
            return this;
        }

        public AsmXmlChecker CheckComponentAttribute(string id, string attr, string val)
        {
            var d = doc.Descendants("CADComponent").Where(c => c.Attribute("ComponentID").Value == id);
            Assert.NotEmpty(d);
            Assert.True(d.First().Attribute(attr).Value == val);
            return this;
        }

    }
}
