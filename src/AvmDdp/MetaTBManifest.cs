using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics.Contracts;
using GME.MGA;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;
using System.Diagnostics;
using System.Text.RegularExpressions;

namespace AVM.DDP
{
    public class MetaTBManifest
    {
        public const string TESTBENCH_FILENAME = "testbench_manifest.json";

        public class Artifact
        {
            public string Location { get; set; }
            public string Tag { get; set; }
        }

        public abstract class ValueWithUnit
        {
            public string Description { get; set; }
            public string Name { get; set; }
            public string Unit { get; set; }
            public string Value { get; set; }
            public string ID { get; set; } // GUID for now -- may need to reverse with GMEID
            public string DisplayedName { get; set; }
            public string GMEID { get; set; } // Object ID
        }

        public class Metric : ValueWithUnit
        {
            public List<Artifact> VisualizationArtifacts { get; set; }

            public Metric()
            {
                VisualizationArtifacts = new List<Artifact>();
            }
        }

        public class Parameter : ValueWithUnit
        {
            public string Range { get; set; }
        }

        public enum StatusEnum
        {
            UNEXECUTED,
            OK,
            FAILED
        }

        public class Step
        {
            public string Description { get; set; }
            public string ExecutionCompletionTimestamp { get; set; }
            public string ExecutionStartTimestamp { get; set; }
            public string Invocation { get; set; }
            public List<Parameter> Parameters { get; set; }
            public string PostProcess { get; set; }
            public string PreProcess { get; set; }
            public string Type { get; set; }
            [JsonConverter(typeof(StringEnumConverter))]
            public StatusEnum Status { get; set; }

            public Step()
            {
                Parameters = new List<Parameter>();
                Status = StatusEnum.UNEXECUTED;
            }
        }

        public class Dependency
        {
            public string Type { get; set; }
            public string Directory { get; set; }
        }

        public class DesignType
        {
            public string Name { get; set; }
            public string Type { get; set; }
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public List<DesignType> Children { get; set; }
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public bool? Selected { get; set; }
        }

        [JsonConverter(typeof(StringEnumConverter))]
        public StatusEnum Status { get; set; }

        public List<Dependency> Dependencies { get; set; }
        public List<Artifact> Artifacts { get; set; }
        public List<Artifact> VisualizationArtifacts { get; set; }
        public string Created { get; set; }
        public string DesignID { get; set; }
        public string DesignName { get; set; }
        public string CfgID { get; set; }
        public List<Metric> Metrics { get; set; }
        public List<Parameter> Parameters { get; set; }
        public List<Step> Steps { get; set; }
        public string TestBench { get; set; }
        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public int? TierLevel { get; set; }
        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public bool? CopyTestResults { get; set; }
        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public DesignType Design { get; set; }

        public MetaTBManifest()
        {         
            Artifacts = new List<Artifact>();
            VisualizationArtifacts = new List<Artifact>();
            Metrics = new List<Metric>();
            Parameters = new List<Parameter>();
            Steps = new List<Step>();
            Dependencies = new List<Dependency>();
        }

        protected void CopyManifest(MetaTBManifest other)
        {
            this.Created = other.Created;
            this.DesignID = other.DesignID;
            this.DesignName = other.DesignName;
            this.CfgID = other.CfgID;
            this.TestBench = other.TestBench;
            this.TierLevel = other.TierLevel;
            this.CopyTestResults = other.CopyTestResults;

            foreach (var item in other.Artifacts)
            {
                this.Artifacts.Add(item);
            }

            foreach (var item in other.Metrics)
            {
                this.Metrics.Add(item);
            }

            foreach (var item in other.Parameters)
            {
                this.Parameters.Add(item);
            }

            foreach (var item in other.Steps)
            {
                this.Steps.Add(item);
            }

            foreach (var item in other.Dependencies)
            {
                this.Dependencies.Add(item);
            }
        }

