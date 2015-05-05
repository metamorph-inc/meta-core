using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Xunit;
using GME.MGA;
using System.Runtime.CompilerServices;
using System.Diagnostics;

namespace ComponentInterchangeTest
{
    public class CyberModelFixture : ComponentInterchangeTestFixtureBase
    {
        public override String xmePath
        {
            get
            {
                return Path.Combine(META.VersionInfo.MetaPath,
                                    "test",
                                    "InterchangeTest",
                                    "ComponentInterchangeTest",
                                    "SharedModels",
                                    "CyberModel",
                                    "CyberController.xme");
            }
        }
    }

    public class CyberModel : IUseFixture<CyberModelFixture>
    {
        [MethodImpl(MethodImplOptions.NoInlining)]
        private string GetCurrentMethod()
        {
            StackTrace st = new StackTrace();
            StackFrame sf = st.GetFrame(1);

            return sf.GetMethod().Name;
        }

        private MgaProject proj { get { return fixture.proj; } }

        #region Fixture
        private CyberModelFixture fixture;
        public void SetFixture(CyberModelFixture data)
        {
            fixture = data;
        }
        #endregion

        [Fact]
        public void Export()
        {
            avm.Component comp = null;
            proj.PerformInTransaction(delegate
            {
                var cyphyComp = proj.GetComponentsByName("Excavator_controller").First();
                comp = CyPhy2ComponentModel.Convert.CyPhyML2AVMComponent(cyphyComp);
            });

            String testName = GetCurrentMethod();
            CyPhyComponentExporter.CyPhyComponentExporterInterpreter
                                  .SerializeAvmComponent(comp,
                                                         Path.Combine(fixture.testPath,
                                                                      testName + ".acm"));

            #region check basics
            Assert.NotNull(comp);
            Assert.Equal(2, comp.Connector.Count);
            Assert.Equal(2, comp.Port.Count);
            Assert.Equal(1, comp.DomainModel.Count);
            #endregion

            #region fetch objects
            var cyberModel = comp.DomainModel.OfType<avm.cyber.CyberModel>().FirstOrDefault();
            Assert.NotNull(cyberModel);
            Assert.Equal(2, cyberModel.Connector.Count);

            var ActualBoom_Connector = comp.Connector.FirstOrDefault(c => c.Name.Equals("ActualBoom_Connector"));
            Assert.NotNull(ActualBoom_Connector);
            var connector_ActualBoom_Connector = ActualBoom_Connector.Role.OfType<avm.modelica.Connector>().First();
            Assert.NotNull(connector_ActualBoom_Connector);
            var ActualBoom_Port = comp.Port.OfType<avm.modelica.Connector>().FirstOrDefault(mc => mc.Name.Equals("ActualBoom_Port"));
            Assert.NotNull(ActualBoom_Port);
            var boomValve_Connector = comp.Connector.FirstOrDefault(c => c.Name.Equals("boomValve_Connector"));
            Assert.NotNull(boomValve_Connector);
            var connector_boomValve_Connector = boomValve_Connector.Role.OfType<avm.modelica.Connector>().First();
            Assert.NotNull(connector_boomValve_Connector);
            var boomValve_Port = comp.Port.OfType<avm.modelica.Connector>().FirstOrDefault(mc => mc.Name.Equals("boomValve_Port"));
            Assert.NotNull(boomValve_Port);
            var sample_Period = comp.Property.OfType<avm.PrimitiveProperty>().FirstOrDefault(p => p.Name.Equals("sample_Period"));
            Assert.NotNull(sample_Period);
            var ActualBoom_1 = cyberModel.Connector.FirstOrDefault(mc => mc.Name.Equals("ActualBoom_1"));
            Assert.NotNull(ActualBoom_1);
            var boomValve_1 = cyberModel.Connector.FirstOrDefault(mc => mc.Name.Equals("boomValve_1"));
            Assert.NotNull(boomValve_1);
            var SamplePeriod = cyberModel.Parameter.FirstOrDefault(p => p.Locator.Equals("SamplePeriod"));
            Assert.NotNull(SamplePeriod);
            #endregion

            AssertConnection(ActualBoom_1, ActualBoom_Port);
            AssertConnection(ActualBoom_1, connector_ActualBoom_Connector);
            AssertConnection(boomValve_1, boomValve_Port);
            AssertConnection(boomValve_1, connector_boomValve_Connector);
            
            Assert.IsType<avm.DerivedValue>(SamplePeriod.Value.ValueExpression);
            var vex = SamplePeriod.Value.ValueExpression as avm.DerivedValue;
            Assert.Equal(sample_Period.Value.ID, vex.ValueSource);

            #region check cyber model attributes
            Assert.Equal("RootFolder.Components.excavator_control", cyberModel.Locator);
            Assert.Equal("Simulink.excavator_control_type", cyberModel.Class);
            Assert.Equal("author", cyberModel.Author);
            Assert.Equal("notes", cyberModel.Notes);
            Assert.Equal(avm.cyber.ModelType.Simulink, cyberModel.Type);
            #endregion
        }

