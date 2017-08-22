using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2CAD_CSharp.TestBenchModel
{
    public struct CustomShotline
    {
        public string BallisticThreatRef;
        public double Azimuth;
        public double Elevation;
        public string ComponentID;
        public string DatumPoint;
        public string ShotlineName;
    }

    public class BallisticTestBench : TestBenchBase
    {

        private Survivability.BallisticConfig ballisticConfig;
        public Survivability.ReferencePlaneTypeEnum referencePlaneType { get; set; }
        public List<CustomShotline> customShotlineList { get; set; }


        public BallisticTestBench(CyPhy2CADSettings cadSetting,
                                  string outputdir,
                                  string projectdir,
                                  bool auto = false) :
                                  base(cadSetting, outputdir, projectdir, auto)
        {
            ballisticConfig = new Survivability.BallisticConfig();
            customShotlineList = new List<CustomShotline>();
            referencePlaneType = Survivability.ReferencePlaneTypeEnum.None;
        }

        private void VerifyBallisticThreat(CyPhy.BallisticThreat threat)
        {
            if (threat is CyPhy.ProjectileBallisticThreat)
            {
                CyPhy.ProjectileBallisticThreat t = threat as CyPhy.ProjectileBallisticThreat;
                if (t.Attributes.Diameter <= 0)
                {
                    Logger.Instance.AddLogMessage("ProjectileBallisticThreat [" + threat.Name + "] diameter is <= 0", Severity.Warning);
                }
                if (t.Attributes.Length <= 0)
                {
                    Logger.Instance.AddLogMessage("ProjectileBallisticThreat [" + threat.Name + "] length is <= 0", Severity.Warning);
                }
                if (t.Attributes.Mass <= 0)
                {
                    Logger.Instance.AddLogMessage("ProjectileBallisticThreat [" + threat.Name + "] mass is <= 0", Severity.Warning);
                }
                if (t.Attributes.Speed <= 0)
                {
                    Logger.Instance.AddLogMessage("ProjectileBallisticThreat [" + threat.Name + "] speed is <= 0", Severity.Warning);
                }
            }
            else if (threat is CyPhy.ShapedChargeJetBallisticThreat)
            {
                CyPhy.ShapedChargeJetBallisticThreat t = threat as CyPhy.ShapedChargeJetBallisticThreat;
                if (t.Attributes.Diameter <= 0)
                {
                    Logger.Instance.AddLogMessage("ShapedChargeJetBallisticThreat [" + threat.Name + "] diameter is <= 0", Severity.Warning);
                }
                if (t.Attributes.Length <= 0)
                {
                    Logger.Instance.AddLogMessage("ShapedChargeJetBallisticThreat [" + threat.Name + "] length is <= 0", Severity.Warning);
                }
                if (t.Attributes.Mass <= 0)
                {
                    Logger.Instance.AddLogMessage("ShapedChargeJetBallisticThreat [" + threat.Name + "] mass is <= 0", Severity.Warning);
                }
                if (t.Attributes.Speed <= 0)
                {
                    Logger.Instance.AddLogMessage("ShapedChargeJetBallisticThreat [" + threat.Name + "] speed is <= 0", Severity.Warning);
                }
            }
        }

        private void VerifyPredefinedBallisticSuite(CyPhy.PredefinedBallisticSuite suite)
        {

        }

        private bool VerifyShotLineModel(CyPhy.ShotlineModel slinemodel)
        {
            if (!slinemodel.Children.AzimuthAngleCollection.Any())
            {
                Logger.Instance.AddLogMessage(String.Format("Shotline Model needs to have an Azimuth Angle: {0}", slinemodel.Name), Severity.Error);
                return true;
            }

            if (!slinemodel.Children.ElevationAngleCollection.Any())
            {
                Logger.Instance.AddLogMessage(String.Format("Shotline Model needs to have an Elevation Angle: {0}", slinemodel.Name), Severity.Error);
                return true;
            }


            if (slinemodel.DstConnections.ShotlineTargetCollection.Count() > 1)
            {
                Logger.Instance.AddLogMessage(String.Format("Shotline Model is connected to > 1 Shotline Target: {0}", slinemodel.Name), Severity.Error);
                return true;
            }

            if (!slinemodel.DstConnections.ShotlineTargetCollection.Any())
            {
                Logger.Instance.AddLogMessage(String.Format("Shotline Model is not connected to a Shotline Target: {0}", slinemodel.Name), Severity.Error);
                return true;
            }

            foreach (var a in slinemodel.Children.AzimuthAngleCollection)
            {
                if (a.Attributes.Value < -360 || a.Attributes.Value > 360)
                {
                    Logger.Instance.AddLogMessage("Azimuth angle is weird (" + a.Attributes.Value + ") in [" + slinemodel.Name + "]", Severity.Warning);

                }
            }

            foreach (var a in slinemodel.Children.ElevationAngleCollection)
            {
                if (a.Attributes.Value < -360 || a.Attributes.Value > 360)
                {
                    Logger.Instance.AddLogMessage("Elevation angle is weird (" + a.Attributes.Value + ") in [" + slinemodel.Name + "]", Severity.Warning);
                }
            }

            return false;

        }


        public override void TraverseTestBench(CyPhy.TestBenchType testBenchBase)
        {
            string stepFormat = "AP203_E2_Single_File";
            if (!STEP_DataExchangeFormats.Contains(stepFormat, StringComparer.OrdinalIgnoreCase))
                STEP_DataExchangeFormats.Add(stepFormat);

            CyPhy.BallisticTestBench testBench = testBenchBase as CyPhy.BallisticTestBench;
            if (testBench == null)
                testBench = CyPhyClasses.BallisticTestBench.Cast(testBenchBase.Impl);

            base.TraverseTestBench(testBenchBase);          //AnalysisID = testBench.ID;

            foreach (var item in testBench.Children.BallisticComputation2MetricCollection)
            {
                TBComputation tbcomputation = new TBComputation();

                if (item.SrcEnds.TotalIntersections != null)
                {
                    tbcomputation.ComputationType = TBComputation.Type.TOTALINTERSECTIONS;
                }
                else if (item.SrcEnds.TotalKills != null)
                {
                    tbcomputation.ComputationType = TBComputation.Type.TOTALKILLS;
                }
                else if (item.SrcEnds.TotalPerforations != null)
                {
                    tbcomputation.ComputationType = TBComputation.Type.TOTALPERFORATIONS;
                }
                else if (item.SrcEnds.TotalShots != null)
                {
                    tbcomputation.ComputationType = TBComputation.Type.TOTALSHOTS;
                }
                else
                {
                    Logger.Instance.AddLogMessage("Unknown Ballistic calculation: " + item.SrcEnd.Impl.Name, Severity.Error);
                }

                tbcomputation.MetricID = item.DstEnds.Metric.ID;
                tbcomputation.MetricName = item.Name;
                tbcomputation.RequestedValueType = "";

                Computations.Add(tbcomputation);
            }

            int predefinedCnt = testBench.Children.PredefinedBallisticSuiteCollection.Count();
            int customCnt = testBench.Children.ShotlineModelCollection.Count();

            if (predefinedCnt > 0)
            {
                if (predefinedCnt > 1)
                {
                    Logger.Instance.AddLogMessage("Ballistic testbench can only have at most 1 predefined shotline suite.", Severity.Error);
                    return;
                }

                if (customCnt > 0)
                {
                    Logger.Instance.AddLogMessage("Ballistic testbench can not have both predefined and custom shotline suites.", Severity.Error);
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

            // analysis
            Survivability.BallisticConfig.Analysis ballisticAnalysis = new Survivability.BallisticConfig.Analysis();
            ballisticAnalysis.ID = AnalysisID;


            if (predefinedCnt > 0)
            {
                ballisticAnalysis.suiteName = testBench.Children.PredefinedBallisticSuiteCollection.First().Attributes.Name;
            }
            else
            {                            
                ballisticAnalysis.suiteName = testBench.Name;
            }
            if (ballisticAnalysis.suiteName.Length == 0)
            {
                Logger.Instance.AddLogMessage("Ballistic analysis suite has no name specified.", Severity.Warning);
            }
            ballisticAnalysis.tier = (int)testBench.Attributes.Tier+1;      // starts at 0
            ballisticConfig.analysis = ballisticAnalysis;

            // ballistic threat
            foreach (var item in testBench.Children.BallisticThreatCollection)
            {
                VerifyBallisticThreat(item);
                Survivability.BallisticConfig.BallisticThreat threat = new Survivability.BallisticConfig.BallisticThreat();
                threat.diameter_meters = item.Attributes.Diameter;
                threat.length_meters = item.Attributes.Length;
                threat.materialRef = item.Attributes.Material;
                threat.speed_metersPerSec = item.Attributes.Speed;

                if (item.Kind == "ProjectileBallisticThreat")
                {
                    threat.type = Survivability.BallisticConfig.BallisticThreat.BallisticThreatTypeEnum.Ballistic;
                }
                else if (item.Kind == "ShapedChargeJetBallisticThreat")
                {
                    threat.type = Survivability.BallisticConfig.BallisticThreat.BallisticThreatTypeEnum.ShapedChargeJet;
                    threat.standoff_meters = (item as CyPhy.ShapedChargeJetBallisticThreat).Attributes.Standoff;
                    int chargeq = (int)(item as CyPhy.ShapedChargeJetBallisticThreat).Attributes.ChargeQuality;
                    threat.chargeQuality = (Survivability.BallisticConfig.BallisticThreat.ChargeQualityEnum)(chargeq);
                }
                threat.name = item.ID;

                ballisticConfig.ballisticThreats.Add(threat);
            }
           

            // critical components
            foreach (var item in testBench.Children.CriticalComponentCollection)
            {
                int type = (int)item.Attributes.Type;
                CyPhy.TIPRefBase refbase = item.Referred.TIPRefBase;
                if (refbase != null)
                {
                    if (refbase.Kind != "Component")
                    {
                        Logger.Instance.AddLogMessage(String.Format("Critical component must refer to a component: {0}", refbase.Name), Severity.Error);
                    }
                    else
                    {
                        Survivability.BallisticConfig.CriticalComponent criticalcomponent = new Survivability.BallisticConfig.CriticalComponent();
                        criticalcomponent.componentID = (refbase as CyPhy.Component).Attributes.InstanceGUID;
                        criticalcomponent.type = (Survivability.BallisticConfig.CriticalComponent.CriticalityTypeEnum)(type);
                        ballisticConfig.criticalComponents.Add(criticalcomponent);
                    }
                }
            }


            // file location
            // see GenerateCADXmlOutput();
 
            // custom shotline
            foreach (var item in testBench.Children.ShotlineModelCollection)
            {
                if (VerifyShotLineModel(item))
                    continue;

                string componentid = "";
                string datumname = "";

                CyPhy.ShotlineTarget target = item.DstConnections.ShotlineTargetCollection.First();
                CyPhy.Point ap = target.DstEnds.Point;
                PointMetricTraversal traverser = new PointMetricTraversal(ap);
                if (!traverser.portsFound.Any())
                {
                    Logger.Instance.AddLogMessage("Shotline Model is connected to an AnalysisPoint which does not end in a PointGeometry [" + item.Path + "]", Severity.Error);
                    continue;
                }

                datumname = (traverser.portsFound.First() as CyPhy.Point).Attributes.DatumName;
                CyPhy.Component targetComponent = CyPhyClasses.Component.Cast(traverser.portsFound.First().ParentContainer.ParentContainer.Impl);
                componentid = targetComponent.Attributes.InstanceGUID;

                foreach (var conn in item.SrcConnections.Threat2ShotlineSuiteCollection)
                {
                    CyPhy.BallisticThreat threat = conn.SrcEnds.BallisticThreat;

                    CustomShotline customshotline = new CustomShotline();
                    customshotline.Azimuth = item.Children.AzimuthAngleCollection.First().Attributes.Value;
                    customshotline.Elevation = item.Children.ElevationAngleCollection.First().Attributes.Value;
                    customshotline.BallisticThreatRef = threat.ID;
                    customshotline.ComponentID = componentid;
                    customshotline.DatumPoint = datumname;
                    customshotline.ShotlineName = item.Attributes.Name; 

                    customShotlineList.Add(customshotline);
                }

            }
        }

        protected override void AddAnalysisToXMLOutput(CAD.AssemblyType cadassembly)
        {
            base.AddAnalysisToXMLOutput(cadassembly);
            CAD.AnalysesType cadanalysis = GetCADAnalysis(cadassembly);

            CAD.BallisticType ballisticanalysis = new CAD.BallisticType();
            ballisticanalysis._id = UtilityHelpers.MakeUdmID();
            ballisticanalysis.AnalysisID = AnalysisID;

            if (referencePlaneType != Survivability.ReferencePlaneTypeEnum.None)
            {
                CAD.ReferencePlaneType refplane = new CAD.ReferencePlaneType();
                refplane._id = UtilityHelpers.MakeUdmID();
                refplane.Type = referencePlaneType.ToString();
                ballisticanalysis.ReferencePlane = refplane;
            }

            if (customShotlineList.Any())
            {
                CAD.ShotlinesType cadshotlinesuite = new CAD.ShotlinesType();
                cadshotlinesuite._id = UtilityHelpers.MakeUdmID();
                List<CAD.ShotlineType> shotlineList = new List<CAD.ShotlineType>();
                foreach (var shotline in customShotlineList)
                {
                    CAD.ShotlineType cadshotline = new CAD.ShotlineType();
                    cadshotline._id = UtilityHelpers.MakeUdmID();
                    cadshotline.Azimuth = shotline.Azimuth;
                    cadshotline.Elevation = shotline.Elevation;
                    cadshotline.BallisticThreatRef = shotline.BallisticThreatRef;
                    cadshotline.ComponentID = shotline.ComponentID;
                    cadshotline.DatumPoint = shotline.DatumPoint;
                    cadshotline.ShotlineName = shotline.ShotlineName;
                    shotlineList.Add(cadshotline);
                }

                cadshotlinesuite.Shotline = shotlineList.ToArray();
                ballisticanalysis.Shotlines = cadshotlinesuite;
            }


            List<CAD.MetricType> metrics = MetricsToCADXMLOutput(Computations, cadassembly.ConfigurationID);
            if (metrics.Count > 0)
            {
                ballisticanalysis.Metrics = new CAD.MetricsType();
                ballisticanalysis.Metrics._id = UtilityHelpers.MakeUdmID();
                ballisticanalysis.Metrics.Metric = metrics.ToArray();
            }

            if (ballisticanalysis.Metrics != null || ballisticanalysis.ReferencePlane != null || ballisticanalysis.Shotlines != null)
            {
                cadanalysis.Ballistic = new CAD.BallisticType[] { ballisticanalysis };
            }
        }

        public void GenerateBallisticConfigJson()
        {
            ballisticConfig.SerializeToJson(this.OutputDirectory, "BallisticConfig.json");
        }

        public override void GenerateCADXMLOutput()
        {
            CAD.AssembliesType assembliesoutroot = cadDataContainer.ToCADXMLOutput(this);

            if (assembliesoutroot.Assembly.Length > 0)
            {
                AddAnalysisToXMLOutput(assembliesoutroot.Assembly[0]);

                // file location
                Survivability.FileLocation fileloc = new Survivability.FileLocation();
                fileloc.directory = "AP203_E2_SINGLE_FILE";
                fileloc.name = assembliesoutroot.Assembly[0].CADComponent.Name + "_asm.stp";
                fileloc.type = Survivability.FileLocation.FileTypeEnum.STEPFile;
                ballisticConfig.fileLocations.Add(fileloc);

                AddStaticAnalysisMetrics(assembliesoutroot.Assembly[0]);
            }

            AddDataExchangeFormatToXMLOutput(assembliesoutroot);
            assembliesoutroot.SerializeToFile(Path.Combine(OutputDirectory, TestBenchBase.CADAssemblyFile));


        }

        public override bool GenerateOutputFiles()
        {
            if (!base.HasErrors())
            {
                GenerateCADXMLOutput();
                GenerateBallisticConfigJson();
                GenerateRunBat();
                GenerateScriptFiles();
                return true;
            }
            return false;
        }

        public override void GenerateScriptFiles()
        {
            base.GenerateScriptFiles();

            if (this.ballisticConfig.analysis.tier >= 3)
            {
                string sh = File.ReadAllText(Path.Combine(META.VersionInfo.MetaPath, "bin", "run_CTH.sh"), Encoding.UTF8);
                File.WriteAllText(Path.Combine(OutputDirectory, "run_CTH.sh"), sh.Replace("\r", ""));
            }
        }

        public override void GenerateRunBat()
        {
            StringBuilder sbuilder = new StringBuilder();
            sbuilder.AppendLine("REM ****************************");
            sbuilder.AppendLine("REM Ballistic Tool");
            sbuilder.AppendLine("REM ****************************");

            sbuilder.AppendLine("set BALLISTIC_CONFIG_FILE=\"BallisticConfig.json\"");
            sbuilder.AppendLine("set BALLISTIC_EXE_FILE_NAME=ballistics.exe");
            sbuilder.AppendLine("set BALLISTIC_EXE=\"%SWRI_BALLISTIC_EXTENSIONS%\\%BALLISTIC_EXE_FILE_NAME%\"\n");

            sbuilder.AppendLine("if exist %BALLISTIC_EXE% goto  :BALLISTIC_EXE_FOUND");
            sbuilder.AppendLine("@echo off");
            sbuilder.AppendLine("echo		Error: Could not find %BALLISTIC_EXE_FILE_NAME%.");
            sbuilder.AppendLine("echo		Your system is not properly configured to run %BALLISTIC_EXE_FILE_NAME%.");
            sbuilder.AppendLine("set ERROR_CODE=2");
            sbuilder.AppendLine("set ERROR_MSG=\"Error: Could not find ballistics.exe.\"\n");
            sbuilder.AppendLine("goto :ERROR_SECTION");

            sbuilder.AppendLine(":BALLISTIC_EXE_FOUND");
            sbuilder.AppendLine("%BALLISTIC_EXE% %BALLISTIC_CONFIG_FILE% --no-display\n");

            sbuilder.AppendLine("set ERROR_CODE=%ERRORLEVEL%");
            sbuilder.AppendLine("if %ERRORLEVEL% NEQ 0 (");
            sbuilder.AppendLine("set ERROR_MSG=\"ballistics.exe exited with error, exit code %ERROR_CODE%\"");
            sbuilder.AppendLine("goto :ERROR_SECTION");
            sbuilder.AppendLine(")");

            if (this.ballisticConfig.analysis.tier >= 3)
            {
                sbuilder.AppendLine("rd /s/q AP203_E2_SINGLE_FILE");
                sbuilder.AppendLine("del /f/q *.prt.* *.asm.*");
                sbuilder.AppendLine("rem ignore delete failures");
                sbuilder.AppendLine("cmd /c \"exit /b 0\"");
            }


            Template.run_bat searchmeta = new Template.run_bat()
            {
                Automation = IsAutomated,
                XMLFileName = "CADAssembly",
                Assembler = "CREO",
                Mesher = "NONE",
                Analyzer = "NONE",
                CallDomainTool = sbuilder.ToString()
            };
            using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "runCADJob.bat")))
            {
                writer.WriteLine(searchmeta.TransformText());
            }


            // shotline viewer cmd file
            Template.run_shotlineviewer viewer = new Template.run_shotlineviewer();
            using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "runShotlineViewer.bat")))
            {
                writer.WriteLine(viewer.TransformText());
            }

        }

    }
}
