using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using Xunit;
using System.IO;

namespace MasterInterpreterTest
{
    public class CyPhyMasterInterpreterRunner
    {

        public static bool RunContextCheck(string projectPath, string absPath)
        {
            bool result = false;
            Assert.True(File.Exists(projectPath), "Project file does not exist.");
            string ProjectConnStr = "MGA=" + Path.GetFullPath(projectPath);

            MgaProject project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);
            try
            {
                var terr = project.BeginTransactionInNewTerr();
                var testObj = project.ObjectByPath[absPath] as MgaFCO;
                project.AbortTransaction();

                using (var masterInterpreter = new CyPhyMasterInterpreter.CyPhyMasterInterpreterAPI(project))
                {
                    CyPhyMasterInterpreter.Rules.ContextCheckerResult[] contextCheckerResults = null;

                    // check context
                    result = masterInterpreter.TryCheckContext(testObj as MgaModel, out contextCheckerResults);
                }
            }
            finally
            {
                project.Close(true);
            }

            return result;
        }


        public static bool GetConfigurations(string projectPath, string absPath)
        {

            bool result = false;
            Assert.True(File.Exists(projectPath), "Project file does not exist.");
            string ProjectConnStr = "MGA=" + Path.GetFullPath(projectPath);

            MgaProject project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);
            try
            {
                var terr = project.BeginTransactionInNewTerr();
                var testObj = project.ObjectByPath[absPath] as MgaFCO;
                project.AbortTransaction();

                using (var masterInterpreter = new CyPhyMasterInterpreter.CyPhyMasterInterpreterAPI(project))
                {
                    IMgaFCOs configurations = null;

                    Assert.ThrowsDelegate d = () =>
                    {
                        configurations = masterInterpreter.GetConfigurations(testObj as MgaModel);
                    };

                    Assert.DoesNotThrow(d);
                }
            }
            finally
            {
                project.Close(true);
            }

            return result;
        }

        public static bool AnalysisModelSupported(string projectPath, string absPath)
        {

            bool result = false;
            Assert.True(File.Exists(projectPath), "Project file does not exist.");
            string ProjectConnStr = "MGA=" + Path.GetFullPath(projectPath);

            MgaProject project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);
            try
            {
                var terr = project.BeginTransactionInNewTerr();
                var testObj = project.ObjectByPath[absPath] as MgaFCO;
                project.AbortTransaction();

                CyPhyMasterInterpreter.AnalysisModelProcessor analysisModelProcessor = null;

                 project.BeginTransactionInNewTerr();
                    try
                    {
                        Assert.ThrowsDelegate d = () =>
                        {
                            analysisModelProcessor = CyPhyMasterInterpreter.AnalysisModelProcessor.GetAnalysisModelProcessor(testObj as MgaModel);
                        };
                        Assert.DoesNotThrow(d);
                            Assert.True(analysisModelProcessor != null, string.Format("Analysis model processor was not able to create the model processor for {0} {1}.", testObj.Name, testObj.Meta.Name));
                    }
                    finally
                    {
                        project.AbortTransaction();
                    }

            }
            finally
            {
                project.Close(true);
            }

            return result;
        }

        public static void AnalysisModelNotSupported(string projectPath, string absPath)
        {
            Assert.True(File.Exists(projectPath), "Project file does not exist.");
            string ProjectConnStr = "MGA=" + Path.GetFullPath(projectPath);

            MgaProject project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);
            try
            {
                var terr = project.BeginTransactionInNewTerr();
                var testObj = project.ObjectByPath[absPath] as MgaFCO;
                project.AbortTransaction();

                CyPhyMasterInterpreter.AnalysisModelProcessor analysisModelProcessor = null;

                project.BeginTransactionInNewTerr();
                try
                {
                    Assert.ThrowsDelegate d = () =>
                    {
                        analysisModelProcessor = CyPhyMasterInterpreter.AnalysisModelProcessor.GetAnalysisModelProcessor(testObj as MgaModel);
                    };

                    Assert.False(analysisModelProcessor != null, string.Format("Analysis model processor was not able to create the model processor for {0} {1}.", testObj.Name, testObj.Meta.Name));

                    Assert.Throws<CyPhyMasterInterpreter.AnalysisModelContextNotSupportedException>(d);
                }
                finally
                {
                    project.AbortTransaction();
                }
            }
            finally
            {
                project.Close(true);
            }
        }

        public static bool RunMasterInterpreter(
            string projectPath,
            string absPath,
            string configPath,
            bool postToJobManager = false,
            bool keepTempModels = false)
        {
            var result = RunMasterInterpreterAndReturnResults(projectPath, absPath, configPath, postToJobManager, keepTempModels);
            return result.Success;
        }

        public static CyPhyMasterInterpreter.MasterInterpreterResult RunMasterInterpreterAndReturnResults(
            string projectPath,
            string absPath,
            string configPath,
            bool postToJobManager = false,
            bool keepTempModels = false)
        {
            Assert.True(File.Exists(projectPath), "Project file does not exist.");
            string ProjectConnStr = "MGA=" + Path.GetFullPath(projectPath);

            MgaProject project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);

            try
            {
                var terr = project.BeginTransactionInNewTerr();
                var testObj = project.ObjectByPath[absPath] as MgaFCO;
                var configObj = project.ObjectByPath[configPath] as MgaFCO;
                project.AbortTransaction();

                using (var masterInterpreter = new CyPhyMasterInterpreter.CyPhyMasterInterpreterAPI(project))
                {
                    masterInterpreter.Logger.GMEConsoleLoggingLevel = CyPhyGUIs.SmartLogger.MessageType_enum.Debug;

                    var miResults = masterInterpreter.RunInTransactionOnOneConfig(testObj as MgaModel, configObj, postToJobManager, keepTempModels);
                    Assert.True(miResults.Length == 1, "MasterInterpreter.RunInTransactionOnOneConfig should always return one result.");
                    return miResults[0];
                }
            }
            finally
            {
                project.Close(true);
            }
        }

    }
}
