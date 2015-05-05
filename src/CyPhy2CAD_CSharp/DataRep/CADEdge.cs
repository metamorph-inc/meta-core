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
    public class CADEdge : CADData
    {
        public int DegreeFreedom    { get; set; }
        public CADData Src   { get; set; }
        public CADData Dst   { get; set; }
        public string SrcPortID { get; set; }
        public string DstPortID { get; set; }
        public string SrcPortName { get; set; }
        public string DstPortName { get; set; }
        public StructuralInterfaceConstraint SrcIntf;
        public StructuralInterfaceConstraint DstIntf;
        public List<Tuple<DataRep.Datum, DataRep.Datum>> ConstraintPairs    { get; set; }

        // Pair of joint informations from the 2 StructuralConnections
        // These together define one CAD/MBD joint
        public KinematicJointPair KinematicJointPair { get; set; }

        public CADEdge(CADData src, CADData dst)
        {
            Type = CADDataType.Edge;
            Src = src;
            Dst = dst;
            ConstraintPairs = new List<Tuple<Datum, Datum>>();            
            Name = "Edge";
        }

        public CAD.ConstraintType ToCADXMLOutput(TestBenchModel.TestBenchBase testBenchBase)
        {
            CAD.ConstraintType constraintout = new CAD.ConstraintType();
            constraintout._id = UtilityHelpers.MakeUdmID();
            CADComponent src = Src as CADComponent;
            CADComponent dst = Dst as CADComponent;

            string srcid = src.DisplayID;
            string dstid = dst.DisplayID;

            if (ConstraintPairs.Any())
            {
                List<CAD.PairType> pairlist = new List<CAD.PairType>();

                foreach (var item in ConstraintPairs)
                {
                    List<CAD.ConstraintFeatureType> constrainfeaturelist = new List<CAD.ConstraintFeatureType>();
                    CAD.PairType pair = new CAD.PairType();
                    pair._id = UtilityHelpers.MakeUdmID();
                    pair.FeatureAlignmentType = item.Item1.Alignment;
                    pair.FeatureGeometryType = item.Item1.Type.ToString().ToUpper();
                    pair.FeatureInterfaceType = "CAD_DATUM";
                    if (item.Item1.Guide || item.Item2.Guide)
                    {
                        pair.Guide = "True";
                    }
                    
                    CAD.ConstraintFeatureType afeature = new CAD.ConstraintFeatureType();
                    afeature._id = UtilityHelpers.MakeUdmID();
                    afeature.ComponentID = item.Item1.ComponentID;
                    afeature.FeatureName = item.Item1.DatumName;
                    afeature.FeatureOrientationType = item.Item1.Orientation;
                    if (item.Item1.Marker != null)
                    {
                        afeature.GeometryMarker = item.Item1.Marker.ToCADXml();
                    }

                    CAD.ConstraintFeatureType bfeature = new CAD.ConstraintFeatureType();
                    bfeature._id = UtilityHelpers.MakeUdmID();
                    bfeature.ComponentID = item.Item2.ComponentID;
                    bfeature.FeatureName = item.Item2.DatumName;
                    bfeature.FeatureOrientationType = item.Item2.Orientation;
                    if (item.Item2.Marker != null)
                    {
                        bfeature.GeometryMarker = item.Item2.Marker.ToCADXml();
                    }

                    constrainfeaturelist.Add(afeature);
                    constrainfeaturelist.Add(bfeature);
                    pair.ConstraintFeature = constrainfeaturelist.ToArray();
                    pairlist.Add(pair);

                }
                constraintout.Pair = pairlist.ToArray();

                // META-3124
                if (KinematicJointPair != null)
                {
                   constraintout.Joint = KinematicJointPair.ToCADXMLOutput();
                }

                // Associated geomnetries
                List<CAD.GeometryType> geoms = new List<CAD.GeometryType>();
                foreach (var geom in SrcIntf.Geometry.Union(DstIntf.Geometry))
                {
                    geoms.Add(geom.ToCADXMLOutput());
                }
                constraintout.Geometry = geoms.ToArray();

                // Adjoining surface treatment (if any)
                if (testBenchBase is TestBenchModel.FEATestBench)
                {
                    string surftreatment = GetAdjoiningSurfaceTreatment(testBenchBase as TestBenchModel.FEATestBench);
                    constraintout.AdjoiningSurfaceType = surftreatment;
                }
            }


            return constraintout;
        }

        private CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum CombineAdjoiningValue(CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum input, TestBenchModel.FEATestBench testBench)
        {
            switch (input)
            {
                case CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Testbench_Default:
                    switch (testBench.AdjSurfTreatment)
                    {
                        case CyPhyClasses.CADTestBench.AttributesClass.AdjoiningTreatment_enum.Bonded:
                            return CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Bonded;
                        case CyPhyClasses.CADTestBench.AttributesClass.AdjoiningTreatment_enum.Contacted:
                            return CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Contacted;
                        case CyPhyClasses.CADTestBench.AttributesClass.AdjoiningTreatment_enum.Free:
                            return CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Free;
                        case CyPhyClasses.CADTestBench.AttributesClass.AdjoiningTreatment_enum.Joint_Implied:
                            return SrcIntf.ImliedSurfTreatment;
                    }
                    break;
                case CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Joint_Implied:
                    return SrcIntf.ImliedSurfTreatment;
                case CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Bonded:
                    return CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Bonded;
                case CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Contacted:
                    return CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Contacted;
                case CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Free:
                    return CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Free;
            }
            // Shouldn't get here
            return CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Free;
        }

        private string AdjoiningEnumToCADString(CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum input)
        {
            switch (input)
            {
                case CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Bonded:
                    return "BONDED";
                case CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Contacted:
                    return "CONTACTED";
                case CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Free:
                    return "FREE";
                default:
                    return null;
            }
        }

        private string GetAdjoiningSurfaceTreatment(TestBenchModel.FEATestBench testBench)
        {
            CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum src = CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Testbench_Default;
            CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum dst = CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Testbench_Default;
            bool srcset = false;
            bool dstset = false;
            // Are the 2 settings compatible?
            if (SrcIntf.AdjSurfTreatment.HasValue)
            {
                srcset = true;
                src = CombineAdjoiningValue(SrcIntf.AdjSurfTreatment.Value, testBench);
            }
            if (DstIntf.AdjSurfTreatment.HasValue)
            {
                dstset = true;
                dst = CombineAdjoiningValue(DstIntf.AdjSurfTreatment.Value, testBench);
            }
            if (srcset && dstset)
            {
                if (src != dst)
                {
                    Logger.Instance.AddLogMessage("Conflicting adjoning surface settings. Component A: " + SrcIntf.CyPhyImpl.ToHyperLink() + ", Type: " + src.ToString() + "; Component B: " + DstIntf.CyPhyImpl.ToHyperLink() + ", Type: " + dst.ToString(), Severity.Error);
                    return null;
                }
                else
                {
                    return AdjoiningEnumToCADString(src);
                }
            }
            else if (srcset && !dstset)
            {
                return AdjoiningEnumToCADString(src);
            }
            else if (!srcset && dstset)
            {
                return AdjoiningEnumToCADString(dst);
            }
            else
            {
                switch (testBench.AdjSurfTreatment)
                {
                    case CyPhyClasses.CADTestBench.AttributesClass.AdjoiningTreatment_enum.Bonded:
                        return AdjoiningEnumToCADString(CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Bonded);
                    case CyPhyClasses.CADTestBench.AttributesClass.AdjoiningTreatment_enum.Contacted:
                        return AdjoiningEnumToCADString(CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Contacted);
                    case CyPhyClasses.CADTestBench.AttributesClass.AdjoiningTreatment_enum.Free:
                        return AdjoiningEnumToCADString(CyPhyClasses.AdjoiningSurfacesTreatment.AttributesClass.Type_enum.Free);
                    case CyPhyClasses.CADTestBench.AttributesClass.AdjoiningTreatment_enum.Joint_Implied:
                        return AdjoiningEnumToCADString(SrcIntf.ImliedSurfTreatment);
                }
            }
            // Shouldn't get here
            return null;
        }

        public object ToMultibodyOutput()
        {
            if (KinematicJointPair != null)
            {
                if (KinematicJointPair.JointType == DataRep.KinematicJoint.KinematicJointType.PRISMATIC || KinematicJointPair.JointType == DataRep.KinematicJoint.KinematicJointType.CYLINDRICAL || KinematicJointPair.JointType == DataRep.KinematicJoint.KinematicJointType.REVOLUTE)   // Joint_One_Axis_Given
                {
                    Multibody.ModelAssemblyJointsJoint_One_Axis_Given joint = new Multibody.ModelAssemblyJointsJoint_One_Axis_Given();
                    joint.ComponentA = new Multibody.ModelAssemblyJointsJoint_One_Axis_GivenComponentA();
                    joint.ComponentA.ComponentID = Src.DisplayID;
                    joint.ComponentA.Location_MetricID = SrcPortID;
                    joint.ComponentB = new Multibody.ModelAssemblyJointsJoint_One_Axis_GivenComponentB();
                    joint.ComponentB.ComponentID = Dst.DisplayID;
                    joint.ComponentB.Location_MetricID = DstPortID;
                    joint.ID = KinematicJointPair.XmlID;

                    if (KinematicJointPair.JointType == DataRep.KinematicJoint.KinematicJointType.PRISMATIC)
                        joint.Type = Multibody.ModelAssemblyJointsJoint_One_Axis_GivenType.Translational;
                    else if (KinematicJointPair.JointType == DataRep.KinematicJoint.KinematicJointType.REVOLUTE)
                        joint.Type = Multibody.ModelAssemblyJointsJoint_One_Axis_GivenType.Revolute;
                    else
                        joint.Type = Multibody.ModelAssemblyJointsJoint_One_Axis_GivenType.Cylindrical;

                    joint.Geometry = new Multibody.ModelAssemblyJointsJoint_One_Axis_GivenGeometry();
                    joint.Geometry.MetricID = UtilityHelpers.MakeUdmID();
                    joint.Geometry.Type = "Vector";

                    return joint;
                }
                else    // Joint_Locations_Only
                {
                    Multibody.ModelAssemblyJointsJoint_Locations_Only joint = new Multibody.ModelAssemblyJointsJoint_Locations_Only();
                    joint.ComponentA = new Multibody.ModelAssemblyJointsJoint_Locations_OnlyComponentA();
                    joint.ComponentA.ComponentID = Src.DisplayID;
                    joint.ComponentA.Location_MetricID = SrcPortID;
                    joint.ComponentB = new Multibody.ModelAssemblyJointsJoint_Locations_OnlyComponentB();
                    joint.ComponentB.ComponentID = Dst.DisplayID;
                    joint.ComponentB.Location_MetricID = DstPortID;
                    joint.ID = KinematicJointPair.XmlID;

                    if (KinematicJointPair.JointType == DataRep.KinematicJoint.KinematicJointType.SPHERICAL)
                    {
                        joint.Type = Multibody.ModelAssemblyJointsJoint_Locations_OnlyType.Spherical;
                    }
                    else
                    {
                        joint.Type = Multibody.ModelAssemblyJointsJoint_Locations_OnlyType.Fixed;
                    }


                    return joint;
                }
            }
            else
                return null;
        }


        public override string ToString()
        {
            StringBuilder sbuilder = new StringBuilder();
            sbuilder.AppendFormat("[src = {0} ({1}), Port {2} | dst = {3} ({4}), Port {5}]\n",
                Src.Type,
                Src.Id,
                SrcPortID,
                Dst.Type,
                Dst.Id,
                DstPortID);
            return sbuilder.ToString();
        }

        public override string ToGraphViz(bool directed = false)
        {
            StringBuilder sbuilder = new StringBuilder();

            if (directed)
                sbuilder.AppendFormat("{0} -> {1} [label = \"DOF ({2})\"]; \n",
                    Src.GraphvizID,
                    Dst.GraphvizID,
                    DegreeFreedom);
            else
                sbuilder.AppendFormat("{0} -- {1} [label = \"DOF ({2})\"]; \n",
                    Src.GraphvizID,
                    Dst.GraphvizID,
                    DegreeFreedom);

            return sbuilder.ToString();
        }
    }   
}
