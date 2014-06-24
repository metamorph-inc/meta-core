using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting;
using System.Net.NetworkInformation;
using JobManagerFramework;

namespace JobManager
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] argv)
        {
            try
            {
                Dictionary<string, string> settings = ParseArguments(argv);

                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);

                int port = 35010; // preferred

                string strPort = string.Empty;

                if (settings.TryGetValue("-P", out strPort))
                {
                    int.TryParse(strPort, out port);
                }

                //TODO: read from settings
                JobManagerForm managerForm = new JobManagerForm(settings);
                Console.Out.WriteLine("JobManager has started");

                managerForm.Text = "JobManager (Port:" + port.ToString() + ")";

                Application.Run(managerForm);
            }
            catch (Exception ex)
            {
                Console.Out.WriteLine("JobManager failed to start. Exception: {0}", ex);
                MessageBox.Show(
                    ex.Message,
                    "Exception",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
            }
        }

        private static Dictionary<string, string> ParseArguments(string[] argv)
        {
            Dictionary<string, string> result = new Dictionary<string, string>();

            for (int i = 0; i < argv.Length; i++)
            {
                if (argv[i].StartsWith("-"))
                {
                    result.Add(argv[i], string.Empty);
                    if (i < argv.Length - 1)
                    {
                        i++;
                        if (argv[i].StartsWith("-"))
                        {
                            i--;
                            continue;
                        }
                        else
                        {
                            result[argv[i - 1]] = argv[i];
                        }
                    }
                }
            }

            return result;
        }

        private static int GetPort(int port)
        {
            IPGlobalProperties ipGlobalProperties = IPGlobalProperties.GetIPGlobalProperties();
            System.Net.IPEndPoint[] tcpListeners = ipGlobalProperties.GetActiveTcpListeners();

            foreach (var item in tcpListeners)
            {
                if (item.Port == port)
                {
                    if (port < 65000)
                    {
                        return GetPort(port + 1);
                    }
                    else
                    {
                        return -1;
                    }
                }
            }

            return port;
        }
    }
}
