 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public class MSD_FailCheckerFixture : XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("MSD_FailChecker", "MSD_FailChecker.xme"); }
        }
    }

    public partial class MSD_FailChecker : IUseFixture<MSD_FailCheckerFixture>
    {
        internal string mgaFile { get { return this.fixture.mgaFile; } }
        private MSD_FailCheckerFixture fixture { get; set; }

        public void SetFixture(MSD_FailCheckerFixture data)
        {
            this.fixture = data;
        }

        //[Fact]
        //[Trait("Model", "MSD_FailChecker")]
        //[Trait("ProjectImport/Open", "MSD_FailChecker")]
        //public void ProjectXmeImport()
        //{
        //    Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
        //}

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("ProjectImport/Open", "MSD_FailChecker")]
        public void ProjectMgaOpen()
        {
            var mgaReference = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(mgaReference, "CyPhyML", null);
            project.Close(true);
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_PropertyParameter_Testbench_MSD_DamperParameterHasNoValueNoConnection()
        {
            string outputDir = "PropertyParameter_Testbench_MSD_DamperParameterHasNoValueNoConnection";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@PropertyParameter|kind=Testing|relpos=0/@Testbench_MSD_DamperParameterHasNoValueNoConnection|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_PropertyParameter_Testbench_TestBenchPropertyHasNoValue()
        {
            string outputDir = "PropertyParameter_Testbench_TestBenchPropertyHasNoValue";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@PropertyParameter|kind=Testing|relpos=0/@Testbench_TestBenchPropertyHasNoValue|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_PropertyParameter_Testbench_TestBenchParamHasNoValue()
        {
            string outputDir = "PropertyParameter_Testbench_TestBenchParamHasNoValue";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@PropertyParameter|kind=Testing|relpos=0/@Testbench_TestBenchParamHasNoValue|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_ModelicaConnector_Testbench_FloatingModelicaConnector_damper()
        {
            string outputDir = "ModelicaConnector_Testbench_FloatingModelicaConnector_damper";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@ModelicaConnector|kind=Testing|relpos=0/@Testbench_FloatingModelicaConnector_damper|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_ModelicaConnector_Testbench_FloatingModelicaConnector_CA()
        {
            string outputDir = "ModelicaConnector_Testbench_FloatingModelicaConnector_CA";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@ModelicaConnector|kind=Testing|relpos=0/@Testbench_FloatingModelicaConnector_CA|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_ModelicaConnector_Testbench_FloatingModelicaConnector_TestComponent()
        {
            string outputDir = "ModelicaConnector_Testbench_FloatingModelicaConnector_TestComponent";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@ModelicaConnector|kind=Testing|relpos=0/@Testbench_FloatingModelicaConnector_TestComponent|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_ModelicaConnector_Testbench_ModelicaConnectorClassConflict_2errors()
        {
            string outputDir = "ModelicaConnector_Testbench_ModelicaConnectorClassConflict_2errors";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@ModelicaConnector|kind=Testing|relpos=0/@Testbench_ModelicaConnectorClassConflict_2errors|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_ModelicaConnector_Testbench_ModelicaConnectorClassInvalid_1()
        {
            string outputDir = "ModelicaConnector_Testbench_ModelicaConnectorClassInvalid_1";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@ModelicaConnector|kind=Testing|relpos=0/@Testbench_ModelicaConnectorClassInvalid_1|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_ModelicaConnector_Testbench_ModelicaConnectorClassInvalid_2()
        {
            string outputDir = "ModelicaConnector_Testbench_ModelicaConnectorClassInvalid_2";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@ModelicaConnector|kind=Testing|relpos=0/@Testbench_ModelicaConnectorClassInvalid_2|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_TestbenchSetup_Testbench_PostProcessingPathEmpty()
        {
            string outputDir = "TestbenchSetup_Testbench_PostProcessingPathEmpty";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@TestbenchSetup|kind=Testing|relpos=0/@Testbench_PostProcessingPathEmpty|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_TestbenchSetup_Testbench_PostProcessingFileDoesNotExist()
        {
            string outputDir = "TestbenchSetup_Testbench_PostProcessingFileDoesNotExist";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@TestbenchSetup|kind=Testing|relpos=0/@Testbench_PostProcessingFileDoesNotExist|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_TestbenchSetup_Testbench_2Workflows()
        {
            string outputDir = "TestbenchSetup_Testbench_2Workflows";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@TestbenchSetup|kind=Testing|relpos=0/@Testbench_2Workflows|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_TestbenchSetup_Testbench_PostProcessingScriptIsNotPython()
        {
            string outputDir = "TestbenchSetup_Testbench_PostProcessingScriptIsNotPython";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@TestbenchSetup|kind=Testing|relpos=0/@Testbench_PostProcessingScriptIsNotPython|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_TestbenchSetup_Testbench_WorkflowRefIsNull()
        {
            string outputDir = "TestbenchSetup_Testbench_WorkflowRefIsNull";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@TestbenchSetup|kind=Testing|relpos=0/@Testbench_WorkflowRefIsNull|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_TestbenchSetup_Testbench_2SolverSettings()
        {
            string outputDir = "TestbenchSetup_Testbench_2SolverSettings";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@TestbenchSetup|kind=Testing|relpos=0/@Testbench_2SolverSettings|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_TestbenchSetup_Testbench_2SUTs()
        {
            string outputDir = "TestbenchSetup_Testbench_2SUTs";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@TestbenchSetup|kind=Testing|relpos=0/@Testbench_2SUTs|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_TestbenchSetup_Testbench_WorkflowHasNoTask()
        {
            string outputDir = "TestbenchSetup_Testbench_WorkflowHasNoTask";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@TestbenchSetup|kind=Testing|relpos=0/@Testbench_WorkflowHasNoTask|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_TestbenchSetup_Testbench_workflowWrongTask()
        {
            string outputDir = "TestbenchSetup_Testbench_workflowWrongTask";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@TestbenchSetup|kind=Testing|relpos=0/@Testbench_workflowWrongTask|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_Names_Testbench_NameInvalidComponent()
        {
            string outputDir = "Names_Testbench_NameInvalidComponent";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@Names|kind=Testing|relpos=0/@Testbench_NameInvalidComponent|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_Names_Testbench_NameInvalidComponentRef()
        {
            string outputDir = "Names_Testbench_NameInvalidComponentRef";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@Names|kind=Testing|relpos=0/@Testbench_NameInvalidComponentRef|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_Names_Testbench_NameInvalidModelicaParameter()
        {
            string outputDir = "Names_Testbench_NameInvalidModelicaParameter";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@Names|kind=Testing|relpos=0/@Testbench_NameInvalidModelicaParameter|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_Names_Testbench_NameInvalidParameter()
        {
            string outputDir = "Names_Testbench_NameInvalidParameter";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@Names|kind=Testing|relpos=0/@Testbench_NameInvalidParameter|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_Names_Testbench_NameInvalidProperty()
        {
            string outputDir = "Names_Testbench_NameInvalidProperty";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@Names|kind=Testing|relpos=0/@Testbench_NameInvalidProperty|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_Names_Testbench_NonUniqueNames_CA()
        {
            string outputDir = "Names_Testbench_NonUniqueNames_CA";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@Names|kind=Testing|relpos=0/@Testbench_NonUniqueNames_CA|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_Names_Testbench_NonUniqueNamesTestComponents()
        {
            string outputDir = "Names_Testbench_NonUniqueNamesTestComponents";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@Names|kind=Testing|relpos=0/@Testbench_NonUniqueNamesTestComponents|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_ModelicaModelURI_Testbench_ModelicaUriInvalid_2()
        {
            string outputDir = "ModelicaModelURI_Testbench_ModelicaUriInvalid_2";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@ModelicaModelURI|kind=Testing|relpos=0/@Testbench_ModelicaUriInvalid_2|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_ModelicaModelURI_Testbench_ModelicaUriInvalid_1()
        {
            string outputDir = "ModelicaModelURI_Testbench_ModelicaUriInvalid_1";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@ModelicaModelURI|kind=Testing|relpos=0/@Testbench_ModelicaUriInvalid_1|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_ComponentsInTree_ComponentDefinedInCA()
        {
            string outputDir = "ComponentsInTree_ComponentDefinedInCA";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@ComponentsInTree|kind=Testing|relpos=0/@ComponentDefinedInCA|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_ComponentsInTree_ComponentDefininedInOtherCA()
        {
            string outputDir = "ComponentsInTree_ComponentDefininedInOtherCA";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@ComponentsInTree|kind=Testing|relpos=0/@ComponentDefininedInOtherCA|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_ComponentsInTree_ComponentDefinedInOtherTB()
        {
            string outputDir = "ComponentsInTree_ComponentDefinedInOtherTB";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@ComponentsInTree|kind=Testing|relpos=0/@ComponentDefinedInOtherTB|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CheckerShouldFail", "MSD_FailChecker")]
        public void Fail_ComponentsInTree_ComponentDefinedInThisTB()
        {
            string outputDir = "ComponentsInTree_ComponentDefinedInThisTB";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@ComponentsInTree|kind=Testing|relpos=0/@ComponentDefinedInThisTB|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.False(result, "CyPhy2Modelica_v2 should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CyPhy2Modelica", "MSD_FailChecker")]
        public void RulesFailures_Testbench()
        {
            string outputDir = "RulesFailures_Testbench";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesFailures|kind=Testing|relpos=0/@Testbench|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CyPhy2Modelica", "MSD_FailChecker")]
        public void RulesWarnings_Testbench_Damper_ParameterHasNoValue()
        {
            string outputDir = "RulesWarnings_Testbench_Damper_ParameterHasNoValue";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesWarnings|kind=Testing|relpos=0/@Testbench_Damper_ParameterHasNoValue|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CyPhy2Modelica", "MSD_FailChecker")]
        public void RulesWarnings_Testbench_TestComponentParamHasNoValue()
        {
            string outputDir = "RulesWarnings_Testbench_TestComponentParamHasNoValue";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesWarnings|kind=Testing|relpos=0/@Testbench_TestComponentParamHasNoValue|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CyPhy2Modelica", "MSD_FailChecker")]
        public void RulesWarnings_Testbench_Damper_PropertyHasNoValue()
        {
            string outputDir = "RulesWarnings_Testbench_Damper_PropertyHasNoValue";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesWarnings|kind=Testing|relpos=0/@Testbench_Damper_PropertyHasNoValue|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CyPhy2Modelica", "MSD_FailChecker")]
        public void RulesWarnings_Testbench_TestComponentPropertyHasNoValue()
        {
            string outputDir = "RulesWarnings_Testbench_TestComponentPropertyHasNoValue";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesWarnings|kind=Testing|relpos=0/@Testbench_TestComponentPropertyHasNoValue|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CyPhy2Modelica", "MSD_FailChecker")]
        public void RulesWarnings_Testbench_CAPropertyHasNoValue()
        {
            string outputDir = "RulesWarnings_Testbench_CAPropertyHasNoValue";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesWarnings|kind=Testing|relpos=0/@Testbench_CAPropertyHasNoValue|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CyPhy2Modelica", "MSD_FailChecker")]
        public void RulesWarnings_Testbench_CAParameterHasNoValue()
        {
            string outputDir = "RulesWarnings_Testbench_CAParameterHasNoValue";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@RulesWarnings|kind=Testing|relpos=0/@Testbench_CAParameterHasNoValue|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }

        [Fact]
        [Trait("Model", "MSD_FailChecker")]
        [Trait("CyPhy2Modelica", "MSD_FailChecker")]
        public void Test_Benches_MassSpringDamperTest()
        {
            string outputDir = "Test Benches_MassSpringDamperTest";
            string testBenchPath = "/@Test Benches|kind=Testing|relpos=0/@MassSpringDamperTest|kind=TestBench|relpos=0";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool result = CyPhy2ModelicaRunner.Run(outputDir, mgaFile, testBenchPath);

            Assert.True(result, "CyPhy2Modelica_v2 failed.");
        }


    }
}

