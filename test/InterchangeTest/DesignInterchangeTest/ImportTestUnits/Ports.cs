using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using GME.MGA;
using System.Diagnostics;
using System.IO;

namespace DesignImporterTests
{
    public class PortsFixture : DesignImporterTestFixtureBase
    {
        public override String pathXME
        {
            get { return "Ports\\Ports.xme"; }
        }
    }


    public class PortsRoundTrip : IUseFixture<PortsFixture>
    {
        [Fact]
        public void ComponentAssembly()
        {
            string asmName = "ComponentAssembly";
            RunRoundTrip(asmName);
        }

        [Fact]
        public void ComponentAssemblyWithCAD()
        {
            string asmName = "ComponentAssemblyWithCAD";
            RunRoundTrip(asmName);
        }

        private void RunRoundTrip(string asmName)
        {
            File.Delete(Path.Combine(fixture.AdmPath, asmName + ".adm"));
            RunDesignExporter(asmName);
            CopyMgaAndRunDesignImporter(asmName);
            ComponentExporterUnitTests.Tests.runCyPhyMLComparator(proj.ProjectConnStr.Substring("MGA=".Length),
                (proj.ProjectConnStr + asmName + ".mga").Substring("MGA=".Length), Environment.CurrentDirectory);
        }

        private void CopyMgaAndRunDesignImporter(string asmName)
        {
            //proj.Save(proj.ProjectConnStr + asmName + ".mga", true);
            File.Copy(proj.ProjectConnStr.Substring("MGA=".Length), (proj.ProjectConnStr + asmName + ".mga").Substring("MGA=".Length), true);

            MgaProject proj2 = (MgaProject)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaProject"));
            proj2.OpenEx(proj.ProjectConnStr + asmName + ".mga", "CyPhyML", null);
            proj2.BeginTransactionInNewTerr();
            try
            {
                MgaFCO componentAssembly = (MgaFCO)proj2.RootFolder.GetObjectByPathDisp("/@ComponentAssemblies/@" + asmName);
                Assert.NotNull(componentAssembly);
                componentAssembly.DestroyObject();
                var importer = new CyPhyDesignImporter.AVMDesignImporter(null, proj2);
                avm.Design design;
                using (StreamReader streamReader = new StreamReader(Path.Combine(fixture.AdmPath, asmName + ".adm")))
                    design = CyPhyDesignImporter.CyPhyDesignImporterInterpreter.DeserializeAvmDesignXml(streamReader);
                var ret = (ISIS.GME.Dsml.CyPhyML.Interfaces.ComponentAssembly)importer.ImportDesign(design, CyPhyDesignImporter.AVMDesignImporter.DesignImportMode.CREATE_CAS);
            }
            finally
            {
                proj2.CommitTransaction();
                if (Debugger.IsAttached)
                {
                    proj2.Save(null, true);
                }
                proj2.Close(true);
            }
        }

        private MgaFCO RunDesignExporter(string asmName)
        {
            MgaFCO componentAssembly;
            proj.BeginTransactionInNewTerr();
            try
            {
                componentAssembly = (MgaFCO)proj.RootFolder.GetObjectByPathDisp("/@ComponentAssemblies/@" + asmName);
                Assert.NotNull(componentAssembly);
                var designExporter = new CyPhyDesignExporter.CyPhyDesignExporterInterpreter();
                designExporter.Initialize(proj);

                var parameters = new CyPhyGUIs.InterpreterMainParameters()
                {
                    CurrentFCO = componentAssembly,
                    Project = proj,
                    OutputDirectory = fixture.AdmPath
                };

                designExporter.MainInTransaction(parameters);

            }
            finally
            {
                proj.AbortTransaction();
            }

            Assert.True(File.Exists(Path.Combine(fixture.AdmPath, asmName + ".adm")));
            return componentAssembly;
        }

        MgaProject proj { get { return fixture.proj; } }
        PortsFixture fixture;
        public void SetFixture(PortsFixture data)
        {
            fixture = data;
        }
    }
}
