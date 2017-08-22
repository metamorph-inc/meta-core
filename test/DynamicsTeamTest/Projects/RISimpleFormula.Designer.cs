 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public class RISimpleFormulaFixture : XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("RISimpleFormula", "RISimpleFormula.xme"); }
        }
    }

    public partial class RISimpleFormula : IUseFixture<RISimpleFormulaFixture>
    {
        internal string mgaFile { get { return this.fixture.mgaFile; } }
        private RISimpleFormulaFixture fixture { get; set; }

        public void SetFixture(RISimpleFormulaFixture data)
        {
            this.fixture = data;
        }

        //[Fact]
        //[Trait("Model", "RISimpleFormula")]
        //[Trait("ProjectImport/Open", "RISimpleFormula")]
        //public void ProjectXmeImport()
        //{
        //    Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
        //}

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("ProjectImport/Open", "RISimpleFormula")]
        public void ProjectMgaOpen()
        {
            var mgaReference = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(mgaReference, "CyPhyML", null);
            project.Close(true);
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void Testing_RICircuit_CA()
        {
            string outputDir = "Testing_RICircuit_CA";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@RICircuit_CA|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void ValidParameterRanges_RICircuit_inf_and_minus_inf()
        {
            string outputDir = "ValidParameterRanges_RICircuit_inf_and_minus_inf";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@ValidParameterRanges|kind=Testing|relpos=0/@RICircuit_inf_and_minus_inf|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void ValidParameterRanges_RICircuit_minus_inf()
        {
            string outputDir = "ValidParameterRanges_RICircuit_minus_inf";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@ValidParameterRanges|kind=Testing|relpos=0/@RICircuit_minus_inf|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void ValidParameterRanges_RICircuit_inf()
        {
            string outputDir = "ValidParameterRanges_RICircuit_inf";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@ValidParameterRanges|kind=Testing|relpos=0/@RICircuit_inf|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void ValidParameterRanges_RICircuit_brackets1()
        {
            string outputDir = "ValidParameterRanges_RICircuit_brackets1";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@ValidParameterRanges|kind=Testing|relpos=0/@RICircuit_brackets1|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void ValidParameterRanges_RICircuit_brackets2()
        {
            string outputDir = "ValidParameterRanges_RICircuit_brackets2";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@ValidParameterRanges|kind=Testing|relpos=0/@RICircuit_brackets2|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void FailTests_RICircuit_GeoMean()
        {
            string outputDir = "FailTests_RICircuit_GeoMean";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@FailTests|kind=Testing|relpos=0/@RICircuit_GeoMean|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CheckerShouldFail", "RISimpleFormula")]
        public void Fail_ParameterRanges_RICircuit_WrongSeparator()
        {
            string outputDir = "ParameterRanges_RICircuit_WrongSeparator";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@FailTests|kind=Testing|relpos=0/@ParameterRanges|kind=Testing|relpos=0/@RICircuit_WrongSeparator|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CheckerShouldFail", "RISimpleFormula")]
        public void Fail_ParameterRanges_RICircuit_TooManySeparatedValues()
        {
            string outputDir = "ParameterRanges_RICircuit_TooManySeparatedValues";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@FailTests|kind=Testing|relpos=0/@ParameterRanges|kind=Testing|relpos=0/@RICircuit_TooManySeparatedValues|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CheckerShouldFail", "RISimpleFormula")]
        public void Fail_ParameterRanges_RICircuit_misplaceBrackets()
        {
            string outputDir = "ParameterRanges_RICircuit_misplaceBrackets";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@FailTests|kind=Testing|relpos=0/@ParameterRanges|kind=Testing|relpos=0/@RICircuit_misplaceBrackets|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CheckerShouldFail", "RISimpleFormula")]
        public void Fail_ParameterRanges_RICircuit_NoneRealInRange()
        {
            string outputDir = "ParameterRanges_RICircuit_NoneRealInRange";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@FailTests|kind=Testing|relpos=0/@ParameterRanges|kind=Testing|relpos=0/@RICircuit_NoneRealInRange|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void ValueFlows_RICircuit_IncomingParameterToProperty()
        {
            string outputDir = "ValueFlows_RICircuit_IncomingParameterToProperty";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@FailTests|kind=Testing|relpos=0/@ValueFlows|kind=Testing|relpos=0/@RICircuit_IncomingParameterToProperty|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void ValueFlows_RICircuit_CustomFormulaInParameterFlow()
        {
            string outputDir = "ValueFlows_RICircuit_CustomFormulaInParameterFlow";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@FailTests|kind=Testing|relpos=0/@ValueFlows|kind=Testing|relpos=0/@RICircuit_CustomFormulaInParameterFlow|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void ValueFlows_RICircuit_IncomingParameterToProperty1()
        {
            string outputDir = "ValueFlows_RICircuit_IncomingParameterToProperty1";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@FailTests|kind=Testing|relpos=0/@ValueFlows|kind=Testing|relpos=0/@RICircuit_IncomingParameterToProperty1|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CheckerShouldFail", "RISimpleFormula")]
        public void Fail_ValueFlows_RICircuit_IncomingMetricToParameterAndProperties()
        {
            string outputDir = "ValueFlows_RICircuit_IncomingMetricToParameterAndProperties";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@FailTests|kind=Testing|relpos=0/@ValueFlows|kind=Testing|relpos=0/@RICircuit_IncomingMetricToParameterAndProperties|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void SimpleFormulas_RICircuit_CustomFormula()
        {
            string outputDir = "SimpleFormulas_RICircuit_CustomFormula";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@SimpleFormulas|kind=Testing|relpos=0/@RICircuit_CustomFormula|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void SimpleFormulas_RICircuit_ArithmeticMean()
        {
            string outputDir = "SimpleFormulas_RICircuit_ArithmeticMean";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@SimpleFormulas|kind=Testing|relpos=0/@RICircuit_ArithmeticMean|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void Multiplication_RICircuit_Product()
        {
            string outputDir = "Multiplication_RICircuit_Product";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@SimpleFormulas|kind=Testing|relpos=0/@Multiplication|kind=Testing|relpos=0/@RICircuit_Product|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void MinMax_RICircuit_Min()
        {
            string outputDir = "MinMax_RICircuit_Min";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@SimpleFormulas|kind=Testing|relpos=0/@MinMax|kind=Testing|relpos=0/@RICircuit_Min|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void MinMax_RICircuit_OneMin()
        {
            string outputDir = "MinMax_RICircuit_OneMin";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@SimpleFormulas|kind=Testing|relpos=0/@MinMax|kind=Testing|relpos=0/@RICircuit_OneMin|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void MinMax_RICircuit_Max()
        {
            string outputDir = "MinMax_RICircuit_Max";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@SimpleFormulas|kind=Testing|relpos=0/@MinMax|kind=Testing|relpos=0/@RICircuit_Max|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void Addition_RICircuit_TestBench()
        {
            string outputDir = "Addition_RICircuit_TestBench";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@SimpleFormulas|kind=Testing|relpos=0/@Addition|kind=Testing|relpos=0/@RICircuit_TestBench|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void Addition_RICircuit_NestedFormulas()
        {
            string outputDir = "Addition_RICircuit_NestedFormulas";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@SimpleFormulas|kind=Testing|relpos=0/@Addition|kind=Testing|relpos=0/@RICircuit_NestedFormulas|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void Addition_RICircuit_OutgoingParameters()
        {
            string outputDir = "Addition_RICircuit_OutgoingParameters";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@SimpleFormulas|kind=Testing|relpos=0/@Addition|kind=Testing|relpos=0/@RICircuit_OutgoingParameters|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void Addition_RICircuit_PassingBetweenComponents()
        {
            string outputDir = "Addition_RICircuit_PassingBetweenComponents";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@SimpleFormulas|kind=Testing|relpos=0/@Addition|kind=Testing|relpos=0/@RICircuit_PassingBetweenComponents|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void Addition_RICircuit_TestBench_IntermediateParameter()
        {
            string outputDir = "Addition_RICircuit_TestBench_IntermediateParameter";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@SimpleFormulas|kind=Testing|relpos=0/@Addition|kind=Testing|relpos=0/@RICircuit_TestBench_IntermediateParameter|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }


        [Fact(Skip = "PCC is unimplemented")]
        [Trait("Model", "RISimpleFormula")]
        [Trait("PCC", "RISimpleFormula")]
        public void RI_PCC_CA()
        {
            string outputDir = "RI_PCC_CA";
            string petExperimentPath = "/@Testing|kind=Testing|relpos=0/@PCC|kind=ParametricExplorationFolder|relpos=0/@RI_PCC_CA|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

    }
}

