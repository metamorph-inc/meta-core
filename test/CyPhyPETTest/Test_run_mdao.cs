using GME.MGA;
using META;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Text.RegularExpressions;
using Xunit;

namespace CyPhyPETTest
{
    public class Test_run_mdao
    {
        [Fact]
        public void TestPCC_run_mdao()
        {
            string assemblyDir = Path.GetDirectoryName(new Uri(Assembly.GetExecutingAssembly().CodeBase).LocalPath);
            string PCC_run_mdao_dir = Path.Combine(assemblyDir, @"..\..\..\..\src\Python27Packages\PCC\PCC\test\PCC_run_mdao");
            string manifestPath = Path.Combine(PCC_run_mdao_dir, "testbench_manifest.json");
            var manifest = File.ReadAllBytes(manifestPath);

            try
            {

                ProcessStartInfo info = new ProcessStartInfo()
                {
                    FileName = VersionInfo.PythonVEnvExe,
                    WorkingDirectory = PCC_run_mdao_dir,
                    Arguments = "-E -m run_mdao mdao_config.json",
                    UseShellExecute = false,
                    CreateNoWindow = true,
                    RedirectStandardError = true,
                    RedirectStandardOutput = true
                };

                Process proc = new Process();
                proc.StartInfo = info;

                proc.Start();
                var stdout = proc.StandardOutput.ReadToEnd();
                var stderr = proc.StandardError.ReadToEnd();
                stdout = stdout.Replace("\r", "");

                Assert.True(0 == proc.ExitCode, "run_mdao failed: " + stderr);

                Action<string> match = regex =>
                {
                    Assert.True(Regex.Match(stdout, regex).Success, String.Format("Output did not match regex '{0}'  {1}", regex, stdout));
                };

                match("Taylor Series:.*\n.*\\b11.*\\b110.*\n.*\\b6.*\\b60");
                match("Correlation:.*\n.*\\b0\\.9999.*\\b0\\.9999");
                match("Mean.*\\b66\\b.*\\b660\\b");
                match("PCC.*\nComplexity estimate.*\nDone!");
            }
            finally
            {
                File.WriteAllBytes(manifestPath, manifest);
            }
        }
    }

    public class Test_Code_Parameters
    {
        [Fact]
        public void TestCodeParameters()
        {
            Dictionary<string, object> assignment = CyPhyPET.PET.getPythonAssignment("x = 1\ny = '2'");
            Assert.Equal(1L, assignment["x"]);
            Assert.Equal("2", assignment["y"]);
        }
    }

    public class WorkFlow_PETFixture : DynamicsTeamTest.XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("..\\PET_simple_proof-of-concept", "WorkFlow_PET.xme"); }
        }
    }


    public class Workflow_PET_Test : IUseFixture<WorkFlow_PETFixture>
    {
        private string mgaFile;

        [Fact]
        public void TestPython()
        {
            string outputDir = "TestPython";
            string petExperimentPath = "/@Testing/@ParametricExploration/@TestPython";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath);

            Assert.True(result.Item2.Success, "CyPhyPET failed.");
            return;

            string stderr = "<did not start process>";
            int retcode = Run(result.Item2.RunCommand, result.Item1.OutputDirectory, out stderr);
            Assert.True(0 == retcode, "run_mdao failed: " + stderr);
        }

        [Fact]
        public void Test_CyPhyPET_unit_matcher()
        {
            var project = new MgaProject();
            project.OpenEx("MGA=" + this.mgaFile, "CyPhyML", null);
            try
            {
                project.BeginTransactionInNewTerr();
                try
                {
                    var wrapper = project.RootFolder.ObjectByPath["/@Testing/@ParametricExploration/@TestPython/@PythonWrapper"];
                    var pythonBlock = ISIS.GME.Dsml.CyPhyML.Classes.PythonWrapper.Cast(wrapper);

                    var binDir = Path.GetDirectoryName(System.Reflection.Assembly.GetAssembly(typeof(Workflow_PET_Test)).CodeBase.Substring("file:///".Length));
                    var path = Path.GetFullPath(binDir + "/../../paraboloid.py");
                    var pet = CyPhyPET.CyPhyPETInterpreter.GetParamsAndUnknownsForPythonOpenMDAO(path, pythonBlock);
                    
                    // "{\"unknowns\": {\"f_xy\": {\"units\": \"m**3\", \"gme_unit_id\": \"id-0065-00000179\", \"shape\": 1, \"val\": 0.0, \"size\": 1}}, \"params\": {\"x\": {\"units\": \"m\", \"gme_unit_id\": \"id-0066-00000014\", \"shape\": 1, \"val\": 0.0, \"size\": 1}, \"y\": {\"units\": \"m**3\", \"gme_unit_id\": \"id-0065-00000179\", \"shape\": 1, \"val\": 0.0, \"size\": 1}}}"
                    var unknowns = pet["unknowns"];
                    var params_ = pet["params"];
                    var x = params_["x"];
                    var y = params_["y"];
                    var f_xy = unknowns["f_xy"];
                    Assert.Equal("Meter", project.GetFCOByID((string)x["gme_unit_id"]).Name);
                    Assert.Equal("Cubic Meter", project.GetFCOByID((string)y["gme_unit_id"]).Name);
                    Assert.Equal("Cubic Meter", project.GetFCOByID((string)f_xy["gme_unit_id"]).Name);
                }
                finally
                {
                    project.CommitTransaction();
                }
            }
            finally
            {
                project.Close(abort: true);
            }
        }

        public void SetFixture(WorkFlow_PETFixture data)
        {
            mgaFile = data.mgaFile;
        }

        public int Run(string runCommand, string outputDir, out string stderr)
        {
            ProcessStartInfo info = new ProcessStartInfo()
            {
                FileName = VersionInfo.PythonVEnvExe,
                WorkingDirectory = outputDir,
                Arguments = "-E -m run_mdao mdao_config.json",
                UseShellExecute = false,
                CreateNoWindow = true,
                RedirectStandardError = true,
                RedirectStandardOutput = true
            };

            Process proc = new Process();
            proc.StartInfo = info;

            proc.Start();
            string err = "";

            proc.ErrorDataReceived += ((sender, e) =>
            {
                if (e.Data != null)
                {
                    try
                    {
                        err = err + e.Data;
                    }
                    catch (System.ObjectDisposedException)
                    {
                    }
                }
            });
            proc.BeginErrorReadLine();
            var stdout = proc.StandardOutput.ReadToEnd();
            proc.WaitForExit();
            stdout = stdout.Replace("\r", "");
            stderr = err.Replace("\r", "");

            return proc.ExitCode;

        }


        static void Main(string[] args)
        {
            int ret = Xunit.ConsoleClient.Program.Main(new string[]
            {
                System.Reflection.Assembly.GetAssembly(typeof(Workflow_PET_Test)).CodeBase.Substring("file:///".Length),
                //"/noshadow",
            });
            Console.In.ReadLine();
            //System.Console.Out.WriteLine("HEllo World");
        }
    }
}
