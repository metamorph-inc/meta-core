using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using GME.MGA;
using GME.CSharp;
using GME;
using GME.MGA.Meta;
using CyPhy2CAD_CSharp.DataRep;

namespace CyPhy2CAD_CSharp.DataRep
{
    public class CADGeometry
    {
        public enum FeatureTypeEnum
        {
            POINT,
            SURFACE
        }

        public class GeometryFeature
        {
            public string ComponentID;
            public string DatumName;
            public string MetricID; // Only used if this has been requested as a metric as well
        }

        public string FeatureID { get; set; }
        public string PrimaryQualifier { get; set; }
        public string SecondaryQualifier { get; set; }
        public string GeometryType { get; set; }
        public List<GeometryFeature> GeometryFeatures;
        private FeatureTypeEnum featureType;

        public CADGeometry()
        {
            GeometryFeatures = new List<GeometryFeature>();
            FeatureID = "";
            PrimaryQualifier = "";
            SecondaryQualifier = "";
            GeometryType = "";
            featureType = FeatureTypeEnum.POINT;
        }

        public void SetGeometryType(string type)
        {
            if (type == "Circle")
                GeometryType = "CIRCLE";
            else if (type == "Cylinder")
                GeometryType = "CYLINDER";
            else if (type == "Sphere")
                GeometryType = "SPHERE";
            else if (type == "Polygon")
                GeometryType = "POLYGON";
            else if (type == "Extrusion")
                GeometryType = "EXTRUSION";
            else if (type == "Face")
                GeometryType = "Face";

            //GeometryType = "CONCENTRIC_CIRCLES";       // not supported
        }

        public void CreateGeometryFeature(string featureName,
                                          string componentID)
        {
            GeometryFeatures.Add(new GeometryFeature() { ComponentID = componentID, DatumName = featureName, MetricID = componentID+":"+featureName });
        }

        public void CreateGeometryFeature(List<KeyValuePair<string, string>> featureList)
        {
            featureList.ForEach(f => GeometryFeatures.Add(new GeometryFeature() { ComponentID = f.Value, DatumName = f.Key, MetricID = f.Value+":"+f.Key }));
        }

        public static List<MgaFCO> FindByRole(MgaModel mgaModel,
                                string roleName)
        {
            return mgaModel.ChildFCOs.Cast<MgaFCO>().Where(x => x.MetaRole.Name == roleName).ToList();
        }

        public static CADGeometry CreateGeometry(CyPhy.GeometryTypes geometryBase)
        {
            List<KeyValuePair<string, string>> featureList = new List<KeyValuePair<string, string>>();
            string geometryType = geometryBase.Kind;
            CADGeometry geometryRep = new CADGeometry
            {
                FeatureID = geometryBase.ID,
                GeometryType = geometryType.ToUpper(),
                PrimaryQualifier = geometryBase.Attributes.BoundaryQualifier.ToString().Replace("_", string.Empty)
            };
            if (geometryType == "Sphere")
            {
                CyPhy.Sphere sphere = CyPhyClasses.Sphere.Cast(geometryBase.Impl);
                if (AddSphere(sphere, featureList))
                    geometryRep.CreateGeometryFeature(featureList);
                else
                    geometryRep = null;

            }
            else if (geometryType == "Cylinder")
            {
                CyPhy.Cylinder cylinder = CyPhyClasses.Cylinder.Cast(geometryBase.Impl);
                geometryRep.SecondaryQualifier = cylinder.Attributes.EndCapInclusionQualifier.ToString();
                if (AddCylinder(cylinder, featureList))
                    geometryRep.CreateGeometryFeature(featureList);
                else
                    geometryRep = null;
            }
            else if (geometryType == "Extrusion")
            {
                CyPhy.Extrusion extrusion = CyPhyClasses.Extrusion.Cast(geometryBase.Impl);
                if (AddExtrusion(extrusion, featureList))
                    geometryRep.CreateGeometryFeature(featureList);
                else
                    geometryRep = null;
            }
            else if (geometryType == "Polygon")
            {
                CyPhy.Polygon polygon = CyPhyClasses.Polygon.Cast(geometryBase.Impl);
                if (AddPolygon(polygon, featureList))
                    geometryRep.CreateGeometryFeature(featureList);
                else
                    geometryRep = null;
            }
            else if (geometryType == "Circle")
            {
                CyPhy.Circle circle = CyPhyClasses.Circle.Cast(geometryBase.Impl);
                if (AddCircle(circle, featureList))
                    geometryRep.CreateGeometryFeature(featureList);
                else
                    geometryRep = null;
            }
            else if (geometryType == "Face")
            {
                CyPhy.Face face = CyPhyClasses.Face.Cast(geometryBase.Impl);
                CADGeometry.FeatureTypeEnum type;
                if (AddFace(face,
                            featureList,
                            out type))
                {
                    geometryRep.featureType = type;
                    geometryRep.CreateGeometryFeature(featureList);
                }
                else
                    geometryRep = null;
            }
            else
            {
                Logger.Instance.AddLogMessage("Geometry shape not supported [" + geometryType + "]", Severity.Warning);
                geometryRep = null;
            }

            return geometryRep;
        }


