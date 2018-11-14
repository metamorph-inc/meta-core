

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
        public CyPhyGUIs.SmartLogger Logger { get; set; }
        public bool Success = false;

        private enum DriverType { None, PCC, Optimizer, ParameterStudy };
        private DriverType theDriver;
        private CyPhy.ParametricExploration pet;
        private MgaFCO rootPET;
        public string outputDirectory { get; set; }
        private string testBenchOutputDir;

        public static readonly ISet<string> testbenchtypes = GetDerivedTypeNames(typeof(CyPhy.TestBenchType));
        public static readonly ISet<string> petWrapperTypes = GetDerivedTypeNames(typeof(CyPhy.ParametricTestBench));
        public Dictionary<string, string> PCCPropertyInputDistributions { get; set; }

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

        public PET(MgaFCO rootPET, MgaFCO currentFCO, CyPhyGUIs.SmartLogger logger)
        {
            this.Logger = logger;
            this.pet = CyPhyClasses.ParametricExploration.Cast(currentFCO);
            this.rootPET = rootPET;
        }

        public void Initialize()
        {
            // Determine type of driver of the Parametric Exploration
            if (this.pet.Children.PCCDriverCollection.Count() == 1)
            {
                this.theDriver = DriverType.PCC;
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
                bool hasDesignVariables = parameterStudy.Children.DesignVariableCollection.Count() > 0;
                if (hasDesignVariables == false)
                {
                    config.details["DOEType"] = "Uniform";
                }
                // FIXME: do this in another thread
                Dictionary<string, object> assignment = getPythonAssignment(parameterStudy.Attributes.Code);
                long num_samples;
                long num_levels;
                object num_samplesObj;
                object num_levelsObj;
                if (parameterStudy.Attributes.DOEType == CyPhyClasses.ParameterStudy.AttributesClass.DOEType_enum.CSV_File)
                {
                    object filename;
                    if (assignment.TryGetValue("filename", out filename) == false || filename is string == false)
                    {
                        throw new ApplicationException("For CSV File input, you must specify filename=r'file.csv' in the ParameterStudy's Code attribute");
                    }
                    var projectDir = Path.GetDirectoryName(Path.GetFullPath(parameterStudy.Impl.Project.ProjectConnStr.Substring("MGA=".Length)));
                    string basename = Path.GetFileName((string)filename);
                    try
                    {
                        File.Copy(Path.Combine(projectDir, (string)filename), Path.Combine(outputDirectory, basename));
                    }
                    catch (IOException e)
                    {
                        throw new ApplicationException(String.Format("Could not copy '{0}': {1}", filename, e.Message), e);
                    }
                    if (basename == "output.csv")
                    {
                        throw new ApplicationException("CSV File input filename must not be 'output.csv'");
                    }
                    if (basename != (string)filename)
                    {
                        var code = config.details["Code"].ToString();
                        var basenameEscaped = escapePythonString(basename);
                        code += String.Format("\nfilename = u'{0}'\n", basenameEscaped);
                        config.details["Code"] = code;
                    }
                }
                else if (parameterStudy.Attributes.DOEType == CyPhyClasses.ParameterStudy.AttributesClass.DOEType_enum.Full_Factorial)
                {
                    if (assignment.TryGetValue("num_levels", out num_levelsObj))
                    {
                        if (num_levelsObj is long)
                        {
                            num_levels = (long)num_levelsObj;
                        }
                        else
                        {
                            throw new ApplicationException("num_levels must be an integer");
                        }
                    }
                    // Legacy support for num_samples assignment in code block.
                    else if (assignment.TryGetValue("num_samples", out num_samplesObj))
                    {
                        if (num_samplesObj is long)
                        {
                            num_samples = (long)num_samplesObj;
                        }
                        else
                        {
                            throw new ApplicationException("num_samples must be an integer");
                        }
                        config.details["Code"] = "num_levels = " + num_samples.ToString();
                    }
                    else if (hasDesignVariables == false)
                    {
                        config.details["Code"] = "num_samples = 1";
                        config.details["DOEType"] = "Uniform";
                    }
                    else
                    {
                        throw new ApplicationException("num_levels must be specified in the Code attribute of a Full Factorial Parameter Study Driver");
                    }
                }
                else
                {
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
                        if (parameterStudy.Attributes.DOEType == CyPhyClasses.ParameterStudy.AttributesClass.DOEType_enum.Opt_Latin_Hypercube)
                        {
                            if (num_samples < 2)
                            {
                                throw new ApplicationException("num_samples must be >= 2 when using Optimized Latin Hypercube");
                            }
                        }
                    }
                    else if (hasDesignVariables == false)
                    {
                        config.details["Code"] = "num_samples = 1";
                        config.details["DOEType"] = "Uniform";
                    }
                    else
                    {
                        throw new ApplicationException("num_samples must be specified in the Code attribute of the Parameter Study");
                    }
                }
            }
        }

        public static string escapePythonString(string value)
        {
            return Regex.Replace(value, "('|[^ -~])", m => String.Format("\\u{0:X4}", (int)m.Groups[0].Value[0]));
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
                ParseDesignVariableRange(designVariable.Attributes.Range, configVariable);
                config.designVariables.Add(designVariable.Name, configVariable);
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

        public static double nextafter(double value, double direction)
        {
            if (Double.IsInfinity(value) || Double.IsNaN(value))
            {
                return value;
            }
            if (value == direction)
            {
                return value;
            }
            int dir = value > direction ? 1 : -1;
            long bits = BitConverter.DoubleToInt64Bits(value);
            if (value > 0)
            {
                return BitConverter.Int64BitsToDouble(bits - dir);
            }
            else if (value < 0)
            {
                return BitConverter.Int64BitsToDouble(bits + dir);
            }
            else
            {
                return -1 * dir * double.Epsilon;
            }
        }

        public static void ParseDesignVariableRange(string range, DesignVariable configVariable)
        {
            var parseErrorMessage = String.Format("Cannot parse Design Variable Range '{0}'. ", range) +
                    "Double ranges are specified by an un-quoted value or two un-quoted values separated by commas. " +
                    "Enumerations are specified by one double-quoted value or two or more double-quoted values separated by semicolons. " +
                    "E.g.: '2.0,2.5' or '\"Low\";\"Medium\";\"High\"'";
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
                var last = matches.Cast<Match>().Last();
                if (last.Index + last.Length != range.Length)
                {
                    throw new ApplicationException(parseErrorMessage);
                }
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
            else if (range.Contains(","))
            {
                var range_split = range.Split(new char[] { ',' });
                if (range_split.Length != 2)
                {
                    throw new ApplicationException(parseErrorMessage);
                }
                string lower = range_split[0];
                string upper = range_split[1];
                bool lowerExcluded = false;
                bool upperExcluded = false;
                if (lower[0] == '(')
                {
                    lowerExcluded = true;
                    lower = lower.Substring(1);
                }
                else if (lower[0] == '[')
                {
                    lowerExcluded = false;
                    lower = lower.Substring(1);
                }
                if (upper.Last() == ')')
                {
                    upperExcluded = true;
                    upper = upper.Substring(0, upper.Length - 1);
                }
                else if (upper.Last() == ']')
                {
                    upperExcluded = false;
                    upper = upper.Substring(0, upper.Length - 1);
                }
                configVariable.RangeMin = Double.Parse(lower, NumberStyles.Float | NumberStyles.AllowThousands, CultureInfo.InvariantCulture);
                configVariable.RangeMax = Double.Parse(upper, NumberStyles.Float | NumberStyles.AllowThousands, CultureInfo.InvariantCulture);
                if (lowerExcluded)
                {
                    configVariable.RangeMin = nextafter((double)configVariable.RangeMin, Double.PositiveInfinity);
                }
                if (upperExcluded)
                {
                    configVariable.RangeMax = nextafter((double)configVariable.RangeMax, Double.NegativeInfinity);
                }
                if (configVariable.RangeMin > configVariable.RangeMax)
                {
                    throw new ApplicationException(String.Format("Invalid Design Variable Range '{0}'. ", range));
                }
            }
            else
            {
                throw new ApplicationException(parseErrorMessage);
            }
        }

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool SetDllDirectory(string lpPathName);

        [DllImport("CyPhyFormulaEvaluator.dll", CallingConvention=CallingConvention.Cdecl)]
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
                details = new Dictionary<string, object>()
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
                var interpreterProgID = Rules.Global.GetTasksFromTestBench(testBench as CyPhy.TestBench);
                if (interpreterProgID.OfType<CyPhy.Task>().Select(task => task.Attributes.COMName).Contains("MGA.Interpreter.CyPhyFormulaEvaluator"))
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
            IEnumerable<MgaConnPoint> sources;
            if (refe != null)
            {
                sources = port.PartOfConns.Cast<MgaConnPoint>().Where(cp => cp.References != null && cp.References.Count > 0 && cp.References[1].ID == refe.ID && cp.ConnRole == "dst");
            }
            else
            {
                sources = port.PartOfConns.Cast<MgaConnPoint>().Where(cp => (cp.References == null || cp.References.Count == 0) && cp.ConnRole == "dst");
            }
            sources = sources.Where(s => getAncestors(s.Owner).Select(x => x.ID).Contains(rootPET.ID));
            if (sources.Count() == 0)
            {
                return null;
            }
            var source = (MgaSimpleConnection)sources.First().Owner;
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
                // this.Label = " && PCC" + JobManager.Job.LabelVersion;
                // TODO convert this to code in python -m run_mdao
                this.GeneratePCCScripts();
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

            var pccConfig = this.BuildUpPCCConfiguration(PCCDriver.Name, PCCDriver, upMethodNum, saMethodNum);

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
                constraints = new Dictionary<string, PETConfig.Constraint>(),
                intermediateVariables = new Dictionary<string, PETConfig.Parameter>(),
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

                var sourcePath = GetSourcePath(null, (MgaFCO) item.Impl);

                pccMetric.TestBenchMetricName = sourcePath[1];
                pccMetric.Name = sourcePath[0] + "." + pccMetric.TestBenchMetricName;

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

            if (testBenchOutputDir != null)
            {
                var jsonFile = Path.Combine(this.outputDirectory, this.testBenchOutputDir, "CyPhy", "PCCProperties.json");
                if (File.Exists(jsonFile))
                {
                    this.Logger.WriteInfo("Found defined PCC-Properties for the DynamicTestBench.");
                    results = Newtonsoft.Json.JsonConvert.DeserializeObject<List<PCCInputDistribution>>(File.ReadAllText(jsonFile));
                }
            }

            return results;
        }

        public void GenerateCode(CyPhy.ExcelWrapper excel)
        {
            if (excel.Attributes.ExcelFilename == "")
            {
                throw new ApplicationException(String.Format("ExcelWrapper {0} must specify Excel Filename", excel.Name));
            }
            var config = GenerateCode((CyPhy.ParametricTestBench)excel);

            var projectDir = Path.GetDirectoryName(Path.GetFullPath(excel.Impl.Project.ProjectConnStr.Substring("MGA=".Length)));
            config.details = new Dictionary<string, object>()
            {
                // TODO: maybe generate a relative path instead of making absolute here
                {"excelFile", Path.Combine(projectDir, excel.Attributes.ExcelFilename)},
            };
            config.type = "excel_wrapper.excel_wrapper.ExcelWrapper";

            var inputs = excel.Children.ParameterCollection.Select(fco => fco.Name).ToList();
            var outputs = excel.Children.MetricCollection.Select(fco => fco.Name).ToList();

            HashSet<string> xlInputs = new HashSet<string>();
            HashSet<string> xlOutputs = new HashSet<string>();
            Dictionary<string, ExcelInterop.ExcelType> types = new Dictionary<string, ExcelInterop.ExcelType>();
            Dictionary<string, List<int>> dimensions = new Dictionary<string, List<int>>();
            ExcelInterop.GetExcelInputsAndOutputs((string)config.details["excelFile"], (string name, string refersTo, ExcelInterop.ExcelType type, List<int> dims) =>
            {
                outputs.Remove(name);
                types[name] = type;
                dimensions[name] = dims;
            }, (string name, string refersTo, string value, ExcelInterop.ExcelType type, List<int> dims) =>
            {
                inputs.Remove(name);
                types[name] = type;
                dimensions[name] = dims;
            },
            () => { });
            config.details["varFile"] = generateXLFileJson(excel, types, dimensions);

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

            config.details = new Dictionary<string, object>()
            {
                {"filename", pyFilename}
            };
            config.type = "run_mdao.python_component.PythonComponent";
        }

        public void GenerateCode(CyPhy.AnalysisBlock analysisBlock)
        {
            var config = GenerateCode((CyPhy.ParametricTestBench)analysisBlock);

            var projectDir = Path.GetDirectoryName(Path.GetFullPath(analysisBlock.Impl.Project.ProjectConnStr.Substring("MGA=".Length)));

            System.Uri pyFileUri = new Uri(Path.Combine(projectDir, analysisBlock.Attributes.PyFilename));
            System.Uri outputUri = new Uri(this.outputDirectory + "\\");
            string pyFilename = Uri.UnescapeDataString(outputUri.MakeRelativeUri(pyFileUri).ToString());
            // n.b. keep forward slashes

            config.details = new Dictionary<string, object>()
            {
                {"filename", pyFilename},
                {"kwargs", CyPhyPETInterpreter.GetConfigurationParameters(analysisBlock)},
            };
            config.type = "run_mdao.python_component.PythonComponent";
        }

        public void GenerateCode(CyPhy.MATLABWrapper matlab)
        {
            var config = GenerateCode((CyPhy.ParametricTestBench)matlab);

            var projectDir = Path.GetDirectoryName(Path.GetFullPath(matlab.Impl.Project.ProjectConnStr.Substring("MGA=".Length)));
            config.details = new Dictionary<string, object>()
            {
                // TODO: maybe generate a relative path instead of making absolute here
                {"mFile", Path.Combine(projectDir, matlab.Attributes.MFilename)},
            };
            config.type = "matlab_wrapper.MatlabWrapper";
        }

        private void CopyFiles(CyPhy.ParametricTestBench wrapper)
        {
            var projectDir = Path.GetDirectoryName(Path.GetFullPath(wrapper.Impl.Project.ProjectConnStr.Substring("MGA=".Length)));
            CyPhyMasterInterpreter.CyPhyMasterInterpreterAPI.CopyFiles(wrapper.Children.CopyFilesCollection, projectDir, this.outputDirectory);
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
                        if (parent.ID == this.pet.ParentContainer.ID)
                        {
                            problemInput.outerSource = new string[] { source.Src.Name };
                        }
                        else
                        {
                            problemInput.outerSource = new string[] { parent.Name, source.Src.Name };
                        }
                    }
                }

                List<MgaFCO> realSources = GetTransitiveSources((MgaFCO)input.Impl, new HashSet<string>()
                {
                    typeof(CyPhy.Metric).Name,
                    typeof(CyPhy.FileOutput).Name,
                    // not a "real" source: typeof(CyPhy.ProblemOutput)
                    typeof(CyPhy.DesignVariable).Name,
                    typeof(CyPhy.PCCParameterLNormal).Name,
                    typeof(CyPhy.PCCParameterNormal).Name,
                    typeof(CyPhy.PCCParameterUniform).Name,
                    typeof(CyPhy.PCCParameterBeta).Name
                });
                var driverTypes = new HashSet<string>()
                {
                    typeof(CyPhy.DesignVariable).Name,
                    typeof(CyPhy.PCCParameterLNormal).Name,
                    typeof(CyPhy.PCCParameterNormal).Name,
                    typeof(CyPhy.PCCParameterUniform).Name,
                    typeof(CyPhy.PCCParameterBeta).Name
                };
                var desVarSources = realSources.Where(s => driverTypes.Contains(s.Meta.Name));
                if (desVarSources.Count() > 0)
                {
                    var desVar = desVarSources.First();
                    if (desVar.Meta.Name == typeof(CyPhy.DesignVariable).Name)
                    {
                        SetProblemInputValueFromDesignVariable(problemInput, desVar);
                    }
                    else
                    {
                        SetProblemInputValueFromPCCParameter(problemInput, desVar);
                    }
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
                            // FIXME is this right?
                            problemInput.value = "0.0";
                            problemInput.pass_by_obj = true;
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

        private static void SetProblemInputValueFromDesignVariable(SubProblem.ProblemInput problemInput, MgaFCO desVar)
        {
            var configDesignVariable = new DesignVariable();
            ParseDesignVariableRange(CyPhyClasses.DesignVariable.Cast(desVar).Attributes.Range, configDesignVariable);
            if (configDesignVariable.items != null)
            {
                if (configDesignVariable.items[0] is string)
                {
                    problemInput.value = String.Format("u'{0}'", escapePythonString((string)configDesignVariable.items[0]));
                    problemInput.pass_by_obj = true;
                }
                else
                {
                    problemInput.value = FormatDoubleForPython((double)configDesignVariable.items[0]);
                    problemInput.pass_by_obj = false;
                }
            }
            else
            {
                problemInput.value = FormatDoubleForPython((double)(configDesignVariable.RangeMin +
                    (configDesignVariable.RangeMax - configDesignVariable.RangeMin) / 2));
                problemInput.pass_by_obj = false;
            }
        }

        private static void SetProblemInputValueFromPCCParameter(SubProblem.ProblemInput problemInput, MgaFCO param)
        {
            string kind = param.Meta.Name;
            if (kind == typeof(CyPhy.PCCParameterBeta).Name)
            {
                var beta = CyPhyClasses.PCCParameterBeta.Cast(param);
                problemInput.value = FormatDoubleForPython(beta.Attributes.HighLimit);
            }
            else if (kind == typeof(CyPhy.PCCParameterBeta).Name)
            {
                var lnormal = CyPhyClasses.PCCParameterLNormal.Cast(param);
                problemInput.value = FormatDoubleForPython(lnormal.Attributes.LogScale);
            }
            else if (kind == typeof(CyPhy.PCCParameterNormal).Name)
            {
                var normal = CyPhyClasses.PCCParameterNormal.Cast(param);
                problemInput.value = FormatDoubleForPython(normal.Attributes.Mean);
            }
            else if (kind == typeof(CyPhy.PCCParameterUniform).Name)
            {
                var beta = CyPhyClasses.PCCParameterUniform.Cast(param);
                problemInput.value = FormatDoubleForPython(beta.Attributes.LowLimit);
            }
        }

        private static string FormatDoubleForPython(double v)
        {
            if (Double.IsNaN(v))
            {
                return "float(\"nan\")";
            }
            // (0.0).ToString returns 0, which Python parses as int
            if (v == Math.Floor(v))
            {
                return v.ToString() + ".0";
            }
            if (Double.IsPositiveInfinity(v))
            {
                return "float(\"inf\")";
            }
            if (Double.IsNegativeInfinity(v))
            {
                return "float(\"-inf\")";
            }

            // G17 ensures enough precision to parse the same value
            return v.ToString("G17", CultureInfo.InvariantCulture);
        }

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

            CopyFiles(excel);
            return config;
        }

        private string generateXLFileJson(CyPhy.ExcelWrapper excel, Dictionary<string, ExcelInterop.ExcelType> types, Dictionary<string, List<int>> dimensions)
        {
            List<Dictionary<string, object>> params_ = new List<Dictionary<string, object>>();
            foreach (var param in excel.Children.ParameterCollection)
            {
                ExcelInterop.ExcelType type = types[param.Name];
                List<int> dims = dimensions[param.Name];
                var details = new Dictionary<string, object>()
                {
                    {"name", param.Name },
                    {"type", type.ToString() },
                    {"desc", param.Attributes.Description },
                    // TODO sheet, row, column, units
                };
                if (type == ExcelInterop.ExcelType.Float)
                {
                    double val = 0.0;
                    Double.TryParse(param.Attributes.Value, NumberStyles.Float | NumberStyles.AllowThousands, CultureInfo.InvariantCulture, out val);
                    details["val"] = val;
                }
                else if (type == ExcelInterop.ExcelType.Str)
                {
                    details["val"] = param.Attributes.Value;
                }
                if (type == ExcelInterop.ExcelType.FloatArray || type == ExcelInterop.ExcelType.StrArray)
                {
                    details["dims"] = dims;
                }
                params_.Add(details);
            }
            List<Dictionary<string, object>> metrics = new List<Dictionary<string, object>>();
            foreach (var metric in excel.Children.MetricCollection)
            {
                ExcelInterop.ExcelType type = types[metric.Name];
                List<int> dims = dimensions[metric.Name];
                var details = new Dictionary<string, object>()
                {
                    {"name", metric.Name },
                    {"type", type.ToString() },
                    {"desc", metric.Attributes.Description },
                    // TODO sheet, row, column, units
                };
                if (type == ExcelInterop.ExcelType.Float)
                {
                    double val = 0.0;
                    Double.TryParse(metric.Attributes.Value, NumberStyles.Float | NumberStyles.AllowThousands, CultureInfo.InvariantCulture, out val);
                    details["val"] = val;
                }
                else if (type == ExcelInterop.ExcelType.Str)
                {
                    details["val"] = metric.Attributes.Value;
                }
                if (type == ExcelInterop.ExcelType.FloatArray || type == ExcelInterop.ExcelType.StrArray)
                {
                    details["dims"] = dims;
                }
                metrics.Add(details);
            }
            var macros = new List<string>();
            if (string.IsNullOrEmpty(excel.Attributes.Macro) == false)
            {
                macros.Add(excel.Attributes.Macro);
            }
            Dictionary<string, object> varFile = new Dictionary<string, object>()
            {
                {"unknowns", metrics},
                {"params", params_},
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
