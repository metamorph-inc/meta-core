using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest
{
    // TODO: Evaluate this class (not used yet).
    public static class CyPhySoTRunner
    {
        /// <summary>
        /// Calls CyPhySoT with early bindings
        /// </summary>
        /// <param name="outputdirname">xme folder from trunk/models/DynamicsTeam</param>
        /// <param name="projectPath">name of mga-file</param>
        /// <param name="absPath">Folder-path to SoT</param>
        /// <returns>Boolean - True -> interpreter call was successful</returns>
        public static bool Run(string outputdirname, string projectPath, string absPath)
        {
            bool result = false;
            Assert.True(File.Exists(projectPath), "Project file does not exist.");
            string ProjectConnStr = "MGA=" + projectPath;

            MgaProject project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);
            try
            {
                var terr = project.BeginTransactionInNewTerr();
                var testObj = project.ObjectByPath[absPath] as MgaFCO;
                project.AbortTransaction();

                string OutputDir = Path.Combine(Path.GetDirectoryName(projectPath), outputdirname);
                if (Directory.Exists(OutputDir))
                {
                    Test.DeleteDirectory(OutputDir);
                }
                Directory.CreateDirectory(OutputDir);

                //dynamic interpreter = Activator.CreateInstance(CyPhyPETInterpreter);
                var interpreter = new CyPhySoT.CyPhySoTInterpreter();
                interpreter.Initialize(project);

                //dynamic mainParameters = Activator.CreateInstance(MainParametersType);
                var mainParameters = new CyPhyGUIs.InterpreterMainParameters();
                mainParameters.Project = project;
                mainParameters.CurrentFCO = testObj;
                mainParameters.SelectedFCOs = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
                mainParameters.StartModeParam = 128;
                mainParameters.ConsoleMessages = false;
                mainParameters.ProjectDirectory = Path.GetDirectoryName(projectPath);
                mainParameters.OutputDirectory = OutputDir;

                //dynamic results = interpreter.Main(mainParameters);
                var results = interpreter.Main(mainParameters);

                Assert.True(File.Exists(ProjectConnStr.Substring("MGA=".Length)));

                result = results.Success;
                if (result == false)
                {
                    Test.DeleteDirectory(OutputDir);
                }
            }
            finally
            {
                project.Close(true);
            }

            return result;
        }
    }
}
