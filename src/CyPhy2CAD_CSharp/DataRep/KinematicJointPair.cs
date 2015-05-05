using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CyPhy2CAD_CSharp.DataRep
{
    // This is the joint which goes into the XML file
    // It consists of 2 "half" joints
    public class KinematicJointPair
    {
        public string XmlID { get; set; } // ID used in the xml output file
        public KinematicJoint A;
        public KinematicJoint B;

        public KinematicJoint.KinematicJointType JointType
        {
            get
            {
                return A.JointType;
            }
        }

        public KinematicJointPair(KinematicJoint a, KinematicJoint b)
        {
            XmlID = UtilityHelpers.MakeUdmID();
            if (a.TopLevelID != null)
            {
                XmlID = a.TopLevelID;
            }
            else if (b.TopLevelID != null)
            {
                XmlID = b.TopLevelID;
            }
            A = a;
            B = b;
        }

        public CAD.JointType ToCADXMLOutput()
        {
            CAD.JointType jointOut = new CAD.JointType();
            jointOut._id = UtilityHelpers.MakeUdmID();

            /*
            if (JointGeometry.Count > 0)
            {
                jointOut.ConstraintPairs = new CAD.ConstraintPairsType();
                jointOut.ConstraintPairs._id = UtilityHelpers.MakeUdmID();

                List<CAD.JointPairType> jointPairs = new List<CAD.JointPairType>();
                foreach (var item in JointGeometry)
                {
                    CAD.JointPairType pair = new CAD.JointPairType();
                    pair._id = UtilityHelpers.MakeUdmID();
                    pair.PairID = item.Key;
                    pair.Type = item.Value;
                }
                jointOut.ConstraintPairs.JointPair = jointPairs.ToArray();
            }
            */

            jointOut.ID = XmlID;
            jointOut.Type = A.JointType.ToString();

            List<CAD.LimitType> limits = new List<CAD.LimitType>();

            double? RotationLimitDefault = A.RotationLimitDefault;
            if (!RotationLimitDefault.HasValue) RotationLimitDefault = B.RotationLimitDefault;

            if (RotationLimitDefault.HasValue)
            {
                CAD.LimitType limit = new CAD.LimitType();
                limit.Role = "RotationDefault";
                limit.Value = RotationLimitDefault.Value;
                limits.Add(limit);
            }

            double? RotationLimitMax = A.RotationLimitMax;
            if (!RotationLimitMax.HasValue) RotationLimitMax = B.RotationLimitMax;

            if (RotationLimitMax.HasValue)
            {
                CAD.LimitType limit = new CAD.LimitType();
                limit.Role = "RotationMax";
                limit.Value = RotationLimitMax.Value;
                limits.Add(limit);
            }

            
            double? RotationLimitMin = A.RotationLimitMin;
            if (!RotationLimitMin.HasValue) RotationLimitMin = B.RotationLimitMin;

            if (RotationLimitMin.HasValue)
            {
                CAD.LimitType limit = new CAD.LimitType();
                limit.Role = "RotationMin";
                limit.Value = RotationLimitMin.Value;
                limits.Add(limit);
            }

            double? TranslationLimitDefault = A.TranslationLimitDefault;
            if (!TranslationLimitDefault.HasValue) TranslationLimitDefault = B.TranslationLimitDefault;

            if (TranslationLimitDefault.HasValue)
            {
                CAD.LimitType limit = new CAD.LimitType();
                limit.Role = "TranslationDefault";
                limit.Value = TranslationLimitDefault.Value;
                limits.Add(limit);
            }

            double? TranslationLimitMax = A.TranslationLimitMax;
            if (!TranslationLimitMax.HasValue) TranslationLimitMax = B.TranslationLimitMax;

            if (TranslationLimitMax.HasValue)
            {
                CAD.LimitType limit = new CAD.LimitType();
                limit.Role = "TranslationMax";
                limit.Value = TranslationLimitMax.Value;
                limits.Add(limit);
            }

            double? TranslationLimitMin = A.TranslationLimitMin;
            if (!TranslationLimitMin.HasValue) TranslationLimitMin = B.TranslationLimitMin;
            
            if (TranslationLimitMin.HasValue)
            {
                CAD.LimitType limit = new CAD.LimitType();
                limit.Role = "TranslationMin";
                limit.Value = TranslationLimitMin.Value;
                limits.Add(limit);
            }

            jointOut.Limit = limits.ToArray();

            List<CAD.LimitReferenceType> limitrefs = new List<CAD.LimitReferenceType>();

            if (A.RotationDefaultReference!=null)
            {
                limitrefs.Add(CreateLimitRef(A.RotationDefaultReference, "RotationDefault"));
            }

            if (B.RotationDefaultReference!=null)
            {
                limitrefs.Add(CreateLimitRef(B.RotationDefaultReference, "RotationDefault"));
            }

            if (A.TranslationDefaultReference != null)
            {
                limitrefs.Add(CreateLimitRef(A.TranslationDefaultReference, "TranslationDefault"));
            }

            if (B.TranslationDefaultReference != null)
            {
                limitrefs.Add(CreateLimitRef(B.TranslationDefaultReference, "TranslationDefault"));
            }

            jointOut.LimitReference = limitrefs.ToArray();

            List<CAD.GeometryMarkerType> markers = new List<CAD.GeometryMarkerType>();
            
            if (A.Marker != null)
            {
                markers.Add(A.Marker.ToCADXml());
            }
            
            if (B.Marker != null)
            {
                markers.Add(B.Marker.ToCADXml());
            }
            
            if (markers.Count > 0)
                jointOut.GeometryMarker = markers.ToArray();

            return jointOut;
        }

        private CAD.LimitReferenceType CreateLimitRef(Datum d, string role)
        {
            CAD.LimitReferenceType limitref = new CAD.LimitReferenceType();
            limitref.ComponentID = d.ComponentID;
            limitref.FeatureGeometryType = d.Type.ToString().ToUpper();
            limitref.FeatureName = d.DatumName;
            limitref.Role = role;
            return limitref;
        }
    }
    
}
