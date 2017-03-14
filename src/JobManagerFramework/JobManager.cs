using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Data.SQLite;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.NetworkInformation;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Security;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using JobManager;
using System.Runtime.Remoting.Channels.Ipc;

namespace JobManagerFramework
{
    public class JobManager : IDisposable
    {
        public class JobAddedEventArgs : EventArgs
        {
            public Job Job { get; set; }
            public Job.StatusEnum Status { get; set; }
        }

        /**
         * Invoked when a new job is submitted to the job server.
         * 
         * May be raised on an arbitrary thread; code that interacts with UI is responsible
         * for scheduling on the UI thread.
         */
        public event EventHandler<JobAddedEventArgs> JobAdded;

        public class SotAddedEventArgs : EventArgs
        {
            public SoT Sot { get; set; }
        }

        /**
         * Invoked when a new SoT is submitted to the job server.
         */
        public event EventHandler<SotAddedEventArgs> SotAdded;

        /**
         * Gets the server-side list of jobs.
         */
        public IList<Job> Jobs
        {
            get { return Server.Jobs; }
        }

        /**
         * Returns true if the server is configured for remote execution.
         */
        public bool IsRemote { get { return Server.IsRemote; } }

        /**
         * Returns true if there are SoTs which haven't posted yet
         */
        public bool HasIncompleteSots
        {
            get
            {
                return this.SoTs.Any(s => s.TestBenchJobMap.Any(j => j.Value.Status == Job.StatusEnum.Ready &&
                                                                     ExtensionMethods.Closure(j.Key,
                                                                             sot => sot.UpstreamTestBenches)
                                                                         .All(
                                                                             tb =>
                                                                                 s.TestBenchJobMap[tb].IsFailed() ==
                                                                                 false)));
            }
        }

        /**
         * Returns the count of unfinished jobs
         */

        public int UnfinishedJobCount
        {
            get { return this.pool.GetNumberOfUnfinishedJobs(); }
        }

        private IpcServerChannel ServerChannel { get; set; }
        private JobServerImpl Server { get; set; }

        private string TempDir { get; set; }

        private System.Threading.ManualResetEvent Configured = new ManualResetEvent(false);
        private BlockingCollection<Action> JobsToBeStarted = new BlockingCollection<Action>(new ConcurrentQueue<Action>());
        private System.Threading.Thread JobsToBeStartedThread { get; set; }

        private Thread JobMonitorThread;
        private AutoResetEvent JobMonitorEvent = new AutoResetEvent(false);

        Thread SoTWorkerThread = null;
        System.Collections.Concurrent.BlockingCollection<Action> SoTTodo = new System.Collections.Concurrent.BlockingCollection<Action>();

        List<SoTImpl> SoTs = new List<SoTImpl>();

        private JobManagerEntities1 Entities;

        /**
         * Job manager application settings.  Applications using the JobManager should populate
         * these settings from runtime configuration specified by the user.
         */
        public class JobManagerConfiguration
        {
            public string VehicleForgeUri { get; set; }
            public string SshHost { get; set; }
            public string SshUser { get; set; }
            public string SshPort { get; set; }
            public bool RemoteExecution { get; set; }
            public bool WipeWorkspaceOnSuccess { get; set; }
            public bool DeleteJobOnSuccess { get; set; }
            public string UserId { get; set; }
            public string JenkinsUri { get; set; }

            public JobManagerConfiguration()
            {
                // Initialize to defaults (taken from the default settings on the original JobManager)
                VehicleForgeUri = "";
                SshHost = "129.59.105.248";
                SshUser = "meta";
                SshPort = "8080";
                RemoteExecution = false;
                WipeWorkspaceOnSuccess = true;
                DeleteJobOnSuccess = true;
                UserId = "";
                JenkinsUri = "";
            }
        }

        public JobManager.JobManagerConfiguration Configuration { get; }

        Jenkins.Jenkins JenkinsInstance;
        public class RemoteJobInfo
        {
            public string JenkinsJobName { get; set; }
            public string ResultsGetURL;
        }
        ConcurrentDictionary<Job, RemoteJobInfo> JobMap;

        public int LocalConcurrentThreads
        {
            get { return pool.NumAllThread; }

            set
            {
                if (pool.GetNumberOfUnfinishedJobs() != 0)
                {
                    throw new InvalidPoolStateException(
                        "Cannot change number of concurrent threads while jobs are running.");
                }
                else
                {
                    //Create a new local pool with the new thread count
                    var oldPool = pool;
                    pool = new LocalPool(value);
                    oldPool.Dispose();
                }
            }
        }

        /// <summary>
        /// Local thread pool for jobs.
        /// </summary>
        private LocalPool pool;

        private int highPriorityJobsRemaining = 0;

