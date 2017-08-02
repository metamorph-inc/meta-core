

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
    using AVM.DDP;
    using System.Security;
    using System.Runtime.InteropServices;
    using static AVM.DDP.PETConfig;

    public class PET
    {
        //public static GME.CSharp.GMEConsole GMEConsole { get; set; }
        public CyPhyGUIs.GMELogger Logger { get; set; }
        public bool Success = false;

        private enum DriverType { PCC, Optimizer, ParameterStudy };
        private DriverType theDriver;
        private CyPhy.ParametricExploration pet;
        public string outputDirectory { get; set; }
        private string testBenchOutputDir;

        private PETConfig _config;
        public PETConfig config
        {
            get
            {
                return _config;
            }
            set
            {
                _config = value;
                components = _config.components;
                drivers = _config.drivers;
                subProblems = _config.subProblems;
            }
        }
        public Dictionary<string, Component> components;
        public Dictionary<string, Driver> drivers;
        public Dictionary<string, SubProblem> subProblems;
        private SubProblem _subProblem;
        public SubProblem subProblem
        {
            get
            {
                return _subProblem;
            }
            set
            {
                _subProblem = value;
                components = subProblem.components;
                drivers = subProblem.drivers;
                subProblems = subProblem.subProblems;
            }
        }



        public static IEnumerable<IMgaObject> getAncestors(IMgaFCO fco, IMgaObject stopAt = null)
        {
            if (fco.ParentModel != null)
            {
                IMgaModel parent = fco.ParentModel;
                while (parent != null && (stopAt == null || parent.ID != stopAt.ID))
                {
                    yield return parent;
                    if (parent.ParentModel == null)
                    {
                        break;
                    }
                    parent = parent.ParentModel;
                }
                fco = parent;
            }
            IMgaFolder folder = fco.ParentFolder;
            while (folder != null && (stopAt == null || folder.ID != stopAt.ID))
            {
                yield return folder;
                folder = folder.ParentFolder;
            }
        }

        public static IEnumerable<IMgaObject> getAncestorsAndSelf(IMgaFCO fco, IMgaObject stopAt = null)
        {
            yield return fco;
            foreach (var parent in getAncestors(fco, stopAt))
            {
                yield return parent;
            }

        }

        public PET(MgaFCO currentFCO, CyPhyGUIs.GMELogger logger)
        {
            this.Logger = logger;
            this.pet = CyPhyClasses.ParametricExploration.Cast(currentFCO);
        }

        public void Initialize()
        {
            // Determine type of driver of the Parametric Exploration
            if (this.pet.Children.PCCDriverCollection.Count() == 1)
            {
                this.theDriver = DriverType.PCC;
                throw new ApplicationException("PCC is unimplemented");
            }
            else if (this.pet.Children.OptimizerCollection.Count() == 1)
            {
                this.theDriver = DriverType.Optimizer;

                var optimizer = this.pet.Children.OptimizerCollection.FirstOrDefault();
                var config = AddConfigurationForMDAODriver(optimizer);
                config.type = "optimizer";
                config.details["OptimizationFunction"] = optimizer.Attributes.OptimizationFunction.ToString();
                if (optimizer.Attributes.OptimizationFunction == CyPhyClasses.Optimizer.AttributesClass.OptimizationFunction_enum.Custom)
                {
                    config.details["OptimizationClass"] = optimizer.Attributes.CustomOptimizer;
                }

                foreach (var intermediateVar in optimizer.Children.IntermediateVariableCollection)
                {
                    var sourcePath = GetSourcePath(null, (MgaFCO)intermediateVar.Impl);
                    if (sourcePath != null)
                    {
                        var intermVar = new PETConfig.Parameter();
                        intermVar.source = sourcePath;
                        setUnit(intermediateVar.Referred.unit, intermVar);
                        checkUnitMatchesSource(intermediateVar);

                        config.intermediateVariables.Add(intermediateVar.Name, intermVar);
                    }
                }

                foreach (var constraint in optimizer.Children.OptimizerConstraintCollection)
                {
                    var sourcePath = GetSourcePath(null, (MgaFCO)constraint.Impl);
                    if (sourcePath != null)
                    {
                        var cons = new PETConfig.Constraint();
                        cons.source = sourcePath;

                        if (!string.IsNullOrWhiteSpace(constraint.Attributes.MinValue))
                        {
                            double minValue;
                            if (double.TryParse(constraint.Attributes.MinValue,
                                NumberStyles.Float | NumberStyles.AllowThousands,
                                CultureInfo.InvariantCulture, out minValue))
                            {
                                cons.RangeMin = minValue;
                            }
                            else
                            {
                                throw new ApplicationException(String.Format("Cannot parse Constraint MinValue '{0}'",
                                    constraint.Attributes.MinValue));
                            }
                        }
                        else
                        {
                            cons.RangeMin = null;
                        }

                        if (!string.IsNullOrWhiteSpace(constraint.Attributes.MaxValue))
                        {
                            double maxValue;
                            if (double.TryParse(constraint.Attributes.MaxValue,
                                NumberStyles.Float | NumberStyles.AllowThousands,
                                CultureInfo.InvariantCulture, out maxValue))
                            {
                                cons.RangeMax = maxValue;
                            }
                            else
                            {
                                throw new ApplicationException(String.Format("Cannot parse Constraint MaxValue '{0}'",
                                    constraint.Attributes.MaxValue));
                            }
                        }
                        else
                        {
                            cons.RangeMax = null;
                        }
                        config.constraints.Add(constraint.Name, cons);
                    }
                }

            }
            else if (this.pet.Children.ParameterStudyCollection.Count() == 1)
            {
                this.theDriver = DriverType.ParameterStudy;

                var parameterStudy = this.pet.Children.ParameterStudyCollection.FirstOrDefault();

                var config = AddConfigurationForMDAODriver(parameterStudy);
                config.type = "parameterStudy";
                // FIXME: do this in another thread
                Dictionary<string, object> assignment = getPythonAssignment(parameterStudy.Attributes.Code);
                long num_samples;
                object num_samplesObj;
                if (assignment.TryGetValue("num_samples", out num_samplesObj))
                {
                    if (num_samplesObj is long)
                    {
                        num_samples = (long)num_samplesObj;
                    }
                    else
                    {
                        throw new ApplicationException("num_samples must be an integer");
                    }
                }
                else
                {
                    throw new ApplicationException("num_samples must be specified in the Code attribute of the Parameter Study");
                }

                if (parameterStudy.Attributes.DOEType == CyPhyClasses.ParameterStudy.AttributesClass.DOEType_enum.Opt_Latin_Hypercube)
                {
                    if (num_samples < 2)
                    {
                        throw new ApplicationException("num_samples must be >= 2 when using Optimized Latin Hypercube");
                    }
                }
            }
        }

        public static Dictionary<string, object> getPythonAssignment(string code)
        {
            Process getParamsAndUnknowns = new Process();
            getParamsAndUnknowns.StartInfo = new ProcessStartInfo(META.VersionInfo.PythonVEnvExe)
            {
                CreateNoWindow = true,
                UseShellExecute = false,
                RedirectStandardError = true,
                RedirectStandardOutput = true,
                RedirectStandardInput = true,
                Arguments = "-c \"import sys, json;" +
                    "assignment = {};" +
                    "eval(compile(sys.stdin.read(), '<driver Code>', 'exec'), globals(), assignment);" +
                    "print(json.dumps(assignment))\""
            };

            getParamsAndUnknowns.Start();
            getParamsAndUnknowns.StandardInput.Write(code);
            getParamsAndUnknowns.StandardInput.Close();
            // n.b. assume buffers will not fill up and deadlock us
            string stdout = getParamsAndUnknowns.StandardOutput.ReadToEnd();
            string stderr = getParamsAndUnknowns.StandardError.ReadToEnd();
            getParamsAndUnknowns.WaitForExit();
            if (getParamsAndUnknowns.ExitCode != 0)
            {
                throw new ApplicationException(stderr);
            }

            return Newtonsoft.Json.JsonConvert.DeserializeObject<Dictionary<string, object>>(stdout.Replace("\r", "").Split('\n').Last(line => string.IsNullOrWhiteSpace(line) == false));
        }

        private PETConfig.Driver AddConfigurationForMDAODriver(CyPhy.MDAODriver driver)
        {

            var config = new PETConfig.Driver()
            {
                designVariables = new Dictionary<string, PETConfig.DesignVariable>(),
                objectives = new Dictionary<string, PETConfig.Parameter>(),
                constraints = new Dictionary<string, PETConfig.Constraint>(),
                intermediateVariables = new Dictionary<string, PETConfig.Parameter>(),
                details = new Dictionary<string, object>()
            };
            this.drivers.Add(driver.Name, config);
            foreach (var designVariable in driver.Children.DesignVariableCollection)
            {
                var configVariable = new PETConfig.DesignVariable();
                setUnit(designVariable.Referred.unit, configVariable);
                config.designVariables.Add(designVariable.Name, configVariable);

                var range = designVariable.Attributes.Range;
                var oneValue = new System.Text.RegularExpressions.Regex(
                    // start with previous match. match number or string
                    "\\G(" +
                    // number:
                    // TODO: scientific notation?
                    // TODO: a la NumberStyles.AllowThousands?
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
                if (matches.Count > 0)
                {
                    var items = matches.Cast<Match>().Select(x => x.Groups[1].Value).Select(x =>
                        x[0] == '"' ? (string)JsonConvert.DeserializeObject(x) :
                            (object)Double.Parse(x, NumberStyles.Float | NumberStyles.AllowThousands, CultureInfo.InvariantCulture)
                        ).ToList();
                    // special-case: a single number produces a double range
                    if (matches.Count == 1 && matches[0].Groups[1].Value[0] != '"')
                    {
                        configVariable.RangeMin = (double)items[0];
                        configVariable.RangeMax = (double)items[0];
                    }
                    else
                    {
                        configVariable.items = items;
                        configVariable.type = "enum";
                    }
                }
                else if (designVariable.Attributes.Range.Contains(","))
                {
                    var range_split = designVariable.Attributes.Range.Split(new char[] { ',' });
                    if (range_split.Length == 2)
                    {
                        configVariable.RangeMin = Double.Parse(range_split[0], NumberStyles.Float | NumberStyles.AllowThousands, CultureInfo.InvariantCulture);
                        configVariable.RangeMax = Double.Parse(range_split[1], NumberStyles.Float | NumberStyles.AllowThousands, CultureInfo.InvariantCulture);
                    }
                }
                else
                {
                    throw new ApplicationException(String.Format("Cannot parse Design Variable Range '{0}'. ", designVariable.Attributes.Range) +
                        "Double ranges are specified by an un-quoted value or two un-quoted values separated by commas. " +
                        "Enumerations are specified by one double-quoted value or two or more double-quoted values separated by semicolons. " +
                        "E.g.: '2.0,2.5' or '\"Low\";\"Medium\";\"High\"'"
                        );
                }
            }
            foreach (var objective in driver.Children.ObjectiveCollection)
            {
                var sourcePath = GetSourcePath(null, (MgaFCO)objective.Impl);
                if (sourcePath != null)
                {
                    var configParameter = new PETConfig.Parameter()
                    {
                        source = sourcePath
                    };
                    config.objectives.Add(objective.Name, configParameter);
                    setUnit(objective.Referred.unit, configParameter);
                    checkUnitMatchesSource(objective);
                }
            }

            foreach (MgaAttribute attribute in ((MgaFCO)driver.Impl).Attributes)
            {
                config.details.Add(attribute.Meta.Name, attribute.StringValue);
            }

            return config;
        }

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool SetDllDirectory(string lpPathName);

        [DllImport("CyPhyFormulaEvaluator.dll")]
        static extern bool AreUnitsEqual(IMgaFCO fco1, IMgaFCO fco2);

        private void checkUnitMatchesSource(ISIS.GME.Common.Interfaces.Reference objective)
        {
            var mgaObjective = (MgaReference)objective.Impl;
            foreach (MgaConnPoint connPoint in mgaObjective.PartOfConns)
            {
                if (connPoint.ConnRole != "dst")
                {
                    continue;
                }
                var src = ((MgaSimpleConnection)connPoint.Owner).Src;
                if (src is MgaReference)
                {
                    var metric = (MgaReference)src;
                    if (metric.Referred == null || mgaObjective.Referred == null)
                    {
                        continue;
                    }
                    if (metric.Referred.ID != mgaObjective.Referred.ID)
                    {
                        foreach (var path in new string[] { Path.Combine(META.VersionInfo.MetaPath, "bin"), Path.Combine(META.VersionInfo.MetaPath, "src", "bin") })
                        {
                            if (File.Exists(Path.Combine(path, "CyPhyFormulaEvaluator.dll")))
                            {
                                SetDllDirectory(path);
                            }
                        }
                        if (AreUnitsEqual(metric.Referred, mgaObjective.Referred) == false)
                        {
                            Logger.WriteFailed(String.Format("Unit for <a href=\"mga:{0}\">{1}</a> must match unit for <a href=\"mga:{2}\">{3}</a>",
                                metric.getTracedObjectOrSelf(Logger.Traceability).ID, SecurityElement.Escape(metric.Name),
                                objective.Impl.getTracedObjectOrSelf(Logger.Traceability).ID, SecurityElement.Escape(objective.Name)));
                            throw new ApplicationException();
                        }
                    }
                }
            }
        }

        public void GenerateCode(CyPhy.TestBenchRef testBenchRef, string testBenchOutputDir)
        {
            var testBench = testBenchRef.Referred.TestBenchType;
            this.testBenchOutputDir = testBenchOutputDir;
            var config = new PETConfig.Component()
            {
                parameters = new Dictionary<string, PETConfig.Parameter>(),
                unknowns = new Dictionary<string, PETConfig.Parameter>(),
                details = new Dictionary<string, string>()
            };
            config.details["directory"] = testBenchOutputDir;

            this.components.Add(testBenchRef.Name, config);
            foreach (var parameter in
                testBench.Children.ParameterCollection.Select(parameter => new { fco = parameter.Impl, unit = parameter.Referred.unit }).
                Concat(testBench.Children.FileInputCollection.Select(fileInput => new { fco = fileInput.Impl, unit = (CyPhy.unit)null }))
                )
            {
                var sourcePath = GetSourcePath((MgaReference)testBenchRef.Impl, (MgaFCO)parameter.fco);
                if (sourcePath != null)
                {
                    var configParameter = new PETConfig.Parameter()
                    {
                        source = sourcePath
                    };
                    config.parameters.Add(parameter.fco.Name, configParameter);
                    setUnit(parameter.unit, configParameter);
                }
            }
            foreach (var metric in testBench.Children.MetricCollection)
            {
                var configParameter = new PETConfig.Parameter();
                config.unknowns.Add(metric.Name, configParameter);
                setUnit(metric.Referred.unit, configParameter);
            }
            foreach (var fileOutput in testBench.Children.FileOutputCollection)
            {
                var configParameter = new PETConfig.Parameter();
                config.unknowns.Add(fileOutput.Name, configParameter);
            }
            if (testBench is CyPhy.TestBench)
            {
                var interpreterProgID = Rules.Global.GetInterpreterProgIDFromTestBench(testBench as CyPhy.TestBench);
                if (interpreterProgID == "MGA.Interpreter.CyPhyFormulaEvaluator")
                {
                    // FIXME: does this still work
                    this.SimpleCalculation((CyPhy.TestBench)testBench);
                }
            }
        }

        public Dictionary<CyPhy.unit, List<Action<string>>> unitsToSet = new Dictionary<CyPhy.unit, List<Action<string>>>();

        private void setUnit(CyPhy.unit unit, Action<string> action)
        {
            if (unit == null)
            {
                return;
            }
            List<Action<string>> setters;
            if (unitsToSet.TryGetValue(unit, out setters) == false)
            {
                setters = new List<Action<string>>();
                unitsToSet[unit] = setters;
            }
            setters.Add(action);
        }

        private void setUnit(CyPhy.unit unit, PETConfig.Parameter configParameter)
        {
            setUnit(unit, units => configParameter.units = units);
        }

        private void setUnit(CyPhy.unit unit, PETConfig.DesignVariable designVariable)
        {
            setUnit(unit, units => designVariable.units = units);
        }

        private string[] GetSourcePath(MgaReference refe, MgaFCO port)
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
            // top-level ParametricExploration: ignore top-level ProblemInput and ProblemOutputs
            if (config != null &&
                source.Src.ParentModel.ID == pet.ID &&
                (source.Src.Meta.Name == typeof(CyPhy.ProblemInput).Name || source.Src.Meta.Name == typeof(CyPhy.ProblemOutput).Name))
            {
                return GetSourcePath(null, source.Src);
            }
            string parentName;
            if (source.SrcReferences != null && source.SrcReferences.Count > 0)
            {
                parentName = source.SrcReferences[1].Name;
            }
            else
            {
                if (pet.ID == source.Src.ParentModel.ID)
                {
                    return new string[] { source.Src.Name };
                }
                parentName = source.Src.ParentModel.Name;
            }
            var sourcePath = new string[] { parentName, source.Src.Name };
            return sourcePath;

            MgaModel parent;
            if (source.SrcReferences != null && source.SrcReferences.Count > 0)
            {
                parent = (MgaModel)source.SrcReferences[1];
            }
            else
            {
                parent = source.Src.ParentModel;
            }
            List<string> path = new List<string>();
            while (parent.ID != pet.ID)
            {
                path.Add(parent.Name);
                parent = parent.ParentModel;
            }
            path.Reverse();
            path.Add(source.Src.Name);
            return path.ToArray();
        }

        public void GenerateDriverCode()
        {
            // Generate Driver
            if (this.theDriver == DriverType.PCC)
            {
                // FIXME
            }
            else if (this.theDriver == DriverType.Optimizer)
            {
            }
            else if (this.theDriver == DriverType.ParameterStudy)
            {
                if (this.pet.Children.ParameterStudyCollection.First().Attributes.SurrogateType != CyPhyClasses.ParameterStudy.AttributesClass.SurrogateType_enum.None)
                {
                    throw new ApplicationException("Surrogates are not supported");
                }
            }

            if (unitsToSet.Count > 0)
            {
                var cyPhyPython = (IMgaComponentEx)Activator.CreateInstance(Type.GetTypeFromProgID("MGA.Interpreter.CyPhyPython"));
                // cyPhyPython.ComponentParameter["script_file"] = Path.Combine(META.VersionInfo.MetaPath, "bin", "CyPhyPET_unit_setter.py");
                cyPhyPython.ComponentParameter["script_file"] = "CyPhyPET_unit_setter.py";

                var fcos = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
                int i = 0;
                foreach (var unit in unitsToSet.Keys)
                {
                    // fcos.Append((MgaFCO)unit.Impl);
                    cyPhyPython.ComponentParameter[String.Format("unit_id_{0}", i)] = unit.Impl.ID;
                    i++;
                }

                cyPhyPython.InvokeEx(pet.Impl.Project, (MgaFCO)pet.Impl, fcos, 128);

                i = 0;
                foreach (var unit in unitsToSet.Keys)
                {
                    foreach (var action in unitsToSet[unit])
                    {
                        var value = (string)cyPhyPython.ComponentParameter[String.Format("unit_id_{0}_ret", i)];
                        if (value == "")
                        {
                            value = null;
                        }
                        action(value);
                    }
                    i++;
                }
            }

        }

        # region DriverTypes

        #endregion

        #region TestBenchTypes

        private void SimpleCalculation(CyPhy.TestBench testBench)
        {
            this.Logger.WriteWarning("Formula has limited support. You may experience issues during the execution.");
            Directory.CreateDirectory(Path.Combine(outputDirectory, "scripts"));
            using (StreamWriter writer = new StreamWriter(Path.Combine(outputDirectory, "scripts", "test_bench.py")))
            {
                Templates.TestBenchExecutors.Formula simpleCalc = new Templates.TestBenchExecutors.Formula()
                {
                    testBench = testBench
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

            System.Uri pyFileUri = new Uri(Path.Combine(projectDir, python.Attributes.PyFilename));
            System.Uri outputUri = new Uri(this.outputDirectory + "\\");
            string pyFilename = Uri.UnescapeDataString(outputUri.MakeRelativeUri(pyFileUri).ToString());
            // n.b. keep forward slashes

            config.details = new Dictionary<string, string>()
            {
                {"filename", pyFilename}
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

        public PETConfig.Component GenerateCode(CyPhy.Constants constants)
        {
            // Get a new config
            var config = new PETConfig.Component()
            {
                parameters = new Dictionary<string, PETConfig.Parameter>(),
                unknowns = new Dictionary<string, PETConfig.Parameter>(),
                type = "IndepVarComp"
            };

            foreach (var metric in constants.Children.MetricCollection)
            {
                var configParameter = new PETConfig.Parameter();

                Exception ex = null;
                try
                {
                    configParameter.value = Newtonsoft.Json.Linq.JToken.Parse(metric.Attributes.Value);
                }
                catch (JsonReaderException jre)
                {
                    ex = jre;
                }
                catch (System.FormatException fe)
                {
                    ex = fe;
                }
                if (ex != null)
                {
                    String msg = "Failed to parse the Value for the Metric " +
                        "<a href=\"mga:{1}\">{0}</a> in <a href=\"mga:{3}\">{2}</a>. " +
                        "If this was intended to be a String, surround it in quotes.";
                    this.Logger.WriteError(String.Format(msg,
                                                         metric.Name, metric.ID,
                                                         constants.Name, constants.ID));
                    this.Logger.WriteDebug(msg);
                    throw ex;
                }

                config.unknowns.Add(metric.Name, configParameter);
                setUnit(metric.Referred.unit, configParameter);
            }

            this.components.Add(constants.Name, config);

            return config;
        }

        internal void GenerateInputsAndOutputs()
        {
            if (this.config != null)
            {
                return;
            }
            foreach (var input in pet.Children.ProblemInputCollection)
            {
                var problemInput = new SubProblem.ProblemInput()
                {
                    //outerSource =
                    //innerSource =
                    value = input.Attributes.Value
                };

                var sources = ((MgaFCO)input.Impl).PartOfConns.Cast<MgaConnPoint>().Where(cp => (cp.References == null || cp.References.Count == 0) && cp.ConnRole == "dst");
                foreach (var connPoint in sources)
                {
                    var source = (MgaSimpleConnection)connPoint.Owner;
                    IMgaFCO parent;
                    if (source.SrcReferences != null && source.SrcReferences.Count > 0)
                    {
                        parent = source.SrcReferences[1];
                    }
                    else
                    {
                        parent = source.Src.ParentModel;
                    }
                    var gparent = parent.ParentModel;
                    // var ggparent = parent == null ? null : parent.ParentModel;
                    if (gparent != null && gparent.ID == pet.ID)
                    {
                        problemInput.innerSource = new string[] { parent.Name, source.Src.Name };
                    }
                    else
                    {
                        // FIXME don't need parent.Name if parent is ParametricExploration ?
                        problemInput.outerSource = new string[] { parent.Name, source.Src.Name };
                    }
                }

                List<MgaFCO> realSources = GetTransitiveSources((MgaFCO)input.Impl, new HashSet<string>()
                {
                    typeof(CyPhy.Metric).Name,
                    typeof(CyPhy.FileOutput).Name,
                    // typeof(CyPhy.ProblemOutput)
                    typeof(CyPhy.DesignVariable).Name
                });
                var desVarSources = realSources.Where(s => s.Meta.Name == typeof(CyPhy.DesignVariable).Name);
                if (desVarSources.Count() > 0)
                {
                    var desVar = desVarSources.First();
                    // FIXME: get from desVar attribute
                    problemInput.value = "0.0";
                    problemInput.pass_by_obj = false;
                }
                else
                {
                    MgaModel realSourceParent;
                    MgaFCO realSource = null;
                    if (realSources.Count > 1)
                    {
                        // should be unreachable because checker should detect this error
                        throw new ApplicationException(String.Format("Error: {0} has more than one source", input.Name));
                    }
                    realSource = realSources.First();
                    realSourceParent = realSource.ParentModel;
                    if (problemInput.value == "")
                    {
                        if (testbenchtypes.Contains(realSourceParent.Meta.Name))
                        {
                            // FIXME read DesignVariable to get type
                            problemInput.value = "0.0";
                            problemInput.pass_by_obj = false;
                        }
                        else if (realSource.Meta.Name == typeof(CyPhy.Metric).Name)
                        {
                            problemInput.value = CyPhyClasses.Metric.Cast(realSource).Attributes.Value;
                            // FIXME: move this to the checker
                            if (problemInput.value == "")
                            {
                                throw new ApplicationException(String.Format("Error: {0} must specify a Value", input.Name));
                            }
                        }
                    }
                    string kind = realSourceParent.Meta.Name;
                    if (testbenchtypes.Contains(kind))
                    {
                        problemInput.pass_by_obj = true;
                    }
                    else if (kind == typeof(CyPhy.ExcelWrapper).Name || kind == typeof(CyPhy.MATLABWrapper).Name)
                    {
                        problemInput.pass_by_obj = false;
                    }
                    else
                    {
                        string pbo = realSource.GetRegistryValueDisp("pass_by_obj") ?? "False";
                        problemInput.pass_by_obj = true.ToString().Equals(pbo, StringComparison.InvariantCultureIgnoreCase);
                    }
                }
                
                subProblem.problemInputs.Add(input.Name, problemInput);
            }
            foreach (var output in pet.Children.ProblemOutputCollection)
            {
                subProblem.problemOutputs.Add(output.Name, GetSourcePath(null, (MgaFCO)output.Impl));
            }
        }

        readonly ISet<string> testbenchtypes = GetDerivedTypeNames(typeof(CyPhy.TestBenchType));
        // readonly ISet<string> petWrapperTypes = GetDerivedTypeNames(typeof(CyPhy.ParametricTestBench));

        private static ISet<string> GetDerivedTypeNames(Type type)
        {
            HashSet<string> ret = new HashSet<string>();
            foreach (Type t in type.Assembly.GetExportedTypes())
            {
                if (type.IsAssignableFrom(t))
                {
                    ret.Add(t.Name);
                }
            }
            return ret;
        }

        public static List<MgaFCO> GetTransitiveSources(MgaFCO fco, ISet<string> stopKinds)
        {
            List<MgaFCO> ret = new List<MgaFCO>();
            Queue<MgaFCO> q = new Queue<MgaFCO>();
            q.Enqueue(fco);
            while (q.Count > 0)
            {
                fco = q.Dequeue();
                foreach (MgaConnPoint cp in fco.PartOfConns)
                {
                    // FIXME: don't go above top-level PET
                    if (cp.ConnRole == "dst")
                    {
                        fco = ((MgaSimpleConnection)cp.Owner).Src;
                        if (stopKinds.Contains(fco.Meta.Name))
                        {
                            ret.Add(fco);
                        }
                        else
                        {
                            q.Enqueue(fco);
                        }
                    }
                }
            }
            return ret;
        }

        public PETConfig.Component GenerateCode(CyPhy.ParametricTestBench excel)
        {
            var config = new PETConfig.Component()
            {
                parameters = new Dictionary<string, PETConfig.Parameter>(),
                unknowns = new Dictionary<string, PETConfig.Parameter>(),
            };

            foreach (var parameter in
                excel.Children.ParameterCollection.Select(parameter => new { fco = (ISIS.GME.Common.Interfaces.FCO)parameter, unit = parameter.Referred.unit }).
                Concat(excel.Children.FileInputCollection.Select(fileInput => new { fco = (ISIS.GME.Common.Interfaces.FCO)fileInput, unit = (CyPhy.unit)null }))
                )
            {
                var sourcePath = GetSourcePath(null, (MgaFCO)parameter.fco.Impl);
                if (sourcePath != null)
                {
                    var configParameter = new PETConfig.Parameter()
                    {
                        source = sourcePath,
                    };
                    config.parameters.Add(parameter.fco.Name, configParameter);
                    setUnit(parameter.unit, configParameter);

                }
            }
            foreach (var metric in excel.Children.MetricCollection)
            {
                var configParameter = new PETConfig.Parameter()
                {
                };
                config.unknowns.Add(metric.Name, configParameter);
                setUnit(metric.Referred.unit, configParameter);
            }
            foreach (var fileOutput in excel.Children.FileOutputCollection)
            {
                config.unknowns.Add(fileOutput.Name, new PETConfig.Parameter());
            }

            this.components.Add(excel.Name, config);

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

    public static class Extensions
    {
        public static IEnumerable<IMgaObject> getTracedObjectOrSelf(this IEnumerable<IMgaObject> enumerable, CyPhyCOMInterfaces.IMgaTraceability traceability)
        {
            foreach (var obj in enumerable)
            {
                string originalID;
                if (traceability != null && traceability.TryGetMappedObject(obj.ID, out originalID))
                {
                    yield return obj.Project.GetObjectByID(originalID);
                }
                else
                {
                    yield return obj;
                }
            }
        }

        public static IMgaObject getTracedObjectOrSelf(this IMgaObject obj, CyPhyCOMInterfaces.IMgaTraceability traceability)
        {
            string originalID;
            if (traceability != null && traceability.TryGetMappedObject(obj.ID, out originalID))
            {
                return obj.Project.GetObjectByID(originalID);
            }
            else
            {
                return obj;
            }
        }
    }
}
