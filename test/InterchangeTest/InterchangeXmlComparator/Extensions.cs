using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml;
using System.Xml.Linq;

namespace InterchangeXmlComparator
{
    public enum PathTypes
    {
        FullPath,
        ShortPath
    }

    public static class Extensions
    {
        public static string GetPath(this XObject node, PathTypes pathType = PathTypes.FullPath)
        {
            if (node == null) return string.Empty;
            if (node.NodeType == XmlNodeType.Attribute)
            {
                var attribute = (XAttribute)node;
                var attrName = attribute.Name.ToString();

                return string.Format("{0}[{1}]", GetPath(attribute.Parent), attrName);
            }
            if (node.NodeType == XmlNodeType.Element)
            {
                return GetPath((XElement)node, pathType);
            }

            return string.Empty;
        }

        private static string GetPath(this XElement element, PathTypes pathType = PathTypes.FullPath)
        {
            if (element == null) return string.Empty;
            var ancestors = element.Ancestors().ToList();

            if (!ancestors.Any())
            {
                return element.Name.ToString();
            }

            var p = ancestors.Select(x => x.Name.LocalName).Aggregate((path, next) => next + "\\" + path);
            p += "\\" + element.Name.LocalName;

            if (pathType == PathTypes.ShortPath) return p;

            if (element.NodeType == XmlNodeType.Element)
            {
                var index = element.Parent.Elements().ToList().IndexOf(element);
                p = p + '[' + index + ']';

                // Add name attribute (if any)
                if (element.Attribute("Name") != null)
                    p = string.Format("{0}[Name=='{1}']", p, element.Attribute("Name").Value);
                else if (element.Attribute("ID") != null)
                    p = string.Format("{0}[ID=='{1}']", p, element.Attribute("ID").Value);

                if (element.HasElements) return p;
                var textValue = element.DescendantNodes().OfType<XText>().Select(x => x.Value).FirstOrDefault();

                if (!string.IsNullOrEmpty(textValue))
                {
                    p = String.Format("{0}\\{{\"{1}\"}}", p, textValue);
                }
            }
            return p;
        }

        public static void AddRange<T>(this HashSet<T> hash, IEnumerable<T> items)
        {
            foreach (var item in items)
            {
                hash.Add(item);
            }
        }
    }
}