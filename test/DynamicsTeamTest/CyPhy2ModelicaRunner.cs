using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Xunit;
using GME.MGA;

namespace DynamicsTeamTest
{
    public static class CyPhy2ModelicaRunner
    {
        /// <summary>
        /// Calls CyPhy2Modelica using early bindings
        /// </summary>
        /// <param name="outputdirname">xme folder from trunk/models/DynamicsTeam</param>
        /// <param name="projectPath">name of mga-file</param>
        /// <param name="absPath">Folder-path to test-bench</param>
        /// <returns>Boolean - True -> interpreter call was successful</returns>
        public static bool Run(string outputdirname, string projectPath, string absPath)
        {
            bool result = false;
            Assert.True(File.Exists(projectPath), "Project file does not exist.");
            string ProjectConnStr = "MGA=" + projectPath;

            //Type CyPhy2Modelica_v2Interpreter = Type.GetTypeFromProgID("MGA.Interpreter.CyPhy2Modelica_v2");
            //Type MainParametersType = Type.GetTypeFromProgID("ISIS.CyPhyML.InterpreterConfiguration");

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

                //dynamic interpreter = Activator.CreateInstance(CyPhy2Modelica_v2Interpreter);
                var interpreter = new CyPhy2Modelica_v2.CyPhy2Modelica_v2Interpreter();
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
                var results = interpreter.MainThrows(mainParameters);

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

        /// <summary>
        /// Calls CyPhy2Modelica using early bindings
        /// </summary>
        /// <param name="outputdirname">xme folder from trunk/models/DynamicsTeam</param>
        /// <param name="projectPath">name of mga-file</param>
        /// <param name="absPath">Folder-path to test-bench</param>
        /// <returns>Boolean - True -> interpreter call was successful</returns>
        public static bool RunMain(string outputdirname, string projectPath, string absPath)
        {
            bool result = false;
            Assert.True(File.Exists(projectPath), "Project file does not exist.");
            string ProjectConnStr = "MGA=" + projectPath;

            //Type CyPhy2Modelica_v2Interpreter = Type.GetTypeFromProgID("MGA.Interpreter.CyPhy2Modelica_v2");
            //Type MainParametersType = Type.GetTypeFromProgID("ISIS.CyPhyML.InterpreterConfiguration");

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

                //dynamic interpreter = Activator.CreateInstance(CyPhy2Modelica_v2Interpreter);
                var interpreter = new CyPhy2Modelica_v2.CyPhy2Modelica_v2Interpreter();
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
        /// <summary>
        /// Calls CyPhy2Modelica through CyPhyGUIs, always closes the project.
        /// 
        /// Raises InterpreterException if CyPhy2Modelica_v2 call failed.
        /// </summary>
        /// <param name="outputdirname">xme folder from trunk/models/DynamicsTeam</param>
        /// <param name="projectPath">name of mga-file</param>
        /// <param name="absPath">Folder-path to test-bench</param>
        /// <returns>The results</returns>
        //public static CyPhyGUIs.IInterpreterResult Run(string outputdirname, string projectPath, string absPath)
        //{
        //    Assert.True(File.Exists(projectPath), "Project file does not exist.");
        //    string ProjectConnStr = "MGA=" + projectPath;
        //    META.ComComponent interpreter;
        //    MgaProject project = new MgaProject();
        //    project.OpenEx(ProjectConnStr, "CyPhyML", null);
        //    try
        //    {
        //        var terr = project.BeginTransactionInNewTerr();
        //        var testObj = project.ObjectByPath[absPath] as MgaFCO;
        //        project.AbortTransaction();

        //        interpreter = new META.ComComponent("MGA.Interpreter.CyPhy2Modelica_v2", true);

        //        Assert.True(interpreter.isValid, "Interpreter cannot be instansiated.");

        //        string OutputDir = Path.Combine(Path.GetDirectoryName(projectPath), outputdirname);
        //        if (Directory.Exists(OutputDir))
        //        {
        //            Test.DeleteDirectory(OutputDir);
        //        }
        //        Directory.CreateDirectory(OutputDir);

        //        interpreter.Initialize(project);
        //        Type configType = Type.GetTypeFromProgID("ISIS.META.CyPhy2Modelica_v2Settings");
        //        var config = META.ComComponent.DeserializeConfiguration(Path.GetFullPath(Path.GetDirectoryName(projectPath)),
        //            configType, interpreter.ProgId);

        //        interpreter.InterpreterConfig = config;
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
