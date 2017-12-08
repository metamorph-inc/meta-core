using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using System.IO;
using System.Diagnostics;
using System.Threading;
using System.Runtime.InteropServices;
using CyPhyMasterInterpreter;
using JobManager;

namespace JobManagerFramework
{
    public class SoTImpl : SoT
    {

        public SoTImpl(JobServer server)
        {
            this.Server = server;
        }

        string currentObjectName { get; set; }
        string sotName { get; set; }
        CyPhySoT.SotConfig sotConfig { get; set; }
        Action<Job> JobAction { get; set; }

        MgaProject Project { get; set; }
        MgaFCO CurrentObj { get; set; }

        public void Run()
        {
            if (string.IsNullOrEmpty(this.WorkingDirectory) ||
                Directory.Exists(this.WorkingDirectory) == false)
            {
                // nothing needs to be done.
                return;
            }

            // TODO: exception handling... 

            // get manifest file
            string manifestFile = Path.Combine(this.WorkingDirectory, "manifest.sot.json");
            if (File.Exists(manifestFile) == false)
            {
                Trace.TraceError("{0} file does not exist", manifestFile);
                return;
            }

            // this could fail... exception handling!
            sotConfig = Newtonsoft.Json.JsonConvert.DeserializeObject<CyPhySoT.SotConfig>(File.ReadAllText(manifestFile));

            // get mga filename
            string mgaFileName = Path.Combine(this.WorkingDirectory, sotConfig.ProjectFileName);

            if (File.Exists(mgaFileName) == false)
            {
                Trace.TraceError("{0} file does not exist", mgaFileName);
                return;
            }

            this.ProjectConnStr = "MGA=" + mgaFileName;

            Semaphore sem = null;

            if (criticalSection.TryGetValue(this.ProjectConnStr, out sem) == false)
            {
                criticalSection[this.ProjectConnStr] = new Semaphore(1, 1);
            }

            // Load GME model
            this.OpenProject();

            if (sotConfig.MultiJobRun)
            {
                Project.BeginTransactionInNewTerr(transactiontype_enum.TRANSACTION_READ_ONLY);
                try
                {
                    this.TestBenches = GetMultiJobRunTestBenches(CurrentObj, Path.GetDirectoryName(sotConfig.ProjectFileName));
                }
                finally
                {
                    Project.AbortTransaction();
                }
            }
            else
            {
                CyPhySoT.CyPhySoTInterpreter sotInterpreter = new CyPhySoT.CyPhySoTInterpreter();
                this.TestBenches = sotInterpreter.GetTestbenchesFromModel(this.Project, (MgaModel)this.CurrentObj, Path.GetDirectoryName(this.WorkingDirectory));
                Project.BeginTransactionInNewTerr(transactiontype_enum.TRANSACTION_NON_NESTED);
                try
                {
                    // replace tb in TestBenches that corresponds to the MultiJobRun with the tbs from GetMultiJobRunTestBenches
                    foreach (var tb in TestBenches.ToList<CyPhySoT.TestBench>())
                    {
                        var modelProcessor = AnalysisModelProcessor.GetAnalysisModelProcessor((IMgaModel)tb.CurrentObj);
                        if (modelProcessor is MultiJobRunProcessor)
                        {
                            List<CyPhySoT.TestBench> multiTbs = GetMultiJobRunTestBenches(tb.CurrentObj, tb.OutputDirectory);
                            foreach (var upstreamTb in tb.UpstreamTestBenches)
                            {
                                upstreamTb.DownstreamTestBenches.Remove(tb);
                                upstreamTb.DownstreamTestBenches.Add(multiTbs[0]);
                                multiTbs[0].UpstreamTestBenches.Add(upstreamTb);
                            }
                            foreach (var downstreamTb in tb.DownstreamTestBenches)
                            {
                                downstreamTb.UpstreamTestBenches.Remove(tb);
                                downstreamTb.UpstreamTestBenches.Add(multiTbs[multiTbs.Count - 1]);
                                multiTbs[multiTbs.Count - 1].DownstreamTestBenches.Add(downstreamTb);
                            }
                            this.TestBenches.AddRange(multiTbs);
                            this.TestBenches.Remove(tb);
                        }
                    }
                }
                finally
                {
                    Project.AbortTransaction();
                }
            }

            // TODO: update value flow!

            // Test bench/job map
            foreach (var testbench in this.TestBenches)
            {
                JobImpl job = new JobImpl(this.Server);

                this.TestBenchJobMap.Add(testbench, job);
            }

            try
            {
                // propagate the values between test benches
                PropagateValueFlow();
            }
            catch (JobFailure ex)
            {
                // FIXME: add some GUI notification
                string failed_txt = Path.Combine(this.WorkingDirectory, "_FAILED.txt");
                File.WriteAllText(failed_txt, String.Format("SoT {0} failed: {1}", this.sotName, ex.ToString()));
                this.CloseProject();
                return;
            }

            // get all testbenches from sot that can be run without dependency
            foreach (var testbench in this.TestBenches.Where(x => x.UpstreamTestBenches.Count == 0))
            {
                // call interpreters for this test bench and post it to the job manager
                RunTestBenchAndStart(testbench);
            }

            this.CloseProject();
        }

