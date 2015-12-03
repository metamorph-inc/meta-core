using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhyMasterInterpreter.Rules
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;


    public class StructuralFEATestBenchChecker : TestBenchTypeChecker<CyPhy.CADTestBench>
    {
        // CAD = Structural FEA
        public StructuralFEATestBenchChecker(CyPhy.CADTestBench testBench)
            : base(testBench)
        {
        }

        public override void CheckNoThrow()
        {
            base.CheckNoThrow();

            this.m_details.AddRange(this.AtLeastOneTestInjectionPoints());
            this.m_details.AddRange(this.FaceConstructMustBeAbaqusModelBased());
        }

        protected IEnumerable<ContextCheckerResult> FaceConstructMustBeAbaqusModelBased()
        {
            List<ContextCheckerResult> results = new List<ContextCheckerResult>();
            if (this.testBench.Children.FaceCollection.Any())
            {
                switch (this.testBench.Attributes.SolverType)
                {
                    case CyPhyClasses.CADTestBench.AttributesClass.SolverType_enum.ABAQUS_Model_Based:
                        foreach (var face in this.testBench.Children.FaceCollection)
                        {
                            var feedback = new ContextCheckerResult()
                            {
                                Success = true,
                                Subject = face.Impl,
                                Message = string.Format("Face construct allowed for solver type ABAQUS model based.")
                            };

                            results.Add(feedback);
                        }

                        break;
                    case CyPhyClasses.CADTestBench.AttributesClass.SolverType_enum.ABAQUS_Deck_Based:
                        break;
                    case CyPhyClasses.CADTestBench.AttributesClass.SolverType_enum.NASTRAN:
                        break;
                }
            }

            return results;
        }
    }
}
