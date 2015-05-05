using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Xunit;

namespace ComponentInterchangeTest
{
    public class ManufacturingFixture
    {
        public ManufacturingFixture()
        {
            // Import the GME Models
            File.Delete(Manufacturing.mgaPath_InputModel);
            GME.MGA.MgaUtils.ImportXME(Manufacturing.xmePath_InputModel, Manufacturing.mgaPath_InputModel);
            
            File.Delete(Manufacturing.mgaPath_DesiredResult);
            GME.MGA.MgaUtils.ImportXME(Manufacturing.xmePath_DesiredResult, Manufacturing.mgaPath_DesiredResult);
            
            Assert.True(File.Exists(Manufacturing.mgaPath_InputModel), String.Format("{0} not found. Model import may have failed.", Manufacturing.mgaPath_InputModel));
            Assert.True(File.Exists(Manufacturing.mgaPath_DesiredResult), String.Format("{0} not found. Model import may have failed.", Manufacturing.mgaPath_DesiredResult));

            // Clear exported components folder
            if (Directory.Exists(Manufacturing.path_ExportedComponents))
                Directory.Delete(Manufacturing.path_ExportedComponents, true);

            // Export the Manuf model.
            var rtnCode_Exporter = CommonFunctions.runCyPhyComponentExporterCL(Manufacturing.mgaPath_DesiredResult);
            Assert.True(0 == rtnCode_Exporter, "Component Exporter had a non-zero return code of " + rtnCode_Exporter);

            // Import to the blank model
            var rtnCode_Importer = CommonFunctions.runCyPhyComponentImporterCLRecursively(Manufacturing.mgaPath_InputModel, Manufacturing.path_ExportedComponents);
            Assert.True(0 == rtnCode_Importer, "Component Importer had a non-zero return code of " + rtnCode_Importer);
        }
    }

    public class ManufacturingModelImport
    {
        [Fact]
        [Trait("ProjectImport/Open", "Manufacturing")]
        public void ProjectXmeImport()
        {
            Assert.DoesNotThrow(() => { new ManufacturingFixture(); });
        }
    }

    public class Manufacturing : IUseFixture<ManufacturingFixture>
    {
        #region Path Variables
        public static readonly string testPath = Path.Combine(
            META.VersionInfo.MetaPath,
            "test",
            "InterchangeTest",
            "ComponentInterchangeTest",
            "SharedModels",
            "Manufacturing"
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

        public static readonly string path_ExportedComponents = Path.Combine(
            testPath,
            "Imported_Components"
            );
        #endregion

        #region Fixture
        ManufacturingFixture fixture;
        public void SetFixture(ManufacturingFixture data)
        {
            fixture = data;
        }
        #endregion

        [Fact]
        [Trait("Interchange", "Manufacturing")]
        public void ImportAll()
        {
            Assert.True(0 == CommonFunctions.RunCyPhyMLComparator(mgaPath_DesiredResult, mgaPath_InputModel), "Imported model doesn't match expected.");
        }
    }
}
