namespace JobManager
{
    partial class RemoteServiceStatusForm
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
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(RemoteServiceStatusForm));
            this.lvRemoteNodes = new System.Windows.Forms.ListView();
            this.gbRemoteNodes = new System.Windows.Forms.GroupBox();
            this.lblTotalCount = new System.Windows.Forms.Label();
            this.lblFreeCount = new System.Windows.Forms.Label();
            this.lblBusyCount = new System.Windows.Forms.Label();
            this.lblTotalNumberLabel = new System.Windows.Forms.Label();
            this.lblTotalFreeLabel = new System.Windows.Forms.Label();
            this.lblTotalBusyLabel = new System.Windows.Forms.Label();
            this.lblVFStatusLabel = new System.Windows.Forms.Label();
            this.lblNodesStatusLabel = new System.Windows.Forms.Label();
            this.lblVFStatus = new System.Windows.Forms.Label();
            this.lblNodeStatus = new System.Windows.Forms.Label();
            this.gbRemoteNodes.SuspendLayout();
            this.SuspendLayout();
            // 
            // lvRemoteNodes
            // 
            this.lvRemoteNodes.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lvRemoteNodes.FullRowSelect = true;
            this.lvRemoteNodes.GridLines = true;
            this.lvRemoteNodes.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.lvRemoteNodes.Location = new System.Drawing.Point(10, 76);
            this.lvRemoteNodes.Name = "lvRemoteNodes";
            this.lvRemoteNodes.Size = new System.Drawing.Size(259, 126);
            this.lvRemoteNodes.TabIndex = 0;
            this.lvRemoteNodes.UseCompatibleStateImageBehavior = false;
            // 
            // gbRemoteNodes
            // 
            this.gbRemoteNodes.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.gbRemoteNodes.Controls.Add(this.lblTotalCount);
            this.gbRemoteNodes.Controls.Add(this.lblFreeCount);
            this.gbRemoteNodes.Controls.Add(this.lblBusyCount);
            this.gbRemoteNodes.Controls.Add(this.lblTotalNumberLabel);
            this.gbRemoteNodes.Controls.Add(this.lblTotalFreeLabel);
            this.gbRemoteNodes.Controls.Add(this.lblTotalBusyLabel);
            this.gbRemoteNodes.Controls.Add(this.lvRemoteNodes);
            this.gbRemoteNodes.Location = new System.Drawing.Point(3, 48);
            this.gbRemoteNodes.Name = "gbRemoteNodes";
            this.gbRemoteNodes.Size = new System.Drawing.Size(278, 213);
            this.gbRemoteNodes.TabIndex = 1;
            this.gbRemoteNodes.TabStop = false;
            this.gbRemoteNodes.Text = "Remote node status";
            // 
            // lblTotalCount
            // 
            this.lblTotalCount.Location = new System.Drawing.Point(10, 55);
            this.lblTotalCount.Name = "lblTotalCount";
            this.lblTotalCount.Size = new System.Drawing.Size(25, 13);
            this.lblTotalCount.TabIndex = 5;
            this.lblTotalCount.Text = "-";
            this.lblTotalCount.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lblFreeCount
            // 
            this.lblFreeCount.Location = new System.Drawing.Point(10, 38);
            this.lblFreeCount.Name = "lblFreeCount";
            this.lblFreeCount.Size = new System.Drawing.Size(25, 13);
            this.lblFreeCount.TabIndex = 4;
            this.lblFreeCount.Text = "-";
            this.lblFreeCount.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lblBusyCount
            // 
            this.lblBusyCount.Location = new System.Drawing.Point(10, 20);
            this.lblBusyCount.Name = "lblBusyCount";
            this.lblBusyCount.Size = new System.Drawing.Size(25, 13);
            this.lblBusyCount.TabIndex = 3;
            this.lblBusyCount.Text = "-";
            this.lblBusyCount.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lblTotalNumberLabel
            // 
            this.lblTotalNumberLabel.AutoSize = true;
            this.lblTotalNumberLabel.Location = new System.Drawing.Point(35, 55);
            this.lblTotalNumberLabel.Name = "lblTotalNumberLabel";
            this.lblTotalNumberLabel.Size = new System.Drawing.Size(130, 13);
            this.lblTotalNumberLabel.TabIndex = 2;
            this.lblTotalNumberLabel.Text = "Total number of executors";
            // 
            // lblTotalFreeLabel
            // 
            this.lblTotalFreeLabel.AutoSize = true;
            this.lblTotalFreeLabel.Location = new System.Drawing.Point(35, 38);
            this.lblTotalFreeLabel.Name = "lblTotalFreeLabel";
            this.lblTotalFreeLabel.Size = new System.Drawing.Size(101, 13);
            this.lblTotalFreeLabel.TabIndex = 1;
            this.lblTotalFreeLabel.Text = "Total free executors";
            // 
            // lblTotalBusyLabel
            // 
            this.lblTotalBusyLabel.AutoSize = true;
            this.lblTotalBusyLabel.Location = new System.Drawing.Point(35, 20);
            this.lblTotalBusyLabel.Name = "lblTotalBusyLabel";
            this.lblTotalBusyLabel.Size = new System.Drawing.Size(105, 13);
            this.lblTotalBusyLabel.TabIndex = 0;
            this.lblTotalBusyLabel.Text = "Total busy executors";
            // 
            // lblVFStatusLabel
            // 
            this.lblVFStatusLabel.AutoSize = true;
            this.lblVFStatusLabel.Location = new System.Drawing.Point(69, 9);
            this.lblVFStatusLabel.Name = "lblVFStatusLabel";
            this.lblVFStatusLabel.Size = new System.Drawing.Size(118, 13);
            this.lblVFStatusLabel.TabIndex = 2;
            this.lblVFStatusLabel.Text = "VF status [OK | DOWN]";
            // 
            // lblNodesStatusLabel
            // 
            this.lblNodesStatusLabel.AutoSize = true;
            this.lblNodesStatusLabel.Location = new System.Drawing.Point(69, 29);
            this.lblNodesStatusLabel.Name = "lblNodesStatusLabel";
            this.lblNodesStatusLabel.Size = new System.Drawing.Size(206, 13);
            this.lblNodesStatusLabel.TabIndex = 3;
            this.lblNodesStatusLabel.Text = "Remote node status [OK | BUSY | DOWN]";
            // 
            // lblVFStatus
            // 
            this.lblVFStatus.AutoSize = true;
            this.lblVFStatus.Location = new System.Drawing.Point(13, 9);
            this.lblVFStatus.Name = "lblVFStatus";
            this.lblVFStatus.Size = new System.Drawing.Size(42, 13);
            this.lblVFStatus.TabIndex = 6;
            this.lblVFStatus.Text = "DOWN";
            this.lblVFStatus.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lblNodeStatus
            // 
            this.lblNodeStatus.AutoSize = true;
            this.lblNodeStatus.Location = new System.Drawing.Point(13, 29);
            this.lblNodeStatus.Name = "lblNodeStatus";
            this.lblNodeStatus.Size = new System.Drawing.Size(42, 13);
            this.lblNodeStatus.TabIndex = 7;
            this.lblNodeStatus.Text = "DOWN";
            this.lblNodeStatus.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // RemoteServiceStatusForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Controls.Add(this.lblNodeStatus);
            this.Controls.Add(this.lblVFStatus);
            this.Controls.Add(this.lblNodesStatusLabel);
            this.Controls.Add(this.lblVFStatusLabel);
            this.Controls.Add(this.gbRemoteNodes);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(300, 300);
            this.Name = "RemoteServiceStatusForm";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Remote Service Status";
            this.gbRemoteNodes.ResumeLayout(false);
            this.gbRemoteNodes.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        internal System.Windows.Forms.ListView lvRemoteNodes;
        internal System.Windows.Forms.GroupBox gbRemoteNodes;
        internal System.Windows.Forms.Label lblTotalCount;
        internal System.Windows.Forms.Label lblFreeCount;
        internal System.Windows.Forms.Label lblBusyCount;
        private System.Windows.Forms.Label lblTotalNumberLabel;
        private System.Windows.Forms.Label lblTotalFreeLabel;
        private System.Windows.Forms.Label lblTotalBusyLabel;
        private System.Windows.Forms.Label lblVFStatusLabel;
        private System.Windows.Forms.Label lblNodesStatusLabel;
        internal System.Windows.Forms.Label lblVFStatus;
        internal System.Windows.Forms.Label lblNodeStatus;

    }
}