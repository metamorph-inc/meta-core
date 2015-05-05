using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DesignConsistencyChecker;
using DesignConsistencyChecker.DesignRule;
using CyPhyCOMInterfaces;
using GME.MGA;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using GME.CSharp;

namespace CyPhy2Modelica_v2.Rules
{
    public class Checker
    {
        private CyPhyGUIs.IInterpreterMainParameters mainParameters { get; set; }
        public bool Success { get; set; }
        public CyPhyGUIs.GMELogger Logger { get; set; }
        public IMgaTraceability Traceability { get; set; }
        public int NbrOfErrors { get; set; }
        public int NbrOfWarnings { get; set; }
        public int NbrOfSuccesses { get; set; }

        public Checker(CyPhyGUIs.IInterpreterMainParameters parameters, CyPhyGUIs.GMELogger logger, IMgaTraceability traceability, CyPhyTypeFactory factory)
        {
            this.mainParameters = parameters;
            this.Logger = logger;
            this.Traceability = traceability;
            this.Factory = factory;
        }

        public CyPhyTypeFactory Factory { get; set; }

        public void Check()
        {
            CyPhy2Modelica_v2.Rules.Global.externalPackageNames = null;
            CyPhy2Modelica_v2.Rules.Global.ProjectDir = this.mainParameters.ProjectDirectory;

            // check current context
            if (this.mainParameters.CurrentFCO == null ||
                this.mainParameters.CurrentFCO.Meta.Name != typeof(CyPhy.TestBench).Name)
            {
                this.Logger.WriteError("A test bench must be opened.");
                this.Success = false;
                this.NbrOfErrors = 1;
                return;
            }
            else
            {
                var testBench = CyPhyClasses.TestBench.Cast(this.mainParameters.CurrentFCO);
                foreach (var tlsut in testBench.Children.TopLevelSystemUnderTestCollection)
                {
                    if (tlsut.AllReferred is CyPhy.DesignContainer)
                    {
                        this.Logger.WriteError("CyPhy2Modelica cannot be directly invoked on a Test Bench referencing a Design Container, "
                            + " please use the Master Interpreter.");
                        this.Success = false;
                        this.NbrOfErrors = 1;
                        return;
                    }
                }
            }

            var ruleMap = this.GetRulesByKind();

            Global.Factory = this.Factory;

            var filter = this.mainParameters.Project.CreateFilter();
            var childObjects = (this.mainParameters.CurrentFCO as MgaModel).GetDescendantFCOs(filter);
            this.Success = true;
            foreach (MgaFCO child in childObjects)
            {
                if (ruleMap.ContainsKey(child.MetaBase.MetaRef))
                {
                    foreach (var rule in ruleMap[child.MetaBase.MetaRef])
                    {
                        this.ReportFeedBack(rule.Invoke(child));
                    }
                }
                else
                {
                    // we are not checking this object
                }
            }
        }

