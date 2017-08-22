using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CyPhyPET
{
    public partial class EditDialog : Form
    {
        public EditDialog()
        {
            InitializeComponent();

            this.buttonOK.Click += ButtonOK_Click;
            this.linkLabelCancel.Click += LinkLabelCancel_Click;
        }

        private void LinkLabelCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void ButtonOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }
    }
}
