using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhyMasterInterpreter.Rules
{
    public class ParametricExplorationChecker : ContextChecker
    {
        private CyPhy.ParametricExploration parametricExploration { get; set; }

        public ParametricExplorationChecker(CyPhy.ParametricExploration parametricExploration)
        {
            this.parametricExploration = parametricExploration;
        }

        public override GME.MGA.IMgaModel GetContext()
        {
            return this.parametricExploration.Impl as GME.MGA.IMgaModel;
        }

        public override void CheckNoThrow()
        {
            base.CheckNoThrow();

            this.m_details.AddRange(this.TestBenchReferences());
            this.m_details.AddRange(this.ExactlyOneDriver());
        }

        protected IEnumerable<ContextCheckerResult> TestBenchReferences()
        {
            List<ContextCheckerResult> results = new List<ContextCheckerResult>();

            var testBenchRefCount = this.parametricExploration.Children.TestBenchRefCollection.Count();
            testBenchRefCount += this.parametricExploration.Children.ParametricTestBenchCollection.Count();

            if (testBenchRefCount == 0)
            {
                var feedback = new ContextCheckerResult()
                {
                    Success = false,
                    Subject = this.parametricExploration.Impl,
                    Message = "Parametric exploration has no TestBenches, ExcelWrapper, PythonWrappers, or MATLABWrappers. There must be at least one."
                };

                results.Add(feedback);
            }

            foreach (var testBenchRef in this.parametricExploration.Children.TestBenchRefCollection)
            {
                // check test benches
                if (testBenchRef.Referred.TestBenchType == null)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = testBenchRef.Impl,
                        Message = "Test bench reference cannot be null."
                    };

                    results.Add(feedback);

                    continue;
                }
                else
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = testBenchRef.Impl,
                        Message = "Test bench reference is not null."
                    };

                    results.Add(feedback);
                }

                var testBench = testBenchRef.Referred.TestBenchType;

                // testbench ref is NOT null at this point
                ContextChecker testBenchChecker = ContextChecker.GetContextChecker(testBench.Impl as GME.MGA.MgaModel);

                testBenchChecker.TryCheck();

                results.AddRange(testBenchChecker.Details);
            }

            return results;
        }

        protected IEnumerable<ContextCheckerResult> ExactlyOneDriver()
        {
            List<ContextCheckerResult> results = new List<ContextCheckerResult>();

            var pccDriverCount = this.parametricExploration.Children.PCCDriverCollection.Count();
            var optimizerCount = this.parametricExploration.Children.OptimizerCollection.Count();
            var parameterStudyCount = this.parametricExploration.Children.ParameterStudyCollection.Count();


            if (pccDriverCount + optimizerCount + parameterStudyCount == 0)
            {
                string errorMessage = "Parametric Exploration model has no driver. It must have exactly one driver PCCDriver OR Optimizer OR ParameterStudy.";

                var feedback = new ContextCheckerResult()
                {
                    Success = false,
                    Subject = this.parametricExploration.Impl,
                    Message = errorMessage
                };

                results.Add(feedback);
            }
            else if (pccDriverCount + optimizerCount + parameterStudyCount == 1)
            {
                foreach (var pccDriver in this.parametricExploration.Children.PCCDriverCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = pccDriver.Impl,
                        Message = string.Format("Test bench has exectly driver: {0}.", pccDriver.Kind)
                    };

                    results.Add(feedback);
                }

                foreach (var optimizer in this.parametricExploration.Children.OptimizerCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = optimizer.Impl,
                        Message = string.Format("Test bench has exectly driver: {0}.", optimizer.Kind)
                    };

                    results.Add(feedback);
                }

                foreach (var parameterStudy in this.parametricExploration.Children.ParameterStudyCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = parameterStudy.Impl,
                        Message = string.Format("Test bench has exectly driver: {0}.", parameterStudy.Kind)
                    };

                    results.Add(feedback);
                }
            }
            else if (pccDriverCount + optimizerCount + parameterStudyCount > 1)
            {
                string errorMessage = "Parametric Exploration model must have exactly one driver PCCDriver OR Optimizer OR ParameterStudy.";

                foreach (var pccDriver in this.parametricExploration.Children.PCCDriverCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = pccDriver.Impl,
                        Message = errorMessage
                    };

                    results.Add(feedback);
                }

                foreach (var optimizer in this.parametricExploration.Children.OptimizerCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = optimizer.Impl,
                        Message = errorMessage
                    };

                    results.Add(feedback);
                }

                foreach (var parameterStudy in this.parametricExploration.Children.ParameterStudyCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = parameterStudy.Impl,
                        Message = errorMessage
                    };

                    results.Add(feedback);
                }
            }
            else
            {
                throw new NotImplementedException();
            }


            return results;
        }
    }
}