        private static bool AddCylinder(CyPhy.Cylinder cylinder,
                                 List<KeyValuePair<string, string>> featureList)
        {
            bool status = true;
            if (cylinder != null)
            {
                // start, end, radius
                MgaModel cylinderMga = cylinder.Impl as MgaModel;
                List<MgaFCO> startPts = new List<MgaFCO>();
                startPts = FindByRole(cylinderMga, "CylinderStart");
                List<MgaFCO> endPts = new List<MgaFCO>();
                endPts = FindByRole(cylinderMga, "CylinderEnd");
                List<MgaFCO> radiusPts = new List<MgaFCO>();
                radiusPts = FindByRole(cylinderMga, "CylinderRadius");

                if (startPts.Count != 1 || endPts.Count != 1 || radiusPts.Count != 1)
                {
                    Logger.Instance.AddLogMessage(String.Format("Cylinder geometry must contain 1 CylinderStart, 1 CylinderEnd, and 1 CylinderRadius: {0}", cylinder.ToHyperLink()), Severity.Error);
                    return false;
                }

                CyPhy.Point startPt = CyPhyClasses.Point.Cast(startPts[0]);
                if (!CreateFeatureFromPoint(startPt,
                                       featureList))
                {
                    Logger.Instance.AddLogMessage(String.Format("Cylinder geometry's CylinderStart point must connect to a Point datum inside a CADModel: {0}", cylinder.ToHyperLink()), Severity.Error);
                    return false;
                }
                CyPhy.Point endPt = CyPhyClasses.Point.Cast(endPts[0]);
                if (!CreateFeatureFromPoint(endPt,
                                       featureList))
                {
                    Logger.Instance.AddLogMessage(String.Format("Cylinder geometry's CylinderEnd point must connect to a Point datum inside a CADModel: {0}", cylinder.ToHyperLink()), Severity.Error);
                    return false;
                }
                CyPhy.Point radiusPt = CyPhyClasses.Point.Cast(radiusPts[0]);
                if (!CreateFeatureFromPoint(radiusPt,
                                       featureList))
                {
                    Logger.Instance.AddLogMessage(String.Format("Cylinder geometry's CylinderRadius point must connect to a Point datum inside a CADModel: {0}", cylinder.ToHyperLink()), Severity.Error);
                    return false;
                }
            }
            else
                status = false;

            //var height = (extrusion.Impl as MgaModel).ChildFCOs.Cast<MgaFCO>().Where(x => x.MetaRole.Name == "ExtrusionHeight").FirstOrDefault();

            return status;
        }

