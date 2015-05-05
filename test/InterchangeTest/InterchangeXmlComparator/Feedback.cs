using System.Collections.Generic;
using System.Xml;
using System.Xml.Linq;

namespace InterchangeXmlComparator
{
    public class Feedback
    {
        private static readonly List<Feedback> Feedbacks = new List<Feedback>();

        public static void Add(Feedback fb)
        {
            Feedbacks.Add(fb);
        }

        public static List<Feedback> FeedBacklist
        {
            get
            {
                return Feedbacks;
            }
        }

        public FeedbackType FeedbackType { get; set; }

        public XObject ExportedNode { get; set; }
        public XObject DesiredNode { get; set; }

        public string ExportedNodeName
        {
            get { return GetNodeName(ExportedNode); }
        }

        public string DesiredNodeName
        {
            get { return GetNodeName(DesiredNode); }
        }

        public string Message { get; set; }

        public string ExportedNodePath
        {
            get { return ExportedNode.GetPath(); }
        }

        public string DesiredNodePath
        {
            get { return DesiredNode.GetPath(); }
        }

        private static string GetNodeName(XObject node)
        {
            if (node == null) return string.Empty;
            if (node.NodeType == XmlNodeType.Element)
            {
                return ((XElement)node).Name.ToString();
            }
            if (node.NodeType == XmlNodeType.Attribute)
            {
                return ((XAttribute)node).Name.ToString();
            }
            return string.Empty;
        }
    }

    public enum FeedbackType
    {
        Warning,
        Error,
    }
}