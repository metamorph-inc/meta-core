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

namespace CyPhy2CAD_CSharp.TestBenchModel
{
    public class FEATestBench : TestBenchBase
    {
        public string SolverType { get; set; }
        public string ElementType { get; set; }
        public string ShellType { get; set; }
        public string MeshType { get; set; }
        public int MaxAdaptiveIterations { get; set; }
        private List<FEALoadBase> Loads = new List<FEALoadBase>();
        private List<FEAConstraintBase> Constraints = new List<FEAConstraintBase>();
        public List<string> PostProcessScripts = new List<string>();
        public List<FEAThermalElement> ThermalElements = new List<FEAThermalElement>();
        public CyPhyClasses.CADTestBench.AttributesClass.AdjoiningTreatment_enum AdjSurfTreatment;
        public string FEAAnalysisType;
        public List<object> TestBenchParameterMappings = new List<object>();

        //public List<TBComputation> StaticComputations = new List<TBComputation>();

        protected CyPhy.CADTestBench CyphyTestBenchRef;

        protected CAD.MeshParametersType cadMeshParameters;


        public FEATestBench (CyPhy2CADSettings cadSetting,
                                  string outputdir,
                                  string projectdir,
                                  bool auto = false) :
                                  base(cadSetting, outputdir, projectdir, auto)
        {
        }

        private void CollectLeafComponents(List<CyPhy.Component> result, CyPhy.ComponentAssembly assembly)
        {
            foreach (var compref in assembly.Children.ComponentRefCollection)
            {
                if (compref.AllReferred != null)
                {
                    if (compref.AllReferred is CyPhy.ComponentAssemblyRef)
                    {
                        CollectLeafComponents(result, compref.AllReferred as CyPhy.ComponentAssembly);
                    }
                    else if (compref.AllReferred is CyPhy.Component)
                    {
                        // Interested in components with CAD Model only
                        if ((compref.AllReferred as CyPhy.Component).Children.CADModelCollection.Any())
                            result.Add(compref as CyPhy.Component);
                    }
                }
            }
            foreach (var compass in assembly.Children.ComponentAssemblyCollection)
            {
                CollectLeafComponents(result, compass);
            }
            foreach (var comp in assembly.Children.ComponentCollection)
            {
                // Interested in components with CAD Model only
                if ((comp as CyPhy.Component).Children.CADModelCollection.Any())
                    result.Add(comp);
            }
        }

        private bool GetParamUnitName(CyPhy.Parameter param, ref string unit)
        {
            if (param.AllReferred as CyPhy.unit != null)
            {
                unit = (param.AllReferred as CyPhy.unit).Attributes.Symbol;
                return true;
            } else {
                return false;
            }
        }

        private CyPhy.Parameter GetForceLoadParam(CyPhy.ForceLoadParam forceload, string name, out double param)
        {
            try
            {
                CyPhy.Parameter prm = forceload.Children.ParameterCollection.Where(p => ((MgaFCO)p.Impl).MetaRole.Name == name).First();
                param = String.IsNullOrEmpty(prm.Attributes.Value)?0:double.Parse(prm.Attributes.Value);
                return prm;
            }
            catch (InvalidOperationException)
            {
                Logger.Instance.AddLogMessage("Parameter" + name + " was not found in ForceLoad.", Severity.Error);
            }
            catch (FormatException)
            {
                Logger.Instance.AddLogMessage("Parameter" + name + " is not a valid floating point value.", Severity.Error);
            }
            param = 0;
            return null;
        }

        private CyPhy.Parameter GetAccelerationLoadParam(CyPhy.AccelerationLoadParam accelerationload, string name, out double param)
        {
            try
            {
                CyPhy.Parameter prm = accelerationload.Children.ParameterCollection.Where(p => ((MgaFCO)p.Impl).MetaRole.Name == name).First();
                param = String.IsNullOrEmpty(prm.Attributes.Value)?0:double.Parse(prm.Attributes.Value);
                return prm;
            }
            catch (InvalidOperationException)
            {
                Logger.Instance.AddLogMessage("Parameter" + name + " was not found in AccelerationLoad.", Severity.Error);
            }
            catch (FormatException)
            {
                Logger.Instance.AddLogMessage("Parameter" + name + " is not a valid floating point value.", Severity.Error);
            }
            param = 0;
            return null;
        }

        private CyPhy.Parameter GetPressureLoadParam(CyPhy.PressureLoadParam pressureload, string name, out double param)
        {
            try
            {
                CyPhy.Parameter prm = pressureload.Children.ParameterCollection.Where(p => ((MgaFCO)p.Impl).MetaRole.Name == name).First();
                param = String.IsNullOrEmpty(prm.Attributes.Value) ? 0 : double.Parse(prm.Attributes.Value);
                return prm;
            }
            catch (InvalidOperationException)
            {
                Logger.Instance.AddLogMessage("Parameter" + name + " was not found in PressureLoad.", Severity.Error);
            }
            catch (FormatException)
            {
                Logger.Instance.AddLogMessage("Parameter" + name + " is not a valid floating point value.", Severity.Error);
            }
            param = 0;
            return null;
        }

