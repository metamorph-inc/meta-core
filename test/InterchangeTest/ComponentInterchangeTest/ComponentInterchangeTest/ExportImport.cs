using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;
using GME.CSharp;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using System.Runtime.InteropServices;
using CyPhy2ComponentModel;
using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyML2AVM;
using AVM2CyPhyML;
using System.Runtime.CompilerServices;
using System.Diagnostics;

namespace ComponentInterchangeTest
{
    public class ExportImportFixture : IDisposable
    {
        public string mgaPath;

        public ExportImportFixture()
        {
            String mgaConnectionString;
            GME.MGA.MgaUtils.ImportXMEForTest(ExportImport.xmePath, out mgaConnectionString);
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

        public static IEnumerable<CyPhy.Components> GetComponentsParentByName(this MgaProject project, String name)
        {
            MgaFilter filter = project.CreateFilter();
            filter.Kind = "Components";
            filter.Name = name;

            return project.AllFCOs(filter)
                          .Cast<MgaFCO>()
                          .Select(x => CyPhyClasses.Components.Cast(x))
                          .Cast<CyPhy.Components>()
                          .Where(c => c.ParentContainer.Kind == "RootFolder");
        }
    }

    public class ExportImportModelImport
    {
        [Fact]
        [Trait("ProjectImport/Open", "ExportImport")]
        public void ProjectXmeImport()
        {
            Assert.DoesNotThrow(() => { 
                var fixture = new ExportImportFixture();
                fixture.Dispose();
            });
        }
    }

    public class ExportImport : IUseFixture<ExportImportFixture>
    {
        #region Path Variables
        public static readonly string testPath = Path.Combine(
            META.VersionInfo.MetaPath,
            "test",
            "InterchangeTest",
            "ComponentInterchangeTest",
            "SharedModels",
            "ExportImport"
            );

        public static readonly string xmePath = Path.Combine(
            testPath,
            "ExportImportTest.xme"
            );
        public static readonly string mgaPath = Path.Combine(
            testPath,
            "ExportImportTest.mga"
            );
        #endregion

        #region Fixture
        ExportImportFixture fixture;
        public void SetFixture(ExportImportFixture data)
        {
            fixture = data;
        }
        #endregion

        [MethodImpl(MethodImplOptions.NoInlining)]
        public string GetCurrentMethod()
        {
            StackTrace st = new StackTrace();
            StackFrame sf = st.GetFrame(1);

            return sf.GetMethod().Name;
        }

        private CyPhy.Component ExportThenImport(CyPhyML.Component comp, out CyPhy.Components importedcomponents, String testName)
        {
            // export the component
            avm.Component exportee = CyPhyML2AVM.AVMComponentBuilder.CyPhyML2AVM(comp);
            CyPhyComponentExporter.CyPhyComponentExporterInterpreter.SerializeAvmComponent(exportee, Path.Combine(testPath, testName + ".acm"));

            // import the component back in
            CyPhy.RootFolder rf = CyPhyClasses.RootFolder.GetRootFolder(fixture.proj);
            importedcomponents = CyPhyClasses.Components.Create(rf);
            return (AVM2CyPhyML.CyPhyMLComponentBuilder.AVM2CyPhyML(importedcomponents, exportee));
        }

