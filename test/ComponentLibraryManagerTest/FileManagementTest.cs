using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using GME.CSharp;
using META;

namespace ComponentLibraryManagerTest
{
    public class FileManagementModelImport
    {
        [Fact]
        [Trait("ProjectImport/Open", "FileManagement")]
        public void ProjectXmeImport()
        {
            Assert.DoesNotThrow(() => { 
                var fixture = new FileManagementTestFixture();
                fixture.Dispose();
            });
        }
    }

    public class FileManagementTest : IUseFixture<FileManagementTestFixture>
    {
        #region Path Declarations
        public static String path_Test = Path.Combine(META.VersionInfo.MetaPath,
                                                       "models",
                                                       "ComponentsAndArchitectureTeam",
                                                       "ComponentLibraryManager");
        public static String path_XME = Path.Combine(path_Test,
                                                     "model.xme");
        public static String path_MGA = Path.Combine(path_Test,
                                                     "model_test.mga");
        public static String path_Manifest = Path.Combine(path_Test,
                                                          "manifest.project.json");
        public static String path_Manifest_Original = Path.Combine(path_Test,
                                                          "manifest.project.org");
        #endregion

        #region Fixture
        FileManagementTestFixture fixture;
        public void SetFixture(FileManagementTestFixture data)
        {
            fixture = data;
        }
        #endregion

        private CyPhy.Component getComponentByPath(String path)
        {
            MgaFilter filter = fixture.proj.CreateFilter();
            filter.Kind = "Component";

            // Only find Components whose parent folders are "Components" folders
            var match = fixture.proj.AllFCOs(filter)
                              .Cast<MgaFCO>()
                              .FirstOrDefault(c => CyPhyClasses.Component.Cast(c).Path == path);
            return CyPhyClasses.Component.Cast(match);
        }

        private void RunTestsOn(String componentPath)
        {
            var lastWriteTime_Manifest = File.GetLastWriteTime(path_Manifest);
            
            var mgaGateway = new MgaGateway(fixture.proj);
            mgaGateway.PerformInTransaction(delegate
            {
                // Get path and check that it's not empty.
                var comp = getComponentByPath(componentPath);
                var relPath_CompDir = comp.GetDirectoryPath();
                Assert.False(String.IsNullOrWhiteSpace(relPath_CompDir), "Component path was incorrectly returned as empty.");

                // Check that the component path exists
                var absPath_CompDir = comp.GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE);
                Assert.True(Directory.Exists(absPath_CompDir), "The directory indicated by the component path does not exist: " + absPath_CompDir);

                CheckResourcesOfDomainModels(comp.Children.CADModelCollection);
                CheckResourcesOfDomainModels(comp.Children.ManufacturingModelCollection);
            });

            Assert.True(lastWriteTime_Manifest == File.GetLastWriteTime(path_Manifest), "Manifest was updated by this operation, but should not have been.");
        }

        private static void CheckResourcesOfDomainModels(IEnumerable<CyPhy.DomainModel> domainModelCollection)
        {
            // Grab the resources and make sure they have paths.
            foreach (var dm in domainModelCollection)
            {
                var dmKind = dm.Kind;

                var relPath_Resource = "";
                Assert.True(META.ComponentLibraryManager.TryGetResourcePath(dm,
                                                                            out relPath_Resource,
                                                                            META.ComponentLibraryManager.PathConvention.REL_TO_COMP_DIR),
                                                                            "Could not get Resource relative path for " + dmKind);
                Assert.False(String.IsNullOrWhiteSpace(relPath_Resource), "Relative Path for " + dmKind + " resource was empty.");

                var absPath_Resource = "";
                Assert.True(META.ComponentLibraryManager.TryGetResourcePath(dm,
                                                                            out absPath_Resource,
                                                                            META.ComponentLibraryManager.PathConvention.ABSOLUTE),
                                                                            "Could not get Resource absolute path for " + dmKind);
                Assert.True(File.Exists(absPath_Resource), "File indicated by the " + dmKind + " Resource path does not exist: " + absPath_Resource);
            }
        }

        [Fact]
        public void FixtureSuccess()
        {

        }

        [Fact]
        public void ComponentArchetype_HasFilePath()
        {
            RunTestsOn("RootFolder/Components/HasFilePath");
        }
        
        [Fact]
        public void ComponentArchetype_HasFilePath_InManifestOnly()
        {
            RunTestsOn("RootFolder/Components/HasFilePath_InManifestOnly");
        }

        [Fact]
        public void ComponentArchetype_HasDirPath()
        {
            RunTestsOn("RootFolder/Components/HasDirPath");
        }

