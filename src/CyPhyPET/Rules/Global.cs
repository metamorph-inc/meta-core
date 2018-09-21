using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DesignConsistencyChecker;
using DesignConsistencyChecker.DesignRule.Attributes;
using DesignConsistencyChecker.DesignRule;
using GME.MGA;
using GME.CSharp;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using CyPhyCOMInterfaces;
using System.Globalization;
using ISIS.GME.Common.Interfaces;
using System.Text.RegularExpressions;

namespace CyPhyPET.Rules
{
    public class Global : RuleBase
    {
        static readonly string NAME_EXPLANATION = "Names must start with an underscore or letter, and may contain only letters, numbers, or underscores";

        [CheckerRule("OneAndOnlyOneDriver", Description = "There should be one and only one driver.")]
        [Tags("PET")]
        [ValidContext("ParametricExploration")]
        public static IEnumerable<RuleFeedbackBase> OneAndOnlyOneDriver(MgaFCO context)
        {
            var result = new List<RuleFeedbackBase>();

            var pet = CyPhyClasses.ParametricExploration.Cast(context);

            var cnt = pet.Children.ParameterStudyCollection.Count() +
                pet.Children.PCCDriverCollection.Count() +
                pet.Children.OptimizerCollection.Count();

            if (cnt > 1)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("The ParameterExploration-model has {0} drivers. There must only be one.", cnt)
                };

                result.Add(feedback);
            }

