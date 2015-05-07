using System;
using System.Runtime.InteropServices;
using GME.MGA;
using GME;
using System.IO;
using System.Diagnostics;

// Modified from Run_ESMoL_Toolchain by Kevin Smyth

namespace Cyber2AVM
{

    public class InvokeUDMCopy
    {
        private string getPath(IMgaFCO fco)
        {
            string path = fco.Name;
            while (fco.ParentModel != null)
            {
                path = fco.ParentModel.Name + "/" + path;
                fco = fco.ParentModel;
            }
            IMgaFolder folder = fco.ParentFolder;
            while (folder != null)
            {
                path = folder.Name + "/" + path;
                folder = folder.ParentFolder;
            }
            return "/" + path;
        }

        string UDM_PATH = "";
        public bool GenerateXML(MgaProject project, string input_filename)
        {
            GME.CSharp.GMEConsole console = GME.CSharp.GMEConsole.CreateFromProject(project);

            string[] filename_parts = input_filename.Split('.');
            string output_filename = filename_parts[0] + ".xml";
            int pathPosition = output_filename.LastIndexOf("\\");
            string output_path = output_filename.Substring(0, pathPosition + 1) + "\\Cyber\\";
            string filename_base = output_filename.Substring(pathPosition + 1);

            if (!Directory.Exists(output_path))
            {
                Directory.CreateDirectory(output_path);
            }

            output_filename = output_path + filename_base;

            UDM_PATH = Environment.GetEnvironmentVariable("UDM_PATH");
            if (UDM_PATH == null || UDM_PATH == "")
            {
                console.Out.WriteLine("Udm not detected. Model will not be translated to XML.");
                return false;
            }

            string keyName = @"HKEY_LOCAL_MACHINE\Software\META";
            string value = @"META_PATH";

            string META_PATH = (string)Microsoft.Win32.Registry.GetValue(
                keyName,
                value,
                "ERROR: " + keyName + value + " does not exist!");


            string cybercomp_paradigm_filename = META_PATH + "\\meta\\CyberComposition_udm.xml";
            string cybercomp_xsd_path = META_PATH + "\\meta\\CyberComposition";

            console.Out.WriteLine("Translating model to XML...");
            console.Out.WriteLine("Output file is " + output_filename);

            runProgram("UdmCopy.exe", new string[] { input_filename, output_filename, cybercomp_paradigm_filename, cybercomp_xsd_path });

            return true;

        }

        private string escapeArguments(string[] arguments)
        {
            string ret = "";
            foreach (string arg in arguments)
            {
                ret += "\"" + arg + "\" ";
            }
            return ret;
        }
        private void runProgram(string program, string[] arguments)
        {
            runProgram(program, escapeArguments(arguments));
        }

        private void runProgram(string program, string arguments)
        {
            program = UDM_PATH + "\\bin\\" + program;
            Process p = new Process();
            // Redirect the output stream of the child process.
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.CreateNoWindow = true;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.RedirectStandardError = false;
            p.StartInfo.FileName = program;
            p.StartInfo.Arguments = arguments;
            p.Start();
            string output = p.StandardOutput.ReadToEnd();
            p.WaitForExit();
            if (p.ExitCode != 0)
            {
                p = new Process();
                p.StartInfo.FileName = "cmd.exe";
                p.StartInfo.Arguments = " /k \"\"" + program + "\" " + arguments + "\"";
                p.Start();

                throw new ApplicationException("Running " + program + " " + arguments + " failed: " + output);
            }
        }

        private delegate void voidDelegate();
    }

}