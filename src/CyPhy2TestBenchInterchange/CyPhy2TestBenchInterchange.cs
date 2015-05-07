using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using avm;
using System.Diagnostics;
using GME.MGA;
using System.Text.RegularExpressions;
using Microsoft.CSharp.RuntimeBinder;

namespace CyPhy2TestBenchInterchange
{
    public class CyPhy2TestBenchInterchange
    {
        public static TestBench Convert(ISIS.GME.Dsml.CyPhyML.Interfaces.TestBench tb)
        {
            if (tb == null)
                throw new ArgumentNullException("tb");

            // TODO: Generate ID
            //var componentConnectorTargetMapping = new Dictionary<CyPhy.Connector, ComponentConnectorInstance>();
            var componentConnectorTargetMapping = new Dictionary<CyPhy.Connector, object>();
            
            //var componentValueflowTargetMapping = new Dictionary<CyPhy.ValueFlowTarget, ContainerPrimitivePropertyInstance>();
            // Note: its is object because it contains: ContainerPrimitivePropertyInstance AND ComponentPrimitivePropertyInstance (TestComponent)
            var componentValueflowTargetMapping = new Dictionary<CyPhy.ValueFlowTarget, object>();
            
            var vftIdCache = new Dictionary<CyPhy.ValueFlowTarget, String>();

            var avmTestBench = new TestBench { Name = tb.Name };

            #region Process ComponentAssembly | DesignContainer

            var systemUnderTest = tb.Children.TopLevelSystemUnderTestCollection.FirstOrDefault();
            if (systemUnderTest != null && systemUnderTest.Referred != null)
            {
                var wrapper = new TestBenchPrimitiveWrapper(systemUnderTest.AllReferred);
                if (wrapper.Primitive != null)
                {
                    var avmTopLevelSystemUnderTest = new TopLevelSystemUnderTest()
                    {
                        // Note: DesignContainers have no ConfigurationUniqueID
                        DesignID = wrapper.ConfigurationUniqueID,

                        // TODO: Check if null
                        IDinSourceModel = wrapper.ID.ToString()
                    };

                    SetLayoutData(avmTopLevelSystemUnderTest, systemUnderTest.Impl);

                    #region Process TopLevelSystemUnderTest parameters

                    foreach (var parameter in wrapper.ParameterCollection)
                    {
                        DataTypeEnum xDataType;
                        if (!ConvertCyPhyDataTypeEnum(parameter.Attributes.DataType, out xDataType)) continue;
                        /*
                        var avmPrimitivePropertyInstance = new ContainerPrimitivePropertyInstance()
                        {
                            IDinSourceModel = parameter.Attributes.ID.ToString(),
                            Value = new Value
                                {
                                    DataType = xDataType,
                                    DataTypeSpecified = true,
                                    DimensionType = DimensionTypeEnum.Scalar,
                                    DimensionTypeSpecified = true,
                                    Dimensions = parameter.Attributes.Dimension,
                                    ID = Guid.NewGuid().ToString("D"),
                                    //parameter.Guid.ToString("D")
                                }
                        };

                        //SetLayoutData(avmPrimitivePropertyInstance, parameter.Impl);

                        componentValueflowTargetMapping[parameter] = avmPrimitivePropertyInstance;
                        vftIdCache[parameter] = avmPrimitivePropertyInstance.Value.ID;
                        avmTopLevelSystemUnderTest.PropertyInstance.Add(avmPrimitivePropertyInstance);
                        */
                    }

                    #endregion

                    #region Process TopLevelSystemUnderTest properties

                    foreach (var property in wrapper.PropertyCollection)
                    {
                        DataTypeEnum xDataType;
                        if (!ConvertCyPhyDataTypeEnum(property.Attributes.DataType, out xDataType)) continue;
                        /*
                        var avmPrimitivePropertyInstance = new ContainerPrimitivePropertyInstance()
                        {
                            IDinSourceModel = property.Attributes.ID.ToString(),
                            Value = new Value
                            {
                                DataType = xDataType,
                                DataTypeSpecified = true,
                                DimensionType = DimensionTypeEnum.Scalar,
                                DimensionTypeSpecified = true,
                                Dimensions = property.Attributes.Dimension,
                                ID = Guid.NewGuid().ToString("D"), //property.Guid.ToString("D")
                            }
                        };

                        //SetLayoutData(avmPrimitivePropertyInstance, property.Impl);

                        avmTopLevelSystemUnderTest.PropertyInstance.Add(avmPrimitivePropertyInstance);
                        componentValueflowTargetMapping[property] = avmPrimitivePropertyInstance;
                        vftIdCache[property] = avmPrimitivePropertyInstance.Value.ID;
                        */
                    }

                    #endregion

                    #region Process TopLevelSystemUnderTest ModelicaConnector

                    
                    foreach (var mc in wrapper.ModelicaConnectorCollection)
                    {
                        /*
                        var avmPortInstance = new ContainerPortInstance
                        {
                            ID = string.IsNullOrEmpty(mc.Attributes.ID) ? GetOrSetID(mc) : mc.Attributes.ID,
                            NameInSourceModel = mc.Name,
                        };

                        modelicaConnectorCache[mc] = avmPortInstance;
                        avmTopLevelSystemUnderTest.PortInstance.Add(avmPortInstance);
                         */
                    }

                    #endregion

                    // TODO: Temprary solution for Zsolt no difference between ModelicaConnector and Connector in the atm file.
                    // => Impossible to import
                    #region Process TopLevelSystemUnderTest Connectors

                    foreach (var cc in wrapper.ConnectorCollection)
                    {
                        // Now it is ContainerPortInstance => Should be changed in the future.
                        /*
                        var avmPortInstance = new ContainerPortInstance
                        {
                            ID = EnsureComponentIdAttribute(cc),
                            NameInSourceModel = cc.Name,
                        };

                        componentConnectorTargetMapping[cc] = avmPortInstance;
                        avmTopLevelSystemUnderTest.PortInstance.Add(avmPortInstance);
                        */
                    }

                    #endregion

                    avmTestBench.TopLevelSystemUnderTest = avmTopLevelSystemUnderTest;

                   
                }
            }
            

            #endregion

            #region Process TestComponents

            foreach (var testComponent in tb.Children.TestComponentCollection)
            {
                var avmComponentInstance = new avm.ComponentInstance()
                {
                    Name = testComponent.Name,
                    ID = EnsureComponentIdAttribute(testComponent).ToString(),
                    ComponentID = testComponent.Guid.ToString("D")
                };

                #region Component connectors

                foreach (var connector in testComponent.Children.ConnectorCollection)
                {
                    var connectorId = EnsureComponentIdAttribute(connector);
                    var avmComponentConnectorInstance = new avm.ComponentConnectorInstance
                    {
                        IDinComponentModel = connector.Attributes.ID,
                        ID = avmComponentInstance.ID + '-' + connectorId,
                    };

                    avmComponentInstance.ConnectorInstance.Add(avmComponentConnectorInstance);

                    if (!componentConnectorTargetMapping.ContainsKey(connector))
                        componentConnectorTargetMapping[connector] = avmComponentConnectorInstance;
                }

                #endregion

                #region Process testComponent parameters

                foreach (var parameter in testComponent.Children.ParameterCollection)
                {
                    DataTypeEnum xDataType;
                    if (!ConvertCyPhyDataTypeEnum(parameter.Attributes.DataType, out xDataType)) continue;

                    var avmPrimitivePropertyInstance = new ComponentPrimitivePropertyInstance()
                    {
                        IDinComponentModel = parameter.Attributes.ID.ToString(),
                        Value = new Value
                        {
                            DataType = xDataType,
                            DataTypeSpecified = true,
                            DimensionType = DimensionTypeEnum.Scalar,
                            DimensionTypeSpecified = true,
                            Dimensions = parameter.Attributes.Dimension,
                            ID = Guid.NewGuid().ToString("D"),
                            //parameter.Guid.ToString("D")
                        }
                    };

                    //SetLayoutData(avmPrimitivePropertyInstance, parameter.Impl);

                    componentValueflowTargetMapping[parameter] = avmPrimitivePropertyInstance;
                    vftIdCache[parameter] = avmPrimitivePropertyInstance.Value.ID;
                    avmComponentInstance.PrimitivePropertyInstance.Add(avmPrimitivePropertyInstance);
                }

                #endregion

                #region Process testComponent properties

                foreach (var property in testComponent.Children.PropertyCollection)
                {
                    DataTypeEnum xDataType;
                    if (!ConvertCyPhyDataTypeEnum(property.Attributes.DataType, out xDataType)) continue;

                    var avmPrimitivePropertyInstance = new ComponentPrimitivePropertyInstance()
                    {
                        IDinComponentModel = property.Attributes.ID.ToString(),
                        Value = new Value
                        {
                            DataType = xDataType,
                            DataTypeSpecified = true,
                            DimensionType = DimensionTypeEnum.Scalar,
                            DimensionTypeSpecified = true,
                            Dimensions = property.Attributes.Dimension,
                            ID = Guid.NewGuid().ToString("D"), //property.Guid.ToString("D")
                        }
                    };

                    //SetLayoutData(avmPrimitivePropertyInstance, property.Impl);

                    avmComponentInstance.PrimitivePropertyInstance.Add(avmPrimitivePropertyInstance);
                    componentValueflowTargetMapping[property] = avmPrimitivePropertyInstance;
                    vftIdCache[property] = avmPrimitivePropertyInstance.Value.ID;
                }

                #endregion

                SetLayoutData(avmComponentInstance, testComponent.Impl);
                avmTestBench.TestComponent.Add(avmComponentInstance);
            }

            // TODO: not use dynamic, it is just a walkaround while Adam is on vacation
            #region Process ConnectorCompositions

            foreach (var cc in tb.Children.ConnectorCompositionCollection)
            {
                var src = cc.SrcEnds.Connector;
                var dst = cc.DstEnds.Connector;

                if (src != null && dst != null)
                {
                    dynamic avmSrc;
                    dynamic avmDst;
                    if (componentConnectorTargetMapping.ContainsKey(src) && componentConnectorTargetMapping.ContainsKey(dst))
                    {
                        avmSrc = componentConnectorTargetMapping[src];
                        avmDst = componentConnectorTargetMapping[dst];

                        try
                        {
                            avmSrc.ConnectorComposition.Add(avmDst.ID);
                        }
                        catch (RuntimeBinderException e)
                        {
                            avmSrc.PortMap.Add(avmDst.ID);
                        }
                    }
                }
            }

            #endregion

            #endregion

            #region Process Testbench parameters

            foreach (var parameter in tb.Children.ParameterCollection)
            {
                DataTypeEnum xDataType;
                if (!ConvertCyPhyDataTypeEnum(parameter.Attributes.DataType, out xDataType)) continue;

                var avmParameter = new Parameter
                {
                    ID = parameter.Attributes.ID,
                    Name = parameter.Name,
                    Notes = parameter.Attributes.Description,
                    Value = new Value
                    {
                        DataType = xDataType,
                        DataTypeSpecified = true,
                        DimensionType = DimensionTypeEnum.Scalar,
                        DimensionTypeSpecified = true,
                        Dimensions = parameter.Attributes.Dimension,
                        ID = Guid.NewGuid().ToString("D"),
                        //parameter.Guid.ToString("D")
                    }
                };

                SetLayoutData(avmParameter, parameter.Impl);

                #region Set value expressions / Derived values

                if (parameter.SrcConnections.ValueFlowCollection.Any())
                {
                    var vft = parameter.SrcConnections.ValueFlowCollection.First().SrcEnds.ValueFlowTarget;

                    String idVft = null;
                    // ContainerPrimitivePropertyInstance cppi = null;
                    dynamic cppi;
                    if (componentValueflowTargetMapping.TryGetValue(vft, out cppi))
                    {
                        idVft = cppi.Value.ID;
                    }
                    else
                    {
                        //idVft = GetOrSetID(vft);
                    }

                    avmParameter.Value.ValueExpression = new DerivedValue { ValueSource = idVft };
                }
                else
                {
                    // FixedValue or ParametricValue????
                    //var avmFixedValue = new FixedValue() {
                    //    Value = parameter.Attributes.Value
                    //};

                    var rangeParts = (parameter.Attributes.Range == null || !parameter.Attributes.Range.Contains("..")) ?
                                      new[] { "", "" } :
                                      parameter.Attributes.Range.Split(new[] { ".." }, StringSplitOptions.RemoveEmptyEntries);

                    if (rangeParts.Count() != 2) rangeParts = new[] { "", "" };

                    var avmParametricValue = new ParametricValue
                    {
                        AssignedValue = new FixedValue { Value = parameter.Attributes.Value },
                        Default = new FixedValue { Value = parameter.Attributes.DefaultValue },
                        Maximum = new FixedValue { Value = rangeParts[1] },
                        Minimum = new FixedValue { Value = rangeParts[0] },
                    };

                    avmParameter.Value.ValueExpression = avmParametricValue;
                    vftIdCache[parameter] = avmParameter.Value.ID;
                }

                #endregion

                avmTestBench.Parameter.Add(avmParameter);
            }

            #endregion

            #region Process Testbench metrics

            foreach (var metric in tb.Children.MetricCollection)
            {
                var avmMetric = new Metric
                {
                    Name = metric.Name,
                    Notes = metric.Attributes.Description
                };

                SetLayoutData(avmMetric, metric.Impl);

                #region Derived values

                // Metric is derived only
                if (metric.SrcConnections.ValueFlowCollection.Any())
                {
                    var vft = metric.SrcConnections.ValueFlowCollection.First().SrcEnds.ValueFlowTarget;

                    String idVft = null;
                    // ContainerPrimitivePropertyInstance cppi = null;
                    dynamic cppi;
                    if (componentValueflowTargetMapping.TryGetValue(vft, out cppi))
                    {
                        idVft = cppi.Value.ID;
                    }
                    else
                    {
                        //idVft = GetOrSetID(vft);
                    }

                    avmMetric.Value.ValueExpression = new DerivedValue { ValueSource = idVft };
                }

                #endregion

                avmTestBench.Metric.Add(avmMetric);
            }

            #endregion

            #region Process Testbench Environments (modelica models)

            foreach (var env in tb.Children.EnvironmentCollection)
            {
                var avmModelicaModel = new avm.modelica.ModelicaModel
                {
                    Name = env.Name,
                    Author = env.Attributes.Author,
                    Class = env.Attributes.Class,
                    Notes = env.Attributes.Notes,
                    UsesResource = env.Attributes.FilePathWithinResource,
                };

                #region Process modelica connectors

                foreach (var mc in env.Children.ModelicaConnectorCollection)
                {
                    var avmModelicaConnector = new avm.modelica.Connector
                    {
                        Name = mc.Name,
                        Class = mc.Attributes.Class,
                        Definition = mc.Attributes.Definition,
                        Notes = mc.Attributes.DefinitionNotes,
                        ID = string.IsNullOrEmpty(mc.Attributes.ID) ? GetOrSetID(mc) : mc.Attributes.ID,
                        //= mc.Attributes.InstanceNotes,
                        Locator = mc.Attributes.Locator,
                    };

                    avmModelicaModel.Connector.Add(avmModelicaConnector);
                    SetLayoutData(avmModelicaConnector, mc.Impl);
                }

                #endregion

                avmTestBench.TestStructure.Add(avmModelicaModel);
                SetLayoutData(avmModelicaModel, env.Impl);
            }

            #endregion

            #region Process Valueflows/set valueexpressions

            // Note: dynamic because of the Testcomponent Properties/Parameters
            foreach (var vf in tb.Children.ValueFlowCollection)
            {
                var srcValueFlow = vf.SrcEnds.ValueFlowTarget;
                var dstValueFlow = vf.DstEnds.ValueFlowTarget;

                if (componentValueflowTargetMapping.ContainsKey(dstValueFlow))
                {
                    dynamic dstPair = componentValueflowTargetMapping[dstValueFlow];
                    dstPair.Value.ValueExpression = new avm.DerivedValue
                    {
                        ValueSource = vftIdCache[srcValueFlow]
                    };
                }
            }

            #endregion

            #region Process PortCompositions

            foreach (var pc in tb.Children.PortCompositionCollection)
            {
                var src = pc.SrcEnds.ModelicaConnector;
                var dst = pc.DstEnds.ModelicaConnector;
                /*
                if (src != null & dst != null)
                {
                    ContainerPortInstance avmSrc;
                    ContainerPortInstance avmDst;
                    if (modelicaConnectorCache.ContainsKey(src))
                    {
                        avmSrc = modelicaConnectorCache[src];
                        avmSrc.PortMap.Add(dst.Attributes.ID);
                    }
                    else if (modelicaConnectorCache.ContainsKey(dst))
                    {
                        avmDst = modelicaConnectorCache[dst];
                        avmDst.PortMap.Add(src.Attributes.ID);
                    }
                }
                */
            }

            #endregion

            #region Process Workflows

            var workflowRef = tb.Children.WorkflowRefCollection.FirstOrDefault();

            if (workflowRef != null)
            {
                var referred = workflowRef.Referred;
                if (referred != null && referred!=null)
                {
                    var workflow = referred.Workflow;

                    var avmWorkflow = new Workflow
                    {
                        Name = workflow.Name,
                    };

                    avmTestBench.Workflow = avmWorkflow;
                    SetLayoutData(avmWorkflow, workflowRef.Impl);

                    var allTasks = workflow.Children.TaskBaseCollection.ToList();

                    // Get the first task
                    var currentTask = allTasks.FirstOrDefault(x => !x.SrcConnections.FlowCollection.Any());
                    if (currentTask != null)
                    {
                        do
                        {
                            // Create avm workflow tasks
                            if (currentTask is CyPhy.Task)
                            {
                                var cyphyTask = (CyPhy.Task)currentTask;
                                var avmTask = new InterpreterTask
                                {
                                    Name = cyphyTask.Name,
                                    COMName = cyphyTask.Attributes.COMName,
                                    Parameters = cyphyTask.Attributes.Parameters
                                };

                                avmWorkflow.Task.Add(avmTask);
                                SetLayoutData(avmTask, cyphyTask.Impl);
                            }
                            else if (currentTask is CyPhy.ExecutionTask)
                            {
                                var cyphyTask = (CyPhy.ExecutionTask)currentTask;
                                var avmTask = new ExecutionTask
                                {
                                    Name = cyphyTask.Name,
                                    Description = cyphyTask.Attributes.Description,
                                    Invocation = cyphyTask.Attributes.Invocation,
                                    //TODO: Ask adam about these:
                                    // = cyphyTask.Attributes.Parameters
                                    // = cyphyTask.Attributes.PostProcess
                                    // = cyphyTask.Attributes.PreProcess
                                };

                                SetLayoutData(avmTask, cyphyTask.Impl);
                                avmWorkflow.Task.Add(avmTask);
                            }
                            else
                            {
                                // ToDo: Give a warning
                                break;
                            }

                            // Next edge
                            var nextFlow = currentTask.DstConnections.FlowCollection.FirstOrDefault();

                            // No more outgoing edge
                            if (nextFlow == null) break;
                            currentTask = nextFlow.DstEnds.TaskBase;

                            // No element at the end of the edge
                            // ToDo: Give a warning
                        } while (currentTask != null);
                    }
                }
            }

            #endregion

            #region Process Settings

            foreach (var settings in tb.Children.SolverSettingsCollection)
            {
                var intervalMethod = AvmModelicaIntervalMethodMapping.FirstOrDefault(x => x.Item1 == settings.Attributes.IntervalMethod);
                var jobManagerToolSelection = AvmModelicaJobManagerToolSelectionMapping.FirstOrDefault(x => x.Item1 == settings.Attributes.JobManagerToolSelection);

                var avmSettings = new avm.modelica.SolverSettings
                {
                    IntervalLength = settings.Attributes.IntervalLength,
                    IntervalLengthSpecified = true,
                    
                    // TODO: Give warning
                    IntervalMethod = intervalMethod!=null?intervalMethod.Item2:avm.modelica.IntervalMethod.IntervalLength,
                    
                    // TODO: Give warning
                    JobManagerToolSelection = jobManagerToolSelection!=null?jobManagerToolSelection.Item2:avm.modelica.JobManagerToolSelection.Dymola_latest,
                    
                    JobManagerToolSelectionSpecified = true,
                    NumberOfIntervals = settings.Attributes.NumberOfIntervals,
                    NumberOfIntervalsSpecified = true,
                    Solver = settings.Attributes.Solver.ToString(),
                    StartTime = settings.Attributes.StartTime,
                    StartTimeSpecified = true,
                    StopTime = settings.Attributes.StartTime,
                    StopTimeSpecified = true,
                    Tolerance = settings.Attributes.Tolerance,
                    ToleranceSpecified = true,
                    ToolSpecificAnnotations = settings.Attributes.ToolSpecificAnnotations
                };

                avmTestBench.Settings.Add(avmSettings);
            }

            #endregion

            return avmTestBench;
        }

