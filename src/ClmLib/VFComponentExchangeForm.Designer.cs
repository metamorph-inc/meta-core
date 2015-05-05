namespace ClmLib
{
    partial class VFComponentExchangeForm
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
            this.dgvSelectorVFComponents = new System.Windows.Forms.DataGridView();
            this.nameDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.itarRestrictionDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.statusDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.zipURLDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.vFComponentListItemBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.lblDataGridView = new System.Windows.Forms.Label();
            this.lblCategory = new System.Windows.Forms.Label();
            this.lblComponentCategory = new System.Windows.Forms.Label();
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnImport = new System.Windows.Forms.Button();
            this.lblSearchResultsInfo = new System.Windows.Forms.Label();
            this.btnPrevious = new System.Windows.Forms.Button();
            this.btnNext = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dgvSelectorVFComponents)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.vFComponentListItemBindingSource)).BeginInit();
            this.SuspendLayout();
            // 
            // dgvSelectorVFComponents
            // 
            this.dgvSelectorVFComponents.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.dgvSelectorVFComponents.AutoGenerateColumns = false;
            this.dgvSelectorVFComponents.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvSelectorVFComponents.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.nameDataGridViewTextBoxColumn,
            this.itarRestrictionDataGridViewTextBoxColumn,
            this.statusDataGridViewTextBoxColumn,
            this.zipURLDataGridViewTextBoxColumn});
            this.dgvSelectorVFComponents.DataSource = this.vFComponentListItemBindingSource;
            this.dgvSelectorVFComponents.Location = new System.Drawing.Point(12, 64);
            this.dgvSelectorVFComponents.Name = "dgvSelectorVFComponents";
            this.dgvSelectorVFComponents.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dgvSelectorVFComponents.Size = new System.Drawing.Size(510, 310);
            this.dgvSelectorVFComponents.TabIndex = 2;
            this.dgvSelectorVFComponents.ColumnHeaderMouseClick += new System.Windows.Forms.DataGridViewCellMouseEventHandler(this.dgvSelectorVFComponents_ColumnHeaderMouseClick);
            // 
            // nameDataGridViewTextBoxColumn
            // 
            this.nameDataGridViewTextBoxColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.nameDataGridViewTextBoxColumn.DataPropertyName = "Name";
            this.nameDataGridViewTextBoxColumn.HeaderText = "Name";
            this.nameDataGridViewTextBoxColumn.Name = "nameDataGridViewTextBoxColumn";
            this.nameDataGridViewTextBoxColumn.ReadOnly = true;
            this.nameDataGridViewTextBoxColumn.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Programmatic;
            // 
            // itarRestrictionDataGridViewTextBoxColumn
            // 
            this.itarRestrictionDataGridViewTextBoxColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.itarRestrictionDataGridViewTextBoxColumn.DataPropertyName = "ItarRestriction";
            this.itarRestrictionDataGridViewTextBoxColumn.HeaderText = "ItarRestriction";
            this.itarRestrictionDataGridViewTextBoxColumn.Name = "itarRestrictionDataGridViewTextBoxColumn";
            this.itarRestrictionDataGridViewTextBoxColumn.ReadOnly = true;
            this.itarRestrictionDataGridViewTextBoxColumn.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Programmatic;
            this.itarRestrictionDataGridViewTextBoxColumn.Width = 97;
            // 
            // statusDataGridViewTextBoxColumn
            // 
            this.statusDataGridViewTextBoxColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.statusDataGridViewTextBoxColumn.DataPropertyName = "Status";
            this.statusDataGridViewTextBoxColumn.HeaderText = "Status";
            this.statusDataGridViewTextBoxColumn.Name = "statusDataGridViewTextBoxColumn";
            this.statusDataGridViewTextBoxColumn.ReadOnly = true;
            this.statusDataGridViewTextBoxColumn.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Programmatic;
            this.statusDataGridViewTextBoxColumn.Width = 62;
            // 
            // zipURLDataGridViewTextBoxColumn
            // 
            this.zipURLDataGridViewTextBoxColumn.DataPropertyName = "ZipURL";
            this.zipURLDataGridViewTextBoxColumn.HeaderText = "ZipURL";
            this.zipURLDataGridViewTextBoxColumn.Name = "zipURLDataGridViewTextBoxColumn";
            this.zipURLDataGridViewTextBoxColumn.ReadOnly = true;
            this.zipURLDataGridViewTextBoxColumn.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Programmatic;
            this.zipURLDataGridViewTextBoxColumn.Visible = false;
            // 
            // vFComponentListItemBindingSource
            // 
            this.vFComponentListItemBindingSource.DataSource = typeof(ClmLib.VFComponentListItem);
            // 
            // lblDataGridView
            // 
            this.lblDataGridView.AutoSize = true;
            this.lblDataGridView.Location = new System.Drawing.Point(13, 14);
            this.lblDataGridView.Name = "lblDataGridView";
            this.lblDataGridView.Size = new System.Drawing.Size(369, 13);
            this.lblDataGridView.TabIndex = 1;
            this.lblDataGridView.Text = "Select Components available on VehicleForge Exchange, then click \"Import\"";
            // 
            // lblCategory
            // 
            this.lblCategory.AutoSize = true;
            this.lblCategory.Location = new System.Drawing.Point(13, 44);
            this.lblCategory.Name = "lblCategory";
            this.lblCategory.Size = new System.Drawing.Size(52, 13);
            this.lblCategory.TabIndex = 2;
            this.lblCategory.Text = "Category:";
            // 
            // lblComponentCategory
            // 
            this.lblComponentCategory.AutoSize = true;
            this.lblComponentCategory.Location = new System.Drawing.Point(71, 44);
            this.lblComponentCategory.Name = "lblComponentCategory";
            this.lblComponentCategory.Size = new System.Drawing.Size(85, 13);
            this.lblComponentCategory.TabIndex = 3;
            this.lblComponentCategory.Text = "_____________";
            // 
            // btnCancel
            // 
            this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(446, 380);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 1;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnImport
            // 
            this.btnImport.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnImport.Location = new System.Drawing.Point(365, 380);
            this.btnImport.Name = "btnImport";
            this.btnImport.Size = new System.Drawing.Size(75, 23);
            this.btnImport.TabIndex = 0;
            this.btnImport.Text = "Import";
            this.btnImport.UseVisualStyleBackColor = true;
            this.btnImport.Click += new System.EventHandler(this.btnImport_Click);
            // 
            // lblSearchResultsInfo
            // 
            this.lblSearchResultsInfo.AutoSize = true;
            this.lblSearchResultsInfo.Location = new System.Drawing.Point(279, 44);
            this.lblSearchResultsInfo.Name = "lblSearchResultsInfo";
            this.lblSearchResultsInfo.Size = new System.Drawing.Size(144, 13);
            this.lblSearchResultsInfo.TabIndex = 8;
            this.lblSearchResultsInfo.Text = "Showing __ of __ search hits";
            // 
            // btnPrevious
            // 
            this.btnPrevious.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnPrevious.Location = new System.Drawing.Point(438, 39);
            this.btnPrevious.Name = "btnPrevious";
            this.btnPrevious.Size = new System.Drawing.Size(39, 23);
            this.btnPrevious.TabIndex = 13;
            this.btnPrevious.Text = "Prev";
            this.btnPrevious.UseVisualStyleBackColor = true;
            this.btnPrevious.Click += new System.EventHandler(this.btnPrevious_Click);
            // 
            // btnNext
            // 
            this.btnNext.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnNext.Location = new System.Drawing.Point(482, 39);
            this.btnNext.Name = "btnNext";
            this.btnNext.Size = new System.Drawing.Size(39, 23);
            this.btnNext.TabIndex = 14;
            this.btnNext.Text = "Next";
            this.btnNext.UseVisualStyleBackColor = true;
            this.btnNext.Click += new System.EventHandler(this.btnNext_Click);
            // 
            // VFComponentExchangeForm
            // 
            this.AcceptButton = this.btnImport;
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(534, 412);
            this.Controls.Add(this.btnNext);
            this.Controls.Add(this.btnPrevious);
            this.Controls.Add(this.lblSearchResultsInfo);
            this.Controls.Add(this.btnImport);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.lblComponentCategory);
            this.Controls.Add(this.lblCategory);
            this.Controls.Add(this.lblDataGridView);
            this.Controls.Add(this.dgvSelectorVFComponents);
            this.MinimumSize = new System.Drawing.Size(550, 450);
            this.Name = "VFComponentExchangeForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "VehicleForge Component Import";
            ((System.ComponentModel.ISupportInitialize)(this.dgvSelectorVFComponents)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.vFComponentListItemBindingSource)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        public System.Windows.Forms.DataGridView dgvSelectorVFComponents;
        private System.Windows.Forms.Label lblDataGridView;
        private System.Windows.Forms.Label lblCategory;
        public System.Windows.Forms.Label lblComponentCategory;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnImport;
        private System.Windows.Forms.BindingSource vFComponentListItemBindingSource;
        private System.Windows.Forms.DataGridViewTextBoxColumn nameDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn itarRestrictionDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn statusDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn zipURLDataGridViewTextBoxColumn;
        private System.Windows.Forms.Label lblSearchResultsInfo;
        private System.Windows.Forms.Button btnPrevious;
        private System.Windows.Forms.Button btnNext;
    }
}