using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;

namespace ComponentInterchangeTest
{
    public class GuideDatumFixture
    {
        public GuideDatumFixture()
        {
            // Import the GME Models
            File.Delete(GuideDatum.mgaPath_InputModel);
            GME.MGA.MgaUtils.ImportXME(GuideDatum.xmePath_InputModel, GuideDatum.mgaPath_InputModel);

            File.Delete(GuideDatum.mgaPath_DesiredResult);
            GME.MGA.MgaUtils.ImportXME(GuideDatum.xmePath_DesiredResult, GuideDatum.mgaPath_DesiredResult);

            Assert.True(File.Exists(GuideDatum.mgaPath_InputModel), String.Format("{0} not found. Model import may have failed.", GuideDatum.mgaPath_InputModel));
            Assert.True(File.Exists(GuideDatum.mgaPath_DesiredResult), String.Format("{0} not found. Model import may have failed.", GuideDatum.mgaPath_DesiredResult));

            // Clear exported components folder
            if (File.Exists(GuideDatum.path_ExportedACM))
            {
                File.Delete(GuideDatum.path_ExportedACM);
            }

            // Export the Manuf model.
            var rtnCode_Exporter = CommonFunctions.runCyPhyComponentExporterCL(GuideDatum.mgaPath_DesiredResult);
            Assert.True(0 == rtnCode_Exporter, "Component Exporter had a non-zero return code of " + rtnCode_Exporter);

            // Import to the blank model
            var rtnCode_Importer = CommonFunctions.runCyPhyComponentImporterCL(GuideDatum.mgaPath_InputModel, GuideDatum.path_ExportedACM);
            Assert.True(0 == rtnCode_Importer, "Component Importer had a non-zero return code of " + rtnCode_Importer);
        }
    }

    public class GuideDatumModelImport
    {
        [Fact]
        [Trait("ProjectImport/Open", "GuideDatum")]
        public void ProjectXmeImport()
        {
            Assert.DoesNotThrow(() => { new GuideDatumFixture(); });
        }
    }

    public class GuideDatum : IUseFixture<GuideDatumFixture>
    {
        #region Path Variables
        public static readonly string testPath = Path.Combine(
            META.VersionInfo.MetaPath,
            "test",
            "InterchangeTest",
            "ComponentInterchangeTest",
            "SharedModels",
            "GuideDatum"
            );

        public static readonly string xmePath_DesiredResult = Path.Combine(
            testPath,
            "DesiredResult.xme"
            );

        public static readonly string mgaPath_DesiredResult = xmePath_DesiredResult.Replace(".xme", ".mga");

        public static readonly string xmePath_InputModel = Path.Combine(
            META.VersionInfo.MetaPath,
            "test",
            "InterchangeTest",
            "ComponentInterchangeTest",
            "SharedModels",
            "BlankInputModel",
            "InputModel.xme"
            );

        public static readonly string mgaPath_InputModel = Path.Combine(
            testPath,
            "InputModel.mga"
            );

        public static readonly string path_ExportedACM = Path.Combine(testPath,
                                       "Imported_Components",
                                       "HasGuideDatums",
                                       "HasGuideDatums.component.acm");
        #endregion

        #region Fixture
        GuideDatumFixture fixture;
        public void SetFixture(GuideDatumFixture data)
        {
            fixture = data;
        }
        #endregion

        [Fact]
        [Trait("Interchange", "GuideDatum")]
        public void RoundTrip()
        {
            Assert.True(0 == CommonFunctions.RunCyPhyMLComparator(mgaPath_DesiredResult, mgaPath_InputModel), "Imported model doesn't match expected.");
        }

        [Fact]
        [Trait("Interchange", "GuideDatum")]
        public void CheckExportedACM()
        {
            avm.Component acm = null;
            using (var reader = new StreamReader(path_ExportedACM))
            {
                acm = CyPhyComponentImporter.CyPhyComponentImporterInterpreter.DeserializeAvmComponentXml(reader);
            }

            Assert.Equal(1, acm.Connector.Count);
            var connector = acm.Connector.FirstOrDefault();

            Assert.Equal(8, connector.Role.Count);
            var datums = connector.Role.OfType<avm.cad.Datum>();
            Assert.Equal(8, datums.Count());

            var guideDatums = connector.ConnectorFeature.OfType<avm.cad.GuideDatum>();
            Assert.Equal(4, guideDatums.Count());

            foreach (var guideDatum in guideDatums)
            {
                var linkedDatum = datums.FirstOrDefault(x => x.ID == guideDatum.Datum);
                Assert.NotNull(linkedDatum);
                Assert.True(linkedDatum.Name.StartsWith("GD_"));
            }
        }
    }
}
