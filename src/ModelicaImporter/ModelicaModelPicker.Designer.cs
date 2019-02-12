namespace ModelicaImporter
{
    partial class ModelicaModelPicker
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ModelicaModelPicker));
            this.tbSearch = new System.Windows.Forms.TextBox();
            this.lblSearch = new System.Windows.Forms.Label();
            this.btnLoadLibraries = new System.Windows.Forms.Button();
            this.pictureBoxLoader = new System.Windows.Forms.PictureBox();
            this.btnViewWebDoc = new System.Windows.Forms.Button();
            this.dgvModelPicker = new System.Windows.Forms.DataGridView();
            this.btnImportSelected = new System.Windows.Forms.Button();
            this.btnAddCustomLibrary = new System.Windows.Forms.Button();
            this.lbExternalLibList = new System.Windows.Forms.ListBox();
            this.lblExternalLibraries = new System.Windows.Forms.Label();
            this.gbComponentType = new System.Windows.Forms.GroupBox();
            this.rbTestComponents = new System.Windows.Forms.RadioButton();
            this.rbComponents = new System.Windows.Forms.RadioButton();
            this.cbIncludeMSL = new System.Windows.Forms.CheckBox();
            this.componentNameDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.descriptionDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.componentDetailBindingSource = new System.Windows.Forms.BindingSource(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLoader)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgvModelPicker)).BeginInit();
            this.gbComponentType.SuspendLayout();
            this.SuspendLayout();
            // 
            // tbSearch
            // 
            this.tbSearch.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tbSearch.Location = new System.Drawing.Point(13, 21);
            this.tbSearch.Name = "tbSearch";
            this.tbSearch.Size = new System.Drawing.Size(818, 20);
            this.tbSearch.TabIndex = 1;
            this.tbSearch.TextChanged += new System.EventHandler(this.tbSearch_TextChanged);
            // 
            // lblSearch
            // 
            this.lblSearch.AutoSize = true;
            this.lblSearch.Location = new System.Drawing.Point(11, 4);
            this.lblSearch.Name = "lblSearch";
            this.lblSearch.Size = new System.Drawing.Size(99, 13);
            this.lblSearch.TabIndex = 10;
            this.lblSearch.Text = "Search for a model:";
            // 
            // btnLoadLibraries
            // 
            this.btnLoadLibraries.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnLoadLibraries.Location = new System.Drawing.Point(839, 151);
            this.btnLoadLibraries.Name = "btnLoadLibraries";
            this.btnLoadLibraries.Size = new System.Drawing.Size(129, 40);
            this.btnLoadLibraries.TabIndex = 2;
            this.btnLoadLibraries.Text = "Load Modelica Libraries";
            this.btnLoadLibraries.UseVisualStyleBackColor = true;
            this.btnLoadLibraries.Click += new System.EventHandler(this.btnLoadLibraries_Click);
            // 
            // pictureBoxLoader
            // 
            this.pictureBoxLoader.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBoxLoader.Image = global::ModelicaImporter.Properties.Resources.ajax_loader;
            this.pictureBoxLoader.Location = new System.Drawing.Point(856, 21);
            this.pictureBoxLoader.Name = "pictureBoxLoader";
            this.pictureBoxLoader.Size = new System.Drawing.Size(26, 19);
            this.pictureBoxLoader.TabIndex = 4;
            this.pictureBoxLoader.TabStop = false;
            // 
            // btnViewWebDoc
            // 
            this.btnViewWebDoc.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnViewWebDoc.Location = new System.Drawing.Point(839, 243);
            this.btnViewWebDoc.Name = "btnViewWebDoc";
            this.btnViewWebDoc.Size = new System.Drawing.Size(129, 40);
            this.btnViewWebDoc.TabIndex = 4;
            this.btnViewWebDoc.Text = "View Web Documentation";
            this.btnViewWebDoc.UseVisualStyleBackColor = true;
            this.btnViewWebDoc.Click += new System.EventHandler(this.btnViewWebDoc_Click);
            this.btnViewWebDoc.MouseHover += new System.EventHandler(this.btnViewWebDoc_MouseHover);
            // 
            // dgvModelPicker
            // 
            this.dgvModelPicker.AllowUserToResizeColumns = false;
            this.dgvModelPicker.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.dgvModelPicker.AutoGenerateColumns = false;
            this.dgvModelPicker.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvModelPicker.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.componentNameDataGridViewTextBoxColumn,
            this.descriptionDataGridViewTextBoxColumn});
            this.dgvModelPicker.DataSource = this.componentDetailBindingSource;
            this.dgvModelPicker.Location = new System.Drawing.Point(12, 47);
            this.dgvModelPicker.MultiSelect = false;
            this.dgvModelPicker.Name = "dgvModelPicker";
            this.dgvModelPicker.ReadOnly = true;
            this.dgvModelPicker.RowHeadersWidth = 30;
            this.dgvModelPicker.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dgvModelPicker.Size = new System.Drawing.Size(821, 503);
            this.dgvModelPicker.TabIndex = 6;
            // 
            // btnImportSelected
            // 
            this.btnImportSelected.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnImportSelected.Location = new System.Drawing.Point(839, 289);
            this.btnImportSelected.Name = "btnImportSelected";
            this.btnImportSelected.Size = new System.Drawing.Size(129, 40);
            this.btnImportSelected.TabIndex = 5;
            this.btnImportSelected.Text = "Import Selected ModelicaModel(s)";
            this.btnImportSelected.UseVisualStyleBackColor = true;
            this.btnImportSelected.Click += new System.EventHandler(this.btnImportSelected_Click);
            // 
            // btnAddCustomLibrary
            // 
            this.btnAddCustomLibrary.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnAddCustomLibrary.Location = new System.Drawing.Point(839, 197);
            this.btnAddCustomLibrary.Name = "btnAddCustomLibrary";
            this.btnAddCustomLibrary.Size = new System.Drawing.Size(129, 40);
            this.btnAddCustomLibrary.TabIndex = 3;
            this.btnAddCustomLibrary.Text = "Add External Modelica Library";
            this.btnAddCustomLibrary.UseVisualStyleBackColor = true;
            this.btnAddCustomLibrary.Click += new System.EventHandler(this.btnAddCustomLibrary_Click);
            // 
            // lbExternalLibList
            // 
            this.lbExternalLibList.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lbExternalLibList.FormattingEnabled = true;
            this.lbExternalLibList.HorizontalScrollbar = true;
            this.lbExternalLibList.Location = new System.Drawing.Point(839, 66);
            this.lbExternalLibList.Name = "lbExternalLibList";
            this.lbExternalLibList.Size = new System.Drawing.Size(129, 56);
            this.lbExternalLibList.TabIndex = 9;
            // 
            // lblExternalLibraries
            // 
            this.lblExternalLibraries.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lblExternalLibraries.AutoSize = true;
            this.lblExternalLibraries.Location = new System.Drawing.Point(839, 48);
            this.lblExternalLibraries.Name = "lblExternalLibraries";
            this.lblExternalLibraries.Size = new System.Drawing.Size(95, 13);
            this.lblExternalLibraries.TabIndex = 10;
            this.lblExternalLibraries.Text = "Modelica Libraries:";
            // 
            // gbComponentType
            // 
            this.gbComponentType.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.gbComponentType.Controls.Add(this.rbTestComponents);
            this.gbComponentType.Controls.Add(this.rbComponents);
            this.gbComponentType.Location = new System.Drawing.Point(839, 335);
            this.gbComponentType.Name = "gbComponentType";
            this.gbComponentType.Size = new System.Drawing.Size(129, 69);
            this.gbComponentType.TabIndex = 6;
            this.gbComponentType.TabStop = false;
            this.gbComponentType.Text = "Import As:";
            this.gbComponentType.Visible = false;
            // 
            // rbTestComponents
            // 
            this.rbTestComponents.AutoSize = true;
            this.rbTestComponents.Location = new System.Drawing.Point(7, 43);
            this.rbTestComponents.Name = "rbTestComponents";
            this.rbTestComponents.Size = new System.Drawing.Size(105, 17);
            this.rbTestComponents.TabIndex = 7;
            this.rbTestComponents.Text = "TestComponents";
            this.rbTestComponents.UseVisualStyleBackColor = true;
            // 
            // rbComponents
            // 
            this.rbComponents.AutoSize = true;
            this.rbComponents.Checked = true;
            this.rbComponents.Location = new System.Drawing.Point(7, 20);
            this.rbComponents.Name = "rbComponents";
            this.rbComponents.Size = new System.Drawing.Size(84, 17);
            this.rbComponents.TabIndex = 6;
            this.rbComponents.TabStop = true;
            this.rbComponents.Text = "Components";
            this.rbComponents.UseVisualStyleBackColor = true;
            // 
            // cbIncludeMSL
            // 
            this.cbIncludeMSL.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cbIncludeMSL.AutoSize = true;
            this.cbIncludeMSL.Checked = true;
            this.cbIncludeMSL.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbIncludeMSL.Location = new System.Drawing.Point(842, 128);
            this.cbIncludeMSL.Name = "cbIncludeMSL";
            this.cbIncludeMSL.Size = new System.Drawing.Size(86, 17);
            this.cbIncludeMSL.TabIndex = 11;
            this.cbIncludeMSL.Text = "Include MSL";
            this.cbIncludeMSL.UseVisualStyleBackColor = true;
            this.cbIncludeMSL.CheckedChanged += new System.EventHandler(this.cbIncludeMSL_CheckedChanged);
            // 
            // componentNameDataGridViewTextBoxColumn
            // 
            this.componentNameDataGridViewTextBoxColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.componentNameDataGridViewTextBoxColumn.DataPropertyName = "ComponentName";
            this.componentNameDataGridViewTextBoxColumn.HeaderText = "ComponentName";
            this.componentNameDataGridViewTextBoxColumn.MinimumWidth = 400;
            this.componentNameDataGridViewTextBoxColumn.Name = "componentNameDataGridViewTextBoxColumn";
            this.componentNameDataGridViewTextBoxColumn.ReadOnly = true;
            this.componentNameDataGridViewTextBoxColumn.Width = 400;
            // 
            // descriptionDataGridViewTextBoxColumn
            // 
            this.descriptionDataGridViewTextBoxColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.descriptionDataGridViewTextBoxColumn.DataPropertyName = "Description";
            this.descriptionDataGridViewTextBoxColumn.HeaderText = "Description";
            this.descriptionDataGridViewTextBoxColumn.MinimumWidth = 431;
            this.descriptionDataGridViewTextBoxColumn.Name = "descriptionDataGridViewTextBoxColumn";
            this.descriptionDataGridViewTextBoxColumn.ReadOnly = true;
            // 
            // componentDetailBindingSource
            // 
            this.componentDetailBindingSource.DataSource = typeof(ModelicaImporter.ComponentDetail);
            // 
            // ModelicaModelPicker
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.ClientSize = new System.Drawing.Size(971, 562);
            this.Controls.Add(this.cbIncludeMSL);
            this.Controls.Add(this.gbComponentType);
            this.Controls.Add(this.lblExternalLibraries);
            this.Controls.Add(this.lbExternalLibList);
            this.Controls.Add(this.btnAddCustomLibrary);
            this.Controls.Add(this.btnImportSelected);
            this.Controls.Add(this.dgvModelPicker);
            this.Controls.Add(this.btnViewWebDoc);
            this.Controls.Add(this.pictureBoxLoader);
            this.Controls.Add(this.btnLoadLibraries);
            this.Controls.Add(this.lblSearch);
            this.Controls.Add(this.tbSearch);
            this.MinimumSize = new System.Drawing.Size(800, 600);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ModelicaModelPicker";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Modelica Model Picker";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLoader)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgvModelPicker)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.componentDetailBindingSource)).EndInit();
            this.gbComponentType.ResumeLayout(false);
            this.gbComponentType.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox tbSearch;
        private System.Windows.Forms.Label lblSearch;
        private System.Windows.Forms.Button btnLoadLibraries;
        private System.Windows.Forms.PictureBox pictureBoxLoader;
        private System.Windows.Forms.Button btnViewWebDoc;
        private System.Windows.Forms.BindingSource componentDetailBindingSource;
        private System.Windows.Forms.DataGridView dgvModelPicker;
        private System.Windows.Forms.Button btnImportSelected;
        private System.Windows.Forms.Button btnAddCustomLibrary;
        private System.Windows.Forms.ListBox lbExternalLibList;
        private System.Windows.Forms.Label lblExternalLibraries;
        private System.Windows.Forms.DataGridViewTextBoxColumn componentNameDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn descriptionDataGridViewTextBoxColumn;
        private System.Windows.Forms.GroupBox gbComponentType;
        private System.Windows.Forms.RadioButton rbTestComponents;
        private System.Windows.Forms.RadioButton rbComponents;
        private System.Windows.Forms.CheckBox cbIncludeMSL;
    }
}