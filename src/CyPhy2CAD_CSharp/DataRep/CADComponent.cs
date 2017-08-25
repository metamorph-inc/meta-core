using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using GmeCommon = ISIS.GME.Common;
using META;
//using GME.CSharp;
//using GME.MGA;


using GME.MGA;
using GME.CSharp;
using GME;
using GME.MGA.Meta;
using System.Threading.Tasks;
using System.Security.Permissions;
using System.Security;

namespace CyPhy2CAD_CSharp.DataRep
{
    public struct CADParameter
    {
        public string Name;
        public string Type;
        public string Value;
        public string Unit;

        public CADParameter(CyPhy.CADParameter parameter)
        {
            Name = (parameter.Attributes.ParameterName == "") ? parameter.Name : parameter.Attributes.ParameterName;
            Type = parameter.Attributes.CADParameterType.ToString();
            Value = (parameter.Attributes.Value == "") ? (parameter.Attributes.DefaultValue == "") ? "0" : parameter.Attributes.DefaultValue : parameter.Attributes.Value;       
            Unit = "";
            if (parameter.Attributes.Unit == "")
            {
                if (parameter.Referred != null && parameter.Referred.unit != null)
                {
                    Unit = parameter.Referred.unit.Attributes.Symbol;
                }
            }
            else
            {
                Unit = parameter.Attributes.Unit;
            }
        
        }
    }

    public class CADComponent : CADData
    {
        public CyPhyClasses.CADModel.AttributesClass.FileFormat_enum CADFormat { get; set; }           // creo
        public string ModelName { get; set; }           // cad file name
        public string ModelType { get; set; }           // part, assembly
        public string ModelURI { get; set; }            // if (/CAD/damper.prt) --> uri is CAD/
        public string CyPhyModelPath { get; set; }
        public bool Standalone { get; set; }
        public string AVMID { get; set; }
        public string RevID { get; set; }
        public string VersionID { get; set; }
        public bool Size2Fit { get; set; }
        public Dictionary<string, StructuralInterfaceConstraint> StructuralInterfaceNodes { get; set; }
        public List<CADParameter> CadParameters { get; set; }
        public Dictionary<string, string> ManufacturingParameters { get; set; }
        public string MaterialName { get; set; }
        public string Classification { get; set; }
        public string HyperLink { get; set; } // For debug messages
        public string SpecialInstructions { get; set; }
        public List<Datum> SpecialDatums = new List<Datum>();
        public Dictionary<string, string> MetaData = new Dictionary<string, string>();

        public static readonly string MakeOrBuyParamStr = "procurement__make_or_buy";
        public static readonly string SpecialInstrParamStr = "SPECIALINSTRUCTIONS";

        private CyPhyCOMInterfaces.IMgaTraceability Traceability;
        protected List<CAD.ElementType> CadElementsList;
        protected List<TestBenchModel.TBComputation> pointCoordinatesList;
        public List<TestBenchModel.TBComputation> PointCoordinatesList
        {
            get
            {
                return pointCoordinatesList;
            }
        }

        public CADComponent(CyPhy.Component cyphycomp, string ProjectDirectory, bool size2fit = false,
            CyPhyCOMInterfaces.IMgaTraceability Traceability=null, CyPhyClasses.CADModel.AttributesClass.FileFormat_enum cadFormat = CyPhyClasses.CADModel.AttributesClass.FileFormat_enum.Creo)
        {
            Type = CADDataType.Component;
            StructuralInterfaceNodes = new Dictionary<string, StructuralInterfaceConstraint>();
            DisplayID = cyphycomp.Attributes.InstanceGUID;
            Id = cyphycomp.ID;
            GraphvizID = UtilityHelpers.CleanString2(cyphycomp.ID, 50, "-");
            AVMID = cyphycomp.Attributes.AVMID;
            RevID = "";
            VersionID = cyphycomp.Attributes.Version;
            CADFormat = cadFormat;
            Name = cyphycomp.Name;
            CadParameters = new List<CADParameter>();
            ModelType = "Part";
            Size2Fit = size2fit;
            MaterialName = "";
            CyPhyModelPath = cyphycomp.GetDirectoryPath(ProjectDirectory: ProjectDirectory);
            Classification = cyphycomp.Attributes.Classifications;
            HyperLink = cyphycomp.ToHyperLink();
            CadElementsList = new List<CAD.ElementType>();
            pointCoordinatesList = new List<TestBenchModel.TBComputation>();
            this.Traceability = Traceability;

            CreateStructuralInterfaceEquivalent(cyphycomp);

            AddManufacturingParameters(cyphycomp);

            var specialinstr = cyphycomp.Children.ParameterCollection.Where(p => p.Name.ToUpper() == SpecialInstrParamStr);
            if (specialinstr.Any())
            {
                SpecialInstructions = specialinstr.First().Attributes.Value.Replace("\"", "");
            }

            // META-3555 hack
            if (cyphycomp.Children.CADModelCollection.Any())
            {
                foreach (var datum in cyphycomp.Children.CADModelCollection.First().Children.CADDatumCollection)
                {
                    if (datum.Name == "FRONT" || datum.Name == "TOP" || datum.Name == "RIGHT")
                    {
                        SpecialDatums.Add(new Datum(datum, "", false));
                    }
                }
            }

            foreach (var prop in cyphycomp.Children.PropertyCollection)
            {
                if (prop.Name.StartsWith("METADATA."))
                {
                    MetaData.Add(prop.Name.Substring(9), prop.Attributes.Value);
                }
            }

            TraverseComposites(cyphycomp);
            CreatePointCoordinatesList(cyphycomp);
        }

