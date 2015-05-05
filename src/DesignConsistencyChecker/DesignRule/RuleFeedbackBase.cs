using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;

namespace DesignConsistencyChecker.DesignRule
{
    public abstract class RuleFeedbackBase
    {
        public FeedbackTypes FeedbackType { get; set; }

        /// <summary>
        /// This message will be displayed for the user.
        /// </summary>
        public string Message { get; set; }

        /// <summary>
        /// If the role is known use the static const strings to select it.
        /// </summary>
        public List<IMgaFCO> InvolvedObjectsByRole { get; set; }

        protected RuleFeedbackBase()
        {
            this.InvolvedObjectsByRole = new List<IMgaFCO>();
        }
    }
}
