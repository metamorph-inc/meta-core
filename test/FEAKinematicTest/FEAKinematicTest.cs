using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using System.Diagnostics;
using Microsoft.Win32;
using Newtonsoft.Json;
using GME.MGA.Meta;

namespace FEAKinematicTest
{
    public class FEAKinematicTest : IUseFixture<FEAKinematicTest.AssemblyAdamsFixture>
    {
        #region Test Setup
        AssemblyAdamsFixture fixture;

        public class AssemblyAdamsFixture
        {
            public readonly string createAssemblyExe;
            public readonly string AdamsScript;
            public readonly string AdamsBat;
            public readonly string AbaqusBat;
            public readonly string PythonExe;
            public readonly string FeaScript;
            public readonly string AbqPPScript;
            public readonly string CADVerifyScript;
            public readonly string abaqus;

            public AssemblyAdamsFixture()
            {
                string metapath = Meta_Path;
                string abaquspath = Abaqus_Path;
                string adamsDir = System.Environment.GetEnvironmentVariable("ADAMS_PATH");
                abaqus = System.Environment.GetEnvironmentVariable("abaqus");
                createAssemblyExe = Path.Combine(META.VersionInfo.MetaPath, "bin", "CAD", "Creo", "bin", "CADCreoParametricCreateAssembly.exe");
                AdamsScript = Path.Combine(metapath ?? "", "bin", "CAD", "Adams", "CreateAdamsModel.py");
                CADVerifyScript = Path.Combine(metapath ?? "", "bin", "CAD", "TestCADResults.py");
                FeaScript = Path.Combine(metapath ?? "", "bin", "CAD", "Abaqus", "AbaqusMain.py");
                AbqPPScript = Path.Combine(metapath ?? "", "bin", "CAD", "ABQ_CompletePostProcess.py");
                AdamsBat = Path.Combine(adamsDir ?? "", "bin", "adams2013_2_x64.bat");
                AbaqusBat = Path.Combine(abaquspath ?? "", "Commands", "abaqus.bat");
                PythonExe = Path.Combine(Meta_Path ?? "", "bin", "Python27", "Scripts", "python.exe");
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

        public static string Meta_Path
        {
            get
            {
                const string keyName = "Software\\META";

                RegistryKey metaKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry32)
                    .OpenSubKey(keyName, false);

                string metaPath = "C:\\Program Files (x86)\\META";
                if (metaKey != null)
                {
                    metaPath = (string)metaKey.GetValue("META_PATH", metaPath);
                }

                return metaPath;
            }
        }

        public static string Abaqus_Path
        {
            get
            {
                const string keyName = "Software\\Wow6432Node\\SIMULIA\\Abaqus";

                RegistryKey AbaqusKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry32)
                    .OpenSubKey(keyName, false);

                string AbaqusPath = "C:\\SIMULIA\\Abaqus";
                if (AbaqusKey != null)
                {
                    AbaqusPath = (string)AbaqusKey.GetValue("AbaqusDir", AbaqusPath);
                }

                return AbaqusPath;
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
            IntPtr createAssemblyJob = CyPhyMetaLink.JobObjectPinvoke.AssignProcessToKillOnCloseJob(process);

            bool exited = false;
            try
            {
                exited = process.WaitForExit(timeout);
                if (!exited)
                {
                    process.Kill();
                    process.WaitForExit();
                }
                exitcode = process.ExitCode;
            }
            finally
            {
                CyPhyMetaLink.JobObjectPinvoke.CloseHandle(createAssemblyJob);
            }

            return exited;
        }

