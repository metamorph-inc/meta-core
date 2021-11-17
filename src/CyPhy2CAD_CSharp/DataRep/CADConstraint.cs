using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using GmeCommon = ISIS.GME.Common;
using GME.CSharp;


namespace CyPhy2CAD_CSharp.DataRep
{
    public class StructuralInterfaceConstraint
    {
        public string PortID { get; set; }
        public string ParentID { get; set; }                        // GUID
        public string ParentInstanceGUID { get; set; }              // InstanceGUID
        public Dictionary<string, Datum> DatumList { get; set; }
        public GmeCommon.Interfaces.FCO CyPhyImpl { get; set; }
        public int DegreeFreedom { get; set; }
        public string InterfaceDefinition { get; set; }
        public KinematicJoint Joint { get; set; }
        public string Name { get; set; }
        public CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum? AdjSurfTreatment; // If specified, this'll be different from DEFAULT

        public List<CADGeometry> Geometry = new List<CADGeometry>(); // If geometries are associated with this interface

        public StructuralInterfaceConstraint(GmeCommon.Interfaces.FCO impl, 
                                             string parentID, 
                                             string parentinstanceGUID)
        {
            CyPhyImpl = impl;
            PortID = impl.ID;
            ParentID = parentID;
            ParentInstanceGUID = parentinstanceGUID;
            DatumList = new Dictionary<string, Datum>();
            DegreeFreedom = 0;

            if (impl is CyPhy.Connector)
            {
                InterfaceDefinition = (impl as CyPhy.Connector).Attributes.Definition;
                Name = impl.Name;

            }
            else if (impl is CyPhy.CADDatum)
            {
                InterfaceDefinition = (impl as CyPhy.CADDatum).Attributes.Definition;
                Name = impl.Name;
            }
            else
            {
                InterfaceDefinition = "";
                Name = "";
            }
        }

