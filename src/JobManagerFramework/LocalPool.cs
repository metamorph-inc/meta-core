using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.IO;
using System.Diagnostics;
using System.Collections.Concurrent;
using System.ComponentModel;
using System.Management;
using JobManager;

namespace JobManagerFramework
{
    public class LocalPool
    {
        /// <summary>
        /// Each job must generate this file if the execution was failed.
        /// </summary>
        public const string Failed = "_FAILED.txt";

        public int NumAllThread { get; private set; }
        public int NumCommandThread { get; private set; }
        public int NumMatLabThread { get; private set; }
        public int NumCADThread { get; private set; }

        /// <summary>
        /// Task factory.
        /// </summary>
        TaskFactory tf { get; set; }

        /// <summary>
        /// Set when shutdown is requested
        /// </summary>
        private readonly ManualResetEvent ShutdownPool = new ManualResetEvent(false);

        CancellationTokenSource ts { get; set; }
        CancellationToken ct { get; set; }

        public static int GetNumberOfPhysicalCores()
        {
            int coreCount = 0;
            // n.b. requires Vista or later
            foreach (var item in new System.Management.ManagementObjectSearcher("Select * from Win32_Processor").Get())
            {
                coreCount += int.Parse(item["NumberOfCores"].ToString());
            }
            return coreCount;
        }

        /**
         * Creates a new LocalPool with the specified initialThreadCount.  Pass 0 to match the number of physical cores.
         */
        public LocalPool(int initialThreadCount = 0)
        {
            ts = new CancellationTokenSource();
            ct = ts.Token;

            tf = new TaskFactory(
                ct,
                TaskCreationOptions.LongRunning,
                TaskContinuationOptions.None,
                null);

            // GetNumberOfPhysicalCores might take a long time, offload it onto the pool
            Task t = tf.StartNew(() =>
            {
                if (initialThreadCount == 0)
                {
                    initialThreadCount = GetNumberOfPhysicalCores();
                }
                for (int i = 0; i < initialThreadCount; i++)
                {
                    tf.StartNew(JobRunner);
                }
            });
        }

        private bool disposed = false;
        /// <summary>
        /// Kills all currently running/pending tasks.
        /// Releases all resources.
        /// </summary>
        public void Dispose()
        {
            lock (this)
            {
                if (!disposed)
                {
                    disposed = true;
                    ShutdownPool.Set();
                    //ts.Cancel();
                    // n.b. should not timeout, but you can't be too careful.
                    lock (RunningJobs)
                    {
                        Thread t = new Thread(() =>
                        {
                            // n.b. cannot WaitAll on STA thread
                            WaitHandle.WaitAll(RunningJobs.ToArray(), 30 * 1000);
                        });
                        t.SetApartmentState(ApartmentState.MTA);
                        t.Start();
                        t.Join();
                    }
                    ShutdownPool.Dispose();
                }
            }
        }

        public void EnqueueJob(Job j)
        {
            Trace.TraceInformation(string.Format("JobEnqueued in local pool: {0} {1}", j.Id, j.Title));

            QueuedJobs.AddLast(j);
            j.Status = Job.StatusEnum.QueuedLocal;

            JobAdded.Set();
        }

        public bool AbortJob(Job j)
        {
            lock (QueuedJobs)
            {
                Trace.TraceInformation(string.Format("Job Aborted in local pool: {0} {1}", j.Id, j.Title));

                //Find job in queue and remove it if present
                bool foundJob = QueuedJobs.Remove(j);
                if (foundJob)
                {
                    j.Status = Job.StatusEnum.FailedAbortByUser;
                    return true;
                }
                else
                {
                    // Job not in queue; means it must be running
                    var abortJobEvent = GetAbortJobEvent(j);
                    if (abortJobEvent != null)
                    {
                        abortJobEvent.Set();
                        return true;
                    }
                    else
                    {
                        Trace.TraceError("Attempted to abort job that appears to not be running");
                        return false;
                    }
                }
            }
        }

        LinkedList<Job> QueuedJobs = new LinkedList<Job>();
        AutoResetEvent JobAdded = new AutoResetEvent(false);
        List<ManualResetEvent> RunningJobs = new List<ManualResetEvent>();
        Dictionary<Job, ManualResetEvent> AbortJobEvents = new Dictionary<Job, ManualResetEvent>();

