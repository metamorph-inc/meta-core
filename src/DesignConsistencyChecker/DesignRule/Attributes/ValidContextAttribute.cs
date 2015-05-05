using System;
using System.Collections.Generic;

namespace DesignConsistencyChecker.DesignRule.Attributes
{
    [AttributeUsage(AttributeTargets.Method, AllowMultiple = false)]
    public class ValidContextAttribute : Attribute
    {
        private List<string> _context;

        public List<string> Context
        {
            get { return _context; }
        }

        public ValidContextAttribute(params string[] context)
        {
            _context = new List<string>(context);
        }
    }
}