        // Sets up joint based on already populated datums
        // Pre-Condition: DatumList is filled out
        private void SetupJoint(List<CyPhy.CADDatum> datumlist, List<CyPhy.CADDatum> limitreflist, Dictionary<string, DataRep.Datum> limitrefmap)
        {
            if ((CyPhyImpl as CyPhy.Connector).Children.KinematicRevoluteJointCollection.Any())
            {
                Joint = new KinematicJoint((CyPhyImpl as CyPhy.Connector).Children.KinematicRevoluteJointCollection.First());
            } else 
            if ((CyPhyImpl as CyPhy.Connector).Children.KinematicCylindricalJointCollection.Any())
            {
                Joint = new KinematicJoint((CyPhyImpl as CyPhy.Connector).Children.KinematicCylindricalJointCollection.First());
            } else 
            if ((CyPhyImpl as CyPhy.Connector).Children.KinematicFixedJointCollection.Any())
            {
                Joint = new KinematicJoint((CyPhyImpl as CyPhy.Connector).Children.KinematicFixedJointCollection.First());
            } else 
            if ((CyPhyImpl as CyPhy.Connector).Children.KinematicTranslationalJointCollection.Any())
            {
                Joint = new KinematicJoint((CyPhyImpl as CyPhy.Connector).Children.KinematicTranslationalJointCollection.First());
            }

            // Get the datums to associate with this joint
            if (Joint != null)
            {
                foreach (var cyphydatum in datumlist)
                {
                    Datum datarepdatum;
                    if (DatumList.TryGetValue(cyphydatum.Name, out datarepdatum))
                    {
                        // Is datum part of defining the joint?
                        if (cyphydatum.SrcConnections.KinematicJointDefinitionCollection.Any())
                        {
                            if (cyphydatum is CyPhy.Axis)
                            {
                                Joint.Axis = datarepdatum;
                            }
                            else if (cyphydatum is CyPhy.Surface)
                            {
                                Joint.AlignmentPlane = datarepdatum;
                            }
                        }
                    }
                }
                if (limitreflist.Any() && Joint.JointType == KinematicJoint.KinematicJointType.REVOLUTE)
                {
                    throw new Exception("Limit references for revolute joints are not supported currently. Guides will be used as limit references. Please remove limit references on rvlute joints from your model. Connector: " + CyPhyImpl.Name + ", Parent: " + CyPhyImpl.ParentContainer.Name);
                }
                var guides = datumlist.Where(d => d.Attributes.DefinitionNotes.Contains("GUIDE"));
                if (guides.Any())
                {
                    Logger.Instance.AddLogMessage("Datum is using old guide format. Please use the attribute 'IsGuide'. Connector: " + CyPhyImpl.Path, Severity.Error);
                    return;
                }
                guides = datumlist.Where(d => d.Attributes.IsGuide);
                if (guides.Count() > 1)
                {
                    throw new Exception("More than one guides in a kinematic joint. This is not supported yet. Connector: " + CyPhyImpl.Path);
                }
                else if (guides.Count() == 1)
                {
                    Joint.RotationDefaultReference = this.DatumList[guides.First().Name];
                }
                
                
                foreach (var limitrefdatum in limitreflist)
                {
                    Datum datarepdatum;
                    if (limitrefmap.TryGetValue(limitrefdatum.Name, out datarepdatum))
                    {
                        // Is this datum part of defining the limits of the joint?
                        if ((limitrefdatum as CyPhy.Surface).SrcConnections.KinematicTranslationalLimitReferenceCollection.Any())
                        {
                            var limittype = (limitrefdatum as CyPhy.Surface).SrcConnections.KinematicTranslationalLimitReferenceCollection.First().Attributes.TranslationalLimitReferenceType;
                            // Default
                            if (limittype == CyPhyClasses.KinematicTranslationalLimitReference.AttributesClass.TranslationalLimitReferenceType_enum.NormalExtent)
                            {
                                Joint.TranslationDefaultReference = datarepdatum;
                            }
                            // Min
                            else if (limittype == CyPhyClasses.KinematicTranslationalLimitReference.AttributesClass.TranslationalLimitReferenceType_enum.MinExtent)
                            {
                                throw new Exception("Min and max references are not yet supported. Please remove these." + CyPhyImpl.Name + ", Parent: " + CyPhyImpl.ParentContainer.Name);
                            }
                            // Max
                            else
                            {
                                throw new Exception("Min and max references are not yet supported. Please remove these." + CyPhyImpl.Name + ", Parent: " + CyPhyImpl.ParentContainer.Name);
                            }
                        }
                    }
                }
                if ((Joint.RotationLimitMax.HasValue || Joint.RotationLimitMin.HasValue) && !Joint.RotationLimitDefault.HasValue)
                {
                    throw new Exception("Joint has rotation limit max or min specified, but not default. Please specify default value as well." + CyPhyImpl.Name + ", Parent: " + CyPhyImpl.ParentContainer.Name);
                }
                if (Joint.RotationLimitDefault.HasValue && Joint.RotationDefaultReference == null)
                {
                    throw new Exception("Joint has rotation limit specified, but there are no guides present to be used as rotation references. Please define guides for the connection as well." + CyPhyImpl.Name + ", Parent: " + CyPhyImpl.ParentContainer.Name);
                }
                if ((Joint.TranslationLimitMax.HasValue || Joint.TranslationLimitMin.HasValue) && !Joint.TranslationLimitDefault.HasValue)
                {
                    throw new Exception("Joint has translation limit max or min specified, but not default. Please specify default value as well." + CyPhyImpl.Name + ", Parent: " + CyPhyImpl.ParentContainer.Name);
                }
                if (Joint.TranslationLimitDefault.HasValue && Joint.TranslationDefaultReference == null)
                {
                    throw new Exception("Joint has translation limit specified, but there is no limit reference present. Please define limit references in the connection as well." + CyPhyImpl.Name + ", Parent: " + CyPhyImpl.ParentContainer.Name);
                }
                if (Joint.RotationLimitMax.HasValue && Joint.RotationLimitMin.HasValue && Joint.RotationLimitMin.Value > Joint.RotationLimitMax.Value)
                {
                    throw new Exception("Joint rotation limit min > max. Please correct this." + CyPhyImpl.Name + ", Parent: " + CyPhyImpl.ParentContainer.Name);
                }
            }
        }

