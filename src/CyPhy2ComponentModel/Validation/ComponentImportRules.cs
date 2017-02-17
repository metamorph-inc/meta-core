using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Xml;
using System.Xml.Linq;
using System.Xml.Schema;

namespace CyPhy2ComponentModel.Validation
{
    public class ComponentImportRules
    {
        public static bool CheckUniqueNames(string inputXMLFile, ref List<string> errorMessages)
        {
            var result = true;
            using (var sr = new StreamReader(inputXMLFile))
            {
                var xdoc = XDocument.Load(sr);

                foreach (var node in xdoc.DescendantNodes().Where(x => x.NodeType == XmlNodeType.Element))
                {
                    var xElement = (XElement)node;
                    var names = new HashSet<string>();
                    foreach (var xChild in xElement.Elements())
                    {
                        var nameAttr = xChild.Attribute("Name");
                        if (nameAttr == null)
                        {
                            continue;
                        }

                        var nameVal = nameAttr.Value;
                        if (string.IsNullOrEmpty(nameVal))
                        {
                            continue;
                        }

                        if (names.Contains(nameVal))
                        {
                            var path =
                                xChild.AncestorsAndSelf()
                                    .Select(x => x.Name.LocalName)
                                    .Aggregate((workingSentence, next) => next + "\\" + workingSentence);
                            errorMessages.Add(string.Format("ERROR: Duplicated names: {0} - {1}", nameVal, path));
                            result = false;
                        }
                        names.Add(nameVal);
                    }
                }
            }

            return result;
        }

        public static bool ValidateXsd(string inputXMLFile, ref List<string> oErrorMessages)
        {
            var noErrors = true;
            var errorMessages = new List<string>();
            using (var sr = new StreamReader(inputXMLFile))
            {
                var schemas = new XmlSchemaSet();

                #region Collect XSD files

                var xsdDirectory = new DirectoryInfo(META.VersionInfo.MetaPath).GetFiles("*.xsd", SearchOption.AllDirectories).FirstOrDefault();
                if (xsdDirectory == null || xsdDirectory.Directory == null)
                {
                    errorMessages.Add("XSD directory cannot be found");
                    return false;
                }

                var xsdFileSet =
                    xsdDirectory.Directory.GetFiles("*.xsd", SearchOption.TopDirectoryOnly).Select(x => x.FullName);

                #endregion

                foreach (var xsd in xsdFileSet)
                {
                    if (File.Exists(xsd))
                    {
                        schemas.Add(XmlSchema.Read(XmlReader.Create(new StreamReader(xsd)), (sender, args) =>
                                                                                           {
                                                                                               errorMessages.Add("Schema is wrong: " + args.Message);
                                                                                           }));
                    }
                    else
                    {
                        errorMessages.Add("XSD file doesn't exist: " + xsd);
                    }
                }

                var xdoc = XDocument.Load(sr);

                xdoc.Validate(schemas, (o, e) =>
                {
                    errorMessages.Add(e.Message);
                    noErrors = false;
                }, true);
            }
            oErrorMessages.AddRange(errorMessages);
            return noErrors;
        }

        public static bool ExecuteAll(string inputXMLFile, ref List<string> errorMessages)
        {
            var results = new List<bool>
                          {
                              CheckUniqueNames(inputXMLFile, ref errorMessages),
                              ValidateXsd(inputXMLFile, ref errorMessages)
                          };

            return results.All(x => x);
        }
    }
}
