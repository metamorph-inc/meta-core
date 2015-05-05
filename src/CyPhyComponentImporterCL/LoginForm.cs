using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CyPhyComponentImporterCL {
    public partial class LoginForm : Form {
        public LoginForm() {
            InitializeComponent();

//            this.txtUsername.Text = CyPhyComponentImporterCL.Properties.Settings.Default.Username;
        }

        private void btnOK_Click(object sender, EventArgs e) {
//            CyPhyComponentImporterCL.Properties.Settings.Default.Username = this.txtUsername.Text;
//            CyPhyComponentImporterCL.Properties.Settings.Default.Save();

            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void btnCancel_Click(object sender, EventArgs e) {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}