        public string MakeOrBuyParam
        {
            get
            {
                if (ManufacturingParameters != null && ManufacturingParameters.Count != 0)
                {
                    string makeorbuyparam;
                    if (ManufacturingParameters.TryGetValue(MakeOrBuyParamStr, out makeorbuyparam))
                    {
                        return makeorbuyparam;
                    }
                    else
                    {
                        return null;
                    }
                }
                else
                {
                    return null;
                }
            }
        }

        private void AddManufacturingParameters(CyPhy.Component cyphycomp)
        {
            ManufacturingParameters = new Dictionary<string, string>();
            foreach (var mmodel in cyphycomp.Children.ManufacturingModelCollection)
            {
                foreach (var param in mmodel.Children.ManufacturingModelParameterCollection)
                {
                    try
                    {
                        ManufacturingParameters.Add(param.Name, param.Attributes.Value);
                    }
                    catch (ArgumentException)
                    {
                        Logger.Instance.AddLogMessage(String.Format("Error adding manufacturing parameter to component {0}. Parameter name {1} is duplicated.", cyphycomp.Name, param.Name), Severity.Warning);
                    }
                }
            }
        }

        string CheckFileExists(string compName, string relPath, string path)
        {
            path = Path.GetFullPath(path);
            var dir = Path.GetDirectoryName(path);
            IEnumerable<string> files;
            try
            {
                files = Directory.EnumerateFiles(dir);
            }
            catch (IOException)
            {
                // e.g. DirectoryNotFoundException
                files = new string[] { };
            }
            foreach (var filename in files)
            {
                if (path.Equals(AVM2CyPhyML.CyPhyMLComponentBuilder.GetCreoFileWithoutVersion(filename), StringComparison.InvariantCultureIgnoreCase))
                {
                    return null;
                }
            }

            return String.Format("Component {0} depends on <span style=\"background-color: DDDDDD\">{1}</span>, but it does not exist on disk",
                compName, SecurityElement.Escape(relPath));
        }

        public Task<string> missingFile;
        private void CreateStructuralInterfaceEquivalent(CyPhy.Component cyphycomp)
        {
            CyPhy.CADModel cadmodel = cyphycomp.Children.CADModelCollection.FirstOrDefault(x => x.Attributes.FileFormat == CADFormat);
            if (cadmodel != null)
            {
                string uri;
                cadmodel.TryGetResourcePath(out uri);
                char[] start = new char[] { '/', '\\' };
                if (!String.IsNullOrEmpty(uri) && CADFormat == CyPhyClasses.CADModel.AttributesClass.FileFormat_enum.Creo)
                {
                    uri = uri.TrimStart(start);

                    string absPath;
                    cadmodel.TryGetResourcePath(out absPath, ComponentLibraryManager.PathConvention.ABSOLUTE);
                    var hyperlink = cyphycomp.ToHyperLink(Traceability);
                    missingFile  = Task.Run(() => CheckFileExists(hyperlink, uri, absPath));

                    // META-1382
                    //ModelName = UtilityHelpers.CleanString2(Path.GetFileNameWithoutExtension(uri));
                    ModelName = Path.GetFileName(uri);
                    List<string> tokens_2 = ModelName.Split('.').ToList<string>();
                    int index = tokens_2.FindIndex(x => x.ToUpper() == "PRT");
                    if (index != -1)
                    {
                        ModelType = "Part";
                        ModelName = string.Join("", tokens_2.GetRange(0, index).ToArray());
                    }
                    else
                    {
                        index = tokens_2.FindIndex(x => x.ToUpper() == "ASM");
                        if (index != -1)
                        {
                            ModelType = "Assembly";
                            ModelName = string.Join("", tokens_2.GetRange(0, index).ToArray());
                        }
                    }
                    // It shouldn't be an empty string
                    if (ModelType.Length == 0) ModelType = "Part";
                }
                else
                {
                    Logger.Instance.AddLogMessage("CADModel Resource Path information unavailable for component [" + cyphycomp.Path + "," + DisplayID + "]!", Severity.Warning);
                }

                ModelURI = uri.Length > 0 ? Path.GetDirectoryName(uri) : "";
                //ModelType = cadmodel.Attributes.FileType.ToString();

                foreach (var param in cadmodel.Children.CADParameterCollection)
                {
                    CADParameter acadparam = new CADParameter(param);
                    CadParameters.Add(acadparam);
                }

                // META-947: Connector replaced StructuralInterface 
                //           Not dealing with nested Connectors right now.
                // foreach (var item in cyphycomp.Children.StructuralInterfaceCollection)
                foreach (CyPhy.Connector item in cyphycomp.Children.ConnectorCollection)
                {
                    FindMatchingSolidModelingFeatures(item, cadmodel);
                }

                foreach (CyPhy.CADDatum item in cyphycomp.Children.CADDatumCollection)
                {
                    // only Coordinate System is supported
                    if (item is CyPhy.CoordinateSystem)
                        FindMatchingSolidModelingFeatures(item, cadmodel);
                    //else
                    //    Logger.Instance.AddLogMessage("Only CoordinateSystem datums outside of a Connector are supported, other datum types not supported.", Severity.Warning);
                }

                // Materials
                if (cyphycomp.Children.MaterialRefCollection.Any())
                {
                    CyPhy.MaterialRef matRef = cyphycomp.Children.MaterialRefCollection.First();
                    CyPhy.Material material = matRef.Referred.Material;
                    if (material != null)
                    {
                        this.MaterialName = material.Attributes.Name;
                    }
                }
            }
        }

