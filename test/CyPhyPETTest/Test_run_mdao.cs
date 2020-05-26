using GME.MGA;
using MasterInterpreterTest;
using META;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Text.RegularExpressions;
using Xunit;
using CyPhyGUIs;
using static CyPhyPET.ConfigurationParameterEditor;

namespace CyPhyPETTest
{
    public class Test_run_mdao
    {
        [Fact]
        public void TestPCC_run_mdao()
        {
            string assemblyDir = Path.GetDirectoryName(new Uri(Assembly.GetExecutingAssembly().CodeBase).LocalPath);
            string PCC_run_mdao_dir = Path.Combine(assemblyDir, @"..\..\..\..\src\Python27Packages\PCC\PCC\test\PCC_run_mdao");
            string manifestPath = Path.Combine(PCC_run_mdao_dir, "testbench_manifest.json");
            var manifest = File.ReadAllBytes(manifestPath);

            try
            {

                ProcessStartInfo info = new ProcessStartInfo()
                {
                    FileName = VersionInfo.PythonVEnvExe,
                    WorkingDirectory = PCC_run_mdao_dir,
                    Arguments = "-E -m run_mdao mdao_config.json",
                    UseShellExecute = false,
                    CreateNoWindow = true,
                    RedirectStandardError = true,
                    RedirectStandardOutput = true
                };

                Process proc = new Process();
                proc.StartInfo = info;

                proc.Start();
                var stdout = proc.StandardOutput.ReadToEnd();
                var stderr = proc.StandardError.ReadToEnd();
                stdout = stdout.Replace("\r", "");

                Assert.True(0 == proc.ExitCode, "run_mdao failed: " + stderr);

                Action<string> match = regex =>
                {
                    Assert.True(Regex.Match(stdout, regex).Success, String.Format("Output did not match regex '{0}'  {1}", regex, stdout));
                };

                match("Taylor Series:.*\n.*\\b11.*\\b110.*\n.*\\b6.*\\b60");
                match("Correlation:.*\n.*\\b0\\.9999.*\\b0\\.9999");
                match("Mean.*\\b66\\b.*\\b660\\b");
                match("PCC.*\nComplexity estimate.*\nDone!");
            }
            finally
            {
                File.WriteAllBytes(manifestPath, manifest);
            }
        }
    }

    public class Test_Code_Parameters
    {
        [Fact]
        public void TestCodeParameters()
        {
            Dictionary<string, object> assignment = CyPhyPET.PET.getPythonAssignment("x = 1\ny = '2'\nz = u'a\ud83d\ude4ca'");
            Assert.Equal(1L, assignment["x"]);
            Assert.Equal("2", assignment["y"]);
            // compute UTF16: map(hex, struct.unpack('H'*3, u'\U0001F64C'.encode('utf16')))
            Assert.Equal("a\ud83d\ude4ca", assignment["z"]);
        }
    }