        private static bool AddCircle(CyPhy.Circle circle, List<KeyValuePair<string, string>> featureList)
        {
            bool status = true;

            List<MgaFCO> edgePts = new List<MgaFCO>();
            edgePts = FindByRole((circle.Impl as MgaModel), "CircleEdge");
            List<MgaFCO> centerPts = new List<MgaFCO>();
            centerPts = FindByRole((circle.Impl as MgaModel), "CircleCenter");

            if (centerPts.Count != 1 || edgePts.Count != 2)
            {
                Logger.Instance.AddLogMessage(String.Format("Circle geometry must contain 2 CircleEdge and 1 CircleCenter points: {0}", circle.ToHyperLink()), Severity.Error);
                return false;
            }


            CyPhy.Point centerPt = CyPhyClasses.Point.Cast(centerPts[0]);
            CyPhy.Point edge1 = CyPhyClasses.Point.Cast(edgePts[0]);
            CyPhy.Point edge2 = CyPhyClasses.Point.Cast(edgePts[1]);

            // Should be checked
            /*if (edge1.Guid.Equals(edge2.Guid))
            {
                Logger.Instance.AddLogMessage(String.Format("Circle is defined by equivalend edge points. These 2 points must be different to be able to identify a plane: {0}", circle.ToHyperLink()), Severity.Error);
                return false;
            }*/

            if (!CreateFeatureFromPoint(centerPt,
                                   featureList))
            {
                Logger.Instance.AddLogMessage(String.Format("Circle geometry's CircleCenter point must connect to a Point datum inside a CADModel: {0}", circle.ToHyperLink()), Severity.Error);
                return false;
            }
            if (!CreateFeatureFromPoint(edge1,
                                   featureList))
            {
                Logger.Instance.AddLogMessage(String.Format("Circle geometry's CircleEdge point must connect to a Point datum inside a CADModel: {0}", circle.ToHyperLink()), Severity.Error);
                return false;
            }

            if (!CreateFeatureFromPoint(edge2,
                                   featureList))
            {
                Logger.Instance.AddLogMessage(String.Format("Circle geometry's CircleEdge point must connect to a Point datum inside a CADModel: {0}", circle.ToHyperLink()), Severity.Error);
                return false;
            }

            return status;
        }

        private static bool AddSphere(CyPhy.Sphere sphere, List<KeyValuePair<string, string>> featureList)
        {
            List<MgaFCO> centerPts = new List<MgaFCO>();
            centerPts = FindByRole((sphere.Impl as MgaModel), "SphereCenter");
            List<MgaFCO> edgePts = new List<MgaFCO>();
            edgePts = FindByRole((sphere.Impl as MgaModel), "SphereEdge");

            if (centerPts.Count != 1 || edgePts.Count != 1)
            {
                Logger.Instance.AddLogMessage(String.Format("Sphere geometry must contain 1 SphereEdge and 1 SphereCenter: {0}", sphere.ToHyperLink()), Severity.Error);
                return false;
            }

            CyPhy.Point centerPt = CyPhyClasses.Point.Cast(centerPts[0]);
            if (!CreateFeatureFromPoint(centerPt,
                                   featureList))
            {
                Logger.Instance.AddLogMessage(String.Format("Sphere geometry's SphereCenter point must connect to a Point datum inside a CADModel: {0}", sphere.ToHyperLink()), Severity.Error);
                return false;
            }

            CyPhy.Point edge = CyPhyClasses.Point.Cast(edgePts[0]);
            if (!CreateFeatureFromPoint(edge,
                                   featureList))
            {
                Logger.Instance.AddLogMessage(String.Format("Sphere geometry's SphereEdge point must connect to a Point datum inside a CADModel: {0}", sphere.ToHyperLink()), Severity.Error);
                return false;
            }

            return true;
        }

        private static bool AddPolygon(CyPhy.Polygon polygon, List<KeyValuePair<string, string>> featureList)
        {
            bool status = true;
            List<CyPhy.OrdinalPoint> ordinalPt = polygon.Children.OrdinalPointCollection.OrderBy(x => x.Attributes.PolygonOrdinalPosition).ToList();
            if (ordinalPt.Count < 3)
                return false;

            foreach (var item in ordinalPt)
            {
                if (!CreateFeatureFromPoint(item,
                                       featureList))
                {
                    Logger.Instance.AddLogMessage(String.Format("Polygon geometry's OrdinalPoint must connect to a Point datum inside a CADModel: {0}", polygon.ToHyperLink()), Severity.Error);
                    return false;
                }
            }

            return status;
        }

        private static bool AddExtrusion(CyPhy.Extrusion extrusion, List<KeyValuePair<string, string>> featureList)           // only extrusion with polygon is supported
        {
            if (extrusion.Children.PolygonCollection.Any())
            {
                CyPhy.Polygon polygon = extrusion.Children.PolygonCollection.FirstOrDefault();
                if (polygon != null)
                {
                    AddPolygon(polygon,
                               featureList);
                    List<MgaFCO> height = FindByRole((extrusion.Impl as MgaModel), "ExtrusionHeight");
                    if (height.Count < 1)
                    {
                        Logger.Instance.AddLogMessage(String.Format("Extrusion geometry must contain exactly 1 ExtrusionHeight point: {0}", extrusion.ToHyperLink()), Severity.Error);
                        return false;
                    }

                    CyPhy.Point offset = CyPhyClasses.Point.Cast(height[0]);
                    if (CreateFeatureFromPoint(offset,
                                           featureList))
                        return true;
                    else
                    {
                        Logger.Instance.AddLogMessage(String.Format("Extrusion geometry's ExtrusionHeight must connect to a Point datum inside a CADModel: {0}", extrusion.ToHyperLink()), Severity.Error);
                        return false;
                    }
                }
            }

            return false;
        }