        public CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum ImliedSurfTreatment
        {
            get
            {
                if (Joint == null)
                {
                    return CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Contacted;
                }
                else
                {
                    switch (Joint.JointType)
                    {
                        case KinematicJoint.KinematicJointType.CYLINDRICAL:
                        case KinematicJoint.KinematicJointType.PRISMATIC:
                        case KinematicJoint.KinematicJointType.REVOLUTE:
                        case KinematicJoint.KinematicJointType.SPHERICAL:
                            return CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Contacted;
                        case KinematicJoint.KinematicJointType.FIXED:
                            return CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Bonded;
                        case KinematicJoint.KinematicJointType.UNDEFINED:
                            return CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Contacted;
                        default:
                            return CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Contacted;
                    }
                }
            }
        }

        public void SetupAdjoiningTreatment()
        {
            if (CyPhyImpl is CyPhy.Connector)
            {
                foreach (var conn in (CyPhyImpl as CyPhy.Connector).DstConnections.AdjoiningSurfConnectionCollection.Union((CyPhyImpl as CyPhy.Connector).SrcConnections.AdjoiningSurfConnectionCollection))
                {
                    var surftreatment = conn.DstEnds.AdjoiningSurfacesTreatment;
                    if (surftreatment == null)
                    {
                        surftreatment = conn.SrcEnds.AdjoiningSurfacesTreatment;
                    }
                    this.AdjSurfTreatment = surftreatment.Attributes.Type;
                }
            }
        }

        public int GetDegreesOfFreedom()
        {
            int AxisCnt = 0, PointCnt = 0, SurfaceCnt = 0, CsysCnt = 0;
            int count = DatumList.Count(), dof = 0;

            foreach (KeyValuePair<string, Datum> item in DatumList)
            {
                string dtype = item.Value.Type.ToString();
                if (dtype == "Axis")
                    AxisCnt++;
                else if (dtype == "Point")
                    PointCnt++;
                else if (dtype == "Surface")
                    SurfaceCnt++;
                else if (dtype == "CoordinateSystem")
                    CsysCnt++;
            }

            if (CsysCnt > 0)
            {
                switch (count)
                {
                    case 0:
                        dof = 0;
                        break;
                    case 1:
                        {
                            if (AxisCnt > 0)
                                dof = 4;
                            else
                                dof = 3;
                        }
                        break;
                    case 2:
                        dof = 5;
                        break;
                    case 3:
                        dof = 6;
                        break;
                    default:
                        dof = 6;
                        break;
                }
            }
            else
                dof = 6;

            return dof;
        }

        public void PopulateStructuralInterface(CyPhy.CADModel cadmodel)
        {
            if (CyPhyImpl is CyPhy.Connector)
            {
                PopulateStructuralInterface(CyPhyImpl as CyPhy.Connector,
                                            cadmodel);
            }
            else if (CyPhyImpl is CyPhy.CADDatum)
            {
                PopulateStructuralInterface(CyPhyImpl as CyPhy.CADDatum,
                                            cadmodel);
            }
        }

        public void PopulateStructuralInterface(CyPhy.CADDatum datum,
                                                CyPhy.CADModel acadmodel)
        {
            // META-947: Creates a virtual connector
            Dictionary<string, DataRep.Datum> featuremap = new Dictionary<string, DataRep.Datum>();
            FindMatchingDatums(datum,
                               acadmodel,
                               featuremap);
            
            this.DatumList = featuremap;
            this.DegreeFreedom = GetDegreesOfFreedom();

        }


