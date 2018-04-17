namespace JobManager
{
	partial class CreateJob
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CreateJob));
            this.txtWorkingDir = new System.Windows.Forms.TextBox();
            this.txtRunCommand = new System.Windows.Forms.TextBox();
            this.txtTitle = new System.Windows.Forms.TextBox();
            this.btnOK = new System.Windows.Forms.Button();
            this.nupNumJob = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.nupNumJob)).BeginInit();
            this.SuspendLayout();
            // 
            // txtWorkingDir
            // 
            this.txtWorkingDir.Location = new System.Drawing.Point(13, 37);
            this.txtWorkingDir.Name = "txtWorkingDir";
            this.txtWorkingDir.Size = new System.Drawing.Size(259, 20);
            this.txtWorkingDir.TabIndex = 1;
            this.txtWorkingDir.Text = "f:\\mga\\";
            // 
            // txtRunCommand
            // 
            this.txtRunCommand.Location = new System.Drawing.Point(13, 63);
            this.txtRunCommand.Name = "txtRunCommand";
            this.txtRunCommand.Size = new System.Drawing.Size(259, 20);
            this.txtRunCommand.TabIndex = 2;
            this.txtRunCommand.Text = "cmd.exe";
            // 
            // txtTitle
            // 
            this.txtTitle.Location = new System.Drawing.Point(13, 12);
            this.txtTitle.Name = "txtTitle";
            this.txtTitle.Size = new System.Drawing.Size(259, 20);
            this.txtTitle.TabIndex = 0;
            this.txtTitle.Text = "New Job title";
            // 
            // btnOK
            // 
            this.btnOK.Location = new System.Drawing.Point(91, 149);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(101, 23);
            this.btnOK.TabIndex = 5;
            this.btnOK.Text = "Create and Post";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // nupNumJob
            // 
            this.nupNumJob.Location = new System.Drawing.Point(13, 150);
            this.nupNumJob.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nupNumJob.Name = "nupNumJob";
            this.nupNumJob.Size = new System.Drawing.Size(47, 20);
            this.nupNumJob.TabIndex = 4;
            this.nupNumJob.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.nupNumJob.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // CreateJob
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 177);
            this.Controls.Add(this.nupNumJob);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.txtRunCommand);
            this.Controls.Add(this.txtTitle);
            this.Controls.Add(this.txtWorkingDir);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "CreateJob";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Create a new job";
            ((System.ComponentModel.ISupportInitialize)(this.nupNumJob)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.TextBox txtWorkingDir;
		private System.Windows.Forms.TextBox txtRunCommand;
		private System.Windows.Forms.TextBox txtTitle;
		private System.Windows.Forms.Button btnOK;
		private System.Windows.Forms.NumericUpDown nupNumJob;
	}
}