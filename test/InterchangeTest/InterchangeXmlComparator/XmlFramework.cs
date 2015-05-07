using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml.Linq;

namespace InterchangeXmlComparator
{
    public static class XmlFramework
    {
        /// <summary>
        /// - Removes empty xmlns attributes
        /// - Change unclear namespaces: qXX -> _preNamespace
        /// </summary>
        /// <param name="rootElement"></param>
        public static void RemoveEmptyNamespaces(XElement rootElement)
        {
            var nsAttributes = new List<Tuple<XAttribute, XAttribute>>();
            var localNames = new HashSet<string>();
            var allAttributes = rootElement.Descendants().SelectMany(x => x.Attributes());

            allAttributes.Where(x => x.Name.LocalName == "xmlns" && string.IsNullOrEmpty(x.Value)).Remove();

            foreach (var attribute in allAttributes.Where(y => y.IsNamespaceDeclaration && !string.IsNullOrEmpty(y.Value)))
            {
                if (attribute.Name.Namespace != "http://www.w3.org/2000/xmlns/" || !attribute.Name.LocalName.StartsWith("q")) continue;

                localNames.Add(attribute.Name.LocalName);
                nsAttributes.Add(new Tuple<XAttribute, XAttribute>(attribute, new XAttribute("_preNamespace", attribute.Value)));
            }

            foreach (var nsAttribute in nsAttributes)
            {
                var parent = nsAttribute.Item1.Parent;

                if (parent == null) continue;

                nsAttribute.Item1.Remove();
                parent.Add(nsAttribute.Item2);
            }

            foreach (var attribute in allAttributes.Where(x => x.Value.Contains(":")))
            {
                foreach (var localName in localNames.Where(x => attribute.Value.Contains(x)))
                {
                    attribute.Value = attribute.Value.Replace(localName + ":", "");
                    break;
                }
            }
        }

        public static void CheckInParent(Dictionary<XElement, List<XElement>> dParentPairs, Dictionary<XElement, List<XElement>> eParentPairs, string keyParentProperty, string keyChildProperty = "Name")
        {
            var q1 = from dParentPair in dParentPairs
                     from eParentPair in eParentPairs
                     let dParentKeyValue = dParentPair.Key.Attribute(keyParentProperty).Value
                     let eParentKeyValue = eParentPair.Key.Attribute(keyParentProperty).Value
                     where dParentPair.Key.Attribute(keyParentProperty) != null
                     where eParentPair.Key.Attribute(keyParentProperty) != null
                     where eParentKeyValue == dParentKeyValue
                     select new
                     {
                         DesignElements = dParentPair.Value,
                         ExportElements = eParentPair.Value
                     };

            foreach (var q in q1)
            {
                CheckElements(q.ExportElements, q.DesignElements, keyChildProperty);
            }
        }

        public static void CheckElements(List<XElement> exportedNodes, List<XElement> desiredNodes, string keyProperty = "Name")
        {
            foreach (var dNode in desiredNodes)
            {
                // look for exported nodes with the same "keyproperty" on the same level (path)
                var eNodes =
                    exportedNodes.Where(
                        x =>
                           x.Attribute(keyProperty) != null
                        && x.Attribute(keyProperty).Value == dNode.Attribute(keyProperty).Value
                        && x.GetPath(PathTypes.ShortPath) == dNode.GetPath(PathTypes.ShortPath)).ToList();

                var eNode = eNodes.FirstOrDefault();

                if (eNode == null)
                {
                    Feedback.Add(new Feedback
                    {
                        DesiredNode = dNode,
                        FeedbackType = FeedbackType.Error,
                        Message = "Element is missing from exported XML"
                    });
                }
                else if (eNodes.Count() > 1)
                {
                    Feedback.Add(new Feedback
                    {
                        DesiredNode = dNode,
                        FeedbackType = FeedbackType.Error,
                        Message = "More than 1 element with the same name on the same level."
                    });
                }
                else
                {
                    CheckElements(eNode, dNode);
                }
            }

            foreach (var eNode in exportedNodes)
            {
                var dNode = desiredNodes.FirstOrDefault(x => x.Attribute(keyProperty) != null && x.Attribute(keyProperty).Value == eNode.Attribute(keyProperty).Value);

                if (dNode == null)
                {
                    Feedback.Add(new Feedback
                    {
                        ExportedNode = eNode,
                        FeedbackType = FeedbackType.Error,
                        Message = "Unnecessary element added to the export"
                    });
                }
                else
                {
                    CheckElements(eNode, dNode);
                }
            }
        }

        public static void CheckElements(XElement exported, XElement desired)
        {
            if (exported.Name.LocalName != desired.Name.LocalName)
                Feedback.Add(new Feedback
                {
                    FeedbackType = FeedbackType.Error,
                    ExportedNode = exported,
                    DesiredNode = desired,
                    Message = "Element names are different"
                });

            // Check text value of leaf nodes
            if (!exported.HasElements && !desired.HasElements)
            {
                var exportedValue = exported.DescendantNodes().OfType<XText>().Select(x => x.Value).FirstOrDefault();
                var desiredValue = desired.DescendantNodes().OfType<XText>().Select(x => x.Value).FirstOrDefault();

                if (exportedValue != desiredValue)
                    Feedback.Add(new Feedback
                    {
                        FeedbackType = FeedbackType.Error,
                        ExportedNode = exported,
                        DesiredNode = desired,
                        Message = "Text values are different"
                    });
            }

            CheckAttributes(exported, desired);
        }

