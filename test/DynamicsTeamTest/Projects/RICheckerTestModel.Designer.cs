 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public class RICheckerTestModelFixture : XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("RICheckerTestModel", "RICheckerTestModel.xme"); }
        }
    }

    public partial class RICheckerTestModel : IUseFixture<RICheckerTestModelFixture>
    {
        internal string mgaFile { get { return this.fixture.mgaFile; } }
        private RICheckerTestModelFixture fixture { get; set; }

        public void SetFixture(RICheckerTestModelFixture data)
        {
            this.fixture = data;
        }

        //[Fact]
        //[Trait("Model", "RICheckerTestModel")]
        //[Trait("ProjectImport/Open", "RICheckerTestModel")]
        //public void ProjectXmeImport()
        //{
        //    Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
        //}

        [Fact]
        [Trait("Model", "RICheckerTestModel")]
        [Trait("ProjectImport/Open", "RICheckerTestModel")]
        public void ProjectMgaOpen()
        {
            var mgaReference = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(mgaReference, "CyPhyML", null);
            project.Close(true);
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }

        [Fact]
        [Trait("Model", "RICheckerTestModel")]
        [Trait("CheckerShouldFail", "RICheckerTestModel")]
        public void Fail_CheckerTests_CA_RICircuit_Missing_Uri_In_Component()
        {
            string outputDir = "CheckerTests_CA_RICircuit_Missing_Uri_In_Component";
            string testBenchPath = "/@Tests|kind=Testing|relpos=0/@CheckerTests_CA|kind=Testing|relpos=0/@RICircuit_Missing_Uri_In_Component|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RICheckerTestModel")]
        [Trait("CheckerShouldFail", "RICheckerTestModel")]
        public void Fail_CheckerTests_CA_RICircuit_Missing_Uri_In_TestComponent()
        {
            string outputDir = "CheckerTests_CA_RICircuit_Missing_Uri_In_TestComponent";
            string testBenchPath = "/@Tests|kind=Testing|relpos=0/@CheckerTests_CA|kind=Testing|relpos=0/@RICircuit_Missing_Uri_In_TestComponent|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RICheckerTestModel")]
        [Trait("CheckerShouldFail", "RICheckerTestModel")]
        public void Fail_CheckerTests_CA_RICircuit_ModelicaConnector_in_CA_connected_to_Connector()
        {
            string outputDir = "CheckerTests_CA_RICircuit_ModelicaConnector_in_CA_connected_to_Connector";
            string testBenchPath = "/@Tests|kind=Testing|relpos=0/@CheckerTests_CA|kind=Testing|relpos=0/@RICircuit_ModelicaConnector_in_CA_connected_to_Connector|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RICheckerTestModel")]
        [Trait("CheckerShouldFail", "RICheckerTestModel")]
        public void Fail_CheckerTests_CA_RICircuit_CA_RegularModelicaConnectorsNotConnectedInComponent()
        {
            string outputDir = "CheckerTests_CA_RICircuit_CA_RegularModelicaConnectorsNotConnectedInComponent";
            string testBenchPath = "/@Tests|kind=Testing|relpos=0/@CheckerTests_CA|kind=Testing|relpos=0/@RICircuit_CA_RegularModelicaConnectorsNotConnectedInComponent|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RICheckerTestModel")]
        [Trait("CyPhy2Modelica", "RICheckerTestModel")]
        public void CheckerTests_CA_RICircuit_ConnectorNotConnectedInComponent()
        {
            string outputDir = "CheckerTests_CA_RICircuit_ConnectorNotConnectedInComponent";
            string testBenchPath = "/@Tests|kind=Testing|relpos=0/@CheckerTests_CA|kind=Testing|relpos=0/@RICircuit_ConnectorNotConnectedInComponent|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RICheckerTestModel")]
        [Trait("CheckerShouldFail", "RICheckerTestModel")]
        public void Fail_CheckerTests_CA_RICircuit_With_Space_in_Name()
        {
            string outputDir = "CheckerTests_CA_RICircuit With Space in Name";
            string testBenchPath = "/@Tests|kind=Testing|relpos=0/@CheckerTests_CA|kind=Testing|relpos=0/@RICircuit With Space in Name|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RICheckerTestModel")]
        [Trait("CheckerShouldFail", "RICheckerTestModel")]
        public void Fail_CheckerTests_CA_()
        {
            string outputDir = "CheckerTests_CA_";
            string testBenchPath = "/@Tests|kind=Testing|relpos=0/@CheckerTests_CA|kind=Testing|relpos=0/@|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RICheckerTestModel")]
        [Trait("CheckerShouldFail", "RICheckerTestModel")]
        public void Fail_CheckerTests_CA_1TB_Starts_with_character()
        {
            string outputDir = "CheckerTests_CA_1TB_Starts_with_character";
            string testBenchPath = "/@Tests|kind=Testing|relpos=0/@CheckerTests_CA|kind=Testing|relpos=0/@1TB_Starts_with_character|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RICheckerTestModel")]
        [Trait("CheckerShouldFail", "RICheckerTestModel")]
        public void Fail_CheckerTests_CA_for()
        {
            string outputDir = "CheckerTests_CA_for";
            string testBenchPath = "/@Tests|kind=Testing|relpos=0/@CheckerTests_CA|kind=Testing|relpos=0/@for|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RICheckerTestModel")]
        [Trait("CheckerShouldFail", "RICheckerTestModel")]
        public void Fail_CheckerTests_CA_RICircuit_CA_SolverSettingNegativeIntervalLength()
        {
            string outputDir = "CheckerTests_CA_RICircuit_CA_SolverSettingNegativeIntervalLength";
            string testBenchPath = "/@Tests|kind=Testing|relpos=0/@CheckerTests_CA|kind=Testing|relpos=0/@RICircuit_CA_SolverSettingNegativeIntervalLength|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RICheckerTestModel")]
        [Trait("CheckerShouldFail", "RICheckerTestModel")]
        public void Fail_CheckerTests_CA_RICircuit_CA_SolverSettingNegativeTolerance()
        {
            string outputDir = "CheckerTests_CA_RICircuit_CA_SolverSettingNegativeTolerance";
            string testBenchPath = "/@Tests|kind=Testing|relpos=0/@CheckerTests_CA|kind=Testing|relpos=0/@RICircuit_CA_SolverSettingNegativeTolerance|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RICheckerTestModel")]
        [Trait("CheckerShouldFail", "RICheckerTestModel")]
        public void Fail_CheckerTests_CA_RICircuit_CA_SolverSettingNegativeNbrOfIntervals()
        {
            string outputDir = "CheckerTests_CA_RICircuit_CA_SolverSettingNegativeNbrOfIntervals";
            string testBenchPath = "/@Tests|kind=Testing|relpos=0/@CheckerTests_CA|kind=Testing|relpos=0/@RICircuit_CA_SolverSettingNegativeNbrOfIntervals|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RICheckerTestModel")]
        [Trait("CyPhy2Modelica", "RICheckerTestModel")]
        public void Tests_RICircuit_CA()
        {
            string outputDir = "Tests_RICircuit_CA";
            string testBenchPath = "/@Tests|kind=Testing|relpos=0/@RICircuit_CA|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }


    }
}