        public string SetupTestAndRunCAD(string XmePath, string TestbenchPath, string dirName, string asmName)
        {
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), dirName);
            bool status = CADTeamTest.CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath, true);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedAsmFile)), "Failed to generate " + CADTeamTest.CADTests.generatedAsmFile);

            int exitcode;

            Assert.True(RunProcess(fixture.createAssemblyExe, "-w . -i CADAssembly.xml", OutputDir, out exitcode));

            Assert.Equal(exitcode, 0);
            Assert.True(CADCreoTest.Cyphy2CADCreoTest.VerifyCADAssemblerLog(Path.Combine(OutputDir, "log", "cad-assembler.log")));
            Assert.True(File.Exists(Path.Combine(OutputDir, asmName)));
            Assert.True(File.Exists(Path.Combine(OutputDir, "ComputedValues.xml")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "CADAssembly_metrics.xml")));
            return OutputDir;
        }
        #endregion

        #region Kinematic Tests

        [Fact]
        public void KinematicTB_4Bar()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@Kinematics|kind=Testing|relpos=0/@Kinematic_FourBar|kind=KinematicTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "Kinematic_FourBar");

            bool status = CADTeamTest.CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath, true);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedAsmFile)), "Failed to generate " + CADTeamTest.CADTests.generatedAsmFile);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedMBDFile)), "Failed to generate " + CADTeamTest.CADTests.generatedMBDFile);

            int exitcode;

            Assert.True(RunProcess(fixture.createAssemblyExe, "-w . -i CADAssembly.xml", OutputDir, out exitcode));

            Assert.Equal(exitcode, 0);
            Assert.True(CADCreoTest.Cyphy2CADCreoTest.VerifyCADAssemblerLog(Path.Combine(OutputDir, "log", "cad-assembler.log")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "systemundertest_1.asm.2")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "ComputedValues.xml")));
            Assert.True(File.Exists(Path.Combine(OutputDir, "CADAssembly_metrics.xml")));

            Assert.True(RunProcess(fixture.PythonExe, "\"" + fixture.CADVerifyScript + "\"" + " " + Path.Combine(OutputDir, "CADAssembly_metrics.xml") + " " + Path.Combine(Path.GetDirectoryName(XmePath), "expected_results", "Kinematic_fourbar_metrics.xml"), OutputDir, out exitcode));

            Assert.Equal(exitcode, 0);

            Assert.True(RunProcess(fixture.PythonExe, "\""+fixture.AdamsScript+"\"", OutputDir, out exitcode));

            Assert.Equal(exitcode, 0);
            Assert.True(File.Exists(Path.Combine(OutputDir, "adams_output.cmd")));

            Assert.True(RunProcess(fixture.AdamsBat, "aview ru-st b adams_output.cmd", OutputDir, out exitcode));

            // Verify the 4 LODs based on the CADAssembly.xml
            Assert.True(System.IO.Directory.GetFiles(OutputDir, "*.lod", System.IO.SearchOption.TopDirectoryOnly).Length == 4);
        }
        #endregion
        /*
        #region Standalone FEA (Deck-Based) tests

        [Fact]
        public void FEATB_4Bar_Static_Deck()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@DeckBased|kind=Testing|relpos=0/@FEA_Fourbar_Deck|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_StaticDeck", "fea_fourbar_deck_1.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");
            string id = Guid.NewGuid().ToString().Substring(0, 6);
            Assert.True(RunProcess(fixture.AbaqusBat, "fromnastran job=" + "\"" + id + "\"" + " input=" + "\"..\\Nastran_mod.nas", AnalysisDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(RunProcess(fixture.AbaqusBat, "analysis interactive job=" + id, AnalysisDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(RunProcess(fixture.AbaqusBat, "odbreport job=" + id + " results", AnalysisDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.AbqPPScript + "\"" + " -- -o " + id + " .odb -p ..\\AnalysisMetaData.xml -m ..\\..\\RequestedMetrics.xml -j ..\\..\\testbench_manifest.json", AnalysisDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(AnalysisDir, id, "OutMetrics.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, id, "OutMetrics.xml")));
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, id, "Stress_Contours")).Any());
        }
        #endregion

        #region Standalone FEA (Model-Based) tests

        [Fact]
        public void FEATB_4Bar_MeshedOnly()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_MeshedOnly", "fea_fourbar_1.asm.2");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -o", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(OutputDir, "Analysis", "Abaqus", "MeshQuality.csv")));

            // Check for mesh numerical accuracy
            ProcessStartInfo pythoncall = new ProcessStartInfo();
            pythoncall.FileName = fixture.PythonBat;
            string module = "..\\..\\CheckFiles.py";
            string input = "\"" + Path.Combine(OutputDir, "Analysis", "Abaqus", "MeshQuality.csv") + "\"";
            string check = "..\\..\\ReferenceFiles\\MeshQuality_FB.csv";
            pythoncall.Arguments = string.Format("{0} {1} {2} {3} {4}", module, "-i", input, "-c", check);
            pythoncall.UseShellExecute = false;
            pythoncall.RedirectStandardOutput = true;
            using (Process process = Process.Start(pythoncall))
            {
                using (StreamReader checker = process.StandardOutput)
                {
                    string result = checker.ReadToEnd();
                    Assert.True(result.Equals("0\r\n"));
                }
            }
        }

        [Fact]
        public void FEATB_4Bar_MeshAndBC()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_MeshAndBC", "fea_fourbar_1.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -b", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "MeshQuality.csv")));
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "Contour_and_BC_plots")).Any());
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "AbaqusMeshAndAssembly.cae")));
        }

        [Fact]
        public void FEATB_4Bar_Static()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_Static", "fea_fourbar_1.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -s", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "staticFEAAnalysis", "MeshQuality.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "staticFEAAnalysis", "OutMetrics.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "staticFEAAnalysis", "OutMetrics.xml")));
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "Contour_and_BC_plots")).Any());
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "staticFEAAnalysis", "Stress_Contours")).Any());
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "AbaqusMeshAndAssembly.cae")));
        }

        [Fact]
        public void FEATB_Plate_Adaptive_Static()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\Validation\FEA\Plate with Hole\PlateWithHoleModel.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@Plate_with_a_hole_adaptive|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "PlateWithHole_AdaptiveStatic", "plate_with_a_hole_adaptive.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -s", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "Adaptivity-1-iter3", "MeshQuality.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "Adaptivity-1-iter3", "OutMetrics.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "Adaptivity-1-iter3", "OutMetrics.xml")));
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "Contour_and_BC_plots")).Any());
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "Adaptivity-1-iter3", "Stress_Contours")).Any());
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "AbaqusMeshAndAssembly.cae")));

            // Check for mesh numerical accuracy
            ProcessStartInfo pythoncall = new ProcessStartInfo();
            pythoncall.FileName = fixture.PythonBat;
            string module = "..\\..\\CheckFiles.py";
            string input = "\"" + Path.Combine(AnalysisDir, "Adaptivity-1-iter3", "MeshQuality.csv") + "\"";
            string check = "..\\..\\ReferenceFiles\\MeshQuality_PWH.csv";
            pythoncall.Arguments = string.Format("{0} {1} {2} {3} {4}", module, "-i", input, "-c", check);
            pythoncall.UseShellExecute = false;
            pythoncall.RedirectStandardOutput = true;
            using (Process process = Process.Start(pythoncall))
            {
                using (StreamReader checker = process.StandardOutput)
                {
                    string result = checker.ReadToEnd();
                    Assert.True(result.Equals("0\r\n"));
                }
            }
        }

        [Fact]
        public void FEATB_Plate_Coupled_Static()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\Validation\FEA\Plate with Hole\PlateWithHoleModel.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@Plate_with_a_hole_coupled|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "PlateWithHole_CoupledStatic", "plate_with_a_hole_coupled.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -s", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "staticFEAAnalysis", "MeshQuality.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "staticFEAAnalysis", "OutMetrics.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "staticFEAAnalysis", "OutMetrics.xml")));
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "Contour_and_BC_plots")).Any());
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "staticFEAAnalysis", "Stress_Contours")).Any());
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "staticFEAAnalysis", "Thermal_Contours")).Any());
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "AbaqusMeshAndAssembly.cae")));
        }

        [Fact]
        public void FEATB_Plate_Coupled_AdaptiveStatic()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\Validation\FEA\Plate with Hole\PlateWithHoleModel.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@Plate_with_a_hole_coupled_adaptive|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "PlateWithHole_Coupled_AdaptiveStatic", "plate_with_a_hole_coupled_adape.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -s", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "Adaptivity-1-iter3", "MeshQuality.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "Adaptivity-1-iter3", "OutMetrics.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "Adaptivity-1-iter3", "OutMetrics.xml")));
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "Contour_and_BC_plots")).Any());
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "Adaptivity-1-iter3", "Stress_Contours")).Any());
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "Adaptivity-1-iter3", "Thermal_Contours")).Any());
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "AbaqusMeshAndAssembly.cae")));
        }

        [Fact]
        public void FEATB_Plate_DynamicImplicit()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\Validation\FEA\Plate with Hole\PlateWithHoleModel.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@Plate_with_a_hole|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "PlateWithHole_DynamicImplicit", "plate_with_a_hole.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -i", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "MeshQuality.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "OutMetrics.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "OutMetrics.xml")));
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "Contour_and_BC_plots")).Any());
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "Stress_Contours")).Any());
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "AbaqusMeshAndAssembly.cae")));
        }

        //[Fact]
        // Execution is very long (Over 20 hours).
        public void FEATB_Plate_DynamicExplicit()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\Validation\FEA\Plate with Hole\PlateWithHoleModel.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@Plate_with_a_hole|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "PlateWithHole_DynamicExplicit", "plate_with_a_hole.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -e", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "MeshQuality.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "OutMetrics.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "OutMetrics.xml")));
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "Contour_and_BC_plots")).Any());
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "Stress_Contours")).Any());
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "AbaqusMeshAndAssembly.cae")));
        }

        [Fact]
        public void FEATB_4Bar_Modal()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_Modal", "fea_fourbar_1.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -m", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "MeshQuality.csv")));
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "Contour_and_BC_plots")).Any());
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "AbaqusMeshAndAssembly.cae")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "modalOutput.csv")));
        }

        [Fact]
        public void FEATB_Plate_Thermal()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\Validation\FEA\Plate with Hole\PlateWithHoleModel.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@Plate_with_a_hole_thermal|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "PlateWithHole_Thermal", "plate_with_a_hole_thermal.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -s", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "staticFEAAnalysis", "MeshQuality.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "staticFEAAnalysis", "OutMetrics.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "staticFEAAnalysis", "OutMetrics.xml")));
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "Contour_and_BC_plots")).Any());
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "staticFEAAnalysis", "Thermal_Contours")).Any());
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "AbaqusMeshAndAssembly.cae")));
        }

        [Fact]
        public void FEATB_Plate_ThermalAdaptiveStatic()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\Validation\FEA\Plate with Hole\PlateWithHoleModel.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@Plate_with_a_hole_thermal_adaptive|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "PlateWithHole_ThermalAdaptiveStatic", "plate_with_a_hole_thermal_adape.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -s", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "Adaptivity-1-iter3", "MeshQuality.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "Adaptivity-1-iter3", "OutMetrics.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "Adaptivity-1-iter3", "OutMetrics.xml")));
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "Contour_and_BC_plots")).Any());
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "Adaptivity-1-iter3", "Thermal_Contours")).Any());
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "AbaqusMeshAndAssembly.cae")));
        }

        //[Fact]
        // Execution time too long.
        public void FEATB_Plate_Coupled_DynamicImplicit()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\Validation\FEA\Plate with Hole\PlateWithHoleModel.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@Plate_with_a_hole_coupled|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "PlateWithHole_CoupledDynamicImplicit", "plate_with_a_hole_coupled.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -i", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "MeshQuality.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "OutMetrics.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "OutMetrics.xml")));
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "Contour_and_BC_plots")).Any());
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "Stress_Contours")).Any());
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "Thermal_Contours")).Any());
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "AbaqusMeshAndAssembly.cae")));
        }

        //[Fact]
        // Execution time too long: ~ 20 hours
        public void FEATB_Plate_Coupled_DynamicExplicit()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\Validation\FEA\Plate with Hole\PlateWithHoleModel.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@Plate_with_a_hole_coupled|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "PlateWithHole_CoupledDynamicExplicit", "plate_with_a_hole_coupled.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -e", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "MeshQuality.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "OutMetrics.csv")));
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "OutMetrics.xml")));
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "Contour_and_BC_plots")).Any());
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "Stress_Contours")).Any());
            Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(AnalysisDir, "dynamicFEAAnalysis", "Thermal_Contours")).Any());
            Assert.True(File.Exists(Path.Combine(AnalysisDir, "AbaqusMeshAndAssembly.cae")));
        }

        #endregion

        #region ADAMS to ABAQUS tests

        [Fact]
        public void FEAKinematicTB_4Bar_Static()
        {
            KinematicTB_4Bar();
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_Static_A2A", "fea_fourbar_1.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");

            AVM.DDP.MetaTBManifest.Dependency kinDependency = new AVM.DDP.MetaTBManifest.Dependency();
            kinDependency.Type = "KinematicTestBench";
            kinDependency.Directory = Path.Combine(OutputDir, "..\\Kinematic_FourBar");
            AVM.DDP.MetaTBManifest manifest = AVM.DDP.MetaTBManifest.OpenForUpdate(OutputDir);
            manifest.AddDependency(kinDependency);
            manifest.Serialize(OutputDir);

            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -s", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);

            string[] comps = Directory.GetDirectories(AnalysisDir);
            foreach (string comp in comps)
            {
                if (Path.Combine(AnalysisDir, "log").Equals(Path.Combine(AnalysisDir, comp)))
                {
                    continue;
                }
                else
                {
                    string compDir = Path.Combine(AnalysisDir, comp);
                    string[] mesh = System.IO.Directory.GetFiles(comp, "Mesh*.csv");
                    Assert.True(mesh.Length > 0);
                    Assert.True(File.Exists(Path.Combine(comp, "OutMetrics.csv")));
                    Assert.True(File.Exists(Path.Combine(comp, "OutMetrics.xml")));
                    Assert.True(Directory.EnumerateFileSystemEntries(Path.Combine(comp, "Stress_Contours")).Any());
                }
            }
        }

        [Fact]
        public void FEAKinematicTB_4Bar_MeshOnly()
        {
            KinematicTB_4Bar();
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_MeshOnly_A2A", "fea_fourbar_1.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");

            AVM.DDP.MetaTBManifest.Dependency kinDependency = new AVM.DDP.MetaTBManifest.Dependency();
            kinDependency.Type = "KinematicTestBench";
            kinDependency.Directory = Path.Combine(OutputDir, "..\\Kinematic_FourBar");
            AVM.DDP.MetaTBManifest manifest = AVM.DDP.MetaTBManifest.OpenForUpdate(OutputDir);
            manifest.AddDependency(kinDependency);
            manifest.Serialize(OutputDir);

            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -o", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);

            string[] mesh = System.IO.Directory.GetFiles(AnalysisDir, "MeshQuality_*.csv");
            Assert.True(mesh.Length == 4);
        }

        [Fact]
        public void FEAKinematicTB_4Bar_DynamicImplicit()
        {
            KinematicTB_4Bar();
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_DynamicImplicit_A2A", "fea_fourbar_1.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");

            AVM.DDP.MetaTBManifest.Dependency kinDependency = new AVM.DDP.MetaTBManifest.Dependency();
            kinDependency.Type = "KinematicTestBench";
            kinDependency.Directory = Path.Combine(OutputDir, "..\\Kinematic_FourBar");
            AVM.DDP.MetaTBManifest manifest = AVM.DDP.MetaTBManifest.OpenForUpdate(OutputDir);
            manifest.AddDependency(kinDependency);
            manifest.Serialize(OutputDir);

            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -i", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);

            string[] comps = Directory.GetDirectories(AnalysisDir);
            foreach (string comp in comps)
            {
                if (Path.Combine(AnalysisDir, "log").Equals(Path.Combine(AnalysisDir, comp)))
                {
                    continue;
                }
                else
                {
                    string compDir = Path.Combine(AnalysisDir, comp);
                    string[] mesh = System.IO.Directory.GetFiles(compDir, "MeshQuality_*.csv");
                    Assert.True(mesh.Length > 0);
                    string[] metrics = System.IO.Directory.GetFiles(compDir, "OutMetrics.csv");
                    Assert.True(metrics.Length > 0);
                }
            }
        }

        //[Fact]
        // Execution time very long.
        public void FEAKinematicTB_4Bar_DynamicExplicit()
        {
            KinematicTB_4Bar();
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_DynamicExplicit_A2A", "fea_fourbar_1.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");

            AVM.DDP.MetaTBManifest.Dependency kinDependency = new AVM.DDP.MetaTBManifest.Dependency();
            kinDependency.Type = "KinematicTestBench";
            kinDependency.Directory = Path.Combine(OutputDir, "..\\Kinematic_FourBar");
            AVM.DDP.MetaTBManifest manifest = AVM.DDP.MetaTBManifest.OpenForUpdate(OutputDir);
            manifest.AddDependency(kinDependency);
            manifest.Serialize(OutputDir);

            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -e", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);

            string[] comps = Directory.GetDirectories(AnalysisDir);
            Assert.True(comps.Length == 4);
            foreach (string comp in comps)
            {
                if (Path.Combine(AnalysisDir, "log").Equals(Path.Combine(AnalysisDir, comp)))
                {
                    continue;
                }
                else
                {
                    string compDir = Path.Combine(AnalysisDir, comp);
                    string[] mesh = System.IO.Directory.GetFiles(compDir, "MeshQuality_*.csv");
                    Assert.True(mesh.Length > 0);
                    string[] metrics = System.IO.Directory.GetFiles(compDir, "OutMetrics.csv");
                    Assert.True(metrics.Length > 0);
                }
            }
        }

        #endregion

        #region FailureTests
        [Fact]
        public void FEATB_4Bar_AdaptiveDE()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar_Adaptive|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_AdaptiveDE", "fea_fourbar_adaptive_1.asm.2");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -e", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(OutputDir, "_FAILED.txt")));
        }

        [Fact]
        public void FEATB_4Bar_AdaptiveDI()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar_Adaptive|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_AdaptiveDE", "fea_fourbar_adaptive_1.asm.2");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -i", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(OutputDir, "_FAILED.txt")));
        }

        [Fact]
        public void FEATB_4Bar_CoupledAdaptiveDI()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar_Coupled_Adaptive|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_CoupledAdaptiveDI", "fea_fourbar_coupled_adaptive_1.asm.2");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -i", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(OutputDir, "_FAILED.txt")));
        }

        [Fact]
        public void FEATB_4Bar_CoupledAdaptiveDE()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar_Coupled_Adaptive|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_CoupledAdaptiveDE", "fea_fourbar_coupled_adaptive_1.asm.2");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -e", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(OutputDir, "_FAILED.txt")));
        }

        [Fact]
        public void FEATB_4Bar_ThermalAdaptiveDI()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar_Thermal_Adaptive|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_ThermalAdaptiveDI", "fea_fourbar_thermal_adaptive_1.asm.2");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -i", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(OutputDir, "_FAILED.txt")));
        }

        [Fact]
        public void FEATB_4Bar_ThermalAdaptiveDE()
        {
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar_Thermal_Adaptive|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_ThermalAdaptiveDE", "fea_fourbar_thermal_adaptive_1.asm.2");
            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -e", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(OutputDir, "_FAILED.txt")));
        }

        [Fact]
        public void FEAKinematicTB_4Bar_AdaptiveDI()
        {
            KinematicTB_4Bar();
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar_Adaptive|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_AdaptiveDI_A2A", "fea_fourbar_adaptive_1.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");

            AVM.DDP.MetaTBManifest.Dependency kinDependency = new AVM.DDP.MetaTBManifest.Dependency();
            kinDependency.Type = "KinematicTestBench";
            kinDependency.Directory = Path.Combine(OutputDir, "..\\Kinematic_FourBar");
            AVM.DDP.MetaTBManifest manifest = AVM.DDP.MetaTBManifest.OpenForUpdate(OutputDir);
            manifest.AddDependency(kinDependency);
            manifest.Serialize(OutputDir);

            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -i", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(OutputDir, "_FAILED.txt")));
        }

        [Fact]
        public void FEAKinematicTB_4Bar_AdaptiveDE()
        {
            KinematicTB_4Bar();
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar_Adaptive|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_AdaptiveDE_A2A", "fea_fourbar_adaptive_1.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");

            AVM.DDP.MetaTBManifest.Dependency kinDependency = new AVM.DDP.MetaTBManifest.Dependency();
            kinDependency.Type = "KinematicTestBench";
            kinDependency.Directory = Path.Combine(OutputDir, "..\\Kinematic_FourBar");
            AVM.DDP.MetaTBManifest manifest = AVM.DDP.MetaTBManifest.OpenForUpdate(OutputDir);
            manifest.AddDependency(kinDependency);
            manifest.Serialize(OutputDir);

            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -e", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(OutputDir, "_FAILED.txt")));
        }

        [Fact]
        public void FEAKinematicTB_4Bar_CoupledAdaptiveDE()
        {
            KinematicTB_4Bar();
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar_Coupled_Adaptive|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_CoupledAdaptiveDE_A2A", "fea_fourbar_coupled_adaptive_1.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");

            AVM.DDP.MetaTBManifest.Dependency kinDependency = new AVM.DDP.MetaTBManifest.Dependency();
            kinDependency.Type = "KinematicTestBench";
            kinDependency.Directory = Path.Combine(OutputDir, "..\\Kinematic_FourBar");
            AVM.DDP.MetaTBManifest manifest = AVM.DDP.MetaTBManifest.OpenForUpdate(OutputDir);
            manifest.AddDependency(kinDependency);
            manifest.Serialize(OutputDir);

            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -e", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(OutputDir, "_FAILED.txt")));
        }

        [Fact]
        public void FEAKinematicTB_4Bar_CoupledAdaptiveDI()
        {
            KinematicTB_4Bar();
            int exitcode;
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar_Coupled_Adaptive|kind=CADTestBench|relpos=0";
            string OutputDir = SetupTestAndRunCAD(XmePath, TestbenchPath, "FEA_Fourbar_CoupledAdaptiveDE_A2A", "fea_fourbar_coupled_adaptive_1.asm.2");
            string AnalysisDir = Path.Combine(OutputDir, "Analysis", "Abaqus");

            AVM.DDP.MetaTBManifest.Dependency kinDependency = new AVM.DDP.MetaTBManifest.Dependency();
            kinDependency.Type = "KinematicTestBench";
            kinDependency.Directory = Path.Combine(OutputDir, "..\\Kinematic_FourBar");
            AVM.DDP.MetaTBManifest manifest = AVM.DDP.MetaTBManifest.OpenForUpdate(OutputDir);
            manifest.AddDependency(kinDependency);
            manifest.Serialize(OutputDir);

            Assert.True(RunProcess(fixture.AbaqusBat, "cae noGUI=" + "\"" + fixture.FeaScript + "\" -- -i", OutputDir, out exitcode, 1200000));
            Assert.True(exitcode == 0);
            Assert.True(File.Exists(Path.Combine(OutputDir, "_FAILED.txt")));
        }

        #endregion
*/
        public void SetFixture(FEAKinematicTest.AssemblyAdamsFixture data)
        {
            this.fixture = data;
        }

    }
}
