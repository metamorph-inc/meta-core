namespace CyPhyMasterInterpreter
{
    partial class ConfigurationSelectionForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ConfigurationSelectionForm));
            this.txtOutputDir = new System.Windows.Forms.TextBox();
            this.lblOutputDir = new System.Windows.Forms.Label();
            this.btnOK = new System.Windows.Forms.Button();
            this.lbConfigModels = new System.Windows.Forms.ListBox();
            this.lbWorkFlow = new System.Windows.Forms.ListBox();
            this.lblConfigModels = new System.Windows.Forms.Label();
            this.lbExportedCAs = new System.Windows.Forms.ListBox();
            this.lblSelectedConfigs = new System.Windows.Forms.Label();
            this.toolTipCA = new System.Windows.Forms.ToolTip(this.components);
            this.btnSelectAll = new System.Windows.Forms.Button();
            this.btnClearAll = new System.Windows.Forms.Button();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.tsslStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.btnCancel = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.chbPostJobs = new System.Windows.Forms.CheckBox();
            this.chbShowDirty = new System.Windows.Forms.CheckBox();
            this.lblCASelected = new System.Windows.Forms.Label();
            this.chbSaveTestBenches = new System.Windows.Forms.CheckBox();
            this.chbOpenDashboard = new System.Windows.Forms.CheckBox();
            this.chbVerbose = new System.Windows.Forms.CheckBox();
            this.commandLinkRunParallel = new CyPhyGUIs.CommandLink();
            this.statusStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // txtOutputDir
            // 
            this.txtOutputDir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtOutputDir.Location = new System.Drawing.Point(13, 24);
            this.txtOutputDir.Name = "txtOutputDir";
            this.txtOutputDir.ReadOnly = true;
            this.txtOutputDir.Size = new System.Drawing.Size(507, 20);
            this.txtOutputDir.TabIndex = 100;
            // 
            // lblOutputDir
            // 
            this.lblOutputDir.AutoSize = true;
            this.lblOutputDir.Location = new System.Drawing.Point(14, 6);
            this.lblOutputDir.Name = "lblOutputDir";
            this.lblOutputDir.Size = new System.Drawing.Size(172, 13);
            this.lblOutputDir.TabIndex = 2;
            this.lblOutputDir.Text = "Output directory for generated files:";
            // 
            // btnOK
            // 
            this.btnOK.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btnOK.Location = new System.Drawing.Point(182, 515);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(80, 22);
            this.btnOK.TabIndex = 0;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // lbConfigModels
            // 
            this.lbConfigModels.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.lbConfigModels.FormattingEnabled = true;
            this.lbConfigModels.HorizontalScrollbar = true;
            this.lbConfigModels.Location = new System.Drawing.Point(13, 95);
            this.lbConfigModels.Name = "lbConfigModels";
            this.lbConfigModels.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.lbConfigModels.Size = new System.Drawing.Size(248, 199);
            this.lbConfigModels.TabIndex = 20;
            this.lbConfigModels.SelectedIndexChanged += new System.EventHandler(this.lbConfigModels_SelectedIndexChanged);
            // 
            // lbWorkFlow
            // 
            this.lbWorkFlow.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lbWorkFlow.Enabled = false;
            this.lbWorkFlow.FormattingEnabled = true;
            this.lbWorkFlow.HorizontalScrollbar = true;
            this.lbWorkFlow.Location = new System.Drawing.Point(13, 342);
            this.lbWorkFlow.Name = "lbWorkFlow";
            this.lbWorkFlow.Size = new System.Drawing.Size(248, 121);
            this.lbWorkFlow.TabIndex = 100;
            this.lbWorkFlow.TabStop = false;
            // 
            // lblConfigModels
            // 
            this.lblConfigModels.AutoSize = true;
            this.lblConfigModels.Location = new System.Drawing.Point(59, 78);
            this.lblConfigModels.Name = "lblConfigModels";
            this.lblConfigModels.Size = new System.Drawing.Size(157, 13);
            this.lblConfigModels.TabIndex = 100;
            this.lblConfigModels.Text = "Generated configuration models";
            // 
            // lbExportedCAs
            // 
            this.lbExportedCAs.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lbExportedCAs.FormattingEnabled = true;
            this.lbExportedCAs.HorizontalScrollbar = true;
            this.lbExportedCAs.Location = new System.Drawing.Point(272, 95);
            this.lbExportedCAs.Name = "lbExportedCAs";
            this.lbExportedCAs.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.lbExportedCAs.Size = new System.Drawing.Size(248, 316);
            this.lbExportedCAs.TabIndex = 21;
            this.lbExportedCAs.SelectedIndexChanged += new System.EventHandler(this.lbExportedCAs_SelectedIndexChanged);
            this.lbExportedCAs.KeyDown += new System.Windows.Forms.KeyEventHandler(this.lbExportedCAs_KeyDown);
            this.lbExportedCAs.MouseMove += new System.Windows.Forms.MouseEventHandler(this.lbExportedCAs_MouseMove);
            // 
            // lblSelectedConfigs
            // 
            this.lblSelectedConfigs.AutoSize = true;
            this.lblSelectedConfigs.Location = new System.Drawing.Point(272, 78);
            this.lblSelectedConfigs.Name = "lblSelectedConfigs";
            this.lblSelectedConfigs.Size = new System.Drawing.Size(139, 13);
            this.lblSelectedConfigs.TabIndex = 100;
            this.lblSelectedConfigs.Text = "Please select configurations";
            // 
            // btnSelectAll
            // 
            this.btnSelectAll.Location = new System.Drawing.Point(272, 54);
            this.btnSelectAll.Name = "btnSelectAll";
            this.btnSelectAll.Size = new System.Drawing.Size(123, 22);
            this.btnSelectAll.TabIndex = 11;
            this.btnSelectAll.Text = "Select All";
            this.btnSelectAll.UseVisualStyleBackColor = true;
            this.btnSelectAll.Click += new System.EventHandler(this.btnSelectAll_Click);
            // 
            // btnClearAll
            // 
            this.btnClearAll.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnClearAll.Location = new System.Drawing.Point(397, 54);
            this.btnClearAll.Name = "btnClearAll";
            this.btnClearAll.Size = new System.Drawing.Size(123, 22);
            this.btnClearAll.TabIndex = 12;
            this.btnClearAll.Text = "Clear All";
            this.btnClearAll.UseVisualStyleBackColor = true;
            this.btnClearAll.Click += new System.EventHandler(this.btnClearAll_Click);
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsslStatus});
            this.statusStrip.Location = new System.Drawing.Point(0, 540);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Padding = new System.Windows.Forms.Padding(1, 0, 15, 0);
            this.statusStrip.Size = new System.Drawing.Size(534, 22);
            this.statusStrip.TabIndex = 13;
            this.statusStrip.Text = "statusStrip";
            // 
            // tsslStatus
            // 
            this.tsslStatus.Name = "tsslStatus";
            this.tsslStatus.Size = new System.Drawing.Size(114, 17);
            this.tsslStatus.Text = "Context information";
            // 
            // btnCancel
            // 
            this.btnCancel.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(272, 515);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(80, 22);
            this.btnCancel.TabIndex = 1;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(67, 324);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(141, 13);
            this.label2.TabIndex = 100;
            this.label2.Text = "Detected workflow definition";
            // 
            // chbPostJobs
            // 
            this.chbPostJobs.AutoSize = true;
            this.chbPostJobs.Location = new System.Drawing.Point(15, 57);
            this.chbPostJobs.Name = "chbPostJobs";
            this.chbPostJobs.Size = new System.Drawing.Size(157, 17);
            this.chbPostJobs.TabIndex = 10;
            this.chbPostJobs.Text = "Post to META Job Manager";
            this.chbPostJobs.UseVisualStyleBackColor = true;
            // 
            // chbShowDirty
            // 
            this.chbShowDirty.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.chbShowDirty.AutoSize = true;
            this.chbShowDirty.Location = new System.Drawing.Point(12, 300);
            this.chbShowDirty.Name = "chbShowDirty";
            this.chbShowDirty.Size = new System.Drawing.Size(75, 17);
            this.chbShowDirty.TabIndex = 30;
            this.chbShowDirty.Text = "Show dirty";
            this.chbShowDirty.UseVisualStyleBackColor = true;
            this.chbShowDirty.CheckedChanged += new System.EventHandler(this.chbShowDirty_CheckedChanged);
            // 
            // lblCASelected
            // 
            this.lblCASelected.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lblCASelected.Location = new System.Drawing.Point(422, 78);
            this.lblCASelected.Name = "lblCASelected";
            this.lblCASelected.Size = new System.Drawing.Size(98, 13);
            this.lblCASelected.TabIndex = 100;
            this.lblCASelected.Text = "0 / 0";
            this.lblCASelected.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // chbSaveTestBenches
            // 
            this.chbSaveTestBenches.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.chbSaveTestBenches.AutoSize = true;
            this.chbSaveTestBenches.Location = new System.Drawing.Point(15, 515);
            this.chbSaveTestBenches.Name = "chbSaveTestBenches";
            this.chbSaveTestBenches.Size = new System.Drawing.Size(165, 17);
            this.chbSaveTestBenches.TabIndex = 42;
            this.chbSaveTestBenches.Text = "Damage model for debugging";
            this.chbSaveTestBenches.UseVisualStyleBackColor = true;
            // 
            // chbOpenDashboard
            // 
            this.chbOpenDashboard.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.chbOpenDashboard.AutoSize = true;
            this.chbOpenDashboard.Location = new System.Drawing.Point(15, 469);
            this.chbOpenDashboard.Name = "chbOpenDashboard";
            this.chbOpenDashboard.Size = new System.Drawing.Size(105, 17);
            this.chbOpenDashboard.TabIndex = 41;
            this.chbOpenDashboard.Text = "Open dashboard";
            this.chbOpenDashboard.UseVisualStyleBackColor = true;
            // 
            // chbVerbose
            // 
            this.chbVerbose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.chbVerbose.AutoSize = true;
            this.chbVerbose.Location = new System.Drawing.Point(15, 493);
            this.chbVerbose.Name = "chbVerbose";
            this.chbVerbose.Size = new System.Drawing.Size(105, 17);
            this.chbVerbose.TabIndex = 101;
            this.chbVerbose.Text = "Verbose console";
            this.chbVerbose.UseVisualStyleBackColor = true;
            // 
            // commandLinkRunParallel
            // 
            this.commandLinkRunParallel.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.commandLinkRunParallel.Location = new System.Drawing.Point(275, 422);
            this.commandLinkRunParallel.Name = "commandLinkRunParallel";
            this.commandLinkRunParallel.Size = new System.Drawing.Size(225, 41);
            this.commandLinkRunParallel.TabIndex = 102;
            this.commandLinkRunParallel.Text = "Run all configs in parallel";
            this.commandLinkRunParallel.UseVisualStyleBackColor = true;
            this.commandLinkRunParallel.Click += new System.EventHandler(this.CommandLinkRunParallel_Click);
            // 
            // ConfigurationSelectionForm
            // 
            this.AcceptButton = this.btnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(534, 562);
            this.Controls.Add(this.commandLinkRunParallel);
            this.Controls.Add(this.chbVerbose);
            this.Controls.Add(this.chbOpenDashboard);
            this.Controls.Add(this.lblCASelected);
            this.Controls.Add(this.chbShowDirty);
            this.Controls.Add(this.chbSaveTestBenches);
            this.Controls.Add(this.chbPostJobs);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.btnClearAll);
            this.Controls.Add(this.btnSelectAll);
            this.Controls.Add(this.lblSelectedConfigs);
            this.Controls.Add(this.lblConfigModels);
            this.Controls.Add(this.lbWorkFlow);
            this.Controls.Add(this.lbExportedCAs);
            this.Controls.Add(this.lbConfigModels);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.lblOutputDir);
            this.Controls.Add(this.txtOutputDir);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(550, 600);
            this.Name = "ConfigurationSelectionForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "CyPhy Master Interpreter";
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

        private System.Windows.Forms.TextBox txtOutputDir;
		private System.Windows.Forms.Label lblOutputDir;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Label lblConfigModels;
		private System.Windows.Forms.Label lblSelectedConfigs;
		private System.Windows.Forms.ToolTip toolTipCA;
		private System.Windows.Forms.Button btnSelectAll;
        private System.Windows.Forms.Button btnClearAll;
		private System.Windows.Forms.StatusStrip statusStrip;
		private System.Windows.Forms.ToolStripStatusLabel tsslStatus;
		private System.Windows.Forms.Button btnCancel;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.CheckBox chbShowDirty;
        private System.Windows.Forms.Label lblCASelected;
        private System.Windows.Forms.ListBox lbConfigModels;
        private System.Windows.Forms.ListBox lbWorkFlow;
        private System.Windows.Forms.ListBox lbExportedCAs;
        private System.Windows.Forms.CheckBox chbPostJobs;
        private System.Windows.Forms.CheckBox chbSaveTestBenches;
        private System.Windows.Forms.CheckBox chbOpenDashboard;
        private System.Windows.Forms.CheckBox chbVerbose;
        private CyPhyGUIs.CommandLink commandLinkRunParallel;
    }
}
