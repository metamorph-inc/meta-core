using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Xml.Linq;

namespace InterchangeXmlComparator
{
    public class ComponentComparator
    {
        private XDocument ExportedDocument { get; set; }
        private XDocument DesiredDocument { get; set; }

        public ComponentComparator(XDocument exportedDocument, XDocument desiredDocument)
        {
            ExportedDocument = exportedDocument;
            DesiredDocument = desiredDocument;
        }

        public void Check()
        {
            #region Check component
            var eComponent = ExportedDocument.Root;
            var dComponent = DesiredDocument.Root;

            if (eComponent == null || dComponent == null)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.Error.WriteLine("Component's Root element is missing!");
                Environment.Exit(1);
            }

            XmlFramework.CheckElements(eComponent, dComponent);

            #endregion

            // Remove unclear namespaces (like empty xmlns attributes and qX namespaces.
            XmlFramework.RemoveEmptyNamespaces(eComponent);
            XmlFramework.RemoveEmptyNamespaces(dComponent);

            #region Check component properties

            var dAllProperties = dComponent.Descendants("Property").ToList();
            var eAllProperties = eComponent.Descendants("Property").ToList();

            const string propertyKeyField = "ID";

            foreach (var dResource in dAllProperties)
            {
                var eProperty = XmlFramework.GetPairElement(dResource, eAllProperties, propertyKeyField);

                if (eProperty == null)
                {
                    Feedback.Add(new Feedback
                    {
                        DesiredNode = dResource,
                        FeedbackType = FeedbackType.Error,
                        Message = "This property is not in the exported model."
                    });
                    continue;
                }

                XmlFramework.CheckElements(eProperty, dResource);
                XmlFramework.CheckValue(eProperty.Element("Value"), dResource.Element("Value"));
            }

            foreach (var eProperty in eAllProperties)
            {
                var dResource = XmlFramework.GetPairElement(eProperty, dAllProperties, propertyKeyField);

                if (dResource == null)
                {
                    Feedback.Add(new Feedback
                    {
                        ExportedNode = eProperty,
                        FeedbackType = FeedbackType.Error,
                        Message = "This property is not in the desired model."
                    });
                    continue;
                }

                XmlFramework.CheckElements(eProperty, dResource);
                XmlFramework.CheckValue(eProperty.Element("Value"), dResource.Element("Value"));
            }

            #endregion

            #region Check component resources

            var dAllResources = dComponent.Descendants("Resource").ToList();
            var eAllResources = eComponent.Descendants("Resource").ToList();

            const string resourceKeyField = "ID";

            foreach (var dResource in dAllResources)
            {
                var eResource = XmlFramework.GetPairElement(dResource, eAllProperties, propertyKeyField);

                if (eResource == null)
                {
                    Feedback.Add(new Feedback
                    {
                        DesiredNode = dResource,
                        FeedbackType = FeedbackType.Error,
                        Message = "This property is not in the exported model."
                    });
                    continue;
                }

                XmlFramework.CheckElements(eResource, dResource);
            }

            foreach (var eResource in eAllResources)
            {
                var dResource = XmlFramework.GetPairElement(eResource, dAllResources, resourceKeyField);

                if (dResource == null)
                {
                    Feedback.Add(new Feedback
                    {
                        ExportedNode = eResource,
                        FeedbackType = FeedbackType.Error,
                        Message = "This property is not in the desired model."
                    });
                    continue;
                }

                XmlFramework.CheckElements(eResource, dResource);
            }

            #endregion

            #region Check domain models

            var otherNodeTypes = new List<string> { "DomainModel", "AnalysisConstruct", "Classifications" };
            var allOtherExported = eComponent.Descendants().Where(x => otherNodeTypes.Contains(x.Name.LocalName)).ToList();
            var allOtherDesired = dComponent.Descendants().Where(x => otherNodeTypes.Contains(x.Name.LocalName)).ToList();

            foreach (var dOther in allOtherDesired)
            {
                var eOther = XmlFramework.GetPairElement(dOther, allOtherExported);

                if (eOther == null)
                {
                    Feedback.Add(new Feedback
                    {
                        DesiredNode = dOther,
                        FeedbackType = FeedbackType.Error,
                        Message = "This property is not in the exported model."
                    });
                    continue;
                }

                XmlFramework.CheckElements(eOther, dOther);
            }

            foreach (var eOther in allOtherExported)
            {
                var dOther = XmlFramework.GetPairElement(eOther, allOtherDesired);

                if (dOther == null)
                {
                    Feedback.Add(new Feedback
                    {
                        DesiredNode = eOther,
                        FeedbackType = FeedbackType.Error,
                        Message = "This property is not in the exported model."
                    });
                    continue;
                }

                XmlFramework.CheckElements(dOther, eOther);
            }

            #endregion

            #region Check Ports
            var expectedPorts = eComponent.Descendants().Where(x => x.Name.LocalName == "Port").ToList();
            var desiredPorts = dComponent.Descendants().Where(x => x.Name.LocalName == "Port").ToList();

            foreach (var dPort in desiredPorts)
            {
                var ePort = XmlFramework.GetPairElement(dPort, expectedPorts);

                if (ePort == null)
                {
                    Feedback.Add(new Feedback
                    {
                        DesiredNode = dPort,
                        FeedbackType = FeedbackType.Error,
                        Message = "This port is not in the exported model."
                    });
                    continue;
                }

                XmlFramework.CheckElements(ePort, dPort);
            }

