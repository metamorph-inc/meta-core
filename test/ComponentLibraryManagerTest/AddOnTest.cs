using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using GME.MGA;
using System.IO;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using META;
using GME.CSharp;
using System.Runtime.CompilerServices;
using System.Diagnostics;

namespace ComponentLibraryManagerTest
{
    public class AddOnTestFixture : IDisposable
    {
        public AddOnTestFixture()
        {
            // Copy to create a new test folder
            if (Directory.Exists(AddOnTest.path_Test))
                Directory.Delete(AddOnTest.path_Test, true);

            Utils.DirectoryCopy(AddOnTest.path_OriginalFiles, AddOnTest.path_Test);

            // Delete any MGA and import from scratch
            File.Delete(AddOnTest.path_MGA);
            GME.MGA.MgaUtils.ImportXME(AddOnTest.path_XME, AddOnTest.path_MGA);
            Assert.True(File.Exists(AddOnTest.path_MGA));

            File.Copy(AddOnTest.path_Manifest_Original, AddOnTest.path_Manifest, true);

            proj = new MgaProject();
            bool ro_mode;
            proj.Open("MGA=" + Path.GetFullPath(AddOnTest.path_MGA), out ro_mode);
        }

        public void Dispose()
        {
            proj.Save();
            proj.Close();
        }

        public MgaProject proj { get; private set; }

        public MgaProject GetProject()
        {
            return this.proj;
        }
    }

    internal static class Utils
    {
        public static void PerformInTransaction(this MgaProject project, MgaGateway.voidDelegate del)
        {
            var mgaGateway = new MgaGateway(project);
            project.CreateTerritoryWithoutSink(out mgaGateway.territory);
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

        [MethodImpl(MethodImplOptions.NoInlining)]
        public static string GetCurrentMethod()
        {
            StackTrace st = new StackTrace();
            StackFrame sf = st.GetFrame(1);

            return sf.GetMethod().Name;
        }

        // Taken from http://msdn.microsoft.com/en-us/library/bb762914(v=vs.100).aspx
        public static void DirectoryCopy(string sourceDirName, string destDirName, bool copySubDirs = true)
        {
            DirectoryInfo dir = new DirectoryInfo(sourceDirName);
            DirectoryInfo[] dirs = dir.GetDirectories();

            // If the source directory does not exist, throw an exception.
            if (!dir.Exists)
            {
                throw new DirectoryNotFoundException(
                    "Source directory does not exist or could not be found: "
                    + sourceDirName);
            }

            // If the destination directory does not exist, create it.
            if (!Directory.Exists(destDirName))
            {
                Directory.CreateDirectory(destDirName);
            }


            // Get the file contents of the directory to copy.
            FileInfo[] files = dir.GetFiles();

            foreach (FileInfo file in files)
            {
                // Create the path to the new copy of the file.
                string temppath = Path.Combine(destDirName, file.Name);

                // Copy the file.
                file.CopyTo(temppath, false);
            }

            // If copySubDirs is true, copy the subdirectories.
            if (copySubDirs)
            {

                foreach (DirectoryInfo subdir in dirs)
                {
                    // Create the subdirectory.
                    string temppath = Path.Combine(destDirName, subdir.Name);

                    // Copy the subdirectories.
                    DirectoryCopy(subdir.FullName, temppath, copySubDirs);
                }
            }
        }
    }

    public class AddOnTest : IUseFixture<AddOnTestFixture>
    {
        #region Path Declarations
        public static String path_OriginalFiles = Path.Combine(META.VersionInfo.MetaPath,
                                                               "models",
                                                               "ComponentsAndArchitectureTeam",
                                                               "ComponentLibraryManager");
        public static String path_Test = Path.Combine(META.VersionInfo.MetaPath,
                                                       "models",
                                                       "ComponentsAndArchitectureTeam",
                                                       "ComponentLibraryManager_AddOn");
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
        AddOnTestFixture fixture;
        public void SetFixture(AddOnTestFixture data)
        {
            fixture = data;
        }
        #endregion

