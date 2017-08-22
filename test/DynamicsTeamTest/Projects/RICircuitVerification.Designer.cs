 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public class RICircuitVerificationFixture
    {
        internal string mgaFile = null;

        public RICircuitVerificationFixture()
        {
            this.mgaFile = Test.ImportXME2Mga("RICircuitVerification", "RICircuitVerification.xme");
        }
    }

    public partial class RICircuitVerification : IUseFixture<RICircuitVerificationFixture>
    {
        internal string mgaFile { get { return this.fixture.mgaFile; } }
        private RICircuitVerificationFixture fixture { get; set; }

        public void SetFixture(RICircuitVerificationFixture data)
        {
            this.fixture = data;
        }

        [Fact]
        [Trait("Model", "RICircuitVerification")]
        [Trait("ProjectImport/Open", "RICircuitVerification")]
        public void ProjectXmeImport()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
        }

        [Fact]
        [Trait("Model", "RICircuitVerification")]
        [Trait("ProjectImport/Open", "RICircuitVerification")]
        public void ProjectMgaOpen()
        {
            var mgaReference = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(mgaReference, "CyPhyML", null);
            project.Close(true);
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }

        [Fact]
        [Trait("Model", "RICircuitVerification")]
        [Trait("CyPhy2Modelica", "RICircuitVerification")]
        public void Testing_RICircuit_CA()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run("Testing_RICircuit_CA", mgaFile,
                    "/@Testing|kind=Testing|relpos=0/@RICircuit_CA|kind=TestBench|relpos=0");

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "RICircuitVerification")]
        [Trait("CyPhy2Modelica", "RICircuitVerification")]
        public void HybridSal_RICircuit()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run("HybridSal_RICircuit", mgaFile,
                    "/@Testing|kind=Testing|relpos=0/@Verification_CA|kind=Testing|relpos=0/@HybridSal|kind=Testing|relpos=0/@RICircuit|kind=TestBench|relpos=0");

            Assert.True(result, "CyPhy2Modelica_v2 failed during interpretation with verification workflow.");
        }
    }
}

