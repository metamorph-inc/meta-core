namespace CLM_light
{
    partial class ComponentSelectionForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ComponentSelectionForm));
            this.dgvSelector = new System.Windows.Forms.DataGridView();
            this.btnOK = new System.Windows.Forms.Button();
            this.lblHint = new System.Windows.Forms.Label();
            this.btnCheckVF = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.componentListItemBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.nameDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.objectTypeDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.pathWithinProjectDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.dgvSelector)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.componentListItemBindingSource)).BeginInit();
            this.SuspendLayout();
            // 
            // dgvSelector
            // 
            this.dgvSelector.AllowUserToAddRows = false;
            this.dgvSelector.AllowUserToDeleteRows = false;
            this.dgvSelector.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.dgvSelector.AutoGenerateColumns = false;
            this.dgvSelector.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
            this.dgvSelector.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvSelector.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.nameDataGridViewTextBoxColumn,
            this.objectTypeDataGridViewTextBoxColumn,
            this.pathWithinProjectDataGridViewTextBoxColumn});
            this.dgvSelector.DataSource = this.componentListItemBindingSource;
            this.dgvSelector.Location = new System.Drawing.Point(13, 32);
            this.dgvSelector.Name = "dgvSelector";
            this.dgvSelector.ReadOnly = true;
            this.dgvSelector.RowHeadersWidth = 20;
            this.dgvSelector.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dgvSelector.Size = new System.Drawing.Size(579, 496);
            this.dgvSelector.TabIndex = 2;
            this.dgvSelector.ColumnHeaderMouseClick += new System.Windows.Forms.DataGridViewCellMouseEventHandler(this.dgvSelector_ColumnHeaderMouseClick);
            // 
            // btnOK
            // 
            this.btnOK.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btnOK.Location = new System.Drawing.Point(410, 534);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(88, 32);
            this.btnOK.TabIndex = 0;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // lblHint
            // 
            this.lblHint.AutoSize = true;
            this.lblHint.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblHint.Location = new System.Drawing.Point(13, 13);
            this.lblHint.Name = "lblHint";
            this.lblHint.Size = new System.Drawing.Size(507, 15);
            this.lblHint.TabIndex = 2;
            this.lblHint.Text = "Please select the Components you would like to insert in the Design Container, th" +
    "en click OK";
            // 
            // btnCheckVF
            // 
            this.btnCheckVF.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btnCheckVF.Location = new System.Drawing.Point(16, 534);
            this.btnCheckVF.Name = "btnCheckVF";
            this.btnCheckVF.Size = new System.Drawing.Size(388, 32);
            this.btnCheckVF.TabIndex = 4;
            this.btnCheckVF.Text = "Check VehicleForge Exchange for matching Components";
            this.btnCheckVF.UseVisualStyleBackColor = true;
            this.btnCheckVF.Click += new System.EventHandler(this.btnCheckVF_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btnCancel.Location = new System.Drawing.Point(504, 534);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(88, 32);
            this.btnCancel.TabIndex = 1;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // componentListItemBindingSource
            // 
            this.componentListItemBindingSource.DataSource = typeof(CLM_light.ComponentListItem);
            // 
            // nameDataGridViewTextBoxColumn
            // 
            this.nameDataGridViewTextBoxColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.nameDataGridViewTextBoxColumn.DataPropertyName = "Name";
            this.nameDataGridViewTextBoxColumn.HeaderText = "Name";
            this.nameDataGridViewTextBoxColumn.Name = "nameDataGridViewTextBoxColumn";
            this.nameDataGridViewTextBoxColumn.ReadOnly = true;
            this.nameDataGridViewTextBoxColumn.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Programmatic;
            this.nameDataGridViewTextBoxColumn.Width = 60;
            // 
            // objectTypeDataGridViewTextBoxColumn
            // 
            this.objectTypeDataGridViewTextBoxColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.objectTypeDataGridViewTextBoxColumn.DataPropertyName = "ObjectType";
            this.objectTypeDataGridViewTextBoxColumn.HeaderText = "ObjectType";
            this.objectTypeDataGridViewTextBoxColumn.Name = "objectTypeDataGridViewTextBoxColumn";
            this.objectTypeDataGridViewTextBoxColumn.ReadOnly = true;
            this.objectTypeDataGridViewTextBoxColumn.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Programmatic;
            this.objectTypeDataGridViewTextBoxColumn.Width = 87;
            // 
            // pathWithinProjectDataGridViewTextBoxColumn
            // 
            this.pathWithinProjectDataGridViewTextBoxColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.pathWithinProjectDataGridViewTextBoxColumn.DataPropertyName = "PathWithinProject";
            this.pathWithinProjectDataGridViewTextBoxColumn.HeaderText = "PathWithinProject";
            this.pathWithinProjectDataGridViewTextBoxColumn.Name = "pathWithinProjectDataGridViewTextBoxColumn";
            this.pathWithinProjectDataGridViewTextBoxColumn.ReadOnly = true;
            this.pathWithinProjectDataGridViewTextBoxColumn.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Programmatic;
            // 
            // ComponentSelectionForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.ClientSize = new System.Drawing.Size(604, 578);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnCheckVF);
            this.Controls.Add(this.lblHint);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.dgvSelector);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(620, 590);
            this.Name = "ComponentSelectionForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Component Selection";
            ((System.ComponentModel.ISupportInitialize)(this.dgvSelector)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.componentListItemBindingSource)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        internal System.Windows.Forms.DataGridView dgvSelector;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Label lblHint;
        private System.Windows.Forms.Button btnCheckVF;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.BindingSource componentListItemBindingSource;
        private System.Windows.Forms.DataGridViewTextBoxColumn nameDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn objectTypeDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn pathWithinProjectDataGridViewTextBoxColumn;
    }
}