        public override void TraverseTestBench(CyPhy.TestBenchType testBenchBase)
        {           
            string stepFormat = "AP203_E2_Single_File";
            if (!STEP_DataExchangeFormats.Contains(stepFormat, StringComparer.CurrentCultureIgnoreCase))
                STEP_DataExchangeFormats.Add(stepFormat);

            CyPhy.CADTestBench testBench = testBenchBase as CyPhy.CADTestBench;
            if (testBench == null)
                testBench = CyPhyClasses.CADTestBench.Cast(testBenchBase.Impl);

            if (testBench.Attributes.SolverType == CyPhyClasses.CADTestBench.AttributesClass.SolverType_enum.PATRAN_NASTRAN)
                NonSTEP_DataExchangeFormats.Add("Parasolid");

            this.CyphyTestBenchRef = testBench;
            base.TraverseTestBench(testBenchBase);

            AdjSurfTreatment = CyphyTestBenchRef.Attributes.AdjoiningTreatment;

            // Solver Settings
            ElementType = "MIDPOINT_PARABOLIC_FIXED";
            SolverType = testBench.Attributes.SolverType.ToString(); 
            MeshType = (testBench.Attributes.ElementType == CyPhyClasses.CADTestBench.AttributesClass.ElementType_enum.Tetra10 ||
                        testBench.Attributes.ElementType == CyPhyClasses.CADTestBench.AttributesClass.ElementType_enum.Tetra4 ) ? "SOLID" : "SURFACE";
            ShellType = (MeshType == "SOLID") ? "N/A" : testBench.Attributes.ElementType.ToString();
            MaxAdaptiveIterations = testBench.Attributes.MaxAdaptiveIterations;

            FEAAnalysisType = "STRUCTURAL";
            if (testBench.Children.ThermalLoadCollection.Any() || testBench.Children.ThermalEnvironmentCollection.Any())
            {
                FEAAnalysisType = "THERMAL";
            }

            // Mesh Parameters
            CyPhy.MeshParameters meshParam = testBench.Children.MeshParametersCollection.FirstOrDefault();
            if (meshParam != null)
            {
                this.cadMeshParameters = new CAD.MeshParametersType()
                {
                    Match_Face_Proximity_Tol = meshParam.Attributes.Match_Face_Proximity_Tol,
                    Max_Curv_Delta_Div_Edge_Len = meshParam.Attributes.Max_Curv_Delta_Divide_Edge_Len,
                    Max_Global_Length = meshParam.Attributes.Max_Global_Length,
                    Ratio_Min_Edge_To_Max_Edge = meshParam.Attributes.Ratio_Min_Edge_To_Max_Edge
                };
            }

            // Metrics
            foreach (var item in testBench.Children.TIP2StructuralMetricCollection)
            {
                if (item.SrcEnds.TestInjectionPoint != null)
                {
                    CyPhy.TestInjectionPoint tip = item.SrcEnds.TestInjectionPoint;
                    CyPhy.StructuralFEAComputation feaComp = item.DstEnds.StructuralFEAComputation;

                    if (tip.AllReferred == null)
                        continue;

                    List<CyPhy.Component> testComponents = new List<CyPhy.Component>();
                    if (tip.AllReferred is CyPhy.ComponentAssembly)
                    {
                        CollectLeafComponents(testComponents, tip.AllReferred as CyPhy.ComponentAssembly);
                    }
                    else if (tip.AllReferred is CyPhy.Component)
                    {
                        // Interested in components with CAD Model only
                        if ((tip.AllReferred as CyPhy.Component).Children.CADModelCollection.Any())
                            testComponents.Add(tip.AllReferred as CyPhy.Component);
                    }

                    foreach (CyPhy.Component comp in testComponents)
                    {
                        string compId = comp.Attributes.InstanceGUID;

                        foreach (var cyphycompport in feaComp.Children.StructuralAnalysisComputationTypeCollection)
                        {
                            TBComputation tbcomputation = new TBComputation();
                            if (cyphycompport is CyPhy.BearingStress)
                            {
                                tbcomputation.ComputationType = TBComputation.Type.BEARINGSTRESS;
                            } else if (cyphycompport is CyPhy.MisesStress)
                            {
                                tbcomputation.ComputationType = TBComputation.Type.VONMISESSTRESS;
                            } else if (cyphycompport is CyPhy.ShearStress)
                            {
                                tbcomputation.ComputationType = TBComputation.Type.SHEARSTRESS;
                            } else if (cyphycompport is CyPhy.MaximumDisplacement)
                            {
                                tbcomputation.ComputationType = TBComputation.Type.DISPLACEMENT;
                            } else if (cyphycompport is CyPhy.FactorOfSafety)
                            {
                                tbcomputation.ComputationType = TBComputation.Type.FACTOROFSAFETY;
                            } else if (cyphycompport is CyPhy.TsaiWuCriteria)
                            {
                                if (feaComp.Children.StructuralAnalysisComputationTypeCollection.Count() > 1)
                                {
                                    Logger.Instance.AddLogMessage("Tsai-Wu failure criteria set, no other Structural FEA Computation types are allowed!", Severity.Error);
                                    return;
                                }

                                if (comp.Children.MaterialContentsCollection.Count() == 0)
                                {
                                    Logger.Instance.AddLogMessage("Tsai-Wu failure criteria set, referenced component [" + comp.Name + "] must be surface model components with material contents!", Severity.Error);
                                    return;
                                }

                                tbcomputation.ComputationType = TBComputation.Type.TSAI_WU_FAILURE;
                            } else if (cyphycompport is CyPhy.HillCriteria)
                            {
                                if (feaComp.Children.StructuralAnalysisComputationTypeCollection.Count() > 1)
                                {
                                    Logger.Instance.AddLogMessage("Hill failure criteria set, no other Structural FEA Computation types are allowed!", Severity.Error);
                                    return;
                                }
                                if (comp.Children.MaterialContentsCollection.Count() == 0)
                                {
                                    Logger.Instance.AddLogMessage("Hill failure criteria set, referenced component [" + comp.Name + "] must be surface model components with material contents!", Severity.Error);
                                    return;
                                }

                                tbcomputation.ComputationType = TBComputation.Type.HILL_FAILURE;
                            } else if (cyphycompport is CyPhy.HoffmanCriteria)                            
                            {
                                if (feaComp.Children.StructuralAnalysisComputationTypeCollection.Count() > 1)
                                {
                                    Logger.Instance.AddLogMessage("Hoffman failure criteria set, no other Structural FEA Computation types are allowed!", Severity.Error);
                                    return;
                                }
                                if (comp.Children.MaterialContentsCollection.Count() == 0)
                                {
                                    Logger.Instance.AddLogMessage("Hoffman failure criteria set, referenced component [" + comp.Name + "] must be surface model components with material contents!", Severity.Error);
                                    return;
                                }

                                tbcomputation.ComputationType = TBComputation.Type.HOFFMAN_FAILURE;
                            }
                            else if (cyphycompport is CyPhy.MaximumFailureCriteria)
                            {
                                if (feaComp.Children.StructuralAnalysisComputationTypeCollection.Count() > 1)
                                {
                                    Logger.Instance.AddLogMessage("Maximum failure criteria set, no other Structural FEA Computation types are allowed!", Severity.Error);
                                    return;
                                }
                                if (comp.Children.MaterialContentsCollection.Count() == 0)
                                {
                                    Logger.Instance.AddLogMessage("Maximum failure criteria set, referenced component [" + comp.Name + "] must be surface model components with material contents!", Severity.Error);
                                    return;
                                }

                                tbcomputation.ComputationType = TBComputation.Type.MAX_FAILURE;
                            }

                            tbcomputation.FeatureDatumName = "";
                            tbcomputation.RequestedValueType = "Scalar";
                            tbcomputation.Details = "InfiniteCycle";
                            tbcomputation.ComponentID = compId;

                            foreach (var cyphyconn in cyphycompport.DstConnections.FEAComputation2MetricCollection)
                            {
                                //  JK 8/19/2016, This should be Guid instead of GME Object ID
                                //tbcomputation.MetricID = cyphyconn.DstEnds.Metric.ID;  
                                tbcomputation.MetricID = cyphyconn.DstEnds.Metric.Guid.ToString();
                            }

                            if (!String.IsNullOrEmpty(tbcomputation.MetricID))
                                this.Computations.Add(tbcomputation);
                        }
                    }
                }
            }

            // Thermal Metrics
            foreach (var item in testBench.Children.TIP2ThermalMetricCollection)
            {
                if (item.SrcEnds.TestInjectionPoint != null)
                {
                    CyPhy.TestInjectionPoint tip = item.SrcEnds.TestInjectionPoint;
                    CyPhy.ThermalFEAComputation feaComp = item.DstEnds.ThermalFEAComputation;

                    if (tip.AllReferred == null)
                        continue;

                    List<CyPhy.Component> testComponents = new List<CyPhy.Component>();
                    if (tip.AllReferred is CyPhy.ComponentAssembly)
                    {
                        CollectLeafComponents(testComponents, tip.AllReferred as CyPhy.ComponentAssembly);
                    }
                    else if (tip.AllReferred is CyPhy.Component)
                    {
                        // Interested in components with CAD Model only
                        if ((tip.AllReferred as CyPhy.Component).Children.CADModelCollection.Any())
                            testComponents.Add(tip.AllReferred as CyPhy.Component);
                    }

                    foreach (CyPhy.Component comp in testComponents)
                    {
                        string compId = comp.Attributes.InstanceGUID;

                        foreach (var cyphycompport in feaComp.Children.ThermalAnalysisMetricsCollection)
                        {
                            TBComputation tbcomputation = new TBComputation();
                            if (cyphycompport is CyPhy.MinimumTemperature)
                            {
                                tbcomputation.ComputationType = TBComputation.Type.MINIMUMTEMPERATURE;
                            } else if (cyphycompport is CyPhy.MaximumTemperature)
                            {
                                tbcomputation.ComputationType = TBComputation.Type.MAXIMUMTEMPERATURE;
                            }
                            tbcomputation.FeatureDatumName = "";
                            tbcomputation.RequestedValueType = "Scalar";
                            tbcomputation.Details = "InfiniteCycle";
                            tbcomputation.ComponentID = compId;

                            foreach (var cyphyconn in cyphycompport.DstConnections.FEAComputation2MetricCollection)
                            {
                                tbcomputation.MetricID = cyphyconn.DstEnds.Metric.ID;
                            }

                            if (!String.IsNullOrEmpty(tbcomputation.MetricID))
                                this.Computations.Add(tbcomputation);
                        }
                    }
                }
            }

            bool convectionPresent = false;
            bool ambientTempPresent = false;

            // thermal elements
            foreach(var item in testBench.Children.ThermalFEAElementsCollection)
            {
                foreach (var conn in item.DstConnections.ThermalElement2TIPCollection)
                {
                    CyPhy.TestInjectionPoint tip = conn.DstEnds.TestInjectionPoint;

                    if (tip.AllReferred == null)
                        continue;

                    List<CyPhy.Component> testComponents = new List<CyPhy.Component>();
                    if (tip.AllReferred is CyPhy.ComponentAssembly)
                    {
                        CollectLeafComponents(testComponents, tip.AllReferred as CyPhy.ComponentAssembly);
                    }
                    else if (tip.AllReferred is CyPhy.Component)
                    {
                        // Interested in components with CAD Model only
                        if ((tip.AllReferred as CyPhy.Component).Children.CADModelCollection.Any())
                            testComponents.Add(tip.AllReferred as CyPhy.Component);
                    }

                    foreach (var component in testComponents)
                    {
                        FEAThermalElement[] element = FEAThermalElement.Extract(item, component.Attributes.InstanceGUID, null);
                        if (element.Where(e => e.Type == "Convection").Any())
                        {
                            convectionPresent = true;
                        }
                        ThermalElements.AddRange(element);
                    }
                }

                foreach (var conn in item.DstConnections.ThermalElements2GeometryCollection)
                {
                    CyPhy.GeometryBase geometryBase = conn.DstEnds.GeometryTypes;
                    string tipContextPath = Path.GetDirectoryName(geometryBase.Path);
                    CADGeometry geometryRep = FillOutGeometryRep(geometryBase.Impl as MgaFCO,
                                                                 tipContextPath);
                    FEAThermalElement[] element = FEAThermalElement.Extract(item, null, geometryRep);
                    ThermalElements.AddRange(element);
                }
            }

            if (testBench.Children.ThermalEnvironmentCollection.Any())
            {
                if (testBench.Children.ThermalEnvironmentCollection.Count() > 1)
                {
                    Logger.Instance.AddLogMessage("Multiple ThermalEnvironments are present in the testbench. There should be only one.", Severity.Error);
                }
                else
                {
                    if (!testBench.Children.ThermalEnvironmentCollection.First().Children.ParameterCollection.Any())
                    {
                        Logger.Instance.AddLogMessage("ThermalEnvironment is present but there are no parameters specified in it.", Severity.Warning);
                    }
                    else
                    {
                        foreach (var param in testBench.Children.ThermalEnvironmentCollection.First().Children.ParameterCollection)
                        {
                            var elem = new FEAThermalElement(param) { Unit = "K", ComponentID = cadDataContainer.assemblies.First().Key };
                            ThermalElements.Add(elem);
                            ambientTempPresent = true;
                        }
                    }
                }
            }

            if (convectionPresent && !ambientTempPresent)
            {
                Logger.Instance.AddLogMessage("Convection is present but no Ambient Temperature has been specified. Please specify Ambient Temperature.", Severity.Error);
            }

            // Constraints
            foreach (var cyphyconstraint in testBench.Children.AnalysisConstraintCollection)
            {
                if (cyphyconstraint.Kind == "PinConstraint")
                {
                    CyPhy.PinConstraint pinConstraint = CyPhyClasses.PinConstraint.Cast(cyphyconstraint.Impl);

                    // Geometry - must be a cylinder
                    foreach (var geometry in pinConstraint.DstConnections.Pin2CylinderCollection)
                    {
                        FEAPinConstraint feapinRep = new FEAPinConstraint();
                        feapinRep.AxialDisplacement = pinConstraint.Attributes.AxialDisplacement.ToString();
                        feapinRep.AxialRotation = pinConstraint.Attributes.AxialRotation.ToString();

                        CyPhy.CylinderGeometryType cylinderType = geometry.DstEnds.CylinderGeometryType;
                        if (cylinderType != null)
                        {
                            string tipContextPath = Path.GetDirectoryName(cylinderType.Path);
                            AddGeometry2Constraint(feapinRep,
                                                   cylinderType.Impl as MgaFCO,
                                                   tipContextPath, true);
                        }
                    }

                }
                else if (cyphyconstraint.Kind == "BallConstraint")
                {
                    CyPhy.BallConstraint ballConstraint = CyPhyClasses.BallConstraint.Cast(cyphyconstraint.Impl);

                    foreach (var item in ballConstraint.DstConnections.Ball2SphereCollection)
                    {
                        FEABallConstraint feaballRep = new FEABallConstraint();
                        CyPhy.SphereGeometryType sphereType = item.DstEnds.SphereGeometryType;
                        if (sphereType != null)
                        {
                            string tipContextPath = Path.GetDirectoryName(sphereType.Path);
                            AddGeometry2Constraint(feaballRep,
                                                   sphereType.Impl as MgaFCO,
                                                   tipContextPath, true);
                        }
                    }
                }
                else if (cyphyconstraint.Kind == "DisplacementConstraint")
                {
                    CyPhy.DisplacementConstraint displacementConstraint = CyPhyClasses.DisplacementConstraint.Cast(cyphyconstraint.Impl);                    

                    string tx = "FREE", ty = "FREE", tz = "FREE", tunit = "mm", rx = "FREE", ry = "FREE", rz = "FREE", runit = "deg";

                    CyPhy.Rotation rotation = displacementConstraint.Children.RotationCollection.FirstOrDefault();
                    if (rotation != null)
                    {
                        bool hasScalar = (rotation.Attributes.XDirection == CyPhyClasses.Rotation.AttributesClass.XDirection_enum.SCALAR) ||
                                         (rotation.Attributes.YDirection == CyPhyClasses.Rotation.AttributesClass.YDirection_enum.SCALAR) ||
                                         (rotation.Attributes.ZDirection == CyPhyClasses.Rotation.AttributesClass.ZDirection_enum.SCALAR) ;
                        rx = (rotation.Attributes.XDirection == CyPhyClasses.Rotation.AttributesClass.XDirection_enum.SCALAR) ? 
                              rotation.Attributes.XDirectionValue.ToString() : rotation.Attributes.XDirection.ToString();
                        ry = (rotation.Attributes.YDirection == CyPhyClasses.Rotation.AttributesClass.YDirection_enum.SCALAR) ?
                              rotation.Attributes.YDirectionValue.ToString() : rotation.Attributes.YDirection.ToString();
                        rz = (rotation.Attributes.ZDirection == CyPhyClasses.Rotation.AttributesClass.ZDirection_enum.SCALAR) ?
                              rotation.Attributes.ZDirectionValue.ToString() : rotation.Attributes.ZDirection.ToString();

                        if (!hasScalar)
                        {
                            runit = "N/A";
                        }
                        else
                        {
                            if (rotation.Referred.unit != null)
                                runit = rotation.Referred.unit.Name;
                        }
                    }

                    CyPhy.Translation translation = displacementConstraint.Children.TranslationCollection.FirstOrDefault();
                    if (translation != null)
                    {
                        bool hasScalar = (translation.Attributes.XDirection == CyPhyClasses.Translation.AttributesClass.XDirection_enum.SCALAR) ||
                                         (translation.Attributes.YDirection == CyPhyClasses.Translation.AttributesClass.YDirection_enum.SCALAR) ||
                                         (translation.Attributes.ZDirection == CyPhyClasses.Translation.AttributesClass.ZDirection_enum.SCALAR);
                        tx = (translation.Attributes.XDirection == CyPhyClasses.Translation.AttributesClass.XDirection_enum.SCALAR) ?
                              translation.Attributes.XDirectionValue.ToString() : translation.Attributes.XDirection.ToString();
                        ty = (translation.Attributes.YDirection == CyPhyClasses.Translation.AttributesClass.YDirection_enum.SCALAR) ?
                              translation.Attributes.YDirectionValue.ToString() : translation.Attributes.YDirection.ToString();
                        tz = (translation.Attributes.ZDirection == CyPhyClasses.Translation.AttributesClass.ZDirection_enum.SCALAR) ?
                              translation.Attributes.ZDirectionValue.ToString() : translation.Attributes.ZDirection.ToString();

                        if (!hasScalar)
                        {
                            tunit = "N/A";
                        }
                        else
                        {
                            if (translation.Referred.unit != null)
                                tunit = translation.Referred.unit.Name;
                        }
                        
                    }


                    foreach (var item in displacementConstraint.DstConnections.Displacement2GeometryCollection)
                    {
                        FEADisplacementConstraint feadispRep = new FEADisplacementConstraint();
                        feadispRep.Rotation_X = rx;
                        feadispRep.Rotation_Y = ry;
                        feadispRep.Rotation_Z = rz;
                        feadispRep.RotationUnits = runit;
                        feadispRep.Translation_X = tx;
                        feadispRep.Translation_Y = ty;
                        feadispRep.Translation_Z = tz;
                        feadispRep.TranslationUnits = tunit;

                        Logger.Instance.AddLogMessage(String.Format("DisplacementConstraint Units - Rotation Component = {0}, Translation Component = {1}", runit, tunit), Severity.Info);

                        CyPhy.GeometryBase geometry = item.DstEnds.GeometryBase;
                        if (geometry != null)
                        {
                            string tipContextPath = Path.GetDirectoryName(geometry.Path);
                            AddGeometry2Constraint(feadispRep,
                                                   geometry.Impl as MgaFCO,
                                                   tipContextPath, true);
                        }
                    }
                }
                
            }

            // Loads
            foreach (var cyphyload in testBench.Children.AnalysisLoadCollection)
            {
                if (cyphyload is CyPhy.ForceLoadParam)
                {
                    CyPhy.ForceLoadParam forceLoad = CyPhyClasses.ForceLoadParam.Cast(cyphyload.Impl);

                    double fx = 0.0, fy = 0.0, fz = 0.0, mx = 0.0, my = 0.0, mz = 0.0;
                    string funit = "N", munit = "N-mm";

                    CyPhy.Parameter p1x = GetForceLoadParam(forceLoad, "ForceX", out fx);
                    CyPhy.Parameter p1y = GetForceLoadParam(forceLoad, "ForceY", out fy);
                    CyPhy.Parameter p1z = GetForceLoadParam(forceLoad, "ForceZ", out fz);
                    CyPhy.Parameter p2x = GetForceLoadParam(forceLoad, "MomentX", out mx);
                    CyPhy.Parameter p2y = GetForceLoadParam(forceLoad, "MomentY", out my);
                    CyPhy.Parameter p2z = GetForceLoadParam(forceLoad, "MomentZ", out mz);

                    GetParamUnitName(p1x, ref funit);
                    GetParamUnitName(p2x, ref munit);


                    foreach (var item in forceLoad.DstConnections.ForceLoadParam2GeometryCollection)

                    {
                        FEAForceLoad feaforceRep = new FEAForceLoad();
                        feaforceRep.Force_X = fx;
                        feaforceRep.Force_Y = fy;
                        feaforceRep.Force_Z = fz;
                        feaforceRep.ForceUnit = funit;
                        feaforceRep.Moment_X = mx;
                        feaforceRep.Moment_Y = my;
                        feaforceRep.Moment_Z = mz;
                        feaforceRep.MomentUnit = munit;

                        string _id = feaforceRep.Force_id;
                        Action<CyPhy.Parameter, string> addMapping = (param, name) =>
                        {
                            var srcParam = param.SrcConnections.ValueFlowCollection.Select(vf => vf.SrcEnds.Parameter).Where(srcp => srcp != null).FirstOrDefault();
                            if (srcParam != null)
                            {
                                TestBenchParameterMappings.Add(new CyPhy2CAD_CSharp.TestBenchModel.FEALoadCadParameterMapping()
                                {
                                    TestBenchParameterName = srcParam.Name,
                                    _id = _id,
                                    AttributeName = name
                                });
                            }
                        };
                        addMapping(p1x, "x");
                        addMapping(p1y, "y");
                        addMapping(p1z, "z");
                        _id = feaforceRep.Moment_id;
                        addMapping(p2x, "x");
                        addMapping(p2y, "y");
                        addMapping(p2z, "z");

                        Logger.Instance.AddLogMessage(String.Format("ForceLoad Units - Force Component = {0}, Moment Component = {1}", funit, munit), Severity.Info);

                        CyPhy.GeometryBase geometry = item.DstEnds.GeometryBase;
                        if (geometry != null)
                        {
                            string tipContextPath = Path.GetDirectoryName(geometry.Path);
                            AddGeometry2Load(feaforceRep,
                                             geometry.Impl as MgaFCO,
                                             tipContextPath, true);
                        }
                    }
                }
                else
                if (cyphyload is CyPhy.ForceLoad)
                {
                    Logger.Instance.AddLogMessage("ForceLoad is used in FEA testbench. This construct is obsolete, please use ForceLoadParam instead.", Severity.Warning);
                    CyPhy.ForceLoad forceLoad = CyPhyClasses.ForceLoad.Cast(cyphyload.Impl);
                    
                    double fx = 0.0, fy = 0.0, fz = 0.0, mx = 0.0, my = 0.0, mz = 0.0;
		            string funit = "N", munit = "N-mm";

                    CyPhy.Force force = forceLoad.Children.ForceCollection.FirstOrDefault();
                    if (force != null)
                    {
                        fx = force.Attributes.XDirectionValue;
                        fy = force.Attributes.YDirectionValue;
                        fz = force.Attributes.ZDirectionValue;
                        if (force.Referred.unit != null)
                            funit = force.Referred.unit.Name;
                    }

                    CyPhy.Moment moment = forceLoad.Children.MomentCollection.FirstOrDefault();
                    if (moment != null)
                    {
                        mx = moment.Attributes.XDirectionValue;
                        my = moment.Attributes.YDirectionValue;
                        mz = moment.Attributes.ZDirectionValue;
                        if (moment.Referred.unit != null)
                            munit = moment.Referred.unit.Name;
                    }


                    foreach (var item in forceLoad.DstConnections.Force2GeometryCollection)
                    {
                        FEAForceLoad feaforceRep = new FEAForceLoad();
                        feaforceRep.Force_X = fx;
                        feaforceRep.Force_Y = fy;
                        feaforceRep.Force_Z = fz;
                        feaforceRep.ForceUnit = funit;
                        feaforceRep.Moment_X = mx;
                        feaforceRep.Moment_Y = my;
                        feaforceRep.Moment_Z = mz;
                        feaforceRep.MomentUnit = munit;

                        Logger.Instance.AddLogMessage(String.Format("ForceLoad Units - Force Component = {0}, Moment Component = {1}", funit, munit), Severity.Info);

                        CyPhy.GeometryBase geometry = item.DstEnds.GeometryBase;
                        if (geometry != null)
                        {
                            string tipContextPath = Path.GetDirectoryName(geometry.Path);
                            AddGeometry2Load(feaforceRep,
                                             geometry.Impl as MgaFCO,
                                             tipContextPath, true);
                        }
                    }

                }
                else if (cyphyload is CyPhy.AccelerationLoadParam)
                {
                    CyPhy.AccelerationLoadParam acceleration = CyPhyClasses.AccelerationLoadParam.Cast(cyphyload.Impl);
                    FEAccelerationLoad feaaccelRep = new FEAccelerationLoad();
                    double x = 0;
                    double y = 0;
                    double z = 0;
                    CyPhy.Parameter p1 = GetAccelerationLoadParam(acceleration, "X", out x);
                    Action<string> addMapping = (name) =>
                    {
                        var srcParam = p1.SrcConnections.ValueFlowCollection.Select(vf => vf.SrcEnds.Parameter).Where(srcp => srcp != null).FirstOrDefault();
                        if (srcParam != null)
                        {
                            TestBenchParameterMappings.Add(new CyPhy2CAD_CSharp.TestBenchModel.FEALoadCadParameterMapping()
                            {
                                TestBenchParameterName = srcParam.Name,
                                _id = feaaccelRep.Acceleration_id,
                                AttributeName = name
                            });
                        }
                    };
                    addMapping("x");
                    p1 = GetAccelerationLoadParam(acceleration, "Y", out y);
                    addMapping("y");
                    p1 = GetAccelerationLoadParam(acceleration, "Z", out z);
                    addMapping("z");
                    feaaccelRep.X = x;
                    feaaccelRep.Y = y;
                    feaaccelRep.Z = z;

                    string unit = "mm/s^2";
                    GetParamUnitName(p1, ref unit);
                    feaaccelRep.Units = unit;

                    Logger.Instance.AddLogMessage(String.Format("AccelerationLoad Units = {0}", feaaccelRep.Units), Severity.Info);

                    this.Loads.Add(feaaccelRep);

                }
                else if (cyphyload is CyPhy.AccelerationLoad)
                {
                    Logger.Instance.AddLogMessage("AccelerationLoad is used in FEA testbench. This construct is obsolete, please use AccelerationLoadParam instead.", Severity.Warning);
                    CyPhy.AccelerationLoad acceleration = CyPhyClasses.AccelerationLoad.Cast(cyphyload.Impl);
                    FEAccelerationLoad feaaccelRep = new FEAccelerationLoad();
                    feaaccelRep.X = acceleration.Attributes.XDirectionValue;
                    feaaccelRep.Y = acceleration.Attributes.YDirectionValue;
                    feaaccelRep.Z = acceleration.Attributes.ZDirectionValue;
                    if (acceleration.Referred.unit != null)
                        feaaccelRep.Units = acceleration.Referred.unit.Name;
                    else
                        feaaccelRep.Units = "mm/s^2";

                    Logger.Instance.AddLogMessage(String.Format("AccelerationLoad Units = {0}", feaaccelRep.Units), Severity.Info);

                    this.Loads.Add(feaaccelRep);
                }
                else if (cyphyload is CyPhy.PressureLoadParam)
                {
                    CyPhy.PressureLoadParam pressure = CyPhyClasses.PressureLoadParam.Cast(cyphyload.Impl);

                    foreach (var item in pressure.DstConnections.PressureParam2GeometryCollection)
                    {
                        FEAPressureLoad feapressRep = new FEAPressureLoad();
                        double p = 0;
                        CyPhy.Parameter p1 = GetPressureLoadParam(pressure, "PressureLoad", out p);
                        feapressRep.Value = p;
                        string unit = "MPa";
                        GetParamUnitName(p1, ref unit);
                        feapressRep.Units = unit;

                        Logger.Instance.AddLogMessage(String.Format("PressureLoad Units = {0}", feapressRep.Units), Severity.Info);

                        CyPhy.GeometryBase geometry = item.DstEnds.GeometryBase;
                        if (geometry != null)
                        {
                            string tipContextPath = Path.GetDirectoryName(geometry.Path);
                            AddGeometry2Load(feapressRep,
                                             geometry.Impl as MgaFCO,
                                             tipContextPath, true);

                            var srcParam = p1.SrcConnections.ValueFlowCollection.Select(vf => vf.SrcEnds.Parameter).Where(srcp => srcp != null).FirstOrDefault();
                            if (srcParam != null)
                            {
                                TestBenchParameterMappings.Add(new CyPhy2CAD_CSharp.TestBenchModel.FEALoadCadParameterMapping()
                                {
                                    TestBenchParameterName = srcParam.Name,
                                    _id = feapressRep.Pressure_id,
                                    AttributeName = "Value"
                                });
                            }
                        }
                    }
                }
                else if (cyphyload is CyPhy.PressureLoad)
                {
                    Logger.Instance.AddLogMessage("PressureLoad is used in FEA testbench. This construct is obsolete, please use PressureLoadParam instead.", Severity.Warning);

                    CyPhy.PressureLoad pressure = CyPhyClasses.PressureLoad.Cast(cyphyload.Impl);

                    foreach (var item in pressure.DstConnections.Pressure2GeometryCollection)
                    {
                        FEAPressureLoad feapressRep = new FEAPressureLoad();
                        feapressRep.Value = pressure.Attributes.Value;
                        if (pressure.Referred.unit != null)
                            feapressRep.Units = pressure.Referred.unit.Name;
                        else
                            feapressRep.Units = "MPa";

                        Logger.Instance.AddLogMessage(String.Format("PressureLoad Units = {0}", feapressRep.Units), Severity.Info);

                        CyPhy.GeometryBase geometry = item.DstEnds.GeometryBase;
                        if (geometry != null)
                        {
                            string tipContextPath = Path.GetDirectoryName(geometry.Path);
                            AddGeometry2Load(feapressRep,
                                             geometry.Impl as MgaFCO,
                                             tipContextPath, true);
                        }
                    }
                }
            }
            // Post Processing Blocks
            foreach (var postprocess in testBench.Children.PostProcessingCollection)
            {
                PostProcessScripts.Add(postprocess.Attributes.ScriptPath);
            }


        }