        [Fact]
        [Trait("Interchange", "CAD")]
        [Trait("Interchange", "CAD Datum Map")]
        public void SingleSurfaceFeatureMapTest()
        {
            String testName = GetCurrentMethod();

            fixture.proj.PerformInTransaction(delegate
            {
                // Open component for a single surface configured as a Feature Map
                var comp = fixture.proj.GetComponentsByName("SingleSurfaceFeatureMap")
                                       .First();
                var preconnuutF = comp.Children.FeatureMapCollection
                                         .Where(p => p.Name == "FeatureMap")
                                         .First();

                // Export then Import the component
                CyPhy.Components importedcomponents;
                CyPhy.Component newcomp = ExportThenImport(comp, out importedcomponents, testName);

                // check that a FeatureMap connection exists
                // FeatureMap connections will be changed to PortCompositions during the import, so check for PortCompositions instead
                bool collection_empty = false;
                string collection_empty_msg = "";
                try
                {
                    Assert.NotEmpty(newcomp.Children.PortCompositionCollection);
                }
                catch (Exception ex)
                {
                    collection_empty = true;
                    collection_empty_msg = String.Format("Post Composition (formerly Feature Map) collection was empty: {0}", ex.Message);
                    newcomp.Delete();
                    importedcomponents.Delete();
                }
                Assert.False(collection_empty, collection_empty_msg);
                var postconnuutF = newcomp.Children.PortCompositionCollection
                                         .First();
                // cleanup
                newcomp.Delete();
                importedcomponents.Delete();
                // check that the Feature Map type is now a PortComposition
                Assert.True(postconnuutF.Properties.Kind == "PortComposition", String.Format("{0} kind changed from {1} to {2}", preconnuutF.Name, preconnuutF.Properties.Kind, postconnuutF.Properties.Kind));
            });
        }

        [Fact]
        [Trait("Interchange", "CAD")]
        [Trait("Interchange", "CAD Datum Map")]
        public void SingleSurfaceFeatureMapOppositeTest()
        {
            String testName = GetCurrentMethod();

            fixture.proj.PerformInTransaction(delegate
            {
                // Open component for a single surface configured as a Feature Map with source and destination reversed
                var comp = fixture.proj.GetComponentsByName("SingleSurfaceFeatureMapOpposite")
                                       .First();
                var preconnuutF = comp.Children.FeatureMapCollection
                                         .Where(p => p.Name == "FeatureMap")
                                         .First();

                // Export then Import the component
                CyPhy.Components importedcomponents;
                CyPhy.Component newcomp = ExportThenImport(comp, out importedcomponents, testName);

                // check that a FeatureMap connection exists
                // FeatureMap connections will be changed to PortCompositions during the import, so check for PortCompositions instead
                bool collection_empty = false;
                string collection_empty_msg = "";
                try
                {
                    Assert.NotEmpty(newcomp.Children.PortCompositionCollection);
                }
                catch (Exception ex)
                {
                    collection_empty = true;
                    collection_empty_msg = String.Format("Post Composition (formerly Feature Map) collection was empty: {0}", ex.Message);
                    newcomp.Delete();
                    importedcomponents.Delete();
                }
                Assert.False(collection_empty, collection_empty_msg);
                var postconnuutF = newcomp.Children.PortCompositionCollection
                                         .First();
                // cleanup
                newcomp.Delete();
                importedcomponents.Delete();
                // check that the Feature Map type is now a PortComposition
                Assert.True(postconnuutF.Properties.Kind == "PortComposition", String.Format("{0} kind changed from {1} to {2}", preconnuutF.Name, preconnuutF.Properties.Kind, postconnuutF.Properties.Kind));
            });
        }

        [Fact]
        [Trait("Interchange", "CAD")]
        [Trait("Interchange", "CAD Datum Map")]
        public void SingleSurfacePortCompTest()
        {
            String testName = GetCurrentMethod();

            fixture.proj.PerformInTransaction(delegate
            {
                // Open component for a single surface configured as a Port Composition
                var comp = fixture.proj.GetComponentsByName("SingleSurfacePortComp")
                                       .First();
                var preconnuutP = comp.Children.PortCompositionCollection
                                         .Where(p => p.Name == "PortComposition")
                                         .First();

                // Export then Import the component
                CyPhy.Components importedcomponents;
                CyPhy.Component newcomp = ExportThenImport(comp, out importedcomponents, testName);

                // check that a connection exists
                bool collection_empty = false;
                string collection_empty_msg = "";
                try
                {
                    Assert.NotEmpty(newcomp.Children.PortCompositionCollection);
                }
                catch (Exception ex)
                {
                    collection_empty = true;
                    collection_empty_msg = String.Format("Port Composition collection was empty: {0}", ex.Message);
                    newcomp.Delete();
                    importedcomponents.Delete();
                }
                Assert.False(collection_empty, collection_empty_msg);
                var postconnuutP = newcomp.Children.PortCompositionCollection
                                         .First();
                // cleanup
                newcomp.Delete();
                importedcomponents.Delete();

                // check that the type is correct
                Assert.True(preconnuutP.Properties.Kind == postconnuutP.Properties.Kind, String.Format("{0} kind changed from {1} to {2}", preconnuutP.Name, preconnuutP.Properties.Kind, postconnuutP.Properties.Kind));
            });
        }

