using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Reflection;
using System.IO;

using GME.MGA;
using avm;

namespace META2AVM_Util
{
    public class UtilFuncs
    {
        /* Copying/Adapting from CyPhyComponentExporter C# Proj*/
        /* Changes While Copying/Adapting: */
        /* Changed private methods to public static methods */
        /* Changed 'cyPhyML' in variables to 'meta' */
        /* Changed usage from getIDAttribute to getMetaAttributeContent */
        /* Changed usage from setIDAttribute to setMetaAttributeContent */
        /* Added 'Meta' in the names of the method for clarity */

        private static PropertyInfo getPropertyInfo(Type type, string propertyName)
        {
            return type.GetProperty(propertyName);
        }

        private static PropertyInfo getPropertyInfo(object object_var, string propertyName)
        {
            return getPropertyInfo(object_var.GetType(), propertyName);
        }

        private static Type getInterfaceType(Type type)
        {
            string typeName = type.UnderlyingSystemType.AssemblyQualifiedName.Replace(".Classes.", ".Interfaces.");
            return Type.GetType(typeName);
        }

        private static Type getInterfaceType(object object_var)
        {
            return getInterfaceType(object_var.GetType());
        }

        //public static string getIDAttribute(object metaObject)
        //{

        //    string id = "";

        //    PropertyInfo metaAttributesPropertyInfo = getPropertyInfo(getInterfaceType(metaObject), "Attributes");
        //    if (metaAttributesPropertyInfo != null)
        //    {
        //        PropertyInfo metaIDPropertyInfo = metaAttributesPropertyInfo.PropertyType.GetProperty("ID");
        //        if (metaIDPropertyInfo != null)
        //        {
        //            id = metaIDPropertyInfo.GetValue(metaAttributesPropertyInfo.GetValue(metaObject, null), null) as string;
        //        }
        //    }

        //    return id;
        //}

        //public static void setIDAttribute(object metaObject, string id)
        //{
        //    PropertyInfo metaAttributesPropertyInfo = getPropertyInfo(getInterfaceType(metaObject), "Attributes");
        //    if (metaAttributesPropertyInfo != null)
        //    {
        //        PropertyInfo metaIDPropertyInfo = metaAttributesPropertyInfo.PropertyType.GetProperty("ID");
        //        if (metaIDPropertyInfo != null)
        //        {
        //            metaIDPropertyInfo.SetValue(metaAttributesPropertyInfo.GetValue(metaObject, null), id, null);
        //        }
        //    }
        //}

        public static bool isValidXMLID(String id)
        {
            string pattern = @"^[a-zA-Z_][\w.-]*$";
            return Regex.IsMatch(id, pattern);
        }

        public static string ensureMetaIDAttribute(object metaObject)
        {
            string id = getMetaAttributeContent(metaObject, "ID"); 

            if (string.IsNullOrWhiteSpace(id)
                || !isValidXMLID(id))
            {
                id = "id-";

                var objType = metaObject.GetType();
                var guidProperty = objType.GetProperty("Guid");
                Guid guid = (Guid)guidProperty.GetValue(metaObject, null);
                id += guid.ToString("D");

                setMetaAttributeContent(metaObject, "ID", id);
            }
            return id;
        }

        public static string ensureMetaIDAttribute()
        {
            string id = "id-";
            id += System.Guid.NewGuid().ToString("D");
            
            return id;
        }


        public static String Safeify(String s_in)
        {
            String rtn = s_in;
            rtn = rtn.Replace("\\", "_");
            rtn = rtn.Replace("/", "_");
            return rtn;
        }

        public static Type[] getAVMClasses()
        {
            return System.Reflection.Assembly.Load("OpenMETA.Interchange").GetTypes().Where(t => t.IsClass).Where(t => t.Namespace.StartsWith("avm") && t.FullName != "avm.simulink.Port").ToArray();
        }
        /* Copy end */

        /* Copying/Adapting from CyPhy2ComponentModel C# Proj*/
        public static void SerializeAvmComponent(avm.Component avmComponent, String s_outFilePath)
        {
            StreamWriter streamWriter = new StreamWriter(s_outFilePath);
            using (streamWriter)
            {
                System.Xml.Serialization.XmlSerializer serializer = new System.Xml.Serialization.XmlSerializer(typeof(Component), getAVMClasses());

                serializer.Serialize(streamWriter, avmComponent);
                streamWriter.Close();
            }
        }
        /* Copy end */

        /* Newly added methods - Akshay Agrawal*/
        public static string getMetaAttributeContent(object metaObject, string attributeName)
        {

            string id = "";

            PropertyInfo metaAttributesPropertyInfo = getPropertyInfo(getInterfaceType(metaObject), "Attributes");
            if (metaAttributesPropertyInfo != null)
            {
                PropertyInfo metaAttributePropertyInfo = metaAttributesPropertyInfo.PropertyType.GetProperty(attributeName);
                if (metaAttributePropertyInfo != null)
                {
                    id = metaAttributePropertyInfo.GetValue(metaAttributesPropertyInfo.GetValue(metaObject, null), null) as string;
                }
            }

            return id;
        }

        public static void setMetaAttributeContent(object metaObject, string attributeName, string attributeContent)
        {
            PropertyInfo metaAttributesPropertyInfo = getPropertyInfo(getInterfaceType(metaObject), "Attributes");
            if (metaAttributesPropertyInfo != null)
            {
                PropertyInfo metaAttributePropertyInfo = metaAttributesPropertyInfo.PropertyType.GetProperty(attributeName);
                if (metaAttributePropertyInfo != null)
                {
                    metaAttributePropertyInfo.SetValue(metaAttributesPropertyInfo.GetValue(metaObject, null), attributeContent, null);
                }
            }
        }
      
        //public static string getMetaObjectName(object metaObject)
        //{

        //    string id = "";

        //    id = typeof(ISIS.GME.Common.Interfaces.Base).GetProperty("Name", typeof(string)).GetValue(metaObject, null) as string;

        //    return id;
        //}

        //public static void setMetaObjectName(object metaObject, string objectName)
        //{
        //    typeof(ISIS.GME.Common.Interfaces.Base).GetProperty("Name", typeof(string)).SetValue(metaObject, objectName, null);
        //}
    }
}
