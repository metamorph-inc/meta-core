 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public class MSD_PETFixture : XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("MSD_PET", "MSD_PET.xme"); }
        }
    }

    public partial class MSD_PET : IUseFixture<MSD_PETFixture>
    {
        internal string mgaFile { get { return this.fixture.mgaFile; } }
        private MSD_PETFixture fixture { get; set; }

        public void SetFixture(MSD_PETFixture data)
        {
            this.fixture = data;
        }

        //[Fact]
        //[Trait("Model", "MSD_PET")]
        //[Trait("ProjectImport/Open", "MSD_PET")]
        //public void ProjectXmeImport()
        //{
        //    Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
        //}

        [Fact]
        [Trait("Model", "MSD_PET")]
        [Trait("ProjectImport/Open", "MSD_PET")]
        public void ProjectMgaOpen()
        {
            var mgaReference = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(mgaReference, "CyPhyML", null);
            project.Close(true);
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }

        [Fact]
        [Trait("Model", "MSD_PET")]
        [Trait("CyPhy2Modelica", "MSD_PET")]
        public void TestBenches_MassSpringDamperTest()
        {
            string outputDir = "Test Benches_MassSpringDamperTest";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@MassSpringDamperTest|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhyPET failed.");
        }


        [Fact(Skip="surrogates are unimplemented")]
        [Trait("Model", "MSD_PET")]
        [Trait("PET", "MSD_PET")]
        public void ResponseSurfaceExample()
        {
            string outputDir = "ResponseSurfaceExample";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@SurrogateModeling|kind=ParametricExplorationFolder|relpos=0/@ResponseSurfaceExample|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact(Skip="surrogates are unimplemented")]
        [Trait("Model", "MSD_PET")]
        [Trait("PET", "MSD_PET")]
        public void LogisticRegressionExample()
        {
            string outputDir = "LogisticRegressionExample";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@SurrogateModeling|kind=ParametricExplorationFolder|relpos=0/@LogisticRegressionExample|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact(Skip="surrogates are unimplemented")]
        [Trait("Model", "MSD_PET")]
        [Trait("PET", "MSD_PET")]
        public void NeuralNetExample()
        {
            string outputDir = "NeuralNetExample";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@SurrogateModeling|kind=ParametricExplorationFolder|relpos=0/@NeuralNetExample|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET")]
        [Trait("PET", "MSD_PET")]
        public void KrigingExample()
        {
            string outputDir = "KrigingExample";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@SurrogateModeling|kind=ParametricExplorationFolder|relpos=0/@KrigingExample|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET")]
        [Trait("PET", "MSD_PET")]
        public void CentralCompositeExample()
        {
            string outputDir = "CentralCompositeExample";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@DOE|kind=ParametricExplorationFolder|relpos=0/@CentralCompositeExample|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET")]
        [Trait("PET", "MSD_PET")]
        public void FullFactorialExample()
        {
            string outputDir = "FullFactorialExample";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@DOE|kind=ParametricExplorationFolder|relpos=0/@FullFactorialExample|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET")]
        [Trait("PET", "MSD_PET")]
        public void LatinHypercubeExample()
        {
            string outputDir = "LatinHypercubeExample";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@DOE|kind=ParametricExplorationFolder|relpos=0/@LatinHypercubeExample|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET")]
        [Trait("PET", "MSD_PET")]
        public void UniformExample()
        {
            string outputDir = "UniformExample";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@DOE|kind=ParametricExplorationFolder|relpos=0/@UniformExample|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET")]
        [Trait("PET", "MSD_PET")]
        public void CONMINExample()
        {
            string outputDir = "CONMINExample";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@Optimization|kind=ParametricExplorationFolder|relpos=0/@CONMINExample|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET")]
        [Trait("PET", "MSD_PET")]
        public void COBYLAExample()
        {
            string outputDir = "COBYLAExample";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@Optimization|kind=ParametricExplorationFolder|relpos=0/@COBYLAExample|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET")]
        [Trait("PET", "MSD_PET")]
        public void NEWSUMTExample()
        {
            string outputDir = "NEWSUMTExample";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@Optimization|kind=ParametricExplorationFolder|relpos=0/@NEWSUMTExample|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

    }
}

