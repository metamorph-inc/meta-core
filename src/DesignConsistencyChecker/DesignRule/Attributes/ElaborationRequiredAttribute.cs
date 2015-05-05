using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DesignConsistencyChecker.DesignRule.Attributes
{
    [AttributeUsage(AttributeTargets.Method, AllowMultiple = false)]
    public class ElaborationRequiredAttribute : Attribute
    {
    }
}
