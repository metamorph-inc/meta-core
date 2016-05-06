using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DesignConsistencyChecker;
using DesignConsistencyChecker.DesignRule.Attributes;
using DesignConsistencyChecker.DesignRule;
using GME.MGA;
using GME.CSharp;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using System.IO;
using CyPhyCOMInterfaces;
using CodeGenMethods = CyPhy2Modelica_v2.Modelica.CodeGenerator;

namespace CyPhy2Modelica_v2.Rules
{
    public class Global : RuleBase
    {
        [CheckerRule("ModelicaParametersHaveValues", Description = "ModelicaParameters within ModelicaModels should have a Value")]
        [Tags("Modelica")]
        [ValidContext("ModelicaModel", "ModelicaConnector")]
        public static IEnumerable<RuleFeedbackBase> ModelicaParametersHaveValues(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                // Skip ModelicaConnectors within Connectors
                if (context.ParentModel is CyPhy.Connector)
                {
                    return result;
                }

                IEnumerable<CyPhy.ModelicaParameter> childParameters = null;

                if (context.Meta.Name == typeof(CyPhy.ModelicaModel).Name)
                {
                    var modelicaModel = CyPhyClasses.ModelicaModel.Cast(context);
                    childParameters = modelicaModel.Children.ModelicaParameterCollection;
                }
                else if (context.Meta.Name == typeof(CyPhy.ModelicaConnector).Name)
                {
                    var modelicaConnector = CyPhyClasses.ModelicaConnector.Cast(context);
                    childParameters = modelicaConnector.Children.ModelicaParameterCollection;
                }
                else if (context.Meta.Name == typeof(CyPhy.CyberModel).Name)
                {
                    var cyberModel = CyPhyClasses.CyberModel.Cast(context);
                    childParameters = cyberModel.Children.ModelicaParameterCollection;
                }

                if (childParameters == null)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Warning,
                        Message = string.Format("Component {0} has no parameters.", context.Name)
                    };
                    result.Add(feedback);
                }
                else
                {
                    foreach (var param in childParameters)
                    {
                        string value = param.Attributes.Value;

                        if (value == null ||
                            value == "")
                        {
                            var valueFlowsIn = param.AllSrcConnections.Count();
                            var valueFlowsOut = param.AllDstConnections.Count();

                            if (valueFlowsIn == 0 &&
                                valueFlowsOut == 0)
                            {
                                var feedback = new GenericRuleFeedback()
                                {
                                    FeedbackType = FeedbackTypes.Error,
                                    Message = string.Format("ModelicaParameter {0} has an empty 'Value' field and no connections",
                                        param.Name)
                                };

                                feedback.InvolvedObjectsByRole.Add(param.Impl as IMgaFCO);
                                result.Add(feedback);
                            }
                        }


                        // META-1615 ModelicaParameter must be connected to a Property or a Parameter
                        var vfs = param.SrcConnections.ModelicaParameterPortMapCollection;
                        if (vfs.Count() == 1)
                        {
                            var vf = vfs.FirstOrDefault();
                            var srcKind = (vf.Impl as MgaSimpleConnection).Src.MetaBase.Name;
                            if (srcKind == typeof(CyPhy.Parameter).Name || srcKind == typeof(CyPhy.Property).Name)
                            {
                                // These are the only allowed flows into ModelicaParameters (Simple/CustomFormulas are not allowed)
                            }
                            else
                            {
                                var feedback = new GenericRuleFeedback()
                                {
                                    FeedbackType = FeedbackTypes.Error,
                                    Message = string.Format("ModelicaParameter {0} can only have incoming values from Properties or Parameters. " +
                                                            "(If you want a formula value, place an intermediate Parameter or Property.)",
                                                            param.Name)
                                };

                                feedback.InvolvedObjectsByRole.Add(param.Impl as IMgaFCO);
                                result.Add(feedback);
                            }
                        }
                        else if (vfs.Count() > 1)
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format("ModelicaParameter {0} has more than two incoming values! It may only have one.",
                                    param.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(param.Impl as IMgaFCO);
                            result.Add(feedback);
                        }
                    }
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("PropertiesMustNotGetValuesFromParameters", Description = "Properties are evaluated in CyPhy and Parameters are evaluated in the generated Modelica code.")]
        [Tags("Modelica")]
        [ValidContext("Property")]
        public static IEnumerable<RuleFeedbackBase> PropertiesMustNotGetValuesFromParameters(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();
                var property = CyPhyClasses.Property.Cast(context);
                if (property.ParentContainer is CyPhy.Component)
                {
                    // TODO: Should check for IsDynamicComponent
                    var component = property.ParentContainer as CyPhy.Component;
                    if (component.Children.ModelicaModelCollection.Any() == false)
                    {
                        return result;
                    }
                }
                // META-3622
                if (property.ParentContainer is CyPhy.ComponentAssembly)
                {
                    if (String.IsNullOrWhiteSpace(property.Attributes.Dimension) == false && property.Attributes.Dimension.Trim() != "1")
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Warning,
                            Message = "Property has Dimension != 1 - it will not be generated."
                        };

                        feedback.InvolvedObjectsByRole.Add(property.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                }
                var vf = property.SrcConnections.ValueFlowCollection.FirstOrDefault();
                if (vf != null)
                {
                    try
                    {
                        int iterDepth = 1;
                        if (HasOccurrencesOfKindInValueFlow(new List<string>() { typeof(CyPhy.Metric).Name }, vf, ref iterDepth))
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format("Property {0} is getting a value from a Metric. Please remove connection." +
                                                        "Metrics are evaluated after the analysis tool has run. ",
                                                        property.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(property.Impl as IMgaFCO);
                            result.Add(feedback);
                        }
                        iterDepth = 1;
                        if (HasOccurrencesOfKindInValueFlow(new List<string>() { typeof(CyPhy.Parameter).Name }, vf, ref iterDepth))
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Warning,
                                Message = string.Format("Property '{0}' is getting a value from a Parameter. If the Parameter is connected up to the TestBench level " +
                                                        "the Modelica code might not be valid w.r.t the value flow. Use only Parameters to map the value flow into the Modelica code.",
                                                        property.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(property.Impl as IMgaFCO);
                            result.Add(feedback);
                        }
                    }
                    catch (NotFiniteNumberException)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("Property '{0}' is part of a looped value flow chain. Please break this loop.",
                                                    property.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(property.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("ParametersHaveValidIncomingFlowsAndRanges", Description = "Parameters cannot have values coming in from CustomFormulas.")]
        [Tags("Modelica")]
        [ValidContext("Parameter")]
        public static IEnumerable<RuleFeedbackBase> ParametersHaveValidIncomingFlowsAndRanges(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();
                var parameter = CyPhyClasses.Parameter.Cast(context);
                if (parameter.ParentContainer is CyPhy.Component)
                {
                    // TODO: Should check for IsDynamicComponent
                    var component = parameter.ParentContainer as CyPhy.Component;
                    if (component.Children.ModelicaModelCollection.Any() == false)
                    {
                        return result;
                    }
                }
                // META-3622
                if (parameter.ParentContainer is CyPhy.Component || parameter.ParentContainer is CyPhy.ComponentAssembly)
                {
                    if (String.IsNullOrWhiteSpace(parameter.Attributes.Dimension) == false && parameter.Attributes.Dimension.Trim() != "1")
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Warning,
                            Message = "Parameter has Dimension != 1 - it will not be generated. This will break mapped value flow chains."
                        };

                        feedback.InvolvedObjectsByRole.Add(parameter.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                }
                var range = parameter.Attributes.Range.Trim().TrimStart('[').TrimEnd(']').Trim();
                if (string.IsNullOrWhiteSpace(range) == false && range != "-inf..inf")
                {
                    var values = range.Split(CyPhy2Modelica_v2.Modelica.CodeGenerator.RangeDividers, StringSplitOptions.None);
                    var rangeErrMsg = string.Format("Parameter {0} has an invalid range, '{1}'. " +
                                                    "The range must two reals (or -inf/inf) separated by {2}. Optionally it can have surrounding brackets.",
                                                    parameter.Name,
                                                    parameter.Attributes.Range,
                                                    String.Join(" or ", CyPhy2Modelica_v2.Modelica.CodeGenerator.RangeDividers.ToArray()));
                    if (values.Length != 2)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = rangeErrMsg
                        };

                        feedback.InvolvedObjectsByRole.Add(parameter.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                    else
                    {
                        double dummyDouble;
                        if (double.TryParse(values[0].Trim(), out dummyDouble) == false && values[0].Trim() != "-inf")
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = rangeErrMsg
                            };

                            feedback.InvolvedObjectsByRole.Add(parameter.Impl as IMgaFCO);
                            result.Add(feedback);
                        }

                        if (double.TryParse(values[1].Trim(), out dummyDouble) == false && values[1].Trim() != "inf")
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = rangeErrMsg
                            };

                            feedback.InvolvedObjectsByRole.Add(parameter.Impl as IMgaFCO);
                            result.Add(feedback);
                        }
                    }
                }

