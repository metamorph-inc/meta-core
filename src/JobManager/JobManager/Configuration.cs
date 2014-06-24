using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Diagnostics;

namespace JobManager
{
    public partial class Configuration : Form
    {
        public Configuration(string password = null)
        {
            //this.jenkins = jenkins;
            InitializeComponent();
            AcceptButton = btnSave;
            chbRemoteExec.CheckedChanged +=new EventHandler(delegate (object o, EventArgs args) {
                panelRemote.Enabled = chbRemoteExec.Checked;
            });
            panelRemote.Enabled = chbRemoteExec.Checked;


            this.txtUsername.Text = Properties.Settings.Default.UserID;
            
            if (string.IsNullOrEmpty(password) == false)
            {
                // auto-configure
                this.mtbPassword.Text = password;
                Properties.Settings.Default.RemoteExecution = true;
            }

            comboVehicleForgeURL.Items.Add("https://gamma.vehicleforge.org");
            comboVehicleForgeURL.Items.Add("https://beta.vehicleforge.org");
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (keyData == Keys.Escape)
                this.Close();
            return base.ProcessCmdKey(ref msg, keyData);
        }

        private bool cancel_Clicked = false;
        void linkCancelCheck_Click(object sender, System.EventArgs e)
        {
            lock (this)
            {
                cancel_Clicked = true;
            }
        }

        internal void btnSave_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.UserID = this.txtUsername.Text;
            //jenkins.Username = this.txtUsername.Text;
            //jenkins.Password = this.mtbPassword.Text;

            if (!chbRemoteExec.Checked)
            {
                Properties.Settings.Default.Save();
                DialogResult = System.Windows.Forms.DialogResult.OK;
                Close();
                return;
            }
            else
            {
                // TODO: try to login with the u/p?
            }

            try
            {
                Uri jenkinsUri = new Uri(comboVehicleForgeURL.Text);

                while (Properties.Settings.Default.VehicleForgeUri.EndsWith("/"))
                    Properties.Settings.Default.VehicleForgeUri = Properties.Settings.Default.VehicleForgeUri.Substring(0, Properties.Settings.Default.VehicleForgeUri.Length - 1);

                btnSave.Enabled = false;
                pictureBoxLoading.Visible = true;
                labelJenkinsTest.Visible = true;
                linkCancelCheck.Visible = true;
                cancel_Clicked = false;
                //jenkins.Username = txtUsername.Text;
                //jenkins.Password = mtbPassword.Text;
                //Action userCreateDelegate = delegate() { jenkins.Login(); };
                IAsyncResult userCreateResult = null;
                //userCreateResult = userCreateDelegate.BeginInvoke(null, null);
                while (true)
                {
                    Application.DoEvents();
                    lock (this)
                    {
                        if (cancel_Clicked)
                            break;
                        if (userCreateResult != null && userCreateResult.AsyncWaitHandle.WaitOne(50))
                            break;
                    }
                }
                pictureBoxLoading.Visible = false;
                labelJenkinsTest.Visible = false;
                linkCancelCheck.Visible = false;
                btnSave.Enabled = true;
                if (cancel_Clicked)
                {
                    return;
                }
                //if (userCreateResult != null)
                //    userCreateDelegate.EndInvoke(userCreateResult);

                Properties.Settings.Default.Save();

                JobManagerForm managerForm = (Owner as JobManagerForm);
                if (managerForm != null)
                {
                    //Dictionary<Job.TypeEnum, JobManagerForm.TargetMachine> config = new Dictionary<Job.TypeEnum, JobManagerForm.TargetMachine>();
                    //JobManagerForm.TargetMachine.TargetMachineType type = JobManagerForm.TargetMachine.TargetMachineType.Local;
                    //if (cmd.Host.Equals("localhost"))
                    //{
                    //    type = JobManagerForm.TargetMachine.TargetMachineType.Local;
                    //}
                    //else
                    //{
                    //    type = JobManagerForm.TargetMachine.TargetMachineType.Remote;
                    //}
                    //config.Add(Job.TypeEnum.Command, new JobManagerForm.TargetMachine(cmd, type));

                    //if (matlab.Host.Equals("localhost"))
                    //{
                    //    type = JobManagerForm.TargetMachine.TargetMachineType.Local;
                    //}
                    //else
                    //{
                    //    type = JobManagerForm.TargetMachine.TargetMachineType.Remote;
                    //}
                    //config.Add(Job.TypeEnum.Matlab, new JobManagerForm.TargetMachine(matlab, type));

                    //if (cad.Host.Equals("localhost"))
                    //{
                    //    type = JobManagerForm.TargetMachine.TargetMachineType.Local;
                    //}
                    //else
                    //{
                    //    type = JobManagerForm.TargetMachine.TargetMachineType.Remote;
                    //}
                    //config.Add(Job.TypeEnum.CAD, new JobManagerForm.TargetMachine(cad, type));

                    //managerForm.UpdateRuntimeConfig(config);
                }

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

        private void btnOpenLink_Click(object sender, EventArgs e)
        {
            try
            {
                // TODO: check the uri is valid
                Uri uri = new Uri(comboVehicleForgeURL.Text);
                System.Diagnostics.Process.Start(uri.ToString());
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
