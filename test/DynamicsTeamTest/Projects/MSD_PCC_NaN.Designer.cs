 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public class MSD_PCC_NaNFixture
    {
        internal string mgaFile = null;

        public MSD_PCC_NaNFixture()
        {
            this.mgaFile = Test.ImportXME2Mga("MSD_PCC_NaN", "MSD_PCC_NaN.xme");
        }
    }

    public partial class MSD_PCC_NaN : IUseFixture<MSD_PCC_NaNFixture>
    {
        internal string mgaFile { get { return this.fixture.mgaFile; } }
        private MSD_PCC_NaNFixture fixture { get; set; }

        public void SetFixture(MSD_PCC_NaNFixture data)
        {
            this.fixture = data;
        }

        [Fact]
        [Trait("Model", "MSD_PCC_NaN")]
        [Trait("ProjectImport/Open", "MSD_PCC_NaN")]
        public void ProjectXmeImport()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
        }

        [Fact]
        [Trait("Model", "MSD_PCC_NaN")]
        [Trait("ProjectImport/Open", "MSD_PCC_NaN")]
        public void ProjectMgaOpen()
        {
            var mgaReference = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(mgaReference, "CyPhyML", null);
            project.Close(true);
            Assert.True(File.Exists(mgaReference.Substring("MGA=".Length)));
        }

        [Fact]
        [Trait("Model", "MSD_PCC_NaN")]
        [Trait("CyPhy2Modelica", "MSD_PCC_NaN")]
        public void MSD_dymola_cfg879()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            CyPhyGUIs.IInterpreterResult result = null;
            try
            {
                result = CyPhy2ModelicaRunner.Run("MSD_dymola_cfg879", mgaFile,
                    "/@TestBenches|kind=Testing|relpos=0/@PCC_NaN|kind=Testing|relpos=0/@MSD_dymola_cfg879|kind=TestBench|relpos=0");
            }
            catch (META.InterpreterException ex)
            {
                Console.WriteLine(ex.Message);
                Assert.NotNull(result);
            }

            Assert.NotNull(result);
            Assert.True(result.Success, "Interpreter return with result.Success as false!");
            var runCommand = result.RunCommand.Split(new char[] { ' ' }).FirstOrDefault();
            Assert.True(File.Exists(Path.Combine(Path.GetDirectoryName(mgaFile), "MSD_dymola_cfg879", runCommand)),
                string.Format("RunCommand file : {0}, does not exist.", runCommand));
        }

        [Fact]
        [Trait("Model", "MSD_PCC_NaN")]
        [Trait("CyPhy2Modelica", "MSD_PCC_NaN")]
        public void MSD_dymola_cfg871()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            CyPhyGUIs.IInterpreterResult result = null;
            try
            {
                result = CyPhy2ModelicaRunner.Run("MSD_dymola_cfg871", mgaFile,
                    "/@TestBenches|kind=Testing|relpos=0/@PCC_NaN|kind=Testing|relpos=0/@MSD_dymola_cfg871|kind=TestBench|relpos=0");
            }
            catch (META.InterpreterException ex)
            {
                Console.WriteLine(ex.Message);
                Assert.NotNull(result);
            }

            Assert.NotNull(result);
            Assert.True(result.Success, "Interpreter return with result.Success as false!");
            var runCommand = result.RunCommand.Split(new char[] { ' ' }).FirstOrDefault();
            Assert.True(File.Exists(Path.Combine(Path.GetDirectoryName(mgaFile), "MSD_dymola_cfg871", runCommand)),
                string.Format("RunCommand file : {0}, does not exist.", runCommand));
        }

        [Fact]
        [Trait("Model", "MSD_PCC_NaN")]
        [Trait("CyPhy2Modelica", "MSD_PCC_NaN")]
        public void MSD_dymola_cfg844()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            CyPhyGUIs.IInterpreterResult result = null;
            try
            {
                result = CyPhy2ModelicaRunner.Run("MSD_dymola_cfg844", mgaFile,
                    "/@TestBenches|kind=Testing|relpos=0/@PCC_NaN|kind=Testing|relpos=0/@MSD_dymola_cfg844|kind=TestBench|relpos=0");
            }
            catch (META.InterpreterException ex)
            {
                Console.WriteLine(ex.Message);
                Assert.NotNull(result);
            }

            Assert.NotNull(result);
            Assert.True(result.Success, "Interpreter return with result.Success as false!");
            var runCommand = result.RunCommand.Split(new char[] { ' ' }).FirstOrDefault();
            Assert.True(File.Exists(Path.Combine(Path.GetDirectoryName(mgaFile), "MSD_dymola_cfg844", runCommand)),
                string.Format("RunCommand file : {0}, does not exist.", runCommand));
        }

        [Fact]
        [Trait("Model", "MSD_PCC_NaN")]
        [Trait("CyPhy2Modelica", "MSD_PCC_NaN")]
        public void MSD_dymola_cfg299()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            CyPhyGUIs.IInterpreterResult result = null;
            try
            {
                result = CyPhy2ModelicaRunner.Run("MSD_dymola_cfg299", mgaFile,
                    "/@TestBenches|kind=Testing|relpos=0/@PCC_NaN|kind=Testing|relpos=0/@MSD_dymola_cfg299|kind=TestBench|relpos=0");
            }
            catch (META.InterpreterException ex)
            {
                Console.WriteLine(ex.Message);
                Assert.NotNull(result);
            }

            Assert.NotNull(result);
            Assert.True(result.Success, "Interpreter return with result.Success as false!");
            var runCommand = result.RunCommand.Split(new char[] { ' ' }).FirstOrDefault();
            Assert.True(File.Exists(Path.Combine(Path.GetDirectoryName(mgaFile), "MSD_dymola_cfg299", runCommand)),
                string.Format("RunCommand file : {0}, does not exist.", runCommand));
        }

        [Fact]
        [Trait("Model", "MSD_PCC_NaN")]
        [Trait("CyPhy2Modelica", "MSD_PCC_NaN")]
        public void MSD_dymola_cfg310()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            CyPhyGUIs.IInterpreterResult result = null;
            try
            {
                result = CyPhy2ModelicaRunner.Run("MSD_dymola_cfg310", mgaFile,
                    "/@TestBenches|kind=Testing|relpos=0/@PCC_NaN|kind=Testing|relpos=0/@MSD_dymola_cfg310|kind=TestBench|relpos=0");
            }
            catch (META.InterpreterException ex)
            {
                Console.WriteLine(ex.Message);
                Assert.NotNull(result);
            }

            Assert.NotNull(result);
            Assert.True(result.Success, "Interpreter return with result.Success as false!");
            var runCommand = result.RunCommand.Split(new char[] { ' ' }).FirstOrDefault();
            Assert.True(File.Exists(Path.Combine(Path.GetDirectoryName(mgaFile), "MSD_dymola_cfg310", runCommand)),
                string.Format("RunCommand file : {0}, does not exist.", runCommand));
        }

        [Fact]
        [Trait("Model", "MSD_PCC_NaN")]
        [Trait("CyPhy2Modelica", "MSD_PCC_NaN")]
        public void MSD_om_CA()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            CyPhyGUIs.IInterpreterResult result = null;
            try
            {
                result = CyPhy2ModelicaRunner.Run("MSD_om_CA", mgaFile,
                    "/@TestBenches|kind=Testing|relpos=0/@SingleConfig|kind=Testing|relpos=0/@MSD_om_CA|kind=TestBench|relpos=0");
            }
            catch (META.InterpreterException ex)
            {
                Console.WriteLine(ex.Message);
                Assert.NotNull(result);
            }

            Assert.NotNull(result);
            Assert.True(result.Success, "Interpreter return with result.Success as false!");
            var runCommand = result.RunCommand.Split(new char[] { ' ' }).FirstOrDefault();
            Assert.True(File.Exists(Path.Combine(Path.GetDirectoryName(mgaFile), "MSD_om_CA", runCommand)),
                string.Format("RunCommand file : {0}, does not exist.", runCommand));
        }

        [Fact]
        [Trait("Model", "MSD_PCC_NaN")]
        [Trait("CyPhy2Modelica", "MSD_PCC_NaN")]
        public void MSD_dymola_CA()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            CyPhyGUIs.IInterpreterResult result = null;
            try
            {
                result = CyPhy2ModelicaRunner.Run("MSD_dymola_CA", mgaFile,
                    "/@TestBenches|kind=Testing|relpos=0/@SingleConfig|kind=Testing|relpos=0/@MSD_dymola_CA|kind=TestBench|relpos=0");
            }
            catch (META.InterpreterException ex)
            {
                Console.WriteLine(ex.Message);
                Assert.NotNull(result);
            }

            Assert.NotNull(result);
            Assert.True(result.Success, "Interpreter return with result.Success as false!");
            var runCommand = result.RunCommand.Split(new char[] { ' ' }).FirstOrDefault();
            Assert.True(File.Exists(Path.Combine(Path.GetDirectoryName(mgaFile), "MSD_dymola_CA", runCommand)),
                string.Format("RunCommand file : {0}, does not exist.", runCommand));
        }


        [Fact]
        [Trait("Model", "MSD_PCC_NaN")]
        [Trait("PCC_NaN", "MSD_PCC_NaN")]
        [Trait("Issue", "META-1427")]
        public void PCC_JointPCC_NaN_cfg299()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            CyPhyGUIs.IInterpreterResult result = null;
            try
            {
                result = CyPhyPETRunner.Run("PCC_JointPCC_NaN_cfg299", mgaFile,
                    "/@TestBenches|kind=Testing|relpos=0/@PCC_NaN|kind=Testing|relpos=0/@PCC_NaN|kind=ParametricExplorationFolder|relpos=0/@PCC_JointPCC_NaN_cfg299|kind=ParametricExploration|relpos=0");
            }
            catch (META.InterpreterException ex)
            {
                Console.WriteLine(ex.Message);
                Assert.NotNull(result);
            }

            Assert.NotNull(result);
            Assert.True(result.Success, "Interpreter return with result.Success as false!");
            var runCommand = result.RunCommand.Split(new char[] { ' ' }).FirstOrDefault();
            Assert.True(File.Exists(Path.Combine(Path.GetDirectoryName(mgaFile), "PCC_JointPCC_NaN_cfg299", runCommand)),
                string.Format("RunCommand file : {0}, does not exist.", runCommand));
        }

        [Fact]
        [Trait("Model", "MSD_PCC_NaN")]
        [Trait("PCC_NaN", "MSD_PCC_NaN")]
        [Trait("Issue", "META-1427")]
        public void PCC_Complexity_NaN_cfg310()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            CyPhyGUIs.IInterpreterResult result = null;
            try
            {
                result = CyPhyPETRunner.Run("PCC_Complexity_NaN_cfg310", mgaFile,
                    "/@TestBenches|kind=Testing|relpos=0/@PCC_NaN|kind=Testing|relpos=0/@PCC_NaN|kind=ParametricExplorationFolder|relpos=0/@PCC_Complexity_NaN_cfg310|kind=ParametricExploration|relpos=0");
            }
            catch (META.InterpreterException ex)
            {
                Console.WriteLine(ex.Message);
                Assert.NotNull(result);
            }

            Assert.NotNull(result);
            Assert.True(result.Success, "Interpreter return with result.Success as false!");
            var runCommand = result.RunCommand.Split(new char[] { ' ' }).FirstOrDefault();
            Assert.True(File.Exists(Path.Combine(Path.GetDirectoryName(mgaFile), "PCC_Complexity_NaN_cfg310", runCommand)),
                string.Format("RunCommand file : {0}, does not exist.", runCommand));
        }

        [Fact]
        [Trait("Model", "MSD_PCC_NaN")]
        [Trait("PCC_NaN", "MSD_PCC_NaN")]
        [Trait("Issue", "META-1427")]
        public void PCC_Complexity_NaN_cfg871()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            CyPhyGUIs.IInterpreterResult result = null;
            try
            {
                result = CyPhyPETRunner.Run("PCC_Complexity_NaN_cfg871", mgaFile,
                    "/@TestBenches|kind=Testing|relpos=0/@PCC_NaN|kind=Testing|relpos=0/@PCC_NaN|kind=ParametricExplorationFolder|relpos=0/@PCC_Complexity_NaN_cfg871|kind=ParametricExploration|relpos=0");
            }
            catch (META.InterpreterException ex)
            {
                Console.WriteLine(ex.Message);
                Assert.NotNull(result);
            }

            Assert.NotNull(result);
            Assert.True(result.Success, "Interpreter return with result.Success as false!");
            var runCommand = result.RunCommand.Split(new char[] { ' ' }).FirstOrDefault();
            Assert.True(File.Exists(Path.Combine(Path.GetDirectoryName(mgaFile), "PCC_Complexity_NaN_cfg871", runCommand)),
                string.Format("RunCommand file : {0}, does not exist.", runCommand));
        }

        [Fact]
        [Trait("Model", "MSD_PCC_NaN")]
        [Trait("PCC_NaN", "MSD_PCC_NaN")]
        [Trait("Issue", "META-1427")]
        public void PCC_JointPCC_NaN_cfg844()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            CyPhyGUIs.IInterpreterResult result = null;
            try
            {
                result = CyPhyPETRunner.Run("PCC_JointPCC_NaN_cfg844", mgaFile,
                    "/@TestBenches|kind=Testing|relpos=0/@PCC_NaN|kind=Testing|relpos=0/@PCC_NaN|kind=ParametricExplorationFolder|relpos=0/@PCC_JointPCC_NaN_cfg844|kind=ParametricExploration|relpos=0");
            }
            catch (META.InterpreterException ex)
            {
                Console.WriteLine(ex.Message);
                Assert.NotNull(result);
            }

            Assert.NotNull(result);
            Assert.True(result.Success, "Interpreter return with result.Success as false!");
            var runCommand = result.RunCommand.Split(new char[] { ' ' }).FirstOrDefault();
            Assert.True(File.Exists(Path.Combine(Path.GetDirectoryName(mgaFile), "PCC_JointPCC_NaN_cfg844", runCommand)),
                string.Format("RunCommand file : {0}, does not exist.", runCommand));
        }

        [Fact]
        [Trait("Model", "MSD_PCC_NaN")]
        [Trait("PCC_NaN", "MSD_PCC_NaN")]
        [Trait("Issue", "META-1427")]
        public void PCC_Complexity_NaN_cfg879()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            CyPhyGUIs.IInterpreterResult result = null;
            try
            {
                result = CyPhyPETRunner.Run("PCC_Complexity_NaN_cfg879", mgaFile,
                    "/@TestBenches|kind=Testing|relpos=0/@PCC_NaN|kind=Testing|relpos=0/@PCC_NaN|kind=ParametricExplorationFolder|relpos=0/@PCC_Complexity_NaN_cfg879|kind=ParametricExploration|relpos=0");
            }
            catch (META.InterpreterException ex)
            {
                Console.WriteLine(ex.Message);
                Assert.NotNull(result);
            }

            Assert.NotNull(result);
            Assert.True(result.Success, "Interpreter return with result.Success as false!");
            var runCommand = result.RunCommand.Split(new char[] { ' ' }).FirstOrDefault();
            Assert.True(File.Exists(Path.Combine(Path.GetDirectoryName(mgaFile), "PCC_Complexity_NaN_cfg879", runCommand)),
                string.Format("RunCommand file : {0}, does not exist.", runCommand));
        }

        [Fact]
        [Trait("Model", "MSD_PCC_NaN")]
        [Trait("PCC", "MSD_PCC_NaN")]
        public void PCC_MSD_om_CA()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            CyPhyGUIs.IInterpreterResult result = null;
            try
            {
                result = CyPhyPETRunner.Run("PCC_MSD_om_CA", mgaFile,
                    "/@TestBenches|kind=Testing|relpos=0/@SingleConfig|kind=Testing|relpos=0/@PCC_CA|kind=ParametricExplorationFolder|relpos=0/@PCC_MSD_om_CA|kind=ParametricExploration|relpos=0");
            }
            catch (META.InterpreterException ex)
            {
                Console.WriteLine(ex.Message);
                Assert.NotNull(result);
            }

            Assert.NotNull(result);
            Assert.True(result.Success, "Interpreter return with result.Success as false!");
            var runCommand = result.RunCommand.Split(new char[] { ' ' }).FirstOrDefault();
            Assert.True(File.Exists(Path.Combine(Path.GetDirectoryName(mgaFile), "PCC_MSD_om_CA", runCommand)),
                string.Format("RunCommand file : {0}, does not exist.", runCommand));
        }

        [Fact]
        [Trait("Model", "MSD_PCC_NaN")]
        [Trait("PCC", "MSD_PCC_NaN")]
        public void PCC_MSD_dymola_CA()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            CyPhyGUIs.IInterpreterResult result = null;
            try
            {
                result = CyPhyPETRunner.Run("PCC_MSD_dymola_CA", mgaFile,
                    "/@TestBenches|kind=Testing|relpos=0/@SingleConfig|kind=Testing|relpos=0/@PCC_CA|kind=ParametricExplorationFolder|relpos=0/@PCC_MSD_dymola_CA|kind=ParametricExploration|relpos=0");
            }
            catch (META.InterpreterException ex)
            {
                Console.WriteLine(ex.Message);
                Assert.NotNull(result);
            }

            Assert.NotNull(result);
            Assert.True(result.Success, "Interpreter return with result.Success as false!");
            var runCommand = result.RunCommand.Split(new char[] { ' ' }).FirstOrDefault();
            Assert.True(File.Exists(Path.Combine(Path.GetDirectoryName(mgaFile), "PCC_MSD_dymola_CA", runCommand)),
                string.Format("RunCommand file : {0}, does not exist.", runCommand));
        }

    }
}

