using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Linq;
using System.IO;
using System.IO.Packaging;
using System.Runtime.Remoting.Channels;
using System.Net;
using System.Diagnostics;
using System.Threading;
using System.Web;
using System.Security;
using System.Data.SQLite;
using System.Windows.Forms.Design;
using System.Collections.Concurrent;


namespace JobManager
{
    public partial class JobManager : Form
    {
        private SpringLabel toolStripStatusLabel { get; set; }

        private RemoteServiceStatusForm remoteServiceStatusForm { get; set; }

        public NotifyIcon NotifyIcon;
        internal JobServerImpl server;

        private string TempDir { get; set; }

        private enum Headers
        {
            Id, Title, TestBenchName, Status, Progress, RunCommand, WorkingDirectory
        }

        private Dictionary<Job.TypeEnum, TargetMachine> runtimeConfig =
            new Dictionary<Job.TypeEnum, TargetMachine>()
		{
			{ Job.TypeEnum.Command,
				new TargetMachine(
					new Uri("tcp://localhost:35010/JobServer"),
					TargetMachine.TargetMachineType.Local)},

			{ Job.TypeEnum.Matlab,
				new TargetMachine(
					new Uri("tcp://localhost:35010/JobServer"),
					TargetMachine.TargetMachineType.Local)},

			{ Job.TypeEnum.CAD,
			  new TargetMachine(
			    new Uri("tcp://localhost:35010/JobServer"),
			    TargetMachine.TargetMachineType.Local)},
		};

        public class TargetMachine
        {
            public enum TargetMachineType
            {
                Local,
                Remote,
            }

            public Uri Connection { get; set; }
            public TargetMachineType Type { get; set; }

            public TargetMachine(
                Uri connection,
                TargetMachineType type = TargetMachineType.Local)
            {
                Connection = connection;
                Type = type;
            }
        }

        public void UpdateRuntimeConfig(Dictionary<Job.TypeEnum, TargetMachine> config)
        {
            foreach (var item in config)
            {
                if (runtimeConfig.ContainsKey(item.Key))
                {
                    runtimeConfig[item.Key] = item.Value;
                }
                else
                {
                    runtimeConfig.Add(item.Key, item.Value);
                }
            }
        }

        /// <summary>
        /// Local thread pool for jobs.
        /// </summary>
        private LocalPool pool = new LocalPool();

        /// <summary>
        /// Controls will be refreshed when the timer has elapsed.
        /// </summary>
        private System.Windows.Forms.Timer timer = new System.Windows.Forms.Timer()
        {
            Interval = 1000,
        };

        private int highPriorityJobsRemaining = 0;

        /// <summary>
        /// Checks if VF and remote executors are online or not.
        /// </summary>
        /// <returns>True if jobs can be posted, otherwise false.</returns>
        private bool IsServiceOnline()
        {
            return this.Jenkins.PingJenkins();
        }

        /// <summary>
        /// Updates all controls that displays status of the remote servers.
        /// </summary>
        private bool UpdateServiceStatus()
        {
            if (server.IsRemote == false)
            {
                return true;
            }
            bool connected = false;
            Trace.TraceInformation("Pinging VF and getting status.");

            StringBuilder sb = new StringBuilder();

            global::JobManager.Jenkins.UserProfile.UserProfile info = null;
            if (this.Jenkins.PingVF(out info))
            {
                sb.AppendFormat("Connected to {0} and logged in as {1}. ",
                    this.server.JenkinsUrl,
                    this.server.UserName);

                this.remoteServiceStatusForm.SafeInvoke(delegate
                {
                    this.remoteServiceStatusForm.lblVFStatus.Text = "OK";
                });

                global::JobManager.Jenkins.StatusInfo.StatusInfo statusInfo = null;

                highPriorityJobsRemaining = 0;
                if (this.Jenkins.PingJenkins(out statusInfo))
                {
                    connected = true;
                    highPriorityJobsRemaining = statusInfo.highPriorityRemaining;
                    // TODO: update detailed status
                    sb.AppendFormat(
                        "[{4} high priority jobs available; {0}: {1} available and {2} busy of {3} total executors]",
                        statusInfo.status,
                        statusInfo.totalAvailable,
                        statusInfo.totalBusy,
                        statusInfo.totalAvailable + statusInfo.totalBusy,
                        statusInfo.highPriorityRemaining);

                    this.remoteServiceStatusForm.SafeInvoke(delegate
                    {
                        this.remoteServiceStatusForm.lblNodeStatus.Text = statusInfo.status;

                        this.remoteServiceStatusForm.lblBusyCount.Text = statusInfo.totalBusy.ToString();
                        this.remoteServiceStatusForm.lblFreeCount.Text = statusInfo.totalAvailable.ToString();

                        this.remoteServiceStatusForm.lblTotalCount.Text =
                            (statusInfo.totalAvailable + statusInfo.totalBusy).ToString();

                        this.remoteServiceStatusForm.lvRemoteNodes.SafeInvoke(delegate
                        {
                            this.remoteServiceStatusForm.lvRemoteNodes.Items.Clear();
                            foreach (var node in statusInfo.nodes)
                            {
                                var listViewItem = new ListViewItem(new string[] 
                                                    {
                                                        node.status,
                                                        node.busy.ToString(),
                                                        node.available.ToString(),
                                                        (node.available + node.busy).ToString(),
                                                        node.name,
                                                        node.description
                                                    });
                                this.remoteServiceStatusForm.lvRemoteNodes.Items.Add(listViewItem);
                            }
                        });

                        this.remoteServiceStatusForm.gbRemoteNodes.Visible = true;
                    });
                }
                else
                {
                    if (statusInfo != null)
                    {
                        sb.AppendFormat("Remote job server is NOT connected. [{0}]", statusInfo.status);

                        this.remoteServiceStatusForm.SafeInvoke(delegate
                        {
                            this.remoteServiceStatusForm.lblNodeStatus.Text = statusInfo.status;
                            this.remoteServiceStatusForm.gbRemoteNodes.Visible = false;
                        });
                    }
                    else
                    {
                        sb.AppendFormat("Remote job server is NOT connected.");

                        this.remoteServiceStatusForm.SafeInvoke(delegate
                        {
                            this.remoteServiceStatusForm.lblNodeStatus.Text = "DOWN";
                            this.remoteServiceStatusForm.gbRemoteNodes.Visible = false;
                        });

                    }
                }
            }
            else
            {
                sb.AppendFormat("VF is NOT connected. {0}", this.server.JenkinsUrl);
                this.remoteServiceStatusForm.SafeInvoke(delegate
                {
                    this.remoteServiceStatusForm.lblVFStatus.Text = "DOWN";
                    this.remoteServiceStatusForm.lblNodeStatus.Text = "DOWN";
                    this.remoteServiceStatusForm.gbRemoteNodes.Visible = false;
                });
            }

            this.toolStripStatusLabel.Text = sb.ToString();

            return connected;
        }

