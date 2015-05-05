using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using GME.MGA;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using System.Text.RegularExpressions;

namespace CyPhy2CAD_CSharp.TestBenchModel
{
    public struct TBComputationType
    {
        public string ComputationType;
        public string RequestedValueType;
        public string MetricID;
        public string FeatureDatumName;
        public string ComponentID;
        public string Details;
    }

    public class TestBenchBase
    {
        /// <summary>
        /// Returns <paramref name="orig"/> without single quotes or trailing backslashes, suitable for r'string'
        /// </summary>
        /// <param name="orig"></param>
        public static string SanitizePythonRawString(string orig)
        {
            return Regex.Replace(orig, "\\\\+$", "").Replace("'", "");
        }

        public static readonly string CADAssemblyFile = "CADAssembly.xml";

        public List<string> DataExchangeFormats { get; set; }
        public List<string> STLDataExchangeFormats { get; set; }
        public List<string> SpecialDataFormatInstructions { get; set; }
        public DataRep.CADContainer cadDataContainer    { get; set; }
        public string OutputDirectory { get; set; }
        public string ProjectDirectory { get; set; }
        public string CadAuxiliaryDirectory { get; set; }
        public bool IsAutomated { get; set; }
        public bool MetaLink    { get; set; } // true if Meta-Link invoked the intepreter
        public List<TBComputationType> Computations { get; set; }
        public string AnalysisID { get; set; }

        // Which CAD model representation to use in each case?
        protected string MakeRep { get; set; }
        protected string BuyRep { get; set; }
        protected string DefaultRep { get; set; }

        public bool InterferenceCheck { get; set; } // Adds an analysis type (only for CAD tets benches)
        public string CADOptions { get; set; } // Additional options for the CAD executable (CAD only)

        public bool CopySTL { get; set; }
        
        public TestBenchBase(CyPhy2CADSettings cadSetting,
                             string outputdir,
                             string projectdir,
                             bool auto = false)
        {
            DataExchangeFormats = new List<string>();
            DataExchangeFormats.AddRange(cadSetting.StepFormats);
            STLDataExchangeFormats = new List<string>();
            if (cadSetting.OtherDataFormat.STLAscii == true)
            {
                STLDataExchangeFormats.Add("ASCII");
            }

            if (cadSetting.OtherDataFormat.STLBinary == true)
            {
                STLDataExchangeFormats.Add("Binary");
            }
            if (cadSetting.OtherDataFormat.Inventor == true)
            {
                STLDataExchangeFormats.Add("Inventor");
            }
            SpecialDataFormatInstructions = new List<string>();
            if (cadSetting.SpecialInstructions.LeafAssembliesMetric == true)
                SpecialDataFormatInstructions.Add("LeafAssembliesMetrics");

            cadDataContainer = new DataRep.CADContainer("");
            OutputDirectory = outputdir;
            ProjectDirectory = projectdir;
            CadAuxiliaryDirectory = cadSetting.AuxiliaryDirectory??"";
            IsAutomated = auto;
            MetaLink = cadSetting.MetaLink;
            Computations = new List<TBComputationType>();
        }

        public string GetRepresentation(DataRep.CADComponent component)
        {
            string mbp = component.MakeOrBuyParam;
            if (mbp=="Buy" && !String.IsNullOrEmpty(BuyRep))
            {
                return BuyRep;
            } else if (mbp == "Make" && !String.IsNullOrEmpty(MakeRep))
            {
                return MakeRep;
            }
            else if (!String.IsNullOrEmpty(DefaultRep))
            {
                return DefaultRep;
            }
            else
            {
                return "";
            }
        }

        public string GetParameterValue(CyPhy.TestBenchType testBench, string paramname)
        {
            var param = testBench.Children.ParameterCollection.Where(p => p.Name == paramname);
            if (param.Any())
            {
                return param.First().Attributes.Value;
            }
            return null;
        }