        [Fact]
        public void BrandNewComponent()
        {
            // Create a branch new component and submit the transaction.
            // This should trigger the add-on.
            // Next, go and check that the Component has a Path, and that that Path exists on disk.

            var project = fixture.GetProject();
            project.EnableAutoAddOns(true);

            project.PerformInTransaction(delegate
            {
                CyPhy.RootFolder rf = ISIS.GME.Common.Utils.CreateObject<CyPhyClasses.RootFolder>(project.RootFolder as MgaObject);
                CyPhy.Components cf = rf.Children.ComponentsCollection.First();

                CyPhy.Component comp = CyPhyClasses.Component.Create(cf);
                comp.Name = "BrandNewComponent";
            });

            project.PerformInTransaction(delegate
            {
                var matchingComponents = project.GetComponentsByName("BrandNewComponent");
                Assert.Equal(1, matchingComponents.Count());

                var comp_Retrieved = matchingComponents.First();
                Assert.True(!String.IsNullOrWhiteSpace(comp_Retrieved.Attributes.AVMID), "Retrieved component is missing AVMID.");
                Assert.True(!String.IsNullOrWhiteSpace(comp_Retrieved.Attributes.Path), "Retrieved component is missing Path.");
                Assert.True(Directory.Exists(comp_Retrieved.GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE)), "Component path doesn't exist.");
            });
            project.Save();
        }

        [Fact]
        public void InsideCopy()
        {
            // Take an existing component and copy it.
            // Close the transaction.
            // The, retrieve that guy and check it.
            // It should have a different AVMID and Path, and that path should exist on disk.
            var project = fixture.GetProject();
            project.EnableAutoAddOns(true);

            string name_CompCopy = "";
            project.PerformInTransaction(delegate
            {
                var comp_ToCopy = project.GetComponentsByName("HasDirPath")
                                                .FirstOrDefault();
                Assert.True(null != comp_ToCopy, "Couldn't find component to copy.");

                var parentFolder = comp_ToCopy.ParentContainer.Impl as IMgaFolder;
                var comp_Copy = parentFolder.CopyFCODisp(comp_ToCopy.Impl as MgaFCO);
                name_CompCopy = comp_Copy.Name = "HasDirPath_copy";
            });

            project.PerformInTransaction(delegate
            {
                CyPhy.Component comp_Original = project.GetComponentsByName("HasDirPath")
                                                       .FirstOrDefault();
                Assert.True(comp_Original != null, "Couldn't find original component.");

                CyPhy.Component comp_Copy = project.GetComponentsByName(name_CompCopy)
                                                   .FirstOrDefault();
                Assert.True(null != comp_Copy, "Couldn't find copied component.");

                Assert.False(comp_Original.Attributes.AVMID == comp_Copy.Attributes.AVMID, "Copied component has the same AVMID as the original.");
                Assert.False(comp_Original.Attributes.Path == comp_Copy.Attributes.Path, "Copied component has the same Path as the original.");
                Assert.True(Directory.Exists(comp_Copy.GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE)), "Copied component's Path does not exist.");
            });
            project.Save();
        }

        [Fact]
        public void OutsideCopy_SameProjDir()
        {
            // Create a new component.
            // Its AVMID and Path will be unique to this project, and that path will exist already on disk.
            // We expect that the Add-on isn't going to change anything.

            var compName = Utils.GetCurrentMethod();

            // First, let's create such a path.
            var path_ToCopy = Path.Combine(path_Test,
                                           "components",
                                           "z672jsd8");
            var path_NewCopy = Path.Combine(path_Test,
                                            "components",
                                            compName);
            Utils.DirectoryCopy(path_ToCopy, path_NewCopy);

            // Now let's simulate copying a component from another project.
            // It should have a Path, AVMID, and a Resource.
            String org_AVMID = "";
            String org_Path = "";

            var project = fixture.GetProject();
            project.EnableAutoAddOns(true);
            project.PerformInTransaction(delegate
            {
                CyPhy.RootFolder rf = ISIS.GME.Dsml.CyPhyML.Classes.RootFolder.GetRootFolder(project);

                CyPhy.Components cf = rf.Children.ComponentsCollection.First();
                CyPhy.Component c = CyPhyClasses.Component.Create(cf);

                c.Name = compName;
                org_AVMID = c.Attributes.AVMID = Guid.NewGuid().ToString("D");
                org_Path = c.Attributes.Path = Path.Combine("components", compName);

                CyPhy.Resource res = CyPhyClasses.Resource.Create(c);
                res.Attributes.Path = "generic_cots_mfg.xml";
                res.Name = res.Attributes.Path;
            });

            project.PerformInTransaction(delegate
            {
                var c = project.GetComponentsByName(compName).FirstOrDefault();
                Assert.True(org_AVMID == c.Attributes.AVMID, "AVMID was changed.");
                Assert.True(org_Path == c.Attributes.Path, "Path was changed.");
            });
            project.Save();
        }

