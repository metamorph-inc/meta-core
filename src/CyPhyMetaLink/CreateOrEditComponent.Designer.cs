namespace CyPhyMetaLink
{
    partial class CreateOrEditComponent
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
            this.btnCreateNewComponent = new CyPhyMetaLink.CommandLink();
            this.btnEditExistingComponent = new CyPhyMetaLink.CommandLink();
            this.btnEmptyCreoOpen = new CyPhyMetaLink.CommandLink();
            this.SuspendLayout();
            // 
            // btnCreateNewComponent
            // 
            this.btnCreateNewComponent.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnCreateNewComponent.Location = new System.Drawing.Point(12, 68);
            this.btnCreateNewComponent.Name = "btnCreateNewComponent";
            this.btnCreateNewComponent.Size = new System.Drawing.Size(301, 48);
            this.btnCreateNewComponent.TabIndex = 2;
            this.btnCreateNewComponent.Text = "Copy component and open in Creo";
            this.btnCreateNewComponent.UseVisualStyleBackColor = true;
            // 
            // btnEditExistingComponent
            // 
            this.btnEditExistingComponent.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnEditExistingComponent.Location = new System.Drawing.Point(12, 12);
            this.btnEditExistingComponent.Name = "btnEditExistingComponent";
            this.btnEditExistingComponent.Size = new System.Drawing.Size(301, 48);
            this.btnEditExistingComponent.TabIndex = 1;
            this.btnEditExistingComponent.Text = "Open existing component in Creo";
            this.btnEditExistingComponent.UseVisualStyleBackColor = true;
            // 
            // btnEmptyCreoOpen
            // 
            this.btnEmptyCreoOpen.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.btnEmptyCreoOpen.Location = new System.Drawing.Point(12, 122);
            this.btnEmptyCreoOpen.Name = "btnEmptyCreoOpen";
            this.btnEmptyCreoOpen.Size = new System.Drawing.Size(301, 48);
            this.btnEmptyCreoOpen.TabIndex = 3;
            this.btnEmptyCreoOpen.Text = "Open Creo without a component";
            this.btnEmptyCreoOpen.UseVisualStyleBackColor = true;
            // 
            // CreateOrEditComponent
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(329, 186);
            this.Controls.Add(this.btnEmptyCreoOpen);
            this.Controls.Add(this.btnEditExistingComponent);
            this.Controls.Add(this.btnCreateNewComponent);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "CreateOrEditComponent";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "MetaLink Component Link";
            this.ResumeLayout(false);

        }

        #endregion

        public CommandLink btnCreateNewComponent;
        public CommandLink btnEditExistingComponent;
        public CommandLink btnEmptyCreoOpen;
    }
}