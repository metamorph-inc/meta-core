

namespace CyPhyPET
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Diagnostics.Contracts;
    using System.IO;

    using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
    using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
    using System.Text.RegularExpressions;
    using System.Diagnostics;
    using CyPhyGUIs;

    public class PET
    {
        //public static GME.CSharp.GMEConsole GMEConsole { get; set; }
        public CyPhyGUIs.GMELogger Logger { get; set; }
        public bool Success = false;
        public string RunCommand { get; set; }
        public string Label { get; set; }

        private enum DriverType {PCC, Optimizer, ParameterStudy};
        private DriverType theDriver;
        private string driverName;
        private string testBenchType;
        private string validationDriver = "";
        private CyPhy.TestBenchType testBench;
        private CyPhy.ParametricExploration pet;
        private string outputDirectory { get; set; }
        public Dictionary<string, string> PCCPropertyInputDistributions { get; set; }

        public PET(CyPhyGUIs.IInterpreterMainParameters parameters, CyPhyGUIs.GMELogger logger)
        {
            this.Logger = logger;
            this.pet = CyPhyClasses.ParametricExploration.Cast(parameters.CurrentFCO);
            this.outputDirectory = parameters.OutputDirectory;
            this.testBench = pet.Children.TestBenchRefCollection.FirstOrDefault().Referred.TestBenchType;
            this.PCCPropertyInputDistributions = new Dictionary<string, string>();
            // Determine type of driver of the Parametric Exploration
            if (this.pet.Children.PCCDriverCollection.Count() == 1)
            {
                this.theDriver = DriverType.PCC;
                this.driverName = "PCCDriver";
            }
            else if (this.pet.Children.OptimizerCollection.Count() == 1)
            {
                this.theDriver = DriverType.Optimizer;
                this.driverName = "Optimizer";
            }
            else if (this.pet.Children.ParameterStudyCollection.Count() == 1)
            {
                this.theDriver = DriverType.ParameterStudy;
                this.driverName = "ParameterStudy";
            }
        }

        public bool GenerateCode()
        {
            Directory.CreateDirectory(Path.Combine(outputDirectory, "scripts"));

            if (this.testBench is CyPhy.TestBench)
            {
                var interpreterProgID = Rules.Global.GetInterpreterProgIDFromTestBench(this.testBench as CyPhy.TestBench);
                switch (interpreterProgID)
                {
                    case "MGA.Interpreter.CyPhy2Modelica_v2":
                        this.testBenchType = "modelica";
                        this.CyPhy2Modelica_v2();
                        break;

                    case "MGA.Interpreter.CyPhy2CAD_CSharp":
                        // Maybe also create testbench_manifest.json.
                        this.testBenchType = "cad";
                        this.CADTestBench();
                        break;

                    case "MGA.Interpreter.CyPhyFormulaEvaluator":
                        // TODO: Generate testbench_manifest.json
                        this.testBenchType = "formula";
                        this.SimpleCalculation();
                        break;

                    case "MGA.Interpreter.CyphyCar":
                        // TODO: Generate testbench_manifest.json
                        this.testBenchType = "car";
                        this.SimpleCalculation();
                        break;
                }
            }
            else if (this.testBench is CyPhy.CADTestBench)
            {
                this.testBenchType = "fea";
                this.FEATestBench();
            }
            else if (this.testBench is CyPhy.BlastTestBench)
            {
                this.testBenchType = "blast";
                this.TemplateTestBench();
            }
            else if (this.testBench is CyPhy.BallisticTestBench)
            {
                this.testBenchType = "ballistic";
                this.TemplateTestBench();
            }
            else if (this.testBench is CyPhy.CFDTestBench)
            {
                this.testBenchType = "cfd";
                this.TemplateTestBench();
            }
            else if (this.testBench is CyPhy.CarTestBench)
            {
                this.testBenchType = "car";
                this.TemplateTestBench();
            }

            // Generate Driver
            if (this.theDriver == DriverType.PCC)
            {
                this.RunCommand = "run_PCC.cmd";
                this.Label = " && PCC" + JobManager.Job.LabelVersion;
                this.GeneratePCCScripts();
            }
            else if (this.theDriver == DriverType.Optimizer)
            {
                this.RunCommand = "run_optimizer.cmd";
                this.Label = "";
                this.GenerateOptimizerScripts();
            }
            else if (this.theDriver == DriverType.ParameterStudy)
            {
                this.RunCommand = "run_parameter_study.cmd";
                this.Label = "";
                this.GenerateParameterStudyScripts();
            }

            using (StreamWriter writer = new StreamWriter(Path.Combine(this.outputDirectory, this.RunCommand)))
            {
                var runDriver = new Templates.runDriver()
                {
                    DriverName = this.driverName,
                    ValidationDriver = this.validationDriver
                };

                writer.WriteLine(runDriver.TransformText());
            }


            // TODO : When and where are these files used????

            //using (StreamWriter writer = new StreamWriter(Path.Combine(outputDirectory, "save_results.py")))
            //{
            //    writer.WriteLine(CyPhyPET.Properties.Resources.save_results);
            //}

            //using (StreamWriter writer = new StreamWriter(Path.Combine(outputDirectory, "mdao_index.html")))
            //{
            //    writer.WriteLine(CyPhyPET.Properties.Resources.index);
            //}

            using (StreamWriter writer = new StreamWriter(Path.Combine(outputDirectory, "zip.py")))
            {
                writer.WriteLine(CyPhyPET.Properties.Resources.zip);
            }

            this.Success = true;
            return this.Success;
        }

        # region DriverTypes

        /// <summary>
        /// Generates PCC scripts
        /// </summary>
        private void GeneratePCCScripts()
        {
            var pccDriverCfgFile = "PCCDriverconfigs.json";
            var PCCDriver = this.pet.Children.PCCDriverCollection.FirstOrDefault();

            var upMethod = PCCDriver.Attributes.PCC_UP_Methods;
            int upMethodNum;

            switch (upMethod)
            {
                case CyPhyClasses.PCCDriver.AttributesClass.PCC_UP_Methods_enum.Monte_Carlo_Simulation__UP_MCS_ :
                    upMethodNum = 1;
                    break;
                case CyPhyClasses.PCCDriver.AttributesClass.PCC_UP_Methods_enum.Taylor_Series_Approximation__UP_TS_:
                    upMethodNum = 2;
                    break;
                case CyPhyClasses.PCCDriver.AttributesClass.PCC_UP_Methods_enum.Most_Probable_Point_Method__UP_MPP_:
                    this.Logger.WriteWarning("The output from Most Probable Point Method is not compatible with the project analyzer Dashboard.");
                    upMethodNum = 3;
                    break;
                case CyPhyClasses.PCCDriver.AttributesClass.PCC_UP_Methods_enum.Full_Factorial_Numerical_Integration__UP_FFNI_:
                    upMethodNum = 4;
                    break;
                case CyPhyClasses.PCCDriver.AttributesClass.PCC_UP_Methods_enum.Univariate_Dimension_Reduction_Method__UP_UDR_:
                    upMethodNum = 5;
                    break;
                case CyPhyClasses.PCCDriver.AttributesClass.PCC_UP_Methods_enum.Polynomial_Chaos_Expansion__UP_PCE_:
                    this.Logger.WriteWarning("The output from Polynomial Chaos Expansion is not compatible with the project analyzer Dashboard.");
                    this.Logger.WriteWarning("Trying to display such data might require a refresh in order to view other data again.");
                    upMethodNum = 6;
                    break;
                default:
                    upMethodNum = 0;
                    break;
            }

            var saMethod = PCCDriver.Attributes.PCC_SA_Methods;
            int saMethodNum;

            switch (saMethod)
            {
                case CyPhyClasses.PCCDriver.AttributesClass.PCC_SA_Methods_enum.Sobol_method__SA_SOBOL_:
                    saMethodNum = 7;
                    break;
                case CyPhyClasses.PCCDriver.AttributesClass.PCC_SA_Methods_enum.FAST_method__SA_FAST_:
                    saMethodNum = 9;
                    break;
                case CyPhyClasses.PCCDriver.AttributesClass.PCC_SA_Methods_enum.EFAST_method__SA_EFAST_:
                    saMethodNum = 10;
                    break;
                default:
                    saMethodNum = 0;
                    break;
            }

            // Generate input config file for OSU code

            var pccConfig = this.BuildUpPCCConfiguration(driverName, PCCDriver, upMethodNum, saMethodNum);

            List<string> objectives = new List<string>();
            List<string> designVariabels = new List<string>();

            foreach (var item in pccConfig.Configurations.Configuration.PCCInputArguments.PCCMetrics)
            {
                objectives.Add("TestBench." + item.TestBenchMetricName);
            }

            foreach (var item in pccConfig.Configurations.Configuration.PCCInputArguments.StochasticInputs.InputDistributions)
            {
                designVariabels.Add("TestBench." + item.TestBenchParameterNames.FirstOrDefault());
            }

            // Add registry defied inputdistributions for Properties in Components
            var PCCPropertyInputs = TryToObtainDyanmicsProperties();
            foreach (var pccProperty in PCCPropertyInputs)
            {
                this.Logger.WriteDebug("Adding Property Input Distribution, Name : {0}, Distribution : {1}", pccProperty.Name, pccProperty.Distribution);
                // TODO: Can multiple testbench names be handled correctly? For now only use the first.
                var testBenchParameterName = pccProperty.TestBenchParameterNames.FirstOrDefault();
                pccProperty.TestBenchParameterNames.Clear();
                pccProperty.TestBenchParameterNames.Add(testBenchParameterName);
                this.PCCPropertyInputDistributions.Add(pccProperty.Name, testBenchParameterName);
                pccConfig.Configurations.Configuration.PCCInputArguments.StochasticInputs.InputDistributions.Add(pccProperty);
            }

            using (StreamWriter writer = new StreamWriter(Path.Combine(this.outputDirectory, "scripts", driverName + ".py")))
            {
                var pccOMDAO = new Templates.PCCDriver()
                {
                    DriverName = this.pet.Children.PCCDriverCollection.FirstOrDefault().Name,
                    DesignVariables = designVariabels,
                    Objectives = objectives,
                    PCCConfigJson = pccDriverCfgFile,
                    PCCPropertyInputs = this.PCCPropertyInputDistributions,
                };

                writer.WriteLine(pccOMDAO.TransformText());
            }

            using (StreamWriter writer1 = new StreamWriter(Path.Combine(this.outputDirectory, pccDriverCfgFile)))
            {
                writer1.WriteLine(Newtonsoft.Json.JsonConvert.SerializeObject(pccConfig, Newtonsoft.Json.Formatting.Indented));
            }
        }

        /// <summary>
        /// Generates Optimizer scripts
        /// </summary>
        private void GenerateOptimizerScripts()
        {
            var optimizer = this.pet.Children.OptimizerCollection.FirstOrDefault();

            var optimizerType = optimizer.Attributes.OptimizationFunction;
            string optimizerName;
            
            switch (optimizerType)
            {
                case CyPhyClasses.Optimizer.AttributesClass.OptimizationFunction_enum.COBYLA:
                    optimizerName = "COBYLAdriver";
                    break;
                case CyPhyClasses.Optimizer.AttributesClass.OptimizationFunction_enum.CONMIN:
                    optimizerName = "CONMINdriver";
                    break;
                case CyPhyClasses.Optimizer.AttributesClass.OptimizationFunction_enum.NEWSUMT:
                    optimizerName = "NEWSUMTdriver";
                    break;
                default:
                    optimizerName = "ERROR";
                    break;
            }

            using (StreamWriter writer = new StreamWriter(Path.Combine(this.outputDirectory, "scripts", driverName + ".py")))
            {
                var optimizerScript = new Templates.Optimizer()
                {
                    OptimizerType = optimizerName,
                    pet = this.pet,
                    OutputDirectory = this.outputDirectory
                };

                writer.WriteLine(optimizerScript.TransformText());
            }
        }

        /// <summary>
        /// Generate scripts for ParamterStudy and DOE
        /// </summary>
        private void GenerateParameterStudyScripts()
        {
            var parameterStudy = this.pet.Children.ParameterStudyCollection.FirstOrDefault();

            var doeType = parameterStudy.Attributes.DOEType;
            string doeName;

            switch (doeType)
            {
                case CyPhyClasses.ParameterStudy.AttributesClass.DOEType_enum.Central_Composite:
                    doeName = "CentralComposite";
                    break;

                case CyPhyClasses.ParameterStudy.AttributesClass.DOEType_enum.Full_Factorial:
                    doeName = "FullFactorial";
                    break;

                case CyPhyClasses.ParameterStudy.AttributesClass.DOEType_enum.Opt_Latin_Hypercube:
                    doeName = "LatinHypercube";
                    break;

                case CyPhyClasses.ParameterStudy.AttributesClass.DOEType_enum.Uniform:
                    doeName = "Uniform";
                    break;

                default:
                    doeName = "";
                    break;
            }

            var surrogateType = parameterStudy.Attributes.SurrogateType;
            string surrogateName = "ERROR";

            switch (surrogateType)
            {
                case CyPhyClasses.ParameterStudy.AttributesClass.SurrogateType_enum.None:
                    if (parameterStudy.Children.DesignVariableCollection.FirstOrDefault().Attributes.Range.StartsWith("["))
                    {
                        this.Logger.WriteWarning("Found disrete ranges in DesignVariables, DOE type (generator) will be ignored.");
                        var orderedParameters = new List<CyPhy.DesignVariable>();
                        foreach (var item in parameterStudy.Children.DesignVariableCollection)
                        {
                             orderedParameters.Add(item);
                        }

                        var csvDoe = new Templates.CSVParameterStudy()
                        {
                            DOEType = doeName,
                            pet = this.pet,
                            Parameters = orderedParameters
                        };

                        orderedParameters = csvDoe.Parameters;
                        var rows = new List<string>();
                        for (int i = 0; i < orderedParameters.FirstOrDefault().Attributes.Range.Split(',').Count(); i++)
                        {
                            rows.Add("");
                        }

                        foreach (var range in orderedParameters.Select(p => p.Attributes.Range.TrimStart('[').TrimEnd(']')))
                        {
                            int index = 0;
                            foreach (var row in range.Split(','))
                            {
                                rows[index] += string.Format(",{0}", row.Trim());
                                index++;
                            }
                        }

                        File.WriteAllLines(Path.Combine(this.outputDirectory, "doe_inputs.csv"), rows.Select(r => r.TrimStart(',')));
                        using (StreamWriter writer = new StreamWriter(Path.Combine(this.outputDirectory, "scripts", driverName + ".py")))
                        {
                            writer.WriteLine(csvDoe.TransformText());
                        }
                    }
                    else
                    {
                        using (StreamWriter writer = new StreamWriter(Path.Combine(this.outputDirectory, "scripts", driverName + ".py")))
                        {
                            var Doe = new Templates.ParameterStudy()
                            {
                                DOEType = doeName,
                                pet = this.pet,
                            };

                            writer.WriteLine(Doe.TransformText());
                        }
                    }

                    break;

                case CyPhyClasses.ParameterStudy.AttributesClass.SurrogateType_enum.Kriging_Surrogate:
                    surrogateName = "KrigingSurrogate";
                    goto default;

                case CyPhyClasses.ParameterStudy.AttributesClass.SurrogateType_enum.Logistic_Regression:
                    surrogateName = "LogisticRegression";
                    goto default;

                case CyPhyClasses.ParameterStudy.AttributesClass.SurrogateType_enum.Neural_Net:
                    surrogateName = "NeuralNet";
                    goto default;

                case CyPhyClasses.ParameterStudy.AttributesClass.SurrogateType_enum.Response_Surface:
                    surrogateName = "ResponseSurface";
                    goto default;

                default:
                    this.GenerateSurrogateScripts(doeName, surrogateName);
                    break;
            }

            using (StreamWriter writer = new StreamWriter(Path.Combine(this.outputDirectory, "scripts", "save_results.py")))
            {
                writer.WriteLine(CyPhyPET.Properties.Resources.save_results);
            }
        }

        private void GenerateSurrogateScripts(string doeName, string surrogateName)
        {
            using (StreamWriter writer = new StreamWriter(Path.Combine(this.outputDirectory, "scripts", "surrogate_model.py")))
            {
                var surrogateAsm = new Templates.SurrogateAssembly()
                {
                    pet = this.pet,
                };

                writer.WriteLine(surrogateAsm.TransformText());
            }

            using (StreamWriter writer = new StreamWriter(Path.Combine(this.outputDirectory, "scripts", this.driverName + ".py")))
            {
                var surrogate = new Templates.ParameterStudySurrogate()
                {
                    DOEType = doeName,
                    SurrogateType = surrogateName,
                    pet = this.pet,
                    ParameterStudyName = this.driverName,
                };

                writer.WriteLine(surrogate.TransformText());
            }

            using (StreamWriter writer = new StreamWriter(Path.Combine(this.outputDirectory, "scripts", "SurrogateModelValidation.py")))
            {
                var surrogateValidation = new Templates.SurrogateValidation()
                {
                    pet = this.pet,
                    SurrogateType = surrogateName,
                };

                writer.WriteLine(surrogateValidation.TransformText());
            }

            this.validationDriver = "SurrogateModelValidation";
        }

        #endregion

        #region TestBenchTypes

        private void CyPhy2Modelica_v2()
        {
            using (StreamWriter writer = new StreamWriter(Path.Combine(outputDirectory, "scripts", "driver_runner.py")))
            {
                writer.WriteLine(CyPhyPET.Properties.Resources.driver_runner);
            }

            this.GenerateTestBenchPy();

            using (StreamWriter writer = new StreamWriter(Path.Combine(outputDirectory, "scripts", "modelica_executor.py")))
            {
                var testBenchExecutor = new Templates.TestBenchExecutors.Modelica()
                {
                    // Put stuff here
                };

                writer.WriteLine(testBenchExecutor.TransformText());
            }

            // Configuration file setup
            var dynamicConfigFile = "test_bench_model_config.json"; // this file must end with _model_config.json
            var toolType = (this.testBench as CyPhy.TestBench).Children.SolverSettingsCollection.FirstOrDefault().Attributes.JobManagerToolSelection;
            string toolName;

            switch(toolType)
            {
                case CyPhyClasses.SolverSettings.AttributesClass.JobManagerToolSelection_enum.Dymola__latest_:
                    toolName = "Dymola";
                    break;
                case CyPhyClasses.SolverSettings.AttributesClass.JobManagerToolSelection_enum.Dymola_2013:
                    toolName = "Dymola";
                    break;
                case CyPhyClasses.SolverSettings.AttributesClass.JobManagerToolSelection_enum.Dymola_2014:
                    toolName = "Dymola";
                    break;
                case CyPhyClasses.SolverSettings.AttributesClass.JobManagerToolSelection_enum.OpenModelica__latest_:
                    toolName =  "OpenModelica";
                    break;
                case CyPhyClasses.SolverSettings.AttributesClass.JobManagerToolSelection_enum.JModelica_1_dot_12:
                    toolName = "JModelica";
                    break;
                default:
                    toolName = "Error";
                    break;
            }

            using (StreamWriter writer = new StreamWriter(Path.Combine(outputDirectory, dynamicConfigFile)))
            {
                var modelFileNameConfig = new Dictionary<string, string>()
                {
                    {"model_file_name", "TestBench" + "/" + "CyPhy"},
                    {"tool_name", toolName}
                };

                writer.WriteLine(Newtonsoft.Json.JsonConvert.SerializeObject(modelFileNameConfig, Newtonsoft.Json.Formatting.Indented));
            }
        }

        private void SimpleCalculation()
        {
            this.Logger.WriteWarning("Formula has limited support. You may experience issues during the execution.");
            using (StreamWriter writer = new StreamWriter(Path.Combine(outputDirectory, "scripts", "test_bench.py")))
            {
                Templates.TestBenchExecutors.Formula simpleCalc = new Templates.TestBenchExecutors.Formula()
                {
                    testBench = this.testBench as CyPhy.TestBench
                };

                writer.WriteLine(simpleCalc.TransformText());
            }

            using (StreamWriter writer = new StreamWriter(Path.Combine(outputDirectory, "scripts", "driver_runner.py")))
            {
                writer.WriteLine(CyPhyPET.Properties.Resources.driver_runner_simpleCalc);
            }
        }

        private void CADTestBench()
        {
            using (StreamWriter writer = new StreamWriter(Path.Combine(outputDirectory, "scripts", "driver_runner.py")))
            {
                writer.WriteLine(CyPhyPET.Properties.Resources.driver_runner);
            }

            this.GenerateTestBenchPy();

            using (StreamWriter writer = new StreamWriter(Path.Combine(outputDirectory, "scripts", "cad_executor.py")))
            {
                var testBenchExecutor = new Templates.TestBenchExecutors.CAD()
                {
                    MaxIterationExecutionTime = 2 // hours
                };

                writer.WriteLine(testBenchExecutor.TransformText());
            }
        }

        private void FEATestBench()
        {
            using (StreamWriter writer = new StreamWriter(Path.Combine(outputDirectory, "scripts", "driver_runner.py")))
            {
                writer.WriteLine(CyPhyPET.Properties.Resources.driver_runner);
            }

            this.GenerateTestBenchPy();

            using (StreamWriter writer = new StreamWriter(Path.Combine(outputDirectory, "scripts", "fea_executor.py")))
            {
                // META-3282
                //var testBenchExecutor = new Templates.TestBenchExecutors.FEA()
                var testBenchExecutor = new Templates.TestBenchExecutors.CAD()
                {
                    MaxIterationExecutionTime = 72 // hours
                };

                writer.WriteLine(testBenchExecutor.TransformText());
            }
        }

        private void TemplateTestBench()
        {
            this.Logger.WriteWarning("Will only generate an empty executor shell!");
            using (StreamWriter writer = new StreamWriter(Path.Combine(outputDirectory, "scripts", "driver_runner.py")))
            {
                writer.WriteLine(CyPhyPET.Properties.Resources.driver_runner);
            }

            this.GenerateTestBenchPy();

            using (StreamWriter writer = new StreamWriter(
                Path.Combine(outputDirectory, "scripts", string.Format("{0}_executor.py", this.testBenchType))))
            {
                var testBenchExecutor = new Templates.TestBenchExecutors.TemplateExecutor()
                {
                    // TODO: Put stuff here.
                };

                writer.WriteLine(testBenchExecutor.TransformText());
            }
        }

        private void GenerateTestBenchPy()
        {
            var wiredParameters = new List<CyPhy.Parameter>();
            var wiredMetrics = new List<CyPhy.Metric>();

            if (theDriver == DriverType.PCC)
            {
                foreach (var paramMapping in pet.Children.DriveParameterCollection)
                {
                    wiredParameters.Add(paramMapping.DstEnds.Parameter);
                }

                foreach (var metricMappping in pet.Children.PCCOutputMappingCollection)
                {
                    wiredMetrics.Add(metricMappping.SrcEnds.Metric);
                }
            }
            else
            {
                foreach (var paramMapping in pet.Children.VariableSweepCollection)
                {
                    wiredParameters.Add(paramMapping.DstEnds.Parameter);
                }

                foreach (var metricMappping in pet.Children.ObjectiveMappingCollection)
                {
                    wiredMetrics.Add(metricMappping.SrcEnds.Metric);
                }
                if (theDriver == DriverType.Optimizer)
                {
                    foreach (var metricMapping in pet.Children.ConstraintMappingCollection)
                    {
                        wiredMetrics.Add(metricMapping.SrcEnds.Metric);
                    }
                }
            }

            using (StreamWriter writer = new StreamWriter(Path.Combine(outputDirectory, "scripts", "test_bench.py")))
            {
                var testBenchTemplate = new Templates.TestBenchComponent()
                {
                    Parameters = wiredParameters,
                    Metrics = wiredMetrics,
                    TestBenchType = this.testBenchType,
                    PCCPropertyInputs = this.PCCPropertyInputDistributions
                };

                writer.WriteLine(testBenchTemplate.TransformText());
            }
        }
        #endregion

        # region HelperMethods

        private PCC.RootObject BuildUpPCCConfiguration(string driverName, CyPhy.PCCDriver PCCDriver, int upMethodNum, int saMethodNum)
        {
            PCC.RootObject rootConfig = new PCC.RootObject();
            PCC.Configurations configs = new PCC.Configurations();
            configs.Configuration = new PCC.Configuration();
            configs.Configuration.Parts = new List<PCC.Part>();
            PCC.Part part = new PCC.Part();
            PCC.PCCInputArguments pccInputArgs = new PCC.PCCInputArguments();

            part.ModelConfigFileName = "test_bench_model_config.json";
            part.ToolConfigFileName = "test_bench_tool_config.json";
            configs.Configuration.Parts.Add(part);
            configs.Configuration.Name = driverName;
            configs.Configuration.ID = PCCDriver.ID;
            configs.Configuration.PCCInputArguments = new PCC.PCCInputArguments();
            pccInputArgs = configs.Configuration.PCCInputArguments;
            pccInputArgs.InputIDs = new List<string>();
            pccInputArgs.OutputIDs = new List<string>();
            pccInputArgs.StochasticInputs = new PCC.StochasticInputs();
            pccInputArgs.StochasticInputs.InputDistributions = new List<PCCInputDistribution>();
            pccInputArgs.PCCMetrics = new List<PCC.PCCMetric>();

            foreach (var item in PCCDriver.Children.PCCParameterCollection)
            {
                //pccInputArgs.InputIDs.Add(item.ID);
                PCCInputDistribution inputDist = new PCCInputDistribution();

                if (item is CyPhy.PCCParameterNormal)
                {
                    inputDist.Distribution = "NORM";
                    inputDist.Param1 = (item as CyPhy.PCCParameterNormal).Attributes.Mean;
                    inputDist.Param2 = (item as CyPhy.PCCParameterNormal).Attributes.StandardDeviation;
                }
                else if (item is CyPhy.PCCParameterUniform)
                {
                    inputDist.Distribution = "UNIF";
                    inputDist.Param1 = (item as CyPhy.PCCParameterUniform).Attributes.LowLimit;
                    inputDist.Param2 = (item as CyPhy.PCCParameterUniform).Attributes.HighLimit;
                }
                else if (item is CyPhy.PCCParameterLNormal)
                {
                    inputDist.Distribution = "LNORM";
                    inputDist.Param1 = (item as CyPhy.PCCParameterLNormal).Attributes.Shape;
                    inputDist.Param2 = (item as CyPhy.PCCParameterLNormal).Attributes.LogScale;
                }
                else if (item is CyPhy.PCCParameterBeta)
                {
                    inputDist.Distribution = "BETA";
                    inputDist.Param1 = (item as CyPhy.PCCParameterBeta).Attributes.Shape1;
                    inputDist.Param2 = (item as CyPhy.PCCParameterBeta).Attributes.Shape2;
                    inputDist.Param3 = (item as CyPhy.PCCParameterBeta).Attributes.LowLimit;
                    inputDist.Param4 = (item as CyPhy.PCCParameterBeta).Attributes.HighLimit;
                }

                foreach (var driveParameter in item.DstConnections.DriveParameterCollection)
                {
                    inputDist.TestBenchParameterNames.Add(driveParameter.DstEnd.Name);
                }

                //inputDist.ID = inputDist.ID == null ? item.ID : inputDist.ID;
                inputDist.ID = item.ID;
                inputDist.Name = item.Name;
                pccInputArgs.StochasticInputs.InputDistributions.Add(inputDist);
            }

            pccInputArgs.StochasticInputs.InputDistributions = pccInputArgs.StochasticInputs.InputDistributions.OrderBy(p => p.TestBenchParameterNames.FirstOrDefault()).ToList();
            foreach (var item in pccInputArgs.StochasticInputs.InputDistributions)
            {
                pccInputArgs.InputIDs.Add(item.ID);
            }

            foreach (var item in PCCDriver.Children.PCCOutputCollection)
            {
                //pccInputArgs.OutputIDs.Add(item.ID);

                if (item.Attributes.TargetPCCValue == 0)
                {
                    this.Logger.WriteWarning("TargetPCCValue is set to zero. Is this correct?");
                }

                var pccMetric = new PCC.PCCMetric();
                pccMetric.ID = item.ID;
                pccMetric.TestBenchMetricName = item.SrcConnections.PCCOutputMappingCollection.FirstOrDefault().SrcEnd.Name;
                pccMetric.Name = "TestBench." + pccMetric.TestBenchMetricName;
                pccMetric.PCC_Spec = item.Attributes.TargetPCCValue;  // Could this ever present a problem?

                var metricLimits = new PCC.Limits();

                double dMin = double.NegativeInfinity;
                double dMax = double.PositiveInfinity;

                if (double.TryParse(item.Attributes.MinValue, out dMin) == false)
                {
                    // using min infinity
                    this.Logger.WriteWarning("MinValue must be 'Infinity,' '-Infinity,' or a double.");
                }

                if (double.TryParse(item.Attributes.MaxValue, out dMax) == false)
                {
                    // using max infinity
                    this.Logger.WriteWarning("MaxValue must be 'Infinity,' '-Infinity,' or a double.");
                }

                metricLimits.Min = dMin;
                metricLimits.Max = dMax;
                metricLimits.op = null; // "min/max/avg/none";       //
                metricLimits.minrange = null; // "value or n/a";     // TODO: add these 3 attributes to the PCCOutput component in CyPhy
                metricLimits.maxrange = null; //"value or n/a";     //
                pccMetric.Limits = metricLimits;

                pccInputArgs.PCCMetrics.Add(pccMetric);
            }

            pccInputArgs.PCCMetrics = pccInputArgs.PCCMetrics.OrderBy(m => m.TestBenchMetricName).ToList();

            foreach (var item in pccInputArgs.PCCMetrics)
            {
                pccInputArgs.OutputIDs.Add(item.ID);
            }

            pccInputArgs.Methods = new List<int>();
            if (upMethodNum > 0)
            {
                pccInputArgs.Methods.Add(upMethodNum);
            }

            if (saMethodNum > 0)
            {
                pccInputArgs.Methods.Add(saMethodNum);
            }

            rootConfig = new PCC.RootObject()
            {
                Configurations = configs
            };

            return rootConfig;
        }

        private List<PCCInputDistribution> TryToObtainDyanmicsProperties()
        {
            var results = new List<PCCInputDistribution>();

            var jsonFile = Path.Combine(this.outputDirectory, "TestBench", "CyPhy", "PCCProperties.json");
            if (File.Exists(jsonFile))
            {
                this.Logger.WriteInfo("Found defined PCC-Properties for the DynamicTestBench.");
                results = Newtonsoft.Json.JsonConvert.DeserializeObject<List<PCCInputDistribution>>(File.ReadAllText(jsonFile));
            }

            return results;
        }

        #endregion
    }
}
