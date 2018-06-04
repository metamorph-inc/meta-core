using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Xml;
using System.Xml.Serialization;

namespace CyPhy2CAD_CSharp
{
    public partial class MainForm : Form
    {
        public const string ConfigFilename = "CyPhy2CAD_config.xml";
        //public CyPhy2CAD_UILib.CADOptions CadOptionsHolder { get; set; }
        public CyPhy2CADSettings ConfigOptions { get; set; }
        public string OutputDir { get; set; }


        public MainForm(CyPhy2CADSettings settings,
                        bool automated = false,
                        bool prepareifab = false)
        {
            InitializeComponent();

            ConfigOptions = settings;


            // Enable/Disable controls
            // [1] automated --> outputdir textbox readonly, outputdir button disabled
            // [2] prepareifab --> step formats readonly
            if (automated)
            {
                textBox_OutputDir.ReadOnly = true;
                button_OutputDir.Enabled = false;
            }
            else
            {
                textBox_OutputDir.Text = ConfigOptions == null ? "" : ConfigOptions.OutputDirectory;
            }

            textBox_AuxiliaryDir.Text = ConfigOptions.AuxiliaryDirectory;

            // PrepareIFab: 1 step formats selected
            // Other: config file
            if (prepareifab)
            {
                SetCheckListBoxItem("AP203_E2_Separate_Part_Files");
                clb_Step.Enabled = false;
            }
            else
            {
                for (int i = 0; i < ConfigOptions.StepFormats.Count; i++)
                {
                    SetCheckListBoxItem(ConfigOptions.StepFormats[i]);
                }
                if (ConfigOptions.wasDeserialized == false)
                {
                    SetCheckListBoxItem("AP203_E2_Single_File");
                    SetCheckListBoxItem("AP203_E2_Separate_Part_Files");
                }
            }
        }

        private void button_OutputDir_Click(object sender, EventArgs e)
        {
            // show folder list
            string outputDir = RunFolderDlg();
            if (outputDir != null)
            {
                textBox_OutputDir.Text = outputDir;
            }
        }

        private void button_AuxiliaryDir_Click(object sender, EventArgs e)
        {
            // show folder list
            string auxDir = RunFolderDlg();
            if (auxDir != null)
            {
                textBox_AuxiliaryDir.Text = auxDir;
            }
        }

        private void button_Ok_Click(object sender, EventArgs e)
        {
            // save settings to config file
            SaveSettings();
            DialogResult = System.Windows.Forms.DialogResult.OK;
            Close();
        }

        private void button_Cancel_Click(object sender, EventArgs e)
        {
            DialogResult = System.Windows.Forms.DialogResult.Cancel;
            Close();
        }

        private void SaveSettings()
        {
             ConfigOptions.AuxiliaryDirectory= textBox_AuxiliaryDir.Text;
             OutputDir = textBox_OutputDir.Text;
             ConfigOptions.OutputDirectory = OutputDir;

            if (clb_Step.CheckedItems.Count > 0)
            {
                ConfigOptions.StepFormats = clb_Step.CheckedItems.Cast<string>().ToList();
            }
            else
            {
                ConfigOptions.StepFormats.Clear();
            }
        }

        /*
        private void SaveOptionsToFile()
        {
            if (Directory.Exists(ProjectPath))
            {
                CadOptionsHolder.AuxiliaryDirectory = textBox_AuxiliaryDir.Text;
                CadOptionsHolder.OutputDirectory = textBox_OutputDir.Text;
                CadOptionsHolder.UseProjectManifest = checkBox_ProjectManifest.Checked;

                if (clb_Step.CheckedItems.Count > 0)
                {
                    CadOptionsHolder.StepFormats = clb_Step.CheckedItems.Cast<string>().ToList();
                }
                else
                {
                    CadOptionsHolder.StepFormats.Clear();
                }

                var settingsFilename = Path.Combine(ProjectPath, ConfigFilename);
                XmlSerializer xs = new XmlSerializer(typeof(CyPhy2CAD_UILib.CADOptions));
                using (XmlWriter writer = XmlWriter.Create(settingsFilename))
                {
                    xs.Serialize(writer, CadOptionsHolder);
                }
            }
        }


        private void GetOptionsFromFile()
        {
            string filename = Path.Combine(ProjectPath, ConfigFilename);
            if (File.Exists(filename))
            {
                XmlSerializer xs = new XmlSerializer(typeof(CyPhy2CAD_UILib.CADOptions));
                using (XmlReader reader = XmlReader.Create(filename))
                {
                    CadOptionsHolder = xs.Deserialize(reader) as CyPhy2CAD_UILib.CADOptions;
                }
            }
            else       // use default
            {
                CadOptionsHolder.AuxiliaryDirectory = "";
                CadOptionsHolder.OutputDirectory = "";
                CadOptionsHolder.UseProjectManifest = true;
                CadOptionsHolder.StepFormats.Add("AP203_E2_Separate_Part_Files");
                CadOptionsHolder.StepFormats.Add("AP203_E2_Single_File");
            }
        }        
        */

        private void SetCheckListBoxItem(string tokens)
        {
            int index = clb_Step.Items.IndexOf(tokens);
            if (index > -1)
                clb_Step.SetItemChecked(index, true);
        }

        private string RunFolderDlg()
        {
            string currentDir = Environment.CurrentDirectory;
            var folderDlg = new META.FolderBrowserDialog();
            folderDlg.SelectedPath = currentDir;
            var result = folderDlg.ShowDialog();

            if (result == System.Windows.Forms.DialogResult.OK)
            {
                return Path.GetFullPath(folderDlg.SelectedPath);
            }
            return null;
        }


    }  // end class

}  // end namespace
