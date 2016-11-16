using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace CyPhyMetaLink
{
    public partial class MainForm : Form
    {
        public string AuxiliaryDir { get; set; }

        public MainForm(string workingDir)
        {
            InitializeComponent();

            AuxiliaryDir = "";
            txt_Working.Text = workingDir;
        }

        private void btn_OK_Click(object sender, EventArgs e)
        {
            AuxiliaryDir = txt_Auxiliary.Text;
            DialogResult = System.Windows.Forms.DialogResult.OK;
            Close();
        }

        private void btn_Cancel_Click(object sender, EventArgs e)
        {
            DialogResult = System.Windows.Forms.DialogResult.Cancel;
            Close();
        }

        private void btn_Folder_Click(object sender, EventArgs e)
        {
            string dir = RunFolderDlg();
            txt_Auxiliary.Text = dir;
        }

        private string RunFolderDlg()
        {
            string selectedDir = "";
            string currentDir = Environment.CurrentDirectory;
            var folderDlg = new META.FolderBrowserDialog();
            folderDlg.SelectedPath = currentDir;
            var result = folderDlg.ShowDialog();

            if (result == System.Windows.Forms.DialogResult.OK)
            {
                selectedDir = Path.GetFullPath(folderDlg.SelectedPath);
            }


            return selectedDir;
        }
    }
}
