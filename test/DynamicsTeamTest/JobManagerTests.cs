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
                // n.b. Assembly.Location is wrong with Shadow Copy enabled
                 META.VersionInfo.MetaPath + @"bin\JobManager.exe",
                 META.VersionInfo.MetaPath + @"src\JobManager\JobManager\bin\Release\JobManager.exe",
                 META.VersionInfo.MetaPath + @"src\JobManager\JobManager\bin\Debug\JobManager.exe",
            };

        [Fact]
        [Trait("JobManager", "OpenAndClose")]
        public void OpenAndCloseJobManager()
        {
            string exe = searchLocations.Where(File.Exists).FirstOrDefault();
            if (exe == null)
            {
                throw new Exception(string.Format("Job Manager was not found on your computer. Make sure your META installer is healthy. Search locations: {0}", string.Join(", ", searchLocations)));
            }

            Console.Out.WriteLine(exe);

            Process proc = new Process();
            proc.StartInfo.Arguments = "-i";
            proc.StartInfo.UseShellExecute = false;
            proc.StartInfo.FileName = exe;
            proc.StartInfo.RedirectStandardInput = true;
            proc.StartInfo.RedirectStandardOutput = true;
            proc.StartInfo.RedirectStandardError = true;
            ManualResetEvent task = new ManualResetEvent(false);
            using (proc)
            using (task)
            {
                StringBuilder stdoutData = new StringBuilder();
                StringBuilder stderrData = new StringBuilder();
                proc.OutputDataReceived += (o, args) =>
                {
                    if (args.Data != null)
                    {
                        lock (stdoutData)
                        {
                            stdoutData.Append(args.Data);
                            // matches Console.Out.WriteLine("JobManager has started"); in JobManager
                            try
                            {
                                task.Set();
                            }
                            catch (ObjectDisposedException) { }
                        }
                    }
                };
                proc.ErrorDataReceived += (o, args) =>
                {
                    if (args.Data != null)
                    {
                        lock (stderrData)
                        {
                            stderrData.Append(args.Data);
                        }
                    }
                };
                proc.Start();
                proc.BeginErrorReadLine();
                proc.BeginOutputReadLine();
                proc.StandardInput.Close();

                try
                {
                    var tokenSource = new CancellationTokenSource();
                    int timeOut = 10000; // ms
                    if (task.WaitOne(timeOut) == false)
                    {
                        Console.WriteLine("The Task timed out!");
                        Assert.True(false, string.Format("JobManager did not write anything to the standard output after start. Operation timed out after {0}  ms.", timeOut));
                    }
                    lock (stdoutData)
                    {
                        Assert.Contains("JobManager has started", stdoutData.ToString());
                    }
                }
                finally
                {
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
            }
        }
    }
}