        #region Helper functions

        #region Enum conversions

        #region Conversion tables

        private static List<Tuple<CyPhyClasses.SolverSettings.AttributesClass.IntervalMethod_enum, avm.modelica.IntervalMethod>> AvmModelicaIntervalMethodMapping =
            new List<Tuple<CyPhyClasses.SolverSettings.AttributesClass.IntervalMethod_enum, avm.modelica.IntervalMethod>>
        {
            
                new Tuple<CyPhyClasses.SolverSettings.AttributesClass.IntervalMethod_enum, avm.modelica.IntervalMethod>(CyPhyClasses.SolverSettings.AttributesClass.IntervalMethod_enum.Interval_Length, avm.modelica.IntervalMethod.IntervalLength),
                new Tuple<CyPhyClasses.SolverSettings.AttributesClass.IntervalMethod_enum, avm.modelica.IntervalMethod>(CyPhyClasses.SolverSettings.AttributesClass.IntervalMethod_enum.Number_of_Intervals, avm.modelica.IntervalMethod.NumberOfIntervals)
        };

        private static List<Tuple<CyPhyClasses.SolverSettings.AttributesClass.JobManagerToolSelection_enum, avm.modelica.JobManagerToolSelection>> AvmModelicaJobManagerToolSelectionMapping =
            new List<Tuple<CyPhyClasses.SolverSettings.AttributesClass.JobManagerToolSelection_enum, avm.modelica.JobManagerToolSelection>>
        {
                new Tuple<CyPhyClasses.SolverSettings.AttributesClass.JobManagerToolSelection_enum, avm.modelica.JobManagerToolSelection>(CyPhyClasses.SolverSettings.AttributesClass.JobManagerToolSelection_enum.Dymola__latest_, avm.modelica.JobManagerToolSelection.Dymola_latest),
                new Tuple<CyPhyClasses.SolverSettings.AttributesClass.JobManagerToolSelection_enum, avm.modelica.JobManagerToolSelection>(CyPhyClasses.SolverSettings.AttributesClass.JobManagerToolSelection_enum.Dymola_2013, avm.modelica.JobManagerToolSelection.Dymola_2013),
                new Tuple<CyPhyClasses.SolverSettings.AttributesClass.JobManagerToolSelection_enum, avm.modelica.JobManagerToolSelection>(CyPhyClasses.SolverSettings.AttributesClass.JobManagerToolSelection_enum.Dymola_2014, avm.modelica.JobManagerToolSelection.Dymola_2014),
                new Tuple<CyPhyClasses.SolverSettings.AttributesClass.JobManagerToolSelection_enum, avm.modelica.JobManagerToolSelection>(CyPhyClasses.SolverSettings.AttributesClass.JobManagerToolSelection_enum.JModelica_1_dot_12, avm.modelica.JobManagerToolSelection.JModelica_1_12),
                new Tuple<CyPhyClasses.SolverSettings.AttributesClass.JobManagerToolSelection_enum, avm.modelica.JobManagerToolSelection>(CyPhyClasses.SolverSettings.AttributesClass.JobManagerToolSelection_enum.OpenModelica__latest_, avm.modelica.JobManagerToolSelection.OpenModelica_latest)
        };

