using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using Xunit;
using System.IO;
using GME.MGA;
using GME.CSharp;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using META;

namespace ComponentAndArchitectureTeamTest
{
    public class ComponentAuthoringFixture
    {
        public string mgaPath;

        public ComponentAuthoringFixture()
        {
            String mgaConnectionString;
            GME.MGA.MgaUtils.ImportXMEForTest(ComponentAuthoring.existingxmePath, out mgaConnectionString);
            mgaPath = mgaConnectionString.Substring("MGA=".Length);

            Assert.True(File.Exists(Path.GetFullPath(mgaPath)),
                        String.Format("{0} not found. Model import may have failed.", mgaPath));

            proj = new MgaProject();
            bool ro_mode;
            proj.Open("MGA=" + Path.GetFullPath(mgaPath), out ro_mode);
            proj.EnableAutoAddOns(true);
        }

        public void Dispose()
        {
            proj.Save();
            proj.Close();
        }

        public MgaProject proj { get; private set; }
    }

    public class ComponentAuthoring : IUseFixture<ComponentAuthoringFixture>
    {
        #region Path Variables
        // this first set is for the created and deleted model
        public static string testcreatepath = Path.Combine(
            META.VersionInfo.MetaPath,
            "test",
            "ComponentAuthoringToolTests");

        public static string testiconpath = Path.Combine(
            META.VersionInfo.MetaPath,
            "meta",
            "CyPhyML",
            "icons",
            "AcousticPowerPort.png");

        public static string testmfgmdlpath = Path.Combine(
            META.VersionInfo.MetaPath,
            "models",
            "Box",
            "components",
            "BottomPlate120x100",
            "generic_cots_mfg.xml");

        public static string testMGApath = Path.Combine(
            testcreatepath,
            "CATtest.MGA");

        public static string RenamedFileName = "FileNameChanged.prt";
        public static string RenamedFileNameWithoutExtension = Path.GetFileNameWithoutExtension(RenamedFileName);
        public static string RenamedFileNameRelativePath = Path.Combine("CAD", RenamedFileName);

        // this set is the saved and reused model
        public static readonly string testPath = Path.Combine(
            META.VersionInfo.MetaPath,
            "models",
            "ComponentsAndArchitectureTeam",
            "ComponentAuthoringTool"
            );
        public static readonly string existingxmePath = Path.Combine(
            testPath,
            "ComponentAuthoringTool.xme"
            );
        public static string testpartpath = Path.Combine(
            testPath,
            "damper.prt.36");
        #endregion

        #region Fixture
        ComponentAuthoringFixture fixture;
        public void SetFixture(ComponentAuthoringFixture data)
        {
            fixture = data;
        }
        #endregion

        [Fact]
        public void TestComponentInstance()
        {
            // these are the actual steps of the test
            fixture.proj.PerformInTransaction(delegate
            {
                // create the environment for the Component authoring class
                CyPhy.RootFolder rf = CyPhyClasses.RootFolder.GetRootFolder(fixture.proj);
                CyPhy.Component testcomp = fixture.proj.GetComponentsByName("JustAComponent").First();
                string ret_msg;

                // new instance of the class to test
                CyPhyComponentAuthoring.CyPhyComponentAuthoringInterpreter testcai = new CyPhyComponentAuthoring.CyPhyComponentAuthoringInterpreter();
                // these class variables need to be set to avoid NULL references
                var CurrentObj = testcomp.Impl as MgaFCO;

                // We are in a Component, check valid pre-conditions
                Assert.True(testcai.CheckPreConditions(CurrentObj, out ret_msg),
                            String.Format("{0} should allow CAT to run in it, but it is not. Err=({1})", testcomp.Name, ret_msg)
                            );

                // create a subcomponent and set it as current 
                CyPhy.Component testsubcomp = fixture.proj.GetComponentsByName("ComponentSubtype").First();
                CurrentObj = testsubcomp.Impl as MgaFCO;
                // We are in a sub-Component, check valid pre-conditions
                Assert.True(testcai.CheckPreConditions(CurrentObj, out ret_msg),
                            String.Format("{0} should allow CAT to run in it, but it is not. Err=({1})", testsubcomp.Name, ret_msg)
                            );

                // create a component instance and set it as current 
                CyPhy.Component testcompinst = fixture.proj.GetComponentsByName("ComponentInstance").First();
                CurrentObj = testcompinst.Impl as MgaFCO;
                // We are in a Component instance, check valid pre-conditions
                Assert.False(testcai.CheckPreConditions(CurrentObj, out ret_msg),
                            String.Format("{0} should NOT allow CAT to run in it, yet it is. Err=({1})", testcompinst.Name, ret_msg)
                            );

                // create a library object and set it as current 
                CyPhy.Component testlibcomp = fixture.proj.GetComponentsByName("LibComponent").First();
                CurrentObj = testlibcomp.Impl as MgaFCO;
                // We are in a library object, check valid pre-conditions
                Assert.False(testcai.CheckPreConditions(CurrentObj, out ret_msg),
                            String.Format("{0} should NOT allow CAT to run in it, yet it is. Err=({1})", testlibcomp.Name, ret_msg)
                            );
            });
        }