        public virtual void TraverseTestBench(CyPhy.TestBenchType testBench)
        {
            AnalysisID = testBench.ID;

            MakeRep = GetParameterValue(testBench, "MAKE_REP");
            DefaultRep = GetParameterValue(testBench, "DEFAULT_REP");
            BuyRep = GetParameterValue(testBench, "BUY_REP");

            var interference = testBench.Children.ParameterCollection.Where(p => p.Name == "INTERFERENCE_CHECK");
            if (interference.Any())
            {
                if (interference.First().Attributes.Value == "1")
                {
                    InterferenceCheck = true;
                }
            }

            // snyako@isis.vanderbilt.edu: Collect additional CAD executable parameters from the assembly
            if (testBench.Children.TopLevelSystemUnderTestCollection.Any())
            {
                if (testBench.Children.TopLevelSystemUnderTestCollection.First().AllReferred is CyPhy.ComponentAssembly)
                {
                    var assembly = testBench.Children.TopLevelSystemUnderTestCollection.First().AllReferred as CyPhy.ComponentAssembly;
                    var param = assembly.Children.ParameterCollection.Where(p => p.Name.ToUpper() == "CADEXEPARAMS");
                    if (param.Any())
                    {
                        this.CADOptions = param.First().Attributes.Value;
                    }
                }
            }
            else if (testBench.Children.ComponentAssemblyCollection.Any())
            {
                var assembly = testBench.Children.ComponentAssemblyCollection.First();
                var param = assembly.Children.ParameterCollection.Where(p => p.Name.ToUpper() == "CADEXEPARAMS");
                if (param.Any())
                {
                    this.CADOptions = param.First().Attributes.Value;
                }
            }
        }

        public void AddDataExchangeFormatToXMLOutput(CAD.AssembliesType assembliesRoot)
        {
            if (DataExchangeFormats.Count > 0 || STLDataExchangeFormats.Count > 0 || SpecialDataFormatInstructions.Count > 0)
            {
                CAD.DataExchangeType dataexchangeout = new CAD.DataExchangeType();
                dataexchangeout._id = UtilityHelpers.MakeUdmID();

                List<CAD.STEPFormatType> exchangelist = new List<CAD.STEPFormatType>();
                foreach (var item in DataExchangeFormats)
                {
                    CAD.STEPFormatType formatout = new CAD.STEPFormatType();
                    formatout._id = UtilityHelpers.MakeUdmID();
                    formatout.Name = item;
                    exchangelist.Add(formatout);    
                }

                List<CAD.NonSTEPFormatType> stllist = new List<CAD.NonSTEPFormatType>();
                foreach (var item in STLDataExchangeFormats)
                {
                    CAD.NonSTEPFormatType formatout = new CAD.NonSTEPFormatType();
                    formatout._id = UtilityHelpers.MakeUdmID();

                    if (item == "Inventor" || item == "Parasolid")
                    {
                        formatout.FormatType = item;
                        formatout.FormatSubType = "";
                    }
                    else
                    {
                        formatout.FormatType = "Stereolithography";
                        formatout.FormatSubType = item.ToUpper();
                    }

                    stllist.Add(formatout);
                }

                List<CAD.SpecialDataFormatInstructionType> specialInst = new List<CAD.SpecialDataFormatInstructionType>();
                foreach (var item in SpecialDataFormatInstructions)
                {
                    CAD.SpecialDataFormatInstructionType formatout = new CAD.SpecialDataFormatInstructionType();
                    formatout._id = UtilityHelpers.MakeUdmID();
                    formatout.Instruction = item;
                    specialInst.Add(formatout);
                }

                if (exchangelist.Count > 0)
                    dataexchangeout.STEPFormat = exchangelist.ToArray();
                if (stllist.Count > 0)
                    dataexchangeout.NonSTEPFormat = stllist.ToArray();
                if (specialInst.Count > 0)
                    dataexchangeout.SpecialDataFormatInstruction = specialInst.ToArray();
                assembliesRoot.DataExchange = dataexchangeout;
            }
        }

        public void CollectDirectories()
        {
            cadDataContainer.CollectDirectories(ProjectDirectory, OutputDirectory);
        }

        public virtual bool HasErrors()
        {
            int assemblyCnt = cadDataContainer.assemblies.Count(), orphanCnt = cadDataContainer.orphans.Count();
            if (assemblyCnt == 0 && orphanCnt == 0)
            {
                Logger.Instance.AddLogMessage("TestBench has no connected components and no components with valid cadmodel links. Nothing will be generated.", Severity.Error);
                return true;
            }

            if (Logger.Instance.ErrorCnt > 0)
            {
                Logger.Instance.AddLogMessage("There are errors! Interpreter can not proceed until errors have been corrected. Nothing is generated.", Severity.Error);
                string logfolder = "<a href=\"file:///" + Path.Combine(OutputDirectory, "log") + "\" target=\"_blank\">" + Path.Combine(OutputDirectory, "log") + "</a>";
                Logger.Instance.AddLogMessage("See " + CyPhy2CAD_CSharp.Logger.LogFileName +" for details: " + logfolder, Severity.Warning);
                return true;
            }

            return false;
        }


        public virtual bool GenerateOutputFiles()
        {
            if (!HasErrors())
            {
                GenerateCADXMLOutput();
                GenerateRunBat();
                GenerateScriptFiles();
                return true;
            }
            return false;
        }