        public JobManager(int preferredPort, JobManagerConfiguration configuration, int localConcurrentThreads = 0)
        {
            pool = new LocalPool(localConcurrentThreads);
            Configuration = configuration;
            InitializeChannels(preferredPort);
            InitializePersistence();

            TempDir = Path.Combine(
                System.IO.Path.GetTempPath(),
                Path.Combine("META", "JobManager"));

            JenkinsInstance = new Jenkins.Jenkins(this.TempDir);
            JobMap = new ConcurrentDictionary<Job, RemoteJobInfo>();

            this.JobsToBeStartedThread = new System.Threading.Thread(this.JobsToBeStartedCleaner);
            this.JobsToBeStartedThread.Name = "JobStarter";
            this.JobsToBeStartedThread.IsBackground = true;
            this.JobsToBeStartedThread.Start();

            LoadSavedJobs();
        }

        ~JobManager()
        {
            this.Dispose();
        }

        public static readonly string ServerName = "JobServer";
        public static readonly string ChannelName = "MetaJobManager";

        private void InitializeChannels(int preferredPort)
        {
            var provider = new BinaryServerFormatterSinkProvider();
            provider.TypeFilterLevel = System.Runtime.Serialization.Formatters.TypeFilterLevel.Full;

            ServerChannel = new IpcServerChannel(ServerName, ChannelName, provider);
            ChannelServices.RegisterChannel(ServerChannel, false);

            Server = new JobServerImpl();
            RemotingServices.Marshal(Server, ServerName);

            Server.JobAdded += JobAddedHandler;
            Server.SoTAdded += SoTAddedHandler;
            Server.handlersAdded.Set(); // TODO: should we wait until UI has added handlers before setting this?
        }

        private void InitializePersistence()
        {
            string dbPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "JobManager.db");
            // entities = new JobManagerEntities1(String.Format("Data Source={0};Version=3;", dbPath));
            SQLiteConnection cnn = new SQLiteConnection(String.Format("data source={0}", dbPath));
            var command = cnn.CreateCommand();
            cnn.Open();
            command.CommandText = @"CREATE TABLE IF NOT EXISTS [SavedJobs] (
                [Title] text NOT NULL,
                [WorkingDirectory] text NOT NULL,
                [RunCommand] text NOT NULL,
                [Status] text NOT NULL,
                [JobName] text PRIMARY KEY NOT NULL
                );";
            command.ExecuteNonQuery();
            command.CommandText = @"PRAGMA user_version;";
            Int64 dbVersion = (Int64)command.ExecuteScalar();
            if (dbVersion == 0)
            {
                command.CommandText = @"ALTER TABLE SavedJobs ADD COLUMN VFUrl TEXT;";
                command.ExecuteNonQuery();
                command.CommandText = @"PRAGMA user_version = 1;";
                command.ExecuteNonQuery();
            }
            cnn.Close();

