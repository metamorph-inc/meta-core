using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace CyPhyPET
{
    public partial class PCCexperimentSettings : Form
    {
        public PCCexperimentSettings()
        {
            InitializeComponent();

            txtPCCCorePath.Text = CyPhyPET.Properties.Settings.Default.PCCCorePath;
        }

        private void btnBrowsePCCCorePath_Click(object sender, EventArgs e)
        {
            string selectedPath = Directory.Exists(txtPCCCorePath.Text) ?
                Path.GetFullPath(txtPCCCorePath.Text) :
                "";

            using (META.FolderBrowserDialog fbd = new META.FolderBrowserDialog()
            {
                SelectedPath = selectedPath,
                //ShowNewFolderButton = false,
            })
            {


                var dr = fbd.ShowDialog();

                if (dr == System.Windows.Forms.DialogResult.OK)
                {
                    txtPCCCorePath.Text = fbd.SelectedPath;
                }
            }
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            CyPhyPET.Properties.Settings.Default.PCCCorePath = txtPCCCorePath.Text;

            CyPhyPET.Properties.Settings.Default.Save();

            this.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.Close();
        }
    }
}