            foreach (var ePort in expectedPorts)
            {
                var dPort = XmlFramework.GetPairElement(ePort, desiredPorts);

                if (dPort == null)
                {
                    Feedback.Add(new Feedback
                    {
                        DesiredNode = ePort,
                        FeedbackType = FeedbackType.Error,
                        Message = "This port is not in the desired model."
                    });
                    continue;
                }

                XmlFramework.CheckElements(dPort, ePort);
            }

            #endregion

            #region Check Join Data

            var eJDs = new List<XElement>(eComponent.Descendants().Where(x => x.Name == "JoinData"));
            var dJDs = new List<XElement>(dComponent.Descendants().Where(x => x.Name == "JoinData"));
            foreach (var eJD in eJDs)
            {
                var dJD = XmlFramework.GetPairElement(eJD, dJDs, "id", false);

                if (dJD == null)
                {
                    Feedback.Add(new Feedback()
                    {
                        ExportedNode = eJD,
                        FeedbackType = FeedbackType.Error,
                        Message = "This JoinData is not in the desired model."
                    });
                    continue;
                }

                XmlFramework.CheckElements(eJD, dJD);
            }
            foreach (var dJD in dJDs)
            {
                var eJD = XmlFramework.GetPairElement(dJD, eJDs, "id", false);

                if (eJD == null)
                {
                    Feedback.Add(new Feedback()
                    {
                        ExportedNode = dJD,
                        FeedbackType = FeedbackType.Error,
                        Message = "This JoinData is missing from the export."
                    });
                    continue;
                }

                XmlFramework.CheckElements(dJD, eJD);
            }

            #endregion

            #region Check connections

            #region Collecting connections

            var ePortmaps = new Dictionary<string, HashSet<string>>();
            foreach (var ePortmapNode in eComponent.Descendants().Where(x => x.Attribute("PortMap") != null && !string.IsNullOrEmpty(x.Attribute("PortMap").Value)))
            {
                var ePortmapIds = ePortmapNode.Attribute("PortMap").Value.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries).ToList();

                if (!ePortmapIds.Any()) continue;

                var baseId = ePortmapNode.Attribute("ID") == null ? string.Empty : ePortmapNode.Attribute("ID").Value;
                if (string.IsNullOrEmpty(baseId)) continue;

                if (!ePortmaps.ContainsKey(baseId))
                    ePortmaps[baseId] = new HashSet<string>();

                ePortmaps[baseId].AddRange(ePortmapIds);

                foreach (var ePortmapId in ePortmapIds)
                {
                    if (!ePortmaps.ContainsKey(ePortmapId))
                        ePortmaps[ePortmapId] = new HashSet<string>();

                    ePortmaps[ePortmapId].Add(baseId);
                }
            }

            var dPortmaps = new Dictionary<string, HashSet<string>>();
            foreach (var dPortmapNode in dComponent.Descendants().Where(x => x.Attribute("PortMap") != null && !string.IsNullOrEmpty(x.Attribute("PortMap").Value)))
            {
                var dPortmapIds = dPortmapNode.Attribute("PortMap").Value.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries).ToList();

                if (!dPortmapIds.Any()) continue;

                var baseId = dPortmapNode.Attribute("ID") == null ? string.Empty : dPortmapNode.Attribute("ID").Value;
                if (string.IsNullOrEmpty(baseId)) continue;

                if (!dPortmaps.ContainsKey(baseId))
                    dPortmaps[baseId] = new HashSet<string>();

                dPortmaps[baseId].AddRange(dPortmapIds);

                foreach (var dPortmapId in dPortmapIds)
                {
                    if (!dPortmaps.ContainsKey(dPortmapId))
                        dPortmaps[dPortmapId] = new HashSet<string>();

                    dPortmaps[dPortmapId].Add(baseId);
                }
            }

            #endregion

            #region Check connection pairs

            foreach (var dPortmap in dPortmaps)
            {
                if (!ePortmaps.ContainsKey(dPortmap.Key))
                {
                    Feedback.Add(new Feedback
                    {
                        DesiredNode = dComponent.Descendants().FirstOrDefault(x => x.Attribute("ID") != null && x.Attribute("ID").Value == dPortmap.Key),
                        FeedbackType = FeedbackType.Error,
                        Message = "Missing connection in the exported file"
                    });
                    continue;
                }

                var result = dPortmap.Value.All(x => ePortmaps[dPortmap.Key].Contains(x));
                if (!result)
                    Feedback.Add(new Feedback
                    {
                        DesiredNode = dComponent.Descendants().FirstOrDefault(x => x.Attribute("ID") != null && x.Attribute("ID").Value == dPortmap.Key),
                        FeedbackType = FeedbackType.Error,
                        Message = "Missing connection in the exported file"
                    });
            }

            foreach (var ePortmap in ePortmaps)
            {
                if (!dPortmaps.ContainsKey(ePortmap.Key))
                {
                    Feedback.Add(new Feedback
                    {
                        ExportedNode = eComponent.Descendants().FirstOrDefault(x => x.Attribute("ID") != null && x.Attribute("ID").Value == ePortmap.Key),
                        FeedbackType = FeedbackType.Error,
                        Message = "Unnecessary connection in the exported file"
                    });
                    continue;
                }

                var result = ePortmap.Value.All(x => dPortmaps[ePortmap.Key].Contains(x));
                if (!result)
                    Feedback.Add(new Feedback
                    {
                        ExportedNode = eComponent.Descendants().FirstOrDefault(x => x.Attribute("ID") != null && x.Attribute("ID").Value == ePortmap.Key),
                        FeedbackType = FeedbackType.Error,
                        Message = "Unnecessary connection in the exported file"
                    });
            }

            #endregion

            #endregion
        }
    }
}