        private void FindMatchingSolidModelingFeatures(CyPhy.CADDatum datum,
                                                       CyPhy.CADModel acadmodel)
        {
            // META-947: Creates a virtual connector
            DataRep.StructuralInterfaceConstraint sirep = new StructuralInterfaceConstraint(datum,
                                                                                            this.Id,
                                                                                            this.DisplayID);
            sirep.PopulateStructuralInterface(acadmodel);

            if (sirep.DatumList.Count > 0)
            {
                StructuralInterfaceNodes[datum.ID] = sirep;
            }
        }

        private void FindMatchingSolidModelingFeatures(CyPhy.Connector a,
                                                       CyPhy.CADModel acadmodel)
        {
            // META-947: Connector instead of StructuralInterface
            // [1] Connectors can be nested so find all cad datums within a connector recursively
            // [2] Find connected datums
            //          Skip Connector without any Datum Ports

            DataRep.StructuralInterfaceConstraint sirep = new StructuralInterfaceConstraint(a,
                                                                                            this.Id,
                                                                                            this.DisplayID);
            sirep.PopulateStructuralInterface(acadmodel);

            if (sirep.DatumList.Count > 0)
            {
                StructuralInterfaceNodes[a.ID] = sirep;
            }
        }

        /*
        private void FindMatchingSolidModelingFeatures(CyPhy.CADDatum datum,
                                                       CyPhy.CADModel acadmodel)
        {
            // META-947: Creates a virtual connector

            Dictionary<string, DataRep.Datum> featuremap = new Dictionary<string, DataRep.Datum>();
            FindMatchingDatums(datum,
                               acadmodel,
                               featuremap);

            if (featuremap.Count > 0)
            {
                // virtual connector
                DataRep.StructuralInterfaceConstraint sirep = new StructuralInterfaceConstraint(datum, this.Id);
                sirep.DatumList = featuremap;
                sirep.DegreeFreedom = sirep.GetDegreesOfFreedom();
                StructuralInterfaceNodes[datum.ID] = sirep;
            }
        }

        private void FindMatchingSolidModelingFeatures(CyPhy.Connector a,
                                                       CyPhy.CADModel acadmodel)
        {
            // META-947: Connector instead of StructuralInterface
            // [1] Connectors can be nested so find all cad datums within a connector recursively
            // [2] Find connected datums
            //     Skip Connector without any Datum Ports

            List<CyPhy.CADDatum> CadDatum_List = new List<CyPhy.CADDatum>();
            FindCadDatumsInConnector(a, CadDatum_List);

            Dictionary<string, DataRep.Datum> featuremap = new Dictionary<string, DataRep.Datum>();    

            if (CadDatum_List.Count() > 0)
            {
                foreach (CyPhy.CADDatum item in CadDatum_List)
                {
                    FindMatchingDatums(item,
                                       acadmodel,
                                       featuremap);
                }

            }

            if (featuremap.Count > 0)
            {
                DataRep.StructuralInterfaceConstraint sirep = new StructuralInterfaceConstraint(a, this.Id);
                sirep.DatumList = featuremap;
                sirep.DegreeFreedom = sirep.GetDegreesOfFreedom();
                StructuralInterfaceNodes[a.ID] = sirep;
            }
        }


        private void FindMatchingDatums(CyPhy.CADDatum datum,
                                        CyPhy.CADModel cadmodel,
                                        Dictionary<string, DataRep.Datum> featuremap)
        {
            string cadmodel_id = cadmodel.ID;
            string alignment = "ALIGN";
            string orientation = "NONE";


            if (datum.Kind == "Surface")
            {
                alignment = (datum as CyPhy.Surface).Attributes.Alignment.ToString();
            }

            CadDatumTraversal traversal = new CadDatumTraversal(datum,
                                                                cadmodel_id);

            if (traversal.datumFound.Count > 0)
            {
                if (traversal.datumFound.Count > 1)
                {
                    Logger.Instance.AddLogMessage("Connector datum connected to multiple datums in the same CADModel [" + datum.Path + "]", Severity.Error);
                    return;
                }

                CyPhy2CAD_CSharp.DataRep.Datum datumRep = new DataRep.Datum(traversal.datumFound.First().Attributes.DatumName,
                                                                              datum.Kind,
                                                                              this.DisplayID);
                if (datum.Kind == "Surface")
                {
                    if (traversal.ReverseMap)
                        orientation = "SIDE_B";
                    else
                        orientation = "SIDE_A";
                }

                if (datum.Kind == "CoordinateSystem")
                {
                    alignment = "CSYS";
                }

                datumRep.Alignment = alignment;
                datumRep.Orientation = orientation;

                if (!featuremap.ContainsKey(datum.Name))
                {
                    featuremap[datum.Name] = datumRep;
                }
            }
        }

        // Connectors can be nested!
        private void FindCadDatumsInConnector(CyPhy.Connector connector,
                                   List<CyPhy.CADDatum> caddatum_list)
        {
            caddatum_list.AddRange(connector.Children.CADDatumCollection);
            
            //foreach (CyPhy.Connector conn in connector.Children.ConnectorCollection)
            //{
            //    FindCadDatumsInConnector(conn, caddatum_list);                  
            //}

        }
        */

