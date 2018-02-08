using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using System.Reflection;
using Xunit;
using System.IO;

namespace ElaboratorTest
{
    public abstract class XmeImportFixture
    {
        protected abstract string xmeFilename { get; }
        private Exception importException;
        private string _mgaFile;
        internal string mgaFile
        {
            get
            {
                if (importException != null)
                {
                    throw new Exception("Xme import failed", importException);
                }
                return _mgaFile;
            }
        }

        public XmeImportFixture()
        {
            try
            {
                this._mgaFile = ImportXME2Mga();
            }
            catch (Exception e)
            {
                importException = e;
            }
        }

        public string ImportXME2Mga()
        {
            string projectConnStr;
            MgaUtils.ImportXMEForTest(xmeFilename, out projectConnStr);
            return projectConnStr.Substring("MGA=".Length);
        }
    }

    public class ElaboratorFixture : XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("..", "..", "..", "..", "models", "DynamicsTeam", "Elaborator", "Elaborator.xme"); }
        }
    }

    public class Test : IUseFixture<ElaboratorFixture>
    {
        [STAThread]
        static int Main(string[] args)
        {
            int ret = Xunit.ConsoleClient.Program.Main(new string[] {
                Assembly.GetAssembly(typeof(Test)).CodeBase.Substring("file:///".Length),
                //"/noshadow",
            });
            Console.In.ReadLine();
            return ret;
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        [Trait("ProjectImport/Open", "Elaborator")]
        public void ProjectMgaOpen()
        {
            var mgaReference = "MGA=" + this.mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(mgaReference, "CyPhyML", null);
            MgaHelper.CheckParadigmVersionUpgrade(project);
            project.Close(true);
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }


        [Fact]
        [Trait("Model", "Elaborator")]
        public void InstanceGUIDComponentReferenceChain()
        {

            string objectAbsPath = "/@01_ComponentAssemblies/@01_InstanceGUIDTests/@InstanceGUIDComponentReferenceChain";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            // check context
            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");

        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void InstanceGUIDSimple()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@01_InstanceGUIDTests/@InstanceGUIDSimple";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void InstanceGUIDWithComponentAssemblyRef()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@01_InstanceGUIDTests/@InstanceGUIDWithComponentAssemblyRef";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void InstanceGUIDWithComponentAssemblyRefOutsideOfTree()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@01_InstanceGUIDTests/@InstanceGUIDWithComponentAssemblyRefOutsideOfTree";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void InstanceGUIDWithComponentAssemblyRefOutsideOfTreeWhichHasOnlyComponents()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@01_InstanceGUIDTests/@InstanceGUIDWithComponentAssemblyRefOutsideOfTreeWhichHasOnlyComponents";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void ComponentRefPointsToComponent()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@Valid/@ComponentRefPointsToComponent";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }



        [Fact]
        [Trait("Model", "Elaborator")]
        public void ComponentRefPointsToComponentAndConnections()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@Valid/@ComponentRefPointsToComponentAndConnections";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void HasComponentAssembliesMultiLevels()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@Valid/@HasComponentAssembliesMultiLevels";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void HasDecisionGroup()
        {
            string objectAbsPath = "/@03_DesignSpaces/@Valid/@HasDecisionGroup";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        // META-2667 needs a GME >= 14.1.18
        [Fact]
        [Trait("Model", "Elaborator")]
        public void HasSubtypes()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@Valid/@ContainsDerivedArchetype";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void SelfLoop()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@Valid/@SelfLoop";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void HasComponentAssembliesTwoLevels()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@Valid/@HasComponentAssembliesTwoLevels";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void HasComponentAssemblyRefsMultiLevel()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@Valid/@HasComponentAssemblyRefsMultiLevel";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void HasInheritedComponents()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@Valid/@HasInheritedComponents";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void SimpleHierarchy()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@Valid/@SimpleHierarchy";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void CircularBetweenSubtrees()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@Invalid/@CircularBetweenSubtrees";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.False(success, "CyPhyElaborator should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void CircularReference()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@Invalid/@CircularReference";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.False(success, "CyPhyElaborator should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void CircularReferencesBetweenTrees()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@Invalid/@CircularReferencesBetweenTrees";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.False(success, "CyPhyElaborator should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void WithinTheTree()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@Invalid/@WithinTheTree";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.False(success, "CyPhyElaborator should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void DesignSpaceDC()
        {
            string objectAbsPath = "/@DesignSpace/@DC";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void HasDesignSpace()
        {
            string objectAbsPath = "/@02_TestBenches/@Invalid/@HasDesignSpace";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.False(success, "CyPhyElaborator should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void StructuralFEATestInjectionPointTest()
        {
            string objectAbsPath = "/@02_TestBenches/@StructuralFEATestInjectionPointTest";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void StructuralFEATestInjectionPointTest2()
        {
            string objectAbsPath = "/@02_TestBenches/@StructuralFEATestInjectionPointTest2";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void StructuralFEATestInjectionPointTest3()
        {
            string objectAbsPath = "/@02_TestBenches/@StructuralFEATestInjectionPointTest3";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void NullReferenceTestInjectionPoint()
        {
            string objectAbsPath = "/@02_TestBenches/@Valid/@NullReferenceTestInjectionPoint";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void AmbiguousTestInjectionPointReference()
        {
            string objectAbsPath = "/@02_TestBenches/@Invalid/@AmbiguousTestInjectionPointReference";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.False(success, "CyPhyElaborator should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void OneTestInjectionPointIsNotInTheSystem()
        {
            string objectAbsPath = "/@02_TestBenches/@Invalid/@OneTestInjectionPointIsNotInTheSystem";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.False(success, "CyPhyElaborator should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void TestInjectionPointPointsToTheReferredComponentAssembly()
        {
            string objectAbsPath = "/@02_TestBenches/@Invalid/@TestInjectionPointPointsToTheReferredComponentAssembly";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.False(success, "CyPhyElaborator should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void ContainsDerived()
        {
            string objectAbsPath = "/@01_ComponentAssemblies/@Valid/@ContainsDerived";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void DesignSpaceComponentRefPointsToComponent()
        {
            string objectAbsPath = "/@03_DesignSpaces/@Valid/@ComponentRefPointsToComponent";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }



        [Fact]
        [Trait("Model", "Elaborator")]
        public void DesignSpaceComponentRefPointsToComponentAndConnections()
        {
            string objectAbsPath = "/@03_DesignSpaces/@Valid/@ComponentRefPointsToComponentAndConnections";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void DesignSpaceHasComponentAssembliesMultiLevels()
        {
            string objectAbsPath = "/@03_DesignSpaces/@Valid/@HasComponentAssembliesMultiLevels";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        // META-2667 needs a GME >= 14.1.18
        //[Fact]
        [Trait("Model", "Elaborator")]
        public void DesignSpaceHasSubtypes()
        {
            string objectAbsPath = "/@03_DesignSpaces/@Valid/@ContainsDerivedArchetype";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void DesignSpaceSelfLoop()
        {
            string objectAbsPath = "/@03_DesignSpaces/@Valid/@SelfLoop";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void DesignSpaceHasComponentAssembliesTwoLevels()
        {
            string objectAbsPath = "/@03_DesignSpaces/@Valid/@HasComponentAssembliesTwoLevels";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void DesignSpaceHasComponentAssemblyRefsMultiLevel()
        {
            string objectAbsPath = "/@03_DesignSpaces/@Valid/@HasComponentAssemblyRefsMultiLevel";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void DesignSpaceHasInheritedComponents()
        {
            string objectAbsPath = "/@03_DesignSpaces/@Valid/@HasInheritedComponents";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void DesignSpaceSimpleHierarchy()
        {
            string objectAbsPath = "/@03_DesignSpaces/@Valid/@SimpleHierarchy";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void DesignSpaceContainsDerived()
        {
            string objectAbsPath = "/@03_DesignSpaces/@Valid/@ContainsDerived";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void ConnectorSubtypeRef()
        {
            string objectAbsPath = "/@02_TestBenches/@Valid/@ConnectorSubtypeRef";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void ConnectorSubtype2Ref()
        {
            string objectAbsPath = "/@02_TestBenches/@Valid/@ConnectorSubtype2Ref";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void DesignSpaceCircularBetweenSubtrees()
        {
            string objectAbsPath = "/@03_DesignSpaces/@Invalid/@CircularBetweenSubtrees";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.False(success, "CyPhyElaborator should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void DesignSpaceCircularReference()
        {
            string objectAbsPath = "/@03_DesignSpaces/@Invalid/@CircularReferences";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.False(success, "CyPhyElaborator should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void DesignSpaceCircularReferencesBetweenTrees()
        {
            string objectAbsPath = "/@03_DesignSpaces/@Invalid/@CircularReferencesBetweenTrees";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.False(success, "CyPhyElaborator should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "Elaborator")]
        public void DesignSpaceWithinTheTree()
        {
            string objectAbsPath = "/@03_DesignSpaces/@Invalid/@WithinTheTree";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.False(success, "CyPhyElaborator should have failed, but did not.");
        }


        private string mgaFile { get { return this.fixture.mgaFile; } }
        private ElaboratorFixture fixture;

        public void SetFixture(ElaboratorFixture data)
        {
            this.fixture = data;
        }
    }
}
