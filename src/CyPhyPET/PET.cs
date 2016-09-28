

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
    using GME.MGA;
    using Newtonsoft.Json;
    using System.Globalization;

    public class PETConfig
    {
        public class Parameter
        {
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public string[] source;
        }
        public class Component
        {
            public Dictionary<string, Parameter> parameters;
            public Dictionary<string, Parameter> unknowns;
            public Dictionary<string, string> details;
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public string type;
        }
        public class DesignVariable
        {
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public string type;
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public double? RangeMin;
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public double? RangeMax;
            [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
            public List<object> items;
        }
        public class Driver
        {
            public string type;
            public Dictionary<string, DesignVariable> designVariables;
            public Dictionary<string, Parameter> objectives;
            public Dictionary<string, object> details;
        }
        public Dictionary<string, Component> components;
        public Dictionary<string, Driver> drivers;
    }

    public class PET
    {
        //public static GME.CSharp.GMEConsole GMEConsole { get; set; }
        public CyPhyGUIs.GMELogger Logger { get; set; }
        public bool Success = false;
        public string RunCommand { get; set; }
        public string Label { get; set; }

        private enum DriverType { PCC, Optimizer, ParameterStudy };
        private DriverType theDriver;
        private string driverName;
        private string validationDriver = "";
        private CyPhy.TestBenchType testBench;
        private CyPhy.ParametricExploration pet;
        private string outputDirectory { get; set; }
        private string testBenchOutputDir;
        public Dictionary<string, string> PCCPropertyInputDistributions { get; set; }
        public PETConfig config = new PETConfig()
        {
            components = new Dictionary<string, PETConfig.Component>(),
            drivers = new Dictionary<string, PETConfig.Driver>()
        };

        public PET(CyPhyGUIs.IInterpreterMainParameters parameters, CyPhyGUIs.GMELogger logger)
        {
            this.Logger = logger;
            this.pet = CyPhyClasses.ParametricExploration.Cast(parameters.CurrentFCO);
            this.outputDirectory = parameters.OutputDirectory;
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

                var optimizer = this.pet.Children.OptimizerCollection.FirstOrDefault();
                var config = AddConfigurationForMDAODriver(optimizer);
                config.type = "optimizer";

                foreach (var constraint in optimizer.Children.OptimizerConstraintCollection)
                {
                    // TODO
                }

            }
            else if (this.pet.Children.ParameterStudyCollection.Count() == 1)
            {
                this.theDriver = DriverType.ParameterStudy;
                this.driverName = "ParameterStudy";

                var parameterStudy = this.pet.Children.ParameterStudyCollection.FirstOrDefault();

                var config = AddConfigurationForMDAODriver(parameterStudy);
                config.type = "parameterStudy";
            }
        }

        private PETConfig.Driver AddConfigurationForMDAODriver(CyPhy.MDAODriver driver)
        {

            var config = new PETConfig.Driver()
            {
                designVariables = new Dictionary<string, PETConfig.DesignVariable>(),
                objectives = new Dictionary<string, PETConfig.Parameter>(),
                details = new Dictionary<string, object>()
            };
            this.config.drivers.Add(driver.Name, config);
            foreach (var designVariable in driver.Children.DesignVariableCollection)
            {
                var configVariable = new PETConfig.DesignVariable();
                config.designVariables.Add(designVariable.Name, configVariable);
                if (designVariable.Attributes.Range.Contains(","))
                {
                    var range = designVariable.Attributes.Range.Split(new char[] { ',' });
                    if (range.Length == 2)
                    {
                        configVariable.RangeMin = Double.Parse(range[0], CultureInfo.InvariantCulture);
                        configVariable.RangeMax = Double.Parse(range[1], CultureInfo.InvariantCulture);
                    }
                }
                else if (designVariable.Attributes.Range.Contains(";"))
                {
                    var range = designVariable.Attributes.Range;
                    var oneValue = new System.Text.RegularExpressions.Regex(
                        // start with previous match. match number or string
                        "\\G(" +
                        // number:
                        "[+-]?(?:\\d*[.])?\\d+" + "|" +
                        // string: match \ escape sequence, or anything but \ or "
                        "\"(?:\\\\(?:\\\\|\"|/|b|f|n|r|t|u\\d{4})|[^\\\\\"])*\"" +
                        // match ends with ; or end of string
                        ")(?:;|$)");
                    /*
                    Print(oneValue.Matches("12;"));
                    Print(oneValue.Matches("12.2;"));
                    Print(oneValue.Matches("\"\""));
                    Print(oneValue.Matches("\";asdf;asd\""));
                    Print(oneValue.Matches("\"\\\"\"")); // \"
                    Print(oneValue.Matches("\"\\\\\"")); // \\
                    Print(oneValue.Matches("\"\\n\"")); // \n
                    Print(oneValue.Matches("\"\\n\";\"\";\"asdf\""));
                    Print(oneValue.Matches("\"\\\"")); // \ => false
                    Print(oneValue.Matches("\"\"\"")); // " => false
                    */
                    MatchCollection matches = oneValue.Matches(range);
                    if (matches.Count == 0)
                    {
                        throw new ApplicationException(String.Format("DesignVariable {0} has invalid Range {1}", designVariable.Name, range));
                    }
                    configVariable.items = matches.Cast<Match>().Select(x => x.Groups[1].Value).Select(x =>
                        x[0] == '"' ? (string)JsonConvert.DeserializeObject(x) :
                            (object)Double.Parse(x, NumberStyles.Float | NumberStyles.AllowThousands, CultureInfo.InvariantCulture)
                        ).ToList();
                    configVariable.type = "enum";
                }
                else
                {
                    double singlePoint;
                    if (Double.TryParse(designVariable.Attributes.Range, NumberStyles.Float | NumberStyles.AllowThousands,
                            CultureInfo.InvariantCulture, out singlePoint))
                    {
                        configVariable.RangeMin = singlePoint;
                        configVariable.RangeMax = singlePoint;
                    }
                    else
                    {
                        throw new ApplicationException(String.Format("Cannot parse Design Variable Range '{0}'", designVariable.Attributes.Range));
                    }
                }
            }
            foreach (var objective in driver.Children.ObjectiveCollection)
            {
                var sourcePath = GetSourcePath(null, (MgaFCO)objective.Impl);
                if (sourcePath != null)
                {
                    config.objectives.Add(objective.Name, new PETConfig.Parameter()
                    {
                        source = sourcePath
                    });
                }
            }

            foreach (MgaAttribute attribute in ((MgaFCO)driver.Impl).Attributes)
            {
                config.details.Add(attribute.Meta.Name, attribute.StringValue);
            }

            return config;
        }

        public void GenerateCode(CyPhy.TestBenchRef testBenchRef, string testBenchOutputDir)
        {
            this.testBench = testBenchRef.Referred.TestBenchType;
            this.testBenchOutputDir = testBenchOutputDir;
            var config = new PETConfig.Component()
            {
                parameters = new Dictionary<string, PETConfig.Parameter>(),
                unknowns = new Dictionary<string, PETConfig.Parameter>(),
                details = new Dictionary<string, string>()
            };
            config.details["directory"] = testBenchOutputDir;

            this.config.components.Add(testBenchRef.Name, config);
            foreach (var parameter in testBench.Children.ParameterCollection)
            {
                var sourcePath = GetSourcePath((MgaReference)testBenchRef.Impl, (MgaFCO)parameter.Impl);
                if (sourcePath != null)
                {
                    config.parameters.Add(parameter.Name, new PETConfig.Parameter()
                    {
                        source = sourcePath
                    });
                }
            }
            foreach (var metric in testBench.Children.MetricCollection)
            {
                config.unknowns.Add(metric.Name, new PETConfig.Parameter()
                {
                });
            }
            if (this.testBench is CyPhy.TestBench)
            {
                var interpreterProgID = Rules.Global.GetInterpreterProgIDFromTestBench(this.testBench as CyPhy.TestBench);
                if (interpreterProgID == "MGA.Interpreter.CyPhyFormulaEvaluator")
                {
                    // FIXME: does this still work
                    this.SimpleCalculation();
                }
            }
        }

        private static string[] GetSourcePath(MgaReference refe, MgaFCO port)
        {
            MgaConnPoint sources;
            if (refe != null)
            {
                sources = port.PartOfConns.Cast<MgaConnPoint>().Where(cp => cp.References != null && cp.References.Count > 0 && cp.References[1].ID == refe.ID && cp.ConnRole == "dst").FirstOrDefault();
            }
            else
            {
                sources = port.PartOfConns.Cast<MgaConnPoint>().Where(cp => (cp.References == null || cp.References.Count == 0) && cp.ConnRole == "dst").FirstOrDefault();
            }
            if (sources == null)
            {
                return null;
            }
            var source = (MgaSimpleConnection)sources.Owner;
            string parentName;
            if (source.SrcReferences != null && source.SrcReferences.Count > 0)
            {
                parentName = source.SrcReferences[1].Name;
            }
            else
            {
                parentName = source.Src.ParentModel.Name;
            }
            var sourcePath = new string[] { parentName, source.Src.Name };
            return sourcePath;
        }

        public void GenerateDriverCode()
        {
            // Generate Driver
            if (this.theDriver == DriverType.PCC)
            {
                this.RunCommand = "python -E -m run_mdao";
                this.Label = " && PCC" + JobManager.Job.LabelVersion;
                // TODO convert this to code in python -m run_mdao
                this.GeneratePCCScripts();
            }
            else if (this.theDriver == DriverType.Optimizer)
            {
                this.RunCommand = "python -E -m run_mdao";
                this.Label = "";
            }
            else if (this.theDriver == DriverType.ParameterStudy)
            {
                this.RunCommand = "python -E -m run_mdao";
                this.Label = "";
                if (this.pet.Children.ParameterStudyCollection.First().Attributes.SurrogateType != CyPhyClasses.ParameterStudy.AttributesClass.SurrogateType_enum.None)
                {
                    throw new ApplicationException("Surrogates are not supported");
                }
            }

            File.WriteAllText(Path.Combine(this.outputDirectory, "mdao_config.json"), Newtonsoft.Json.JsonConvert.SerializeObject(this.config, Newtonsoft.Json.Formatting.Indented), new System.Text.UTF8Encoding(false));

            File.WriteAllText(Path.Combine(this.outputDirectory, "..", "CyPhyPET_combine.cmd"),
                "FOR /F \"skip=2 tokens=2,*\" %%A IN ('C:\\Windows\\SysWoW64\\REG.exe query \"HKLM\\software\\META\" /v \"META_PATH\"') DO set META_PATH=%%B" +
                @"%META_PATH%\bin\Python27\Scripts\Python.exe %META_PATH%\bin\PetViz.py");

            using (StreamWriter writer = new StreamWriter(Path.Combine(outputDirectory, "zip.py")))
            {
                writer.WriteLine(CyPhyPET.Properties.Resources.zip);
            }
        }

        # region DriverTypes

        /// <summary>
        /// Generates PCC scripts
        /// </summary>
        private PCC.RootObject GeneratePCCScripts()
        {
            var pccDriverCfgFile = "PCCDriverconfigs.json";
            var PCCDriver = this.pet.Children.PCCDriverCollection.FirstOrDefault();

            var upMethod = PCCDriver.Attributes.PCC_UP_Methods;
            int upMethodNum;

            switch (upMethod)
            {
                case CyPhyClasses.PCCDriver.AttributesClass.PCC_UP_Methods_enum.Monte_Carlo_Simulation__UP_MCS_:
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

            PETConfig.Driver driver = new PETConfig.Driver()
            {
                type = "PCCDriver",
                details = new Dictionary<string, object>(),
                designVariables = new Dictionary<string, PETConfig.DesignVariable>(),
                objectives = new Dictionary<string, PETConfig.Parameter>(),
            };
            this.config.drivers.Add(PCCDriver.Name, driver);
            driver.details["Configurations"] = pccConfig.Configurations;

            foreach (var designVariable in PCCDriver.Children.PCCParameterCollection)
            {
                var configVariable = new PETConfig.DesignVariable();
                driver.designVariables.Add(designVariable.Name, configVariable);
            }
            foreach (var objective in PCCDriver.Children.PCCOutputCollection)
            {
                var sourcePath = GetSourcePath(null, (MgaFCO)objective.Impl);
                if (sourcePath != null)
                {
                    driver.objectives.Add(objective.Name, new PETConfig.Parameter()
                    {
                        source = sourcePath
                    });
                }
            }

            return pccConfig;
        }

        #endregion

        #region TestBenchTypes

        private void SimpleCalculation()
        {
            this.Logger.WriteWarning("Formula has limited support. You may experience issues during the execution.");
            Directory.CreateDirectory(Path.Combine(outputDirectory, "scripts"));
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
                pccMetric.Name = item.SrcConnections.PCCOutputMappingCollection.First().SrcEnd.ParentContainer.Name + "." + pccMetric.TestBenchMetricName;
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

            var jsonFile = Path.Combine(this.outputDirectory, this.testBenchOutputDir, "CyPhy", "PCCProperties.json");
            if (File.Exists(jsonFile))
            {
                this.Logger.WriteInfo("Found defined PCC-Properties for the DynamicTestBench.");
                results = Newtonsoft.Json.JsonConvert.DeserializeObject<List<PCCInputDistribution>>(File.ReadAllText(jsonFile));
            }

            return results;
        }

        public void GenerateCode(CyPhy.ExcelWrapper excel)
        {
            var config = GenerateCode((CyPhy.ParametricTestBench)excel);

            var projectDir = Path.GetDirectoryName(Path.GetFullPath(excel.Impl.Project.ProjectConnStr.Substring("MGA=".Length)));
            config.details = new Dictionary<string, string>()
            {
                // TODO: maybe generate a relative path instead of making absolute here
                {"excelFile", Path.Combine(projectDir, excel.Attributes.ExcelFilename)},
                {"varFile", generateXLFileJson(excel)}
            };
            config.type = "excel_wrapper.excel_wrapper.ExcelWrapper";

            var inputs = excel.Children.ParameterCollection.Select(fco => fco.Name).ToList();
            var outputs = excel.Children.MetricCollection.Select(fco => fco.Name).ToList();

            HashSet<string> xlInputs = new HashSet<string>();
            HashSet<string> xlOutputs = new HashSet<string>();
            ExcelInterop.GetExcelInputsAndOutputs(config.details["excelFile"], (string name, string refersTo) =>
            {
                outputs.Remove(name);
            }, (string name, string refersTo) =>
            {
                inputs.Remove(name);
            },
            () => { });

            if (inputs.Count > 0)
            {
                throw new ApplicationException(String.Format("ExcelWrapper {0} has inputs {1} that are not in the Excel file", excel.Name, String.Join(",", inputs.ToArray())));
            }
            if (outputs.Count > 0)
            {
                throw new ApplicationException(String.Format("ExcelWrapper {0} has outputs {1} that are not in the Excel file", excel.Name, String.Join(",", outputs.ToArray())));
            }
        }

        public void GenerateCode(CyPhy.PythonWrapper python)
        {
            var config = GenerateCode((CyPhy.ParametricTestBench)python);

            var projectDir = Path.GetDirectoryName(Path.GetFullPath(python.Impl.Project.ProjectConnStr.Substring("MGA=".Length)));
            config.details = new Dictionary<string, string>()
            {
                // TODO: maybe generate a relative path instead of making absolute here
                {"filename", Path.Combine(projectDir, python.Attributes.PyFilename)}
            };
            config.type = "run_mdao.python_component.PythonComponent";
        }

        public void GenerateCode(CyPhy.MATLABWrapper matlab)
        {
            var config = GenerateCode((CyPhy.ParametricTestBench)matlab);

            var projectDir = Path.GetDirectoryName(Path.GetFullPath(matlab.Impl.Project.ProjectConnStr.Substring("MGA=".Length)));
            config.details = new Dictionary<string, string>()
            {
                // TODO: maybe generate a relative path instead of making absolute here
                {"mFile", Path.Combine(projectDir, matlab.Attributes.MFilename)},
            };
            config.type = "matlab_wrapper.MatlabWrapper";
        }

        public PETConfig.Component GenerateCode(CyPhy.ParametricTestBench excel)
        {
            var config = new PETConfig.Component()
            {
                parameters = new Dictionary<string, PETConfig.Parameter>(),
                unknowns = new Dictionary<string, PETConfig.Parameter>(),
            };

            foreach (var parameter in excel.Children.ParameterCollection)
            {
                if (parameter.AllSrcConnections.Count() == 0)
                {
                    continue;
                }
                var sourcePath = GetSourcePath(null, (MgaFCO)parameter.Impl);
                if (sourcePath != null)
                {
                    config.parameters.Add(parameter.Name, new PETConfig.Parameter()
                    {
                        source = sourcePath
                    });
                }
            }
            foreach (var metric in excel.Children.MetricCollection)
            {
                config.unknowns.Add(metric.Name, new PETConfig.Parameter()
                {
                });
            }

            this.config.components.Add(excel.Name, config);

            return config;
        }

        private string generateXLFileJson(CyPhy.ExcelWrapper excel)
        {
            List<Dictionary<string, object>> paramss = new List<Dictionary<string, object>>();
            foreach (var param in excel.Children.ParameterCollection)
            {
                double val = 0.0;
                Double.TryParse(param.Attributes.Value, NumberStyles.Float | NumberStyles.AllowThousands, CultureInfo.InvariantCulture, out val);
                paramss.Add(new Dictionary<string, object>()
                {
                    {"name", param.Name },
                    {"type", "Float" },
                    {"desc", param.Attributes.Description },
                    {"val", val }
                    // TODO sheet, row, column, units
                });
            }
            List<Dictionary<string, object>> metrics = new List<Dictionary<string, object>>();
            foreach (var metric in excel.Children.MetricCollection)
            {
                double val = 0.0;
                Double.TryParse(metric.Attributes.Value, NumberStyles.Float | NumberStyles.AllowThousands, CultureInfo.InvariantCulture, out val);
                metrics.Add(new Dictionary<string, object>()
                {
                    {"name", metric.Name },
                    {"type", "Float" },
                    {"desc", metric.Attributes.Description },
                    {"val", val }
                    // TODO sheet, row, column, units
                });
            }
            var macros = new List<string>();
            if (string.IsNullOrEmpty(excel.Attributes.Macro) == false)
            {
                macros.Add(excel.Attributes.Macro);
            }
            Dictionary<string, object> varFile = new Dictionary<string, object>()
            {
                {"unknowns", metrics},
                {"params", paramss},
                {"macros", macros }
            };
            var filename = Path.Combine(this.outputDirectory, excel.Name + ".json");
            File.WriteAllText(filename, JsonConvert.SerializeObject(varFile, Formatting.Indented));
            return Path.GetFileName(filename);
        }

        #endregion
    }
}