        private void JobRunner()
        {
            ManualResetEvent jobIsRunning = new ManualResetEvent(true);
            lock (RunningJobs)
            {
                RunningJobs.Add(jobIsRunning);
            }
            while (ShutdownPool.WaitOne(0) == false)
            {
                Job job;
                job = GetNextJob();
                if (job == null)
                {
                    WaitHandle.WaitAny(new WaitHandle[] { JobAdded, ShutdownPool });
                }
                else
                {
                    jobIsRunning.Reset();
                    RunJob(job);
                    jobIsRunning.Set();
                }
            }
        }

        private void RunJob(Job job)
        {
            try
            {
                // n.b. this could race, but don't even start the job if shutdown is requested
                bool shutdownRequested = this.ShutdownPool.WaitOne(0);
                if (shutdownRequested)
                {
                    job.Status = Job.StatusEnum.FailedExecution;
                    return;
                }

                System.Diagnostics.Process proc0 = new System.Diagnostics.Process();

                System.Diagnostics.ProcessStartInfo psi =
                    new System.Diagnostics.ProcessStartInfo();

                // if (File.Exists(Path.Combine(job.WorkingDirectory, "testbench_manifest.json")))
                // {
                //    psi.FileName = META.VersionInfo.PythonVEnvExe;
                //    psi.Arguments = "-m testbenchexecutor --detailed-errors testbench_manifest.json";
                // }
                // else
                psi.Arguments = "/S /C \"" + job.RunCommand + "\"";
                psi.FileName = "cmd";
                psi.UseShellExecute = false; // true
                psi.CreateNoWindow = true;
                psi.WindowStyle = System.Diagnostics.ProcessWindowStyle.Minimized;
                psi.WorkingDirectory = Path.Combine(job.WorkingDirectory);
                //psi.EnvironmentVariables["PATH"] = META.VersionInfo.PythonVEnvPath + "\\Scripts;" + System.Environment.GetEnvironmentVariable("PATH");

                psi.RedirectStandardOutput = true;
                psi.RedirectStandardError = true;
                psi.RedirectStandardInput = true;
                proc0.StartInfo = psi;

                proc0.EnableRaisingEvents = true;
                ManualResetEvent processExited = new ManualResetEvent(false);
                proc0.Exited += (o, e) =>
                {
                    try
                    {
                        processExited.Set();
                    }
                    catch (System.ObjectDisposedException)
                    {
                    }
                };

                using (processExited)
                using (StreamWriter stderr = new StreamWriter(Path.Combine(job.WorkingDirectory, "stderr.txt")))
                using (StreamWriter stdout = new StreamWriter(Path.Combine(job.WorkingDirectory, "stdout.txt")))
                using (proc0)
                {
                    var jobAborted = new ManualResetEvent(false);
                    AddAbortJobEvent(job, jobAborted);
                    try
                    {
                        proc0.OutputDataReceived += ((sender, e) =>
                        {
                            if (e.Data != null)
                            {
                                try
                                {
                                    stdout.WriteLine(e.Data);
                                }
                                catch (System.ObjectDisposedException)
                                {
                                }
                            }
                        });
                        proc0.ErrorDataReceived += ((sender, e) =>
                        {
                            if (e.Data != null)
                            {
                                try
                                {
                                    stderr.WriteLine(e.Data);
                                }
                                catch (System.ObjectDisposedException)
                                {
                                }
                            }
                        });
                        string failedLog = Path.Combine(job.WorkingDirectory, Failed);
                        File.Delete(failedLog);
                        proc0.Start();
                        job.Status = Job.StatusEnum.RunningLocal;
                        if (psi.RedirectStandardOutput)
                        {
                            // begin read only if it was redirected
                            proc0.BeginOutputReadLine();
                        }
                        if (psi.RedirectStandardError)
                        {
                            // begin read only if it was redirected
                            proc0.BeginErrorReadLine();
                        }
                        //JobStatusChanged.Invoke(job);

                        String commandToShowToUser;
                        if (psi.FileName == META.VersionInfo.PythonVEnvExe)
                        {
                            commandToShowToUser = META.VersionInfo.PythonVEnvExe;
                        }
                        else
                        {
                            commandToShowToUser = job.WorkingDirectory + ": " + job.RunCommand;
                        }
                        proc0.StandardInput.Close();

                        int iWaitHandle =
                            WaitHandle.WaitAny(new WaitHandle[] {processExited, this.ShutdownPool, jobAborted});
                        if (iWaitHandle == 1)
                        {
                            // JobManager is closing
                            try
                            {
                                KillProcessAndChildren(proc0);
                            }
                            catch (System.InvalidOperationException)
                            {
                                // possible race between WaitAny and process exit
                            }
                            using (StreamWriter writer = new StreamWriter(failedLog))
                            {
                                writer.WriteLine("Execution was cancelled due to JobManager exit");
                            }
                            job.Status = Job.StatusEnum.FailedExecution;
                        } else if (iWaitHandle == 2)
                        {
                            // Job aborted by user
                            try
                            {
                                KillProcessAndChildren(proc0);
                            }
                            catch (System.InvalidOperationException)
                            {
                                // possible race between WaitAny and process exit
                            }
                            using (StreamWriter writer = new StreamWriter(failedLog))
                            {
                                writer.WriteLine("Execution was cancelled due to user abort");
                            }
                            job.Status = Job.StatusEnum.FailedAbortByUser;
                        }
                        else if (File.Exists(failedLog))
                        {
                            job.Status = Job.StatusEnum.Failed;
                            //JobStatusChanged.Invoke(job);
                        }
                        else if (proc0.ExitCode != 0)
                        {
                            using (StreamWriter writer = new StreamWriter(failedLog))
                            {
                                writer.WriteLine(String.Format("\"{0}\" exited with non-zero code {1}",
                                    commandToShowToUser, proc0.ExitCode));
                            }
                            job.Status = Job.StatusEnum.FailedExecution;
                        }
                        else
                        {
                            job.Status = Job.StatusEnum.Succeeded;
                        }

                    }
                    catch (Exception ex)
                    {
                        job.Status = Job.StatusEnum.Failed;
                        //JobStatusChanged.Invoke(job);
                        System.Diagnostics.Debug.WriteLine(ex);
                        Trace.TraceError(ex.ToString());
                    }
                    finally
                    {
                        RemoveAndCloseAbortJobEvent(job);
                    }
                }
            }
            catch (Exception ex)
            {
                Trace.TraceError(ex.ToString());
            }
        }