        private void AddGeometry2Constraint(FEAConstraintBase constraintRep,
                                    MgaFCO geometryFCO,
                                    string tipContextPath, bool addcomputations)
        {
            GeometryTraversal traverser = new GeometryTraversal();
            traverser.TraverseGeometry(geometryFCO);

            CADGeometry geomRep = FillOutGeometryRep(geometryFCO,
                                                     tipContextPath);

            if (addcomputations)
            {
                foreach (var point in geomRep.GeometryFeatures)
                {
                    TBComputation tbcomputation = new TBComputation();
                    tbcomputation.ComputationType = TBComputation.Type.POINTCOORDINATES;
                    tbcomputation.MetricID = point.ComponentID + ":" + point.DatumName;
                    tbcomputation.RequestedValueType = "Vector";
                    tbcomputation.Details = point.DatumName;   //tbcomputation.FeatureDatumName = point.DatumName;
                    tbcomputation.ComponentID = point.ComponentID;

                    StaticAnalysisMetrics.Add(tbcomputation);
                }
            }

            if (geomRep != null)
            {
                constraintRep.AddGeometry(geomRep);
                this.Constraints.Add(constraintRep);
            }
            
        }

        private void AddGeometry2Load(FEALoadBase loadRep,
                                      MgaFCO geometryFCO,
                                      string tipContextPath, bool addcomputations)
        {
            GeometryTraversal traverser = new GeometryTraversal();
            traverser.TraverseGeometry(geometryFCO);
            
            CADGeometry geomRep = FillOutGeometryRep(geometryFCO,
                                                     tipContextPath);

            if (geomRep != null)
            {
                if (addcomputations)
                {
                    foreach (var point in geomRep.GeometryFeatures)
                    {
                        TBComputation tbcomputation = new TBComputation();
                        tbcomputation.ComputationType = TBComputation.Type.POINTCOORDINATES;
                        tbcomputation.MetricID = point.ComponentID + ":" + point.DatumName;
                        tbcomputation.RequestedValueType = "Vector";
                        tbcomputation.Details = point.DatumName;       //tbcomputation.FeatureDatumName = point.DatumName;
                        tbcomputation.ComponentID = point.ComponentID;

                        StaticAnalysisMetrics.Add(tbcomputation);
                    }
                }

                loadRep.AddGeometry(geomRep);
                this.Loads.Add(loadRep);
            }

        }

