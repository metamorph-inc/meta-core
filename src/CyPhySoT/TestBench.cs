// -----------------------------------------------------------------------
// <copyright file="TestBench.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace CyPhySoT
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using GME.MGA;
    using System.IO;
    using META;
    using System.Xml.Serialization;
    using CyPhyGUIs;
    using System.Runtime.Serialization;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class TestBench
    {
        public string ProgId { get; set; }
        public Dictionary<string, string> WorkflowParametersDict { get; set; }
        public string OutputDirectory { get; set; }
        public string OriginalProjectFileName { get; set; }

        public string Name { get; set; }

        public string RunCommand { get; set; }
        public string ArtifactPattern { get; set; }
        public string Labels { get; set; }
        public string BuildQuery { get; set; }
        public string ResultsZip { get; set; }

        public List<TestBench> UpstreamTestBenches { get; set; }
        public List<TestBench> DownstreamTestBenches { get; set; }

        List<AVM.DDP.MetaTBManifest.Dependency> Dependencies { get; set; }

        public MgaProject Project { get; set; }
        private string m_CurrentObjId { get; set; }
        private MgaFCO m_CurrentObj;
        public MgaFCO CurrentObj
        {
            get
            {
                return m_CurrentObj;
            }
            set
            {
                m_CurrentObjId = value.ID;
                m_CurrentObj = value;
            }
        }
        public MgaFCOs SelectedObjs { get; set; }
        public int ParamInvoke { get; set; }

        public ComComponent Interpreter { get; set; }


        public TestBench()
        {
            this.WorkflowParametersDict = new Dictionary<string, string>();
            this.SelectedObjs = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
            this.UpstreamTestBenches = new List<TestBench>();
            this.DownstreamTestBenches = new List<TestBench>();
            this.Dependencies = new List<AVM.DDP.MetaTBManifest.Dependency>();
        }

        public new TestBench MemberwiseClone()
        {
            return (TestBench)base.MemberwiseClone();
        }

        public void Run()
        {

            AVM.DDP.MetaAvmProject avmProj;
            var terr = this.Project.BeginTransactionInNewTerr();
            try
            {
            this.CurrentObj = this.Project.GetFCOByID(this.m_CurrentObjId);

            // This can only be a testbench at this point
            ISIS.GME.Dsml.CyPhyML.Interfaces.TestBenchType tb = ISIS.GME.Dsml.CyPhyML.Classes.TestBenchType.Cast(this.CurrentObj as MgaObject);
            this.CallFormulaEvaluator(this.CurrentObj); // FIXME: KMS: For multijobrun, is this necessary?
            avmProj = AVM.DDP.MetaAvmProject.Create(Path.GetDirectoryName(OriginalProjectFileName), Project);
            avmProj.SaveSummaryReportJson(this.OutputDirectory, this.CurrentObj);
            avmProj.SaveTestBenchManifest(this.OutputDirectory, tb, Dependencies);
            avmProj.UpdateResultsJson(this.CurrentObj, this.OutputDirectory, DateTime.Now);
            // TODO: test bench export??
            }
            finally
            {
                this.Project.AbortTransaction();
            }
            var currentProjectDir = Path.GetDirectoryName(this.Project.ProjectConnStr.Substring("MGA=".Length));
            ComComponent interpreter = new ComComponent(ProgId);


            // Read the copied over configuration (the original one may be changed and/or opened).
            if (interpreter.InterpreterConfig != null)
            {
                interpreter.InterpreterConfig = META.ComComponent.DeserializeConfiguration(currentProjectDir, interpreter.InterpreterConfig.GetType(), interpreter.ProgId);
            }
            
            this.Interpreter = interpreter;

            interpreter.Initialize(Project);

            interpreter.WorkflowParameters = WorkflowParametersDict;
            interpreter.SetWorkflowParameterValues();
            interpreter.MainParameters.OutputDirectory = this.OutputDirectory;
            interpreter.MainParameters.ProjectDirectory = Path.GetDirectoryName(this.OriginalProjectFileName);
            interpreter.MainParameters.ConsoleMessages = false;
            interpreter.MainParameters.Project = Project;
            interpreter.MainParameters.CurrentFCO = CurrentObj;
            interpreter.MainParameters.SelectedFCOs = SelectedObjs;
            interpreter.MainParameters.StartModeParam = ParamInvoke;

            interpreter.Main();

            this.RunCommand = interpreter.result.RunCommand;
            this.Labels = interpreter.result.Labels;
            this.BuildQuery = interpreter.result.BuildQuery;
            this.ResultsZip = interpreter.result.ZippyServerSideHook;

            this.Project.BeginTransaction(terr);
            try
            {
                var manifest = AVM.DDP.MetaTBManifest.OpenForUpdate(this.OutputDirectory);
                manifest.AddAllTasks(ISIS.GME.Dsml.CyPhyML.Classes.TestBenchType.Cast(this.CurrentObj), new ComComponent[] { interpreter });
                manifest.Serialize(this.OutputDirectory);
                // if some magic happens in the test bench and some interpreters would update the model
                // and they are NOT updating the summary file accordingly we will do it here.
                // RISK: if any interpreter wants to update the summary file like CyPython this could mess up the values.
                this.CallFormulaEvaluator(this.CurrentObj);
                avmProj.SaveSummaryReportJson(this.OutputDirectory, this.CurrentObj);
            }
            finally
            {
                this.Project.AbortTransaction();
            }
        }

        public List<AVM.DDP.MetaTBManifest.Dependency> CollectDeps()
        {
            foreach (TestBench tb in this.UpstreamTestBenches)
            {
                Dependencies.Add(new AVM.DDP.MetaTBManifest.Dependency() { Directory = tb.OutputDirectory, Type = tb.CurrentObj.MetaBase.Name });
            }
            return Dependencies;
        }

        private bool CallFormulaEvaluator(MgaFCO currentObj)
        {
            var result = false;
            Type typeFormulaEval = Type.GetTypeFromProgID("MGA.Interpreter.CyPhyFormulaEvaluator");
            var formulaEval = Activator.CreateInstance(typeFormulaEval) as GME.MGA.IMgaComponentEx;

            formulaEval.Initialize(this.Project);
            try
            {
                formulaEval.InvokeEx(this.Project, currentObj, this.SelectedObjs, 128);
            }
            catch (System.Runtime.InteropServices.COMException)
            {
                // FIXME: handle this exception properly
            }

            return result;
        }
    }
}