        [Fact]
        [Trait("Interchange", "CAD")]
        [Trait("Interchange", "CAD Datum Map")]
        public void SingleSurfacePortCompOppositeTest()
        {
            String testName = GetCurrentMethod();

            fixture.proj.PerformInTransaction(delegate
            {
                // Open component for a single surface configured as a Port Composition with source and destination reversed
                var comp = fixture.proj.GetComponentsByName("SingleSurfacePortCompOpposite")
                                       .First();
                var preconnuutP = comp.Children.PortCompositionCollection
                                         .Where(p => p.Name == "PortComposition")
                                         .First();

                // Export then Import the component
                CyPhy.Components importedcomponents;
                CyPhy.Component newcomp = ExportThenImport(comp, out importedcomponents, testName);

                // check that a connection exists
                bool collection_empty = false;
                string collection_empty_msg = "";
                try
                {
                    Assert.NotEmpty(newcomp.Children.PortCompositionCollection);
                }
                catch (Exception ex)
                {
                    collection_empty = true;
                    collection_empty_msg = String.Format("Port Composition collection was empty: {0}", ex.Message);
                    newcomp.Delete();
                    importedcomponents.Delete();
                }
                Assert.False(collection_empty, collection_empty_msg);
                var postconnuutP = newcomp.Children.PortCompositionCollection
                                         .First();
                newcomp.Delete();
                importedcomponents.Delete();
                // check that the type is correct
                Assert.True(preconnuutP.Properties.Kind == postconnuutP.Properties.Kind, String.Format("{0} kind changed from {1} to {2}", preconnuutP.Name, preconnuutP.Properties.Kind, postconnuutP.Properties.Kind));
            });
        }

        [Fact]
        [Trait("Interchange", "CAD")]
        [Trait("Interchange", "CAD Datum Map")]
        public void SingleSurfaceReverseMapTest()
        {
            String testName = GetCurrentMethod();

            fixture.proj.PerformInTransaction(delegate
            {
                // Open component for a single surface configured as a Surface Reverse Map
                var comp = fixture.proj.GetComponentsByName("SingleSurfaceReverseMap")
                                       .First();
                var preconnuutR = comp.Children.SurfaceReverseMapCollection
                                         .Where(p => p.Name == "SurfaceReverseMap")
                                         .First();

                // Export then Import the component
                CyPhy.Components importedcomponents;
                CyPhy.Component newcomp = ExportThenImport(comp, out importedcomponents, testName);

                // check that a SurfaceReverseMap connection exists
                bool collection_empty = false;
                string collection_empty_msg = "";
                try
                {
                    Assert.NotEmpty(newcomp.Children.SurfaceReverseMapCollection);
                }
                catch (Exception ex)
                {
                    collection_empty = true;
                    collection_empty_msg = String.Format("Surface Reverse Map collection was empty: {0}", ex.Message);
                    newcomp.Delete();
                    importedcomponents.Delete();
                }
                Assert.False(collection_empty, collection_empty_msg);
                var postconnuutR = newcomp.Children.SurfaceReverseMapCollection
                                         .First();
                newcomp.Delete();
                importedcomponents.Delete();
                // check that the type is correct
                Assert.True(preconnuutR.Properties.Kind == postconnuutR.Properties.Kind, String.Format("{0} kind changed from {1} to {2}", preconnuutR.Name, preconnuutR.Properties.Kind, postconnuutR.Properties.Kind));
            });
        }

