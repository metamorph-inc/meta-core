 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public class DriveLine_v3Fixture : XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("DriveLine_v3", "DriveLine_v3.xme"); }
        }
    }

    public partial class DriveLine_v3 : IUseFixture<DriveLine_v3Fixture>
    {
        internal string mgaFile { get { return this.fixture.mgaFile; } }
        private DriveLine_v3Fixture fixture { get; set; }

        public void SetFixture(DriveLine_v3Fixture data)
        {
            this.fixture = data;
        }

        //[Fact]
        //[Trait("Model", "DriveLine_v3")]
        //[Trait("ProjectImport/Open", "DriveLine_v3")]
        //public void ProjectXmeImport()
        //{
        //    Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
        //}

        [Fact]
        [Trait("Model", "DriveLine_v3")]
        [Trait("ProjectImport/Open", "DriveLine_v3")]
        public void ProjectMgaOpen()
        {
            var mgaReference = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(mgaReference, "CyPhyML", null);
            project.Close(true);
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }

        [Fact]
        [Trait("Model", "DriveLine_v3")]
        [Trait("CheckerShouldFail", "DriveLine_v3")]
        public void Fail_CAD_CAD()
        {
            string outputDir = "CAD_CAD";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@CAD|kind=Testing|relpos=0/@CAD|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = false;
            result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "DriveLine_v3")]
        [Trait("CyPhy2Modelica", "DriveLine_v3")]
        public void Dynamics_FullSpeedForward1Metric_cfg1()
        {
            string outputDir = "Dynamics_FullSpeedForward1Metric_cfg1";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@Dynamics|kind=Testing|relpos=0/@FullSpeedForward1Metric_cfg1|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = false;
            result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "DriveLine_v3")]
        [Trait("CyPhy2Modelica", "DriveLine_v3")]
        public void Dynamics_FullSpeedForward_cfg1()
        {
            string outputDir = "Dynamics_FullSpeedForward_cfg1";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@Dynamics|kind=Testing|relpos=0/@FullSpeedForward_cfg1|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = false;
            result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }


        [Fact(Skip = "PCC is unimplemented")]
        [Trait("Model", "DriveLine_v3")]
        [Trait("PCC", "DriveLine_v3")]
        public void FullSpeedForwardMaxSpeed_cfg1()
        {
            string outputDir = "FullSpeedForwardMaxSpeed_cfg1";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@Dynamics|kind=Testing|relpos=0/@PCC|kind=ParametricExplorationFolder|relpos=0/@FullSpeedForwardMaxSpeed_cfg1|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = false;
            result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

    }
}

