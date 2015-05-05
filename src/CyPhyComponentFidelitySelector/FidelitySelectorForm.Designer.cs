namespace CyPhyComponentFidelitySelector
{
    partial class FidelitySelectorForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FidelitySelectorForm));
            this.dgvData = new System.Windows.Forms.DataGridView();
            this.btnSaveAndClose = new System.Windows.Forms.Button();
            this.gbApplyToScope = new System.Windows.Forms.GroupBox();
            this.rbThisProject = new System.Windows.Forms.RadioButton();
            this.rbThisFolder = new System.Windows.Forms.RadioButton();
            this.rbThisTestbench = new System.Windows.Forms.RadioButton();
            ((System.ComponentModel.ISupportInitialize)(this.dgvData)).BeginInit();
            this.gbApplyToScope.SuspendLayout();
            this.SuspendLayout();
            // 
            // dgvData
            // 
            this.dgvData.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.dgvData.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvData.Location = new System.Drawing.Point(12, 12);
            this.dgvData.Name = "dgvData";
            this.dgvData.RowHeadersVisible = false;
            this.dgvData.Size = new System.Drawing.Size(460, 249);
            this.dgvData.TabIndex = 0;
            // 
            // btnSaveAndClose
            // 
            this.btnSaveAndClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnSaveAndClose.Location = new System.Drawing.Point(327, 300);
            this.btnSaveAndClose.Name = "btnSaveAndClose";
            this.btnSaveAndClose.Size = new System.Drawing.Size(145, 50);
            this.btnSaveAndClose.TabIndex = 1;
            this.btnSaveAndClose.Text = "Save and Close";
            this.btnSaveAndClose.UseVisualStyleBackColor = true;
            this.btnSaveAndClose.Click += new System.EventHandler(this.btnSaveAndClose_Click);
            // 
            // gbApplyToScope
            // 
            this.gbApplyToScope.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.gbApplyToScope.Controls.Add(this.rbThisProject);
            this.gbApplyToScope.Controls.Add(this.rbThisFolder);
            this.gbApplyToScope.Controls.Add(this.rbThisTestbench);
            this.gbApplyToScope.Location = new System.Drawing.Point(12, 267);
            this.gbApplyToScope.Name = "gbApplyToScope";
            this.gbApplyToScope.Size = new System.Drawing.Size(244, 83);
            this.gbApplyToScope.TabIndex = 2;
            this.gbApplyToScope.TabStop = false;
            this.gbApplyToScope.Text = "Apply Fidelity Settings to:";
            // 
            // rbThisProject
            // 
            this.rbThisProject.AutoSize = true;
            this.rbThisProject.Location = new System.Drawing.Point(6, 57);
            this.rbThisProject.Name = "rbThisProject";
            this.rbThisProject.Size = new System.Drawing.Size(167, 17);
            this.rbThisProject.TabIndex = 2;
            this.rbThisProject.Text = "All Testbenches in this Project";
            this.rbThisProject.UseVisualStyleBackColor = true;
            // 
            // rbThisFolder
            // 
            this.rbThisFolder.AutoSize = true;
            this.rbThisFolder.Location = new System.Drawing.Point(6, 38);
            this.rbThisFolder.Name = "rbThisFolder";
            this.rbThisFolder.Size = new System.Drawing.Size(149, 17);
            this.rbThisFolder.TabIndex = 1;
            this.rbThisFolder.Text = "Testbenches in this Folder";
            this.rbThisFolder.UseVisualStyleBackColor = true;
            // 
            // rbThisTestbench
            // 
            this.rbThisTestbench.AutoSize = true;
            this.rbThisTestbench.Checked = true;
            this.rbThisTestbench.Location = new System.Drawing.Point(6, 19);
            this.rbThisTestbench.Name = "rbThisTestbench";
            this.rbThisTestbench.Size = new System.Drawing.Size(123, 17);
            this.rbThisTestbench.TabIndex = 0;
            this.rbThisTestbench.TabStop = true;
            this.rbThisTestbench.Text = "This Testbench Only";
            this.rbThisTestbench.UseVisualStyleBackColor = true;
            // 
            // FidelitySelectorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.ClientSize = new System.Drawing.Size(484, 362);
            this.Controls.Add(this.gbApplyToScope);
            this.Controls.Add(this.btnSaveAndClose);
            this.Controls.Add(this.dgvData);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(450, 375);
            this.Name = "FidelitySelectorForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "FidelitySelectorForm";
            ((System.ComponentModel.ISupportInitialize)(this.dgvData)).EndInit();
            this.gbApplyToScope.ResumeLayout(false);
            this.gbApplyToScope.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataGridViewTextBoxColumn selectedItemDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridView dgvData;
        private System.Windows.Forms.Button btnSaveAndClose;
        private System.Windows.Forms.GroupBox gbApplyToScope;
        public System.Windows.Forms.RadioButton rbThisProject;
        public System.Windows.Forms.RadioButton rbThisFolder;
        public System.Windows.Forms.RadioButton rbThisTestbench;

    }
}