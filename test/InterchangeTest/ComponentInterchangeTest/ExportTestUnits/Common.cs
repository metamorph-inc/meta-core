using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;

namespace ComponentExporterUnitTests
{
    public class Common
    {
        /// <summary>
        /// Unpack an XME file and return the path to the MGA
        /// </summary>
        /// <param name="xmePath">Path to the XME file to be unpacked</param>
        /// <returns>Path to MGA file</returns>
        public static string unpackXme(string xmePath)
        {
            if (!File.Exists(xmePath))
            {
                throw new FileNotFoundException(String.Format("{0} not found", xmePath));
            }
            string mgaPath = Path.ChangeExtension(xmePath, "mga");
            GME.MGA.MgaUtils.ImportXME(xmePath, mgaPath);
            
            return mgaPath;
        }

        public static int processCommon(Process process, bool redirect = false)
        {
            using (process)
            {
                process.StartInfo.UseShellExecute = false;
                if (redirect)
                {
                    process.StartInfo.RedirectStandardError = true;
                    //process.StartInfo.RedirectStandardOutput = true;
                    process.StartInfo.CreateNoWindow = true;
                }

                process.StartInfo.CreateNoWindow = true;
                process.Start();
                if (redirect)
                {
                    char[] buffer = new char[4096];
                    while (true)
                    {
                        int read = process.StandardError.Read(buffer, 0, 4096);
                        if (read == 0)
                        {
                            break;
                        }
                        Console.Error.Write(buffer, 0, read);
                    }

                }
                process.WaitForExit();

                return process.ExitCode;
            }
        }

        public static int runProcessAndGetOutput(Process process, out string output, bool err_only=false)
        {
            using (process)
            {
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardError = true;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.CreateNoWindow = true;

                StringBuilder out_ = new StringBuilder();
                
                process.ErrorDataReceived += (o, dataArgs) =>
                {
                    if (dataArgs.Data == null)
                        return;
                    out_.Append(dataArgs.Data);
                };
                process.OutputDataReceived += (o, dataArgs) =>
                {
                    if (dataArgs.Data == null)
                        return;
                    if (err_only)
                    {
                        Console.Out.WriteLine(dataArgs.Data);
                    }
                    else
                    {
                        out_.Append(dataArgs.Data);
                    }
                };

                
                process.Start();
                process.BeginOutputReadLine();
                process.BeginErrorReadLine();
                process.WaitForExit();

                output = out_.ToString();
                return process.ExitCode;
            }
        }

        public static int runCyPhyComponentExporterCL(string mgaPath)
        {
            var testPath = Path.GetDirectoryName(mgaPath);
            
            /* string currentDir = Directory.GetCurrentDirectory();
            try
            {
                Directory.SetCurrentDirectory(testPath);
                return CyPhyComponentExporterCL.CyPhyComponentExporterCL.Main(
                    new string[] { 
                    mgaPath, testPath
                });
            }
            finally
            {
                Directory.SetCurrentDirectory(currentDir);
            } */

            var process = new Process
            {
                StartInfo =
                {
                    FileName = Path.Combine(META.VersionInfo.MetaPath, "src", "CyPhyComponentExporterCL", "bin", "Release", "CyPhyComponentExporterCL.exe"),
                    WorkingDirectory = testPath
                }
            };

            process.StartInfo.Arguments += String.Format(" \"{0}\" \"{1}\"", mgaPath, testPath);

            return Common.processCommon(process,true);
        }

        public static int RunXmlComparator(string exported, string desired)
        {
            string xmlComparatorPath = Path.Combine(
                META.VersionInfo.MetaPath,                
                "test",                
                "InterchangeTest",
                "InterchangeXmlComparator",
                "bin",
                "Release",
                "InterchangeXmlComparator.exe"
                );

            var process = new Process
            {
                StartInfo =
                {
                    FileName = xmlComparatorPath
                }
            };

            process.StartInfo.Arguments += String.Format(" -e \"{0}\" -d \"{1}\" -m Component", exported, desired);
            string output;
            int retval = Common.runProcessAndGetOutput(process, out output);
            Xunit.Assert.True(0 == retval, "XmlComparator returned non-zero: " + output);
            return retval;
        }

    }
}
