 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public class WaterPumpFixture : XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("WaterPump", "WaterPump.xme"); }
        }
    }

    public partial class WaterPump : IUseFixture<WaterPumpFixture>
    {
        internal string mgaFile { get { return this.fixture.mgaFile; } }
        private WaterPumpFixture fixture { get; set; }

        public void SetFixture(WaterPumpFixture data)
        {
            this.fixture = data;
        }

        //[Fact]
        //[Trait("Model", "WaterPump")]
        //[Trait("ProjectImport/Open", "WaterPump")]
        //public void ProjectXmeImport()
        //{
        //    Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
        //}

        [Fact]
        [Trait("Model", "WaterPump")]
        [Trait("ProjectImport/Open", "WaterPump")]
        public void ProjectMgaOpen()
        {
            var mgaReference = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(mgaReference, "CyPhyML", null);
            project.Close(true);
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }

        [Fact]
        [Trait("Model", "WaterPump")]
        [Trait("CyPhy2Modelica", "WaterPump")]
        public void TestBenches_Test_Delivered_Electric_Pump_TransportedFluid()
        {
            string outputDir = "TestBenches_Test_Delivered_Electric_Pump_TransportedFluid";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@Test_Delivered_Electric_Pump_TransportedFluid|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "WaterPump")]
        [Trait("CheckerShouldFail", "WaterPump")]
        public void Fail_MediaFails_DifferentMediumInTestComponent()
        {
            string outputDir = "MediaFails_DifferentMediumInTestComponent";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@MediaFails|kind=Testing|relpos=0/@DifferentMediumInTestComponent|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "WaterPump")]
        [Trait("CheckerShouldFail", "WaterPump")]
        public void Fail_MediaFails_PartialMediumTranportedFluidNotDefined()
        {
            string outputDir = "MediaFails_PartialMediumTranportedFluidNotDefined";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@MediaFails|kind=Testing|relpos=0/@PartialMediumTranportedFluidNotDefined|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "WaterPump")]
        [Trait("CheckerShouldFail", "WaterPump")]
        public void Fail_MediaFails_PartialMediumInComponent()
        {
            string outputDir = "MediaFails_PartialMediumInComponent";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@MediaFails|kind=Testing|relpos=0/@PartialMediumInComponent|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "WaterPump")]
        [Trait("CyPhy2Modelica", "WaterPump")]
        public void TestBenches_Test_Delivered_Electric_Pump()
        {
            string outputDir = "TestBenches_Test_Delivered_Electric_Pump";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@Test_Delivered_Electric_Pump|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }


    }
}

