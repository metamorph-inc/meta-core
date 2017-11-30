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

        public static IEnumerable<CyPhy.ParametricExploration> getParametricExplorationsRecursively(CyPhy.ParametricExploration exp)
        {
            Queue<CyPhy.ParametricExploration> exps = new Queue<ISIS.GME.Dsml.CyPhyML.Interfaces.ParametricExploration>();
            exps.Enqueue(exp);
            while (exps.Count > 0)
            {
                exp = exps.Dequeue();
                yield return exp;
                foreach (var sub in exp.Children.ParametricExplorationCollection)
                {
                    exps.Enqueue(sub);
                }
            }

        }

        protected IEnumerable<ContextCheckerResult> TestBenchReferences()
        {
            List<ContextCheckerResult> results = new List<ContextCheckerResult>();

            var allParametricExplorations = getParametricExplorationsRecursively(this.parametricExploration).ToList();
            var testBenchRefCount = allParametricExplorations.SelectMany(pe => pe.Children.TestBenchRefCollection).Count();
            testBenchRefCount += allParametricExplorations.SelectMany(pe => pe.Children.ParametricTestBenchCollection).Count();

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

            foreach (var testBenchRef in allParametricExplorations.SelectMany(pe => pe.Children.TestBenchRefCollection))
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


            if (pccDriverCount + optimizerCount + parameterStudyCount == 1)
            {
                foreach (var pccDriver in this.parametricExploration.Children.PCCDriverCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = pccDriver.Impl,
                        Message = string.Format("Test bench has exactly one driver: {0}.", pccDriver.Kind)
                    };

                    results.Add(feedback);
                }

                foreach (var optimizer in this.parametricExploration.Children.OptimizerCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = optimizer.Impl,
                        Message = string.Format("Test bench has exactly one driver: {0}.", optimizer.Kind)
                    };

                    results.Add(feedback);
                }

                foreach (var parameterStudy in this.parametricExploration.Children.ParameterStudyCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = parameterStudy.Impl,
                        Message = string.Format("Test bench has exactly one driver: {0}.", parameterStudy.Kind)
                    };

                    results.Add(feedback);
                }
            }
            else if (pccDriverCount + optimizerCount + parameterStudyCount > 1)
            {
                string errorMessage = "Parametric Exploration model must not ave exactly more than one driver: PCCDriver OR Optimizer OR ParameterStudy.";

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
            }


            return results;
        }
    }
}