        public CAD.CADComponentType ToCADXMLOutput(string representation)
        {
            CAD.CADComponentType cadoutput = new CAD.CADComponentType();
            cadoutput._id = UtilityHelpers.MakeUdmID();
            cadoutput.Type = ModelType.ToUpper();
            cadoutput.Standalone = false;
            cadoutput.SpecialInstruction = Size2Fit ? "SIZE_TO_FIT" : "";
            if (!String.IsNullOrEmpty(SpecialInstructions))
            {
                cadoutput.SpecialInstruction += (cadoutput.SpecialInstruction.Length != 0 ? "," : "") + SpecialInstructions;
            }
            cadoutput.Name = String.IsNullOrEmpty(ModelName) ? "" : ModelName;
            cadoutput.DisplayName = Name;
            cadoutput.MaterialID = MaterialName;
            cadoutput.ComponentID = DisplayID;
            cadoutput.Classification = Classification;


            if (CadParameters.Any())
            {
                CAD.ParametricParametersType parameterRoot = new CAD.ParametricParametersType();
                parameterRoot._id = UtilityHelpers.MakeUdmID();

                List<CAD.CADParameterType> paramarray = new List<CAD.CADParameterType>();
                foreach (var item in CadParameters)
                {
                    CAD.CADParameterType parameter = new CAD.CADParameterType();
                    parameter._id = UtilityHelpers.MakeUdmID();
                    parameter.Type = item.Type;
                    parameter.Name = item.Name;
                    parameter.Value = item.Value;
                    parameter.Units = new CAD.UnitsType();
                    parameter.Units._id = UtilityHelpers.MakeUdmID();
                    parameter.Units.Value = item.Unit;
                    paramarray.Add(parameter);
                }
                parameterRoot.CADParameter = paramarray.ToArray();
                cadoutput.ParametricParameters = parameterRoot;
            }

            if (ManufacturingParameters != null && ManufacturingParameters.Count != 0)
            {
                cadoutput.ManufacturingParameter = new CAD.ManufacturingParameterType[ManufacturingParameters.Count];

                int i = 0;
                foreach (var param in ManufacturingParameters)
                {
                    cadoutput.ManufacturingParameter[i++] = new CAD.ManufacturingParameterType()
                    {
                        Name = param.Key,
                        Value = param.Value
                    };
                }
            }

            if (MetaData != null && MetaData.Count != 0)
            {
                cadoutput.MetaData = new CAD.MetaDataType[MetaData.Count];

                int i = 0;
                foreach (var param in MetaData)
                {
                    cadoutput.MetaData[i++] = new CAD.MetaDataType()
                    {
                        Key = param.Key,
                        Value = param.Value
                    };
                }

            }

            cadoutput.Representation = representation;
            if (this.CadElementsList.Count > 0)
            {
                cadoutput.Elements = new CAD.ElementsType()
                {
                    _id = UtilityHelpers.MakeUdmID()
                };

                cadoutput.Elements.Element = this.CadElementsList.ToArray();

            }

            return cadoutput;
        }

        public override string ToString()
        {
            StringBuilder sbuilder = new StringBuilder();
            sbuilder.AppendFormat("[{0}, {1}, {2}]\n",
                Name,
                Id,
                DisplayID);
            sbuilder.AppendLine();

            foreach (var item in StructuralInterfaceNodes.Values)
                sbuilder.AppendLine(item.ToString());

            return sbuilder.ToString();
        }

        public override string ToGraphViz(bool directed = false)
        {
            StringBuilder sbuilder = new StringBuilder();
            sbuilder.AppendFormat("{0} [label=\"{1} \\n {2} \\n {3}\"];\n",
                GraphvizID,
                DisplayID,
                Name,
                Id);

            return sbuilder.ToString();
        }