        [Fact]
        public void TestCADImportResourceNaming()
        {
            // delete any previous test results
            if (Directory.Exists(testcreatepath))
            {
                Directory.Delete(testcreatepath, true);
            }
            // create a new blank project
            MgaProject proj = new MgaProject();
            proj.Create("MGA=" + testMGApath, "CyPhyML");

            // these are the actual steps of the test
            proj.PerformInTransaction(delegate
            {
                // create the environment for the Component authoring class
                CyPhy.RootFolder rf = CyPhyClasses.RootFolder.GetRootFolder(proj);
                CyPhy.Components components = CyPhyClasses.Components.Create(rf);
                CyPhy.Component testcomp = CyPhyClasses.Component.Create(components);

                // new instance of the class to test
                CyPhyComponentAuthoring.Modules.CADModelImport testcam = new CyPhyComponentAuthoring.Modules.CADModelImport();
                // these class variables need to be set to avoid NULL references
                testcam.SetCurrentComp(testcomp);
                testcam.CurrentProj = proj;
                testcam.CurrentObj = testcomp.Impl as MgaFCO;

                // call the module with a part file to skip the CREO steps
                testcam.ImportCADModel(testpartpath);

                // verify results
                // insure the resource path was created correctly
                var correct_name = testcomp.Children.ResourceCollection.Where(p => p.Name == "damper.prt").First();
                Assert.True(correct_name.Attributes.Path == "CAD\\damper.prt",
                            String.Format("{0} should have had value {1}; instead found {2}", correct_name.Name, "CAD\\damper.prt", correct_name.Attributes.Path)
                            );
                // insure the part file was copied to the backend folder correctly
                var getcadmdl = testcomp.Children.CADModelCollection.First();
                string returnedpath;
                getcadmdl.TryGetResourcePath(out returnedpath, ComponentLibraryManager.PathConvention.ABSOLUTE);
                Assert.True(File.Exists(returnedpath + ".36"),
                    String.Format("Could not find the source file for the created resource, got {0}", returnedpath));
            });
            proj.Save();
            proj.Close();
            Directory.Delete(testcreatepath, true);
        }

        [Fact]
        public void TestCATDialogBoxCentering()
        {
            // these are the actual steps of the test
            fixture.proj.PerformInTransaction(delegate
            {
                // create the environment for the Component authoring class
                CyPhy.RootFolder rf = CyPhyClasses.RootFolder.GetRootFolder(fixture.proj);
                CyPhy.Component testcomp = fixture.proj.GetComponentsByName("JustAComponent").First();

                // new instance of the class to test
                CyPhyComponentAuthoring.CyPhyComponentAuthoringInterpreter testcai = new CyPhyComponentAuthoring.CyPhyComponentAuthoringInterpreter();

                // Call the create dialog box method
                testcai.PopulateDialogBox(true);
                // Get the dialog box location and verify it is in the center of the screen
                Assert.True(testcai.ThisDialogBox.StartPosition == FormStartPosition.CenterScreen,
                            String.Format("CAT dialog box is not in the center of the screen")
                            );
            });
        }