        private CADGeometry FillOutGeometryRep(MgaFCO geometryFCO,
                                        string tipContextPath)
        {
            GeometryTraversal traverser = new GeometryTraversal();
            traverser.TraverseGeometry(geometryFCO);

            if (traverser.geometryFound.Count > 0)
            {
                CyPhy.GeometryTypes geometryCyPhy = CyPhyClasses.GeometryTypes.Cast(traverser.geometryFound.First());
                if (Path.GetDirectoryName(geometryCyPhy.Path).Contains(tipContextPath))            // within context of TIP
                {
                    CADGeometry geomRep = CADGeometry.CreateGeometry(geometryCyPhy);
                    return geomRep;
                }
                else
                    return null;
            }
            else
                return null;
        }

        public override bool GenerateOutputFiles()
        {
            if (!HasErrors())
            {
                GenerateCADXMLOutput();
                GenerateRunBat();
                GenerateScriptFiles();
                GenerateProcessingScripts(PostProcessScripts);
                return true;
            }
            return false;
        }

        public override void GenerateCADXMLOutput()
        {
            CAD.AssembliesType assembliesoutroot = cadDataContainer.ToCADXMLOutput(this);

            if (assembliesoutroot.Assembly.Length > 0)
            {
                AddAnalysisToXMLOutput(assembliesoutroot.Assembly[0]);
            }
                       

            // materials
            /*
            // META-1544: removed
            List<CAD.MaterialType> materialList = new List<CAD.MaterialType>();
            CAD.MaterialsType materials = new CAD.MaterialsType();
            materials._id = UtilityHelpers.MakeUdmID();
            foreach (var item in cadDataContainer.CreateMaterialList())
            {
                CAD.MaterialType material = new CAD.MaterialType();
                material._id = UtilityHelpers.MakeUdmID();
                material.MaterialID = item.Key;
                material.MaterialName = item.Value;
                material.MaterialType1 = "";
                materialList.Add(material);
            }
            materials.Material = materialList.ToArray();
            assembliesoutroot.Materials = new CAD.MaterialsType[1];
            assembliesoutroot.Materials[0] = materials;
            */

            AddDataExchangeFormatToXMLOutput(assembliesoutroot);
            assembliesoutroot.SerializeToFile(Path.Combine(OutputDirectory, TestBenchBase.CADAssemblyFile));

            assembliesoutroot.Assembly[0].Analyses.SerializeToFile(Path.Combine(OutputDirectory, TestBenchBase.CADAnalysisFile));
        }