        public void PopulateStructuralInterface(CyPhy.Connector a,
                                                CyPhy.CADModel acadmodel)
        {
            // META-947: Connector instead of StructuralInterface
            // [1] Connectors can be nested so find all cad datums within a connector recursively
            // [2] Find connected datums
            //     Skip Connector without any Datum Ports

            // Limitref datums won't be considered part of the connection
            List<CyPhy.CADDatum> CadDatum_List = new List<CyPhy.CADDatum>();
            List<CyPhy.CADDatum> LimitRefDatum_List = new List<CyPhy.CADDatum>();
            FindCadDatumsInConnector(a, CadDatum_List, LimitRefDatum_List);

            Dictionary<string, DataRep.Datum> featuremap = new Dictionary<string, DataRep.Datum>();
            Dictionary<string, DataRep.Datum> limitrefmap = new Dictionary<string, DataRep.Datum>();

            foreach (CyPhy.CADDatum item in CadDatum_List)
            {
                FindMatchingDatums(item, acadmodel, featuremap);
            }
            foreach (CyPhy.CADDatum item in LimitRefDatum_List)
            {
                FindMatchingDatums(item, acadmodel, limitrefmap);
            }

            this.DatumList = featuremap;
            this.DegreeFreedom = GetDegreesOfFreedom();

            SetupJoint(CadDatum_List, LimitRefDatum_List, limitrefmap);

            SetupAdjoiningTreatment();

            foreach (var intfgeom in a.SrcConnections.InterfaceGeometryCollection.Union(a.DstConnections.InterfaceGeometryCollection))
            {
                var geom = (intfgeom.SrcEnds.GeometryTypes == null) ? intfgeom.DstEnds.GeometryTypes : intfgeom.SrcEnds.GeometryTypes;
                if (geom != null)
                {
                    Geometry.Add(CADGeometry.CreateGeometry(geom));
                }
            }
        }

        private bool FindMatchingDatums(CyPhy.CADDatum datum,
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
                    return true; // Error
                }

                // META-3232
                /*
                DataRep.Datum datumRep = new DataRep.Datum(traversal.datumFound.First().Attributes.DatumName,
                                                           datum.Kind,
                                                           this.ParentInstanceGUID, guide);
                */
                bool guide = datum.Attributes.DefinitionNotes.Contains("GUIDE");
                if (guide)
                {
                    Logger.Instance.AddLogMessage("Datum is using old guide format. Please use the attribute 'IsGuide'. [" + datum.Path + "]", Severity.Error);
                    return true; // Error
                }
                guide |= datum.Attributes.IsGuide;
                DataRep.Datum datumRep = new DataRep.Datum(traversal.datumFound.First(),
                                                           this.ParentInstanceGUID, guide);

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

            return false;
        }

        private bool IsLimitRef(CyPhy.CADDatum datum)
        {
            return datum is CyPhy.Surface &&
                ((datum as CyPhy.Surface).SrcConnections.KinematicRotationalLimitReferenceCollection.Any() ||
                (datum as CyPhy.Surface).SrcConnections.KinematicTranslationalLimitReferenceCollection.Any());
        }

        // Connectors may be nested in the future!
        private void FindCadDatumsInConnector(CyPhy.Connector connector,
                                              List<CyPhy.CADDatum> caddatum_list, // These datums are part of the interface
                                              List<CyPhy.CADDatum> caddatum_list_joint_refs) // These datums are references belonging to joints, not part of the interface
        {
            foreach (var datum in connector.Children.CADDatumCollection)
            {
                if (!IsLimitRef(datum))
                {
                    caddatum_list.Add(datum);
                }
                else
                {
                    caddatum_list_joint_refs.Add(datum);
                }
            }
        }


        private static bool MatchSIType(string a_original,
                         string b_original)
        {
            string a = a_original.ToLower();
            string b = b_original.ToLower();

            int a_size = a.Count();
            int b_size = b.Count();

            if (a == b)
            {
                return true;
            }
            else
            {
                if (a_size != b_size)
                    return false;

                for (int i = 0; i < a_size; i++)
                {
                    if (a[i] != '*' && b[i] != '*')
                        if (a[i] != b[i])
                            return false;
                }
                return true;
            }
        }


