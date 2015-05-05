using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Xunit;

namespace ComponentInterchangeTest
{
    public class Formulas : IUseFixture<FormulasFixture>
    {
        #region Path Variables
        public static readonly string testPath = Path.Combine(
            META.VersionInfo.MetaPath,
            "test",
            "InterchangeTest",
            "ComponentInterchangeTest",
            "SharedModels",
            "Formulas"
            );
        public static readonly string xmePath = Path.Combine(
            testPath,
            "Formulas.xme"
            );
        public static readonly string mgaPath = Path.Combine(
            testPath,
            "Formulas.mga"
            );
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
            META.VersionInfo.MetaPath,
            "test",
            "InterchangeTest",
            "ComponentInterchangeTest",
            "SharedModels",
            "Formulas",
            "InputModel.mga"
            );
        #endregion

        #region Fixture
        FormulasFixture fixture;
        public void SetFixture(FormulasFixture data)
        {
            fixture = data;
        }
        #endregion
        
        [Fact]
        public void ModelImported()
        {
            // Fixture does the heavy lifting
        }
        
        [Fact]
        public void RoundTripTest()
        {
            Assert.True(0 == CommonFunctions.RunCyPhyMLComparator(mgaPath, mgaPath_InputModel), "Imported models don't match expected.");
        }
    }

    public class FormulasModelImport
    {
        [Fact]
        [Trait("ProjectImport/Open", "Formulas")]
        public void ProjectXmeImport()
        {
            Assert.DoesNotThrow(() => { new FormulasFixture(); });
        }
    }

    public class FormulasFixture
    {
        public FormulasFixture()
        {
            var compDir = Path.Combine(Formulas.testPath, "Imported_Components");

            // Clear the Components folder
            try
            {
                if (Directory.Exists(compDir))
                    Directory.Delete(compDir, true);
            }
            catch (Exception ex)
            {
                // Results will be unreliable. Might as well quit now.
                throw ex;
            }

            // Import the model.
            File.Delete(Formulas.mgaPath);
            GME.MGA.MgaUtils.ImportXME(Formulas.xmePath, Formulas.mgaPath);
            Assert.True(File.Exists(Formulas.mgaPath), String.Format("{0} not found. Model import may have failed.", Formulas.mgaPath));

            // Export the components
            var returnCode = CommonFunctions.runCyPhyComponentExporterCL(Formulas.mgaPath);
            Assert.True(0 == returnCode, "Exporter had non-zero return code of " + returnCode);

            // Import the blank model.
            File.Delete(Formulas.mgaPath_InputModel);
            GME.MGA.MgaUtils.ImportXME(Formulas.xmePath_InputModel, Formulas.mgaPath_InputModel);
            Assert.True(File.Exists(Formulas.mgaPath_InputModel), String.Format("{0} not found. Model import may have failed.", Formulas.mgaPath_InputModel));

            // Import the components to the blank model
            Assert.True(0 == CommonFunctions.runCyPhyComponentImporterCLRecursively(Formulas.mgaPath_InputModel, compDir), "Component Importer had non-zero return code.");            
        }
    }
}
