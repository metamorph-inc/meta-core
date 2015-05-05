using System;

namespace DesignConsistencyChecker.DesignRule.Attributes
{
    [AttributeUsage(AttributeTargets.Method, AllowMultiple = false)]
    public class CheckerRuleAttribute : Attribute
    {
        public string Description { get; set; }

        private string _ruleName;

        public string RuleName
        {
            get { return _ruleName; }
        }

        public CheckerRuleAttribute(string ruleName)
        {
            _ruleName = ruleName;
        }
    }
}