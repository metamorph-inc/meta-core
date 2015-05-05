 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public class MSDVerificationFixture : XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("MSDVerification", "MSDVerification.xme"); }
        }
    }

    public partial class MSDVerification : IUseFixture<MSDVerificationFixture>
    {
        internal string mgaFile { get { return this.fixture.mgaFile; } }
        private MSDVerificationFixture fixture { get; set; }

        public void SetFixture(MSDVerificationFixture data)
        {
            this.fixture = data;
        }

        //[Fact]
        //[Trait("Model", "MSDVerification")]
        //[Trait("ProjectImport/Open", "MSDVerification")]
        //public void ProjectXmeImport()
        //{
        //    Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
        //}

        [Fact]
        [Trait("Model", "MSDVerification")]
        [Trait("ProjectImport/Open", "MSDVerification")]
        public void ProjectMgaOpen()
        {
            var mgaReference = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(mgaReference, "CyPhyML", null);
            project.Close(true);
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }

        [Fact]
        [Trait("Model", "MSDVerification")]
        [Trait("CyPhy2Modelica", "MSDVerification")]
        public void HybridSal_MSD()
        {
            string outputDir = "HybridSal_MSD";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@Verification_CA|kind=Testing|relpos=0/@HybridSal|kind=Testing|relpos=0/@MSD|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed during interpretation with verification workflow.");
        }

        [Fact]
        [Trait("Model", "MSDVerification")]
        [Trait("CyPhy2Modelica", "MSDVerification")]
        public void QR_MSD()
        {
            string outputDir = "QR_MSD";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@Verification_CA|kind=Testing|relpos=0/@QR|kind=Testing|relpos=0/@MSD|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed during interpretation with verification workflow.");
        }

        [Fact]
        [Trait("Model", "MSDVerification")]
        [Trait("CyPhy2Modelica", "MSDVerification")]
        public void TestBenches_MSD_CA()
        {
            string outputDir = "TestBenches_MSD_CA";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@MSD_CA|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }


    }
}

