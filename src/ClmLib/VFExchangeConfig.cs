using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ISIS.Web;

namespace ClmLib
{
    public partial class VFExchangeConfig : Form
    {
        public const string GAMMA_URL = "https://gamma.vehicleforge.org";
        public const string BETA_URL = "https://beta.vehicleforge.org";

        public Credentials Credentials { get; set; }

        public VFExchangeConfig()
        {
            InitializeComponent();

            this.AcceptButton = this.btnSave;

            this.txtUsername.Text = Properties.Settings.Default.VehicleForgeUsername;
            this.mtbPassword.Text = string.Empty;

            this.comboVehicleForgeURL.Items.Add(GAMMA_URL);
            this.comboVehicleForgeURL.Items.Add(BETA_URL);

            if (string.IsNullOrWhiteSpace(Properties.Settings.Default.LastVehicleForgeUrl))
            {
                this.comboVehicleForgeURL.Text = GAMMA_URL;
            }
            else
            {
                this.comboVehicleForgeURL.Text = Properties.Settings.Default.LastVehicleForgeUrl;
            }

            this.Credentials = new Credentials(this.comboVehicleForgeURL.Text, this.txtUsername.Text, this.mtbPassword.Text);
        }

        private void btnTestLink_Click(object sender, EventArgs e)
        {
            try
            {
                // TODO: check the uri is valid
                Uri uri = new Uri(this.comboVehicleForgeURL.Text);
                System.Diagnostics.Process.Start(uri.ToString());
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            try
            {
                Properties.Settings.Default.LastVehicleForgeUrl = this.comboVehicleForgeURL.Text;
                Properties.Settings.Default.VehicleForgeUsername = this.txtUsername.Text;

                this.Credentials.Url = this.comboVehicleForgeURL.Text;
                this.Credentials.Username = this.txtUsername.Text;
                this.Credentials.Password = this.mtbPassword.Text;

                DialogResult = System.Windows.Forms.DialogResult.OK;
                Close();
            }
            catch (Exception ex)
            {
                string message = ex.Message;
                if (ex.InnerException != null)
                    message += ": " + ex.InnerException.Message;
                MessageBox.Show(message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = System.Windows.Forms.DialogResult.Cancel;
            Close();
        }

    }

}