        protected override void AddAnalysisToXMLOutput(CAD.AssemblyType assembly)
        {
            base.AddAnalysisToXMLOutput(assembly);
            CAD.AnalysesType cadanalysis = GetCADAnalysis(assembly);

            CAD.FEAType feaanalysis = new CAD.FEAType();
            feaanalysis._id = UtilityHelpers.MakeUdmID();
            feaanalysis.AnalysisID = AnalysisID;
            feaanalysis.Type = FEAAnalysisType;
            feaanalysis.MaxAdaptiveIterations = MaxAdaptiveIterations;
            feaanalysis.MaxElementSize = this.CyphyTestBenchRef.Attributes.MaximumElementSize;
            feaanalysis.InfiniteCycle = this.CyphyTestBenchRef.Attributes.InfiniteCycle;
            // solvers
            CAD.SolversType solversType = new CAD.SolversType();
            solversType._id = UtilityHelpers.MakeUdmID();
            CAD.SolverType solver = new CAD.SolverType();
            solver._id = UtilityHelpers.MakeUdmID();
            solver.ElementShapeType = ElementType;
            solver.MeshType = MeshType;
            solver.ShellElementType = ShellType;
            if (SolverType == "CALCULIX")
            {
                solver.Type = "NASTRAN";
            }
            else
            {
                solver.Type = SolverType;
            }
            
            solversType.Solver = new CAD.SolverType[1];
            solversType.Solver[0] = solver;
            feaanalysis.Solvers = solversType;

            // mesh parameter
            if (this.cadMeshParameters != null)
            {
                feaanalysis.MeshParameters = this.cadMeshParameters;
            }

            // loads
            if (Loads.Count > 0)
            {
                List<CAD.LoadType> loadList = new List<CAD.LoadType>();
                foreach (var item in Loads)
                {
                    loadList.Add(item.ToCADXMLOutput());
                }

                feaanalysis.Loads = new CAD.LoadsType();
                feaanalysis.Loads._id = UtilityHelpers.MakeUdmID();
                feaanalysis.Loads.Load = loadList.ToArray();
            }

            // constraints
            if (Constraints.Count > 0)
            {
                List<CAD.AnalysisConstraintType> constraintList = new List<CAD.AnalysisConstraintType>();
                foreach (var item in Constraints)
                {
                    constraintList.Add(item.ToCADXMLOutput());
                }
                feaanalysis.AnalysisConstraints = new CAD.AnalysisConstraintsType();
                feaanalysis.AnalysisConstraints._id = UtilityHelpers.MakeUdmID();
                feaanalysis.AnalysisConstraints.AnalysisConstraint = constraintList.ToArray();
            }
            
            // thermal
            List<CAD.ThermalElementType> thermalOutList = new List<CAD.ThermalElementType>();
            if (ThermalElements.Count > 0)
            {
                foreach (var thermalLoad in ThermalElements)
                {
                    CAD.ThermalElementType thermalOut = new CAD.ThermalElementType();
                    thermalOut._id = UtilityHelpers.MakeUdmID();
                    thermalOut.LoadType = thermalLoad.Type;
                    thermalOut.Unit = thermalLoad.Unit;
                    thermalOut.Value = thermalLoad.LoadValue;

                    if (thermalLoad.Geometry == null)
                    {
                        CAD.ComponentType component = new CAD.ComponentType();
                        component._id = UtilityHelpers.MakeUdmID();
                        component.ComponentID = thermalLoad.ComponentID;
                        component.InfiniteCycle = false;
                        thermalOut.Component = new CAD.ComponentType[] { component };
                    }
                    else
                    {
                        thermalOut.Geometry = new CAD.GeometryType[] { thermalLoad.Geometry.ToCADXMLOutput() };
                    }

                    thermalOutList.Add(thermalOut);
                }
                feaanalysis.ThermalElements = new CAD.ThermalElementsType();
                feaanalysis.ThermalElements._id = UtilityHelpers.MakeUdmID();
                feaanalysis.ThermalElements.ThermalElement = thermalOutList.ToArray();
                
            }
             
            // metrics
            List<CAD.MetricType> metriclist = new List<CAD.MetricType>();
            foreach (var item in Computations)
            {
                CAD.MetricType metric = new CAD.MetricType();
                metric._id = UtilityHelpers.MakeUdmID();
                metric.ComponentID = item.ComponentID;
                metric.MetricID = item.MetricID;
                metric.RequestedValueType = item.RequestedValueType;
                metric.MetricType1 = item.ComputationType.ToString();
                metric.Details = "";
                metric.MetricName = item.MetricName;
                metriclist.Add(metric);
            }
            if (metriclist.Any())
            {
                feaanalysis.Metrics = new CAD.MetricsType();
                feaanalysis.Metrics._id = UtilityHelpers.MakeUdmID();
                feaanalysis.Metrics.Metric = metriclist.ToArray();
            }

            cadanalysis.FEA = new CAD.FEAType[] { feaanalysis };

            this.ExportComponentPoints = true;
            AddStaticAnalysisMetrics(assembly);

        }

