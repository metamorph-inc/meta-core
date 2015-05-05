namespace CyPhyPET
{
    partial class PCCPropertyInputDistributionEditor
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PCCPropertyInputDistributionEditor));
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.panel1 = new System.Windows.Forms.Panel();
            this.lblParam4 = new System.Windows.Forms.Label();
            this.lblParam3 = new System.Windows.Forms.Label();
            this.lblParam2 = new System.Windows.Forms.Label();
            this.lblParam1 = new System.Windows.Forms.Label();
            this.lblDist = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.tbParam4 = new System.Windows.Forms.TextBox();
            this.tbParam3 = new System.Windows.Forms.TextBox();
            this.tbParam2 = new System.Windows.Forms.TextBox();
            this.tbParam1 = new System.Windows.Forms.TextBox();
            this.cbDist = new System.Windows.Forms.ComboBox();
            this.lblPropertyName = new System.Windows.Forms.Label();
            this.panel3 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.panel4 = new System.Windows.Forms.Panel();
            this.label2 = new System.Windows.Forms.Label();
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnOK = new System.Windows.Forms.Button();
            this.btnUpdate = new System.Windows.Forms.Button();
            this.lblName = new System.Windows.Forms.Label();
            this.nameDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.pCCPropertyBindingSource = new System.Windows.Forms.BindingSource(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            this.panel4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pCCPropertyBindingSource)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::CyPhyPET.Properties.Resources.beta;
            this.pictureBox1.InitialImage = null;
            this.pictureBox1.Location = new System.Drawing.Point(376, 237);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(280, 156);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.pictureBox1.TabIndex = 1;
            this.pictureBox1.TabStop = false;
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.AllowUserToDeleteRows = false;
            this.dataGridView1.AllowUserToOrderColumns = true;
            this.dataGridView1.AllowUserToResizeColumns = false;
            this.dataGridView1.AllowUserToResizeRows = false;
            this.dataGridView1.AutoGenerateColumns = false;
            this.dataGridView1.BackgroundColor = System.Drawing.SystemColors.ControlLightLight;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.ColumnHeadersVisible = false;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.nameDataGridViewTextBoxColumn});
            this.dataGridView1.DataSource = this.pCCPropertyBindingSource;
            this.dataGridView1.Location = new System.Drawing.Point(13, 43);
            this.dataGridView1.MultiSelect = false;
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.RowHeadersVisible = false;
            this.dataGridView1.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.dataGridView1.Size = new System.Drawing.Size(329, 393);
            this.dataGridView1.TabIndex = 9;
            this.dataGridView1.SelectionChanged += new System.EventHandler(this.dataGridView1_SelectionChanged);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.lblParam4);
            this.panel1.Controls.Add(this.lblParam3);
            this.panel1.Controls.Add(this.lblParam2);
            this.panel1.Controls.Add(this.lblParam1);
            this.panel1.Controls.Add(this.lblDist);
            this.panel1.Location = new System.Drawing.Point(376, 59);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(116, 143);
            this.panel1.TabIndex = 3;
            // 
            // lblParam4
            // 
            this.lblParam4.AutoSize = true;
            this.lblParam4.Location = new System.Drawing.Point(4, 123);
            this.lblParam4.Name = "lblParam4";
            this.lblParam4.Size = new System.Drawing.Size(49, 13);
            this.lblParam4.TabIndex = 5;
            this.lblParam4.Text = "Param4 :";
            // 
            // lblParam3
            // 
            this.lblParam3.AutoSize = true;
            this.lblParam3.Location = new System.Drawing.Point(4, 97);
            this.lblParam3.Name = "lblParam3";
            this.lblParam3.Size = new System.Drawing.Size(49, 13);
            this.lblParam3.TabIndex = 4;
            this.lblParam3.Text = "Param3 :";
            // 
            // lblParam2
            // 
            this.lblParam2.AutoSize = true;
            this.lblParam2.Location = new System.Drawing.Point(4, 71);
            this.lblParam2.Name = "lblParam2";
            this.lblParam2.Size = new System.Drawing.Size(49, 13);
            this.lblParam2.TabIndex = 3;
            this.lblParam2.Text = "Param2 :";
            // 
            // lblParam1
            // 
            this.lblParam1.AutoSize = true;
            this.lblParam1.Location = new System.Drawing.Point(4, 45);
            this.lblParam1.Name = "lblParam1";
            this.lblParam1.Size = new System.Drawing.Size(49, 13);
            this.lblParam1.TabIndex = 2;
            this.lblParam1.Text = "Param1 :";
            // 
            // lblDist
            // 
            this.lblDist.AutoSize = true;
            this.lblDist.Location = new System.Drawing.Point(4, 18);
            this.lblDist.Name = "lblDist";
            this.lblDist.Size = new System.Drawing.Size(65, 13);
            this.lblDist.TabIndex = 1;
            this.lblDist.Text = "Distribution :";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.tbParam4);
            this.panel2.Controls.Add(this.tbParam3);
            this.panel2.Controls.Add(this.tbParam2);
            this.panel2.Controls.Add(this.tbParam1);
            this.panel2.Controls.Add(this.cbDist);
            this.panel2.Controls.Add(this.lblPropertyName);
            this.panel2.Location = new System.Drawing.Point(495, 59);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(161, 143);
            this.panel2.TabIndex = 4;
            // 
            // tbParam4
            // 
            this.tbParam4.Location = new System.Drawing.Point(3, 120);
            this.tbParam4.Name = "tbParam4";
            this.tbParam4.Size = new System.Drawing.Size(152, 20);
            this.tbParam4.TabIndex = 5;
            this.tbParam4.Text = "0.0";
            this.tbParam4.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbParam3
            // 
            this.tbParam3.Location = new System.Drawing.Point(3, 94);
            this.tbParam3.Name = "tbParam3";
            this.tbParam3.Size = new System.Drawing.Size(152, 20);
            this.tbParam3.TabIndex = 4;
            this.tbParam3.Text = "0.0";
            this.tbParam3.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbParam2
            // 
            this.tbParam2.Location = new System.Drawing.Point(3, 68);
            this.tbParam2.Name = "tbParam2";
            this.tbParam2.Size = new System.Drawing.Size(152, 20);
            this.tbParam2.TabIndex = 3;
            this.tbParam2.Text = "0.0";
            this.tbParam2.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // tbParam1
            // 
            this.tbParam1.Location = new System.Drawing.Point(3, 42);
            this.tbParam1.Name = "tbParam1";
            this.tbParam1.Size = new System.Drawing.Size(152, 20);
            this.tbParam1.TabIndex = 2;
            this.tbParam1.Text = "0.0";
            this.tbParam1.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // cbDist
            // 
            this.cbDist.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbDist.FormattingEnabled = true;
            this.cbDist.Items.AddRange(new object[] {
            "None",
            "Beta",
            "Log Normal",
            "Normal",
            "Uniform"});
            this.cbDist.Location = new System.Drawing.Point(3, 15);
            this.cbDist.Name = "cbDist";
            this.cbDist.Size = new System.Drawing.Size(152, 21);
            this.cbDist.TabIndex = 1;
            this.cbDist.SelectedIndexChanged += new System.EventHandler(this.cbDist_SelectedIndexChanged);
            // 
            // lblPropertyName
            // 
            this.lblPropertyName.AutoSize = true;
            this.lblPropertyName.Location = new System.Drawing.Point(3, 4);
            this.lblPropertyName.Name = "lblPropertyName";
            this.lblPropertyName.Size = new System.Drawing.Size(0, 13);
            this.lblPropertyName.TabIndex = 1;
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.label1);
            this.panel3.Location = new System.Drawing.Point(363, 12);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(280, 25);
            this.panel3.TabIndex = 5;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(7, 4);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(137, 15);
            this.label1.TabIndex = 0;
            this.label1.Text = "Distribution Settings";
            // 
            // panel4
            // 
            this.panel4.Controls.Add(this.label2);
            this.panel4.Location = new System.Drawing.Point(2, 12);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(280, 25);
            this.panel4.TabIndex = 6;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(7, 4);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(135, 15);
            this.label2.TabIndex = 0;
            this.label2.Text = "Avaliable Properties";
            // 
            // btnCancel
            // 
            this.btnCancel.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(581, 420);
            this.btnCancel.MaximumSize = new System.Drawing.Size(75, 23);
            this.btnCancel.MinimumSize = new System.Drawing.Size(75, 23);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 8;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnOK
            // 
            this.btnOK.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btnOK.Location = new System.Drawing.Point(501, 420);
            this.btnOK.MaximumSize = new System.Drawing.Size(75, 23);
            this.btnOK.MinimumSize = new System.Drawing.Size(75, 23);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(75, 23);
            this.btnOK.TabIndex = 7;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // btnUpdate
            // 
            this.btnUpdate.Location = new System.Drawing.Point(581, 208);
            this.btnUpdate.Name = "btnUpdate";
            this.btnUpdate.Size = new System.Drawing.Size(75, 23);
            this.btnUpdate.TabIndex = 6;
            this.btnUpdate.Text = "Update";
            this.btnUpdate.UseVisualStyleBackColor = true;
            this.btnUpdate.Click += new System.EventHandler(this.btnUpdate_Click);
            // 
            // lblName
            // 
            this.lblName.AutoSize = true;
            this.lblName.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblName.Location = new System.Drawing.Point(380, 44);
            this.lblName.Name = "lblName";
            this.lblName.Size = new System.Drawing.Size(236, 13);
            this.lblName.TabIndex = 6;
            this.lblName.Text = "This will be the name of a PCC-Property.";
            // 
            // nameDataGridViewTextBoxColumn
            // 
            this.nameDataGridViewTextBoxColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.DisplayedCells;
            this.nameDataGridViewTextBoxColumn.DataPropertyName = "Name";
            this.nameDataGridViewTextBoxColumn.HeaderText = "Name";
            this.nameDataGridViewTextBoxColumn.MinimumWidth = 326;
            this.nameDataGridViewTextBoxColumn.Name = "nameDataGridViewTextBoxColumn";
            this.nameDataGridViewTextBoxColumn.ReadOnly = true;
            this.nameDataGridViewTextBoxColumn.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this.nameDataGridViewTextBoxColumn.Width = 326;
            // 
            // pCCPropertyBindingSource
            // 
            this.pCCPropertyBindingSource.DataSource = typeof(CyPhyPET.PCC.PCCProperty);
            // 
            // PCCPropertyInputDistributionEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(667, 455);
            this.Controls.Add(this.lblName);
            this.Controls.Add(this.btnUpdate);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.panel4);
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.panel2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximumSize = new System.Drawing.Size(673, 483);
            this.MinimumSize = new System.Drawing.Size(673, 483);
            this.Name = "PCCPropertyInputDistributionEditor";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "PCC Property Input Distribution Editor";
            this.TopMost = true;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.PCCPropertyInputDistributionEditor_FormClosing);
            this.Load += new System.EventHandler(this.PCCPropertyInputDistributionEditor_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.panel4.ResumeLayout(false);
            this.panel4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pCCPropertyBindingSource)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label lblParam4;
        private System.Windows.Forms.Label lblParam3;
        private System.Windows.Forms.Label lblParam2;
        private System.Windows.Forms.Label lblParam1;
        private System.Windows.Forms.Label lblDist;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.TextBox tbParam4;
        private System.Windows.Forms.TextBox tbParam3;
        private System.Windows.Forms.TextBox tbParam2;
        private System.Windows.Forms.TextBox tbParam1;
        private System.Windows.Forms.ComboBox cbDist;
        private System.Windows.Forms.Label lblPropertyName;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.BindingSource pCCPropertyBindingSource;
        private System.Windows.Forms.DataGridViewTextBoxColumn nameDataGridViewTextBoxColumn;
        private System.Windows.Forms.Button btnUpdate;
        private System.Windows.Forms.Label lblName;
    }
}