        private Job GetNextJob()
        {
            Job job = null;
            lock (QueuedJobs)
            {
                for (LinkedListNode<Job> node = QueuedJobs.First; node != null; node = node.Next)
                {
                    job = node.Value;
                    QueuedJobs.Remove(node);
                    break;
                }
            }

            return job;
        }

        internal int GetNumberOfUnfinishedJobs()
        {
            lock (QueuedJobs)
            lock (RunningJobs)
            {
                return QueuedJobs.Count + RunningJobs.Where(e => e.WaitOne(0) == false).Count();
            }
        }

        private void AddAbortJobEvent(Job job, ManualResetEvent ev)
        {
            lock (AbortJobEvents)
            {
                AbortJobEvents[job] = ev;
            }
        }

        private void RemoveAndCloseAbortJobEvent(Job job)
        {
            lock (AbortJobEvents)
            {
                var abortJobEvent = GetAbortJobEvent(job);
                if (abortJobEvent != null)
                {
                    AbortJobEvents.Remove(job);
                    abortJobEvent.Close();
                }
            }
        }

        private ManualResetEvent GetAbortJobEvent(Job job)
        {
            lock (AbortJobEvents)
            {
                ManualResetEvent abortJobEvent;

                var success = AbortJobEvents.TryGetValue(job, out abortJobEvent);

                if (success)
                {
                    return abortJobEvent;
                }
                else
                {
                    return null;
                }
            }
        }

        private static void KillProcessAndChildren(Process process)
        {
            KillProcessAndChildren(process.Id);
        }

        /// <summary>
        /// Kill a process, and all of its children, grandchildren, etc.
        /// </summary>
        /// <param name="pid">Process ID.</param>
        private static void KillProcessAndChildren(int pid)
        {
            ManagementObjectSearcher searcher = new ManagementObjectSearcher
              ("Select * From Win32_Process Where ParentProcessID=" + pid);
            ManagementObjectCollection moc = searcher.Get();
            foreach (var mo in moc)
            {
                KillProcessAndChildren(Convert.ToInt32(mo["ProcessID"]));
            }
            try
            {
                Process proc = Process.GetProcessById(pid);
                proc.Kill();
            }
            catch (ArgumentException)
            {
                // Process already exited.
            }
        }
    }
}
