using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using GME.MGA;

namespace CyPhy2CAD_CSharp.TestBenchModel
{
    /*
    public class CarComputation
    {
        public enum ComputationType
        {
            TURNRADIUS,
            TOPSPEED,
            ACCEL0_60,
        }
        public ComputationType Type;
        public Dictionary<String, String> Parameters = new Dictionary<string, string>();
        public string Metric;
    }
    */
    public class TestBench : TestBenchBase
    {
        // list of assemblies
        // stuff from test bench: analysis point, cadcomputation, metrics

        public List<string> PostProcessScripts { get; set; }
        //public List<CarComputation> CarComputations = new List<CarComputation>();

        public TestBench(CyPhy2CADSettings cadSetting,
                         string outputdir,
                         string projectdir,
                         bool auto = false) :
                         base(cadSetting, outputdir, projectdir, auto)
        {
            PostProcessScripts = new List<string>();
        }


        public override void TraverseTestBench(CyPhy.TestBenchType testBenchBase)
        {
            CyPhy.TestBench testBench = testBenchBase as CyPhy.TestBench;
            if (testBench == null)
                testBench = CyPhyClasses.TestBench.Cast(testBenchBase.Impl);
            base.TraverseTestBench(testBenchBase);   //AnalysisID = testBench.ID;

            
            // CADComputations Metrics
            foreach (var conn in testBench.Children.CADComputation2MetricCollection)
            {
                CyPhy.CADComputationType cadcomputation = conn.SrcEnds.CADComputationType;

                TBComputation tbcomputation = new TBComputation();
                tbcomputation.MetricID = conn.DstEnds.Metric.ID;
                if (cadcomputation is CyPhy.CenterOfGravity)
                {
                    tbcomputation.RequestedValueType = (cadcomputation as CyPhy.CenterOfGravity).Attributes.CADComputationRequestedValue.ToString();
                    tbcomputation.ComputationType = TBComputation.Type.CENTEROFGRAVITY;
                }
                else if (cadcomputation is CyPhy.BoundingBox)
                {
                    tbcomputation.RequestedValueType = (cadcomputation as CyPhy.BoundingBox).Attributes.CADComputationRequestedValue.ToString();
                    tbcomputation.ComputationType = TBComputation.Type.BOUNDINGBOX;
                }
                else if (cadcomputation is CyPhy.InterferenceCount)
                {
                    tbcomputation.RequestedValueType = "Scalar";
                    tbcomputation.ComputationType = TBComputation.Type.INTERFERENCECOUNT;
                }
                else if (cadcomputation is CyPhy.Mass)
                {
                    tbcomputation.RequestedValueType = "Scalar";
                    tbcomputation.ComputationType = TBComputation.Type.MASS;
                }

                Computations.Add(tbcomputation);
            }

            // PointCoordinate Metrics
            foreach (var metric in testBench.Children.MetricCollection)
            {
                List<CyPhy.Point> points_list = new List<CyPhy.Point>();
                foreach (var pt in metric.SrcConnections.PointCoordinates2MetricCollection)
                {
                    points_list.Add(pt.SrcEnds.Point);
                }

                foreach (var pt in metric.DstConnections.PointCoordinates2MetricCollection)
                {
                    points_list.Add(pt.DstEnds.Point);
                }

                if (points_list.Any())
                {
                    if (points_list.Count() > 1)
                        Logger.Instance.AddLogMessage("Metric should not be connected to multiple Point datums in a test bench.", Severity.Error);

                    CyPhy.Point point = points_list.First();

                    PointMetricTraversal traverser = new PointMetricTraversal(point);
                    if (traverser.portsFound.Count() == 1)
                    {
                        TBComputation tbcomputation = new TBComputation();
                        tbcomputation.ComputationType = TBComputation.Type.POINTCOORDINATES;
                        tbcomputation.MetricID = metric.ID;
                        tbcomputation.RequestedValueType = "Vector";
                        tbcomputation.FeatureDatumName = (traverser.portsFound.First() as CyPhy.Point).Attributes.DatumName;
                        tbcomputation.ComponentID = CyPhyClasses.Component.Cast(traverser.portsFound.First().ParentContainer.ParentContainer.Impl).Attributes.InstanceGUID;
                        tbcomputation.MetricName = metric.Name;
                        Computations.Add(tbcomputation);
                    }
                }
            }

            // Post Processing Blocks
            foreach (var postprocess in testBench.Children.PostProcessingCollection)
            {
                PostProcessScripts.Add(postprocess.Attributes.ScriptPath);
            }
        }

        public override bool HasErrors()
        {
            int assemblyCnt = cadDataContainer.assemblies.Count(), orphanCnt = cadDataContainer.orphans.Count();
            if (assemblyCnt == 0 && orphanCnt == 0)
            {
                Logger.Instance.AddLogMessage("TestBench has no connected components and no components with valid cadmodel links. Nothing will be generated.", Severity.Error);
                return true;
            }

            if ((assemblyCnt > 1 || orphanCnt > 0) && Computations.Any())
            {
                Logger.Instance.AddLogMessage("Test Bench has islands of connected components and/or orphan components, metrics can not be computed. Please remove the metrics or orphan components from test bench and try again.", Severity.Error);
                return true;
            }

            if (Logger.Instance.ErrorCnt > 0)
            {
                Logger.Instance.AddLogMessage("There are errors! Interpreter can not proceed until errors have been corrected. Nothing is generated.", Severity.Error);
                string logfolder = "<a href=\"file:///" + Path.Combine(OutputDirectory, "log") + "\" target=\"_blank\">" + Path.Combine(OutputDirectory, "log") +"</a>";
		        Logger.Instance.AddLogMessage("See " + CyPhy2CAD_CSharp.Logger.LogFileName + " for details: " + logfolder, Severity.Warning);
                return true;
            }

            return false;
        }