        private void MonitorJenkinsJobs(object _)
        {
            Thread.Sleep(1000);
            while (true)
            {
                try
                {
                    if (this.UpdateServiceStatus())
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
                                Jenkins.DebugLog.WriteLine(job.Title + " failed: " + ex.ToString().Replace("\n", "\t"));
                                Trace.TraceError(job.Title + " failed: " + ex.ToString().Replace("\n", "\t"));
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    Trace.TraceWarning(ex.ToString());
                }

                bool signaled = JobMonitorEvent.WaitOne(global::JobManager.Jenkins.Jenkins.VF_JOB_POLL_FREQUENCY);
            }
        }

        private Thread JobMonitorThread;
        private AutoResetEvent JobMonitorEvent = new AutoResetEvent(false);
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

            global::JobManager.Jenkins.Job.Job jenkinsJob;
            try
            {
                jenkinsJob = Jenkins.GetJobInfo(jenkinsJobName, rethrow: true);
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
                Jenkins.SaveLastConsole(jenkinsJob.name, Path.Combine(job.WorkingDirectory, "consoleText.txt"));
                // delete workspace.zip here if it exists, or SaveWorkspace will try to resume
                // n.b. "If the file to be deleted does not exist, no exception is thrown."
                File.Delete(Path.Combine(job.WorkingDirectory, "workspace.zip"));
                var success = SaveWorkspace(job, remoteJobInfo);

                if (WipeWorkspaceOnSuccess)
                {
                    // wipe workspace before we delete the job
                    Jenkins.WipeWorkSpace(jenkinsJob.name);
                }

                if (false && DeleteJobOnSuccess) // META-2493 META-2492
                {
                    // delete job from server
                    Jenkins.DeleteJob(jenkinsJob.name);
                }

                job.Status = success;
            }
            else if (jenkinsJob.lastFailedBuild != null)
            {
                Jenkins.SaveLastConsole(jenkinsJob.name, Path.Combine(job.WorkingDirectory, "consoleText.txt"));
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
                Jenkins.SaveLastConsole(jenkinsJob.name, Path.Combine(job.WorkingDirectory, "consoleText.txt"));
                File.Copy(Path.Combine(job.WorkingDirectory, "consoleText.txt"), Path.Combine(job.WorkingDirectory, LocalPool.Failed), true);
                job.Status = Job.StatusEnum.FailedAbortOnServer;
            }
            else if (jenkinsJob.inQueue)
            {
                if (job.Status == Job.StatusEnum.AbortOnServerRequested)
                {
                    job.Status = Job.StatusEnum.FailedAbortOnServer;
                    this.Jenkins.StopJob(remoteJobInfo.JenkinsJobName);
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
                        this.Jenkins.StopJob(remoteJobInfo.JenkinsJobName);
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
                this.Jenkins.StopJob(remoteJobInfo.JenkinsJobName);
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
                if (Jenkins.DownloadFileFromVF(remoteJobInfo.JenkinsJobName, ref remoteJobInfo.ResultsGetURL) == false)
                {
                    Trace.TraceError("DownloadFileFromVF failed {0}", job.Title);
                    job.Status = Job.StatusEnum.FailedToDownload;
                    return Job.StatusEnum.FailedToDownload;
                }
            }

            bool result = Jenkins.DownloadFileFromSwift(Path.Combine(job.WorkingDirectory, "workspace.zip"), remoteJobInfo.ResultsGetURL, delegate(int percent)
            {
                lvJobQueue.BeginInvoke((MethodInvoker)delegate()
                {
                    ListViewItem item = lvJobQueue.Items.Cast<ListViewItem>().
                       FirstOrDefault(x => x.SubItems[(int)Headers.Id].Text == job.Id.ToString());
                    if (item != null)
                    {
                        item.SubItems[(int)Headers.Progress].Text = percent + "%";
                    }
                });
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

        protected virtual bool IsFileLocked(FileInfo file)
        {
            FileStream stream = null;

            try
            {
                stream = file.Open(FileMode.Open, FileAccess.ReadWrite, FileShare.None);
            }
            catch (IOException)
            {
                //the file is unavailable because it is:
                //still being written to
                //or being processed by another thread
                //or does not exist (has already been processed)
                return true;
            }
            finally
            {
                if (stream != null)
                    stream.Close();
            }

            //file is not locked
            return false;
        }

        Jenkins.Jenkins Jenkins;
        public class RemoteJobInfo
        {
            public string JenkinsJobName { get; set; }
            public string ResultsGetURL;
        }
        ConcurrentDictionary<Job, RemoteJobInfo> JobMap;

        public string TraceFileName { get; set; }
        private string password { get; set; }
        JobManagerEntities1 entities;

        public JobManager(JobServerImpl server, Dictionary<string, string> settings = null)
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

            entities = new JobManagerEntities1(String.Format("metadata=res://*/SavedJobs.csdl|res://*/SavedJobs.ssdl|res://*/SavedJobs.msl;provider=System.Data.SQLite;provider connection string=\"data source={0}\"", dbPath));
            // Fail fast
            entities.SavedJobs.ToList();

            this.server = server;

            InitializeComponent();

            this.toolStripStatusLabel = new SpringLabel();
            this.statusStrip1.Items.Add(this.toolStripStatusLabel);

            this.remoteServiceStatusForm = new RemoteServiceStatusForm();

            if (settings != null)
            {
                string value = string.Empty;
                if (settings.TryGetValue("-u", out value))
                {
                    Properties.Settings.Default.UserID = value;
                }
                if (settings.TryGetValue("-p", out value))
                {
                    password = value;
                }
                if (settings.TryGetValue("-U", out value))
                {
                    Properties.Settings.Default.VehicleForgeUri = value;
                }
            }

            //this.TempDir = Path.Combine(System.IO.Path.GetTempPath(), "META_JobManager");

            this.TempDir = Path.Combine(
                System.IO.Path.GetTempPath(),
                Path.Combine("META", "JobManager"));


            if (Directory.Exists(this.TempDir) == false)
            {
                Directory.CreateDirectory(this.TempDir);
            }

            this.TraceFileName = Path.Combine(this.TempDir, string.Format("JobManagerLog.trace.{0}.txt", Process.GetCurrentProcess().Id));

            this.logToolStripMenuItem.ToolTipText += string.Format(" ({0})", Path.GetFullPath(this.TraceFileName));

            this.Jenkins = new Jenkins.Jenkins(this.TempDir);
            this.JobMap = new ConcurrentDictionary<Job, RemoteJobInfo>();

            this.InitJobQueue();

            var fs = new FileStream(this.TraceFileName, FileMode.Create);
            TraceListener fileTL = new TextWriterTraceListener(fs)
            {
                TraceOutputOptions = TraceOptions.DateTime
            };
            // use TraceXXX to get timestamp per http://stackoverflow.com/questions/863394/add-timestamp-to-trace-writeline

            Trace.AutoFlush = true;
            Trace.Listeners.Add(fileTL);

            Trace.TraceInformation(META.Logger.Header());

            this.FormClosing += (sender, args) =>
            {
                if (this.SoTs.Any(s => s.TestBenchJobMap.Any(j => j.Value.Status == Job.StatusEnum.Ready &&
                    ExtensionMethods.Closure(j.Key, sot => sot.UpstreamTestBenches).All(tb => s.TestBenchJobMap[tb].IsFailed() == false))))
                {
                    DialogResult dr = MessageBox.Show(this, "Some jobs have not been posted and will be lost. Do you want to exit anyways?", "JobManager", MessageBoxButtons.YesNo);
                    if (dr == DialogResult.No)
                    {
                        args.Cancel = true;
                        return;
                    }
                }
                if (server.IsRemote == false)
                {
                    int unfinished = this.pool.GetNumberOfUnfinishedJobs();
                    if (unfinished > 0)
                    {
                        DialogResult dr = MessageBox.Show(this, unfinished + " jobs have not completed. Do you want to exit and terminate running processes?", "JobManager", MessageBoxButtons.YesNo);
                        if (dr == DialogResult.No)
                        {
                            args.Cancel = true;
                            return;
                        }
                    }
                    this.pool.Dispose();
                }
                Trace.TraceInformation("JobManager is closing");
                SoTTodo.Add(null);
            };
            this.Resize += new EventHandler(JobManager_Resize);
            NotifyIcon = new NotifyIcon();
            NotifyIcon.Icon = Icon;
            NotifyIcon.Visible = false;
            NotifyIcon.Click += new EventHandler(NotifyIcon_Click);
            this.exitToolStripMenuItem.Click += new EventHandler((o, ev) =>
            {
                this.Close();
            });

            this.server.JobAdded += JobAdded;
            this.server.SoTAdded += SoTAdded;
            this.server.handlersAdded.Set();
            //pool.JobStatusChanged += JobStatusChanged;
            this.timer.Start();

            // load saved settings
            Uri cmd = new Uri(Properties.Settings.Default.CommandUri);
            Uri matlab = new Uri(Properties.Settings.Default.MatLabUri);
            Uri cad = new Uri(Properties.Settings.Default.CADUri);

            Dictionary<Job.TypeEnum, JobManager.TargetMachine> config = new Dictionary<Job.TypeEnum, JobManager.TargetMachine>();
            JobManager.TargetMachine.TargetMachineType type = JobManager.TargetMachine.TargetMachineType.Local;
            if (cmd.Host.Equals("localhost"))
            {
                type = JobManager.TargetMachine.TargetMachineType.Local;
            }
            else
            {
                type = JobManager.TargetMachine.TargetMachineType.Remote;
            }
            config.Add(Job.TypeEnum.Command, new JobManager.TargetMachine(cmd, type));

            if (matlab.Host.Equals("localhost"))
            {
                type = JobManager.TargetMachine.TargetMachineType.Local;
            }
            else
            {
                type = JobManager.TargetMachine.TargetMachineType.Remote;
            }
            config.Add(Job.TypeEnum.Matlab, new JobManager.TargetMachine(matlab, type));

            if (cad.Host.Equals("localhost"))
            {
                type = JobManager.TargetMachine.TargetMachineType.Local;
            }
            else
            {
                type = JobManager.TargetMachine.TargetMachineType.Remote;
            }
            config.Add(Job.TypeEnum.CAD, new JobManager.TargetMachine(cad, type));

            UpdateRuntimeConfig(config);

            this.JobsToBeStartedThread = new System.Threading.Thread(this.JobsToBeStartedCleaner);
            this.JobsToBeStartedThread.Name = "JobStarter";
            this.JobsToBeStartedThread.IsBackground = true;
            this.JobsToBeStartedThread.Start();

            if (settings.ContainsKey("-i"))
            {
                return;
            }
            Shown += new EventHandler(delegate(object o, EventArgs args)
                {
                    if (this.Configure(password) == DialogResult.OK)
                    {
                        lock (this)
                        {
                            this.configured.Set();
                            if (server.IsRemote)
                            {
                                this.toolStripStatusLabel.Text = "Configured for remote execution.";
                                this.remoteStatusToolStripMenuItem.Enabled = true;
                                this.remoteStatusToolStripMenuItem.Visible = true;
                            }
                            else
                            {
                                this.toolStripStatusLabel.Text = "Configured for local execution.";
                                this.remoteStatusToolStripMenuItem.Enabled = false;
                                this.remoteStatusToolStripMenuItem.Visible = false;
                            }
                            if (server.IsRemote)
                            {
                                // retreiving saved jobs and adding to the job queue and jobmap
                                foreach (SavedJob saved in entities.SavedJobs)
                                {
                                    if (string.IsNullOrEmpty(saved.VFUrl) == false && server.JenkinsUrl != saved.VFUrl)
                                        continue;
                                    Job job = new JobImpl(server)
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
                                    var jenkinsJobInfo = Jenkins.GetJobInfo(saved.JobName);
                                    if (jenkinsJobInfo == null)
                                    {
                                        Trace.TraceInformation("Saved job {0} ({1}) no longer exists on server. Marking failed.", job.Title, jenkinsJobInfo);
                                        job.Status = Job.StatusEnum.FailedAbortOnServer;
                                        entities.SavedJobs.DeleteObject(saved);
                                    }
                                    else
                                    {
                                        Trace.TraceInformation("Adding saved job {0} ({1})", job.Title, jenkinsJobInfo);
                                        ((JobImpl)job).remoteInfo = JobMap[job] = new RemoteJobInfo() { JenkinsJobName = jenkinsJobInfo.name };
                                        server.AddJob(job);
                                        entities.SavedJobs.Detach(saved);
                                    }
                                }
                                entities.SaveChanges();

                                // start timer for remote execution monitoring
                                this.RestartMonitorTimer();
                            }
                        }
                    }
                    else
                    {
                        this.toolStripStatusLabel.Text = "Closing";
                        this.Close();
                    }
                });

            Trace.TraceInformation("Application started.");
        }


        private void InitJobQueue()
        {
            lvJobQueue.HeaderStyle = ColumnHeaderStyle.Clickable;
            lvJobQueue.View = View.Details;

            lvJobQueue.Columns.Add(Headers.Id.ToString(), Headers.Id.ToString());
            lvJobQueue.Columns[Headers.Id.ToString()].Width = 30;

            lvJobQueue.Columns.Add(Headers.Title.ToString(), Headers.Title.ToString());
            lvJobQueue.Columns[Headers.Title.ToString()].Width = 200;

            lvJobQueue.Columns.Add(Headers.TestBenchName.ToString(), "Test Bench Name");
            lvJobQueue.Columns[Headers.TestBenchName.ToString()].Width = 200;

            lvJobQueue.Columns.Add(Headers.Status.ToString(), Headers.Status.ToString());
            lvJobQueue.Columns[Headers.Status.ToString()].Width = 100;

            lvJobQueue.Columns.Add(Headers.Progress.ToString(), Headers.Progress.ToString());
            lvJobQueue.Columns[Headers.Progress.ToString()].Width = 40;

            lvJobQueue.Columns.Add(Headers.RunCommand.ToString(), "Command");
            lvJobQueue.Columns[Headers.RunCommand.ToString()].Width = 200;

            lvJobQueue.Columns.Add(Headers.WorkingDirectory.ToString(), "Directory");
            lvJobQueue.Columns[Headers.WorkingDirectory.ToString()].Width = 400;

            //lvJobQueue.AutoResizeColumns(ColumnHeaderAutoResizeStyle.ColumnContent);
        }

        private System.Threading.ManualResetEvent configured = new ManualResetEvent(false);
        private BlockingCollection<Action> jobsToBeStarted = new BlockingCollection<Action>(new ConcurrentQueue<Action>());
        private System.Threading.Thread JobsToBeStartedThread { get; set; }

        private void JobsToBeStartedCleaner(object _)
        {
            this.configured.WaitOne();
            while (true)
            {
                Action job = jobsToBeStarted.Take();
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

        public void JobAdded(JobImpl job, Job.StatusEnum status)
        {
            Trace.TraceInformation("Job added: {0} {1}", job.Id, job.Title);

            lock (this)
            {
                job.JobStatusChanged += JobStatusChanged;
                if (job.Status == Job.StatusEnum.Ready)
                {
                    jobsToBeStarted.Add(delegate() { StartJob(job); });
                }
                lvJobQueue.SafeInvoke(delegate
                {
                    var item = new ListViewItem(new string[] 
									{
										job.Id.ToString(),
										job.Title,
                                        job.TestBenchName,
										job.Status.ToString(),
                                        "", // META-2837
										job.RunCommand,
										job.WorkingDirectory
									});
                    for (int i = 0; i < lvJobQueue.Columns.Count; i++)
                    {
                        item.SubItems[i].Name = lvJobQueue.Columns[i].Name;
                    }
                    lvJobQueue.Items.Add(item);
                    AutoResizeColumns();
                });
            }
        }

        Thread SoTWorkerThread = null;
        System.Collections.Concurrent.BlockingCollection<Action> SoTTodo = new System.Collections.Concurrent.BlockingCollection<Action>();

        List<SoTImpl> SoTs = new List<SoTImpl>();
        public void SoTAdded(SoT sot_)
        {
            SoTImpl sot = (SoTImpl)sot_;
            Trace.TraceInformation("SoT added.");
            lock (this)
            {
                if (SoTWorkerThread == null)
                {
                    SoTWorkerThread = new System.Threading.Thread(
                    delegate()
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
                jobsToBeStarted.Add(delegate()
                {
                    SoTTodo.Add(delegate { sot.Run(); });
                });
            }
        }

        const int AUTORESIZE_COLUMN_FREQUENCY = 32;
        int autoresize_counter = 10;

        private void AutoResizeColumns()
        {
            if (autoresize_counter-- < 0)
            {
                autoresize_counter = AUTORESIZE_COLUMN_FREQUENCY;
                lvJobQueue.AutoResizeColumns(ColumnHeaderAutoResizeStyle.ColumnContent);
                lvJobQueue.Columns[(int)Headers.Progress].Width = Math.Max(lvJobQueue.Columns[(int)Headers.Progress].Width, 40);
            }
        }

        public void StartJob(JobImpl job)
        {
            this.UpdateServiceStatus();
            Run(job);
        }

        private void Run(JobImpl job)
        {
            string failedLog = Path.Combine(job.WorkingDirectory, LocalPool.Failed);
            File.Delete(failedLog);

            TargetMachine target = runtimeConfig[job.Type];

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

            if (Properties.Settings.Default.RemoteExecution)
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
                        Thread.Sleep(global::JobManager.Jenkins.Jenkins.VF_JOB_POLL_FREQUENCY);

                        // put the job back to the list and try to start it again later
                        jobsToBeStarted.Add(() => { job.Start(); });
                        return;
                    }

                    // create a job on the remote machine

                    // KMS: add random hex number because SoT steps have the same WorkingDirectory
                    // ZsL: the 8 digit random hex numbers were not enough. If users had many jobs 500+ got exception the
                    //      jobname is not unique: problems in database due jobname is the primary key and on jenkins too.
                    var randomid = Path.GetFileName(job.WorkingDirectory); //Guid.NewGuid().ToString("N");
                    var anotherRandomId = Path.GetFileNameWithoutExtension(Path.GetRandomFileName());
                    string jobname = string.Format("{0}_{1}{2}", Properties.Settings.Default.UserID, randomid, anotherRandomId);
                    string description = string.Format("{0}_{1}_{2:00000}_{3}", Properties.Settings.Default.UserID, job.Title,
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
                        string zipFileUrl = Jenkins.UploadFileToVF(zipFile, jobname, delegate(int percent)
                        {
                            lvJobQueue.BeginInvoke((MethodInvoker)delegate()
                            {
                                ListViewItem item = lvJobQueue.Items.Cast<ListViewItem>().
                                   FirstOrDefault(x => x.SubItems[(int)Headers.Id].Text == job.Id.ToString());
                                if (item != null)
                                {
                                    item.SubItems[(int)Headers.Progress].Text = percent + "%";
                                }
                            });
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

                    var jobinfonew = Jenkins.CreateJob(jobname, config_xml);

                    if (jobinfonew == null)
                    {
                        // job creation failed
                        job.Status = Job.StatusEnum.FailedToUploadServer;
                        return;
                    }

                    job.Status = Job.StatusEnum.PostedToServer;

                    string returned_config_xml = Jenkins.GetJobConfig(jobname);
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
                    else ; // FIXME throw?

                    // send zip and start job
                    if (Jenkins.BuildJob(jobname, jobname, job.BuildQuery) == null)
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
                    Jenkins.DebugLog.WriteLine(ex.ToString().Replace("\n", "  "));
                    job.Status = Job.StatusEnum.FailedToUploadServer;
                    MessageBox.Show(ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    Trace.TraceError(ex.ToString());
                }

            }
            else
            {
                // if local
                pool.EnqueueJob(job);
            }
        }


        public void JobStatusChanged(Job job_, Job.StatusEnum status)
        {
            try
            {
                JobImpl job = (JobImpl)job_;
                Trace.TraceInformation("Form JobStatusChanged was called for: {0}, which changed status to {1}", job.Title, status.ToString());
                autoresize_counter--;
                lvJobQueue.SafeInvoke(delegate
                {
                    Color color = Color.White;
                    switch (status)
                    {
                        case Job.StatusEnum.Ready:
                            color = Color.PeachPuff;
                            break;
                        case Job.StatusEnum.QueuedLocal:
                        case Job.StatusEnum.QueuedOnServer:
                        case Job.StatusEnum.StartedOnServer:
                            color = Color.LightYellow;
                            break;
                        case Job.StatusEnum.UploadPackage:
                        case Job.StatusEnum.DownloadResults:
                            color = Color.PaleTurquoise;
                            break;
                        case Job.StatusEnum.PostedToServer:
                            color = Color.Khaki;
                            break;
                        case Job.StatusEnum.RunningOnServer:
                        case Job.StatusEnum.RunningLocal:
                            color = Color.LightSkyBlue;
                            break;
                        case Job.StatusEnum.Succeeded:
                            color = Color.LightGreen;
                            break;
                        case Job.StatusEnum.Failed:
                        case Job.StatusEnum.FailedAbortOnServer:
                        case Job.StatusEnum.FailedExecution:
                        case Job.StatusEnum.FailedToUploadServer:
                        case Job.StatusEnum.FailedToDownload:
                        case Job.StatusEnum.AbortOnServerRequested:
                            color = Color.Tomato;
                            break;
                        default:
                            color = Color.White;
                            break;
                    };

                    if (server.IsRemote)
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
                                saved.VFUrl = server.JenkinsUrl;
                                entities.SavedJobs.AddObject(saved);
                                entities.SaveChanges();
                                entities.SavedJobs.Detach(saved);
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
                                foreach (var entity in entities.SavedJobs.Where(x => x.JobName == remoteJobInfo.JenkinsJobName))
                                {
                                    entities.SavedJobs.DeleteObject(entity);
                                }
                                entities.SaveChanges();
                            }
                        }
                    }

                    var item = lvJobQueue.Items.
                        Cast<ListViewItem>().
                        FirstOrDefault(x => x.SubItems[(int)Headers.Id].Text == job.Id.ToString());

                    item.BackColor = color;
                    item.SubItems[(int)Headers.Status].Text = status.ToString();

                    AutoResizeColumns();
                });

                if (status == Job.StatusEnum.Ready)
                {
                    jobsToBeStarted.Add(delegate() { StartJob(job); });
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                Trace.TraceError(ex.ToString());
            }

        }

        void NotifyIcon_Click(object sender, EventArgs e)
        {
            if ((e as MouseEventArgs).Button == System.Windows.Forms.MouseButtons.Left)
            {
                WindowState = FormWindowState.Normal;
                Show();
                NotifyIcon.Visible = false;
            }
        }

        void JobManager_Resize(object sender, EventArgs e)
        {
            if (WindowState == FormWindowState.Minimized)
            {
                //NotifyIcon.Visible = true;
                //Hide();
            }
        }

        class ListViewItemComparer : System.Collections.IComparer
        {
            private int col;
            public ListViewItemComparer(int column)
            {
                col = column;
            }
            public int Compare(object x, object y)
            {
                int returnVal = -1;
                returnVal = String.Compare(((ListViewItem)x).SubItems[col].Text,
                ((ListViewItem)y).SubItems[col].Text);
                return returnVal;
            }
        }

        private void lvJobQueue_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            lvJobQueue.ListViewItemSorter = new ListViewItemComparer(e.Column);
            lvJobQueue.Sort();
        }

        void lvJobQueue_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            if (e.KeyData == (Keys.C | Keys.Control))
            {
                ListViewItem item = (sender as ListView).FocusedItem;
                if (item != null)
                {
                    Clipboard.SetText(item.SubItems[4].Text);
                }
            }
        }

        private void lvJobQueue_SelectedIndexChanged(object sender, EventArgs e)
        {
            var lv = sender as ListView;
            ListViewItem item = lv.FocusedItem;
            if (item != null)
            {
                string status = item.SubItems[Headers.Status.ToString()].Text;
                showInExplorerToolStripMenuItem.Enabled = true;
            }

            var selectedJobs = lv.SelectedItems.Cast<ListViewItem>().Select(x => server.Jobs.FirstOrDefault(job => job.Id.ToString() == x.SubItems[Headers.Id.ToString()].Text)).Cast<JobImpl>();

            openLogFileToolStripMenuItem.Enabled = selectedJobs.All(x =>
                x.IsFailed() &&
                File.Exists(Path.Combine(x.WorkingDirectory, LocalPool.Failed)));

            rerunToolStripMenuItem.Enabled = selectedJobs.All(x =>
                (x.IsFailed() || x.Status == Job.StatusEnum.Succeeded) &&
                x.RerunEnabled &&
                Directory.Exists(x.WorkingDirectory));

            retryDownloadToolStripMenuItem.Enabled = selectedJobs.All(x =>
                (x.Status == Job.StatusEnum.FailedToDownload) &&
                Directory.Exists(x.WorkingDirectory));

            abortExecutionOnServerToolStripMenuItem.Enabled = selectedJobs.All(x => new Job.StatusEnum[] { 
                        Job.StatusEnum.RunningOnServer, Job.StatusEnum.StartedOnServer, Job.StatusEnum.QueuedOnServer }.Contains(x.Status) &&
                    Directory.Exists(x.WorkingDirectory));

            makeHighPriorityToolStripMenuItem.Enabled = selectedJobs.All(x => new Job.StatusEnum[] { 
                    Job.StatusEnum.PostedToServer, Job.StatusEnum.StartedOnServer, Job.StatusEnum.QueuedOnServer }.Contains(x.Status)) &&
                highPriorityJobsRemaining >= selectedJobs.Count();
        }

        private bool WipeWorkspaceOnSuccess { get { return global::JobManager.Properties.Settings.Default.WipeWorkspaceOnSuccess; } }
        private bool DeleteJobOnSuccess { get { return global::JobManager.Properties.Settings.Default.DeleteJobOnSuccess; } }


        private DialogResult Configure(string password = null)
        {
            using (Configuration cfgRemote = new Configuration(Jenkins, password))
            {
                this.toolStripStatusLabel.Text = "Configuring";
                if (string.IsNullOrEmpty(password))
                {
                    var dr = cfgRemote.ShowDialog(this);
                    server.JenkinsUrl = Properties.Settings.Default.VehicleForgeUri;
                    server.UserName = Properties.Settings.Default.UserID;
                    server.IsRemote = Properties.Settings.Default.RemoteExecution;
                    return dr;
                }
                else
                {
                    cfgRemote.btnSave_Click(cfgRemote.btnSave, null);
                    server.JenkinsUrl = Properties.Settings.Default.VehicleForgeUri;
                    server.UserName = Properties.Settings.Default.UserID;
                    server.IsRemote = Properties.Settings.Default.RemoteExecution;
                    return cfgRemote.DialogResult;
                }
            }
        }

        private void logToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (File.Exists(this.TraceFileName))
            {
                Process.Start(this.TraceFileName);
            }
        }

        private void createJobToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateJob cj = new CreateJob();
            cj.Show(this);
        }

        private void showInExplorerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ListViewItem item = lvJobQueue.FocusedItem;
            if (item != null)
            {
                string directory = item.SubItems[Headers.WorkingDirectory.ToString()].Text;
                if (System.IO.Directory.Exists(directory))
                {
                    System.Diagnostics.Process.Start(directory);
                }
                else
                {
                    MessageBox.Show(String.Format("Directory does not exist. {0}", directory));
                }
                lvJobQueue.FocusedItem = item;
            }
        }