        private List<CyPhySoT.TestBench> GetMultiJobRunTestBenches(MgaFCO CurrentObj, string OutputDirectory)
        {
            var TestBenches = new List<CyPhySoT.TestBench>();
            var testbench1 = new CyPhySoT.TestBench();
            var workflowRef = CurrentObj
                .ChildObjects
                .OfType<MgaReference>()
                .FirstOrDefault(x => x.Meta.Name == "WorkflowRef");
            string parameters = "{}";
            MgaAtom task;
            if (workflowRef != null)
            {
                task = workflowRef
                    .Referred
                    .ChildObjects
                    .OfType<MgaAtom>()
                    .FirstOrDefault(x => x.Meta.Name == "Task");
                parameters = task.StrAttrByName["Parameters"];
                testbench1.ProgId = task.StrAttrByName["COMName"];
            }
            else
            {
                testbench1.ProgId = "MGA.Interpreter.CyPhy2CAD_CSharp";
            }

            testbench1.WorkflowParametersDict = new Dictionary<string, string>();
            if (string.IsNullOrWhiteSpace(parameters) == false)
            {
                try
                {
                    testbench1.WorkflowParametersDict = Newtonsoft.Json.JsonConvert.DeserializeObject<Dictionary<string, string>>(parameters);
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex);
                    Trace.TraceWarning(ex.ToString());
                }
            }

            testbench1.OutputDirectory = OutputDirectory;
            if (Directory.Exists(testbench1.OutputDirectory) == false)
            {
                Directory.CreateDirectory(testbench1.OutputDirectory);
            }
            testbench1.Project = CurrentObj.Project;
            testbench1.CurrentObj = CurrentObj;
            testbench1.ParamInvoke = 0;

            testbench1.Name = CurrentObj.Name;

            TestBenches.Add(testbench1);

            var testbench2 = testbench1.MemberwiseClone();
            testbench2.UpstreamTestBenches = new List<CyPhySoT.TestBench>();
            testbench2.DownstreamTestBenches = new List<CyPhySoT.TestBench>();
            testbench2.UpstreamTestBenches.Add(testbench1);
            testbench1.DownstreamTestBenches.Add(testbench2);
            TestBenches.Add(testbench2);
            testbench2.ProgId = "MGA.Interpreter.CyPhyPython";

            var metaToPyScript = new Dictionary<string, string>()
                    {
                        {"CFDTestBench", "runOpenFoam.py"},
                        {"BlastTestBench", "runLSDyna.py"},
                        {"BallisticTestBench", "runCTH.py"},
                    };

            testbench2.WorkflowParametersDict = new Dictionary<string, string>()
                    {
                        {"script_file", metaToPyScript[CurrentObj.Meta.Name]}
                    };
            return TestBenches;
        }

        private void MonitorAndPostDownstreamTestBenches(Job job)
        {
            var kvp = this.TestBenchJobMap.FirstOrDefault(x => x.Value == job);
            {
                foreach (var tbdown in kvp.Key.DownstreamTestBenches)
                {
                    if (tbdown.UpstreamTestBenches.All(x => this.TestBenchJobMap[x].Status == Job.StatusEnum.Succeeded))
                    {
                        var downJob = this.TestBenchJobMap[tbdown];
                        var jobInServer = this.Server.Jobs.FirstOrDefault(x => x.Id == downJob.Id);
                        if (jobInServer != null)
                        {
                            // already posted.
                            continue;
                        }
                        if (downJob.Status != Job.StatusEnum.Succeeded ||
                            downJob.IsFailed())
                        {
                            this.OpenProject();

                            try
                            {
                                // get all upstream test bench results back to the model.
                                GetResultsBackToModel(tbdown.UpstreamTestBenches);
                                // propagate the values between test benches
                                PropagateValueFlow();
                                // call interpreters for this test bench and post it to the job manager
                                RunTestBenchAndStart(tbdown);
                            }
                            finally
                            {
                                this.CloseProject();
                            }
                        }
                    }
                }
            }
        }

