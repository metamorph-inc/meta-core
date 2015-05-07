using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using GmeCommon = ISIS.GME.Common;
using META;
using GME.CSharp;

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
        public string CADFormat { get; set; }           // creo
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

        public CADComponent(CyPhy.Component cyphycomp, string ProjectDirectory, bool size2fit = false, string format = "Creo")
        {
            Type = CADDataType.Component;
            StructuralInterfaceNodes = new Dictionary<string, StructuralInterfaceConstraint>();
            DisplayID = cyphycomp.Attributes.InstanceGUID;
            Id = cyphycomp.ID;
            GraphvizID = UtilityHelpers.CleanString2(cyphycomp.ID, 50, "-");
            AVMID = cyphycomp.Attributes.AVMID;
            RevID = "";
            VersionID = cyphycomp.Attributes.Version;
            CADFormat = format;
            Name = cyphycomp.Name;
            CadParameters = new List<CADParameter>();
            ModelType = "Part";
            Size2Fit = size2fit;
            MaterialName = "";
            CyPhyModelPath = cyphycomp.GetDirectoryPath(ProjectDirectory: ProjectDirectory);
            Classification = cyphycomp.Attributes.Classifications;
            HyperLink = cyphycomp.ToHyperLink();

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

        private void CreateStructuralInterfaceEquivalent(CyPhy.Component cyphycomp)
        {
            CyPhy.CADModel cadmodel = cyphycomp.Children.CADModelCollection.FirstOrDefault(x => x.Attributes.FileFormat.ToString() == "Creo");
            if (cadmodel != null)
            {
                string uri;
                cadmodel.TryGetResourcePath(out uri);
                char[] start = new char[] { '/', '\\' };
                if (!String.IsNullOrEmpty(uri))
                {
                    uri = uri.TrimStart(start);

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

                // META-1917: probably needs to be removed later b/c CAD Model Type attribute will be removed from metamodel
                if (String.IsNullOrEmpty(ModelType))
                {
                    Logger.Instance.AddLogMessage("Creo file extension can not be deduced from CADModel Resource Path for component [" + Name + "," + DisplayID + "], using CAD Model Type attribute instead!", Severity.Warning);
                    ModelType = cadmodel.Attributes.FileType.ToString();
                }
                // end META-1917

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
