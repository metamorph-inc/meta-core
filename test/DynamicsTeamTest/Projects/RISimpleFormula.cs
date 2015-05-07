using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public partial class RISimpleFormula : IUseFixture<RISimpleFormulaFixture>
    {
        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CheckerShouldFail", "RISimpleFormula")]
        public void Fail_ValueFlows_RICircuit_ResistorFail3()
        {
            string outputDir = "ValueFlows_RICircuit_ResistorFail3";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@FailTests|kind=Testing|relpos=0/@ValueFlows|kind=Testing|relpos=0/@RICircuit_ResistorFail3|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.RunMain(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CheckerShouldFail", "RISimpleFormula")]
        public void Fail_ValueFlows_RICircuit_ResistorFail2()
        {
            string outputDir = "ValueFlows_RICircuit_ResistorFail2";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@FailTests|kind=Testing|relpos=0/@ValueFlows|kind=Testing|relpos=0/@RICircuit_ResistorFail2|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.RunMain(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CheckerShouldFail", "RISimpleFormula")]
        public void Fail_ValueFlows_RICircuit_ResistorFail1()
        {
            string outputDir = "ValueFlows_RICircuit_ResistorFail1";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@FailTests|kind=Testing|relpos=0/@ValueFlows|kind=Testing|relpos=0/@RICircuit_ResistorFail1|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.RunMain(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void NonScalarParamsProps_ParamCA()
        {
            string outputDir = "NonScalarParamsProps_ParamCA";
            string testBenchPath = "/@Testing/@NonScalarParamsProps/@ParamCA";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void NonScalarParamsProps_ParamComponent()
        {
            string outputDir = "NonScalarParamsProps_ParamComponent";
            string testBenchPath = "/@Testing/@NonScalarParamsProps/@ParamComponent";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RISimpleFormula")]
        [Trait("CyPhy2Modelica", "RISimpleFormula")]
        public void NonScalarParamsProps_PropCA()
        {
            string outputDir = "NonScalarParamsProps_PropCA";
            string testBenchPath = "/@Testing/@NonScalarParamsProps/@PropCA";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }
    }
}
