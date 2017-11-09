using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using System.Reflection;
using Xunit;
using System.IO;
using System.Runtime.InteropServices;

namespace ElaboratorTest
{
    public class RollingWheelWithLibFixture : XmeImportFixture
    {
        protected override string xmeFilename
        {
            get { return Path.Combine("..", "..", "..", "..", "models", "DynamicsTeam", "RollingWheelWithLib", "RollingWheelWithLib.xme"); }
        }
    }

    public class RollingWheelWithLibTest : IUseFixture<RollingWheelWithLibFixture>
    {
        //[STAThread]
        //static int Main(string[] args)
        //{
        //    int ret = Xunit.ConsoleClient.Program.Main(new string[] {
        //        Assembly.GetAssembly(typeof(Test)).CodeBase.Substring("file:///".Length),
        //        //"/noshadow",
        //    });
        //    Console.In.ReadLine();
        //    return ret;
        //}

        private string mgaFile { get { return this.fixture.mgaFile; } }
        private RollingWheelWithLibFixture fixture;

        public void SetFixture(RollingWheelWithLibFixture data)
        {
            this.fixture = data;
        }

        //[Fact]
        //[Trait("Model", "RollingWheelWithLib")]
        //[Trait("ProjectImport/Open", "RollingWheelWithLib")]
        //public void ProjectXmeImport()
        //{
        //    Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");
        //}

        [Fact]
        [Trait("Model", "RollingWheelWithLib")]
        [Trait("ProjectImport/Open", "RollingWheelWithLib")]
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
        [Trait("Model", "RollingWheelWithLib")]
        public void ComponentAssembly()
        {
            string objectAbsPath = "@CAWheel/@Wheel";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "RollingWheelWithLib")]
        public void TestBench()
        {
            string objectAbsPath = "/@TestBenches/@RollingWheel";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "RollingWheelWithLib")]
        public void LibraryComponentAssembly()
        {
            string objectAbsPath = "/@_Lib_/@CAWheel/@Wheel";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.False(success, "CyPhyElaborator should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RollingWheelWithLib")]
        public void LibraryTestBench()
        {
            string objectAbsPath = "/@_Lib_/@TestBenches/@RollingWheel";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.False(success, "CyPhyElaborator should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RollingWheelWithLib")]
        public void RefersToSameComponentRefTwice_Fail()
        {
            string objectAbsPath = "/@TestBenches/@BallisticCriticalComponent/@RefersToSameComponentRefTwice_Fail";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.False(success, "CyPhyElaborator should have failed, but did not.");
        }

        [Fact]
        [Trait("Model", "RollingWheelWithLib")]
        public void ReferencedByRefersTo()
        {
            string objectAbsPath = "/@TestBenches/@BallisticCriticalComponent/@ReferencedByRefersTo";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "RollingWheelWithLib")]
        public void RefersToMultiple()
        {
            string objectAbsPath = "/@TestBenches/@BallisticCriticalComponent/@RefersToMultiple";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "RollingWheelWithLib")]
        public void RefersToSameComponentTwice()
        {
            string objectAbsPath = "/@TestBenches/@BallisticCriticalComponent/@RefersToSameComponentTwice";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "RollingWheelWithLib")]
        public void RefersToSingle()
        {
            string objectAbsPath = "/@TestBenches/@BallisticCriticalComponent/@RefersToSingle";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");

            var success = ElaboratorRunner.RunElaborator(mgaFile, objectAbsPath);

            Assert.True(success, "CyPhyElaborator should have succeeded, but did not.");
        }

        [Fact]
        [Trait("Model", "RollingWheelWithLib")]
        public void FormulaEvaluatorLibraryComponent()
        {
            string objectAbsPath = "/@_Lib_/@ModelicaImports/@Damper";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");
            try
            {
                FormulaEvaluateRunner.RunFormulaEvaluate(mgaFile, objectAbsPath, true);

            }
            catch (COMException err)
            {
                Assert.True(err.Message.Contains("Cannot run FormulaEvaluator on Library Objects."));
            }
        }

        [Fact]
        [Trait("Model", "RollingWheelWithLib")]
        public void FormulaEvaluatorLibraryCA()
        {
            string objectAbsPath = "/@_Lib_/@CAWheel/@Wheel";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");
            try
            {
                FormulaEvaluateRunner.RunFormulaEvaluate(mgaFile, objectAbsPath, true);
            }
            catch (COMException err)
            {
                Assert.True(err.Message.Contains("Cannot run FormulaEvaluator on Library Objects."));
            }
        }

        [Fact]
        [Trait("Model", "RollingWheelWithLib")]
        public void FormulaEvaluatorLibraryTestComponent()
        {
            string objectAbsPath = "/@_Lib_/@ModelicaImports/@ModelicaImports/@Sine";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");
            try
            {
                FormulaEvaluateRunner.RunFormulaEvaluate(mgaFile, objectAbsPath, true);
            }
            catch (COMException err)
            {
                Assert.True(err.Message.Contains("Cannot run FormulaEvaluator on Library Objects."));
            }
        }

        [Fact]
        [Trait("Model", "RollingWheelWithLib")]
        public void FormulaEvaluatorLibraryTestBench()
        {
            string objectAbsPath = "/@_Lib_/@TestBenches/@RollingWheel";

            Assert.True(File.Exists(this.mgaFile), "Failed to generate the mga.");
            try
            {
                FormulaEvaluateRunner.RunFormulaEvaluate(mgaFile, objectAbsPath, true);
            }
            catch (COMException err)
            {
                Assert.True(err.Message.Contains("Cannot run FormulaEvaluator on Library Objects."));
            }
        }
    }
}