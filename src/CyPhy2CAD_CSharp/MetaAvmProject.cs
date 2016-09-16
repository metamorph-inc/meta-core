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
    using System.Diagnostics.Contracts;

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

        internal string m_filename { get; set; }
        private TextWriter infoTextWriter { get; set; }
        private TextWriter errorTextWriter { get; set; }

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

            CyPhy.TestBench tb;
            if (fco.Meta.Name == typeof(CyPhy.TestBench).Name)
            {
                tb = ISIS.GME.Dsml.CyPhyML.Classes.TestBench.Cast(fco);
            }
            else if (fco.Meta.Name == typeof(CyPhy.ParametricExploration).Name)
            {
                tb = ISIS.GME.Dsml.CyPhyML.Classes.TestBench.Cast(fco.ChildObjects.
                    Cast<MgaFCO>().OfType<MgaReference>().
                    Where(x => x.MetaBase.Name == typeof(CyPhy.TestBenchRef).Name).First().Referred);
            }
            else if (fco.Meta.Name == typeof(CyPhy.TestBenchSuite).Name)
            {
                tb = ISIS.GME.Dsml.CyPhyML.Classes.TestBench.Cast(fco.ChildObjects.
                    Cast<MgaFCO>().OfType<MgaReference>().
                    Where(x => x.MetaBase.Name == typeof(CyPhy.TestBenchRef).Name).First().Referred);
            }
            else
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
                string d = design.SerializeToFile(Path.GetFullPath(Path.Combine(designFolder, design.Name + ".metadesign.json")));

                string designFileName = Path.Combine(".", designFolder, design.Name + ".metadesign.json").Replace('\\', '/');
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
            string OutputSubDir)
        {
            string jsonFile = Path.Combine(Path.GetDirectoryName(this.m_filename), "results", "results.metaresults.json");
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
                        results = JsonConvert.DeserializeObject<AVM.DDP.MetaResults>(content);
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

                    thisResult.Time = DateTime.Now.ToString("yyyy-MM-dd HH-mm-ss");

                    thisResult.TestBench = singleFco.RegistryValue["TestBenchUniqueName"] + ".testbench.json";

                    if (singleFco.Meta.Name == typeof(CyPhy.TestBench).Name ||
                        singleFco.Meta.Name == typeof(CyPhy.ParametricExploration).Name ||
                        singleFco.Meta.Name == typeof(CyPhy.TestBenchSuite).Name)
                    {
                        CyPhy.TestBench testBench = null;
                        if (singleFco.Meta.Name == typeof(CyPhy.TestBench).Name)
                        {
                            testBench = ISIS.GME.Dsml.CyPhyML.Classes.TestBench.Cast(singleFco);
                        }
                        else if (singleFco.Meta.Name == typeof(CyPhy.ParametricExploration).Name)
                        {
                            var pet = ISIS.GME.Dsml.CyPhyML.Classes.ParametricExploration.Cast(singleFco);
                            testBench = pet.Children.TestBenchRefCollection.FirstOrDefault().Referred.TestBench;
                        }
                        else if (singleFco.Meta.Name == typeof(CyPhy.TestBenchSuite).Name)
                        {
                            var pet = ISIS.GME.Dsml.CyPhyML.Classes.TestBenchSuite.Cast(singleFco);
                            testBench = pet.Children.TestBenchRefCollection.FirstOrDefault().Referred.TestBench;
                        }

                        var tlsut = testBench.Children.TopLevelSystemUnderTestCollection.FirstOrDefault();
                        if (tlsut != null)
                        {
                            if (tlsut.Referred.ComponentAssembly != null)
                            {
                                var cfg = tlsut.Referred.ComponentAssembly;
                                thisResult.Design = cfg.Name + ".metadesign.json";

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

        public void UpdateTestBenchJson(MgaFCO testBench)
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

            if (fco.Meta.Name == "TestBench")
            {
                CyPhy.TestBench tb = ISIS.GME.Dsml.CyPhyML.Classes.TestBench.Cast(fco as MgaObject);

                AVM.DDP.MetaTBManifest manifest = new AVM.DDP.MetaTBManifest();
                manifest.MakeManifest(tb, interpreterOutputDir);
                manifest.Serialize(interpreterOutputDir);
            }
            else if (fco.Meta.Name == "CADTestBench")
            {
                var tb = ISIS.GME.Dsml.CyPhyML.Classes.CADTestBench.Cast(fco as MgaObject);

                AVM.DDP.MetaTBManifest manifest = new AVM.DDP.MetaTBManifest();
                manifest.MakeManifest(tb, interpreterOutputDir);
                manifest.Serialize(interpreterOutputDir);
            }
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

            Directory.SetCurrentDirectory(outputDirectory);

            string avmProjFileName = Path.GetFullPath(Path.Combine(outputDirectory, "manifest.project.json"));

            if (File.Exists(avmProjFileName))
            {
                string sjson = "{}";
                using (StreamReader reader = new StreamReader(avmProjFileName))
                {
                    sjson = reader.ReadToEnd();
                    avmProj = JsonConvert.DeserializeObject<AVM.DDP.MetaAvmProject>(sjson);
                }
            }

            if (console != null)
            {
                avmProj.infoTextWriter = console.Info;
                avmProj.errorTextWriter = console.Error;
            }

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
                    avmProj.infoTextWriter.WriteLine(msg);
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
                var dsProjectJsonLink = Path.Combine(".", designSpaceFolder, rootDS.Name + ".metadesign.json").Replace('\\', '/');
                var dsFileName = Path.Combine(dirName, rootDS.Name + ".metadesign.json");

                if (avmProj.Project.DesignSpaceModels.Contains(dsProjectJsonLink) == false)
                {
                    avmProj.Project.DesignSpaceModels.Add(dsProjectJsonLink);
                }
                var designContainer = ISIS.GME.Dsml.CyPhyML.Classes.DesignContainer.Cast(rootDS);
                var design = CyPhy2DesignInterchange.CyPhy2DesignInterchange.Convert(designContainer);
                string d = design.SerializeToFile(Path.GetFullPath(dsFileName));
            }

            Directory.SetCurrentDirectory(currentDir);
            return avmProj;
        }

        /// <summary>
        /// Creates a relative path from one file or folder to another.
        /// </summary>
        /// <param name="fromPath">Contains the directory that defines the start of the relative path.</param>
        /// <param name="toPath">Contains the path that defines the endpoint of the relative path.</param>
        /// <param name="dontEscape">Boolean indicating whether to add uri safe escapes to the relative path</param>
        /// <returns>The relative path from the start directory to the end path.</returns>
        public static string MakeRelativePath(string fromPath, string toPath)
        {
            Contract.Requires(string.IsNullOrEmpty(fromPath) == false);
            Contract.Requires(string.IsNullOrEmpty(toPath) == false);

            Uri fromUri = new Uri(fromPath);
            Uri toUri = new Uri(toPath);

            Uri relativeUri = fromUri.MakeRelativeUri(toUri);
            string relativePath = Uri.UnescapeDataString(relativeUri.ToString());
            relativePath = relativePath.Substring(relativePath.IndexOf('/'));
            relativePath = "." + relativePath;

            return relativePath.Replace('/', Path.DirectorySeparatorChar);
        }
    }
}
