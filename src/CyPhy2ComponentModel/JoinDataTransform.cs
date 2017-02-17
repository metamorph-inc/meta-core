using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using avm;
using iFAB;
using GME.MGA;

namespace CyPhy2ComponentModel
{
    public static class JoinDataTransform
    {
        #region CyPhy to iFAB maps
        private static Dictionary<CyPhyClasses.WeldedJoin.AttributesClass.InspectionRequirement_enum, iFAB.inspectionRequirement> d_InspectionRequirement = new Dictionary<CyPhyClasses.WeldedJoin.AttributesClass.InspectionRequirement_enum, iFAB.inspectionRequirement>() {
            { CyPhyClasses.WeldedJoin.AttributesClass.InspectionRequirement_enum.Visual, iFAB.inspectionRequirement.Visual },
            { CyPhyClasses.WeldedJoin.AttributesClass.InspectionRequirement_enum.X_dash_Ray, iFAB.inspectionRequirement.XRay },
        };

        private static Dictionary<CyPhyClasses.MechanicalJoin.AttributesClass.TorqueUnit_enum, iFAB.torqueUnit> d_TorqueUnits = new Dictionary<CyPhyClasses.MechanicalJoin.AttributesClass.TorqueUnit_enum, iFAB.torqueUnit>() {
            { CyPhyClasses.MechanicalJoin.AttributesClass.TorqueUnit_enum.N_m, iFAB.torqueUnit.Nm }
        };

        private static Dictionary<CyPhyClasses.MechanicalJoin.AttributesClass.ForceUnit_enum, iFAB.forceUnit> d_ForceUnits = new Dictionary<CyPhyClasses.MechanicalJoin.AttributesClass.ForceUnit_enum, iFAB.forceUnit>() {
            { CyPhyClasses.MechanicalJoin.AttributesClass.ForceUnit_enum.N, iFAB.forceUnit.N }
        };

        private static Dictionary<CyPhyClasses.MechanicalJoin.AttributesClass.FasteningMethod_enum, iFAB.fasteningMethod> d_FasteningMethod = new Dictionary<CyPhyClasses.MechanicalJoin.AttributesClass.FasteningMethod_enum, iFAB.fasteningMethod>() {
            { CyPhyClasses.MechanicalJoin.AttributesClass.FasteningMethod_enum.Bolted, iFAB.fasteningMethod.Bolted }
        };
        #endregion

        #region iFAB to CyPhy maps
        private static Dictionary<iFAB.inspectionRequirement, CyPhyClasses.WeldedJoin.AttributesClass.InspectionRequirement_enum> d_InspectionRequirement_Reverse = new Dictionary<inspectionRequirement, CyPhyClasses.WeldedJoin.AttributesClass.InspectionRequirement_enum>() {
            {  iFAB.inspectionRequirement.Visual, CyPhyClasses.WeldedJoin.AttributesClass.InspectionRequirement_enum.Visual },
            {  iFAB.inspectionRequirement.XRay, CyPhyClasses.WeldedJoin.AttributesClass.InspectionRequirement_enum.X_dash_Ray },
        };

        private static Dictionary<iFAB.torqueUnit, CyPhyClasses.MechanicalJoin.AttributesClass.TorqueUnit_enum> d_TorqueUnits_Reverse = new Dictionary<torqueUnit, CyPhyClasses.MechanicalJoin.AttributesClass.TorqueUnit_enum>() {
            { iFAB.torqueUnit.Nm, CyPhyClasses.MechanicalJoin.AttributesClass.TorqueUnit_enum.N_m }
        };

        private static Dictionary<iFAB.forceUnit, CyPhyClasses.MechanicalJoin.AttributesClass.ForceUnit_enum> d_ForceUnits_Reverse = new Dictionary<forceUnit, CyPhyClasses.MechanicalJoin.AttributesClass.ForceUnit_enum>() {
            { iFAB.forceUnit.N, CyPhyClasses.MechanicalJoin.AttributesClass.ForceUnit_enum.N }
        };

        private static Dictionary<iFAB.fasteningMethod, CyPhyClasses.MechanicalJoin.AttributesClass.FasteningMethod_enum> d_FasteningMethod_Reverse = new Dictionary<fasteningMethod, CyPhyClasses.MechanicalJoin.AttributesClass.FasteningMethod_enum>() {
            { iFAB.fasteningMethod.Bolted, CyPhyClasses.MechanicalJoin.AttributesClass.FasteningMethod_enum.Bolted }
        };
        #endregion

