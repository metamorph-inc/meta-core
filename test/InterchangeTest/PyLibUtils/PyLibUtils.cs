using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;

namespace ComponentImporterUnitTests
{
    public static class PyLibUtils
    {        
        public static int TryImportUsingPyLib(String filePath, out String output)
        {
            var path_PythonTestScript = Path.Combine(META.VersionInfo.MetaPath,
                                                     "test",
                                                     "InterchangeTest",
                                                     "PyLibUtils",
                                                     "ImportWithPyLib.py");
            var path_PyExe = META.VersionInfo.PythonVEnvExe;

            var process = new Process
            {
                StartInfo =
                {
                    FileName = path_PyExe,
                    Arguments = path_PythonTestScript + " " + filePath,
                    UseShellExecute = false,
                    RedirectStandardError = true,
                    RedirectStandardOutput = true,
                    CreateNoWindow = true
                }
            };

            process.Start();

            String procErrorOutput = "";
            char[] buffer = new char[4096];
            while (true)
            {
                int read = process.StandardError.Read(buffer, 0, 4096);
                if (read == 0)
                {
                    break;
                }
                procErrorOutput += buffer.ToString();
            }
            process.WaitForExit();

            output = procErrorOutput;
            return process.ExitCode;
        }
    }
}
