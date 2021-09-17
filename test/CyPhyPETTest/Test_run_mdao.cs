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
using System.Runtime.InteropServices;

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

        [DllImport("CyPhyPython.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern double GetDoubleValue([MarshalAs(UnmanagedType.BStr)] string expression, [Out][MarshalAs(UnmanagedType.BStr)]out string error);

        private void AssertDoublePythonRoundtrip(double val)
        {
            string formatted = CyPhyPET.PET.FormatDoubleForPython(val);
            string error;
            double valFromPython = GetDoubleValue(formatted, out error);
            if (string.IsNullOrEmpty(error) == false)
            {
                throw new ApplicationException(error);
            }
            Assert.Equal(val, valFromPython);
        }

        [Fact]
        [Trait("Feature", "PythonParsing")]
        public void TestPythonDoubleGeneration()
        {
            object cyPhyPython = Activator.CreateInstance(Type.GetTypeFromProgID("MGA.Interpreter.CyPhyPython"));

            foreach (double value in double_tests)
            {
                AssertDoublePythonRoundtrip(value);
            }
            foreach (double value in tcl_suite)
            {
                AssertDoublePythonRoundtrip(value);
            }
            GC.KeepAlive(cyPhyPython);
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
                System.Reflection.Assembly.GetAssembly(typeof(Test_run_mdao)).CodeBase.Substring("file:///".Length),
                //"/noshadow",
                // [Trait("THIS", "ONE")]
                // "/trait", "Feature=PythonWrapper",
                //"/trait", "THIS=ONE",
                //"/trait", "Type=Development", // Only run test(s) currently under development -- Those decorated with: [Trait("Type", "Development")]
            });
            Console.In.ReadLine();
            //System.Console.Out.WriteLine("HEllo World");
        }

        double[] double_tests = new double[] {
            0.0001,
            0.001,
            0.01,
            0.1,
            1,
            1.0001,
            1.1,
            10,
            111,
            100,
            Double.NegativeInfinity,
            Double.PositiveInfinity,
            Double.NaN,
            1.82736408912,
            182736408.912,
            18273.6408912,
            1.82736408912e21,
            1.82736408912e14,
            2.2250738585072012e-308, // http://www.oracle.com/technetwork/topics/security/alert-cve-2010-4476-305811.html
            2.2250738585072011e-308, // https://bugs.php.net/bug.php?id=53632
            9.87e+31, // http://www.exploringbinary.com/fifteen-digits-dont-round-trip-through-sqlite-reals/
            9.944932e+31,
            8.948471e+15,
            1.23456789012345e+300,
            1.23456e-300,
            0.000001,
        };
        double[] tcl_suite = new double[]
            /*
            # From http://bugs.python.org/issue1580
            # These numbers are used to test floating point binary-to-decimal conversion.
            # They are based on the TCL test suite (tests/expr.test), which is based on
            # test data from:
            # Brigitte Verdonk, Annie Cuyt, Dennis Verschaeren, A precision and range
            # independent tool for testing floating-point arithmetic II: Conversions,
            # ACM Transactions on Mathematical Software 27:2 (March 2001), pp. 119-140.
            */
            {
            0E0,
            -0E0,
            1E0,
            15E-1,
            125E-2,
            1125E-3,
            10625E-4,
            103125E-5,
            1015625E-6,
            10078125E-7,
            100390625E-8,
            1001953125E-9,
            10009765625E-10,
            100048828125E-11,
            1000244140625E-12,
            10001220703125E-13,
            100006103515625E-14,
            1000030517578125E-15,
            10000152587890625E-16,
            +8E153,
            -1E153,
            +9E306,
            -2E153,
            +7E-304,
            -3E-49,
            +7E-303,
            -6E-49,
            +9E43,
            -9E44,
            +8E303,
            -1E303,
            +7E-287,
            -2E-204,
            +2E-205,
            -9E-47,
            +34E195,
            -68E195,
            +85E194,
            -67E97,
            +93E-234,
            -19E-87,
            +38E-87,
            -38E-88,
            -69E220,
            +18E43,
            -36E43,
            +61E-99,
            -43E-92,
            +86E-92,
            -51E-74,
            +283E85,
            -566E85,
            +589E187,
            -839E143,
            -744E-234,
            +930E-235,
            -186E-234,
            +604E175,
            -302E175,
            +755E174,
            -151E175,
            +662E-213,
            -408E-74,
            +510E-75,
            +6782E55,
            -2309E92,
            +7963E34,
            -3391E55,
            +7903E-96,
            -7611E-226,
            +4907E-196,
            -5547E-311,
            +5311E241,
            -5311E243,
            +5311E242,
            +9269E-45,
            -8559E-289,
            +8699E-276,
            -8085E-64,
            +74819E201,
            -82081E41,
            +51881E37,
            -55061E157,
            +77402E-215,
            -33891E-92,
            +38701E-215,
            -82139E-76,
            +75859E25,
            +89509E140,
            -57533E287,
            +46073E-32,
            -92146E-32,
            +83771E-74,
            -34796E-276,
            +584169E229,
            +164162E41,
            -328324E41,
            +209901E-11,
            -419802E-11,
            +940189E-112,
            -892771E-213,
            +757803E120,
            -252601E120,
            +252601E121,
            -505202E120,
            +970811E-264,
            -654839E-60,
            +289767E-178,
            -579534E-178,
            -8823691E130,
            +9346704E229,
            -1168338E229,
            -6063369E-136,
            +3865421E-225,
            -5783893E-127,
            +2572231E223,
            -5144462E223,
            +1817623E109,
            +6431543E-97,
            -5444097E-21,
            +8076999E-121,
            -9997649E-270,
            +50609263E157,
            +70589528E130,
            -88236910E129,
            +87575437E-310,
            -23135572E-127,
            +85900881E177,
            -84863171E113,
            +68761586E232,
            -50464069E286,
            +27869147E-248,
            -55738294E-248,
            +70176353E-53,
            -80555086E-32,
            -491080654E121,
            +526250918E287,
            -245540327E121,
            -175150874E-310,
            +350301748E-310,
            -437877185E-311,
            +458117166E52,
            -916234332E52,
            +229058583E52,
            -525789935E98,
            +282926897E-227,
            -565853794E-227,
            +667284113E-240,
            -971212611E-126,
            +9981396317E-182,
            -5035231965E-156,
            +8336960483E-153,
            -8056371144E-155,
            +6418488827E79,
            -3981006983E252,
            +7962013966E252,
            -4713898551E261,
            +8715380633E-58,
            -9078555839E-109,
            +9712126110E-127,
            +42333842451E201,
            -84667684902E201,
            +23792120709E-315,
            -78564021519E-227,
            +71812054883E-188,
            -30311163631E-116,
            +71803914657E292,
            +36314223356E-109,
            +18157111678E-109,
            -45392779195E-110,
            +778380362293E218,
            -685763015669E280,
            +952918668151E70,
            -548357443505E32,
            +384865004907E-285,
            -769730009814E-285,
            +697015418417E-93,
            -915654049301E-28,
            +178548656339E169,
            -742522891517E259,
            +742522891517E258,
            -357097312678E169,
            -3113521449172E218,
            +3891901811465E217,
            -1556760724586E218,
            +9997878507563E-195,
            -7247563029154E-319,
            +3623781514577E-319,
            -3092446298323E-200,
            +6363857920591E145,
            -8233559360849E94,
            +2689845954547E49,
            -5379691909094E49,
            +5560322501926E-301,
            -7812878489261E-179,
            +8439398533053E-256,
            -2780161250963E-301,
            -87605699161665E155,
            -17521139832333E156,
            -88218101363513E-170,
            +38639244311627E-115,
            +35593959807306E261,
            -53390939710959E260,
            +71187919614612E261,
            -88984899518265E260,
            +77003665618895E-73,
            -15400733123779E-72,
            +61602932495116E-72,
            -30801466247558E-72,
            +834735494917063E-300,
            -589795149206434E-151,
            +475603213226859E-42,
            -294897574603217E-151,
            +850813008001913E93,
            -203449172043339E185,
            +406898344086678E185,
            -813796688173356E185,
            +6045338514609393E244,
            -5145963778954906E142,
            +2572981889477453E142,
            -6965949469487146E74,
            +6182410494241627E-119,
            -8510309498186985E-277,
            +6647704637273331E-212,
            -2215901545757777E-212,
            +3771476185376383E276,
            -3729901848043846E212,
            +3771476185376383E277,
            -9977830465649166E119,
            +8439928496349319E-142,
            -8204230082070882E-59,
            +8853686434843997E-244,
            -5553274272288559E-104,
            +36149023611096162E144,
            -36149023611096162E147,
            +18074511805548081E146,
            -18074511805548081E147,
            +97338774138954421E-290,
            -88133809804950961E-308,
            +94080055902682397E-243,
            -24691002732654881E-115,
            +52306490527514614E49,
            -26153245263757307E49,
            +55188692254193604E165,
            -68985865317742005E164,
            +27176258005319167E-261,
            -73169230107256116E-248,
            +91461537634070145E-249,
            -54352516010638334E-261,
            +586144289638535878E280,
            -601117006785295431E245,
            +293072144819267939E280,
            -953184713238516652E272,
            +902042358290366539E-281,
            -557035730189854663E-294,
            +902042358290366539E-280,
            -354944100507554393E-238,
            +272104041512242479E199,
            -816312124536727437E199,
            +544208083024484958E199,
            -792644927852378159E78,
            -679406450132979175E-263,
            +543525160106383340E-262,
            +7400253695682920196E215,
            -1850063423920730049E215,
            +3700126847841460098E215,
            -9250317119603650245E214,
            +8396094300569779681E-252,
            -3507665085003296281E-75,
            +7015330170006592562E-75,
            -7015330170006592562E-74,
            +7185620434951919351E205,
            -1360520207561212395E198,
            +2178999185345151731E-184,
            -8691089486201567102E-218,
            +4345544743100783551E-218,
            -4357998370690303462E-184,
            +59825267349106892461E177,
            -62259110684423957791E47,
            +58380168477038565599E265,
            -62259110684423957791E48,
            -33584377202279118724E-252,
            -57484963479615354808E205,
            +71856204349519193510E204,
            -14371240869903838702E205,
            +36992084760177624177E-318,
            -73984169520355248354E-318,
            +99257763227713890244E-115,
            -87336362425182547697E-280,
            +7E289,
            -3E153,
            +6E153,
            -5E243,
            +7E-161,
            -7E-172,
            +8E-63,
            -7E-113,
            +8E126,
            -4E126,
            +5E125,
            -1E126,
            +8E-163,
            -1E-163,
            +2E-163,
            -4E-163,
            +51E195,
            -37E46,
            +74E46,
            -56E289,
            +69E-145,
            -70E-162,
            +56E-161,
            -21E-303,
            +34E-276,
            -68E-276,
            +85E-277,
            -87E-274,
            +829E102,
            -623E100,
            +723E-162,
            -457E-102,
            +914E-102,
            -323E-135,
            +151E176,
            -302E176,
            +921E90,
            -604E176,
            +823E-206,
            -463E-114,
            +348E-274,
            +9968E100,
            -6230E99,
            +1246E100,
            +6676E-296,
            -8345E-297,
            +1669E-296,
            -3338E-296,
            +3257E58,
            -6514E58,
            +2416E176,
            +8085E-63,
            -3234E-62,
            +1617E-62,
            -6468E-62,
            +53418E111,
            -60513E160,
            +26709E111,
            -99447E166,
            +12549E48,
            -25098E48,
            +50196E48,
            -62745E47,
            +83771E-73,
            -97451E-167,
            +86637E-203,
            -75569E-254,
            +473806E83,
            -947612E83,
            +292369E76,
            -584738E76,
            +933587E-140,
            -720919E-14,
            +535001E-149,
            -890521E-235,
            +548057E81,
            -706181E88,
            +820997E106,
            -320681E63,
            +928609E-261,
            -302276E-254,
            +151138E-254,
            +4691773E45,
            -9383546E45,
            +3059949E-243,
            -6119898E-243,
            +5356626E-213,
            -4877378E-199,
            +7716693E223,
            -5452869E109,
            +4590831E156,
            -9181662E156,
            -3714436E-261,
            +4643045E-262,
            -7428872E-261,
            +52942146E130,
            -27966061E145,
            +26471073E130,
            -55932122E145,
            +95412548E-99,
            -47706274E-99,
            +23853137E-99,
            -78493654E-301,
            +65346417E29,
            -51083099E167,
            +89396333E264,
            -84863171E114,
            +59540836E-251,
            -74426045E-252,
            +14885209E-251,
            -29770418E-251,
            +982161308E122,
            -245540327E122,
            +491080654E122,
            +525452622E-310,
            -771837113E-134,
            +820858081E-150,
            -262726311E-310,
            +923091487E209,
            -653777767E273,
            +842116236E-53,
            -741111169E-202,
            +839507247E-284,
            -951487269E-264,
            -9821613080E121,
            +6677856011E-31,
            -3573796826E-266,
            +7147593652E-266,
            -9981396317E-181,
            +3268888835E272,
            -2615111068E273,
            +1307555534E273,
            +2990671154E-190,
            -1495335577E-190,
            +5981342308E-190,
            -7476677885E-191,
            +82259684194E-202,
            -93227267727E-49,
            +41129842097E-202,
            -47584241418E-314,
            -79360293406E92,
            +57332259349E225,
            -57202326162E111,
            +86860597053E-206,
            -53827010643E-200,
            +53587107423E-61,
            +635007636765E200,
            +508006109412E201,
            -254003054706E201,
            +561029718715E-72,
            -897647549944E-71,
            +112205943743E-71,
            -873947086081E-236,
            +809184709177E116,
            -573112917422E81,
            +286556458711E81,
            +952805821491E-259,
            -132189992873E-44,
            -173696038493E-144,
            +1831132757599E-107,
            -9155663787995E-108,
            +7324531030396E-107,
            -9277338894969E-200,
            +8188292423973E287,
            -5672557437938E59,
            +2836278718969E59,
            -9995153153494E54,
            +9224786422069E-291,
            -3142213164987E-294,
            +6284426329974E-294,
            -8340483752889E-301,
            +67039371486466E89,
            -62150786615239E197,
            +33519685743233E89,
            -52563419496999E156,
            +32599460466991E-65,
            -41010988798007E-133,
            +65198920933982E-65,
            -82021977596014E-133,
            +80527976643809E61,
            -74712611505209E158,
            +53390939710959E261,
            -69277302659155E225,
            +46202199371337E-72,
            -23438635467783E-179,
            +41921560615349E-67,
            -92404398742674E-72,
            +738545606647197E124,
            -972708181182949E117,
            -837992143580825E87,
            +609610927149051E-255,
            -475603213226859E-41,
            +563002800671023E-177,
            -951206426453718E-41,
            +805416432656519E202,
            -530658674694337E159,
            +946574173863918E208,
            -318329953318553E113,
            -462021993713370E-73,
            +369617594970696E-72,
            +3666156212014994E233,
            -1833078106007497E233,
            +8301790508624232E174,
            -1037723813578029E174,
            +7297662880581139E-286,
            -5106185698912191E-276,
            +7487252720986826E-165,
            -3743626360493413E-165,
            +3773057430100257E230,
            -7546114860200514E230,
            +4321222892463822E58,
            -7793560217139653E51,
            +26525993941010681E112,
            -53051987882021362E112,
            +72844871414247907E77,
            -88839359596763261E105,
            +18718131802467065E-166,
            -14974505441973652E-165,
            +73429396004640239E106,
            -58483921078398283E57,
            +41391519190645203E165,
            -82783038381290406E165,
            +58767043776702677E-163,
            -90506231831231999E-129,
            +64409240769861689E-159,
            -77305427432277771E-190,
            +476592356619258326E273,
            -953184713238516652E273,
            +899810892172646163E283,
            -929167076892018333E187,
            +647761278967534239E-312,
            -644290479820542942E-180,
            +926145344610700019E-225,
            -958507931896511964E-246,
            +272104041512242479E200,
            -792644927852378159E79,
            +544208083024484958E200,
            -929963218616126365E290,
            +305574339166810102E-219,
            -152787169583405051E-219,
            +611148678333620204E-219,
            -763935847917025255E-220,
            +7439550220920798612E158,
            -3719775110460399306E158,
            +9299437776150998265E157,
            -7120190517612959703E120,
            +3507665085003296281E-73,
            -7015330170006592562E-73,
            -6684428762278255956E-294,
            -1088416166048969916E200,
            -8707329328391759328E200,
            +4439021781608558002E-65,
            -8878043563217116004E-65,
            +2219510890804279001E-65,
            +33051223951904955802E55,
            -56961524140903677624E120,
            +71201905176129597030E119,
            +14030660340013185124E-73,
            -17538325425016481405E-74,
            +67536228609141569109E-133,
            -35620497849450218807E-306,
            +66550376797582521751E-126,
            -71240995698900437614E-306,
            +3E24,
            -6E24,
            +6E26,
            -7E25,
            +1E-14,
            -2E-14,
            +4E-14,
            -8E-14,
            +5E26,
            -8E27,
            +1E27,
            -4E27,
            +9E-13,
            -7E-20,
            +56E25,
            -70E24,
            +51E26,
            +71E-17,
            -31E-5,
            +62E-5,
            -94E-8,
            +67E27,
            -81E24,
            +54E23,
            -54E25,
            +63E-22,
            -63E-23,
            +43E-4,
            -86E-4,
            +942E26,
            -471E25,
            +803E24,
            -471E26,
            -409E-21,
            +818E-21,
            -867E-8,
            +538E27,
            -857E24,
            +269E27,
            -403E26,
            +959E-7,
            -959E-6,
            +373E-27,
            -746E-27,
            +4069E24,
            -4069E23,
            -8138E24,
            +8294E-15,
            -4147E-14,
            +4147E-15,
            -8294E-14,
            +538E27,
            -2690E26,
            +269E27,
            -2152E27,
            +1721E-17,
            -7979E-27,
            +6884E-17,
            -8605E-18,
            +82854E27,
            -55684E24,
            +27842E24,
            -48959E25,
            +81921E-17,
            -76207E-8,
            +4147E-15,
            -41470E-16,
            +89309E24,
            +75859E26,
            -75859E25,
            +14257E-23,
            -28514E-23,
            +57028E-23,
            -71285E-24,
            +344863E27,
            -951735E27,
            +200677E23,
            -401354E24,
            +839604E-11,
            -209901E-11,
            +419802E-11,
            -537734E-24,
            +910308E26,
            -227577E26,
            +455154E26,
            -531013E25,
            +963019E-21,
            -519827E-13,
            +623402E-27,
            -311701E-27,
            +9613651E26,
            -9191316E23,
            +4595658E23,
            -2297829E23,
            -1679208E-11,
            +3379223E27,
            -6758446E27,
            +5444097E-21,
            -8399969E-27,
            +8366487E-16,
            -8366487E-15,
            +65060671E25,
            +65212389E23,
            +55544957E-13,
            -51040905E-20,
            +99585767E-22,
            -99585767E-23,
            +40978393E26,
            -67488159E24,
            +69005339E23,
            -81956786E26,
            -87105552E-21,
            +10888194E-21,
            -21776388E-21,
            +635806667E27,
            -670026614E25,
            +335013307E26,
            -335013307E25,
            +371790617E-24,
            -371790617E-25,
            +743581234E-24,
            -743581234E-25,
            +202464477E24,
            -404928954E24,
            +997853758E27,
            -997853758E26,
            +405498418E-17,
            -582579084E-14,
            +608247627E-18,
            -291289542E-14,
            -9537100005E26,
            +6358066670E27,
            -1271613334E27,
            +5229646999E-16,
            +5229646999E-17,
            +4429943614E24,
            -8859887228E24,
            +2214971807E24,
            -4176887093E26,
            +4003495257E-20,
            -4361901637E-23,
            +8723803274E-23,
            -8006990514E-20,
            +72835110098E27,
            -36417555049E27,
            +84279630104E25,
            -84279630104E24,
            +21206176437E-27,
            -66461566917E-22,
            +64808355539E-16,
            -84932679673E-19,
            +65205430094E26,
            -68384463429E25,
            +32602715047E26,
            -62662203426E27,
            +58784444678E-18,
            -50980203373E-21,
            +29392222339E-18,
            -75529940323E-27,
            -937495906299E26,
            +842642485799E-20,
            -387824150699E-23,
            +924948814726E-27,
            -775648301398E-23,
            +547075707432E25,
            +683844634290E24,
            -136768926858E25,
            +509802033730E-22,
            +101960406746E-21,
            -815683253968E-21,
            +7344124123524E24,
            -9180155154405E23,
            +6479463327323E27,
            -1836031030881E24,
            +4337269293039E-19,
            -4599163554373E-23,
            +9198327108746E-23,
            +4812803938347E27,
            -8412030890011E23,
            +9625607876694E27,
            -4739968828249E24,
            +9697183891673E-23,
            -7368108517543E-20,
            +51461358161422E25,
            -77192037242133E26,
            +77192037242133E25,
            -51461358161422E27,
            +43999661561541E-21,
            -87999323123082E-21,
            +48374886826137E-26,
            -57684246567111E-23,
            +87192805957686E23,
            -75108713005913E24,
            +64233110587487E27,
            -77577471133384E-23,
            +48485919458365E-24,
            -56908598265713E-26,
            +589722294620133E23,
            +652835804449289E-22,
            -656415363936202E-23,
            +579336749585745E-25,
            -381292764980839E-26,
            +965265859649698E23,
            -848925235434882E27,
            +536177612222491E23,
            -424462617717441E27,
            +276009279888989E-27,
            -608927158043691E-26,
            +552018559777978E-27,
            -425678377667758E-22,
            +8013702726927119E26,
            +8862627962362001E27,
            -5068007907757162E26,
            -7379714799828406E-23,
            +4114538064016107E-27,
            -3689857399914203E-23,
            +5575954851815478E23,
            +3395700941739528E27,
            +4115535777581961E-23,
            -8231071555163922E-23,
            +6550246696190871E-26,
            -68083046403986701E27,
            +43566388595783643E27,
            -87132777191567286E27,
            +59644881059342141E25,
            -83852770718576667E23,
            +99482967418206961E-25,
            -99482967418206961E-26,
            +87446669969994614E-27,
            -43723334984997307E-27,
            +5E24,
            -8E25,
            +1E25,
            -4E25,
            +2E-5,
            -5E-6,
            +4E-5,
            -3E-20,
            +3E27,
            -9E26,
            +7E25,
            -6E27,
            +2E-21,
            -5E-22,
            -4E-21,
            +87E25,
            -97E24,
            +82E-24,
            -41E-24,
            +76E-23,
            +83E25,
            -50E27,
            +25E27,
            -99E27,
            +97E-10,
            -57E-20,
            +997E23,
            +776E24,
            -388E24,
            +521E-10,
            -506E-26,
            +739E-10,
            -867E-7,
            -415E24,
            +332E25,
            -664E25,
            +291E-13,
            -982E-8,
            +582E-13,
            -491E-8,
            +4574E26,
            -8609E26,
            +2287E26,
            -4818E24,
            +6529E-8,
            -8151E-21,
            +1557E-12,
            -2573E-18,
            +4929E-16,
            -3053E-22,
            +9858E-16,
            -7767E-11,
            +54339E26,
            -62409E25,
            +32819E27,
            -89849E27,
            +63876E-20,
            -15969E-20,
            +31938E-20,
            -79845E-21,
            +89306E27,
            -25487E24,
            +79889E24,
            -97379E26,
            +81002E-8,
            -43149E-25,
            +40501E-8,
            -60318E-10,
            -648299E27,
            +780649E24,
            +720919E-14,
            -629703E-11,
            +557913E24,
            -847899E23,
            +565445E27,
            -736531E24,
            +680013E-19,
            -529981E-10,
            +382923E-23,
            -633614E-18,
            +2165479E27,
            -8661916E27,
            +4330958E27,
            -9391993E22,
            -5767352E-14,
            +7209190E-15,
            -1441838E-14,
            +8478990E22,
            +1473062E24,
            +8366487E-14,
            -8399969E-25,
            +9366737E-12,
            -9406141E-13,
            +65970979E24,
            -65060671E26,
            +54923002E27,
            -63846927E25,
            +99585767E-21,
            +67488159E25,
            -69005339E24,
            +81956786E27,
            -40978393E27,
            +77505754E-12,
            -38752877E-12,
            +82772981E-15,
            -95593517E-25,
            +200036989E25,
            -772686455E27,
            +859139907E23,
            -400073978E25,
            +569014327E-14,
            -794263862E-15,
            +397131931E-15,
            -380398957E-16,
            +567366773E27,
            -337440795E24,
            +134976318E25,
            -269952636E25,
            +932080597E-20,
            -331091924E-15,
            -413864905E-16,
            +8539246247E26,
            -5859139791E26,
            +6105010149E24,
            -3090745820E27,
            +3470877773E-20,
            -6136309089E-27,
            +8917758713E-19,
            -6941755546E-20,
            +9194900535E25,
            -1838980107E26,
            +7355920428E26,
            -3677960214E26,
            +8473634343E-17,
            -8870766274E-16,
            +4435383137E-16,
            -9598990129E-15,
            +71563496764E26,
            -89454370955E25,
            +17890874191E26,
            -35781748382E26,
            +57973447842E-19,
            -28986723921E-19,
            +76822711313E-19,
            -97699466874E-20,
            +67748656762E27,
            -19394840991E24,
            +38789681982E24,
            -33874328381E27,
            +54323763886E-27,
            -58987193887E-20,
            +27161881943E-27,
            -93042648033E-19,
            +520831059055E27,
            -768124264394E25,
            +384062132197E25,
            +765337749889E-25,
            +794368912771E25,
            -994162090146E23,
            +781652779431E26,
            +910077190046E-26,
            -455038595023E-26,
            +471897551096E-20,
            -906698409911E-21,
            +8854128003935E25,
            -8146122716299E27,
            +7083302403148E26,
            -3541651201574E26,
            +8394920649291E-25,
            -7657975756753E-22,
            +5473834002228E-20,
            -6842292502785E-21,
            -2109568884597E25,
            +8438275538388E25,
            -4219137769194E25,
            +3200141789841E-25,
            -8655689322607E-22,
            +6400283579682E-25,
            -8837719634493E-21,
            +19428217075297E24,
            -38856434150594E24,
            +77712868301188E24,
            -77192037242133E27,
            +76579757567530E-23,
            +15315951513506E-22,
            -38289878783765E-23,
            +49378033925202E25,
            -50940527102367E24,
            +98756067850404E25,
            -99589397544892E26,
            -56908598265713E-25,
            +97470695699657E-22,
            -35851901247343E-25,
            +154384074484266E27,
            -308768148968532E27,
            +910990389005985E23,
            +271742424169201E-27,
            -543484848338402E-27,
            +162192083357563E-26,
            -869254552770081E-23,
            +664831007626046E24,
            -332415503813023E24,
            +943701829041427E24,
            -101881054204734E24,
            +828027839666967E-27,
            -280276135608777E-27,
            +212839188833879E-21,
            -113817196531426E-25,
            +9711553197796883E27,
            -2739849386524269E26,
            +5479698773048538E26,
            +6124568318523113E-25,
            -1139777988171071E-24,
            +6322612303128019E-27,
            -2955864564844617E-25,
            -9994029144998961E25,
            -2971238324022087E27,
            -1656055679333934E-27,
            -1445488709150234E-26,
            +55824717499885172E27,
            -69780896874856465E26,
            +84161538867545199E25,
            -27912358749942586E27,
            +24711112462926331E-25,
            -12645224606256038E-27,
            -12249136637046226E-25,
            +74874448287465757E27,
            -35642836832753303E24,
            -71285673665506606E24,
            +43723334984997307E-26,
            +10182419849537963E-24,
            -93501703572661982E-26,
    };
    }
}