        private void PropagateValueFlow()
        {
            //this.OpenProject();

            try
            {
                Trace.TraceInformation("BEGIN: Value flow propagation using formula evaluator");

                Type tFormulaEval = Type.GetTypeFromProgID("MGA.Interpreter.CyPhyFormulaEvaluator");
                IMgaComponentEx formulaEval = Activator.CreateInstance(tFormulaEval) as IMgaComponentEx;

                Type tMgaFCOs = Type.GetTypeFromProgID("Mga.MgaFCOs");
                MgaFCOs selectedObjs = Activator.CreateInstance(tMgaFCOs) as MgaFCOs;

                formulaEval.Initialize(this.Project);
                formulaEval.InvokeEx(this.Project, this.CurrentObj, selectedObjs, 128);
                Trace.TraceInformation("END: Value flow propagation using formula evaluator ");
            }
            catch (Exception ex)
            {
                Trace.TraceError(ex.ToString());
                throw new JobFailure(ex.Message, ex);
            }

            //this.SaveAndCloseProject();
        }

        /// <summary>
        /// Opens the project and find the CurrentObject
        /// Note: You must call SaveAndCloseProject(), which releases the taken semaphore.
        /// </summary>
        private void OpenProject()
        {
            criticalSection[this.ProjectConnStr].WaitOne();

            // Always creating a new instance of MgaProject
            Type type = Type.GetTypeFromProgID("Mga.MgaProject");
            this.Project = Activator.CreateInstance(type) as MgaProject;

            bool ro_mode;
            Trace.TraceInformation("Opening project {0}", this.ProjectConnStr);

            // Do NOT rely on ro_mode!
            this.Project.Open(this.ProjectConnStr, out ro_mode);

            try
            {
                var terr = this.Project.BeginTransactionInNewTerr();

                // ALWAYS find object 
                this.CurrentObj = this.Project.GetFCOByID(sotConfig.SoTID);
                currentObjectName = this.CurrentObj.Name;
                sotName = this.CurrentObj.RegistryValue["SoTUniqueName"];

                if (string.IsNullOrEmpty(sotName))
                {
                    sotName = currentObjectName;
                }

                this.Project.AbortTransaction();

                // FIXME: do we need this?
                //terr.Destroy();
            }
            catch (COMException ex)
            {
                // FCO was not found
                Trace.TraceError(ex.ToString());
                return;
            }
        }

        private static Dictionary<string, Semaphore> criticalSection = new Dictionary<string, Semaphore>();

        /// <summary>
        /// Saves and closes the project after this call do NOT use Project and CurrentObj
        /// Note: You must call OpenProject(), which takes a semaphore.
        /// </summary>
        private void CloseProject()
        {
            Trace.TraceInformation("Closing project {0}", this.ProjectConnStr);
            this.Project.Close(true);

            criticalSection[this.ProjectConnStr].Release();
        }

        private void GetResultsBackToModel(List<CyPhySoT.TestBench> testBenches)
        {
            var terr = this.Project.BeginTransactionInNewTerr();
            try
            {
                foreach (var testBench in testBenches)
                {
                    Trace.TraceInformation("GetResultsBackToModel {0}", testBench.Name);
                    var filename = Path.Combine(testBench.OutputDirectory, AVM.DDP.MetaTBManifest.TESTBENCH_FILENAME);
                    {
                        var tbreport = Newtonsoft.Json.JsonConvert.DeserializeObject<AVM.DDP.MetaTBManifest>(File.ReadAllText(filename));
                        if (File.Exists(filename) == false)
                        {
                            Trace.TraceError("{0} does not exist.", filename);
                            throw new JobFailure(String.Format("{0} does not exist.", filename));
                        }

                        try
                        {
                            foreach (var metric in tbreport.Metrics)
                            {
                                if (string.IsNullOrWhiteSpace(metric.GMEID) == false)
                                {
                                    var metricMgaObj = this.Project.GetFCOByID(metric.GMEID);
                                    metricMgaObj.StrAttrByName["Value"] = metric.Value.ToString();
                                    Trace.TraceInformation("{0} metric object was updated.", metricMgaObj.AbsPath);
                                }
                            }
                        }
                        catch (Exception ex)
                        {
                            Trace.TraceError(ex.ToString());
                        }
                    }
                }
            }
            catch
            {
                this.Project.AbortTransaction();
                throw;
            }
            this.Project.CommitTransaction();
        }

