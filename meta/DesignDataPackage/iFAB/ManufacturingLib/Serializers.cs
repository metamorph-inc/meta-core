using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Serialization;
using System.IO;
using System.Xml;

public static class Serializer
{
    public static String Serialize(Object obj)
    {
        XmlSerializer xs = new XmlSerializer(obj.GetType());
        StringWriter sw = new StringWriter();
        sw.NewLine = "";

        xs.Serialize(sw, obj);
        return sw.ToString();
    }
}

namespace Assembly
{        
    public partial class assemblyDetails
    {
        static assemblyDetails Deserialize(string path)
        {
            XmlSerializer xs = new XmlSerializer(typeof(assemblyDetails));
            StreamReader sr = new StreamReader(path);

            assemblyDetails ad = (assemblyDetails)xs.Deserialize(sr);
            sr.Close();

            return ad;
        }

        public static assemblyDetails DeserializeFromString(string xml) {
            XmlSerializer xs = new XmlSerializer(typeof(assemblyDetails));
            StringReader sr = new StringReader(xml);

            assemblyDetails ad = (assemblyDetails)xs.Deserialize(sr);
            sr.Close();

            return ad;
        }


        String Serialize()
        {
            XmlSerializer xs = new XmlSerializer(typeof(assemblyDetails));
            StringWriter sw = new StringWriter();

            xs.Serialize(sw, this);
            return sw.ToString();
        }

        String SerializeToFile(String path)
        {
            String xml = this.Serialize();

            StreamWriter sw = new StreamWriter(path);
            sw.Write(xml);
            sw.Close();

            return xml;
        }
    }
}

namespace Part
{
    public partial class manufacturingDetails
    {
        public static manufacturingDetails Deserialize(string path)
        {
            XmlSerializer xs = new XmlSerializer(typeof(manufacturingDetails));
            StreamReader sr = new StreamReader(path);

            manufacturingDetails md = (manufacturingDetails)xs.Deserialize(sr);
            sr.Close();

            return md;
        }

        String Serialize()
        {
            XmlSerializer xs = new XmlSerializer(typeof(manufacturingDetails));
            StringWriter sw = new StringWriter();

            xs.Serialize(sw, this);
            return sw.ToString();
        }

        String SerializeToFile(String path)
        {
            String xml = this.Serialize();

            StreamWriter sw = new StreamWriter(path);
            sw.Write(xml);
            sw.Close();

            return xml;
        }
    }
}