        #endregion

        #region DataTypeEnumConversion
        private static bool ConvertCyPhyDataTypeEnum(CyPhyClasses.Property.AttributesClass.DataType_enum cyphyDataType, out DataTypeEnum xDataType)
        {
            if (cyphyDataType == CyPhyClasses.Property.AttributesClass.DataType_enum.Float)
            {
                xDataType = DataTypeEnum.Real;
            }
            else
            {
                var valid = Enum.TryParse(cyphyDataType.ToString(), true, out xDataType);
                return valid;
            }
            return true;
        }

        private static bool ConvertCyPhyDataTypeEnum(CyPhyClasses.Parameter.AttributesClass.DataType_enum cyphyDataType, out DataTypeEnum xDataType)
        {
            if (cyphyDataType == CyPhyClasses.Parameter.AttributesClass.DataType_enum.Float)
            {
                xDataType = DataTypeEnum.Real;
            }
            else
            {
                var valid = Enum.TryParse(cyphyDataType.ToString(), true, out xDataType);
                return valid;
            }
            return true;
        }

        #endregion

        #endregion

        #region Get/Set ID

        private static string GetOrSetID(CyPhy.ValueFlowTarget vft)
        {
            var id = GetIdAttribute(vft);

            if (string.IsNullOrWhiteSpace(id))
            {
                id = vft.Guid.ToString("D");
                SetIdAttributeString(vft, id);
            }

            return id;
        }

