using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace JobManager
{
	public partial class CreateJob : Form
	{
		public CreateJob()
		{
			InitializeComponent();

			cbType.DataSource = Enum.GetValues(typeof(Job.TypeEnum));
			cbType.SelectedItem = Job.TypeEnum.Command;

		}

		private void btnOK_Click(object sender, EventArgs e)
		{
			try
			{
				// validate inputs
				string title = txtTitle.Text;
				string workingDirectory = System.IO.Path.GetFullPath(txtWorkingDir.Text);
				string runCommand = txtRunCommand.Text;
				Job.TypeEnum type = Job.TypeEnum.Command;

				if (Enum.TryParse(cbType.SelectedItem.ToString(), out type) == false)
				{
					MessageBox.Show(
						"Invalid type.",
						"Enum type is invalid",
						MessageBoxButtons.OK,
						MessageBoxIcon.Error);
					return;
				}

				// create new job(s) on the server
				for (int i = 1; i <= nupNumJob.Value; ++i)
				{
					if (nupNumJob.Value > 1)
					{
						title = String.Format("{0} ({1})", txtTitle.Text, i);
					}

                    //TODO: Create new jobs in JobManager
					/*Job j = (Owner as JobManagerForm).server.CreateJob();
					j.RunCommand = runCommand;
					j.Title = title;
					j.WorkingDirectory = workingDirectory;
					j.Type = type;
                    j.Status = Job.StatusEnum.Ready;
					(Owner as JobManagerForm).server.AddJob(j);*/
				}

				DialogResult = System.Windows.Forms.DialogResult.OK;
				Close();
			}
			catch (Exception ex)
			{
				MessageBox.Show(
					ex.Message,
					"Exception",
					MessageBoxButtons.OK,
					MessageBoxIcon.Error);
			}

		}
	}
}