            return result;
        }

        #region TestBench
        /// <summary>
        /// Looks if a work-flow with a task is defined in testBench and if so, returns the ProgID defined
        /// in task. If the rules are not fulfilled an empty string is returned.
        /// </summary>
        /// <param name="testBench">Test-bench to look into.</param>
        /// <returns>ProgID for interpreter defined in task.</returns>
        public static List<CyPhy.TaskBase> GetTasksFromTestBench(CyPhy.TestBench testBench)
        {
            if (testBench.Children.WorkflowRefCollection.Count() == 1)
            {
                var workflowRef = testBench.Children.WorkflowRefCollection.FirstOrDefault();
                if (workflowRef != null &&
                    workflowRef.Referred.Workflow != null)
                {
                    var workflow = workflowRef.Referred.Workflow;
                    var modelTasks = workflow.Children.TaskBaseCollection;
                    List<CyPhy.TaskBase> orderedTasks = Enumerable.Repeat<CyPhy.TaskBase>(null, modelTasks.Count()).ToList();
                    Dictionary<CyPhy.TaskBase, int> tasks = new Dictionary<CyPhy.TaskBase, int>();
                    foreach (CyPhy.TaskBase task in modelTasks)
                    {
                        int index = GetTaskIndex(task, tasks);
                        orderedTasks[index] = task;
                    }
                    if (orderedTasks[orderedTasks.Count - 1] == null)
                    {
                        throw new ApplicationException("Branching is not supported in Workflow " + workflow.Name);
                    }
                    return orderedTasks;
                }
            }
            throw new ApplicationException(String.Format("TestBench {0} must have exactly one WorkflowRef", testBench.Name));
        }

        public static int GetTaskIndex(CyPhy.TaskBase task, Dictionary<CyPhy.TaskBase, int> tasks)
        {
            int index;
            if (tasks.TryGetValue(task, out index))
            {
                if (index == -1)
                {
                    throw new ApplicationException("Workflow loop involving " + task.Name);
                }
                return index;
            }
            index = 0;
            tasks.Add(task, -1);
            foreach (CyPhy.Flow connection in task.SrcConnections.FlowCollection)
            {
                index = Math.Max(index, GetTaskIndex(connection.SrcEnds.TaskBase, tasks) + 1);
            }
            tasks[task] = index;
            return index;
        }

        [CheckerRule("UniqueTestBenchRefNames", Description = "TestBenchReferences should have unique names")]
        [Tags("PET")]
        [ValidContext("ParametricExploration")]
        public static IEnumerable<RuleFeedbackBase> UniqueTestBenchRefNames(MgaFCO context)
        {
            var pet = CyPhyClasses.ParametricExploration.Cast(context);
            return UniqueTestBenchRefNames(pet);
        }

        // from openmdao group.py
        static readonly Regex namecheck_rgx = new Regex("^" + "[_a-zA-Z][_a-zA-Z0-9]*" + "$");
        public static IEnumerable<RuleFeedbackBase> CheckComponentName(MgaFCO context)
        {
            var result = new List<RuleFeedbackBase>();

            if (namecheck_rgx.Match(context.Name).Success == false)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = NAME_EXPLANATION
                };
                feedback.InvolvedObjectsByRole.Add(context);
                result.Add(feedback);
            }

            return result;
        }

        public static IEnumerable<RuleFeedbackBase> UniqueTestBenchRefNames(CyPhy.ParametricExploration pet)
        {
            var components = pet.Children.TestBenchRefCollection
                .Concat<ISIS.GME.Common.Interfaces.FCO>(pet.Children.ParametricTestBenchCollection)
                .Concat(pet.Children.ConstantsCollection)
                .Concat(pet.Children.ProblemInputCollection)
                .Concat(pet.Children.ProblemOutputCollection)
                .Concat(pet.Children.DriverCollection)
                .Concat(pet.Children.ParametricExplorationCollection);

            return UniqueNames(components);
        }

        public static IEnumerable<RuleFeedbackBase> UniqueNames(IEnumerable<FCO> components)
        {
            var result = new List<RuleFeedbackBase>();
            var dupNames = components.GroupBy(tb => tb.Name).Where(group => group.Count() > 1)
                .Select(group => group.Key);
            if (dupNames.Count() > 0)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("PET requires unique names. Duplicate names: {0}", String.Join(",", dupNames.ToArray()))
                };
                result.Add(feedback);
            }

            return result;
        }

        internal static IEnumerable<RuleFeedbackBase> CheckProblemOutput(MgaFCO child)
        {
            var result = new List<RuleFeedbackBase>();
            var incomingConnections = child.PartOfConns.Cast<IMgaConnPoint>().Where(cp => cp.ConnRole == "dst").Select(cp => cp.Owner)
                .Cast<IMgaSimpleConnection>();
            if (incomingConnections.Count() != 1)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = incomingConnections.Count() == 0 ? "ProblemOutput must have one incoming connection" :
                        "ProblemOutput may not have more than one incoming connection"
                };
                feedback.InvolvedObjectsByRole.Add(child);
                result.Add(feedback);
            }

            if (isValidParamOrOutputName(child.Name) == false)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message =
                        string.Format("Problem Output ({0}) has an invalid name. " + NAME_EXPLANATION, (object)child.Name)
                };

                feedback.InvolvedObjectsByRole.Add(child);
                result.Add(feedback);
            }

            return result;
        }

        internal static IEnumerable<RuleFeedbackBase> CheckProblemInput(MgaFCO child)
        {
            var result = new List<RuleFeedbackBase>();
            var incomingConnections = Enumerable.Cast<IMgaConnPoint>(child.PartOfConns).Where(cp => cp.ConnRole == "dst").Select(cp => cp.Owner)
                .Cast<IMgaSimpleConnection>();
            if (incomingConnections.Where(conn => conn.ParentModel.ID == child.ParentModel.ID).Count() > 1)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("ProblemInput may have only one incoming connection at the same level of hierarchy")
                };
                feedback.InvolvedObjectsByRole.Add((IMgaFCO)child);
                result.Add(feedback);
            }
            if (incomingConnections.Where(conn => conn.ParentModel.ID != child.ParentModel.ID).Count() > 1)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("ProblemInput may have only one incoming connection from the parent ParametricExploration")
                };
                feedback.InvolvedObjectsByRole.Add((IMgaFCO)child);
                result.Add(feedback);
            }
            if (incomingConnections.Where(conn => conn.ParentModel.ID == child.ParentModel.ID).Count() == 1 &&
                incomingConnections.Where(conn => conn.ParentModel.ID == child.ParentModel.ID).First().Src.Meta.Name != typeof(CyPhy.DesignVariable).Name &&
                incomingConnections.Where(conn => conn.ParentModel.ID != child.ParentModel.ID).Count() == 1)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("ProblemInput must be connected to a Design Variable") // TODO: if it is connected one hierarchy level up
                };
                feedback.InvolvedObjectsByRole.Add((IMgaFCO)child);
                result.Add(feedback);
            }

            if (isValidParamOrOutputName(child.Name) == false)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message =
                        string.Format("Problem Input ({0}) has an invalid name. " + NAME_EXPLANATION, (object)child.Name)
                };

                feedback.InvolvedObjectsByRole.Add(child);
                result.Add(feedback);
            }

            return result;
        }

        [CheckerRule("TestBenchCorrectlySetUp", Description = "Basic rules for the test-bench reference checked.")]
        [Tags("PET")]
        [ValidContext("TestBenchRef")]
        public static IEnumerable<RuleFeedbackBase> TestBenchCorrectlySetUp(MgaFCO context)
        {
            var result = new List<RuleFeedbackBase>();
            var testBenchRef = CyPhyClasses.TestBenchRef.Cast(context);
            if (testBenchRef.AllReferred == null)
            {
                result.Add(new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("TestBenchRef, '{0}', is a null reference; it must reference a TestBench.", testBenchRef)
                });
            }
            else if (testBenchRef.AllReferred is CyPhy.TestBench)
            {
                result.AddRange(checkRulesWorkFlow(testBenchRef.AllReferred as CyPhy.TestBench));
            }
            else if (testBenchRef.AllReferred is CyPhy.TestBenchType)
            {

            }
            else
            {
                // No language support of other test-bech types.
                throw new NotImplementedException("Ooops! No support for this test-bech type.");
            }

            // TODO: Maybe map the IDs into the python code and update py_modelica. In that case the check for
            // only connected parameters/metrics could suffice.
            if (testBenchRef.AllReferred != null)
            {
                var testBench = testBenchRef.AllReferred as CyPhy.TestBenchType;

                var parameters = testBench.Children.ParameterCollection;
                if (parameters.Count() != parameters.Select(p => p.Name).Distinct().Count())
                {
                    var names = String.Join(", ", parameters.Select(p => p.Name).GroupBy(p => p).Where(x => x.Count() > 1).Select(x => x.First()).ToArray());
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("Duplicate parameter names '{0}'.", names)
                    };

                    feedback.InvolvedObjectsByRole.Add(testBench.Impl as IMgaFCO);
                    result.Add(feedback);
                }

                var metrics = testBench.Children.MetricCollection;
                if (metrics.Count() != metrics.Select(p => p.Name).Distinct().Count())
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("Metrics in test-bench share common names.")
                    };

                    feedback.InvolvedObjectsByRole.Add(testBench as IMgaFCO);
                    result.Add(feedback);
                }
            }

            return result;
        }

        private static IEnumerable<RuleFeedbackBase> checkRulesWorkFlow(CyPhy.TestBench testBench)
        {
            var result = new List<RuleFeedbackBase>();
            if (testBench.Children.WorkflowRefCollection.Count() != 1)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("Test-bench must have exactly one workflow {0}.", testBench.Name)
                };

                feedback.InvolvedObjectsByRole.Add(testBench.Impl as IMgaFCO);
                result.Add(feedback);
            }
            else
            {
                var workflowRef = testBench.Children.WorkflowRefCollection.FirstOrDefault();
                if (workflowRef != null &&
                    workflowRef.Referred.Workflow == null)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("Workflow reference is null: {0}.", workflowRef.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(workflowRef.Impl as IMgaFCO);
                    result.Add(feedback);
                }
                else
                {
                    var workflow = workflowRef.Referred.Workflow;
                    if (workflow.Children.TaskCollection.Count() < 1)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("Workflow must have at least one task: {0}.",
                                workflow.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(workflow.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                }
            }

            return result;
        }

        #endregion

        #region PCC
        [CheckerRule("PCCDriverSetUpCorrectly", Description = "Checks all rules for a PCCDriver")]
        [Tags("PET")]
        [ValidContext("PCCDriver")]
        public static IEnumerable<RuleFeedbackBase> PCCDriverSetUpCorrectly(MgaFCO context)
        {
            var result = new List<RuleFeedbackBase>();
            var pccDriver = CyPhyClasses.PCCDriver.Cast(context);
            return checkPccDriverChildren(pccDriver);
        }

        private static IEnumerable<RuleFeedbackBase> checkPccDriverChildren(CyPhy.PCCDriver pccDriver)
        {
            // This function checks the PCCDriver's parameters:
            // -Ensure there is 1 connection to a TB for each one
            // -Ensure their Attributes are valid (max > min, non-zero StdDev, positive values where needed)

            var checkResults = new List<RuleFeedbackBase>();
            //List<string> childNames = new List<string>();

            // Get children, check for connections
            var parameters = pccDriver.Children.PCCParameterCollection;
            var outputs = pccDriver.Children.PCCOutputCollection;

            if (parameters.Count() < 1)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("PCC Driver {0} must have at least 1 Parameter",
                    pccDriver.Name)
                };

                feedback.InvolvedObjectsByRole.Add(pccDriver.Impl as IMgaFCO);
                checkResults.Add(feedback);
            }

            if (outputs.Count() < 1)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("PCC Driver {0} must have at least 1 Output",
                    pccDriver.Name)
                };

                feedback.InvolvedObjectsByRole.Add(pccDriver.Impl as IMgaFCO);
                checkResults.Add(feedback);
            }

            var tbParamsWithConnections = new HashSet<Tuple<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>>();

            // Check for connections and names of parameters in the test-bench.
            foreach (var param in parameters)
            {
                var attributeCheckResults = checkPccParamAttributes(param);
                if (attributeCheckResults != null &&
                    attributeCheckResults.Count() != 0)
                {
                    checkResults.AddRange(attributeCheckResults);
                }

                var driveParamCollection = param.DstConnections.DriveParameterCollection;
                var problemInputFlowSourceConnection = param.DstConnections.ProblemInputFlowSourceConnectionCollection;
                if (problemInputFlowSourceConnection.Count() + driveParamCollection.Count() > 0)
                {
                    foreach (var driveParam in driveParamCollection.Concat<ISIS.GME.Common.Interfaces.Connection>(problemInputFlowSourceConnection))
                    {
                        var source = driveParam.GenericDstEnd;
                        var tb = driveParam.GenericDstEndRef;

                        if (isValidParamOrOutputName(source.Name) == false)
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message =
                                    string.Format("Connected Parameter ({0}) has an invalid name. " + NAME_EXPLANATION, source.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(source.Impl as IMgaFCO);
                            checkResults.Add(feedback);
                        }

                        if (source.Kind == typeof(CyPhy.Parameter).Name)
                        {
                            // FIXME: if source.Kind == ProblemInput, follow where it goes and perform this check
                            var value = CyPhyClasses.Parameter.Cast(source.Impl).Attributes.Value;
                            double dummy;
                            if (value != "" && double.TryParse(value, out dummy) == false)
                            {
                                var feedback = new GenericRuleFeedback()
                                {
                                    FeedbackType = FeedbackTypes.Error,
                                    Message =
                                        string.Format("Connected Parameter {0} has a value, '{1}', that is not real.",
                                            source.Name, value)
                                };

                                feedback.InvolvedObjectsByRole.Add(source.Impl as IMgaFCO);
                                checkResults.Add(feedback);
                            }
                        }

                        if (tbParamsWithConnections.Add(
                         new Tuple<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>(tb, source)) == false)
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format("Parameter ({0}) must have only 1 connection from a PCCDriverParameter", source.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(source.Impl as IMgaFCO);
                            checkResults.Add(feedback);
                        }
                    }

                }
                else if (driveParamCollection != null)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("Driver Parameter ({0}) must have at least 1 connection to a Testbench Parameter/Property", param.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(param.Impl as IMgaFCO);
                    checkResults.Add(feedback);
                }
            }

            var tbMetricsWithConnections = new HashSet<Tuple<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>>();
            // Check for connections and names of metrics in the test-bench.
            foreach (var output in outputs)
            {
                var attributeCheckResults = checkPccOutputAttributes(output);
                if (attributeCheckResults != null &&
                    attributeCheckResults.Count() != 0)
                {
                    checkResults.AddRange(attributeCheckResults);
                }

                var pccOutputMappingCollection = output.SrcConnections.PCCOutputMappingCollection;
                var problemOutputFlowCollection = output.SrcConnections.ProblemOutputFlowTargetConnectionCollection;

                if (problemOutputFlowCollection.Count() +
                    pccOutputMappingCollection.Count() == 1)
                {
                    var pccOutputMap = (ISIS.GME.Common.Interfaces.Connection)pccOutputMappingCollection.FirstOrDefault() ?? problemOutputFlowCollection.FirstOrDefault();
                    var tbMetric = pccOutputMap.GenericSrcEnd;
                    var tb = pccOutputMap.GenericSrcEndRef;

                    if (tbMetric != null)
                    {
                        if (isValidParamOrOutputName(tbMetric.Name) == false)
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format("Connected Metric ({0}) has an invalid name. " + NAME_EXPLANATION, tbMetric.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(tbMetric.Impl as IMgaFCO);
                            checkResults.Add(feedback);
                        }
                        if (tbMetricsWithConnections.Add(
                            new Tuple<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>(tb, tbMetric)) == false)
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format("TestBench Metric ({0}) must have only 1 connection to a PCCOutput", tbMetric.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(tbMetric.Impl as IMgaFCO);
                            checkResults.Add(feedback);
                        }
                    }
                }
                else if (problemOutputFlowCollection.Count() +
                    pccOutputMappingCollection.Count() != 1)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("Driver Output ({0}) must have exactly 1 connection from a Testbench Metric or Problem Output.", output.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(output.Impl as IMgaFCO);
                    checkResults.Add(feedback);
                }
            }

            return checkResults;
        }

        private static IEnumerable<RuleFeedbackBase> checkPccOutputAttributes(CyPhy.PCCOutput output)
        {
            var attributeCheckResults = new List<RuleFeedbackBase>();

            double maxValue = 0;
            double minValue = 0;
            double pccTarget = output.Attributes.TargetPCCValue;

            bool compareValues =
                double.TryParse(output.Attributes.MaxValue.Trim(), out maxValue) &&
                double.TryParse(output.Attributes.MinValue.Trim(), out minValue);

            bool pccTargetValid =
                (0 <= pccTarget) &&
                (pccTarget <= 1);

            if (compareValues)
            {
                if (minValue > maxValue)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("PCC Output ({0}) MaxValue must be greater than MinValue", output.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(output.Impl as IMgaFCO);
                    attributeCheckResults.Add(feedback);
                }
            }
            else
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("PCC Output ({0}): Could not parse MinValue and/or MaxValue; please make sure both Attributes have decimal values.", output.Name)
                };

                feedback.InvolvedObjectsByRole.Add(output.Impl as IMgaFCO);
                attributeCheckResults.Add(feedback);
            }
            if (!pccTargetValid)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("PCC Output ({0}) TargetPCCValue must be within [0, 1]", output.Name)
                };

                feedback.InvolvedObjectsByRole.Add(output.Impl as IMgaFCO);
                attributeCheckResults.Add(feedback);
            }

            return attributeCheckResults;
        }

        private static IEnumerable<RuleFeedbackBase> checkPccParamAttributes(CyPhy.PCCParameter param)
        {
            var attributeCheckResults = new List<RuleFeedbackBase>();

            if (param is CyPhy.PCCParameterNormal)
            {
                var normalParam = param as CyPhy.PCCParameterNormal;
                double mean = normalParam.Attributes.Mean;
                double stdDev = normalParam.Attributes.StandardDeviation;
                if (stdDev <= 0)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("PCC NormalParameter ({0}) StandardDeviation must be greater than 0.", param.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(param.Impl as IMgaFCO);
                    attributeCheckResults.Add(feedback);
                }
            }
            else if (param is CyPhy.PCCParameterUniform)
            {
                var uniformParam = param as CyPhy.PCCParameterUniform;
                double highLimit = uniformParam.Attributes.HighLimit;
                double lowLimit = uniformParam.Attributes.LowLimit;

                if (lowLimit >= highLimit)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("PCC UniformParameter ({0}) HighLimit (b) must be greater than LowLimit (a).", param.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(param.Impl as IMgaFCO);
                    attributeCheckResults.Add(feedback);
                }
            }
            else if (param is CyPhy.PCCParameterLNormal)
            {
                var logNormalParam = param as CyPhy.PCCParameterLNormal;
                double logScale = logNormalParam.Attributes.LogScale;
                double shape = logNormalParam.Attributes.Shape;

                if (shape <= 0)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("PCC LogNormalParameter ({0}) Shape  must be greater than 0.", param.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(param.Impl as IMgaFCO);
                    attributeCheckResults.Add(feedback);
                }
            }
            else if (param is CyPhy.PCCParameterBeta)
            {
                var betaParam = param as CyPhy.PCCParameterBeta;
                double highLimit = betaParam.Attributes.HighLimit;
                double lowLimit = betaParam.Attributes.LowLimit;
                double shape1 = betaParam.Attributes.Shape1;
                double shape2 = betaParam.Attributes.Shape2;

                if (lowLimit >= highLimit)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("PCC BetaParameter ({0}) HighLimit (b) must be greater than LowLimit (a).", param.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(param.Impl as IMgaFCO);
                    attributeCheckResults.Add(feedback);
                }
                if (shape1 <= 0)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("PCC BetaParameter ({0}) Shape1 (alpha) must be greater than 0.", param.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(param.Impl as IMgaFCO);
                    attributeCheckResults.Add(feedback);
                }
                if (shape2 <= 0)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("PCC BetaParameter ({0}) Shape2 (beta) must be greater than 0.", param.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(param.Impl as IMgaFCO);
                    attributeCheckResults.Add(feedback);
                }
            }
            else
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("Could not cast PCC parameter ({0}) as any known type.", param.Name)
                };

                feedback.InvolvedObjectsByRole.Add(param.Impl as IMgaFCO);
                attributeCheckResults.Add(feedback);
            }

            return attributeCheckResults;
        }

        #endregion

        #region ParameterStudy
        [CheckerRule("ParameterStudySetUpCorrectly", Description = "Checks rules for a ParameterStudy")]
        [Tags("PET")]
        [ValidContext("ParameterStudy")]
        public static IEnumerable<RuleFeedbackBase> ParameterStudySetUpCorrectly(MgaFCO context)
        {
            var parameterStudy = CyPhyClasses.ParameterStudy.Cast(context);
            return checkMdaoDriverChildren(context);
        }

        private static IEnumerable<RuleFeedbackBase> checkMdaoDriverChildren(MgaFCO context)
        {
            var checkResults = new List<RuleFeedbackBase>();

            var mdaoDriver = CyPhyClasses.MDAODriver.Cast(context);
            // META-2995
            bool allowDiscreteRanges = false;
            if (context.MetaBase.Name == typeof(CyPhy.ParameterStudy).Name)
            {
                var parameterStudy = CyPhyClasses.ParameterStudy.Cast(context);
                allowDiscreteRanges = parameterStudy.Attributes.SurrogateType == CyPhyClasses.ParameterStudy.AttributesClass.SurrogateType_enum.None;
            }

            // Get children, check for connections
            var designVariables = mdaoDriver.Children.DesignVariableCollection;
            var objectives = mdaoDriver.Children.ObjectiveCollection;

            if (objectives.Count() < 1)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("MDAO Driver {0} must have at least 1 Objective",
                    context.Name)
                };

                feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                checkResults.Add(feedback);
            }

            var tbParamsWithConnections = new HashSet<Tuple<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>>();
            // Check for connections and names of parameters in the test-bench.
            var rangeLengths = new List<int>();
            foreach (var designVar in designVariables)
            {
                checkResults.AddRange(checkMdaoDesignVariableAttributes(designVar, allowDiscreteRanges: allowDiscreteRanges));
                // META-2995
                rangeLengths.Add(discreteRange);

                var varSweepCollection = designVar.DstConnections.VariableSweepCollection;
                if (varSweepCollection.Count() == 1)
                {
                    CyPhy.VariableSweep varSweep = varSweepCollection.FirstOrDefault();
                    var tbParam = varSweep.DstEnds.Parameter;
                    var tb = varSweep.GenericDstEndRef;
                    if (tbParam != null)
                    {
                        var tbParamParent = tbParam.ParentContainer;
                        if (tbParamParent != null &&
                            ((tbParamParent is CyPhy.TestBenchType) == false && (tbParamParent is CyPhy.ParametricTestBench) == false))
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format("Driver DesignVariable ({0}) must have 1 connection to a Testbench Parameter.", designVar.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(designVar.Impl as IMgaFCO);
                            checkResults.Add(feedback);
                        }
                        else
                        {
                            if (isValidParamOrOutputName(tbParam.Name) == false)
                            {
                                var feedback = new GenericRuleFeedback()
                                {
                                    FeedbackType = FeedbackTypes.Error,
                                    Message = string.Format("Connected Parameter ({0}) has an invalid name. " + NAME_EXPLANATION, tbParam.Name)
                                };

                                feedback.InvolvedObjectsByRole.Add(tbParam.Impl as IMgaFCO);
                                checkResults.Add(feedback);
                            }

                            var value = tbParam.Attributes.Value;
                            double dummy;
                            if ((tbParamParent is CyPhy.ExcelWrapper) == false && (tbParamParent is CyPhy.PythonWrapper) == false && value != "" && double.TryParse(value, out dummy) == false)
                            {
                                var feedback = new GenericRuleFeedback()
                                {
                                    FeedbackType = FeedbackTypes.Error,
                                    Message = string.Format("Connected Parameter {0} has a value, '{1}', that is not real.", tbParam.Name, value)
                                };

                                feedback.InvolvedObjectsByRole.Add(tbParam.Impl as IMgaFCO);
                                checkResults.Add(feedback);
                            }
                        }

                        if (tbParamsWithConnections.Add(
                            new Tuple<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>(tb, tbParam)) == false)
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format("TestBench Parameter ({0}) must have only 1 connection from a DesignVariable", tbParam.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(tbParam.Impl as IMgaFCO);
                            checkResults.Add(feedback);
                        }
                    }
                }
                else if (varSweepCollection != null &&
                    varSweepCollection.Count() == 0)
                {
                    var problemInputs = designVar.DstConnections.ProblemInputFlowSourceConnectionCollection;
                    if (problemInputs.Count() < 1)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("Driver DesignVariable ({0}) must have at least 1 connection to a Testbench Parameter/Property", designVar.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(designVar.Impl as IMgaFCO);
                        checkResults.Add(feedback);
                    }
                }
            }

            // META-2995
            if (allowDiscreteRanges && (rangeLengths.Distinct().Count() > 1))
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("Discrete ranges in ({0}) are not of the same length.", mdaoDriver.Name)
                };

                feedback.InvolvedObjectsByRole.Add(mdaoDriver.Impl as IMgaFCO);
                checkResults.Add(feedback);
            }
            // Check for connections and names of metrics in the test-bench.
            foreach (var obj in objectives)
            {
                var objMappingCollection = obj.SrcConnections.ObjectiveMappingCollection;
                var fileFlowCollection = obj.SrcConnections.FileResultFlowCollection;
                var outputColection = obj.SrcConnections.ProblemOutputFlowTargetConnectionCollection;
                if (objMappingCollection.Count() + fileFlowCollection.Count() + outputColection.Count() == 1)
                {
                    FCO tbMetric;
                    if (objMappingCollection.Count() == 1)
                    {
                        CyPhy.ObjectiveMapping objMap = objMappingCollection.FirstOrDefault();
                        tbMetric = objMap.SrcEnd;
                    }
                    else if (outputColection.Count() == 1)
                    {
                        tbMetric = outputColection.First().SrcEnd;
                    }
                    else
                    {
                        var fileFlow = fileFlowCollection.First();
                        tbMetric = fileFlow.SrcEnd;
                    }
                    if (tbMetric != null)
                    {
                        var tbMetricParent = tbMetric.ParentContainer;
                        if ((tbMetricParent is CyPhy.TestBenchType) == false
                            && (tbMetricParent is CyPhy.ParametricTestBench) == false
                            && (tbMetricParent is CyPhy.Constants) == false
                            && (tbMetricParent is CyPhy.ParametricExploration) == false)
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format("Driver Objective ({0}) must have 1 connection from a Testbench Metric", obj.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(obj.Impl as IMgaFCO);
                            checkResults.Add(feedback);
                        }
                        else
                        {
                            if (isValidParamOrOutputName(tbMetric.Name) == false)
                            {
                                var feedback = new GenericRuleFeedback()
                                {
                                    FeedbackType = FeedbackTypes.Error,
                                    Message = string.Format("Connected Metric ({0}) has an invalid name. " + NAME_EXPLANATION, tbMetric.Name)
                                };

                                feedback.InvolvedObjectsByRole.Add(tbMetric.Impl as IMgaFCO);
                                checkResults.Add(feedback);
                            }
                        }
                    }
                }
                else
                {
                    if (objMappingCollection.Count() + fileFlowCollection.Count() + outputColection.Count() > 1)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("Driver Objective ({0}) must not have multiple incoming connections", obj.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(obj.Impl as IMgaFCO);
                        checkResults.Add(feedback);
                    }
                    else if (objMappingCollection.Count() + fileFlowCollection.Count() + outputColection.Count() == 0)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Warning,
                            Message = string.Format("Driver Objective ({0}) does not have an incoming connection", obj.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(obj.Impl as IMgaFCO);
                        checkResults.Add(feedback);
                    }
                }
            }

            return checkResults;
        }

        private static int discreteRange = -1;
        private static IEnumerable<RuleFeedbackBase> checkMdaoDesignVariableAttributes(
            CyPhy.DesignVariable designVar,
            bool allowDiscreteRanges = false)
        {
            var attributeCheckResults = new List<RuleFeedbackBase>();

            string range = designVar.Attributes.Range;
            string[] splitRange = range.Split(',');

            if (allowDiscreteRanges && range.StartsWith("[") && range.EndsWith("]"))
            {
                // META-2995
                var rangePieces = range.TrimStart('[').TrimEnd(']').Split(',');
                discreteRange = rangePieces.Count();
                foreach (var rp in rangePieces)
                {
                    double val = 0;
                    if (double.TryParse(rp, NumberStyles.Float | NumberStyles.AllowThousands, CultureInfo.InvariantCulture, out val) == false)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("{0} seems to defined as a discrete range of values. It must be of the form [x1, .., xn], where xi is a double, but found a '{1}'.",
                                designVar.Name, rp)
                        };

                        feedback.InvolvedObjectsByRole.Add(designVar.Impl as IMgaFCO);
                        attributeCheckResults.Add(feedback);
                    }
                }
            }
            else if (splitRange.Count() == 2)
            {
                double min = 0;
                double max = 0;
                string minStr = splitRange[0];
                string maxStr = splitRange[1];
                if (minStr[0] == '[' || minStr[0] == '(')
                {
                    minStr = minStr.Substring(1);
                }
                if (maxStr.Last() == ']' || maxStr.Last() == ')')
                {
                    maxStr = maxStr.Substring(0, maxStr.Length - 1);
                }
                bool canGetMaxAndMin =
                    double.TryParse(minStr, NumberStyles.Float | NumberStyles.AllowThousands,
                            CultureInfo.InvariantCulture, out min) &&
                    double.TryParse(maxStr, NumberStyles.Float | NumberStyles.AllowThousands,
                            CultureInfo.InvariantCulture, out max);
                if (canGetMaxAndMin)
                {
                    if (min > max)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("MDAODriver DesignVariable ({0}) Range: Min greater than Max", designVar.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(designVar.Impl as IMgaFCO);
                        attributeCheckResults.Add(feedback);
                    }
                }
                else
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("MDAODriver DesignVariable ({0}) Range: Could not parse values... Are they decimals?", designVar.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(designVar.Impl as IMgaFCO);
                    attributeCheckResults.Add(feedback);
                }
            }
            else if (splitRange.Count() == 1)
            {
                double singlePoint = 0;
                if (double.TryParse(splitRange[0].Trim(), NumberStyles.Float | NumberStyles.AllowThousands,
                            CultureInfo.InvariantCulture, out singlePoint) == false)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("MDAODriver DesignVariable ({0}) Range: Could not parse value... is it a decimal?", designVar.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(designVar.Impl as IMgaFCO);
                    // FIXME: need to rewrite this test for ;-delimited enums
                    // attributeCheckResults.Add(feedback);
                }
            }
            else// if (range != "-inf..inf") We need to make a judgement call on whether or not to accept "-inf..inf"
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("MDAODriver DesignVariable ({0}) Range Attribute should have format 'Min, Max' where Min and Max are Reals, or 'Val' where Val is Real", designVar.Name)
                };

                feedback.InvolvedObjectsByRole.Add(designVar.Impl as IMgaFCO);
                attributeCheckResults.Add(feedback);
            }

            return attributeCheckResults;
        }

        #endregion

        #region Optimizer
        [CheckerRule("OptimizerSetUpCorrectly", Description = "Checks rules for an Optimizer")]
        [Tags("PET")]
        [ValidContext("Optimizer")]
        public static IEnumerable<RuleFeedbackBase> OptimizerSetUpCorrectly(MgaFCO context)
        {
            var result = new List<RuleFeedbackBase>();
            var optimizer = CyPhyClasses.Optimizer.Cast(context);
            result.AddRange(checkMdaoDriverChildren(context));

            var optimizerConstraints = optimizer.Children.OptimizerConstraintCollection;
            foreach (var optConstraint in optimizerConstraints)
            {
                var attributeCheckResults = checkOptimizerConstraintAttributes(optConstraint);
                if (attributeCheckResults != null &&
                    attributeCheckResults.Count() != 0)
                {
                    result.AddRange(attributeCheckResults);
                }
            }

            if (optimizer.Children.ObjectiveCollection.Count() > 1)
            {
                result.Add(new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("Optimizers cannot have more than one objective.")
                });
            }

            return result;
        }

        private static IEnumerable<RuleFeedbackBase> checkOptimizerConstraintAttributes(CyPhy.OptimizerConstraint optConstraint)
        {
            var attributeCheckResults = new List<RuleFeedbackBase>();

            double minValue = Double.NegativeInfinity;
            double maxValue = Double.PositiveInfinity;

            bool canGetMinAndMax =
                (optConstraint.Attributes.MinValue == "" || double.TryParse(optConstraint.Attributes.MinValue.Trim(), NumberStyles.Float | NumberStyles.AllowThousands, CultureInfo.InvariantCulture, out minValue)) &&
                (optConstraint.Attributes.MaxValue == "" || double.TryParse(optConstraint.Attributes.MaxValue.Trim(), NumberStyles.Float | NumberStyles.AllowThousands, CultureInfo.InvariantCulture, out maxValue));

            if (canGetMinAndMax)
            {
                if (minValue > maxValue)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Warning,
                        //FeedbackType = FeedbackTypes.Error: Seems like it should be an Error, but I don't want to block anybody
                        Message = string.Format("OptimizerConstraint ({0}) Range Attribute: Min greater than Max", optConstraint.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(optConstraint.Impl as IMgaFCO);
                    attributeCheckResults.Add(feedback);
                }
            }
            else
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Warning,
                    Message = string.Format("OptimizerConstraint ({0}) Attributes: MinValue and/or MaxValue are not decimals", optConstraint.Name)
                };

                feedback.InvolvedObjectsByRole.Add(optConstraint.Impl as IMgaFCO);
                attributeCheckResults.Add(feedback);
            }

            return attributeCheckResults;
        }

        #endregion

        // from openmdao component.py
        static readonly Regex param_output_namecheck_rgx = new Regex("^" + "([_a-zA-Z][_a-zA-Z0-9]*)+(:[_a-zA-Z][_a-zA-Z0-9]*)*" + "$");
        public static bool isValidParamOrOutputName(string childName)
        {
            return param_output_namecheck_rgx.Match(childName).Success;
        }

    }
}
