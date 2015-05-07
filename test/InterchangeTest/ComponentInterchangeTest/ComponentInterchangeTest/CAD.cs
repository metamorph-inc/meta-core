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

namespace ComponentInterchangeTest
{
    public class CADFixture
    {
        public CADFixture()
        {
            // Clear the Components folder
            try
            {
                Directory.Delete(Path.Combine(CAD.testPath, "Imported_Components"), true);
            }
            catch (DirectoryNotFoundException)
            {
                // It's okay if it didn't exist.
            }
            catch (Exception ex)
            {
                Assert.True(false, String.Format("{0}: {1}", ex.GetType().Name, ex.Message));
                return;
            }

            // Import the model.
            mgaPath = CommonFunctions.unpackXme(CAD.xmePath);

            // Export the components
            Assert.Equal(0, CommonFunctions.runCyPhyComponentExporterCL(mgaPath));
        }

        public string mgaPath { get; private set; }
    }

    public class CAD : IUseFixture<CADFixture>
    {
        #region Path Variables
        public static readonly string testPath = Path.Combine(
            META.VersionInfo.MetaPath,
            "test",
            "InterchangeTest",
            "ComponentInterchangeTest",
            "SharedModels",
            "CAD"
            );

        public static readonly string xmePath = Path.Combine(
            testPath,
            "CAD.xme"
            );

        #endregion

        #region fixture
        CADFixture fixture;
        public void SetFixture(CADFixture data)
        {
            fixture = data;
        }
        #endregion

        [Fact]
        [Trait("Interchange","CAD")]
        [Trait("Interchange","Component Export")]
        public void AllComponentsExported()
        {
            var exportedACMRoot = Path.Combine(testPath, "Imported_Components");
            var acmFiles = Directory.GetFiles(exportedACMRoot, "*.acm", SearchOption.AllDirectories);
            Assert.Equal(4, acmFiles.Length);
        }
        
        [Fact]
        [Trait("Interchange", "CAD")]
        public void ImportAll()
        {
            var importXmePath = Path.Combine(testPath,"InputModel.xme");
            var importMgaPath = CommonFunctions.unpackXme(importXmePath);
            Assert.True(File.Exists(importMgaPath),"MGA file not found. Model import may have failed.");
            
            var compFolderRoot = Path.Combine(testPath,"Imported_Components");
            int rtnCode = CommonFunctions.runCyPhyComponentImporterCLRecursively(importMgaPath, compFolderRoot);
            Assert.True(rtnCode == 0, String.Format("Importer failed on one or more components"));

            var desiredMgaPath = Path.Combine(testPath, "CAD.mga");
            Assert.True(0 == CommonFunctions.RunCyPhyMLComparator(desiredMgaPath, importMgaPath), "Imported model doesn't match expected.");
        }

        [Fact]
        [Trait("Interchange", "CAD")]
        public void ImportCADMetric_ValueFixed()
        {
            String cadMetricName = "TOTAL_MASS_Fixed";
            Double expectedVal = 1.0;
            String testModelName = "test_model_fixed.mga";
            ImportAndTestCADMetric(cadMetricName, expectedVal, testModelName);
        }

        [Fact]
        [Trait("Interchange", "CAD")]
        [Trait("JIRA", "META-2823")]
        public void ImportCADMetric_ValueParametric()
        {
            String cadMetricName = "TOTAL_MASS_Param";
            Double expectedVal = 1.0;
            String testModelName = "test_model_param.mga";
            ImportAndTestCADMetric(cadMetricName, expectedVal, testModelName);
        }

        private static void ImportAndTestCADMetric(String cadMetricName, Double expectedVal, String testModelName)
        {

            var testPath = Path.Combine(META.VersionInfo.MetaPath,
                                        "test",
                                        "InterchangeTest",
                                        "ComponentInterchangeTest",
                                        "ImportTestModels",
                                        "CADMetric");
            var testModel = Path.Combine(testPath,
                                         testModelName);
            var testACM = Path.Combine(testPath,
                                       "InputModel.acm");

            GME.MGA.MgaProject proj = new GME.MGA.MgaProjectClass();

            String connString = "MGA=" + testModel;
            proj.Create(connString, "CyPhyML");

            proj.PerformInTransaction(delegate
            {
                var rf = CyPhyClasses.RootFolder.GetRootFolder(proj);
                var cf = CyPhyClasses.Components.Create(rf);
                cf.Name = "Components";

                avm.Component acm = null;
                using (var reader = new StreamReader(testACM))
                {
                    acm = CyPhyComponentImporter.CyPhyComponentImporterInterpreter.DeserializeAvmComponentXml(reader);
                }
                var comp = CyPhy2ComponentModel.Convert.AVMComponent2CyPhyML(cf, acm);

                var cm_param = comp.Children
                                   .CADModelCollection
                                   .FirstOrDefault()
                                   .Children
                                   .CADMetricCollection
                                   .Where(cm => cm.Name == cadMetricName)
                                   .FirstOrDefault();

                Assert.True(cm_param != null,
                            String.Format("Could not find CAD Metric: {0}", cadMetricName));

                Assert.False(String.IsNullOrWhiteSpace(cm_param.Attributes.Value),
                             String.Format("CAD Metric \"{0}\" has a null or whitespace value",
                                           cadMetricName));

                var cm_paramVal = Double.Parse(cm_param.Attributes.Value);

                Assert.True(cm_paramVal == expectedVal,
                            String.Format("CAD Metric \"{0}\" has wrong value ({1} expected, {2} found)",
                                          cadMetricName,
                                          expectedVal,
                                          cm_param.Attributes.Value));

                var dstProp = cm_param.DstConnections
                                      .CADMetricPortMapCollection
                                      .FirstOrDefault()
                                      .DstEnds
                                      .Property;
                
                var dstPropVal = Double.Parse(dstProp.Attributes.Value);

                Assert.True(dstPropVal == cm_paramVal, "CADMetric value didn't propagate.");
            });
            proj.Save();
            proj.Close();
        }       
    }
}