        private static string GetOrSetID(CyPhy.ModelicaConnector mc)
        {
            var id = GetIdAttribute(mc);

            if (string.IsNullOrWhiteSpace(id))
            {
                id = mc.Guid.ToString("D");
                SetIdAttributeString(mc, id);
            }

            return id;
        }

        private static Type GetIdAttributeType(object cyphyObject)
        {
            var cyPhyMLAttributesPropertyInfo = GetPropertyInfo(GetInterfaceType(cyphyObject), "Attributes");
            if (cyPhyMLAttributesPropertyInfo != null)
            {
                var cyPhyMLIDPropertyInfo = cyPhyMLAttributesPropertyInfo.PropertyType.GetProperty("ID");
                return cyPhyMLIDPropertyInfo.PropertyType;
            }
            return null;
        }

        private static string GetIdAttribute(object cyPhyObject)
        {
            var id = string.Empty;

            var cyPhyMLAttributesPropertyInfo = GetPropertyInfo(GetInterfaceType(cyPhyObject), "Attributes");
            if (cyPhyMLAttributesPropertyInfo != null)
            {
                var cyPhyMLIDPropertyInfo = cyPhyMLAttributesPropertyInfo.PropertyType.GetProperty("ID");
                if (cyPhyMLIDPropertyInfo != null)
                {
                    id = cyPhyMLIDPropertyInfo.GetValue(cyPhyMLAttributesPropertyInfo.GetValue(cyPhyObject, null), null).ToString();
                }
            }

            return id;
        }