            // This call needs the Sqlite provider to be present in consumer's app.config--  see original job manager or PET browser for example
            Entities = new JobManagerEntities1(String.Format("metadata=res://*/SavedJobs.csdl|res://*/SavedJobs.ssdl|res://*/SavedJobs.msl;provider=System.Data.SQLite;provider connection string=\"data source={0}\"", dbPath));
            // Fail fast
            Entities.SavedJobs.ToList();
        }

        private void LoadSavedJobs()
        {
            lock (this)
            {
                this.Configured.Set();
                if (Server.IsRemote)
                {
                    // retreiving saved jobs and adding to the job queue and jobmap
                    foreach (SavedJob saved in Entities.SavedJobs)
                    {
                        if (string.IsNullOrEmpty(saved.VFUrl) == false && Server.JenkinsUrl != saved.VFUrl)
                            continue;
                        Job job = new JobImpl(Server)
                        {
                            Title = saved.Title,
                            Status = Job.StatusEnum.StartedOnServer, // TODO: wrong, but not a big deal, will be re-read from the server
                            RunCommand = saved.RunCommand,
                            WorkingDirectory = saved.WorkingDirectory,
                        };
                        try
                        {
                            Directory.CreateDirectory(saved.WorkingDirectory);
                        }
                        catch (Exception e)
                        {
                            Trace.TraceError(e.ToString());
                        }
                        var jenkinsJobInfo = JenkinsInstance.GetJobInfo(saved.JobName);
                        if (jenkinsJobInfo == null)
                        {
                            Trace.TraceInformation("Saved job {0} ({1}) no longer exists on server. Marking failed.", job.Title, jenkinsJobInfo);
                            job.Status = Job.StatusEnum.FailedAbortOnServer;
                            Entities.SavedJobs.DeleteObject(saved);
                        }
                        else
                        {
                            Trace.TraceInformation("Adding saved job {0} ({1})", job.Title, jenkinsJobInfo);
                            ((JobImpl)job).remoteInfo = JobMap[job] = new RemoteJobInfo() { JenkinsJobName = jenkinsJobInfo.name };
                            Server.AddJob(job);
                            Entities.SavedJobs.Detach(saved);
                        }
                    }
                    Entities.SaveChanges();

                    // start timer for remote execution monitoring
                    this.RestartMonitorTimer();
                }
            }
        }

        private void RestartMonitorTimer()
        {
            if (JobMonitorThread == null)
            {
                Trace.TraceInformation("Starting JobMonitor for remote jobs due time: {0}.");
                JobMonitorThread = new System.Threading.Thread(MonitorJenkinsJobs);
                JobMonitorThread.Name = "JobMonitor";
                JobMonitorThread.IsBackground = true;
                JobMonitorThread.Start();
            }
            else
            {
                Trace.TraceInformation("Set JobMonitor for now");
                JobMonitorEvent.Set();
            }
        }

        private bool disposed = false;
        public void Dispose()
        {
            if (!disposed)
            {
                disposed = true;
                if (ChannelServices.RegisteredChannels.Contains(ServerChannel))
                {
                    ChannelServices.UnregisterChannel(ServerChannel);
                }

                SoTTodo.Add(null); //signals SOT thread to terminate
                this.pool.Dispose();
            }
        }

        public class ServiceStatus
        {
            public bool isRemoteServer = false;
            public bool vfConnected = false;
            public bool jenkinsConnected = false;
            public Jenkins.StatusInfo.StatusInfo statusInfo = null;
        }

        /**
         * Gets service status of remote servers.
         */
        public ServiceStatus GetServiceStatus()
        {
            var result = new ServiceStatus();

            if (Server.IsRemote == false)
            {
                result.isRemoteServer = false;
                return result;
            }
            result.isRemoteServer = true;

            Trace.TraceInformation("Pinging VF and getting status");

            Jenkins.UserProfile.UserProfile info = null;

            if (JenkinsInstance.PingVF(out info))
            {
                result.vfConnected = true;
                if (JenkinsInstance.PingJenkins(out result.statusInfo))
                {
                    result.jenkinsConnected = true;
                }
            }

            return result;
        }

        /**
         * Retries the download for selected jobs, if download failed
         */
        public void RetryDownload(IEnumerable<Job> jobs)
        {
            var toRedownload = jobs.Where(x => x.Status == Job.StatusEnum.FailedToDownload);

            foreach (var selectedJob in toRedownload)
            {
                selectedJob.Status = Job.StatusEnum.RedownloadQueued;
            }

            RestartMonitorTimer();
        }

        /**
         * Aborts selected jobs on server
         */
        public void AbortJobs(IEnumerable<Job> jobs)
        {
            
            var toAbort = jobs.Where(x => new Job.StatusEnum[] {
                    Job.StatusEnum.RunningOnServer, Job.StatusEnum.StartedOnServer, Job.StatusEnum.QueuedOnServer, Job.StatusEnum.RunningLocal, Job.StatusEnum.QueuedLocal, Job.StatusEnum.Ready }.Contains(x.Status));

            foreach (var selectedJob in toAbort)
            {
                if (new Job.StatusEnum[]
                {
                    Job.StatusEnum.RunningOnServer, Job.StatusEnum.StartedOnServer, Job.StatusEnum.QueuedOnServer
                }.Contains(selectedJob.Status))
                {
                    // Cancel remote execution
                    // selectedJob.SubItems[Headers.Status.ToString()].Text = "Redownload Queued";
                    selectedJob.Status = Job.StatusEnum.AbortOnServerRequested;
                }
                else
                {
                    // Cancel local execution
                    pool.AbortJob(selectedJob);
                }
            }
            RestartMonitorTimer();
        }

        /**
         * Re-runs selected jobs on server
         */
        public void ReRunJobs(IEnumerable<Job> jobs)
        {
            var toReRun = jobs.Cast<JobImpl>().Where(x =>
                (x.IsFailed() || x.Status == Job.StatusEnum.Succeeded) &&
                x.RerunEnabled);

            foreach (var selectedJob in toReRun)
            {
                try
                {
                    // run it again
                    selectedJob.Status = Job.StatusEnum.WaitingForStart;
                    selectedJob.Start();
                    Trace.TraceInformation("Rerunning job: {0} {1}", selectedJob.Id, selectedJob.Title);
                }
                catch (Exception ex)
                {
                    Trace.TraceError(ex.ToString());
                }
            }
        }

        /**
         * Makes specified jobs high priority on server
         */

        public void MakeHighPriority(IEnumerable<Job> jobs)
        {
            var toMakeHighPriority = jobs.Where(x => new Job.StatusEnum[] {
                    Job.StatusEnum.PostedToServer, Job.StatusEnum.StartedOnServer, Job.StatusEnum.QueuedOnServer }.Contains(x.Status));

            foreach (var selectedJob in toMakeHighPriority)
            {
                RemoteJobInfo remoteJob;
                if (JobMap.TryGetValue(selectedJob, out remoteJob))
                {
                    // TODO: would be good to do this on another thread
                    JenkinsInstance.MakeHighPriority(remoteJob.JenkinsJobName);
                    highPriorityJobsRemaining--;
                    // called primarly for UpdateServiceStatus, which will update the status bar
                    RestartMonitorTimer();
                }
            }
        }

        private void JobAddedHandler(JobImpl job, Job.StatusEnum status)
        {
            lock (this)
            {
                job.JobStatusChanged += JobStatusChanged;
                if (job.Status == Job.StatusEnum.Ready)
                {
                    JobsToBeStarted.Add(() => { StartJob(job); });
                }
            }
            if (JobAdded != null)
            {
                JobAdded(this, new JobAddedEventArgs
                {
                    Job = job,
                    Status = status
                });
            }
        }

        private void StartJob(JobImpl job)
        {
            //TODO: Update service status here
            Run(job);
        }

        private void Run(JobImpl job)
        {
            string failedLog = Path.Combine(job.WorkingDirectory, LocalPool.Failed);
            File.Delete(failedLog);

            if (string.IsNullOrEmpty(job.RunCommand)
                // || (job.RunCommand.Split(' ').Count() > 1 &&
                //File.Exists(Path.Combine(job.WorkingDirectory, job.RunCommand.Split(' ').FirstOrDefault())) == false)
                )
            {
                Trace.TraceError("Job will not be executed because the runCommand is empty or does not exist: {0}", Path.Combine(job.WorkingDirectory, job.RunCommand ?? ""));
                using (StreamWriter writer = new StreamWriter(failedLog))
                {
                    writer.WriteLine("ERROR: Job will not be executed because the runCommand is empty or does not exist: {0}", Path.Combine(job.WorkingDirectory, job.RunCommand ?? ""));
                }
                job.Status = Job.StatusEnum.FailedExecution;
                return;
            }

            if (Configuration.RemoteExecution)
            {
                Trace.TraceInformation("Prepare Job for remote execution: {0} {1}", job.Id, job.Title);

                try
                {
                    // zip working directory
                    string zipFile = Path.Combine(job.WorkingDirectory, "source_data.zip");
                    string zipPy = Path.Combine(job.WorkingDirectory, "zip.py");

                    // if zip.py does not exist create
                    if (File.Exists(zipPy) == false)
                    {
                        using (StreamWriter writer = new StreamWriter(zipPy))
                        {
                            writer.WriteLine(@"#!/usr/bin/python

import zipfile
import sys
import os
import os.path

path_join = os.path.join
if sys.platform == 'win32':
    def path_join(*args):
        return '\\\\?\\' + os.path.join(os.getcwd(), os.path.join(*args))

output_filename = 'source_data.zip'

if os.path.exists(output_filename):
    os.remove(output_filename)

# LS_Dyna workers have RHEL6. RHEL6 has Python2.6, which doesnt have zipfile.ZipFile.__exit__ http://bugs.python.org/issue5511 . So we dont use 'with'
z = zipfile.ZipFile(output_filename, 'w', allowZip64=True)
try:
    parent_dir_name = os.path.basename(os.getcwd())
    os.chdir('..')
    for dirpath, dirs, files in os.walk(parent_dir_name):
    # Fix META-1850: make sure all dirs are copied.
      for d in dirs:
        dn = path_join(dirpath, d)
        z.write(dn, arcname=os.path.join(dirpath, d), compress_type=zipfile.ZIP_DEFLATED)
      for f in files:
        if output_filename == f:
            continue
        fn = path_join(dirpath, f)
        #print fn
        z.write(fn, arcname=os.path.join(dirpath, f), compress_type=zipfile.ZIP_DEFLATED)
finally:
    z.close()
");
                        }
                    }

                    if (File.Exists(zipFile) == false)
                    {
                        // call zip.py to zip the package if it does not exist
                        job.Status = Job.StatusEnum.ZippingPackage;
                        ProcessStartInfo psi = new ProcessStartInfo(META.VersionInfo.PythonVEnvExe)
                        {
                            Arguments = "-E \"" + zipPy + "\"",
                            WorkingDirectory = job.WorkingDirectory,
                            WindowStyle = ProcessWindowStyle.Hidden,
                            UseShellExecute = false,
                            CreateNoWindow = true,
                            RedirectStandardError = true,
                        };

                        Process proc = new Process()
                        {
                            StartInfo = psi,
                        };

                        proc.Start();
                        string stderr = proc.StandardError.ReadToEnd();
                        proc.WaitForExit();
                        if (proc.ExitCode != 0)
                        {
                            job.Status = Job.StatusEnum.Failed;
                            Trace.TraceError("zip.py failed with exit code {0}. stderr was {1}", proc.ExitCode, stderr);
                            return;
                        }
                    }

                    if (File.Exists(zipFile) == false)
                    {
                        job.Status = Job.StatusEnum.Failed;
                        Trace.TraceError("zip.py did not produce {0}", zipFile);
                        return;
                    }

                    if (this.IsServiceOnline() == false)
                    {
                        // put all job into a waiting status if the server is disconnected
                        job.Status = Job.StatusEnum.WaitingForStart;

                        // hack: maybe the jenkins job server is down. don't hammer the server
                        Thread.Sleep(Jenkins.Jenkins.VF_JOB_POLL_FREQUENCY);
                        
                        // put the job back to the list and try to start it again later
                        JobsToBeStarted.Add(() => { job.Start(); });
                        return;
                    }

                    // create a job on the remote machine

                    // KMS: add random hex number because SoT steps have the same WorkingDirectory
                    // ZsL: the 8 digit random hex numbers were not enough. If users had many jobs 500+ got exception the
                    //      jobname is not unique: problems in database due jobname is the primary key and on jenkins too.
                    var randomid = Path.GetFileName(job.WorkingDirectory); //Guid.NewGuid().ToString("N");
                    var anotherRandomId = Path.GetFileNameWithoutExtension(Path.GetRandomFileName());
                    string jobname = string.Format("{0}_{1}{2}", Configuration.UserId, randomid, anotherRandomId);
                    string description = string.Format("{0}_{1}_{2:00000}_{3}", Configuration.UserId, job.Title,
                        job.Id, randomid);
                    jobname = jobname.Replace(' ', '_');

                    //if (Jenkins.JobExists(jobname))
                    {
                        // job already exists
                        //   Jenkins.DeleteJob(jobname);
                    }


                    string cmd = Path.GetExtension(job.RunCommand) == ".py" ?
                        "python " + job.RunCommand :
                        job.RunCommand;

                    string labels = job.Labels;

                    job.Status = Job.StatusEnum.UploadPackage;

                    string getUrl;
                    if (String.IsNullOrWhiteSpace(job.GetURLOverride) == false)
                    {
                        getUrl = "r'" + job.GetURLOverride + "'";
                    }
                    else
                    {
                        getUrl = "os.environ['sourceGetUrl']"; // parameter provided to Jenkins by VF
                        string zipFileUrl = JenkinsInstance.UploadFileToVF(zipFile, jobname, delegate (int percent)
                        {
                            //TODO: UI Callback for Jenkins upload progress
                        });

                        if (zipFileUrl == null)
                        {
                            job.Status = Job.StatusEnum.FailedToUploadServer;
                            return;
                        }
                    }

                    // 1. {0} description
                    // 2. {1} working directory name
                    // 3. {2} runcommand that needs to be executed in the working directory
                    // 4. {3} required slave node (Label expression)
                    // 5. {4} source.zip GET URL
                    // 6. {5} zip.py server side hook
                    // 7. {6} name of the zipped results file
                    string resultZipName = "results.zip";
                    string resultZipPy = string.Format(job.ResultsZip, resultZipName, Path.GetFileName(job.WorkingDirectory));
                    var config_xml = String.Format(
                            Properties.Resources.job_config,
                            SecurityElement.Escape(description),
                            Path.GetFileName(job.WorkingDirectory),
                            SecurityElement.Escape(cmd),
                            SecurityElement.Escape(labels),
                            SecurityElement.Escape(getUrl),
                            SecurityElement.Escape(resultZipPy).Replace("\\", "\\\\"),
                            SecurityElement.Escape(resultZipName));

                    var jobinfonew = JenkinsInstance.CreateJob(jobname, config_xml);

                    if (jobinfonew == null)
                    {
                        // job creation failed
                        job.Status = Job.StatusEnum.FailedToUploadServer;
                        return;
                    }

                    job.Status = Job.StatusEnum.PostedToServer;

                    string returned_config_xml = JenkinsInstance.GetJobConfig(jobname);
                    if (returned_config_xml != null)
                    {
                        if (returned_config_xml.IndexOf("org.jvnet.hudson.plugins.Jython") == -1)
                        {
                            string logFilename = System.IO.Path.Combine(job.WorkingDirectory, LocalPool.Failed);
                            File.WriteAllText(logFilename, "Jenkins does not have the Jython plugin installed");
                            Trace.TraceError("Jenkins does not have the Jython plugin installed");
                            job.Status = Job.StatusEnum.FailedToUploadServer;
                            return;
                        }
                    }
                    else
                    {
                        
                    }; // FIXME throw?

                    // send zip and start job
                    if (JenkinsInstance.BuildJob(jobname, jobname, job.BuildQuery) == null)
                    {
                        job.Status = Job.StatusEnum.FailedToUploadServer;
                        return;
                    }

                    // on rerun, job is already in the map
                    job.remoteInfo = JobMap[job] = new RemoteJobInfo() { JenkinsJobName = jobname };
                    job.Status = Job.StatusEnum.StartedOnServer;
                }
                catch (Exception ex)
                {
                    JenkinsInstance.DebugLog.WriteLine(ex.ToString().Replace("\n", "  "));
                    job.Status = Job.StatusEnum.FailedToUploadServer;
                    //TODO: Jenkins upload error callback
                    //MessageBox.Show(ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    Trace.TraceError(ex.ToString());
                }
            }
            else
            {
                // if local
                pool.EnqueueJob(job);
            }
        }

        private void SoTAddedHandler(SoT sot_)
        {
            SoTImpl sot = (SoTImpl)sot_;
            Trace.TraceInformation("SoT added.");
            lock (this)
            {
                if (SoTWorkerThread == null)
                {
                    SoTWorkerThread = new System.Threading.Thread(
                    delegate ()
                    {
                        while (true)
                        {
                            Action action = SoTTodo.Take();
                            if (action == null)
                            {
                                return;
                            }
                            else
                            {
                                action.Invoke();
                            }
                        }
                    }
                    );
                    SoTWorkerThread.SetApartmentState(System.Threading.ApartmentState.STA);
                    SoTWorkerThread.Start();
                    SoTWorkerThread.Name = "STA thread for SoT.Run";
                }
                sot.SoTTodo = SoTTodo;
                SoTs.Add(sot);
                JobsToBeStarted.Add(delegate ()
                {
                    SoTTodo.Add(delegate { sot.Run(); });
                });
            }
        }

        private void JobStatusChanged(Job job_, Job.StatusEnum status)
        {
            try
            {
                JobImpl job = (JobImpl)job_;
                Trace.TraceInformation("Form JobStatusChanged was called for: {0}, which changed status to {1}", job.Title, status.ToString());
                

                if (Server.IsRemote)
                {
                    if (status == Job.StatusEnum.StartedOnServer)
                    {
                        RemoteJobInfo remoteJobInfo;
                        if (JobMap.TryGetValue(job, out remoteJobInfo) == false)
                        {
                            Trace.TraceError("Could not find jenkins job for {0}. Persistence will not work", job.Title);
                        }
                        else
                        {
                            SavedJob saved = SavedJob.CreateSavedJob(
                                job.Title, job.WorkingDirectory, job.RunCommand, job.Status.ToString(), remoteJobInfo.JenkinsJobName);
                            saved.VFUrl = Server.JenkinsUrl;
                            Entities.SavedJobs.AddObject(saved);
                            Entities.SaveChanges();
                            Entities.SavedJobs.Detach(saved);
                        }
                    }
                    else if (Job.IsFailedStatus(status) || status == Job.StatusEnum.Succeeded)
                    {
                        RemoteJobInfo remoteJobInfo;
                        if (JobMap.TryGetValue(job, out remoteJobInfo) == false)
                        {
                            Trace.TraceError("Could not find jenkins job for {0}. It may remain in the database", job.Title);
                        }
                        else
                        {
                            foreach (var entity in Entities.SavedJobs.Where(x => x.JobName == remoteJobInfo.JenkinsJobName))
                            {
                                Entities.SavedJobs.DeleteObject(entity);
                            }
                            Entities.SaveChanges();
                        }
                    }
                }

                if (status == Job.StatusEnum.Ready)
                {
                    JobsToBeStarted.Add(delegate () { StartJob(job); });
                }
            }
            catch (Exception ex)
            {
                //TODO: Propagate error back to UI?
                Trace.TraceError(ex.ToString());
            }

        }

        private void MonitorJenkinsJobs(object _)
        {
            Thread.Sleep(1000);
            while (true)
            {
                try
                {
                    var status = GetServiceStatus();
                    if (status.isRemoteServer && status.jenkinsConnected && status.vfConnected)
                    {
                        var jobs = JobMap.ToList().Where(j => !(j.Key.IsFailed() || j.Key.Status == Job.StatusEnum.Succeeded)).ToList();
                        jobs.Sort((Comparison<KeyValuePair<Job, RemoteJobInfo>>)((x, y) => { return x.Key.Id.CompareTo(y.Key.Id); }));
                        foreach (var kvp2 in jobs)
                        {
                            var jenkinsJobName = kvp2.Value.JenkinsJobName;
                            var job = kvp2.Key;

                            try
                            {
                                try
                                {
                                    // if online get updated job status
                                    MonitorJenkinsJob(job, kvp2.Value);
                                }
                                catch (WebException)
                                {
                                    if (IsServiceOnline())
                                    {
                                        throw;
                                    }
                                }
                            }
                            catch (Exception ex)
                            {
                                job.Status = Job.StatusEnum.Failed;
                                JenkinsInstance.DebugLog.WriteLine(job.Title + " failed: " + ex.ToString().Replace("\n", "\t"));
                                Trace.TraceError(job.Title + " failed: " + ex.ToString().Replace("\n", "\t"));
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    Trace.TraceWarning(ex.ToString());
                }

                bool signaled = JobMonitorEvent.WaitOne(Jenkins.Jenkins.VF_JOB_POLL_FREQUENCY);
            }
        }

        private void MonitorJenkinsJob(Job job, RemoteJobInfo remoteJobInfo)
        {
            string jenkinsJobName = remoteJobInfo.JenkinsJobName;
            if (job.Status == Job.StatusEnum.RedownloadQueued)
            {
                // n.b. job may not exist on server
                var success = SaveWorkspace(job, remoteJobInfo);

                job.Status = success;
                return;
            }

            Jenkins.Job.Job jenkinsJob;
            try
            {
                jenkinsJob = JenkinsInstance.GetJobInfo(jenkinsJobName, rethrow: true);
            }
            catch (WebException e)
            {
                if (e is Jenkins.WebExceptionWithStatusCode && ((Jenkins.WebExceptionWithStatusCode)e).StatusCode == 404)
                {
                    Trace.TraceError("Job {0} no longer exists (404 received from server). Marking job as Failed", jenkinsJobName);
                    job.Status = Job.StatusEnum.Failed;
                    return;
                }
                throw;
            }
            if (jenkinsJob == null)
            {
                // should not be reachable
                Trace.TraceError("Could not retrieve status for job {0}. Marking job as Failed", jenkinsJobName);
                job.Status = Job.StatusEnum.Failed;
                return;
            }

            if (jenkinsJob.lastSuccessfulBuild != null)
            {
                JenkinsInstance.SaveLastConsole(jenkinsJob.name, Path.Combine(job.WorkingDirectory, "consoleText.txt"));
                // delete workspace.zip here if it exists, or SaveWorkspace will try to resume
                // n.b. "If the file to be deleted does not exist, no exception is thrown."
                File.Delete(Path.Combine(job.WorkingDirectory, "workspace.zip"));
                var success = SaveWorkspace(job, remoteJobInfo);

                if (Configuration.WipeWorkspaceOnSuccess)
                {
                    // wipe workspace before we delete the job
                    JenkinsInstance.WipeWorkSpace(jenkinsJob.name);
                }

                if (false && Configuration.DeleteJobOnSuccess) // META-2493 META-2492
                {
                    // delete job from server
                    JenkinsInstance.DeleteJob(jenkinsJob.name);
                }

                job.Status = success;
            }
            else if (jenkinsJob.lastFailedBuild != null)
            {
                JenkinsInstance.SaveLastConsole(jenkinsJob.name, Path.Combine(job.WorkingDirectory, "consoleText.txt"));
                File.Copy(Path.Combine(job.WorkingDirectory, "consoleText.txt"), Path.Combine(job.WorkingDirectory, LocalPool.Failed), true);

                try
                {
                    // delete workspace.zip here if it exists, or SaveWorkspace will try to resume
                    // n.b. "If the file to be deleted does not exist, no exception is thrown."
                    File.Delete(Path.Combine(job.WorkingDirectory, "workspace.zip"));
                    job.Status = SaveWorkspace(job, remoteJobInfo);
                }
                catch (Exception e)
                {
                    Trace.TraceError("Error retrieving results for {0} ({1}): {2}", job.Title, jenkinsJob.name, e.ToString());
                    job.Status = Job.StatusEnum.FailedToDownload;
                }
            }
            else if (jenkinsJob.lastCompletedBuild != null)
            {
                // build got cancelled
                JenkinsInstance.SaveLastConsole(jenkinsJob.name, Path.Combine(job.WorkingDirectory, "consoleText.txt"));
                File.Copy(Path.Combine(job.WorkingDirectory, "consoleText.txt"), Path.Combine(job.WorkingDirectory, LocalPool.Failed), true);
                job.Status = Job.StatusEnum.FailedAbortOnServer;
            }
            else if (jenkinsJob.inQueue)
            {
                if (job.Status == Job.StatusEnum.AbortOnServerRequested)
                {
                    job.Status = Job.StatusEnum.FailedAbortOnServer;
                    JenkinsInstance.StopJob(remoteJobInfo.JenkinsJobName);
                }
                else
                {
                    job.Status = Job.StatusEnum.QueuedOnServer;

                    if (jenkinsJob.queueItem != null)
                    {
                        if (jenkinsJob.queueItem.stuck)
                        {
                            //Trace.TraceError(Newtonsoft.Json.JsonConvert.SerializeObject(jenkinsJob.queueItem));
                            Trace.TraceError("In queue reason: {0}", jenkinsJob.queueItem.why);
                        }
                        else
                        {
                            Trace.TraceWarning("In queue reason: {0}", jenkinsJob.queueItem.why);
                        }
                    }
                }
            }
            else if (jenkinsJob.nextBuildNumber > 1)
            {
                if (jenkinsJob.lastBuild != null &&
                    jenkinsJob.lastBuild.number < jenkinsJob.nextBuildNumber)
                {
                    if (job.Status == Job.StatusEnum.AbortOnServerRequested)
                    {
                        // n.b. job may not exist on server
                        JenkinsInstance.StopJob(remoteJobInfo.JenkinsJobName);
                    }
                    else
                    {
                        job.Status = Job.StatusEnum.RunningOnServer;
                    }
                }
            }
            else if (job.Status == Job.StatusEnum.AbortOnServerRequested)
            {
                job.Status = Job.StatusEnum.FailedAbortOnServer;
                JenkinsInstance.StopJob(remoteJobInfo.JenkinsJobName);
            }
            // TODO: how to detect if it got cancelled in the queue, before it runs?
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="jenkinsJob"></param>
        /// <param name="job"></param>
        /// <returns>Indicates if job failed or not.</returns>
        private Job.StatusEnum SaveWorkspace(
            Job job,
            RemoteJobInfo remoteJobInfo)
        {
            job.Status = Job.StatusEnum.DownloadResults;

            if (remoteJobInfo.ResultsGetURL == null)
            {
                if (JenkinsInstance.DownloadFileFromVF(remoteJobInfo.JenkinsJobName, ref remoteJobInfo.ResultsGetURL) == false)
                {
                    Trace.TraceError("DownloadFileFromVF failed {0}", job.Title);
                    job.Status = Job.StatusEnum.FailedToDownload;
                    return Job.StatusEnum.FailedToDownload;
                }
            }

            bool result = JenkinsInstance.DownloadFileFromSwift(Path.Combine(job.WorkingDirectory, "workspace.zip"), remoteJobInfo.ResultsGetURL, delegate (int percent)
            {
                //TODO: Download progress notification for client
                /*lvJobQueue.BeginInvoke((MethodInvoker)delegate ()
                {
                    ListViewItem item = lvJobQueue.Items.Cast<ListViewItem>().
                       FirstOrDefault(x => x.SubItems[(int)Headers.Id].Text == job.Id.ToString());
                    if (item != null)
                    {
                        item.SubItems[(int)Headers.Progress].Text = percent + "%";
                    }
                });*/
            });

            if (result == false)
            {
                job.Status = Job.StatusEnum.FailedToDownload;
                return Job.StatusEnum.FailedToDownload;
            }

            // unzip package
            string unzipPy = Path.Combine(job.WorkingDirectory, "unzip.py");
            if (File.Exists(unzipPy) == false)
            {
                using (StreamWriter writer = new StreamWriter(unzipPy))
                {
                    writer.WriteLine(@"#!/usr/bin/py
import os
import os.path
import sys
import shutil
import zipfile

path_join = os.path.join
if sys.platform == 'win32':
    def path_join(*args):
        return '\\\\?\\' + os.path.join(os.getcwd(), os.path.join(*args))
try:
    parent_dir_name = os.path.basename(os.getcwd())

    zip = zipfile.ZipFile('workspace.zip')

    # OLD version zip.namelist()[0] is unpredictable
    #root_src_dir = zip.namelist()[0] + parent_dir_name
    # ASSUMPTION workspace.zip has always the parent_dir_name as a zipped directory
    root_src_dir = parent_dir_name

    print root_src_dir
    for entry in zip.infolist():
        if entry.filename.startswith(root_src_dir):
            dest = entry.filename[len(root_src_dir)+1:]
            if dest == '':
                continue
            if dest.endswith('/'):
                if not os.path.isdir(dest):
                    os.mkdir(dest)
            else:
                if os.path.basename(dest) != 'workspace.zip':
                    entry.filename = dest
                    zip.extract(entry, path=path_join(os.getcwd()))
except Exception as msg:
    import traceback
    sys.stderr.write(traceback.format_exc())
    with open('_FAILED.txt', 'wb') as f_out:
        f_out.write(str(msg))
        f_out.write('\nMost likely due to a too long file-path for Windows (max 260).')
    if os.name == 'nt':
        os._exit(3)
    elif os.name == 'posix':
        os._exit(os.EX_OSFILE)

");
                }
            }

            // call unzip.py to unzip the package
            ProcessStartInfo psi = new ProcessStartInfo(META.VersionInfo.PythonVEnvExe)
            {
                Arguments = "-E \"" + unzipPy + "\"",
                WorkingDirectory = job.WorkingDirectory,
                WindowStyle = ProcessWindowStyle.Hidden,
                UseShellExecute = false,
                CreateNoWindow = true,
                RedirectStandardError = true,
            };

            Process proc = new Process()
            {
                StartInfo = psi,
            };

            proc.Start();
            string stderr = proc.StandardError.ReadToEnd();
            proc.WaitForExit();

            if (proc.ExitCode != 0)
            {
                string logFilename = System.IO.Path.Combine(job.WorkingDirectory, LocalPool.Failed);
                File.WriteAllText(logFilename, "unzip.py failed:\n" + stderr);
                Trace.TraceError("unzip.py failed {0}", job.Title);
                return Job.StatusEnum.Failed;
            }
            else
            {
                if (File.Exists(System.IO.Path.Combine(job.WorkingDirectory, LocalPool.Failed)))
                {
                    return Job.StatusEnum.FailedExecution;
                }
                return Job.StatusEnum.Succeeded;
            }
        }

        private void JobsToBeStartedCleaner(object _)
        {
            this.Configured.WaitOne();
            while (true)
            {
                Action job = JobsToBeStarted.Take();
                try
                {
                    job.Invoke();
                }
                catch (Exception e)
                {
                    Trace.TraceError("Error posting job: " + e.ToString());
                }
            }
        }

        /// <summary>
        /// Checks if VF and remote executors are online or not.
        /// </summary>
        /// <returns>True if jobs can be posted, otherwise false.</returns>
        private bool IsServiceOnline()
        {
            return JenkinsInstance.PingJenkins();
        }

        private static int GetPort(int port)
        {
            IPGlobalProperties ipGlobalProperties = IPGlobalProperties.GetIPGlobalProperties();
            System.Net.IPEndPoint[] tcpListeners = ipGlobalProperties.GetActiveTcpListeners();

            foreach (var item in tcpListeners)
            {
                if (item.Port == port)
                {
                    if (port < 65000)
                    {
                        return GetPort(port + 1);
                    }
                    else
                    {
                        return -1;
                    }
                }
            }

            return port;
        }

        private static class ExtensionMethods
        {
            public static IEnumerable<T> Closure<T>(T root, Func<T, IEnumerable<T>> children)
            {
                var seen = new HashSet<T>();
                var stack = new Stack<T>();
                stack.Push(root);

                while (stack.Count != 0)
                {
                    T item = stack.Pop();
                    if (seen.Contains(item))
                        continue;
                    seen.Add(item);
                    yield return item;
                    foreach (var child in children(item))
                        stack.Push(child);
                }
            }
        }
    }
}