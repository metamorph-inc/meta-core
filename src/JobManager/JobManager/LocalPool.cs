using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.IO;
using System.Diagnostics;

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
        /// Task - Job pairs.
        /// </summary>
        Dictionary<Task, Job> tasks =
            new Dictionary<Task, Job>();

        /// <summary>
        /// Task factory.
        /// </summary>
        TaskFactory tf { get; set; }

        /// <summary>
        /// Global semaphore limits the maximum number of processes.
        /// </summary>
        Semaphore SemAll { get; set; }

        /// <summary>
        /// Semaphores for different type of jobs.
        /// </summary>
        Dictionary<Job.TypeEnum, Semaphore> SemJob { get; set; }

        /// <summary>
        /// Hold a reference to the currently running/finished user processes.
        /// </summary>
        Dictionary<Job, System.Diagnostics.Process> processes =
            new Dictionary<Job, System.Diagnostics.Process>();

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

            SemAll = new Semaphore(numAllThread, numAllThread);
            SemJob = new Dictionary<Job.TypeEnum, Semaphore>();
            SemJob.Add(Job.TypeEnum.Command, new Semaphore(numCommandThread, numCommandThread));
            SemJob.Add(Job.TypeEnum.Matlab, new Semaphore(numMatLabThread, numMatLabThread));
            SemJob.Add(Job.TypeEnum.CAD, new Semaphore(numCADThread, numCADThread));
        }

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
                    // n.b. should not timeout, but you can't be too careful.
                    Task.WaitAll(tasks.Keys.ToArray(), 30 * 1000);
                    ShutdownPool.Dispose();
                }
            }
        }

        public void EnqueueJob(Job j)
        {
            Trace.TraceInformation(string.Format("JobEnqueued in local pool: {0} {1}", j.Id, j.Title));

            Task t = tf.StartNew(() => RunJob(j), ct).
                ContinueWith(job => Done(job));


            tasks.Add(t, j);

            //Task t = new Task<Job>(() => RunJob(j)).ContinueWith(job => Done(job));

        }

        private Job RunJob(Job job)
        {
            try
            {
                job.Status = Job.StatusEnum.QueuedLocal;
                SemJob[job.Type].WaitOne();
                SemAll.WaitOne();
                // n.b. this could race, but don't even start the job if shutdown is requested
                bool shutdownRequested = this.ShutdownPool.WaitOne(1);
                if (shutdownRequested)
                {
                    job.Status = Job.StatusEnum.FailedExecution;
                    return job;
                }

                System.Diagnostics.Process proc0 = new System.Diagnostics.Process();
                lock (processes)
                {
                    if (processes.ContainsKey(job))
                    {
                        processes.Remove(job);
                    }

                    processes.Add(job, proc0);
                }
                proc0.Exited += new EventHandler((o, args) =>
                {
                    lock (processes)
                    {
                        processes.Remove(job);
                    }
                });

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
                SemAll.Release();
                SemJob[job.Type].Release();
            }
            return job;
        }


        private void Done(Task<Job> job)
        {
            if (job.Status == TaskStatus.Faulted)
            {
                tasks[job].Status = Job.StatusEnum.FailedExecution;
                //JobStatusChanged.Invoke(job.Result);
            }
            else
            {
                if (job.Result.Status == Job.StatusEnum.RunningLocal)
                {
                    if (job.IsCompleted)
                    {
                        job.Result.Status = Job.StatusEnum.Succeeded;
                    }
                    else if (job.IsFaulted ||
                        job.IsCanceled)
                    {
                        job.Result.Status = Job.StatusEnum.FailedExecution;
                    }
                    //JobStatusChanged.Invoke(job.Result);
                }
            }
        }


        internal int GetNumberOfUnfinishedJobs()
        {
            return tasks.Keys.Where(task => task.IsCompleted == false).Count();
        }
    }
}