        [Fact]
        public void OutsideCopy_DifferentProjDir()
        {
            // Create a new component.
            // Its AVMID and Path will be unique to this project, and that path will NOT exist already on disk,
            // because we expect in this case that the component was copied from another project somewhere else.
            // WHAT DO WE EXPECT THE ADD-ON TO DO?
            // 1. Because the folder does not exist on disk, a new path and backing folder will be created.
            // 2. AVMID will stay the same.

            var compName = Utils.GetCurrentMethod();

            // First, let's create a fake path.
            var path_NewCopy = Path.Combine(path_Test,
                                            "components",
                                            compName);

            // Now let's simulate copying a component from another project.
            // It should have a Path, AVMID, and a Resource.
            String org_AVMID = "";
            String org_Path = "";

            var project = fixture.GetProject();
            project.EnableAutoAddOns(true);
            project.PerformInTransaction(delegate
            {
                CyPhy.RootFolder rf = ISIS.GME.Dsml.CyPhyML.Classes.RootFolder.GetRootFolder(project);

                CyPhy.Components cf = rf.Children.ComponentsCollection.First();
                CyPhy.Component c = CyPhyClasses.Component.Create(cf);

                c.Name = compName;
                org_AVMID = c.Attributes.AVMID = Guid.NewGuid().ToString("D");
                org_Path = c.Attributes.Path = @"components\jdsof123\";

                CyPhy.Resource res = CyPhyClasses.Resource.Create(c);
                res.Attributes.Path = "generic_cots_mfg.xml";
                res.Name = res.Attributes.Path;
            });

            project.PerformInTransaction(delegate
            {
                var c = project.GetComponentsByName(compName).FirstOrDefault();
                Assert.True(org_AVMID == c.Attributes.AVMID, "AVMID was changed.");
                Assert.True(org_Path != c.Attributes.Path, "Component should have been assigned a new path.");
                Assert.True(Directory.Exists(c.GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE)), "New component folder doesn't exist on disk.");
            });
            project.Save();
        }

        [Fact]
        public void OutsideCopy_DifferentProjDir_AVMIDCollision()
        {
            // Create a new component.
            // Its AVMID will NOT be unique to this project, and thus a new (unique) AVMID must be assigned.
            // Its Path will be unique to this project, and that path will NOT exist already on disk,
            // because we expect in this case that the component was copied from another project somewhere else.
            // WHAT DO WE EXPECT THE ADD-ON TO DO?
            // 1. Assign a new AVMID.
            // 2. Because the folder does not exist on disk, a new path and backing folder will be created for this guy.

            var compName = Utils.GetCurrentMethod();

            // First, let's create a fake path.
            var path_NewCopy = Path.Combine(path_Test,
                                            "components",
                                            compName);

            // Now let's simulate copying a component from another project.
            // It should have a Path, AVMID, and a Resource.
            String org_AVMID = "";
            String org_Path = "";

            var project = fixture.GetProject();
            project.EnableAutoAddOns(true);
            project.PerformInTransaction(delegate
            {
                CyPhy.RootFolder rf = ISIS.GME.Dsml.CyPhyML.Classes.RootFolder.GetRootFolder(project);

                CyPhy.Components cf = rf.Children.ComponentsCollection.First();
                CyPhy.Component c = CyPhyClasses.Component.Create(cf);

                c.Name = compName;
                org_AVMID = c.Attributes.AVMID = "101b"; // This AVMID should collide
                org_Path = c.Attributes.Path = Path.Combine("components", compName);

                CyPhy.Resource res = CyPhyClasses.Resource.Create(c);
                res.Attributes.Path = "generic_cots_mfg.xml";
                res.Name = res.Attributes.Path;
            });

            project.PerformInTransaction(delegate
            {
                var c = project.GetComponentsByName(compName).FirstOrDefault();
                Assert.True(org_AVMID != c.Attributes.AVMID, "A new AVMID should have been assigned.");
                Assert.True(org_Path != c.Attributes.Path, "Component should have been assigned a new path.");
                Assert.True(Directory.Exists(c.GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE)), "New component folder doesn't exist on disk.");
            });
            project.Save();
        }

        [Fact]
        public void CopyHasInvalidPath()
        {
            // Create a new component.
            // We'll make it look like a copy, but the source (inside/outside) is not important.
            // That's because we'll give it a path that is invalid.
            // WHAT DO WE EXPECT THE ADD-ON TO DO?
            // Warn the user, and create a new folder.

            // These characters are not valid in Windows paths
            List<String> invalidCharacters = new List<String>()
            {
                "?",
                "<",
                ">",
                "\\",
                ":",
                "*",
                "|",
                " "
            };

            // Tuple structure: Component Name | Path Changed | Path Exists
            List<Tuple<String, Boolean, Boolean>> results = new List<Tuple<String, Boolean, Boolean>>();

            foreach (var invalidChar in invalidCharacters)
            {
                String org_Path = String.Format("components\\invalid{0}", invalidChar);

                var compName = String.Format("{0}{1}", Utils.GetCurrentMethod(), invalidChar);
                string objID = "";

                var project = fixture.GetProject();
                project.EnableAutoAddOns(true);
                project.PerformInTransaction(delegate
                {
                    CyPhy.RootFolder rf = ISIS.GME.Dsml.CyPhyML.Classes.RootFolder.GetRootFolder(project);

                    CyPhy.Components cf = rf.Children.ComponentsCollection.First();
                    CyPhy.Component c = CyPhyClasses.Component.Create(cf);

                    c.Name = compName;
                    c.Attributes.Path = org_Path;
                    objID = c.ID;

                    CyPhy.Resource res = CyPhyClasses.Resource.Create(c);
                    res.Attributes.Path = "generic_cots_mfg.xml";
                    res.Name = res.Attributes.Path;
                });

                // Perform tests, but queue them for batch display later.
                project.PerformInTransaction(delegate
                {
                    //var c = project.GetComponentsByName(compName).FirstOrDefault();
                    var c = CyPhyClasses.Component.Cast(project.GetObjectByID(objID));
                    results.Add(new Tuple<string, bool, bool>(
                        c.Name,
                        org_Path != c.Attributes.Path,
                        Directory.Exists(c.GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE))
                    ));
                });
            }

            // Did anybody not get a new path?
            var item2Failures = results.Where(r => r.Item2 == false);
            if (item2Failures.Any())
            {
                String message = "Components were not assigned a new path: ";
                foreach (var result in item2Failures)
                {
                    message += " " + result.Item1;
                }

                // Force assert failure
                Assert.False(true, message);
            }

            // Did anybody's path not exist?
            var item3Failures = results.Where(r => r.Item3 == false);
            if (item3Failures.Any())
            {
                String message = "New component folders do not exist on disk for: ";
                foreach (var result in item3Failures)
                {
                    message += " " + result.Item1;
                }

                // Force assert failure
                Assert.False(true, message);
            }
        }

        [Fact]
        public void OutsideCopy_SameProjDir_MissingAVMID()
        {
            // Simulate copying a component from another MGA in the same project folder.
            // It will have a Path unique to this project, and the path will already exist.
            // However, it will be missing an AVMID.
            // WHAT DO WE EXPECT THE ADDON TO DO?
            // We expect it to assign a new AVMID, but change nothing else.

            var compName = Utils.GetCurrentMethod();

            // First, let's create such a path.
            var path_ToCopy = Path.Combine(path_Test,
                                           "components",
                                           "z672jsd8");
            var path_NewCopy = Path.Combine(path_Test,
                                            "components",
                                            compName);
            Utils.DirectoryCopy(path_ToCopy, path_NewCopy);

            // Now let's simulate copying a component from another project.
            // It should have a Path, AVMID, and a Resource.
            String org_AVMID = "";
            String org_Path = "";

            var project = fixture.GetProject();
            project.EnableAutoAddOns(true);
            project.PerformInTransaction(delegate
            {
                CyPhy.RootFolder rf = ISIS.GME.Dsml.CyPhyML.Classes.RootFolder.GetRootFolder(project);

                CyPhy.Components cf = rf.Children.ComponentsCollection.First();
                CyPhy.Component c = CyPhyClasses.Component.Create(cf);

                c.Name = compName;
                c.Attributes.AVMID = org_AVMID;
                org_Path = c.Attributes.Path = Path.Combine("components", compName);

                CyPhy.Resource res = CyPhyClasses.Resource.Create(c);
                res.Attributes.Path = "generic_cots_mfg.xml";
                res.Name = res.Attributes.Path;
            });

            project.PerformInTransaction(delegate
            {
                var c = project.GetComponentsByName(compName).FirstOrDefault();
                Assert.True(org_AVMID != c.Attributes.AVMID, "A new AVMID should have been assigned.");
                Assert.True(org_Path == c.Attributes.Path, "The path should not have been changed.");
            });
            project.Save();
        }

        [Fact]
        public void InsideCopy_MissingAVMID()
        {
            // Take an existing AVMID-less component and copy it
            // Close the transaction.
            // The, retrieve that guy and check it.
            // WHAT DO WE EXPECT THE ADDON TO DO?
            // There should be a new AVMID.
            // The contents of the original's folder should also have been copied to the new component's folder,
            // since it should still register as a copy.
            var project = fixture.GetProject();
            project.EnableAutoAddOns(true);

            string name_CompCopy = Utils.GetCurrentMethod();
            string name_CompOriginal = "HasPathButNoAVMID";
            project.PerformInTransaction(delegate
            {
                var comp_ToCopy = project.GetComponentsByName(name_CompOriginal)
                                         .FirstOrDefault();
                Assert.True(null != comp_ToCopy, "Couldn't find component to copy.");

                var parentFolder = comp_ToCopy.ParentContainer.Impl as IMgaFolder;
                var comp_Copy = parentFolder.CopyFCODisp(comp_ToCopy.Impl as MgaFCO);
                comp_Copy.Name = name_CompCopy;
                comp_Copy.SetAttributeByNameDisp("AVMID", "");
            });

            project.PerformInTransaction(delegate
            {
                CyPhy.Component comp_Original = project.GetComponentsByName(name_CompOriginal)
                                                       .FirstOrDefault();
                Assert.True(comp_Original != null, "Couldn't find original component.");

                CyPhy.Component comp_Copy = project.GetComponentsByName(name_CompCopy)
                                                   .FirstOrDefault();
                Assert.True(null != comp_Copy, "Couldn't find copied component.");

                Assert.False(String.IsNullOrWhiteSpace(comp_Copy.Attributes.AVMID), "Copied component wasn't assigned a new AVMID.");
                Assert.False(comp_Original.Attributes.Path == comp_Copy.Attributes.Path, "Copied component has the same Path as the original.");

                var path_CompCopy = comp_Copy.GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE);
                Assert.True(Directory.Exists(path_CompCopy), "Copied component's Path does not exist on disk.");

                var path_Resource = Path.Combine(path_CompCopy, "generic_cots_mfg.xml");
                Assert.True(File.Exists(path_Resource), "Copied component has a new path, but the original content wasn't copied.");
            });
            project.Save();
        }

        [Fact]
        [Trait("JIRA","META-2856")]
        public void InsideCopy_OriginalPathHasPeriod()
        {
            /* The "twist" here is that the original path,
             * although it points to a directory, has a period
             * in it, which makes it look like a file.
             * We need to avoid being tricked by that.
             */

            /* Take an existing component and copy it.
             * After copy, close the transaction
             * Then, retrieve that guy and check it
             * It should have a different AVMID and Path, that path should exist on disk.
             * Take one of the resources and make sure it exists too.
             */ 
            var project = fixture.GetProject();
            project.EnableAutoAddOns(true);

            string name_CompCopy = "";
            project.PerformInTransaction(delegate
            {
                var comp_ToCopy = project.GetComponentsByName("HasDirPath_WithPeriod")
                                         .FirstOrDefault();
                Assert.True(null != comp_ToCopy, "Couldn't find component to copy.");

                var parentFolder = comp_ToCopy.ParentContainer.Impl as IMgaFolder;
                var comp_Copy = parentFolder.CopyFCODisp(comp_ToCopy.Impl as MgaFCO);
                name_CompCopy = comp_Copy.Name = "HasDirPath_WithPeriod_copy";
            });

            project.PerformInTransaction(delegate
            {
                CyPhy.Component comp_Original = project.GetComponentsByName("HasDirPath_WithPeriod")
                                                       .FirstOrDefault();
                Assert.True(comp_Original != null, "Couldn't find original component.");

                CyPhy.Component comp_Copy = project.GetComponentsByName(name_CompCopy)
                                                   .FirstOrDefault();
                Assert.True(null != comp_Copy, "Couldn't find copied component.");

                Assert.False(comp_Original.Attributes.AVMID == comp_Copy.Attributes.AVMID, "Copied component has the same AVMID as the original.");

                String org_full = Path.GetFullPath(comp_Original.Attributes.Path);
                String new_full = Path.GetFullPath(comp_Copy.Attributes.Path);
                Assert.False(org_full == new_full, 
                             "Copied component has the same Path as the original.");
                Assert.True(Directory.Exists(comp_Copy.GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE)), "Copied component's Path does not exist.");

                foreach (var resource in comp_Copy.Children.ResourceCollection)
                {
                    var resFullPath = Path.Combine(comp_Copy.GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE),
                                                   resource.Attributes.Path);

                    Assert.True(File.Exists(resFullPath),
                                String.Format("Resource could not be found on disk: {0}", resFullPath));
                }
            });

            project.Save();
        }


        [Fact]
        [Trait("JIRA", "META-2856")]
        public void InsideCopy_OriginalPathHasPeriod_AndTrailingSlash()
        {
            /* The "twist" here is that the original path,
             * although it points to a directory, has a period
             * in it, but also a trailing slash.
             * We need to avoid being tricked into thinking
             * that the path points to a file.
             */

            /* Take an existing component and copy it.
             * After copy, close the transaction
             * Then, retrieve that guy and check it
             * It should have a different AVMID and Path, that path should exist on disk.
             * Take one of the resources and make sure it exists too.
             */
            var project = fixture.GetProject();
            project.EnableAutoAddOns(true);

            string name_CompCopy = "";
            project.PerformInTransaction(delegate
            {
                var comp_ToCopy = project.GetComponentsByName("HasDirPath_WithPeriod_TrailingSlash")
                                         .FirstOrDefault();
                Assert.True(null != comp_ToCopy, "Couldn't find component to copy.");

                var parentFolder = comp_ToCopy.ParentContainer.Impl as IMgaFolder;
                var comp_Copy = parentFolder.CopyFCODisp(comp_ToCopy.Impl as MgaFCO);
                name_CompCopy = comp_Copy.Name = "HasDirPath_WithPeriod_TrailingSlash_copy";
            });

            project.PerformInTransaction(delegate
            {
                CyPhy.Component comp_Original = project.GetComponentsByName("HasDirPath_WithPeriod_TrailingSlash")
                                                       .FirstOrDefault();
                Assert.True(comp_Original != null, "Couldn't find original component.");

                CyPhy.Component comp_Copy = project.GetComponentsByName(name_CompCopy)
                                                   .FirstOrDefault();
                Assert.True(null != comp_Copy, "Couldn't find copied component.");

                Assert.False(comp_Original.Attributes.AVMID == comp_Copy.Attributes.AVMID, "Copied component has the same AVMID as the original.");

                String org_full = Path.GetFullPath(comp_Original.Attributes.Path);
                String new_full = Path.GetFullPath(comp_Copy.Attributes.Path);
                Assert.False(org_full == new_full,
                             "Copied component has the same Path as the original.");
                Assert.True(Directory.Exists(comp_Copy.GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE)), "Copied component's Path does not exist.");

                foreach (var resource in comp_Copy.Children.ResourceCollection)
                {
                    var resFullPath = Path.Combine(comp_Copy.GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE),
                                                   resource.Attributes.Path);

                    Assert.True(File.Exists(resFullPath),
                                String.Format("Resource could not be found on disk: {0}", resFullPath));
                }
            });

            project.Save();
        }
    }
}
