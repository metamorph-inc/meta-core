using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DesignConsistencyChecker.DesignRule
{
    public class CheckerFeedback
    {
        public CheckerResultTypes CheckerResultType { get; set; }

        public CheckerFeedback()
        {
            CheckerResultType = CheckerResultTypes.Ok;
        }
    }

    public enum CheckerResultTypes
    {
        Ok,
        InvalidContext
    }
}
