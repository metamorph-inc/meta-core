namespace JobManager
{
	partial class Configuration
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Configuration));
            this.btnSave = new System.Windows.Forms.Button();
            this.pictureBoxLoading = new System.Windows.Forms.PictureBox();
            this.labelJenkinsTest = new System.Windows.Forms.Label();
            this.labelJenkinsUrl = new System.Windows.Forms.Label();
            this.linkCancelCheck = new System.Windows.Forms.LinkLabel();
            this.gbOnSuccess = new System.Windows.Forms.GroupBox();
            this.chbWipeWorkspaceOnSuccess = new System.Windows.Forms.CheckBox();
            this.chbDeleteOnSuccess = new System.Windows.Forms.CheckBox();
            this.btnOpenLink = new System.Windows.Forms.Button();
            this.panelRemote = new System.Windows.Forms.Panel();
            this.comboVehicleForgeURL = new System.Windows.Forms.ComboBox();
            this.mtbPassword = new System.Windows.Forms.MaskedTextBox();
            this.txtUsername = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.labelUserID = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.chbRemoteExec = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLoading)).BeginInit();
            this.gbOnSuccess.SuspendLayout();
            this.panelRemote.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnSave
            // 
            this.btnSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnSave.Location = new System.Drawing.Point(359, 231);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(75, 23);
            this.btnSave.TabIndex = 5;
            this.btnSave.Text = "Run";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // pictureBoxLoading
            // 
            this.pictureBoxLoading.Image = global::JobManager.Properties.Resources.ajax_loader;
            this.pictureBoxLoading.Location = new System.Drawing.Point(0, 0);
            this.pictureBoxLoading.Name = "pictureBoxLoading";
            this.pictureBoxLoading.Size = new System.Drawing.Size(16, 16);
            this.pictureBoxLoading.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxLoading.TabIndex = 5;
            this.pictureBoxLoading.TabStop = false;
            this.pictureBoxLoading.Visible = false;
            // 
            // labelJenkinsTest
            // 
            this.labelJenkinsTest.AutoSize = true;
            this.labelJenkinsTest.Location = new System.Drawing.Point(22, 3);
            this.labelJenkinsTest.Name = "labelJenkinsTest";
            this.labelJenkinsTest.Size = new System.Drawing.Size(98, 13);
            this.labelJenkinsTest.TabIndex = 60;
            this.labelJenkinsTest.Text = "Testing connection";
            this.labelJenkinsTest.Visible = false;
            // 
            // labelJenkinsUrl
            // 
            this.labelJenkinsUrl.AutoSize = true;
            this.labelJenkinsUrl.Location = new System.Drawing.Point(5, 4);
            this.labelJenkinsUrl.Name = "labelJenkinsUrl";
            this.labelJenkinsUrl.Size = new System.Drawing.Size(94, 13);
            this.labelJenkinsUrl.TabIndex = 7;
            this.labelJenkinsUrl.Text = "VehicleForge URL";
            // 
            // linkCancelCheck
            // 
            this.linkCancelCheck.AutoSize = true;
            this.linkCancelCheck.Location = new System.Drawing.Point(158, 3);
            this.linkCancelCheck.Name = "linkCancelCheck";
            this.linkCancelCheck.Size = new System.Drawing.Size(39, 13);
            this.linkCancelCheck.TabIndex = 80;
            this.linkCancelCheck.TabStop = true;
            this.linkCancelCheck.Text = "cancel";
            this.linkCancelCheck.Visible = false;
            this.linkCancelCheck.Click += new System.EventHandler(this.linkCancelCheck_Click);
            // 
            // gbOnSuccess
            // 
            this.gbOnSuccess.Controls.Add(this.chbWipeWorkspaceOnSuccess);
            this.gbOnSuccess.Controls.Add(this.chbDeleteOnSuccess);
            this.gbOnSuccess.Location = new System.Drawing.Point(0, 120);
            this.gbOnSuccess.Name = "gbOnSuccess";
            this.gbOnSuccess.Size = new System.Drawing.Size(114, 65);
            this.gbOnSuccess.TabIndex = 11;
            this.gbOnSuccess.TabStop = false;
            this.gbOnSuccess.Text = "On success";
            // 
            // chbWipeWorkspaceOnSuccess
            // 
            this.chbWipeWorkspaceOnSuccess.AutoSize = true;
            this.chbWipeWorkspaceOnSuccess.Checked = global::JobManager.Properties.Settings.Default.WipeWorkspaceOnSuccess;
            this.chbWipeWorkspaceOnSuccess.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chbWipeWorkspaceOnSuccess.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::JobManager.Properties.Settings.Default, "WipeWorkspaceOnSuccess", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.chbWipeWorkspaceOnSuccess.Location = new System.Drawing.Point(6, 19);
            this.chbWipeWorkspaceOnSuccess.Name = "chbWipeWorkspaceOnSuccess";
            this.chbWipeWorkspaceOnSuccess.Size = new System.Drawing.Size(106, 17);
            this.chbWipeWorkspaceOnSuccess.TabIndex = 7;
            this.chbWipeWorkspaceOnSuccess.Text = "Wipe workspace";
            this.chbWipeWorkspaceOnSuccess.UseVisualStyleBackColor = true;
            // 
            // chbDeleteOnSuccess
            // 
            this.chbDeleteOnSuccess.AutoSize = true;
            this.chbDeleteOnSuccess.Checked = global::JobManager.Properties.Settings.Default.DeleteJobOnSuccess;
            this.chbDeleteOnSuccess.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chbDeleteOnSuccess.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::JobManager.Properties.Settings.Default, "DeleteJobOnSuccess", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.chbDeleteOnSuccess.Location = new System.Drawing.Point(6, 42);
            this.chbDeleteOnSuccess.Name = "chbDeleteOnSuccess";
            this.chbDeleteOnSuccess.Size = new System.Drawing.Size(74, 17);
            this.chbDeleteOnSuccess.TabIndex = 8;
            this.chbDeleteOnSuccess.Text = "Delete job";
            this.chbDeleteOnSuccess.UseVisualStyleBackColor = true;
            // 
            // btnOpenLink
            // 
            this.btnOpenLink.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnOpenLink.Location = new System.Drawing.Point(344, 19);
            this.btnOpenLink.Name = "btnOpenLink";
            this.btnOpenLink.Size = new System.Drawing.Size(75, 23);
            this.btnOpenLink.TabIndex = 6;
            this.btnOpenLink.Text = "Open link";
            this.btnOpenLink.UseVisualStyleBackColor = true;
            this.btnOpenLink.Click += new System.EventHandler(this.btnOpenLink_Click);
            // 
            // panelRemote
            // 
            this.panelRemote.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelRemote.Controls.Add(this.comboVehicleForgeURL);
            this.panelRemote.Controls.Add(this.mtbPassword);
            this.panelRemote.Controls.Add(this.txtUsername);
            this.panelRemote.Controls.Add(this.label2);
            this.panelRemote.Controls.Add(this.labelUserID);
            this.panelRemote.Controls.Add(this.btnOpenLink);
            this.panelRemote.Controls.Add(this.labelJenkinsUrl);
            this.panelRemote.Controls.Add(this.gbOnSuccess);
            this.panelRemote.Location = new System.Drawing.Point(12, 36);
            this.panelRemote.Name = "panelRemote";
            this.panelRemote.Size = new System.Drawing.Size(422, 188);
            this.panelRemote.TabIndex = 17;
            // 
            // comboVehicleForgeURL
            // 
            this.comboVehicleForgeURL.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.comboVehicleForgeURL.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::JobManager.Properties.Settings.Default, "VehicleForgeUri", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.comboVehicleForgeURL.FormattingEnabled = true;
            this.comboVehicleForgeURL.Location = new System.Drawing.Point(5, 21);
            this.comboVehicleForgeURL.Name = "comboVehicleForgeURL";
            this.comboVehicleForgeURL.Size = new System.Drawing.Size(333, 21);
            this.comboVehicleForgeURL.TabIndex = 2;
            this.comboVehicleForgeURL.Text = global::JobManager.Properties.Settings.Default.VehicleForgeUri;
            // 
            // mtbPassword
            // 
            this.mtbPassword.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.mtbPassword.Location = new System.Drawing.Point(5, 94);
            this.mtbPassword.Name = "mtbPassword";
            this.mtbPassword.Size = new System.Drawing.Size(333, 20);
            this.mtbPassword.TabIndex = 4;
            this.mtbPassword.UseSystemPasswordChar = true;
            // 
            // txtUsername
            // 
            this.txtUsername.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtUsername.Location = new System.Drawing.Point(5, 55);
            this.txtUsername.Name = "txtUsername";
            this.txtUsername.Size = new System.Drawing.Size(333, 20);
            this.txtUsername.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(5, 78);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 13);
            this.label2.TabIndex = 14;
            this.label2.Text = "Password";
            // 
            // labelUserID
            // 
            this.labelUserID.AutoSize = true;
            this.labelUserID.Location = new System.Drawing.Point(5, 42);
            this.labelUserID.Name = "labelUserID";
            this.labelUserID.Size = new System.Drawing.Size(43, 13);
            this.labelUserID.TabIndex = 13;
            this.labelUserID.Text = "User ID";
            // 
            // panel2
            // 
            this.panel2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.panel2.Controls.Add(this.pictureBoxLoading);
            this.panel2.Controls.Add(this.linkCancelCheck);
            this.panel2.Controls.Add(this.labelJenkinsTest);
            this.panel2.Location = new System.Drawing.Point(16, 231);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(200, 23);
            this.panel2.TabIndex = 18;
            // 
            // chbRemoteExec
            // 
            this.chbRemoteExec.AutoSize = true;
            this.chbRemoteExec.Checked = global::JobManager.Properties.Settings.Default.RemoteExecution;
            this.chbRemoteExec.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::JobManager.Properties.Settings.Default, "RemoteExecution", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.chbRemoteExec.Location = new System.Drawing.Point(12, 13);
            this.chbRemoteExec.Name = "chbRemoteExec";
            this.chbRemoteExec.Size = new System.Drawing.Size(113, 17);
            this.chbRemoteExec.TabIndex = 1;
            this.chbRemoteExec.Text = "Remote Execution";
            this.chbRemoteExec.UseVisualStyleBackColor = true;
            // 
            // Configuration
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.ClientSize = new System.Drawing.Size(446, 260);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.chbRemoteExec);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.panelRemote);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Configuration";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "JobManager Configuration";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLoading)).EndInit();
            this.gbOnSuccess.ResumeLayout(false);
            this.gbOnSuccess.PerformLayout();
            this.panelRemote.ResumeLayout(false);
            this.panelRemote.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

        private System.Windows.Forms.PictureBox pictureBoxLoading;
        private System.Windows.Forms.Label labelJenkinsTest;
        private System.Windows.Forms.Label labelJenkinsUrl;
        private System.Windows.Forms.LinkLabel linkCancelCheck;
        private System.Windows.Forms.GroupBox gbOnSuccess;
        internal System.Windows.Forms.CheckBox chbWipeWorkspaceOnSuccess;
        internal System.Windows.Forms.CheckBox chbDeleteOnSuccess;
        private System.Windows.Forms.Button btnOpenLink;
        private System.Windows.Forms.CheckBox chbRemoteExec;
        private System.Windows.Forms.Panel panelRemote;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label labelUserID;
        internal System.Windows.Forms.MaskedTextBox mtbPassword;
        internal System.Windows.Forms.TextBox txtUsername;
        private System.Windows.Forms.ComboBox comboVehicleForgeURL;
        internal System.Windows.Forms.Button btnSave;
	}
}