        private static void SetIdAttributeString(object cyPhyObject, string id)
        {
            var cyPhyMLAttributesPropertyInfo = GetPropertyInfo(GetInterfaceType(cyPhyObject), "Attributes");
            if (cyPhyMLAttributesPropertyInfo != null)
            {
                var cyPhyMLIDPropertyInfo = cyPhyMLAttributesPropertyInfo.PropertyType.GetProperty("ID");
                if (cyPhyMLIDPropertyInfo != null)
                {
                    cyPhyMLIDPropertyInfo.SetValue(cyPhyMLAttributesPropertyInfo.GetValue(cyPhyObject, null), id, null);
                }
            }
        }

        private static void SetIdAttributeInt(object cyPhyObject, int id)
        {
            var cyPhyMLAttributesPropertyInfo = GetPropertyInfo(GetInterfaceType(cyPhyObject), "Attributes");
            if (cyPhyMLAttributesPropertyInfo != null)
            {
                var cyPhyMLIDPropertyInfo = cyPhyMLAttributesPropertyInfo.PropertyType.GetProperty("ID");
                if (cyPhyMLIDPropertyInfo != null)
                {
                    cyPhyMLIDPropertyInfo.SetValue(cyPhyMLAttributesPropertyInfo.GetValue(cyPhyObject, null), id, null);
                }
            }
        }