        private static string IDPrefix = "jd";

        public static List<CyPhy.JoinData> TransformJoinData(iFAB.assemblyDetail ad, ISIS.GME.Common.Interfaces.Container parent)
        {
            var rtn = new List<CyPhy.JoinData>();

            foreach (var join in ad.Items)
            {
                CyPhy.JoinData jd = null;
                if (join is welded)
                {
                    var w = join as welded;
                    if (parent is CyPhy.Connector)
                    {
                        jd = CyPhyClasses.WeldedJoin.Create(parent as CyPhy.Connector);
                    }
                    else
                    {
                        jd = CyPhyClasses.WeldedJoin.CreateGeneric(parent) as CyPhy.WeldedJoin;
                    }
                }
                else if (join is brazed)
                {
                    var w = join as brazed;
                    if (parent is CyPhy.Connector)
                    {
                        jd = CyPhyClasses.BrazedJoin.Create(parent as CyPhy.Connector);
                    }
                    else
                    {
                        jd = CyPhyClasses.BrazedJoin.CreateGeneric(parent) as CyPhy.BrazedJoin;
                    }
                }
                else if (join is soldered)
                {
                    var w = join as soldered;
                    if (parent is CyPhy.Connector)
                    {
                        jd = CyPhyClasses.SolderedJoin.Create(parent as CyPhy.Connector);
                    }
                    else
                    {
                        jd = CyPhyClasses.SolderedJoin.CreateGeneric(parent) as CyPhy.SolderedJoin;
                    }
                }
                else if (join is glued)
                {
                    var w = join as glued;
                    if (parent is CyPhy.Connector)
                    {
                        jd = CyPhyClasses.GluedJoin.Create(parent as CyPhy.Connector);
                    }
                    else
                    {
                        jd = CyPhyClasses.GluedJoin.CreateGeneric(parent) as CyPhy.GluedJoin;
                    }
                }
                else if (join is mechanical)
                {
                    var w = join as mechanical;
                    if (parent is CyPhy.Connector)
                    {
                        jd = CyPhyClasses.MechanicalJoin.Create(parent as CyPhy.Connector);
                    }
                    else
                    {
                        jd = CyPhyClasses.MechanicalJoin.CreateGeneric(parent) as CyPhy.MechanicalJoin;
                    }
                }
                else
                {
                    continue;
                }

                jd.Attributes.Description = ad.id;
                rtn.Add(jd);
            }

            return rtn;
        }

