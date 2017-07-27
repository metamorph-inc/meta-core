using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Xunit;
using System.Diagnostics;

namespace CADCreoTest
{
    public class Cyphy2CADCreoTest : IUseFixture<CADCreoTest.Cyphy2CADCreoTest.CadAssemblyFixture>
    {
        CadAssemblyFixture fixture;

        public class MetaLinkFixture
        {
            public readonly string createAssemblyExe;

            public MetaLinkFixture()
            {
                createAssemblyExe = Path.Combine(META.VersionInfo.MetaPath, "bin", "CAD", "Creo", "bin", "CADCreoParametricMetaLink.exe");
                if (File.Exists(createAssemblyExe) == false)
                {
                    throw new FileNotFoundException("CADCreoParametricMetaLink.exe is not installed.");
                }
            }
        }

        public class CadAssemblyFixture
        {
            public readonly string createAssemblyExe;

            public CadAssemblyFixture()
            {
                createAssemblyExe = Path.Combine(META.VersionInfo.MetaPath, "bin", "CAD", "Creo", "bin", "CADCreoParametricCreateAssembly.exe");
                if (File.Exists(createAssemblyExe) == false)
                {
                    throw new FileNotFoundException("CADCreoParametricCreateAssembly.exe is not installed.");
                }
            }
        }


        [Fact]
        public void BallisticTB_Creo()
        {
            CyPhyPropagateTest.MetaLinkCreoTest.KillCreo();
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CADTeam\MSD_CAD.xme");
            string TestbenchPath = "/@MyTestBenches|kind=Testing|relpos=0/@TestBench_Config|kind=Testing|relpos=0/@Ballistic|kind=Testing|relpos=0/@Custom_Ballistics_Valid|kind=BallisticTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "BallisticTB_Custom_Valid");

            bool status = CADTeamTest.CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath, true);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedAsmFile)), "Failed to generate " + CADTeamTest.CADTests.generatedAsmFile);

            ProcessStartInfo info = new ProcessStartInfo()
            {
                RedirectStandardOutput = false,
                RedirectStandardError = false,
                RedirectStandardInput = false,
                UseShellExecute = false,
                WindowStyle = ProcessWindowStyle.Normal,
                CreateNoWindow = false,
                FileName = fixture.createAssemblyExe,
                Arguments = "-w . -i CADAssembly.xml",
                WorkingDirectory = OutputDir
                // TODO -p ?
            };

            Process createAssembly = new Process();
            createAssembly.StartInfo = info;

            createAssembly.Start();

            bool exited = createAssembly.WaitForExit(45000);
            if (!exited)
            {
                createAssembly.Kill();
                createAssembly.WaitForExit();
            }
            Assert.True(exited);

            Assert.Equal(createAssembly.ExitCode, 0);
            Assert.True(VerifyCADAssemblerLog(Path.Combine(OutputDir, "log", "cad-assembler.log")));
            Assert.True(File.Exists(Path.Combine(OutputDir,"mymassspringdamper_1.asm.2")));
        }

        [Fact]
        public void KinematicTB_Creo_4Bar()
        {
            CyPhyPropagateTest.MetaLinkCreoTest.KillCreo();
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@Kinematics|kind=Testing|relpos=0/@Kinematic_FourBar|kind=KinematicTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "Kinematic_FourBar");

            bool status = CADTeamTest.CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath, true);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedAsmFile)), "Failed to generate " + CADTeamTest.CADTests.generatedAsmFile);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedMBDFile)), "Failed to generate " + CADTeamTest.CADTests.generatedMBDFile);

            ProcessStartInfo info = new ProcessStartInfo()
            {
                RedirectStandardOutput = false,
                RedirectStandardError = false,
                RedirectStandardInput = false,
                UseShellExecute = false,
                WindowStyle = ProcessWindowStyle.Normal,
                CreateNoWindow = false,
                FileName = fixture.createAssemblyExe,
                Arguments = "-w . -i CADAssembly.xml",
                WorkingDirectory = OutputDir
                // TODO -p ?
            };

            Process createAssembly = new Process();
            createAssembly.StartInfo = info;

            createAssembly.Start();

            bool exited = createAssembly.WaitForExit(45000);
            if (!exited)
            {
                createAssembly.Kill();
                createAssembly.WaitForExit();
            }
            Assert.True(exited);

            Assert.Equal(createAssembly.ExitCode, 0);
            Assert.True(VerifyCADAssemblerLog(Path.Combine(OutputDir, "log", "cad-assembler.log")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "systemundertest_1.asm.2")));
            //Assert.True(File.Exists(Path.Combine(OutputDir, "PARASOLID","SystemUnderTest_1_asm.x_t")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "ComputedValues.xml")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "CADAssembly_metrics.xml")));
        }

        [Fact]
        public void KinematicTB_Creo_Excavator()
        {
            CyPhyPropagateTest.MetaLinkCreoTest.KillCreo();
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@Kinematics|kind=Testing|relpos=0/@Kinematic_Excavator|kind=KinematicTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "Kinematic_Excavator");

            bool status = CADTeamTest.CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath, true);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedAsmFile)), "Failed to generate " + CADTeamTest.CADTests.generatedAsmFile);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedMBDFile)), "Failed to generate " + CADTeamTest.CADTests.generatedMBDFile);

            ProcessStartInfo info = new ProcessStartInfo()
            {
                RedirectStandardOutput = false,
                RedirectStandardError = false,
                RedirectStandardInput = false,
                UseShellExecute = false,
                WindowStyle = ProcessWindowStyle.Normal,
                CreateNoWindow = false,
                FileName = fixture.createAssemblyExe,
                Arguments = "-w . -i CADAssembly.xml",
                WorkingDirectory = OutputDir
                // TODO -p ?
            };

            Process createAssembly = new Process();
            createAssembly.StartInfo = info;

            createAssembly.Start();

            bool exited = createAssembly.WaitForExit(45000);
            if (!exited)
            {
                createAssembly.Kill();
                createAssembly.WaitForExit();
            }
            Assert.True(exited);

            Assert.Equal(createAssembly.ExitCode, 0);
            Assert.True(VerifyCADAssemblerLog(Path.Combine(OutputDir, "log", "cad-assembler.log")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "systemundertest_1.asm.2")));
            //Assert.True(File.Exists(Path.Combine(OutputDir, "PARASOLID", "SystemUnderTest_1_asm.x_t")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "ComputedValues.xml")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "CADAssembly_metrics.xml")));
        }

        public static bool VerifyCADAssemblerLog(string logfile)
        {
            return true;
        }


        public void SetFixture(Cyphy2CADCreoTest.CadAssemblyFixture data)
        {
            this.fixture = data;
        }
    }
}
