namespace CyPhyReliabilityAnalysis
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
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnOK = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.label1 = new System.Windows.Forms.Label();
            this.cbMaterial = new System.Windows.Forms.ComboBox();
            this.cbSize = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.tbAngular = new System.Windows.Forms.TextBox();
            this.tbGusset = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.lblAngular = new System.Windows.Forms.Label();
            this.lblGusset = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // btnCancel
            // 
            this.btnCancel.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(200, 227);
            this.btnCancel.MaximumSize = new System.Drawing.Size(75, 23);
            this.btnCancel.MinimumSize = new System.Drawing.Size(75, 23);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 9;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnOK
            // 
            this.btnOK.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btnOK.Location = new System.Drawing.Point(119, 227);
            this.btnOK.MaximumSize = new System.Drawing.Size(75, 23);
            this.btnOK.MinimumSize = new System.Drawing.Size(75, 23);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(75, 23);
            this.btnOK.TabIndex = 8;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::CyPhyReliabilityAnalysis.Properties.Resources.BracketConfiguration;
            this.pictureBox1.InitialImage = global::CyPhyReliabilityAnalysis.Properties.Resources.BracketConfiguration;
            this.pictureBox1.Location = new System.Drawing.Point(293, 12);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(307, 238);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 10;
            this.pictureBox1.TabStop = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(50, 13);
            this.label1.TabIndex = 11;
            this.label1.Text = "Material :";
            // 
            // cbMaterial
            // 
            this.cbMaterial.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbMaterial.FormattingEnabled = true;
            this.cbMaterial.Items.AddRange(new object[] {
            "Steel",
            "Aluminum"});
            this.cbMaterial.Location = new System.Drawing.Point(69, 17);
            this.cbMaterial.Name = "cbMaterial";
            this.cbMaterial.Size = new System.Drawing.Size(206, 21);
            this.cbMaterial.TabIndex = 12;
            this.cbMaterial.SelectedIndexChanged += new System.EventHandler(this.cbMaterial_SelectedIndexChanged);
            // 
            // cbSize
            // 
            this.cbSize.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbSize.FormattingEnabled = true;
            this.cbSize.Items.AddRange(new object[] {
            "(1) 76.2mm x 76.2mm x 76.2mm",
            "(2) 101.6mm x 101.6mm x 101.6mm",
            "(3) 127mm x 127mm x 127mm"});
            this.cbSize.Location = new System.Drawing.Point(69, 58);
            this.cbSize.Name = "cbSize";
            this.cbSize.Size = new System.Drawing.Size(206, 21);
            this.cbSize.TabIndex = 14;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 61);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(33, 13);
            this.label2.TabIndex = 13;
            this.label2.Text = "Size :";
            // 
            // tbAngular
            // 
            this.tbAngular.Location = new System.Drawing.Point(16, 119);
            this.tbAngular.Name = "tbAngular";
            this.tbAngular.Size = new System.Drawing.Size(114, 20);
            this.tbAngular.TabIndex = 15;
            this.tbAngular.TextChanged += new System.EventHandler(this.tbAngular_TextChanged);
            // 
            // tbGusset
            // 
            this.tbGusset.Location = new System.Drawing.Point(16, 179);
            this.tbGusset.Name = "tbGusset";
            this.tbGusset.Size = new System.Drawing.Size(114, 20);
            this.tbGusset.TabIndex = 16;
            this.tbGusset.TextChanged += new System.EventHandler(this.tbGusset_TextChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(13, 100);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(120, 13);
            this.label5.TabIndex = 19;
            this.label5.Text = "Angular Thickness [mm]";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 161);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(117, 13);
            this.label3.TabIndex = 20;
            this.label3.Text = "Gusset Thickness [mm]";
            // 
            // lblAngular
            // 
            this.lblAngular.AutoSize = true;
            this.lblAngular.Location = new System.Drawing.Point(136, 122);
            this.lblAngular.Name = "lblAngular";
            this.lblAngular.Size = new System.Drawing.Size(114, 13);
            this.lblAngular.TabIndex = 21;
            this.lblAngular.Text = "min=10.16, max=20.32";
            // 
            // lblGusset
            // 
            this.lblGusset.AutoSize = true;
            this.lblGusset.Location = new System.Drawing.Point(136, 182);
            this.lblGusset.Name = "lblGusset";
            this.lblGusset.Size = new System.Drawing.Size(102, 13);
            this.lblGusset.TabIndex = 22;
            this.lblGusset.Text = "min=3.81, max=8.89";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(614, 259);
            this.Controls.Add(this.lblGusset);
            this.Controls.Add(this.lblAngular);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.tbGusset);
            this.Controls.Add(this.tbAngular);
            this.Controls.Add(this.cbSize);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cbMaterial);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximumSize = new System.Drawing.Size(630, 297);
            this.MinimumSize = new System.Drawing.Size(630, 297);
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Bracket Configuration";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cbMaterial;
        private System.Windows.Forms.ComboBox cbSize;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tbAngular;
        private System.Windows.Forms.TextBox tbGusset;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label lblAngular;
        private System.Windows.Forms.Label lblGusset;
    }
}