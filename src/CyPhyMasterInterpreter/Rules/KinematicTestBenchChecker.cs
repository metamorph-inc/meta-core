using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhyMasterInterpreter.Rules
{
    public class KinematicTestBenchChecker : TestBenchTypeChecker<CyPhy.KinematicTestBench>
    {
        public KinematicTestBenchChecker(CyPhy.KinematicTestBench testBench)
            : base(testBench)
        {
        }

        public override void CheckNoThrow()
        {
            base.CheckNoThrow();

            // TODO: ...
            // Dynamics
            // CAD
            // ??? Any other type?
            // check workflow assumptions
        }
    }
}