        /*
        // This code is copy/pasted from TestBench. Since this class is not a subclass of TestBench,
        // it can't be re-used. Can this code be re-used from there by moving it to TestBenchBase?
        private void AddStaticAnalysis(CAD.AssemblyType assemblyRoot, List<TBComputation> computations)
        {
            if (computations.Any())
            {
                CAD.AnalysesType cadanalysis = GetCADAnalysis(assemblyRoot);

                CAD.StaticType staticanalysis = new CAD.StaticType();
                staticanalysis._id = UtilityHelpers.MakeUdmID();
                staticanalysis.AnalysisID = AnalysisID;

                List<CAD.MetricType> metriclist = new List<CAD.MetricType>();
                foreach (var item in computations)
                {
                    if (item.ComputationType == TBComputation.Type.POINTCOORDINATES)
                    {
                        CAD.MetricType ptout = new CAD.MetricType();
                        ptout._id = UtilityHelpers.MakeUdmID();
                        ptout.ComponentID = item.ComponentID;
                        ptout.MetricID = item.MetricID;
                        ptout.MetricType1 = item.ComputationType.ToString();
                        ptout.RequestedValueType = item.RequestedValueType;
                        ptout.Details = item.FeatureDatumName;
                        ptout.ComponentID = String.IsNullOrEmpty(item.ComponentID) ? "" : item.ComponentID;     // PointCoordinate metric is tied to a specific Component  
                        ptout.MetricName = item.MetricName??"";
                        metriclist.Add(ptout);
                    }
                    else
                    {
                        CAD.MetricType metric = new CAD.MetricType();
                        metric._id = UtilityHelpers.MakeUdmID();
                        metric.MetricID = item.MetricID;
                        metric.MetricType1 = item.ComputationType.ToString();
                        metric.RequestedValueType = item.RequestedValueType;
                        metric.ComponentID = assemblyRoot.ConfigurationID;
                        metric.MetricName = item.MetricName??"";
                        metric.Details = "";
                        metriclist.Add(metric);
                    }
                }

                staticanalysis.Metrics = new CAD.MetricsType();
                staticanalysis.Metrics._id = UtilityHelpers.MakeUdmID();
                staticanalysis.Metrics.Metric = metriclist.ToArray();

                cadanalysis.Static = new CAD.StaticType[] { staticanalysis };
            }
        } */

