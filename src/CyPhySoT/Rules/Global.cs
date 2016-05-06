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
using System.IO;
using CyPhyCOMInterfaces;

namespace CyPhySoT.Rules
{
    public class Global : RuleBase
    {
        [CheckerRule("SoTObjectsValid", Description = "All objects within this SoT are valid.")]
        [Tags("SoT")]
        [ValidContext("TestBenchSuite")]
        public static IEnumerable<RuleFeedbackBase> SoTObjectsValid(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                // check for unsupported object types
                var unsupportedKinds = new List<string>();
                unsupportedKinds.AddRange(new string[] { typeof(CyPhy.TopLevelSystemUnderTest).Name });

                var fcos = (context as MgaModel)
                    .ChildObjects
                    .Cast<MgaFCO>()
                    .Where(x => unsupportedKinds.Contains(x.Meta.Name));

                foreach (var item in fcos)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("Context: {0} - {1}[{2}] is not supported",
                            context.Name,
                            item.Name,
                            item.Meta.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                    result.Add(feedback);
                }

                return result; 
            }
            catch (Exception ex)
            {
                var result = new List<RuleFeedbackBase>();

                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("Context: {0} - Exception: {1} {2}",
                        context.Name,
                        ex.Message,
                        ex.StackTrace)
                };

                feedback.InvolvedObjectsByRole.Add(context);
                result.Add(feedback);

