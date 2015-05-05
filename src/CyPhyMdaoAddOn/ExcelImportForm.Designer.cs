namespace CyPhyMdaoAddOn
{
	partial class ExcelImportForm
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
			this.lbParameters = new System.Windows.Forms.ListBox();
			this.lbNamedCells = new System.Windows.Forms.ListBox();
			this.lbMetrics = new System.Windows.Forms.ListBox();
			this.btnAddParameter = new System.Windows.Forms.Button();
			this.btnAddMetric = new System.Windows.Forms.Button();
			this.btnOK = new System.Windows.Forms.Button();
			this.lblParameters = new System.Windows.Forms.Label();
			this.lblMetrics = new System.Windows.Forms.Label();
			this.lblNamedCells = new System.Windows.Forms.Label();
			this.btnRemoveParameter = new System.Windows.Forms.Button();
			this.btnRemoveMetric = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// lbParameters
			// 
			this.lbParameters.FormattingEnabled = true;
			this.lbParameters.Location = new System.Drawing.Point(12, 23);
			this.lbParameters.Name = "lbParameters";
			this.lbParameters.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
			this.lbParameters.Size = new System.Drawing.Size(173, 199);
			this.lbParameters.TabIndex = 0;
			this.lbParameters.KeyDown += new System.Windows.Forms.KeyEventHandler(this.lbParameters_KeyDown);
			// 
			// lbNamedCells
			// 
			this.lbNamedCells.FormattingEnabled = true;
			this.lbNamedCells.Location = new System.Drawing.Point(227, 23);
			this.lbNamedCells.Name = "lbNamedCells";
			this.lbNamedCells.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
			this.lbNamedCells.Size = new System.Drawing.Size(173, 199);
			this.lbNamedCells.TabIndex = 0;
			this.lbNamedCells.KeyDown += new System.Windows.Forms.KeyEventHandler(this.lbNamedCells_KeyDown);
			// 
			// lbMetrics
			// 
			this.lbMetrics.FormattingEnabled = true;
			this.lbMetrics.Location = new System.Drawing.Point(440, 23);
			this.lbMetrics.Name = "lbMetrics";
			this.lbMetrics.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
			this.lbMetrics.Size = new System.Drawing.Size(173, 199);
			this.lbMetrics.TabIndex = 0;
			this.lbMetrics.KeyDown += new System.Windows.Forms.KeyEventHandler(this.lbMetrics_KeyDown);
			// 
			// btnAddParameter
			// 
			this.btnAddParameter.Location = new System.Drawing.Point(191, 92);
			this.btnAddParameter.Name = "btnAddParameter";
			this.btnAddParameter.Size = new System.Drawing.Size(29, 23);
			this.btnAddParameter.TabIndex = 1;
			this.btnAddParameter.Text = "<";
			this.btnAddParameter.UseVisualStyleBackColor = true;
			this.btnAddParameter.Click += new System.EventHandler(this.btnAddParameter_Click);
			// 
			// btnAddMetric
			// 
			this.btnAddMetric.Location = new System.Drawing.Point(406, 92);
			this.btnAddMetric.Name = "btnAddMetric";
			this.btnAddMetric.Size = new System.Drawing.Size(29, 23);
			this.btnAddMetric.TabIndex = 1;
			this.btnAddMetric.Text = ">";
			this.btnAddMetric.UseVisualStyleBackColor = true;
			this.btnAddMetric.Click += new System.EventHandler(this.btnAddMetric_Click);
			// 
			// btnOK
			// 
			this.btnOK.Location = new System.Drawing.Point(278, 228);
			this.btnOK.Name = "btnOK";
			this.btnOK.Size = new System.Drawing.Size(75, 23);
			this.btnOK.TabIndex = 2;
			this.btnOK.Text = "OK";
			this.btnOK.UseVisualStyleBackColor = true;
			this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
			// 
			// lblParameters
			// 
			this.lblParameters.AutoSize = true;
			this.lblParameters.Location = new System.Drawing.Point(62, 3);
			this.lblParameters.Name = "lblParameters";
			this.lblParameters.Size = new System.Drawing.Size(60, 13);
			this.lblParameters.TabIndex = 3;
			this.lblParameters.Text = "Parameters";
			// 
			// lblMetrics
			// 
			this.lblMetrics.AutoSize = true;
			this.lblMetrics.Location = new System.Drawing.Point(501, 3);
			this.lblMetrics.Name = "lblMetrics";
			this.lblMetrics.Size = new System.Drawing.Size(41, 13);
			this.lblMetrics.TabIndex = 3;
			this.lblMetrics.Text = "Metrics";
			// 
			// lblNamedCells
			// 
			this.lblNamedCells.AutoSize = true;
			this.lblNamedCells.Location = new System.Drawing.Point(253, 3);
			this.lblNamedCells.Name = "lblNamedCells";
			this.lblNamedCells.Size = new System.Drawing.Size(110, 13);
			this.lblNamedCells.TabIndex = 3;
			this.lblNamedCells.Text = "Detected named cells";
			// 
			// btnRemoveParameter
			// 
			this.btnRemoveParameter.Location = new System.Drawing.Point(191, 121);
			this.btnRemoveParameter.Name = "btnRemoveParameter";
			this.btnRemoveParameter.Size = new System.Drawing.Size(29, 23);
			this.btnRemoveParameter.TabIndex = 1;
			this.btnRemoveParameter.Text = ">";
			this.btnRemoveParameter.UseVisualStyleBackColor = true;
			this.btnRemoveParameter.Click += new System.EventHandler(this.btnRemoveParameter_Click);
			// 
			// btnRemoveMetric
			// 
			this.btnRemoveMetric.Location = new System.Drawing.Point(406, 121);
			this.btnRemoveMetric.Name = "btnRemoveMetric";
			this.btnRemoveMetric.Size = new System.Drawing.Size(29, 23);
			this.btnRemoveMetric.TabIndex = 1;
			this.btnRemoveMetric.Text = "<";
			this.btnRemoveMetric.UseVisualStyleBackColor = true;
			this.btnRemoveMetric.Click += new System.EventHandler(this.btnRemoveMetric_Click);
			// 
			// ExcelImportForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(625, 262);
			this.Controls.Add(this.lblMetrics);
			this.Controls.Add(this.lblNamedCells);
			this.Controls.Add(this.lblParameters);
			this.Controls.Add(this.btnOK);
			this.Controls.Add(this.btnRemoveMetric);
			this.Controls.Add(this.btnRemoveParameter);
			this.Controls.Add(this.btnAddMetric);
			this.Controls.Add(this.btnAddParameter);
			this.Controls.Add(this.lbMetrics);
			this.Controls.Add(this.lbNamedCells);
			this.Controls.Add(this.lbParameters);
			this.Name = "ExcelImportForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "ExcelImportForm";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button btnAddParameter;
		private System.Windows.Forms.Button btnAddMetric;
		private System.Windows.Forms.Button btnOK;
		private System.Windows.Forms.Label lblParameters;
		private System.Windows.Forms.Label lblMetrics;
		private System.Windows.Forms.Label lblNamedCells;
		internal System.Windows.Forms.ListBox lbParameters;
		internal System.Windows.Forms.ListBox lbMetrics;
		internal System.Windows.Forms.ListBox lbNamedCells;
		private System.Windows.Forms.Button btnRemoveParameter;
		private System.Windows.Forms.Button btnRemoveMetric;

	}
}