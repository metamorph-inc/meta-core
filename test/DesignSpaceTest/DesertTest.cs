﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using GME.MGA;
using System.IO;
using System.Reflection;
using GME.CSharp;
using ISIS.GME.Dsml.CyPhyML.Interfaces;
using System.Runtime.InteropServices;

namespace DesignSpaceTest
{
    public class ToyDSFixture : IDisposable
    {
        public ToyDSFixture()
        {
            try
            {
                string connection;
                MgaUtils.ImportXMEForTest(
                    Path.Combine(Path.GetDirectoryName(Assembly.GetAssembly(this.GetType()).CodeBase.Substring("file:///".Length)),
                    @"..\..\..\..\models\DesignSpace\ToyDS.xme"),
                    out connection);

                Type type = Type.GetTypeFromProgID("Mga.MgaProject");
                proj_ = Activator.CreateInstance(type) as MgaProject;
                proj_.OpenEx(connection, "CyPhyML", null);
            }
            catch (Exception e)
            {
                this.exception = e;
            }
        }

        private Exception exception;
        private IMgaProject proj_;
        public IMgaProject proj {
            get {
                if (exception != null)
                {
                    throw exception;
                }
                return proj_;
            }
        }

        public void Dispose()
        {
            if (exception == null)
            {
                proj_.Save();
                proj_.Close(true);
            }
        }
    }

    public class DesertTest : IUseFixture<ToyDSFixture>
    {
        public void DesertTestBase(string dsPath, Action<IEnumerable<Configurations>> helperTest, Action<Configurations> exporterTest)
        {
            var gateway = new MgaGateway(project);
            Type desertType = Type.GetTypeFromProgID("MGA.Interpreter.DesignSpaceHelper");
            var desert = (IMgaComponentEx)Activator.CreateInstance(desertType);

            MgaFCO currentobj = null;
            gateway.PerformInTransaction(() =>
            {
                currentobj = (MgaFCO)project.RootFolder.ObjectByPath[dsPath];
                var configurations = ISIS.GME.Dsml.CyPhyML.Classes.DesignContainer.Cast(currentobj).Children.ConfigurationsCollection;
                foreach (var configuration in configurations)
                {
                    configuration.Delete();
                }
            }, abort: false);
            Xunit.Assert.True(currentobj != null, string.Format("'{0}' does not exist in model", dsPath));

            desert.Initialize(project);
            desert.InvokeEx(project, currentobj, null, 128);
            Configurations configs = null;
            gateway.PerformInTransaction(() =>
            {
                var configurations = ISIS.GME.Dsml.CyPhyML.Classes.DesignContainer.Cast(currentobj).Children.ConfigurationsCollection;
                configs = configurations.First();
                helperTest(configurations);
            });

            if (exporterTest != null)
            {
                Type caExporterType = Type.GetTypeFromProgID("MGA.Interpreter.CyPhyCAExporter");
                var caExporter = (IMgaComponentEx)Activator.CreateInstance(caExporterType);

                gateway.PerformInTransaction(() =>
                {
                    MgaFCOs selected = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
                    foreach (MgaFCO cwc in configs.Children.CWCCollection.Select(x => (MgaFCO)x.Impl))
                    {
                        selected.Append(cwc);
                    }

                    caExporter.Initialize(project);
                    caExporter.InvokeEx(project, selected[1].ParentModel as MgaFCO, selected, 128);
                    exporterTest(configs);
                });
            }
        }

        [Fact]
        void TestDesertAutomation()
        {
            DesertTestBase("/@DesignSpaces/@DesignContainer", (configurations) =>
            {
                Assert.Equal(1, configurations.Count());
                Assert.Equal(2, configurations.First().Children.CWCCollection.Count());
            }, (configs) =>
            {
                foreach (var cwc in configs.Children.CWCCollection)
                {
                    Assert.Equal(1, cwc.DstConnections.Config2CACollection.Count());
                    var caConn = cwc.DstConnections.Config2CACollection.First();
                    // ((MgaModel)ca.Impl).GetDescendantFCOs(project.CreateFilter()).Count
                    var ca = ISIS.GME.Dsml.CyPhyML.Classes.ComponentAssemblyRef.Cast(caConn.DstEnd.Impl).Referred.ComponentAssembly;
                    Assert.Equal(1, ca.Children.ConnectorCollection.Count());
                }
            });

        }

        [Fact]
        void TestDesert_DesignContainer_SimpleProp()
        {
            DesertTestBase("/@DesignSpaces/@DesignContainer_SimpleProp", (configurations) =>
            {
                Assert.Equal(1, configurations.Count());
                Assert.Equal(2, configurations.First().Children.CWCCollection.Count());
            }, null);

        }

        [Fact(Skip = "Fails due to desert bug")]
        void TestDesert_DesignContainer_Alt_SimpleProp()
        {
            DesertTestBase("/@DesignSpaces/@DesignContainer_Alt_SimpleProp", (configurations) =>
            {
                Assert.Equal(1, configurations.Count());
                Assert.Equal(2, configurations.First().Children.CWCCollection.Count());
            }, null);

        }

        [Fact]
        void TestDesert_DesignContainer_DupPropName()
        {
            // bug was fixed where desert would stack overflow. Now the model is rejected
            Assert.Throws(typeof(COMException), () =>
                DesertTestBase("/@DesignSpaces/@DesignContainer_DupPropName", (configurations) =>
                {
                }, null));

        }

        [Fact]
        void TestDesert_DesignContainer_Formula()
        {
            DesertTestBase("/@DesignSpaces/@DesignContainer_Formula", (configurations) =>
            {
                Assert.Equal(1, configurations.Count());
                Assert.Equal(1, configurations.First().Children.CWCCollection.Count());
            }, null);
        }

        [Fact]
        void TestDesert_DesignContainerParamConstraint()
        {
            DesertTestBase("/@DesignSpaces/@DesignContainerParamConstraint", (configurations) =>
            {
                Assert.Equal(1, configurations.Count());
                Assert.Equal(1, configurations.First().Children.CWCCollection.Count());
            }, null);
        }

        [Fact]
        void TestDesert_DesignContainer_Opt_Constraint()
        {
            DesertTestBase("/@DesignSpaces/@DesignContainer_Opt_Constraint", (configurations) =>
            {
                Assert.Equal(1, configurations.Count());
                Assert.Equal(3, configurations.First().Children.CWCCollection.Count());
            }, null);
        }

        [Fact]
        void TestDesert_DesignContainer_Opt_Constraint2()
        {
            DesertTestBase("/@DesignSpaces/@DesignContainer_Opt_Constraint2", (configurations) =>
            {
                Assert.Equal(1, configurations.Count());
                Assert.Equal(2, configurations.First().Children.CWCCollection.Count());
            }, null);
        }

        [Fact]
        void TestDesert_DesignContainer_Opt_Constraint3()
        {
            DesertTestBase("/@DesignSpaces/@DesignContainer_Opt_Constraint3", (configurations) =>
            {
                Assert.Equal(1, configurations.Count());
                Assert.Equal(3, configurations.First().Children.CWCCollection.Count());
            }, null);
        }

        private MgaProject project { get { return (MgaProject)fixture.proj; } }

        ToyDSFixture fixture;
        public void SetFixture(ToyDSFixture data)
        {
            fixture = data;
        }
    }
}