        private static bool AddFace(CyPhy.Face face, List<KeyValuePair<string, string>> featureList, out CADGeometry.FeatureTypeEnum type)
        {
            List<MgaFCO> referencePts = new List<MgaFCO>();
            referencePts = FindByRole(face.Impl as MgaModel, "ReferencePoint");
            int ptCnt = referencePts.Count();
            int surfCnt = face.Children.SurfaceCollection.Count();

            type = CADGeometry.FeatureTypeEnum.POINT;
            if ((ptCnt + surfCnt) > 1)
            {
                // insert error message
                Logger.Instance.AddLogMessage(String.Format("Face geometry must contain a ReferencePoint or a ReferenceSurface: {0}", face.ToHyperLink()), Severity.Error);
                return false;
            }

            if (ptCnt > 0)          // Plane
            {
                type = CADGeometry.FeatureTypeEnum.POINT;
                CyPhy.Point point = CyPhyClasses.Point.Cast(referencePts.First());
                if (CreateFeatureFromPoint(point,
                                           featureList))
                    return true;
                else
                {
                    Logger.Instance.AddLogMessage(String.Format("Face geometry's ReferencePoint must connect to a Point datum inside a CADModel: {0}", face.ToHyperLink()), Severity.Error);
                    return false;
                }

            }

            if (surfCnt > 0)        // Surface
            {
                type = CADGeometry.FeatureTypeEnum.SURFACE;
                CyPhy.Surface surface = face.Children.SurfaceCollection.First();
                if (CreateFeatureFromPoint(surface,
                                           featureList))
                    return true;
                else
                {
                    Logger.Instance.AddLogMessage(String.Format("Face geometry's ReferenceSurface must connect to a Surface datum inside a CADModel: {0}", face.ToHyperLink()), Severity.Error);
                    return false;
                }
            }

            return false;

        }

        private static bool CreateFeatureFromPoint(CyPhy.Port port, List<KeyValuePair<string, string>> featureList)
        {
            PointMetricTraversal traverser = new PointMetricTraversal(port);
            if (traverser.portsFound.Count() == 1)
            {
                CyPhy.Port foundPort = traverser.portsFound.First();
                string datumName = "";
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

                KeyValuePair<string, string> pair = new KeyValuePair<string, string>(datumName,
                                                                                     CyPhyClasses.Component.Cast(foundPort.ParentContainer.ParentContainer.Impl).Attributes.InstanceGUID);
                featureList.Add(pair);
                return true;
            }
            else
                return false;
        }

        public CAD.GeometryType ToCADXMLOutput()
        {
            CAD.GeometryType geometryout = new CAD.GeometryType();
            geometryout._id = UtilityHelpers.MakeUdmID();

            CAD.FeaturesType featuresout = new CAD.FeaturesType();
            featuresout._id = UtilityHelpers.MakeUdmID();
            featuresout.FeatureInterfaceType = "CAD_DATUM";         // fixed
            featuresout.FeatureGeometryType = featureType.ToString();              //featuresout.FeatureGeometryType = "POINT";                    
            featuresout.GeometryType = GeometryType;
            featuresout.FeatureID = FeatureID;
            featuresout.PrimaryGeometryQualifier = PrimaryQualifier;
            featuresout.SecondaryGeometryQualifier = SecondaryQualifier;

            List<CAD.FeatureType> featureList = new List<CAD.FeatureType>();
            foreach (var item in GeometryFeatures)
            {
                CAD.FeatureType feature = new CAD.FeatureType
                {
                    _id = UtilityHelpers.MakeUdmID(),
                    ComponentID = item.ComponentID,
                    Name = item.DatumName,
                    MetricID = item.MetricID
                };
                featureList.Add(feature);
            }
            featuresout.Feature = featureList.ToArray();

            geometryout.Features = new CAD.FeaturesType[1];
            geometryout.Features[0] = featuresout;
            return geometryout;
        }


    }
}