        public override void GenerateRunBat()
        {
            Template.run_bat searchmeta = new Template.run_bat()
            {
                Automation = IsAutomated,
                XMLFileName = "CADAssembly",
                ComputedMetricsPath = "\"Analysis\\Abaqus\\ComputedValues.xml\"",
                AdditionalOptions = CADOptions ?? "",
            };

            searchmeta.Assembler = "CREO";
            if (SolverType == CyPhyClasses.CADTestBench.AttributesClass.SolverType_enum.ABAQUS_Model_Based.ToString())
            {
                searchmeta.Mesher = "ABAQUS";
                searchmeta.Analyzer = "ABAQUSMODEL";
            }
            else if (SolverType == CyPhyClasses.CADTestBench.AttributesClass.SolverType_enum.ABAQUS_Deck_Based.ToString())
            {
                searchmeta.Mesher = "CREO";
                searchmeta.Analyzer = "ABAQUSDECK";
            }
            else if (SolverType == CyPhyClasses.CADTestBench.AttributesClass.SolverType_enum.NASTRAN.ToString())
            {
                searchmeta.Mesher = "CREO";
                searchmeta.Analyzer = "NASTRAN";
            }
            else if (SolverType == CyPhyClasses.CADTestBench.AttributesClass.SolverType_enum.CALCULIX.ToString())
            {
                searchmeta.Mesher = "CREO";
                searchmeta.Analyzer = "CALCULIX";
            }
            else if (SolverType == CyPhyClasses.CADTestBench.AttributesClass.SolverType_enum.PATRAN_NASTRAN.ToString())
            {
                searchmeta.Mesher = "PATRAN";
                searchmeta.Analyzer = "PATRAN_NASTRAN";
            }

            if (SolverType == CyPhyClasses.CADTestBench.AttributesClass.SolverType_enum.ABAQUS_Model_Based.ToString())
            {
                switch (CyphyTestBenchRef.Attributes.FEAMode)
                {
                    case CyPhyClasses.CADTestBench.AttributesClass.FEAMode_enum.Meshing_Only:
                        searchmeta.Analyzer = "NONE";
                        break;
                    case CyPhyClasses.CADTestBench.AttributesClass.FEAMode_enum.Meshing_and_Boundary_Conditions:
                        searchmeta.Analyzer = "NONE";
                        searchmeta.Mesher = "ABAQUSMDLCHECK";
                        break;
                    case CyPhyClasses.CADTestBench.AttributesClass.FEAMode_enum.Modal:
                        searchmeta.Mode = "MODAL";
                        break;
                    case CyPhyClasses.CADTestBench.AttributesClass.FEAMode_enum.Dynamic__Explicit_:
                        searchmeta.Mode = "DYNEXPL";
                        break;
                    case CyPhyClasses.CADTestBench.AttributesClass.FEAMode_enum.Dynamic__Implicit_:
                        searchmeta.Mode = "DYNIMPL";
                        break;
                    case CyPhyClasses.CADTestBench.AttributesClass.FEAMode_enum.Static__Standard_:
                        searchmeta.Mode = "STATIC";
                        break;
                }
            }
            /*if (SolverType == CyPhyClasses.CADTestBench.AttributesClass.SolverType_enum.ABAQUS_Model_Based.ToString())
                sbuilder.AppendLine("set ERROR_MSG=\"Script Error: error level is %ERROR_CODE%, see log/CyPhy2AbaqusCmd.log for details.\"");
            else
                sbuilder.AppendLine("set ERROR_MSG=\"%FEA_SCRIPT% encountered error during execution, error level is %ERROR_CODE%\"");
            sbuilder.AppendLine("goto :ERROR_SECTION");
            sbuilder.AppendLine(")");*/
            
            using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "runCADJob.bat")))
            {
                writer.WriteLine(searchmeta.TransformText());
            }
            
        }     
       

    }
}
