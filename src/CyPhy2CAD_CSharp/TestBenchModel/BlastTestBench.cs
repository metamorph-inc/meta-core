using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2CAD_CSharp.TestBenchModel
{
    public class BlastTestBench : TestBenchBase
    {

        public Survivability.BlastConfig blastConfig { get; set; }
        public Survivability.ReferencePlaneTypeEnum referencePlaneType { get; set; }     
        
        public BlastTestBench(CyPhy2CADSettings cadSetting,
                                  string outputdir,
                                  string projectdir,
                                  bool auto = false) :
                                  base(cadSetting, outputdir, projectdir, auto)
        {
            blastConfig = new Survivability.BlastConfig();
        }


        public override void TraverseTestBench(CyPhy.TestBenchType testBenchBase)
        {
            CyPhy.BlastTestBench testBench = testBenchBase as CyPhy.BlastTestBench;
            if (testBench == null)
                testBench = CyPhyClasses.BlastTestBench.Cast(testBenchBase.Impl);

            base.TraverseTestBench(testBenchBase);

            foreach (var item in testBench.Children.BlastComputation2MetricCollection)
            {
                CyPhy.BlastComputationType cadcomputation = item.SrcEnds.BlastComputationType;

                TBComputation tbcomputation = new TBComputation();
                tbcomputation.MetricID = item.DstEnds.Metric.ID;
                if (cadcomputation is CyPhy.MaxHorizonalVelocity)
                {
                    tbcomputation.ComputationType = TBComputation.Type.MAXHORIZONALVELOCITY;
                } else if (cadcomputation is CyPhy.MaxVerticalJump)
                {
                    tbcomputation.ComputationType = TBComputation.Type.MAXVERTICALJUMP;
                } else if (cadcomputation is CyPhy.MaxVerticalVelocity)
                {
                    tbcomputation.ComputationType = TBComputation.Type.MAXVERTICALVELOCITY;
                }
                tbcomputation.RequestedValueType = "";

                Computations.Add(tbcomputation);
            }

            int predefinedCnt = testBench.Children.PredefinedBlastSuiteCollection.Count();
            int customCnt = testBench.Children.BlastModelCollection.Count();

            if (predefinedCnt > 0)
            {
                if (predefinedCnt > 1)
                {
                    Logger.Instance.AddLogMessage("Blast testbench can only have at most 1 predefined blast suite.", Severity.Error);
                    return;
                }

                if (customCnt > 0)
                {
                    Logger.Instance.AddLogMessage("Blast testbench can not have both predefined and custom blast suite.", Severity.Error);
                    return;
                }
            }

            // reference plane
            if (testBench.Children.ReferencePlaneCollection.Any())
            {
                if (testBench.Children.ReferencePlaneCollection.First().Attributes.ReferencePlaneType == 0)
                {
                    referencePlaneType = Survivability.ReferencePlaneTypeEnum.Ground;
                }
                else
                {
                    referencePlaneType = Survivability.ReferencePlaneTypeEnum.Waterline;
                }
            }

            Survivability.BlastConfig.Analysis blastAnalysis = new Survivability.BlastConfig.Analysis();
            blastAnalysis.ID = AnalysisID;            //blastAnalysis.ID = testBench.ID;

            blastAnalysis.numDisplayTimeSteps = testBench.Attributes.DisplayTimeSteps;
            blastAnalysis.duration_seconds = testBench.Attributes.Duration;

            if (predefinedCnt > 0)
            {
                blastAnalysis.suiteName = testBench.Children.PredefinedBlastSuiteCollection.First().Attributes.BlastSuiteName;
            }

            if (customCnt > 0)
            {
                blastAnalysis.suiteName = testBench.Children.BlastModelCollection.First().Attributes.BlastSuiteName;
            }

            blastAnalysis.tier = (int)testBench.Attributes.BlastTier + 1;      // starts at 0
            blastConfig.analysis = blastAnalysis;

            string stepFormat;
            if (blastAnalysis.tier > 1)
            {
                stepFormat = "AP203_E2_Separate_Part_Files";
            }
            else
            {
                stepFormat = "AP203_E2_Single_File";
            }

            if (!DataExchangeFormats.Contains(stepFormat))
                DataExchangeFormats.Add(stepFormat);

            // blast threat
            if (customCnt > 0)
            {
                CyPhy.BlastModel blastSuite = testBench.Children.BlastModelCollection.First();
                if (!blastSuite.Children.BlastThreatCollection.Any())
                {
                    Logger.Instance.AddLogMessage("Custom blast testbench must contain a blast threat within a blast suite.", Severity.Error);
                    return;
                }

                if (!blastSuite.Children.ChargeLocationCollection.Any())
                {
                    Logger.Instance.AddLogMessage("Custom blast testbench must contain a charge location within a blast suite.", Severity.Error);
                    return;
                }

                CyPhy.BlastThreat threat = blastSuite.Children.BlastThreatCollection.First();
                Survivability.BlastConfig.BlastThreat configThreat = new Survivability.BlastConfig.BlastThreat();
                configThreat.burialDepth_meters = threat.Attributes.ThreatBurialDepth;
                configThreat.chargeSize_kgOfTNT = threat.Attributes.TNTChargeSize;
                int tTypeint = (int)threat.Attributes.ThreatType;
                configThreat.type = (Survivability.BlastConfig.BlastThreatTypeEnum)tTypeint;


                Survivability.BlastConfig.Shape shape = new Survivability.BlastConfig.Shape();
                shape.height_meters = threat.Attributes.ThreatShapeHeight;
                shape.radius_meters = threat.Attributes.ThreatShapeRadius;
                int sTypeint = (int)threat.Attributes.ThreatShapeType;
                shape.type = (Survivability.BlastConfig.Shape.ShapeTypeEnum)sTypeint;
                configThreat.shape = shape;

                CyPhy.ChargeLocation loc = blastSuite.Children.ChargeLocationCollection.First();
                Survivability.BlastConfig.ChargeLocation configChargeLoc = new Survivability.BlastConfig.ChargeLocation();
                configChargeLoc.x_meters = loc.Attributes.X;
                configChargeLoc.y_meters = loc.Attributes.Y;
                configChargeLoc.z_meters = loc.Attributes.Z;
                configThreat.chargeLocation = configChargeLoc;

                blastConfig.blastThreat = configThreat;
            }
        }

        public void GenerateConfigJson()
        {
            blastConfig.SerializeToJson(this.OutputDirectory, "BlastConfig.json");
        }

        public override void GenerateCADXMLOutput()
        {
            CAD.AssembliesType assembliesoutroot = cadDataContainer.ToCADXMLOutput(this);
            if (assembliesoutroot.Assembly.Length > 0)
            {
                AddAnalysisToXMLOutput(assembliesoutroot.Assembly[0]);

                // file location
                Survivability.FileLocation fileloc = new Survivability.FileLocation();
                if (blastConfig.analysis.tier > 1)
                    fileloc.directory = "AP203_E2_SEPARATE_PART_FILES";
                else
                    fileloc.directory = "AP203_E2_SINGLE_FILE";
                fileloc.name = assembliesoutroot.Assembly[0].CADComponent.Name + "_asm.stp";
                fileloc.type = Survivability.FileLocation.FileTypeEnum.STEPFile;
                blastConfig.fileLocations.Add(fileloc);
            }

            AddDataExchangeFormatToXMLOutput(assembliesoutroot);
            assembliesoutroot.SerializeToFile(Path.Combine(OutputDirectory, TestBenchBase.CADAssemblyFile));
        }

        protected override void AddAnalysisToXMLOutput(CAD.AssemblyType cadassembly)
        {
            base.AddAnalysisToXMLOutput(cadassembly);
            CAD.AnalysesType cadanalysis = GetCADAnalysis(cadassembly);

            CAD.BlastType blastanalysis = new CAD.BlastType();
            blastanalysis._id = UtilityHelpers.MakeUdmID();
            blastanalysis.AnalysisID = blastConfig.analysis.ID;
            if (referencePlaneType != Survivability.ReferencePlaneTypeEnum.None)
            {
                CAD.ReferencePlaneType refplane = new CAD.ReferencePlaneType();
                refplane._id = UtilityHelpers.MakeUdmID();
                refplane.Type = referencePlaneType.ToString();
                blastanalysis.ReferencePlane = new CAD.ReferencePlaneType[1];            // cardinality should be changed to have 0..1 in CAD_Uml.xme
                blastanalysis.ReferencePlane[0] = refplane;                      
            }

            List<CAD.MetricType> metrics = MetricsToCADXMLOutput(cadassembly.ConfigurationID);
            if (metrics.Count > 0)
            {
                blastanalysis.Metrics = new CAD.MetricsType();
                blastanalysis.Metrics._id = UtilityHelpers.MakeUdmID();
                blastanalysis.Metrics.Metric = metrics.ToArray();
            }

            if (blastanalysis.ReferencePlane != null || blastanalysis.Metrics != null)
            {
                cadanalysis.Blast = new CAD.BlastType[] { blastanalysis };
            }
        }

        public override bool GenerateOutputFiles()
        {
            if (!base.HasErrors())
            {
                GenerateCADXMLOutput();
                GenerateConfigJson();
                GenerateRunBat();
                GenerateScriptFiles();
                return true;
            }
            return false;
        }

        public override void GenerateScriptFiles()
        {
            base.GenerateScriptFiles();

            // META-1718: LS-DYNA
            if (blastConfig.analysis.tier > 2)
            {
                var run_dyna = CyPhy2CAD_CSharp.Properties.Resources.runLSDyna;
                using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "runLSDyna.sh")))
                {
                    writer.Write(Encoding.UTF8.GetString(run_dyna).Replace("\r", ""));
                }
            }
        }

        public override void GenerateRunBat()
        {
            StringBuilder sbuilder = new StringBuilder();
            sbuilder.AppendLine();
            sbuilder.AppendLine("REM ****************************");
            sbuilder.AppendLine("REM Blast Tool");
            sbuilder.AppendLine("REM ****************************");
            sbuilder.AppendLine("set BLAST_CONFIG_FILE=\"BlastConfig.json\"");
            sbuilder.AppendLine("set BLAST_EXE_FILE_NAME=blast.exe");
            sbuilder.AppendLine("set BLAST_EXE=\"%SWRI_BALLISTIC_EXTENSIONS%\\%BLAST_EXE_FILE_NAME%\"\n");

            // FIXME if both %SWRI_BALLISTIC_EXTENSIONS% and %BLAST_EXE_FILE_NAME% are empty, BLAST_EXE==\ which always exists
            sbuilder.AppendLine("if exist %BLAST_EXE% goto  :BLAST_EXE_FOUND");
            sbuilder.AppendLine("@echo off");
            sbuilder.AppendLine("echo		Error: Could not find %BALLISTIC_EXE_FILE_NAME%.");
            sbuilder.AppendLine("echo		Your system is not properly configured to run %BALLISTIC_EXE_FILE_NAME%.");
            sbuilder.AppendLine("set ERROR_CODE=2");
            sbuilder.AppendLine("set ERROR_MSG=\"Error: Could not find ballistics.exe.\"");
            sbuilder.AppendLine("goto :ERROR_SECTION\n");

            sbuilder.AppendLine(":BLAST_EXE_FOUND");
            sbuilder.AppendLine("%BLAST_EXE% %BLAST_CONFIG_FILE%\n");

            sbuilder.AppendLine("set ERROR_CODE=%ERRORLEVEL%");
            sbuilder.AppendLine("if %ERRORLEVEL% NEQ 0 (");
            sbuilder.AppendLine("set ERROR_MSG=\"blast.exe exited with error, exit code %ERROR_CODE%\"");
            sbuilder.AppendLine("goto :ERROR_SECTION");
            sbuilder.AppendLine(")");

            Template.run_bat searchmeta = new Template.run_bat()
            {
                Automation = IsAutomated,
                XMLFileName = "CADAssembly",
             //   ComputedMetricsPath = "ComputedValues.xml",
                Assembler = "CREO",
                Mesher = "NONE",
                Analyzer = "NONE",
                CallDomainTool = sbuilder.ToString()
            };
            using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "runCADJob.bat")))
            {
                writer.WriteLine(searchmeta.TransformText());
            }
        }
    }
}
