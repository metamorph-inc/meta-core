using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2CAD_CSharp.DataRep
{
    public enum CADDataType
    {
        Assembly,
        Component,
        Edge,
        Forwarder
    }

    abstract public class CADData
    {
        public string Id { get; set; }              // GME GUID
        public string DisplayID { get; set; }       // m_assemblyInterface_ID, InstanceGUID
        public CADDataType Type { get; set; }
        public string GraphvizID { get; set; }
        public string Name { get; set; } 

        abstract public string ToGraphViz(bool directed = false);
    }
   

    public class CADForwarder : CADData
    {
        public string ParentID  { get; set; }

        public CADForwarder()
        {
            Type = CADDataType.Forwarder;
            Name = "Forwarder";
        }

        public override string ToString()
        {
            StringBuilder sbuilder = new StringBuilder();
            sbuilder.AppendFormat("[{0}]\n",
                Id);

            return sbuilder.ToString();
        }

        public override string ToGraphViz(bool directed = false)
        {
            return "";
        }
    }

    public class CADAssembly : CADData
    {
        public string ConfigID { get; set; }
        public string DisplayName { get; set; }
        public List<CADForwarder> ForwarderList { get; set; }
        public Dictionary<string, CADComponent> ChildComponents { get; set; }
        public List<CADEdge> ChildEdges { get; set; }
        public Dictionary<string, CADComponent> ChildSize2FitComponents { get; set; }
        public List<CADEdge> ChildSize2FitEdges { get; set; }
        public MetaLinkData MetaLinkData { get; set; }

        public string RootComponentID { get; set; }
        private List<TestBenchModel.TBComputation> pointCoordinatesList;
        public List<TestBenchModel.TBComputation> PointCoordinatesList
        {
            get
            {
                return pointCoordinatesList;
            }
        }

        public CADAssembly()
        {
            Type = CADDataType.Assembly;
            ForwarderList = new List<CADForwarder>();
            ChildComponents = new Dictionary<string, CADComponent>();
            ChildEdges = new List<CADEdge>();
            ChildSize2FitComponents = new Dictionary<string, CADComponent>();
            ChildSize2FitEdges = new List<CADEdge>();
            pointCoordinatesList = new List<TestBenchModel.TBComputation>();
        }

        public CADAssembly(string id,
                           string name)
        {
            Type = CADDataType.Assembly;
            ForwarderList = new List<CADForwarder>();
            ChildComponents = new Dictionary<string, CADComponent>();
            ChildEdges = new List<CADEdge>();
            ChildSize2FitComponents = new Dictionary<string, CADComponent>();
            ChildSize2FitEdges = new List<CADEdge>();
            ConfigID = id;
            Id = id;
            Name = name;
            DisplayName = name;
            pointCoordinatesList = new List<TestBenchModel.TBComputation>();
        }

        public bool Size2Fit(string id)
        {
            return ChildSize2FitComponents.ContainsKey(id);
        }

        public CAD.AssemblyType ToCADXMLOutput(TestBenchModel.TestBenchBase tb)
        {
            CAD.AssemblyType cadassemblyoutput = new CAD.AssemblyType();
            cadassemblyoutput._id = UtilityHelpers.MakeUdmID();
            cadassemblyoutput.ConfigurationID = ConfigID;

            CAD.CADComponentType cadassemblycomponentoutput = new CAD.CADComponentType();
            cadassemblycomponentoutput._id = UtilityHelpers.MakeUdmID();
            cadassemblycomponentoutput.ComponentID = ConfigID;
            cadassemblycomponentoutput.Type = "ASSEMBLY";
            cadassemblycomponentoutput.Standalone = false;
            cadassemblycomponentoutput.SpecialInstruction = "";
            cadassemblycomponentoutput.Name = this.Name;
            cadassemblycomponentoutput.DisplayName = DisplayName;
            cadassemblycomponentoutput.MaterialID = "";
            cadassemblycomponentoutput.Representation = "";
            cadassemblycomponentoutput.Classification = "";

            
            Dictionary<string, List<CAD.ConstraintType>> compidToConstraint = new Dictionary<string, List<CAD.ConstraintType>>();       // Dictionary<Id, List<Constraints>>
            var sortedEdges = ChildEdges.OrderBy(x => x.DstPortName).ToList();
            foreach (CADEdge edge in sortedEdges)        //foreach (CADEdge edge in ChildEdges)
            {
                CAD.ConstraintType constraintout = edge.ToCADXMLOutput(tb);
                if (!compidToConstraint.ContainsKey(edge.Dst.Id))
                {
                    compidToConstraint.Add(edge.Dst.Id, new List<CAD.ConstraintType>());
                }
                compidToConstraint[edge.Dst.Id].Add(constraintout);
            }

            var sortedS2FEdges = ChildSize2FitEdges.OrderBy(x => x.DstPortName).ToList();
            foreach (CADEdge edge in sortedS2FEdges)        //foreach (CADEdge edge in ChildSize2FitEdges)
            {
                CAD.ConstraintType constraintout = edge.ToCADXMLOutput(tb);
                if (!compidToConstraint.ContainsKey(edge.Dst.Id))
                {
                    compidToConstraint.Add(edge.Dst.Id, new List<CAD.ConstraintType>());
                }
                compidToConstraint[edge.Dst.Id].Add(constraintout);
            }

            // META-2885
            //Dictionary<string, CAD.CADComponentType> tmpComponents = new Dictionary<string, CAD.CADComponentType>();
            List<CAD.CADComponentType> tmpComponents = new List<CAD.CADComponentType>();
            var sortedComponents = ChildComponents.OrderBy(x => x.Value.Name).ToDictionary(x => x.Key, x => x.Value).Values.ToList();
            foreach (CADComponent component in sortedComponents)      //foreach (CADComponent component in ChildComponents.Values)
            {
                CAD.CADComponentType componentout = component.ToCADXMLOutput(tb.GetRepresentation(component));

                // root constraint
                if (component.Id == RootComponentID)
                {
                    bool rootIsAsmType = component.ModelType == "Assembly";     //bool rootIsAsmType = component.ModelType == "PART";
                    List<CAD.PairType> pairlist = new List<CAD.PairType>();
                    CAD.ConstraintType rootconstraintout = new CAD.ConstraintType();
                    rootconstraintout._id = UtilityHelpers.MakeUdmID();

                    // front pair
                    pairlist.Add(MakeRootConstraint("FRONT",
                                                    component.DisplayID,
                                                    ConfigID,
                                                    rootIsAsmType,
                                                    component.SpecialDatums.Where(d => d.DatumName=="FRONT").FirstOrDefault()));

                    // top pair
                    pairlist.Add(MakeRootConstraint("TOP",
                                                    component.DisplayID,
                                                    ConfigID,
                                                    rootIsAsmType,
                                                    component.SpecialDatums.Where(d => d.DatumName == "TOP").FirstOrDefault()));

                    // right pair
                    pairlist.Add(MakeRootConstraint("RIGHT",
                                                    component.DisplayID,
                                                    ConfigID,
                                                    rootIsAsmType,
                                                    component.SpecialDatums.Where(d => d.DatumName == "RIGHT").FirstOrDefault()));

                    rootconstraintout.Pair = pairlist.ToArray();

                    componentout.Constraint = new CAD.ConstraintType[1];
                    componentout.Constraint[0] = rootconstraintout;
                }
                else
                {
                    if (compidToConstraint.ContainsKey(component.Id))
                    {
                        componentout.Constraint = compidToConstraint[component.Id].ToArray();
                    }
                }

                //tmpComponents[component.Id] = componentout;
                tmpComponents.Add(componentout);

                // 3/7/2016 - Export_All_Component_Points at TB level
                pointCoordinatesList.AddRange(component.PointCoordinatesList);

            }
            
            var size2fitCompSorted = ChildSize2FitComponents.OrderBy(x => x.Value.Name).ToDictionary(x => x.Key, x => x.Value).Values.ToList();
            foreach (CADComponent component in size2fitCompSorted)//foreach (CADComponent component in ChildSize2FitComponents.Values)      // META-2885
            {
                CAD.CADComponentType componentout = component.ToCADXMLOutput(tb.GetRepresentation(component));
                if (compidToConstraint.ContainsKey(component.Id))
                {
                    componentout.Constraint = compidToConstraint[component.Id].ToArray();
                }

                //tmpComponents[component.Id] = componentout;
                tmpComponents.Add(componentout);

                // 3/7/2016 - Export_All_Component_Points at TB level
                pointCoordinatesList.AddRange(component.PointCoordinatesList);
            }

            // Fill MetaLinkData. Only needed in case if data is prepared for Meta-Link communication
            if (MetaLinkData != null && MetaLinkData.Components.Count>0)
            {
                cadassemblyoutput.MetaLinkData = new CAD.MetaLinkDataType();
                cadassemblyoutput.MetaLinkData.CadComponentMLData = new CAD.CadComponentMLDataType[MetaLinkData.Components.Count];
                int i  = 0;
                foreach (MetaLinkData.Component comp in MetaLinkData.Components)
                {
                    cadassemblyoutput.MetaLinkData.CadComponentMLData[i] = new CAD.CadComponentMLDataType();
                    cadassemblyoutput.MetaLinkData.CadComponentMLData[i].ID = comp.ID;
                    cadassemblyoutput.MetaLinkData.CadComponentMLData[i].AvmID = comp.AvmID;
                    if (comp.Connectors.Count>0)
                    {
                        cadassemblyoutput.MetaLinkData.CadComponentMLData[i].ConnectorMLData = new CAD.ConnectorMLDataType[comp.Connectors.Count];
                        int j = 0;
                        foreach (MetaLinkData.Connector conn in comp.Connectors)
                        {
                            cadassemblyoutput.MetaLinkData.CadComponentMLData[i].ConnectorMLData[j] = new CAD.ConnectorMLDataType();
                            cadassemblyoutput.MetaLinkData.CadComponentMLData[i].ConnectorMLData[j].ID = conn.ID;
                            cadassemblyoutput.MetaLinkData.CadComponentMLData[i].ConnectorMLData[j].DisplayName = conn.DisplayName;
                            if (conn.Datums.Count > 0)
                            {
                                cadassemblyoutput.MetaLinkData.CadComponentMLData[i].ConnectorMLData[j].DatumMLData = new CAD.DatumMLDataType[conn.Datums.Count];
                                int k = 0;
                                foreach (MetaLinkData.Datum d in conn.Datums)
                                {
                                    cadassemblyoutput.MetaLinkData.CadComponentMLData[i].ConnectorMLData[j].DatumMLData[k] = new CAD.DatumMLDataType();
                                    cadassemblyoutput.MetaLinkData.CadComponentMLData[i].ConnectorMLData[j].DatumMLData[k].ID = d.ID;
                                    cadassemblyoutput.MetaLinkData.CadComponentMLData[i].ConnectorMLData[j].DatumMLData[k].DisplayName = d.DisplayName;
                                    k++;
                                }
                            }
                            j++;
                        }
                    }
                    i++;
                }
            }

            cadassemblycomponentoutput.CADComponent = tmpComponents.ToArray();       //cadassemblycomponentoutput.CADComponent = tmpComponents.Values.ToArray();
            cadassemblyoutput.CADComponent = cadassemblycomponentoutput;

            CAD.PropertiesType asmproperties = new CAD.PropertiesType();
            asmproperties._id = UtilityHelpers.MakeUdmID();
            CAD.UnitsType asmunit = new CAD.UnitsType();
            asmunit._id = UtilityHelpers.MakeUdmID();
            asmunit.Value = "value1";
            asmproperties.Units = asmunit;
            cadassemblyoutput.Properties = asmproperties;
            return cadassemblyoutput;
        }


        private CAD.PairType MakeRootConstraint(string location,
                                                string componentDispID,
                                                string assemblyID,
                                                bool rootIsAsmType,
                                                Datum datum)
        {
            CAD.PairType pair = new CAD.PairType();
            pair._id = UtilityHelpers.MakeUdmID();
            pair.FeatureGeometryType = "SURFACE";
            pair.FeatureAlignmentType = "ALIGN";
            pair.FeatureInterfaceType = "CAD_DATUM";

            List<CAD.ConstraintFeatureType> featurelist = new List<CAD.ConstraintFeatureType>();
            CAD.ConstraintFeatureType constraintFeature = new CAD.ConstraintFeatureType();
            constraintFeature._id = UtilityHelpers.MakeUdmID();
            constraintFeature.ComponentID = componentDispID;
            constraintFeature.FeatureOrientationType = "SIDE_A";
            if (rootIsAsmType)
            {
                constraintFeature.FeatureName = "ASM_" + location;
            }
            else
            {
                constraintFeature.FeatureName = location;
            }
            
            // META-3555 hack
            if (datum != null && datum.Marker != null)
            {
                constraintFeature.GeometryMarker = datum.Marker.ToCADXml();
            }

            featurelist.Add(constraintFeature);

            CAD.ConstraintFeatureType asmConstraint = new CAD.ConstraintFeatureType();
            asmConstraint._id = UtilityHelpers.MakeUdmID();
            asmConstraint.ComponentID = assemblyID;
            asmConstraint.FeatureOrientationType = "SIDE_A";
            asmConstraint.FeatureName = "ASM_" + location;
            featurelist.Add(asmConstraint);
            pair.ConstraintFeature = featurelist.ToArray();

            return pair;
        }

        public bool RegularComponent(string id)
        {
            return ChildComponents.ContainsKey(id);
        }

        public void CollectDirectories(Dictionary<string, CadManifestObject> CADManifestList,
                                                 string projectDir,
                                                 string outputDir)
        {
            foreach (var component in ChildComponents.Values)
            {
                string key = component.AVMID + component.RevID + component.VersionID;
                if (!CADManifestList.ContainsKey(key))
                {
                    // If key not found, create one.
                    string modelpath = component.CyPhyModelPath;
                    if (modelpath == "") modelpath = ".";
                    CadManifestObject manifestobject = new CadManifestObject();
                    manifestobject.AvmID = component.AVMID;
                    manifestobject.Version = component.VersionID;

                    string cadfolder = Path.Combine(projectDir, modelpath, component.ModelURI??"");
                    string outputfolder = Path.Combine(outputDir, "abc.xyz");
                    Uri uricad = new Uri(cadfolder);
                    Uri uriout = new Uri(outputfolder);
                    manifestobject.LocalFolderPath = Uri.UnescapeDataString(uriout.MakeRelativeUri(uricad).ToString().Replace('/', '\\'));
                    manifestobject.RemoteMETAProPath = Path.Combine(modelpath, component.ModelURI ?? "");
                    manifestobject.RemoteFolderPath = Path.GetDirectoryName(modelpath);
                    CADManifestList[key] = manifestobject;
                }
            }

            foreach (var component in ChildSize2FitComponents.Values)
            {
                string key = component.AVMID + component.RevID + component.VersionID;
                if (!CADManifestList.ContainsKey(key))
                {
                }
            }
        }

        // snyako@isis.vanderbilt.edu : This code is not used anymore
        /*
        public Multibody.ModelAssembly ToMultibodyOutput()
        {
            Multibody.ModelAssembly mbdAssembly = new Multibody.ModelAssembly();
            if ((ChildComponents.Count + ChildSize2FitComponents.Count) > 0)
            {
                mbdAssembly.Components = new Multibody.ModelAssemblyComponents();

                List<Multibody.ModelAssemblyComponentsComponent> mbdComponentList = new List<Multibody.ModelAssemblyComponentsComponent>();
                foreach (var component in ChildComponents)
                {
                    mbdComponentList.Add(component.Value.ToMultibodyOutput());
                }

                foreach (var component in ChildSize2FitComponents)
                {
                    mbdComponentList.Add(component.Value.ToMultibodyOutput());
                }
                mbdAssembly.Components.Component = mbdComponentList.ToArray();
            }

            if ((ChildEdges.Count + ChildSize2FitEdges.Count) > 0)
            {
                mbdAssembly.Joints = new Multibody.ModelAssemblyJoints();

                List<object> jointList = new List<object>();
                foreach (var edge in ChildEdges)
                {
                    jointList.Add(edge.ToMultibodyOutput());
                }

                foreach (var edge in ChildSize2FitEdges)
                {
                    jointList.Add(edge.ToMultibodyOutput());
                }

                mbdAssembly.Joints.Items = jointList.ToArray();
            }            

            return mbdAssembly;
        }
        */

        public override string ToString()
        {
            StringBuilder sbuilder = new StringBuilder();
            sbuilder.AppendFormat("Assembly [ {0}, {1}]\n",
                ConfigID,
                Id).AppendLine();

            sbuilder.Append("<<<    Forwarders  >>>").AppendLine();
            foreach (CADForwarder item in ForwarderList)
                sbuilder.Append(item.ToString()).AppendLine();                

            sbuilder.Append("<<<    Components  >>>").AppendLine();
            foreach (CADComponent item in ChildComponents.Values)
                sbuilder.Append(item.ToString()).AppendLine();

            sbuilder.Append("<<<    Size2Fit Components  >>>").AppendLine();
            foreach (CADComponent item in ChildSize2FitComponents.Values)
                sbuilder.Append(item.ToString()).AppendLine();


            sbuilder.Append("<<<    Edges  >>>").AppendLine();
            foreach (CADEdge item in ChildEdges)
                sbuilder.Append(item.ToString()).AppendLine();

            return sbuilder.ToString();
        }

        public override string ToGraphViz(bool directed = false)
        {
            StringBuilder sbuilder = new StringBuilder();
            sbuilder.AppendFormat("subgraph cluster_{0}",
                UtilityHelpers.CleanString2(Id, 100, "-"));
            sbuilder.Append("{\n").AppendLine();
            sbuilder.AppendFormat("label = \"{0}\";",
                Id).AppendLine();

            foreach (CADComponent item in ChildComponents.Values)
                sbuilder.Append(item.ToGraphViz(directed)).AppendLine();

            foreach (CADComponent item in ChildSize2FitComponents.Values)
                sbuilder.Append(item.ToGraphViz(directed)).AppendLine();

            foreach (CADEdge item in ChildEdges)
                sbuilder.Append(item.ToGraphViz(directed)).AppendLine();

            sbuilder.Append("}\n").AppendLine();

            return sbuilder.ToString();
        }
    }


}   // end namespace
