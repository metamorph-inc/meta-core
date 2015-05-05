using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using System.Diagnostics;

namespace FEAKinematicTest
{
    public class FEAKinematicTest : IUseFixture<FEAKinematicTest.AssemblyAdamsFixture>
    {
        AssemblyAdamsFixture fixture;

        public class AssemblyAdamsFixture
        {
            public readonly string createAssemblyExe;
            public readonly string AdamsScript;
            public readonly string AdamsBat;
            public readonly string FeaScript;

            public AssemblyAdamsFixture()
            {
                string proeIsisExtensionsDir = System.Environment.GetEnvironmentVariable("PROE_ISIS_EXTENSIONS");
                string adamsDir = System.Environment.GetEnvironmentVariable("ADAMS_PATH");
                createAssemblyExe = Path.Combine(proeIsisExtensionsDir ?? "", "bin", "CADCreoParametricCreateAssembly.exe");
                AdamsScript = Path.Combine(proeIsisExtensionsDir ?? "", "bin", "Adams", "CreateAdamsModel.py");
                FeaScript = Path.Combine(proeIsisExtensionsDir ?? "", "bin", "CyPhy2AbaqusCmd.py");
                AdamsBat = Path.Combine(adamsDir ?? "", "bin", "adams2013_2_x64.bat");
                if (File.Exists(createAssemblyExe) == false)
                {
                    throw new FileNotFoundException("CADCreoParametricCreateAssembly.exe is not installed.");
                }
                if (File.Exists(AdamsScript) == false)
                {
                    throw new FileNotFoundException("CreateAdamsModel.py is not installed.");
                }
            }
        }

        private bool RunProcess(string exec, string args, string workingdir, out int exitcode, int timeout = 45000)
        {
            ProcessStartInfo info = new ProcessStartInfo()
            {
                RedirectStandardOutput = false,
                RedirectStandardError = false,
                RedirectStandardInput = false,
                UseShellExecute = false,
                WindowStyle = ProcessWindowStyle.Normal,
                CreateNoWindow = false,
                FileName = exec,
                Arguments = args,
                WorkingDirectory = workingdir
            };

            Process process = new Process();
            process.StartInfo = info;

            process.Start();

            bool exited = process.WaitForExit(timeout);
            if (!exited)
            {
                process.Kill();
                process.WaitForExit();
            }
            exitcode = process.ExitCode;

            return exited;
        }

        //[Fact]
        public void KinematicTB_4Bar()
        {
            CyPhyPropagateTest.MetaLinkCreoTest.KillCreo();
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@Kinematic_FourBar|kind=KinematicTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "Kinematic_FourBar");

            bool status = CADTeamTest.CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath, true);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedAsmFile)), "Failed to generate " + CADTeamTest.CADTests.generatedAsmFile);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedMBDFile)), "Failed to generate " + CADTeamTest.CADTests.generatedMBDFile);

            int exitcode;

            Assert.True(RunProcess(fixture.createAssemblyExe, "-w . -i CADAssembly.xml", OutputDir, out exitcode));

            Assert.Equal(exitcode, 0);
            Assert.True(CADCreoTest.Cyphy2CADCreoTest.VerifyCADAssemblerLog(Path.Combine(OutputDir, "log", "cad-assembler.log")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "systemundertest_1.asm.2")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "PARASOLID", "SystemUnderTest_1_asm.x_t")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "ComputedValues.xml")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "CADAssembly_metrics.xml")));

            Assert.True(RunProcess(@"c:\Python27\python.exe", "\""+fixture.AdamsScript+"\"", OutputDir, out exitcode));

            Assert.Equal(exitcode, 0);
            Assert.True(File.Exists(Path.Combine(OutputDir, "adams_output.cmd")));

            Assert.True(RunProcess(fixture.AdamsBat, "aview ru-st b adams_output.cmd", OutputDir, out exitcode));

            Assert.True(File.Exists(Path.Combine(OutputDir, "LT_BAR_01_4Z.lod")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "LT_BAR_02_3Z.lod")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "LT_BAR_03_1Z.lod")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "LT_BAR_04_2Z.lod")));

        }

        //[Fact]
        public void FEATB_4Bar()
        {
            CyPhyPropagateTest.MetaLinkCreoTest.KillCreo();

            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA_Fourbar|kind=CADTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "FEA_FourBar");

            bool status = CADTeamTest.CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath, true);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedAsmFile)), "Failed to generate " + CADTeamTest.CADTests.generatedAsmFile);

            int exitcode;

            Assert.True(RunProcess(fixture.createAssemblyExe, "-w . -i CADAssembly.xml", OutputDir, out exitcode));

            Assert.Equal(exitcode, 0);
            Assert.True(CADCreoTest.Cyphy2CADCreoTest.VerifyCADAssemblerLog(Path.Combine(OutputDir, "log", "cad-assembler.log")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "fea_fourbar_1.asm.2")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "ComputedValues.xml")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "CADAssembly_metrics.xml")));

            Assert.True(RunProcess(@"c:\SIMULIA\Abaqus\Commands\abaqus.bat", "cae noGUI=" + "\""+fixture.FeaScript+"\"", OutputDir, out exitcode, 120000));

            Assert.True(exitcode == 0);
        }

        [Fact]
        public void FEAKinematicTB_4Bar()
        {
            KinematicTB_4Bar();

            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA_Fourbar|kind=CADTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "Kinematic_FourBar");

            bool status = CADTeamTest.CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath, false, false);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedAsmFile)), "Failed to generate " + CADTeamTest.CADTests.generatedAsmFile);

            int exitcode;

            Assert.True(RunProcess(fixture.createAssemblyExe, "-w . -i CADAssembly.xml", OutputDir, out exitcode));

            Assert.Equal(exitcode, 0);
            Assert.True(CADCreoTest.Cyphy2CADCreoTest.VerifyCADAssemblerLog(Path.Combine(OutputDir, "log", "cad-assembler.log")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "fea_fourbar_1.asm.2")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "ComputedValues.xml")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "CADAssembly_metrics.xml")));

            Assert.True(RunProcess(@"c:\SIMULIA\Abaqus\Commands\abaqus.bat", "cae noGUI=" + "\"" + fixture.FeaScript + "\"", OutputDir, out exitcode, 120000));

            Assert.True(exitcode == 0);
        }

        public void SetFixture(FEAKinematicTest.AssemblyAdamsFixture data)
        {
            this.fixture = data;
        }

    }
}
