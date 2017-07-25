namespace WorkflowDecorator
{
	partial class ParameterSettingsForm
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.lblAnalysisToolSelection = new System.Windows.Forms.Label();
            this.cmbAnalysisToolSelection = new System.Windows.Forms.ComboBox();
            this.parameterGridView = new System.Windows.Forms.DataGridView();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.parameterGridView)).BeginInit();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.Controls.Add(this.lblAnalysisToolSelection);
            this.panel1.Controls.Add(this.cmbAnalysisToolSelection);
            this.panel1.Controls.Add(this.parameterGridView);
            this.panel1.Location = new System.Drawing.Point(12, 12);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(531, 401);
            this.panel1.TabIndex = 0;
            // 
            // lblAnalysisToolSelection
            // 
            this.lblAnalysisToolSelection.AutoSize = true;
            this.lblAnalysisToolSelection.Location = new System.Drawing.Point(3, 6);
            this.lblAnalysisToolSelection.Name = "lblAnalysisToolSelection";
            this.lblAnalysisToolSelection.Size = new System.Drawing.Size(116, 13);
            this.lblAnalysisToolSelection.TabIndex = 2;
            this.lblAnalysisToolSelection.Text = "Analysis Tool Selection";
            // 
            // cmbAnalysisToolSelection
            // 
            this.cmbAnalysisToolSelection.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbAnalysisToolSelection.FormattingEnabled = true;
            this.cmbAnalysisToolSelection.Location = new System.Drawing.Point(155, 3);
            this.cmbAnalysisToolSelection.Name = "cmbAnalysisToolSelection";
            this.cmbAnalysisToolSelection.Size = new System.Drawing.Size(188, 21);
            this.cmbAnalysisToolSelection.Sorted = true;
            this.cmbAnalysisToolSelection.TabIndex = 1;
            // 
            // parameterGridView
            // 
            this.parameterGridView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.parameterGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.parameterGridView.Location = new System.Drawing.Point(3, 30);
            this.parameterGridView.Name = "parameterGridView";
            this.parameterGridView.Size = new System.Drawing.Size(525, 368);
            this.parameterGridView.TabIndex = 0;
            this.parameterGridView.ColumnHeaderMouseClick += new System.Windows.Forms.DataGridViewCellMouseEventHandler(this.parameterGridView_ColumnHeaderMouseClick);
            // 
            // ParameterSettingsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(555, 425);
            this.Controls.Add(this.panel1);
            this.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.Name = "ParameterSettingsForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Workflow Parameters";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ParameterSettingsForm_FormClosing);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.ParameterSettingsForm_FormClosed);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.parameterGridView)).EndInit();
            this.ResumeLayout(false);

		}

		#endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.DataGridView parameterGridView;
        private System.Windows.Forms.Label lblAnalysisToolSelection;
        private System.Windows.Forms.ComboBox cmbAnalysisToolSelection;

    }
}
