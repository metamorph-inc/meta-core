namespace ClmLib
{
    partial class VFDownloadProgressDialog
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
            this.pbDownloadProgress = new System.Windows.Forms.ProgressBar();
            this.lblDownloadingText = new System.Windows.Forms.Label();
            this.lblNumberComponents = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // pbDownloadProgress
            // 
            this.pbDownloadProgress.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pbDownloadProgress.Location = new System.Drawing.Point(12, 29);
            this.pbDownloadProgress.Name = "pbDownloadProgress";
            this.pbDownloadProgress.Size = new System.Drawing.Size(260, 23);
            this.pbDownloadProgress.TabIndex = 0;
            // 
            // lblDownloadingText
            // 
            this.lblDownloadingText.AutoSize = true;
            this.lblDownloadingText.Location = new System.Drawing.Point(13, 9);
            this.lblDownloadingText.Name = "lblDownloadingText";
            this.lblDownloadingText.Size = new System.Drawing.Size(72, 13);
            this.lblDownloadingText.TabIndex = 1;
            this.lblDownloadingText.Text = "Downloading ";
            // 
            // lblNumberComponents
            // 
            this.lblNumberComponents.AutoSize = true;
            this.lblNumberComponents.Location = new System.Drawing.Point(91, 9);
            this.lblNumberComponents.Name = "lblNumberComponents";
            this.lblNumberComponents.Size = new System.Drawing.Size(121, 13);
            this.lblNumberComponents.TabIndex = 2;
            this.lblNumberComponents.Text = "___________________";
            // 
            // VFDownloadProgressDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.ClientSize = new System.Drawing.Size(284, 62);
            this.Controls.Add(this.lblNumberComponents);
            this.Controls.Add(this.lblDownloadingText);
            this.Controls.Add(this.pbDownloadProgress);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "VFDownloadProgressDialog";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Download Progress";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblDownloadingText;
        public System.Windows.Forms.ProgressBar pbDownloadProgress;
        public System.Windows.Forms.Label lblNumberComponents;
    }
}