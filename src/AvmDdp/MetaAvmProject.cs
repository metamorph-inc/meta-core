// -----------------------------------------------------------------------
// <copyright file="MetaAvmProject.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace AVM.DDP
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.IO;
    using Newtonsoft.Json;
    using GME.MGA;
    using System.Diagnostics;
    using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
    using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
    using System.Diagnostics.Contracts;
    using ISIS.GME.Common.Interfaces;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class MetaAvmProject
    {

        public class Component
        {
            public string revision { get; set; }
            public string modelpath { get; set; }
            public string avmid { get; set; }
            public string version { get; set; }
            public string name { get; set; }

            public Component()
            {

            }

            public Component Clone()
            {
                return (Component)this.MemberwiseClone();
            }
        }

        public class Results
        {
            public List<string> UrlHints { get; set; }

            public Results()
            {
                UrlHints = new List<string>();
            }
        }

        public class Requirements
        {
            public List<string> UrlHints { get; set; }

            public string id { get; set; }
            public string vfLink { get; set; }
            public string text { get; set; }

            public Requirements()
            {
                UrlHints = new List<string>();
            }

        }

        public class ProjectClass
        {
            //public int NumberOfReferencedFiles { get; set; }
            public List<Component> Components { get; set; }
            public List<string> DesignSpaceModels { get; set; }
            public List<string> Configurations { get; set; }
            public List<string> TestBenches { get; set; }
            public Results Results { get; set; }
            public Requirements Requirements { get; set; }

            public string CyPhyProjectFileName { get; set; }

            public string LastModified { get; set; }

            public ProjectClass()
            {
                this.Components = new List<Component>();
                this.DesignSpaceModels = new List<string>();
                this.Configurations = new List<string>();
                this.TestBenches = new List<string>();
                this.Results = new Results();
                this.Requirements = new Requirements();
                this.LastModified = DateTime.Now.ToString("yyyy-MM-dd HH-mm-ss");
            }
        }

        public ProjectClass Project { get; set; }

        /// <summary>
        /// Use the static create method.
        /// </summary>
        public MetaAvmProject()
        {
            Project = new ProjectClass();
            infoTextWriter = Console.Out;
            errorTextWriter = Console.Error;
        }

        [JsonIgnore]
        public string OutputDirectory { get; set; }

        [JsonIgnore]
        public string m_filename { get; set; }
        private TextWriter infoTextWriter { get; set; }
        private TextWriter errorTextWriter { get; set; }

        [JsonIgnore]
        public Dictionary<string, Func<MgaFCO, CyPhy.TestBenchType>> TestbenchAndCompositeTypes
        {
            get
            {
                return new[] {
                        new { t = typeof(CyPhy.TestBench), cast = (Func<MgaFCO, CyPhy.TestBenchType>)(CyPhyClasses.TestBench.Cast) },
                        new { t = typeof(CyPhy.BallisticTestBench), cast = (Func<MgaFCO, CyPhy.TestBenchType>)(CyPhyClasses.BallisticTestBench.Cast) },
                        new { t = typeof(CyPhy.BlastTestBench), cast = (Func<MgaFCO, CyPhy.TestBenchType>)(CyPhyClasses.BlastTestBench.Cast) },
                        new { t = typeof(CyPhy.CADTestBench), cast = (Func<MgaFCO, CyPhy.TestBenchType>)(CyPhyClasses.CADTestBench.Cast) },
                        new { t = typeof(CyPhy.CFDTestBench), cast = (Func<MgaFCO, CyPhy.TestBenchType>)(CyPhyClasses.CFDTestBench.Cast) },
                        new { t = typeof(CyPhy.ParametricExploration), cast = (Func<MgaFCO, CyPhy.TestBenchType>)(fco =>
                        {
                            var tbRef = CyPhyClasses.ParametricExploration.Cast(fco).Children.TestBenchRefCollection.FirstOrDefault();
                            if (tbRef == null)
                            {
                                return null;
                            }
                            return tbRef.Referred.TestBenchType; })
                        },
                        new { t = typeof(CyPhy.TestBenchSuite), cast = (Func<MgaFCO, CyPhy.TestBenchType>)(fco => CyPhyClasses.ParametricExploration.Cast(fco)
                            .Children.TestBenchRefCollection.FirstOrDefault().Referred.TestBench) },
                    }.ToDictionary(t => t.t.Name, c => c.cast);
            }
        }

        public string Serialize(bool saveToFile = true, string filename = null)
        {
            string json = JsonConvert.SerializeObject(this, Newtonsoft.Json.Formatting.Indented);

            if (string.IsNullOrEmpty(filename))
            {
                filename = this.m_filename;
            }

            if (saveToFile)
            {
                File.WriteAllText(filename, json);
            }

            return json;
        }

        private string GetResultsFolder()
        {
            return Directory.CreateDirectory(Path.Combine(this.OutputDirectory, "results")).FullName;
        }

        private string GetDesignsFolder()
        {
            return Directory.CreateDirectory(Path.Combine(this.OutputDirectory, "designs")).FullName;
        }

        private string GetDesignSpacesFolder()
        {
            return Directory.CreateDirectory(Path.Combine(this.OutputDirectory, "design-space")).FullName;
        }

        private string GetTestbenchFolder()
        {
            return Directory.CreateDirectory(Path.Combine(this.OutputDirectory, "test-benches")).FullName;
        }

        public void SaveTestBenchManifest(string designName, string configurationName, string testBenchName, FCO expandedTestBenchType, string outputDir, DateTime analysisStartTime)
        {
            if (outputDir == null)
            {
                throw new ArgumentNullException("outputDirectory");
            }

            AVM.DDP.MetaTBManifest manifest = new AVM.DDP.MetaTBManifest();
            manifest.MakeManifest(expandedTestBenchType, outputDir);

            // design name fixture
            manifest.DesignName = designName;

            // test bench name fixture
            manifest.TestBench = testBenchName;

            manifest.CfgID = configurationName;

            manifest.Serialize(outputDir);

            this.UpdateResultsJson(expandedTestBenchType.Impl as MgaFCO, outputDir, analysisStartTime);
        }

        public void SaveTestBenchManifest(string designName, string configurationName, CyPhy.TestBenchType expandedTestBenchType, string outputDir, CyPhy.TestBenchType originalTestBenchType, DateTime analysisStartTime)
        {
            if (expandedTestBenchType == null)
            {
                throw new ArgumentNullException("expandedTestBenchType");
            }

            if (originalTestBenchType == null)
            {
                expandedTestBenchType = originalTestBenchType;
            }

            SaveTestBenchManifest(designName, configurationName, originalTestBenchType.Name, expandedTestBenchType, outputDir, analysisStartTime);
         }

        public bool SaveTestBench(CyPhy.TestBenchType testBenchType)
        {
            string testBenchFileFullPath = this.UpdateTestBenchJson(testBenchType.Impl as MgaFCO);

            string testBenchFile = Path.Combine(".", Path.GetFileName(Path.GetDirectoryName(testBenchFileFullPath)), Path.GetFileName(testBenchFileFullPath)).Replace('\\', '/');

            if (this.Project.TestBenches.Contains(testBenchFile) == false)
            {
                this.Project.TestBenches.Add(testBenchFile);
            }

            return true;
        }

        public bool SaveDesign(CyPhy.DesignEntity designEntity)
        {
            if (designEntity == null)
            {
                throw new ArgumentNullException("designEntity");
            }

            string outputFolder = "design-exporter-output";

            if (designEntity is CyPhy.DesignContainer)
            {
                outputFolder = this.GetDesignSpacesFolder();
            }
            else if (designEntity is CyPhy.ComponentAssembly)
            {
                outputFolder = this.GetDesignsFolder();
            }

            try
            {
                this.infoTextWriter.WriteLine("Exporting design {0}", designEntity.Name);
                var design = CyPhy2DesignInterchange.CyPhy2DesignInterchange.Convert(designEntity);
                //design.SaveToFile(Path.GetFullPath(Path.Combine(outputFolder, design.Name + ".adm")));
                XSD2CSharp.AvmXmlSerializer.SaveToFile(Path.GetFullPath(Path.Combine(outputFolder, design.Name + ".adm")), design);

                string designFileName = Path.Combine(".", Path.GetFileName(outputFolder), design.Name + ".adm").Replace('\\', '/');
                if (designEntity is CyPhy.DesignContainer)
                {
                    if (this.Project.DesignSpaceModels.Contains(designFileName) == false)
                    {
                        this.Project.DesignSpaceModels.Add(designFileName);
                    }
                }
                else if (designEntity is CyPhy.ComponentAssembly)
                {
                    if (this.Project.Configurations.Contains(designFileName) == false)
                    {
                        this.Project.Configurations.Add(designFileName);
                    }
                }

                return true;
            }
            catch (Exception ex)
            {
                this.errorTextWriter.WriteLine("Exception in CyPhy2DesignInterchange: {0}", ex);
                return false;
            }
        }

        /// <summary>
        /// Saves associated design.
        /// </summary>
        /// <param name="fco">TestBench, ParametricExploration, SoT</param>
        public bool SaveDesign(MgaFCO fco)
        {
            string designFolder = "designs";

            if (Directory.Exists(designFolder) == false)
            {
                Directory.CreateDirectory(designFolder);
            }

            CyPhy.TestBenchType tb;
            Func<MgaFCO, CyPhy.TestBenchType> cast;
            if (TestbenchAndCompositeTypes.TryGetValue(fco.Meta.Name, out cast) == false)
            {
                return false;
            }
            tb = cast(fco);
            if (tb == null)
            {
                return false;
            }

            var tlsut = tb.Children.TopLevelSystemUnderTestCollection.FirstOrDefault();
            if (tlsut == null)
            {
                return false;
            }
            if (tlsut.Referred.ComponentAssembly == null)
            {
                return false;
            }

            try
            {
                this.infoTextWriter.WriteLine("Exporting design {0}", fco.Name);
                var design = CyPhy2DesignInterchange.CyPhy2DesignInterchange.Convert(tlsut.Referred.ComponentAssembly);
                design.SaveToFile(Path.GetFullPath(Path.Combine(designFolder, design.Name + ".adm")));

                string designFileName = Path.Combine(".", designFolder, design.Name + ".adm").Replace('\\', '/');
                if (this.Project.Configurations.Contains(designFileName) == false)
                {
                    this.Project.Configurations.Add(designFileName);
                }

                return true;
            }
            catch (Exception ex)
            {
                this.errorTextWriter.WriteLine("Exception in CyPhy2DesignInterchange: {0}", ex);
                return false;
            }

        }

        /// <summary>
        /// Call this within a transaction.
        /// </summary>
        /// <param name="originalSubject"></param>
        /// <param name="singleFco"></param>
        /// <param name="OutputSubDir"></param>
        public void UpdateResultsJson(
            MgaFCO singleFco,
            string OutputSubDir,
            DateTime time)
        {
            string jsonFile = Path.Combine(this.GetResultsFolder(), "results.metaresults.json");
            AVM.DDP.MetaResults results = null;
            bool createdNew;
            using (System.Threading.Mutex jsonFileMutex = new System.Threading.Mutex(false, "results_metaresults_mutex", out createdNew))
            {
                jsonFileMutex.WaitOne();
                try
                {
                    if (File.Exists(jsonFile))
                    {
                        string content = "";
                        using (StreamReader reader = new StreamReader(jsonFile))
                        {
                            content = reader.ReadToEnd();
                        }

                        try
                        {
                            results = JsonConvert.DeserializeObject<AVM.DDP.MetaResults>(content);
                        }
                        catch (Newtonsoft.Json.JsonReaderException ex)
                        {
                            // Will this cause problems if we run it from the job manager??? Should we only log it istead?
                            throw new Exception(string.Format("{0} file is probably malformed. Not a valid json. {1}{2}", Path.GetFullPath(jsonFile), Environment.NewLine, ex.Message));
                        }

                        // TODO: remove broken links
                    }
                    else
                    {
                        results = new AVM.DDP.MetaResults();
                    }

                    AVM.DDP.MetaResults.Result thisResult = new AVM.DDP.MetaResults.Result();

                    thisResult.Summary = MakeRelativePath(
                        Path.GetDirectoryName(jsonFile),
                        Path.Combine(OutputSubDir, AVM.DDP.MetaTBManifest.TESTBENCH_FILENAME)).Replace('\\', '/');

                    thisResult.Time = time.ToString("yyyy-MM-dd HH-mm-ss");

                    if (string.IsNullOrWhiteSpace(singleFco.RegistryValue["TestBenchUniqueName"]) == false)
                    {
                        thisResult.TestBench = singleFco.RegistryValue["TestBenchUniqueName"] + ".testbench.json";
                    }

                    Func<MgaFCO, CyPhy.TestBenchType> cast;
                    if (TestbenchAndCompositeTypes.TryGetValue(singleFco.Meta.Name, out cast) && cast(singleFco) != null)
                    {
                        CyPhy.TestBenchType testBench = cast(singleFco);

                        var tlsut = testBench.Children.TopLevelSystemUnderTestCollection.FirstOrDefault();
                        if (tlsut != null)
                        {
                            if (tlsut.AllReferred is CyPhy.ComponentAssembly)
                            {
                                var cfg = tlsut.Referred.ComponentAssembly;
                                //thisResult.Design = cfg.Name + ".metadesign.json";

                                var cid = cfg.Attributes.ConfigurationUniqueID;
                                //this.ConfigurationUniqueID = cid;

                                if (string.IsNullOrWhiteSpace(cid))
                                {
                                    cid = Guid.NewGuid().ToString("B");
                                    cfg.Attributes.ConfigurationUniqueID = cid;
                                }

                                if (!string.IsNullOrEmpty(cid))
                                {
                                    try
                                    {
                                        Guid guid = new Guid(cid);
                                        thisResult.DesignID = guid.ToString("B");
                                    }
                                    catch (System.FormatException ex)
                                    {
                                        Trace.TraceError("{0} is not a vaild GUID.", cid);
                                        Trace.TraceError(ex.ToString());
                                    }
                                }
                            }
                        }
                    }

                    results.Results.Add(thisResult);

                    var dirname = Path.GetDirectoryName(jsonFile);
                    if (Directory.Exists(dirname) == false)
                    {
                        Directory.CreateDirectory(dirname);
                    }

                    using (StreamWriter writer = new StreamWriter(jsonFile))
                    {
                        writer.WriteLine(JsonConvert.SerializeObject(results, Newtonsoft.Json.Formatting.Indented));
                    }
                }
                finally
                {
                    jsonFileMutex.ReleaseMutex();
                    jsonFileMutex.Dispose();
                }
            }
        }

        public string UpdateTestBenchJson(MgaFCO testBench)
        {
            string testBenchPath = Path.Combine(
                Path.GetDirectoryName(this.m_filename),
                "test-benches");

            if (Directory.Exists(testBenchPath) == false)
            {
                Directory.CreateDirectory(testBenchPath);
            }

            string jsonFileName = Path.Combine(
                testBenchPath,
                testBench.Name + ".testbench.json");

            AVM.DDP.MetaTestBench metaTestBench = new AVM.DDP.MetaTestBench();
            metaTestBench.Name = testBench.Name;

            Dictionary<MgaFCO, AVM.DDP.MetaTestBench.Requirement> reqMap =
                new Dictionary<MgaFCO, AVM.DDP.MetaTestBench.Requirement>();

            Dictionary<MgaFCO, AVM.DDP.MetaTestBench.Metric> metricMap =
                new Dictionary<MgaFCO, AVM.DDP.MetaTestBench.Metric>();

            // TODO: metrics, parameters, requirements
            foreach (MgaFCO item in testBench.ChildObjects)
            {
                if (item.Meta.Name == typeof(CyPhy.Parameter).Name)
                {
                    AVM.DDP.MetaTestBench.Parameter p = new AVM.DDP.MetaTestBench.Parameter();
                    p.Name = item.Name;
                    p.Value = item.GetStrAttrByNameDisp("Value");

                    p.Unit = (item as MgaReference).Referred != null ?
                        (item as MgaReference).Referred.Name :
                        "";

                    metaTestBench.Parameters.Add(p);
                }
                else if (item.Meta.Name == typeof(CyPhy.Metric).Name)
                {
                    AVM.DDP.MetaTestBench.Metric m = new AVM.DDP.MetaTestBench.Metric();
                    m.Name = item.Name;
                    m.Value = null;

                    m.Unit = (item as MgaReference).Referred != null ?
                        (item as MgaReference).Referred.Name :
                        "";

                    metaTestBench.Metrics.Add(m);
                    metricMap.Add(item, m);
                }
                else if (item.Meta.Name == typeof(CyPhy.RequirementLinkRef).Name)
                {
                    MgaFCO requirement = (item as MgaReference).Referred;
                    if (requirement != null)
                    {
                        AVM.DDP.MetaTestBench.Requirement r = new AVM.DDP.MetaTestBench.Requirement();
                        r.Name = requirement.Name;
                        r.SourceRepository = requirement.GetStrAttrByNameDisp("SourceRepository");
                        r.ID = requirement.GetStrAttrByNameDisp("ID");
                        r.Text = requirement.GetStrAttrByNameDisp("Text");

                        metaTestBench.Requirements.Add(r);
                        reqMap.Add(item, r);
                    }
                }
            }

            // HACK: for testing only!
            metaTestBench.Metrics.ForEach(x => x.Requirement = metaTestBench.Requirements.FirstOrDefault());

            foreach (IMgaFCO item in testBench.ChildObjects)
            {
                // TODO: Metric2Requirement Link connection
            }

            using (StreamWriter writer = new StreamWriter(jsonFileName))
            {
                writer.WriteLine(JsonConvert.SerializeObject(
                    metaTestBench,
                    Newtonsoft.Json.Formatting.Indented,
                    new JsonSerializerSettings()
                    {
                        PreserveReferencesHandling = PreserveReferencesHandling.Objects,
                    }));
            }

            return jsonFileName;
        }

        /// <summary>
        /// Function to be invoked from SOT TestBench.
        /// </summary>
        /// <param name="outputDirectory"></param>
        /// <param name="testBenchType"></param>
        /// <param name="dependencies"></param>
        public void SaveTestBenchManifest(string outputDirectory, CyPhy.TestBenchType testBenchType, List<MetaTBManifest.Dependency> dependencies)
        {
            if (testBenchType == null)
            {
                throw new ArgumentNullException();
            }

            if (outputDirectory == null)
            {
                throw new ArgumentNullException();
            }

            AVM.DDP.MetaTBManifest manifest = new AVM.DDP.MetaTBManifest();
            manifest.MakeManifest(testBenchType, outputDirectory);
            manifest.Dependencies = dependencies;
            manifest.Serialize(outputDirectory);
        }

        public void SaveTestBenchManifest(string outputDirectory, CyPhy.TestBenchType testBenchType)
        {
            if (testBenchType == null)
            {
                throw new ArgumentNullException();
            }

            if (outputDirectory == null)
            {
                throw new ArgumentNullException();
            }

            AVM.DDP.MetaTBManifest manifest = new AVM.DDP.MetaTBManifest();
            manifest.MakeManifest(testBenchType, outputDirectory);
            manifest.Serialize(outputDirectory);
        }

        /// <summary>
        /// Call this function if a GME transaction is open.
        /// </summary>
        /// <param name="testBenchName"></param>
        /// <param name="interpreterOutputDir"></param>
        /// <param name="fco"></param>
        public void SaveSummaryReportJson(string interpreterOutputDir, MgaFCO fco)
        {
            // TODO: index result file!

            if (fco.Meta.Name == "TestBench" || fco.Meta.Name == "CFDTestBench" || fco.Meta.Name == "BallisticTestBench" || fco.Meta.Name == "BlastTestBench" || fco.Meta.Name == "CADTestBench")
            {
                CyPhy.TestBenchType tb = ISIS.GME.Dsml.CyPhyML.Classes.TestBenchType.Cast(fco as MgaObject);

                //AVM.DDP.MetaTBReport report = new AVM.DDP.MetaTBReport();

                //string fullpathFileName = Path.Combine(interpreterOutputDir, "summary.testresults.json");
                //if (File.Exists(fullpathFileName))
                //{
                //    report = JsonConvert.DeserializeObject<AVM.DDP.MetaTBReport>(File.ReadAllText(fullpathFileName));
                //}

                //report.GenerateSummary(
                //    tb,
                //    "summary.testresults.json",
                //    interpreterOutputDir);

                //                AVM.DDP.MetaTBManifest manifest = new AVM.DDP.MetaTBManifest();
                //              manifest.MakeManifest(tb, interpreterOutputDir);
                //            manifest.Serialize(interpreterOutputDir);
            }
        }


        public static MetaAvmProject Create(MgaProject project)
        {
            return MetaAvmProject.Create(MgaExtensions.MgaExtensions.GetProjectDirectoryPath(project), project);
        }

        /// <summary>
        /// Call this function within a transaction!
        /// </summary>
        /// <param name="outputDirectory"></param>
        /// <param name="project"></param>
        /// <param name="rootDS"></param>
        /// <param name="textWriter"></param>
        /// <returns></returns>
        public static MetaAvmProject Create(
            string outputDirectory,
            MgaProject project = null,
            MgaModel rootDS = null,
            GME.CSharp.GMEConsole console = null
            )
        {
            MetaAvmProject avmProj = new MetaAvmProject();

            string currentDir = Environment.CurrentDirectory;

            if (Directory.Exists(outputDirectory) == false)
                Directory.CreateDirectory(outputDirectory);

            Directory.SetCurrentDirectory(outputDirectory);

            string avmProjFileName = Path.GetFullPath(Path.Combine(outputDirectory, "manifest.project.json"));

            if (File.Exists(avmProjFileName))
            {
                string sjson = "{}";
                using (StreamReader reader = new StreamReader(avmProjFileName))
                {
                    sjson = reader.ReadToEnd();

                    try
                    {
                        avmProj = JsonConvert.DeserializeObject<AVM.DDP.MetaAvmProject>(sjson);
                    }
                    catch (Newtonsoft.Json.JsonReaderException ex)
                    {
                        throw new Exception(string.Format("{0} file is probably malformed. Not a valid json. {1}{2}", Path.GetFullPath(avmProjFileName), Environment.NewLine, ex.Message));
                    }
                }
            }

            if (console != null)
            {
                avmProj.infoTextWriter = console.Info;
                avmProj.errorTextWriter = console.Error;
            }

            avmProj.OutputDirectory = outputDirectory;
            avmProj.m_filename = avmProjFileName;


            avmProj.Project.LastModified = DateTime.Now.ToString("yyyy-MM-dd HH-mm-ss");

            if (avmProj.Project.Results.UrlHints.Contains("./results/results.metaresults.json") == false)
            {
                avmProj.Project.Results.UrlHints.Add("./results/results.metaresults.json");
            }

            if (avmProj.Project.Requirements.UrlHints.Contains("./requirements/requirements.json") == false)
            {
                avmProj.Project.Requirements.UrlHints.Add("./requirements/requirements.json");
            }

            string reqSourceRepository = string.Empty;
            string reqId = string.Empty;
            string reqText = string.Empty;

            if (project != null)
            {
                avmProj.Project.CyPhyProjectFileName = MgaExtensions.MgaExtensions.GetProjectName(project);

                CyPhy.RootFolder rf = ISIS.GME.Dsml.CyPhyML.Classes.RootFolder.GetRootFolder(project);

                var topLevelLinks = rf.Children.RequirementsCollection
                    .SelectMany(x => x.Children.RequirementLinkCollection);

                var defaults = topLevelLinks.Where(x => x.Name.ToLowerInvariant() == "default");
                var reqDefault = defaults.FirstOrDefault();
                if (defaults.Count() > 1)
                {
                    // more requ found first one will be used
                    string msg = string.Format("More than one Requirement was not found {0} in use.", reqDefault.Impl.AbsPath);
                    Trace.TraceWarning(msg);
                    avmProj.infoTextWriter.WriteLine(msg);

                }

                if (reqDefault != null)
                {
                    reqSourceRepository = reqDefault.Attributes.SourceRepository;
                    reqId = reqDefault.Attributes.ID;
                    reqText = reqDefault.Attributes.Text;
                }
                else
                {
                    // requ not found
                    string msg = "Requirement was not found.";
                    Trace.TraceWarning(msg);
                    
                    // RESTORE THIS MESSAGE ONCE WE SWITCH TO USING GMELOGGER
                    // avmProj.infoTextWriter.WriteLine(msg);
                }
            }

            avmProj.Project.Requirements.vfLink = reqSourceRepository;
            avmProj.Project.Requirements.id = reqId;
            avmProj.Project.Requirements.text = reqText;

            string dirName = Path.GetFullPath(Path.Combine(outputDirectory, "requirements"));

            if (Directory.Exists(dirName) == false)
            {
                Directory.CreateDirectory(dirName);
                string requFileName = Path.GetFullPath(Path.Combine(dirName, "requirements.json"));

                if (File.Exists(requFileName) == false)
                {
                    using (StreamWriter writer = new StreamWriter(requFileName))
                    {
                        // default requirement
                        writer.WriteLine(@"{");
                        writer.WriteLine("    \"name\": \"Undefined\",");
                        writer.WriteLine("    \"children\": []");
                        writer.WriteLine(@"}");
                    }
                }
            }

            string designSpaceFolder = "design-space";
            dirName = Path.GetFullPath(Path.Combine(outputDirectory, designSpaceFolder));

            if (Directory.Exists(dirName) == false)
            {
                Directory.CreateDirectory(dirName);
            }

            if (rootDS != null)
            {
                // export designspace
                var dsProjectJsonLink = Path.Combine(".", designSpaceFolder, rootDS.Name + ".adm").Replace('\\', '/');
                var dsFileName = Path.Combine(dirName, rootDS.Name + ".adm");

                if (avmProj.Project.DesignSpaceModels.Contains(dsProjectJsonLink) == false)
                {
                    avmProj.Project.DesignSpaceModels.Add(dsProjectJsonLink);
                }
                var designContainer = ISIS.GME.Dsml.CyPhyML.Classes.DesignContainer.Cast(rootDS);
                var design = CyPhy2DesignInterchange.CyPhy2DesignInterchange.Convert(designContainer);
                design.SaveToFile(Path.GetFullPath(dsFileName));
            }

            Directory.SetCurrentDirectory(currentDir);
            return avmProj;
        }

        /// <summary>
        /// Creates a relative path from one file or folder to another.
        /// </summary>
        /// <param name="fromPath">Contains the directory that defines the start of the relative path.</param>
        /// <param name="toPath">Contains the path that defines the endpoint of the relative path.</param>
        /// <returns>The relative path from the start directory to the end path.</returns>
        public static string MakeRelativePath(string fromPath, string toPath)
        {
            Contract.Requires(string.IsNullOrEmpty(fromPath) == false);
            Contract.Requires(string.IsNullOrEmpty(toPath) == false);

            Uri fromUri = new Uri(Path.GetFullPath(fromPath.Replace("%", "%25")), true);
            Uri toUri = new Uri(Path.GetFullPath(toPath.Replace("%", "%25")), true);

            Uri relativeUri = fromUri.MakeRelativeUri(toUri);
            string relativePath = Uri.UnescapeDataString(relativeUri.ToString());
            if (relativePath.StartsWith("../") == false)
            {
                relativePath = relativePath.Substring(relativePath.IndexOf('/'));
                relativePath = "." + relativePath;
            }

            return relativePath.Replace('/', Path.DirectorySeparatorChar);
        }
    }
}