                var vf = parameter.SrcConnections.ValueFlowCollection.FirstOrDefault();
                if (vf != null)
                {
                    try
                    {
                        int iterDepth = 1;
                        if (HasOccurrencesOfKindInValueFlow(new List<string>() { typeof(CyPhy.Metric).Name }, vf, ref iterDepth))
                        {
                            string errMsg = string.Format("Parameter {0} is getting a value from a Metric. This is not allowed.", parameter.Name);
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = errMsg
                            };

                            feedback.InvolvedObjectsByRole.Add(parameter.Impl as IMgaFCO);
                            result.Add(feedback);
                        }
                        iterDepth = 0;
                        if (HasOccurrencesOfKindInValueFlow(new List<string>() { typeof(CyPhy.CustomFormula).Name }, vf, ref iterDepth, breakOnProperty: true))
                        {
                            string warnMsg = string.Format("Parameter {0} is getting a value through a CustomFormula. " +
                                                   "Only Properties should have such incoming value flows (since they are not generated in the Modelica code). " +
                                                   "If you try to modify a parameter in the Modelica code you may get unexpected results.",
                                                    parameter.Name);

                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Warning,
                                Message = warnMsg
                            };

                            feedback.InvolvedObjectsByRole.Add(parameter.Impl as IMgaFCO);
                            result.Add(feedback);
                        }
                    }
                    catch (NotFiniteNumberException)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("Parameter '{0}' is part of a looped value flow chain. Please break this loop.",
                                                    parameter.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(parameter.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("ComponentPropertiesHaveValues", Description = "Properties should have a Value")]
        [Tags("Modelica")]
        [ValidContext("Component", "TestComponent", "ComponentAssembly", "TestBench")]
        public static IEnumerable<RuleFeedbackBase> ComponentPropertiesHaveValues(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();
                bool checkBasetypeProperties = false;
                bool isTestBench = false;
                IEnumerable<CyPhy.Property> childProperties = null;
                IEnumerable<CyPhy.Property> basetypeChildProperties = null;

                if (context.Meta.Name == typeof(CyPhy.Component).Name)
                {
                    var component = CyPhyClasses.Component.Cast(context);
                    // TODO: Should check for IsDynamicComponent
                    if (component.Children.ModelicaModelCollection.Any() == false)
                    {
                        return result;
                    }

                    childProperties = component.Children.PropertyCollection;

                    if (component.IsInstance)
                    {
                        var basetype = Modelica.Extentions.DerivedFrom(component);
                        if (basetype != null)
                        {
                            checkBasetypeProperties = true;
                            basetypeChildProperties = basetype.Children.PropertyCollection;
                        }
                    }
                }
                else if (context.Meta.Name == typeof(CyPhy.TestComponent).Name)
                {
                    var testComponent = CyPhyClasses.TestComponent.Cast(context);
                    childProperties = testComponent.Children.PropertyCollection;
                }
                else if (context.Meta.Name == typeof(CyPhy.ComponentAssembly).Name)
                {
                    var componentAssembly = CyPhyClasses.ComponentAssembly.Cast(context);
                    childProperties = componentAssembly.Children.PropertyCollection;
                }
                else if (context.Meta.Name == typeof(CyPhy.TestBench).Name)
                {
                    var testBench = CyPhyClasses.TestBench.Cast(context);
                    childProperties = testBench.Children.PropertyCollection;
                    isTestBench = true;
                }

                foreach (var prop in childProperties)
                {
                    string value = prop.Attributes.Value;

                    if (string.IsNullOrWhiteSpace(value))
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Warning,
                            Message = string.Format("Property {0} has an empty 'Value' field. Zero, 0, will be used.",
                                prop.Name)
                        };

                        if (isTestBench)
                        {
                            feedback.FeedbackType = FeedbackTypes.Error;
                            feedback.Message = string.Format("Test-bench property {0} has an empty 'Value' field.", prop.Name);
                        }

                        feedback.InvolvedObjectsByRole.Add(prop.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                }

                if (checkBasetypeProperties)
                {
                    foreach (var prop in basetypeChildProperties)
                    {
                        string value = prop.Attributes.Value;

                        if (value == null ||
                            value == "")
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Warning,
                                Message = string.Format("Property {0} in the tree-component has no 'Value'. Zero, 0, will be used.",
                                    prop.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(prop.Impl as IMgaFCO);
                            result.Add(feedback);
                        }
                    }
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("ComponentParametersHaveValues", Description = "Parameters should have a Value")]
        [Tags("Modelica")]
        [ValidContext("Component", "TestComponent", "ComponentAssembly", "TestBench")]
        public static IEnumerable<RuleFeedbackBase> ComponentParametersHaveValues(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();
                bool checkBasetypeParameters = false;
                bool isTestBench = false;
                IEnumerable<CyPhy.Parameter> childParameters = null;
                IEnumerable<CyPhy.Parameter> basetypeChildParameters = null;

                if (context.Meta.Name == typeof(CyPhy.Component).Name)
                {
                    var component = CyPhyClasses.Component.Cast(context);
                    // TODO: Should check for IsDynamicComponent
                    if (component.Children.ModelicaModelCollection.Any() == false)
                    {
                        return result;
                    }

                    childParameters = component.Children.ParameterCollection;

                    if (component.IsInstance)
                    {
                        var basetype = Modelica.Extentions.DerivedFrom(component);
                        if (basetype != null)
                        {
                            checkBasetypeParameters = true;
                            basetypeChildParameters = basetype.Children.ParameterCollection;
                        }
                    }
                }
                else if (context.Meta.Name == typeof(CyPhy.TestComponent).Name)
                {
                    var testComponent = CyPhyClasses.TestComponent.Cast(context);
                    childParameters = testComponent.Children.ParameterCollection;
                }
                else if (context.Meta.Name == typeof(CyPhy.ComponentAssembly).Name)
                {
                    var componentAssembly = CyPhyClasses.ComponentAssembly.Cast(context);
                    childParameters = componentAssembly.Children.ParameterCollection;
                }
                else if (context.Meta.Name == typeof(CyPhy.TestBench).Name)
                {
                    var testBench = CyPhyClasses.TestBench.Cast(context);
                    childParameters = testBench.Children.ParameterCollection;
                    isTestBench = true;
                }

                foreach (var param in childParameters)
                {
                    string value = param.Attributes.Value;

                    if (string.IsNullOrWhiteSpace(value))
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Warning,
                            Message = string.Format("Parameter {0} has an empty 'Value' field. Zero, 0, will be used.",
                                param.Name)
                        };

                        if (isTestBench)
                        {
                            feedback.FeedbackType = FeedbackTypes.Error;
                            feedback.Message = string.Format("Test-bench parameter {0} has an empty 'Value' field.", param.Name);
                        }

                        feedback.InvolvedObjectsByRole.Add(param.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                }

                if (checkBasetypeParameters)
                {
                    foreach (var param in basetypeChildParameters)
                    {
                        string value = param.Attributes.Value;

                        if (value == null ||
                            value == "")
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Warning,
                                Message = string.Format("Parameter {0} in the tree component has no 'Value'. Zero, 0, will be used.",
                                    param.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(param.Impl as IMgaFCO);
                            result.Add(feedback);
                        }
                    }
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("ComponentIsDefinedInComponentFolder", Description = "Components needs to be defined in a Component Folder (red).")]
        [Tags("Modelica")]
        [ValidContext("Component")]
        public static IEnumerable<RuleFeedbackBase> ComponentIsDefinedInComponentFolder(MgaFCO context)
        {
            
            try
            {
                var result = new List<RuleFeedbackBase>();
                getExternalPackageNames(context);
                var component = CyPhyClasses.Component.Cast(context);
                // TODO: Should check for IsDynamicComponent
                if (component.Children.ModelicaModelCollection.Any() == false)
                {
                    return result;
                }

                var baseType = Modelica.Extentions.DerivedFrom(component);
                if (baseType != null)
                {
                    if (baseType.ParentContainer.Kind != typeof(CyPhy.Components).Name)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("Component {0} is defined in a {1}, please move it to a Component Folder (red).",
                                component.Name, baseType.ParentContainer.Kind)
                        };

                        feedback.InvolvedObjectsByRole.Add(context);
                        result.Add(feedback);
                    }

                    (baseType.Impl as MgaFCO).RegistryValue["IsInDesign"] = "1";

                    //try
                    //{
                    //    TryToCallFormulaEvaluator(baseType.Impl as MgaFCO);
                    //}
                    //catch (Exception ex)
                    //{
                    //    var feedback = new GenericRuleFeedback()
                    //    {
                    //        FeedbackType = FeedbackTypes.Error,
                    //        Message = string.Format("Calling formula evaluator on Component {0} raised an exception. Make sure there are no loops or awkwardnesses in the components value flows. Exception : {0}",
                    //            component.Name, ex.ToString())
                    //    };

                    //    feedback.InvolvedObjectsByRole.Add(context);
                    //    result.Add(feedback);
                    //}

                    var invalidChars = checkForInvalidCharacters(baseType.Name);
                    var reservedName = checkForInvalidName(baseType.Name);
                    if (string.IsNullOrEmpty(invalidChars) == false ||
                        string.IsNullOrEmpty(reservedName) == false)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                        };

                        if (string.IsNullOrEmpty(invalidChars) == false)
                        {
                            feedback.Message = string.Format("The definition of component '{0}' contains invalid characters '{1}'.",
                                baseType.Name, invalidChars);
                        }
                        else
                        {
                            feedback.Message = string.Format("The definition of component '{0}' has a protected name '{1}'.",
                                baseType.Name, baseType.Name);
                        }

                        feedback.InvolvedObjectsByRole.Add(context);
                        result.Add(feedback);
                        result.Add(feedback);
                    }

                    if (string.IsNullOrEmpty(baseType.Name))
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("The definition of component '{0}' has an empty name.", baseType.Name)
                        };
                        feedback.InvolvedObjectsByRole.Add(context);
                        result.Add(feedback);
                        result.Add(feedback);
                    }
                    else if (char.IsDigit(baseType.Name.First()))
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("The definition of component '{0}' name starts with a digit.", baseType.Name)
                        };
                        feedback.InvolvedObjectsByRole.Add(context);
                        result.Add(feedback);
                        result.Add(feedback);
                    }
                }
                else
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("Component {0} is defined in the design, please move it to a Component Folder (red) and add it as a reference instea.",
                            component.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(context);
                    result.Add(feedback);
                }
                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("PostProcessingPathIsValid", Description = "PostProcessing block must point to an existing file")]
        [Tags("Modelica")]
        [ValidContext("PostProcessing")]
        public static IEnumerable<RuleFeedbackBase> PostProcessingPathIsValid(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                var postProcessing = CyPhyClasses.PostProcessing.Cast(context);

                string scriptPath = postProcessing.Attributes.ScriptPath;

                if (scriptPath == null ||
                    scriptPath == "")
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("PostProcessing block {0} has an empty 'ScriptPath' field",
                            context.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                    result.Add(feedback);
                }
                else
                {
                    string projectDir = GetProjectDir(context);
                    string fullPath = System.IO.Path.Combine(projectDir, scriptPath);
                    if (System.IO.File.Exists(fullPath) == false)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("PostProcessing block {0} 'ScriptPath' points to a file that does not exist",
                                context.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                        result.Add(feedback);
                    }
                    else if (System.IO.Path.GetExtension(fullPath) != ".py")
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("PostProcessing block {0} 'ScriptPath' points to a file that is invalid (not *.py)",
                                context.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                        result.Add(feedback);
                    }
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        #region PortParameterUpdates
        [CheckerRule("PortParameterUpdates", Description = "Updating and checking port parameters")]
        [Tags("Modelica")]
        [ValidContext("ModelicaModel")]
        public static IEnumerable<RuleFeedbackBase> PortParameterUpdates(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                var modelicaModel = CyPhyClasses.ModelicaModel.Cast(context);

                List<CyPhy.ModelicaConnector> visited = new List<CyPhy.ModelicaConnector>();

                foreach (var mmConnector in modelicaModel.Children.ModelicaConnectorCollection)
                {
                    PropagateParameters(mmConnector, mmConnector, visited, result);
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        /// <summary>
        /// This function does not only propagate parameters and redeclares, it also checks redeclares for rules.
        /// </summary>
        /// <param name="startPoint">ModelicaConnector inside ModelicaModel</param>
        /// <param name="mmConnector">ModelicaConnector that can be anywhere and cannot be visted</param>
        /// <param name="visited">List of visted ModelicaConnectors</param>
        /// <param name="result">FeedBackErrors/Warnings</param>
        private static void PropagateParameters(CyPhy.ModelicaConnector startPoint, CyPhy.ModelicaConnector mmConnector, List<CyPhy.ModelicaConnector> visited, List<RuleFeedbackBase> result)
        {
            // get both endpoints which are not in visited
            IEnumerable<CyPhy.ModelicaConnector> endPoints;
            visited.Add(startPoint);
            var parent = mmConnector.ParentContainer as CyPhy.Connector;
            
            endPoints =
                mmConnector
                .DstConnections
                .PortCompositionCollection
                .Where(x => x.DstEnds.ModelicaConnector != null)  // META-3186 Add null check.
                .Select(x => x.DstEnds.ModelicaConnector)
            .Concat(
                mmConnector
                .SrcConnections
                .PortCompositionCollection
                .Where(x => x.SrcEnds.ModelicaConnector != null) // META-3186 Add null check.
                .Select(x => x.SrcEnds.ModelicaConnector))
            .Where(x => visited.Any(y => y.ID == x.ID) == false);

            if (parent != null)
            { 
                endPoints = endPoints
                .Concat(
                        parent
                        .DstConnections
                        .ConnectorCompositionCollection
                        .Select(x => x.DstEnds.Connector)
                    .Concat(
                        parent
                        .SrcConnections
                        .ConnectorCompositionCollection
                        .Select(x => x.SrcEnds.Connector))
                    .Where(x => x.Children.ModelicaConnectorCollection.FirstOrDefault() != null)
                    .Select(x => x.Children.ModelicaConnectorCollection.FirstOrDefault())
                    .Where(x => visited.Any(y => y.ID == x.ID) == false));
            }

            foreach (var endPoint in endPoints)
            {
                visited.Add(endPoint);
                // To get the parentContainer move up one level if ModelicaConnector is in a Connector.
                MgaObject parentContainer = null;
                GME.MGA.Meta.objtype_enum type;
                (endPoint.Impl as IMgaFCO).GetParent(out parentContainer, out type);
                if (parentContainer.MetaBase.Name == typeof(CyPhy.Connector).Name)
                {
                    parentContainer.GetParent(out parentContainer, out type);
                }
                //var parentContainer = endPoint.ParentContainer.Kind == typeof(CyPhy.Connector).Name ? endPoint.ParentContainer.ParentContainer : endPoint.ParentContainer;

                if (parentContainer.MetaBase.Name == typeof(CyPhy.ComponentAssembly).Name)
                {
                    var mgaParentContainer = endPoint.ParentContainer.Impl as MgaModel;
                    if (mgaParentContainer.IsPrimaryDerived)
                    {
                        mgaParentContainer.DetachFromArcheType();
                    }

                    foreach (var child in endPoint.AllChildren.ToList())
                    {
                        (child.Impl as MgaFCO).DestroyObject();
                    }

                    foreach (var child in startPoint.AllChildren)
                    {
                        if (child.Kind == typeof(CyPhy.ModelicaRedeclare).Name)
                        {
                            var redeclareParameter = CyPhyClasses.ModelicaRedeclare.Cast(child.Impl as MgaFCO);
                            if (redeclareParameter != null)
                            {
                                CheckRulesForRedeclares(startPoint, redeclareParameter, result);
                            }
                        }
                        (endPoint.Impl as MgaModel).CopyFCODisp(child.Impl as MgaFCO, (child.Impl as MgaFCO).MetaRole);
                    }
                }
                else if (parentContainer.MetaBase.Name == typeof(CyPhy.ModelicaModel).Name)
                {
                    CheckRulesForConnectedRedeclares(startPoint, endPoint, result);
                }

                PropagateParameters(startPoint, endPoint, visited, result);
            }
        }

        /// <summary>
        /// Assumption : A connector can only contain one redeclare. 
        /// (If it cotains more a warning is printed that it won't be checked).
        /// </summary>
        /// <param name="startPoint"></param>
        /// <param name="mcEndpoint"></param>
        /// <param name="result"></param>
        private static void CheckRulesForConnectedRedeclares(CyPhy.ModelicaConnector startPoint, CyPhy.ModelicaConnector endPoint, List<RuleFeedbackBase> result)
        {
            var cntStartPoint = startPoint.Children.ModelicaRedeclareCollection.Count();
            var cntEndPoint = endPoint.Children.ModelicaRedeclareCollection.Count();

            if (cntEndPoint != cntStartPoint)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("Connected {0} and {1}, do not contain the same number of ModelicaRedeclares, " +
                    "they contain {2} and {3} respectively.",
                        startPoint.Name,
                        endPoint.Name,
                        cntStartPoint,
                        cntEndPoint)
                };

                feedback.InvolvedObjectsByRole.Add(startPoint.Impl as IMgaFCO);
                feedback.InvolvedObjectsByRole.Add(endPoint.Impl as IMgaFCO);

                result.Add(feedback);
            }
            else if (cntStartPoint == 1)
            {
                var spRedeclare = startPoint.Children.ModelicaRedeclareCollection.FirstOrDefault();
                var epRedeclare = endPoint.Children.ModelicaRedeclareCollection.FirstOrDefault();

                CheckRulesForRedeclares(startPoint, spRedeclare, result);
                CheckRulesForRedeclares(endPoint, epRedeclare, result);

                if (spRedeclare.Attributes.Value != epRedeclare.Attributes.Value)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("{0} and {1} connected through {2} and {3}, do not have the same Values, " +
                        "(elaborated) values are \"{4}\" and \"{5}\" respectively.",
                            spRedeclare.Name, 
                            epRedeclare.Name,
                            startPoint.Name,
                            endPoint.Name,
                            spRedeclare.Attributes.Value, 
                            epRedeclare.Attributes.Value)
                    };

                    feedback.InvolvedObjectsByRole.Add(startPoint.Impl as IMgaFCO);
                    feedback.InvolvedObjectsByRole.Add(endPoint.Impl as IMgaFCO);

                    result.Add(feedback);
                }
            }
            else if (cntStartPoint > 0)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Warning,
                    Message = string.Format("Connected {0} and {1}, contain more than one ({2}) ModelicaRedeclares. " +
                    "The values of these will not be checked.",
                        startPoint.Name,
                        endPoint.Name,
                        cntStartPoint)
                };

                feedback.InvolvedObjectsByRole.Add(startPoint.Impl as IMgaFCO);
                feedback.InvolvedObjectsByRole.Add(endPoint.Impl as IMgaFCO);

                result.Add(feedback);
            }

        }

        /// <summary>
        /// Checks if the value is correct. Elaborates name-referenced values, e.g. Transported_Fluid.
        /// Assumption : A value is classified as a referred name if it does not contain a "."
        /// </summary>
        /// <param name="point">ModelicaConnector that containing the redeclare </param>
        /// <param name="redeclareParameter">Redeclare that will be propagated</param>
        /// <param name="result">FeedBackErrors/Warnings</param>
        private static void CheckRulesForRedeclares(CyPhy.ModelicaConnector point, CyPhy.ModelicaRedeclare redeclareParameter, List<RuleFeedbackBase> result)
        {
            // Make sure that Value isn't null or white-space
            if (string.IsNullOrWhiteSpace(redeclareParameter.Attributes.Value)
                && (point.ParentContainer.Kind == typeof(CyPhy.ModelicaModel).Name ||
                    point.ParentContainer.Kind == typeof(CyPhy.CyberModel).Name ) )
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("Redeclare {0} {1} has an empty Value.",
                        redeclareParameter.Attributes.ModelicaRedeclareType,
                        redeclareParameter.Name)
                };

                feedback.InvolvedObjectsByRole.Add(redeclareParameter.Impl as IMgaFCO);
                result.Add(feedback);
            }
            else if (redeclareParameter.Attributes.Value.Contains(".") == false)
            {
                NameReferenceDepth = 1;
                // TryToElaborateNamedReferencedRedeclare checks any found value for validity
                if (point.ParentContainer.Kind == typeof(CyPhy.ModelicaModel).Name)
                {
                    var startModelicaModel = CyPhyClasses.ModelicaModel.Cast(point.ParentContainer.Impl as MgaFCO);
                    redeclareParameter.Attributes.Value = TryToElaborateNamedReferencedRedeclare(startModelicaModel, redeclareParameter, result);
                }
                else if (point.ParentContainer.Kind == typeof(CyPhy.CyberModel).Name)
                {
                    var startCyberModel = CyPhyClasses.CyberModel.Cast(point.ParentContainer.Impl as MgaFCO);
                    redeclareParameter.Attributes.Value = TryToElaborateNamedReferencedRedeclare(startCyberModel, redeclareParameter, result);
                }
            }
            else if (redeclareParameter.Attributes.Value == "Modelica.Media.Interfaces.PartialMedium" &&
                point.ParentContainer.Kind == typeof(CyPhy.ModelicaModel).Name)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("Redeclare {0} {1} has a no specified fluid \"Modelica.Media.Interfaces.PartialMedium\".",
                        redeclareParameter.Attributes.ModelicaRedeclareType, 
                        redeclareParameter.Name)
                };

                feedback.InvolvedObjectsByRole.Add(redeclareParameter.Impl as IMgaFCO);
                result.Add(feedback);
            }
        }

        private static int NameReferenceDepth = 0;
        /// <summary>
        /// Recursively tries to obtain the value of redeclareParameter. Checks validity for found values.
        /// If the chain of referred names is > 50 it generates an error. 
        /// </summary>
        /// <param name="modelicaModel">modelicaModel that potentially holds the refered name.</param>
        /// <param name="redeclareParameter">redeclareParameter whos value seems to refer to another.</param>
        /// <param name="result">FeedBackErrors/Warnings</param>
        /// <returns>Elaborated value</returns>
        private static string TryToElaborateNamedReferencedRedeclare(CyPhy.ModelicaModel modelicaModel, CyPhy.ModelicaRedeclare redeclareParameter, List<RuleFeedbackBase> result)
        {
            var rpsInModelicaModel = modelicaModel.Children.ModelicaRedeclareCollection
                    .Where(rp => rp.Name == redeclareParameter.Attributes.Value);

            string elaboratedValue = redeclareParameter.Attributes.Value;

            if (rpsInModelicaModel.Count() == 0)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("Redeclare {0} {1} refers to {0} {2}, but it is not present in {3}.",
                        redeclareParameter.Attributes.ModelicaRedeclareType, 
                        redeclareParameter.Name,
                        redeclareParameter.Attributes.Value, 
                        modelicaModel.Name)
                };

                feedback.InvolvedObjectsByRole.Add(redeclareParameter.Impl as IMgaFCO);
                result.Add(feedback);
            }
            else if (rpsInModelicaModel.Count() == 1)
            {
                var referencedRedeclareParameter = rpsInModelicaModel.FirstOrDefault();

                // Check value of the referenced redeclare
                if (string.IsNullOrWhiteSpace(referencedRedeclareParameter.Attributes.Value))
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("Redeclare {0} {1} has an empty Value.",
                            referencedRedeclareParameter.Attributes.ModelicaRedeclareType, 
                            referencedRedeclareParameter.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(referencedRedeclareParameter.Impl as IMgaFCO);
                    result.Add(feedback);
                }
                else if (referencedRedeclareParameter.Attributes.Value == "Modelica.Media.Interfaces.PartialMedium")
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("Redeclare {0} {1} has a no specified fluid \"Modelica.Media.Interfaces.PartialMedium\".",
                            referencedRedeclareParameter.Attributes.ModelicaRedeclareType, 
                            referencedRedeclareParameter.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(referencedRedeclareParameter.Impl as IMgaFCO);
                    result.Add(feedback);
                }
                else if (referencedRedeclareParameter.Attributes.Value.Contains(".") == false)
                {
                    // Potential infinite loop
                    NameReferenceDepth += 1;
                    if(NameReferenceDepth > 1000)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("Redeclare {0} {1} value refers to a name, {2}, whos value refers to a name," +
                            "whos value refers to a name etc. Circular dependency found (1000+ referrals)! Please elaborate values.",
                                referencedRedeclareParameter.Attributes.ModelicaRedeclareType, 
                                referencedRedeclareParameter.Name,
                                referencedRedeclareParameter.Attributes.Value)
                        };

                        feedback.InvolvedObjectsByRole.Add(referencedRedeclareParameter.Impl as IMgaFCO);
                        result.Add(feedback);

                        return "Circular referenced values.";
                    }

                    referencedRedeclareParameter.Attributes.Value = TryToElaborateNamedReferencedRedeclare(modelicaModel, referencedRedeclareParameter, result);
                }

                elaboratedValue = referencedRedeclareParameter.Attributes.Value;
            }

            return elaboratedValue;
        }

        #endregion

        [CheckerRule("Workflow", Description = "Only one workflow must exist")]
        [Tags("Modelica")]
        [ValidContext("TestBench")]
        public static IEnumerable<RuleFeedbackBase> Workflow(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                var testBench = CyPhyClasses.TestBench.Cast(context);

                if (testBench.Children.WorkflowRefCollection.Count() != 1)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("Exactly one workflow must exist {0}.", context.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                    result.Add(feedback);
                }
                else
                {
                    var workflowRef = testBench.Children.WorkflowRefCollection.FirstOrDefault();
                    if (workflowRef != null &&
                        workflowRef.Referred.Workflow == null)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("Workflow reference cannot be null: {0}.", workflowRef.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(workflowRef.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                    else
                    {
                        var interpreterName = "MGA.Interpreter." + System.Reflection.Assembly.GetExecutingAssembly().FullName.Substring(0, System.Reflection.Assembly.GetExecutingAssembly().FullName.IndexOf(','));

                        var workflow = workflowRef.Referred.Workflow;
                        if (workflow.Children.TaskCollection.Count() != 1)
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format("Workflow must have exactly one task ({0}): {1}.",
                                workflow.Name,
                                interpreterName)
                            };

                            feedback.InvolvedObjectsByRole.Add(workflow.Impl as IMgaFCO);
                            result.Add(feedback);
                        }
                        else
                        {
                            var task = workflow.Children.TaskCollection.FirstOrDefault();

                            if (task != null &&
                                task.Attributes.COMName != interpreterName)
                            {
                                var feedback = new GenericRuleFeedback()
                                {
                                    FeedbackType = FeedbackTypes.Error,
                                    Message = string.Format("Workflow task {0} does not match {1}: {2}",
                                    task.Attributes.COMName,
                                    interpreterName,
                                    workflow.Name)
                                };

                                feedback.InvolvedObjectsByRole.Add(workflow.Impl as IMgaFCO);
                                result.Add(feedback);
                            }

                        }
                    }
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("OneSUTInTestBench", Description = "One and only one SUT in a TestBench")]
        [Tags("Modelica")]
        [ValidContext("TestBench")]
        public static IEnumerable<RuleFeedbackBase> OneCAInTestBench(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                var testBench = CyPhyClasses.TestBench.Cast(context);
                var compAssmCollection = testBench.Children.ComponentAssemblyCollection;
                var componentCollection = testBench.Children.ComponentCollection;
                var TLSUTCollection = testBench.Children.TopLevelSystemUnderTestCollection;
                var SUTcount = compAssmCollection.Count() + componentCollection.Count() + TLSUTCollection.Count();

                if (SUTcount != 1)
                {
                    var names = testBench.Children.ComponentAssemblyCollection.Select(x => x.Name)
                        .Concat(testBench.Children.ComponentCollection.Select(x => x.Name));

                    var involvedCompAssemblies = testBench.Children.ComponentAssemblyCollection.Select(x => x.Impl as MgaFCO);
                    var involvedComponents = testBench.Children.ComponentCollection.Select(x => x.Impl as MgaFCO);

                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("Testbench ({0}) must contain only one SystemUnderTest. {1}",
                            context.Name,
                            string.Join(", ", names))
                    };

                    feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                    feedback.InvolvedObjectsByRole.AddRange(involvedCompAssemblies);
                    feedback.InvolvedObjectsByRole.AddRange(involvedComponents);

                    result.Add(feedback);
                }
                else //SUTcount == 1
                {
                    if (componentCollection.FirstOrDefault() != null)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("SystemUnderTest ({0}) is a Component; CyPhy2Modelica requires a ComponentAssembly as the SystemUnderTest.",
                                componentCollection.FirstOrDefault().Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(componentCollection.FirstOrDefault().Impl as MgaFCO);
                        result.Add(feedback);
                    }
                    // if SUT is still 'TLSUT', it is probably a DesignContainer; check
                    else if (TLSUTCollection.FirstOrDefault() != null)
                    {
                        var TLSUT = TLSUTCollection.FirstOrDefault();
                        var referredKind = TLSUT.AllReferred.Kind;

                        if (referredKind == typeof(CyPhy.DesignContainer).Name)
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format("SystemUnderTest ({0}) refers to a DesignContainer; try the MasterInterpreter.",
                                    TLSUTCollection.FirstOrDefault().Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(TLSUTCollection.FirstOrDefault().Impl as IMgaFCO);
                            result.Add(feedback);
                        }
                        else
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format("SystemUnderTest ({0}) refers to a {1}; CyPhy2Modelica requires a ComponentAssembly as the SystemUnderTest.",
                                    TLSUTCollection.FirstOrDefault().Name,
                                    referredKind)
                            };

                            feedback.InvolvedObjectsByRole.Add(TLSUTCollection.FirstOrDefault().Impl as IMgaFCO);
                            result.Add(feedback);
                        }
                    }
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("ElaboratorMustNotLeaveRefsInTestBench", Description = "Checking if references are elaborated.")]
        [Tags("Modelica")]
        [ValidContext("TestBench")]
        public static IEnumerable<RuleFeedbackBase> ElaboratorMustNotLeaveRefsInTestBench(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                var testBench = CyPhyClasses.TestBench.Cast(context);

                if (testBench.Children.ComponentRefCollection.FirstOrDefault() != null)
                {
                    var names = testBench.Children.ComponentRefCollection.Select(x => x.Name);
                    var involvedObjects = testBench.Children.ComponentRefCollection.Select(x => x.Impl as IMgaFCO);

                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("Component references not elaborated within {0}: {1}",
                            context.Name,
                            string.Join(", ", names))
                    };

                    feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                    feedback.InvolvedObjectsByRole.AddRange(involvedObjects);
                    result.Add(feedback);
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("ElaboratorMustNotLeaveRefsInComponentAssembly", Description = "Checking if references are elaborated.")]
        [Tags("Modelica")]
        [ValidContext("ComponentAssembly")]
        public static IEnumerable<RuleFeedbackBase> ElaboratorMustNotLeaveRefsInComponentAssembly(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                var componentAssembly = CyPhyClasses.ComponentAssembly.Cast(context);

                if (componentAssembly.Children.ComponentRefCollection.FirstOrDefault() != null)
                {
                    var names = componentAssembly.Children.ComponentRefCollection.Select(x => x.Name);
                    var involvedObjects = componentAssembly.Children.ComponentRefCollection.Select(x => x.Impl as IMgaFCO);

                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("Component references not elaborated within {0}: {1}",
                            context.Name,
                            string.Join(", ", names))
                    };

                    feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                    feedback.InvolvedObjectsByRole.AddRange(involvedObjects);
                    result.Add(feedback);
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("ComponentAssemblyCannotShareNameWithItsParent", Description = "A component assembly can not have the same name as the component assembly it is within.")]
        [Tags("Modelica")]
        [ValidContext("ComponentAssembly")]
        public static IEnumerable<RuleFeedbackBase> ComponentAssemblyCannotShareNameWithItsParent(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                var componentAssembly = CyPhyClasses.ComponentAssembly.Cast(context);

                var parent = componentAssembly.ParentContainer;
                if (parent.Kind == typeof(CyPhy.ComponentAssembly).Name && parent.Name == componentAssembly.Name)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("ComponentAssemly {0} has the same name as its parent - this is not allowed.",
                            context.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(context);
                    result.Add(feedback);
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("SolverSettings", Description = "One solver settings object or use default")]
        [Tags("Modelica")]
        [ValidContext("TestBench")]
        public static IEnumerable<RuleFeedbackBase> SolverSettings(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                var testBench = CyPhyClasses.TestBench.Cast(context);

                if (testBench.Children.SolverSettingsCollection.FirstOrDefault() == null)
                {
                    var solverSettings = CyPhyClasses.SolverSettings.Create(testBench);
                    solverSettings.Name = "DefaultSolverSettings";

                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("Test bench ({0}) requires a SolverSettings block; please add one.", context.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                    result.Add(feedback);
                }
                else if (testBench.Children.SolverSettingsCollection.Count() > 1)
                {
                    var names = testBench.Children.SolverSettingsCollection.Select(x => x.Name);
                    var involvedObjects = testBench.Children.SolverSettingsCollection.Select(x => x.Impl as IMgaFCO);

                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("Test bench must have one solver settings object {0}. {1}",
                            context.Name,
                            string.Join(", ", names))
                    };

                    feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                    feedback.InvolvedObjectsByRole.AddRange(involvedObjects);
                    result.Add(feedback);
                }
                else
                {
                    // Check Attributes of an existing solversetting
                    var ss = testBench.Children.SolverSettingsCollection.FirstOrDefault();

                    if (ss.Attributes.Tolerance < 0)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("Tolerance must be greater, or equal which implies tool default, to 0. Given was '{0}'.",
                                                    ss.Attributes.Tolerance)
                        };

                        feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                        feedback.InvolvedObjectsByRole.Add(ss.Impl as MgaFCO);
                        result.Add(feedback);
                    }

                    switch (ss.Attributes.IntervalMethod)
                    {
                        case CyPhyClasses.SolverSettings.AttributesClass.IntervalMethod_enum.Number_of_Intervals:
                            if (ss.Attributes.NumberOfIntervals < 0)
                            {
                                var feedback = new GenericRuleFeedback()
                                {
                                    FeedbackType = FeedbackTypes.Error,
                                    Message = string.Format("Number of intervals must be greater, or equal which implies tool default, to 0. Given was '{0}'.",
                                                            ss.Attributes.NumberOfIntervals)
                                };

                                feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                                feedback.InvolvedObjectsByRole.Add(ss.Impl as MgaFCO);
                                result.Add(feedback);
                            }

                            break;

                        case CyPhyClasses.SolverSettings.AttributesClass.IntervalMethod_enum.Interval_Length:
                            if (ss.Attributes.IntervalLength < 0)
                            {
                                var feedback = new GenericRuleFeedback()
                                {
                                    FeedbackType = FeedbackTypes.Error,
                                    Message = string.Format("Interval length must be greater, or equal which implies tool default, to 0. Given was '{0}'.",
                                                            ss.Attributes.IntervalLength)
                                };

                                feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                                feedback.InvolvedObjectsByRole.Add(ss.Impl as MgaFCO);
                                result.Add(feedback);
                            }

                            break;
                    }
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("ConnectorWithinComponentAssembly", Description = "Within ComponentAssemblies, ModelicaConnectors within Connectors should have 0 connections")]
        [Tags("Modelica")]
        [ValidContext("ComponentAssembly")]
        public static IEnumerable<RuleFeedbackBase> ConnectorWithinComponentAssembly(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                var compAssm = CyPhyClasses.ComponentAssembly.Cast(context);
                var connectors = compAssm.Children.ConnectorCollection;

                foreach (var conn in connectors)
                {
                    // Should only be one ModelicaConnector; another rule checks for this condition
                    var modelicaConnector = conn.Children.ModelicaConnectorCollection.FirstOrDefault();

                    if (modelicaConnector == null)
                    {
                        // This does not need to be printed every single time.

                        //var feedback = new GenericRuleFeedback()
                        //{
                        //    FeedbackType = FeedbackTypes.Warning,
                        //    Message = string.Format(
                        //    "Connector ({0}) contains no ModelicaConnector (within {1})",
                        //    conn.Name,
                        //    compAssm.Name)
                        //};

                        //feedback.InvolvedObjectsByRole.Add(conn.Impl as IMgaFCO);
                        //feedback.InvolvedObjectsByRole.Add(compAssm.Impl as IMgaFCO);
                        //result.Add(feedback);

                        continue;
                    }

                    var src_PortCompositionCollection = modelicaConnector.SrcConnections.PortCompositionCollection;
                    var dst_PortCompositionCollection = modelicaConnector.DstConnections.PortCompositionCollection;

                    if (src_PortCompositionCollection.Count() != 0 ||
                        dst_PortCompositionCollection.Count() != 0)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format(
                            "ModelicaConnector within Connector within ComponentAssembly should have zero connections: {0}.",
                            modelicaConnector.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(modelicaConnector.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }


        }

        [CheckerRule("ConnectorWithinComponent", Description = "Within Components & TestComponents, ModelicaConnectors within Connectors must have a valid connection")]
        [Tags("Modelica")]
        [ValidContext("Component", "TestComponent")]
        public static IEnumerable<RuleFeedbackBase> ConnectorWithinComponent(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                var componentType = CyPhyClasses.ComponentType.Cast(context);

                var connectors = componentType.Children.ConnectorCollection;

                foreach (var connector in connectors)
                {
                    // There should only be one ModelicaConnector within Connector
                    var modelicaConnector = connector.Children.ModelicaConnectorCollection.FirstOrDefault();

                    if (modelicaConnector == null)
                    {
                        //var feedback = new GenericRuleFeedback()
                        //{
                        //    FeedbackType = FeedbackTypes.Warning,
                        //    Message = string.Format(
                        //    "Connector ({0}) contains no ModelicaConnector (within {1})",
                        //    connector.Name,
                        //    componentType.Name)
                        //};

                        //feedback.InvolvedObjectsByRole.Add(connector.Impl as IMgaFCO);
                        //feedback.InvolvedObjectsByRole.Add(componentType.Impl as IMgaFCO);
                        //result.Add(feedback);

                        continue;
                    }

                    List<CyPhy.PortComposition> srcConns = new List<CyPhy.PortComposition>();
                    List<CyPhy.PortComposition> dstConns = new List<CyPhy.PortComposition>();
                    List<CyPhy.PortComposition> allConns = new List<CyPhy.PortComposition>();

                    var srcConnPortCompCollection = modelicaConnector.SrcConnections.PortCompositionCollection;
                    var dstConnPortCompCollection = modelicaConnector.DstConnections.PortCompositionCollection;

                    if (srcConnPortCompCollection != null)
                    {
                        srcConns.AddRange(srcConnPortCompCollection);
                        allConns.AddRange(srcConnPortCompCollection);
                    }
                    if (dstConnPortCompCollection != null)
                    {
                        dstConns.AddRange(dstConnPortCompCollection);
                        allConns.AddRange(dstConnPortCompCollection);
                    }

                    foreach (var conn in allConns)
                    {
                        var mgaConn = conn.Impl as IMgaSimpleConnection;

                        string srcID = mgaConn.Src.ID;
                        string dstID = mgaConn.Dst.ID;

                        MgaObject srcParent = null;
                        GME.MGA.Meta.objtype_enum type;
                        mgaConn.Src.GetParent(out srcParent, out type);

                        MgaObject dstParent = null;
                        GME.MGA.Meta.objtype_enum typeDst;
                        mgaConn.Dst.GetParent(out dstParent, out typeDst);
                        string srcParentID = srcParent.ID;
                        string dstParentID = dstParent.ID;
                        MgaFCO parentContainer = conn.ParentContainer.Impl as MgaFCO;

                        // check for self-connections
                        if (srcID == dstID)
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format(
                                "ModelicaConnector must not have a self-connection: {0} within {1}",
                                modelicaConnector.Name,
                                modelicaConnector.ParentContainer.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(modelicaConnector.ParentContainer.Impl as IMgaFCO);
                            result.Add(feedback);
                        }
                        // check for ports connected within the same container
                        else if (srcParentID == dstParentID)
                        {
                            // Narrow it down to cases where SrcEnd, DstEnd, & conn are all in the same container
                            if (srcParentID == parentContainer.ID)
                            {
                                var feedback = new GenericRuleFeedback()
                                {
                                    FeedbackType = FeedbackTypes.Error,
                                    Message = string.Format(
                                    "Must not connect 2 ModelicaConnectors within the same Container: {0}<=>{1} within {2}",
                                    conn.SrcEnd.Name,
                                    conn.DstEnd.Name,
                                    parentContainer.Name)
                                };

                                feedback.InvolvedObjectsByRole.Add(parentContainer as IMgaFCO);
                                result.Add(feedback);
                            }
                            else if (parentContainer.MetaBase.Name == typeof(CyPhy.Component).Name)
                            {
                                var feedback = new GenericRuleFeedback()
                                {
                                    FeedbackType = FeedbackTypes.Error,
                                    Message = string.Format(
                                    "Must not connect 2 ModelicaConnectors within the same Container: {0}<=>{1} within {2}",
                                    conn.SrcEnd.Name,
                                    conn.DstEnd.Name,
                                    parentContainer.Name)
                                };

                                feedback.InvolvedObjectsByRole.Add(parentContainer as IMgaFCO);
                                result.Add(feedback);
                            }
                        }
                    }

                    bool hasOneValidConn = false;
                    HashSet<string> connectedPortIDs = new HashSet<string>();

                    foreach (var conn in srcConns)
                    {
                        var mgaConn = conn.Impl as IMgaSimpleConnection;
                        string srcID = mgaConn.Src.ID;

                        // check if this srcPort already has a connection to the port of interest
                        if (connectedPortIDs.Contains(srcID))
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format(
                                "Redundant connection between {0}<=>{1} within {2}",
                                modelicaConnector.Name,
                                conn.SrcEnd.Name,
                                context.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(context);
                            result.Add(feedback);
                        }
                        // add this srcPort to the list of connected ports
                        connectedPortIDs.Add(srcID);

                        // Make a note of any valid connection; must have at least one to pass this check
                        if (conn.SrcEnd.ParentContainer is CyPhy.ModelicaModel ||
                            conn.SrcEnd.ParentContainer is CyPhy.CyberModel )
                        {
                            hasOneValidConn = true;
                        }
                    }

                    foreach (var conn in dstConns)
                    {
                        var mgaConn = conn.Impl as IMgaSimpleConnection;
                        string dstID = mgaConn.Dst.ID;

                        MgaObject dstParent = null;
                        GME.MGA.Meta.objtype_enum typeDst;
                        mgaConn.Dst.GetParent(out dstParent, out typeDst);

                        // check if this dstPort already has a connection to the port of interest
                        if (connectedPortIDs.Contains(dstID))
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format(
                                "Redundant connection between {0}<=>{1} within {2}",
                                modelicaConnector.Name,
                                conn.DstEnd.Name,
                                context.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(context);
                            result.Add(feedback);
                        }
                        // add this dstPort to the list of connected ports
                        connectedPortIDs.Add(dstID);

                        // Make a note of any valid connection; must have at least one to pass this check
                        if (dstParent.MetaBase.Name == typeof(CyPhy.ModelicaModel).Name ||
                            dstParent.MetaBase.Name == typeof(CyPhy.CyberModel).Name )
                        {
                            hasOneValidConn = true;
                        }
                    }

                    // if we have gone through all the connections and not specifically found a valid connection, FAIL
                    if (hasOneValidConn == false)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Warning,
                            Message = string.Format(
                            "Could not find a valid connection for {0}",
                            modelicaConnector.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(modelicaConnector.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("ModelicaConnectorsMustHaveOneValidConnection", Description = "ModelicaConnectors in Components, TestComponents, and ComponentAssemblies must have a valid connection")]
        [Tags("Modelica")]
        [ValidContext("ModelicaConnector")]
        public static IEnumerable<RuleFeedbackBase> ModelicaConnectorsMustHaveOneValidConnection(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                CyPhy.ModelicaConnector modelicaConnector = CyPhyClasses.ModelicaConnector.Cast(context);
                var connParent = modelicaConnector.ParentContainer;

                // this rule should not apply to ModelicaConnectors within Connectors
                if (connParent is CyPhy.Connector)
                {
                    return result;
                }

                //List<CyPhy.PortComposition> srcConns = new List<CyPhy.PortComposition>();
                //List<CyPhy.PortComposition> dstConns = new List<CyPhy.PortComposition>();
                List<CyPhy.PortComposition> allConns = new List<CyPhy.PortComposition>();

                var srcConns = modelicaConnector.SrcConnections.PortCompositionCollection;
                var dstConns = modelicaConnector.DstConnections.PortCompositionCollection;

                // Within Components, TestComponents, and ComponentAssemblies, every ModelicaConnector must have at least one connection
                if (srcConns.Count() == 0 &&
                    dstConns.Count() == 0)
                {
                    if (connParent is CyPhy.Component ||
                        connParent is CyPhy.TestComponent ||
                        connParent is CyPhy.ComponentAssembly)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format(
                            "ModelicaConnector must have at least 1 connection! {0}.",
                            modelicaConnector.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(modelicaConnector.Impl as IMgaFCO);
                        result.Add(feedback);
                    }

                    return result;
                }

                allConns.AddRange(srcConns);
                allConns.AddRange(dstConns);

                foreach (var conn in allConns)
                {
                    var mgaConn = conn.Impl as IMgaSimpleConnection;
                    var mgaSrc = mgaConn.Src;
                    var mgaDst = mgaConn.Dst;
                    // META-3186
                    if (mgaSrc.MetaBase.Name != typeof(CyPhy.ModelicaConnector).Name ||
                        mgaDst.MetaBase.Name != typeof(CyPhy.ModelicaConnector).Name)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format(
                            "'{0}' connects a connector that is not a ModelicaConnector.",
                            modelicaConnector.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(mgaConn);
                        feedback.InvolvedObjectsByRole.Add(mgaSrc);
                        feedback.InvolvedObjectsByRole.Add(mgaDst);
                        result.Add(feedback);
                        return result;
                    }

                    string srcID = mgaSrc.ID;
                    string dstID = mgaDst.ID;

                    MgaObject srcParent = null;
                    GME.MGA.Meta.objtype_enum type;
                    mgaSrc.GetParent(out srcParent, out type);

                    MgaObject dstParent = null;
                    GME.MGA.Meta.objtype_enum typeDst;
                    mgaDst.GetParent(out dstParent, out typeDst);

                    MgaObject mgaParent = null;
                    GME.MGA.Meta.objtype_enum typeParent;
                    mgaDst.GetParent(out mgaParent, out typeParent);

                    string srcParentID = srcParent.ID;
                    string dstParentID = dstParent.ID;
                    string parentID = mgaParent.ID;

                    bool classesMatching = true;
                    string srcClass = mgaSrc.GetStrAttrByNameDisp("Class");
                    string dstClass = mgaDst.GetStrAttrByNameDisp("Class");

                    if (srcClass != dstClass)
                    {
                        // Sometimes the Class field is an empty string... Could be an Elaborator problem?
                        if (string.IsNullOrEmpty(srcClass) == false &&
                            string.IsNullOrEmpty(dstClass) == false)
                        {
                            string[] srcClassSplit = srcClass.Split('.');
                            int srcClassCount = srcClassSplit.Count();
                            string[] dstClassSplit = dstClass.Split('.');
                            int dstClassCount = dstClassSplit.Count();

                            // If the URI length is equal
                            if (srcClassCount == dstClassCount)
                            {
                                int baseCount = srcClassCount - 1;
                                string srcBaseClass = string.Join(".", srcClassSplit.Take(baseCount));
                                string dstBaseClass = string.Join(".", dstClassSplit.Take(baseCount));

                                if (srcBaseClass == dstBaseClass)
                                {
                                    string srcLast = srcClassSplit.LastOrDefault();
                                    string dstLast = dstClassSplit.LastOrDefault();

                                    bool acceptableClassPair =
                                        srcLast.Contains("Real") && dstLast.Contains("Real") ||
                                        srcLast.Contains("Boolean") && dstLast.Contains("Boolean") ||
                                        srcLast.Contains("Integer") && dstLast.Contains("Integer") ||
                                        srcLast.Contains("Frame") && dstLast.Contains("Frame") ||
                                        srcLast.Contains("Flange") && dstLast.Contains("Flange") ||
                                        srcLast.Contains("Pin") && dstLast.Contains("Pin") ||
                                        srcLast.Contains("FWB") && dstLast.Contains("FWB") ||
                                        srcLast.Contains("Hydraulic") && dstLast.Contains("Hydraulic") ||
                                        srcLast.Contains("Fluid") && dstLast.Contains("Fluid") ||
                                        srcLast.Contains("FlowPort") && dstLast.Contains("FlowPort");

                                    if (acceptableClassPair == false)
                                    {
                                        classesMatching = false;
                                    }
                                    else // it might be an acceptable mismatch...
                                    {
                                        string badCharsInSrc = checkForInvalidCharacters(srcLast);
                                        string badCharsInDst = checkForInvalidCharacters(dstLast);

                                        if (badCharsInSrc != badCharsInDst)
                                        {
                                            classesMatching = false;
                                        }
                                    }
                                }
                                else
                                {
                                    classesMatching = false;
                                }
                            }
                            else
                            {
                                classesMatching = false;
                            }
                        }
                        else
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format(
                                "ModelicaConnector Classes are both empty: {0}.{1}<=>{2}.{3} within {4}",
                                conn.SrcEnd.ParentContainer.Name,
                                conn.SrcEnd.Name,
                                conn.DstEnd.ParentContainer.Name,
                                conn.DstEnd.Name,
                                conn.ParentContainer.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(conn.ParentContainer.Impl as IMgaFCO);
                            result.Add(feedback);
                        }
                    }

                    if (classesMatching == false)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format(
                            "ModelicaConnector Classes do not match: {0}<=>{1} within {2}; Src Class: '{3}' Dst Class: '{4}'",
                            conn.SrcEnd.Name,
                            conn.DstEnd.Name,
                            conn.ParentContainer.Name,
                            srcClass,
                            dstClass)
                        };

                        feedback.InvolvedObjectsByRole.Add(conn.SrcEnd.Impl as IMgaFCO);
                        feedback.InvolvedObjectsByRole.Add(conn.DstEnd.Impl as IMgaFCO);
                        result.Add(feedback);
                    };

                    // check for self-connections
                    if (srcID == dstID)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format(
                            "ModelicaConnector must not have a self-connection: {0} within {1}",
                            modelicaConnector.Name,
                            conn.ParentContainer.Name)
                        };
                        feedback.InvolvedObjectsByRole.Add(conn.ParentContainer.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                    // check for ports connected within the same container
                    else if (srcParentID == dstParentID)
                    {
                        // Narrow it down to cases where SrcEnd, DstEnd, & conn are all in the same container
                        if (srcParentID == parentID)
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format(
                                "Must not connect 2 ModelicaConnectors within the same Container: {0}.{1}<=>{2}.{3} within {4}",
                                conn.SrcEnd.ParentContainer.Name,
                                conn.SrcEnd.Name,
                                conn.DstEnd.ParentContainer.Name,
                                conn.DstEnd.Name,
                                conn.ParentContainer.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(mgaParent as IMgaFCO);
                            result.Add(feedback);
                        }
                        else if (mgaParent.MetaBase.Name == typeof(CyPhy.Component).Name)
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format(
                                "Must not connect 2 ModelicaConnectors within the same Container: {0}<=>{1} within {2}",
                                conn.SrcEnd.Name,
                                conn.DstEnd.Name,
                                conn.ParentContainer.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(conn.ParentContainer.Impl as IMgaFCO);
                            result.Add(feedback);
                        }
                    }
                }

                bool hasOneValidConn = false;
                List<string> connectedPortIDs = new List<string>();

                foreach (var conn in srcConns)
                {
                    var mgaConn = conn.Impl as IMgaSimpleConnection;
                    var mgaSrc = mgaConn.Src;
                    string srcID = mgaSrc.ID;
                    // check if this srcPort already has a connection to the port of interest
                    if (connectedPortIDs.Contains(srcID))
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format(
                            "Redundant connection between {0}<=>{1} within {2}",
                            modelicaConnector.Name,
                            conn.SrcEnd.Name,
                            connParent.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(connParent.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                    // add this srcPort to the list of connected ports
                    connectedPortIDs.Add(srcID);

                    // Make a note of any valid connection; must have at least one to pass this check
                    if (connParent is CyPhy.Component ||
                        connParent is CyPhy.TestComponent)
                    {
                        if (conn.SrcEnd.ParentContainer is CyPhy.ModelicaModel ||
                            conn.SrcEnd.ParentContainer is CyPhy.CyberModel )
                        {
                            hasOneValidConn = true;
                        }
                    }
                    else if (connParent is CyPhy.ComponentAssembly)
                    {
                        if (conn.SrcEnd.ParentContainer is CyPhy.ComponentAssembly ||
                            conn.SrcEnd.ParentContainer is CyPhy.Component)
                        {
                            hasOneValidConn = true;
                        }
                    }
                }
                foreach (var conn in dstConns)
                {
                    // check if this dstPort already has a connection to the port of interest
                    if (connectedPortIDs.Contains(conn.DstEnd.ID))
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format(
                            "Redundant connection between {0}<=>{1} within {2}",
                            modelicaConnector.Name,
                            conn.DstEnd.Name,
                            connParent.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(connParent.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                    // add this dstPort to the list of connected ports
                    connectedPortIDs.Add(conn.DstEnd.ID);

                    // Make a note of any valid connection; must have at least one to pass this check
                    if (connParent is CyPhy.Component ||
                        connParent is CyPhy.TestComponent)
                    {
                        if (conn.DstEnd.ParentContainer is CyPhy.ModelicaModel ||
                            conn.DstEnd.ParentContainer is CyPhy.CyberModel )
                        {
                            hasOneValidConn = true;
                        }
                    }
                    else if (connParent is CyPhy.ComponentAssembly)
                    {
                        if (conn.DstEnd.ParentContainer is CyPhy.ComponentAssembly ||
                            conn.DstEnd.ParentContainer is CyPhy.Component)
                        {
                            hasOneValidConn = true;
                        }
                    }
                }

                // if we have gone through all the connections and not specifically found a valid connection, FAIL
                if (connParent is CyPhy.Component ||
                    connParent is CyPhy.TestComponent ||
                    connParent is CyPhy.ComponentAssembly)
                {
                    if (hasOneValidConn == false)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format(
                            "Could not find a valid connection for {0}",
                            modelicaConnector.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(modelicaConnector.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("ConnectorsMustHaveOneValidConnection", Description = "Connectors in Components, TestComponents, and ComponentAssemblies must have a valid connection")]
        [Tags("Modelica")]
        [ValidContext("Connector")]
        public static IEnumerable<RuleFeedbackBase> ConnectorsMustHaveOneValidConnection(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                CyPhy.Connector connector = CyPhyClasses.Connector.Cast(context);
                if (connector.Children.ModelicaConnectorCollection.Any() == false)
                {
                    // If there are no ModelicaConnectors inside no rules apply (Connector and connections will not be generated)
                    return result;
                }

                var connParent = connector.ParentContainer;

                //List<CyPhy.ConnectorComposition> srcConns = new List<CyPhy.ConnectorComposition>();
                //List<CyPhy.ConnectorComposition> dstConns = new List<CyPhy.ConnectorComposition>();
                //List<CyPhy.ConnectorComposition> allConns = new List<CyPhy.ConnectorComposition>();

                var srcConns = connector.SrcConnections.ConnectorCompositionCollection;
                var dstConns = connector.DstConnections.ConnectorCompositionCollection;
                var allConns = srcConns.Concat(dstConns);

                // Within ComponentAssemblies, every Connector must have at least one connection
                if (srcConns.Count() == 0 && dstConns.Count() == 0 && connParent is CyPhy.ComponentAssembly)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format(
                        "Connector with ModelicaConnectors must have at least 1 connection! {0}.",
                        connector.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(connector.Impl as IMgaFCO);
                    result.Add(feedback);
                }

                foreach (var conn in allConns)
                {
                    string srcID = conn.SrcEnd.ID;
                    string dstID = conn.DstEnd.ID;

                    var srcModelicaConnector = (conn.SrcEnd as CyPhy.Connector).Children.ModelicaConnectorCollection.FirstOrDefault();
                    var dstModelicaConnector = (conn.DstEnd as CyPhy.Connector).Children.ModelicaConnectorCollection.FirstOrDefault();

                    if (srcModelicaConnector == null || dstModelicaConnector == null)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Warning,
                            Message = string.Format(
                            "Connection between Connectors with and without ModelicaConnectors will not be generated, ({0}.{1}<=>{2}.{3} within {4}).",
                            conn.SrcEnd.ParentContainer.Name,
                            conn.SrcEnd.Name,
                            conn.DstEnd.ParentContainer.Name,
                            conn.DstEnd.Name,
                            conn.ParentContainer.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(conn.ParentContainer.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                    else if (srcModelicaConnector != null && dstModelicaConnector != null)
                    {
                        string srcClass = srcModelicaConnector.Attributes.Class;
                        string dstClass = dstModelicaConnector.Attributes.Class;
                        bool classesMatching = true;

                        if (srcClass != dstClass)
                        {
                            // Sometimes the Class field is an empty string... Could be an Elaborator problem?
                            if (srcClass != "" &&
                                dstClass != "")
                            {
                                string[] srcClassSplit = srcClass.Split('.');
                                int srcClassCount = srcClassSplit.Count();
                                string[] dstClassSplit = dstClass.Split('.');
                                int dstClassCount = dstClassSplit.Count();

                                // If the URI length is equal
                                if (srcClassCount == dstClassCount)
                                {
                                    int baseCount = srcClassCount - 1;
                                    string srcBaseClass = string.Join(".", srcClassSplit.Take(baseCount));
                                    string dstBaseClass = string.Join(".", dstClassSplit.Take(baseCount));

                                    if (srcBaseClass == dstBaseClass)
                                    {
                                        string srcLast = srcClassSplit.LastOrDefault();
                                        string dstLast = dstClassSplit.LastOrDefault();

                                        bool acceptableClassPair =
                                            srcLast.Contains("Real") && dstLast.Contains("Real") ||
                                            srcLast.Contains("Boolean") && dstLast.Contains("Boolean") ||
                                            srcLast.Contains("Integer") && dstLast.Contains("Integer") ||
                                            srcLast.Contains("Frame") && dstLast.Contains("Frame") ||
                                            srcLast.Contains("Flange") && dstLast.Contains("Flange") ||
                                            srcLast.Contains("Pin") && dstLast.Contains("Pin") ||
                                            srcLast.Contains("FWB") && dstLast.Contains("FWB") ||
                                            srcLast.Contains("Hydraulic") && dstLast.Contains("Hydraulic") ||
                                            srcLast.Contains("Fluid") && dstLast.Contains("Fluid") ||
                                            srcLast.Contains("FlowPort") && dstLast.Contains("FlowPort");

                                        if (acceptableClassPair == false)
                                        {
                                            classesMatching = false;
                                        }
                                        else // it might be an acceptable pair...
                                        {
                                            string badCharsInSrc = checkForInvalidCharacters(srcLast);
                                            string badCharsInDst = checkForInvalidCharacters(dstLast);

                                            if (badCharsInSrc != badCharsInDst)
                                            {
                                                classesMatching = false;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        classesMatching = false;
                                    }
                                }
                                else
                                {
                                    classesMatching = false;
                                }
                            }
                            else
                            {
                                if (string.IsNullOrWhiteSpace(srcClass))
                                {
                                    var feedback = new GenericRuleFeedback()
                                    {
                                        FeedbackType = FeedbackTypes.Error,
                                        Message = string.Format(
                                        "ModelicaConnector '{0}' Class is empty.",
                                        conn.SrcEnd.Name)
                                    };

                                    feedback.InvolvedObjectsByRole.Add(conn.SrcEnd.Impl as IMgaFCO);
                                    result.Add(feedback);
                                }
                                if (string.IsNullOrWhiteSpace(dstClass))
                                {
                                    var feedback = new GenericRuleFeedback()
                                    {
                                        FeedbackType = FeedbackTypes.Error,
                                        Message = string.Format(
                                        "ModelicaConnector '{0}' Class is empty.",
                                        conn.DstEnd.Name)
                                    };

                                    feedback.InvolvedObjectsByRole.Add(conn.DstEnd.Impl as IMgaFCO);
                                    result.Add(feedback);
                                }
                            }
                        }

                        if (classesMatching == false)
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format(
                                "ModelicaConnector Classes do not match: {0}<=>{1} within {2}; Src Class: '{3}' Dst Class: '{4}'",
                                conn.SrcEnd.Name,
                                conn.DstEnd.Name,
                                conn.ParentContainer.Name,
                                srcClass,
                                dstClass)
                            };

                            feedback.InvolvedObjectsByRole.Add(conn.SrcEnd.Impl as IMgaFCO);
                            feedback.InvolvedObjectsByRole.Add(conn.DstEnd.Impl as IMgaFCO);
                            result.Add(feedback);
                        }
                    }

                    // check for self-connections
                    if (srcID == dstID)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format(
                            "Connector must not have a self-connection: {0}.{1} within {2}",
                            connector.ParentContainer.Name,
                            connector.Name,
                            conn.ParentContainer.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(conn.ParentContainer.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                    // check for ports connected within the same container
                    else if (conn.SrcEnd.ParentContainer.ID == conn.DstEnd.ParentContainer.ID)
                    {
                        // This narrows it down to connections where SrcEnd, DstEnd, & Conn all have the same parent (we don't want thi)
                        if (conn.SrcEnd.ParentContainer.ID == conn.ParentContainer.ID)
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format(
                                "Must not connect 2 Connectors within the same Container: {0}<=>{1} within {2}",
                                conn.SrcEnd.Name,
                                conn.DstEnd.Name,
                                conn.ParentContainer.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(conn.ParentContainer.Impl as IMgaFCO);
                            result.Add(feedback);
                        }
                    }
                }

                bool hasOneValidConn = false;
                List<string> connectedPortIDs = new List<string>();

                foreach (var conn in srcConns)
                {
                    // check if this srcPort already has a connection to the port of interest
                    if (connectedPortIDs.Contains(conn.SrcEnd.ID))
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format(
                            "Redundant connection between {0}.{1}<=>{2}.{3} within {4}",
                            connector.ParentContainer.Name,
                            connector.Name,
                            conn.SrcEnd.ParentContainer.Name,
                            conn.SrcEnd.Name,
                            conn.ParentContainer.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(conn.ParentContainer.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                    // add this srcPort to the list of connected ports
                    connectedPortIDs.Add(conn.SrcEnd.ID);

                    // Make a note of any valid connection; must have at least one to pass this check
                    if (connParent is CyPhy.ComponentAssembly)
                    {
                        if (conn.SrcEnd.ParentContainer is CyPhy.ComponentAssembly ||
                            conn.SrcEnd.ParentContainer is CyPhy.Component)
                        {
                            hasOneValidConn = true;
                        }
                    }
                }
                foreach (var conn in dstConns)
                {
                    // check if this dstPort already has a connection to the port of interest
                    if (connectedPortIDs.Contains(conn.DstEnd.ID))
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format(
                            "Redundant connection between {0}.{1}<=>{2}.{3} within {4}",
                            connector.ParentContainer.Name,
                            connector.Name,
                            conn.DstEnd.ParentContainer.Name,
                            conn.DstEnd.Name,
                            conn.ParentContainer.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(conn.ParentContainer.Impl as IMgaFCO);
                        result.Add(feedback);
                    }
                    // add this dstPort to the list of connected ports
                    connectedPortIDs.Add(conn.DstEnd.ID);

                    // Make a note of any valid connection; must have at least one to pass this check
                    if (connParent is CyPhy.ComponentAssembly)
                    {
                        if (conn.DstEnd.ParentContainer is CyPhy.ComponentAssembly ||
                            conn.DstEnd.ParentContainer is CyPhy.Component)
                        {
                            hasOneValidConn = true;
                        }
                    }
                }

                // if we have gone through all the connections and not specifically found a valid connection, FAIL
                if (connParent is CyPhy.ComponentAssembly &&
                    hasOneValidConn == false)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format(
                        "Could not find a valid connection for {0}",
                        connector.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(connector.Impl as IMgaFCO);

                    result.Add(feedback);
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("NamesMustBeValid", Description = "Based on context/containment, names must be unique/valid")]
        [Tags("Modelica")]
        [ValidContext("TestBench", "TestComponent", "Component", "ComponentAssembly", "ModelicaModel", "Connector", "ModelicaConnector")]
        public static IEnumerable<RuleFeedbackBase> NamesMustBeValid(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();
                var parentIsComponent = false;
                if (context.MetaBase.Name == typeof(CyPhy.Component).Name)
                {
                    parentIsComponent = true;
                    // TODO: Should check for IsDynamicComponent
                    var component = CyPhyClasses.Component.Cast(context);
                    if (component.Children.ModelicaModelCollection.Any() == false)
                    {
                        return result;
                    }
                }

                List<string> TBtestCompBaseNames = new List<string>();

                // we don't care what type of object it is for certain cases; children should always have unique names
                List<string> names = new List<string>();

                var childFCOs = context.ChildObjects.OfType<MgaFCO>();

                foreach (var child in childFCOs)
                {
                    string childName = child.Name;
                    string childKind = child.Meta.Name;

                    // Only check names for defined Kinds
                    if (NameCheckWhiteList.Contains(childKind) == false)
                    {
                        continue;
                    }

                    // Don't check connectors with no modelica-connectors
                    if (child.Meta.Name == typeof(CyPhy.Connector).Name)
                    {
                        var connector = CyPhyClasses.Connector.Cast(child);
                        if (connector.Children.ModelicaConnectorCollection.Count() == 0)
                        {
                            continue;
                        }
                    }

                    // Don't check names of Properties inside Components
                    if (parentIsComponent)
                    {
                        if (childKind == typeof(CyPhy.Property).Name)
                        {
                            continue;
                        }
                    }

                    if (childName.Length < 1)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format(
                            "{0} (Kind: {1}) has no name (name length is 0)",
                            child.Name,
                            child.Meta.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(child);
                        result.Add(feedback);
                    }

                    string badCharacters = null;
                    badCharacters = checkForInvalidCharacters(childName);

                    // Check if name is valid
                    if (string.IsNullOrEmpty(badCharacters) == false)
                    {
                        // ModelicaParameters are allowed to have '.' in the name: modifiers such as w.start, length.min
                        bool isParameterWithDots =
                            (child.Meta.Name == typeof(CyPhy.ModelicaParameter).Name &&
                            badCharacters == ".");

                        bool isArrayModelicaConnector = child.Meta.Name == typeof(CyPhy.ModelicaConnector).Name &&
                            badCharacters == "[]";

                        if (isParameterWithDots == false && isArrayModelicaConnector == false)
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format(
                                "{0} has an invalid name; it contains '{1}'",
                                child.Name,
                                badCharacters)
                            };

                            feedback.InvolvedObjectsByRole.Add(child);
                            result.Add(feedback);
                        }
                    }

                    // get the list of packages attached from the CyPhy2Modelica form
                    getExternalPackageNames(context);

                    string protectedName = checkForInvalidName(childName);

                    if (protectedName != null)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format(
                            "{0} has an invalid name; {1} is a reserved name",
                            child.Name,
                            protectedName)
                        };

                        feedback.InvolvedObjectsByRole.Add(child);
                        result.Add(feedback);
                    }

                    if (childName.Length > 0 && char.IsDigit(childName[0]))
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format(
                            "{0} has an invalid name; names must not begin with a digit",
                            child.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(child);
                        result.Add(feedback);
                    }

                    if (names.Contains(childName))
                    {
                        var conflictedFcos = childFCOs.Where(x => x.Name == childName);

                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format(
                            "'{0}' ({1}) must have a unique name within '{2}' ({3})",
                            childName,
                            conflictedFcos.FirstOrDefault().Meta.Name,
                            context.Name,
                            context.Meta.Name)
                        };

                        foreach (var fco in conflictedFcos)
                        {
                            feedback.InvolvedObjectsByRole.Add(fco as IMgaFCO);
                        }

                        feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                        result.Add(feedback);
                    }

                    // TestComponent BaseType names must be unique
                    else if (child is CyPhy.TestComponent)
                    {
                        // assume it has an archetype, because we should not ever encounter a root object when running CyPhy2Modelica... (?)
                        string baseName = child.ArcheType.Name;
                        if (TBtestCompBaseNames.Contains(baseName))
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format(
                                "The basetype of {0} has the same name as sibling TestComponent's basetype. Please change one of them.",
                                child.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(child);
                            result.Add(feedback);
                        }
                        TBtestCompBaseNames.Add(baseName);
                    }
                    else if (child is CyPhy.ComponentAssembly && context is CyPhy.ComponentAssembly)
                    {
                        if (childName == context.Name)
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format(
                                "{0} has the same name as its parent container; all ComponentAssemblies must have unique names",
                                child.Name)
                            };

                            feedback.InvolvedObjectsByRole.Add(child);
                            result.Add(feedback);
                        }
                    }
                    names.Add(childName);
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("TestBenchNameMustBeValid", Description = "Test bench name must be vaild.")]
        [Tags("Modelica")]
        [ValidContext("TestBench")]
        public static IEnumerable<RuleFeedbackBase> TestBenchNameMustBeValid(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();
                var tbName = context.Name;
                string badCharacters = null;
                badCharacters = checkForInvalidCharacters(tbName);
                if (string.IsNullOrEmpty(badCharacters) == false)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format(
                            "The test-bench '{0}' contains invalid characters; '{1}'.",
                            tbName,
                            badCharacters)
                    };

                    feedback.InvolvedObjectsByRole.Add(context);
                    result.Add(feedback);
                }

                getExternalPackageNames(context);
                var protectedName = checkForInvalidName(tbName);
                if (string.IsNullOrEmpty(protectedName) == false)
                {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format(
                            "Test-bench name {0} is invalid; {1} is a reserved name.",
                            tbName,
                            protectedName)
                        };

                        feedback.InvolvedObjectsByRole.Add(context);
                        result.Add(feedback);
                }

                if (tbName.Length == 0)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format(
                        "Test-bench name {0} is empty, please give it a name.",
                        tbName)
                    };

                    feedback.InvolvedObjectsByRole.Add(context);
                    result.Add(feedback);
                }
                else if(char.IsDigit(tbName[0]))
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format(
                        "Test-bench name {0} is invalid; names must not begin with a digit.",
                        tbName)
                    };

                    feedback.InvolvedObjectsByRole.Add(context);
                    result.Add(feedback);
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("ModelicaConnectorHasValidURI", Description = "ModelicaConnector must have valid URI")]
        [Tags("Modelica")]
        [ValidContext("ModelicaConnector")]
        public static IEnumerable<RuleFeedbackBase> ModelicaConnectorHasValidURI(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                string[] splitURI = null;

                var modelicaConnector = CyPhyClasses.ModelicaConnector.Cast(context);

                // If the ModelicaConnector is in a Connector, the Class might be empty...
                if (modelicaConnector.ParentContainer is CyPhy.Connector &&
                    string.IsNullOrWhiteSpace(modelicaConnector.Attributes.Class))
                {
                    return result;
                }

                splitURI = modelicaConnector.Attributes.Class.Split('.');
                int pathLength = splitURI.Length;

                // URI must have at least 2 classes, i.e., AttachedLib.Model, (3 might be better)
                if (splitURI.Length < 2)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("{0}: 'Class' attribute should refer to a model within the attached package (path is too short or empty)",
                            context.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                    result.Add(feedback);
                }

                getExternalPackageNames(context);

                if (externalPackageNames.Contains(splitURI[0]) == false)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("{0}: the first element in the 'Class' attribute should refer to an attached Modelica package",
                            context.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                    result.Add(feedback);
                }

                string badCharacters = null;

                foreach (string name in splitURI)
                {
                    string additionalBadChars = checkForInvalidCharacters(name);
                    if (string.IsNullOrEmpty(additionalBadChars) == false)
                    {
                        badCharacters = badCharacters + additionalBadChars;
                    }

                    if (name.Length > 0 &&
                        char.IsDigit(name[0]))
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format(
                            "{0} has an invalid Class; '{1}' must not begin with a digit",
                            context.Name,
                            name)
                        };

                        feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                        result.Add(feedback);
                    }
                }

                if (badCharacters != null &&
                    badCharacters != "")
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format(
                        "{0} - Class '{1}' contains invalid characters: '{2}'",
                        context.Name,
                        modelicaConnector.Attributes.Class,
                        badCharacters)
                    };

                    feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                    result.Add(feedback);
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("ModelicaModelHasValidURI", Description = "ModelicaModel must have valid URI, referring to an attached library")]
        [Tags("Modelica")]
        [ValidContext("Component", "TestComponent")]
        public static IEnumerable<RuleFeedbackBase> ModelicaModelHasValidURI(MgaFCO context)
        {
            var result = new List<RuleFeedbackBase>();
            try
            {
                // Check if it's a Component
                if (context.Meta.Name == typeof(CyPhy.Component).Name)
                {
                    bool checkAllModelicaURIs = false;

                    CyPhy.Component component = CyPhyClasses.Component.Cast(context);

                    // If it has a single ModelicaModel, check the URI
                    var nbrOfModelicaModels = (context as GME.MGA.IMgaModel).GetChildrenOfKind(typeof(CyPhy.ModelicaModel).Name).Count;
                    if (nbrOfModelicaModels == 0)
                    {
                        return result;
                    }
                    else if (nbrOfModelicaModels == 1)
                    {
                        var modelicaModel = component.Children.ModelicaModelCollection.FirstOrDefault();
                        result.AddRange(checkModelicaModelURI(modelicaModel.Impl as MgaFCO));
                    }
                    else // It has multiple ModelicaModels, check if we have 'Classifications'
                    {
                        string componentClass = component.Attributes.Classifications;

                        if (string.IsNullOrWhiteSpace(componentClass))
                        {
                            // No discernable Classifications, check all ModelicaModel URIs
                            checkAllModelicaURIs = true;
                        }
                        else // Component's Classifications is 'legit'
                        {
                            var thisComponentFidelitySetting = new Dictionary<string, string>();
                            string modelicaModelName = null;

                            // Get the Testbench's FidelitySettings Regeistry Node
                            try
                            {
                                CyPhy.TestBench tb = CyPhyClasses.TestBench.Cast(GetTestbench(context));
                                var fidelitySettings = CodeGenMethods.GetFidelitySettingsFromTestBench(tb);

                                fidelitySettings.TryGetValue(componentClass, out thisComponentFidelitySetting);
                                thisComponentFidelitySetting.TryGetValue(typeof(CyPhy.ModelicaModel).Name, out modelicaModelName);
                            }
                            catch (Exception)
                            {
                                var feedback = new GenericRuleFeedback()
                                {
                                    FeedbackType = FeedbackTypes.Warning,
                                    Message = string.Format("Problem getting FidelitySettings for {0}; checking all Modelica URIs",
                                        context.Name)
                                };

                                feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                                result.Add(feedback);

                                checkAllModelicaURIs = true;
                            }

                            if (modelicaModelName != null)
                            {
                                var modelicaModel = component.Children.ModelicaModelCollection.FirstOrDefault(x => x.Name == modelicaModelName);
                                if (modelicaModel == null)
                                {
                                    checkAllModelicaURIs = true;
                                }
                                else
                                {
                                    // Actually found the correct model
                                    result.AddRange(checkModelicaModelURI(modelicaModel.Impl as MgaFCO));
                                }
                            }
                            else
                            {
                                checkAllModelicaURIs = true;
                            }
                        }

                        if (checkAllModelicaURIs)
                        {
                            foreach (var mm in component.Children.ModelicaModelCollection)
                            {
                                result.AddRange(checkModelicaModelURI(mm.Impl as MgaFCO));
                            }
                        }
                    }
                }
                else if (context.Meta.Name == typeof(CyPhy.TestComponent).Name)
                {
                    // This is a TestComponent
                    CyPhy.ModelicaModel modelicaModel = 
                        (CyPhyClasses.TestComponent.Cast(context).Children.ModelicaModelCollection.FirstOrDefault());
                    if (modelicaModel == null)
                    {
                        var feedback = new GenericRuleFeedback()
                        {
                            FeedbackType = FeedbackTypes.Error,
                            Message = string.Format("Test-component does not have a modelica-model, '{0}'.",
                                context.Name)
                        };

                        feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
                        result.Add(feedback);
                    }
                    else
                    {
                        result.AddRange(checkModelicaModelURI(modelicaModel.Impl as MgaFCO));
                    }
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("ComponentTreeHasValidStructure", Description = "Component tree must map to valid Modelica code.")]
        [Tags("Modelica")]
        [ValidContext("TestBench")]
        public static IEnumerable<RuleFeedbackBase> ComponentTreeHasValidStructure(MgaFCO context)
        {
            try
            {
                getExternalPackageNames(context);
                var result = new List<RuleFeedbackBase>();
                CyPhy.RootFolder projectRootFolder = CyPhyClasses.RootFolder.GetRootFolder(context.Project);

                var invalidChars = checkForInvalidCharacters(projectRootFolder.Name);
                var reservedName = checkForInvalidName(projectRootFolder.Name);
                if (string.IsNullOrEmpty(invalidChars) == false ||
                    string.IsNullOrEmpty(reservedName) == false)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                    };

                    if (string.IsNullOrEmpty(invalidChars) == false)
                    {
                        feedback.Message = string.Format("The project's RootFolder '{0}' contains invalid characters '{1}'.",
                            projectRootFolder.Name, invalidChars);
                    }
                    else
                    {
                        feedback.Message = string.Format("The project's RootFolder '{0}' has a protected name.",
                            projectRootFolder.Name);
                    }
                    result.Add(feedback);
                }
                else
                {
                    var folderNames = new List<string>();
                    foreach (var componentsFolder in projectRootFolder.Children.ComponentsCollection)
                    {
                        // Check uniquness of names of components folder in root-folder
                        if (folderNames.Contains(componentsFolder.Name))
                        {
                            var feedback = new GenericRuleFeedback()
                            {
                                FeedbackType = FeedbackTypes.Error,
                                Message = string.Format("The project's RootFolder '{0}' contains component-folders with non-unique names '{1}'.",
                                    projectRootFolder.Name, componentsFolder.Name)
                            };

                            result.Add(feedback);
                        }
                        else
                        {
                            folderNames.Add(componentsFolder.Name);
                        }
                        // Check rules for folders
                        if (FolderFilter.FolderHasModelicaComponents(componentsFolder))
                        {
                            var folder = new FolderTreeNode(componentsFolder);
                            FolderFilter.PopulateWithFilteredNodes(ref folder);
                            result.AddRange(CheckFolderStructure(folder));
                        }
                    }
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }

        }

        [CheckerRule("NoMoreThanOneModelicaConnectorWithinConnector", Description = "Any connector must not contain more than 1 ModelicaConnector.")]
        [Tags("Modelica")]
        [ValidContext("Connector")]
        public static IEnumerable<RuleFeedbackBase> NoMoreThanOneModelicaConnectorWithinConnectors(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                var connector = CyPhyClasses.Connector.Cast(context);

                var modelicaConnectors = connector.AllChildren.OfType<CyPhy.ModelicaConnector>();

                if (modelicaConnectors.Count() > 1)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format("Not more than one ModelicaConnector allowed within Connector: {0}",
                            connector.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(connector.Impl as IMgaFCO);
                    result.Add(feedback);
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }
        }

        [CheckerRule("ConnectorsShouldHaveChildren", Description = "All Connectors should contain some object (ModelicaConnector).")]
        [Tags("Modelica")]
        [ValidContext("Connector")]
        public static IEnumerable<RuleFeedbackBase> ConnectorsShouldHaveChildren(MgaFCO context)
        {
            try
            {
                var result = new List<RuleFeedbackBase>();

                var connector = CyPhyClasses.Connector.Cast(context);

                var children = connector.AllChildren;

                if (children.Count() == 0)
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Warning,
                        Message = string.Format("Connector has no child objects: {0}",
                            connector.Name)
                    };

                    feedback.InvolvedObjectsByRole.Add(connector.Impl as IMgaFCO);
                    result.Add(feedback);
                }

                return result;
            }
            catch (Exception ex)
            {
                return GetExceptionRuleFeedBack(ex, context);
            }


        }

        private static HashSet<string> NameCheckWhiteList = new HashSet<string>()
        {
            {typeof(CyPhy.Component).Name},
            {typeof(CyPhy.TestComponent).Name},
            {typeof(CyPhy.ComponentAssembly).Name},
            {typeof(CyPhy.Connector).Name},
            {typeof(CyPhy.ModelicaConnector).Name},
            //{typeof(CyPhy.ModelicaModel).Name},
            {typeof(CyPhy.ModelicaParameter).Name},
            {typeof(CyPhy.Property).Name},
            {typeof(CyPhy.Parameter).Name},
            {typeof(CyPhy.Metric).Name},
            {typeof(CyPhy.Environment).Name},
            {typeof(CyPhy.ModelicaRedeclare).Name}
        };

        public static HashSet<char> invalidCharacters = new HashSet<char>()
        {
            {'!'},
            {'@'},
            {'#'},
            {'$'},
            {'%'},
            {'^'},
            {'&'},
            {'*'},
            {'('},
            {')'},
            {'-'},
            {'{'},
            {'}'},
            {'['},
            {']'},
            {'|'},
            {';'},
            {':'},
            {','},
            {'<'},
            {'.'},
            {'>'},
            {'/'},
            {'?'},
            {'~'},
            {'`'},
            {'+'},
            {'='},
            {' '},
        };

        public static HashSet<string> ReservedModelicaKeyWords = new HashSet<String>() 
            {
                "algorithm", "discrete", " false", "model", "redeclare", 
                "and", "each", "final", "not", "replaceable", 
                "annotation", "else", "flow", "operator", "return", 
                "assert", "elseif", "for", "or", "stream", 
                "block", "elsewhen", "function", "outer", "then", 
                "break", "encapsulated", "if", "output", "true", 
                "class", "end", "import", "package", "type", 
                "connect", "enumeration", "in", "parameter", "when", 
                "connector", "equation", "initial", "partial", "while", 
                "constant", "expandable", "inner", "protected", "within", 
                "constrainedby", "extends", "input", "public", 
                "der", "external", "loop", "record" 
            }; /// from ModelicaLanguageSpecification version 3.2

        public static List<string> externalPackageNames = null;

        // used to get attached package names; these strings are invalid for certain uses in a CyPhy project
        private static void getExternalPackageNames(MgaFCO context)
        {
            if (externalPackageNames == null)
            {
                externalPackageNames = new List<string>();
                // get previous GUI config
                var cyPhy2Modelica_v2 = new META.ComComponent("MGA.Interpreter.CyPhy2Modelica_v2");
                var projectDir = GetProjectDir(context);
                var config = META.ComComponent.DeserializeConfiguration(projectDir, typeof(CyPhy2Modelica_v2Settings), cyPhy2Modelica_v2.ProgId);
                if (config == null)
                {
                    config = new CyPhy2Modelica_v2Settings();
                }
                
                var paths = (config as CyPhy2Modelica_v2Settings).IncludeDirectoryPath;
                foreach (var path in paths)
                {
                    externalPackageNames.Add(ModelicaLibrary.GetLibraryFromPath(path).Name);
                }

                externalPackageNames.Add("Modelica");
                externalPackageNames.Add(Modelica.CodeGenerator.MainPackage);
                // Cyber business
                externalPackageNames.Add(CyPhyClasses.CyberModel.AttributesClass.ModelType_enum.ESMoL.ToString());
                externalPackageNames.Add(CyPhyClasses.CyberModel.AttributesClass.ModelType_enum.SignalFlow.ToString());
                externalPackageNames.Add(CyPhyClasses.CyberModel.AttributesClass.ModelType_enum.Simulink.ToString());
            }
        }

        public static string ProjectDir;

        public static string GetProjectDir(MgaFCO context)
        {
            if (string.IsNullOrWhiteSpace(ProjectDir))
            {
                ProjectDir = System.IO.Path.GetDirectoryName(context.Project.ProjectConnStr.Substring("MGA=".Length));
            }

            return ProjectDir;
        }

        public static string checkForInvalidCharacters(string childName)
        {
            var badCharList = "";

            foreach (char badChar in invalidCharacters)
            {
                if (childName.Contains(badChar))
                {
                    badCharList = badCharList + badChar;
                }
            }

            return badCharList;
        }

        public static string checkForInvalidName(string childName)
        {
            string invalidName = null;

            if (externalPackageNames.Contains(childName))
            {
                invalidName = childName;
            }
            else if (ReservedModelicaKeyWords.Contains(childName))
            {
                invalidName = childName;
            }

            return invalidName;
        }

        public static bool HasOccurrencesOfKindInValueFlow(List<string> kinds, CyPhy.ValueFlow valueFlow, ref int iterDepth, bool breakOnProperty = false)
        {
            iterDepth++;
            if (iterDepth > 10000)
            {
                throw new NotFiniteNumberException();
            }
            var result = false;
            var mgaValueFlow = valueFlow.Impl as GME.MGA.IMgaSimpleConnection;

            MgaObject vfParent = null;
            GME.MGA.Meta.objtype_enum typeParent;
            mgaValueFlow.GetParent(out vfParent, out typeParent);

            var mgaSrc = mgaValueFlow.Src;

            if (vfParent.MetaBase.Name == typeof(CyPhy.TestBenchSuite).Name)
            {
                // In a SoT the incoming value is allowed to come from a Metric.
                return false;
            }

            if (breakOnProperty && mgaSrc.MetaBase.Name == typeof(CyPhy.Property).Name)
            {
                // Stop the checking  of Parameters when a Property is encountered.
                return false;
            }

            if (kinds.Contains(mgaSrc.MetaBase.Name))
            {
                return true;
            }

            foreach (var vf in CyPhyClasses.ValueFlowTarget.Cast(mgaSrc).SrcConnections.ValueFlowCollection)
            {
                result = result || HasOccurrencesOfKindInValueFlow(kinds, vf, ref iterDepth, breakOnProperty);
            }

            return result;
        }

        private static IEnumerable<RuleFeedbackBase> checkModelicaModelURI(GME.MGA.MgaFCO mgaModelicaModel)
        {
            var result = new List<RuleFeedbackBase>();

            string[] splitURI = null;

            string modelicaClass = mgaModelicaModel.GetStrAttrByNameDisp("Class");
            splitURI = modelicaClass.Split('.');

            int pathLength = splitURI.Length;

            // URI must have at least 2 classes, i.e., AttachedLib.Model, (3 might be better)
            if (splitURI.Length < 2)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("{0}: 'Class' attribute {1} should refer to a model within the attached package (path is too short or empty)",
                        mgaModelicaModel.Name,
                        modelicaClass)
                };

                feedback.InvolvedObjectsByRole.Add(mgaModelicaModel);
                result.Add(feedback);

                return result;
            }

            getExternalPackageNames(mgaModelicaModel);

            if (externalPackageNames.Contains(splitURI[0]) == false)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("{0}: the first element in the 'Class' attribute {1} should refer to an attached package",
                        mgaModelicaModel.Name,
                        modelicaClass)
                };

                feedback.InvolvedObjectsByRole.Add(mgaModelicaModel);
                result.Add(feedback);

                return result;
            }

            string badCharacters = null;

            foreach (string name in splitURI)
            {
                string additionalBadChars = checkForInvalidCharacters(name);
                if (string.IsNullOrEmpty(additionalBadChars) == false)
                {
                    badCharacters = badCharacters + additionalBadChars;
                }

                if (name.Length > 0 &&
                    char.IsDigit(name[0]))
                {
                    var feedback = new GenericRuleFeedback()
                    {
                        FeedbackType = FeedbackTypes.Error,
                        Message = string.Format(
                        "{0} has an invalid Class {1}; Classes must not begin with a digit",
                        mgaModelicaModel.Name,
                        modelicaClass)
                    };

                    feedback.InvolvedObjectsByRole.Add(mgaModelicaModel);
                    result.Add(feedback);

                    return result;
                }
            }

            if (badCharacters != null &&
                badCharacters != "")
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format(
                    "{0} - Class field contains invalid characters: {1}",
                    mgaModelicaModel.Name,
                    badCharacters)
                };

                feedback.InvolvedObjectsByRole.Add(mgaModelicaModel);
                result.Add(feedback);
            }

            return result;
        }

        private static List<RuleFeedbackBase> CheckFolderStructure(FolderTreeNode folder)
        {
            var result = new List<RuleFeedbackBase>();
            var invalidChars = checkForInvalidCharacters(folder.Name);
            var reservedName = checkForInvalidName(folder.Name);
            if (string.IsNullOrEmpty(invalidChars) == false ||
                string.IsNullOrEmpty(reservedName) == false)
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                };

                if (string.IsNullOrEmpty(invalidChars) == false)
                {
                    feedback.Message = string.Format("The component folder at '{0}' contains invalid characters '{1}'.",
                        folder.Folder.Path, invalidChars);
                }
                else
                {
                    feedback.Message = string.Format("The component folder at '{0}' has a protected name '{1}'.",
                        folder.Folder.Path, folder.Name);
                }
                result.Add(feedback);
            }

            if (string.IsNullOrEmpty(folder.Name))
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("The component folder at '{0}' has an empty name.", folder.Folder.Path)
                };

                result.Add(feedback);
            }
            else if (char.IsDigit(folder.Name.First()))
            {
                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("The component folder at '{0}' name starts with a digit.", folder.Folder.Path)
                };

                result.Add(feedback);
            }

            var names = folder.Children.Select(c => c.Name).ToList();
            var distinctNames = folder.Children.Select(c => c.Name).Distinct();

            if (names.Count() != distinctNames.Count())
            {
                foreach (var uniqueName in distinctNames)
                {
                    if (names.Contains(uniqueName))
                    {
                        names.Remove(uniqueName);
                    }
                }

                var feedback = new GenericRuleFeedback()
                {
                    FeedbackType = FeedbackTypes.Error,
                    Message = string.Format("The component folder at '{0}' contains components/component-folders with non-unique names: {1}.",
                        folder.Folder.Path, string.Join(", ", names))
                };

                result.Add(feedback);
            }

            foreach (var subFolder in folder.Children.Where(c => c is FolderTreeNode))
            {
                result.AddRange(CheckFolderStructure(subFolder as FolderTreeNode));
            }

            return result;
        }

        private static MgaFCO GetTestbench(MgaFCO context)
        {
            MgaFCO parent = null;

            if (context.ParentModel != null)
            {
                parent = (context.ParentModel) as MgaFCO;
            }

            if (parent != null &&
                parent.Meta.Name != typeof(CyPhy.TestBench).Name)
            {
                parent = GetTestbench(parent);
            }

            return parent;
        }

        private static List<RuleFeedbackBase> GetExceptionRuleFeedBack(Exception ex, IMgaFCO context)
        {
            var result = new List<RuleFeedbackBase>();

            var feedback = new GenericRuleFeedback<Exception>()
            {
                AdditionalInfo = ex,
                FeedbackType = FeedbackTypes.Error,
                Message = string.Format("Context: {0} - Exception: {1} {2}",
                    context.Name,
                    ex.Message,
                    ex.StackTrace)
            };

            feedback.InvolvedObjectsByRole.Add(context as IMgaFCO);
            result.Add(feedback);

            return result;
        }

        public static void TryToCallFormulaEvaluator(MgaFCO currentObj)
        {
            Type tFormulaEval = Type.GetTypeFromProgID("MGA.Interpreter.CyPhyFormulaEvaluator");
            var formulaEval = Activator.CreateInstance(tFormulaEval) as GME.MGA.IMgaComponentEx;
            Type tMgaFCOs = Type.GetTypeFromProgID("Mga.MgaFCOs");
            MgaFCOs selectedObjs = Activator.CreateInstance(tMgaFCOs) as MgaFCOs;

            formulaEval.Initialize(currentObj.Project);
            try
            {
                formulaEval.InvokeEx(currentObj.Project, currentObj, selectedObjs, 128);
            }
            catch (System.Runtime.InteropServices.COMException)
            {
                // FIXME: handle this exception properly
            }
        }

        public static CyPhyTypeFactory Factory { get; set; }
    }
}
