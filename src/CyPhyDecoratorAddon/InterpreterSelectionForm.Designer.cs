namespace CyPhyDecoratorAddon
{
	partial class InterpreterSelectionForm
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
            this.lbInterpreters = new System.Windows.Forms.ListBox();
            this.btnOK = new System.Windows.Forms.Button();
            this.lblSelectInterpreter = new System.Windows.Forms.Label();
            this.chbAutoConnect = new System.Windows.Forms.CheckBox();
            this.lbTasks = new System.Windows.Forms.ListBox();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // lbInterpreters
            // 
            this.lbInterpreters.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lbInterpreters.FormattingEnabled = true;
            this.lbInterpreters.Location = new System.Drawing.Point(12, 23);
            this.lbInterpreters.Name = "lbInterpreters";
            this.lbInterpreters.Size = new System.Drawing.Size(416, 134);
            this.lbInterpreters.TabIndex = 9;
            // 
            // btnOK
            // 
            this.btnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnOK.Location = new System.Drawing.Point(356, 279);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(75, 23);
            this.btnOK.TabIndex = 19;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // lblSelectInterpreter
            // 
            this.lblSelectInterpreter.AutoSize = true;
            this.lblSelectInterpreter.Location = new System.Drawing.Point(13, 4);
            this.lblSelectInterpreter.Name = "lblSelectInterpreter";
            this.lblSelectInterpreter.Size = new System.Drawing.Size(161, 13);
            this.lblSelectInterpreter.TabIndex = 11;
            this.lblSelectInterpreter.Text = "Please select the interpreter type";
            // 
            // chbAutoConnect
            // 
            this.chbAutoConnect.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.chbAutoConnect.AutoSize = true;
            this.chbAutoConnect.Checked = true;
            this.chbAutoConnect.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chbAutoConnect.Location = new System.Drawing.Point(12, 265);
            this.chbAutoConnect.Name = "chbAutoConnect";
            this.chbAutoConnect.Size = new System.Drawing.Size(163, 17);
            this.chbAutoConnect.TabIndex = 14;
            this.chbAutoConnect.Text = "Add to the selected workflow";
            this.chbAutoConnect.UseVisualStyleBackColor = true;
            // 
            // lbTasks
            // 
            this.lbTasks.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lbTasks.FormattingEnabled = true;
            this.lbTasks.Location = new System.Drawing.Point(12, 190);
            this.lbTasks.Name = "lbTasks";
            this.lbTasks.Size = new System.Drawing.Size(415, 69);
            this.lbTasks.TabIndex = 13;
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(11, 172);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(133, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Please select the workflow";
            // 
            // InterpreterSelectionForm
            // 
            this.AcceptButton = this.btnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(443, 314);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.lbTasks);
            this.Controls.Add(this.chbAutoConnect);
            this.Controls.Add(this.lblSelectInterpreter);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.lbInterpreters);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Name = "InterpreterSelectionForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Interpreter Selection";
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button btnOK;
		internal System.Windows.Forms.ListBox lbInterpreters;
		internal System.Windows.Forms.Label lblSelectInterpreter;
		internal System.Windows.Forms.CheckBox chbAutoConnect;
        internal System.Windows.Forms.ListBox lbTasks;
        internal System.Windows.Forms.Label label1;
	}
}