                return result;
            }
        }

        [CheckerRule("TestBenchRefsWorkflowsAreValid", Description = "TestBenchRefs has valid workflows.")]
        [Tags("SoT")]
        [ValidContext("TestBenchSuite")]
        public static IEnumerable<RuleFeedbackBase> TestBenchRefsWorkflowsAreValid(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                // no TB null ref
                var tbRefs = (context as MgaModel)
                    .ChildObjects
                    .OfType<MgaReference>()
                    .Where(x => x.Meta.Name == typeof(CyPhy.TestBenchRef).Name);

                List<string> tbRefNames = new List<string>();
                List<string> tbIDs = new List<string>();
                MgaFCO referredSUT = null;

                foreach (var tbRef in tbRefs)
                {
                    // Unique names
                    if (tbRefNames.Contains(tbRef.Name))
                    {
                        var feedbackName = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("TestBenchRef name must be unique: {0} (within {1})",
                                tbRef.Name,
                                context.Name)
                        };

                        feedbackName.InvolvedObjectsByRole.Add(tbRef as IMgaFCO);
                        result.Add(feedbackName);
                    }
                    else
                    {
                        tbRefNames.Add(tbRef.Name);
                    }

                    // Referred TB is not null
                    if (tbRef.Referred == null)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("TestBenchRef cannot be null: {0} (within {1})",
                                tbRef.Name,
                                context.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                        result.Add(feedback);
                    }
                    else
                    {
                        // Can only use each TB once in an SoT
                        if (tbIDs.Contains(tbRef.Referred.ID))
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format("Multiple TestBenchRefs cannot refer to the same TestBench: {0} (within {1})",
                                    tbRef.Name,
                                    context.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                            result.Add(feedback);
                        }
                        else
                        {
                            tbIDs.Add(tbRef.Referred.ID);
                        }

                        CyPhy.TestBenchType referredTB = CyPhyClasses.TestBenchType.Cast(tbRef.Referred);
                        if (referredTB.Children.TopLevelSystemUnderTestCollection.Count() != 0)
                        {
                            var tlsut = referredTB.Children.TopLevelSystemUnderTestCollection.FirstOrDefault();
                            if ((tlsut.AllReferred.Impl as MgaFCO) != null)
                            {
                                if (referredSUT == null)
                                {
                                    referredSUT = tlsut.AllReferred.Impl as MgaFCO;
                                }
                                else if ((tlsut.AllReferred.Impl as MgaFCO).ID != referredSUT.ID)
                                {
                                    var feedback = new GenericRuleFeedback()
                                    {
                                        FeedbackType = FeedbackTypes.Error,
                                        Message = string.Format("All TestBenches in {0} must use to the same SystemUnderTest",
                                            context.Name)
                                    };

                                    feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                                    result.Add(feedback);
                                }
                            }
                        }
                        else
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format("TestBench {0} does not have a SystemUnderTest",
                                    tbRef.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(tbRef as IMgaFCO);
                            result.Add(feedback);
                        }

                        if (referredTB.Children.WorkflowRefCollection.Any())
                        {
                            if (referredTB.Children.WorkflowRefCollection.Count() == 1)
                            {
                                // one workflow

                                var workflowRef = referredTB.Children.WorkflowRefCollection.FirstOrDefault();
                                if (workflowRef.Referred.Workflow != null)
                                {
                                    var workflow = workflowRef.Referred.Workflow;
                                    if (workflow.Children.TaskCollection.Any())
                                    {
                                        // OK?
                                    }
                                    else
                                    {
                                        // no tasks
                                        var feedback = new GenericRuleFeedback()
                                        {
                                            FeedbackType = FeedbackTypes.Error,
                                            Message = string.Format("TestBench {0} has a workflow with no tasks",
                                                tbRef.Name)
                                        };

                                        feedback.InvolvedObjectsByRole.Add(tbRef as IMgaFCO);
                                        feedback.InvolvedObjectsByRole.Add(workflow.Impl as IMgaFCO);

                                        result.Add(feedback);
                                    }
                                }
                                else
                                {
                                    // workflow ref is null
                                    var feedback = new GenericRuleFeedback()
                                    {
                                        FeedbackType = FeedbackTypes.Error,
                                        Message = string.Format("TestBench {0} has a workflow which is a null reference",
                                            tbRef.Name)
                                    };

                                    feedback.InvolvedObjectsByRole.Add(tbRef as IMgaFCO);
                                    feedback.InvolvedObjectsByRole.Add(workflowRef.Impl as IMgaFCO);

                                    result.Add(feedback);
                                }
                            }
                            else
                            {
                                // more than one workflow
                                var feedback = new GenericRuleFeedback()
                                {
                                    FeedbackType = FeedbackTypes.Error,
                                    Message = string.Format("TestBench {0} has more than one workflow element",
                                        tbRef.Name)
                                };

                                feedback.InvolvedObjectsByRole.Add(tbRef as IMgaFCO);

                                result.Add(feedback);
                            }
                        }
                        else
                        {
                            // no workflow
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format("TestBench {0} has no workflow element",
                                    tbRef.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(tbRef as IMgaFCO);

                            result.Add(feedback);
                        }

                    }
                }

                return result;
            }
            catch (Exception ex)
            {
                var result = new List<RuleFeedbackBase>();

                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("Context: {0} - Exception: {1} {2}",
                        context.Name,
                        ex.Message,
                        ex.StackTrace)
                };

                feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                result.Add(feedback);

                return result;
            }
        }

        #region PreviousImplementation
        //[CheckerRule("SoTRulesAreSatisfied", Description = "All SoT rules are satisfied.")]
        //[Tags("SoT")]
        //[ElaborationRequired]
        //[ValidContext("TestBenchSuite")]
        //public static IEnumerable<RuleFeedbackBase> SoTRulesAreSatisfied(MgaFCO context)
        //{
        //    try
        //    {
        //        var result = new List<RuleFeedbackBase>();

        //        // check for unsupported object types
        //        var unsupportedKinds = new List<string>();
        //        unsupportedKinds.AddRange(new string[] { typeof(CyPhy.TopLevelSystemUnderTest).Name });

        //        var fcos = (context as MgaModel)
        //            .ChildObjects
        //            .Cast<MgaFCO>()
        //            .Where(x => unsupportedKinds.Contains(x.Meta.Name));

        //        // no TB null ref
        //        var tbRefs = (context as MgaModel)
        //            .ChildObjects
        //            .OfType<MgaReference>()
        //            .Where(x => x.Meta.Name == typeof(CyPhy.TestBenchRef).Name);

        //        var tbs = tbRefs.Where(x => x.Referred != null).Select(x => x.Referred);

        //        List<MgaFCO> uniqueTBs = new List<MgaFCO>();

        //        MgaFCO referredRootDesignNode = null;

        //        foreach (var tb in tbs)
        //        {
        //            var suts = tb.ChildObjects.Cast<IMgaFCO>().Where(x => x.MetaBase.Name == typeof(CyPhy.TopLevelSystemUnderTest).Name);

        //            if (suts.Count() != 1)
        //            {
        //                var feedback = new GenericRuleFeedback()
        //                {
        //                    FeedbackType = FeedbackTypes.Error,
        //                    Message = string.Format("Context: {0} - Test bench must have one system under test object. This is not the case for {1}.",
        //                        context.Name,
        //                        tb.Name)
        //                };

        //                feedback.InvolvedObjectsByRole.Add(context);
        //                result.Add(feedback);

        //                return result;
        //            }
        //            else
        //            {
        //                if ((suts.FirstOrDefault() as MgaReference).Referred.RootFCO.MetaBase.Name == typeof(CyPhy.DesignContainer).Name)
        //                {
        //                    var feedback = new GenericRuleFeedback()
        //                    {
        //                        FeedbackType = FeedbackTypes.Error,
        //                        Message = string.Format("Context: {0} - Test bench's system under test object: {1} points to a design space {2}." +
        //                                                " Use master interpreter instead.",
        //                                                context.Name,
        //                                                tb.Name,
        //                                                (suts.FirstOrDefault() as MgaReference).Referred.RootFCO.Name)
        //                    };

        //                    feedback.InvolvedObjectsByRole.Add(context);
        //                    result.Add(feedback);

        //                    return result;
        //                }

        //                // referenced test bechnes have system under test object referred to the same design
        //                if (referredRootDesignNode == null)
        //                {
        //                    referredRootDesignNode = (suts.FirstOrDefault() as MgaReference).Referred.RootFCO;
        //                }
        //                else if (referredRootDesignNode.ID != (suts.FirstOrDefault() as MgaReference).Referred.RootFCO.ID)
        //                {
        //                    var feedback = new GenericRuleFeedback()
        //                    {
        //                        FeedbackType = FeedbackTypes.Error,
        //                        Message = string.Format("Context: {0} - Test bench: {1} refers to {2} design, while other test benches refer to: {3}.",
        //                                                context.Name,
        //                                                tb.Name,
        //                                                (suts.FirstOrDefault() as MgaReference).Referred.RootFCO.Name,
        //                                                referredRootDesignNode.Name)
        //                    };

        //                    feedback.InvolvedObjectsByRole.Add(context);
        //                    result.Add(feedback);

        //                    return result;
        //                }
        //            }

        //            // TODO: Should SoT check these?
        //            var workflowRefs = tb
        //                .ChildObjects
        //                .OfType<MgaReference>()
        //                .Where(x => x.Meta.Name == typeof(CyPhy.WorkflowRef).Name);

        //            if (workflowRefs.Count() != 1)
        //            {
        //                var feedback = new GenericRuleFeedback()
        //                {
        //                    FeedbackType = FeedbackTypes.Error,
        //                    Message = string.Format("Context: {0} - There must be exactly one workflow reference in {1}.",
        //                                            context.Name,
        //                                            tb.Name)
        //                };

        //                feedback.InvolvedObjectsByRole.Add(context);
        //                result.Add(feedback);
        //            }

        //            var workflowRef = workflowRefs.FirstOrDefault();

        //            if (workflowRef != null)
        //            {
        //                if (workflowRef.Referred == null)
        //                {
        //                    var feedback = new GenericRuleFeedback()
        //                    {
        //                        FeedbackType = FeedbackTypes.Error,
        //                        Message = string.Format("Context: {0} - Workflow reference cannot be null {1}.",
        //                                                context.Name,
        //                                                workflowRef.Name)
        //                    };

        //                    feedback.InvolvedObjectsByRole.Add(context);
        //                    result.Add(feedback);
        //                }
        //                else
        //                {
        //                    var tasks = workflowRef.Referred.ChildObjects;
        //                    if (tasks.Count == 1)
        //                    {
        //                        var task = tasks.Cast<MgaFCO>().FirstOrDefault();
        //                        if (string.IsNullOrWhiteSpace(task.StrAttrByName["COMName"]))
        //                        {
        //                            var feedback = new GenericRuleFeedback()
        //                            {
        //                                FeedbackType = FeedbackTypes.Error,
        //                                Message = string.Format("Context: {0} - COMName cannot be an empty string for {1}.",
        //                                                        context.Name,
        //                                                        task.Name)
        //                            };

        //                            feedback.InvolvedObjectsByRole.Add(context);
        //                            result.Add(feedback);
        //                        }
        //                    }
        //                    else
        //                    {
        //                        var feedback = new GenericRuleFeedback()
        //                        {
        //                            FeedbackType = FeedbackTypes.Error,
        //                            Message = string.Format("Context: {0} - There must be exactly one task in {1}.",
        //                                                    context.Name,
        //                                                    workflowRef.Referred.Name)
        //                        };

        //                        feedback.InvolvedObjectsByRole.Add(context);
        //                        result.Add(feedback);
        //                    }
        //                }
        //            }

        //        }

        //        // unique TB names
        //        var names = tbRefs.Select(x => x.Name).ToList();
        //        if (names.Distinct().Count() < names.Count())
        //        {
        //            var sb = new StringBuilder();
        //            names.Sort();
        //            names.ForEach(x => sb.AppendLine(string.Format(":: {0}", x)));
        //            var feedback = new GenericRuleFeedback()
        //            {
        //                FeedbackType = FeedbackTypes.Error,
        //                Message = string.Format("Context: {0} - Test bench references must have unique names {1}.",
        //                                        context.Name,
        //                                        sb.ToString())
        //            };

        //            feedback.InvolvedObjectsByRole.Add(context);
        //            result.Add(feedback);
        //        }

        //        //error = error || UpdateDependency((context as MgaModel));
        //        return result;
        //    }
        //    catch (Exception ex)
        //    {
        //        var result = new List<RuleFeedbackBase>();

        //        var feedback = new GenericRuleFeedback()
        //        {
        //            FeedbackType = FeedbackTypes.Error,
        //            Message = string.Format("Context: {0} - Exception: {1} {2}",
        //                context.Name,
        //                ex.Message,
        //                ex.StackTrace)
        //        };

        //        feedback.InvolvedObjectsByRole.Add(context);
        //        result.Add(feedback);

        //        return result;
        //    }
        //}
        #endregion

    }
}
