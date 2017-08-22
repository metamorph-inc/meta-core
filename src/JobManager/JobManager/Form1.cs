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
using JobManagerFramework;
using JobManagerFramework.Jenkins;


namespace JobManager
{
    public partial class JobManagerForm : Form
    {
        private SpringLabel toolStripStatusLabel { get; set; }

        private RemoteServiceStatusForm remoteServiceStatusForm { get; set; }

        public NotifyIcon NotifyIcon;

        private string TempDir { get; set; }

        private JobManagerFramework.JobManager manager;

        private JobServerImpl Server
        {
            get
            {
                return manager.Server;
            }
        }

        private enum Headers
        {
            Id, Title, TestBenchName, Status, Progress, RunCommand, WorkingDirectory
        }

        private int highPriorityJobsRemaining = 0;

        /// <summary>
        /// Updates all controls that displays status of the remote servers.
        /// </summary>
        private void UpdateServiceStatus(JobManagerFramework.JobManager.ServiceStatus serviceStatus)
        {
            var sb = new StringBuilder();
            highPriorityJobsRemaining = 0;
            var statusInfo = serviceStatus.statusInfo;
            if (serviceStatus.isRemoteServer == false)
            {
                return;
            }
            if (serviceStatus.vfConnected)
            {
                this.remoteServiceStatusForm.lblVFStatus.Text = "UP";
                if (statusInfo != null)
                {
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
                sb.AppendFormat("VF is NOT connected. {0}", this.manager.Server.JenkinsUrl);
                this.remoteServiceStatusForm.SafeInvoke(delegate
                {
                    this.remoteServiceStatusForm.lblVFStatus.Text = "DOWN";
                    this.remoteServiceStatusForm.lblNodeStatus.Text = "DOWN";
                    this.remoteServiceStatusForm.gbRemoteNodes.Visible = false;
                });
            }

            // this.toolStripStatusLabel.Text = sb.ToString();
            return;
        }

        public string TraceFileName { get; set; }
        private string password { get; set; }

        public JobManagerForm(Dictionary<string, string> settings = null)
        {
            InitializeComponent();

            InitJobQueue();

            this.toolStripStatusLabel = new SpringLabel();
            this.toolStripStatusLabel.Text = "Loading";
            this.statusStrip1.Items.Add(this.toolStripStatusLabel);
            this.lvJobQueue.DoubleClick += showInExplorerToolStripMenuItem_Click;

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

            var fs = new FileStream(this.TraceFileName, FileMode.Create);
            TraceListener fileTL = new TextWriterTraceListener(fs)
            {
                TraceOutputOptions = TraceOptions.DateTime
            };
            // use TraceXXX to get timestamp per http://stackoverflow.com/questions/863394/add-timestamp-to-trace-writeline

            Trace.AutoFlush = true;
            Trace.Listeners.Add(fileTL);

            Trace.TraceInformation(META.Logger.Header());

            manager = new JobManagerFramework.JobManager();

            this.FormClosing += (sender, args) =>
            {
                if (manager.HasIncompleteSots)
                {
                    DialogResult dr = MessageBox.Show(this, "Some jobs have not been posted and will be lost. Do you want to exit anyways?", "JobManager", MessageBoxButtons.YesNo);
                    if (dr == DialogResult.No)
                    {
                        args.Cancel = true;
                        return;
                    }
                }
                if (manager.IsRemote == false)
                {
                    int unfinished = manager.UnfinishedJobCount;
                    if (unfinished > 0)
                    {
                        DialogResult dr = MessageBox.Show(this, unfinished + " jobs have not completed. Do you want to exit and terminate running processes?", "JobManager", MessageBoxButtons.YesNo);
                        if (dr == DialogResult.No)
                        {
                            args.Cancel = true;
                            return;
                        }
                    }
                }
                Trace.TraceInformation("JobManager is closing");
                manager.Dispose();
                manager = null;
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

            manager.JobAdded += Manager_JobAdded;
            manager.SotAdded += Manager_SotAdded;
            //pool.JobStatusChanged += JobStatusChanged;

            if (settings.ContainsKey("-i"))
            {
                this.toolStripStatusLabel.Text = "Configured for local execution.";
                this.remoteStatusToolStripMenuItem.Enabled = false;
                this.remoteStatusToolStripMenuItem.Visible = false;
                return;
            }
            Shown += new EventHandler(delegate (object o, EventArgs args)
                {
                    if (this.Configure() == DialogResult.OK)
                    {
                        lock (this)
                        {
                            //TODO: determine if set for remote execution and show in status bar
                            if (manager.IsRemote)
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

                            manager.ServiceStatusUpdated += UpdateServiceStatus;
                            manager.LoadSavedJobs();
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

        private void Manager_SotAdded(object sender, JobManagerFramework.JobManager.SotAddedEventArgs e)
        {
            // TODO: do we need to do anything here?
        }

        private void Manager_JobAdded(object sender, JobManagerFramework.JobManager.JobAddedEventArgs e)
        {
            ((JobImpl)e.Job).JobStatusChanged += JobStatusChanged;

            lvJobQueue.SafeInvoke(delegate
            {
                var item = new ListViewItem(new string[]
                                {
                                        e.Job.Id.ToString(),
                                        e.Job.Title,
                                        e.Job.TestBenchName,
                                        e.Job.Status.ToString(),
                                        "", // META-2837
										e.Job.RunCommand,
                                        e.Job.WorkingDirectory
                                });
                for (int i = 0; i < lvJobQueue.Columns.Count; i++)
                {
                    item.SubItems[i].Name = lvJobQueue.Columns[i].Name;
                }
                lvJobQueue.Items.Add(item);
                AutoResizeColumns();
            });
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

        /*public void JobAdded(JobImpl job, Job.StatusEnum status)
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
        }*/

        /*public void SoTAdded(SoT sot_)
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
        }*/

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
                        case Job.StatusEnum.FailedAbortByUser:
                        case Job.StatusEnum.FailedExecution:
                        case Job.StatusEnum.FailedToUploadServer:
                        case Job.StatusEnum.FailedToDownload:
                        case Job.StatusEnum.AbortOnServerRequested:
                            color = Color.Tomato;
                            break;
                        default:
                            color = Color.White;
                            break;
                    }

                    var item = lvJobQueue.Items.
                        Cast<ListViewItem>().
                        FirstOrDefault(x => x.SubItems[(int)Headers.Id].Text == job.Id.ToString());

                    item.BackColor = color;
                    item.SubItems[(int)Headers.Status].Text = status.ToString();

                    AutoResizeColumns();
                });
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

            var selectedJobs = lv.SelectedItems.Cast<ListViewItem>().Select(x => manager.Jobs.FirstOrDefault(job => job.Id.ToString() == x.SubItems[Headers.Id.ToString()].Text)).Cast<JobImpl>();

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


        private DialogResult Configure()
        {
            using (Configuration cfgRemote = new Configuration(manager, password))
            {
                this.toolStripStatusLabel.Text = "Configuring";
                DialogResult dr;
                if (string.IsNullOrEmpty(password))
                {
                    dr = cfgRemote.ShowDialog(this);
                }
                else
                {
                    cfgRemote.btnSave_Click(cfgRemote.btnSave, null);
                    dr = cfgRemote.DialogResult;
                }
                Server.IsRemote = Properties.Settings.Default.RemoteExecution;
                
                if (Server.IsRemote)
                {
                    Server.JenkinsUrl = Properties.Settings.Default.VehicleForgeUri;
                    Server.UserName = Properties.Settings.Default.UserID;
                }
                return dr;
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
                    manager.Jobs.FirstOrDefault(job => job.Id.ToString() == x.SubItems[Headers.Id.ToString()].Text))
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
                    manager.Jobs.Where(job => job.Id.ToString() == x.SubItems[Headers.Id.ToString()].Text));

            manager.ReRunJobs(toReRun);
        }

        private void JobManager_Load(object sender, EventArgs e)
        {
            ListViewHelper.EnableDoubleBuffer(this.lvJobQueue);
            ListViewHelper.EnableDoubleBuffer(this.remoteServiceStatusForm.lvRemoteNodes);
        }

        private void remoteStatusToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.remoteServiceStatusForm == null ||
                this.remoteServiceStatusForm.IsDisposed)
            {
                this.remoteServiceStatusForm = new RemoteServiceStatusForm();
                this.remoteServiceStatusForm.Show(this);
            }
            else
            {
                this.remoteServiceStatusForm.Hide();
                this.remoteServiceStatusForm.Show(this);
            }
        }

        private void retryDownloadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var toReDownload = lvJobQueue
                .SelectedItems
                .Cast<ListViewItem>()
                .SelectMany(x =>
                        manager.Jobs.Where(job => job.Id.ToString() == x.SubItems[Headers.Id.ToString()].Text));

            manager.RetryDownload(toReDownload);
        }

        private void abortExecutionOnServerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var toAbort = lvJobQueue
                .SelectedItems
                .Cast<ListViewItem>()
                .SelectMany(x =>
                        manager.Jobs.Where(job => job.Id.ToString() == x.SubItems[Headers.Id.ToString()].Text));

            manager.AbortJobs(toAbort);
        }

        private void makeHighPriorityToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var toMakeHighPriority = lvJobQueue
                .SelectedItems
                .Cast<ListViewItem>()
                .SelectMany(x =>
                        manager.Jobs.Where(job => job.Id.ToString() == x.SubItems[Headers.Id.ToString()].Text));

            manager.MakeHighPriority(toMakeHighPriority);
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
                {
                    continue;
                }

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
