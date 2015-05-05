using System.ComponentModel;
using GME.MGA;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DesignConsistencyChecker.DesignRule.Attributes;
using CyPhyCOMInterfaces;

namespace DesignConsistencyChecker.DesignRule.TestBench
{
    

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class ModelicaTestBench2 : RuleBase
    {
        public ModelicaTestBench2()
            : base()
        {
            // TODO: initialize valid context
        }

        //[CheckerRule("Masik rule")]
        //[ValidContext("a","b","c")]
        //[Tags("Modelica")]
        //public static IEnumerable<RuleFeedbackBase> CheckRule(MgaFCO context)
        //{
        //    return new List<RuleFeedbackBase>();
        //}
    }
}