    public class WorkFlow_PETFixture : DynamicsTeamTest.XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("..\\PET_simple_proof-of-concept", "WorkFlow_PET.xme"); }
        }
    }

    public class Workflow_PET_Test : IUseFixture<WorkFlow_PETFixture>
    {
        private string mgaFile;

        [Fact]
        [Trait("Feature", "PythonWrapper")]
        public void TestPython()
        {
            string outputDir = "results/TestPython";
            string petExperimentPath = "/@Testing/@ParametricExploration/@TestPython";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath);

            Assert.True(result.Item2.Success, "CyPhyPET failed.");

            string stderr = "<did not start process>";
            int retcode = Run(result.Item2.RunCommand, result.Item1.OutputDirectory, out stderr);
            Assert.True(0 == retcode, "run_mdao failed: " + stderr);
        }

        [Fact]
        [Trait("Feature", "CyPhyInterpreter")]
        public void TestTwoPythonInterpreter()
        {
            string outputDir = "results/TestTwoPythonInterpreter";
            string petExperimentPath = "/@Testing/@ParametricExploration/@TestTwoPythonInterpreter";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath);

            Assert.True(result.Item2.Success, "CyPhyPET failed.");

            string stderr = "<did not start process>";
            int retcode = Run(result.Item2.RunCommand, result.Item1.OutputDirectory, out stderr);
            Assert.True(0 == retcode, "run_mdao failed: " + stderr);

            string csv = File.ReadAllText(Path.Combine(result.Item1.OutputDirectory, "output.csv"));
            Assert.True(csv.Contains("512.0"), ".csv does not contain expected output:\n" + csv);
        }

        [Fact]
        [Trait("Feature", "InitArguments")]
        public void TestInitArguments()
        {
            MgaProject project = new MgaProject();
            project.OpenEx("MGA=" + mgaFile, "CyPhyML", null);
            try
            {
                project.BeginTransactionInNewTerr();
                try
                {
                    string petExperimentPath = "/@Testing/@ParametricExploration/@TestAnalysis";
                    string assemblyDir = Path.GetDirectoryName(new Uri(Assembly.GetExecutingAssembly().CodeBase).LocalPath);
                    string pyFile = Path.Combine(assemblyDir,
                        "..\\..\\..\\..\\models\\PET_simple_proof-of-concept\\sample_analysis.py");

                    MgaModel model = (MgaModel)project.ObjectByPath[petExperimentPath];
                    var params_ = CyPhyPET.CyPhyPETInterpreter.GetPythonComponentInitializerParameters(pyFile, (MgaFCO)model);
                    Assert.Equal(new string[] { "self", "config1", "config2", "opt1", "opt2" }, params_.args);
                    Assert.Equal(new string[] { "self", "config1", "config2" }, params_.requiredArgs);
                    Assert.Equal(new object[] { null, 0.0 }, params_.defaults);

                    var analysisBlock = ISIS.GME.Dsml.CyPhyML.Classes.AnalysisBlock.Cast(model.ObjectByPath["/@AnalysisBlock"]);
                    CyPhyPET.CyPhyPETInterpreter.EditConfigurationParameters(params_, analysisBlock, showGui: false);

                    var kwargs = CyPhyPET.CyPhyPETInterpreter.GetConfigurationParameters(analysisBlock);
                    CyPhyPET.CyPhyPETInterpreter.GetParamsAndUnknownsForPythonOpenMDAO(pyFile, analysisBlock, kwargs);
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

        [Fact]
        [Trait("Feature", "PythonParsing")]
        public void TestPythonParsing()
        {
            object cyPhyPython = Activator.CreateInstance(Type.GetTypeFromProgID("MGA.Interpreter.CyPhyPython"));
            Assert.Equal(null, GetExpressionParseError("1"));
            Assert.Equal(null, GetExpressionParseError("[1, \"2\"]"));
            Assert.Equal(null, GetExpressionParseError("1.0"));
            Assert.Equal(null, GetExpressionParseError("u'abcd\u0065'"));
            Assert.Equal(null, GetExpressionParseError("list()"));

            Assert.True(GetExpressionParseError("unknown_function()").Contains("is not defined"));
            Assert.True(GetExpressionParseError("def x").Contains("SyntaxError"));
            GC.KeepAlive(cyPhyPython);
        }

        [Fact]
        [Trait("Feature", "AnalysisBlock")]
        public void TestAnalysisBlock()
        {
            string outputDir = "results/TestAnalysis";
            string petExperimentPath = "/@Testing/@ParametricExploration/@TestAnalysis";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath);

            Assert.True(result.Item2.Success, "CyPhyPET failed.");

            string stderr = "<did not start process>";
            int retcode = Run(result.Item2.RunCommand, result.Item1.OutputDirectory, out stderr);
            Assert.True(0 == retcode, "run_mdao failed: " + stderr);

            string csv = File.ReadAllText(Path.Combine(result.Item1.OutputDirectory, "output.csv"));
            Assert.True(csv.Contains("600.0"), ".csv does not contain expected output:\n" + csv);
        }

        [Fact]
        public void OptimizationProblemWithExposedInitialConditions()
        {
            string outputDir = "results/" + GetCurrentMethod();
            string petExperimentPath = "/@Testing/@PETHierarchy/@" + GetCurrentMethod();

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath);

            Assert.True(result.Item2.Success, "CyPhyPET failed.");

            var configContents = File.ReadAllText(Path.Combine(result.Item1.OutputDirectory, "mdao_config.json"));
            var config = JsonConvert.DeserializeObject<AVM.DDP.PETConfig>(configContents);

            Assert.Equal(new string[] { "Optimizer", "x" }, config.components["Paraboloid"].parameters["x"].source);
            Assert.Equal(new string[] { "Optimizer", "y" }, config.components["Paraboloid"].parameters["y"].source);


            return;
        }

        [Fact]
        public void OptimizationInitialConditionProfiling()
        {
            string outputDir = "results/" + GetCurrentMethod();
            string petExperimentPath = "/@Testing/@PETHierarchy/@" + GetCurrentMethod();

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath);

            Assert.True(result.Item2.Success, "CyPhyPET failed.");

            var configContents = File.ReadAllText(Path.Combine(result.Item1.OutputDirectory, "mdao_config.json"));
            var config = JsonConvert.DeserializeObject<AVM.DDP.PETConfig>(configContents);

            Assert.Equal(new string[] { "ParameterStudy", "x_0" }, config.components["SaveTime"].parameters["pass_in"].source);
            Assert.Equal(new string[] { "OptimizationProblem", "f_xy" }, config.components["MeasureTime"].parameters["finished"].source);

            Assert.Equal(new string[] { "OptimizationProblem", "f_xy" }, config.drivers["ParameterStudy"].objectives["f_xy"].source);
            Assert.Equal(new string[] { "MeasureTime", "time" }, config.drivers["ParameterStudy"].objectives["Time"].source);

            Assert.Equal(new string[] { "x_0" }, config.subProblems["OptimizationProblem"].components["Paraboloid"].parameters["x"].source);

            Assert.Equal(new string[] { "ParameterStudy", "y_0" }, config.subProblems["OptimizationProblem"].problemInputs["y_0"].outerSource);
            Assert.Equal(new string[] { "Optimizer", "y" }, config.subProblems["OptimizationProblem"].problemInputs["y_0"].innerSource);
            Assert.Equal(false, config.subProblems["OptimizationProblem"].problemInputs["y_0"].pass_by_obj);
            Assert.Equal("0.0", config.subProblems["OptimizationProblem"].problemInputs["y_0"].value);

            Assert.Equal(false, config.subProblems["OptimizationProblem"].problemInputs["x_0"].pass_by_obj);
            Assert.Equal("0.0", config.subProblems["OptimizationProblem"].problemInputs["x_0"].value);

            Assert.Equal(new string[] { "x_0" }, config.subProblems["OptimizationProblem"].problemOutputs["x_f"]);
            Assert.Equal(new string[] { "Paraboloid", "f_xy" }, config.subProblems["OptimizationProblem"].problemOutputs["f_xy"]);

            Assert.Equal(new string[] { "Paraboloid", "f_xy" }, config.subProblems["OptimizationProblem"].drivers["Optimizer"].objectives["f_xy"].source);

            return;
        }

        [Fact]
        public void TestExcel()
        {
            var excelType = Type.GetTypeFromProgID("Excel.Application");
            if (excelType == null)
            {
                Console.Out.WriteLine("Skipping " + GetCurrentMethod() + " because Excel is not installed");
                return;
            }

            string outputDir = "results/" + GetCurrentMethod();
            string petExperimentPath = "/@Testing/@ParametricExploration/@" + GetCurrentMethod();

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath);

            Assert.True(result.Item2.Success, "CyPhyPET failed.");

            var configContents = File.ReadAllText(Path.Combine(result.Item1.OutputDirectory, "ExcelWrapper.json"));
            var config = JsonConvert.DeserializeObject<Dictionary<string, object>>(configContents);

            var unknowns = ((JArray)config["unknowns"]).Cast<JObject>().ToDictionary(u => u["name"], u => u);
            Assert.Equal("Float", unknowns["y"]["type"]);
            Assert.Equal("Str", unknowns["sout"]["type"]);
            var params_ = ((JArray)config["params"]).Cast<JObject>().ToDictionary(u => u["name"], u => u);
            Assert.Equal("Float", params_["b"]["type"]);
            Assert.Equal("Str", params_["s"]["type"]);
        }

        [Fact]
        public void TestExcelArray()
        {
            var excelType = Type.GetTypeFromProgID("Excel.Application");
            if (excelType == null)
            {
                Console.Out.WriteLine("Skipping " + GetCurrentMethod() + " because Excel is not installed");
                return;
            }

            string outputDir = "results/" + GetCurrentMethod();
            string petExperimentPath = "/@Testing/@ParametricExploration/@" + GetCurrentMethod();

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath);

            Assert.True(result.Item2.Success, "CyPhyPET failed.");

            var resultOutputDir = result.Item1.OutputDirectory;
            //Run run_mdao
            string stderr = "<did not start process>";
            int retcode = Run("This doesn't do anything... I think", resultOutputDir, out stderr);
            Assert.True(0 == retcode, "run_mdao failed: " + stderr);

            //Check output.csv results
            var lines = File.ReadAllLines(Path.Combine(resultOutputDir, "output.csv"));
            Assert.Equal("GUID,AnalysisError,Average,FourthValue,Array,StrArray,Sentence,Number", lines[0]);
            Assert.Equal("16.357142857142858,32.714285714285715,\"[[7.0, 0.7142857142857143, 25.0, 32.714285714285715]]\",\"[[u'This', u'that']]\",This is a lot of that.,5.0", lines[1].Substring(Guid.Empty.ToString("D").Length + ",False,".Length));
            Assert.Equal("56.416666666666664,112.83333333333333,\"[[12.0, 0.8333333333333334, 100.0, 112.83333333333333]]\",\"[[u'This', u'words.']]\",This is a lot of words..,10.0", lines[2].Substring(Guid.Empty.ToString("D").Length + ",False,".Length));
        }

        [Fact]
        public void TestExcelImport()
        {
            var excelType = Type.GetTypeFromProgID("Excel.Application");
            if (excelType == null)
            {
                Console.Out.WriteLine("Skipping " + GetCurrentMethod() + " because Excel is not installed");
                return;
            }

            MgaProject project = new MgaProject();
            project.OpenEx("MGA=" + this.mgaFile, "CyPhyML", null);
            try
            {
                var terr = project.BeginTransactionInNewTerr();

                var excel = (IMgaModel)project.ObjectByPath["/@Testing/@ParametricExploration/@TestExcel/@ExcelWrapper"];
                foreach (var fco in excel.ChildFCOs.Cast<IMgaFCO>().ToList())
                {
                    fco.DestroyObject();
                }

                var excelDS = ISIS.GME.Dsml.CyPhyML.Classes.ExcelWrapper.Cast(excel);
                string mgaDir = Path.GetDirectoryName(Path.GetFullPath(excel.Project.ProjectConnStr.Substring("MGA=".Length)));

                CyPhyPET.CyPhyPETInterpreter.CreateParametersAndMetricsForExcel(excelDS, Path.Combine(mgaDir, excelDS.Attributes.ExcelFilename));

                Dictionary<string, ISIS.GME.Dsml.CyPhyML.Interfaces.Parameter> params_ = excelDS.Children.ParameterCollection.ToDictionary(p => p.Name, p => p);
                Assert.Contains("x", params_.Keys);
                Assert.Contains("b", params_.Keys);
                Assert.Contains("s", params_.Keys);
                Assert.Equal("12", params_["x"].Attributes.Value);

                Dictionary<string, ISIS.GME.Dsml.CyPhyML.Interfaces.Metric> metrics = excelDS.Children.MetricCollection.ToDictionary(m => m.Name, m => m);
                Assert.Contains("y", metrics.Keys);
                Assert.Contains("bout", metrics.Keys);
                Assert.Contains("sout", metrics.Keys);
            }
            finally
            {
                project.AbortTransaction();
                project.Close();
            }
        }


        [Fact]
        public void OptimizationInitialConditionProfiling_OptimizationProblem()
        {
            string outputDir = "results/" + GetCurrentMethod();
            string petExperimentPath = "/@Testing/@PETHierarchy/@OptimizationInitialConditionProfiling/@OptimizationProblem";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath);

            Assert.True(result.Item2.Success, "CyPhyPET failed.");

            var configContents = File.ReadAllText(Path.Combine(result.Item1.OutputDirectory, "mdao_config.json"));
            var config = JsonConvert.DeserializeObject<AVM.DDP.PETConfig>(configContents);

            Assert.Equal(new string[] { "Optimizer", "x" }, config.components["Paraboloid"].parameters["x"].source);

            Assert.Equal(new string[] { "Paraboloid", "f_xy" }, config.drivers["Optimizer"].objectives["f_xy"].source);

            return;
        }

        [Fact]
        [Trait("Feature", "CopyFiles")]
        public void TestFileCopy()
        {
            string outputDir = "results/" + GetCurrentMethod();
            string petExperimentPath = "/@Testing/@ParametricExploration/@TestFileCopy";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath);

            Assert.True(result.Item2.Success, "CyPhyPET failed.");

            foreach (string path in new string[] {
                "component_files/23kc5zsm/empty",
                "component_files/23kc5zsm/Images/Icon.png",
                "component_files/23kc5zsm/CAD/pkg_0603.step",
            })
            {
                var fullPath = Path.Combine(Path.GetDirectoryName(mgaFile), outputDir, path);
                Assert.True(File.Exists(fullPath), String.Format("File '{0}' does not exist", fullPath));
            }
        }

        [Fact]
        [Trait("Feature", "CopyFiles")]
        public void TestFileCopyAbsolute()
        {
            string outputDir = "results/" + GetCurrentMethod();
            string petExperimentPath = "/@Testing/@ParametricExploration/@TestFileCopyAbsolute";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath, preProcess:
                (project) =>
                {
                    project.BeginTransactionInNewTerr();
                    var testFileCopy = (MgaFCO)project.RootFolder.ObjectByPath["/@Testing/@ParametricExploration/@TestFileCopy"];
                    var testFileCopy2 = testFileCopy.ParentFolder.CopyFCODisp(testFileCopy);
                    testFileCopy2.Name = "TestFileCopyAbsolute";
                    var fileCopy = (MgaFCO)testFileCopy2.ObjectByPath["/@PythonWrapper/@CopyFiles"];
                    fileCopy.SetStrAttrByNameDisp("Source", Path.GetFullPath(Path.Combine(Path.GetDirectoryName(mgaFile), fileCopy.StrAttrByName["Source"])));

                    project.CommitTransaction();
                });

            Assert.True(result.Item2.Success, "CyPhyPET failed.");

            foreach (string path in new string[] {
                "component_files/23kc5zsm/empty",
                "component_files/23kc5zsm/Images/Icon.png",
                "component_files/23kc5zsm/CAD/pkg_0603.step",
            })
            {
                var fullPath = Path.Combine(Path.GetDirectoryName(mgaFile), outputDir, path);
                Assert.True(File.Exists(fullPath), String.Format("File '{0}' does not exist", fullPath));
            }
        }

        [Fact]
        [Trait("Feature", "CopyFiles")]
        public void TestFileCopyStar()
        {
            string outputDir = "results/" + GetCurrentMethod();
            string petExperimentPath = "/@Testing/@ParametricExploration/@TestFileCopyStar";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath, preProcess:
                (project) =>
                {
                    project.BeginTransactionInNewTerr();
                    var testFileCopy = (MgaFCO)project.RootFolder.ObjectByPath["/@Testing/@ParametricExploration/@TestFileCopy"];
                    var testFileCopy2 = testFileCopy.ParentFolder.CopyFCODisp(testFileCopy);
                    testFileCopy2.Name = "TestFileCopyStar";
                    var fileCopy = (MgaFCO)testFileCopy2.ObjectByPath["/@PythonWrapper/@CopyFiles"];
                    fileCopy.SetStrAttrByNameDisp("Source", fileCopy.StrAttrByName["Source"] + "/*");

                    project.CommitTransaction();
                });

            Assert.True(result.Item2.Success, "CyPhyPET failed.");

            foreach (string path in new string[] {
                "component_files/empty",
                "component_files/Images/Icon.png",
                "component_files/CAD/pkg_0603.step",
            })
            {
                var fullPath = Path.Combine(Path.GetDirectoryName(mgaFile), outputDir, path);
                Assert.True(File.Exists(fullPath), String.Format("File '{0}' does not exist", fullPath));
            }
        }

        [Fact]
        public void SubproblemConnectedToSubproblem()
        {

            string outputDir = "results/" + GetCurrentMethod();
            string petExperimentPath = "/@Testing/@PETHierarchy/@SubproblemConnectedToSubproblem";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath);

            Assert.True(result.Item2.Success, "CyPhyPET failed.");

            var configContents = File.ReadAllText(Path.Combine(result.Item1.OutputDirectory, "mdao_config.json"));
            var config = JsonConvert.DeserializeObject<AVM.DDP.PETConfig>(configContents);

            Assert.Equal(new string[] { "sub1", "out" }, config.subProblems["sub2"].problemInputs["in"].outerSource);
        }

        [Fact]
        public void MultiplePETNestingExample()
        {
            string outputDir = "results/" + GetCurrentMethod();
            string pathPet = "/@Testing/@PETHierarchy/@" + GetCurrentMethod();
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");

            //Run CyPhyPET
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, pathPet);
            Assert.True(result.Item2.Success, "CyPhyPET failed.");
            var resultOutputDir = result.Item1.OutputDirectory;

            //Check mdao_config.json
            var configContents = File.ReadAllText(Path.Combine(result.Item1.OutputDirectory, "mdao_config.json"));
            var config = JsonConvert.DeserializeObject<AVM.DDP.PETConfig>(configContents);
            Assert.Equal(new string[] { "x" }, config.subProblems["sub"].subProblems["subsub"].problemInputs["x"].outerSource);
            Assert.Equal(new string[] { "y" }, config.subProblems["sub"].subProblems["subsub"].problemInputs["y"].outerSource);

            //Run run_mdao
            string stderr = "<did not start process>";
            int retcode = Run("This doesn't do anything... I think", resultOutputDir, out stderr);
            Assert.True(0 == retcode, "run_mdao failed: " + stderr);

            //Check output.csv results
            var lines = File.ReadAllLines(Path.Combine(resultOutputDir, "output.csv"));
            Assert.Equal("GUID,AnalysisError,x,y,f_xy", lines[0]);

            double[] expectedFxy = new double[] { 7422.0, 2822.0, 3222.0, 2122.0, 22.0, 2922.0, 1822.0, 2222.0, 7622.0 };
            int index = 0;
            const double Epsilon = 0.0001;
            foreach (var line in lines.Skip(1))
            {
                var values = line.Split(',');
                Assert.True((Math.Abs(Convert.ToDouble(values[4]) - expectedFxy[index])) < Epsilon, "output.csv 'f_xy' fields don't match expected");
                index++;
            }

            return;
        }

        [Fact]
        public void DuplicatePETNamesExample()
        {
            string outputDir = "results/" + GetCurrentMethod();
            string pathPet = "/@Testing/@PETHierarchy/@" + GetCurrentMethod();
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");

            //Run CyPhyPET
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, pathPet);
            Assert.False(result.Item2.Success, "CyPhyPET did not fail as expected. It should produce a Duplicate names error in the MGE Console.");

            return;
        }

        [Fact]
        public void StringEnumDriver()
        {
            string outputDir = "results/" + GetCurrentMethod();
            string petExperimentPath = "/@Testing/@PETHierarchy/@StringEnumDriver";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath);

            Assert.True(result.Item2.Success, "CyPhyPET failed.");

            var configContents = File.ReadAllText(Path.Combine(result.Item1.OutputDirectory, "mdao_config.json"));
            var config = JsonConvert.DeserializeObject<AVM.DDP.PETConfig>(configContents);

            Assert.Equal("u'one'", config.subProblems["ParametricExploration"].problemInputs["ProblemInput"].value);
            Assert.Equal(true, config.subProblems["ParametricExploration"].problemInputs["ProblemInput"].pass_by_obj);

            Assert.Equal(petExperimentPath.Replace("@", ""), config.PETName);
        }

        [Fact]
        public void PythonMetricSyntax()
        {
            string outputDir = "results/" + GetCurrentMethod();
            string petExperimentPath = "/@Testing/@PETHierarchy/@PythonMetricSyntax";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath);

            Assert.True(result.Item2.Success, "CyPhyPET failed.");

            var configContents = File.ReadAllText(Path.Combine(result.Item1.OutputDirectory, "mdao_config.json"));
            var config = JsonConvert.DeserializeObject<AVM.DDP.PETConfig>(configContents);

            Assert.Equal("u''", config.subProblems["Subproblem"].problemInputs["a"].value);
            Assert.Equal(true, config.subProblems["Subproblem"].problemInputs["a"].pass_by_obj);

            Assert.Equal(petExperimentPath.Replace("@", ""), config.PETName);
        }

        [Fact]
        public void ConstantStrings()
        {
            string outputDir = "results/" + GetCurrentMethod();
            string petExperimentPath = "/@Testing/@PETHierarchy/@" + GetCurrentMethod();

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath);

            Assert.True(result.Item2.Success, "CyPhyPET failed.");

            var configContents = File.ReadAllText(Path.Combine(result.Item1.OutputDirectory, "mdao_config.json"));
            var config = JsonConvert.DeserializeObject<AVM.DDP.PETConfig>(configContents);

            Assert.Equal("Constant", config.components["Constants"].unknowns["AString"].value);
            Assert.Equal("u'Constant'", config.subProblems["SubPET"].problemInputs["ProblemInput"].value);

            Assert.Equal((new double[] { 1, 2, 3 }).ToArray(), ((JArray)config.components["Constants"].unknowns["AnArray"].value).Select(x => (double)x).ToArray());
            Assert.Equal("[1, 2, 3, ]", config.subProblems["SubPET"].problemInputs["ArrProblemInput"].value);
        }


        [Fact]
        public void Test_CyPhyPET_unit_matcher()
        {
            var project = new MgaProject();
            project.OpenEx("MGA=" + this.mgaFile, "CyPhyML", null);
            try
            {
                project.BeginTransactionInNewTerr();
                try
                {
                    var wrapper = project.RootFolder.ObjectByPath["/@Testing/@ParametricExploration/@TestPython/@PythonWrapper"];
                    var pythonBlock = ISIS.GME.Dsml.CyPhyML.Classes.PythonWrapper.Cast(wrapper);

                    var binDir = Path.GetDirectoryName(System.Reflection.Assembly.GetAssembly(typeof(Workflow_PET_Test)).CodeBase.Substring("file:///".Length));
                    var path = Path.GetFullPath(binDir + "/../../paraboloid.py");
                    var pet = CyPhyPET.CyPhyPETInterpreter.GetParamsAndUnknownsForPythonOpenMDAO(path, pythonBlock);
                    
                    // "{\"unknowns\": {\"f_xy\": {\"units\": \"m**3\", \"gme_unit_id\": \"id-0065-00000179\", \"shape\": 1, \"val\": 0.0, \"size\": 1}}, \"params\": {\"x\": {\"units\": \"m\", \"gme_unit_id\": \"id-0066-00000014\", \"shape\": 1, \"val\": 0.0, \"size\": 1}, \"y\": {\"units\": \"m**3\", \"gme_unit_id\": \"id-0065-00000179\", \"shape\": 1, \"val\": 0.0, \"size\": 1}}}"
                    var unknowns = pet["unknowns"];
                    var params_ = pet["params"];
                    var x = params_["x"];
                    var y = params_["y"];
                    var f_xy = unknowns["f_xy"];
                    Assert.Equal("Meter", project.GetFCOByID((string)x["gme_unit_id"]).Name);
                    Assert.Equal("Cubic Meter", project.GetFCOByID((string)y["gme_unit_id"]).Name);
                    Assert.Equal("Cubic Meter", project.GetFCOByID((string)f_xy["gme_unit_id"]).Name);
                }
                finally
                {
                    project.CommitTransaction();
                }
            }
            finally
            {
                project.Close(abort: true);
            }
        }

        [Fact]
        public void UnitsMatch()
        {
            string outputDir = "results/" + GetCurrentMethod();
            string petExperimentPath = "/@Testing/@ParametricExploration/@UnitsMatch";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath);

            Assert.True(result.Item2.Success, "CyPhyPET failed.");
        }


        [Fact]
        public void UnitsDoNotMatch()
        {
            string outputDir = "results/" + GetCurrentMethod();
            string petExperimentPath = "/@Testing/@ParametricExploration/@UnitsDoNotMatch";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var logger = new SmartLogger();
            var stringWriter = new StringWriter();
            logger.AddWriter(stringWriter);
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath, logger);
            stringWriter.Flush();
            var loggerContents = stringWriter.GetStringBuilder().ToString();

            Assert.Contains("must match unit for ", loggerContents);
            Assert.False(result.Item2.Success, "CyPhyPET should have failed.");
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        public string GetCurrentMethod()
        {
            StackTrace st = new StackTrace();
            StackFrame sf = st.GetFrame(1);

            return sf.GetMethod().Name;
        }

        // From http://stackoverflow.com/a/28557035
        private static JObject SortPropertiesAlphabetically(JObject original)
        {
            var result = new JObject();

            foreach (var property in original.Properties().ToList().OrderBy(p => p.Name))
            {
                var value = property.Value as JObject;

                if (value != null)
                {
                    value = SortPropertiesAlphabetically(value);
                    result.Add(property.Name, value);
                }
                else
                {
                    result.Add(property.Name, property.Value);
                }
            }

            return result;
        }

        [Fact]
        public void Constants()
        {
            var path_pet = "/@Testing/@ParametricExploration/@TestConstants";
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull("results/" + GetCurrentMethod(), this.mgaFile, path_pet);

            var path_outdir = result.Item1.OutputDirectory;

            var path_mdao_config = Path.Combine(path_outdir, "mdao_config.json");
            var config = Newtonsoft.Json.Linq.JObject.Parse(File.ReadAllText(path_mdao_config));
            var constants = config["components"]["Constants"];
            Assert.Equal("IndepVarComp", constants["type"]);

            var json_expected = Newtonsoft.Json.Linq.JObject.Parse(@"{
                    'parameters': {},
                    'unknowns': {
                        'Array': {
                            'units': '1e-09*m**2*kg/s**2/A**2',
                            'value': [
                            1.0,
                            2.2,
                            3.3
                                ]
                        },
                        'Float': {
                            'units': 'A*J**-1',
                            'value': 3.2
                        },
                        'Integer': {
                            'units': '4046.8564224*m**2',
                            'value': 4
                        },
                        'String': {
                            'value': 'AString'
                        }
                    },
                    'details': null,
                    'type': 'IndepVarComp'
                }");

            Assert.Equal(JsonConvert.SerializeObject(SortPropertiesAlphabetically(json_expected)),
                         JsonConvert.SerializeObject(SortPropertiesAlphabetically((JObject)constants)));

            Assert.Equal("/Testing/ParametricExploration/TestConstants", (string)config["PETName"]);
        }

        [Fact]
        public void NoDesignVariables()
        {
            string outputDir = "results/" + GetCurrentMethod();
            string petExperimentPath = "/@Testing/@ParametricExploration/@" + GetCurrentMethod();

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath);

            Assert.True(result.Item2.Success, "CyPhyPET failed.");

            var configContents = File.ReadAllText(Path.Combine(result.Item1.OutputDirectory, "mdao_config.json"));
            var config = JsonConvert.DeserializeObject<AVM.DDP.PETConfig>(configContents);

            Assert.Equal("num_samples = 1", config.drivers["ParameterStudy"].details["Code"]);
            Assert.Equal("Uniform", config.drivers["ParameterStudy"].details["DOEType"]);
        }

        [Fact]
        public void Constants_fail_array()
        {
            var path_pet = "/@Testing/@ParametricExploration/@TestConstants_fail_array";
            var test_name = GetCurrentMethod();
            Assert.Throws<Newtonsoft.Json.JsonReaderException>(delegate
            {
                DynamicsTeamTest.CyPhyPETRunner.RunReturnFull("results/" + test_name, this.mgaFile, path_pet);
            });
        }

        [Fact]
        public void Constants_fail_float()
        {
            var path_pet = "/@Testing/@ParametricExploration/@TestConstants_fail_float";
            var test_name = GetCurrentMethod();
            Assert.Throws<System.FormatException>(delegate
            {
                DynamicsTeamTest.CyPhyPETRunner.RunReturnFull("results/" + test_name, this.mgaFile, path_pet);
            });
        }

        [Fact]
        public void Constants_fail_string()
        {
            var path_pet = "/@Testing/@ParametricExploration/@TestConstants_fail_string";
            var test_name = GetCurrentMethod();
            Assert.Throws<Newtonsoft.Json.JsonReaderException>(delegate
            {
                DynamicsTeamTest.CyPhyPETRunner.RunReturnFull("results/" + test_name, this.mgaFile, path_pet);
            });
        }

        [Fact]
        public void Support_FEA_Test_Bench()
        {
            var path_pet = "/@Testing/@ParametricExploration/@TestFEA";
            var test_name = GetCurrentMethod();
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull("results/" + test_name, this.mgaFile, path_pet);
            Assert.True(result.Item2.Success);
        }

        [Fact]
        public void Support_FEA_Test_Bench_Via_MasterInterpreter()
        {
            //string outputDir = "BallisticTestBench_BallisticHasTestInjectionPoint_Invalid";
            string objectAbsPath = "/@Testing/@ParametricExploration/@TestFEA";
            string configAbsPath = "/@Designs/@Box";

            var success = CyPhyMasterInterpreterRunner.RunMasterInterpreter(
                projectPath: this.mgaFile,
                absPath: objectAbsPath,
                configPath: configAbsPath,
                postToJobManager: false,
                keepTempModels: false);

            Assert.True(success, "CyPhyMasterInterpreter run should have succeeded, but did not.");
        }

        [Fact]
        public void PET_File_Recorder_via_MasterInterpreter()
        {
            string objectAbsPath = "/@Testing/@ParametricExploration/@TestFileRecorder";

            //Run CyPhyPET
            var result = CyPhyMasterInterpreterRunner.RunMasterInterpreterAndReturnResults(
                projectPath: this.mgaFile,
                absPath: objectAbsPath,
                configPath: objectAbsPath,
                postToJobManager: false,
                keepTempModels: false);

            Assert.True(result.Success, "CyPhyMasterInterpreter run should have succeeded, but did not.");
            var outputDir = result.OutputDirectory;

            //Run run_mdao
            string stderr = "<did not start process>";
            int retcode = Run("Unused Parameter?", outputDir, out stderr);
            Assert.True(0 == retcode, "run_mdao failed: " + stderr);

            //Compare values in 'output.csv' to 'file_x.txt'
            var lines = File.ReadAllLines(Path.Combine(outputDir, "output.csv"));
            Assert.Equal("GUID,AnalysisError,x", lines[0]);
            foreach (var line in lines.Skip(1))
            {
                var values = line.Split(',');
                var filepath = Path.Combine(outputDir, "artifacts", values[0], "file_x.txt");
                Assert.Equal(File.ReadAllLines(filepath)[0], values[2]); // "Values didn't match between 'output.csv' and 'file_x.txt'."
            }
        }

        [Fact]
        public void PET_OptimizerConstraintIntermediate_via_MasterInterpreter()
        {
            string objectAbsPath = "/@Testing/@ParametricExploration/@TestOptimizerIntermediateValueConstraint";

            //Run CyPhyPET
            var result = CyPhyMasterInterpreterRunner.RunMasterInterpreterAndReturnResults(
                projectPath: this.mgaFile,
                absPath: objectAbsPath,
                configPath: objectAbsPath,
                postToJobManager: false,
                keepTempModels: false);

            Assert.True(result.Success, "CyPhyMasterInterpreter run should have succeeded, but did not.");
            var outputDir = result.OutputDirectory;

            //Check mdao_config.json
            var configContents = File.ReadAllText(Path.Combine(outputDir, "mdao_config.json"));
            var config = JsonConvert.DeserializeObject<AVM.DDP.PETConfig>(configContents);

            Assert.Equal(config.drivers["Optimizer"].constraints["constraint"].source[0], "Paraboloid");
            Assert.Equal(config.drivers["Optimizer"].constraints["constraint"].source[1], "f_xy");
            Assert.Equal(config.drivers["Optimizer"].intermediateVariables["z"].source[0], "Add2");
            Assert.Equal(config.drivers["Optimizer"].intermediateVariables["z"].source[1], "y");

            ////Run run_mdao
            string stderr = "<did not start process>";
            int retcode = Run(null, outputDir, out stderr);
            Assert.True(0 == retcode, "run_mdao failed: " + stderr);

            //Check output.csv results
            var lines = File.ReadAllLines(Path.Combine(outputDir, "output.csv"));
            Assert.Equal("GUID,AnalysisError,z,f_xy,constraint,x,y", lines[0]);

            //Check final optimized answer
            var final_values = lines[lines.Count() - 1].Split(',');
            Assert.Equal(6.6667, Double.Parse(final_values[2]), 4);
            Assert.Equal(-27.3333, Double.Parse(final_values[3]), 4);
            Assert.Equal(-27.3333, Double.Parse(final_values[4]), 4);
            Assert.Equal(4.6667, Double.Parse(final_values[5]), 4);
            Assert.Equal(-7.3333, Double.Parse(final_values[6]), 4);
        }



        [Fact]
        public void Test__TestTestBench_With_Files__MasterInterpreter()
        {
            string objectAbsPath = "/@Testing/@ParametricExploration/@TestTestBench_With_Files";
            string configAbsPath = "/@Designs/@SimpleSystem";

            var result = CyPhyMasterInterpreterRunner.RunMasterInterpreterAndReturnResults(
                projectPath: this.mgaFile,
                absPath: objectAbsPath,
                configPath: configAbsPath,
                postToJobManager: false,
                keepTempModels: false);

            Assert.True(result.Success, "CyPhyMasterInterpreter run should have succeeded, but did not.");

            var configContents = File.ReadAllText(Path.Combine(result.OutputDirectory, "mdao_config.json"));
            var config = JsonConvert.DeserializeObject<AVM.DDP.PETConfig>(configContents);

            Assert.Equal(objectAbsPath.Replace("@", ""), config.PETName);
        }

        public void SetFixture(WorkFlow_PETFixture data)
        {
            mgaFile = Path.GetFullPath(data.mgaFile);
        }

        public int Run(string runCommand, string outputDir, out string stderr)
        {
            ProcessStartInfo info = new ProcessStartInfo()
            {
                FileName = VersionInfo.PythonVEnvExe,
                WorkingDirectory = outputDir,
                Arguments = "-E -m run_mdao mdao_config.json",
                UseShellExecute = false,
                CreateNoWindow = true,
                RedirectStandardError = true,
                RedirectStandardOutput = true
            };

            Process proc = new Process();
            proc.StartInfo = info;

            proc.Start();
            string err = "";

            proc.ErrorDataReceived += ((sender, e) =>
            {
                if (e.Data != null)
                {
                    try
                    {
                        err = err + e.Data;
                    }
                    catch (System.ObjectDisposedException)
                    {
                    }
                }
            });
            proc.BeginErrorReadLine();
            var stdout = proc.StandardOutput.ReadToEnd();
            proc.WaitForExit();
            stdout = stdout.Replace("\r", "");
            stderr = err.Replace("\r", "");

            return proc.ExitCode;

        }


        static void Main(string[] args)
        {
            int ret = Xunit.ConsoleClient.Program.Main(new string[]
            {
                System.Reflection.Assembly.GetAssembly(typeof(Workflow_PET_Test)).CodeBase.Substring("file:///".Length),
                //"/noshadow",
                // [Trait("THIS", "ONE")]
                // "/trait", "Feature=PythonWrapper",
                //"/trait", "THIS=ONE",
                //"/trait", "Type=Development", // Only run test(s) currently under development -- Those decorated with: [Trait("Type", "Development")]
            });
            Console.In.ReadLine();
            //System.Console.Out.WriteLine("HEllo World");
        }
    }
}