        public override void GenerateRunBat()
        {
            Template.run_bat searchmeta = new Template.run_bat()
            {
                Automation = IsAutomated,
                XMLFileName = "CADAssembly",
                AdditionalOptions = CADOptions ?? "",
                Assembler = "CREO",
                Analyzer = "NONE",
                Mesher = "NONE"
            };
            /*string domaintool = "";
            foreach (var carcomp in CarComputations){
                string inputs="";
                foreach (KeyValuePair<string,string> i in carcomp.Parameters)
                {
                    if (i.Key == "torque_curve")
                    {
                        using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "torque_curve.txt")))
                        {
                            writer.WriteLine(i.Value);
                        }
                    }
                    else
                    {
                        inputs += i.Key + "=" + i.Value + ";";
                    }
                }
                domaintool += "\"%MetaPath%\\bin\\Python27\\Scripts\\Python.exe\" \"%MetaPath%\\bin\\CAD\\CarCalculators.py\" -calc " + carcomp.Type.ToString().ToLower() + " -inputs " + inputs + " -metric " + carcomp.Metric + "\n";
            }
            searchmeta.CallDomainTool = domaintool;*/
            using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "runCADJob.bat")))
            {
                writer.WriteLine(searchmeta.TransformText());
            }
        }

        // main function for generating output files
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
            if (ProcessingInstructions.Count != 0)
            {
                CAD.ProcessingInstructionsType instr = new CAD.ProcessingInstructionsType();
                CAD.ProcessingInstructionType[] instructions = new CAD.ProcessingInstructionType[ProcessingInstructions.Count];
                int j = 0;
                foreach (var i in ProcessingInstructions)
                {
                    instructions[j] = new CAD.ProcessingInstructionType();
                    instructions[j].Primary = i.Key;
                    instructions[j].Secondary = i.Value;
                }
                instr.ProcessingInstruction = instructions;
                assembliesoutroot.ProcessingInstructions = instr;
            }
            // R.O. 1/26/2015, InterferenceCheck deprecated. Now interference check is specified by adding a InterferenceCount to
            // a CADComputationComponent
            //if ((Computations.Any() || InterferenceCheck) && assembliesoutroot.Assembly.Length > 0)
            if (Computations.Any() && assembliesoutroot.Assembly.Length > 0)
            {
                AddAnalysisToXMLOutput(assembliesoutroot.Assembly[0]);
            }
            AddDataExchangeFormatToXMLOutput(assembliesoutroot);
            assembliesoutroot.SerializeToFile(Path.Combine(OutputDirectory, TestBenchBase.CADAssemblyFile));
        }

        // Computation - Metric
        protected override void AddAnalysisToXMLOutput(CAD.AssemblyType assemblyRoot)
        {
            base.AddAnalysisToXMLOutput(assemblyRoot);

            if (Computations.Any())
            {
                CAD.AnalysesType cadanalysis = GetCADAnalysis(assemblyRoot);

                CAD.StaticType staticanalysis = new CAD.StaticType();
                staticanalysis._id = UtilityHelpers.MakeUdmID();
                staticanalysis.AnalysisID = AnalysisID;

                List<CAD.MetricType> metriclist = new List<CAD.MetricType>();
                foreach (var item in Computations)
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
                        ptout.MetricName = item.MetricName;
                        ptout.ComponentID = String.IsNullOrEmpty(item.ComponentID) ? "" : item.ComponentID;     // PointCoordinate metric is tied to a specific Component  
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
                        metric.MetricName = item.MetricName;
                        metric.Details = "";
                        metriclist.Add(metric);
                    }
                }

                staticanalysis.Metrics = new CAD.MetricsType();
                staticanalysis.Metrics._id = UtilityHelpers.MakeUdmID();
                staticanalysis.Metrics.Metric = metriclist.ToArray();

                cadanalysis.Static = new CAD.StaticType[] { staticanalysis };
            }
        }

        /*
        public void GeneratePostProcessingScripts()
        {
            // generate + copy
            if (PostProcessScripts.Count > 0)
            {
                Template.postprocess_cmd appendscript = new Template.postprocess_cmd();
                using (StreamWriter writer = new StreamWriter(Path.Combine(this.OutputDirectory, "TestBench_PostProcess.cmd")))
                {
                    writer.WriteLine(appendscript.TransformText());
                }

                List<string> scripts = new List<string>();
                foreach (var item in PostProcessScripts)
                {
                    scripts.Add(Path.GetFileName(item));
                    
                    File.Copy(Path.Combine(ProjectDirectory, item), Path.Combine(OutputDirectory, Path.GetFileName(item)), true);
                }

                Template.postprocess_py postpy = new Template.postprocess_py()
                {
                    ScriptNames = scripts
                };
                using (StreamWriter writer = new StreamWriter(Path.Combine(this.OutputDirectory, "main_post_process.py")))
                {
                    writer.WriteLine(postpy.TransformText());
                }

                string commonscriptdir = Path.Combine(ProjectDirectory, "Post_Processing", "common");
                if (Directory.Exists(commonscriptdir))
                {
                    string destDirName = Path.Combine(OutputDirectory, "common");
				    if (!Directory.Exists(destDirName))
                        Directory.CreateDirectory(destDirName);               
                    
                    DirectoryInfo dir = new DirectoryInfo(commonscriptdir);
                    FileInfo[] files = dir.GetFiles();
                    foreach (FileInfo file in files)
                    {
                        string temppath = Path.Combine(destDirName, file.Name);
                        file.CopyTo(temppath, false);
                    }
                }
            }
        }
         */


    }  // end class
}   // end namespace
