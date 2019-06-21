using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;
using GME.MGA;

namespace ComponentImporterUnitTests
{
    class Common
    {
        public static readonly string gmepyPath = Path.Combine(META.VersionInfo.MetaPath, "externals", "common-scripts", "gmepy.exe");
        public static readonly string InterchangeTestDirectory = Path.Combine(META.VersionInfo.MetaPath, "test", "InterchangeTest");
        public static readonly string _interchangeBinDirectory = Path.Combine(InterchangeTestDirectory, "bin");
        public static readonly string _importModelDirectory = Path.Combine(InterchangeTestDirectory, "ComponentInterchangeTest", "ImportTestModels");
        public static readonly string blankInputModelPath = Path.Combine(InterchangeTestDirectory, "ComponentInterchangeTest", "SharedModels", "BlankInputModel", "InputModel.xme");

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
            string mgaPath = Path.Combine(
                Path.GetDirectoryName(xmePath),
                Path.GetFileNameWithoutExtension(xmePath) + "_importTest.mga");
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

        public static int runCyPhyComponentImporterCL(string mgaPath, string acmPath)
        {
            var testPath = Path.GetDirectoryName(mgaPath);
            var process = new Process
            {
                StartInfo =
                {
                    FileName = Path.Combine(META.VersionInfo.MetaPath, "src", "CyPhyComponentImporterCL", "bin", "Release", "CyPhyComponentImporterCL.exe"),
                    WorkingDirectory = testPath
                }
            };

            process.StartInfo.Arguments += String.Format(" \"{0}\" \"{1}\"", acmPath, mgaPath);

            return Common.processCommon(process);
        }

        public static int RunCyPhyMLComparator(string desired, string imported)
        {
            var path = Path.GetDirectoryName(desired);
            var process = new Process
            {
                StartInfo =
                {
                    FileName = Path.Combine(META.VersionInfo.MetaPath, "src", "bin", "CyPhyMLComparator.exe")
                }
            };

            process.StartInfo.Arguments += String.Format("\"{0}\" \"{1}\"", desired, imported);

            return processCommon(process,true);
        }

        public static MgaProject GetProject(String filename)
        {
            MgaProject result = null;

            if (filename != null && filename != "")
            {
                if (Path.GetExtension(filename) == ".mga")
                {
                    result = new MgaProject();
                    if (System.IO.File.Exists(filename))
                    {
                        Console.Out.Write("Opening {0} ... ", filename);
                        bool ro_mode;
                        result.Open("MGA=" + Path.GetFullPath(filename), out ro_mode);
                    }
                    else
                    {
                        Console.Out.Write("Creating {0} ... ", filename);
                        result.Create("MGA=" + filename, "CyPhyML");
                    }
                    Console.Out.WriteLine("Done.");
                }
                else
                {
                    Console.Error.WriteLine("{0} file must be an mga project.", filename);
                }
            }
            else
            {
                Console.Error.WriteLine("Please specify an Mga project.");
            }

            return result;
        }
    }
}