        public static bool CheckAttributes(XElement exported, XElement desired, bool leaveFeedback = true)
        {
            IEnumerable<XAttribute> exportedAttributes = exported.Attributes();
            IEnumerable<XAttribute> desiredAttributes = desired.Attributes();

            var noErrors = true;
            foreach (var dAttribute in desiredAttributes)
            {
                var matchExported = exportedAttributes.FirstOrDefault(x => x.Name.LocalName == dAttribute.Name.LocalName);
                if (matchExported == null || matchExported.Value != dAttribute.Value)
                {
                    noErrors = false;
                    if (leaveFeedback) 
                    { 
                        Feedback.Add(new Feedback
                        {
                            FeedbackType = FeedbackType.Error,
                            ExportedNode = exported.Attribute(dAttribute.Name.LocalName),
                            DesiredNode = dAttribute,
                            Message = String.Format("Attributes are different: Expected '{0}' vs '{1}'", dAttribute.Value ?? "", matchExported == null ? "[missing]" : matchExported.Value)
                        });
                    }
                }
            }

            foreach (var eAttribute in exportedAttributes)
            {
                var matchDesired = desiredAttributes.FirstOrDefault(x => x.Name.LocalName == eAttribute.Name.LocalName);
                if (matchDesired == null || matchDesired.Value != eAttribute.Value)
                {
                    noErrors = false;
                    if (leaveFeedback)
                    {
                        Feedback.Add(new Feedback
                        {
                            FeedbackType = FeedbackType.Error,
                            ExportedNode = exported.Attribute(eAttribute.Name),
                            DesiredNode = eAttribute,
                            Message = String.Format("Attributes are different: Expected '{0}' vs '{1}'", matchDesired == null ? "[missing]" : matchDesired.Value, eAttribute.Value ?? "")
                        });
                    }
                }
            }

            return noErrors;
        }

        public static void CheckValue(XElement exportedValue, XElement desiredValue)
        {
            CheckElements(exportedValue, desiredValue);

            // Does it have valueSource?
            var eValueExpression = exportedValue.Element("ValueExpression");
            var dValueExpression = desiredValue.Element("ValueExpression");

            // No value expression
            if (eValueExpression == null && dValueExpression == null) return;

            CheckElements(eValueExpression, dValueExpression);

            var eValue = eValueExpression.Element("Value");
            var dValue = dValueExpression.Element("Value");

            // No value
            if (eValue == null && dValue == null) return;

            CheckElements(eValue, dValue);
        }

        // Finds element in lElements with one keyproperty.
        public static XElement GetPairElement(XElement element, IEnumerable<XElement> lElements, string keyProperty,bool useLocalName = true)
        {
            return GetPairElement(element, lElements, new List<string> { keyProperty },useLocalName);
        }

        // Finds element in lElements. Keyproperties are in OR relationship
        public static XElement GetPairElement(XElement element, IEnumerable<XElement> lElements, List<string> keyProperties,bool useLocalName = true)
        {
            var keyProperty = element.Attributes().FirstOrDefault(x => keyProperties.Contains(x.Name.LocalName));
            if (keyProperty == null) return null;

            if (useLocalName)
            {
                var q = (from e in lElements
                         let path = element.GetPath(PathTypes.ShortPath)
                         where e.Attribute(keyProperty.Name.LocalName) != null
                               && e.Attribute(keyProperty.Name.LocalName).Value == keyProperty.Value
                               && e.GetPath(PathTypes.ShortPath) == path
                         select e).FirstOrDefault();

                return q;
            }
            else
            {
                var q = (from e in lElements
                         let path = element.GetPath(PathTypes.ShortPath)
                         where e.Attribute(keyProperty.Name) != null
                               && e.Attribute(keyProperty.Name).Value == keyProperty.Value
                               && e.GetPath(PathTypes.ShortPath) == path
                         select e).FirstOrDefault();

                return q;
            }
        }

        // Finds element based on attribute names/values and text node values
        public static XElement GetPairElement(XElement element, IEnumerable<XElement> lElements)
        {
            /* 
            //Alternative implementation -- better for debugging
            XElement q = null;
            foreach (var e in lElements)
            {
                var path = element.GetPath(PathTypes.ShortPath);
                if (false == (e.GetPath(PathTypes.ShortPath) == path))
                    continue;
                if (false == (e.HasElements == element.HasElements))
                    continue;
                if (false == (!e.HasElements && e.Value == element.Value || e.HasElements))
                    continue;
                // Don't leave feedback if we're only comparing to find equivalence
                if (false == CheckAttributes(element, e,false))
                    continue;
                
                q = e;
                break;
            }

            return q;
            */
            
            // CheckAttributes has "false" flag -- don't leave feedback if we're only comparing to determine equivalence
            var q = (from e in lElements
                     let path = element.GetPath(PathTypes.ShortPath)
                     where
                           CheckAttributes(element, e,false)
                        && e.GetPath(PathTypes.ShortPath) == path
                        && e.HasElements == element.HasElements
                        && ( !e.HasElements && e.Value == element.Value || e.HasElements)
                     select e).FirstOrDefault();

            return q;
        }
    }
}