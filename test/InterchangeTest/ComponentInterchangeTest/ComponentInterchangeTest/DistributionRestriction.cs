using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using GME.MGA;
using System.IO;
using META;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace ComponentInterchangeTest
{
    public class DistributionRestrictionFixture : IDisposable
    {
        public static string testPath = Path.Combine(META.VersionInfo.MetaPath,
                                                     "test",
                                                     "InterchangeTest",
                                                     "ComponentInterchangeTest",
                                                     "SharedModels",
                                                     "DistributionRestrictions");

        private string xmePath = Path.Combine(testPath, "DistributionRestrictions.xme");
        public string mgaPath { get; private set; }
        public MgaProject proj { get; private set; }

        public DistributionRestrictionFixture()
        {
            String mgaConnectionString;
            GME.MGA.MgaUtils.ImportXMEForTest(xmePath, out mgaConnectionString);
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
    }

    public class DistributionRestrictionFixtureModelImport
    {
        [Fact]
        [Trait("ProjectImport/Open", "DistributionRestriction")]
        public void ProjectXmeImport()
        {
            Assert.DoesNotThrow(() => { 
                var fixture = new DistributionRestrictionFixture();
                fixture.Dispose();
            });
        }
    }

    public class DistributionRestriction : IUseFixture<DistributionRestrictionFixture>
    {
        #region Fixture
        DistributionRestrictionFixture fixture;
        public void SetFixture(DistributionRestrictionFixture data)
        {
            fixture = data;
        }
        #endregion

        private string testPath
        {
            get
            {
                return DistributionRestrictionFixture.testPath;
            }
        }

        private MgaProject proj
        {
            get
            {
                return fixture.proj;
            }
        }

        [Fact]
        public void ImportOldITARStatement_NotITAR()
        {
            var acmPath = Path.Combine(testPath, "NotITAR.oldstyle.acm");

            IMgaFCO fco = null;
            proj.PerformInTransaction(delegate
            {
                var importer = new CyPhyComponentImporter.CyPhyComponentImporterInterpreter();
                importer.Initialize(fixture.proj);

                fco = importer.ImportFiles(fixture.proj, fixture.proj.GetRootDirectoryPath(), new[] { acmPath }, doNotReplaceAll: true)[1];
            });

            proj.PerformInTransaction(delegate
            {
                var comp = CyPhyClasses.Component.Cast(fco);

                Assert.False(comp.Children.ITARCollection.Any());
                Assert.False(comp.Children.DoDDistributionStatementCollection.Any());
            });
        }

        [Fact]
        public void ImportOldITARStatement_ITAR()
        {
            // There should be only an ITAR object, with attribute set to ITAR
            var acmPath = Path.Combine(testPath, "ITAR.oldstyle.acm");

            IMgaFCO fco = null;
            proj.PerformInTransaction(delegate
            {
                var importer = new CyPhyComponentImporter.CyPhyComponentImporterInterpreter();
                importer.Initialize(fixture.proj);
                fco = importer.ImportFiles(fixture.proj, fixture.proj.GetRootDirectoryPath(), new[] { acmPath }, doNotReplaceAll: true)[1];
            });

            proj.PerformInTransaction(delegate
            {
                var comp = CyPhyClasses.Component.Cast(fco);

                var itarCollection = comp.Children.ITARCollection;
                Assert.True(itarCollection.Count() == 1);

                var itar = itarCollection.First();
                Assert.True(itar.Attributes.RestrictionLevel == CyPhyClasses.ITAR.AttributesClass.RestrictionLevel_enum.ITAR);

                Assert.False(comp.Children.DoDDistributionStatementCollection.Any());
            });
        }

        [Fact]
        public void ImportOldITARStatement_ITARDistributionD()
        {
            // There should be both an ITAR element and a DistributionD element
            var acmPath = Path.Combine(testPath, "DistD.oldstyle.acm");

            IMgaFCO fco = null;
            proj.PerformInTransaction(delegate
            {
                var importer = new CyPhyComponentImporter.CyPhyComponentImporterInterpreter();
                importer.Initialize(fixture.proj);

                fco = importer.ImportFiles(fixture.proj, fixture.proj.GetRootDirectoryPath(), new[] { acmPath }, doNotReplaceAll: true)[1];
            });

            proj.PerformInTransaction(delegate
            {
                var comp = CyPhyClasses.Component.Cast(fco);

                var itarCollection = comp.Children.ITARCollection;
                Assert.True(itarCollection.Count() == 1);

                var itar = itarCollection.First();
                Assert.True(itar.Attributes.RestrictionLevel == CyPhyClasses.ITAR.AttributesClass.RestrictionLevel_enum.ITAR);

                var distStatementCollection = comp.Children.DoDDistributionStatementCollection;
                Assert.True(distStatementCollection.Count() == 1);

                var distStatement = distStatementCollection.First();
                Assert.True(distStatement.Attributes.DoDDistributionStatementEnum == CyPhyClasses.DoDDistributionStatement.AttributesClass.DoDDistributionStatementEnum_enum.StatementD);
            });
        }

        [Fact]
        public void ExportOldITARStatement_NotITAR()
        {
            string compName = "NotITAR";
            string compFolName = "Components_oldstyle";

            var avmComp = FindAndConvert(compName, compFolName);

            Assert.False(avmComp.DistributionRestriction.Any());            
        }


        [Fact]
        public void ExportOldITARStatement_ITAR()
        {
            string compName = "ITAR";
            string compFolName = "Components_oldstyle";

            var avmComp = FindAndConvert(compName, compFolName);

            Assert.True(avmComp.DistributionRestriction.OfType<avm.ITAR>().Count() == 1);
            Assert.False(avmComp.DistributionRestriction.OfType<avm.DoDDistributionStatement>().Any());
        }

        [Fact]
        public void ExportOldITARStatement_ITARDistD()
        {
            string compName = "DistD";
            string compFolName = "Components_oldstyle";

            var avmComp = FindAndConvert(compName, compFolName);

            Assert.True(avmComp.DistributionRestriction.OfType<avm.ITAR>().Count() == 1);

            var distStatementCollection = avmComp.DistributionRestriction.OfType<avm.DoDDistributionStatement>();

            Assert.True(distStatementCollection.Count() == 1);
            Assert.True(distStatementCollection.First().Type == avm.DoDDistributionStatementEnum.StatementD);
        }

        [Fact]
        public void Export_NewStyle_Dist()
        {
            string compName = "Dist";
            string compFolName = "Components_newstyle";

            var avmComp = FindAndConvert(compName, compFolName);

            Assert.False(avmComp.DistributionRestriction.OfType<avm.ITAR>().Any());

            var distStatementCollection = avmComp.DistributionRestriction.OfType<avm.DoDDistributionStatement>();
            Assert.True(distStatementCollection.Count() == 1);
            Assert.True(distStatementCollection.First().Type == avm.DoDDistributionStatementEnum.StatementB);
        }

        [Fact]
        public void Export_NewStyle_ITAR()
        {
            string compName = "ITAR";
            string compFolName = "Components_newstyle";

            var avmComp = FindAndConvert(compName, compFolName);

            var itarCollection = avmComp.DistributionRestriction.OfType<avm.ITAR>();
            Assert.True(itarCollection.Count() == 1);

            Assert.False(avmComp.DistributionRestriction.OfType<avm.DoDDistributionStatement>().Any());
        }

        [Fact]
        public void Export_NewStyle_ITARDist()
        {
            string compName = "ITARDist";
            string compFolName = "Components_newstyle";

            var avmComp = FindAndConvert(compName, compFolName);

            var itarCollection = avmComp.DistributionRestriction.OfType<avm.ITAR>();
            Assert.True(itarCollection.Count() == 1);

            var distStatementCollection = avmComp.DistributionRestriction.OfType<avm.DoDDistributionStatement>();
            Assert.True(distStatementCollection.Count() == 1);
            Assert.True(distStatementCollection.First().Type == avm.DoDDistributionStatementEnum.StatementD);
        }

        [Fact]
        public void Export_NewStyle_NotITAR()
        {
            string compName = "NotITAR";
            string compFolName = "Components_newstyle";

            var avmComp = FindAndConvert(compName, compFolName);

            Assert.False(avmComp.DistributionRestriction.Any());
        }

        private avm.Component FindAndConvert(string compName, string compFolName)
        {
            avm.Component avmComp = null;

            proj.PerformInTransaction(delegate
            {
                CyPhy.RootFolder rf = CyPhyClasses.RootFolder.GetRootFolder(proj);
                CyPhy.Components cf = rf.Children.ComponentsCollection.First(f => f.Name == compFolName);
                CyPhy.Component comp = cf.Children.ComponentCollection.First(c => c.Name == compName);

                avmComp = CyPhy2ComponentModel.Convert.CyPhyML2AVMComponent(comp);
            });

            string outPath = Path.Combine(testPath, "output");
            if (!Directory.Exists(outPath))
            {
                Directory.CreateDirectory(outPath);
            }

            CyPhyComponentExporter.CyPhyComponentExporterInterpreter.SerializeAvmComponent(avmComp, Path.Combine(outPath,
                                                                                                                 String.Format("{0}-{1}.exported.acm",
                                                                                                                               compFolName, 
                                                                                                                               compName)));

            return avmComp;
        }

        [Fact]
        public void Import_NewStyle_NotITAR()
        {
            proj.PerformInTransaction(delegate
            {
                CyPhy.RootFolder rf = CyPhyClasses.RootFolder.GetRootFolder(proj);
                CyPhy.Components cf = rf.Children.ComponentsCollection.First(f => f.Name == "Imported_Components");

                avm.Component avmComp = new avm.Component()
                {
                    Name = "Imported_NewStyle_NotITAR"
                };

                var cyphyComp = CyPhy2ComponentModel.Convert.AVMComponent2CyPhyML(cf, avmComp);

                Assert.False(cyphyComp.Children.DistributionRestrictionCollection.Any());
            });
        }

        [Fact]
        public void Import_NewStyle_ITAR()
        {
            proj.PerformInTransaction(delegate
            {
                CyPhy.RootFolder rf = CyPhyClasses.RootFolder.GetRootFolder(proj);
                CyPhy.Components cf = rf.Children.ComponentsCollection.First(f => f.Name == "Imported_Components");

                avm.Component avmComp = new avm.Component()
                {
                    Name = "Imported_NewStyle_ITAR"
                };
                avmComp.DistributionRestriction.Add(new avm.ITAR());

                var cyphyComp = CyPhy2ComponentModel.Convert.AVMComponent2CyPhyML(cf, avmComp);

                var itarCollection = cyphyComp.Children.ITARCollection;
                Assert.True(itarCollection.Count() == 1);
                Assert.True(itarCollection.First().Attributes.RestrictionLevel == CyPhyClasses.ITAR.AttributesClass.RestrictionLevel_enum.ITAR);

                Assert.False(cyphyComp.Children.DoDDistributionStatementCollection.Any());
            });
        }

        [Fact]
        public void Import_NewStyle_ITARDist()
        {
            string testNote = "STATEMENT A -- test note";

            proj.PerformInTransaction(delegate
            {
                CyPhy.RootFolder rf = CyPhyClasses.RootFolder.GetRootFolder(proj);
                CyPhy.Components cf = rf.Children.ComponentsCollection.First(f => f.Name == "Imported_Components");

                avm.Component avmComp = new avm.Component()
                {
                    Name = "Imported_NewStyle_ITARDist"
                };
                avmComp.DistributionRestriction.Add(new avm.ITAR());
                avmComp.DistributionRestriction.Add(new avm.DoDDistributionStatement()
                {
                    Type = avm.DoDDistributionStatementEnum.StatementA,
                    Notes = testNote
                });

                var cyphyComp = CyPhy2ComponentModel.Convert.AVMComponent2CyPhyML(cf, avmComp);

                var itarCollection = cyphyComp.Children.ITARCollection;
                Assert.True(itarCollection.Count() == 1);
                Assert.True(itarCollection.First().Attributes.RestrictionLevel == CyPhyClasses.ITAR.AttributesClass.RestrictionLevel_enum.ITAR);

                var distCollection = cyphyComp.Children.DoDDistributionStatementCollection;
                Assert.True(distCollection.Count() == 1);
                Assert.True(distCollection.First().Attributes.DoDDistributionStatementEnum
                            == CyPhyClasses.DoDDistributionStatement.AttributesClass.DoDDistributionStatementEnum_enum.StatementA);
                Assert.Equal(testNote, distCollection.First().Attributes.Notes);
            });
        }

        [Fact]
        public void Import_NewStyle_Dist()
        {
            string testNote = "STATEMENT C -- test note";

            proj.PerformInTransaction(delegate
            {
                CyPhy.RootFolder rf = CyPhyClasses.RootFolder.GetRootFolder(proj);
                CyPhy.Components cf = rf.Children.ComponentsCollection.First(f => f.Name == "Imported_Components");

                avm.Component avmComp = new avm.Component()
                {
                    Name = "Imported_NewStyle_Dist"
                };
                avmComp.DistributionRestriction.Add(new avm.DoDDistributionStatement()
                {
                    Type = avm.DoDDistributionStatementEnum.StatementC,
                    Notes = testNote
                });

                var cyphyComp = CyPhy2ComponentModel.Convert.AVMComponent2CyPhyML(cf, avmComp);

                Assert.False(cyphyComp.Children.ITARCollection.Any());

                var distCollection = cyphyComp.Children.DoDDistributionStatementCollection;
                Assert.True(distCollection.Count() == 1);
                Assert.True(distCollection.First().Attributes.DoDDistributionStatementEnum
                            == CyPhyClasses.DoDDistributionStatement.AttributesClass.DoDDistributionStatementEnum_enum.StatementC);
                Assert.Equal(testNote, distCollection.First().Attributes.Notes);
            });
        }
    }
}
