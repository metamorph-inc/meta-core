using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CyPhyReliabilityAnalysis
{
    public partial class MainForm : Form
    {
        private CyPhyReliabilityAnalysisSettings settings;
        private double angularMin;
        private double angularMax;
        private double gussetMin;
        private double gussetMax;
        private double angularThickness;
        private double gussetThickness;

        public MainForm(CyPhyReliabilityAnalysisSettings oldSettings)
        {
            InitializeComponent();
            this.settings = oldSettings;
            this.angularThickness = this.settings.AngularThickness;
            this.gussetThickness = this.settings.GussetThickness;
            if (this.settings.Material == "Steel")
            {
                this.cbMaterial.SelectedIndex = 0;
            }
            else
            {
                this.cbMaterial.SelectedIndex = 1;
            }

            this.cbSize.SelectedIndex = this.settings.Size - 1;
        }

        private bool SaveSettings()
        {
            if (this.angularThickness < this.angularMin || this.angularThickness > this.angularMax ||
                this.gussetThickness < this.gussetMin || this.gussetThickness > this.gussetMax)
            {
                MessageBox.Show("Entered thicknesses are not within given ranges!", "Invalid values", MessageBoxButtons.OK);
                return false;
            }

            this.settings.AngularThickness = this.angularThickness;
            this.settings.GussetThickness = this.gussetThickness;
            this.settings.Material = this.cbMaterial.SelectedItem.ToString();
            this.settings.Size = this.cbSize.SelectedIndex + 1;

            return true;
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = System.Windows.Forms.DialogResult.Cancel;
            Close();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (SaveSettings())
            {
                DialogResult = System.Windows.Forms.DialogResult.OK;
                Close();
            }
        }

        private void tbAngular_TextChanged(object sender, EventArgs e)
        {
            double value;
            if (double.TryParse(this.tbAngular.Text, out value))
            {
                this.angularThickness = value;
            }
            else
            {
                this.angularThickness = -1;
            }

            if (this.angularThickness < this.angularMin || this.angularThickness > this.angularMax)
            {
                this.lblAngular.ForeColor = Color.Red;
                this.lblAngular.Font = new Font(this.lblAngular.Font, FontStyle.Bold);
            }
            else
            {
                this.lblAngular.ForeColor = Color.Black;
                this.lblAngular.Font = new Font(this.lblAngular.Font, FontStyle.Regular);
            }
        }

        private void tbGusset_TextChanged(object sender, EventArgs e)
        {
            double value;
            if (double.TryParse(this.tbGusset.Text, out value))
            {
                this.gussetThickness = value;
            }
            else
            {
                this.gussetThickness = -1;
            }

            if (this.gussetThickness < this.gussetMin || this.gussetThickness > this.gussetMax)
            {
                this.lblGusset.ForeColor = Color.Red;
                this.lblGusset.Font = new Font(this.lblGusset.Font, FontStyle.Bold);
            }
            else
            {
                this.lblGusset.ForeColor = Color.Black;
                this.lblGusset.Font = new Font(this.lblGusset.Font, FontStyle.Regular);
            }
        }

        private void cbMaterial_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.cbMaterial.SelectedIndex == 0)
            {
                // Steel
                this.angularMin = 10.16;
                this.angularMax = 20.32;
                this.gussetMin = 3.81;
                this.gussetMax = 8.89;
            }
            else
            {
                // Aluminium
                this.angularMin = 17.78;
                this.angularMax = 22.86;
                this.gussetMin = 12.7;
                this.gussetMax = 17.78;
            }

            if (this.angularThickness < this.angularMin || this.angularThickness > this.angularMax)
            {
                this.angularThickness = this.angularMin;
            }

            if (this.gussetThickness < this.gussetMin || this.gussetThickness > this.gussetMax)
            {
                this.gussetThickness = this.gussetMin;
            }

            this.lblAngular.Text = string.Format("min={0}, max={1}", this.angularMin, this.angularMax);
            this.lblAngular.ForeColor = Color.Black;
            this.lblAngular.Font = new Font(this.lblAngular.Font, FontStyle.Regular);

            this.lblGusset.Text = string.Format("min={0}, max={1}", this.gussetMin, this.gussetMax);
            this.lblGusset.ForeColor = Color.Black;
            this.lblGusset.Font = new Font(this.lblGusset.Font, FontStyle.Regular);

            this.tbAngular.Text = this.angularThickness.ToString();
            this.tbGusset.Text = this.gussetThickness.ToString();
        }
    }
}
