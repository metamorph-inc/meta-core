using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Xunit;

namespace ComponentImporterUnitTests
{
    public class ModelicaStructures
    {
        #region Path Variables
        private static readonly string testPath = Path.Combine(
            META.VersionInfo.MetaPath,
            "test",
            "InterchangeTest",
            "ComponentInterchangeTest",
            "SharedModels",
            "Modelica"
            );

        private static readonly string xmePathInputModel = Path.Combine(
            testPath,
            "InputModel.xme"
            );

        private static readonly string xmePathExemplarModel = Path.Combine(
            testPath,
            "Modelica.xme"
            );
        #endregion
        
        [Fact]
        [Trait("Interchange","Modelica")]
        [Trait("Interchange","Component Import")]
        public void ModelicaImport()
        {
            // Import the models.
            var mgaPathInputModel = Common.unpackXme(xmePathInputModel);
            var mgaPathExemplarModel = Common.unpackXme(xmePathExemplarModel);

            // Run importer
            foreach (var acm in Directory.GetFiles(testPath,"*.acm"))
            {
                int rCode = Common.runCyPhyComponentImporterCL(mgaPathInputModel, acm);
                Assert.True(rCode == 0,String.Format("Component Importer failed while importing {0}",acm));
            }
            
            Assert.True(0 == Common.RunCyPhyMLComparator(mgaPathExemplarModel, mgaPathInputModel),
                        "Imported model doesn't match expected");
        }
    }
}