        [Fact]
        public void TestAddCustomIconTool()
        {
            // delete any previous test results
            if (Directory.Exists(testcreatepath))
            {
                Directory.Delete(testcreatepath, true);
            }
            // create a new blank project
            MgaProject proj = new MgaProject();
            proj.Create("MGA=" + testMGApath, "CyPhyML");

            // these are the actual steps of the test
            proj.PerformInTransaction(delegate
            {
                // create the environment for the Component authoring class
                CyPhy.RootFolder rf = CyPhyClasses.RootFolder.GetRootFolder(proj);
                CyPhy.Components components = CyPhyClasses.Components.Create(rf);
                CyPhy.Component testcomp = CyPhyClasses.Component.Create(components);

                // new instance of the class to test
                CyPhyComponentAuthoring.Modules.CustomIconAdd CATModule = new CyPhyComponentAuthoring.Modules.CustomIconAdd();

                //// these class variables need to be set to avoid NULL references
                CATModule.SetCurrentComp(testcomp);
                CATModule.CurrentProj = proj;
                CATModule.CurrentObj = testcomp.Impl as MgaFCO;

                // call the primary function directly
                CATModule.AddCustomIcon(testiconpath);

                // verify results
                // 1. insure the icon file was copied to the backend folder correctly
                string iconAbsolutePath = Path.Combine(testcomp.GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE), "Icon.png");
                Assert.True(File.Exists(iconAbsolutePath),
                    String.Format("Could not find the source file for the created resource, got {0}", iconAbsolutePath));

                // 2. insure the resource path was created correctly
                var iconResource = testcomp.Children.ResourceCollection.Where(p => p.Name == "Icon.png").First();
                Assert.True(iconResource.Attributes.Path == "Icon.png",
                            String.Format("{0} Resource should have had value {1}; instead found {2}", iconResource.Name, "Icon.png", iconResource.Attributes.Path)
                            );

                // 3. Verify the registry entry exists
                string expected_registry_entry = Path.Combine(testcomp.GetDirectoryPath(ComponentLibraryManager.PathConvention.REL_TO_PROJ_ROOT), "Icon.png");
                string registry_entry = (testcomp.Impl as GME.MGA.IMgaFCO).get_RegistryValue("icon");
                Assert.True(registry_entry.Equals(expected_registry_entry),
                            String.Format("Registry should have had value {0}; instead found {1}", expected_registry_entry, registry_entry)
                            );
            });
            proj.Save();
            proj.Close();
            Directory.Delete(testcreatepath, true);
        }

        [Fact]
        public void TestAddMfgModelTool()
        {
            // delete any previous test results
            if (Directory.Exists(testcreatepath))
            {
                Directory.Delete(testcreatepath, true);
            }
            // create a new blank project
            MgaProject proj = new MgaProject();
            proj.Create("MGA=" + testMGApath, "CyPhyML");

            // these are the actual steps of the test
            proj.PerformInTransaction(delegate
            {
                // create the environment for the Component authoring class
                CyPhy.RootFolder rf = CyPhyClasses.RootFolder.GetRootFolder(proj);
                CyPhy.Components components = CyPhyClasses.Components.Create(rf);
                CyPhy.Component testcomp = CyPhyClasses.Component.Create(components);

                // new instance of the class to test
                CyPhyComponentAuthoring.Modules.MfgModelImport CATModule = new CyPhyComponentAuthoring.Modules.MfgModelImport();

                //// these class variables need to be set to avoid NULL references
                CATModule.SetCurrentComp(testcomp);
                CATModule.CurrentProj = proj;
                CATModule.CurrentObj = testcomp.Impl as MgaFCO;

                // call the primary function directly
                CATModule.import_manufacturing_model(testmfgmdlpath);

                // verify results
                // 1. insure the mfg file was copied to the backend folder correctly
                // insure the part file was copied to the backend folder correctly
                var getmfgmdl = testcomp.Children.ManufacturingModelCollection.First();
                string returnedpath;
                getmfgmdl.TryGetResourcePath(out returnedpath, ComponentLibraryManager.PathConvention.ABSOLUTE);
                string demanglepathpath = returnedpath.Replace("\\", "/");
                Assert.True(File.Exists(demanglepathpath),
                    String.Format("Could not find the source file for the created resource, got {0}", demanglepathpath));

                // 2. insure the resource path was created correctly
                var mfgResource = testcomp.Children.ResourceCollection.Where(p => p.Name == "generic_cots_mfg.xml").First();
                Assert.True(mfgResource.Attributes.Path == Path.Combine("Manufacturing", "generic_cots_mfg.xml"),
                            String.Format("{0} Resource should have had value {1}; instead found {2}", mfgResource.Name, "generic_cots_mfg.xml", mfgResource.Attributes.Path)
                            );
            });
            proj.Save();
            proj.Close();
            Directory.Delete(testcreatepath, true);
        }