        public void MakeManifest(
            ISIS.GME.Common.Interfaces.FCO testBench,
            string outputDir = "",
            bool update = true)
        {
            Contract.Requires(testBench != null);

            string fullPathFileName = Path.Combine(outputDir, TESTBENCH_FILENAME);
            bool shouldUpdate = false;

            if (update && System.IO.File.Exists(fullPathFileName))
            {
                MetaTBManifest oldManifest = Deserialize(fullPathFileName);
                this.CopyManifest(oldManifest);
                shouldUpdate = true;
            }

            this.TestBench = string.IsNullOrEmpty((testBench.Impl as MgaFCO).RegistryValue["TestBenchUniqueName"]) ?
                testBench.Name :
                (testBench.Impl as MgaFCO).RegistryValue["TestBenchUniqueName"];

            this.Created = DateTime.UtcNow.ToString("o");
            // FIXME: put design name here...
            this.DesignName = testBench.Name;

            if (testBench is CyPhy.BallisticTestBench)
            {
                this.TierLevel = (int)(testBench as CyPhy.BallisticTestBench).Attributes.Tier;
            }

            if (testBench is CyPhy.TestBenchType)
            {
                var testBenchType = ISIS.GME.Dsml.CyPhyML.Classes.TestBenchType.Cast(testBench.Impl);

                foreach (var item in testBenchType.Children.MetricCollection)
                {
                    if (shouldUpdate)
                    {
                        var oldData = this.Metrics.FirstOrDefault(x => x.ID == item.Guid.ToString());
                        if (oldData != null)
                        {
                            this.Metrics.Remove(oldData);
                        }
                    }

                    AVM.DDP.MetaTBManifest.Metric metric = new AVM.DDP.MetaTBManifest.Metric();
                    metric.Name = item.Name.Trim();
                    metric.Unit = (item.Impl as MgaReference).Referred == null ?
                        string.Empty :
                        (item.Impl as MgaReference).Referred.Name;

                    // TODO: add the displayed name
                    metric.Value = item.Attributes.Value;
                    metric.ID = item.Guid.ToString();
                    metric.GMEID = item.ID;
                    metric.Description = item.Attributes.Description;
                    this.Metrics.Add(metric);
                }

                foreach (var item in testBenchType.Children.ParameterCollection)
                {
                    if (shouldUpdate)
                    {
                        var oldData = this.Parameters.FirstOrDefault(x => x.ID == item.Guid.ToString());
                        if (oldData != null)
                        {
                            this.Parameters.Remove(oldData);
                        }
                    }

                    AVM.DDP.MetaTBManifest.Parameter parameter = new AVM.DDP.MetaTBManifest.Parameter();
                    parameter.Name = item.Name;
                    parameter.Unit = (item.Impl as MgaReference).Referred == null ?
                        string.Empty :
                        (item.Impl as MgaReference).Referred.Name;

                    parameter.Description = item.Attributes.Description;
                    parameter.GMEID = item.ID;
                    parameter.ID = item.Guid.ToString();
                    parameter.Range = item.Attributes.Range;
                    parameter.Value = item.Attributes.Value;

                    this.Parameters.Add(parameter);
                }

                // get designID
                string designID = null;
                var tlsut = testBenchType.Children.TopLevelSystemUnderTestCollection.FirstOrDefault();
                var catlsut = testBenchType.Children.ComponentAssemblyCollection.FirstOrDefault();
                if (tlsut != null)
                {
                    // if it is a reference
                    if (tlsut.Referred.DesignEntity != null)
                    {
                        designID = tlsut.Referred.DesignEntity.Properties.Guid.ToString("B");
                        if (tlsut.Referred.DesignEntity is CyPhy.ComponentAssembly)
                        {
                            catlsut = tlsut.Referred.ComponentAssembly;
                            var cid = catlsut.Attributes.ConfigurationUniqueID;
                            //this.ConfigurationUniqueID = cid;

                            if (string.IsNullOrWhiteSpace(cid))
                            {
                                cid = Guid.NewGuid().ToString("B");
                                catlsut.Attributes.ConfigurationUniqueID = cid;
                            }

                            if (!string.IsNullOrEmpty(cid))
                            {
                                try
                                {
                                    Guid guid = new Guid(cid);
                                    designID = guid.ToString("B");
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
                else if (catlsut != null)
                {
                    // if it is an instance
                    var cid = catlsut.Attributes.ConfigurationUniqueID;

                    if (!string.IsNullOrEmpty(cid))
                    {
                        Guid guid = new Guid(cid);
                        designID = guid.ToString("B");
                    }
                }

                // this.CopyTestResults = testBench.Attributes.CopyResults;

                this.DesignID = designID;
            }
        }

        // Use this method in next iteration (13.17)
        public void DoTasks(CyPhy.Workflow workflow, bool update = true)
        {
            foreach (CyPhy.Task currentTask in workflow.Children.TaskCollection)
            {
                // TODO: use Tarjan's SCC for better efficiency; this will suffice for now
                HashSet<CyPhy.Task> seen = new HashSet<CyPhy.Task>();
                seen.Add(currentTask);

                Queue<CyPhy.Flow> flows = new Queue<CyPhy.Flow>();
                foreach (var flow in currentTask.DstConnections.FlowCollection)
                {
                    flows.Enqueue(flow);
                }

                while (flows.Any())
                {
                    var nextTask = flows.Dequeue().DstEnds.Task;
                    if (nextTask == currentTask)
                    {
                        // TODO: Flag cycle error with GMEConsole
                        return;
                    }

                    if (!seen.Contains(nextTask))
                    {
                        seen.Add(nextTask);
                        foreach (var nextFlow in nextTask.DstConnections.FlowCollection)
                        {
                            flows.Enqueue(nextFlow);
                        }
                    }
                }
            }

            HashSet<CyPhy.Task> processed = new HashSet<CyPhy.Task>();
            Queue<CyPhy.Task> waiting = new Queue<CyPhy.Task>();

            foreach (var item in workflow.Children.TaskCollection.Where(x => !x.SrcConnections.FlowCollection.Any()))
            {
                waiting.Enqueue(item);
            }

            // In the case of joins
            while (waiting.Any())
            {
                var currTask = waiting.Dequeue();
                bool allPreviousDone = true;

                foreach (var flow in currTask.SrcConnections.FlowCollection)
                {
                    if (!processed.Contains(flow.SrcEnds.Task))
                    {
                        waiting.Enqueue(currTask);
                        allPreviousDone = false;
                        break;
                    }
                }

                if (!allPreviousDone)
                {
                    continue;
                }

                processed.Add(currTask);

                var json = currTask.Attributes.Parameters;
                Dictionary<string, string> values =
                    JsonConvert.DeserializeObject<Dictionary<string, string>>(json);

                if (values != null)
                {
                    foreach (var item in values)
                    {
                        if (item.Key == "AnalysisTool")
                        {
                            //var toolInfo = META.AnalysisTool.GetByName(item.Value);
                            //if (toolInfo == null)
                            //{
                            //    // TODO:
                            //    // 1. Grab progID of interpreter from workflow
                            //    // 2. Set Invocation to progID.bat
                            //}

                            //// TODO: populate description with generic info from model about workflow, task item, progid

                            //var step = new Step();
                            //step.Invocation = toolInfo.RunCommand;
                            //this.Steps.Add(step);
                        }
                    }
                }

                foreach (var item in currTask.DstConnections.FlowCollection)
                {
                    if (!waiting.Contains(item.DstEnds.Task))
                    {
                        waiting.Enqueue(item.DstEnds.Task);
                    }
                }
            }
        }

        public void Serialize(string outputDir = "")
        {
            string manifestJson = Newtonsoft.Json.JsonConvert.SerializeObject(
                this,
                Newtonsoft.Json.Formatting.Indented);

            string fullPathFileName = Path.Combine(outputDir, TESTBENCH_FILENAME);

            using (StreamWriter writer = new StreamWriter(fullPathFileName))
            {
                writer.WriteLine(manifestJson);
            }
        }

        public static MetaTBManifest Deserialize(string fileName)
        {
            MetaTBManifest manifest = JsonConvert.DeserializeObject<MetaTBManifest>(System.IO.File.ReadAllText(fileName));
            manifest.Artifacts = manifest.Artifacts ?? new List<Artifact>();
            manifest.Dependencies = manifest.Dependencies ?? new List<Dependency>();
            manifest.Metrics = manifest.Metrics ?? new List<Metric>();
            manifest.Parameters = manifest.Parameters ?? new List<Parameter>();
            manifest.Steps = manifest.Steps ?? new List<Step>();
            manifest.VisualizationArtifacts = manifest.VisualizationArtifacts ?? new List<Artifact>();
            return manifest;
        }

        public static MetaTBManifest OpenForUpdate(string outputDir = "")
        {
            string fullPathFileName = Path.Combine(outputDir, TESTBENCH_FILENAME);

            if (System.IO.File.Exists(fullPathFileName))
            {
                MetaTBManifest oldManifest = Deserialize(fullPathFileName);
                return oldManifest;
            }

            return new MetaTBManifest();
        }

        public void AddArtifact(
            string location,
            string tag,
            bool update = true)
        {
            Contract.Requires(location != null);

            // TODO: determine how to handle empty tags
            if (update && !string.IsNullOrWhiteSpace(tag))
            {
                var oldArtifact = this.Artifacts.FirstOrDefault(x => x.Tag == tag);
                if (oldArtifact != null)
                {
                    this.Artifacts.Remove(oldArtifact);
                }
            }

            Artifact artifact = new Artifact();
            artifact.Location = location;
            artifact.Tag = tag;
            this.Artifacts.Add(artifact);
        }

        public void AddStep(
            Step step,
            bool update = true)
        {
            Contract.Requires(step != null);

            if (update)
            {
                var oldStep = this.Steps.FirstOrDefault(x => x.Description == step.Description);
                if (oldStep != null)
                {
                    this.Steps.Remove(oldStep);
                }
            }

            this.Steps.Add(step);
        }

        public void AddAllTasks(
            CyPhy.TestBenchType testBenchType,
            IEnumerable<global::META.ComComponent> interpreters,
            string relativePathToProjectDir)
        {
            Contract.Requires(testBenchType != null);
            Contract.Requires(interpreters != null);

            var workflowRef = testBenchType.Children
                                           .WorkflowRefCollection
                                           .ToList();

            if (workflowRef.Count == 1 && workflowRef[0].AllReferred != null)
            {
                var workflow = workflowRef[0].Referred.Workflow;
                var allTasks = workflow.Children.TaskBaseCollection;

                var startTask = allTasks
                                .Where(x => x.AllSrcConnections.Count() == 0)
                                .FirstOrDefault();
                CyPhy.TaskBase nextTask = startTask;

                int regularTaskIndex = 0;
                int regularTaskCount = interpreters.Count();                
                var processed = new List<CyPhy.TaskBase>();

                while (nextTask != null &&
                        processed.Contains(nextTask) == false)
                {
                    processed.Add(nextTask);

                    if (nextTask.Impl.MetaBase.Name == (typeof(CyPhy.Task).Name))
                    {
                        if (regularTaskIndex < regularTaskCount)
                        {
                            var currTask = interpreters.ElementAt(regularTaskIndex++);
                            if (currTask.result != null &&
                                currTask.result.RunCommand != null)
                            {
                                var step = new Step();
                                step.Invocation = currTask.result.RunCommand;
                                this.Steps.Add(step);
                            }
                        }
                    }
                    else if (nextTask.Impl.MetaBase.Name == (typeof(CyPhy.ExecutionTask).Name))
                    {
                        CyPhy.ExecutionTask executionTask = ISIS.GME.Dsml.CyPhyML.Classes.ExecutionTask.Cast(nextTask.Impl);

                        var step = new Step();

                        step.Description = executionTask.Attributes.Description;
                        // %project_dir% is relative path to MgaExtensions.MgaExtensions.GetProjectDirectoryPath(testBenchType.Impl.Project)
                        step.Invocation = Regex.Replace(executionTask.Attributes.Invocation, "%project_dir%", relativePathToProjectDir, RegexOptions.CultureInvariant | RegexOptions.IgnoreCase);
                        step.PreProcess = executionTask.Attributes.PreProcess;
                        step.PostProcess = executionTask.Attributes.PostProcess;

                        var splitStrings = new string[]{Environment.NewLine, "\n"};
                        foreach (var parameterValue in executionTask.Attributes.Parameters.Split(splitStrings, StringSplitOptions.RemoveEmptyEntries))
                        {
                            Parameter parameter = new Parameter();
                            parameter.Name = parameterValue;
                            step.Parameters.Add(parameter);
                        }

                        this.Steps.Add(step);
                    }

                    var flow = nextTask.DstConnections.FlowCollection.FirstOrDefault();
                    if (flow == null)
                    {
                        nextTask = null;
                    }
                    else
                    {
                        nextTask = flow.DstEnds.TaskBase;
                    }
                }
            }
        }

        public void AddDependency(Dependency dep)
        {
            Dependencies.Add(dep);
        }
    
    }
}