        private void TraverseComposites(CyPhy.Component component)
        {

            foreach (var material in component.Children.MaterialContentsCollection)
            {
                CAD.ElementType cadElement = new CAD.ElementType()
                {
                    _id = UtilityHelpers.MakeUdmID(),
                    ElementType1 = "SURFACE"
                };

                MgaModel materialMga = material.Impl as MgaModel;
                List<MgaFCO> startDirPts = new List<MgaFCO>();
                List<MgaFCO> endDirPts = new List<MgaFCO>();

                startDirPts = CyPhy2CAD_CSharp.DataRep.CADGeometry.FindByRole(materialMga, "Start_Direction");
                endDirPts = CyPhy2CAD_CSharp.DataRep.CADGeometry.FindByRole(materialMga, "End_Direction");

                if (startDirPts.Count != 1 && endDirPts.Count != 1)
                {
                    Logger.Instance.AddLogMessage("Material Content must contain one Start_Direction and one End_Direction point.", Severity.Warning); 
                    continue;
                }


                CyPhy.Point startDirPt = CyPhyClasses.Point.Cast(startDirPts[0]);
                string startDirPtDatumName= GetFeatureName(startDirPt);

                CyPhy.Point endDirPt = CyPhyClasses.Point.Cast(endDirPts[0]);
                string endDirPtDatumName= GetFeatureName(endDirPt);

                CAD.OrientationType cadOrientation = new CAD.OrientationType()
                {
                    _id = UtilityHelpers.MakeUdmID()
                };
                CAD.GeometryType cadGeometry = new CAD.GeometryType()
                {
                    _id = UtilityHelpers.MakeUdmID()
                };


                CAD.FeatureType cadStartFeature = new CAD.FeatureType()
                {
                    _id = UtilityHelpers.MakeUdmID(),
                    ComponentID = DisplayID,
                    Name = startDirPtDatumName,
                    MetricID = DisplayID + ":" + startDirPtDatumName
                };
                TestBenchModel.TBComputation startFeatureComputation = new TestBenchModel.TBComputation()
                {
                    ComponentID = DisplayID,
                    ComputationType = TestBenchModel.TBComputation.Type.POINTCOORDINATES,
                    Details = startDirPtDatumName,
                    FeatureDatumName = startDirPtDatumName,
                    MetricID = DisplayID + ":" + startDirPtDatumName,
                    RequestedValueType = "Vector"
                }; 
                pointCoordinatesList.Add(startFeatureComputation);
                if (String.IsNullOrEmpty(startDirPtDatumName))
                    Logger.Instance.AddLogMessage("Empty point datum name [" + startDirPt.Path + "]", Severity.Warning);

                CAD.FeatureType cadEndFeature = new CAD.FeatureType()
                {
                    _id = UtilityHelpers.MakeUdmID(),
                    ComponentID = DisplayID,
                    Name = endDirPtDatumName,
                    MetricID = DisplayID + ":" + endDirPtDatumName
                };
                TestBenchModel.TBComputation endFeatureComputation = new TestBenchModel.TBComputation()
                {
                    ComponentID = DisplayID,
                    ComputationType = TestBenchModel.TBComputation.Type.POINTCOORDINATES,
                    Details = endDirPtDatumName,
                    FeatureDatumName = endDirPtDatumName,
                    MetricID = DisplayID + ":" + endDirPtDatumName,
                    RequestedValueType = "Vector"
                };
                pointCoordinatesList.Add(endFeatureComputation);
                if (String.IsNullOrEmpty(endDirPtDatumName))
                    Logger.Instance.AddLogMessage("Empty point datum name [" + endDirPt.Path + "]", Severity.Warning);

                CAD.FeaturesType cadFeatures = new CAD.FeaturesType()
                {
                    _id = UtilityHelpers.MakeUdmID(),
                    FeatureID = material.ID,
                    GeometryType = "Vector",
                    FeatureInterfaceType = "CAD_DATUM",
                    FeatureGeometryType = "POINT",
                    PrimaryGeometryQualifier = "",
                    SecondaryGeometryQualifier = ""
                };

                cadFeatures.Feature= new CAD.FeatureType[2];
                cadFeatures.Feature[0] = cadStartFeature;
                cadFeatures.Feature[1] = cadEndFeature;
                cadGeometry.Features = new CAD.FeaturesType[1];
                cadGeometry.Features[0] = cadFeatures;
                cadOrientation.Geometry = cadGeometry;


                // Material Layups
                CAD.MaterialLayupType cadLayers = new CAD.MaterialLayupType()
                {
                    _id = UtilityHelpers.MakeUdmID(),
                    Position = material.Attributes.Position.ToString().ToUpper(),
                    Offset = (material.Attributes.Position.ToString().ToUpper() != "OFFSET_BY_VALUE") ? 0 : (material.Attributes.PositionOffset),
                    Direction = material.Attributes.MaterialLayupDirection.ToString().ToUpper() 
                };

                int layerCnt = material.Children.MaterialLayerCollection.Count();
                if (layerCnt > 0)
                {
                    cadLayers.Layer = new CAD.LayerType[layerCnt];

                    int k = 0;
                    foreach (var layer in material.Children.MaterialLayerCollection.OrderBy(i => i.Attributes.LayerID))
                    {                        
                        CAD.LayerType cadLayer = new CAD.LayerType()
                        {
                            _id = UtilityHelpers.MakeUdmID(),
                            ID = layer.Attributes.LayerID,
                            Drop_Order = layer.Attributes.DropOrder,
                            Material_Name = layer.Attributes.LayerMaterial,
                            Orientation = layer.Attributes.LayerOrientation,
                            Thickness = layer.Attributes.LayerThickness
                        };

                        cadLayers.Layer[k] = cadLayer;
                        k++;
                    }
                }

                CAD.ElementContentsType cadElementContents = new CAD.ElementContentsType();
                cadElementContents._id = UtilityHelpers.MakeUdmID();
                cadElementContents.Orientation = cadOrientation;
                cadElementContents.MaterialLayup = cadLayers;
                cadElement.ElementContents = cadElementContents;


                if (material.DstConnections.ContentsToGeometryCollection.Count() < 1 || material.DstConnections.ContentsToGeometryCollection.Count() > 1)
                {
                    Logger.Instance.AddLogMessage("Material Content need to connect to one Face geometry.", Severity.Warning); 
                    continue;
                }


                // Face, Polygon, or Extrusion Geometry
                CyPhy.ContentsToGeometry conn = material.DstConnections.ContentsToGeometryCollection.FirstOrDefault();
                if (conn != null)
                {
                    CyPhy.Face faceGeometry = conn.DstEnds.Face;
                    CyPhy.Polygon polgonGeometry = conn.DstEnds.Polygon;
                    CyPhy.Extrusion extrusionGeometry = conn.DstEnds.Extrusion;


                    int countGeometryTypes = 0;

                    if (faceGeometry != null ) ++countGeometryTypes;
                    if (polgonGeometry != null ) ++countGeometryTypes;
                    if (extrusionGeometry != null ) ++countGeometryTypes;

                    if (countGeometryTypes != 1)
                    {
                        Logger.Instance.AddLogMessage("MaterialContents must be connected to one and only one geometry type (i.e. FACE, POLOGON, or EXTRUSION).", Severity.Warning);
                        continue;
                    }


                    // Approach when only FACE was supported
                    //String primBoundaryQ = faceGeometry.Attributes.BoundaryQualifier.ToString().Replace("_", string.Empty);
                    //MgaModel faceMga = faceGeometry.Impl as MgaModel;
                    //List<MgaFCO> normalDirPts = new List<MgaFCO>();
                    //normalDirPts = CyPhy2CAD_CSharp.DataRep.CADGeometry.FindByRole(faceMga, "Direction_Reference_Point");

                    String primBoundaryQ = "";
                    MgaModel faceMga;
                    List<MgaFCO> normalDirPts = new List<MgaFCO>();

                    if (faceGeometry != null)
                    {
                        primBoundaryQ = faceGeometry.Attributes.BoundaryQualifier.ToString().Replace("_", string.Empty);
                        faceMga = faceGeometry.Impl as MgaModel;
                        normalDirPts = CyPhy2CAD_CSharp.DataRep.CADGeometry.FindByRole(faceMga, "Direction_Reference_Point");
                    }
                    else if (polgonGeometry != null)
                    {
                        primBoundaryQ = polgonGeometry.Attributes.BoundaryQualifier.ToString().Replace("_", string.Empty);
                        faceMga = polgonGeometry.Impl as MgaModel;
                        normalDirPts = CyPhy2CAD_CSharp.DataRep.CADGeometry.FindByRole(faceMga, "Direction_Reference_Point");
                    } else if ( extrusionGeometry != null )
                    {
                        primBoundaryQ = extrusionGeometry.Attributes.BoundaryQualifier.ToString().Replace("_", string.Empty);
                        faceMga = extrusionGeometry.Impl as MgaModel;
                        normalDirPts = CyPhy2CAD_CSharp.DataRep.CADGeometry.FindByRole(faceMga, "Direction_Reference_Point");
                    }

                    if (normalDirPts.Count != 1)
                    {
                        Logger.Instance.AddLogMessage("Geometry (FACE, POLYGON, or EXTRUSION) can contain only one Direction_Reference_Point point.", Severity.Warning); 
                        continue;
                    }

                    CyPhy.Point normalDirPt = CyPhyClasses.Point.Cast(normalDirPts.FirstOrDefault());
                    string normalPtFeatureName = GetFeatureName(normalDirPt);
                    if (String.IsNullOrEmpty(normalPtFeatureName))
                    {
                        Logger.Instance.AddLogMessage("Direction_Reference_Point point of the Face/Ploygon/Extrusion geometry doesn't have a datum name. Make sure it is connected to a valid point inside a CADModel.", Severity.Warning);
                        continue;
                    }

                    CyPhy2CAD_CSharp.DataRep.CADGeometry faceOrExtruOrPolyGeometryRep = null;
                    if (faceGeometry != null)
                    {
                        faceOrExtruOrPolyGeometryRep = CyPhy2CAD_CSharp.DataRep.CADGeometry.CreateGeometry(faceGeometry);
                        if (faceOrExtruOrPolyGeometryRep == null)
                        {
                            Logger.Instance.AddLogMessage("Unsuccessfully created a representation of a Face Geometry.", Severity.Warning);
                            continue;
                        }
                    }
                    else if (polgonGeometry != null)
                    {
                        faceOrExtruOrPolyGeometryRep = CyPhy2CAD_CSharp.DataRep.CADGeometry.CreateGeometry(polgonGeometry);
                        if (faceOrExtruOrPolyGeometryRep == null)
                        {
                            Logger.Instance.AddLogMessage("Unsuccessfully created a representation of a Polygon Geometry.", Severity.Warning);
                            continue;
                        }
                    }
                    else if (extrusionGeometry != null)
                    {
                        faceOrExtruOrPolyGeometryRep = CyPhy2CAD_CSharp.DataRep.CADGeometry.CreateGeometry(extrusionGeometry);
                        if (faceOrExtruOrPolyGeometryRep == null)
                        {
                            Logger.Instance.AddLogMessage("Unsuccessfully created a representation of a Extrusion Geometry.", Severity.Warning);
                            continue;
                        }
                    }

                    // Element/Geometry
                    CAD.GeometryType cadFaceOrExtruOrPolyGeometryOut = faceOrExtruOrPolyGeometryRep.ToCADXMLOutput();
                    if (cadFaceOrExtruOrPolyGeometryOut == null)
                    {
                        Logger.Instance.AddLogMessage("Unsuccessfully converted a representation of a Face/Ploygon/Extrusion Geometry to CAD xml.", Severity.Warning);
                        continue;
                    }


                    cadElement.Geometry = cadFaceOrExtruOrPolyGeometryOut;
                    foreach (var faceOrExtruOrPolyGeomFeatures in cadFaceOrExtruOrPolyGeometryOut.Features)
                    {
                        foreach (var feature_temp in faceOrExtruOrPolyGeomFeatures.Feature)
                        {
                            TestBenchModel.TBComputation faceGeometryComputation = new TestBenchModel.TBComputation()
                            {
                                ComponentID = DisplayID,
                                ComputationType = TestBenchModel.TBComputation.Type.POINTCOORDINATES,
                                Details = feature_temp.Name,
                                FeatureDatumName = feature_temp.Name,
                                MetricID = feature_temp.MetricID,
                                RequestedValueType = "Vector"
                            };
                            pointCoordinatesList.Add(faceGeometryComputation);
                        }
                    }

                    // Element/Geometry

                    string direction_temp = "";

                    if (faceGeometry != null) direction_temp = (faceGeometry.Attributes.NormalDirection == CyPhyClasses.Face.AttributesClass.NormalDirection_enum.Toward_Reference_Point) ? "TOWARD" : "AWAY";
                    else if (polgonGeometry != null) direction_temp = (polgonGeometry.Attributes.NormalDirection == CyPhyClasses.Polygon.AttributesClass.NormalDirection_enum.Toward_Reference_Point) ? "TOWARD" : "AWAY";
                    else if (extrusionGeometry != null) direction_temp = (extrusionGeometry.Attributes.NormalDirection == CyPhyClasses.Extrusion.AttributesClass.NormalDirection_enum.Toward_Reference_Point) ? "TOWARD" : "AWAY";

                    CAD.SurfaceNormalType cadSurfaceNormal = new CAD.SurfaceNormalType()
                    {
                        _id = UtilityHelpers.MakeUdmID(),
                        Direction = direction_temp
                    };

                    CAD.FeaturesType cadSurfaceNormFeatures = new CAD.FeaturesType()
                    {
                        _id = UtilityHelpers.MakeUdmID(),
                        FeatureID = normalDirPt.ID,
                        GeometryType = "POINT",
                        FeatureInterfaceType = "CAD_DATUM",
                        FeatureGeometryType = "POINT",
                        Feature = new CAD.FeatureType[1],
                        PrimaryGeometryQualifier = primBoundaryQ,
                        SecondaryGeometryQualifier = ""
                    };

                    cadSurfaceNormFeatures.Feature[0] = new CAD.FeatureType()
                    {
                        _id = UtilityHelpers.MakeUdmID(),
                        Name = normalPtFeatureName,
                        ComponentID = DisplayID,
                        MetricID = DisplayID + ":" + normalPtFeatureName
                    };
                    TestBenchModel.TBComputation surfNormalComputation = new TestBenchModel.TBComputation()
                    {
                        ComponentID = DisplayID,
                        ComputationType = TestBenchModel.TBComputation.Type.POINTCOORDINATES,
                        Details = normalPtFeatureName,
                        FeatureDatumName = normalPtFeatureName,
                        MetricID = DisplayID + ":" + normalPtFeatureName,
                        RequestedValueType = "Vector"
                    };
                    pointCoordinatesList.Add(surfNormalComputation);
                    if (String.IsNullOrEmpty(normalPtFeatureName))
                        Logger.Instance.AddLogMessage("Empty point datum name [" + normalDirPt.Path + "]", Severity.Warning);

                    CAD.GeometryType cadSurfaceNormGeom = new CAD.GeometryType()
                    {
                        _id = UtilityHelpers.MakeUdmID(),
                        Features = new CAD.FeaturesType[1],                    
                    };

                    cadSurfaceNormGeom.Features[0] = cadSurfaceNormFeatures;
                    cadSurfaceNormal.Geometry = cadSurfaceNormGeom;
                    cadElement.SurfaceNormal = cadSurfaceNormal;

                }

                this.CadElementsList.Add(cadElement);                
            }

        }