        private void AssertConnection(avm.Port port1, avm.Port port2)
        {
            Assert.True(port1.PortMap.Contains(port2.ID) || port2.PortMap.Contains(port1.ID));
        }

        [Fact]
        public void Import()
        {
            var pathACM = Path.Combine(fixture.testPath, "CompWithCyber.acm");

            avm.Component avmComp = null;
            using (var tr = new StreamReader(pathACM))
            {
                avmComp = CyPhyComponentImporter.CyPhyComponentImporterInterpreter
                                             .DeserializeAvmComponentXml(tr);
            }
            proj.PerformInTransaction(delegate
            {
                var rf = ISIS.GME.Dsml.CyPhyML.Classes.RootFolder.GetRootFolder(proj);
                var cf = rf.Children.ComponentsCollection.First();

                var comp = CyPhy2ComponentModel.Convert.AVMComponent2CyPhyML(cf, avmComp);

                Assert.Equal(1, comp.Children.CyberModelCollection.Count());
                
                var CyberModel = comp.Children.CyberModelCollection.First();

                Assert.Equal("author", CyberModel.Attributes.Author);
                Assert.Equal("notes", CyberModel.Attributes.Notes);
                Assert.Equal("RootFolder.Components.excavator_control", CyberModel.Attributes.FilePathWithinResource);
                Assert.Equal("Simulink.excavator_control_type", CyberModel.Attributes.Class);
                Assert.Equal(ISIS.GME.Dsml.CyPhyML.Classes.CyberModel.AttributesClass.ModelType_enum.Simulink, CyberModel.Attributes.ModelType);

                #region fetch objects
                var ActualBoom_Connector = comp.Children.ConnectorCollection.FirstOrDefault(c => c.Name.Equals("ActualBoom_Connector"));
                Assert.NotNull(ActualBoom_Connector);
                var connector_ActualBoom_1 = ActualBoom_Connector.Children.ModelicaConnectorCollection.FirstOrDefault();
                Assert.NotNull(connector_ActualBoom_1);                
                var boomValve_Connector = comp.Children.ConnectorCollection.FirstOrDefault(c => c.Name.Equals("boomValve_Connector"));
                Assert.NotNull(boomValve_Connector);
                var connector_boomValve_1 = boomValve_Connector.Children.ModelicaConnectorCollection.FirstOrDefault();
                Assert.NotNull(connector_boomValve_1);
                var ActualBoom_Port = comp.Children.ModelicaConnectorCollection.FirstOrDefault(mc => mc.Name.Equals("ActualBoom_Port"));
                Assert.NotNull(ActualBoom_Port);
                var boomValve_Port = comp.Children.ModelicaConnectorCollection.FirstOrDefault(mc => mc.Name.Equals("boomValve_Port"));
                Assert.NotNull(boomValve_Port);
                var ActualBoom_1 = CyberModel.Children.ModelicaConnectorCollection.FirstOrDefault(mc => mc.Name.Equals("ActualBoom_1"));
                Assert.NotNull(ActualBoom_1);
                var boomValve_1 = CyberModel.Children.ModelicaConnectorCollection.FirstOrDefault(mc => mc.Name.Equals("boomValve_1"));
                Assert.NotNull(boomValve_1);

                var sample_Period = comp.Children.ParameterCollection.FirstOrDefault(p => p.Name.Equals("sample_Period"));
                Assert.NotNull(sample_Period);
                var SamplePeriod = CyberModel.Children.ModelicaParameterCollection.FirstOrDefault(mp => mp.Name.Equals("SamplePeriod"));
                Assert.NotNull(SamplePeriod);
                #endregion

                AssertConnected(comp, connector_ActualBoom_1.Impl, ActualBoom_1.Impl);
                AssertConnected(comp, connector_boomValve_1.Impl, boomValve_1.Impl);
                AssertConnected(comp, ActualBoom_Port.Impl, ActualBoom_1.Impl);
                AssertConnected(comp, boomValve_Port.Impl, boomValve_1.Impl);

                Assert.Equal(1, comp.Children.ModelicaParameterPortMapCollection.Count());
                Assert.Equal(1, comp.Children.ModelicaParameterPortMapCollection.Count(vf => (vf.SrcEnd.ID == sample_Period.ID && vf.DstEnd.ID == SamplePeriod.ID)));
            });
        }

        private void AssertConnected(ISIS.GME.Dsml.CyPhyML.Interfaces.Component comp, IMgaObject obj1, IMgaObject obj2)
        {
            var conns1 = comp.Children.PortCompositionCollection.Where(pc => pc.SrcEnd.ID == obj1.ID && pc.DstEnd.ID == obj2.ID);
            var conns2 = comp.Children.PortCompositionCollection.Where(pc => pc.SrcEnd.ID == obj2.ID && pc.DstEnd.ID == obj1.ID);
            Assert.Equal(1, conns1.Count() + conns2.Count());
        }
    }
}