        private void ReportFeedBack(IEnumerable<RuleFeedbackBase> feedBacks)
        {
            bool noFeedBack = true;
            foreach (var feedBack in feedBacks)
            {
                noFeedBack = false;
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

            if (noFeedBack)
            {
                this.NbrOfSuccesses += 1;
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

        public Dictionary<int, List<Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>>> GetRulesByKind()
        {
            var results = new Dictionary<int, List<Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>>>();

            // ComponentAssembly
            results.Add(this.Factory.ComponentAssemblyMeta, new List<Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>>());

            results[this.Factory.ComponentAssemblyMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ComponentAssemblyCannotShareNameWithItsParent));
            results[this.Factory.ComponentAssemblyMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ComponentParametersHaveValues));
            results[this.Factory.ComponentAssemblyMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ComponentPropertiesHaveValues));
            results[this.Factory.ComponentAssemblyMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ConnectorWithinComponentAssembly));
            results[this.Factory.ComponentAssemblyMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ElaboratorMustNotLeaveRefsInComponentAssembly));
            results[this.Factory.ComponentAssemblyMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.NamesMustBeValid));

            // Component
            results.Add(this.Factory.ComponentMeta, new List<Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>>());

            results[this.Factory.ComponentMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ComponentIsDefinedInComponentFolder));
            results[this.Factory.ComponentMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ComponentParametersHaveValues));
            results[this.Factory.ComponentMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ComponentPropertiesHaveValues));
            results[this.Factory.ComponentMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ConnectorWithinComponent));
            results[this.Factory.ComponentMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ModelicaModelHasValidURI));
            results[this.Factory.ComponentMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.NamesMustBeValid));

            // TestComponent
            results.Add(this.Factory.TestComponentMeta, new List<Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>>());

            results[this.Factory.TestComponentMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ComponentParametersHaveValues));
            results[this.Factory.TestComponentMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ComponentPropertiesHaveValues));
            results[this.Factory.TestComponentMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ConnectorWithinComponent));
            results[this.Factory.TestComponentMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ModelicaModelHasValidURI));
            results[this.Factory.TestComponentMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.NamesMustBeValid));

            // TestBench
            results.Add(this.Factory.TestBenchMeta, new List<Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>>());

            results[this.Factory.TestBenchMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ComponentParametersHaveValues));
            results[this.Factory.TestBenchMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ComponentPropertiesHaveValues));
            results[this.Factory.TestBenchMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ComponentTreeHasValidStructure));
            results[this.Factory.TestBenchMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ElaboratorMustNotLeaveRefsInTestBench));
            results[this.Factory.TestBenchMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.NamesMustBeValid));
            results[this.Factory.TestBenchMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.OneCAInTestBench));
            results[this.Factory.TestBenchMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.SolverSettings));
            results[this.Factory.TestBenchMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.Workflow));
            results[this.Factory.TestBenchMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.TestBenchNameMustBeValid));

            // Connector
            results.Add(this.Factory.ConnectorMeta, new List<Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>>());

            results[this.Factory.ConnectorMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ConnectorsMustHaveOneValidConnection));
            results[this.Factory.ConnectorMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ConnectorsShouldHaveChildren));
            results[this.Factory.ConnectorMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.NamesMustBeValid));
            results[this.Factory.ConnectorMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.NoMoreThanOneModelicaConnectorWithinConnectors));

            // ModelicaConnector
            results.Add(this.Factory.ModelicaConnectorMeta, new List<Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>>());

            results[this.Factory.ModelicaConnectorMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ModelicaConnectorHasValidURI));
            results[this.Factory.ModelicaConnectorMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ModelicaConnectorsMustHaveOneValidConnection));
            results[this.Factory.ModelicaConnectorMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ModelicaParametersHaveValues));
            results[this.Factory.ModelicaConnectorMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.NamesMustBeValid));

            // ModelicaModel
            results.Add(this.Factory.ModelicaModelMeta, new List<Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>>());

            results[this.Factory.ModelicaModelMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ModelicaParametersHaveValues));
            results[this.Factory.ModelicaModelMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.NamesMustBeValid));
            results[this.Factory.ModelicaModelMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.PortParameterUpdates));

            // Parameter
            results.Add(this.Factory.ParameterMeta, new List<Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>>());

            results[this.Factory.ParameterMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.ParametersHaveValidIncomingFlowsAndRanges));

            // Property
            results.Add(this.Factory.PropertyMeta, new List<Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>>());

            results[this.Factory.PropertyMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.PropertiesMustNotGetValuesFromParameters));

            // PostProcessing
            results.Add(this.Factory.PostProcessingMeta, new List<Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>>());

            results[this.Factory.PostProcessingMeta].Add(new Func<GME.MGA.MgaFCO, IEnumerable<RuleFeedbackBase>>(Global.PostProcessingPathIsValid));


            return results;
        }

    }
}
