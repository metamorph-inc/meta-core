namespace CyPhy2Modelica_v2.Modelica
{
    using System;
    using System.Collections.Generic;
    using System.IO;
    using System.Linq;
    using System.Text;
    using CyPhyGUIs;
    using Newtonsoft.Json;
    using Newtonsoft.Json.Linq;
    using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
    using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

    /// <summary>
    /// Class with methods for generating the <paramref name="MainPackage"></paramref>.
    /// </summary>
    /// Listed are applied naming conventions.
    /// <list type="number">
    /// <item> <c>someNamePort</c> - ModelicaConnector that may or may not be generated into Modelica code (if it will - it will be named as (2) before).</item>
    /// <item> <c>someNameConnector</c> - ModelicaConnector already part of modelica code or will be generated into such.</item>
    /// <item> <c>someName_mo</c> - instance of one of the modelica classes under Modelica-namespace (such will be generated into Modelica code).</item>
    /// </list>
    public class CodeGenerator
    {
        /// <summary>
        /// Top-level name of generated package.
        /// </summary>
        public const string MainPackage = "CyPhy";

        /// <summary>
        /// Prefix used for generated parameters inside (test)components to avoid name conflicts to the extended model.
        /// </summary>
        public const string ParameterPrefix = "__CyPhy__";

        /// <summary>
        /// Factor for scaling the GME canvas coordinates into Modelica Diagram coordinates
        /// </summary>
        public const int ScaleFactor = 4;

        /// <summary>
        /// Size of instances inside modelica (containing) models.
        /// </summary>
        public const int ComponentIconSize = 15;

        /// <summary>
        /// Parameter ranges are split based on any of these strings.
        /// </summary>
        public static string[] RangeDividers = new string[] { ",", ";", ".." };

        /// <summary>
        /// Mapping from instances in the modelica test-bench to ComponentInfos.
        /// </summary>
        private Dictionary<string, ComponentInfo> instanceURIMap;

        /// <summary>
        /// Mapping from component/test-components/component-assemblies in the modelica tree to ComponentInfos.
        /// </summary>
        private Dictionary<string, ComponentInfo> modelURIMap;

        /// <summary>
        /// Mapping from the extended modelica models to ComponentsInfos (from ModelicaModels).
        /// </summary>
        private Dictionary<string, ComponentInfo> extendsURIMap;

        /// <summary>
        /// Mapping from GME-ID to a component in the project tree.
        /// </summary>
        private Dictionary<string, Component> treeComponents;

        /// <summary>
        /// Traceability map (given by CyPhyElaborateCS and sometimes MasterInterpreter too).
        /// </summary>
        private CyPhyCOMInterfaces.IMgaTraceability traceability;

        /// <summary>
        /// Contains info about PCC-Properties defined in the registry nodes of Components.
        /// (Consumed by CyPhyPET.)
        /// </summary>
        private List<PCCInputDistribution> pccInputs;

        /// <summary>
        /// The regular simulation test-bench that is to be executed using the generated package.
        /// </summary>
        private TestBench testBench_mo;

        /// <summary>
        /// Tree for Components that will be written into one file
        /// </summary>
        private Package componentsPackage_mo;

        /// <summary>
        /// Contains info of which CyPhyModelicaModel should be chosen from a certain classification of Componets.
        /// </summary>
        private Dictionary<string, Dictionary<string, string>> fidelitySettings;

        /// <summary>
        /// List of top-level modelica packages.
        /// </summary>
        private List<string> reservedPackageNames;

        /// <summary>
        /// Metrics that will be generated in the verification test-bench.
        /// </summary>
        private List<RequirementMetric> requirementMetrics;

        /// <summary>
        /// User settings for current invocation.
        /// </summary>
        private CyPhy2Modelica_v2Settings modelicaSettings;

        /// <summary>
        /// Intializes a new instance of the <see cref="CodeGenerator"/> class.
        /// </summary>
        /// <param name="parameters">Passed through the Main function in CyPhyGUIs API.</param>
        /// <param name="logger">Instance of logger to log to.</param>
        /// <param name="traceability">Traceability map from CyPhyElborate (and if called by it, the MasterInterpreter too).</param>
        public CodeGenerator(CyPhyGUIs.IInterpreterMainParameters parameters, CyPhyGUIs.GMELogger logger, CyPhyCOMInterfaces.IMgaTraceability traceability)
        {
            this.Logger = logger;
            this.MainParameters = parameters;
            this.modelicaSettings = this.MainParameters.config as CyPhy2Modelica_v2Settings;
            this.traceability = traceability;
            this.LimitDefintion = new LimitDefinition();
            this.reservedPackageNames = new List<string>() { "Modelica", MainPackage };
            this.instanceURIMap = new Dictionary<string, ComponentInfo>();
            this.modelURIMap = new Dictionary<string, ComponentInfo>();
            this.extendsURIMap = new Dictionary<string, ComponentInfo>();
            this.pccInputs = new List<PCCInputDistribution>();
            this.ModelicaURIMap = new ComponentMap()
            {
                InstanceMapping = this.instanceURIMap,
                ModelMapping = this.modelURIMap,
                ExtendsMapping = this.extendsURIMap
            };
            var externalPackages = (this.MainParameters.config as CyPhy2Modelica_v2Settings).IncludeDirectoryPath;
            foreach (var packagePath in externalPackages)
            {
                this.reservedPackageNames.Add(ModelicaLibrary.GetLibraryFromPath(packagePath).Name);
            }

            this.requirementMetrics = new List<RequirementMetric>();
        }

        /// <summary>
        /// Gets or sets a value indicating whether or not to generate all components (containing a ModelicaModel)
        /// in the modelica package (if rules are passed).
        /// </summary>
        public bool GenerateUnusedComponents { get; set; }

        /// <summary>
        /// Gets or sets settings for the solver.
        /// </summary>
        /// <seealso cref="Component"/>
        public Modelica.SolverSettings SolverSettings { get; set; }

        /// <summary>
        /// Gets or sets class containing all limits defined in the used ModelicaModels.
        /// </summary>
        public LimitDefinition LimitDefintion { get; set; }

        /// <summary>
        /// Gets or sets a mapping from ModelicaURIs to ComponentInfos.
        /// </summary>
        /// <seealso cref="ComponentInfo"/>
        public ComponentMap ModelicaURIMap { get; set; }

        /// <summary>
        /// Gets or sets an instance of logger.
        /// </summary>
        public CyPhyGUIs.GMELogger Logger { get; set; }

        /// <summary>
        /// Gets Path to result directory.
        /// </summary>
        private string OutputDir
        {
            get
            {
                return this.MainParameters.OutputDirectory;
            }
        }

        /// <summary>
        /// Gets or sets the MainParameters from the interpreter call.
        /// </summary>
        private CyPhyGUIs.IInterpreterMainParameters MainParameters { get; set; }

        /// <summary>
        /// Looks and, if successful, returns the fidelity setting for a given test-bench.
        /// </summary>
        /// <param name="testBench">Can contain registry node with fidelity settings.</param>
        /// <returns>Dictionary with info about FidelitySetting</returns>
        public static Dictionary<string, Dictionary<string, string>> GetFidelitySettingsFromTestBench(CyPhy.TestBench testBench)
        {
            var fidelitySettings = new Dictionary<string, Dictionary<string, string>>();
            var strSettings = (testBench.Impl as GME.MGA.MgaFCO).RegistryValue["FidelitySettings"];

            if (string.IsNullOrEmpty(strSettings) == false)
            {
                fidelitySettings = Newtonsoft.Json.JsonConvert.DeserializeObject<Dictionary<string, Dictionary<string, string>>>(strSettings);
            }

            return fidelitySettings;
        }

        /// <summary>
        /// Converts the range attribute of a Parameter in CyPhy to a modelica min/max modifier string.
        /// The rule "ParametersHaveValidIncomingFlowsAndRanges" in Global.cs, makes sure that the range 
        /// is either two doubles (or -inf/inf) after split by RangeDividers.
        /// Since META-2465 surrounding brackets are also supported, e.g. [a, b].
        /// </summary>
        /// <param name="parameter">CyPhyParameter to which the modifier belongs.</param>
        /// <returns>Modelica min/max modifier string.</returns>
        private string GetParameterModifier(CyPhy.Parameter parameter)
        {
            string modifier = string.Empty;
            string range = parameter.Attributes.Range.Trim().TrimStart('[').TrimEnd(']').Trim();
            if (string.IsNullOrWhiteSpace(range) == false && range != "-inf..inf")
            {
                var values = range.Split(RangeDividers, StringSplitOptions.None);
                if (values.Length == 2)
                {
                    if (values[0].Contains("-inf") && values[1].Contains("inf"))
                    {
                        // Do not generate a min/max.
                    }
                    else if (values[0].Contains("-inf"))
                    {
                        modifier = string.Format("(max={0})", values[1].Trim());
                    }
                    else if (values[1].Contains("inf"))
                    {
                        modifier = string.Format("(min={0})", values[0].Trim());
                    }
                    else
                    {
                        modifier = string.Format("(min={0}, max={1})", values[0].Trim(), values[1].Trim());
                    }
                }
            }

            return modifier;
        }

        /// <summary>
        /// Get the value-expression for parameter encapsulated in the ModelicaValue.
        /// </summary>
        /// <param name="parameter">CyPhyParameter to which the ModelicaValue belongs.</param>
        /// <returns>Value-expression as part of the ModelicaValue.</returns>
        private ModelicaValue GetParameterModelicaValue(CyPhy.Parameter parameter)
        {
            var modelicaValue = new ModelicaValue(parameter.Attributes.Value);

            var vf = parameter.SrcConnections.ValueFlowCollection.FirstOrDefault();
            if (vf != null)
            {
                if (vf.ParentContainer.ID == parameter.ParentContainer.ID)
                {
                    // Value flow takes place on same level as receiving parameter.
                    if (vf.SrcEnds.Parameter != null)
                    {
                        var srcParameter = vf.SrcEnds.Parameter;
                        if (this.ParentContainerIsDynamic(srcParameter))
                        {
                            var parameterName = this.ParentIsComponent(srcParameter) ? ParameterPrefix + srcParameter.Name : srcParameter.Name;

                            if (srcParameter.ParentContainer.ID != parameter.ParentContainer.ID)
                            {
                                // The Parameter is in a subModel (prepend instance name of the subModel).
                                parameterName = string.Format("{0}.{1}", srcParameter.ParentContainer.Name, parameterName);
                            }

                            modelicaValue = new ModelicaValue(parameterName);
                        }
                    }
                    else if (vf.SrcEnds.SimpleFormula != null)
                    {
                        modelicaValue = new ModelicaValue(this.GetFormulaExpression(vf.SrcEnds.SimpleFormula));
                    }
                    else
                    {
                        // If Property or CustomFormula or any other kind.-> Use the value passed from FormulaEvaluator,
                        // i.e. as given in the instantiation of modelicaValue in beginning of this method.
                    }
                }
            }

            switch (parameter.Attributes.DataType)
            {
                case CyPhyClasses.Parameter.AttributesClass.DataType_enum.Boolean:
                    modelicaValue.ClassName = "Boolean";
                    break;
                case CyPhyClasses.Parameter.AttributesClass.DataType_enum.Float:
                    modelicaValue.ClassName = "Real";
                    break;
                case CyPhyClasses.Parameter.AttributesClass.DataType_enum.Integer:
                    modelicaValue.ClassName = "Integer";
                    break;
                case CyPhyClasses.Parameter.AttributesClass.DataType_enum.String:
                    if (modelicaValue.Value == "false" || modelicaValue.Value == "true")
                    {
                        modelicaValue.ClassName = "Boolean";
                    }
                    else if (modelicaValue.Value.StartsWith("\"") && modelicaValue.Value.EndsWith("\""))
                    {
                        modelicaValue.ClassName = "String";
                    }
                    else
                    {
                        modelicaValue.ClassName = "Real";
                    }

                    break;
            }

            return modelicaValue;
        }

        /// <summary>
        /// Starts the generation of modelica code. Only method invocation that is needed on an instance of this class.
        /// </summary>
        public void GenerateModelicaCode()
        {
            var project = this.MainParameters.Project;
            var testBench = CyPhyClasses.TestBench.Cast(this.MainParameters.CurrentFCO);
            this.fidelitySettings = GetFidelitySettingsFromTestBench(testBench);
            this.testBench_mo = new TestBench(testBench);
            this.TagComponentsAndComponentAssembliesWithIsDynamicModel();

            this.treeComponents = new Dictionary<string, Component>();

            var projectRootFolder = CyPhyClasses.RootFolder.GetRootFolder(project);
            this.BuildUpAllComponets(projectRootFolder);

            this.SolverSettings = new Modelica.SolverSettings(testBench.Children.SolverSettingsCollection.FirstOrDefault());
            this.testBench_mo.SolverSettings = this.SolverSettings;
            
            this.Logger.WriteDebug("Solver settings being used:");
            this.Logger.WriteDebug("- [Dymola] Solver: {0}", this.SolverSettings.DymolaSolver);
            this.Logger.WriteDebug("- [OpenModelica] Solver: {0}", this.SolverSettings.OpenModelicaSolver);
            this.Logger.WriteDebug("- Tool selection for JobManager: {0}", this.SolverSettings.ToolSelection);
            this.Logger.WriteDebug("- Start time: {0}", this.SolverSettings.StartTime);
            this.Logger.WriteDebug("- Stop time: {0}", this.SolverSettings.StopTime);

            this.BuildUpTestBench();

            this.WriteModelicaCode();
        }

        #region ParameterPassing

        /// <summary>
        /// Get the expression encapsulated in the UnitParameter for a parameter that gets its value from outside its model.
        /// </summary>
        /// <param name="parentID">ID of parent of the model that contains the parameter.</param>
        /// <param name="parameter">CyPhyParameter to which the returned UnitParameter will be correspond to.</param>
        /// <param name="vf">Incoming value-flow to parameter.</param>
        /// <returns>Value-expression in UnitParameter.</returns>
        private UnitParameter GetArgumentParameter(string parentID, CyPhy.Parameter parameter, CyPhy.ValueFlow vf)
        {
            ModelicaValue modelicaValue = null;
            if (vf.SrcEnds.Parameter != null)
            {
                var srcParameter = vf.SrcEnds.Parameter;
                if (this.ParentContainerIsDynamic(srcParameter))
                {
                    var parameterName = this.ParentIsComponent(srcParameter) ? ParameterPrefix + srcParameter.Name : srcParameter.Name;

                    // The Parameter is in a subModel (prepend instance name of the subModel).
                    if (srcParameter.ParentContainer.ID != parentID)
                    {
                        parameterName = string.Format("{0}.{1}", srcParameter.ParentContainer.Name, parameterName);
                    }

                    modelicaValue = new ModelicaValue(parameterName);
                }
                else
                {
                    modelicaValue = new ModelicaValue(srcParameter.Attributes.Value);
                }
            }
            else if (vf.SrcEnds.SimpleFormula != null)
            {
                modelicaValue = new ModelicaValue(this.GetFormulaExpression(vf.SrcEnds.SimpleFormula));
            }
            else if (vf.SrcEnds.Property != null)
            {
                modelicaValue = new ModelicaValue(vf.SrcEnds.Property.Attributes.Value);
            }
            else if (vf.SrcEnds.CustomFormula != null)
            {
                modelicaValue = new ModelicaValue(parameter.Attributes.Value);
            }

            var parameter_mo = new UnitParameter()
            {
                Name = this.ParentIsComponent(parameter) ? ParameterPrefix + parameter.Name : parameter.Name,
                Value = modelicaValue
            };

            return parameter_mo;
        }

        /// <summary>
        /// Returns the output expression (in modelica syntax) from a SimpleFormula.
        /// Depending on the incoming value flow sources the following is extracted;
        /// <list type="number">
        /// <item> <c>Property</c> - The value is used</item>
        /// <item> <c>Parameter</c> - The (relative) modelica URI (basically the name) is used.</item>
        /// <item> <c>SimpleForumla</c> - Calls the function recursively.</item>
        /// <item> <c>CustomForumla</c> - Not allowed.</item>
        /// </list>
        /// </summary>
        /// <param name="sf">SimpleFormula from which the expression is obtained.</param>
        /// <returns>Expression in modelica syntax</returns>
        private string GetFormulaExpression(CyPhy.SimpleFormula sf)
        {
            List<string> values = new List<string>();
            foreach (var vf in sf.SrcConnections.ValueFlowCollection)
            {
                if (vf.SrcEnds.Property != null)
                {
                    values.Add(this.GetPropertyModelicaValue(vf.SrcEnds.Property).Value);
                }
                else if (vf.SrcEnds.Parameter != null)
                {
                    var srcParameter = vf.SrcEnds.Parameter;
                    if (this.ParentContainerIsDynamic(srcParameter))
                    {
                        var parameterName = this.ParentIsComponent(srcParameter) ? ParameterPrefix + srcParameter.Name : srcParameter.Name;

                        if (srcParameter.ParentContainer.ID != sf.ParentContainer.ID)
                        {
                            // The Parameter is in a subModel (prepend instance name of the subModel).
                            parameterName = string.Format("{0}.{1}", srcParameter.ParentContainer.Name, parameterName);
                        }

                        values.Add(parameterName);
                    }
                    else
                    {
                        values.Add((new ModelicaValue(srcParameter.Attributes.Value)).Value);
                    }
                }
                else if (vf.SrcEnds.SimpleFormula != null)
                {
                    values.Add(this.GetFormulaExpression(vf.SrcEnds.SimpleFormula));
                }
                else
                {
                    // Can't get logger here since method is static 
                }
            }

            var value = new StringBuilder();
            switch (sf.Attributes.Method)
            {
                case CyPhyClasses.SimpleFormula.AttributesClass.Method_enum.Addition:
                    value.Append("sum");
                    break;
                case CyPhyClasses.SimpleFormula.AttributesClass.Method_enum.Multiplication:
                    value.Append("product");
                    break;
                case CyPhyClasses.SimpleFormula.AttributesClass.Method_enum.Maximum:
                    value.Append("max");
                    break;
                case CyPhyClasses.SimpleFormula.AttributesClass.Method_enum.Minimum:
                    value.Append("min");
                    break;
                case CyPhyClasses.SimpleFormula.AttributesClass.Method_enum.ArithmeticMean:
                    value.Append("sum");
                    break;
                case CyPhyClasses.SimpleFormula.AttributesClass.Method_enum.GeometricMean:
                    value.Append("product");
                    break;
            }

            value.Append("({");
            bool first = true;
            values.Sort();
            foreach (var val in values)
            {
                if (first)
                {
                    value.Append(val);
                    first = false;
                }
                else
                {
                    value.Append(", " + val);
                }
            }

            value.Append("})");
            if (sf.Attributes.Method == CyPhyClasses.SimpleFormula.AttributesClass.Method_enum.ArithmeticMean)
            {
                value.Append("/" + values.Count);
            }
            else if (sf.Attributes.Method == CyPhyClasses.SimpleFormula.AttributesClass.Method_enum.GeometricMean)
            {
                value.Append(string.Format("^(1/{0})", values.Count));
            }

            return value.ToString();
        }

        /// <summary>
        /// Checks if the given parameter is contained in a Component or a TestComponent.
        /// </summary>
        /// <param name="parameter">CyPhy Parameter</param>
        /// <returns>Is the parent component or testcomponent?</returns>
        private bool ParentIsComponent(CyPhy.Parameter parameter)
        {
            return parameter.ParentContainer.Kind == typeof(CyPhy.Component).Name || parameter.ParentContainer.Kind == typeof(CyPhy.TestComponent).Name;
        }

        /// <summary>
        /// Get the ModelicaValue for a Property. Not the ClassName will always be Real (not used for Properties in any case).
        /// If elaborateType is set to true, the type will be looked for in property.Attributes.DataType. If it is set to string
        /// and can be parsed as a boolean it is set to Boolean, else to Real.
        /// </summary>
        /// <param name="property">The Property to get the value for.</param>
        /// <param name="elaborateType">Indicates if the ClassName is of importance.</param>
        /// <returns>The ModelicaValue from the property.</returns>
        private ModelicaValue GetPropertyModelicaValue(CyPhy.Property property, bool elaborateType = false)
        {
            var modelicaValue = new ModelicaValue(property.Attributes.Value);
            if (elaborateType)
            {
                switch (property.Attributes.DataType)
                {
                    case CyPhyClasses.Property.AttributesClass.DataType_enum.Boolean:
                        modelicaValue.ClassName = "Boolean";
                        break;
                    case CyPhyClasses.Property.AttributesClass.DataType_enum.Float:
                        modelicaValue.ClassName = "Real";
                        break;
                    case CyPhyClasses.Property.AttributesClass.DataType_enum.Integer:
                        modelicaValue.ClassName = "Integer";
                        break;
                    case CyPhyClasses.Property.AttributesClass.DataType_enum.String:
                        if (modelicaValue.Value == "false" || modelicaValue.Value == "true")
                        {
                            modelicaValue.ClassName = "Boolean";
                        }
                        else if (modelicaValue.Value.StartsWith("\"") && modelicaValue.Value.EndsWith("\""))
                        {
                            modelicaValue.ClassName = "String";
                        }
                        else
                        {
                            modelicaValue.ClassName = "Real";
                        }

                        break;
                }
            }
            else
            {
                modelicaValue.ClassName = "Real";
            }

            return modelicaValue;
        }

        /// <summary>
        /// Extracts and returns the value for a ModelicaParameter.
        /// The value will be;
        /// <list type="number">
        /// <item> <c>Property</c> - If inTree -> get the value from the Property, else -> its given value.</item>
        /// <item> <c>Parameter</c> - The name (+ ParameterPrefix) of the Parameter.</item>
        /// </list>
        /// </summary>
        /// <param name="parameter">The ModelicaParameter.</param>
        /// <param name="inTree">If the component is an instance or belongs to a component in the tree.</param>
        /// <returns>The string value for parameter.</returns>
        private string GetModelicaParameterValue(CyPhy.ModelicaParameter parameter, bool inTree = false)
        {
            var value = string.IsNullOrWhiteSpace(parameter.Attributes.Value) ? "0" : parameter.Attributes.Value;

            var connection = parameter.SrcConnections.ModelicaParameterPortMapCollection.FirstOrDefault();
            if (connection != null)
            {
                var srcEndKind = (connection.Impl as GME.MGA.IMgaSimpleConnection).Src.MetaBase.Name;
                if (srcEndKind == typeof(CyPhy.Parameter).Name)
                {
                    value = ParameterPrefix + (connection.SrcEnd as CyPhy.Parameter).Name;
                }
                else if (inTree == true && srcEndKind == typeof(CyPhy.Property).Name)
                {
                    value = (connection.Impl as GME.MGA.IMgaSimpleConnection).Src.StrAttrByName["Value"];
                }
            }

            return value;
        }

        #endregion

        /// <summary>
        /// Builds an abstract tree with info about the packages and models in the component sub-package.
        /// </summary>
        /// <param name="rootFolder">rootFolder of the project.</param>
        private void BuildUpAllComponets(CyPhy.RootFolder rootFolder)
        {
            this.componentsPackage_mo = new Package()
            {
                Name = rootFolder.Name,
                FullName = string.Format("{0}.{1}.{2}", MainPackage, "Components", rootFolder.Name)
            };

            foreach (var componentsFolder in rootFolder.Children.ComponentsCollection)
            {
                if (FolderFilter.FolderHasModelicaComponents(componentsFolder))
                {
                    var folder = new FolderTreeNode(componentsFolder);
                    FolderFilter.PopulateWithFilteredNodes(ref folder);
                    var subPackage_mo = new Package
                    {
                        Name = folder.Name,
                        FullName = this.componentsPackage_mo.FullName + "." + folder.Name,
                        IndentLevel = 1,
                    };

                    this.componentsPackage_mo.Children.Add(subPackage_mo);
                    this.BuildUpAllComponentsRec(folder as FolderTreeNode, subPackage_mo);
                }
            }
        }

        /// <summary>
        /// Recursive call for BuildUpAllComponents
        /// </summary>
        /// <param name="folder">Folder at the current depth.</param>
        /// <param name="package_mo">modelica package at the current depth</param>
        /// <param name="lvl">depth (used for indentation).</param>
        private void BuildUpAllComponentsRec(FolderTreeNode folder, Package package_mo, int lvl = 1)
        {
            foreach (var component in folder.Children.OfType<ComponentTreeNode>().Select(c => c.Component))
            {
                var isInDesign = string.IsNullOrEmpty((component.Impl as GME.MGA.IMgaFCO).RegistryNode["IsInDesign"].Value) == false;

                if (isInDesign == false && this.GenerateUnusedComponents == false)
                {
                    continue;
                }

                var modelicaModel = this.GetModelicaModelFromComponent(component);
                if (modelicaModel == null)
                {
                    continue;
                }

                // Don't check the rules twice for components in the design
                if (isInDesign == false)
                {
                    if (this.CheckRulesOnComponent(component, modelicaModel) == false)
                    {
                        this.Logger.WriteDebug("Component, {0}, will not be generated in package.", component.Name);
                        continue;
                    }
                }

                var component_mo = new Component(component)
                {
                    FullName = package_mo.FullName + "." + component.Name,
                    Extends = modelicaModel.Attributes.Class
                };

                // Add all defined Parameters at the component-level
                foreach (var cyPhyParameter in component.Children.ParameterCollection)
                {
                    // META-3622
                    if (String.IsNullOrWhiteSpace(cyPhyParameter.Attributes.Dimension) || cyPhyParameter.Attributes.Dimension.Trim() == "1")
                    {
                        var parameter_mo = new UnitParameter()
                        {
                            Name = CodeGenerator.ParameterPrefix + cyPhyParameter.Name,
                            Value = this.GetParameterModelicaValue(cyPhyParameter),
                            Modifier = this.GetParameterModifier(cyPhyParameter)
                        };

                        component_mo.CyPhyParameters.Add(parameter_mo);
                    }
                }

                // Add all modelicaParameter from the ModelicaModel
                foreach (var parameter in modelicaModel.Children.ModelicaParameterCollection)
                {
                    var value = this.GetModelicaParameterValue(parameter, true);
                    if (string.IsNullOrWhiteSpace(value) == false)
                    {
                        component_mo.Parameters.Add(
                            new UnitParameter()
                            {
                                Name = parameter.Name,
                                Value = new ModelicaValue(value)
                            });
                    }
                }

                // Add all modelicaRedeclares from the ModelicaModel
                foreach (var redeclareParameter in modelicaModel.Children.ModelicaRedeclareCollection)
                {
                    var value = redeclareParameter.Attributes.Value;

                    if (string.IsNullOrWhiteSpace(value) == false)
                    {
                        component_mo.Parameters.Add(
                            new UnitRedeclareParameter(redeclareParameter)
                            {
                                Value = new ModelicaValue(value)
                            });
                    }
                }

                this.treeComponents.Add(component.ID, component_mo);
                package_mo.Models.Add(component_mo);
            }

            lvl += 1;
            foreach (var subFolder in folder.Children.OfType<FolderTreeNode>())
            {
                var subPackage_mo = new Package
                {
                    Name = subFolder.Name,
                    FullName = package_mo.FullName + "." + subFolder.Name,
                    IndentLevel = lvl,
                };

                package_mo.Children.Add(subPackage_mo);
                this.BuildUpAllComponentsRec(subFolder as FolderTreeNode, subPackage_mo, lvl);
            }
        }

        /// <summary>
        /// Does most of the work for GenerateModelicaCode()
        /// </summary>
        private void BuildUpTestBench()
        {
            var testBench = this.testBench_mo.Impl;
            this.instanceURIMap.Add(
                string.Format("{0}.TestBenches.{1}", MainPackage, testBench.Name),
                new ComponentInfo(testBench.Impl as GME.MGA.IMgaFCO, "ContextAssembly", this.traceability));

            // ------- Parameters/Properties & Metrics ---------
            foreach (var defParameter in testBench.Children.ParameterCollection)
            {
                var parameter_mo = new UnitParameter()
                {
                    Name = defParameter.Name,
                    Value = this.GetParameterModelicaValue(defParameter),
                    Modifier = this.GetParameterModifier(defParameter)
                };

                this.testBench_mo.Parameters.Add(parameter_mo);
            }

            foreach (var defProperty in testBench.Children.PropertyCollection)
            {
                var parameter_mo = new UnitParameter()
                {
                    Name = defProperty.Name,
                    Value = this.GetPropertyModelicaValue(defProperty, true)
                };

                this.testBench_mo.Parameters.Add(parameter_mo);
            }

            foreach (var metric in testBench.Children.MetricCollection)
            {
                var metric_mo = new Metric()
                {
                    Name = metric.Name,
                    Description = metric.Attributes.Description,
                };

                if (metric.SrcConnections.PPOutput2MetricCollection.Count() == 1)
                {
                    metric_mo.PostProcessing = true;
                }

                this.testBench_mo.Metrics.Add(metric_mo);
            }

            var ca = testBench.Children.ComponentAssemblyCollection.FirstOrDefault();
            var componentAssemblyFullName = string.Format("{0}.ComponentAssemblies.{1}", MainPackage, ca.Name);
            var hasInnerCAs = ca.Children.ComponentAssemblyCollection.Count() > 0;
            if (hasInnerCAs)
            {
                componentAssemblyFullName += "." + ca.Name;
            }

            var componentAssembly_mo = new ComponentAssembly(ca)
            {
                FullName = componentAssemblyFullName,
                HasInnerCAs = hasInnerCAs,
                ConstrainedBys = this.modelicaSettings.GenerateConstrainedBys
            };

            this.testBench_mo.ComponentAssemblies.Add(componentAssembly_mo);

            // Make sure there are margins to the upper- and left-border
            int canvasX = ca.GenericAspect.X > ScaleFactor * 3 * ComponentIconSize ? ca.GenericAspect.X : ScaleFactor * 3 * ComponentIconSize;
            int canvasY = ca.GenericAspect.Y > ScaleFactor * 2 * ComponentIconSize ? ca.GenericAspect.Y : ScaleFactor * 2 * ComponentIconSize;

            // Make sure there are margins to the bottom- and right-border
            this.testBench_mo.CanvasXMax = canvasX + (ScaleFactor * 3 * ComponentIconSize);
            this.testBench_mo.CanvasYMax = canvasY + (ScaleFactor * 2 * ComponentIconSize);

            var componentAssemblyInstance_mo = new ComponentAssemblyInstance(ca)
            {
                InstanceOf = componentAssembly_mo,
                CanvasX = canvasX,
                CanvasY = canvasY
            };

            foreach (var parameter in ca.Children.ParameterCollection)
            {
                var vf = parameter.SrcConnections.ValueFlowCollection.FirstOrDefault();

                // The Value flow is in the TestBench (if in the CA it's dealt with of the definition of the Parameter)
                if (vf != null && vf.ParentContainer.ID == testBench.ID)
                {
                    var parameter_mo = this.GetArgumentParameter(testBench.ID, parameter, vf);

                    componentAssemblyInstance_mo.Parameters.Add(parameter_mo);
                }
            }

            var componentAssemblyInfo = new ComponentInfo(ca.Impl as GME.MGA.IMgaFCO, "PlantAssembly", this.traceability);
            this.instanceURIMap.Add(string.Format("{0}.TestBenches.{1}.{2}", MainPackage, this.testBench_mo.Name, ca.Name), componentAssemblyInfo);
            this.modelURIMap.Add(string.Format("{0}.ComponentAssemblies.{1}.{1}", MainPackage, ca.Name), componentAssemblyInfo);

            this.testBench_mo.ComponentAssemblyInstances.Add(componentAssemblyInstance_mo);

            // ---------- SystemUnderTest (ComponentAssembly)-------
            this.BuildUpComponentAssembly(componentAssembly_mo, null);

            // ----------- TestComponents ----------
            foreach (var testComponent in testBench.Children.TestComponentCollection)
            {
                var modelicaModel = testComponent.Children.ModelicaModelCollection.FirstOrDefault();
                if (modelicaModel == null)
                {
                    continue;
                }

                this.AddTestComponent(testComponent, modelicaModel);
            }

            // -------- Environments ------
            foreach (var environment in testBench.Children.EnvironmentCollection)
            {
                this.AddEnvironment(environment);
                this.instanceURIMap.Add(
                string.Format("{0}.TestBenches.{1}.{2}", MainPackage, testBench.Name, environment.Name),
                    new ComponentInfo(environment.Impl as GME.MGA.IMgaFCO, "Environment", this.traceability));
            }
        }

        /// <summary>
        /// Adds TestComponent and instance to test-bench. Populates all mapping related to the TestComponent.
        /// </summary>
        /// <param name="testComponent">CyPhy TestComponent.</param>
        /// <param name="modelicaModel">CyPhy ModelicaModel of the testComponent.</param>
        private void AddTestComponent(CyPhy.TestComponent testComponent, CyPhy.ModelicaModel modelicaModel)
        {
            Modelica.Component testComponent_mo = null;

            // META-1624 : TestComponents in the test-bench are all seen as self-contained models 
            //             and will be generated as they appear there inside the Modelica package-tree.
            testComponent_mo = new Component(testComponent)
            {
                FullName = MainPackage + ".TestComponents." + testComponent.Name,
                Extends = modelicaModel.Attributes.Class,
            };

            // Add all defined Parameters at the component-level
            foreach (var cyPhyParameter in testComponent.Children.ParameterCollection)
            {
                var parameter_mo = new UnitParameter()
                {
                    Name = CodeGenerator.ParameterPrefix + cyPhyParameter.Name,
                    Value = this.GetParameterModelicaValue(cyPhyParameter),
                    Modifier = this.GetParameterModifier(cyPhyParameter)
                };

                testComponent_mo.CyPhyParameters.Add(parameter_mo);
            }

            // Add all modelicaParameters from the ModelicaModel.
            foreach (var parameter in modelicaModel.Children.ModelicaParameterCollection)
            {
                var value = this.GetModelicaParameterValue(parameter);
                if (string.IsNullOrWhiteSpace(value) == false)
                {
                    testComponent_mo.Parameters.Add(
                        new UnitParameter()
                        {
                            Name = parameter.Name,
                            Value = new ModelicaValue(value)
                        });
                }
            }

            // Add all modelicaRedeclares from the ModelicaModel.
            foreach (var redeclareParameter in modelicaModel.Children.ModelicaRedeclareCollection)
            {
                var value = redeclareParameter.Attributes.Value;

                if (string.IsNullOrWhiteSpace(value) == false)
                {
                    testComponent_mo.Parameters.Add(
                        new UnitRedeclareParameter(redeclareParameter)
                        {
                            Value = new ModelicaValue(value)
                        });
                }
            }

            this.testBench_mo.TestComponents.Add(testComponent_mo);

            // Make sure there are margins to the upper- and left-border
            int canvasX = testComponent.GenericAspect.X > ScaleFactor * ComponentIconSize ? 
                testComponent.GenericAspect.X : ScaleFactor * ComponentIconSize;
            int canvasY = testComponent.GenericAspect.Y > ScaleFactor * ComponentIconSize ? 
                testComponent.GenericAspect.Y : ScaleFactor * ComponentIconSize;

            // Make sure there are margins to the bottom- and right-border
            this.testBench_mo.CanvasXMax = canvasX + (ScaleFactor * ComponentIconSize) > this.testBench_mo.CanvasXMax ? 
                canvasX + (ScaleFactor * ComponentIconSize) : this.testBench_mo.CanvasXMax;
            this.testBench_mo.CanvasYMax = canvasY + (ScaleFactor * ComponentIconSize) > this.testBench_mo.CanvasYMax ? 
                canvasY + (ScaleFactor * ComponentIconSize) : this.testBench_mo.CanvasYMax;

            var testComponentInstance_mo = new ComponentInstance(testComponent)
            {
                InstanceOf = testComponent_mo,
                CanvasX = canvasX,
                CanvasY = canvasY
            };

            var testComponentInfo = new ComponentInfo(testComponent.Impl as GME.MGA.IMgaFCO, "ContextModel", this.traceability);
            this.instanceURIMap.Add(string.Format("{0}.TestBenches.{1}.{2}", MainPackage, this.testBench_mo.Name, testComponent.Name), testComponentInfo);
            var uri = MainPackage + ".TestComponents." + testComponent.Name;
            if (this.modelURIMap.ContainsKey(uri) == false)
            {
                this.modelURIMap.Add(uri, testComponentInfo);
            }

            if (this.extendsURIMap.ContainsKey(testComponent_mo.Extends) == false)
            {
                var testComponentExtendInfo = new ComponentInfo(modelicaModel.Impl as GME.MGA.IMgaFCO, "ContextModel", this.traceability);
                this.extendsURIMap.Add(testComponent_mo.Extends, testComponentExtendInfo);
            }

            // Parameters of instance
            foreach (var parameter in testComponent.Children.ParameterCollection)
            {
                var vf = parameter.SrcConnections.ValueFlowCollection.FirstOrDefault();
                if (vf != null && vf.ParentContainer.ID == this.testBench_mo.Impl.ID)
                {
                    var parameter_mo = this.GetArgumentParameter(this.testBench_mo.Impl.ID, parameter, vf);
                    testComponentInstance_mo.Parameters.Add(parameter_mo);
                }
            }

            this.testBench_mo.TestComponentInstances.Add(testComponentInstance_mo);

            // ModelicaConnectors
            foreach (var port in testComponent.Children.ModelicaConnectorCollection)
            {
                foreach (var dstPort in port.DstConnections.PortCompositionCollection
                    .Select(c => c.DstEnds.ModelicaConnector)
                    .Where(p => p.ParentContainer is CyPhy.ModelicaModel == false))
                {
                    CyPhy.ModelicaConnector srcConnector = this.GetModelicaModelConnectorFromComponentPort(port);
                    if (srcConnector == null)
                    {
                        continue;
                    }

                    CyPhy.ModelicaConnector dstConnector = null;

                    if (dstPort.ParentContainer is CyPhy.TestComponent)
                    {
                        dstConnector = this.GetModelicaModelConnectorFromComponentPort(dstPort);
                    }
                    else if (dstPort.ParentContainer is CyPhy.ComponentAssembly)
                    {
                        dstConnector = dstPort;
                    }
                    else if (dstPort.ParentContainer is CyPhy.Environment)
                    {
                        dstConnector = dstPort;
                    }

                    // The Fidelty Selected might not have this Connection - don't make it.
                    // Or it might be a floating connector with no purpose.
                    if (dstConnector == null)
                    {
                        continue;
                    }

                    var connection_mo = new Connection(srcConnector.Name, dstConnector.Name)
                    {
                        SrcInstanceName = testComponent.Name,
                        DstInstanceName = dstPort.ParentContainer.Name,
                        Type = srcConnector.Attributes.Class
                    };

                    this.testBench_mo.Connections.Add(connection_mo);
                }
            }

            // Connectors - aka portContainers
            foreach (var portContainer in testComponent.Children.ConnectorCollection)
            {
                if (portContainer.Children.ModelicaConnectorCollection.Count() != 1)
                {
                    continue;
                }

                foreach (var dstPortContainer in portContainer.DstConnections.ConnectorCompositionCollection.Select(c => c.DstEnds.Connector))
                {
                    if (dstPortContainer.Children.ModelicaConnectorCollection.Count() != 1)
                    {
                        continue;
                    }

                    CyPhy.ModelicaConnector srcConnector = this.GetModelicaModelConnectorFromComponentPortContainer(portContainer);
                    if (srcConnector == null)
                    {
                        continue;
                    }

                    CyPhy.ModelicaConnector dstConnector = null;

                    string dstConnectorName = "ERROR No Name Assigned";

                    if (dstPortContainer.ParentContainer is CyPhy.TestComponent)
                    {
                        dstConnector = this.GetModelicaModelConnectorFromComponentPortContainer(dstPortContainer);
                        dstConnectorName = dstConnector.Name;
                    }
                    else if (dstPortContainer.ParentContainer is CyPhy.ComponentAssembly)
                    {
                        dstConnector = dstPortContainer.Children.ModelicaConnectorCollection.FirstOrDefault();
                        dstConnectorName = dstPortContainer.Name.Replace(' ', '_');
                    }
                    else if (dstPortContainer.ParentContainer is CyPhy.Environment)
                    {
                        this.Logger.WriteDebug("Environment connected in Connector-chains not supported!");
                        continue;
                    }

                    // The Fidelty Selected might not have this Connection - don't make it.
                    // Or it might be a floating connector with no purpose.
                    if (dstConnector == null)
                    {
                        continue;
                    }

                    var connection_mo = new Connection(srcConnector.Name, dstConnectorName)
                    {
                        SrcInstanceName = testComponent.Name,
                        DstInstanceName = dstPortContainer.ParentContainer.Name,
                        Type = srcConnector.Attributes.Class
                    };

                    this.testBench_mo.Connections.Add(connection_mo);
                }
            }
        }

        /// <summary>
        /// Adds Environment to the test-bench.
        /// </summary>
        /// <param name="environment">CyPhy Environment</param>
        private void AddEnvironment(CyPhy.Environment environment)
        {
            // Make sure there are margins to the upper- and left-border
            int canvasX = environment.GenericAspect.X > ScaleFactor * ComponentIconSize ? 
                environment.GenericAspect.X : ScaleFactor * ComponentIconSize;
            int canvasY = environment.GenericAspect.Y > ScaleFactor * ComponentIconSize ? 
                environment.GenericAspect.Y : ScaleFactor * ComponentIconSize;

            // Make sure there are margins to the bottom- and right-border
            this.testBench_mo.CanvasXMax = canvasX + (ScaleFactor * ComponentIconSize) > this.testBench_mo.CanvasXMax ? 
                canvasX + (ScaleFactor * ComponentIconSize) : this.testBench_mo.CanvasXMax;
            this.testBench_mo.CanvasYMax = canvasY + (ScaleFactor * ComponentIconSize) > this.testBench_mo.CanvasYMax ? 
                canvasY + (ScaleFactor * ComponentIconSize) : this.testBench_mo.CanvasYMax;

            var environment_mo = new Environment(environment)
            {
                CanvasX = canvasX,
                CanvasY = canvasY,
                URI = environment.Attributes.Class
            };

            foreach (var parameter in environment.Children.ModelicaParameterCollection)
            {
                var value = this.GetModelicaParameterValue(parameter);
                if (string.IsNullOrWhiteSpace(value) == false)
                {
                    environment_mo.Parameters.Add(
                        new UnitParameter()
                        {
                            Name = parameter.Name,
                            Value = new ModelicaValue(value)
                        });
                }
            }

            foreach (var redeclareParameter in environment.Children.ModelicaRedeclareCollection)
            {
                var value = redeclareParameter.Attributes.Value;

                if (string.IsNullOrWhiteSpace(value) == false)
                {
                    environment_mo.Parameters.Add(
                        new UnitRedeclareParameter(redeclareParameter)
                        {
                            Value = new ModelicaValue(value)
                        });
                }
            }

            this.testBench_mo.Environments.Add(environment_mo);

            foreach (var connector in environment.Children.ModelicaConnectorCollection)
            {
                foreach (var dstPort in connector.DstConnections.PortCompositionCollection.Select(c => c.DstEnds.ModelicaConnector))
                {
                    CyPhy.ModelicaConnector srcConnector = connector;

                    CyPhy.ModelicaConnector dstConnector = null;

                    if (dstPort.ParentContainer is CyPhy.TestComponent)
                    {
                        dstConnector = this.GetModelicaModelConnectorFromComponentPort(dstPort);
                    }
                    else if (dstPort.ParentContainer is CyPhy.ComponentAssembly)
                    {
                        dstConnector = dstPort;
                    }
                    else if (dstPort.ParentContainer is CyPhy.Environment)
                    {
                        dstConnector = dstPort;
                    }

                    // The Fidelty Selected might not have this Connection - don't make it.
                    // Or it might be a floating connector with no purpose.
                    if (dstConnector == null)
                    {
                        continue;
                    }

                    var connection_mo = new Connection(srcConnector.Name, dstConnector.Name)
                    {
                        SrcInstanceName = environment.Name,
                        DstInstanceName = dstPort.ParentContainer.Name,
                        Type = srcConnector.Attributes.Class
                    };

                    this.testBench_mo.Connections.Add(connection_mo);
                }
            }
        }

        /// <summary>
        /// Builds up all components (through AddComponent), connectors (through AddConnector/AddModelicaConnector)
        /// and inner component assemblies (and calls this function recursively on such), for componentAssembly_mo.
        /// </summary>
        /// <param name="componentAssembly_mo">ComponentAssembly to start from.</param>
        /// <param name="parentComponentAssembly_mo">If the callee was a component assembly -> the callee.</param>
        private void BuildUpComponentAssembly(ComponentAssembly componentAssembly_mo, ComponentAssembly parentComponentAssembly_mo)
        {
            var ca = componentAssembly_mo.Impl;

            // ------- Parameters/Properties & Metrics ---------
            foreach (var defParameter in ca.Children.ParameterCollection)
            {
                // META-3622
                if (String.IsNullOrWhiteSpace(defParameter.Attributes.Dimension) || defParameter.Attributes.Dimension.Trim() == "1")
                {
                    var parameter_mo = new UnitParameter()
                    {
                        Name = defParameter.Name,
                        Value = this.GetParameterModelicaValue(defParameter),
                        Modifier = this.GetParameterModifier(defParameter)
                    };

                    componentAssembly_mo.Parameters.Add(parameter_mo);
                }
            }

            foreach (var defProperty in ca.Children.PropertyCollection)
            {
                // META-3622
                if (String.IsNullOrWhiteSpace(defProperty.Attributes.Dimension) || defProperty.Attributes.Dimension.Trim() == "1")
                {
                    var parameter_mo = new UnitParameter()
                    {
                        Name = defProperty.Name,
                        Value = this.GetPropertyModelicaValue(defProperty, true)
                    };

                    componentAssembly_mo.Parameters.Add(parameter_mo);
                }
            }

            foreach (var metric in ca.Children.MetricCollection)
            {
                var metric_mo = new Metric()
                {
                    Name = metric.Name,
                    Description = metric.Attributes.Description,
                };

                componentAssembly_mo.Metrics.Add(metric_mo);
            }

            // ------- ComponentAssemblies -------
            foreach (var innerComponentAssembly in ca.Children.ComponentAssemblyCollection)
            {
                if (this.ContainsDynamicComponents(innerComponentAssembly) == false)
                {
                    this.Logger.WriteWarning(
                        "Component Assembly '{0}' inside '{1}' does not contain any dynamic models, it will not be generated.", 
                        innerComponentAssembly.Name,
                        ca.Name);
                    continue;
                }

                var innerCAFullName = componentAssembly_mo.FullName.Substring(0, componentAssembly_mo.FullName.LastIndexOf('.'));

                bool innerCAHasChildren = innerComponentAssembly.Children.ComponentAssemblyCollection.Count() > 0;
                if (innerCAHasChildren)
                {
                    innerCAFullName += "." + innerComponentAssembly.Name;
                }

                innerCAFullName += "." + innerComponentAssembly.Name;

                var innerComponentAssembly_mo = new ComponentAssembly(innerComponentAssembly)
                {
                    FullName = innerCAFullName,
                    ParentComponentAssembly = componentAssembly_mo,
                    HasInnerCAs = innerCAHasChildren,
                    ConstrainedBys = this.modelicaSettings.GenerateConstrainedBys
                };

                this.testBench_mo.ComponentAssemblies.Add(innerComponentAssembly_mo);

                // Make sure there are margins to the upper- and left-border
                int canvasX = innerComponentAssembly.GenericAspect.X > ScaleFactor * 3 * ComponentIconSize ? innerComponentAssembly.GenericAspect.X : ScaleFactor * 3 * ComponentIconSize;
                int canvasY = innerComponentAssembly.GenericAspect.Y > ScaleFactor * 2 * ComponentIconSize ? innerComponentAssembly.GenericAspect.Y : ScaleFactor * 2 * ComponentIconSize;

                // Make sure there are margins to the bottom- and right-border
                componentAssembly_mo.CanvasXMax = canvasX + (ScaleFactor * 3 * ComponentIconSize) > componentAssembly_mo.CanvasXMax ? canvasX + (ScaleFactor * 3 * ComponentIconSize) : componentAssembly_mo.CanvasXMax;
                componentAssembly_mo.CanvasYMax = canvasY + (ScaleFactor * 2 * ComponentIconSize) > componentAssembly_mo.CanvasYMax ? canvasY + (ScaleFactor * 2 * ComponentIconSize) : componentAssembly_mo.CanvasYMax;

                var innerComponentAssemblyInstance_mo = new ComponentAssemblyInstance(innerComponentAssembly)
                {
                    InstanceOf = innerComponentAssembly_mo,
                    CanvasX = canvasX,
                    CanvasY = canvasY
                };

                foreach (var parameter in innerComponentAssembly.Children.ParameterCollection)
                {
                    var vf = parameter.SrcConnections.ValueFlowCollection.FirstOrDefault();
                    if (vf != null && vf.ParentContainer.ID == ca.ID)
                    {
                        var parameter_mo = this.GetArgumentParameter(ca.ID, parameter, vf);
                        innerComponentAssemblyInstance_mo.Parameters.Add(parameter_mo);
                    }
                }

                componentAssembly_mo.ComponentAssemblyInstances.Add(innerComponentAssemblyInstance_mo);

                string componentAssemblyInstanceURI = componentAssembly_mo.Name + "." + innerComponentAssemblyInstance_mo.Name;
                var parentCA = parentComponentAssembly_mo;
                while (parentCA != null)
                {
                    componentAssemblyInstanceURI = parentCA.Name + "." + componentAssemblyInstanceURI;
                    parentCA = parentCA.ParentComponentAssembly;
                }

                var componentAssemblyInfo = new ComponentInfo(innerComponentAssembly.Impl as GME.MGA.IMgaFCO, "PlantAssembly", this.traceability);

                this.instanceURIMap.Add(string.Format("{0}.TestBenches.{1}.{2}", MainPackage, this.testBench_mo.Name, componentAssemblyInstanceURI), componentAssemblyInfo);
                this.modelURIMap.Add(innerComponentAssembly_mo.FullName, componentAssemblyInfo);

                this.BuildUpComponentAssembly(innerComponentAssembly_mo, componentAssembly_mo);
            }

            // ------- Components -------
            foreach (var component in ca.Children.ComponentCollection)
            {
                if (this.ComponentIsDynamic(component) == false)
                {
                    this.Logger.WriteWarning(
                        "Component '{0}' inside '{1}' does not contain any dynamic models, it will not be generated.",
                        component.Name,
                        ca.Name);
                    continue;
                }

                if (this.treeComponents.ContainsKey(component.DerivedFrom().ID) == false)
                {
                    this.Logger.WriteDebug(
                        "Component-instance {0}, does not have a generated component. Probably since the component does not have a ModelicaModel.",
                        component.Name);
                    continue;
                }

                this.AddComponent(componentAssembly_mo, parentComponentAssembly_mo, component);
            }

            // ------- ModelicaConnectors -------
            foreach (var connector in ca.Children.ModelicaConnectorCollection)
            {
                this.AddModelicaConnector(componentAssembly_mo, parentComponentAssembly_mo, connector);
            }

            // ------- Connectors -------
            foreach (var connectorContainer in ca.Children.ConnectorCollection)
            {
                if (connectorContainer.Children.ModelicaConnectorCollection.Count() != 1)
                {
                    continue;
                }

                this.AddConnector(componentAssembly_mo, parentComponentAssembly_mo, connectorContainer);
            }
        }

        #region DynamicOrNotHelpers

        /// <summary>
        /// Puts a "1" in registry-node "IsDynamicModel" for Component and ComponentAssemblies based on if they are dynamic-models or not.
        /// For Components the following must hold for it to be a dynamic-model:
        /// <list type="number">
        /// <item> Component has a ModelicaModel.</item>
        /// <item> If the (fidelity selected modelicaModel) contains modelica-connectors with connections.</item>
        /// </list>
        /// For ComponentAssemblies the following must hold:
        /// <list type="number">
        /// <item> Any of its children, including children of contained ComponentAssemblies, contain "dynamic-components".</item>
        /// </list>
        /// </summary>
        private void TagComponentsAndComponentAssembliesWithIsDynamicModel()
        {
            var mainCA = this.testBench_mo.Impl.Children.ComponentAssemblyCollection.FirstOrDefault();
            var componentFilter = this.MainParameters.Project.CreateFilter();
            componentFilter.Kind = typeof(CyPhy.Component).Name;
            foreach (GME.MGA.IMgaFCO component in (mainCA.Impl as GME.MGA.MgaModel).GetDescendantFCOs(componentFilter))
            {
                if (this.ComponentHasDynamicConnections(component))
                {
                    component.RegistryValue["isDynamicModel"] = "1";
                }
                else
                {
                    component.RegistryValue["isDynamicModel"] = "0";
                }

                GME.MGA.IMgaFCO parent = null;
                var node = component;

                do
                {
                    parent = node.ParentModel;
                    var hasConn = this.CAHasDyanmicConnectors(parent);
                    bool value = hasConn && (this.GetDynamicModelFlag(node) || this.GetDynamicModelFlag(parent));
                    SetDynamicModelFlag(parent, value);
                    node = parent;
                } while (parent.ID != mainCA.ID);
            }

            // Set the root to true
            this.SetDynamicModelFlag(mainCA.Impl as GME.MGA.IMgaFCO, true);
            this.PropagateDynamicFlagRec(mainCA.Impl as GME.MGA.MgaModel);
            this.Logger.WriteDebug("<b>{0}</b>", mainCA.Name);
            this.PrintDynamicFlagStatusRec(mainCA.Impl as GME.MGA.MgaModel);
        }

        private void PrintDynamicFlagStatusRec(GME.MGA.MgaModel ca, int level = 1)
        {
            foreach (GME.MGA.MgaModel child in ca.GetChildrenOfKind(typeof(CyPhy.ComponentAssembly).Name))
            {
                if (this.GetDynamicModelFlag(child))
                {
                    this.Logger.WriteDebug("<b>{0} {1}</b>", new String('-', level), child.Name);
                }
                else
                {
                    this.Logger.WriteDebug("{0} {1}", new String('-', level), child.Name);
                }

                this.PrintDynamicFlagStatusRec(child, level + 1);
            }

            foreach (GME.MGA.MgaModel child in ca.GetChildrenOfKind(typeof(CyPhy.Component).Name))
            {
                if (this.GetDynamicModelFlag(child))
                {
                    this.Logger.WriteDebug("<b>{0} {1}</b>", new String('-', level), child.Name);
                }
                else
                {
                    this.Logger.WriteDebug("{0} {1}", new String('-', level), child.Name);
                }
            }
        }

        private void PropagateDynamicFlagRec(GME.MGA.MgaModel ca)
        {
            foreach (GME.MGA.MgaModel child in ca.GetChildrenOfKind(typeof(CyPhy.ComponentAssembly).Name))
            {
                if (this.GetDynamicModelFlag(child))
                {
                    this.PropagateDynamicFlagRec(child);
                }
                else
                {
                    var filter = this.MainParameters.Project.CreateFilter();
                    filter.Kind = typeof(CyPhy.Component).Name;
                    foreach (GME.MGA.IMgaFCO c in child.GetDescendantFCOs(filter))
                    {
                        this.SetDynamicModelFlag(c, false);
                    }

                    filter.Kind = typeof(CyPhy.ComponentAssembly).Name;
                    foreach (GME.MGA.IMgaFCO c in child.GetDescendantFCOs(filter))
                    {
                        this.SetDynamicModelFlag(c, false);
                    }
                }
            }
        }

        private bool GetDynamicModelFlag(GME.MGA.IMgaFCO context)
        {
            bool isDynamic = false;
            if (string.IsNullOrWhiteSpace(context.RegistryValue["isDynamicModel"]))
            {
                context.RegistryValue["isDynamicModel"] = "0";
            }

            isDynamic = context.RegistryValue["isDynamicModel"] == "1";

            return isDynamic;
        }

        private void SetDynamicModelFlag(GME.MGA.IMgaFCO context, bool value)
        {
            context.RegistryValue["isDynamicModel"] = value ? "1" : "0";
        }

        private bool ComponentHasDynamicConnections(GME.MGA.IMgaFCO mgaComponent)
        {
            var component = CyPhyClasses.Component.Cast(mgaComponent);
            var modelicaModel = this.GetModelicaModelFromComponent(component);

            if (modelicaModel == null)
            {
                return false;
            }

            foreach (var mc in modelicaModel.Children.ModelicaConnectorCollection)
            {
                if (mc.DstConnections.PortCompositionCollection.Any() || mc.SrcConnections.PortCompositionCollection.Any())
                {
                    return true;
                }
            }

            return false;
        }

        private bool CAHasDyanmicConnectors(GME.MGA.IMgaFCO mgaCA)
        {
            bool hasDynamicConnectors = false;
            if (string.IsNullOrWhiteSpace(mgaCA.RegistryValue["hasConn"]))
            {
                var ca = CyPhyClasses.ComponentAssembly.Cast(mgaCA);

                //hasDynamicConnectors = ca.Children.ConnectorCollection.Where(c => c.Children.ModelicaConnectorCollection.Any()).Any() ||
                //     ca.Children.ModelicaConnectorCollection.Any();
                //
                // For now treat all ComponentAssemblies as Dynamic one
                hasDynamicConnectors = true;
            }
            else
            {
                hasDynamicConnectors = mgaCA.RegistryValue["hasConn"] == "1";
            }

            return hasDynamicConnectors;
        }

        /// <summary>
        /// Determins if the parent container of parameter is Dynamic. 
        /// This is always true if the parent container is not a Component or ComponentAssembly.
        /// In those two cases it depends on what ComponentIsDynamic respectively ContainsDynamicComponents returns.
        /// </summary>
        /// <param name="parameter">Parameter whos container should be checked.</param>
        /// <returns>True if the container is dynamic and TagComponentsAndComponentAssembliesWithIsDynamicModel has been invoked.</returns>
        private bool ParentContainerIsDynamic(CyPhy.Parameter parameter)
        {
            var containerKind = parameter.ParentContainer.Kind;
            // Only use the named value if it is a dynamic component or ca. If not use the attribute-value.
            if (containerKind == typeof(CyPhy.Component).Name || containerKind == typeof(CyPhy.ComponentAssembly).Name)
            {
                return this.GetDynamicModelFlag(parameter.ParentContainer.Impl as GME.MGA.IMgaFCO);
            }
            else
            {
                return true;
            }
        }

        /// <summary>
        /// Returns true if given componentAssembly or any of its children contain "dynamic-components" 
        /// (i.e. ComponentIsDynamic returns true).
        /// </summary>
        /// <param name="componentAssembly">Component Assembly</param>
        /// <returns>True if GetDescendantFCOs includes "dynamic-components".</returns>
        private bool ContainsDynamicComponents(CyPhy.ComponentAssembly componentAssembly)
        {
            return this.GetDynamicModelFlag(componentAssembly.Impl as GME.MGA.IMgaFCO);
        }

        /// <summary>
        /// A Component is dynamic if:
        /// 1) Component has a ModelicaModel
        /// 2) If the (fidelity selected modelicaModel) contains modelica-connectors with connections.
        /// </summary>
        /// <param name="component">Component</param>
        /// <returns>True if rules apply and TagComponentsAndComponentAssembliesWithIsDynamicModel has been invoked.</returns>
        private bool ComponentIsDynamic(CyPhy.Component component)
        {
            return this.GetDynamicModelFlag(component.Impl as GME.MGA.IMgaFCO);
        }

        #endregion

        /// <summary>
        /// Adds the component to componentAssembly_mo as an instance.
        /// </summary>
        /// <param name="componentAssembly_mo">ComponentAssembly that contains the component.</param>
        /// <param name="parentComponentAssembly_mo">ComponentAssembly that contains componentAssembly_mo.</param>
        /// <param name="component">CyPhy Component to add.</param>
        private void AddComponent(ComponentAssembly componentAssembly_mo, ComponentAssembly parentComponentAssembly_mo, CyPhy.Component component)
        {
            var ca = componentAssembly_mo.Impl;
            var modelicaModel = this.GetModelicaModelFromComponent(component);

            // Make sure there are margins to the top- and left-border
            int canvasX = component.GenericAspect.X > ScaleFactor * ComponentIconSize ? 
                component.GenericAspect.X : ScaleFactor * ComponentIconSize;
            int canvasY = component.GenericAspect.Y > ScaleFactor * ComponentIconSize ? 
                component.GenericAspect.Y : ScaleFactor * ComponentIconSize;

            // Make sure there are margins to the bottom- and right-border
            componentAssembly_mo.CanvasXMax = canvasX + (ScaleFactor * ComponentIconSize) > componentAssembly_mo.CanvasXMax ? 
                canvasX + (ScaleFactor * ComponentIconSize) : componentAssembly_mo.CanvasXMax;
            componentAssembly_mo.CanvasYMax = canvasY + (ScaleFactor * ComponentIconSize) > componentAssembly_mo.CanvasYMax ? 
                canvasY + (ScaleFactor * ComponentIconSize) : componentAssembly_mo.CanvasYMax;

            var componentInstance_mo = new ComponentInstance(component)
            {
                InstanceOf = this.treeComponents[component.DerivedFrom().ID],
                CanvasX = canvasX,
                CanvasY = canvasY
            };

            foreach (var parameter in component.Children.ParameterCollection)
            {
                // META-3622
                if (String.IsNullOrWhiteSpace(parameter.Attributes.Dimension) || parameter.Attributes.Dimension.Trim() == "1")
                {
                    var vf = parameter.SrcConnections.ValueFlowCollection.FirstOrDefault();
                    if (vf != null && vf.ParentContainer.ID == ca.ID)
                    {
                        var parameter_mo = this.GetArgumentParameter(ca.ID, parameter, vf);
                        componentInstance_mo.Parameters.Add(parameter_mo);
                    }
                }
            }

            // META-2189 Overwrite ModelicaParameters connected to properties with formulated values in the design.
            foreach (var modelicaParameter in modelicaModel.Children.ModelicaParameterCollection)
            {
                var connection = modelicaParameter.SrcConnections.ModelicaParameterPortMapCollection.FirstOrDefault();
                if (connection != null && connection.SrcEnds.Property != null)
                {
                    double temp;
                    var value = modelicaParameter.Attributes.Value.Trim();
                    if (double.TryParse(modelicaParameter.Attributes.Value, out temp) || value == "true" || value == "false" || 
                        (value.StartsWith("\"") && value.EndsWith("\"")))
                    {
                        var parameter_mo = new UnitParameter();
                        parameter_mo.Name = modelicaParameter.Name;
                        parameter_mo.Value = new ModelicaValue(modelicaParameter.Attributes.Value.Trim());
                        componentInstance_mo.Parameters.Add(parameter_mo);
                    }
                }
            }

            componentAssembly_mo.ComponentInstances.Add(componentInstance_mo);

            string componentModelicaURI = componentAssembly_mo.Name + "." + componentInstance_mo.Name;
            var parentCA = parentComponentAssembly_mo;
            while (parentCA != null)
            {
                componentModelicaURI = parentCA.Name + "." + componentModelicaURI;
                parentCA = parentCA.ParentComponentAssembly;
            }

            this.ExtractPCCInputs(component, componentModelicaURI);


            string modelType;
            if (modelicaModel is CyPhy.CyberModel)
            {
                modelType = "ControllerModel";
            }
            else
            {
                modelType = "PlantModel";
            }

            var componentInfo = new ComponentInfo(component.Impl as GME.MGA.IMgaFCO, modelType, this.traceability);
            this.instanceURIMap.Add(string.Format("{0}.TestBenches.{1}.{2}", MainPackage, this.testBench_mo.Name, componentModelicaURI), componentInfo);
            if (this.modelURIMap.ContainsKey(componentInstance_mo.InstanceOf.FullName) == false)
            {
                this.modelURIMap.Add(componentInstance_mo.InstanceOf.FullName, componentInfo);
            }

            if (this.extendsURIMap.ContainsKey(componentInstance_mo.InstanceOf.Extends) == false)
            {
                var componentExtendInfo = new ComponentInfo(modelicaModel.Impl as GME.MGA.IMgaFCO, modelType, this.traceability);
                this.extendsURIMap.Add(componentInstance_mo.InstanceOf.Extends, componentExtendInfo);
            }

            // Iterate over all ports and look for connections FROM the port. These connections will generate equations in the 
            // component assembly if the connections is not entering a ModelicaModel.

            // Connections from ModelicaConnectors (Components)
            foreach (var port in component.Children.ModelicaConnectorCollection)
            {
                foreach (var dstPort in port.DstConnections.PortCompositionCollection
                    .Select(c => c.DstEnds.ModelicaConnector)
                    .Where(p => p.ParentContainer is CyPhy.ModelicaModel == false))
                {
                    CyPhy.ModelicaConnector srcConnector = this.GetModelicaModelConnectorFromComponentPort(port);
                    if (srcConnector == null)
                    {
                        continue;
                    }

                    CyPhy.ModelicaConnector dstConnector = null;

                    if (dstPort.ParentContainer is CyPhy.Component)
                    {
                        dstConnector = this.GetModelicaModelConnectorFromComponentPort(dstPort);
                    }
                    else if (dstPort.ParentContainer is CyPhy.ComponentAssembly)
                    {
                        dstConnector = dstPort;
                    }

                    // The Fidelty Selected might not have this Connection - don't make it.
                    // Or it might be a floating connector with no purpose.
                    if (dstConnector == null)
                    {
                        continue;
                    }

                    var connection_mo = new Connection(srcConnector.Name, dstConnector.Name)
                    {
                        SrcInstanceName = component.Name,
                        Type = srcConnector.Attributes.Class
                    };

                    if (dstPort.ParentContainer.ID != ca.ID)
                    {
                        connection_mo.DstInstanceName = dstPort.ParentContainer.Name;
                    }

                    componentAssembly_mo.Connections.Add(connection_mo);
                }
            }

            // Connections from Connectors (Components)
            foreach (var portContainer in component.Children.ConnectorCollection)
            {
                if (portContainer.Children.ModelicaConnectorCollection.Count() != 1)
                {
                    continue;
                }

                foreach (var dstPortContainer in portContainer.DstConnections.ConnectorCompositionCollection.Select(c => c.DstEnds.Connector))
                {
                    if (dstPortContainer.Children.ModelicaConnectorCollection.Count() != 1)
                    {
                        continue;
                    }

                    CyPhy.ModelicaConnector srcConnector = this.GetModelicaModelConnectorFromComponentPortContainer(portContainer);
                    if (srcConnector == null)
                    {
                        continue;
                    }

                    CyPhy.ModelicaConnector dstConnector = null;

                    string dstConnectorName = "ERROR No Name Assigned";
                    if (dstPortContainer.ParentContainer is CyPhy.Component)
                    {
                        var portComponent = CyPhyClasses.Component.Cast(dstPortContainer.ParentContainer.Impl);
                        if (this.ComponentIsDynamic(portComponent) == false)
                        {
                            this.Logger.WriteWarning("'{0}' does not contain any dynamic-components, connections to it skipped.", portComponent.Name);
                            continue;
                        }

                        dstConnector = this.GetModelicaModelConnectorFromComponentPortContainer(dstPortContainer);
                        if (dstConnector != null)
                        {
                            dstConnectorName = dstConnector.Name;
                        }
                    }
                    else if (dstPortContainer.ParentContainer is CyPhy.ComponentAssembly)
                    {
                        var portCA = dstPortContainer.ParentContainer.Impl;
                        if (this.ContainsDynamicComponents(CyPhyClasses.ComponentAssembly.Cast(portCA)) == false)
                        {
                            this.Logger.WriteWarning("'{0}' does not contain any dynamic-components, connections to it skipped.", portCA.Name);
                            continue;
                        }

                        dstConnector = dstPortContainer.Children.ModelicaConnectorCollection.FirstOrDefault();
                        dstConnectorName = dstPortContainer.Name.Replace(' ', '_');
                    }

                    // The Fidelty Selected might not have this Connection - don't make it.
                    // Or it might be a floating connector with no purpose.
                    if (dstConnector == null)
                    {
                        continue;
                    }

                    var connection_mo = new Connection(srcConnector.Name, dstConnectorName)
                    {
                        SrcInstanceName = component.Name,
                        Type = srcConnector.Attributes.Class
                    };

                    if (dstPortContainer.ParentContainer.ID != ca.ID)
                    {
                        connection_mo.DstInstanceName = dstPortContainer.ParentContainer.Name;
                    }

                    componentAssembly_mo.Connections.Add(connection_mo);
                }
            }

            // Add limit-checks for the component to the test-bench
            foreach (var modelicaModelLimit in modelicaModel.Children.LimitCheckCollection)
            {
                // TODO: When META-1124 and META-1129 both are merged, merge Limit and LimitCheck class.
                var limit = new Limit(modelicaModelLimit)
                {
                    ComponentModelicaUri = componentModelicaURI
                };
                this.testBench_mo.Limits.Add(limit);
                var limitCheck = new LimitCheck()
                {
                    ParentKind = modelicaModelLimit.ParentContainer.Kind,
                    LimitFullPath = modelicaModelLimit.Path,
                    LimitName = modelicaModelLimit.Name,
                    VariableName = modelicaModelLimit.Attributes.VariableName,
                    Type = modelicaModelLimit.Attributes.LimitType.ToString(),
                    Value = modelicaModelLimit.Attributes.Value,
                    VariableFullPath = string.Format("{0}.{1}", componentModelicaURI, modelicaModelLimit.Attributes.VariableName),
                    ModelicaRecordName = string.Format("Limit{0}", this.testBench_mo.Limits.Count)
                };

                this.LimitDefintion.LimitChecks.Add(limitCheck);

            }
        }

        /// <summary>
        /// Adds connector to componentAssembly_mo.
        /// </summary>
        /// <param name="componentAssembly_mo">ComponentAssembly that contains the component.</param>
        /// <param name="parentComponentAssembly_mo">ComponentAssembly that contains componentAssembly_mo.</param>
        /// <param name="connector">CyPhy ModelicaConnector to add.</param>
        private void AddModelicaConnector(ComponentAssembly componentAssembly_mo, ComponentAssembly parentComponentAssembly_mo, CyPhy.ModelicaConnector connector)
        {
            var ca = componentAssembly_mo.Impl;

            // Connectors do not need margins at the borders
            int canvasX = connector.GenericAspect.X;
            int canvasY = connector.GenericAspect.Y;

            componentAssembly_mo.CanvasXMax = canvasX > componentAssembly_mo.CanvasXMax ? canvasX : componentAssembly_mo.CanvasXMax;
            componentAssembly_mo.CanvasYMax = canvasY > componentAssembly_mo.CanvasYMax ? canvasY : componentAssembly_mo.CanvasYMax;

            var connector_mo = new Connector(connector)
            {
                ModelicaURI = connector.Attributes.Class,
                CanvasX = canvasX,
                CanvasY = canvasY
            };

            foreach (var parameter in connector.Children.ModelicaParameterCollection)
            {
                var value = this.GetModelicaParameterValue(parameter);
                if (string.IsNullOrWhiteSpace(value) == false)
                {
                    connector_mo.Parameters.Add(
                        new UnitParameter()
                        {
                            Name = parameter.Name,
                            Value = new ModelicaValue(value)
                        });
                }
            }

            foreach (var redeclareParameter in connector.Children.ModelicaRedeclareCollection)
            {
                var value = redeclareParameter.Attributes.Value;

                if (string.IsNullOrWhiteSpace(value) == false)
                {
                    connector_mo.Parameters.Add(
                        new UnitRedeclareParameter(redeclareParameter)
                        {
                            Value = new ModelicaValue(value)
                        });
                }
            }

            componentAssembly_mo.Connectors.Add(connector_mo);

            // Iterate over all ports that are connected FROM the connector. 
            foreach (var dstPort in connector.DstConnections.PortCompositionCollection.Select(c => c.DstEnds.ModelicaConnector))
            {
                var srcConnector = connector;

                if (dstPort.ParentContainer is CyPhy.Component)
                {
                    var portComponent = dstPort.ParentContainer as CyPhy.Component;
                    var dstConnector = this.GetModelicaModelConnectorFromComponentPort(dstPort);
                    if (dstConnector == null)
                    {
                        continue;
                    }

                    var connection_mo = new Connection(srcConnector.Name, dstConnector.Name)
                    {
                        DstInstanceName = dstPort.ParentContainer.Name,
                        Type = srcConnector.Attributes.Class
                    };

                    if (portComponent.ParentContainer.ID.Equals(ca.ID))
                    {
                        // Component is within current Component Assembly
                        componentAssembly_mo.Connections.Add(connection_mo);
                    }
                    else
                    {
                        // Component is within the parent Component Assembly
                        connection_mo.SrcInstanceName = componentAssembly_mo.Name;
                        parentComponentAssembly_mo.Connections.Add(connection_mo);
                    }
                }
                else if (dstPort.ParentContainer is CyPhy.ComponentAssembly)
                {
                    var portCA = dstPort.ParentContainer as CyPhy.ComponentAssembly;
                    var componentAssemblyIDList = componentAssembly_mo.ComponentAssemblyInstances.Select(c => c.Impl.ID);

                    if (componentAssemblyIDList.Contains(portCA.ID))
                    {
                        // Case 1: The connected ComponentAssembly is contained within the current one
                        var connection_mo = new Connection(srcConnector.Name, dstPort.Name)
                        {
                            DstInstanceName = dstPort.ParentContainer.Name,
                            Type = srcConnector.Attributes.Class
                        };

                        componentAssembly_mo.Connections.Add(connection_mo);
                    }
                    else if (portCA.ID.Equals(ca.ID))
                    {
                        // Case 2: The connected ComponentAssembly is itself (Connector to Connector - not pretty modelling)
                        var connection_mo = new Connection(srcConnector.Name, dstPort.Name)
                        {
                            Type = srcConnector.Attributes.Class
                        };

                        componentAssembly_mo.Connections.Add(connection_mo);
                    }
                    else if (parentComponentAssembly_mo != null)
                    {
                        // Case 3: The connected ComponentAssembly is the current one's parent
                        if (portCA.ID.Equals(parentComponentAssembly_mo.Impl.ID))
                        {
                            var connection_mo = new Connection(srcConnector.Name, dstPort.Name)
                            {
                                SrcInstanceName = componentAssembly_mo.Name,
                                Type = srcConnector.Attributes.Class
                            };

                            parentComponentAssembly_mo.Connections.Add(connection_mo);
                        }
                        else
                        {
                            // Case 4: (By exclusion) The connected ComponentAssembly is at the same level (i.e. within the parent ComponentAssembly)
                            var connection_mo = new Connection(srcConnector.Name, dstPort.Name)
                            {
                                SrcInstanceName = componentAssembly_mo.Name,
                                DstInstanceName = dstPort.ParentContainer.Name,
                                Type = srcConnector.Attributes.Class
                            };

                            parentComponentAssembly_mo.Connections.Add(connection_mo);
                        }
                    }
                }
                else if (dstPort.ParentContainer is CyPhy.TestComponent)
                {
                    var dstConnector = this.GetModelicaModelConnectorFromComponentPort(dstPort);
                    if (dstConnector == null)
                    {
                        continue;
                    }

                    var connection_mo = new Connection(srcConnector.Name, dstConnector.Name)
                    {
                        SrcInstanceName = componentAssembly_mo.Name,
                        DstInstanceName = dstPort.ParentContainer.Name,
                        Type = srcConnector.Attributes.Class
                    };

                    this.testBench_mo.Connections.Add(connection_mo);
                }
                else if (dstPort.ParentContainer is CyPhy.Environment)
                {
                    var connection_mo = new Connection(srcConnector.Name, dstPort.Name)
                    {
                        SrcInstanceName = componentAssembly_mo.Name,
                        DstInstanceName = dstPort.ParentContainer.Name,
                        Type = srcConnector.Attributes.Class
                    };

                    this.testBench_mo.Connections.Add(connection_mo);
                }
            }
        }

        /// <summary>
        /// Adds connectorContainer (Connector containing a ModelicaConnector) to componentAssembly_mo.
        /// </summary>
        /// <param name="componentAssembly_mo">ComponentAssembly that contains the component.</param>
        /// <param name="parentComponentAssembly_mo">ComponentAssembly that contains componentAssembly_mo.</param>
        /// <param name="connectorContainer">CyPhy Connector to add.</param>
        private void AddConnector(ComponentAssembly componentAssembly_mo, ComponentAssembly parentComponentAssembly_mo, CyPhy.Connector connectorContainer)
        {
            var ca = componentAssembly_mo.Impl;
            var containedConnector = connectorContainer.Children.ModelicaConnectorCollection.FirstOrDefault();

            // Connectors do not need margins at the borders
            int canvasX = connectorContainer.GenericAspect.X;
            int canvasY = connectorContainer.GenericAspect.Y;

            componentAssembly_mo.CanvasXMax = canvasX > componentAssembly_mo.CanvasXMax ? canvasX : componentAssembly_mo.CanvasXMax;
            componentAssembly_mo.CanvasYMax = canvasY > componentAssembly_mo.CanvasYMax ? canvasY : componentAssembly_mo.CanvasYMax;

            var connector_mo = new Connector(containedConnector)
            {
                ModelicaURI = containedConnector.Attributes.Class,
                CanvasX = canvasX,
                CanvasY = canvasY
            };

            connector_mo.Name = connectorContainer.Name.Replace(' ', '_');

            foreach (var parameter in containedConnector.Children.ModelicaParameterCollection)
            {
                var value = this.GetModelicaParameterValue(parameter);
                if (string.IsNullOrWhiteSpace(value) == false)
                {
                    connector_mo.Parameters.Add(
                        new UnitParameter()
                        {
                            Name = parameter.Name,
                            Value = new ModelicaValue(value)
                        });
                }
            }

            foreach (var redeclareParameter in containedConnector.Children.ModelicaRedeclareCollection)
            {
                var value = redeclareParameter.Attributes.Value;

                if (string.IsNullOrWhiteSpace(value) == false)
                {
                    connector_mo.Parameters.Add(
                        new UnitRedeclareParameter(redeclareParameter)
                        {
                            Value = new ModelicaValue(value)
                        });
                }
            }

            componentAssembly_mo.Connectors.Add(connector_mo);

            // Iterate over all ports that are connected FROM the connector. 
            foreach (var dstPortContainer in connectorContainer.DstConnections.ConnectorCompositionCollection.Select(c => c.DstEnds.Connector))
            {
                var srcConnectorContainer = connectorContainer;
                var srcContainedConnector = containedConnector;

                if (dstPortContainer.ParentContainer is CyPhy.Component)
                {
                    var portComponent = dstPortContainer.ParentContainer as CyPhy.Component;
                    if (this.ComponentIsDynamic(portComponent) == false)
                    {
                        this.Logger.WriteWarning("'{0}' does not contain any dynamic-components, connections to it skipped.", portComponent.Name);
                        continue;
                    }

                    var dstConnector = this.GetModelicaModelConnectorFromComponentPortContainer(dstPortContainer);

                    if (dstConnector == null)
                    {
                        continue;
                    }

                    var connection_mo = new Connection(srcConnectorContainer.Name, dstConnector.Name)
                    {
                        DstInstanceName = dstPortContainer.ParentContainer.Name,
                        Type = srcContainedConnector.Attributes.Class
                    };

                    if (portComponent.ParentContainer.ID.Equals(ca.ID))
                    {
                        // Component is within current Component Assembly
                        componentAssembly_mo.Connections.Add(connection_mo);
                    }
                    else
                    {
                        // Component is within the parent Component Assembly
                        connection_mo.SrcInstanceName = componentAssembly_mo.Name;
                        parentComponentAssembly_mo.Connections.Add(connection_mo);
                    }
                }
                else if (dstPortContainer.ParentContainer is CyPhy.ComponentAssembly)
                {
                    var portCA = dstPortContainer.ParentContainer as CyPhy.ComponentAssembly;
                    if (this.ContainsDynamicComponents(portCA) == false)
                    {
                        this.Logger.WriteWarning("'{0}' does not contain any dynamic-components, connections to it skipped.", portCA.Name);
                        continue;
                    }

                    var componentAssemblyIDList = componentAssembly_mo.ComponentAssemblyInstances.Select(c => c.Impl.ID);

                    if (componentAssemblyIDList.Contains(portCA.ID))
                    {
                        // Case 1: The connected ComponentAssembly is contained within the current one
                        var connection_mo = new Connection(srcConnectorContainer.Name, dstPortContainer.Name)
                        {
                            DstInstanceName = dstPortContainer.ParentContainer.Name,
                            Type = srcContainedConnector.Attributes.Class
                        };

                        componentAssembly_mo.Connections.Add(connection_mo);
                    }
                    else if (portCA.ID.Equals(ca.ID))
                    {
                        // Case 2: The connected ComponentAssembly is itself (Connector to Connector - not pretty modelling)
                        var connection_mo = new Connection(srcConnectorContainer.Name, dstPortContainer.Name)
                        {
                            Type = srcContainedConnector.Attributes.Class
                        };

                        componentAssembly_mo.Connections.Add(connection_mo);
                    }
                    else if (parentComponentAssembly_mo != null)
                    {
                        // Case 3: The connected ComponentAssembly is the current one's parent
                        if (portCA.ID.Equals(parentComponentAssembly_mo.Impl.ID))
                        {
                            var connection_mo = new Connection(srcConnectorContainer.Name, dstPortContainer.Name)
                            {
                                SrcInstanceName = componentAssembly_mo.Name,
                                Type = srcContainedConnector.Attributes.Class
                            };

                            parentComponentAssembly_mo.Connections.Add(connection_mo);
                        }
                        else
                        {
                            // Case 4: (By exclusion) The connected ComponentAssembly is at the same level (i.e. within the parent ComponentAssembly)
                            var connection_mo = new Connection(srcConnectorContainer.Name, dstPortContainer.Name)
                            {
                                SrcInstanceName = componentAssembly_mo.Name,
                                DstInstanceName = dstPortContainer.ParentContainer.Name,
                                Type = srcContainedConnector.Attributes.Class
                            };

                            parentComponentAssembly_mo.Connections.Add(connection_mo);
                        }
                    }
                }
                else if (dstPortContainer.ParentContainer is CyPhy.TestComponent)
                {
                    var dstConnector = this.GetModelicaModelConnectorFromComponentPortContainer(dstPortContainer);
                    if (dstConnector == null)
                    {
                        continue;
                    }

                    var connection_mo = new Connection(srcConnectorContainer.Name, dstConnector.Name)
                    {
                        SrcInstanceName = componentAssembly_mo.Name,
                        DstInstanceName = dstPortContainer.ParentContainer.Name,
                        Type = srcContainedConnector.Attributes.Class
                    };

                    this.testBench_mo.Connections.Add(connection_mo);
                }
                else if (dstPortContainer.ParentContainer is CyPhy.Environment)
                {
                    this.Logger.WriteDebug("Environment connected in Connector-chains not supported!");
                    continue;
                }
            }
        }

        #region HelperMethods

        /// <summary>
        /// If the CyPhyComponent has more than one modelica-model, tries to use the fidelity settings to get the selected modelica-model.
        /// If there is no fidelity settings defined the chosen modelica-model is added to the fidelity settings.
        /// </summary>
        /// <param name="component">CyPhyComponent from which the ModelicaModel should be extracted.</param>
        /// <returns>Selected ModelicaModel.</returns>
        private CyPhy.ModelicaModel GetModelicaModelFromComponent(CyPhy.Component component)
        {
            CyPhy.ModelicaModel modelicaModel = null;

            if ((component.Impl as GME.MGA.IMgaModel).GetChildrenOfKind(typeof(CyPhy.ModelicaModel).Name).Count +
                (component.Impl as GME.MGA.IMgaModel).GetChildrenOfKind(typeof(CyPhy.CyberModel).Name).Count == 1)
            {
                modelicaModel = component.Children.ModelicaModelCollection.FirstOrDefault();
            }
            else
            {
                var classification = component.Attributes.Classifications;

                Dictionary<string, string> selection = new Dictionary<string, string>();
                if (this.fidelitySettings.TryGetValue(classification, out selection))
                {
                    foreach (var kind in selection)
                    {
                        if (kind.Key == "ModelicaModel")
                        {
                            modelicaModel = component.Children.ModelicaModelCollection.FirstOrDefault(x => x.Name == kind.Value);
                            break;
                        }
                    }
                }

                if (modelicaModel == null)
                {
                    modelicaModel = component.Children.ModelicaModelCollection.FirstOrDefault();
                    if (modelicaModel == null)
                    {
                        this.Logger.WriteDebug("Modelica model was not found for component: {0}", component.Path);
                    }
                    else
                    {
                        this.Logger.WriteWarning("No fidelity selection for component: {0}, {1} was selected", component.Path, modelicaModel.Name);
                        this.Logger.WriteWarning("Please run the fidelity selector to avoid ambigious choices!");
                        if (this.fidelitySettings.ContainsKey(classification) == false)
                        {
                            this.fidelitySettings.Add(classification, new Dictionary<string, string>() { { "ModelicaModel", modelicaModel.Name } });
                            this.Logger.WriteDebug("Added {0} : ModelicaModel : {1}, to fidelitySettings (not written back to model).", classification, modelicaModel.Name);
                        }
                    }
                }
            }

            return modelicaModel;
        }

        /// <summary>
        /// From a ModelicaConnector inside a CyPhy(Test)Component it tries to find and return the connected ModelicaConnector inside
        /// the ModelicaModel of the component. Returns null if there is no ModelicaModel or such ModelicaConnector.
        /// </summary>
        /// <param name="port">ModelicaConnector inside a CyPhy(Test)Component.</param>
        /// <returns>ModelicaConnector inside ModelicaModel.</returns>
        private CyPhy.ModelicaConnector GetModelicaModelConnectorFromComponentPort(CyPhy.ModelicaConnector port)
        {
            CyPhy.ModelicaModel modelicaModel = null;

            var containingComponent = port.ParentContainer;
            if (containingComponent is CyPhy.TestComponent)
            {
                modelicaModel = (containingComponent as CyPhy.TestComponent).Children.ModelicaModelCollection.FirstOrDefault();
            }
            else if (containingComponent is CyPhy.Component)
            {
                modelicaModel = this.GetModelicaModelFromComponent(containingComponent as CyPhy.Component);
            }

            if (modelicaModel == null)
            {
                this.Logger.WriteDebug("No ModelicaModel found in component {0}, connection skipped...", containingComponent.Name);
                return null;
            }

            CyPhy.ModelicaConnector connector = null;
            connector = port.SrcConnections.PortCompositionCollection
                .Where(c => c.SrcEnd.ParentContainer.ID.Equals(modelicaModel.ID))
                .Select(c => c.SrcEnd).Cast<CyPhy.ModelicaConnector>()
                .FirstOrDefault();

            if (connector == null)
            {
                connector = port.DstConnections.PortCompositionCollection
                    .Where(c => c.DstEnd.ParentContainer.ID.Equals(modelicaModel.ID))
                    .Select(c => c.DstEnd).Cast<CyPhy.ModelicaConnector>()
                    .FirstOrDefault();
            }

            return connector;
        }

        /// <summary>
        /// From a Connector inside a CyPhy(Test)Component it tries to find and return the connected ModelicaConnector inside
        /// the ModelicaModel of the component. Returns null if there is no ModelicaModel or such ModelicaConnector.
        /// </summary>
        /// <param name="port">ModelicaConnector inside a CyPhy(Test)Component.</param>
        /// <returns>ModelicaConnector inside ModelicaModel.</returns>
        private CyPhy.ModelicaConnector GetModelicaModelConnectorFromComponentPortContainer(CyPhy.Connector port)
        {
            CyPhy.ModelicaModel modelicaModel = null;

            var containingComponent = port.ParentContainer;
            if (containingComponent is CyPhy.TestComponent)
            {
                modelicaModel = (containingComponent as CyPhy.TestComponent).Children.ModelicaModelCollection.FirstOrDefault();
            }
            else if (containingComponent is CyPhy.Component)
            {
                modelicaModel = this.GetModelicaModelFromComponent(containingComponent as CyPhy.Component);
            }

            if (modelicaModel == null)
            {
                this.Logger.WriteDebug("No ModelicaModel found in component {0}, connection skipped...", containingComponent.Name);
                return null;
            }

            CyPhy.ModelicaConnector connector = null;
            var containedPort = port.Children.ModelicaConnectorCollection.FirstOrDefault();
            if (containedPort == null) 
            {
                // META-3185
                this.Logger.WriteDebug("No ModelicaConnector found in connector {0} within {1}, connection skipped...", port.Name, containingComponent.Name);
                return null;
            }

            connector = containedPort.SrcConnections.PortCompositionCollection
                .Where(c => c.SrcEnd.ParentContainer.ID.Equals(modelicaModel.ID))
                .Select(c => c.SrcEnd).Cast<CyPhy.ModelicaConnector>()
                .FirstOrDefault();

            if (connector == null)
            {
                connector = containedPort.DstConnections.PortCompositionCollection
                    .Where(c => c.DstEnd.ParentContainer.ID.Equals(modelicaModel.ID))
                    .Select(c => c.DstEnd).Cast<CyPhy.ModelicaConnector>()
                    .FirstOrDefault();
            }

            return connector;
        }

        /// <summary>
        /// Goes through all Properties of component and looks for a registry node Distribution. If found at least three arguments divided by
        /// ',' is expected. The first argument is the type of input distribution, the rest (2 or 4) are the parameters for the input distribution.
        /// If everything is fulfilled the input is added to pccInputs. Which will be written to a json-file at the end of the code-generation.
        /// </summary>
        /// <param name="component">CyPhyComponent to extract PCCInputs from.</param>
        /// <param name="componentURI">The modelica URI for the component.</param>
        private void ExtractPCCInputs(CyPhy.Component component, string componentURI)
        {
            // TODO: Give feedback to user (move to checker?)
            foreach (var property in component.Children.PropertyCollection)
            {
                var dist = (property.Impl as GME.MGA.MgaFCO).RegistryNode["Distribution"].Value;
                if (dist != null)
                {
                    var inputDistribution = new PCCInputDistribution();
                    var distParameters = dist.Split(',');
                    string distType = distParameters.FirstOrDefault().Trim();
                    var pccInput = new PCCInputDistribution();
                    if (distType == "NORM" || distType == "UNIF" || distType == "LNORM" || distType == "BETA")
                    {
                        double param1;
                        double param2;
                        double param3 = 0;
                        double param4 = 0;
                        if (distParameters.Count() < 3 ||
                            double.TryParse(distParameters[1].Trim(), out param1) == false ||
                            double.TryParse(distParameters[2].Trim(), out param2) == false)
                        {
                            this.Logger.WriteWarning(
                                "Found Distribution registry node for property : {0}, in component : {1}, but in wrong format.",
                                property.Name,
                                component.Name);
                            continue;
                        }
                        else
                        {
                            if (distType == "BETA" &&
                                    (distParameters.Count() < 5 ||
                                     double.TryParse(distParameters[3].Trim(), out param3) == false ||
                                     double.TryParse(distParameters[4].Trim(), out param4) == false))
                            {
                                this.Logger.WriteWarning(
                                    "Found Distribution registry node for property : {0}, in component : {1}, but in wrong format.",
                                    property.Name,
                                    component.Name);
                                continue;
                            }

                            pccInput.Distribution = distType;
                            pccInput.Param1 = param1;
                            pccInput.Param2 = param2;
                            pccInput.Param3 = param3;
                            pccInput.Param4 = param4;
                        }
                    }
                    else
                    {
                        this.Logger.WriteWarning(
                            "Found Distribution registry node for property : {0}, in component : {1}, but in wrong format.",
                            property.Name,
                            component.Name);
                        continue;
                    }

                    bool badChars = false;
                    foreach (char c in property.Name)
                    {
                        if(Rules.Global.invalidCharacters.Contains(c))
                        {
                            badChars = true;
                            break;
                        }
                    }

                    if (badChars)
                    {
                        this.Logger.WriteWarning(
                            "Found Distribution registry node for property : {0}, in component : {1}, property name is invalid '{0}'.",
                            property.Name,
                            component.Name);
                        continue;
                    }

                    pccInput.Name = string.Format("{0}.{1}", componentURI, property.Name).Replace('.', '_');
                    pccInput.ID = property.ID;
                    foreach (var modelicaPortMap in property.DstConnections.ModelicaParameterPortMapCollection)
                    {
                        pccInput.TestBenchParameterNames.Add(string.Format("{0}.{1}", componentURI, modelicaPortMap.DstEnds.ModelicaParameter.Name));
                    }

                    this.pccInputs.Add(pccInput);
                }
            }
        }

        #region IOMethods

        /// <summary>
        /// Writes out all modelica-code to the output directory.
        /// </summary>
        private void WriteModelicaCode()
        {
            var extendModelicaIconPackage = this.WritePackageDotMoFiles();

            // Write out all components into one file
            var sb = new StringBuilder();
            sb.AppendLine(string.Format("within {0}.{1};", MainPackage, "Components"));
            this.GetComponentPackageFile(this.componentsPackage_mo, sb, extendModelicaIconPackage);
            var fileName = Path.Combine(this.OutputDir, MainPackage, "Components", this.componentsPackage_mo.Name + ".mo");
            File.WriteAllText(fileName, sb.ToString());

            // Write out all component assemblies into one file
            var componentAssemblyString = new StringBuilder();
            componentAssemblyString.AppendLine(string.Format("within {0}.ComponentAssemblies;", CodeGenerator.MainPackage));
            var topLevelSysCA_mo = this.testBench_mo.ComponentAssemblyInstances.FirstOrDefault().InstanceOf;
            this.GetComponentAssemblyPackageFile(topLevelSysCA_mo, componentAssemblyString, extendModelicaIconPackage, this.testBench_mo.Environments);
            File.WriteAllText(Path.Combine(this.OutputDir, MainPackage, "ComponentAssemblies", topLevelSysCA_mo.Name + ".mo"), componentAssemblyString.ToString());

            // Write out test components
            foreach (var testComponent_mo in this.testBench_mo.TestComponents)
            {
                fileName = Path.Combine(this.OutputDir, MainPackage, "TestComponents", testComponent_mo.Name + ".mo");
                File.WriteAllText(fileName, testComponent_mo.ToString());
            }

            // Write out test-bench
            File.WriteAllText(Path.Combine(this.OutputDir, MainPackage, "TestBenches", this.testBench_mo.Name + ".mo"), this.testBench_mo.ToString());

            // Write out verification test-bench
            var verificationMoText = this.GenerateVerificationTestBench();
            File.WriteAllText(Path.Combine(this.OutputDir, MainPackage, "TestBenches", "verif_" + this.testBench_mo.Name + ".mo"), verificationMoText.ToString());

            this.WriteModelicaMapJson();
            this.WritePCCInputParametersJson();
        }

        /// <summary>
        /// Writes out the directories and package.mo files in these. If MSL >= 3.2 the packages will extend the Modelica.Icons.Package.
        /// </summary>
        /// <returns>Bool indicating if the MSL is >= 3.2</returns>
        private bool WritePackageDotMoFiles()
        {
            var packages = new List<string>()
            {
               "ComponentAssemblies", "TestComponents", "TestBenches", "Components"
            };

            bool extendModelicaIconPackage;
            var usesAnnotation = GetUsesAnnotation(out extendModelicaIconPackage);
            var externalLibrayInfo = GetExternalLibarySvnInfo();
            var sb = new StringBuilder();
            sb.AppendLine("within ;");
            sb.AppendLine(string.Format("package {0}", MainPackage));
            if (extendModelicaIconPackage)
            {
                sb.Append("extends Modelica.Icons.Package;");
            }

            sb.AppendLine(string.Format("  annotation ({0}, {1});", usesAnnotation, externalLibrayInfo));
            sb.AppendLine(string.Format("end {0};", MainPackage));

            var dir = Path.Combine(this.OutputDir, MainPackage);
            Directory.CreateDirectory(dir);
            File.WriteAllText(Path.Combine(dir, "package.mo"), sb.ToString());

            foreach (var package in packages)
            {
                sb.Clear();
                sb.AppendLine(string.Format("within {0};", MainPackage));
                sb.AppendLine(string.Format("package {0}", package));
                if (extendModelicaIconPackage)
                {
                    sb.AppendLine("extends Modelica.Icons.Package;");
                }

                sb.AppendLine(string.Format("end {0};", package));

                dir = Path.Combine(this.OutputDir, MainPackage, package);
                Directory.CreateDirectory(dir);
                File.WriteAllText(Path.Combine(dir, "package.mo"), sb.ToString());
            }

            return extendModelicaIconPackage;
        }

        private string GetExternalLibarySvnInfo()
        {
            var sb = new StringBuilder();
            sb.AppendLine("Documentation(info = \"<HTML><p>");
            sb.AppendLine("<p><h3>External Libraries:</h3></p>");
            foreach (var library in (this.MainParameters.config as CyPhy2Modelica_v2Settings).IncludeDirectoryPath)
            {
                string libraryPath = library;
                if (libraryPath.EndsWith(".mo"))
                {
                    libraryPath = Path.GetDirectoryName(libraryPath);
                }
                if (Path.IsPathRooted(library) == false)
                {
                    libraryPath = Path.Combine(this.MainParameters.ProjectDirectory, libraryPath);
                }

                sb.AppendLine("<table>");
                sb.AppendLine("<caption>" + libraryPath.Replace("\\", "\\\\") + "</caption>");
                sb.AppendLine("<tbody>");
                this.Logger.WriteDebug("Trying to get svn info for {0}.", libraryPath);
                try
                {
                    var p = new System.Diagnostics.Process();
                    p.StartInfo = new System.Diagnostics.ProcessStartInfo()
                    {
                        Arguments = string.Format(string.Format("info \"{0}\"", libraryPath)),
                        CreateNoWindow = true,
                        FileName = "svn",
                        UseShellExecute = false,
                        RedirectStandardOutput = true,
                        RedirectStandardError = true,
                        WorkingDirectory = this.MainParameters.ProjectDirectory
                    };


                    p.Start();
                    p.WaitForExit();
                    if (p.ExitCode != 0)
                    {
                        this.Logger.WriteDebug(p.StandardError.ReadToEnd());
                    }
                    else
                    {
                        var line = p.StandardOutput.ReadLine();
                        while (line != null)
                        {
                            line = line.Replace("\\", "\\\\");
                            var linePieces = line.Split(new char[] { ':' }, 2);
                            if (linePieces.Count() == 2)
                            {
                                sb.Append("<tr><td>" + linePieces[0].Trim() + "</td>");
                                sb.AppendLine("<td>" + linePieces[1].Trim() + "</td></tr>");
                            }
                            line = p.StandardOutput.ReadLine();
                        }
                    }
                }
                catch (System.ComponentModel.Win32Exception)
                {
                    this.Logger.WriteDebug("svn command line tools not installed");
                }
                sb.AppendLine("</tbody>");
                sb.AppendLine("</table>");
            }

            sb.AppendLine("</HTML>\")");

            return sb.ToString();
        }

        private string GetUsesAnnotation(out bool extendModelicaIconPackage)
        {
            var usesAnnotationString = new StringBuilder("uses(");
            bool nonDefaultModelica = false;
            extendModelicaIconPackage = true;
            foreach (var path in (this.MainParameters.config as CyPhy2Modelica_v2.CyPhy2Modelica_v2Settings).IncludeDirectoryPath)
            {
                var library = CyPhy2Modelica_v2.ModelicaLibrary.GetLibraryFromPath(path);
                if (library.Name == "Modelica")
                {
                    nonDefaultModelica = true;
                    extendModelicaIconPackage = this.CheckIfModelicaVersionIsGreaterOrEqualTo32(library.Version);
                }

                usesAnnotationString.Append(" " + library.Name);
                if (library.Version != "default")
                {
                    usesAnnotationString.Append(string.Format("(version = \"{0}\")", library.Version));
                }

                usesAnnotationString.Append(",");
            }

            if (nonDefaultModelica)
            {
                usesAnnotationString.Replace(',', ')', usesAnnotationString.Length - 1, 1);
            }
            else
            {
                usesAnnotationString.Append("Modelica(version= \"3.2\"))");
            }

            return usesAnnotationString.ToString();
        }

        /// <summary>
        /// Recursively goes through the package-tree and appends components and packages to one string.
        /// </summary>
        /// <param name="package">Root package.</param>
        /// <param name="sb">Text will be appened to sb.</param>
        /// <param name="extendModelicaIconPackage">Bool indicating if the MSL is >= 3.2.</param>
        private void GetComponentPackageFile(Package package, StringBuilder sb, bool extendModelicaIconPackage)
        {
            sb.Append(' ', package.IndentLevel * 2);
            sb.AppendLine(string.Format("package {0}", package.Name));
            sb.Append(' ', package.IndentLevel * 2);
            if (extendModelicaIconPackage)
            {
                sb.AppendLine("extends Modelica.Icons.Package;");
            }

            foreach (var component in package.Models)
            {
                sb.Append(component.ToStringIndented(package.IndentLevel + 1));
            }

            foreach (var subPackage in package.Children)
            {
                this.GetComponentPackageFile(subPackage, sb, extendModelicaIconPackage);
            }

            sb.Append(' ', package.IndentLevel * 2);
            sb.AppendLine(string.Format("end {0};", package.Name));
        }

        /// <summary>
        /// Recursively goes through the component assembly "tree" and appends component assemblies and packages to one string.
        /// </summary>
        /// <param name="ca_mo">Root component-assembly.</param>
        /// <param name="sb">Text will be appended to sb.</param>
        /// <param name="extendModelicaIconPackage">Bool indicating if the MSL is >= 3.2.</param>
        /// <param name="environments">These will be generated in all component-assemblies.</param>
        private void GetComponentAssemblyPackageFile(Modelica.ComponentAssembly ca_mo, StringBuilder sb, bool extendModelicaIconPackage, SortedSet<Modelica.Environment> environments)
        {
            ca_mo.Environments = environments;
            if (ca_mo.HasInnerCAs)
            {
                sb.AppendLine(string.Format("package {0}", ca_mo.Name));
                if (extendModelicaIconPackage)
                {
                    sb.AppendLine("extends Modelica.Icons.Package;");
                }
            }

            sb.Append(ca_mo.ToString());

            foreach (var inner_ca_mo in ca_mo.ComponentAssemblyInstances.Select(c => c.InstanceOf))
            {
                this.GetComponentAssemblyPackageFile(inner_ca_mo, sb, extendModelicaIconPackage, environments);
            }

            if (ca_mo.HasInnerCAs)
            {
                sb.AppendLine(string.Format("end {0};", ca_mo.Name));
            }
        }

        /// <summary>
        /// Writes the ModelicaURI mappings to CyPhyObjects into "modelicaURI2CyPhyMap.json".
        /// </summary>
        private void WriteModelicaMapJson()
        {
            string filename = Path.Combine(this.OutputDir, MainPackage, "modelicaURI2CyPhyMap.json");
            this.ModelicaURIMap.TestBench = string.Format("{0}.TestBenches.{1}", MainPackage, this.testBench_mo.Name);
            this.ModelicaURIMap.VerifTestBench = string.Format("{0}.TestBenches.verif_{1}", MainPackage, this.testBench_mo.Name);

            using (StreamWriter writer = new StreamWriter(filename))
            {
                var jsonString = Newtonsoft.Json.JsonConvert.SerializeObject(
                    this.ModelicaURIMap,
                    Newtonsoft.Json.Formatting.Indented);

                writer.WriteLine(jsonString);
            }
        }

        /// <summary>
        /// If there are any PCCInputs defined on the CyPhyComponent Properties, the gathered ones in pccInputs are written out to "PCCProperties.json".
        /// </summary>
        private void WritePCCInputParametersJson()
        {
            if (this.pccInputs.Count == 0)
            {
                return;
            }

            string filename = Path.Combine(this.OutputDir, MainPackage, "PCCProperties.json");
            using (StreamWriter writer = new StreamWriter(filename))
            {
                var jsonString = Newtonsoft.Json.JsonConvert.SerializeObject(
                    this.pccInputs,
                    Newtonsoft.Json.Formatting.Indented);

                writer.WriteLine(jsonString);
            }
        }

        #endregion

        #region RuleCheckingOnTreeComponents

        /// <summary>
        /// Checks if childName is a reserved keyword or package in modelica.
        /// </summary>
        /// <param name="childName">Name to check.</param>
        /// <returns>invalidName, (null if not invalid).</returns>
        private string CheckForReservedName(string childName)
        {
            string invalidName = null;

            if (this.reservedPackageNames.Contains(childName))
            {
                invalidName = childName;
            }
            else if (Rules.Global.ReservedModelicaKeyWords.Contains(childName))
            {
                invalidName = childName;
            }

            return invalidName;
        }

        /// <summary>
        /// Uses rules from the checker to see if a component in the tree fulfills all rules. If not a warning message is given and the component is not generated.
        /// </summary>
        /// <param name="component">To be checked.</param>
        /// <param name="modelicaModel">The ModelicaModel of the component.</param>
        /// <returns>Indicating if all rules were passed.</returns>
        private bool CheckRulesOnComponent(CyPhy.Component component, CyPhy.ModelicaModel modelicaModel)
        {
            bool success = true;

            // Check the name of the Component
            var invalidChars = Rules.Global.checkForInvalidCharacters(component.Name);

            if (string.IsNullOrEmpty(invalidChars) == false)
            {
                this.Logger.WriteDebug(
                    "The definition of component '{0}' contains invalid characters '{1}'.",
                    component.Name,
                    invalidChars);
                success = false;
            }
            else if (string.IsNullOrEmpty(component.Name))
            {
                this.Logger.WriteDebug("The definition of component '{0}' has an empty name.", component.Name);
                success = false;
            }
            else if (char.IsDigit(component.Name.First()))
            {
                this.Logger.WriteDebug("The definition of component '{0}' name starts with a digit.", component.Name);
                success = false;
            }
            else if (string.IsNullOrEmpty(this.CheckForReservedName(component.Name)) == false)
            {
                this.Logger.WriteDebug("The definition of component '{0}' has a reserved name.", component.Name);
                success = false;
            }

            // Rule-checks on the Component
            success = success && this.WriteWarningMessagesForFeedBack(Rules.Global.ComponentPropertiesHaveValues(component.Impl as GME.MGA.MgaFCO));
            success = success && this.WriteWarningMessagesForFeedBack(Rules.Global.NamesMustBeValid(component.Impl as GME.MGA.MgaFCO));
            success = success && this.WriteWarningMessagesForFeedBack(Rules.Global.ConnectorWithinComponent(component.Impl as GME.MGA.MgaFCO));
            success = success && this.WriteWarningMessagesForFeedBack(Rules.Global.ComponentParametersHaveValues(component.Impl as GME.MGA.MgaFCO));
            success = success && this.WriteWarningMessagesForFeedBack(Rules.Global.ModelicaModelHasValidURI(component.Impl as GME.MGA.MgaFCO));

            // Rule-checks on the ModelicaModel
            success = success && this.WriteWarningMessagesForFeedBack(Rules.Global.ModelicaParametersHaveValues(modelicaModel.Impl as GME.MGA.MgaFCO));
            success = success && this.WriteWarningMessagesForFeedBack(Rules.Global.NamesMustBeValid(modelicaModel.Impl as GME.MGA.MgaFCO));

            // Rule-checks on Properties
            foreach (var prop in component.Children.PropertyCollection)
            {
                success = success && this.WriteWarningMessagesForFeedBack(Rules.Global.PropertiesMustNotGetValuesFromParameters(prop.Impl as GME.MGA.MgaFCO));
            }

            // Rule-checks on Parameters
            foreach (var param in component.Children.ParameterCollection)
            {
                success = success && this.WriteWarningMessagesForFeedBack(Rules.Global.ParametersHaveValidIncomingFlowsAndRanges(param.Impl as GME.MGA.MgaFCO));
            }

            return success;
        }

        /// <summary>
        /// Check if version of Modelica Standard Library is greater or equal to 3.2.
        /// </summary>
        /// <param name="mslVersion">Modelica version, e.g. 3.2.1.</param>
        /// <returns>True if version was >= 3.2</returns>
        /// <example>
        /// <code>
        /// <pre>
        /// var library = CyPhy2Modelica_v2.ModelicaLibrary.GetLibraryFromPath(path);
        /// bool nonDefaultModelica = false;
        /// bool extendModelicaIconPackage = true;\n
        /// if (library.Name == "Modelica")\n
        /// {
        ///     nonDefaultModelica = true;\n
        ///     extendModelicaIconPackage = CheckIfModelicaVersionIsGreaterOrEqualTo32(library.Version);\n
        /// }
        /// </pre>
        /// </code>
        /// </example>
        private bool CheckIfModelicaVersionIsGreaterOrEqualTo32(string mslVersion)
        {
            // This fails more than neccessary, however the only loss is that the packages will not be gray.
            double version;
            var result = false;
            if (double.TryParse(mslVersion, out version))
            {
                result = version >= 3.2;
            }
            else if (mslVersion.Contains('.'))
            {
                var versionPieces = mslVersion.Split('.');
                if (versionPieces.Length == 3)
                {
                    int v;
                    if (int.TryParse(versionPieces[0], out v))
                    {
                        if (v >= 3)
                        {
                            if (int.TryParse(versionPieces[1], out v))
                            {
                                if (v >= 2)
                                {
                                    result = true;
                                }
                            }
                        }
                    }
                }
            }

            return result;
        }

        /// <summary>
        /// Goes through the checkerFeedBack from the rules and converts errors to warning-messages.
        /// </summary>
        /// <param name="checkerFeedBack">From the checked rule.</param>
        /// <returns>Indicating if checkerFeedBack contained errors.</returns>
        private bool WriteWarningMessagesForFeedBack(IEnumerable<DesignConsistencyChecker.DesignRule.RuleFeedbackBase> checkerFeedBack)
        {
            bool success = true;
            if (checkerFeedBack.Where(f => f.FeedbackType == DesignConsistencyChecker.DesignRule.FeedbackTypes.Error).Count() > 0)
            {
                foreach (var ruleViolation in checkerFeedBack)
                {
                    this.Logger.WriteDebug(ruleViolation.Message);
                }

                success = false;
            }

            return success;
        }

        #endregion

        #region FormulaEvaluator

        /// <summary>
        /// Calls formula evaluator on given CyPhyComponent.
        /// </summary>
        /// <param name="componentInTree">CyPhyComponent to be formulated.</param>
        /// <returns>Indicating a successful call to the interpreter.</returns>
        private bool CallFormulaEvaluatorOnComponent(CyPhy.Component componentInTree)
        {
            var result = false;
            try
            {
                this.Logger.WriteDebug("Trying to call CyPhyFormulaEvaluator on : {0}  at : {1}", componentInTree.Name, componentInTree);
                Type typeFormulaEval = Type.GetTypeFromProgID("MGA.Interpreter.CyPhyFormulaEvaluator");
                var formulaEval = Activator.CreateInstance(typeFormulaEval) as GME.MGA.IMgaComponentEx;

                formulaEval.Initialize(this.MainParameters.Project);
                formulaEval.InvokeEx(this.MainParameters.Project, componentInTree.Impl as GME.MGA.MgaFCO, this.MainParameters.SelectedFCOs, 128);
                result = true;
            }
            catch (Exception ex)
            {
                this.Logger.WriteDebug("Formula Evaluator failed on : {0}  at : {1}", componentInTree.Name, componentInTree);
                this.Logger.WriteDebug(ex.ToString());
            }

            return result;
        }

        #endregion

        #region Verification

        /// <summary>
        /// Generates text for the verification test-bench.
        /// </summary>
        /// <returns>StringBuilder containing the text.</returns>
        private StringBuilder GenerateVerificationTestBench()
        {
            var sb = new StringBuilder();

            sb.AppendLine(string.Format("within {0}.{1};", CodeGenerator.MainPackage, "TestBenches"));
            sb.AppendLine(string.Format("model verif_{0}", this.testBench_mo.Name));
            sb.AppendLine(string.Format("  extends {0}.{1}.{2};", CodeGenerator.MainPackage, "TestBenches", this.testBench_mo.Name));
            sb.AppendLine();
            sb.AppendLine("    // Requirement Definition");
            sb.AppendLine("    type Requirement = enumeration(");
            sb.AppendLine("      success,");
            sb.AppendLine("      unknown,");
            sb.AppendLine("      violated);");
            int cnt = 1;
            sb.AppendLine();
            sb.AppendLine("    // Limit-Checks definitions");
            foreach (var limit in this.testBench_mo.Limits)
            {
                string prettyLimitType;
                if (limit.LimitType == CyPhyClasses.LimitCheck.AttributesClass.LimitType_enum.max)
                {
                    prettyLimitType = "Max value limit-check";
                }
                else if (limit.LimitType == CyPhyClasses.LimitCheck.AttributesClass.LimitType_enum.min)
                {
                    prettyLimitType = "Min value limit-check";
                }
                else
                {
                    prettyLimitType = "Max absolute value limit-check";
                }

                sb.AppendLine(string.Format(
                                "    Requirement Limit{0}(start = Requirement.unknown, fixed=true) \" {1} on {2}\";",
                                cnt,
                                prettyLimitType,
                                limit.VariablePath));
                cnt++;
            }

            sb.AppendLine();
            sb.AppendLine("    // Requirements-Checks");
            this.PopulateRequirementMetrics();
            cnt = 1;
            foreach (var metric in this.requirementMetrics)
            {
                string prettyMetricType;
                if (metric.BiggerIsBetter)
                {
                    prettyMetricType = "Min value requirement-metric check";
                }
                else
                {
                    prettyMetricType = "Max value requirement-metric check";
                }

                sb.AppendLine(string.Format(
                                    "    Requirement Requirement{0}(start = Requirement.unknown, fixed=true) \" {1} on {0}\";",
                                    metric.MetricName,
                                    prettyMetricType));

                cnt++;
            }

            sb.AppendLine("equation");
            cnt = 1;
            sb.AppendLine("    // Limit-Checks equations");
            foreach (var limit in this.testBench_mo.Limits)
            {
                sb.Append(string.Format("    if pre(Limit{0}) == Requirement.violated or ", cnt));
                if (limit.LimitType == CyPhyClasses.LimitCheck.AttributesClass.LimitType_enum.max)
                {
                    sb.AppendLine(string.Format("{0} > {1} then", limit.VariablePath, limit.Value));
                }
                else if (limit.LimitType == CyPhyClasses.LimitCheck.AttributesClass.LimitType_enum.min)
                {
                    sb.AppendLine(string.Format("{0} < {1} then", limit.VariablePath, limit.Value));
                }
                else
                {
                    sb.AppendLine(string.Format("{0} > {1} or {0} < -{1} then", limit.VariablePath, limit.Value));
                }

                sb.AppendLine(string.Format("      Limit{0} = Requirement.violated;", cnt));
                sb.AppendLine("    else");
                sb.AppendLine(string.Format("      Limit{0} = Requirement.unknown;", cnt));
                sb.AppendLine("    end if;");
                sb.AppendLine();
                cnt++;
            }

            cnt = 1;
            sb.AppendLine();
            sb.AppendLine("    // Requirement-Metrics Checks equations");
            foreach (var metric in this.requirementMetrics)
            {
                //// TODO: How to use the Requirement Type {unknow, success, violated}
                ////if (metric.BiggerIsBetter)
                ////{
                sb.Append(string.Format("    if pre(Requirement{0}) == Requirement.success or ", metric.MetricName));
                sb.AppendLine(string.Format("{0} then", metric.SuccessCriterion));
                sb.AppendLine(string.Format("      Requirement{0} = Requirement.success;", metric.MetricName));
                sb.AppendLine("    else");
                sb.AppendLine(string.Format("      Requirement{0} = Requirement.unknown;", metric.MetricName));
                sb.AppendLine("    end if;");
                sb.AppendLine();
                ////}
                ////else
                ////{
                ////    sb.Append(string.Format("    if RequirementMetric{0} == Requirement.violated or ", cnt));
                ////    sb.AppendLine(string.Format("{0} then", metric.MetricExpression));
                ////    sb.AppendLine(string.Format("      RequirementMetric{0} = Requirement.violated;", cnt));
                ////    sb.AppendLine("    else");
                ////    sb.AppendLine(string.Format("      RequirementMetric{0} = Requirement.success;", cnt));
                ////    sb.AppendLine("    end if;");
                ////    sb.AppendLine("");
                ////}

                cnt++;
            }

            sb.AppendLine(string.Format("end {0};", "verif_" + this.testBench_mo.Name));

            return sb;
        }

        /// <summary>
        /// Tries to find requirements for metrics in requirements/requirements.json and populates requirementMetrics if it found matching metrics.
        /// </summary>
        private void PopulateRequirementMetrics()
        {
            var requirementFile = new FileInfo(Path.Combine(this.MainParameters.ProjectDirectory, "requirements", "requirements.json"));
            if (requirementFile.Exists == false)
            {
                return;
            }

            var testBenchName = string.IsNullOrEmpty((this.testBench_mo.Impl.Impl as GME.MGA.MgaFCO).RegistryValue["TestBenchUniqueName"]) ?
                this.testBench_mo.Name : (this.testBench_mo.Impl.Impl as GME.MGA.MgaFCO).RegistryValue["TestBenchUniqueName"];

            try
            {
                var jsonString = File.ReadAllText(requirementFile.FullName);
                var jsonObject = JsonConvert.DeserializeObject(jsonString) as JObject;

                if (jsonObject.HasValues)
                {
                    this.ReadRequirementMetricsRecursivley(jsonObject, testBenchName);
                }
            }
            catch
            {
                this.Logger.WriteDebug("requirements.json is of invalid format! See file at {0}", requirementFile.FullName);
            }
        }

        /// <summary>
        /// Goes throught the requirements json data-structure recursively.
        /// </summary>
        /// <param name="jsonObject">Root for json-data structure.</param>
        /// <param name="testBenchName">Name of this test-bench.</param>
        private void ReadRequirementMetricsRecursivley(JObject jsonObject, string testBenchName)
        {
            if (jsonObject["testBench"] != null)
            {
                if (jsonObject["testBench"].ToString() == testBenchName)
                {
                    if (jsonObject["metricName"] == null)
                    {
                        this.Logger.WriteDebug("Found requirements metric for this test-bench in " +
                            "requirements.json in /requirements. But it did not have a metricName defined! It will be skipped.");
                    }
                    else
                    {
                        if (this.TryToAddRequirementMetric(jsonObject["metricName"].ToString(), jsonObject) == false)
                        {
                            this.Logger.WriteDebug("The metric will not be added as part of the verification test-bench.");
                        }
                    }
                }
            }

            if (jsonObject["children"] != null)
            {
                foreach (var child in jsonObject["children"])
                {
                    this.ReadRequirementMetricsRecursivley(child as JObject, testBenchName);
                }
            }
        }

        /// <summary>
        /// Checks if the metricObject has requirements data - if so it adds it to requirementMetrics (for the verification testbench).
        /// </summary>
        /// <param name="metricName">Name of metric.</param>
        /// <param name="metricObject">Metrics data from requirements.json.</param>
        /// <returns>True if correct requirements could be found for the metric.</returns>
        private bool TryToAddRequirementMetric(string metricName, JObject metricObject)
        {
            double objective = 0;
            double threshold = 0;
            string metricExpression = string.Empty;

            // Examples of metricExpression, i.e. metricObject["successCriterion"]: 
            //          "road_Wheel_Load_Both_Sides.vehicleSpeed > 20"
            //          "road_Wheel_Load_Both_Sides.Accel_10kph < 4.6 and road_Wheel_Load_Both_Sides.Accel_10kph > 0"
            if (metricObject["successCriterion"] == null)
            {
                this.Logger.WriteDebug(
                    "Metric {0} in the requirements.json in /requirements did not have the key \"successCriterion\". " +
                    "It must be a boolean expression in Modelica syntax.",
                    metricName);
                return false;
            }
            else
            {
                metricExpression = metricObject["successCriterion"].ToString();
                if (string.IsNullOrWhiteSpace(metricExpression))
                {
                    this.Logger.WriteDebug(
                        "Metric {0} has an empty \"successCriterion\". It must be a boolean expression in Modelica syntax.",
                        metricName);
                    return false;
                }
            }

            if (metricObject["objective"] != null)
            {
                bool success = double.TryParse(metricObject["objective"].ToString(), out objective);
                if (success == false)
                {
                    this.Logger.WriteDebug(
                        "The objective for requirement-metric {0} must be a double. " +
                        "The current given value, {1}, is not. Please edit the requirements.json in /requirements.",
                        metricName,
                        metricObject["objective"]);

                    return false;
                }
            }
            else
            {
                this.Logger.WriteDebug("No objective defined for requirement-metric {0}. Please edit the requirements.json in /requirements.", metricName);
                return false;
            }

            if (metricObject["threshold"] != null)
            {
                bool success = double.TryParse(metricObject["threshold"].ToString(), out threshold);
                if (success == false)
                {
                    this.Logger.WriteDebug(
                        "The threshold for requirement-metric {0} must be a double. " +
                        "The current given value, {1}, is not. Please edit the requirements.json in /requirements.",
                        metricName,
                        metricObject["threshold"]);
                    return false;
                }
            }
            else
            {
                this.Logger.WriteDebug(
                    "No objective defined for requirement-metric {0}. " +
                    "Please edit the requirements.json in /requirements.",
                    metricName);
                return false;
            }

            if (threshold == objective)
            {
                this.Logger.WriteDebug(
                    "The threshold and objective for requirement-metric {0} have the same value, {1}. " +
                    "They must be different doubles. Please edit the requirements.json in /requirements.",
                    metricName,
                    metricObject["threshold"]);
                return false;
            }

            this.requirementMetrics.Add(new RequirementMetric(metricName, metricExpression, threshold, objective));
            this.Logger.WriteDebug("Added {0} to verification test-bench, success criterion : \"{1}\".", metricName, metricExpression);

            return true;
        }

        #endregion

        #endregion
    }
}
