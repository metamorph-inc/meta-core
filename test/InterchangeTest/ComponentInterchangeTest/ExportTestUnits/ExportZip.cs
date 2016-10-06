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
using Ionic.Zip;

namespace ComponentExporterUnitTests
{
    public class ExportZipFixture : IDisposable
    {
        public static String path_Test = Path.Combine(META.VersionInfo.MetaPath,
                                                      "test",
                                                      "InterchangeTest",
                                                      "ComponentInterchangeTest",
                                                      "ExportTestModels",
                                                      "ExportZip");
        public static String path_XME = Path.Combine(path_Test,
                                                     "ExportZip.xme");
        public String path_MGA { get; private set; }
        public MgaProject proj { get; private set; }


        public ExportZipFixture()
        {
            String mgaConnectionString;
            GME.MGA.MgaUtils.ImportXMEForTest(path_XME, out mgaConnectionString);
            path_MGA = mgaConnectionString.Substring("MGA=".Length);

            Assert.True(File.Exists(Path.GetFullPath(path_MGA)),
                        String.Format("{0} not found. Model import may have failed.", path_MGA));

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
    }

    internal static class Utils
    {
        public static void PerformInTransaction(this MgaProject project, MgaGateway.voidDelegate del)
        {
            var mgaGateway = new MgaGateway(project);
            mgaGateway.PerformInTransaction(del);
        }

        public static IEnumerable<CyPhy.Component> GetComponentsByName(this MgaProject project, String name)
        {
            MgaFilter filter = project.CreateFilter();
            filter.Kind = "Component";
            filter.Name = name;

            return project.AllFCOs(filter)
                          .Cast<MgaFCO>()
                          .Select(x => CyPhyClasses.Component.Cast(x))
                          .Cast<CyPhy.Component>()
                          .Where(c => c.ParentContainer.Kind == "Components");

        }
    }

    public class ExportZip : IUseFixture<ExportZipFixture>
    {
        #region Fixture
        ExportZipFixture fixture;
        public void SetFixture(ExportZipFixture data)
        {
            fixture = data;
        }
        #endregion

        [Fact]
        public void ExportComponentAsZip()
        {
            var path_ZipExpected = Path.Combine(ExportZipFixture.path_Test, 
                                                "Spring_Tungsten.expected.zip");

            var path_ZipGenerated = Path.Combine(ExportZipFixture.path_Test,
                                        "Spring_Tungsten.zip");
            if (File.Exists(path_ZipGenerated))
                File.Delete(path_ZipGenerated);
            Assert.False(File.Exists(path_ZipGenerated), 
                         String.Format("{0} couldn't be deleted; Test won't be valid unless it can be newly created by the test.", path_ZipGenerated));

            // Export ZIP package
            fixture.proj.PerformInTransaction(delegate
            {
                CyPhy.Component comp = fixture.proj.GetComponentsByName("Spring_Tungsten").First();
                CyPhyComponentExporter.CyPhyComponentExporterInterpreter.ExportComponentPackage(comp, ExportZipFixture.path_Test);
            });

            Assert.True(File.Exists(path_ZipGenerated),
                        String.Format("{0} couldn't be found; Export may have failed.", path_ZipGenerated));

            avm.Component component = null;
            // Check that ZIP matches expected.
            // Assemble a list of files that didn't match.
            List<String> missing = new List<String>();
            using (ZipFile zip_Expected = ZipFile.Read(path_ZipExpected))
            {
                using (ZipFile zip_Generated = ZipFile.Read(path_ZipGenerated))
                {
                    foreach (var entry in zip_Expected)
                    {
                        var generatedFile = zip_Generated.Where(e => e.FileName == entry.FileName);
                        // If no entry with matching FileName, add it to the mising list.
                        if (false == generatedFile.Any())
                        {
                            missing.Add(entry.FileName);
                        }
                        else
                        {
                            if (generatedFile.FirstOrDefault().FileName == "Spring_Tungsten.acm")
                            {
                                MemoryStream stream = new MemoryStream();
                                generatedFile.FirstOrDefault().Extract(stream);
                                component = XSD2CSharp.AvmXmlSerializer.Deserialize<avm.Component>(Encoding.UTF8.GetString(stream.ToArray()));
                            }
                        }
                    }
                }
            }

            if (missing.Any())
            {
                String msg = String.Format("{0} was missing these expected files: ", path_ZipGenerated);
                foreach (var filename in missing)
                    msg += filename + "  ";
                Assert.True(false, msg);
            }

            Assert.NotNull(component);
            var path = component.ResourceDependency.Where(x => x.Name == "TUNGSTEN_SPRING.PRT").FirstOrDefault().Path;
            Assert.Equal("CAD\\TUNGSTEN_SPRING.PRT.3", path);
        }
    }
}
