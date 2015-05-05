namespace JobManager
{
    partial class JobManager
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            NotifyIcon.Dispose();
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(JobManager));
            this.panel1 = new System.Windows.Forms.Panel();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.lvJobQueue = new System.Windows.Forms.ListView();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.showInExplorerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openLogFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.rerunToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.retryDownloadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.abortExecutionOnServerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.makeHighPriorityToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createJobToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.logToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.remoteStatusToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panel1.SuspendLayout();
            this.contextMenuStrip1.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.statusStrip1);
            this.panel1.Controls.Add(this.lvJobQueue);
            this.panel1.Controls.Add(this.menuStrip1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(784, 562);
            this.panel1.TabIndex = 0;
            // 
            // statusStrip1
            // 
            this.statusStrip1.Location = new System.Drawing.Point(0, 540);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(784, 22);
            this.statusStrip1.TabIndex = 8;
            this.statusStrip1.Text = "statusStrip";
            // 
            // lvJobQueue
            // 
            this.lvJobQueue.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lvJobQueue.ContextMenuStrip = this.contextMenuStrip1;
            this.lvJobQueue.FullRowSelect = true;
            this.lvJobQueue.Location = new System.Drawing.Point(3, 27);
            this.lvJobQueue.Name = "lvJobQueue";
            this.lvJobQueue.Size = new System.Drawing.Size(778, 510);
            this.lvJobQueue.TabIndex = 4;
            this.lvJobQueue.UseCompatibleStateImageBehavior = false;
            this.lvJobQueue.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.lvJobQueue_ColumnClick);
            this.lvJobQueue.SelectedIndexChanged += new System.EventHandler(this.lvJobQueue_SelectedIndexChanged);
            this.lvJobQueue.KeyUp += new System.Windows.Forms.KeyEventHandler(this.lvJobQueue_KeyUp);
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.showInExplorerToolStripMenuItem,
            this.openLogFileToolStripMenuItem,
            this.rerunToolStripMenuItem,
            this.retryDownloadToolStripMenuItem,
            this.abortExecutionOnServerToolStripMenuItem,
            this.makeHighPriorityToolStripMenuItem});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(210, 158);
            // 
            // showInExplorerToolStripMenuItem
            // 
            this.showInExplorerToolStripMenuItem.Enabled = false;
            this.showInExplorerToolStripMenuItem.Name = "showInExplorerToolStripMenuItem";
            this.showInExplorerToolStripMenuItem.Size = new System.Drawing.Size(209, 22);
            this.showInExplorerToolStripMenuItem.Text = "Show in explorer";
            this.showInExplorerToolStripMenuItem.Click += new System.EventHandler(this.showInExplorerToolStripMenuItem_Click);
            // 
            // openLogFileToolStripMenuItem
            // 
            this.openLogFileToolStripMenuItem.Enabled = false;
            this.openLogFileToolStripMenuItem.Name = "openLogFileToolStripMenuItem";
            this.openLogFileToolStripMenuItem.Size = new System.Drawing.Size(209, 22);
            this.openLogFileToolStripMenuItem.Text = "Open log file";
            this.openLogFileToolStripMenuItem.Click += new System.EventHandler(this.openLogFileToolStripMenuItem_Click);
            // 
            // rerunToolStripMenuItem
            // 
            this.rerunToolStripMenuItem.Enabled = false;
            this.rerunToolStripMenuItem.Name = "rerunToolStripMenuItem";
            this.rerunToolStripMenuItem.Size = new System.Drawing.Size(209, 22);
            this.rerunToolStripMenuItem.Text = "Rerun";
            this.rerunToolStripMenuItem.Click += new System.EventHandler(this.rerunToolStripMenuItem_Click);
            // 
            // retryDownloadToolStripMenuItem
            // 
            this.retryDownloadToolStripMenuItem.Enabled = false;
            this.retryDownloadToolStripMenuItem.Name = "retryDownloadToolStripMenuItem";
            this.retryDownloadToolStripMenuItem.Size = new System.Drawing.Size(209, 22);
            this.retryDownloadToolStripMenuItem.Text = "Retry download";
            this.retryDownloadToolStripMenuItem.Click += new System.EventHandler(this.retryDownloadToolStripMenuItem_Click);
            // 
            // abortExecutionOnServerToolStripMenuItem
            // 
            this.abortExecutionOnServerToolStripMenuItem.Enabled = false;
            this.abortExecutionOnServerToolStripMenuItem.Name = "abortExecutionOnServerToolStripMenuItem";
            this.abortExecutionOnServerToolStripMenuItem.Size = new System.Drawing.Size(209, 22);
            this.abortExecutionOnServerToolStripMenuItem.Text = "Abort execution on server";
            this.abortExecutionOnServerToolStripMenuItem.Click += new System.EventHandler(this.abortExecutionOnServerToolStripMenuItem_Click);
            // 
            // makeHighPriorityToolStripMenuItem
            // 
            this.makeHighPriorityToolStripMenuItem.Enabled = false;
            this.makeHighPriorityToolStripMenuItem.Name = "makeHighPriorityToolStripMenuItem";
            this.makeHighPriorityToolStripMenuItem.Size = new System.Drawing.Size(209, 22);
            this.makeHighPriorityToolStripMenuItem.Text = "Make high priority";
            this.makeHighPriorityToolStripMenuItem.Click += new System.EventHandler(this.makeHighPriorityToolStripMenuItem_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.viewToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(784, 24);
            this.menuStrip1.TabIndex = 7;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.createJobToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // createJobToolStripMenuItem
            // 
            this.createJobToolStripMenuItem.Name = "createJobToolStripMenuItem";
            this.createJobToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.createJobToolStripMenuItem.Text = "Create Job";
            this.createJobToolStripMenuItem.Visible = false;
            this.createJobToolStripMenuItem.Click += new System.EventHandler(this.createJobToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.F4)));
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.exitToolStripMenuItem.Text = "E&xit";
            // 
            // viewToolStripMenuItem
            // 
            this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.logToolStripMenuItem,
            this.remoteStatusToolStripMenuItem});
            this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
            this.viewToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.viewToolStripMenuItem.Text = "&View";
            // 
            // logToolStripMenuItem
            // 
            this.logToolStripMenuItem.Name = "logToolStripMenuItem";
            this.logToolStripMenuItem.Size = new System.Drawing.Size(106, 22);
            this.logToolStripMenuItem.Text = "Log";
            this.logToolStripMenuItem.ToolTipText = "Opens the log file with the deafult text editor.";
            this.logToolStripMenuItem.Click += new System.EventHandler(this.logToolStripMenuItem_Click);
            // 
            // remoteStatusToolStripMenuItem
            // 
            this.remoteStatusToolStripMenuItem.Enabled = false;
            this.remoteStatusToolStripMenuItem.Name = "remoteStatusToolStripMenuItem";
            this.remoteStatusToolStripMenuItem.Size = new System.Drawing.Size(106, 22);
            this.remoteStatusToolStripMenuItem.Text = "Status";
            this.remoteStatusToolStripMenuItem.ToolTipText = "Shows detailed status of the remote execution service.";
            this.remoteStatusToolStripMenuItem.Visible = false;
            this.remoteStatusToolStripMenuItem.Click += new System.EventHandler(this.remoteStatusToolStripMenuItem_Click);
            // 
            // JobManager
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(784, 562);
            this.Controls.Add(this.panel1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "JobManager";
            this.Text = "META Job Manager";
            this.Load += new System.EventHandler(this.JobManager_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.contextMenuStrip1.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.ListView lvJobQueue;
                private System.Windows.Forms.MenuStrip menuStrip1;
                private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem logToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem createJobToolStripMenuItem;
                private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
                private System.Windows.Forms.ToolStripMenuItem showInExplorerToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem openLogFileToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem rerunToolStripMenuItem;
                private System.Windows.Forms.StatusStrip statusStrip1;
                private System.Windows.Forms.ToolStripMenuItem remoteStatusToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem retryDownloadToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem abortExecutionOnServerToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem makeHighPriorityToolStripMenuItem;

    }
}

