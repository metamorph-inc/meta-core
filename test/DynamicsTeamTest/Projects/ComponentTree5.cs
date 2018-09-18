
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public class ComponentTree5Fixture : XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("ComponentTree5_Hierarchy", "ComponentTree5.xme"); }
        }
    }

    public class ComponentTree5 : IUseFixture<ComponentTree5Fixture>
    {
        internal string mgaFile { get { return this.fixture.mgaFile; } }
        private ComponentTree5Fixture fixture { get; set; }

        public void SetFixture(ComponentTree5Fixture data)
        {
            this.fixture = data;
        }

        [Fact]
        [Trait("Model", "ComponentTree5")]
        [Trait("ProjectImport/Open", "ComponentTree5")]
        [Trait("CyPhy2Modelica", "OpenModelica_12_ModelChecker")]
        public void ProjectMgaOpen()
        {
            var mgaReference = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(mgaReference, "CyPhyML", null);
            project.Close(true);
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }

        [Fact]
        [Trait("Model", "ComponentTree5")]
        [Trait("CheckerShouldSucceed", "ComponentTree5")]
        [Trait("CyPhy2Modelica", "OpenModelica_12_ModelChecker")]
        public void Succeed_Hierarchy()
        {
            string outputDir = "Testing_RICircuit_CA";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@RICircuit_CA_Hierarchy|kind=TestBench|relpos=0";
            var result = CyPhy2ModelicaRunner.RunWithChecker(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 should have succeeded.");
        }

        [Fact]
        [Trait("Model", "ComponentTree5")]
        [Trait("CheckerShouldSucceed", "ComponentTree5")]
        [Trait("CyPhy2Modelica", "OpenModelica_12_ModelChecker")]
        public void Succeed_Flat()
        {
            string outputDir = "Testing_RICircuit_CA";
            string testBenchPath = "/@Testing|kind=Testing|relpos=0/@RICircuit_CA_Flat|kind=TestBench|relpos=0";
            var result = CyPhy2ModelicaRunner.RunWithChecker(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 should have succeeded.");
        }
    }
}

