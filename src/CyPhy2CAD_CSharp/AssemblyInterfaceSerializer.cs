using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Serialization;
using System.IO;


namespace CyPhy2CAD_CSharp
{
    /*
    public partial class AssembliesType
    {
        String Serialize()
        {
            // Additional information: Could not load file or assembly 'CyPhy2CAD_CSharp.XmlSerializers, Version=1.0.0.0, Culture=neutral, PublicKeyToken=8adbc89a2d94c2a4' or one of its dependencies. The system cannot find the file specified.
            XmlSerializer xs = new XmlSerializer(typeof(AssembliesType)); // n.b. An exception is expected here. It is caught inside of the .NET framework code
            StringWriter sw = new Utf8StringWriter();       //StringWriter sw = new StringWriter();

            xs.Serialize(sw, this);
            return sw.ToString();
        }

        public void SerializeToFile(String path)
        {
            String xml = this.Serialize();

            using (StreamWriter sw = new StreamWriter(path))
            {
                sw.Write(xml);
            }
        }

        public class Utf8StringWriter : StringWriter
        {
            public override Encoding Encoding
            {
                get { return Encoding.UTF8; }
            }
        }       
    }
    */

    public class MyXmlSerializeBase
    {
        String Serialize()
        {
            // Additional information: Could not load file or assembly 'CyPhy2CAD_CSharp.XmlSerializers, Version=1.0.0.0, Culture=neutral, PublicKeyToken=8adbc89a2d94c2a4' or one of its dependencies. The system cannot find the file specified.
            XmlSerializer xs = new XmlSerializer(this.GetType()); // n.b. An exception is expected here. It is caught inside of the .NET framework code
            StringWriter sw = new Utf8StringWriter();       //StringWriter sw = new StringWriter();

            xs.Serialize(sw, this);
            return sw.ToString();
        }

        public void SerializeToFile(String path)
        {
            String xml = this.Serialize();

            using (StreamWriter sw = new StreamWriter(path))
            {
                sw.Write(xml);
            }
        }

        public class Utf8StringWriter : StringWriter
        {
            public override Encoding Encoding
            {
                get { return Encoding.UTF8; }
            }
        }
    }
}

namespace CAD
{
    public partial class AssembliesType : CyPhy2CAD_CSharp.MyXmlSerializeBase
    {
    }

    public partial class AnalysesType : CyPhy2CAD_CSharp.MyXmlSerializeBase
    {
    }



}