        #endregion

        #region Properties by reflection

        private static System.Reflection.PropertyInfo GetPropertyInfo(Type type, string propertyName)
        {
            return type.GetProperty(propertyName);
        }

        private static System.Reflection.PropertyInfo GetPropertyInfo(object object_var, string propertyName)
        {
            return GetPropertyInfo(object_var.GetType(), propertyName);
        }

        private static Type GetInterfaceType(Type type)
        {
            string typeName = type.UnderlyingSystemType.AssemblyQualifiedName.Replace(".Classes.", ".Interfaces.");
            return Type.GetType(typeName);
        }

        private static Type GetInterfaceType(object object_var)
        {
            return GetInterfaceType(object_var.GetType());
        }

        #endregion

        #region Layout helper

        private static void SetLayoutData(object avmObj, IMgaObject cyphyObj)
        {
            if (cyphyObj is IMgaFCO)
            {
                var cyphyFCO = cyphyObj as IMgaFCO;

                var objWrapper = new LayoutDataWrapper(avmObj);

                // Look for layout data associated with the All aspect.
                // If we find that aspect, set all the values.
                // Otherwise, we expect that avmObj's positional values will remain unset.
                Boolean hasAllAspect = false;
                foreach (IMgaPart part in cyphyFCO.Parts)
                {
                    if (part.MetaAspect.Name == "All")
                    {
                        hasAllAspect = true;
                        break;
                    }
                }

                foreach (IMgaPart part in cyphyFCO.Parts)
                {
                    if (part.MetaAspect.Name == "All" || hasAllAspect == false)
                    {
                        String icon;
                        int xpos = 0;
                        int ypos = 0;
                        part.GetGmeAttrs(out icon, out xpos, out ypos);

                        if (xpos >= 0 && ypos >= 0)
                        {
                            objWrapper.xpos = System.Convert.ToUInt32(xpos);
                            objWrapper.ypos = System.Convert.ToUInt32(ypos);
                        }

                        //  We got what we came for. Terminate the loop.
                        break;
                    }
                }
            }
        }