        public readonly Dictionary<CyPhySoT.TestBench, JobImpl> TestBenchJobMap = new Dictionary<CyPhySoT.TestBench, JobImpl>();
        public System.Collections.Concurrent.BlockingCollection<Action> SoTTodo;
        private List<CyPhySoT.TestBench> TestBenches { get; set; }

        private void RunTestBenchAndStart(CyPhySoT.TestBench testbench)
        {
            //this.OpenProject();

            // Updating with the active objects
            testbench.Project = this.Project;

            testbench.OriginalProjectFileName = sotConfig.OriginalProjectFileName;

            Exception interpreterError = null;
            try
            {
                // run interpreters
                testbench.Run();
            }
            catch (Exception e)
            {
                interpreterError = e;
            }

            //this.SaveAndCloseProject();

            // TODO: catch exceptions from interpreter
            string title = string.Format("{0}__{1}", currentObjectName, testbench.Name);
            Trace.TraceInformation("Job needs to be posted {0}", title);

            // Set up job properties
            JobImpl job = this.TestBenchJobMap[testbench];
            job.RunCommand = testbench.RunCommand;
            job.WorkingDirectory = testbench.OutputDirectory;
            job.Title = sotName;
            job.TestBenchName = testbench.Name;

            // artifacts/labels
            job.Labels = testbench.Labels;

            if (string.IsNullOrWhiteSpace(job.Labels))
            {
                job.Labels = Job.DefaultLabels;
            }

            job.BuildQuery = testbench.BuildQuery;

            if (string.IsNullOrWhiteSpace(job.BuildQuery))
            {
                job.BuildQuery = Job.DefaultBuildQuery;
            }

            if (string.IsNullOrWhiteSpace(testbench.ResultsZip) == false)
            {
                job.ResultsZip = testbench.ResultsZip;
            }

            if (sotConfig.MultiJobRun)
            {
                CyPhySoT.TestBench tb = testbench.UpstreamTestBenches.FirstOrDefault();
                JobImpl upstreamJob;
                if (tb != null && TestBenchJobMap.TryGetValue(tb, out upstreamJob) && upstreamJob.remoteInfo != null)
                {
                    job.GetURLOverride = upstreamJob.remoteInfo.ResultsGetURL;
                }
            }

            job.JobStatusChanged += JobStatusChanged;

            job.Status = Job.StatusEnum.WaitingForStart;
            this.Server.AddJob(job);

            if (interpreterError != null)
            {
                string failed_txt = Path.Combine(testbench.OutputDirectory, "_FAILED.txt");
                if (File.Exists(failed_txt) == false)
                {
                    File.WriteAllText(failed_txt, String.Format("Interpreter {0} failed: {1}", testbench.ProgId, interpreterError.ToString()));
                }
                job.Status = Job.StatusEnum.Failed;
            }
            else
            {
                if (this.JobAction != null)
                {
                    this.JobAction(job);
                }

                job.Start();
            }

        }

        public void JobStatusChanged(Job job, Job.StatusEnum status)
        {
            SoTTodo.Add(delegate { JobStatusChangedSTA(job); });
        }

        public void JobStatusChangedSTA(Job job)
        {
            if (new Job.StatusEnum[] { Job.StatusEnum.WaitingForStart, Job.StatusEnum.Ready, Job.StatusEnum.QueuedLocal}.Contains(job.Status) == false)
            {
                var kvp = this.TestBenchJobMap.FirstOrDefault(x => x.Value == job);
                {
                    foreach (var upstream in kvp.Key.UpstreamTestBenches)
                    {
                        this.TestBenchJobMap[upstream].RerunEnabled = false;
                    }
                }
            }
            if (job.IsFailed())
            {
                // TODO: how to indicate that the jobs are failed?
            }
            else if (job.Status == Job.StatusEnum.Succeeded)
            {
                // FIXME: could this fail??? would be nice to put it into a try catch.
                try
                {
                    MonitorAndPostDownstreamTestBenches(job);
                }
                catch (JobFailure ex)
                {
                    job.Status = Job.StatusEnum.Failed; // FIXME would be better to mark only downstream jobs as failed?
                    File.WriteAllText(Path.Combine(job.WorkingDirectory, "_FAILED.txt"), ex.Message);
                }
            }
        }

        public override object InitializeLifetimeService()
        {
            return null;
        }
    }
}
