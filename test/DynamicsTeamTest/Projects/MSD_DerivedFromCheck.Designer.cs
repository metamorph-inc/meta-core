 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public class MSD_DerivedFromCheckFixture : XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("MSD_DerivedFromCheck", "MSD_DerivedFromCheck.xme"); }
        }
    }

    public partial class MSD_DerivedFromCheck : IUseFixture<MSD_DerivedFromCheckFixture>
    {
        internal string mgaFile { get { return this.fixture.mgaFile; } }
        private MSD_DerivedFromCheckFixture fixture { get; set; }

        public void SetFixture(MSD_DerivedFromCheckFixture data)
        {
            this.fixture = data;
        }

        //[Fact]
        //[Trait("Model", "MSD_DerivedFromCheck")]
        //[Trait("ProjectImport/Open", "MSD_DerivedFromCheck")]
        //public void ProjectXmeImport()
        //{
        //    Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
        //}

        [Fact]
        [Trait("Model", "MSD_DerivedFromCheck")]
        [Trait("ProjectImport/Open", "MSD_DerivedFromCheck")]
        public void ProjectMgaOpen()
        {
            var mgaReference = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(mgaReference, "CyPhyML", null);
            project.Close(true);
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }

        [Fact]
        [Trait("Model", "MSD_DerivedFromCheck")]
        [Trait("CyPhy2Modelica", "MSD_DerivedFromCheck")]
        public void TestBenches_MSD()
        {
            string outputDir = "TestBenches_MSD";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@MSD|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_DerivedFromCheck")]
        [Trait("CyPhy2Modelica", "MSD_DerivedFromCheck")]
        public void TestBenches_MSD_Damper_Formula_Shared_Parameter()
        {
            string outputDir = "TestBenches_MSD_Damper_Formula_Shared_Parameter";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@MSD_Damper_Formula_Shared_Parameter|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_DerivedFromCheck")]
        [Trait("CyPhy2Modelica", "MSD_DerivedFromCheck")]
        public void TestBenches_MSD_Damper_External_Property()
        {
            string outputDir = "TestBenches_MSD_Damper_External_Property";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@MSD_Damper_External_Property|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_DerivedFromCheck")]
        [Trait("CyPhy2Modelica", "MSD_DerivedFromCheck")]
        public void TestBenches_MSD_Damper_External_Parameter()
        {
            string outputDir = "TestBenches_MSD_Damper_External_Parameter";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@MSD_Damper_External_Parameter|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_DerivedFromCheck")]
        [Trait("CyPhy2Modelica", "MSD_DerivedFromCheck")]
        public void TestBenches_MSD_Damper_Formula_Parameter()
        {
            string outputDir = "TestBenches_MSD_Damper_Formula_Parameter";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@MSD_Damper_Formula_Parameter|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_DerivedFromCheck")]
        [Trait("CyPhy2Modelica", "MSD_DerivedFromCheck")]
        public void TestBenches_MSD_Damper_Formula_Property()
        {
            string outputDir = "TestBenches_MSD_Damper_Formula_Property";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@MSD_Damper_Formula_Property|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }


    }
}

