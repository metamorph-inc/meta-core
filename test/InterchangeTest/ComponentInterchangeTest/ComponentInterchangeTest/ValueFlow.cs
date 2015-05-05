using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Xunit;

namespace ComponentInterchangeTest
{
    public class ValueFlowModelImport
    {
        [Fact]
        [Trait("ProjectImport/Open", "ValueFlow")]
        public void ProjectXmeImport()
        {
            Assert.DoesNotThrow(() => { new ValueFlowFixture(); });
        }
    }

    public class ValueFlow : IUseFixture<ValueFlowFixture>
    {
        #region Path Variables
        public static readonly string testPath = Path.Combine(
            META.VersionInfo.MetaPath,
            "test",
            "InterchangeTest",
            "ComponentInterchangeTest",
            "SharedModels",
            "ValueFlow"
            );

        public static readonly string xmePath = Path.Combine(
            testPath,
            "ValueFlow.xme"
            );
        public static readonly string mgaPath = Path.Combine(
            testPath,
            "ValueFlow.mga"
            );
        #endregion

        #region Fixture
        ValueFlowFixture fixture;
        public void SetFixture(ValueFlowFixture data)
        {
            fixture = data;
        }
        #endregion
        
        [Fact]
        [Trait("Interchange","ValueFlow")]
        [Trait("Interchange","Component Export")]
        public void AllComponentsExported()
        {
            var exportedACMRoot = Path.Combine(testPath, "Imported_Components");
            var acmFiles = Directory.GetFiles(exportedACMRoot, "*.acm", SearchOption.AllDirectories);
            Assert.Equal(4, acmFiles.Length);
        }
        
        [Fact]
        public void ImportAll()
        {
            var importXmePath = Path.Combine(testPath,"InputModel.xme");
            var importMgaPath = CommonFunctions.unpackXme(importXmePath);
            Assert.True(File.Exists(importMgaPath),"MGA file not found. Model import may have failed.");
            
            var compFolderRoot = Path.Combine(testPath,"Imported_Components");
            int rtnCode = CommonFunctions.runCyPhyComponentImporterCLRecursively(importMgaPath, compFolderRoot);
            Assert.True(rtnCode == 0, String.Format("Importer failed on one or more components"));

            Assert.True(0 == CommonFunctions.RunCyPhyMLComparator(mgaPath, importMgaPath), "Imported model doesn't match expected.");
        }
    }
}
