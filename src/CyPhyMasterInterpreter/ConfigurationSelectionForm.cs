using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Xml.Serialization;
using System.Xml;
using GME.MGA;
using Microsoft.Win32;
using System.Diagnostics;
using GME.Util;
using System.Collections;
using META;
using System.Runtime.InteropServices;

namespace CyPhyMasterInterpreter
{
    public partial class ConfigurationSelectionForm : Form
    {
        public ConfigurationSelectionOutput ConfigurationSelectionResult { get; set; }
        private ConfigurationSelectionInput m_Input { get; set; }

        public ConfigurationSelectionForm(ConfigurationSelectionInput input, bool enableDebugging)
        {
            this.EnableDebugging = enableDebugging;
            this.InitializeComponent();

            // verify all properties in input
            if (input == null ||
                input.Context == null ||
                input.Groups == null ||
                input.InterpreterNames == null)
            {
                throw new ArgumentNullException();
            }

            this.m_Input = input;

            this.ConfigurationSelectionResult = new ConfigurationSelectionOutput();

            this.chbPostJobs.Checked = Properties.Settings.Default.bPostTojobManager;
            this.chbVerbose.Checked = Properties.Settings.Default.bVerboseLogging;

            this.InitForm();
        }

        private void CommandLinkRunParallel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Yes;
            this.Close();
        }

        public void InitForm()
        {
            this.txtOutputDir.Text = this.m_Input.OutputDirectory;
            this.tsslStatus.Text = this.m_Input.OperationModeInformation;

            this.lbExportedCAs.Items.Clear();
            this.lbConfigModels.Items.Clear();

            if (this.m_Input.IsDesignSpace)
            {
                // design space case
                this.chbShowDirty.Enabled = true;
                this.lbConfigModels.Enabled = true;

                foreach (var group in this.m_Input.Groups)
                {
                    if (group.IsDirty == false)
                    {
                        this.lbConfigModels.Items.Add(group);
                    }
                }
            }
            else
            {
                // component assembly case
                this.chbShowDirty.Enabled = false;
                this.lbConfigModels.Enabled = false;
                this.lbConfigModels.BackColor = Color.LightGray;
                this.commandLinkRunParallel.Enabled = false;

                foreach (var config in this.m_Input.Groups.SelectMany(x => x.Configurations))
                {
                    this.AddExportedCAItemQuadratic(config);
                }
            }


            if (this.lbConfigModels.Items.Count > 0)
            {
                this.lbConfigModels.SelectedItem = this.lbConfigModels.Items[0];
                this.lbExportedCAs.SelectedItem = this.lbExportedCAs.Items.Cast<object>().FirstOrDefault();
            }
            else if (lbExportedCAs.Items.Count > 0)
            {
                this.lbExportedCAs.SelectedItem = this.lbExportedCAs.Items.Cast<object>().FirstOrDefault();
            }

            foreach (var item in this.m_Input.InterpreterNames)
            {
                this.lbWorkFlow.Items.Add(item);
            }

            this.lbExportedCAs.Refresh();
            this.lbConfigModels.Refresh();

            this.chbSaveTestBenches.Visible = EnableDebugging;
        }


        private void btnOK_Click(object sender, EventArgs e)
        {
            this.SaveSettingsAndResults();

            // if this is false the form will not be closed
            bool canClose = true;

            if (this.ConfigurationSelectionResult.SelectedConfigurations.Any())
            {
                // if at least one config is selected we can close with ok.
                canClose = true;
            }
            else
            {
                // if no configs are selected user cannot close the dialog box.
                canClose = false;

                MessageBox.Show("Please select at least one configuration from configuration list or hit cancel",
                    "No configuration selected", MessageBoxButtons.OK);
            }

            if (canClose)
            {
                // assume everything is ok
                this.DialogResult = System.Windows.Forms.DialogResult.OK;
                this.Close();
            }
        }

        /// <summary>
        /// For non interactive support, this function will show no UI components.
        /// </summary>
        public void SaveSettingsAndResults()
        {
            // save settings
            Properties.Settings.Default.bPostTojobManager = this.chbPostJobs.Checked;
            Properties.Settings.Default.bShowDirty = this.chbShowDirty.Checked;
            Properties.Settings.Default.bVerboseLogging = this.chbVerbose.Checked;
            Properties.Settings.Default.Save();

            // prepare our results
            this.ConfigurationSelectionResult.KeepTemporaryModels = this.chbSaveTestBenches.Checked;
            this.ConfigurationSelectionResult.PostToJobManager = this.chbPostJobs.Checked;
            this.ConfigurationSelectionResult.VerboseLogging = this.chbVerbose.Checked;
            this.ConfigurationSelectionResult.SelectedConfigurations = this.lbExportedCAs.SelectedItems.Cast<GMELightObject>().ToArray();
            var selected = new HashSet<GMELightObject>(this.lbExportedCAs.SelectedItems.Cast<GMELightObject>());
            this.ConfigurationSelectionResult.UnselectedConfigurations = this.lbExportedCAs.Items.Cast<GMELightObject>().Where(o => selected.Contains(o) == false);
            this.ConfigurationSelectionResult.ConfigurationGroups = this.lbConfigModels.SelectedItems.Cast<ConfigurationGroupLight>().ToList();
        }


