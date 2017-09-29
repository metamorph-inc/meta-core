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
            this.chbVerbose = new System.Windows.Forms.CheckBox();
            this.commandLinkRunParallel = new CyPhyGUIs.CommandLink();
            this.linkLabelRunDesertAllConfigs = new System.Windows.Forms.LinkLabel();
            this.labelNoDesetConfigurations = new System.Windows.Forms.Label();
            this.linkLabelRunDesert = new System.Windows.Forms.LinkLabel();
            this.statusStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // txtOutputDir
            // 
            this.txtOutputDir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtOutputDir.Location = new System.Drawing.Point(20, 36);
            this.txtOutputDir.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.txtOutputDir.Name = "txtOutputDir";
            this.txtOutputDir.ReadOnly = true;
            this.txtOutputDir.Size = new System.Drawing.Size(758, 26);
            this.txtOutputDir.TabIndex = 100;
            // 
            // lblOutputDir
            // 
            this.lblOutputDir.AutoSize = true;
            this.lblOutputDir.Location = new System.Drawing.Point(21, 9);
            this.lblOutputDir.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblOutputDir.Name = "lblOutputDir";
            this.lblOutputDir.Size = new System.Drawing.Size(258, 20);
            this.lblOutputDir.TabIndex = 2;
            this.lblOutputDir.Text = "Output directory for generated files:";
            // 
            // btnOK
            // 
            this.btnOK.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btnOK.Location = new System.Drawing.Point(273, 772);
            this.btnOK.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(120, 33);
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
            this.lbConfigModels.ItemHeight = 20;
            this.lbConfigModels.Location = new System.Drawing.Point(20, 142);
            this.lbConfigModels.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.lbConfigModels.Name = "lbConfigModels";
            this.lbConfigModels.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.lbConfigModels.Size = new System.Drawing.Size(370, 284);
            this.lbConfigModels.TabIndex = 20;
            this.lbConfigModels.SelectedIndexChanged += new System.EventHandler(this.lbConfigModels_SelectedIndexChanged);
            this.lbConfigModels.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.lbConfigModels_PreviewKeyDown);
            // 
            // lbWorkFlow
            // 
            this.lbWorkFlow.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lbWorkFlow.Enabled = false;
            this.lbWorkFlow.FormattingEnabled = true;
            this.lbWorkFlow.HorizontalScrollbar = true;
            this.lbWorkFlow.ItemHeight = 20;
            this.lbWorkFlow.Location = new System.Drawing.Point(20, 513);
            this.lbWorkFlow.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.lbWorkFlow.Name = "lbWorkFlow";
            this.lbWorkFlow.Size = new System.Drawing.Size(370, 164);
            this.lbWorkFlow.TabIndex = 100;
            this.lbWorkFlow.TabStop = false;
            // 
            // lblConfigModels
            // 
            this.lblConfigModels.AutoSize = true;
            this.lblConfigModels.Location = new System.Drawing.Point(88, 117);
            this.lblConfigModels.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblConfigModels.Name = "lblConfigModels";
            this.lblConfigModels.Size = new System.Drawing.Size(237, 20);
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
            this.lbExportedCAs.ItemHeight = 20;
            this.lbExportedCAs.Location = new System.Drawing.Point(408, 142);
            this.lbExportedCAs.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.lbExportedCAs.Name = "lbExportedCAs";
            this.lbExportedCAs.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.lbExportedCAs.Size = new System.Drawing.Size(370, 464);
            this.lbExportedCAs.TabIndex = 21;
            this.lbExportedCAs.SelectedIndexChanged += new System.EventHandler(this.lbExportedCAs_SelectedIndexChanged);
            this.lbExportedCAs.KeyDown += new System.Windows.Forms.KeyEventHandler(this.lbExportedCAs_KeyDown);
            this.lbExportedCAs.MouseMove += new System.Windows.Forms.MouseEventHandler(this.lbExportedCAs_MouseMove);
            // 
            // lblSelectedConfigs
            // 
            this.lblSelectedConfigs.AutoSize = true;
            this.lblSelectedConfigs.Location = new System.Drawing.Point(408, 117);
            this.lblSelectedConfigs.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblSelectedConfigs.Name = "lblSelectedConfigs";
            this.lblSelectedConfigs.Size = new System.Drawing.Size(207, 20);
            this.lblSelectedConfigs.TabIndex = 100;
            this.lblSelectedConfigs.Text = "Please select configurations";
            // 
            // btnSelectAll
            // 
            this.btnSelectAll.Location = new System.Drawing.Point(408, 81);
            this.btnSelectAll.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btnSelectAll.Name = "btnSelectAll";
            this.btnSelectAll.Size = new System.Drawing.Size(184, 33);
            this.btnSelectAll.TabIndex = 11;
            this.btnSelectAll.Text = "Select All";
            this.btnSelectAll.UseVisualStyleBackColor = true;
            this.btnSelectAll.Click += new System.EventHandler(this.btnSelectAll_Click);
            // 
            // btnClearAll
            // 
            this.btnClearAll.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnClearAll.Location = new System.Drawing.Point(596, 81);
            this.btnClearAll.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btnClearAll.Name = "btnClearAll";
            this.btnClearAll.Size = new System.Drawing.Size(184, 33);
            this.btnClearAll.TabIndex = 12;
            this.btnClearAll.Text = "Clear All";
            this.btnClearAll.UseVisualStyleBackColor = true;
            this.btnClearAll.Click += new System.EventHandler(this.btnClearAll_Click);
            // 
            // statusStrip
            // 
            this.statusStrip.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsslStatus});
            this.statusStrip.Location = new System.Drawing.Point(0, 813);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Padding = new System.Windows.Forms.Padding(2, 0, 22, 0);
            this.statusStrip.Size = new System.Drawing.Size(801, 30);
            this.statusStrip.TabIndex = 13;
            this.statusStrip.Text = "statusStrip";
            // 
            // tsslStatus
            // 
            this.tsslStatus.Name = "tsslStatus";
            this.tsslStatus.Size = new System.Drawing.Size(171, 25);
            this.tsslStatus.Text = "Context information";
            // 
            // btnCancel
            // 
            this.btnCancel.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(408, 772);
            this.btnCancel.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(120, 33);
            this.btnCancel.TabIndex = 1;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(100, 486);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(208, 20);
            this.label2.TabIndex = 100;
            this.label2.Text = "Detected workflow definition";
            // 
            // chbPostJobs
            // 
            this.chbPostJobs.AutoSize = true;
            this.chbPostJobs.Location = new System.Drawing.Point(22, 86);
            this.chbPostJobs.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.chbPostJobs.Name = "chbPostJobs";
            this.chbPostJobs.Size = new System.Drawing.Size(230, 24);
            this.chbPostJobs.TabIndex = 10;
            this.chbPostJobs.Text = "Post to META Job Manager";
            this.chbPostJobs.UseVisualStyleBackColor = true;
            // 
            // chbShowDirty
            // 
            this.chbShowDirty.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.chbShowDirty.AutoSize = true;
            this.chbShowDirty.Location = new System.Drawing.Point(18, 452);
            this.chbShowDirty.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.chbShowDirty.Name = "chbShowDirty";
            this.chbShowDirty.Size = new System.Drawing.Size(108, 24);
            this.chbShowDirty.TabIndex = 30;
            this.chbShowDirty.Text = "Show dirty";
            this.chbShowDirty.UseVisualStyleBackColor = true;
            this.chbShowDirty.CheckedChanged += new System.EventHandler(this.chbShowDirty_CheckedChanged);
            // 
            // lblCASelected
            // 
            this.lblCASelected.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lblCASelected.Location = new System.Drawing.Point(633, 117);
            this.lblCASelected.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblCASelected.Name = "lblCASelected";
            this.lblCASelected.Size = new System.Drawing.Size(147, 20);
            this.lblCASelected.TabIndex = 100;
            this.lblCASelected.Text = "0 / 0";
            this.lblCASelected.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // chbSaveTestBenches
            // 
            this.chbSaveTestBenches.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.chbSaveTestBenches.AutoSize = true;
            this.chbSaveTestBenches.Location = new System.Drawing.Point(18, 738);
            this.chbSaveTestBenches.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.chbSaveTestBenches.Name = "chbSaveTestBenches";
            this.chbSaveTestBenches.Size = new System.Drawing.Size(245, 24);
            this.chbSaveTestBenches.TabIndex = 42;
            this.chbSaveTestBenches.Text = "Damage model for debugging";
            this.chbSaveTestBenches.UseVisualStyleBackColor = true;
            // 
            // chbVerbose
            // 
            this.chbVerbose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.chbVerbose.AutoSize = true;
            this.chbVerbose.Location = new System.Drawing.Point(18, 706);
            this.chbVerbose.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.chbVerbose.Name = "chbVerbose";
            this.chbVerbose.Size = new System.Drawing.Size(154, 24);
            this.chbVerbose.TabIndex = 101;
            this.chbVerbose.Text = "Verbose console";
            this.chbVerbose.UseVisualStyleBackColor = true;
            // 
            // commandLinkRunParallel
            // 
            this.commandLinkRunParallel.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.commandLinkRunParallel.Location = new System.Drawing.Point(412, 633);
            this.commandLinkRunParallel.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.commandLinkRunParallel.Name = "commandLinkRunParallel";
            this.commandLinkRunParallel.Size = new System.Drawing.Size(338, 62);
            this.commandLinkRunParallel.TabIndex = 102;
            this.commandLinkRunParallel.Text = "Run in parallel";
            this.commandLinkRunParallel.UseVisualStyleBackColor = true;
            this.commandLinkRunParallel.Click += new System.EventHandler(this.CommandLinkRunParallel_Click);
            // 
            // linkLabelRunDesertAllConfigs
            // 
            this.linkLabelRunDesertAllConfigs.AutoSize = true;
            this.linkLabelRunDesertAllConfigs.BackColor = System.Drawing.SystemColors.Window;
            this.linkLabelRunDesertAllConfigs.Location = new System.Drawing.Point(25, 250);
            this.linkLabelRunDesertAllConfigs.Name = "linkLabelRunDesertAllConfigs";
            this.linkLabelRunDesertAllConfigs.Size = new System.Drawing.Size(244, 20);
            this.linkLabelRunDesertAllConfigs.TabIndex = 103;
            this.linkLabelRunDesertAllConfigs.TabStop = true;
            this.linkLabelRunDesertAllConfigs.Text = "Run Desert and export all configs";
            this.linkLabelRunDesertAllConfigs.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabelRunDesertAllConfigs_LinkClicked);
            // 
            // labelNoDesetConfigurations
            // 
            this.labelNoDesetConfigurations.AutoSize = true;
            this.labelNoDesetConfigurations.BackColor = System.Drawing.SystemColors.Window;
            this.labelNoDesetConfigurations.Location = new System.Drawing.Point(25, 180);
            this.labelNoDesetConfigurations.Name = "labelNoDesetConfigurations";
            this.labelNoDesetConfigurations.Size = new System.Drawing.Size(133, 20);
            this.labelNoDesetConfigurations.TabIndex = 104;
            this.labelNoDesetConfigurations.Text = "No configurations";
            // 
            // linkLabelRunDesert
            // 
            this.linkLabelRunDesert.AutoSize = true;
            this.linkLabelRunDesert.BackColor = System.Drawing.SystemColors.Window;
            this.linkLabelRunDesert.Location = new System.Drawing.Point(25, 215);
            this.linkLabelRunDesert.Name = "linkLabelRunDesert";
            this.linkLabelRunDesert.Size = new System.Drawing.Size(91, 20);
            this.linkLabelRunDesert.TabIndex = 105;
            this.linkLabelRunDesert.TabStop = true;
            this.linkLabelRunDesert.Text = "Run Desert";
            this.linkLabelRunDesert.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabel1_LinkClicked);
            // 
            // ConfigurationSelectionForm
            // 
            this.AcceptButton = this.btnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(144F, 144F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(801, 843);
            this.Controls.Add(this.linkLabelRunDesert);
            this.Controls.Add(this.labelNoDesetConfigurations);
            this.Controls.Add(this.linkLabelRunDesertAllConfigs);
            this.Controls.Add(this.commandLinkRunParallel);
            this.Controls.Add(this.chbVerbose);
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
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.MinimumSize = new System.Drawing.Size(814, 872);
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
        private System.Windows.Forms.CheckBox chbVerbose;
        private CyPhyGUIs.CommandLink commandLinkRunParallel;
        private System.Windows.Forms.LinkLabel linkLabelRunDesertAllConfigs;
        private System.Windows.Forms.Label labelNoDesetConfigurations;
        private System.Windows.Forms.LinkLabel linkLabelRunDesert;
    }
}
