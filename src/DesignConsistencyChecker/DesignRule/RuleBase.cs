using System.Reflection;
using DesignConsistencyChecker.DesignRule.Attributes;
using GME.MGA;

namespace DesignConsistencyChecker.DesignRule
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using CyPhyCOMInterfaces;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public abstract class RuleBase
    {
        public List<RuleDescriptor> Rules { get; private set; }

        protected RuleBase()
        {
            this.Rules = new List<RuleDescriptor>();

            var methods = GetType().GetMembers().Where(x=>x.MemberType==MemberTypes.Method);

            foreach (var method in methods.Where(x => x.GetCustomAttributes(true).Any(y => y is CheckerRuleAttribute)).OfType<MethodInfo>().Where(x => x.IsStatic && x.ReturnType == typeof(IEnumerable<RuleFeedbackBase>)))
            {
                var checkMethod = (Func<MgaFCO, IEnumerable<RuleFeedbackBase>>)Delegate.CreateDelegate(typeof(Func<MgaFCO, IEnumerable<RuleFeedbackBase>>), method);
                var checkerRuleAttribute = method.GetCustomAttributes(true).OfType<CheckerRuleAttribute>().FirstOrDefault();
                var tagsAttribute = method.GetCustomAttributes(true).OfType<TagsAttribute>().FirstOrDefault();
                var interpretersRequiringAttribute = method.GetCustomAttributes(true).OfType<InterpretersRequiringAttribute>().FirstOrDefault();
                var validContextAttribute = method.GetCustomAttributes(true).OfType<ValidContextAttribute>().FirstOrDefault();
                var requiresElaboration = method.GetCustomAttributes(true).OfType<ElaborationRequiredAttribute>().FirstOrDefault() != null;

                if (checkerRuleAttribute == null) continue;

                if (validContextAttribute==null) throw new Exception(String.Format("Valid context must be set for rule: {0}", method.Name));

                var ruleName = checkerRuleAttribute.RuleName;
                
                Rules.Add(new RuleDescriptor(checkMethod)
                              {
                                  Name = ruleName,
                                  Tags = (tagsAttribute == null) ? new List<string>() : tagsAttribute.Tags,
                                  Description = checkerRuleAttribute.Description,
                                  InterpretersRequiring = interpretersRequiringAttribute==null?new List<string>() : interpretersRequiringAttribute.Interpreters,
                                  ValidContext = validContextAttribute.Context,
                                  RequiresElaboration = requiresElaboration
                              });
            }
        }
    }
}
