using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace XSD2CSharp
{
    public static class AvmXmlSerializer
    {
        private static Type[] getAVMClasses()
        {
            return System.Reflection.Assembly.GetExecutingAssembly().GetTypes().Where(t => t.IsClass).Where(t => t.Namespace.StartsWith("avm") && t.FullName != "avm.simulink.Port").ToArray();
        }

        public static void SaveToFile(string fileName, object avmObject)
        {
            System.IO.File.WriteAllText(fileName, Serialize(avmObject));
        }

        public static string Serialize(object avmObject)
        {
            var serializer = new System.Xml.Serialization.XmlSerializer(avmObject.GetType(), getAVMClasses());

            System.IO.StreamReader streamReader = null;
            System.IO.MemoryStream memoryStream = null;
            using (memoryStream = new System.IO.MemoryStream())
            {
                serializer.Serialize(memoryStream, avmObject);
                memoryStream.Seek(0, System.IO.SeekOrigin.Begin);
                using (streamReader = new System.IO.StreamReader(memoryStream))
                {
                    return streamReader.ReadToEnd();
                }
            }
        }

        public static T Deserialize<T>(String xml)
        {
            var sr = new StringReader(xml);

            System.Xml.XmlReaderSettings xmlReaderSettings = new System.Xml.XmlReaderSettings();
            xmlReaderSettings.IgnoreWhitespace = true;

            System.Xml.XmlReader xmlReader = System.Xml.XmlReader.Create(sr, xmlReaderSettings);

            System.Xml.Serialization.XmlSerializer serializer = new System.Xml.Serialization.XmlSerializer(typeof(T), getAVMClasses());

            var imported = (T)serializer.Deserialize(xmlReader);
            return imported;
        }
    }
}
