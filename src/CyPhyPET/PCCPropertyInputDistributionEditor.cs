//

namespace CyPhyPET
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Data;
    using System.Drawing;
    using System.Linq;
    using System.Text;
    using System.Windows.Forms;
    using GME.MGA;
    using GME.MGA.Core;

    using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
    using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
    using CyPhyGUIs;
    using System.Collections;

    public partial class PCCPropertyInputDistributionEditor : Form
    {
        public CyPhyGUIs.SmartLogger Logger { get; set; }
        public CyPhyGUIs.IInterpreterMainParameters MainParameters { get; set; }
        //public static string[] DISTTYPES = new string[4] { "BETA", "LNORM", "NORM", "UNIF"};
        public static Dictionary<string, int> DistToIndex = new Dictionary<string, int>()
        {
            {"None", 0},
            {"BETA", 1},
            {"LNORM", 2},
            {"NORM", 3},
            {"UNIF", 4},
        };

        public static Dictionary<int, string> IndexToDist = new Dictionary<int, string>()
        {
            {0, "None"},
            {1, "BETA"},
            {2, "LNORM"},
            {3, "NORM"},
            {4, "UNIF"},
        };

        private IList PropertyData { get; set; }
        private List<PCC.PCCProperty> DirtyProperties { get; set; }
        private bool OKClicked = false;

        public PCCPropertyInputDistributionEditor(CyPhyGUIs.IInterpreterMainParameters mainParameters, CyPhyGUIs.SmartLogger logger)
        {
            this.Logger = logger;
            this.MainParameters = mainParameters;
            InitializeComponent();
            this.cbDist.SelectedIndex = 0;
            this.DirtyProperties = new List<PCC.PCCProperty>();
            this.PropertyData = ((System.Windows.Forms.BindingSource)dataGridView1.DataSource).List;
            bool hadProperty = false;
            foreach (var property in this.GetPCCPropertiesFromComponent(true).OrderBy(p => p.Name))
            {
                this.PropertyData.Add(property);
                hadProperty = true;
            }

            if (hadProperty == false)
            {
                this.Logger.WriteWarning("No properties can be used as PCC Property Input distributions!");
                this.Logger.WriteWarning("A property can only be used if:");
                this.Logger.WriteWarning(" [1] It maps to a Modelica Parameter of a Modelica Model.");
                this.Logger.WriteWarning(" [2] It does NOT have any incoming value flows.");
                this.Logger.WriteWarning(" [3] It has a real value.");
                this.Close();
            }
        }

        /// <summary>
        /// Returns PCCProperties for all Properties in the component that fullfil the requirements to be a PCCProperty.
        /// If called with doLog and a Property has the registry node "Distribution" it will be added to DirtyProperties if the property
        /// does not fulfill the requirements.
        /// </summary>
        /// <param name="doLog">Indicating if info should be written to log and if DirtyProperties are gathered.</param>
        /// <returns>All properties that fulfill the requirements.</returns>
        private List<PCC.PCCProperty> GetPCCPropertiesFromComponent(bool doLog = false)
        {
            var results = new List<PCC.PCCProperty>();
            var component = CyPhyClasses.Component.Cast(this.MainParameters.CurrentFCO);
            if (doLog)
            {
                this.Logger.WriteInfo("Looking for candidate properties in component..");
            }
            foreach (var property in component.Children.PropertyCollection)
            {
                double value;
                if (property.DstConnections.ModelicaParameterPortMapCollection.Any() == false && doLog)
                {
                    this.Logger.WriteInfo("Not adding {0} to GUI - it does not map to a ModelicaParameter", property.Name);
                    var distValue = (property.Impl as GME.MGA.MgaFCO).RegistryNode["Distribution"].Value;
                    if (distValue != null)
                    {
                        this.Logger.WriteDebug("It had a Distribution registry, will be cleaned up...");
                        this.DirtyProperties.Add(new PCC.PCCProperty(property));
                    }
                }
                else if (property.SrcConnections.ValueFlowCollection.Any() && doLog)
                {
                    this.Logger.WriteInfo("Not adding {0} to GUI - it has an incoming value flow.", property.Name);
                    var distValue = (property.Impl as GME.MGA.MgaFCO).RegistryNode["Distribution"].Value;
                    if (distValue != null)
                    {
                        this.Logger.WriteDebug("It had a Distribution registry, will be cleaned up...");
                        this.DirtyProperties.Add(new PCC.PCCProperty(property));
                    }
                }
                else if (double.TryParse(property.Attributes.Value, out value) == false && doLog)
                {
                    this.Logger.WriteInfo("Not adding {0} to GUI - '{1}' is not a real value.", property.Name, property.Attributes.Value);
                    var distValue = (property.Impl as GME.MGA.MgaFCO).RegistryNode["Distribution"].Value;
                    if (distValue != null)
                    {
                        this.Logger.WriteDebug("It had a Distribution registry, will be cleaned up...");
                        this.DirtyProperties.Add(new PCC.PCCProperty(property));
                    }
                }
                else
                {
                    var pccProp = new PCC.PCCProperty(property);
                    var distValue = (property.Impl as GME.MGA.MgaFCO).RegistryNode["Distribution"].Value;
                    if (distValue != null)
                    {
                        if (doLog)
                        {
                            this.Logger.WriteDebug("Found Distribution registry node for {0}, Value : {1}", property.Name, distValue);
                        }

                        var success = pccProp.UpdateDistributionFromExistingNode(distValue);
                        if (success == false && doLog)
                        {
                            this.Logger.WriteDebug("but was in wrong format. Resetting...");
                        }
                    }

                    results.Add(pccProp);
                }
            }

            return results;
        }

        private void cbDist_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (this.cbDist.SelectedIndex)
            {
                case 0:
                    this.pictureBox1.Image = null;
                    this.lblParam1.Text = "N/A";
                    this.lblParam2.Text = "N/A";
                    this.lblParam3.Text = "N/A";
                    this.lblParam4.Text = "N/A";
                    this.tbParam1.Enabled = false;
                    this.tbParam2.Enabled = false;
                    this.tbParam3.Enabled = false;
                    this.tbParam4.Enabled = false;

                    break;
                case 1:
                    this.pictureBox1.Image = CyPhyPET.Properties.Resources.beta;
                    this.lblParam1.Text = "Low Limit (a):";
                    this.lblParam2.Text = "High Limit (b):";
                    this.lblParam3.Text = "Shape 1 (alpha):";
                    this.lblParam4.Text = "Shape 2 (beta):";
                    this.tbParam1.Enabled = true;
                    this.tbParam2.Enabled = true;
                    this.tbParam3.Enabled = true;
                    this.tbParam4.Enabled = true;
                    break;
                case 2:
                    this.pictureBox1.Image = CyPhyPET.Properties.Resources.lognormal;
                    this.lblParam1.Text = "Shape :";
                    this.lblParam2.Text = "LogScale :";
                    this.lblParam3.Text = "N/A";
                    this.lblParam4.Text = "N/A";
                    this.tbParam1.Enabled = true;
                    this.tbParam2.Enabled = true;
                    this.tbParam3.Enabled = false;
                    this.tbParam4.Enabled = false;
                    break;
                case 3:
                    this.pictureBox1.Image = CyPhyPET.Properties.Resources.normal;
                    this.lblParam1.Text = "Mean :";
                    this.lblParam2.Text = "Standard Deviation :";
                    this.lblParam3.Text = "N/A";
                    this.lblParam4.Text = "N/A";
                    this.tbParam1.Enabled = true;
                    this.tbParam2.Enabled = true;
                    this.tbParam3.Enabled = false;
                    this.tbParam4.Enabled = false;
                    break;
                case 4:
                    this.pictureBox1.Image = CyPhyPET.Properties.Resources.uniform;
                    this.lblParam1.Text = "Low Limit (a):";
                    this.lblParam2.Text = "High Limit (b):";
                    this.lblParam3.Text = "N/A";
                    this.lblParam4.Text = "N/A";
                    this.tbParam1.Enabled = true;
                    this.tbParam2.Enabled = true;
                    this.tbParam3.Enabled = false;
                    this.tbParam4.Enabled = false;
                    break;

            }
            
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Logger.WriteWarning("Form cancelled by user, no updates were saved..");
            this.Close();
        }

        private void btnUpdate_Click(object sender, EventArgs e)
        {
            var row = this.dataGridView1.CurrentRow;
            var pccProperty = row.DataBoundItem as PCC.PCCProperty;
            double param1 = 0.0;
            double param2 = 0.0;
            double param3 = 0.0;
            double param4 = 0.0;
            int distIndex = cbDist.SelectedIndex; 
            if (distIndex > 0)
            {
                try
                {
                    param1 = Convert.ToDouble(this.tbParam1.Text.Trim());
                    param2 = Convert.ToDouble(this.tbParam2.Text.Trim());
                    if (distIndex == 1)
                    {
                        param3 = Convert.ToDouble(this.tbParam3.Text.Trim());
                        param4 = Convert.ToDouble(this.tbParam4.Text.Trim());
                    }
                }
                catch (FormatException err)
                {
                    this.Logger.WriteDebug("User entered invalid param-value, exception handled; {0}", err);
                    MessageBox.Show("Parameter values must be doubles.", "Invalid parameter value",
                                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }

            pccProperty.DistType = IndexToDist[distIndex];
            pccProperty.Param1 = param1;
            pccProperty.Param2 = param2;
            pccProperty.Param3 = param3;
            pccProperty.Param4 = param4;
            this.UpdateRowColor(row);
        }

        private void dataGridView1_SelectionChanged(object sender, EventArgs e)
        {
            var pccProperty = this.dataGridView1.CurrentRow.DataBoundItem as PCC.PCCProperty;
            this.lblName.Text = pccProperty.Name;
            this.cbDist.SelectedIndex = DistToIndex[pccProperty.DistType];
            this.tbParam1.Text = pccProperty.Param1.ToString();
            this.tbParam2.Text = pccProperty.Param2.ToString();
            this.tbParam3.Text = pccProperty.Param3.ToString();
            this.tbParam4.Text = pccProperty.Param4.ToString();
        }

        private void PCCPropertyInputDistributionEditor_Load(object sender, EventArgs e)
        {
            foreach (DataGridViewRow row in this.dataGridView1.Rows)
            {
                this.UpdateRowColor(row);
            }
        }

        /// <summary>
        /// Update the coloring of the row when the PCCProperties are populated or have been updated.
        /// </summary>
        /// <param name="row"></param>
        private void UpdateRowColor(DataGridViewRow row)
        {
            var distType = (row.DataBoundItem as PCC.PCCProperty).DistType;
            switch (distType)
            {
                case "None":
                    row.DefaultCellStyle.ForeColor = Color.Black;
                    row.DefaultCellStyle.SelectionBackColor = Color.LightGray;
                    break;
                case "BETA":
                    row.DefaultCellStyle.ForeColor = Color.Red;
                    row.DefaultCellStyle.SelectionBackColor = Color.Red;
                    break;
                case "LNORM":
                    row.DefaultCellStyle.ForeColor = Color.Chartreuse;
                    row.DefaultCellStyle.SelectionBackColor = Color.Chartreuse;
                    break;
                case "NORM":
                    row.DefaultCellStyle.ForeColor = Color.Blue;
                    row.DefaultCellStyle.SelectionBackColor = Color.Blue;
                    break;
                case "UNIF":
                    row.DefaultCellStyle.ForeColor = Color.Aqua;
                    row.DefaultCellStyle.SelectionBackColor = Color.Aqua;
                    break;
            }
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            this.OKClicked = true;
            this.Logger.WriteInfo("Changes updated in component.");
            this.Close();
        }

        /// <summary>
        /// The writing to the registry takes place only here. If the form was cancelled the orignal setup is kept.
        /// In both cases the registry is cleaned up for properties that no longer fulfill the requirements.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void PCCPropertyInputDistributionEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            int nbrOfPCCProperties = 0;
            if (this.OKClicked)
            {
                foreach (PCC.PCCProperty pccProperty in this.PropertyData)
                {
                    pccProperty.UpdateRegistryNode();
                    if (DistToIndex[pccProperty.DistType] > 0)
                    {
                        nbrOfPCCProperties++;
                    }
                }
            }
            else
            {
                this.Logger.WriteDebug("Go through original config to clean up registry..");
                foreach (var pccProperty in this.GetPCCPropertiesFromComponent(false))
                {
                    pccProperty.UpdateRegistryNode();
                    if (DistToIndex[pccProperty.DistType] > 0)
                    {
                        nbrOfPCCProperties++;
                    }
                }
            }

            // Clean up dirty Properties
            foreach (var dirtyProperty in this.DirtyProperties)
            {
                dirtyProperty.UpdateRegistryNode();
                this.Logger.WriteDebug("Cleaned up dirty node for {0}", dirtyProperty.Name);
            }

            // Add/Remove NbrOfPCCDistributions to Component
            if (nbrOfPCCProperties == 0)
            {
                var dist = this.MainParameters.CurrentFCO.RegistryNode["NbrOfPCCDistributions"];
                if (dist != null)
                {
                    dist.RemoveTree();
                }
            }
            else
            {
                this.MainParameters.CurrentFCO.RegistryNode["NbrOfPCCDistributions"].Value = nbrOfPCCProperties.ToString();
            }
        }
    }
}
