using DesignConsistencyChecker.DesignRule.Attributes;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using CyPhyCOMInterfaces;

namespace DesignConsistencyChecker.DesignRule.TestBench
{
    

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class ModelicaTestBench : RuleBase
    {

        //[CheckerRule("Modelica TB", Description = "Description is not mandatory.")]
        //[Tags("Modelica","Modelica 2")]
        //[ValidContext("AggregatePort")]
        public static IEnumerable<RuleFeedbackBase> CheckRule1(MgaFCO context)
        {
            return new List<RuleFeedbackBase>
                      {
                          new GenericRuleFeedback{FeedbackType = FeedbackTypes.Error, Message = "Error1"},
                          new GenericRuleFeedback{FeedbackType = FeedbackTypes.Error, Message = "Error2"},
                          new GenericRuleFeedback{FeedbackType = FeedbackTypes.Error, Message = "Error3"},
                      };
        }

        //[CheckerRule("Modelica TB 2", Description = "Description!")]
        //[Tags("Modelica", "Modelica 2")]
        //[ValidContext("Component")]
        public static IEnumerable<RuleFeedbackBase> CheckRule2(MgaFCO context)
        {
            return new List<RuleFeedbackBase>
                      {
                          new GenericRuleFeedback{FeedbackType = FeedbackTypes.Warning, Message = "Warning1"},
                      };
        }


        [CheckerRule("TLSUT", Description = "Top level system under test rule.")]
        [Tags("Modelica", "CAD", "FormulaEvaluator", "CyPython")]
        [ValidContext("TestBench")]
        public static IEnumerable<RuleFeedbackBase> CheckTLSUT(MgaFCO context)
        {
            var result = new List<RuleFeedbackBase>();

            var tsults = context.ChildObjects.OfType<MgaReference>().Where(x => x.Meta.Name == "TopLevelSystemUnderTest");

            var count = tsults.Count();

            if (count == 0)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = "There is no top level system under test object."
                };

                result.Add(feedback);
            }
            else if (count == 1)
            {
                var referred = tsults.FirstOrDefault().Referred;
                if (referred == null)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = "Top level system under test reference cannot be null."
                    };

                    result.Add(feedback);
                }
                else if (referred.Meta.Name != "DesignContainer")
                {
                    var feedback = new GenericRuleFeedback
                                       {
                        FeedbackType = FeedbackTypes.Warning,
                        Message = "Top level system under test reference MUST point to a Design Container."
                    };

                    result.Add(feedback);
                }
            }
            else
            {
                var feedback = new GenericRuleFeedback
                                   {
                    FeedbackType = FeedbackTypes.Error,
                    Message = "There is more than one top level system under test object."
                };

                result.Add(feedback);
            }

            return result;
        }



        // HyperLink
        // sb.AppendFormat("<a href=\"mga:{0}\">{1} (not mapped)</a>", subject.ID, subject.Name);
       
    }
}
