using System.ComponentModel;
using GME.MGA;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DesignConsistencyChecker.DesignRule.Attributes;

using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyMLClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using CyPhyCOMInterfaces;

namespace DesignConsistencyChecker.DesignRule.UniquePPMNames
{
    
    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class UniquePPMNames : RuleBase
    {
        public UniquePPMNames()
            : base()
        {
            // TODO: initialize valid context
        }

        [CheckerRule("UniquePPMNames")]
        [Tags("Modelica")]
        [ValidContext("Component")]
        public static IEnumerable<RuleFeedbackBase> CheckRule(MgaFCO context)
        {
            var result = new List<RuleFeedbackBase>();

            CyPhyML.Component cyPhyMLComponent = ISIS.GME.Common.Utils.CreateObject<CyPhyMLClasses.Component>(context as MgaObject);

            Dictionary<string, List<CyPhyML.HasDescriptionAndGUID>> namePPMListMap = new Dictionary<string, List<CyPhyML.HasDescriptionAndGUID> >();

            List<object> objectList = cyPhyMLComponent.AllChildren.ToList<object>();
            foreach( CyPhyML.HasDescriptionAndGUID cyPhyMLHasDescriptionAndGUID in cyPhyMLComponent.AllChildren.
             Where(x => x.GetType().UnderlyingSystemType == typeof(CyPhyMLClasses.Parameter) || x.GetType().UnderlyingSystemType == typeof(CyPhyMLClasses.Property) || x.GetType().UnderlyingSystemType == typeof(CyPhyMLClasses.Metric))
            ) {
                Type type = cyPhyMLHasDescriptionAndGUID.GetType();
                string name = cyPhyMLHasDescriptionAndGUID.Name;
                if (!namePPMListMap.ContainsKey(name)) {
                    namePPMListMap[name] = new List<CyPhyML.HasDescriptionAndGUID>();
                }
                namePPMListMap[name].Add(cyPhyMLHasDescriptionAndGUID);
            }

            foreach (string name in namePPMListMap.Keys) {
                List<CyPhyML.HasDescriptionAndGUID> ppmList = namePPMListMap[name];
                if (ppmList.Count > 1) {
                    var genericRuleFeedback = new GenericRuleFeedback() {
                        FeedbackType = FeedbackTypes.Error,
                        Message = "Name \"" + name + "\" not unique between Parameter, Property, and Metric children of Component \"" + cyPhyMLComponent.Name + "\""
                    };
                    result.Add(genericRuleFeedback);
                }
            }
            return result;
        }
    }
}
