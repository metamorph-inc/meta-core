using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DesignConsistencyChecker;
using DesignConsistencyChecker.DesignRule;
using CyPhyCOMInterfaces;

namespace CyPhySoT.Rules
{
    public class Checker
    {
        private CyPhyGUIs.IInterpreterMainParameters mainParameters { get; set; }

        public Dictionary<RuleDescriptor, List<RuleFeedbackBase>> Results { get; set; }
        //public static GME.CSharp.GMEConsole GMEConsole { get; set; }
        public CyPhyGUIs.GMELogger Logger { get; set; }
        public Checker(CyPhyGUIs.IInterpreterMainParameters parameters, CyPhyGUIs.GMELogger logger)
        {
            this.Logger = logger;
            this.mainParameters = parameters;
            this.Results = new Dictionary<RuleDescriptor, List<RuleFeedbackBase>>();
        }

        int NbrOfErrors { get; set; }
        int NbrOfWarnings { get; set; }
        /// <summary>
        /// Checks all rules from this dll.
        /// </summary>
        /// <returns>True if there are no errors, otherwise false</returns>
        public bool Check(IMgaTraceability traceability)
        {
            bool result = true;

            // check current context
            if (this.mainParameters.CurrentFCO == null ||
                this.mainParameters.CurrentFCO.Meta.Name != typeof(ISIS.GME.Dsml.CyPhyML.Interfaces.TestBenchSuite).Name)
            {
                this.Logger.WriteError("A TestBenchSuite must be opened.");
                result = false;
                NbrOfErrors = 1;
                return result;
            }

            // reset the results dictionary
            this.Results = new Dictionary<RuleDescriptor, List<RuleFeedbackBase>>();

            // Create a checker object
            DesignConsistencyChecker.Framework.Checker dccChecker =
                new DesignConsistencyChecker.Framework.Checker(this.mainParameters.CurrentFCO, this.mainParameters.Project, traceability, this.Logger);

            dccChecker.RegisterRulesForTypes(new Type[] { typeof(CyPhySoT.Rules.Global) }, "CyPhySoT");

            NbrOfErrors = 0;
            NbrOfWarnings = 0;
            // get all rules and check them

            List<RuleFeedbackBase> ruleFeedbacks;
            List<CheckerFeedback> checkerFeedbacks;
            dccChecker.CheckRules(dccChecker.GetRegisteredRules, out ruleFeedbacks, out checkerFeedbacks);
            if (ruleFeedbacks.Any(x => x.FeedbackType == FeedbackTypes.Error))
            {
                result = false;
            }
            NbrOfErrors += ruleFeedbacks.Where(x => x.FeedbackType == FeedbackTypes.Error).Count();
            NbrOfWarnings += ruleFeedbacks.Where(x => x.FeedbackType == FeedbackTypes.Warning).Count();

            return result;
        }

        public void PrintDetails()
        {
            if (NbrOfErrors > 0)
            {
                this.Logger.WriteError("Errors: {0}, Warnings: {1}", NbrOfErrors, NbrOfWarnings);
            }
            else if (NbrOfWarnings > 0)
            {
                this.Logger.WriteWarning("Errors: {0}, Warnings: {1}", NbrOfErrors, NbrOfWarnings);
            }
            else
            {
                this.Logger.WriteDebug("Errors: {0}, Warnings: {1}", NbrOfErrors, NbrOfWarnings);
            }
        }
    }
}
