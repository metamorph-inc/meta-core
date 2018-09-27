using System;
using System.Windows.Forms;

namespace CyPhyComponentAuthoring.GUIs
{
    public partial class CADProgress : Form
    {
        public CADProgress()
        {
            InitializeComponent();

            DialogResult = DialogResult.OK;

            if (Application.RenderWithVisualStyles)
            {
                progressBar.Style = ProgressBarStyle.Marquee;
            }
            else
            {
                progressBar.Style = ProgressBarStyle.Continuous;
                progressBar.Maximum = 100;
                progressBar.Value = 0;
                timer.Enabled = true;
                timer.Tick += timer_Tick;
            }
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            if (progressBar.Value < progressBar.Maximum)
            {
                progressBar.Increment(5);
            }
            else
            {
                progressBar.Value = progressBar.Minimum;
            }
        }
    }
}