        [Fact]
        [Trait("Interchange", "CAD")]
        [Trait("Interchange", "CAD Datum Map")]
        public void SingleSurfaceReverseMapOppositeTest()
        {
            String testName = GetCurrentMethod();

            fixture.proj.PerformInTransaction(delegate
            {
                // Open component for a single surface configured as a Surface Reverse Map with source and destination reversed
                var comp = fixture.proj.GetComponentsByName("SingleSurfaceReverseMapOpposite")
                                       .First();
                var preconnuutR = comp.Children.SurfaceReverseMapCollection
                                         .Where(p => p.Name == "SurfaceReverseMap")
                                         .First();

                // Export then Import the component
                CyPhy.Components importedcomponents;
                CyPhy.Component newcomp = ExportThenImport(comp, out importedcomponents, testName);

                // check that a SurfaceReverseMap connection exists
                bool collection_empty = false;
                string collection_empty_msg = "";
                try
                {
                    Assert.NotEmpty(newcomp.Children.SurfaceReverseMapCollection);
                }
                catch (Exception ex)
                {
                    collection_empty = true;
                    collection_empty_msg = String.Format("Surface Reverse Map collection was empty: {0}", ex.Message);
                    newcomp.Delete();
                    importedcomponents.Delete();
                }
                Assert.False(collection_empty, collection_empty_msg);
                var postconnuutR = newcomp.Children.SurfaceReverseMapCollection
                                         .First();
                newcomp.Delete();
                importedcomponents.Delete();
                // check that the type is correct
                Assert.True(preconnuutR.Properties.Kind == postconnuutR.Properties.Kind, String.Format("{0} kind changed from {1} to {2}", preconnuutR.Name, preconnuutR.Properties.Kind, postconnuutR.Properties.Kind));
            });
        }

        [Fact]
        [Trait("Interchange", "CAD")]
        [Trait("Interchange", "CAD Datum Map")]
        public void DoubleSurfaceNoFeatureMapTest()
        {
            String testName = GetCurrentMethod();

            // Two surfaces configured as a Port Composition and a Surface Reverse Map
            fixture.proj.PerformInTransaction(delegate
            {
                // A single surface configured as a Feature Map
                var comp = fixture.proj.GetComponentsByName("DoubleSurfaceNoFeatureMap")
                                       .First();
                var preconnuutP = comp.Children.PortCompositionCollection
                         .Where(p => p.Name == "PortComposition")
                         .First();
                var preconnuutR = comp.Children.SurfaceReverseMapCollection
                                         .Where(p => p.Name == "SurfaceReverseMap")
                                         .First();

                // Export then Import the component
                CyPhy.Components importedcomponents;
                CyPhy.Component newcomp = ExportThenImport(comp, out importedcomponents, testName);

                // check that a PortComposition connection exists
                bool collection_empty = false;
                string collection_empty_msg = "";
                try
                {
                    Assert.NotEmpty(newcomp.Children.PortCompositionCollection);
                }
                catch (Exception ex)
                {
                    collection_empty = true;
                    collection_empty_msg = String.Format("Port Composition collection was empty: {0}", ex.Message);
                    newcomp.Delete();
                    importedcomponents.Delete();
                }
                Assert.False(collection_empty, collection_empty_msg);
                var postconnuutP = newcomp.Children.PortCompositionCollection
                                         .First();
                // check that a SurfaceReverseMap connection exists
                try
                {
                    Assert.NotEmpty(newcomp.Children.SurfaceReverseMapCollection);
                }
                catch (Exception ex)
                {
                    collection_empty = true;
                    collection_empty_msg = String.Format("Surface Reverse Map collection was empty: {0}", ex.Message);
                    newcomp.Delete();
                    importedcomponents.Delete();
                }
                Assert.False(collection_empty, collection_empty_msg);
                var postconnuutR = newcomp.Children.SurfaceReverseMapCollection
                                         .First();
                // cleanup
                newcomp.Delete();
                importedcomponents.Delete();

                // check that the Port Composition type is correct
                Assert.True(preconnuutP.Properties.Kind == postconnuutP.Properties.Kind, String.Format("{0} kind changed from {1} to {2}", preconnuutP.Name, preconnuutP.Properties.Kind, postconnuutP.Properties.Kind));
                // check that the Surface Reverse Map type is correct
                Assert.True(preconnuutR.Properties.Kind == postconnuutR.Properties.Kind, String.Format("{0} kind changed from {1} to {2}", preconnuutR.Name, preconnuutR.Properties.Kind, postconnuutR.Properties.Kind));
            });
        }

