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
using CyPhyGUIs;

namespace DesignExporterUnitTests
{
    public class DupComponentIDsFixture : IDisposable
    {
        public static String PathTest = Path.Combine(META.VersionInfo.MetaPath,
                                                     "test",
                                                     "InterchangeTest",
                                                     "DesignInterchangeTest",
                                                     "ExportTestModels",
                                                     "DupComponentIDs");

        public String pathXME = Path.Combine(PathTest, "DupComponentIDs.xme");

        public DupComponentIDsFixture()
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

    public class DupComponentIDs : IUseFixture<DupComponentIDsFixture>
    {
        #region Fixture
        DupComponentIDsFixture fixture;
        public void SetFixture(DupComponentIDsFixture data)
        {
            fixture = data;
        }
        #endregion

        private MgaProject proj { get { return fixture.proj; } }

        [Fact]
        public void DupConnectorIds()
        {
            String pathCA = "ComponentAssemblies/DupConnectorIds";

            Xunit.Assert.Throws(typeof(ApplicationException), () =>
            {
                avm.Design design = Convert(pathCA);
            });
        }

        [Fact]
        public void DupPortIds()
        {
            String pathCA = "ComponentAssemblies/DupPortIds";

            Xunit.Assert.Throws(typeof(ApplicationException), () =>
            {
                avm.Design design = Convert(pathCA);
            });
        }

        private avm.Design Convert(String pathDE)
        {
            MgaObject objDE = null;
            proj.PerformInTransaction(delegate
            {
                objDE = proj.get_ObjectByPath(pathDE);
            });
            Assert.NotNull(objDE);

            var interp = new CyPhyDesignExporter.CyPhyDesignExporterInterpreter();
            interp.Initialize(proj);
            InterpreterMainParameters param = new InterpreterMainParameters()
            {
                OutputDirectory = DupComponentIDsFixture.PathTest,
                CurrentFCO = objDE as MgaFCO,
                Project = proj
            };
            var result = interp.Main(param);
            Assert.True(result.Success);

            // Load the new .adm file
            var pathAdm = Path.Combine(DupComponentIDsFixture.PathTest,
                                       pathDE.Split('/').Last() + ".adm");
            var xml = File.ReadAllText(pathAdm);
            var design = OpenMETA.Interchange.AvmXmlSerializer.Deserialize<avm.Design>(xml);
            Assert.NotNull(design);

            return design;
        }
    }
}
