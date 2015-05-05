// -----------------------------------------------------------------------
// <copyright file="JobManagerTests.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace DynamicsTeamTest
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using Xunit;
    using System.IO;
    using System.Diagnostics;
    using System.Threading;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class JobManagerTests
    {
        private List<string> searchLocations = new List<string>()
            {
                 META.VersionInfo.MetaPath + @"bin\JobManager.exe",
                 META.VersionInfo.MetaPath + @"src\JobManager\JobManager\bin\Release\JobManager.exe",
                 META.VersionInfo.MetaPath + @"src\JobManager\JobManager\bin\Debug\JobManager.exe",
            };

        [Fact]
        [Trait("JobManager", "OpenAndClose")]
        public void OpenAndCloseJobManager()
        {
            Assert.DoesNotThrow(() =>
            {
                var notFound = true;

                // n.b. Assembly.Location is wrong with Shadow Copy enabled
                foreach (var exe in searchLocations)
                {
                    if (File.Exists(exe))
                    {
                        Console.Out.WriteLine(exe);

                        Process proc = new Process();
                        proc.StartInfo.Arguments = "-i";
                        proc.StartInfo.UseShellExecute = false;
                        proc.StartInfo.FileName = exe;
                        proc.StartInfo.RedirectStandardOutput = true;
                        proc.StartInfo.RedirectStandardError = true;
                        if (proc.Start())
                        {
                            proc.WaitForInputIdle(10 * 1000);

                            var tokenSource = new CancellationTokenSource();
                            CancellationToken token = tokenSource.Token;
                            int timeOut = 10000; // ms

                            var task = System.Threading.Tasks.Task.Factory.StartNew(() => {
                                // call the readline using a timeout and a cancellation token
                                // FIXME: unfortunately this still succeeds even the widnows firewall is not enabled.
                                string status = proc.StandardOutput.ReadLine(); // matches Console.Out.WriteLine("JobManager has started"); in JobManager

                                Console.Out.WriteLine(status);

                                if (status.Contains("failed") /* "JobManager failed to start" */)
                                {
                                    throw new Exception("Job Manager failed to start.");
                                }
                            }, token);

                            if (task.Wait(timeOut, token) == false)
                            {
                                Console.WriteLine("The Task timed out!");
                                Assert.True(false, string.Format("JobManager did not write anything to the standard output after start. Operation timed out after {0}  ms.", timeOut));
                            }

                            if (proc.HasExited == false)
                            {
                                // successfully opened now kill it
                                try
                                {
                                    proc.Kill();
                                }
                                catch (System.InvalidOperationException) { } // possible race with proc.HasExited
                            }
                        }
                        else
                        {
                            throw new Exception("Job Manager failed to start.");
                        }
                        notFound = false;

                        break;
                    }
                }

                if (notFound)
                {
                    throw new Exception(string.Format("Job Manager was not found on your computer. Make sure your META installer is healthy. Search locations: {0}", string.Join(", ", searchLocations)));
                }
            });
        }
    }
}
