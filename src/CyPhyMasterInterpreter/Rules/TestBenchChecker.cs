using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhyMasterInterpreter.Rules
{
    public class TestBenchChecker : TestBenchTypeChecker<CyPhy.TestBench>
    {
        public TestBenchChecker(CyPhy.TestBench testBench)
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

            this.m_details.AddRange(this.ValidateTestInjectionPoints());
        }
    }
}
