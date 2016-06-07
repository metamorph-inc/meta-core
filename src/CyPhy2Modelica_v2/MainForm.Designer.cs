namespace CyPhy2Modelica_v2
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.btnOK = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnBrowseIncludeDir = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.btnRemoveLib = new System.Windows.Forms.Button();
            this.chbCheckWithDymola = new System.Windows.Forms.CheckBox();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.Version = new System.Windows.Forms.DataGridViewButtonColumn();
            this.chbCheckWithOM = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.chbConstrained = new System.Windows.Forms.CheckBox();
            this.isSelectedDataGridViewCheckBoxColumn = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.nameDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.modelicaLibraryBindingSource = new System.Windows.Forms.BindingSource(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.modelicaLibraryBindingSource)).BeginInit();
            this.SuspendLayout();
            // 
            // btnOK
            // 
            this.btnOK.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btnOK.Location = new System.Drawing.Point(189, 192);
            this.btnOK.MaximumSize = new System.Drawing.Size(75, 23);
            this.btnOK.MinimumSize = new System.Drawing.Size(75, 23);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(75, 23);
            this.btnOK.TabIndex = 1;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(270, 192);
            this.btnCancel.MaximumSize = new System.Drawing.Size(75, 23);
            this.btnCancel.MinimumSize = new System.Drawing.Size(75, 23);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 7;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnBrowseIncludeDir
            // 
            this.btnBrowseIncludeDir.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnBrowseIncludeDir.Location = new System.Drawing.Point(408, 27);
            this.btnBrowseIncludeDir.Name = "btnBrowseIncludeDir";
            this.btnBrowseIncludeDir.Size = new System.Drawing.Size(123, 38);
            this.btnBrowseIncludeDir.TabIndex = 2;
            this.btnBrowseIncludeDir.Text = "Browse for Library Directory";
            this.btnBrowseIncludeDir.UseVisualStyleBackColor = true;
            this.btnBrowseIncludeDir.Click += new System.EventHandler(this.btnBrowseIncludeDir_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(8, 7);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(451, 13);
            this.label1.TabIndex = 9;
            this.label1.Text = "Checked package paths will be added to the MODELICALIBPATH (MSL 3.2 already inclu" +
    "ded)";
            // 
            // btnRemoveLib
            // 
            this.btnRemoveLib.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnRemoveLib.Location = new System.Drawing.Point(408, 69);
            this.btnRemoveLib.Name = "btnRemoveLib";
            this.btnRemoveLib.Size = new System.Drawing.Size(123, 38);
            this.btnRemoveLib.TabIndex = 3;
            this.btnRemoveLib.Text = "Remove Highlighted";
            this.btnRemoveLib.UseVisualStyleBackColor = true;
            this.btnRemoveLib.Click += new System.EventHandler(this.btnRemoveLib_Click);
            // 
            // chbCheckWithDymola
            // 
            this.chbCheckWithDymola.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.chbCheckWithDymola.AutoSize = true;
            this.chbCheckWithDymola.Location = new System.Drawing.Point(422, 137);
            this.chbCheckWithDymola.Name = "chbCheckWithDymola";
            this.chbCheckWithDymola.Size = new System.Drawing.Size(61, 17);
            this.chbCheckWithDymola.TabIndex = 16;
            this.chbCheckWithDymola.Text = "Dymola";
            this.chbCheckWithDymola.UseVisualStyleBackColor = true;
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.AllowUserToDeleteRows = false;
            this.dataGridView1.AllowUserToResizeColumns = false;
            this.dataGridView1.AllowUserToResizeRows = false;
            this.dataGridView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.dataGridView1.AutoGenerateColumns = false;
            this.dataGridView1.BackgroundColor = System.Drawing.SystemColors.ControlLightLight;
            this.dataGridView1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.isSelectedDataGridViewCheckBoxColumn,
            this.nameDataGridViewTextBoxColumn,
            this.Version});
            this.dataGridView1.DataSource = this.modelicaLibraryBindingSource;
            this.dataGridView1.Location = new System.Drawing.Point(12, 29);
            this.dataGridView1.MultiSelect = false;
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridView1.ShowEditingIcon = false;
            this.dataGridView1.Size = new System.Drawing.Size(390, 157);
            this.dataGridView1.TabIndex = 17;
            this.dataGridView1.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellContentClick);
            // 
            // Version
            // 
            this.Version.DataPropertyName = "Version";
            this.Version.HeaderText = "Version";
            this.Version.MinimumWidth = 80;
            this.Version.Name = "Version";
            this.Version.ReadOnly = true;
            this.Version.Resizable = System.Windows.Forms.DataGridViewTriState.False;
            this.Version.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
            this.Version.Text = "Version";
            this.Version.Width = 80;
            // 
            // chbCheckWithOM
            // 
            this.chbCheckWithOM.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.chbCheckWithOM.AutoSize = true;
            this.chbCheckWithOM.Location = new System.Drawing.Point(422, 160);
            this.chbCheckWithOM.Name = "chbCheckWithOM";
            this.chbCheckWithOM.Size = new System.Drawing.Size(95, 17);
            this.chbCheckWithOM.TabIndex = 18;
            this.chbCheckWithOM.Text = "OpenModelica";
            this.chbCheckWithOM.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(408, 114);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(100, 13);
            this.label2.TabIndex = 19;
            this.label2.Text = "Check model using:";
            // 
            // isSelectedDataGridViewCheckBoxColumn
            // 
            this.isSelectedDataGridViewCheckBoxColumn.DataPropertyName = "IsSelected";
            this.isSelectedDataGridViewCheckBoxColumn.HeaderText = "Use";
            this.isSelectedDataGridViewCheckBoxColumn.MinimumWidth = 35;
            this.isSelectedDataGridViewCheckBoxColumn.Name = "isSelectedDataGridViewCheckBoxColumn";
            this.isSelectedDataGridViewCheckBoxColumn.ReadOnly = true;
            this.isSelectedDataGridViewCheckBoxColumn.Width = 35;
            // chbConstrained
            // 
            this.chbConstrained.AutoSize = true;
            this.chbConstrained.Location = new System.Drawing.Point(12, 196);
            this.chbConstrained.Name = "chbConstrained";
            this.chbConstrained.Size = new System.Drawing.Size(144, 17);
            this.chbConstrained.TabIndex = 20;
            this.chbConstrained.Text = "Generate constrainedbys";
            this.chbConstrained.UseVisualStyleBackColor = true;
            // 
            // nameDataGridViewTextBoxColumn
            // 
            this.nameDataGridViewTextBoxColumn.DataPropertyName = "Name";
            this.nameDataGridViewTextBoxColumn.HeaderText = "Name";
            this.nameDataGridViewTextBoxColumn.MinimumWidth = 232;
            this.nameDataGridViewTextBoxColumn.Name = "nameDataGridViewTextBoxColumn";
            this.nameDataGridViewTextBoxColumn.ReadOnly = true;
            this.nameDataGridViewTextBoxColumn.Width = 232;
            // 
            // modelicaLibraryBindingSource
            // 
            this.modelicaLibraryBindingSource.DataSource = typeof(CyPhy2Modelica_v2.ModelicaLibrary);
            // 
            // MainForm
            // 
            this.AcceptButton = this.btnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(535, 222);
            this.Controls.Add(this.chbConstrained);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.chbCheckWithOM);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.chbCheckWithDymola);
            this.Controls.Add(this.btnRemoveLib);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnBrowseIncludeDir);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(551, 256);
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Modelica Interpreter Settings";
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.modelicaLibraryBindingSource)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnBrowseIncludeDir;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnRemoveLib;
        private System.Windows.Forms.CheckBox chbCheckWithDymola;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.BindingSource modelicaLibraryBindingSource;
        private System.Windows.Forms.DataGridViewCheckBoxColumn isSelectedDataGridViewCheckBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn nameDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewButtonColumn Version;
        private System.Windows.Forms.CheckBox chbCheckWithOM;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.CheckBox chbConstrained;
	}
}