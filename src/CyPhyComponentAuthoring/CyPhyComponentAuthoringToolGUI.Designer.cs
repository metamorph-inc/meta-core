namespace CyPhyComponentAuthoring
{
    partial class CyPhyComponentAuthoringToolGUI
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CyPhyComponentAuthoringToolGUI));
            this.CatModuleListView = new System.Windows.Forms.ListView();
            this.ComponentIconList = new System.Windows.Forms.ImageList(this.components);
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.ModuleNameLabel = new System.Windows.Forms.TextBox();
            this.RunToolButton = new System.Windows.Forms.Button();
            this.ModuleDetailsTextBox = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // CatModuleListView
            // 
            this.CatModuleListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CatModuleListView.LargeImageList = this.ComponentIconList;
            this.CatModuleListView.Location = new System.Drawing.Point(0, 0);
            this.CatModuleListView.MultiSelect = false;
            this.CatModuleListView.Name = "CatModuleListView";
            this.CatModuleListView.Size = new System.Drawing.Size(527, 395);
            this.CatModuleListView.TabIndex = 0;
            this.CatModuleListView.TileSize = new System.Drawing.Size(168, 56);
            this.CatModuleListView.UseCompatibleStateImageBehavior = false;
            this.CatModuleListView.View = System.Windows.Forms.View.Tile;
            this.CatModuleListView.ItemActivate += new System.EventHandler(this.CatModuleListView_ItemActivate);
            this.CatModuleListView.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.CatModuleListView_ItemSelectionChanged);
            // 
            // ComponentIconList
            // 
            this.ComponentIconList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ComponentIconList.ImageStream")));
            this.ComponentIconList.TransparentColor = System.Drawing.Color.Transparent;
            this.ComponentIconList.Images.SetKeyName(0, "CyPhyComponentAuthoring.ico");
            // 
            // splitContainer1
            // 
            this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.splitContainer1.Location = new System.Drawing.Point(12, 12);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.CatModuleListView);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.ModuleNameLabel);
            this.splitContainer1.Panel2.Controls.Add(this.RunToolButton);
            this.splitContainer1.Panel2.Controls.Add(this.ModuleDetailsTextBox);
            this.splitContainer1.Size = new System.Drawing.Size(767, 395);
            this.splitContainer1.SplitterDistance = 527;
            this.splitContainer1.TabIndex = 2;
            // 
            // ModuleNameLabel
            // 
            this.ModuleNameLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ModuleNameLabel.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.ModuleNameLabel.Cursor = System.Windows.Forms.Cursors.Default;
            this.ModuleNameLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ModuleNameLabel.Location = new System.Drawing.Point(6, 3);
            this.ModuleNameLabel.Multiline = true;
            this.ModuleNameLabel.Name = "ModuleNameLabel";
            this.ModuleNameLabel.ReadOnly = true;
            this.ModuleNameLabel.Size = new System.Drawing.Size(227, 20);
            this.ModuleNameLabel.TabIndex = 3;
            // 
            // RunToolButton
            // 
            this.RunToolButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.RunToolButton.Enabled = false;
            this.RunToolButton.Location = new System.Drawing.Point(3, 363);
            this.RunToolButton.Name = "RunToolButton";
            this.RunToolButton.Size = new System.Drawing.Size(230, 33);
            this.RunToolButton.TabIndex = 2;
            this.RunToolButton.Text = "Run Tool";
            this.RunToolButton.UseVisualStyleBackColor = true;
            this.RunToolButton.Click += new System.EventHandler(this.RunToolButton_Click);
            // 
            // ModuleDetailsTextBox
            // 
            this.ModuleDetailsTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ModuleDetailsTextBox.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.ModuleDetailsTextBox.Cursor = System.Windows.Forms.Cursors.Default;
            this.ModuleDetailsTextBox.Location = new System.Drawing.Point(6, 22);
            this.ModuleDetailsTextBox.Multiline = true;
            this.ModuleDetailsTextBox.Name = "ModuleDetailsTextBox";
            this.ModuleDetailsTextBox.ReadOnly = true;
            this.ModuleDetailsTextBox.Size = new System.Drawing.Size(227, 331);
            this.ModuleDetailsTextBox.TabIndex = 1;
            // 
            // CyPhyComponentAuthoringToolGUI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ClientSize = new System.Drawing.Size(791, 419);
            this.Controls.Add(this.splitContainer1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "CyPhyComponentAuthoringToolGUI";
            this.Text = "Component Authoring Tool (CAT)";
            this.Load += new System.EventHandler(this.CyPhyComponentAuthoringToolGUI_Load);
            this.DragDrop += new System.Windows.Forms.DragEventHandler(this.CyPhyComponentAuthoringToolGUI_DragDrop);
            this.DragEnter += new System.Windows.Forms.DragEventHandler(this.CyPhyComponentAuthoringToolGUI_DragEnter);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        public System.Windows.Forms.ListView CatModuleListView;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Button RunToolButton;
        private System.Windows.Forms.TextBox ModuleDetailsTextBox;
        public System.Windows.Forms.ImageList ComponentIconList;
        private System.Windows.Forms.TextBox ModuleNameLabel;
    }
}


