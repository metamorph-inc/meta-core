using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Xunit;
using System.IO;
using GME.MGA;
using Xunit.Sdk;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;

namespace DesignImporterTests
{
    public abstract class DesignImporterTestFixtureBase : IDisposable
    {
        public static String PathTest = Path.Combine(
            Path.GetDirectoryName(Assembly.GetExecutingAssembly().CodeBase.Substring("file:///".Length)),
            @"..\..\");

        public String AdmPath
        {
            get { return Path.GetDirectoryName(proj.ProjectConnStr.Substring(("MGA=".Length))); }
        }

        public DesignImporterTestFixtureBase()
        {
            String mgaConnectionString;
            GME.MGA.MgaUtils.ImportXMEForTest(Path.Combine(PathTest, pathXME), out mgaConnectionString);

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

        public abstract String pathXME { get; }
    }


    public class RollingWheelFixture : DesignImporterTestFixtureBase
    {
        public override String pathXME
        {
            get { return "RollingWheel.xme"; }
        }
    }


    public class ImportWebGMEDesigns : IUseFixture<RollingWheelFixture>
    {
        [STAThread]
        public static int Main(string[] args)
        {
            int ret = Xunit.ConsoleClient.Program.Main(new string[]
            {
                Assembly.GetExecutingAssembly().CodeBase.Substring("file:///".Length),
                //"/noshadow",
            });
            Console.In.ReadLine();
            return ret;
        }

        //[Fact]
        public void TestRollingWheelWithFormula()
        {
            var admFilename = "WheelWithFormula.adm";
            avm.Design design;
            using (StreamReader streamReader = new StreamReader(Path.Combine(fixture.AdmPath, admFilename)))
                design = CyPhyDesignImporter.CyPhyDesignImporterInterpreter.DeserializeAvmDesignXml(streamReader);

            Assert.True(0 < design.RootContainer.Property.Count);

            proj.BeginTransactionInNewTerr();
            try
            {
                var importer = new CyPhyDesignImporter.AVMDesignImporter(null, proj);
                var ret = (CyPhy.DesignContainer)importer.ImportDesign(design, CyPhyDesignImporter.AVMDesignImporter.DesignImportMode.CREATE_DS);

                Xunit.Assert.Equal(3, ret.Children.PropertyCollection.Count());
            }
            finally
            {
                proj.CommitTransaction();
                if (Debugger.IsAttached)
                {
                    proj.Save(proj.ProjectConnStr + admFilename + ".mga", true);
                }
            }
        }

        private IMgaProject proj { get { return fixture.proj; } }

        private RollingWheelFixture fixture;
        public void SetFixture(RollingWheelFixture data)
        {
            fixture = data;
        }
    }

}