        public virtual void GenerateCADXMLOutput()
        {
            CAD.AssembliesType assembliesoutroot = cadDataContainer.ToCADXMLOutput(this,MetaLink);
            if (MetaLink)
            {
                CAD.ProcessingInstructionsType instr = new CAD.ProcessingInstructionsType();
                CAD.ProcessingInstructionType[] instructions = new CAD.ProcessingInstructionType[2];
                instructions[0] = new CAD.ProcessingInstructionType();
                instructions[1] = new CAD.ProcessingInstructionType();
                instructions[0].Primary = "UNIQUELY_NAME_ALL_CAD_MODEL_INSTANCES";
                instructions[1].Primary = "OUTPUT_JOINT_INFORMATION";
                instructions[0].Secondary = "";
                instructions[1].Secondary = "VALIDATE_JOINT_INFORMATION"; 
                instr.ProcessingInstruction = instructions;
                assembliesoutroot.ProcessingInstructions = instr;

            }
            AddDataExchangeFormatToXMLOutput(assembliesoutroot);
            assembliesoutroot.SerializeToFile(Path.Combine(OutputDirectory, TestBenchBase.CADAssemblyFile));
        }

        public List<CAD.MetricType> MetricsToCADXMLOutput(string componentID)
        {
            List<CAD.MetricType> metriclist = new List<CAD.MetricType>();
            foreach (var item in Computations)
            {
                CAD.MetricType metric = new CAD.MetricType();
                metric._id = UtilityHelpers.MakeUdmID();
                metric.ComponentID = componentID;
                metric.MetricID = item.MetricID;
                metric.RequestedValueType = "Scalar";
                metric.MetricType1 = item.ComputationType;
                metric.Details = "";
                metriclist.Add(metric);
            }

            return metriclist;
        }

