using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


using Xunit;
using System.IO;
using GME.MGA;


namespace DynamicsTeamTest
{
    public static class CyPhyPETRunner
    {
        /// <summary>
        /// Calls CyPhyPET with early bindings
        /// </summary>
        /// <param name="outputdirname">xme folder from trunk/models/DynamicsTeam</param>
        /// <param name="projectPath">name of mga-file</param>
        /// <param name="absPath">Folder-path to PET-Exploration</param>
        /// <returns>Boolean - True -> interpreter call was successful</returns>
        public static bool Run(string outputdirname, string projectPath, string absPath)
        {
            bool result = false;

            result = RunReturnFull(outputdirname, projectPath, absPath).Item2.Success;

            return result;
        }

        public static Tuple<CyPhyGUIs.InterpreterMainParameters, CyPhyGUIs.IInterpreterResult> RunReturnFull(string outputdirname, string projectPath, string absPath,
            CyPhyGUIs.SmartLogger logger = null, Action<MgaProject> preProcess = null)
        {
            var mainParameters = new CyPhyGUIs.InterpreterMainParameters();
            CyPhyGUIs.IInterpreterResult results = null;
            Assert.True(File.Exists(projectPath), "Project file does not exist.");
            string ProjectConnStr = "MGA=" + projectPath;

            //Type CyPhyPETInterpreter = Type.GetTypeFromProgID("MGA.Interpreter.CyPhyPET");
            //Type MainParametersType = Type.GetTypeFromProgID("ISIS.CyPhyML.InterpreterConfiguration");

            MgaProject project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);
            try
            {
                if (preProcess != null)
                {
                    preProcess(project);
                }
                var terr = project.BeginTransactionInNewTerr();
                var testObj = project.ObjectByPath[absPath] as MgaFCO;
                Assert.True(testObj != null, String.Format("Could not find FCO by path '{0}'", absPath));
                mainParameters.OriginalCurrentFCOName = testObj.Name;
                project.AbortTransaction();

                string OutputDir = Path.Combine(Path.GetDirectoryName(projectPath), outputdirname);
                OutputDir = Path.GetFullPath(OutputDir);
                if (Directory.Exists(OutputDir))
                {
                    CyPhyGUIs.CyPhyDirectory.EnsureEmptyDirectory(OutputDir);
                }
                Directory.CreateDirectory(OutputDir);

                //dynamic interpreter = Activator.CreateInstance(CyPhyPETInterpreter);
                var interpreter = new CyPhyPET.CyPhyPETInterpreter();
                interpreter.Initialize(project);

                //dynamic mainParameters = Activator.CreateInstance(MainParametersType);
                mainParameters.Project = project;
                mainParameters.CurrentFCO = testObj;
                mainParameters.SelectedFCOs = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
                mainParameters.StartModeParam = 128;
                mainParameters.ConsoleMessages = false;
                mainParameters.ProjectDirectory = Path.GetDirectoryName(projectPath);
                mainParameters.OutputDirectory = OutputDir;

                //dynamic results = interpreter.Main(mainParameters);
                interpreter.Logger = logger;
                results = interpreter.MainThrows(mainParameters);

                Assert.True(File.Exists(ProjectConnStr.Substring("MGA=".Length)));

                if (results.Success == false)
                {
                    Test.DeleteDirectory(OutputDir);
                }
            }
            finally
            {
                project.Close(true);
            }

            return new Tuple<CyPhyGUIs.InterpreterMainParameters, CyPhyGUIs.IInterpreterResult>(mainParameters, results);
        }

        /// <summary>
        /// Calls CyPhyPET, always closes the project.
        /// </summary>
        /// <param name="outputdirname">xme folder from trunk/models/DynamicsTeam</param>
        /// <param name="projectPath">name of mga-file</param>
        /// <param name="absPath">Folder-path to PET-Exploration</param>
        /// <returns>The results</returns>
        //public static CyPhyGUIs.IInterpreterResult Run(string outputdirname, string projectPath, string absPath)
        //{
        //    Assert.True(File.Exists(projectPath), "Project file does not exist.");
        //    META.ComComponent interpreter;
        //    string ProjectConnStr = "MGA=" + projectPath;
        //    MgaProject project = new MgaProject();
        //    project.OpenEx(ProjectConnStr, "CyPhyML", null);
        //    try
        //    {
        //        var terr = project.BeginTransactionInNewTerr();
        //        var testObj = project.ObjectByPath[absPath] as MgaFCO;
        //        project.AbortTransaction();

        //        interpreter = new META.ComComponent("MGA.Interpreter.CyPhyPET", true);

        //        Assert.True(interpreter.isValid, "Interpreter cannot be instansiated.");

        //        string OutputDir = Path.Combine(Path.GetDirectoryName(projectPath), outputdirname);
        //        if (Directory.Exists(OutputDir))
        //        {
        //            Test.DeleteDirectory(OutputDir);
        //        }
        //        Directory.CreateDirectory(OutputDir);

        //        interpreter.Initialize(project);

        //        interpreter.MainParameters.Project = project;
        //        interpreter.MainParameters.CurrentFCO = testObj;
        //        interpreter.MainParameters.SelectedFCOs = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
        //        interpreter.MainParameters.StartModeParam = 128;
        //        interpreter.MainParameters.ConsoleMessages = false;
        //        interpreter.MainParameters.ProjectDirectory = Path.GetDirectoryName(projectPath);
        //        interpreter.MainParameters.OutputDirectory = OutputDir;
        //        interpreter.Main();

        //        Assert.True(File.Exists(ProjectConnStr.Substring("MGA=".Length)));
        //    }
        //    finally
        //    {
        //        project.Close(true);
        //    }

        //    Assert.NotNull(interpreter);

        //    return interpreter.result;
        //}
    }
}
