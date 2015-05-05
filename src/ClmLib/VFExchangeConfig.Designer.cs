namespace ClmLib
{
    partial class VFExchangeConfig
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
            this.btnSave = new System.Windows.Forms.Button();
            this.panelRemote = new System.Windows.Forms.Panel();
            this.comboVehicleForgeURL = new System.Windows.Forms.ComboBox();
            this.mtbPassword = new System.Windows.Forms.MaskedTextBox();
            this.txtUsername = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.labelUserID = new System.Windows.Forms.Label();
            this.btnTestLink = new System.Windows.Forms.Button();
            this.labelJenkinsUrl = new System.Windows.Forms.Label();
            this.btnCancel = new System.Windows.Forms.Button();
            this.panelRemote.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnSave
            // 
            this.btnSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnSave.Location = new System.Drawing.Point(213, 147);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(75, 23);
            this.btnSave.TabIndex = 19;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
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
            this.panelRemote.Controls.Add(this.btnTestLink);
            this.panelRemote.Controls.Add(this.labelJenkinsUrl);
            this.panelRemote.Location = new System.Drawing.Point(12, 12);
            this.panelRemote.Name = "panelRemote";
            this.panelRemote.Size = new System.Drawing.Size(360, 130);
            this.panelRemote.TabIndex = 20;
            // 
            // comboVehicleForgeURL
            // 
            this.comboVehicleForgeURL.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.comboVehicleForgeURL.FormattingEnabled = true;
            this.comboVehicleForgeURL.Location = new System.Drawing.Point(5, 21);
            this.comboVehicleForgeURL.Name = "comboVehicleForgeURL";
            this.comboVehicleForgeURL.Size = new System.Drawing.Size(271, 21);
            this.comboVehicleForgeURL.TabIndex = 2;
            // 
            // mtbPassword
            // 
            this.mtbPassword.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.mtbPassword.Location = new System.Drawing.Point(5, 94);
            this.mtbPassword.Name = "mtbPassword";
            this.mtbPassword.Size = new System.Drawing.Size(271, 20);
            this.mtbPassword.TabIndex = 4;
            this.mtbPassword.UseSystemPasswordChar = true;
            // 
            // txtUsername
            // 
            this.txtUsername.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtUsername.Location = new System.Drawing.Point(5, 55);
            this.txtUsername.Name = "txtUsername";
            this.txtUsername.Size = new System.Drawing.Size(271, 20);
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
            this.labelUserID.Size = new System.Drawing.Size(55, 13);
            this.labelUserID.TabIndex = 13;
            this.labelUserID.Text = "Username";
            // 
            // btnTestLink
            // 
            this.btnTestLink.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnTestLink.Location = new System.Drawing.Point(282, 19);
            this.btnTestLink.Name = "btnTestLink";
            this.btnTestLink.Size = new System.Drawing.Size(75, 23);
            this.btnTestLink.TabIndex = 6;
            this.btnTestLink.Text = "Test Link";
            this.btnTestLink.UseVisualStyleBackColor = true;
            this.btnTestLink.Click += new System.EventHandler(this.btnTestLink_Click);
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
            // btnCancel
            // 
            this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(294, 147);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 21;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // VFExchangeConfig
            // 
            this.AccessibleRole = System.Windows.Forms.AccessibleRole.PageTabList;
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(384, 182);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.panelRemote);
            this.MaximizeBox = false;
            this.MinimumSize = new System.Drawing.Size(400, 220);
            this.Name = "VFExchangeConfig";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "VehicleForge Login";
            this.panelRemote.ResumeLayout(false);
            this.panelRemote.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        internal System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Panel panelRemote;
        private System.Windows.Forms.ComboBox comboVehicleForgeURL;
        internal System.Windows.Forms.MaskedTextBox mtbPassword;
        internal System.Windows.Forms.TextBox txtUsername;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label labelUserID;
        private System.Windows.Forms.Button btnTestLink;
        private System.Windows.Forms.Label labelJenkinsUrl;
        private System.Windows.Forms.Button btnCancel;
    }
}