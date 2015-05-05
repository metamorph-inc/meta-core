using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2Modelica_v2.Modelica
{
    public class SolverSettings
    {
        public SolverSettings(CyPhy.SolverSettings impl)
        {
            this.impl = impl;
        }

        private CyPhy.SolverSettings impl { get; set; }

        public string DymolaSolver
        {
            get
            {
                var solverString = this.impl.Attributes.Solver.ToString();
                var result = solverString;

                if (solverString.StartsWith("Dymola__"))
                {
                    var map = new Dictionary<CyPhyClasses.SolverSettings.AttributesClass.Solver_enum, string>()
                    {
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.Dymola__Cerk23__dash__order__3, "cerk23" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.Dymola__Cerk34__dash__order__4, "cerk34" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.Dymola__Cerk45__dash__order__5, "cerk45" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.Dymola__Dopri45__dash__order__5, "dopri45" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.Dymola__Dopri853__dash__order__8, "dopri853" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.Dymola__Esdirk23a__dash__order__3_stiff, "esdirk23a" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.Dymola__Esdirk34a__dash__order__4_stiff, "esdirk34a" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.Dymola__Esdirk45a__dash__order__5_stiff, "esdirk45a" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.Dymola__Lsodar, "lsodar" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.Dymola__Radau_IIa__dash__order__5_stiff, "radau" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.Dymola__Rkfix2, "rkfix2" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.Dymola__Rkfix3, "rkfix3" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.Dymola__Rkfix4, "rkfix4" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.Dymola__Sdirk34hw__dash__order__4_stiff, "sdirk34hw" },
                    };

                    result = map[this.impl.Attributes.Solver];
                }
                else if (solverString.StartsWith("OM__") || solverString.StartsWith("JModelica__"))
                {
                    result = "dassl";
                }

                return result;
            }
        }

        public string OpenModelicaSolver
        {
            get
            {
                var solverString = this.impl.Attributes.Solver.ToString();
                var result = solverString;

                if (solverString.StartsWith("OM__"))
                {
                    var map = new Dictionary<CyPhyClasses.SolverSettings.AttributesClass.Solver_enum, string>()
                    {
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.OM__dallsSymJac, "OM__dallsSymJac" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.OM__dasslwort, "OM__dasslwort" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.OM__inline_dash_euler, "OM__inline_dash_euler" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.OM__inline_dash_rungekutta, "OM__inline_dash_rungekutta" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.OM__rungekutta, "OM__rungekutta" },
                    };

                    result = map[this.impl.Attributes.Solver];
                }
                else if (solverString.StartsWith("Dymola__") || solverString.StartsWith("JModelica__"))
                {
                    result = "dassl";
                }

                return result;
            }
        }

        public string JModelicaSolver
        {
            get
            {
                var solverString = this.impl.Attributes.Solver.ToString();
                var result = solverString;

                if (solverString.StartsWith("JModelica__"))
                {
                    var map = new Dictionary<CyPhyClasses.SolverSettings.AttributesClass.Solver_enum, string>()
                    {
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.JModelica__CVode__default_, "CVode" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.JModelica__Dopri5, "Dopri5" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.JModelica__ExplicitEuler, "ExplicitEuler" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.JModelica__LSODAR, "LSODAR" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.JModelica__Radau5ODE, "Radau5ODE" },
                        {CyPhyClasses.SolverSettings.AttributesClass.Solver_enum.JModelica__RodasODE, "RodasODE" },
                    };

                    result = map[this.impl.Attributes.Solver];
                }
                else
                {
                    result = "CVode";
                }

                return result;
            }
        }

        public string ToolSelection
        {
            get
            {
                return this.impl.Attributes.JobManagerToolSelection.ToString();
            }
        }

        public double StartTime
        {
            get
            {
                return this.impl.Attributes.StartTime;
            }
        }

        public double StopTime
        {
            get
            {
                return this.impl.Attributes.StopTime;
            }
        }

        public int NumberOfIntervals
        {
            get
            {
                return this.impl.Attributes.NumberOfIntervals;
            }
        }

        public bool UsesNumberOfIntervals
        {
            get
            {
                return this.impl.Attributes.IntervalMethod == CyPhyClasses.SolverSettings.AttributesClass.IntervalMethod_enum.Number_of_Intervals;
            }
        }

        public double IntervalLength
        {
            get
            {
                return this.impl.Attributes.IntervalLength;
            }
        }

        public string ToolAnnotation
        {
            get
            {
                return this.impl.Attributes.ToolSpecificAnnotations;
            }
        }

        public double Tolerance
        {
            get
            {
                return this.impl.Attributes.Tolerance;
            }
        }
    }
}
