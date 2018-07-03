// -----------------------------------------------------------------------
// <copyright file="AnalysisTool.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace META
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using Microsoft.Win32;
    using System.IO;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class AnalysisTool
    {
        public const string ParameterNameInWorkflow = "AnalysisTool";

        public static List<AnalysisTool> GetFromProgId(string progid)
        {
            if (_byProgId == null)
            {
                CacheTools();
            }
            List<AnalysisTool> tools = new List<AnalysisTool>();
            if (progid != null)
            {
                _byProgId.TryGetValue(progid, out tools);
                if (tools == null)
                {
                    tools = new List<AnalysisTool>();
                }
            }
            return tools;
        }

        public static AnalysisTool GetByName(string analysisToolName)
        {
            if (_byName == null)
            {
                CacheTools();
            }
            AnalysisTool result = null;
            if (analysisToolName != null)
            {
                _byName.TryGetValue(analysisToolName, out result);
            }
            return result;
        }

        private static SortedDictionary<string, List<AnalysisTool>> _byProgId;
        private static SortedDictionary<string, AnalysisTool> _byName;

        private static void CacheTools()
        {
            _byName = new SortedDictionary<string, AnalysisTool>();
            _byProgId = new SortedDictionary<string, List<AnalysisTool>>();

            var analysisToolKey = Registry.LocalMachine.OpenSubKey(@"Software\META\AnalysisTools");
            if (analysisToolKey != null)
            {
                foreach (string subkeyname in analysisToolKey.GetSubKeyNames())
                {
                    AnalysisTool analysistool = new AnalysisTool()
                        {
                            Name = subkeyname
                        };

                    var subkey = analysisToolKey.OpenSubKey(subkeyname);
                    string value;
                    value = subkey.GetValue("InstallLocation") as string;
                    if (value != null)
                    {
                        analysistool.InstallLocation = value;
                    }

                    value = subkey.GetValue("Version") as string;
                    if (value != null)
                    {
                        analysistool.Version = value;
                    }

                    value = subkey.GetValue("OutputDirectory") as string;
                    if (value != null)
                    {
                        analysistool.OutputDirectory = value;
                    }

                    value = subkey.GetValue("RunCommand") as string;
                    if (value != null)
                    {
                        analysistool.RunCommand = value;
                    }

                    value = subkey.GetValue("RequiredInterpreter") as string;
                    if (value != null)
                    {
                        analysistool.RequiredInterpreter = value;
                    }

                    List<AnalysisTool> tools = new List<AnalysisTool>();
                    _byProgId.TryGetValue(analysistool.RequiredInterpreter, out tools);
                    if (tools == null)
                    {
                        tools = new List<AnalysisTool>();
                    }
                    tools.Add(analysistool);
                    _byProgId[analysistool.RequiredInterpreter] = tools;
                    _byName[analysistool.Name] = analysistool;
                }
            }
        }

        public static void ApplyToolSelection(
            string progid,
            Dictionary<string, string> workflowParameters,
            CyPhyGUIs.InterpreterResult interpreterResult,
            CyPhyGUIs.IInterpreterMainParameters interpreterMainParameters,
            bool modifyLabels=true)
        {
            string toolName;
            workflowParameters.TryGetValue(AnalysisTool.ParameterNameInWorkflow, out toolName);
            if (string.IsNullOrEmpty(toolName) ||
                toolName == "Default")
            {
                return;
            }

            var tool = GetByName(toolName);

            if (tool == null)
            {
                throw new ApplicationException(String.Format("Error: analysis tool \"{0}\" is not registered", toolName));
            }

            // copy tool.OutputDirectory to generated directory
            MethodDelegateCopy toolOutputDirCopy = new MethodDelegateCopy(DirectoryCopy);
            IAsyncResult ar = toolOutputDirCopy.BeginInvoke(Path.Combine(tool.InstallLocation, tool.OutputDirectory), interpreterMainParameters.OutputDirectory, true, null, null);
            toolOutputDirCopy.EndInvoke(ar);

            interpreterResult.RunCommand = tool.RunCommand;
            if (modifyLabels)
            {
                interpreterResult.Labels += " && " + tool.Name;
            }
        }


        public string Name { get; private set; }
        public string InstallLocation { get; private set; }
        public string Version { get; private set; }
        public string OutputDirectory { get; private set; }
        public string RunCommand { get; private set; }
        public string RequiredInterpreter { get; private set; }


        private delegate void MethodDelegateCopy(
            string sourceDirName,
            string destDirName,
            bool copySubDirs);

        private static void DirectoryCopy(
            string sourceDirName,
            string destDirName,
            bool copySubDirs)
        {
            DirectoryInfo dir = new DirectoryInfo(sourceDirName);
            DirectoryInfo[] dirs = dir.GetDirectories();

            if (!dir.Exists)
            {
                throw new DirectoryNotFoundException(
                    "Source directory does not exist or could not be found: "
                    + sourceDirName);
            }

            if (!Directory.Exists(destDirName))
            {
                Directory.CreateDirectory(destDirName);
            }

            FileInfo[] files = dir.GetFiles();
            foreach (FileInfo file in files)
            {
                string temppath = Path.Combine(destDirName, file.Name);
                file.CopyTo(temppath, true);
            }

            if (copySubDirs)
            {
                foreach (DirectoryInfo subdir in dirs)
                {
                    string temppath = Path.Combine(destDirName, subdir.Name);
                    DirectoryCopy(subdir.FullName, temppath, copySubDirs);
                }
            }
        }
    }
}