        [Fact]
        [Trait("Interchange", "CAD")]
        [Trait("Interchange", "CAD Datum Map")]
        public void DoubleSurfaceNoPortCompTest()
        {
            String testName = GetCurrentMethod();

            // Two surfaces configured as a Feature Map and a Surface Reverse Map
            fixture.proj.PerformInTransaction(delegate
            {
                // A single surface configured as a Feature Map
                var comp = fixture.proj.GetComponentsByName("DoubleSurfaceNoPortComp")
                                       .First();
                var preconnuutF = comp.Children.FeatureMapCollection
                                         .Where(p => p.Name == "FeatureMap")
                                         .First();
                var preconnuutR = comp.Children.SurfaceReverseMapCollection
                                         .Where(p => p.Name == "SurfaceReverseMap")
                                         .First();

                // Export then Import the component
                CyPhy.Components importedcomponents;
                CyPhy.Component newcomp = ExportThenImport(comp, out importedcomponents, testName);

                // check that a FeatureMap connection exists
                // check that a FeatureMap connection exists
                // FeatureMap connections will be changed to PortCompositions during the import, so check for PortCompositions instead
                bool collection_empty = false;
                string collection_empty_msg = "";
                try
                {
                    Assert.NotEmpty(newcomp.Children.PortCompositionCollection);
                }
                catch (Exception ex)
                {
                    collection_empty = true;
                    collection_empty_msg = String.Format("Post Composition (formerly Feature Map) collection was empty: {0}", ex.Message);
                    newcomp.Delete();
                    importedcomponents.Delete();
                }
                Assert.False(collection_empty, collection_empty_msg);
                var postconnuutF = newcomp.Children.PortCompositionCollection
                                         .First();

                // check that a SurfaceReverseMap connection exists
                try
                {
                    Assert.NotEmpty(newcomp.Children.SurfaceReverseMapCollection);
                }
                catch (Exception ex)
                {
                    collection_empty = true;
                    collection_empty_msg = String.Format("Surface Reverse Map collection was empty: {0}", ex.Message);
                    newcomp.Delete();
                    importedcomponents.Delete();
                }
                Assert.False(collection_empty, collection_empty_msg);
                var postconnuutR = newcomp.Children.SurfaceReverseMapCollection
                                         .First();
                // cleanup
                newcomp.Delete();
                importedcomponents.Delete();

                // check that the Feature Map type is now a PortComposition
                Assert.True(postconnuutF.Properties.Kind == "PortComposition", String.Format("{0} kind changed from {1} to {2}", preconnuutF.Name, preconnuutF.Properties.Kind, postconnuutF.Properties.Kind));
                // check that the Surface Reverse Map type is correct
                Assert.True(preconnuutR.Properties.Kind == postconnuutR.Properties.Kind, String.Format("{0} kind changed from {1} to {2}", preconnuutR.Name, preconnuutR.Properties.Kind, postconnuutR.Properties.Kind));
            });
        }

