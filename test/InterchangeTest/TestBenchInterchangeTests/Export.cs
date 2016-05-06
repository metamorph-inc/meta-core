using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Xunit;
using GME.MGA;
using GME.CSharp;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using System.Text.RegularExpressions;

namespace TestBenchInterchangeTests
{
        public class ExportModel : IUseFixture<ExportModelFixture>
        {
            private MgaProject proj { get { return fixture.proj; } }

            #region Fixture
            private ExportModelFixture fixture;
            public void SetFixture(ExportModelFixture data)
            {
                fixture = data;
            }
            #endregion

            private void PerformInTransaction(MgaGateway.voidDelegate del)
            {
                var mgaGateway = new MgaGateway(proj);
                mgaGateway.PerformInTransaction(del, abort: false);
            }

            private CyPhy.TestBench GetTestBenchByName(String name)
            {
                MgaFilter filter = proj.CreateFilter();
                filter.Kind = "TestBench";
                filter.Name = name;

                return proj.AllFCOs(filter)
                           .Cast<MgaFCO>()
                           .Select(x => CyPhyClasses.TestBench.Cast(x))
                           .Cast<CyPhy.TestBench>()
                           .FirstOrDefault();
            }

            [Fact]
            public void SimpleModel()
            {
                avm.TestBench tb = null;
                PerformInTransaction(delegate
                {
                    var cyphyTB = GetTestBenchByName("Simple");
                    tb = CyPhy2TestBenchInterchange.CyPhy2TestBenchInterchange.Convert(cyphyTB);

                });

                Assert.Equal("Simple", tb.Name);

                #region check top level system under test
                var tlsut = tb.TopLevelSystemUnderTest;
                Assert.NotNull(tlsut);

                Assert.Equal("simple-123", tlsut.DesignID);
                Assert.Equal("1", tlsut.IDinSourceModel);

                /*
                 Assert.Equal(2, tlsut.PropertyInstance.Count);

                var dParam = tlsut.PropertyInstance.FirstOrDefault(pi => pi.IDinSourceModel.Equals("designparam1"));
                Assert.NotNull(dParam);
                Assert.NotNull(dParam.Value);
                Assert.NotNull(dParam.Value.ValueExpression);
                Assert.IsType<avm.DerivedValue>(dParam.Value.ValueExpression);

                var dProp = tlsut.PropertyInstance.FirstOrDefault(pi => pi.IDinSourceModel.Equals("designprop1"));
                Assert.NotNull(dProp);
                Assert.NotNull(dProp.Value);
                var dPropID = dProp.Value.ID;
                 */
                #endregion

                #region check TB parameter
                var tbParam = tb.Parameter.FirstOrDefault();
                Assert.NotNull(tbParam);
                Assert.NotNull(tbParam.ID);
                Assert.Equal("Parameter", tbParam.Name);
                Assert.Equal("notes", tbParam.Notes);
                Assert.True(tbParam.XPositionSpecified);
                Assert.True(tbParam.YPositionSpecified);

                var tbParamValue = tbParam.Value;
                Assert.NotNull(tbParamValue);
                // NOTE: no... shouldn't be...
                //Assert.Equal("param1", tbParamValue.ID);
                //
                Assert.IsType<avm.ParametricValue>(tbParamValue.ValueExpression);
                var tbParamVE = tbParamValue.ValueExpression as avm.ParametricValue;

                AssertFixedValue("1", tbParamVE.AssignedValue);
                AssertFixedValue("2", tbParamVE.Default);
                AssertFixedValue("-inf", tbParamVE.Minimum);
                AssertFixedValue("inf", tbParamVE.Maximum);
                #endregion

                #region check TB metric
                var tbMetric = tb.Metric.FirstOrDefault();
                Assert.NotNull(tbMetric);
                Assert.Equal("notes", tbMetric.Notes);
                Assert.True(tbMetric.XPositionSpecified);
                Assert.True(tbMetric.YPositionSpecified);

                var tbMetricValue = tbMetric.Value;
                Assert.NotNull(tbMetricValue);
                Assert.IsType<avm.DerivedValue>(tbMetricValue.ValueExpression);
                var tbMetricVE = tbMetricValue.ValueExpression;
                Assert.IsType<avm.DerivedValue>(tbMetricVE);
                #endregion

                /*
                // Check connection from design to Metric
                AssertValueFlow(dProp.Value, tbMetric.Value);
                // Check connection from Parameter to design
                AssertValueFlow(tbParam.Value, dParam.Value);
                 */
            }

