using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using Xunit;
using META;
using System.Diagnostics;

namespace PythonTest
{
    public class PythonTest
    {
        [STAThread]
        public static int Main(string[] args)
        {
            int ret = Xunit.ConsoleClient.Program.Main(new string[] {
                Assembly.GetAssembly(typeof(PythonTest)).CodeBase.Substring("file:///".Length),
                //"/noshadow",
            });
            Console.In.ReadLine();
            return ret;
        }

        private Exception ImportModule(string moduleName)
        {
            return CheckCommand(String.Format("import {0}", moduleName));
        }

        private Exception CheckCommand(string command)
        {
            try
            {
                Process p = new Process();
                p.StartInfo = new ProcessStartInfo()
                {
                    FileName = VersionInfo.PythonVEnvExe,
                    Arguments = String.Format("-c \"{0}\"", command),
                    CreateNoWindow = true,
                    UseShellExecute = false
                };
                p.Start();
                p.WaitForExit();
                if (p.ExitCode != 0)
                {
                    throw new Exception("python.exe exited with non-zero code " + p.ExitCode);
                }
            }
            catch (Exception e)
            {
                return e;
            }
            return null;
        }

        [Fact]
        public void TestImports()
        {
            var module_names = new string[] {
                    //"isis_meta", not used anywhere
                    "MaterialLibraryInterface",
                    "meta_nrmm",
                    "PCC",
                    "py_modelica",
                    "py_modelica_exporter",
            };

            foreach (var test in module_names.Select(moduleName =>
                {
                    Func<Exception> m = () => ImportModule(moduleName);
                    var asyncResult = m.BeginInvoke(null, null);
                    return new Tuple<string, IAsyncResult, Func<Exception>>(moduleName, asyncResult, m);
                }).ToList())
            {
                Exception e = test.Item3.EndInvoke(test.Item2);
                if (e != null)
                {
                    Assert.True(e == null, "Importing " + test.Item1 + " failed: " + e.ToString());
                }
            }
        }

        [Fact]
        public void TestTk()
        {
            var e = CheckCommand("import Tkinter; Tkinter.Tk()");
            if (e != null)
            {
                Assert.True(e == null, "Tkinter failed: " + e.ToString());
            }
        }

        [Fact]
        public void TestPythonExitCode()
        {
            Process p = new Process();
            p.StartInfo = new ProcessStartInfo()
            {
                FileName = VersionInfo.PythonVEnvExe,
                Arguments = String.Format("-c \"import py_modelica; import scipy.io; import sys; sys.exit(42);\""),
                CreateNoWindow = true,
                UseShellExecute = false
            };
            p.Start();
            p.WaitForExit();
            Assert.Equal(42, p.ExitCode);
        }
    }
}