        public static bool MatchStructuralInterfaceDatums(DataRep.StructuralInterfaceConstraint a,
                                                          DataRep.StructuralInterfaceConstraint b,
                                                          List<Tuple<DataRep.Datum, DataRep.Datum>> constraintPairs,
                                                          CyPhyCOMInterfaces.IMgaTraceability Traceability)
        {
            string apath = a.CyPhyImpl.Path, bpath = b.CyPhyImpl.Path;

            // Means no error
            bool result = false;

            if (!DataRep.StructuralInterfaceConstraint.MatchSIType(a.InterfaceDefinition, b.InterfaceDefinition))         //if (!MatchSIType(a.InterfaceDefinition, b.InterfaceDefinition))
            {
                Logger.Instance.AddLogMessage("Mismatched Type attribute on connected StructuralInterfaces. Interface 1: ["
                    + a.CyPhyImpl.ToHyperLink(Traceability) + "] (" + a.InterfaceDefinition + "). Interface 2: [" + b.CyPhyImpl.ToHyperLink(Traceability) + "] (" + b.InterfaceDefinition + ")", Severity.Error);
                return true;
            }

            List<string> adatumnames = a.DatumList.Keys.ToList();
            List<string> bdatumnames = b.DatumList.Keys.ToList();

            if (adatumnames.Count() != bdatumnames.Count())
            {
                Logger.Instance.AddLogMessage("Connected StructuralInterfaces have different number of datum ports: ["
                    + a.CyPhyImpl.ToHyperLink(Traceability) + ", " + b.CyPhyImpl.ToHyperLink(Traceability) + "]", Severity.Error);
                return true;
            }

            if (adatumnames.Count() > 1 && bdatumnames.Count() > 1)
            {
                foreach (KeyValuePair<string, DataRep.Datum> adatum in a.DatumList)
                {
                    if (b.DatumList.ContainsKey(adatum.Key))
                    {
                        DataRep.DatumType a_type = adatum.Value.Type;
                        DataRep.DatumType b_type = b.DatumList[adatum.Key].Type;
                        if (a_type != b_type)
                        {
                            Logger.Instance.AddLogMessage("Matching datum ports are different type ["
                                + a.CyPhyImpl.ToHyperLink(Traceability) + "," + b.CyPhyImpl.ToHyperLink(Traceability) + "]", Severity.Error);
                            result = true;
                            continue;
                        }
                        else
                        {
                            if (a_type == DataRep.DatumType.Surface)
                            {
                                if (adatum.Value.Alignment != b.DatumList[adatum.Key].Alignment)
                                {
                                    Logger.Instance.AddLogMessage("Matching Surface datum ports have different Alignment attributes ["
                                        + a.CyPhyImpl.ToHyperLink(Traceability) + "," + b.CyPhyImpl.ToHyperLink(Traceability) + "]", Severity.Error);
                                    result = true;
                                    continue;
                                }
                            }

                            var atumple = new Tuple<DataRep.Datum, DataRep.Datum>(adatum.Value, b.DatumList[adatum.Key]);
                            constraintPairs.Add(atumple);
                        }

                        adatumnames.Remove(adatum.Key);
                        bdatumnames.Remove(adatum.Key);
                    }

                }

                if (adatumnames.Any() || bdatumnames.Any())
                {
                    Logger.Instance.AddLogMessage(String.Format("Connected connectors contain unmatched named feature ports. Port names must match inside connectors. Connectors are: {0} ({1}) and {2} ({3}).",
                        a.CyPhyImpl.ToHyperLink(Traceability), String.Join(",", adatumnames.ToArray()), b.CyPhyImpl.ToHyperLink(Traceability), String.Join(",", bdatumnames)), Severity.Error);
                    return true;
                }
            }
            else
            {
                var atuple = new Tuple<DataRep.Datum, DataRep.Datum>(a.DatumList.Values.First(), b.DatumList.Values.First());
                constraintPairs.Add(atuple);
            }
            return result;
        }


        public override string ToString()
        {
            StringBuilder sbuilder = new StringBuilder();
            sbuilder.AppendFormat("\n     <SI: {0}, DOF({1}) ",
                                  PortID,
                                  DegreeFreedom);
            foreach (var item in DatumList.Values)
            {
                sbuilder.AppendFormat("|({0},{1})",
                                      item.DatumName,
                                      item.Type.ToString());
            }
            sbuilder.AppendFormat(">");
            sbuilder.AppendLine();
            return sbuilder.ToString();
        }
    }
}