            [Fact]
            public void Workflow()
            {
                avm.TestBench tb = null;
                PerformInTransaction(delegate
                {
                    var cyphyTB = GetTestBenchByName("Workflow");
                    tb = CyPhy2TestBenchInterchange.CyPhy2TestBenchInterchange.Convert(cyphyTB);
                });

                Assert.Equal("Workflow", tb.Name);

                var workflow = CheckWorkflow(tb);
                Assert.Equal(2, workflow.Task.Count);

                var task1 = workflow.Task[0];
                Assert.IsType<avm.InterpreterTask>(task1);
                var iTask = task1 as avm.InterpreterTask;
                Assert.Equal("Task", iTask.Name);
                Assert.Equal("MGA.Interpreter.CyPhy2Modelica_v2", iTask.COMName);
                Assert.Equal("{\"simulationTargetTool\":\"Dymola\",\"AnalysisTool\":\"Default\"}", Regex.Replace(iTask.Parameters, @"\s+", ""));

                var task2 = workflow.Task[1];
                Assert.IsType<avm.ExecutionTask>(task2);
                var eTask = task2 as avm.ExecutionTask;
                Assert.Equal("ExecutionTask", eTask.Name);
                Assert.Equal("This is a description", eTask.Description);
                Assert.Equal("dir *.*", eTask.Invocation);
            }

            [Fact]
            public void Modelica()
            {
                avm.TestBench tb = null;
                PerformInTransaction(delegate
                {
                    var cyphyTB = GetTestBenchByName("Modelica");
                    tb = CyPhy2TestBenchInterchange.CyPhy2TestBenchInterchange.Convert(cyphyTB);
                });

                Assert.Equal("Modelica", tb.Name);

                #region check workflow
                var workflow = CheckWorkflow(tb);
                Assert.Equal(1, workflow.Task.Count);

                var task = workflow.Task[0];
                Assert.IsType<avm.InterpreterTask>(task);
                var iTask = task as avm.InterpreterTask;
                Assert.Equal("Task", iTask.Name);
                Assert.Equal("MGA.Interpreter.CyPhy2Modelica_v2", iTask.COMName);
                Assert.Equal("{\"simulationTargetTool\":\"Dymola\",\"AnalysisTool\":\"Default\"}", Regex.Replace(iTask.Parameters, @"\s+", ""));
                #endregion

                #region check system under test
                Assert.NotNull(tb.TopLevelSystemUnderTest);
                var sut = tb.TopLevelSystemUnderTest;
                Assert.Equal("idmodelicadesign", sut.DesignID);
                Assert.Equal("1", sut.IDinSourceModel);
                Assert.True(sut.XPositionSpecified);
                Assert.True(sut.YPositionSpecified);

                /*
                Assert.Equal(2, sut.PortInstance.Count);

                var sutMC1 = sut.PortInstance.FirstOrDefault(pi => pi.NameInSourceModel.Equals("MC1"));
                Assert.NotNull(sutMC1);

                var sutMC2 = sut.PortInstance.FirstOrDefault(pi => pi.NameInSourceModel.Equals("MC2"));
                Assert.NotNull(sutMC2);
                 */
                #endregion

                #region check environments
                Assert.Equal(2, tb.TestStructure.Count);
                Assert.Equal(2, tb.TestStructure.OfType<avm.modelica.ModelicaModel>()
                                                .Count());

                var env1 = tb.TestStructure.OfType<avm.modelica.ModelicaModel>()
                                           .FirstOrDefault(mm => mm.Name.Equals("Environment"));
                Assert.NotNull(env1);
                Assert.Equal("author", env1.Author);
                Assert.Equal("notes", env1.Notes);
                Assert.Equal("someenvironment", env1.Class);

                Assert.Equal(2, env1.Connector.Count);

                var env1Conn1 = env1.Connector.FirstOrDefault(mc => mc.Name.Equals("MC1"));
                Assert.NotNull(env1Conn1);
                Assert.Equal("someconnectorclass", env1Conn1.Class);
                Assert.Equal("mc1", env1Conn1.Locator);

                var env1Conn2 = env1.Connector.FirstOrDefault(mc => mc.Name.Equals("MC2"));
                Assert.NotNull(env1Conn2);
                Assert.Equal("someconnectorclass", env1Conn2.Class);
                Assert.Equal("mc2", env1Conn2.Locator);


                var env2 = tb.TestStructure.OfType<avm.modelica.ModelicaModel>()
                                           .FirstOrDefault(mm => mm.Name.Equals("Environment2"));
                Assert.NotNull(env2);
                Assert.Equal("author", env2.Author);
                Assert.Equal("notes", env2.Notes);
                Assert.Equal("someenvironment2", env2.Class);

                Assert.Equal(1, env2.Connector.Count);

                var env2Conn = env2.Connector.FirstOrDefault(mc => mc.Name.Equals("MCA"));
                Assert.NotNull(env2Conn);
                Assert.Equal("someconnectorclass", env2Conn.Class);
                Assert.Equal("mca", env2Conn.Locator);
                #endregion

                /*
                #region check PortComposition
                AssertPortComposition(sutMC1, env1Conn1);
                AssertPortComposition(sutMC2, env1Conn2);
                AssertPortComposition(sutMC2, env2Conn);
                #endregion
                 */
            }

