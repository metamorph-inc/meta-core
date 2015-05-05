using System;
using System.Collections.Generic;

namespace DesignConsistencyChecker.DesignRule.Attributes
{
    [AttributeUsage(AttributeTargets.Method, AllowMultiple = false)]
    public class InterpretersRequiringAttribute : Attribute
    {

        private List<string> _interpreters;

        public List<string> Interpreters
        {
            get { return _interpreters; }
        }

        public InterpretersRequiringAttribute(params string[] interpreters)
        {
            _interpreters = new List<string>(interpreters);
        }
    }
}