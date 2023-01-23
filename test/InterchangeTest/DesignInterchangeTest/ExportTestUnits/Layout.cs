using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using GME.CSharp;
using GME.MGA;
using System.IO;
using System.Threading.Tasks;
using System.Collections.Concurrent;
using OpenMETA.Interchange;

namespace DesignExporterUnitTests
{
    public class LayoutFixture : IDisposable
    {
        public static String PathTest = Path.Combine(META.VersionInfo.MetaPath,
                                                     "test",
                                                     "InterchangeTest",
                                                     "DesignInterchangeTest",
                                                     "ExportTestModels",
                                                     "Layout");

        public String pathXME = Path.Combine(PathTest, "Layout.xme");

        public LayoutFixture()
        {
            String mgaConnectionString;
            GME.MGA.MgaUtils.ImportXMEForTest(pathXME, out mgaConnectionString);

            proj = new MgaProject();
            bool ro_mode;
            proj.Open(mgaConnectionString, out ro_mode);
            proj.EnableAutoAddOns(true);
        }

        public void Dispose()
        {
            proj.Save();
            proj.Close();
        }

        public MgaProject proj { get; private set; }
    }

    internal static class Utils
    {
        public static void PerformInTransaction(this MgaProject project, MgaGateway.voidDelegate del)
        {
            var mgaGateway = new MgaGateway(project);
            mgaGateway.PerformInTransaction(del);
        }
    }

    public class Layout : IUseFixture<LayoutFixture>
    {
        #region Fixture
        LayoutFixture fixture;
        public void SetFixture(LayoutFixture data)
        {
            fixture = data;
        }
        #endregion

        private MgaProject proj { get { return fixture.proj; } }
        
        [Fact]
        public void Export()
        {
            avm.Design design = null;
            proj.PerformInTransaction(delegate
            {
                // Retrieve design contain as MgaObject
                MgaObject objDesignContainer = null;
                objDesignContainer = proj.get_ObjectByPath(@"DesignSpaces/DesignContainer");
                Assert.NotNull(objDesignContainer);

                // Cast as DesignContainer and run converter
                var designContainer = ISIS.GME.Dsml.CyPhyML.Classes.DesignContainer.Cast(objDesignContainer);
                design = CyPhy2DesignInterchange.CyPhy2DesignInterchange.Convert(designContainer);
            });
            Assert.NotNull(design);

            String pathXmlOut = Path.Combine(LayoutFixture.PathTest, "DesignContainer.adm");
            using (StreamWriter sw = new StreamWriter(pathXmlOut, false))
            {
                sw.Write(AvmXmlSerializer.Serialize(design));
            }

            var checker = new LayoutDataChecker();
            var result = checker.Check(design.RootContainer);

            if (result.Any())
            {
                String msg = Environment.NewLine + 
                             String.Join(Environment.NewLine,
                                         result);
                Assert.True(false, msg);
            }
        }
    }

    internal class LayoutDataChecker
    {
        private ConcurrentBag<String> ObjectsMissingLayout;

        public IEnumerable<String> Check(avm.Container container)
        {
            ObjectsMissingLayout = new ConcurrentBag<String>();
            RecursivelyCheckForLayoutData(container);
            return ObjectsMissingLayout.AsEnumerable();
        }

        private void TestLayoutData(object obj, String path)
        {
            Boolean pass = true;
            var objType = obj.GetType();

            var xpos = objType.GetProperty("XPositionSpecified");
            Boolean? xposSpecified = xpos.GetValue(obj, null) as Boolean?;
            if (xposSpecified.HasValue == false || xposSpecified.Value == false)
            {
                pass = false;
            }

            var ypos = objType.GetProperty("YPositionSpecified");
            Boolean? yposSpecified = ypos.GetValue(obj, null) as Boolean?;
            if (yposSpecified.HasValue == false || yposSpecified.Value == false)
            {
                pass = false;
            }

            if (!pass)
            {
                ObjectsMissingLayout.Add(String.Format("{0} ({1}) doesn't contain X&Y layout data", path, objType.Name));
            }
        }

        private void RecursivelyCheckForLayoutData(avm.Container container, String path = null)
        {
            String containerPath = String.Join("/", path, container.Name);

            // Test this container for layout data
            if (false == String.IsNullOrWhiteSpace(path))
            {
                TestLayoutData(container, containerPath);
            }

            // Test child objects for layout data
            container.ComponentInstance.ForEach(x => TestLayoutData(x, String.Join("/", containerPath, x.Name)));
            container.Connector.ForEach(x => TestLayoutData(x, String.Join("/", containerPath, x.Name)));
            container.Formula.ForEach(x => TestLayoutData(x, String.Join("/", containerPath, x.Name)));
            container.Port.ForEach(x => TestLayoutData(x, String.Join("/", containerPath, x.Name)));
            container.Property.ForEach(x => TestLayoutData(x, String.Join("/", containerPath, x.Name)));

            // Recursively check child containers
            container.Container1
                     .AsParallel()
                     .ForAll(c => RecursivelyCheckForLayoutData(c, containerPath));
        }
    }
}
