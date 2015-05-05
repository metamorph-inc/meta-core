namespace MasterInterpreterTest.UnitTests
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using CyPhyMasterInterpreter;
    using Xunit;
    using MasterInterpreterTest.Projects;
    using GME.MGA;
    using System.IO;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class AnalysisModelProcessorTests : IUseFixture<MasterInterpreterFixture>
    {
        internal string mgaFile { get { return this.fixture.mgaFile; } }
        private MasterInterpreterFixture fixture { get; set; }

        public void SetFixture(MasterInterpreterFixture data)
        {
            this.fixture = data;
        }

        [Fact]
        [Trait("MasterInterpreter", "AnalysisModelProcessor")]
        public void ShouldThrowExceptions()
        {
            // null context
            Assert.Throws<ArgumentNullException>(() => { AnalysisModelProcessor.GetAnalysisModelProcessor(null); });

            Assert.True(File.Exists(this.mgaFile), "Project file does not exist.");
            string ProjectConnStr = "MGA=" + Path.GetFullPath(this.mgaFile);

            MgaProject project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);
            try
            {
                var terr = project.BeginTransactionInNewTerr();
                var componentAssembly = project.RootFolder.GetDescendantFCOs(project.CreateFilter()).OfType<MgaModel>().FirstOrDefault(x => x.MetaBase.Name == "ComponentAssembly");

                Assert.True(componentAssembly != null, string.Format("{0} project must contain one component assembly.", Path.GetFullPath(this.mgaFile)));

                // invalid context
                Assert.Throws<AnalysisModelContextNotSupportedException>(() => { AnalysisModelProcessor.GetAnalysisModelProcessor(componentAssembly); });
            }
            finally
            {
                project.AbortTransaction();
                project.Close(true);
            }

        }
    }
}