        [DllImport("shlwapi.dll", CharSet = CharSet.Unicode, ExactSpelling = true)]
        static extern int StrCmpLogicalW(String x, String y);

        /// <summary>
        /// Inserts the configurations in alphabetical order and respects the ordering if the name has numbers.
        /// </summary>
        /// <param name="config">Configuration to insert to configuration list.</param>
        private void AddExportedCAItemQuadratic(GMELightObject config)
        {
            int i = 0;
            for (; i < this.lbExportedCAs.Items.Count; i++)
            {
                if (StrCmpLogicalW(((GMELightObject)this.lbExportedCAs.Items[i]).Name, config.Name) != -1)
                {
                    break;
                }
            }
            this.lbExportedCAs.Items.Insert(i, config);
        }

        public class StrCmpLogicalCompararer : IComparer<String>
        {
            public int Compare(string x, string y)
            {
                return StrCmpLogicalW(x, y);
            }
        }

        private void lbConfigModels_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.lbExportedCAs.Items.Clear();
            int i = 0;
            foreach (var config in lbConfigModels.SelectedItems.Cast<ConfigurationGroupLight>().SelectMany(group => group.Configurations)
                .OrderBy(fco => fco.Name, new StrCmpLogicalCompararer()))
            {
                this.lbExportedCAs.Items.Insert(i++, config);
            }

            this.lbExportedCAs.Refresh();
            this.lblCASelected.Text = this.lbExportedCAs.SelectedItems.Count + " / " + this.lbExportedCAs.Items.Count;
        }

        private int LastTipIdx { get; set; }
        public bool EnableDebugging { get; internal set; }

        private void lbExportedCAs_MouseMove(object sender, MouseEventArgs e)
        {
            string newToolTipText = string.Empty;
            int nIdx = this.lbExportedCAs.IndexFromPoint(e.Location);

            if (nIdx >= 0 &&
                nIdx < this.lbExportedCAs.Items.Count)
            {
                newToolTipText = (this.lbExportedCAs.Items[nIdx] as GMELightObject).ToolTip;
            }

            if (LastTipIdx != nIdx)
            {
                // refresh only if the mouse is over a new element
                this.toolTipCA.SetToolTip(lbExportedCAs, newToolTipText);
                this.LastTipIdx = nIdx;
            }
        }

        private void btnClearAll_Click(object sender, EventArgs e)
        {
            this.lbExportedCAs.SelectedItems.Clear();
        }

        private void btnSelectAll_Click(object sender, EventArgs e)
        {
            this.SelectAll(lbExportedCAs);
        }

        private void SelectAll(ListBox listBox)
        {
            for (int i = 0; i < listBox.Items.Count; ++i)
            {
                listBox.SetSelected(i, true);
            }
        }

        private void lbExportedCAs_KeyDown(object sender, KeyEventArgs e)
        {
            //will be true if Ctrl + A is pressed, false otherwise
            bool ctrlA;

            ctrlA = ((e.KeyCode == Keys.A) &&              // test for A pressed
                    ((e.Modifiers & Keys.Control) != 0));  // test for Ctrl modifier

            if (ctrlA)
            {
                this.SelectAll(lbExportedCAs);
            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.Close();
        }

        private void chbShowDirty_CheckedChanged(object sender, EventArgs e)
        {
            this.lbConfigModels.Items.Clear();

            // TODO: collapse this to one foreach
            if (this.chbShowDirty.Checked)
            {
                foreach (var group in this.m_Input.Groups)
                {
                    this.lbConfigModels.Items.Add(group);
                }
            }
            else
            {
                foreach (var group in this.m_Input.Groups)
                {
                    if (group.IsDirty == false)
                    {
                        this.lbConfigModels.Items.Add(group);
                    }
                }
            }

            if (this.m_Input.IsDesignSpace)
            {
                this.lbExportedCAs.Items.Clear();
                this.lblCASelected.Text = "0 / 0";
            }

            if (this.lbConfigModels.Items.Count == 1)
            {
                this.lbConfigModels.SelectedItem = this.lbConfigModels.Items[0];
            }
        }

        private void lbExportedCAs_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.lblCASelected.Text = this.lbExportedCAs.SelectedItems.Count + " / " + this.lbExportedCAs.Items.Count;
        }
    }
}
