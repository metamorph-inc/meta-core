namespace CyPhyComponentImporter
{
    partial class UpdatePrompt
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
            this.DialogText = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.applyToAll = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // DialogText
            // 
            this.DialogText.Location = new System.Drawing.Point(13, 13);
            this.DialogText.Name = "DialogText";
            this.DialogText.Size = new System.Drawing.Size(354, 171);
            this.DialogText.TabIndex = 0;
            this.DialogText.Text = "label1";
            this.DialogText.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // button1
            // 
            this.button1.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.button1.Location = new System.Drawing.Point(200, 187);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "Replace";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // button3
            // 
            this.button3.DialogResult = System.Windows.Forms.DialogResult.Abort;
            this.button3.Location = new System.Drawing.Point(281, 187);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(86, 23);
            this.button3.TabIndex = 1;
            this.button3.Text = "Import as New";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // applyToAll
            // 
            this.applyToAll.AutoSize = true;
            this.applyToAll.Location = new System.Drawing.Point(112, 191);
            this.applyToAll.Name = "applyToAll";
            this.applyToAll.Size = new System.Drawing.Size(82, 17);
            this.applyToAll.TabIndex = 2;
            this.applyToAll.Text = "Apply To All";
            this.applyToAll.UseVisualStyleBackColor = true;
            // 
            // UpdatePrompt
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(379, 222);
            this.Controls.Add(this.applyToAll);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.DialogText);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "UpdatePrompt";
            this.Text = "UpdatePrompt";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button3;
        public System.Windows.Forms.Label DialogText;
        public System.Windows.Forms.CheckBox applyToAll;

    }
}