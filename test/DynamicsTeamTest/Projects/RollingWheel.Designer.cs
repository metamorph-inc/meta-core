 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public class RollingWheelFixture : XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("RollingWheel", "RollingWheel.xme"); }
        }
    }

    public partial class RollingWheel : IUseFixture<RollingWheelFixture>
    {
        internal string mgaFile { get { return this.fixture.mgaFile; } }
        private RollingWheelFixture fixture { get; set; }

        public void SetFixture(RollingWheelFixture data)
        {
            this.fixture = data;
        }

        //[Fact]
        //[Trait("Model", "RollingWheel")]
        //[Trait("ProjectImport/Open", "RollingWheel")]
        //public void ProjectXmeImport()
        //{
        //    Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
        //}

        [Fact]
        [Trait("Model", "RollingWheel")]
        [Trait("ProjectImport/Open", "RollingWheel")]
        public void ProjectMgaOpen()
        {
            var mgaReference = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(mgaReference, "CyPhyML", null);
            project.Close(true);
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }

        [Fact]
        [Trait("Model", "RollingWheel")]
        [Trait("CyPhy2Modelica", "RollingWheel")]
        public void TestBenches_RollingWheelBooleanParameterInTestComponent()
        {
            string outputDir = "TestBenches_RollingWheelBooleanParameterInTestComponent";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@RollingWheelBooleanParameterInTestComponent|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RollingWheel")]
        [Trait("CyPhy2Modelica", "RollingWheel")]
        public void TestBenches_RollingWheelBooleanParameterInComponent()
        {
            string outputDir = "TestBenches_RollingWheelBooleanParameterInComponent";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@RollingWheelBooleanParameterInComponent|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RollingWheel")]
        [Trait("CyPhy2Modelica", "RollingWheel")]
        public void TestBenches_RollingWheel()
        {
            string outputDir = "TestBenches_RollingWheel";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@RollingWheel|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RollingWheel")]
        [Trait("CyPhy2Modelica", "RollingWheel")]
        [Trait("CyPhy2Modelica", "OpenModelica_12_ModelChecker")]
        public void TestBenches_RollingWheel_WithChecker()
        {
            string outputDir = "TestBenches_RollingWheel";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@RollingWheel|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.RunWithChecker(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RollingWheel")]
        [Trait("CyPhy2Modelica", "RollingWheel")]
        public void TestBenches_RollingWheelDummyPropertiesInTestBench()
        {
            string outputDir = "TestBenches_RollingWheelDummyPropertiesInTestBench";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@RollingWheelDummyPropertiesInTestBench|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }


    }
}

