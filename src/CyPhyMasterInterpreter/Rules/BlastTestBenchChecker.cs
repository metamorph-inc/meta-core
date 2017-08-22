using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;


namespace CyPhyMasterInterpreter.Rules
{
    public class BlastTestBenchChecker : TestBenchTypeChecker<CyPhy.BlastTestBench>
    {
        // Blast
        public BlastTestBenchChecker(CyPhy.BlastTestBench testBench)
            : base(testBench)
        {
        }

        public override void CheckNoThrow()
        {
            base.CheckNoThrow();

            this.m_details.AddRange(this.NoTestInjectionPoints());
            this.m_details.AddRange(this.ExactlyOneReferencePlane());
            this.m_details.AddRange(this.ExactlyOnePredefinedBlastSuiteOrOneBlastModel());
        }

        protected IEnumerable<ContextCheckerResult> ExactlyOneReferencePlane()
        {
            List<ContextCheckerResult> results = new List<ContextCheckerResult>();

            var referencePlaneCount = this.testBench.Children.ReferencePlaneCollection.Count();

            if (referencePlaneCount == 0)
            {
                var feedback = new ContextCheckerResult()
                {
                    Success = false,
                    Subject = this.testBench.Impl,
                    Message = "Test bench must have one reference plane."
                };

                results.Add(feedback);
            }
            else if (referencePlaneCount == 1)
            {
                var feedback = new ContextCheckerResult()
                {
                    Success = true,
                    Subject = this.testBench.Children.ReferencePlaneCollection.FirstOrDefault().Impl,
                    Message = "One reference plane found."
                };

                results.Add(feedback);
            }
            else if (referencePlaneCount > 1)
            {
                foreach (var referencePlane in this.testBench.Children.ReferencePlaneCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = referencePlane.Impl,
                        Message = "Only one ReferencePlace object is allowed."
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

        protected IEnumerable<ContextCheckerResult> ExactlyOnePredefinedBlastSuiteOrOneBlastModel()
        {
            List<ContextCheckerResult> results = new List<ContextCheckerResult>();

            var predefinedBlastSuiteCount = this.testBench.Children.PredefinedBlastSuiteCollection.Count();
            var blastModelCount = this.testBench.Children.BlastModelCollection.Count();


            if (predefinedBlastSuiteCount + blastModelCount == 0)
            {
                string errorMessage = "Test bench has no PredefinedBlastSuite OR BlastModel. Test bench must have either one PredefinedBlastSuite or one BlastModel.";

                var feedback = new ContextCheckerResult()
                {
                    Success = false,
                    Subject = this.testBench.Impl,
                    Message = errorMessage
                };

                results.Add(feedback);

                feedback = new ContextCheckerResult()
                {
                    Success = false,
                    Subject = this.testBench.Impl,
                    Message = errorMessage
                };

                results.Add(feedback);
            }
            else if (predefinedBlastSuiteCount + blastModelCount == 1)
            {
                foreach (var predefinedBlastSuite in this.testBench.Children.PredefinedBlastSuiteCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = predefinedBlastSuite.Impl,
                        Message = string.Format("Test bench has exectly one solver settings: {0}.", predefinedBlastSuite.Kind)
                    };

                    results.Add(feedback);
                }

                foreach (var blastModel in this.testBench.Children.BlastModelCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = blastModel.Impl,
                        Message = string.Format("Test bench has exectly one solver settings: {0}.", blastModel.Kind)
                    };

                    results.Add(feedback);
                }
            }
            else if (predefinedBlastSuiteCount + blastModelCount > 1)
            {
                string errorMessage = "Test bench has more than one PredefinedBlastSuite/BlastModel. Test bench must have either one PredefinedBlastSuite or one BlastModel.";

                foreach (var predefinedBlastSuite in this.testBench.Children.PredefinedBlastSuiteCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = predefinedBlastSuite.Impl,
                        Message = errorMessage
                    };

                    results.Add(feedback);
                }

                foreach (var blastModel in this.testBench.Children.BlastModelCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = blastModel.Impl,
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
