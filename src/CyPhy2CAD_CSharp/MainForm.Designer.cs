namespace CyPhy2CAD_CSharp
{
    partial class MainForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.button_OutputDir = new System.Windows.Forms.Button();
            this.button_AuxiliaryDir = new System.Windows.Forms.Button();
            this.textBox_OutputDir = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox_AuxiliaryDir = new System.Windows.Forms.TextBox();
            this.button_Cancel = new System.Windows.Forms.Button();
            this.button_Ok = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.clb_Step = new System.Windows.Forms.CheckedListBox();
            this.SuspendLayout();
            // 
            // button_OutputDir
            // 
            this.button_OutputDir.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.button_OutputDir.Location = new System.Drawing.Point(484, 27);
            this.button_OutputDir.Name = "button_OutputDir";
            this.button_OutputDir.Size = new System.Drawing.Size(51, 23);
            this.button_OutputDir.TabIndex = 2;
            this.button_OutputDir.Text = "...";
            this.button_OutputDir.UseVisualStyleBackColor = true;
            this.button_OutputDir.Click += new System.EventHandler(this.button_OutputDir_Click);
            // 
            // button_AuxiliaryDir
            // 
            this.button_AuxiliaryDir.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.button_AuxiliaryDir.Location = new System.Drawing.Point(484, 73);
            this.button_AuxiliaryDir.Name = "button_AuxiliaryDir";
            this.button_AuxiliaryDir.Size = new System.Drawing.Size(51, 23);
            this.button_AuxiliaryDir.TabIndex = 4;
            this.button_AuxiliaryDir.Text = "...";
            this.button_AuxiliaryDir.UseVisualStyleBackColor = true;
            this.button_AuxiliaryDir.Click += new System.EventHandler(this.button_AuxiliaryDir_Click);
            // 
            // textBox_OutputDir
            // 
            this.textBox_OutputDir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBox_OutputDir.Location = new System.Drawing.Point(12, 29);
            this.textBox_OutputDir.Name = "textBox_OutputDir";
            this.textBox_OutputDir.Size = new System.Drawing.Size(466, 20);
            this.textBox_OutputDir.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 10);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(87, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Output Directory:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 59);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(118, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "CAD Auxiliary Directory:";
            // 
            // textBox_AuxiliaryDir
            // 
            this.textBox_AuxiliaryDir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBox_AuxiliaryDir.Location = new System.Drawing.Point(12, 75);
            this.textBox_AuxiliaryDir.Name = "textBox_AuxiliaryDir";
            this.textBox_AuxiliaryDir.Size = new System.Drawing.Size(466, 20);
            this.textBox_AuxiliaryDir.TabIndex = 3;
            // 
            // button_Cancel
            // 
            this.button_Cancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.button_Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.button_Cancel.Location = new System.Drawing.Point(460, 218);
            this.button_Cancel.Name = "button_Cancel";
            this.button_Cancel.Size = new System.Drawing.Size(75, 23);
            this.button_Cancel.TabIndex = 7;
            this.button_Cancel.Text = "Cancel";
            this.button_Cancel.UseVisualStyleBackColor = true;
            this.button_Cancel.Click += new System.EventHandler(this.button_Cancel_Click);
            // 
            // button_Ok
            // 
            this.button_Ok.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.button_Ok.Location = new System.Drawing.Point(378, 218);
            this.button_Ok.Name = "button_Ok";
            this.button_Ok.Size = new System.Drawing.Size(75, 23);
            this.button_Ok.TabIndex = 6;
            this.button_Ok.Text = "OK";
            this.button_Ok.UseVisualStyleBackColor = true;
            this.button_Ok.Click += new System.EventHandler(this.button_Ok_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 112);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(78, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "STEP Formats:";
            // 
            // clb_Step
            // 
            this.clb_Step.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.clb_Step.CheckOnClick = true;
            this.clb_Step.FormattingEnabled = true;
            this.clb_Step.Items.AddRange(new object[] {
            "AP203_Single_File",
            "AP203_E2_Single_File",
            "AP203_E2_Separate_Part_Files",
            "AP214_Single_File",
            "AP214_Separate_Part_Files"});
            this.clb_Step.Location = new System.Drawing.Point(12, 130);
            this.clb_Step.Name = "clb_Step";
            this.clb_Step.Size = new System.Drawing.Size(522, 79);
            this.clb_Step.TabIndex = 11;
            // 
            // MainForm
            // 
            this.AcceptButton = this.button_Ok;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.button_Cancel;
            this.ClientSize = new System.Drawing.Size(547, 252);
            this.Controls.Add(this.clb_Step);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.button_Ok);
            this.Controls.Add(this.button_Cancel);
            this.Controls.Add(this.textBox_AuxiliaryDir);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox_OutputDir);
            this.Controls.Add(this.button_AuxiliaryDir);
            this.Controls.Add(this.button_OutputDir);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "CAD Options";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button_OutputDir;
        private System.Windows.Forms.Button button_AuxiliaryDir;
        private System.Windows.Forms.TextBox textBox_OutputDir;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBox_AuxiliaryDir;
        private System.Windows.Forms.Button button_Cancel;
        private System.Windows.Forms.Button button_Ok;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.CheckedListBox clb_Step;
    }
}