using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Microsoft.Win32;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2CAD_CSharp.TestBenchModel
{
    public class CFDTestBenchData : TestBenchBase
    {
        public List<string> PostProcessScripts { get; set; }
        public List<string> PreProcessingScripts { get; set; }

        public CFDTestBenchData(CyPhy2CADSettings cadSetting,
                                string outputdir,
                                string projectdir,
                                bool auto = false) :
                                base(cadSetting, outputdir, projectdir, auto)
        {
            PostProcessScripts = new List<string>();
            Computations = new List<TBComputation>();
        }

        private List<CFDConfig> cfdConfigs = new List<CFDConfig>();

        public override void TraverseTestBench(CyPhy.TestBenchType testBenchBase)
        {
            // META-3195 - CFD needs Inventor format now instead of STL ASCII
            STLDataExchangeFormats.Add("Inventor");

            CyPhy.CFDTestBench testBench = testBenchBase as CyPhy.CFDTestBench;
            if (testBench == null)
                testBench = CyPhyClasses.CFDTestBench.Cast(testBenchBase.Impl);

            base.TraverseTestBench(testBenchBase); //AnalysisID = testBench.ID;

            foreach (var conn in testBench.Children.CFDComputation2MetricCollection)
            {
                TBComputation tbcomputation = new TBComputation();
                tbcomputation.MetricID = conn.DstEnds.Metric.ID;
                tbcomputation.ComputationType = TBComputation.Type.COEFFICIENTOFDRAG;
                tbcomputation.RequestedValueType = "SCALAR";
                Computations.Add(tbcomputation);
            }

            int calmSolverCnt = testBench.Children.CalmWaterSolverSettingsCollection.Count();
            int waveSolverCnt = testBench.Children.WaveResistanceSolverSettingsCollection.Count();
            int correlationSolverCnt = testBench.Children.CorrelationSettingsCollection.Count();
            int hydrostaticsSolverCnt = testBench.Children.HydrostaticsSolverSettingsCollection.Count();

            if ((calmSolverCnt + waveSolverCnt + correlationSolverCnt + hydrostaticsSolverCnt) < 1)
            {
                Logger.Instance.AddLogMessage("No CFD solver settings in the test bench", Severity.Error);
                return;
            }

            if (hydrostaticsSolverCnt > 1 || (calmSolverCnt + waveSolverCnt + correlationSolverCnt) > 1)
            {
                Logger.Instance.AddLogMessage(
                    "Multiple CFD solver settings in the test bench", 
                    Severity.Error);
                Logger.Instance.AddLogMessage( 
                    "A test bench can only contain one hydrostatics and one calm, wave or correlation solver setting.", 
                    Severity.Info);
                return;
            }

            #region ValidAttributes

            double MinVelocity = 0;
            double MaxVelocity = 343;
            double AbsZero = -273.15;
            double MaxTemp = 100;
            double MinAngle = 0;
            double MaxAngle = 360;

            #endregion

            if (hydrostaticsSolverCnt == 1)
            {
                CyPhy.HydrostaticsSolverSettings solverSettings = testBench.Children.HydrostaticsSolverSettingsCollection.First();

                if (solverSettings.Attributes.FluidTemperature < AbsZero ||
                    solverSettings.Attributes.FluidTemperature > MaxTemp)
                {
                    string message = string.Format(
                        "'{0}': 'FluidTemperature' attribute must be [{1} - {2}]",
                        solverSettings.Name,
                        AbsZero,
                        MaxTemp);

                    Logger.Instance.AddLogMessage(
                        message,
                        Severity.Error);
                }

                if (solverSettings.Attributes.PitchAngleStart < MinAngle ||
                    solverSettings.Attributes.PitchAngleStart > MaxAngle ||
                    solverSettings.Attributes.PitchAngleEnd < MinAngle ||
                    solverSettings.Attributes.PitchAngleEnd > MaxAngle ||
                    solverSettings.Attributes.RollAngleStart < MinAngle ||
                    solverSettings.Attributes.RollAngleStart > MaxAngle ||
                    solverSettings.Attributes.RollAngleEnd < MinAngle ||
                    solverSettings.Attributes.RollAngleEnd > MaxAngle ||
                    solverSettings.Attributes.PitchAngleStepSize < MinAngle ||
                    solverSettings.Attributes.PitchAngleStepSize > MaxAngle ||
                    solverSettings.Attributes.RollAngleStepSize < MinAngle ||
                    solverSettings.Attributes.RollAngleStepSize > MaxAngle)
                {
                    string message = string.Format(
                        "'{0}': 'Angle' attributes must be [{1}-{2}]",
                        solverSettings.Name,
                        MinAngle,
                        MaxAngle);

                    Logger.Instance.AddLogMessage(
                        message,
                        Severity.Error);
                }

                cfdConfigs.Add(new CFDConfig.HydrostaticsSolverSettings(solverSettings, testBench));
            }

            if (calmSolverCnt == 1)
            {
                CyPhy.CalmWaterSolverSettings solverSettings = testBench.Children.CalmWaterSolverSettingsCollection.First();

                if (solverSettings.Attributes.VehicleVelocity > MaxVelocity ||
                    solverSettings.Attributes.VehicleVelocity < MinVelocity)
                {
                    string message = string.Format(
                        "'{0}': 'Velocity' attribute must be [{1}-{2}]",
                        solverSettings.Name,
                        MinVelocity,
                        MaxVelocity);

                    Logger.Instance.AddLogMessage(
                        message,
                        Severity.Error);
                }

                if (solverSettings.Attributes.FluidTemperature < AbsZero ||
                    solverSettings.Attributes.FluidTemperature > MaxTemp)
                {
                    string message = string.Format(
                        "'{0}': 'FluidTemperature' attribute must be [{1} - {2}]",
                        solverSettings.Name,
                        AbsZero,
                        MaxTemp);

                    Logger.Instance.AddLogMessage(
                        message,
                        Severity.Error);
                }

                cfdConfigs.Add(new CFDConfig.CalmWaterSolverSettings(solverSettings, testBench));
                return;
            }

            if (waveSolverCnt == 1)
            {
                CyPhy.WaveResistanceSolverSettings solverSettings = testBench.Children.WaveResistanceSolverSettingsCollection.First();

                if (solverSettings.Attributes.VehicleVelocity > MaxVelocity ||
                    solverSettings.Attributes.VehicleVelocity < MinVelocity)
                {
                    string message = string.Format(
                        "'{0}': 'Velocity' attribute must be [{1}-{2}]",
                        solverSettings.Name,
                        MinVelocity,
                        MaxVelocity);

                    Logger.Instance.AddLogMessage(
                        message,
                        Severity.Error);
                }

                if (solverSettings.Attributes.FluidTemperature < AbsZero ||
                    solverSettings.Attributes.FluidTemperature > MaxTemp)
                {
                    string message = string.Format(
                        "'{0}': 'FluidTemperature' attribute must be [{1} - {2}]",
                        solverSettings.Name,
                        AbsZero,
                        MaxTemp);

                    Logger.Instance.AddLogMessage(
                        message,
                        Severity.Error);
                }

                if (solverSettings.Attributes.WaveFrequency < 0)
                {
                    string message = string.Format(
                        "'{0}': 'WaveFrequency' attribute must be greater than 0",
                        solverSettings.Name);

                    Logger.Instance.AddLogMessage(
                        message,
                        Severity.Error);
                }

                if (solverSettings.Attributes.WaveDirection < MinAngle ||
                    solverSettings.Attributes.WaveDirection > MaxAngle)
                {
                    string message = string.Format(
                        "'{0}': 'Angle' attributes must be [{1}-{2}]",
                        solverSettings.Name,
                        MinAngle,
                        MaxAngle);

                    Logger.Instance.AddLogMessage(
                        message,
                        Severity.Error);
                }

                cfdConfigs.Add(new CFDConfig.WaveResistanceSolverSettings(solverSettings, testBench));
                return;
            }

            if (correlationSolverCnt == 1)
            {
                CyPhy.CorrelationSettings solverSettings = testBench.Children.CorrelationSettingsCollection.First();

                if (solverSettings.Attributes.VehicleVelocity > MaxVelocity ||
                    solverSettings.Attributes.VehicleVelocity < 0)
                {
                    string message = string.Format(
                        "'{0}': 'Velocity' attribute must be [{1}-{2}]",
                        solverSettings.Name,
                        MinVelocity,
                        MaxVelocity);

                    Logger.Instance.AddLogMessage(
                        message,
                        Severity.Error);
                }

                cfdConfigs.Add(new CFDConfig.CorrelationSolverSettings(solverSettings, testBench));
                return;
            }

            // Post Processing Blocks
            foreach (var postprocess in testBench.Children.PostProcessingCollection)
            {
                PostProcessScripts.Add(postprocess.Attributes.ScriptPath);
            }

        }


        public void GenerateCFDConfig()
        {
            string configOutputDir = Path.Combine(this.OutputDirectory, "CFD");
            if (!Directory.Exists(configOutputDir))
                Directory.CreateDirectory(configOutputDir);

            // Hydrostatics solver setting will go to the CadAssembly.xml as it is interpreted by CadCreateAssembly.exe
            foreach (var config in cfdConfigs.Where(cfg => !(cfg is CFDConfig.HydrostaticsSolverSettings)))
            {
                config.SerializeToJson(configOutputDir);
            }
        }

        public void GenerateOpenFoamScript()
        {
            using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "runOpenFoam.sh")))
            {
                writer.Write("#!/bin/sh\n");
                writer.Write("cd CFD/OpenFoam/case\n");
                writer.Write("chmod 755 Allrun.sh\n");
                writer.Write("sh Allrun.sh");
            }

            /*
            Template.run_openfoam runopenfoam = new Template.run_openfoam();
            using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "runOpenFoam.sh")))
            {
                writer.WriteLine(runopenfoam.TransformText());
            }
            */
        }

        
        public override void GenerateCADXMLOutput()
        {
            CAD.AssembliesType assembliesRoot = cadDataContainer.ToCADXMLOutput(this);
            if (assembliesRoot.Assembly.Length > 0)
            {
                if (assembliesRoot.Assembly[0].CADComponent != null)
                {
                    // META-2262: Modify orientation here instead of in CADData.cs
                    ModifyRootComponentOrientation(assembliesRoot.Assembly[0].CADComponent,
                                                   assembliesRoot.Assembly[0].CADComponent.ComponentID);
                }

                AddAnalysisToXMLOutput(assembliesRoot.Assembly[0]);
            }

            AddDataExchangeFormatToXMLOutput(assembliesRoot);

            assembliesRoot.SerializeToFile(Path.Combine(OutputDirectory, TestBenchBase.CADAssemblyFile));
        }

        protected override void AddAnalysisToXMLOutput(CAD.AssemblyType cadassembly)
        {
            base.AddAnalysisToXMLOutput(cadassembly);
            CAD.AnalysesType cadanalysis = GetCADAnalysis(cadassembly);
            
            CAD.CFDType cfdanalysis = new CAD.CFDType();
            cfdanalysis._id = UtilityHelpers.MakeUdmID();
            cfdanalysis.AnalysisID = AnalysisID;

            List<CAD.MetricType> metrics = MetricsToCADXMLOutput(cadassembly.ConfigurationID);
            if (metrics.Count > 0)
            {
                cfdanalysis.Metrics = new CAD.MetricsType();
                cfdanalysis.Metrics._id = UtilityHelpers.MakeUdmID();
                cfdanalysis.Metrics.Metric = metrics.ToArray();
            }

            // Hydrostatics solver setting will go to the CadAssembly.xml as it is interpreted by CadCreateAssembly.exe
            foreach (var config in cfdConfigs.Where(cfg => cfg is CFDConfig.HydrostaticsSolverSettings))
            {
                CFDConfig.HydrostaticsSolverSettings hconfig = config as CFDConfig.HydrostaticsSolverSettings;
                cfdanalysis.HydrostaticsSolverSettings = new CAD.HydrostaticsSolverSettingsType[1];
                cfdanalysis.HydrostaticsSolverSettings[0] = new CAD.HydrostaticsSolverSettingsType()
                {
                    VehicleVelocity = 1,
                    FluidMaterial = hconfig.FluidMaterial.ToString(),
                    FluidTemperature = hconfig.FluidTemperature,
                    PitchAngleStart = hconfig.PitchAngleStart,
                    PitchAngleEnd = hconfig.PitchAngleEnd,
                    PitchAngleStepSize = hconfig.PitchAngleStep,
                    RollAngleStart = hconfig.RollAngleStart,
                    RollAngleEnd = hconfig.RollAngleEnd,
                    RollAngleStepSize = hconfig.RollAngleStep
                };
            }

            cadanalysis.CFD = new CAD.CFDType[] { cfdanalysis };
           
        }

        public override bool GenerateOutputFiles()
        {
            if (!base.HasErrors())
            {
                GenerateCADXMLOutput();
                GenerateCFDConfig();
                GenerateRunBat();
                GenerateScriptFiles();

                string[] openFoamKinds = new string[] {
                        typeof(CFDConfig.CalmWaterSolverSettings).Name,
                        typeof(CFDConfig.CorrelationSolverSettings).Name,
                        typeof(CFDConfig.WaveResistanceSolverSettings).Name };

                if (cfdConfigs.Any(x => openFoamKinds.Contains(x.GetType().Name)))
                {
                    GenerateOpenFoamScript();
                    CopyCFDPreprocessing();
                }
                GenerateProcessingScripts(PostProcessScripts);
                    
                return true;
            }
            return false;
        }

        public override void GenerateRunBat()
        {
            Template.run_bat_cfd searchmeta = new Template.run_bat_cfd()
            {
                Automation = IsAutomated,
            };
            using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "runCADJob.bat")))
            {
                writer.WriteLine(searchmeta.TransformText());
            }

            string keyName = @"HKEY_LOCAL_MACHINE\Software\META";
            string value = @"META_PATH";

            string metaPath = (string)Registry.GetValue(
                keyName,
                value,
                "C:\\Program Files (x86)\\META");

            if (!Directory.Exists(metaPath))
                Logger.Instance.AddLogMessage("META_PATH is not found in registry!", Severity.Error);

            string ergonomicsPath = Path.Combine(metaPath, @"analysis_tools\PYTHON_RICARDO\output_ergonomics\scripts");
            UtilityHelpers.CopyFiles(ergonomicsPath, Path.Combine(OutputDirectory, "scripts"));
        }

        private void CopyCFDPreprocessing()
        {
            META.AnalysisTool tool = META.AnalysisTool.GetByName("cfd_preprocessing");
            if (tool != null)
            {
                string cfdPreprocessDir = Path.Combine(tool.InstallLocation, tool.OutputDirectory);
                if (Directory.Exists(cfdPreprocessDir))
                {
                    UtilityHelpers.CopyFiles(cfdPreprocessDir,
                                             Path.Combine(OutputDirectory, "PreProcessing"));

                }
                else
                    Logger.Instance.AddLogMessage(cfdPreprocessDir + " does not exist. META analysis tool did not install correctly.", Severity.Warning);
            }
            else
                Logger.Instance.AddLogMessage("Could not retrieve information about CFD Preprocessing tool from registry!", Severity.Warning);

            using (StreamWriter writer = new StreamWriter(Path.Combine(this.OutputDirectory, "TestBench_PreProcess.cmd")))
            {
                Template.postprocess_cmd appendscript = new Template.postprocess_cmd()
                {
                    ScriptName = "main_pre_process.py"
                };

                writer.WriteLine(appendscript.TransformText());
            }

            List<string> ScriptPaths = new List<string>();
            ScriptPaths.Add("PreProcessing\\CFDPreprocess.py");
            using (StreamWriter writer = new StreamWriter(Path.Combine(this.OutputDirectory, "main_pre_process.py")))
            {
                Template.postprocess_py postpy = new Template.postprocess_py()
                {
                    ScriptNames = ScriptPaths,      //ScriptNames = scripts,
                    LogName = "pre_process.log"
                };
                writer.WriteLine(postpy.TransformText());
            }

        }

        // META-2262: Modify orientation here instead of in CADData.cs
        private void ModifyRootComponentOrientation(CAD.CADComponentType componentParent,
                                                    string assemblyID)
        {
            List<string> orientations = new List<string>();
            orientations.Add("ASM_TOP");
            orientations.Add("ASM_RIGHT");
            orientations.Add("ASM_FRONT");

            // Not doing recursive right now, just changing it at the first level
            //foreach (var aComponent in componentParent.CADComponent)
            if (componentParent.CADComponent != null)
            {
                foreach (var aComponent in componentParent.CADComponent)
                {
                    if (aComponent.Constraint != null)
                    {
                        foreach (var constraint in aComponent.Constraint)
                        {
                            if (constraint.Pair != null)
                            {
                                foreach (var pair in constraint.Pair)
                                {
                                    if (pair.ConstraintFeature != null)
                                    {
                                        foreach (var feature in pair.ConstraintFeature)
                                        {
                                            if (feature.ComponentID == assemblyID)
                                            {
                                                if (feature.FeatureName == "ASM_TOP")
                                                {
                                                    feature.FeatureName = "ASM_FRONT";
                                                    orientations.Remove("ASM_TOP");
                                                    if (!orientations.Any())
                                                        return;
                                                }
                                                else if (feature.FeatureName == "ASM_RIGHT")
                                                {
                                                    feature.FeatureName = "ASM_TOP";
                                                    orientations.Remove("ASM_RIGHT");
                                                    if (!orientations.Any())
                                                        return;
                                                }
                                                else if (feature.FeatureName == "ASM_FRONT")
                                                {
                                                    feature.FeatureName = "ASM_RIGHT";
                                                    orientations.Remove("ASM_FRONT");
                                                    if (!orientations.Any())
                                                        return;
                                                }
                                            }
                                        }
                                    }
                                }
                            }                    
                        }
                    }
                }
            }


        }

    }   // end class
}   // end namespace
