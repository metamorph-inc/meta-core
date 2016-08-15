 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public class MSD_PCCFixture : XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("MSD_PCC", "MSD_PCC.xme"); }
        }
    }

    public partial class MSD_PCC : IUseFixture<MSD_PCCFixture>
    {
        public string mgaFile { get { return this.fixture.mgaFile; } }
        private MSD_PCCFixture fixture { get; set; }

        public void SetFixture(MSD_PCCFixture data)
        {
            this.fixture = data;
        }

        //[Fact]
        //[Trait("Model", "MSD_PCC")]
        //[Trait("ProjectImport/Open", "MSD_PCC")]
        //public void ProjectXmeImport()
        //{
        //    Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
        //}

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("ProjectImport/Open", "MSD_PCC")]
        public void ProjectMgaOpen()
        {
            var mgaReference = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(mgaReference, "CyPhyML", null);
            project.Close(true);
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("CyPhy2Modelica", "MSD_PCC")]
        public void TestingParameterAndMetricMappings_MSD_for_testing()
        {
            string outputDir = "TestingParameterAndMetricMappings_MSD_for_testing";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@SingleConfig|kind=Testing|relpos=0/@TestingParameterAndMetricMappings|kind=Testing|relpos=0/@MSD_for_testing|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("CyPhy2Modelica", "MSD_PCC")]
        public void SingleConfig_MSD_dymola_CA()
        {
            string outputDir = "SingleConfig_MSD_dymola_CA";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@SingleConfig|kind=Testing|relpos=0/@MSD_dymola_CA|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("CyPhy2Modelica", "MSD_PCC")]
        public void SingleConfig_MSD_om_CA()
        {
            string outputDir = "SingleConfig_MSD_om_CA";
            string testBenchPath = "/@TestBenches|kind=Testing|relpos=0/@SingleConfig|kind=Testing|relpos=0/@MSD_om_CA|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }


        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("PCC", "MSD_PCC")]
        public void PCC_MSD_dymola_CA()
        {
            string outputDir = "PCC_MSD_dymola_CA";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@SingleConfig|kind=Testing|relpos=0/@PCC_CA|kind=ParametricExplorationFolder|relpos=0/@PCC_MSD_dymola_CA|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("PCC", "MSD_PCC")]
        public void PCC_MSD_om_CA()
        {
            string outputDir = "PCC_MSD_om_CA";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@SingleConfig|kind=Testing|relpos=0/@PCC_CA|kind=ParametricExplorationFolder|relpos=0/@PCC_MSD_om_CA|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("PCC", "MSD_PCC")]
        public void PCC2Params1Output()
        {
            string outputDir = "PCC2Params1Output";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@SingleConfig|kind=Testing|relpos=0/@TestingParameterAndMetricMappings|kind=Testing|relpos=0/@PCCTests|kind=ParametricExplorationFolder|relpos=0/@PCC2Params1Output|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("PCC", "MSD_PCC")]
        public void PCC1Param2Outputs()
        {
            string outputDir = "PCC1Param2Outputs";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@SingleConfig|kind=Testing|relpos=0/@TestingParameterAndMetricMappings|kind=Testing|relpos=0/@PCCTests|kind=ParametricExplorationFolder|relpos=0/@PCC1Param2Outputs|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("PCC", "MSD_PCC")]
        public void PCCBijection()
        {
            string outputDir = "PCCBijection";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@SingleConfig|kind=Testing|relpos=0/@TestingParameterAndMetricMappings|kind=Testing|relpos=0/@PCCTests|kind=ParametricExplorationFolder|relpos=0/@PCCBijection|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("CheckerShouldFail", "MSD_PCC")]
        public void PET_Fail_Opt1Param2Outputs()
        {
            string outputDir = "Opt1Param2Outputs";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@SingleConfig|kind=Testing|relpos=0/@TestingParameterAndMetricMappings|kind=Testing|relpos=0/@PETTests|kind=ParametricExplorationFolder|relpos=0/@Optimizer|kind=ParametricExplorationFolder|relpos=0/@Opt1Param2Outputs|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("PET", "MSD_PCC")]
        public void Opt2Param1Output()
        {
            string outputDir = "Opt2Param1Output";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@SingleConfig|kind=Testing|relpos=0/@TestingParameterAndMetricMappings|kind=Testing|relpos=0/@PETTests|kind=ParametricExplorationFolder|relpos=0/@Optimizer|kind=ParametricExplorationFolder|relpos=0/@Opt2Param1Output|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("PET", "MSD_PCC")]
        public void PS1Param1Output()
        {
            string outputDir = "PS1Param1Output";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@SingleConfig|kind=Testing|relpos=0/@TestingParameterAndMetricMappings|kind=Testing|relpos=0/@PETTests|kind=ParametricExplorationFolder|relpos=0/@ParameterStudy|kind=ParametricExplorationFolder|relpos=0/@PS1Param1Output|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact(Skip="surrogates are unimplemented")]
        [Trait("Model", "MSD_PCC")]
        [Trait("PET", "MSD_PCC")]
        public void PSRS1Param1Output()
        {
            string outputDir = "PSRS1Param1Output";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@SingleConfig|kind=Testing|relpos=0/@TestingParameterAndMetricMappings|kind=Testing|relpos=0/@PETTests|kind=ParametricExplorationFolder|relpos=0/@ParameterStudy|kind=ParametricExplorationFolder|relpos=0/@PSRS1Param1Output|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("PCC", "MSD_PCC")]
        public void BETA_UP_UDR()
        {
            string outputDir = "BETA_UP_UDR";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@FailingModels|kind=Testing|relpos=0/@FixesNeededInPCCModule|kind=ParametricExplorationFolder|relpos=0/@BETA_UP_UDR|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("PCC", "MSD_PCC")]
        public void BETA_UP_FFNI()
        {
            string outputDir = "BETA_UP_FFNI";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@FailingModels|kind=Testing|relpos=0/@FixesNeededInPCCModule|kind=ParametricExplorationFolder|relpos=0/@BETA_UP_FFNI|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("CheckerShouldFail", "MSD_PCC")]
        public void PET_Fail_Uniform_limits_same()
        {
            string outputDir = "Uniform_limits_same";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@FailingModels|kind=Testing|relpos=0/@InterpreterShouldFail|kind=ParametricExplorationFolder|relpos=0/@Uniform_limits_same|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("CheckerShouldFail", "MSD_PCC")]
        public void PET_Fail_BETA_beta_zero()
        {
            string outputDir = "BETA_beta_zero";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@FailingModels|kind=Testing|relpos=0/@InterpreterShouldFail|kind=ParametricExplorationFolder|relpos=0/@BETA_beta_zero|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("CheckerShouldFail", "MSD_PCC")]
        public void PET_Fail_BETA_limits_same()
        {
            string outputDir = "BETA_limits_same";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@FailingModels|kind=Testing|relpos=0/@InterpreterShouldFail|kind=ParametricExplorationFolder|relpos=0/@BETA_limits_same|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("CheckerShouldFail", "MSD_PCC")]
        public void PET_Fail_BETA_alph_neg()
        {
            string outputDir = "BETA_alph_neg";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@FailingModels|kind=Testing|relpos=0/@InterpreterShouldFail|kind=ParametricExplorationFolder|relpos=0/@BETA_alph_neg|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("CheckerShouldFail", "MSD_PCC")]
        public void PET_Fail_BETA_alpha_zero()
        {
            string outputDir = "BETA_alpha_zero";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@FailingModels|kind=Testing|relpos=0/@InterpreterShouldFail|kind=ParametricExplorationFolder|relpos=0/@BETA_alpha_zero|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("CheckerShouldFail", "MSD_PCC")]
        public void PET_Fail_BETA_beta_neg()
        {
            string outputDir = "BETA_beta_neg";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@FailingModels|kind=Testing|relpos=0/@InterpreterShouldFail|kind=ParametricExplorationFolder|relpos=0/@BETA_beta_neg|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("CheckerShouldFail", "MSD_PCC")]
        public void PET_Fail_LogNormal_shape_zero()
        {
            string outputDir = "LogNormal_shape_zero";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@FailingModels|kind=Testing|relpos=0/@InterpreterShouldFail|kind=ParametricExplorationFolder|relpos=0/@LogNormal_shape_zero|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("PCC", "MSD_PCC")]
        public void LogNormal_UP_PCE()
        {
            string outputDir = "LogNormal_UP_PCE";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@FailingModels|kind=Testing|relpos=0/@ReferenceExperiments|kind=ParametricExplorationFolder|relpos=0/@LogNormal_UP_PCE|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("PCC", "MSD_PCC")]
        public void LogNormal_UP_MPP()
        {
            string outputDir = "LogNormal_UP_MPP";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@FailingModels|kind=Testing|relpos=0/@ReferenceExperiments|kind=ParametricExplorationFolder|relpos=0/@LogNormal_UP_MPP|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("PCC", "MSD_PCC")]
        public void LogNormal_UP_TS()
        {
            string outputDir = "LogNormal_UP_TS";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@FailingModels|kind=Testing|relpos=0/@ReferenceExperiments|kind=ParametricExplorationFolder|relpos=0/@LogNormal_UP_TS|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("PCC", "MSD_PCC")]
        public void LogNormal_UP_FFNI()
        {
            string outputDir = "LogNormal_UP_FFNI";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@FailingModels|kind=Testing|relpos=0/@ReferenceExperiments|kind=ParametricExplorationFolder|relpos=0/@LogNormal_UP_FFNI|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("PCC", "MSD_PCC")]
        public void LogNormal_UP_UDR()
        {
            string outputDir = "LogNormal_UP_UDR";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@FailingModels|kind=Testing|relpos=0/@ReferenceExperiments|kind=ParametricExplorationFolder|relpos=0/@LogNormal_UP_UDR|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC")]
        [Trait("PCC", "MSD_PCC")]
        public void BETA_UP_TS()
        {
            string outputDir = "BETA_UP_TS";
            string petExperimentPath = "/@TestBenches|kind=Testing|relpos=0/@FailingModels|kind=Testing|relpos=0/@ReferenceExperiments|kind=ParametricExplorationFolder|relpos=0/@BETA_UP_TS|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

    }
}

