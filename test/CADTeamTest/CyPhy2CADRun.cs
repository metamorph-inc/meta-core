using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using GME.MGA;
using Xunit;

namespace CADTeamTest
{
    public static class CyPhy2CADRun
    {
        internal static void CopyDirectory(string strSource, string strDestination)
        {
            Directory.CreateDirectory(strDestination);
            Directory.CreateDirectory(strSource);
            DirectoryInfo dirInfo = new DirectoryInfo(strSource);
            FileInfo[] files = dirInfo.GetFiles();
            foreach (FileInfo tempfile in files)
            {
                tempfile.CopyTo(Path.Combine(strDestination, tempfile.Name));
            }
            DirectoryInfo[] dirctororys = dirInfo.GetDirectories();
            foreach (DirectoryInfo tempdir in dirctororys)
            {
                CopyDirectory(Path.Combine(strSource, tempdir.Name), Path.Combine(strDestination, tempdir.Name));
            }

        }

        public static string GetProjectDir(MgaProject project)
        {
            string workingDir = Path.GetTempPath();
            if (project.ProjectConnStr.StartsWith("MGA="))
            {
                workingDir = Path.GetDirectoryName(project.ProjectConnStr.Substring("MGA=".Length));
            }
            return workingDir;
        }

        public static bool Run(string originalProjectDir, MgaProject project, MgaFCO testObj, bool copycomponents)
        {
            bool status = true;
            try
            {
                var outputdirname = GetProjectDir(project);
                if (copycomponents)
                {
                    CopyDirectory(Path.Combine(originalProjectDir, "components"), Path.Combine(outputdirname, "components"));
                }

                var interpreter = new CyPhy2CAD_CSharp.CyPhy2CAD_CSharpInterpreter();
                interpreter.Initialize(project);

                var mainParameters = new CyPhyGUIs.InterpreterMainParameters();
                var cadSettings = new CyPhy2CAD_CSharp.CyPhy2CADSettings();
                cadSettings.OutputDirectory = outputdirname;
                cadSettings.AuxiliaryDirectory = "";
                mainParameters.config = cadSettings;
                mainParameters.Project = project;
                mainParameters.CurrentFCO = testObj;
                mainParameters.SelectedFCOs = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
                mainParameters.StartModeParam = 128;
                mainParameters.ConsoleMessages = false;
                mainParameters.ProjectDirectory = Path.GetDirectoryName(GetProjectDir(project));
                mainParameters.OutputDirectory = outputdirname;

                interpreter.Main(mainParameters);
            }
            catch (Exception)
            {
                status = false;
            }
            finally
            {
                project.Close();
            }

            return status;

        }

        public static bool Run(string outputdirname, string xmePath, string absPath, bool copycomponents = true, bool deletedir = true)
        {
            xmePath = Path.GetFullPath(xmePath);
            outputdirname = Path.GetFullPath(outputdirname);
            bool status = true;
            string ProjectConnStr;
            if (deletedir && Directory.Exists(outputdirname))
            {
                Directory.Delete(outputdirname, true);
            }
            Directory.CreateDirectory(outputdirname);

            MgaUtils.ImportXMEForTest(xmePath, Path.Combine(outputdirname, Path.GetFileNameWithoutExtension(xmePath) + "_CADtest.mga"), out ProjectConnStr);

            MgaProject project = new MgaProject();
            bool ro_mode;
            project.Open(ProjectConnStr, out ro_mode);

            try
            {
                var terr = project.BeginTransactionInNewTerr();
                var testObj = project.ObjectByPath[absPath] as MgaFCO;
                project.AbortTransaction();

                return Run(Path.GetDirectoryName(xmePath), project, testObj, copycomponents);
            }
            catch(Exception)
            {
                status = false;
            }
            finally
            {
                project.Close();
            }

            return status;
        }
    }
}
