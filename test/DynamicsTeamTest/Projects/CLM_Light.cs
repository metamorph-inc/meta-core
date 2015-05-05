using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public class CLM_LightFixture : XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("CLM_Light", "CLM_Light.xme"); }
        }
    }

    public partial class CLM_Light : IUseFixture<CLM_LightFixture>
    {
        internal string mgaFile { get { return this.fixture.mgaFile; } }
        private CLM_LightFixture fixture { get; set; }

        public void SetFixture(CLM_LightFixture data)
        {
            this.fixture = data;
        }

        //[Fact]
        //[Trait("Model", "CLM_Light")]
        //[Trait("ProjectImport/Open", "CLM_Light")]
        //public void ProjectXmeImport()
        //{
        //    Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
        //}

        [Fact]
        [Trait("Model", "CLM_Light")]
        [Trait("ProjectImport/Open", "CLM_Light")]
        public void ProjectMgaOpen()
        {
            var mgaReference = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(mgaReference, "CyPhyML", null);
            project.Close(true);
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }

        [Fact]
        [Trait("Model", "CLM_Light")]
        [Trait("CLM_light", "CLM_Light")]
        public void CLM_light_SuccessfulImport_Spring_2()
        {
            List<string> componentPaths = new List<string>();

            string designContainerPath = "/@DesignSpace_MSD/@Spring_1";
            string componentToInsertPath = "/@Components/@Springs/@Spring_2";

            componentPaths.Add(componentToInsertPath);

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool insertionSuccessful = CLM_LightRunner.Run(mgaFile, designContainerPath, componentPaths);
            Assert.True(insertionSuccessful, string.Format("Failed to insert {0}", componentToInsertPath));
        }

        [Fact]
        [Trait("Model", "CLM_Light")]
        [Trait("CLM_light", "CLM_Light")]
        public void CLM_light_SuccessfulImport_Spring_2_into_Optional_DesignContainer()
        {
            List<string> componentPaths = new List<string>();

            string designContainerPath = "/@DesignSpace_MSD/@Spring_OptionalContainer";
            string componentToInsertPath = "/@Components/@Springs/@Spring_2";

            componentPaths.Add(componentToInsertPath);

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool insertionSuccessful = CLM_LightRunner.Run(mgaFile, designContainerPath, componentPaths);
            Assert.True(insertionSuccessful, string.Format("Failed to insert {0}", componentToInsertPath));
        }

        [Fact]
        [Trait("Model", "CLM_Light")]
        [Trait("CLM_light", "CLM_Light")]
        public void CLM_light_SuccessfulImport_Spring_3()
        {
            List<string> componentPaths = new List<string>();

            string designContainerPath = "/@DesignSpace_MSD/@Spring_1";
            string componentToInsertPath = "/@Components/@Springs/@Spring_3";

            componentPaths.Add(componentToInsertPath);

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");

            bool insertionSuccessful = CLM_LightRunner.Run(mgaFile, designContainerPath, componentPaths);
            Assert.True(insertionSuccessful, string.Format("Failed to insert {0}", componentToInsertPath));
        }

        [Fact]
        [Trait("Model", "CLM_Light")]
        [Trait("CLM_light", "CLM_Light")]
        public void CLM_light_SuccessfulImport_Spring_ModelicaParameterPortMapWithSameNameAsProperty()
        {
            List<string> componentPaths = new List<string>();

            string designContainerPath = "/@DesignSpace_MSD/@Spring_1";
            string componentToInsertPath = "/@Components/@Springs/@Spring_ModelicaParameterPortMapWithSameNameAsProperty";

            componentPaths.Add(componentToInsertPath);

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");

            bool insertionSuccessful = CLM_LightRunner.Run(mgaFile, designContainerPath, componentPaths);
            Assert.True(insertionSuccessful, string.Format("Failed to insert {0}", componentToInsertPath));
        }

        [Fact]
        [Trait("Model", "CLM_Light")]
        [Trait("CLM_light", "CLM_Light")]
        public void CLM_light_SuccessfulImport_Spring_PortClassIncorrect()
        {
            List<string> componentPaths = new List<string>();

            string designContainerPath = "/@DesignSpace_MSD/@Spring_1";
            string componentToInsertPath = "/@Components/@Springs/@CLM_OddCases/@Spring_PortClassIncorrect";

            componentPaths.Add(componentToInsertPath);

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");

            bool insertionSuccessful = CLM_LightRunner.Run(mgaFile, designContainerPath, componentPaths);
            Assert.True(insertionSuccessful, string.Format("Failed to insert {0}", componentToInsertPath));
        }

        [Fact]
        [Trait("Model", "CLM_Light")]
        [Trait("CLM_light", "CLM_Light")]
        public void CLM_light_FailedImport_Spring_RedundantPorts()
        {
            List<string> componentPaths = new List<string>();

            string designContainerPath = "/@DesignSpace_MSD/@Spring_1";
            string componentToInsertPath = "/@Components/@Springs/@CLM_Failure/@Spring_RedundantPorts";

            componentPaths.Add(componentToInsertPath);

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool insertionSuccessful = CLM_LightRunner.Run(mgaFile, designContainerPath, componentPaths);
            Assert.False(insertionSuccessful, string.Format("Failed to insert {0}", componentToInsertPath));
        }

        [Fact]
        [Trait("Model", "CLM_Light")]
        [Trait("CLM_light", "CLM_Light")]
        public void CLM_light_FailedImport_Spring_PropertyWithPortName()
        {
            List<string> componentPaths = new List<string>();

            string designContainerPath = "/@DesignSpace_MSD/@Spring_1";
            string componentToInsertPath = "/@Components/@Springs/@CLM_Failure/@Spring_PropertyWithPortName";

            componentPaths.Add(componentToInsertPath);

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool insertionSuccessful = CLM_LightRunner.Run(mgaFile, designContainerPath, componentPaths);
            Assert.False(insertionSuccessful, string.Format("Failed to insert {0}", componentToInsertPath));

        }

        [Fact]
        [Trait("Model", "CLM_Light")]
        [Trait("CLM_light", "CLM_Light")]
        public void CLM_light_FailedImport_Spring_PortNameIncorrect()
        {
            List<string> componentPaths = new List<string>();

            string designContainerPath = "/@DesignSpace_MSD/@Spring_1";
            string componentToInsertPath = "/@Components/@Springs/@CLM_Failure/@Spring_PortNameIncorrect";

            componentPaths.Add(componentToInsertPath);

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool insertionSuccessful = CLM_LightRunner.Run(mgaFile, designContainerPath, componentPaths);
            Assert.False(insertionSuccessful, string.Format("Failed to insert {0}", componentToInsertPath));
        }

        [Fact]
        [Trait("Model", "CLM_Light")]
        [Trait("CLM_light", "CLM_Light")]
        public void CLM_light_FailedImport_Spring_ParamInsteadOfProperty()
        {
            List<string> componentPaths = new List<string>();

            string designContainerPath = "/@DesignSpace_MSD/@Spring_1";
            string componentToInsertPath = "/@Components/@Springs/@CLM_Failure/@Spring_ParamInsteadOfProperty";

            componentPaths.Add(componentToInsertPath);

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool insertionSuccessful = CLM_LightRunner.Run(mgaFile, designContainerPath, componentPaths);
            Assert.False(insertionSuccessful, string.Format("Failed to insert {0}", componentToInsertPath));
        }

        [Fact]
        [Trait("Model", "CLM_Light")]
        [Trait("CLM_light", "CLM_Light")]
        public void CLM_light_FailedImport_Spring_MissingProperty()
        {
            List<string> componentPaths = new List<string>();

            string designContainerPath = "/@DesignSpace_MSD/@Spring_1";
            string componentToInsertPath = "/@Components/@Springs/@CLM_Failure/@Spring_MissingProperty";

            componentPaths.Add(componentToInsertPath);

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            bool insertionSuccessful = CLM_LightRunner.Run(mgaFile, designContainerPath, componentPaths);

            Assert.False(insertionSuccessful, string.Format("Failed to insert {0}", componentToInsertPath));

        }

        [Fact]
        [Trait("Model", "CLM_Light")]
        [Trait("CLM_light", "CLM_Light")]
        public void CLM_light_FailedImport_Spring_MissingPort()
        {
            List<string> componentPaths = new List<string>();

            string designContainerPath = "/@DesignSpace_MSD/@Spring_1";
            string componentToInsertPath = "/@Components/@Springs/@CLM_Failure/@Spring_MissingPort";

            componentPaths.Add(componentToInsertPath);

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");

            bool insertionSuccessful = CLM_LightRunner.Run(mgaFile, designContainerPath, componentPaths);

            Assert.False(insertionSuccessful, string.Format("Failed to insert {0}", componentToInsertPath));
        }

        [Fact]
        [Trait("Model", "CLM_Light")]
        [Trait("CLM_light", "CLM_Light")]
        public void CLM_light_FailedImport_DesignContainerInstance()
        {
            List<string> componentPaths = new List<string>();

            string designContainerPath = "/@DesignSpace_MSD/@Instances/@Spring_1";
            string componentToInsertPath = "/@Components/@Springs/@Spring_2";

            componentPaths.Add(componentToInsertPath);

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");

            bool insertionSuccessful = CLM_LightRunner.Run(mgaFile, designContainerPath, componentPaths);

            Assert.False(insertionSuccessful, string.Format("Failed to insert {0}", componentToInsertPath));
        }

        [Fact]
        [Trait("Model", "CLM_Light")]
        [Trait("CLM_light", "CLM_Light")]
        public void CLM_light_FailedImport_DesignContainerLibraryObject()
        {
            List<string> componentPaths = new List<string>();

            string designContainerPath = "/@AttachedLibrary/@DesignSpace_MSD/@Spring_1";
            string componentToInsertPath = "/@Components/@Springs/@Spring_2";

            componentPaths.Add(componentToInsertPath);

            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");

            bool insertionSuccessful = CLM_LightRunner.Run(mgaFile, designContainerPath, componentPaths);

            Assert.False(insertionSuccessful, string.Format("Failed to insert {0}", componentToInsertPath));
        }
    }
}