        [Fact]
        public void TestCADFileReNaming()
        {
            // delete any previous test results
            if (Directory.Exists(testcreatepath))
            {
                Directory.Delete(testcreatepath, true);
            }
            // create a new blank project
            MgaProject proj = new MgaProject();
            proj.Create("MGA=" + testMGApath, "CyPhyML");

            // these are the actual steps of the test
            proj.PerformInTransaction(delegate
            {
                // create the environment for the Component authoring class
                CyPhy.RootFolder rf = CyPhyClasses.RootFolder.GetRootFolder(proj);
                CyPhy.Components components = CyPhyClasses.Components.Create(rf);
                CyPhy.Component testcomp = CyPhyClasses.Component.Create(components);

                // Import a CAD file into the test project
                CyPhyComponentAuthoring.Modules.CADModelImport importcam = new CyPhyComponentAuthoring.Modules.CADModelImport();
                // these class variables need to be set to avoid NULL references
                importcam.SetCurrentComp(testcomp);
                importcam.CurrentProj = proj;
                importcam.CurrentObj = testcomp.Impl as MgaFCO;

                // import the CAD file
                importcam.ImportCADModel(testpartpath);
                // Get a path to the imported Cad file
                var getcadmdl = testcomp.Children.CADModelCollection.First();
                string importedpath;
                getcadmdl.TryGetResourcePath(out importedpath, ComponentLibraryManager.PathConvention.ABSOLUTE);
                // add in Creo version number
                importedpath += Path.GetExtension(testpartpath);

                // Rename the CAD file
                CyPhyComponentAuthoring.Modules.CADFileRename renamecam = new CyPhyComponentAuthoring.Modules.CADFileRename();
                // these class variables need to be set to avoid NULL references
                renamecam.SetCurrentComp(testcomp);
                renamecam.CurrentProj = proj;
                renamecam.CurrentObj = testcomp.Impl as MgaFCO;

                // call the module with a part file and the new file name
                renamecam.RenameCADFile(importedpath, RenamedFileNameWithoutExtension);

                // verify results
                // 1. Verify the file was renamed
                var renamecadmdl = testcomp.Children.CADModelCollection.First();
                string renamedpath;
                renamecadmdl.TryGetResourcePath(out renamedpath, ComponentLibraryManager.PathConvention.ABSOLUTE);
                // add in Creo version
                renamedpath += ".1";
                Assert.True(File.Exists(renamedpath),
                    String.Format("Could not find the renamed CAD file, found {0}", renamedpath));

                // 2. Verify the Model was renamed
                bool model_not_found = false;
                try
                {
                    var model_name = testcomp.Children.CADModelCollection.Where(p => p.Name == RenamedFileName).First();
                }
                catch (Exception ex)
                {
                    model_not_found = true;
                }
                Assert.False(model_not_found,
                            String.Format("No CAD model found with the renamed name {0}", RenamedFileName)
                            );

                // 3. Verify the resource name and path were changed
                CyPhy.Resource resource_name = null;
                bool resource_not_found = false;
                try
                {
                    resource_name = testcomp.Children.ResourceCollection.Where(p => p.Name == RenamedFileName).First();
                }
                catch (Exception ex)
                {
                    resource_not_found = true;
                }
                Assert.False(resource_not_found,
                            String.Format("No resource found with the renamed name {0}", RenamedFileName)
                            );
                Assert.True(resource_name.Attributes.Path == RenamedFileNameRelativePath,
                            String.Format("{0} should have had value {1}; instead found {2}", resource_name.Name, RenamedFileNameRelativePath, resource_name.Attributes.Path)
                            );
            });
            proj.Save();
            proj.Close();
        }
    }
}
