using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public partial class WaterPump : IUseFixture<WaterPumpFixture>
    {
        [Fact]
        [Trait("Model", "WaterPump")]
        public void LibraryAttach()
        {
            var mgaReference = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.EnableAutoAddOns(true);
            project.OpenEx(mgaReference, "CyPhyML", null);
            try
            {
                Assert.Contains("MGA.Addon.CyPhyDecoratorAddon",
                    project.AddOnComponents.Cast<IMgaComponentEx>().Select(addon => addon.ComponentProgID));
                MgaFolder lib;
                project.BeginTransactionInNewTerr();
                try
                {
                    project.RootFolder.AttachLibrary(project.ProjectConnStr, out lib);
                }
                finally
                {
                    project.CommitTransaction();
                }
            }
            finally
            {
                project.Close(true);
            }
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }
    }
}