        private void openLogFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var toOpenLog = lvJobQueue
                .SelectedItems
                .Cast<ListViewItem>()
                .Select(x =>
                    server.Jobs.FirstOrDefault(job => job.Id.ToString() == x.SubItems[Headers.Id.ToString()].Text))
                .Where(x => x.IsFailed());

            foreach (var item in toOpenLog)
            {
                string directory = item.WorkingDirectory;
                string logFile = System.IO.Path.Combine(directory, LocalPool.Failed);
                if (System.IO.File.Exists(logFile))
                {
                    System.Diagnostics.Process.Start(logFile);
                }
                else
                {
                    MessageBox.Show(String.Format("Log file does not exist. {0}", logFile));
                }
            }
        }

        private void rerunToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var toReRun = lvJobQueue
                .SelectedItems
                .Cast<ListViewItem>()
                .SelectMany(x =>
                    server.Jobs.Where(job => job.Id.ToString() == x.SubItems[Headers.Id.ToString()].Text))
                .Cast<JobImpl>()
                .Where(x =>
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

        private void JobManager_Load(object sender, EventArgs e)
        {
            this.toolStripStatusLabel.Text = "Loading";
            ListViewHelper.EnableDoubleBuffer(this.lvJobQueue);
            ListViewHelper.EnableDoubleBuffer(this.remoteServiceStatusForm.lvRemoteNodes);
        }

        private void remoteStatusToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.remoteServiceStatusForm == null ||
                this.remoteServiceStatusForm.IsDisposed)
            {
                this.remoteServiceStatusForm = new RemoteServiceStatusForm();
                this.remoteServiceStatusForm.Show();
                this.UpdateServiceStatus();
            }
            else
            {
                this.remoteServiceStatusForm.Hide();
                this.remoteServiceStatusForm.Show();
                this.UpdateServiceStatus();
            }
        }

        private void retryDownloadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var toReDownload = lvJobQueue
                .SelectedItems
                .Cast<ListViewItem>()
                .SelectMany(x =>
                    server.Jobs.Where(job => job.Id.ToString() == x.SubItems[Headers.Id.ToString()].Text))
                .Where(x => x.Status == Job.StatusEnum.FailedToDownload);

            foreach (var selectedJob in toReDownload)
            {
                // selectedJob.SubItems[Headers.Status.ToString()].Text = "Redownload Queued";
                selectedJob.Status = Job.StatusEnum.RedownloadQueued;
            }
            RestartMonitorTimer();
        }

        private void abortExecutionOnServerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var toAbort = lvJobQueue
                .SelectedItems
                .Cast<ListViewItem>()
                .SelectMany(x =>
                    server.Jobs.Where(job => job.Id.ToString() == x.SubItems[Headers.Id.ToString()].Text))
                .Where(x => new Job.StatusEnum[] { 
                    Job.StatusEnum.RunningOnServer, Job.StatusEnum.StartedOnServer, Job.StatusEnum.QueuedOnServer }.Contains(x.Status));

            foreach (var selectedJob in toAbort)
            {
                // selectedJob.SubItems[Headers.Status.ToString()].Text = "Redownload Queued";
                selectedJob.Status = Job.StatusEnum.AbortOnServerRequested;
            }
            RestartMonitorTimer();
        }

        private void makeHighPriorityToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var toMakeHighPriority = lvJobQueue
                .SelectedItems
                .Cast<ListViewItem>()
                .SelectMany(x =>
                    server.Jobs.Where(job => job.Id.ToString() == x.SubItems[Headers.Id.ToString()].Text))
                .Where(x => new Job.StatusEnum[] {
                    Job.StatusEnum.PostedToServer, Job.StatusEnum.StartedOnServer, Job.StatusEnum.QueuedOnServer }.Contains(x.Status));

            foreach (var selectedJob in toMakeHighPriority)
            {
                RemoteJobInfo remoteJob;
                if (JobMap.TryGetValue(selectedJob, out remoteJob))
                {
                    // TODO: would be good to do this on another thread
                    Jenkins.MakeHighPriority(remoteJob.JenkinsJobName);
                    highPriorityJobsRemaining--;
                    // called primarly for UpdateServiceStatus, which will update the status bar
                    RestartMonitorTimer();
                }
            }
        }
    }

    /// <summary>
    /// http://stackoverflow.com/questions/2903172/indicate-truncation-in-tooltipstatuslabel-automatically
    /// </summary>
    [ToolStripItemDesignerAvailability(ToolStripItemDesignerAvailability.StatusStrip)]
    public class SpringLabel : ToolStripStatusLabel
    {
        public SpringLabel()
        {
            this.Spring = true;
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            var flags = TextFormatFlags.Left | TextFormatFlags.EndEllipsis;
            var bounds = new Rectangle(0, 0, this.Bounds.Width, this.Bounds.Height);
            TextRenderer.DrawText(e.Graphics, this.Text, this.Font, bounds, this.ForeColor, flags);
        }
    }


    public class DisableKeepAliveWebClient : WebClient
    {
        public bool? AllowReadStreamBuffering { get; set; }
        public CookieContainer cookies { get; set; }

        public DisableKeepAliveWebClient()
        {
        }

        protected override WebRequest GetWebRequest(Uri address)
        {
            WebRequest request = base.GetWebRequest(address);
            HttpWebRequest webRequest = request as HttpWebRequest;
            if (webRequest != null)
            {
                webRequest.Timeout = Jenkins.Jenkins.HTTP_WEB_REQUEST_TIMEOUT;
                webRequest.KeepAlive = false;
                webRequest.CookieContainer = cookies;
                if (AllowReadStreamBuffering != null)
                {
                    this.AllowReadStreamBuffering = AllowReadStreamBuffering;
                }
            }
            return request;
        }
    }

    public static class ExtensionMethods
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


        /// <summary>
        /// http://stackoverflow.com/questions/808867/invoke-or-begininvoke-cannot-be-called-on-a-control-until-the-window-handle-has
        /// </summary>
        /// <param name="uiElement"></param>
        /// <param name="updater"></param>
        /// <param name="forceSynchronous"></param>
        public static void SafeInvoke(
            this Control uiElement,
            Action updater,
            bool forceSynchronous = false)
        {
            if (uiElement == null)
            {
                throw new ArgumentNullException("uiElement");
            }

            if (uiElement.InvokeRequired)
            {
                // FIXME: there is a race here: the control may be Dispose()d after InvokeRequired, but before Invoke
                if (forceSynchronous)
                {
                    uiElement.Invoke((Action)delegate { SafeInvoke(uiElement, updater, forceSynchronous); });
                }
                else
                {
                    uiElement.BeginInvoke((Action)delegate { SafeInvoke(uiElement, updater, forceSynchronous); });
                }
            }
            else
            {
                if (uiElement.IsDisposed)
                {
                    // Zsolt: return is ok for us
                    return;
                    throw new ObjectDisposedException("Control is already disposed.");
                }

                updater();
            }
        }
    }
}
