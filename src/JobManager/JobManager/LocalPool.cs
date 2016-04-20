using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.IO;
using System.Diagnostics;
using System.Collections.Concurrent;

namespace JobManager
{
    internal class LocalPool
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


        public LocalPool(
            int numCommandThread = 4,
            int numMatLabThread = 4,
            int numCADThread = 2)
        {
            ts = new CancellationTokenSource();
            ct = ts.Token;

            tf = new TaskFactory(
                ct,
                TaskCreationOptions.LongRunning,
                TaskContinuationOptions.None,
                null);

            int numAllThread = numCommandThread + numMatLabThread + numCADThread;

            // do not use more threads than cores
            numAllThread = numAllThread < Environment.ProcessorCount ?
                numAllThread :
                Environment.ProcessorCount;

            NumAllThread = numAllThread;
            NumCommandThread = numCommandThread;
            NumMatLabThread = numMatLabThread;
            NumCADThread = numCADThread;

            lock (QueuedJobs)
            {
                JobCapacities.Add(Job.TypeEnum.Command, numCommandThread);
                JobCapacities.Add(Job.TypeEnum.Matlab, numMatLabThread);
                JobCapacities.Add(Job.TypeEnum.CAD, numCADThread);
            }
            for (int i = 0; i < numAllThread; i++)
            {
                Task t = tf.StartNew(JobRunner);
            }
        }
        Dictionary<Job.TypeEnum, int> JobCapacities = new Dictionary<Job.TypeEnum, int>();

        ~LocalPool()
        {
            Dispose();
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
                    ts.Cancel();
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
            JobAdded.Set();
        }

        LinkedList<Job> QueuedJobs = new LinkedList<Job>();
        AutoResetEvent JobAdded = new AutoResetEvent(false);
        List<ManualResetEvent> RunningJobs = new List<ManualResetEvent>();

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
                    JobAdded.WaitOne();
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

                psi.Arguments = "/S /C \"" + job.RunCommand + "\"";
                psi.CreateNoWindow = true;
                psi.FileName = "cmd";
                psi.UseShellExecute = false; // true
                psi.WindowStyle = System.Diagnostics.ProcessWindowStyle.Minimized;
                psi.WorkingDirectory = Path.Combine(job.WorkingDirectory);

                psi.RedirectStandardOutput = true;
                psi.RedirectStandardError = true;
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

                        int iWaitHandle = WaitHandle.WaitAny(new WaitHandle[] { processExited, this.ShutdownPool });
                        if (iWaitHandle == 1)
                        {
                            // JobManager is closing
                            try
                            {
                                proc0.Kill();
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
                        }
                        else if (File.Exists(failedLog))
                        {
                            job.Status = Job.StatusEnum.Failed;
                            //JobStatusChanged.Invoke(job);
                        }
                        else if (proc0.ExitCode == 1)
                        {
                            // command not found
                            using (StreamWriter writer = new StreamWriter(failedLog))
                            {
                                writer.WriteLine("Specified command was not found: {0}",
                                    Path.Combine(job.WorkingDirectory, job.RunCommand));
                            }
                            job.Status = Job.StatusEnum.FailedExecution;
                            //JobStatusChanged.Invoke(job);
                        }
                        else if (proc0.ExitCode != 0)
                        {
                            using (StreamWriter writer = new StreamWriter(failedLog))
                            {
                                writer.WriteLine(String.Format("\"{0}\" exited with non-zero code {1}", job.RunCommand, proc0.ExitCode));
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
            }
            catch (Exception ex)
            {
                Trace.TraceError(ex.ToString());
            }
            finally
            {
                lock (QueuedJobs)
                {
                    JobCapacities[job.Type]++;
                }
            }
        }

        private Job GetNextJob()
        {
            Job job = null;
            lock (QueuedJobs)
            {
                for (LinkedListNode<Job> node = QueuedJobs.First; node != null; node = node.Next)
                {
                    if (JobCapacities[node.Value.Type] > 0)
                    {
                        JobCapacities[node.Value.Type]--;
                        job = node.Value;
                        QueuedJobs.Remove(node);
                        break;
                    }
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
    }
}