            private static void AssertFixedValue(String expected, avm.ValueExpressionType tbParamVE)
            {
                Assert.NotNull(tbParamVE);
                Assert.IsType<avm.FixedValue>(tbParamVE);
                Assert.Equal(expected, (tbParamVE as avm.FixedValue).Value);
            }

            private static void AssertValueFlow(avm.Value src, avm.Value dst)
            {
                Assert.IsType<avm.DerivedValue>(dst.ValueExpression);
                var dstVE = dst.ValueExpression as avm.DerivedValue;
                Assert.Equal(src.ID, dstVE.ValueSource);
            }

            private static void AssertPortComposition(avm.PortMapTarget p1, avm.PortMapTarget p2)
            {
                Assert.True(p1.PortMap.Contains(p2.ID) ||
                            p2.PortMap.Contains(p1.ID));
            }

            private avm.Workflow CheckWorkflow(avm.TestBench tb)
            {
                Assert.NotNull(tb.Workflow);
                Assert.NotEmpty(tb.Workflow.Task);

                return tb.Workflow;
            }
        }

    public class ExportRoundTrip : TestBenchInterchangeTests.TBRoundTripBase, IUseFixture<ExportModelFixture>
    {
        [Fact(Skip="not implemented")]
        public void Simple()
        {
            RunRoundTrip("Simple");
        }

        [Fact]
        public void Simple_NoValueFlow()
        {
            RunRoundTrip("Simple_NoValueFlow");
        }

        [Fact(Skip="not implemented")]
        public void Modelica()
        {
            RunRoundTrip("Modelica");
        }

        [Fact(Skip = "not implemented")]
        public void Workflow()
        {
            RunRoundTrip("Workflow");
        }

        public override MgaProject proj { get { return fixture.proj; } }
        public override string AtmPath { get { return fixture.testPath; } }

        private ExportModelFixture fixture;
        public void SetFixture(ExportModelFixture data)
        {
            fixture = data;
        }
    }

    public class ExportModelFixture : GMEProjectFixtureBase
    {
        public override String xmePath
        {
            get
            {
                return Path.Combine(META.VersionInfo.MetaPath,
                                    "test",
                                    "InterchangeTest",
                                    "TestBenchInterchangeTests",
                                    "models",
                                    "TestBench.xme");
            }
        }
    }

}
