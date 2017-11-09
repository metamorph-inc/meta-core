using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace CyPhyMasterInterpreter
{
    public partial class ProgressDialog : Form
    {
        [DllImport("user32.dll")]
        public static extern bool EnableWindow(IntPtr hwnd, bool bEnable);

        public ProgressDialog(CyPhyMasterInterpreterAPI masterInterpreter)
        {
            InitializeComponent();

            this.MasterInterpreter = masterInterpreter;

            this.m_stopwatch = new System.Diagnostics.Stopwatch();

            // Set default position of the dialog box to not overlay the console.
            var workingArea = Screen.GetWorkingArea(this);
            this.Top = 200;
        }

        private System.Diagnostics.Stopwatch m_stopwatch { get; set; }

        private IntPtr m_mainWindow = System.Diagnostics.Process.GetCurrentProcess().MainWindowHandle;

        public void ShowWithDisabledMainWindow()
        {
            EnableWindow(m_mainWindow, false);
            this.Show();
            this.m_RaiseExceptionOnClose = true;
            this.m_FormClosed = false;
            this.m_UserCancelled = true;
        }

        protected override void DestroyHandle()
        {
            EnableWindow(m_mainWindow, true);
            base.DestroyHandle();
        }

        private bool m_RaiseExceptionOnClose { get; set; }
        private bool m_FormClosed { get; set; }
        private bool m_UserCancelled { get; set; }

        public CyPhyMasterInterpreterAPI MasterInterpreter { get; set; }

        public void MultipleConfigurationProgressHandler(ProgressCallbackEventArgs progressArgs)
        {
            if (this.m_FormClosed)
            {
                // if form is closed we have no access to controls
                this.MasterInterpreter.Cancel();
                return;
            }

            this.m_stopwatch.Start();

            this.progressBarMultiple.Value = Math.Min((int)progressArgs.Percent, 100);

            if (progressArgs.Configuration == null)
            {
                this.lblMultiple.Text = string.Format("{0} - {1}", progressArgs.Context, progressArgs.Title);
            }
            else
            {
                this.lblMultiple.Text = string.Format("{0} - [{1}] - {2}", progressArgs.Context, progressArgs.Configuration, progressArgs.Title);
            }

            if (progressArgs.Percent == 0)
            {
                this.lblTimeEstimate.Text = "Calculating remaining time...";
            }
            else
            {
                TimeSpan remaining = new TimeSpan(this.m_stopwatch.Elapsed.Ticks * (long)((100 - progressArgs.Percent) / progressArgs.Percent));
                // TODO: make this label more user friendly
                this.lblTimeEstimate.Text = string.Format("Estimated remaining time: {0}.", remaining.Format());
            }

            if (progressArgs.Percent == 100)
            {
                if (this.IsDisposed == false)
                {
                    this.m_RaiseExceptionOnClose = false;
                    this.m_UserCancelled = false;
                    this.Close();
                }
            }
        }

        public void SingleConfigurationProgressHandler(ProgressCallbackEventArgs progressArgs)
        {
            if (this.m_FormClosed)
            {
                // if form is closed we have no access to controls
                this.MasterInterpreter.Cancel();
                return;
            }

            this.progressBarSingle.Value = Math.Min((int)progressArgs.Percent, 100);
            this.lblSingle.Text = string.Format("{0} - {2}", progressArgs.Context, progressArgs.Configuration, progressArgs.Title);
        }

        private void ProgressDialog_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.m_FormClosed = true;
            if (this.m_UserCancelled)
            {
                // ALT+F4 or clicking on the close button
                this.MasterInterpreter.Cancel();

                MessageBox.Show("Model transformation was cancelled. The last configuration is still being processed, which may take a few more seconds. When the console indicates the Master Interpreter finished, model editing may be continued.", "Canceling", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
    }

    public static class ExtensionMethods
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="timeSpan"></param>
        /// <returns></returns>
        public static string Format(this TimeSpan timeSpan)
        {
            StringBuilder sb = new StringBuilder();

            int maxToShow = 1;
            int currentlyShown = 0;

            if ((maxToShow > currentlyShown) && (timeSpan.Days != 0))
            {
                sb.Append(" ");
                sb.Append(timeSpan.Days);
                sb.Append(" ");
                sb.Append("days");
                currentlyShown++;
            }
            if ((maxToShow > currentlyShown) && (timeSpan.Hours != 0 || sb.Length != 0))
            {
                sb.Append(" ");
                sb.Append(timeSpan.Hours);
                sb.Append(" ");
                sb.Append("hours");
                currentlyShown++;
            }
            if ((maxToShow > currentlyShown) && (timeSpan.Minutes != 0 || sb.Length != 0))
            {
                sb.Append(" ");
                sb.Append(timeSpan.Minutes);
                sb.Append(" ");
                sb.Append("minutes");
                currentlyShown++;
            }

            if (maxToShow > currentlyShown)
            {
                if ((currentlyShown > 0) || (timeSpan.Seconds > 1))
                {
                    sb.Append(" ");
                    sb.Append(timeSpan.Seconds);
                    sb.Append(" ");
                    sb.Append("seconds");
                    currentlyShown++;
                }
                else
                {
                    sb.Append(" few seconds left");
                }
            }
            return sb.ToString();
        }
    }
}
