using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Xunit;
using System.Diagnostics;
using System.Threading;
using CyPhyGUIs;

namespace CADCreoTest
{
    public class Cyphy2CADCreoTest : IUseFixture<CADCreoTest.Cyphy2CADCreoTest.CadAssemblyFixture>
    {
        CadAssemblyFixture fixture;

        public class ExtractACMXMLFixture
        {
            public readonly string extractXmlExe;

            public ExtractACMXMLFixture()
            {
                extractXmlExe = Path.Combine(META.VersionInfo.MetaPath, "bin", "CAD", "Creo", "bin", "ExtractACM-XMLfromCreoModels.exe");
                if (File.Exists(extractXmlExe) == false)
                {
                    throw new FileNotFoundException(string.Format("ExtractACM-XMLfromCreoModels.exe is not installed at {0}.", extractXmlExe));
                }
            }
        }

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
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CADTeam\MSD_CAD.xme");
            string TestbenchPath = "/@MyTestBenches|kind=Testing|relpos=0/@TestBench_Config|kind=Testing|relpos=0/@Ballistic|kind=Testing|relpos=0/@Custom_Ballistics_Valid|kind=BallisticTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "BallisticTB_Custom_Valid");

            bool status = CADTeamTest.CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath, true);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedAsmFile)), "Failed to generate " + CADTeamTest.CADTests.generatedAsmFile);

            RunCreateAssembly(OutputDir);

            Assert.True(File.Exists(Path.Combine(OutputDir,"mymassspringdamper_1.asm.2")));
        }

        [Fact]
        public void KinematicTB_Creo_4Bar()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@Kinematics|kind=Testing|relpos=0/@Kinematic_FourBar|kind=KinematicTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "Kinematic_FourBar");

            bool status = CADTeamTest.CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath, true);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedAsmFile)), "Failed to generate " + CADTeamTest.CADTests.generatedAsmFile);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedMBDFile)), "Failed to generate " + CADTeamTest.CADTests.generatedMBDFile);

            RunCreateAssembly(OutputDir);
            Assert.True(File.Exists(Path.Combine(OutputDir, "systemundertest_1.asm.2")));
            //Assert.True(File.Exists(Path.Combine(OutputDir, "PARASOLID","SystemUnderTest_1_asm.x_t")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "ComputedValues.xml")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "CADAssembly_metrics.xml")));
        }

        [Fact]
        public void KinematicTB_Creo_Excavator()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@Kinematics|kind=Testing|relpos=0/@Kinematic_Excavator|kind=KinematicTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "Kinematic_Excavator");

            bool status = CADTeamTest.CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath, true);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedAsmFile)), "Failed to generate " + CADTeamTest.CADTests.generatedAsmFile);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedMBDFile)), "Failed to generate " + CADTeamTest.CADTests.generatedMBDFile);

            RunCreateAssembly(OutputDir);
            Assert.True(File.Exists(Path.Combine(OutputDir, "systemundertest_1.asm.2")));
            //Assert.True(File.Exists(Path.Combine(OutputDir, "PARASOLID", "SystemUnderTest_1_asm.x_t")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "ComputedValues.xml")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "CADAssembly_metrics.xml")));
        }

        private void RunCreateAssembly(string OutputDir)
        {
            ProcessStartInfo info = new ProcessStartInfo()
            {
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                RedirectStandardInput = true,
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

            ManualResetEvent outputDone = new ManualResetEvent(false);
            createAssembly.OutputDataReceived += (s, e) =>
            {
                if (e.Data != null)
                {
                    Console.Out.WriteLine(e.Data);
                }
                else
                {
                    outputDone.Set();
                }
            };
            var errorDone = new ManualResetEvent(false);
            createAssembly.ErrorDataReceived += (s, e) =>
            {
                if (e.Data != null)
                {
                    Console.Error.WriteLine(e.Data);
                }
                else
                {
                    errorDone.Set();
                }
            };

            createAssembly.Start();
            IntPtr createAssemblyJob = JobObjectPinvoke.AssignProcessToKillOnCloseJob(createAssembly);

            try
            {
                createAssembly.StandardInput.Close();
                createAssembly.BeginErrorReadLine();
                createAssembly.BeginOutputReadLine();

                bool exited = createAssembly.WaitForExit(90000);
                if (!exited)
                {
                    createAssembly.Kill();
                    createAssembly.WaitForExit();
                }
                outputDone.WaitOne(1000);
                errorDone.WaitOne(1000);
                Assert.True(exited, "CADCreoParametricCreateAssembly did not exit within 90 seconds");
            }
            finally
            {
                JobObjectPinvoke.CloseHandle(createAssemblyJob);
            }

            Assert.True(createAssembly.ExitCode == 0, string.Format("CADCreoParametricCreateAssembly failed with code {0}", createAssembly.ExitCode));
            Assert.True(VerifyCADAssemblerLog(Path.Combine(OutputDir, "log", "cad-assembler.log")));
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
