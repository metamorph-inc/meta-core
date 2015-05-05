using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;

namespace ComponentExporterUnitTests
{
    public class ModelicaStructuresFixture
    {
        public ModelicaStructuresFixture()
        {
            // Clear the Components folder
            var compFolder = Path.Combine(ModelicaStructures.testPath, "Imported_Components");
            try
            {
                if (Directory.Exists(compFolder))
                    Directory.Delete(compFolder, true);
            }
            catch (IOException ex)
            {
                // Results will be unreliable. Might as well quit now.
                throw ex;
            }

            // Import the model.
            GME.MGA.MgaUtils.ImportXME(ModelicaStructures.xmePath, ModelicaStructures.mgaPath);
            Assert.True(File.Exists(ModelicaStructures.mgaPath), "MGA file not found; Import may have failed.");

            // Export the components
            Assert.True(0 == Common.runCyPhyComponentExporterCL(ModelicaStructures.mgaPath), "Component Exporter had non-zero return code");
        }
    }

    public class ModelicaStructures : IUseFixture<ModelicaStructuresFixture>
    {
        #region Path Variables
        public static readonly string testPath = Path.Combine(
            META.VersionInfo.MetaPath, 
            "test", 
            "InterchangeTest", 
            "ComponentInterchangeTest", 
            "SharedModels", 
            "Modelica");

        public static readonly string xmePath = Path.Combine(
            testPath,
            "Modelica.xme");

        public static readonly string mgaPath = Path.Combine(
            testPath,
            "Modelica.mga");
        #endregion
        
        #region Fixture
        ModelicaStructuresFixture fixture;
        public void SetFixture(ModelicaStructuresFixture data)
        {
            fixture = data;
        }
        #endregion
        
        [Fact]
        [Trait("Interchange", "Modelica")]
        [Trait("Interchange", "Component Export")]
        public void ModelRedeclareExport()
        {
            var generatedACM = Path.Combine(testPath, "Imported_Components", "Comp_ModelRedeclare", "Comp_ModelRedeclare.component.acm");
            var expectedACM = Path.Combine(testPath, "Comp_ModelRedeclare.expected.acm");

            Assert.True(File.Exists(generatedACM), "Exported ACM file not found.");
            Assert.True(0 == Common.RunXmlComparator(generatedACM, expectedACM), "Generated ACM file doesn't match expected output.");
        }

        [Fact]
        [Trait("Interchange", "Modelica")]
        [Trait("Interchange", "Component Export")]
        public void PortRedeclareExport()
        {
            var generatedACM = Path.Combine(testPath, "Imported_Components", "Comp_PortRedeclare", "Comp_PortRedeclare.component.acm");
            var expectedACM = Path.Combine(testPath, "Comp_PortRedeclare.expected.acm");

            Assert.True(File.Exists(generatedACM), "Exported ACM file not found.");
            Assert.True(0 == Common.RunXmlComparator(generatedACM, expectedACM), "Generated ACM file doesn't match expected output.");
        }

        [Fact]
        [Trait("Interchange", "Modelica")]
        [Trait("Interchange", "Component Export")]
        public void PortRedeclareAtCompLevelExport()
        {
            var generatedACM = Path.Combine(testPath, "Imported_Components", "Comp_PortRedeclareAtCompLvl", "Comp_PortRedeclareAtCompLvl.component.acm");
            var expectedACM = Path.Combine(testPath, "Comp_PortRedeclareAtCompLvl.expected.acm");

            Assert.True(File.Exists(generatedACM), "Exported ACM file not found.");
            Assert.True(0 == Common.RunXmlComparator(generatedACM, expectedACM), "Generated ACM file doesn't match expected output.");
        }
    }
}
