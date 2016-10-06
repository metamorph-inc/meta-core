using Xunit;
using System.IO;
using GME.MGA;

namespace MasterInterpreterTest.Projects
{
    public class MBDFixture
    {
        internal string mgaFile = null;

        public MBDFixture()
        {
            this.mgaFile = Test.ImportXME2Mga(Path.Combine("..", "MBD"), "MBD.xme");
        }
    }

    public class MBDModelImport
    {
        [Fact]
        [Trait("Model", "MBD")]
        [Trait("ProjectImport/Open", "MBD")]
        public void ProjectXmeImport()
        {
            Assert.DoesNotThrow(() => { new MBDFixture(); });
        }
    }

    public partial class MBD : IUseFixture<MBDFixture>
    {
        internal string mgaFile { get { return this.fixture.mgaFile; } }
        private MBDFixture fixture { get; set; }

        public void SetFixture(MBDFixture data)
        {
            this.fixture = data;
        }

        [Fact]
        [Trait("Model", "MBD")]
        [Trait("ProjectImport/Open", "MBD")]
        public void ProjectMgaOpen()
        {
            var mgaReference = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(mgaReference, "CyPhyML", null);
            MgaHelper.CheckParadigmVersionUpgrade(project);
            project.Close(true);
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }

        [Fact]
        [Trait("Model", "MBD")]
        [Trait("MasterInterpreter", "RunInterpreterSuccess")]
        public void Kinematic_Excavator()
        {
            //string outputDir = "MI_10_TestBench_MSD_om_DS_MassSpringDamper_cfg4";
            string objectAbsPath = "/@Testing/@Kinematics/@Kinematic_Excavator";
            string configAbsPath = "/@ComponentAssemblies/@Excavator/@Excavator";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");

            var success = CyPhyMasterInterpreterRunner.RunMasterInterpreter(
                projectPath: mgaFile,
                absPath: objectAbsPath,
                configPath: configAbsPath,
                postToJobManager: false,
                keepTempModels: false);

            Assert.True(success, "CyPhyMasterInterpreter run should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "MBD")]
        [Trait("MasterInterpreter", "RunInterpreterFail")]
        public void Kinematic_Excavator_CyPhy2CADFail()
        {
            //string outputDir = "MI_10_TestBench_MSD_om_DS_MassSpringDamper_cfg4";
            string objectAbsPath = "/@Testing/@TestFail/@Kinematic_Excavator_CyPhy2CADFail";
            string configAbsPath = "/@ComponentAssemblies/@TestFail/@Excavator_CyPhy2CADFail";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");

            var success = CyPhyMasterInterpreterRunner.RunMasterInterpreter(
                projectPath: mgaFile,
                absPath: objectAbsPath,
                configPath: configAbsPath,
                postToJobManager: false,
                keepTempModels: false);

            Assert.False(success, "CyPhyMasterInterpreter run should have failed, but did not.");
        }


        [Fact]
        [Trait("Model", "MBD")]
        [Trait("MasterInterpreter", "RunInterpreterSuccess")]
        public void MBD_SOT()
        {
            //string outputDir = "MI_10_TestBench_MSD_om_DS_MassSpringDamper_cfg4";
            string objectAbsPath = "/@Testing/@TestBenchSuites/@Kinematic_SOT";
            string configAbsPath = "/@ComponentAssemblies/@FourBar/@FourBar";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");

            var success = CyPhyMasterInterpreterRunner.RunMasterInterpreter(
                projectPath: mgaFile,
                absPath: objectAbsPath,
                configPath: configAbsPath,
                postToJobManager: false,
                keepTempModels: false);

            Assert.True(success, "CyPhyMasterInterpreter run should have succeeded, but did not.");
        }

    }

}