        [Fact]
        public void ComponentArchetype_HasDirPath_InManifestOnly()
        {
            RunTestsOn("RootFolder/Components/HasDirPath_InManifestOnly");
        }
        
        [Fact]
        public void ComponentArchetype_HasNoPath()
        {
            // We expect that the Component Library Manager will create a folder and assign a path.
            var lastWriteTime_Manifest = File.GetLastWriteTime(path_Manifest);
            var orgNumCompDirs = Directory.EnumerateDirectories(Path.Combine(path_Test, "components"), "*", SearchOption.TopDirectoryOnly).Count();

            var mgaGateway = new MgaGateway(fixture.proj);
            mgaGateway.PerformInTransaction(delegate
            {
                // Load the manifest and check how many components it contains.
                var manifest = AVM.DDP.MetaAvmProject.Create(fixture.proj.RootFolder.Project);
                var org_NumManifestComps = manifest.Project.Components.Count;
                
                // Get path and check that it's not empty.
                var comp = getComponentByPath("RootFolder/Components/HasNoPath");
                var relPath_CompDir = comp.GetDirectoryPath();
                Assert.False(String.IsNullOrWhiteSpace(relPath_CompDir), "Component path should be non-empty.");
                
                // Grab the resources. We expect the relative paths to be returned without error.
                foreach (var dm in comp.Children.CADModelCollection)
                {
                    var relPath_Resource = "";
                    Assert.True(META.ComponentLibraryManager.TryGetResourcePath(dm,
                                                                                out relPath_Resource,
                                                                                META.ComponentLibraryManager.PathConvention.REL_TO_COMP_DIR),
                                                                                "Could not get Resource relative path for CADModel");
                    Assert.False(String.IsNullOrWhiteSpace(relPath_Resource), "Relative Path for CADModel Resource was empty.");

                    var absPath_Resource = "";
                    Assert.True(META.ComponentLibraryManager.TryGetResourcePath(dm,
                                                                                out absPath_Resource,
                                                                                META.ComponentLibraryManager.PathConvention.ABSOLUTE),
                                                                                "Could not get Resource absolute path for CADModel");
                }
                
                manifest = AVM.DDP.MetaAvmProject.Create(fixture.proj.RootFolder.Project);
                var new_NumManifestComps = manifest.Project.Components.Count;
                Assert.False(new_NumManifestComps == org_NumManifestComps + 1, "New component entry created in manifest when it shouldn't have been.");
            });

            Assert.True(lastWriteTime_Manifest == File.GetLastWriteTime(path_Manifest), "Manifest was updated by this operation, but shouldn't have been.");

            var finalNumCompDirs = Directory.EnumerateDirectories(Path.Combine(path_Test, "components"), "*", SearchOption.TopDirectoryOnly).Count();
            Assert.True(finalNumCompDirs == 1 + orgNumCompDirs, "No component directory created.");
        }

        [Fact]
        public void ComponentInstance_HasFilePath()
        {
            RunTestsOn("RootFolder/Asm/Asm/HasFilePath");
        }

        [Fact]
        public void ComponentInstance_HasFilePath_InManifestOnly()
        {
            RunTestsOn("RootFolder/Asm/Asm/HasFilePath_InManifestOnly");
        }

        [Fact]
        public void ComponentInstance_HasDirPath()
        {
            RunTestsOn("RootFolder/Asm/Asm/HasDirPath");
        }

        [Fact]
        public void ComponentInstance_HasDirPath_InManifestOnly()
        {
            RunTestsOn("RootFolder/Asm/Asm/HasDirPath_InManifestOnly");
        }

        [Fact]
        public void TestRelativePath()
        {
            string fromPath = "C:\\Users\\kevin\\Documents\\meta-tonka\\META\\models\\ComponentsAndArchitectureTeam\\ComponentLibraryManager\\components\\a786d5f1\\/";
            string toPath = "C:\\Users\\kevin\\Documents\\meta-tonka\\META\\models\\ComponentsAndArchitectureTeam\\ComponentLibraryManager\\components\\a786d5f1\\generic_cots_mfg.xml";

            Assert.Equal("generic_cots_mfg.xml", ComponentLibraryManager.MakeRelativePath(fromPath, toPath));
        }

        [Fact]
        public void TestRelativePathUNC()
        {
            string fromPath = "\\\\ComponentsAndArchitectureTeam\\ComponentLibraryManager\\components\\a786d5f1\\/";
            string toPath = "\\\\ComponentsAndArchitectureTeam\\ComponentLibraryManager\\components\\a786d5f1\\generic_cots_mfg.xml";

            Assert.Equal("generic_cots_mfg.xml", ComponentLibraryManager.MakeRelativePath(fromPath, toPath));
        }
    }
}
