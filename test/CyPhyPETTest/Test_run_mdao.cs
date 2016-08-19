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
                    Arguments = "-m run_mdao mdao_config.json",
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

    public class WorkFlow_PETFixture : DynamicsTeamTest.XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("..\\PET_simple_proof-of-concept", "WorkFlow_PET.xme"); }
        }
    }


    public class PCC_Full_Test : IUseFixture<WorkFlow_PETFixture>
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
                Arguments = "-m run_mdao mdao_config.json",
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
    }
}
