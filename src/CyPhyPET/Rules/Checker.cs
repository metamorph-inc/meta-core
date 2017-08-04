using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DesignConsistencyChecker;
using DesignConsistencyChecker.DesignRule;
using CyPhyCOMInterfaces;
using GME.MGA;
using GME.CSharp;
using CyPhyMasterInterpreter.Rules;

namespace CyPhyPET.Rules
{
    public class Checker
    {
        private CyPhyGUIs.IInterpreterMainParameters mainParameters { get; set; }

        public bool Success { get; set; }
        public CyPhyGUIs.GMELogger Logger { get; set; }
        public IMgaTraceability Traceability { get; set; }
        public Checker(CyPhyGUIs.IInterpreterMainParameters parameters, CyPhyGUIs.GMELogger logger, IMgaTraceability traceability)
        {
            this.Logger = logger;
            this.mainParameters = parameters;
            this.Traceability = traceability;
        }

        public int NbrOfErrors { get; set; }
        public int NbrOfWarnings { get; set; }
        public int NbrOfSuccesses { get; set; }
        /// <summary>
        /// Checks all rules from this dll.
        /// </summary>
        /// <returns>True if there are no errors, otherwise false</returns>
        public void Check()
        {
            
            // check current context
            if (this.mainParameters.CurrentFCO == null ||
                this.mainParameters.CurrentFCO.Meta.Name != typeof(ISIS.GME.Dsml.CyPhyML.Interfaces.ParametricExploration).Name)
            {
                this.Logger.WriteError("A parametric exploration model must be opened.");
                this.NbrOfErrors = 1;
                this.Success = false;
                return;
            }

            var filter = this.mainParameters.Project.CreateFilter();
            var rootContext = (this.mainParameters.CurrentFCO as MgaFCO);
            var childObjects = ((MgaModel)rootContext).GetDescendantFCOs(filter);
            this.Success = true;
            if (rootContext.Meta.Name == "ParametricExploration")
            {
                ReportFeedBack(Global.OneAndOnlyOneDriver(rootContext), rootContext, "OneAndOnlyOneDriver", "There should be one and only one driver.");
            }

            foreach (MgaFCO child in childObjects)
            {
                //this.Logger.WriteInfo("Child to be checked : {0}", child.Meta.Name);
                if (child.Meta.Name == "ParametricExploration")
                {
                    ReportFeedBack(Global.UniqueTestBenchRefNames(child), child, "UniqueTestBenchRefNames", "TestBenchReferences should have unique names");
                    ReportFeedBack(Global.CheckComponentName(child), child, "ValidComponentName", "Name must be valid");
                }
                else if (child.Meta.Name == "TestBenchRef")
                {
                    ReportFeedBack(Global.TestBenchCorrectlySetUp(child), child, "TestBenchCorrectlySetUp", "Basic rules for the test-bench reference checked.");
                }
                else if (child.Meta.Name == "PCCDriver")
                {
                    ReportFeedBack(Global.PCCDriverSetUpCorrectly(child), child, "PCCDriverSetUpCorrectly", "Checks all rules for a PCCDriver");
                }
                else if (child.Meta.Name == "ParameterStudy")
                {
                    ReportFeedBack(Global.ParameterStudySetUpCorrectly(child), child, "ParameterStudySetUpCorrectly", "Checks rules for a ParameterStudy");
                }
                else if (child.Meta.Name == "Optimizer")
                {
                    ReportFeedBack(Global.OptimizerSetUpCorrectly(child), child, "OptimizerSetUpCorrectly", "Checks rules for an Optimizer");
                }
                else if (child.Meta.Name == typeof(ISIS.GME.Dsml.CyPhyML.Interfaces.ProblemInput).Name)
                {
                    ReportFeedBack(Global.CheckProblemInput(child), child, "CheckProblemInput", "Checks rules for a ProblemInput");
                }
                else if (child.Meta.Name == typeof(ISIS.GME.Dsml.CyPhyML.Interfaces.ProblemOutput).Name)
                {
                    ReportFeedBack(Global.CheckProblemOutput(child), child, "CheckProblemOutput", "Checks rules for a ProblemOutput");
                }
                else if (PET.petWrapperTypes.Contains(child.Meta.Name))
                {
                    ReportFeedBack(Global.CheckComponentName(child), child, "ValidComponentName", "Name must be valid");
                }
            }
        }


        private void ReportFeedBack(IEnumerable<RuleFeedbackBase> feedBacks, IMgaFCO context, string ruleName, string ruleDescription)
        {
            if (feedBacks.Count() == 0)
            {
                this.Logger.WriteSuccess("{0} : {1} - {2}", context.ToMgaHyperLink(this.Traceability), ruleName, ruleDescription);
                this.NbrOfSuccesses += 1;
            }
            else
            {
                foreach (var feedBack in feedBacks)
                {
                    if (feedBack.FeedbackType == FeedbackTypes.Error)
                    {
                        this.Logger.WriteFailed(feedBack.Message);
                        if (feedBack.InvolvedObjectsByRole != null)
                        {
                            this.Logger.WriteFailed("Involved Objects : ");
                            foreach (var fco in feedBack.InvolvedObjectsByRole)
                            {
                                if (fco != null)
                                {
                                    // TODO: turn object references to valid objects using Traceability map
                                    this.Logger.WriteFailed("[ {0} ]", fco.ToMgaHyperLink(this.Traceability));
                                }
                                else
                                {
                                    this.Logger.WriteError("[ Could not get fco from InvolvedObjectsByRole for this error. (fco was null) ]");
                                }
                            }
                        }

                        this.Success = false;
                        this.NbrOfErrors += 1;
                    }
                    else if (feedBack.FeedbackType == FeedbackTypes.Warning)
                    {
                        this.Logger.WriteWarning(feedBack.Message);
                        if (feedBack.InvolvedObjectsByRole != null)
                        {
                            this.Logger.WriteWarning("Involved Objects : ");
                            foreach (var fco in feedBack.InvolvedObjectsByRole)
                            {
                                if (fco != null)
                                {
                                    // TODO: turn object references to valid objects using Traceability map
                                    this.Logger.WriteWarning("[ {0} ]", fco.ToMgaHyperLink(this.Traceability));
                                }
                                else
                                {
                                    this.Logger.WriteError("[ Could not get fco from InvolvedObjectsByRole for this error. (fco was null) ]");
                                }
                            }
                        }

                        this.NbrOfWarnings += 1;
                    }
                }
            }
        }

        public void PrintDetails()
        {
            if (this.NbrOfErrors > 0)
            {
                this.Logger.WriteError("Errors: {0}, Warnings: {1}, Successes : {2}", this.NbrOfErrors, this.NbrOfWarnings, this.NbrOfSuccesses);
            }
            else if (this.NbrOfWarnings > 0)
            {
                this.Logger.WriteWarning("Errors: {0}, Warnings: {1}, Successes : {2}", this.NbrOfErrors, this.NbrOfWarnings, this.NbrOfSuccesses);
            }
            else
            {
                this.Logger.WriteDebug("Errors: {0}, Warnings: {1}, Successes : {2}", this.NbrOfErrors, this.NbrOfWarnings, this.NbrOfSuccesses);
            }
        }

    }
}
