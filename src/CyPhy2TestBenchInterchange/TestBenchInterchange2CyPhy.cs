using avm;
using GME.MGA;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2TestBenchInterchange
{

    public class TestBenchInterchange2CyPhy
    {
        public static CyPhy.TestBenchType Convert(avm.TestBench avmTestBench, IMgaProject project)
        {
            if (avmTestBench == null)
                throw new ArgumentNullException("avmTestBench");

            var rootFolder = CyPhyClasses.RootFolder.GetRootFolder((MgaProject)project);
            var testBenchFolder = rootFolder.Children.TestingCollection.Where(d => d.Name == "TestBenches").FirstOrDefault();

            #region Create TestBenchesFolder
            
            if (testBenchFolder == null)
            {
                testBenchFolder = CyPhyClasses.Testing.Create(rootFolder);
                testBenchFolder.Name = "TestBenches";
            }

            #endregion

            #region Create TestBench

            var cyphyTestBench = CyPhyClasses.TestBench.Create(testBenchFolder);
            cyphyTestBench.Name = avmTestBench.Name;


            #endregion

            #region Create TopLevelSystemUnderTest

            var avmSystemUnderTest = avmTestBench.TopLevelSystemUnderTest;
            CyPhy.TopLevelSystemUnderTest cyphySystemUnderTest = null;
            TestBenchPrimitiveWrapper cyphySystemUnderTestReferred = null;
            if (avmSystemUnderTest != null)
            {
                cyphySystemUnderTest = CyPhyClasses.TopLevelSystemUnderTest.Create(cyphyTestBench);
                SetLayoutData(avmSystemUnderTest, cyphySystemUnderTest.Impl);
                if (!string.IsNullOrEmpty(avmSystemUnderTest.DesignID))
                {
                    // Looking for the referred component assembly
                    var allComponentAssembly = GetAllComponentAssemblies(rootFolder.Children.ComponentAssembliesCollection);
                    var componentAssembly = allComponentAssembly.FirstOrDefault(x=>x.Attributes.ConfigurationUniqueID == avmSystemUnderTest.DesignID);
                    if (componentAssembly != null)
                    {
                        cyphySystemUnderTest.GenericReferred = componentAssembly;
                        cyphySystemUnderTest.Name = componentAssembly.Name;
                        cyphySystemUnderTestReferred = new TestBenchPrimitiveWrapper(componentAssembly);
                    }
                }
            }

            #endregion

            #region Create Parameters

            var parameterCache = new List<Tuple<avm.Parameter, CyPhy.Parameter>>();
            foreach (var avmParameter in avmTestBench.Parameter)
            {
                var cyphyTestBenchParameter = CyPhyClasses.Parameter.Create(cyphyTestBench);
                cyphyTestBenchParameter.Name = avmParameter.Name;
                cyphyTestBenchParameter.Attributes.Description = avmParameter.Notes;
                cyphyTestBenchParameter.Attributes.ID = avmParameter.ID;
                SetLayoutData(avmParameter, cyphyTestBenchParameter.Impl);

                parameterCache.Add(new Tuple<avm.Parameter, CyPhy.Parameter>(avmParameter, cyphyTestBenchParameter));

                #region Set attribute values

                var avmValue = avmParameter.Value;
                if (avmValue != null)
                {
                    CyPhyClasses.Parameter.AttributesClass.DataType_enum cyphyDataType;

                    // TODO: else give a warning
                    if (ConvertCyPhyDataTypeEnum(avmValue.DataType, out cyphyDataType))
                    {
                        cyphyTestBenchParameter.Attributes.DataType = cyphyDataType;
                    }

                    cyphyTestBenchParameter.Attributes.Dimension = avmValue.Dimensions;

                    var avmValueExpression = avmValue.ValueExpression;
                    if (avmValueExpression != null && avmValueExpression is avm.ParametricValue)
                    {
                        var avmParametricValue = (avm.ParametricValue)avmValueExpression;
                        
                        var avmAssignedValue = avmParametricValue.AssignedValue as FixedValue;
                        var avmDefault = avmParametricValue.Default as FixedValue;
                        var avmMaximum = avmParametricValue.Maximum as FixedValue;
                        var avmMinimum = avmParametricValue.Minimum as FixedValue;

                        if (avmAssignedValue!=null && avmAssignedValue.Value!=null)
                            cyphyTestBenchParameter.Attributes.Value = avmAssignedValue.Value;
                        if (avmDefault!=null && avmDefault.Value!=null)
                            cyphyTestBenchParameter.Attributes.DefaultValue = avmDefault.Value;
                        
                        if (avmMinimum!=null && !string.IsNullOrEmpty(avmMinimum.Value) && avmMaximum!=null && !string.IsNullOrEmpty(avmMaximum.Value))
                            cyphyTestBenchParameter.Attributes.Range = String.Format("{0}..{1}",avmMinimum.Value,avmMaximum.Value);
                    }
                }

                #endregion
            }

            #endregion

            #region Create Metrics

            var metricCache = new List<Tuple<avm.Metric, CyPhy.Metric>>();
            foreach (var avmMetric in avmTestBench.Metric)
            {
                var cyphyTestBenchMetric = CyPhyClasses.Metric.Create(cyphyTestBench);

                cyphyTestBenchMetric.Name = avmMetric.Name;
                cyphyTestBenchMetric.Attributes.Description = avmMetric.Notes;
                SetLayoutData(avmMetric, cyphyTestBenchMetric.Impl);

                metricCache.Add(new Tuple<avm.Metric, CyPhy.Metric>(avmMetric, cyphyTestBenchMetric));
            }

            #endregion

            #region Create valueflows

            // Start with TopLevelSystemUnderTest
            if (avmSystemUnderTest != null && cyphySystemUnderTest != null)
            {
                /*
                // Parameters and properties
                foreach (var avmParameter in avmSystemUnderTest.PropertyInstance.Where(x=>x.Value!=null))
                {
                    // Target of valueflow
                    if (avmParameter.Value.ValueExpression is DerivedValue)
                    {
                        var derivedValue = (DerivedValue)avmParameter.Value.ValueExpression;
                        // Look for the parameter instance
                        dynamic cyphyValueflowSrc = cyphySystemUnderTestReferred.ParameterCollection.FirstOrDefault(x => x.Attributes.ID == avmParameter.IDinSourceModel);

                        // Maybe it is a property
                        if (cyphyValueflowSrc == null)
                        {
                            cyphyValueflowSrc = cyphySystemUnderTestReferred.PropertyCollection.FirstOrDefault(x => x.Attributes.ID == avmParameter.IDinSourceModel);
                        }

                        // TODO: Else warning
                        if (cyphyValueflowSrc != null)
                        {
                            var cachePair = parameterCache.FirstOrDefault(x=>x.Item1.Value.ID == derivedValue.ValueSource);
                            if (cachePair != null)
                            {
                                var cyphyValueflowDst = cachePair.Item2;
                                var srcRef = ((CyPhy.ValueFlowTarget)cyphyValueflowSrc).ParentContainer is CyPhyClasses.TestBench ? null : cyphySystemUnderTest;
                                var dstRef = ((CyPhy.ValueFlowTarget)cyphyValueflowDst).ParentContainer is CyPhyClasses.TestBench ? null : cyphySystemUnderTest;
                                // Doesnt work
                                CyPhyClasses.ValueFlow.Connect((CyPhy.ValueFlowTarget)cyphyValueflowSrc, (CyPhy.ValueFlowTarget)cyphyValueflowDst, srcRef, dstRef, cyphyTestBench);
                                //CyPhyClasses.ValueFlow.ConnectGeneric((CyPhy.ValueFlowTarget)cyphyValueflowDst, (CyPhy.ValueFlowTarget)cyphyValueflowSrc, null, null, cyphyTestBench);
                            }
                        }
                    }
                }
                 */ 
            }

            #endregion

            return cyphyTestBench;
        }

        #region Helpers

        //private static IEnumerable<CyPhy.ComponentAssembly> componentAssembliesCache = new List<CyPhy.ComponentAssembly>();
        private static IEnumerable<CyPhy.ComponentAssembly> GetAllComponentAssemblies(IEnumerable<CyPhy.ComponentAssemblies> folders)
        {
            var result = new List<CyPhy.ComponentAssembly>();

            foreach (var childFolder in folders)
            {
                result.AddRange(GetAllComponentAssemblies(childFolder.Children.ComponentAssembliesCollection));
                result.AddRange(childFolder.Children.ComponentAssemblyCollection);
            }

            return result;
        }

        #region DataTypeEnumConversion
        private static bool ConvertCyPhyDataTypeEnum(DataTypeEnum xDataType, out CyPhyClasses.Property.AttributesClass.DataType_enum cyphyDataType)
        {
            if (xDataType == DataTypeEnum.Real)
            {
                cyphyDataType = CyPhyClasses.Property.AttributesClass.DataType_enum.Float;
            }
            else
            {
                var valid = Enum.TryParse(xDataType.ToString(), true, out cyphyDataType);
                return valid;
            }
            return true;
        }

        private static bool ConvertCyPhyDataTypeEnum(DataTypeEnum xDataType, out CyPhyClasses.Parameter.AttributesClass.DataType_enum cyphyDataType)
        {
            if (xDataType == DataTypeEnum.Real)
            {
                cyphyDataType = CyPhyClasses.Parameter.AttributesClass.DataType_enum.Float;
            }
            else
            {
                var valid = Enum.TryParse(xDataType.ToString(), true, out cyphyDataType);
                return valid;
            }
            return true;
        }

        #endregion

        #region Layout

        public static bool SetLayoutData(object avmObj, IMgaObject cyphyObj)
        {
            if (cyphyObj is IMgaFCO)
            {
                var cyphyFCO = cyphyObj as IMgaFCO;
                if (cyphyFCO.ParentModel == null)
                {
                    return false;
                }
                var objWrapper = new LayoutDataWrapper(avmObj);

                if (objWrapper.hasLayoutData)
                {
                    // Look for all aspects, and set our layout data.
                    foreach (IMgaPart part in cyphyFCO.Parts)
                    {
                        part.SetGmeAttrs(null,
                            System.Convert.ToInt32(objWrapper.xpos),
                            System.Convert.ToInt32(objWrapper.ypos));

                    }
                    return true;
                }
            }
            return false;
        }

        #endregion

        #endregion

        //public IEnumerable<ISIS.GME.Common.Interfaces.Model> ImportFiles(string[] fileNames, IMgaProject project)
        //{
        //    var result = new List<ISIS.GME.Common.Interfaces.Model>();
        //    CyPhy.RootFolder rootFolder = ISIS.GME.Common.Utils.CreateObject<CyPhyClasses.RootFolder>(project.RootFolder as MgaObject);
        //    // Dictionary<string, CyPhy.Component> avmidComponentMap = CyPhyComponentImporterInterpreter.getCyPhyMLComponentDictionary_ByAVMID(rootFolder);

        //    foreach (var inputFilePath in fileNames)
        //    {
        //        var container = ImportFile(inputFilePath);
        //        result.Add(container);
        //    }
        //    return result;
        //}
    }
}