        public virtual void GenerateScriptFiles()
        {
            // <local, remote>
            
            Dictionary<string, string> componentPaths = new Dictionary<string, string>();
            foreach (var item in cadDataContainer.CADManifestList)
            {
                try
                {
                    componentPaths.Add(item.Value.LocalFolderPath, item.Value.RemoteMETAProPath);
                }
                catch (ArgumentException)
                {
                    // A warning here is too excessive and is scares the user. This is part of the normal operation.
                    // Logger.Instance.AddLogMessage("Duplicate path in CADManifestlist: " + item.Value.LocalFolderPath + "=" + item.Value.RemoteMETAProPath, Severity.Warning);
                }
            }

            if (!MetaLink)
            {
                // cad manifest
                // META-2147: Workaround until VF + Adam implements the additional component in manifest.project.json to indicate the source of a component
                // empty cad manifest is generated for this workaround
                //cadDataContainer.GenerateCADManifest(this.OutputDirectory);
                CadManifestObjectList emptyCadManifest = new CadManifestObjectList();
                using (StreamWriter writer = new StreamWriter(Path.Combine(this.OutputDirectory, "cad.manifest.json")))
                {
                    string line = Newtonsoft.Json.JsonConvert.SerializeObject(emptyCadManifest,
                                                                              Newtonsoft.Json.Formatting.Indented);
                    writer.WriteLine(line);
                }

                // zip.py
                Template.zip_to_remote_py remotezip = new Template.zip_to_remote_py()
                {
                    CadFolders = componentPaths.Values.ToList()      //CadFolders = cadDataContainer.GetRemoteCadFolders()
                };
                using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "zip.py")))
                {
                    writer.WriteLine(remotezip.TransformText());
                }
            }
            string relativeAuxDir = "";
            // search_META.pro
            List<string> cad_search_paths = new List<string>();
            cad_search_paths.AddRange(componentPaths.Keys.ToList());       //cad_search_paths.AddRange(cadDataContainer.GetLocalCadFolders());
            if (CadAuxiliaryDirectory != "")
            {
                Uri uricad = new Uri(Path.GetFullPath(CadAuxiliaryDirectory));
                Uri uriout = new Uri(Path.Combine(OutputDirectory, "abc.xyz"));
                relativeAuxDir = Uri.UnescapeDataString(uriout.MakeRelativeUri(uricad).ToString().Replace('/', '\\'));
                cad_search_paths.Insert(0, relativeAuxDir);
            }

            Template.search_meta_local searchmeta = new Template.search_meta_local()
            {
                CadFolders = cad_search_paths
            };
            using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "search_META.pro")))
            {
                writer.WriteLine(searchmeta.TransformText());
            }

            // META-3442
            if (this.CopySTL)
            {
                Template.copy_bat copystl = new Template.copy_bat()
                {
                    CadFolders = cad_search_paths
                };
                using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "Copy_STL.cmd")))
                {
                    writer.WriteLine(copystl.TransformText());
                }
            }

            // copy.bat
            // META-2147: Workaround            //if (CadAuxiliaryDirectory != "")
            {
                Template.copy_parts_full_py copybat = new Template.copy_parts_full_py()
                {
                    CadFolders = componentPaths,
                    AuxFolder = relativeAuxDir
                };
                using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "Copy_Parts.py"))) //using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "Copy_Parts.bat")))
                {
                    writer.WriteLine(copybat.TransformText());
                }
            }
        }

        public virtual void GenerateRunBat()
        {
            Template.run_bat searchmeta = new Template.run_bat()
            {
                Automation = IsAutomated,
                XMLFileName = "CADAssembly",
                AdditionalOptions = CADOptions??"",
                CallDomainTool = ""
            };
            using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "runCreateCADAssembly.bat")))
            {
                writer.WriteLine(searchmeta.TransformText());
            }
        }

        protected CAD.AnalysesType GetCADAnalysis(CAD.AssemblyType assembly)
        {
            CAD.AnalysesType analysis;
            if (assembly.Analyses == null)
            {
                analysis = new CAD.AnalysesType();
                analysis._id = UtilityHelpers.MakeUdmID();
                assembly.Analyses = analysis;
            }
            else
            {
                analysis = assembly.Analyses;
            }
            return analysis;
        }

        protected virtual void AddAnalysisToXMLOutput(CAD.AssemblyType assembly)
        {
            if (InterferenceCheck)
            {
                CAD.AnalysesType cadanalysis = GetCADAnalysis(assembly);
                CAD.InterferenceType intfanalysis = new CAD.InterferenceType();
                intfanalysis._id = UtilityHelpers.MakeUdmID();
                intfanalysis.AnalysisID = AnalysisID;
                cadanalysis.Interference = new CAD.InterferenceType[] { intfanalysis };
            }
        }

        public void GenerateProcessingScripts(List<string> ScriptPaths, bool preProcess = false)
        {
            // generate + copy
            if (ScriptPaths.Count > 0)
            {
                string cmdName, pyName, dirName, logName;
                if (preProcess)
                {
                    cmdName = "TestBench_PreProcess.cmd";
                    pyName = "main_pre_process.py";
                    dirName = "PreProcessing";
                    logName = "pre_process.log";
                }
                else
                {
                    cmdName = "TestBench_PostProcess.cmd";
                    pyName = "main_post_process.py";
                    dirName = "PostProcessing";
                    logName = "post_process.log";
                }

                Template.postprocess_cmd appendscript = new Template.postprocess_cmd()
                {
                    ScriptName = pyName
                };
                //using (StreamWriter writer = new StreamWriter(Path.Combine(this.OutputDirectory, "TestBench_PostProcess.cmd")))
                using (StreamWriter writer = new StreamWriter(Path.Combine(this.OutputDirectory, cmdName)))
                {
                    writer.WriteLine(appendscript.TransformText());
                }

                List<string> scripts = new List<string>();
                foreach (var item in ScriptPaths)
                {
                    //scripts.Add(Path.GetFileName(item));
                    //dirName = Path.GetDirectoryName(item);

                    string outputScriptDir = Path.Combine(OutputDirectory, Path.GetDirectoryName(item));
                    if (!Directory.Exists(outputScriptDir))
                        Directory.CreateDirectory(outputScriptDir);

                    if (File.Exists(Path.Combine(ProjectDirectory, item)))
                        File.Copy(Path.Combine(ProjectDirectory, item), Path.Combine(outputScriptDir, Path.GetFileName(item)), true);
                }

                Template.postprocess_py postpy = new Template.postprocess_py()
                {
                    ScriptNames = ScriptPaths,      //ScriptNames = scripts,
                    LogName = logName
                };
                //using (StreamWriter writer = new StreamWriter(Path.Combine(this.OutputDirectory, "main_post_process.py")))
                using (StreamWriter writer = new StreamWriter(Path.Combine(this.OutputDirectory, pyName)))
                {
                    writer.WriteLine(postpy.TransformText());
                }

                string commonscriptdir = Path.Combine(ProjectDirectory, dirName, "common");
                if (Directory.Exists(commonscriptdir))
                {
                    string destDirName = Path.Combine(OutputDirectory, dirName, "common");
                    if (!Directory.Exists(destDirName))
                        Directory.CreateDirectory(destDirName);

                    DirectoryInfo dir = new DirectoryInfo(commonscriptdir);
                    FileInfo[] files = dir.GetFiles();
                    foreach (FileInfo file in files)
                    {
                        string temppath = Path.Combine(destDirName, file.Name);
                        file.CopyTo(temppath, true);
                    }
                }
            }
        }
    }
}
 