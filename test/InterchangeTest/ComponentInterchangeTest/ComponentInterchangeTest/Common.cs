using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;
using GME.MGA;
using Xunit;

namespace ComponentInterchangeTest
{
    public abstract class ComponentInterchangeTestFixtureBase : IDisposable
    {
        public String testPath
        {
            get
            {
                return Path.GetDirectoryName(xmePath);
            }
        }
        public abstract String xmePath { get; }
        public MgaProject proj { get; private set; }

        public ComponentInterchangeTestFixtureBase()
        {
            String mgaConnectionString;
            GME.MGA.MgaUtils.ImportXMEForTest(xmePath, out mgaConnectionString);
            var mgaPath = mgaConnectionString.Substring("MGA=".Length);

            Assert.True(File.Exists(Path.GetFullPath(mgaPath)),
                        String.Format("{0} not found. Model import may have failed.", mgaPath));

            proj = new MgaProject();
            bool ro_mode;
            proj.Open("MGA=" + Path.GetFullPath(mgaPath), out ro_mode);
            proj.EnableAutoAddOns(true);
        }

        public void Dispose()
        {
            proj.Save();
            proj.Close();
        }
    }

    class CommonFunctions
    {
        /// <summary>
        /// Unpack an XME file and return the path to the MGA
        /// </summary>
        /// <param name="xmePath">Path to the XME file to be unpacked</param>
        /// <returns>Path to MGA file</returns>
        public static string unpackXme(string xmePath,string suffix="")
        {
            if (!File.Exists(xmePath))
            {
                throw new FileNotFoundException(String.Format("{0} not found", xmePath));
            }
            string mgaPath = Path.Combine(
                Path.GetDirectoryName(xmePath),
                Path.GetFileNameWithoutExtension(xmePath) + suffix + ".mga");
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
                    process.StartInfo.RedirectStandardOutput = true;
                }
                
                process.StartInfo.CreateNoWindow = true;
                StringBuilder stdout = new StringBuilder();
                if (process.StartInfo.RedirectStandardOutput)
                {
                    process.OutputDataReceived += (o, dataArgs) =>
                    {
                        if (dataArgs.Data == null)
                            return;
                        lock (stdout)
                        {
                            stdout.Append(dataArgs.Data);
                        }
                    };
                }

                process.Start();
                if (process.StartInfo.RedirectStandardOutput)
                {
                    process.BeginOutputReadLine();
                }

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

        public static int runCyPhyComponentImporterCLRecursively(string mgaPath,string compFolderRoot)
        {
            var arguments = ("-r " + compFolderRoot + " " + mgaPath).Split(' ');

            return CyPhyComponentImporterCL.CyPhyComponentImporterCL.Main(arguments);
        }

        public static int runCyPhyComponentImporterCL(string mgaPath, string acmPath)
        {
            var arguments = (acmPath + " " + mgaPath).Split(' ');
            return CyPhyComponentImporterCL.CyPhyComponentImporterCL.Main(arguments);
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

            process.StartInfo.Arguments += desired;
            process.StartInfo.Arguments += " " + imported;

            return processCommon(process,true);
        }


        public static int runCyPhyComponentExporterCL(string mgaPath)
        {
            var testPath = Path.GetDirectoryName(mgaPath);
            var process = new Process
            {
                StartInfo =
                {
                    FileName = Path.Combine(META.VersionInfo.MetaPath, "src", "CyPhyComponentExporterCL", "bin", "Release", "CyPhyComponentExporterCL.exe"),
                    WorkingDirectory = testPath
                }
            };

            process.StartInfo.Arguments += " " + mgaPath;
            process.StartInfo.Arguments += " " + testPath;

            return CommonFunctions.processCommon(process, true);
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

            process.StartInfo.Arguments += String.Format(" -e {0} -d {1} -m Component", exported, desired);
            return CommonFunctions.processCommon(process, true);
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
