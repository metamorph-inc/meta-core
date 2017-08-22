using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using META;

namespace CyPhyMasterInterpreter
{
    public class MultiJobRunProcessor : TestBenchTypeProcessor
    {
        public MultiJobRunProcessor(CyPhy.TestBenchType testBenchType)
            : base(testBenchType)
        {
        }

        Queue<ComComponent> _interpretersToConfigure;
        public override System.Collections.Generic.Queue<META.ComComponent> InterpretersToConfiguration
        {
            get
            {
                if (_interpretersToConfigure == null)
                {
                    _interpretersToConfigure = new Queue<ComComponent>();
                    foreach (var interpreter in base.GetWorkflow())
                    {
                        _interpretersToConfigure.Enqueue(interpreter);
                    }
                }
                return _interpretersToConfigure;
            }
        }

        public override Queue<META.ComComponent> GetWorkflow()
        {
            Queue<ComComponent> workflow = new Queue<ComComponent>();

            this.ExecuteInTransaction(() =>
            {
                workflow.Enqueue(new ComComponent("MGA.Interpreter.CyPhyMultiJobRun", true));
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
    }
}