        private string GetFeatureName(CyPhy.Port port)
        {
            string datumName = "";
            PointMetricTraversal traverser = new PointMetricTraversal(port);
            if (traverser.portsFound.Count() == 1)
            {
                CyPhy.Port foundPort = traverser.portsFound.First();

                if (foundPort.Kind == "Surface")
                {
                    datumName = (foundPort as CyPhy.Surface).Attributes.DatumName;
                }
                else if (foundPort.Kind == "Point")
                {
                    datumName = (foundPort as CyPhy.Point).Attributes.DatumName;
                }
                else if (foundPort.Kind == "Axis")
                {
                    datumName = (foundPort as CyPhy.Axis).Attributes.DatumName;
                }

            }

            return datumName;
        }

        private void CreatePointCoordinatesList(CyPhy.Component component)
        {
            CyPhy.CADModel cadmodel = component.Children.CADModelCollection.FirstOrDefault(x => x.Attributes.FileFormat.ToString() == "Creo");
            if (cadmodel == null)
            {
                return;
            }

            string componentID = component.ID;
            string cadmodelID = cadmodel.ID;

            /*
            var conns = component.Children.PortCompositionCollection
                .Where(x => x.SrcEnd.Kind == "Point" && x.DstEnd.Kind == "Point")
                .Cast<IMgaSimpleConnection>()
                .Where(x => (x.Src.ParentModel.ID == componentID || x.Src.ParentModel.ID == cadmodelID) && (x.Dst.ParentModel.ID == componentID || x.Dst.ParentModel.ID == cadmodelID));
            */

            foreach (var p in cadmodel.Children.PointCollection)
            {
                MgaFCO pointFCO = p.Impl as MgaFCO;
                var connPt = pointFCO.PartOfConns
                .Cast<IMgaConnPoint>()
                .Select(x => x.Owner)
                .Cast<IMgaSimpleConnection>()
                .Where(x => ((x.Src.ID == pointFCO.ID) && (x.Dst.ParentModel.ID == componentID)) || ((x.Dst.ID == pointFCO.ID) && (x.Src.ParentModel.ID == componentID)));

                if (connPt.Count() > 0)
                {
                    CyPhy.Point point = CyPhyClasses.Point.Cast(pointFCO);
                    if (point != null)
                    {
                        TestBenchModel.TBComputation computation = new TestBenchModel.TBComputation()
                        {
                            ComponentID = DisplayID,
                            ComputationType = TestBenchModel.TBComputation.Type.POINTCOORDINATES,
                            Details = point.Attributes.DatumName,
                            MetricID = DisplayID + ":" + point.Attributes.DatumName,
                            RequestedValueType = "Vector"
                        };

                        this.PointCoordinatesList.Add(computation);
                    }
                }
            }

            
        }

