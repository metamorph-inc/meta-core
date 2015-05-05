using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Diagnostics;
using Microsoft.Win32;



namespace CAT_CADtoCyPhyConverter
{
    static class CAT_CADtoCyPhyConverter
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
//            Application.Run(new Form1());

            bool CREO_present = false;

            string registry_key = @"SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall";
            using (Microsoft.Win32.RegistryKey key = Registry.LocalMachine.OpenSubKey(registry_key))
            {
                foreach (string subkey_name in key.GetSubKeyNames())
                {
                    using (RegistryKey subkey = key.OpenSubKey(subkey_name))
                    {
                        try
                        {
                            string str = subkey.GetValue("DisplayName").ToString();
                            if (str.Equals("Creo Layout Version 2.0 Datecode [M070]"))
                            {
                                MessageBox.Show(str, "CREO Found on this Machine");
                                CREO_present = true;
                            }
                        }
                        catch (Exception nada)
                        {
                        }
                    }
                }
            }

            if (CREO_present)
            {
                try
                {
                    Process firstProc = new Process();
                    firstProc.StartInfo.FileName = "C:\\Program Files\\PTC\\Creo 2.0\\Layout\\bin\\layout.exe";
                    firstProc.EnableRaisingEvents = true;

                    firstProc.Start();

                    firstProc.WaitForExit();

                    //You may want to perform different actions depending on the exit code.
                    MessageBox.Show("CREO exited: " + firstProc.ExitCode);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("An error occurred!!!: " + ex.Message);
                    return;
                }
            }
        }
    }
}