        [Fact]
        [Trait("Interchange", "CAD")]
        [Trait("Interchange", "CAD Datum Map")]
        public void DoubleSurfaceNoReverseMapTest()
        {
            String testName = GetCurrentMethod();

            // Two surfaces configured as a Feature Map and a Port Composition
            fixture.proj.PerformInTransaction(delegate
            {
                // A single surface configured as a Feature Map
                var comp = fixture.proj.GetComponentsByName("DoubleSurfaceNoReverseMap")
                                       .First();
                var preconnuutF = comp.Children.FeatureMapCollection
                                         .Where(p => p.Name == "FeatureMap")
                                         .First();
                var preconnuutP = comp.Children.PortCompositionCollection
                         .Where(p => p.Name == "PortComposition")
                         .First();

                // Export then Import the component
                CyPhy.Components importedcomponents;
                CyPhy.Component newcomp = ExportThenImport(comp, out importedcomponents, testName);

                // check that a FeatureMap connection exists
                // FeatureMap connections will be changed to PortCompositions during the import, so check for PortCompositions instead
                bool collection_empty = false;
                string collection_empty_msg = "";
                try
                {
                    Assert.NotEmpty(newcomp.Children.PortCompositionCollection);
                }
                catch (Exception ex)
                {
                    collection_empty = true;
                    collection_empty_msg = String.Format("Post Composition (formerly Feature Map) collection was empty: {0}", ex.Message);
                    newcomp.Delete();
                    importedcomponents.Delete();
                }
                Assert.False(collection_empty, collection_empty_msg);
                var postconnuutF = newcomp.Children.PortCompositionCollection
                                         .First();

                // check that a PortComposition connection exists
                try
                {
                    Assert.NotEmpty(newcomp.Children.PortCompositionCollection);
                }
                catch (Exception ex)
                {
                    collection_empty = true;
                    collection_empty_msg = String.Format("Port Composition collection was empty: {0}", ex.Message);
                    newcomp.Delete();
                    importedcomponents.Delete();
                }
                Assert.False(collection_empty, collection_empty_msg);
                var postconnuutP = newcomp.Children.PortCompositionCollection
                                         .First();
                // cleanup
                newcomp.Delete();
                importedcomponents.Delete();

                // check that the Feature Map type is now a PortComposition
                Assert.True(postconnuutF.Properties.Kind == "PortComposition", String.Format("{0} kind changed from {1} to {2}", preconnuutF.Name, preconnuutF.Properties.Kind, postconnuutF.Properties.Kind));
                // check that the Port Composition type is correct
                Assert.True(preconnuutP.Properties.Kind == postconnuutP.Properties.Kind, String.Format("{0} kind changed from {1} to {2}", preconnuutP.Name, preconnuutP.Properties.Kind, postconnuutP.Properties.Kind));
            });
        }

        [Fact]
        [Trait("Interchange", "CAD")]
        [Trait("Interchange", "CAD Datum Map")]
        public void TripleSurfaceTest()
        {
            String testName = GetCurrentMethod();

            // Three surfaces, one each of a Feature Map, a Port Composition and a Surface Reverse Map
            fixture.proj.PerformInTransaction(delegate
            {
                // A single surface configured as a Feature Map
                var comp = fixture.proj.GetComponentsByName("TripleSurface")
                                       .First();
                var preconnuutF = comp.Children.FeatureMapCollection
                                         .Where(p => p.Name == "FeatureMap")
                                         .First();
                var preconnuutP = comp.Children.PortCompositionCollection
                         .Where(p => p.Name == "PortComposition")
                         .First();
                var preconnuutR = comp.Children.SurfaceReverseMapCollection
                         .Where(p => p.Name == "SurfaceReverseMap")
                         .First();

                // Export then Import the component
                CyPhy.Components importedcomponents;
                CyPhy.Component newcomp = ExportThenImport(comp, out importedcomponents, testName);

                // check that a FeatureMap connection exists
                // FeatureMap connections will be changed to PortCompositions during the import, so check for PortCompositions instead
                bool collection_empty = false;
                string collection_empty_msg = "";
                try
                {
                    Assert.NotEmpty(newcomp.Children.PortCompositionCollection);
                }
                catch (Exception ex)
                {
                    collection_empty = true;
                    collection_empty_msg = String.Format("Post Composition (formerly Feature Map) collection was empty: {0}", ex.Message);
                    newcomp.Delete();
                    importedcomponents.Delete();
                }
                Assert.False(collection_empty, collection_empty_msg);
                var postconnuutF = newcomp.Children.PortCompositionCollection
                                         .First();

                // check that a PortComposition connection exists
                try
                {
                    Assert.NotEmpty(newcomp.Children.PortCompositionCollection);
                }
                catch (Exception ex)
                {
                    collection_empty = true;
                    collection_empty_msg = String.Format("Port Composition collection was empty: {0}", ex.Message);
                    newcomp.Delete();
                    importedcomponents.Delete();
                }
                Assert.False(collection_empty, collection_empty_msg);
                var postconnuutP = newcomp.Children.PortCompositionCollection
                                         .First();

                // check that a SurfaceReverseMap connection exists
                try
                {
                    Assert.NotEmpty(newcomp.Children.SurfaceReverseMapCollection);
                }
                catch (Exception ex)
                {
                    collection_empty = true;
                    collection_empty_msg = String.Format("Surface Reverse Map collection was empty: {0}", ex.Message);
                    newcomp.Delete();
                    importedcomponents.Delete();
                }
                Assert.False(collection_empty, collection_empty_msg);
                var postconnuutR = newcomp.Children.SurfaceReverseMapCollection
                                         .First();
                // cleanup
                newcomp.Delete();
                importedcomponents.Delete();

                // check that the Feature Map type is now a PortComposition
                Assert.True(postconnuutF.Properties.Kind == "PortComposition", String.Format("{0} kind changed from {1} to {2}", preconnuutF.Name, preconnuutF.Properties.Kind, postconnuutF.Properties.Kind));
                // check that the Port Composition type is correct
                Assert.True(preconnuutP.Properties.Kind == postconnuutP.Properties.Kind, String.Format("{0} kind changed from {1} to {2}", preconnuutP.Name, preconnuutP.Properties.Kind, postconnuutP.Properties.Kind));
                // check that the Surface Reverse Map type is correct
                Assert.True(preconnuutR.Properties.Kind == postconnuutR.Properties.Kind, String.Format("{0} kind changed from {1} to {2}", preconnuutR.Name, preconnuutR.Properties.Kind, postconnuutR.Properties.Kind));
            });
        }