        #region Deprecated
        /*
        // due to META-947
        private void FindMatchingSolidModelingFeatures(CyPhy.StructuralInterface a,
                                                       CyPhy.CADModel acadmodel)
        {
            Dictionary<string, DataRep.Datum> featuremap = new Dictionary<string, DataRep.Datum>();    

            foreach (var item in a.Children.StructuralInterfaceFeatureCollection)
            {
                List<CyPhy.GeometryFeature> cadmodelfeatures = new List<CyPhy.GeometryFeature>();
                foreach (var conn in item.SrcConnections.FeatureMapCollection.Where(x => x.SrcEnds.GeometryFeature.ParentContainer.ID == acadmodel.ID))
                    cadmodelfeatures.Add(conn.SrcEnds.GeometryFeature);

                foreach (var conn in item.DstConnections.FeatureMapCollection.Where(x => x.DstEnds.GeometryFeature.ParentContainer.ID == acadmodel.ID))
                    cadmodelfeatures.Add(conn.DstEnds.GeometryFeature);

                if (cadmodelfeatures.Count > 0)
                {
                    if (cadmodelfeatures.Count > 1)
                    {
                        continue;
                    }

                    string kind = cadmodelfeatures.First().Kind;
                    if (item.Kind + "Geometry" == kind)
                    {
                        cadmodelfeatures.First();
                        CyPhy2CAD_CSharp.DataRep.Datum datum = new DataRep.Datum(cadmodelfeatures.First().Attributes.Datum, 
                                                                             item.Kind,
                                                                             this.DisplayID);

                        if (item.Kind == "Surface")
                        {
                            datum.Alignment = (item as CyPhy.Surface).Attributes.Alignment.ToString();
                            datum.Orientation = (cadmodelfeatures.First() as CyPhy.SurfaceGeometry).Attributes.Orientation.ToString();
                        }
                        else
                        {
                            datum.Alignment = "ALIGN";
                            datum.Orientation = "NONE";
                        }

                        if (featuremap.ContainsKey(item.Name))
                        {
                            //messages.Add(new Message("StructuralInterface datum ports do not have unique names [" + item.Path + "]", Severity.Error));
                            continue;
                        }
                        else
                            featuremap[item.Name] = datum;
                    }
                    else
                    {
                        //messages.Add(new Message("StructuralInterface datum connected to wrong type of datum in CADModel [" + item.Path + "]", Severity.Error));
                        continue;                        
                    }
                }

            }

            if (featuremap.Count > 0)
            {
                DataRep.StructuralInterfaceConstraint sirep = new StructuralInterfaceConstraint(a, this.Id);
                sirep.DegreeFreedom = sirep.GetDegreesOfFreedom();
                sirep.DatumList = featuremap;
                StructuralInterfaceNodes[a.ID] = sirep;
            }
        }
        */

        #endregion


    }   // end class
}   // end namespace
