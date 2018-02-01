 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public class MSD_PET_RuleFailureFixture : XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("MSD_PET_RuleFailure", "MSD_PET_RuleFailure.xme"); }
        }
    }

    public partial class MSD_PET_RuleFailure : IUseFixture<MSD_PET_RuleFailureFixture>
    {
        internal string mgaFile { get { return this.fixture.mgaFile; } }
        private MSD_PET_RuleFailureFixture fixture { get; set; }

        public void SetFixture(MSD_PET_RuleFailureFixture data)
        {
            this.fixture = data;
        }

        //[Fact]
        //[Trait("Model", "MSD_PET_RuleFailure")]
        //[Trait("ProjectImport/Open", "MSD_PET_RuleFailure")]
        //public void ProjectXmeImport()
        //{
        //    Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
        //}

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("ProjectImport/Open", "MSD_PET_RuleFailure")]
        public void ProjectMgaOpen()
        {
            var mgaReference = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(mgaReference, "CyPhyML", null);
            project.Close(true);
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CyPhy2Modelica", "MSD_PET_RuleFailure")]
        public void RuleFailures_FailureTB_MultipleTBRefs_1()
        {
            string outputDir = "RuleFailures_FailureTB_MultipleTBRefs_1";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RuleFailures|kind=Testing|relpos=0/@FailureTB_MultipleTBRefs_1|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CyPhy2Modelica", "MSD_PET_RuleFailure")]
        public void RuleFailures_PCC_DriverParam2MultipleTBParams()
        {
            string outputDir = "RuleFailures_PCC_DriverParam2MultipleTBParams";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RuleFailures|kind=Testing|relpos=0/@PCC_DriverParam2MultipleTBParams|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CyPhy2Modelica", "MSD_PET_RuleFailure")]
        public void RuleFailures_FailureTB_InvalidType_MobilitySimulation()
        {
            string outputDir = "RuleFailures_FailureTB_InvalidType_MobilitySimulation";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RuleFailures|kind=Testing|relpos=0/@FailureTB_InvalidType_MobilitySimulation|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CyPhy2Modelica", "MSD_PET_RuleFailure")]
        public void RuleFailures_FailureTB_SimpleCalculation()
        {
            string outputDir = "RuleFailures_FailureTB_SimpleCalculation";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RuleFailures|kind=Testing|relpos=0/@FailureTB_SimpleCalculation|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CyPhy2Modelica", "MSD_PET_RuleFailure")]
        public void RuleFailures_FailureTB_MultipleTBRefs_2()
        {
            string outputDir = "RuleFailures_FailureTB_MultipleTBRefs_2";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RuleFailures|kind=Testing|relpos=0/@FailureTB_MultipleTBRefs_2|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void Fail_RuleFailures_CyPhy2ModelicaRulesFailure()
        {
            string outputDir = "RuleFailures_CyPhy2ModelicaRulesFailure";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RuleFailures|kind=Testing|relpos=0/@CyPhy2ModelicaRulesFailure|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CyPhy2Modelica", "MSD_PET_RuleFailure")]
        public void TestBenches_MassSpringDamperTest()
        {
            string outputDir = "Test Benches_MassSpringDamperTest";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@MassSpringDamperTest|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }


        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("PET", "MSD_PET_RuleFailure")]
        public void ResponseSurfaceExample()
        {
            string outputDir = "ResponseSurfaceExample";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@SurrogateModeling|kind=ParametricExplorationFolder|relpos=0/@ResponseSurfaceExample|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("PET", "MSD_PET_RuleFailure")]
        public void LogisticRegressionExample()
        {
            string outputDir = "LogisticRegressionExample";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@SurrogateModeling|kind=ParametricExplorationFolder|relpos=0/@LogisticRegressionExample|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("PET", "MSD_PET_RuleFailure")]
        public void NeuralNetExample()
        {
            string outputDir = "NeuralNetExample";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@SurrogateModeling|kind=ParametricExplorationFolder|relpos=0/@NeuralNetExample|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("PET", "MSD_PET_RuleFailure")]
        public void KrigingExample()
        {
            string outputDir = "KrigingExample";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@SurrogateModeling|kind=ParametricExplorationFolder|relpos=0/@KrigingExample|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("PET", "MSD_PET_RuleFailure")]
        public void UniformExample()
        {
            string outputDir = "UniformExample";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@DOE|kind=ParametricExplorationFolder|relpos=0/@UniformExample|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("PET", "MSD_PET_RuleFailure")]
        public void COBYLAExample()
        {
            string outputDir = "COBYLAExample";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@Optimization|kind=ParametricExplorationFolder|relpos=0/@COBYLAExample|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);
            Assert.True(result, "CyPhyPET failed.");
        }

        //[Fact]
        //[Trait("Model", "MSD_PET_RuleFailure")]
        //[Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        //public void PET_Fail_PCC_SimpleCalculationTB()
        //{
        //    string outputDir = "PCC_SimpleCalculationTB";
        //    string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@PCC_SimpleCalculationTB|kind=ParametricExploration|relpos=0";

        //    Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
        //    bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

        //    Assert.False(result, "CyPhyPET should have failed, but did not.");
        //}

        //[Fact]
        //[Trait("Model", "MSD_PET_RuleFailure")]
        //[Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        //public void PET_Fail_ResponseSurface_SimpleCalculationTB()
        //{
        //    string outputDir = "ResponseSurface_SimpleCalculationTB";
        //    string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@ResponseSurface_SimpleCalculationTB|kind=ParametricExploration|relpos=0";

        //    Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
        //    bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

        //    Assert.False(result, "CyPhyPET should have failed, but did not.");
        //}

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_MultipleDrivers()
        {
            string outputDir = "MultipleDrivers";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@MultipleDrivers|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        public void PET_Fail_NoDriver()
        {
            string outputDir = "NoDriver";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@NoDriver|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET should not have failed");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_MultipleTestBenchRefs()
        {
            string outputDir = "MultipleTestBenchRefs";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@MultipleTestBenchRefs|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_TestBenchRef_Null()
        {
            string outputDir = "TestBenchRef_Null";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@TestBenchRef_Null|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        //[Fact]
        //[Trait("Model", "MSD_PET_RuleFailure")]
        //[Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        //public void PET_Fail_ParameterStudy_InvalidTestBenchType()
        //{
        //    string outputDir = "ParameterStudy_InvalidTestBenchType";
        //    string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@ParameterStudy_InvalidTestBenchType|kind=ParametricExploration|relpos=0";

        //    Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
        //    bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

        //    Assert.False(result, "CyPhyPET should have failed, but did not.");
        //}

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_PCCDriver_noParameters()
        {
            string outputDir = "PCCDriver_noParameters";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@PCCDriver_noParameters|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_PCCDriver_noOutputs()
        {
            string outputDir = "PCCDriver_noOutputs";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@PCCDriver_noOutputs|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_PCCDriver_ParameterNotConnected()
        {
            string outputDir = "PCCDriver_ParameterNotConnected";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@PCCDriver_ParameterNotConnected|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_PCC_MultipleDriverParams2TBParam()
        {
            string outputDir = "PCC_MultipleDriverParams2TBParam";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@PCC_MultipleDriverParams2TBParam|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_PCC_DriverParam2MultipleTBParams()
        {
            string outputDir = "PCC_DriverParam2MultipleTBParams";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@PCC_DriverParam2MultipleTBParams|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_PCCDriver_OutputNotConnected()
        {
            string outputDir = "PCCDriver_OutputNotConnected";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@PCCDriver_OutputNotConnected|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_PCCDriver_TwoMetrics2OneOutput()
        {
            string outputDir = "PCCDriver_TwoMetrics2OneOutput";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@PCCDriver_TwoMetrics2OneOutput|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_PCCDriver_OutputAttributes_MinGreaterThanMax()
        {
            string outputDir = "PCCDriver_OutputAttributes_MinGreaterThanMax";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@PCCDriver_OutputAttributes_MinGreaterThanMax|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_PCCDriver_OutputAttributes_MinIsInvalidString()
        {
            string outputDir = "PCCDriver_OutputAttributes_MinIsInvalidString";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@PCCDriver_OutputAttributes_MinIsInvalidString|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_PCCDriver_OutputAttributes_TargetValueInvalid()
        {
            string outputDir = "PCCDriver_OutputAttributes_TargetValueInvalid";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@PCCDriver_OutputAttributes_TargetValueInvalid|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_PCC_DriverParamAttributes_BetaMinGreaterThanMax()
        {
            string outputDir = "PCC_DriverParamAttributes_BetaMinGreaterThanMax";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@PCC_DriverParamAttributes_BetaMinGreaterThanMax|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_PCC_DriverParamAttributes_UniformParamMinGreaterThanMax()
        {
            string outputDir = "PCC_DriverParamAttributes_UniformParamMinGreaterThanMax";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@PCC_DriverParamAttributes_UniformParamMinGreaterThanMax|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_PCC_DriverParamAttributes_NormStdDevNegative()
        {
            string outputDir = "PCC_DriverParamAttributes_NormStdDevNegative";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@PCC_DriverParamAttributes_NormStdDevNegative|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_MDAO_zeroDesignVariables()
        {
            string outputDir = "MDAO_zeroDesignVariables";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@MDAO_zeroDesignVariables|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_MDAO_DesignVariableInvalidValues()
        {
            string outputDir = "MDAO_DesignVariableInvalidValues";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@MDAO_DesignVariableInvalidValues|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_MDAO_zeroObjectives()
        {
            string outputDir = "MDAO_zeroObjectives";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@MDAO_zeroObjectives|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_MDAO_MissingObjectiveConnection()
        {
            string outputDir = "MDAO_MissingObjectiveConnection";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@MDAO_MissingObjectiveConnection|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_MDAO_MissingDesignVariableConnection()
        {
            string outputDir = "MDAO_MissingDesignVariableConnection";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@MDAO_MissingDesignVariableConnection|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_MDAO_TwoDesignVars2OneParam()
        {
            string outputDir = "MDAO_TwoDesignVars2OneParam";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@MDAO_TwoDesignVars2OneParam|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_MDAO_OneMetric2TwoObjectives()
        {
            string outputDir = "MDAO_OneMetric2TwoObjectives";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@MDAO_OneMetric2TwoObjectives|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_MDAO_OneObjective2TwoMetrics()
        {
            string outputDir = "MDAO_OneObjective2TwoMetrics";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@MDAO_OneObjective2TwoMetrics|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_MDAO_OneDesignVar2TwoParams()
        {
            string outputDir = "MDAO_OneDesignVar2TwoParams";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@MDAO_OneDesignVar2TwoParams|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_Optimizer_ConstraintMinGreaterThanMax()
        {
            string outputDir = "Optimizer_ConstraintMinGreaterThanMax";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@Optimizer_ConstraintMinGreaterThanMax|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_Optimizer_ConstraintValuesInvalid()
        {
            string outputDir = "Optimizer_ConstraintValuesInvalid";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@Optimizer_ConstraintValuesInvalid|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_MDAO_DesignVarMinGreaterThanMax()
        {
            string outputDir = "MDAO_DesignVarMinGreaterThanMax";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@MDAO_DesignVarMinGreaterThanMax|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("CheckerShouldFail", "MSD_PET_RuleFailure")]
        public void PET_Fail_PCC_BrokenTestBench()
        {
            string outputDir = "PCC_BrokenTestBench";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PETRuleFailures|kind=ParametricExplorationFolder|relpos=0/@PCC_BrokenTestBench|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.False(result, "CyPhyPET should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_PET_RuleFailure")]
        [Trait("PCC", "MSD_PET_RuleFailure")]
        public void PCC_for_MSD()
        {
            string outputDir = "PCC_for_MSD";
            string petExperimentPath = "/@Examples|kind=Testing|relpos=0/@PCC|kind=ParametricExplorationFolder|relpos=0/@PCC_for_MSD|kind=ParametricExploration|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhyPETRunner.Run(outputDir, mgaFile, petExperimentPath);

            Assert.True(result, "CyPhyPET failed.");
        }

    }
}