        #endregion

        private static Dictionary<Type, HashSet<string>> testComponentIdCache = new Dictionary<Type, HashSet<string>>();
        private static string EnsureComponentIdAttribute(object cyphyObject)
        {
            var type = cyphyObject.GetType();

            // Yeah baby: string or int... pfff... what a messsss! grrr
            var attributeIdType = GetIdAttributeType(cyphyObject);
            if (!testComponentIdCache.ContainsKey(type))
                testComponentIdCache[type] = new HashSet<string>();

            if (attributeIdType == typeof(int))
            {
                var id = 0;
                if (!int.TryParse(GetIdAttribute(cyphyObject), out id) || id == 0)
                {
                    do
                    {
                        id = new Random().Next(1000, 30000);
                    }
                    while (testComponentIdCache[type].Contains(id.ToString()));

                    testComponentIdCache[type].Add(id.ToString());
                    SetIdAttributeInt(cyphyObject, id);
                }
                else
                    testComponentIdCache[type].Add(id.ToString());

                return id.ToString();
            }
            else if (attributeIdType == typeof(string))
            {
                var id = GetIdAttribute(cyphyObject);
                if (string.IsNullOrEmpty(id))
                {
                    do
                    {
                        id = (new Random().Next(1000, 30000)).ToString();
                    }
                    while (testComponentIdCache[type].Contains(id));

                    testComponentIdCache[type].Add(id);
                    SetIdAttributeString(cyphyObject, id);
                }
                else
                    testComponentIdCache[type].Add(id);
                return id;
            }

            return string.Empty;
        }

        private static bool IsValidXmlId(String id)
        {
            // FIXME this is wrong
            // NameStartChar	   ::=   	":" | [A-Z] | "_" | [a-z] | [#xC0-#xD6] | [#xD8-#xF6] | [#xF8-#x2FF] | [#x370-#x37D] | [#x37F-#x1FFF] | [#x200C-#x200D] | [#x2070-#x218F] | [#x2C00-#x2FEF] | [#x3001-#xD7FF] | [#xF900-#xFDCF] | [#xFDF0-#xFFFD] | [#x10000-#xEFFFF]
            // NameChar	   ::=   	 NameStartChar | "-" | "." | [0-9] | #xB7 | [#x0300-#x036F] | [#x203F-#x2040]
            // NCNameChar	   ::=   	NameChar - ':'
            // NCNameStartChar	   ::=   	NameStartChar - ':'
            string pattern = @"^[a-zA-Z_][\w.-]*$";
            return Regex.IsMatch(id, pattern);
        }

        #endregion
    }
}