        public static iFAB.assemblyDetail TransformJoinData(CyPhy.JoinData jd)
        {
            var rtn = new iFAB.assemblyDetail()
            {
                id = IDPrefix + jd.Guid.ToString(),
                name = jd.Name,
                description = jd.Attributes.Description,
                Items = new List<object>()
            };

            if (jd is CyPhy.WeldedJoin || jd.Kind == "WeldedJoin")
            {
                CyPhy.WeldedJoin wj = jd as CyPhy.WeldedJoin;
                if (wj == null)
                {
                    wj = CyPhyClasses.WeldedJoin.Cast(jd.Impl);
                }

                iFAB.welded w = new iFAB.welded()
                {
                    inspectionRequirement = d_InspectionRequirement[wj.Attributes.InspectionRequirement],
                    jointType = (iFAB.jointType)Enum.Parse(typeof(iFAB.jointType), wj.Attributes.JointType.ToString()),
                    length = new iFAB.length()
                    {
                        Value = decimal.Parse(wj.Attributes.Length.ToString()),
                        unit = (iFAB.lengthUnit)Enum.Parse(typeof(iFAB.lengthUnit), wj.Attributes.LengthUnit.ToString())
                    },
                    weldPenetration = (iFAB.weldPenetration)Enum.Parse(typeof(iFAB.weldPenetration), wj.Attributes.WeldPenetration.ToString()),
                    weldType = (iFAB.weldType)Enum.Parse(typeof(iFAB.weldType), wj.Attributes.WeldType.ToString()),
                    twoSided = wj.Attributes.TwoSided
                };

                rtn.Items.Add(w);
            }
            else if (jd is CyPhy.BrazedJoin || jd.Kind == "BrazedJoin")
            {
                CyPhy.BrazedJoin bj = jd as CyPhy.BrazedJoin;
                if (bj == null)
                {
                    bj = CyPhyClasses.BrazedJoin.Cast(jd.Impl);
                }

                iFAB.brazed b = new iFAB.brazed()
                {
                    fillerMaterial = bj.Attributes.FillerMaterial,
                    fluxMaterial = bj.Attributes.FluxMaterial,
                    length = new iFAB.length()
                    {
                        Value = decimal.Parse(bj.Attributes.Length.ToString()),
                        unit = (iFAB.lengthUnit)Enum.Parse(typeof(iFAB.lengthUnit), bj.Attributes.LengthUnit.ToString()),
                    }
                };

                rtn.Items.Add(b);
            }
            else if (jd is CyPhy.SolderedJoin || jd.Kind == "SolderedJoin")
            {
                CyPhy.SolderedJoin sj = jd as CyPhy.SolderedJoin;
                if (sj == null)
                {
                    sj = CyPhyClasses.SolderedJoin.Cast(jd.Impl);
                }

                iFAB.soldered s = new iFAB.soldered()
                {
                    fillerMaterial = sj.Attributes.FillerMaterial,
                    fluxMaterial = sj.Attributes.FluxMaterial,
                    length = new iFAB.length()
                    {
                        Value = decimal.Parse(sj.Attributes.Length.ToString()),
                        unit = (iFAB.lengthUnit)Enum.Parse(typeof(iFAB.lengthUnit), sj.Attributes.LengthUnit.ToString())
                    }
                };

                rtn.Items.Add(s);
            }
            else if (jd is CyPhy.GluedJoin || jd.Kind == "GluedJoin")
            {
                CyPhy.GluedJoin gj = jd as CyPhy.GluedJoin;
                if (gj == null)
                {
                    gj = CyPhyClasses.GluedJoin.Cast(jd.Impl);
                }

                iFAB.glued g = new iFAB.glued()
                {
                    length = new iFAB.length()
                    {
                        Value = decimal.Parse(gj.Attributes.Length.ToString()),
                        unit = (iFAB.lengthUnit)Enum.Parse(typeof(iFAB.lengthUnit), gj.Attributes.LengthUnit.ToString())
                    },
                    material = gj.Attributes.Material,
                    volume = new iFAB.volume()
                    {
                        Value = decimal.Parse(gj.Attributes.Volume.ToString()),
                        unit = (iFAB.volumeUnit)Enum.Parse(typeof(iFAB.volumeUnit), gj.Attributes.VolumeUnit.ToString())
                    }
                };

                rtn.Items.Add(g);
            }
            else if (jd is CyPhy.MechanicalJoin || jd.Kind == "MechanicalJoin")
            {
                CyPhy.MechanicalJoin mj = jd as CyPhy.MechanicalJoin;
                if (mj == null)
                {
                    mj = CyPhyClasses.MechanicalJoin.Cast(jd.Impl);
                }

                iFAB.mechanical m = new iFAB.mechanical()
                {
                    fasteningMethod = d_FasteningMethod[mj.Attributes.FasteningMethod],
                    fasteningQuantity = mj.Attributes.FasteningQuantity.ToString(),
                    force = new iFAB.force()
                    {
                        Value = decimal.Parse(mj.Attributes.Force.ToString()),
                        unit = d_ForceUnits[mj.Attributes.ForceUnit]
                    },
                    torque = new iFAB.torque()
                    {
                        Value = decimal.Parse(mj.Attributes.Torque.ToString()),
                        unit = d_TorqueUnits[mj.Attributes.TorqueUnit]
                    },
                    linkingPart = new List<partReference>()
                };

                foreach (CyPhy.Fastener f in mj.Children.FastenerCollection)
                {
                    iFAB.partReference pr = new partReference();

                    // If this refers to a Component model, get that guy's AVMID.
                    // If not, use the AVMID provided in the attribute of this object.
                    if (f.Referred.Component != null)
                    {
                        pr.id = f.Referred.Component.Attributes.AVMID;
                    }
                    else
                    {
                        pr.id = f.Attributes.AVMID;
                    }

                    for (int i = 0; i < f.Attributes.Quantity; i++)
                        m.linkingPart.Add(pr);
                }

                rtn.Items.Add(m);
            }
            else
            {
                throw new NotSupportedException("JoinData export not supported for object of type " + jd.Impl.MetaBase.Name);
            }

            return rtn;
        }
    }
}
