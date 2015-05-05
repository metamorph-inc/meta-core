using System;
using System.Collections.Generic;
using System.Data;
using GME.MGA;
using CyPhyCOMInterfaces;

namespace DesignConsistencyChecker.DesignRule
{
    public class RuleDescriptor
    {
        public string Name { get; set; }
        public string Description { get; set; }
        public List<string> Tags { get; set; }
        public List<string> ValidContext { get; set; }
        public List<string> InterpretersRequiring { get; set; }
        public bool RequiresElaboration { get; set; }

        public Func<MgaFCO, IEnumerable<RuleFeedbackBase>> CheckRuleDelegate { get; private set; }

        public RuleDescriptor(Func<MgaFCO, IEnumerable<RuleFeedbackBase>> checkRuleDelegate)
        {
            if (checkRuleDelegate == null) throw new NoNullAllowedException("Checkrule delegate cannot be null!");
            CheckRuleDelegate = checkRuleDelegate;
            Tags = new List<string>();
            ValidContext = new List<string>();
            InterpretersRequiring = new List<string>();
            RequiresElaboration = false;
        }

        public IEnumerable<RuleFeedbackBase> Check(MgaFCO context)
        {
            return CheckRuleDelegate.Invoke(context);
        }
    }
}