        [Fact]
        [Trait("Interchange", "CAD")]
        [Trait("Interchange", "CAD Datum Map")]
        public void ModelNamesTest()
        {
            String testName = GetCurrentMethod();

            List<string> pre_namelist = new List<string> { };
            List<string> post_namelist = new List<string> { };

            fixture.proj.PerformInTransaction(delegate
            {
                // A Component with every kind of model
                var comp = fixture.proj.GetComponentsByName("ModelNames")
                                       .First();

                // gather the names of each model
                if (comp.Children.CADModelCollection.Count() > 0)
                {
                    pre_namelist.Add(comp.Children.CADModelCollection.First().Name);
                }
                if (comp.Children.ManufacturingModelCollection.Count() > 0)
                {
                    pre_namelist.Add(comp.Children.ManufacturingModelCollection.First().Name);
                }
                if (comp.Children.ModelicaModelCollection.Count() > 0)
                {
                    pre_namelist.Add(comp.Children.ModelicaModelCollection.First().Name);
                }

                // Export then Import the component
                CyPhy.Components importedcomponents;
                CyPhy.Component newcomp = ExportThenImport(comp, out importedcomponents, testName);

                // capture the names of each model returned
                if (newcomp.Children.CADModelCollection.Count() > 0)
                {
                    post_namelist.Add(newcomp.Children.CADModelCollection.First().Name);
                }
                if (newcomp.Children.ManufacturingModelCollection.Count() > 0)
                {
                    post_namelist.Add(newcomp.Children.ManufacturingModelCollection.First().Name);
                }
                if (newcomp.Children.ModelicaModelCollection.Count() > 0)
                {
                    post_namelist.Add(newcomp.Children.ModelicaModelCollection.First().Name);
                }

                // compare the lists and see what names didn't make the round trip
                string list_of_missing_names = "These names were not found ";
                bool any_missing = false;
                foreach (string name in pre_namelist)
                {
                    if (!post_namelist.Contains(name))
                    {
                        list_of_missing_names += name;
                        list_of_missing_names += ", ";
                        any_missing = true;
                    }
                }

                // cleanup
                newcomp.Delete();
                importedcomponents.Delete();

                // report any failures
                Assert.False(any_missing, list_of_missing_names);
            });
        }
    }
}
