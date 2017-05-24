using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using Xunit;

using GME.MGA;
using GME.CSharp;
using System.Text.RegularExpressions;
using System.IO;
using System.Threading.Tasks;
using System.Threading;

namespace ModelTest
{
    public class Test
    {
        [Fact]
        [Trait("Model", "Box")]
        public void BoxModelImport()
        {
            var p_BoxModel = Path.Combine(META.VersionInfo.MetaPath,
                                          "models",
                                          "Box");
            var p_BoxModelXME = Path.Combine(p_BoxModel,
                                             "box.xme");
            var p_BoxModelMGA = p_BoxModelXME.Replace("box.xme", "box_import_test.mga");


            Assert.True(File.Exists(p_BoxModelXME), "box.xme not found at expected location: " + p_BoxModelXME);

            // Delete existing mga
            try
            {
                File.Delete(p_BoxModelMGA);
            }
            catch (FileNotFoundException)
            {
                // it's okay
            }

            GME.MGA.MgaUtils.ImportXME(p_BoxModelXME, p_BoxModelMGA);
            Assert.True(File.Exists(p_BoxModelMGA), "MGA file not found. Import may have failed.");
        }

        [Fact]
        [Trait("Model", "All")]
        public void TestAllCyPhyModels()
        {
            // Find all XMEs in the solution.
            // Try to import each one.
            // If one or more fail to import, fail the test and list the failing models.

            var proc = System.Diagnostics.Process.Start(new ProcessStartInfo()
            {
                Arguments = "ls-files **.xme",
                FileName = "git",
                WorkingDirectory = META.VersionInfo.MetaPath,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false,
                CreateNoWindow = true
            });
            proc.EnableRaisingEvents = true;
            ManualResetEvent stdoutDone = new ManualResetEvent(false);
            List<string> lines = new List<string>();
            proc.OutputDataReceived += (s, e) =>
            {
                if (e.Data != null)
                {
                    lock (this)
                    {
                        lines.Add(e.Data);
                    }
                }
                else
                {
                    stdoutDone.Set();
                }
            };
            proc.BeginOutputReadLine();
            var exited = proc.WaitForExit(300 * 1000);
            if (!exited)
            {
                proc.Kill();
                lock (this)
                {
                    Console.Out.WriteLine(String.Join("\n", lines));
                }
                Console.Out.WriteLine(proc.StandardError.ReadToEnd());
            }
            Assert.True(exited);
            Assert.Equal(0, proc.ExitCode);
            stdoutDone.WaitOne();

            IEnumerable<string> xmes;
            lock (this)
            {
                xmes = lines.Select(line => Path.Combine(META.VersionInfo.MetaPath, line));
            }

            var failures = new ConcurrentDictionary<String, String>();
            var path_temporary = Path.Combine(META.VersionInfo.MetaPath,
                "test",
                "ModelTest",
                "temp");

            if (Directory.Exists(path_temporary))
            {
                Directory.Delete(path_temporary, true);
            }
            Directory.CreateDirectory(path_temporary);

            Parallel.ForEach(xmes, new ParallelOptions()
            {
                // FIXME: due to GME bug, parallelism must be limited. Remove when using GME after 5/17/2017
                MaxDegreeOfParallelism = 1
            },
            xme =>
            {
                // Because only failed imports are of interest,
                // and failed imports result in empty MGAs,
                // it's not important to be able to find the MGA
                // that results from a failed import.
                // So we use a random name.
                var mgaPath = Path.Combine(path_temporary, Path.GetRandomFileName());

                try
                {
                    GME.MGA.MgaUtils.ImportXME(xme, mgaPath);
                }
                catch (Exception ex)
                {
                    var reason = ex.ToString();
                    if (reason.Contains("Paradigm 'ESMoL' is not registered"))
                    {
                        // This is okay, we don't expect it to be registered.
                    }
                    else if (reason.Contains("Paradigm 'CyberComposition' is not registered"))
                    {
                        // This is okay, we don't expect it to be registered.
                    }
                    else if (reason.Contains("Paradigm 'EBG' is not registered"))
                    {
                        // This is okay, we don't expect it to be registered.
                    }
                    else if (reason.Contains("Paradigm 'SCAMLA' is not registered"))
                    {
                        // This is okay, we don't expect it to be registered.
                    }
                    else
                    {
                        // This is a failure we're concerned about.
                        failures[xme] = ex.ToString();
                    }
                }
            });

            if (failures.Any())
            {
                var msg = String.Format("{0} of {1} XME file(s) failed to import" + Environment.NewLine,
                                        failures.Count,
                                        xmes.Count());
                foreach (var entry in failures)
                {
                    msg += String.Format("{0}{2}{1}{2}{2}",
                        entry.Key,
                        entry.Value,
                        Environment.NewLine);
                }
                Assert.False(true, msg);
            }
        }
    }
}
