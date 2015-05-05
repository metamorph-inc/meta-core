using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using System.Threading.Tasks;
using System.Collections.Concurrent;

namespace ComponentInterchangeTest
{
    public class ID_HandlingFixture
    {
        public ID_HandlingFixture()
        {
            // Clear the Components folder
            var compFolder = Path.Combine(ID_Handling.testPath, "Imported_Components");
            try
            {
                if (Directory.Exists(compFolder))
                    Directory.Delete(compFolder, true);
            }
            catch (Exception ex)
            {
                // Results will be unreliable unless that folder was deleted; Better quit now.
                throw ex;
            }

            // Import the model.
            GME.MGA.MgaUtils.ImportXME(ID_Handling.xmePath, ID_Handling.mgaPath);
            Assert.True(File.Exists(ID_Handling.mgaPath), "MGA file not found; Import may have failed.");

            // Export the components
            Assert.True(0 == CommonFunctions.runCyPhyComponentExporterCL(ID_Handling.mgaPath), "Component Exporter had non-zero return code.");
        }
    }

    public class ID_HandlingModelImport
    {
        [Fact]
        [Trait("ProjectImport/Open", "ID_Handling")]
        public void ProjectXmeImport()
        {
            Assert.DoesNotThrow(() => { new ID_HandlingFixture(); });
        }
    }

    public class ID_Handling : IUseFixture<ID_HandlingFixture>
    {
        #region Path Variables
        public static readonly string testPath = Path.Combine(
            META.VersionInfo.MetaPath,
            "test",
            "InterchangeTest",
            "ComponentInterchangeTest",
            "SharedModels",
            "ID_Handling");

        public static readonly string xmePath = Path.Combine(
            testPath,
            "ID_Handling.xme");

        public static readonly string mgaPath = Path.Combine(
            testPath,
            "ID_Handling.mga");
        #endregion

        #region Fixture
        ID_HandlingFixture fixture;
        public void SetFixture(ID_HandlingFixture data)
        {
            fixture = data;
        }
        #endregion
        
        [Fact]
        [Trait("Interchange","ID Handling")]
        [Trait("Interchange","Component Export")]
        public void AllComponentsExported()
        {
            var exportedACMRoot = Path.Combine(testPath, "Imported_Components");
            var acmFiles = Directory.GetFiles(exportedACMRoot, "*.acm", SearchOption.AllDirectories);
            Assert.Equal(30, acmFiles.Length);
        }

        [Fact]
        public void ImportAll()
        {
            var importXmePath = Path.Combine(testPath,"ImportModel.xme");
            var importMgaPath = CommonFunctions.unpackXme(importXmePath);
            Assert.True(File.Exists(importMgaPath),"MGA file not found. Model import may have failed.");
            
            var compFolderRoot = Path.Combine(testPath,"Imported_Components");
            int rtnCode = CommonFunctions.runCyPhyComponentImporterCLRecursively(importMgaPath, compFolderRoot);
            Assert.True(rtnCode == 0, String.Format("Importer failed on one or more components"));
        }

    }
}
