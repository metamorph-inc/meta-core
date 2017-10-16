using GME.MGA;
using META;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Text;
using System.Text.RegularExpressions;
using MasterInterpreterTest;
using Newtonsoft.Json.Linq;
using Newtonsoft.Json.Serialization;
using Xunit;


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
            Dictionary<string, object> assignment = CyPhyPET.PET.getPythonAssignment("x = 1\ny = '2'");
            Assert.Equal(1L, assignment["x"]);
            Assert.Equal("2", assignment["y"]);
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
        public void TestPython()
        {
            string outputDir = "results/TestPython";
            string petExperimentPath = "/@Testing/@ParametricExploration/@TestPython";

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            var result = DynamicsTeamTest.CyPhyPETRunner.RunReturnFull(outputDir, mgaFile, petExperimentPath);

            Assert.True(result.Item2.Success, "CyPhyPET failed.");
            return;

            string stderr = "<did not start process>";
            int retcode = Run(result.Item2.RunCommand, result.Item1.OutputDirectory, out stderr);
            Assert.True(0 == retcode, "run_mdao failed: " + stderr);
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
            Assert.True(lines[0] == "GUID,x,y,f_xy", "output.csv header doesn't match expected");

            double[] expectedFxy = new double[] { 7422.0, 2822.0, 3222.0, 2122.0, 22.0, 2922.0, 1822.0, 2222.0, 7622.0 };
            int index = 0;
            const double Epsilon = 0.0001;
            foreach (var line in lines.Skip(1))
            {
                var values = line.Split(',');
                Assert.True((Math.Abs(Convert.ToDouble(values[3]) - expectedFxy[index])) < Epsilon, "output.csv 'f_xy' fields don't match expected");
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

            Assert.Equal("u\"one\"", config.subProblems["ParametricExploration"].problemInputs["ProblemInput"].value);
            Assert.Equal(true, config.subProblems["ParametricExploration"].problemInputs["ProblemInput"].pass_by_obj);

            Assert.Equal(petExperimentPath.Replace("@", ""), config.PETName);
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
            Assert.True(lines[0] == "GUID,x", "Header doesn't match.");
            foreach (var line in lines.Skip(1))
            {
                var values = line.Split(',');
                var filepath = Path.Combine(outputDir, "artifacts", values[0], "file_x.txt");
                Assert.True(File.ReadAllLines(filepath)[0] == values[1], "Values didn't match between 'output.csv' and 'file_x.txt'.");
            }
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
            mgaFile = data.mgaFile;
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
                //"/trait", "THIS=ONE",
            });
            Console.In.ReadLine();
            //System.Console.Out.WriteLine("HEllo World");
        }
    }
}
