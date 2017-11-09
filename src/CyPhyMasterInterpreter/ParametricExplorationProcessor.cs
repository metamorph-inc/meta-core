﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using META;

namespace CyPhyMasterInterpreter
{
    public class ParametricExplorationProcessor : AnalysisModelProcessor
    {
        public CyPhy.ParametricExploration parametricExploration { get; set; }
        public CyPhy.ParametricExploration expandedParametricExploration { get; set; }

        public ParametricExplorationProcessor(CyPhy.ParametricExploration parametricExploration)
        {
            this.parametricExploration = parametricExploration;

            var tbRef = parametricExploration
               .Children
               .TestBenchRefCollection
               .FirstOrDefault();
            if (tbRef != null)
            {
                this.OriginalSystemUnderTest = tbRef
                    .Referred
                    .TestBenchType
                    .Children
                    .TopLevelSystemUnderTestCollection
                    .FirstOrDefault();
            }
        }

        public override MgaModel GetInvokedObject()
        {
            return this.parametricExploration.Impl as MgaModel;
        }

        public override MgaModel GetExpandedObject()
        {
            return this.expandedParametricExploration.Impl as MgaModel;
        }

        public override bool SaveDesign(AVM.DDP.MetaAvmProject projectManifest)
        {
            if (this.Configuration == null)
            {
                // standalone PET doesn't have design model
                return true;
            }
            return base.SaveDesign(projectManifest);
        }

        public override void Expand(CyPhy.ParametricExploration parametricExploration)
        {
            this.expandedParametricExploration = this.parametricExploration;
        }

        public override void Expand(CyPhy.ComponentAssembly configuration)
        {
            this.Configuration = configuration;

            // FIXME: this test should be repaired. It does not work correctly if TBs have different SUTs. Be safe and make a copy always
            // if (this.OriginalSystemUnderTest.Referred.DesignEntity.ID == configuration.ID)
            if (false)
            {
                this.expandedParametricExploration = this.parametricExploration;
            }
            else
            {
                // create temp folder for parametric exploration
                CyPhy.ParametricExplorationFolder testing = CyPhyClasses.ParametricExplorationFolder.Cast(this.parametricExploration.ParentContainer.Impl);

                var tempFolderName = AnalysisModelProcessor.GetTemporaryFolderName(this.parametricExploration.Impl);

                CyPhy.ParametricExplorationFolder tempFolder = testing.Children.ParametricExplorationFolderCollection.FirstOrDefault(x => x.Name == tempFolderName);
                if (tempFolder == null)
                {
                    tempFolder = CyPhyClasses.ParametricExplorationFolder.Create(testing);
                    tempFolder.Name = tempFolderName;

                    this.AddToTraceabilityAndTemporary(tempFolder.Impl, testing.Impl, recursive: false);
                }

                // copy parametric exploration
                var tempCopy = (tempFolder.Impl as MgaFolder).CopyFCODisp(this.parametricExploration.Impl as MgaFCO);
                // fix name
                tempCopy.Name = AnalysisModelProcessor.GetTemporaryObjectName(this.parametricExploration.Impl, configuration.Impl);

                this.AddToTraceabilityAndTemporary(tempCopy, this.parametricExploration.Impl);

                // set expanded property to the expanded element
                this.expandedParametricExploration = CyPhyClasses.ParametricExploration.Cast(tempCopy);
            }

            // expand all test benches
            foreach (var testBenchRef in this.expandedParametricExploration.Children.TestBenchRefCollection)
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
                workflow.Enqueue(new ComComponent("MGA.Interpreter.CyPhyPET", true));
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
                if (interpreter.result.Success)
                {
                    string runCommand = interpreter.result.RunCommand;
                    string title = string.Empty;
                    string testbenchName = string.Empty;
                    string workingDirectory = interpreter.MainParameters.OutputDirectory;


                    string interpreterName = interpreter.Name.StartsWith("MGA.Interpreter.") ?
                        interpreter.Name.Substring("MGA.Interpreter.".Length) :
                        interpreter.Name;

                    title = String.Format("{0}_{1}", interpreterName, this.expandedParametricExploration.Name).Replace(" ", "_");
                    testbenchName = this.parametricExploration.Name;

                    success = success && manager.EnqueueJob(runCommand, title, testbenchName, workingDirectory, ProjectDirectory, interpreter);
                }
            }

            // TODO: should this be inside the Dispatch and triggred by a timer ???
            manager.AddJobs();


            if (this.Interpreters.Count == 0)
            {
                success = false;
            }

            return success;
        }

        public override void SaveTestBenchManifest(AVM.DDP.MetaAvmProject project, string configurationName, DateTime analysisStartTime)
        {
            if (project == null)
            {
                throw new ArgumentNullException("project");
            }

            this.ThrowIfNotExpanded();

            this.EnsureOutputDirectory();

            project.SaveTestBenchManifest(
                this.Configuration != null ? this.Configuration.Name : this.parametricExploration.Name,
                configurationName,
                this.parametricExploration.Name,
                expandedParametricExploration,
                this.OutputDirectory,
                analysisStartTime);
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
            foreach (var testbenchRef in this.parametricExploration.Children.TestBenchRefCollection)
            {
                var thisSuccess = project.SaveTestBench(testbenchRef.Referred.TestBenchType);
                success = success && thisSuccess;
            }

            return success;
        }

        public override bool UpdateTestBenchManifestExecutionSteps(AVM.DDP.MetaTBManifest manifest)
        {
            manifest.Steps.Add(new AVM.DDP.MetaTBManifest.Step()
            {
                Invocation = String.Format("\"{0}\" -E -m run_mdao", VersionInfo.PythonVEnvExe),
                Description = "ParametricExploration",
                // Type = "Parametric Study",
                Status = AVM.DDP.MetaTBManifest.StatusEnum.UNEXECUTED
            });
            return true;
        }
    }

}
