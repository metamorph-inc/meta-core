using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhyMasterInterpreter.Rules
{
    public class CFDTestBenchChecker : TestBenchTypeChecker<CyPhy.CFDTestBench>
    {
        // CFD
        public CFDTestBenchChecker(CyPhy.CFDTestBench testBench)
            : base(testBench)
        {
        }

        public override void CheckNoThrow()
        {
            base.CheckNoThrow();

            this.m_details.AddRange(this.NoTestInjectionPoints());
            this.m_details.AddRange(this.ExactlyOneSolverSettings());
        }

        protected IEnumerable<ContextCheckerResult> ExactlyOneSolverSettings()
        {
            List<ContextCheckerResult> results = new List<ContextCheckerResult>();

            var calmWaterSolverCount = this.testBench.Children.CalmWaterSolverSettingsCollection.Count();
            var waveResistanceSolverCount = this.testBench.Children.WaveResistanceSolverSettingsCollection.Count();
            var correlationSettingsCount = this.testBench.Children.CorrelationSettingsCollection.Count();
            var hydrostaticsSolverCount = this.testBench.Children.HydrostaticsSolverSettingsCollection.Count();

            if (calmWaterSolverCount + waveResistanceSolverCount + correlationSettingsCount + hydrostaticsSolverCount == 0)
            {
                string errorMessage = "Test bench has no solver settings. It must have of one solver settings CalmWaterSolverSettings, " +
                    "WaveResistanceSolverSettings, CorrelationSettings or HydrostaticsSolverSettings. A HydrostaticsSolverSettings can " +
                    "further be combined with one of the other types.";

                var feedback = new ContextCheckerResult()
                {
                    Success = false,
                    Subject = this.testBench.Impl,
                    Message = errorMessage
                };

                results.Add(feedback);
            }
            else if (calmWaterSolverCount + waveResistanceSolverCount + correlationSettingsCount + hydrostaticsSolverCount == 1)
            {
                foreach (var calmWaterSolver in this.testBench.Children.CalmWaterSolverSettingsCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = calmWaterSolver.Impl,
                        Message = string.Format("Test bench has exectly one solver settings: {0}.", calmWaterSolver.Kind)
                    };

                    results.Add(feedback);
                }

                foreach (var waveResistanceSolver in this.testBench.Children.WaveResistanceSolverSettingsCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = waveResistanceSolver.Impl,
                        Message = string.Format("Test bench has exectly one solver settings: {0}.", waveResistanceSolver.Kind)
                    };

                    results.Add(feedback);
                }

                foreach (var correlationSetting in this.testBench.Children.CorrelationSettingsCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = correlationSetting.Impl,
                        Message = string.Format("Test bench has exectly one solver settings: {0}.", correlationSetting.Kind)
                    };

                    results.Add(feedback);
                }

                foreach (var hydrostaticsSolver in this.testBench.Children.HydrostaticsSolverSettingsCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = hydrostaticsSolver.Impl,
                        Message = string.Format("Test bench has exectly one solver settings: {0}.", hydrostaticsSolver.Kind)
                    };

                    results.Add(feedback);
                }
            }
            else if (calmWaterSolverCount + waveResistanceSolverCount + correlationSettingsCount > 1)
            {
                string errorMessage = "Test bench can only have one solver setting of type CalmWaterSolverSettings, WaveResistanceSolverSettings and CorrelationSettings.";

                foreach (var calmWaterSolver in this.testBench.Children.CalmWaterSolverSettingsCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = calmWaterSolver.Impl,
                        Message = errorMessage
                    };

                    results.Add(feedback);
                }

                foreach (var waveResistanceSolver in this.testBench.Children.WaveResistanceSolverSettingsCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = waveResistanceSolver.Impl,
                        Message = errorMessage
                    };

                    results.Add(feedback);
                }

                foreach (var correlationSetting in this.testBench.Children.CorrelationSettingsCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = correlationSetting.Impl,
                        Message = errorMessage
                    };

                    results.Add(feedback);
                }
            }
            else if (hydrostaticsSolverCount > 1)
            {
                string errorMessage = "Test bench can only have one solver setting of type HydrostaticsSolverSettings.";

                foreach (var hydrostaticsSolver in this.testBench.Children.HydrostaticsSolverSettingsCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = hydrostaticsSolver.Impl,
                        Message = errorMessage
                    };

                    results.Add(feedback);
                }
            }
            else if ((calmWaterSolverCount + waveResistanceSolverCount + correlationSettingsCount) == 1 &&
                      hydrostaticsSolverCount == 1)
            {
                foreach (var hydrostaticsSolver in this.testBench.Children.HydrostaticsSolverSettingsCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = hydrostaticsSolver.Impl,
                        Message = string.Format("Test bench has one {0} and other solver-settings.", hydrostaticsSolver.Kind)
                    };

                    results.Add(feedback);
                }

                foreach (var calmWaterSolver in this.testBench.Children.CalmWaterSolverSettingsCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = calmWaterSolver.Impl,
                        Message = string.Format("Test bench has one HydrostaticsSolverSettings and a {0}.", calmWaterSolver.Kind)
                    };

                    results.Add(feedback);
                }

                foreach (var waveResistanceSolver in this.testBench.Children.WaveResistanceSolverSettingsCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = waveResistanceSolver.Impl,
                        Message = string.Format("Test bench has one HydrostaticsSolverSettings and a {0}.", waveResistanceSolver.Kind)
                    };

                    results.Add(feedback);
                }

                foreach (var correlationSetting in this.testBench.Children.CorrelationSettingsCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = correlationSetting.Impl,
                        Message = string.Format("Test bench has one HydrostaticsSolverSettings and a {0}.", correlationSetting.Kind)
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
