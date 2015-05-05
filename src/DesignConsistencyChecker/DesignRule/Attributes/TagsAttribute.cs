using System;
using System.Collections.Generic;

namespace DesignConsistencyChecker.DesignRule.Attributes
{
    [AttributeUsage(AttributeTargets.Method, AllowMultiple = false)]
    public class TagsAttribute : Attribute
    {
        private List<string> _tags;

        public List<string> Tags
        {
            get { return _tags; }
        }

        public TagsAttribute(params string[] tags)
        {
            _tags = new List<string>(tags);
        }
    }
}