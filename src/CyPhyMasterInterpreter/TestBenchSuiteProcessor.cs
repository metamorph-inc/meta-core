using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using META;

namespace CyPhyMasterInterpreter
{
    public class TestBenchSuiteProcessor : AnalysisModelProcessor
    {
        public CyPhy.TestBenchSuite testBenchSuite { get; set; }
        public CyPhy.TestBenchSuite expandedTestBenchSuite { get; set; }

        public TestBenchSuiteProcessor(CyPhy.TestBenchSuite testBenchSuite)
        {
            this.testBenchSuite = testBenchSuite;

            this.OriginalSystemUnderTest = testBenchSuite
                .Children
                .TestBenchRefCollection
                .FirstOrDefault()
                .Referred
                .TestBenchType
                .Children
                .TopLevelSystemUnderTestCollection
                .FirstOrDefault();
        }

        public override MgaModel GetInvokedObject()
        {
            return this.testBenchSuite.Impl as MgaModel;
        }

        public override MgaModel GetExpandedObject()
        {
            return this.expandedTestBenchSuite.Impl as MgaModel;
        }

        public override void Expand(CyPhy.ComponentAssembly configuration)
        {
            this.Configuration = configuration;

            if (this.OriginalSystemUnderTest.Referred.DesignEntity.ID == configuration.ID)
            {
                this.expandedTestBenchSuite = this.testBenchSuite;
            }
            else
            {
                // create temp folder for test bench suite
                CyPhy.TestBenchSuiteFolder testing = CyPhyClasses.TestBenchSuiteFolder.Cast(this.testBenchSuite.ParentContainer.Impl);

                var tempFolderName = AnalysisModelProcessor.GetTemporaryFolderName(this.testBenchSuite.Impl);

                CyPhy.TestBenchSuiteFolder tempFolder = testing.Children.TestBenchSuiteFolderCollection.FirstOrDefault(x => x.Name == tempFolderName);
                if (tempFolder == null)
                {
                    tempFolder = CyPhyClasses.TestBenchSuiteFolder.Create(testing);
                    tempFolder.Name = tempFolderName;

                    this.AddToTraceabilityAndTemporary(tempFolder.Impl, testing.Impl, recursive: false);
                }

                // copy test bench suite
                var tempCopy = (tempFolder.Impl as MgaFolder).CopyFCODisp(this.testBenchSuite.Impl as MgaFCO);
                // fix name
                tempCopy.Name = AnalysisModelProcessor.GetTemporaryObjectName(this.testBenchSuite.Impl, configuration.Impl);

                this.AddToTraceabilityAndTemporary(tempCopy, this.testBenchSuite.Impl);

                // set expanded property to the expanded element
                this.expandedTestBenchSuite = CyPhyClasses.TestBenchSuite.Cast(tempCopy);
            }

            // expand all test benches
            foreach (var testBenchRef in this.expandedTestBenchSuite.Children.TestBenchRefCollection)
            {
                var testBenchTypeExpander = new TestBenchTypeProcessor(testBenchRef.Referred.TestBenchType);
                testBenchTypeExpander.Expand(configuration);

                // switch references
                var switcher = new ReferenceSwitcher.ReferenceSwitcherInterpreter();
                // TODO: handle failures for this
                switcher.SwitchReference(testBenchTypeExpander.expandedTestBenchType.Impl as MgaFCO, testBenchRef.Impl as IMgaReference);

                this.InnerExpanders.Push(testBenchTypeExpander);
            }
        }

        public override Queue<ComComponent> GetWorkflow()
        {
            Queue<ComComponent> workflow = new Queue<ComComponent>();
            this.ExecuteInTransaction(() =>
            {
                workflow.Enqueue(new ComComponent("MGA.Interpreter.CyPhySoT", true));
            });
            return workflow;
        }

        public override bool PostToJobManager(JobManagerDispatch manager)
        {
            if (this.Interpreters == null)
            {
                throw new InvalidOperationException("Call RunInterpreters method first.");
            }

            bool success = true;

            foreach (var interpreter in this.Interpreters)
            {
                // TODO: what if some of them failed ???
                // TODO: can be more than one ???
                if (interpreter.result.Success)
                {
                    string workingDirectory = interpreter.MainParameters.OutputDirectory;

                    success = success && manager.EnqueueSoT(workingDirectory, ProjectDirectory);
                }
            }

            // TODO: should this be inside the Dispatch and triggred by a timer ???
            manager.AddSoTs();


            if (this.Interpreters.Count == 0)
            {
                success = false;
            }

            return success;
        }

        public override bool SaveTestBenchManifest(AVM.DDP.MetaAvmProject project, string configurationName, DateTime analysisStartTime)
        {
            if (project == null)
            {
                throw new ArgumentNullException("project");
            }

            // TODO: implement this method!

            // FIXME: return fake true for now.
            return true;
        }

        public override bool SaveTestBench(AVM.DDP.MetaAvmProject project)
        {
            if (project == null)
            {
                throw new ArgumentNullException("project");
            }

            this.ThrowIfNotExpanded();

            this.EnsureOutputDirectory();

            var success = true;

            // NOTE: saving the original design space test benches
            foreach (var testbenchRef in this.testBenchSuite.Children.TestBenchRefCollection)
            {
                var thisSuccess = project.SaveTestBench(testbenchRef.Referred.TestBenchType);
                success = success && thisSuccess;
            }

            return success;
        }

        public override bool UpdateTestBenchManifestExecutionSteps(AVM.DDP.MetaTBManifest manifest)
        {
            // FIXME: return fake true for now.
            return true;
        }
    }
}
