using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using GME.Util;
using GME.MGA;
using Xunit;
using System.Xml.Schema;
using System.Xml.Linq;

namespace CADTeamTest
{
    public class CADTests
    {
        public const string generatedAsmFile = "CADAssembly.xml";
        public const string generatedMBDFile = "Analysis_MBD.xml";
        public const string generatedBlastConfig = "BlastConfig.json";
        public const string generatedBallisticConfig = "BallisticConfig.json";

        private void UnpackXmes(string xmeFile)
        {
            unpackXME(xmeFile);
        }

        private void unpackXME(string xmeFilename)
        {
            if (!File.Exists(xmeFilename)) return;
            var mgaFilename = Path.ChangeExtension(xmeFilename, "mga");
            MgaUtils.ImportXME(xmeFilename, mgaFilename);
        }
        
        #region OriginalTestCases

        [Fact]
        public void TestBench_Valid()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CADTeam\MSD_CAD.xme");
            string TestbenchPath = "/@MyTestBenches|kind=Testing|relpos=0/@TestBench_Config|kind=Testing|relpos=0/@CADAssembly|kind=Testing|relpos=0/@TestBench_Valid|kind=TestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "TestBench_Valid");
            var generatedAsmPath = Path.Combine(OutputDir, generatedAsmFile);

            bool status = CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath);
            Assert.True(File.Exists(generatedAsmPath), "Failed to generate " + generatedAsmFile);

            new AsmXmlChecker(generatedAsmPath);
        }

        [Fact]
        public void TestBench_Invalid()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CADTeam\MSD_CAD.xme");
            string TestbenchPath = "/@MyTestBenches|kind=Testing|relpos=0/@TestBench_Config|kind=Testing|relpos=0/@CADAssembly|kind=Testing|relpos=0/@TestBench_Invalid|kind=TestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "TestBench_Invalid");

            bool status = CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath);
            Assert.False(File.Exists(Path.Combine(OutputDir, generatedAsmFile)), "Failed to generate " + generatedAsmFile);
        }

        //[Fact(Skip = "not implemented")]
        public void BlastTB_Custom_Valid()
        {

        }

        [Fact]
        public void BlastTB_Predefined_Valid()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CADTeam\MSD_CAD.xme");
            string TestbenchPath = "/@MyTestBenches|kind=Testing|relpos=0/@TestBench_Config|kind=Testing|relpos=0/@Blast|kind=Testing|relpos=0/@Predefined_Blast_Valid|kind=BlastTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "BlastTB_Predefined_Valid");

            bool status = CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath);
            Assert.True(File.Exists(Path.Combine(OutputDir, generatedAsmFile)), "Failed to generate " + generatedAsmFile);
            Assert.True(File.Exists(Path.Combine(OutputDir, generatedBlastConfig)), "Failed to generate " + generatedBlastConfig);

            new AsmXmlChecker(Path.Combine(OutputDir, generatedAsmFile));
        }

        [Fact]
        public void BlastTB_Predefined_Invalid()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CADTeam\MSD_CAD.xme");
            string TestbenchPath = "/@MyTestBenches|kind=Testing|relpos=0/@TestBench_Config|kind=Testing|relpos=0/@Blast|kind=Testing|relpos=0/@Predefined_Blast_Invalid|kind=BlastTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "BlastTB_Predefined_Invalid");

            bool status = CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath);
            Assert.False(File.Exists(Path.Combine(OutputDir, generatedAsmFile)), "Failed to generate " + generatedAsmFile);
            Assert.False(File.Exists(Path.Combine(OutputDir, generatedBlastConfig)), "Failed to generate " + generatedBlastConfig);
        }

        [Fact]
        public void BallisticTB_Custom_Valid()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CADTeam\MSD_CAD.xme");
            string TestbenchPath = "/@MyTestBenches|kind=Testing|relpos=0/@TestBench_Config|kind=Testing|relpos=0/@Ballistic|kind=Testing|relpos=0/@Custom_Ballistics_Valid|kind=BallisticTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "BallisticTB_Custom_Valid");

            bool status = CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath);
            Assert.True(File.Exists(Path.Combine(OutputDir, generatedAsmFile)), "Failed to generate " + generatedAsmFile);
            Assert.True(File.Exists(Path.Combine(OutputDir, generatedBallisticConfig)), "Failed to generate " + generatedBallisticConfig);

            new AsmXmlChecker(Path.Combine(OutputDir, generatedAsmFile))
                
                .CheckComponentAttribute("{f6cf014a-caeb-440a-b2b1-d700fe99e652}", "Representation", "BuyRep_Test")
                .CheckComponentAttribute("{f4a87e6b-4ce4-46d9-92cc-28b19ee61399}", "Representation", "DefaultRep_Test")
                .CheckComponentAttribute("{94353ed0-bf00-40d3-9d22-b14c70271460}", "Representation", "MakeRep_Test");
        }

        [Fact]
        public void BallisticTB_Custom_Invalid()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CADTeam\MSD_CAD.xme");
            string TestbenchPath = "/@MyTestBenches|kind=Testing|relpos=0/@TestBench_Config|kind=Testing|relpos=0/@Ballistic|kind=Testing|relpos=0/@Custom_Ballistics_Invalid|kind=BallisticTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "BallisticTB_Custom_Invalid");

            bool status = CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath);
            Assert.False(File.Exists(Path.Combine(OutputDir, generatedAsmFile)), "Failed to generate " + generatedAsmFile);
            Assert.False(File.Exists(Path.Combine(OutputDir, generatedBallisticConfig)), "Failed to generate " + generatedBallisticConfig);
        }

        [Fact]
        public void BallisticTB_Predefined_Valid()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CADTeam\MSD_CAD.xme");
            string TestbenchPath = "/@MyTestBenches|kind=Testing|relpos=0/@TestBench_Config|kind=Testing|relpos=0/@Ballistic|kind=Testing|relpos=0/@Predefined_Ballistics_Valid|kind=BallisticTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "BallisticTB_Predefined_Valid");

            bool status = CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath);
            Assert.True(File.Exists(Path.Combine(OutputDir, generatedAsmFile)), "Failed to generate " + generatedAsmFile);
            Assert.True(File.Exists(Path.Combine(OutputDir, generatedBallisticConfig)), "Failed to generate " + generatedBallisticConfig);

            new AsmXmlChecker(Path.Combine(OutputDir, generatedAsmFile)).CheckNodes(new string[] { "Interference" }, false);
        }

        [Fact]
        public void BallisticTB_Predefined_Invalid()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CADTeam\MSD_CAD.xme");
            string TestbenchPath = "/@MyTestBenches|kind=Testing|relpos=0/@TestBench_Config|kind=Testing|relpos=0/@Ballistic|kind=Testing|relpos=0/@Predefined_Ballistics_Invalid|kind=BallisticTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "BallisticTB_Predefined_Invalid");

            bool status = CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath);
            Assert.False(File.Exists(Path.Combine(OutputDir, generatedAsmFile)), "Failed to generate " + generatedAsmFile);
            Assert.False(File.Exists(Path.Combine(OutputDir, generatedBallisticConfig)), "Failed to generate " + generatedBallisticConfig);
        }

        [Fact]
        public void BallisticTB_Invalid()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CADTeam\MSD_CAD.xme");
            string TestbenchPath = "/@MyTestBenches|kind=Testing|relpos=0/@TestBench_Config|kind=Testing|relpos=0/@Ballistic|kind=Testing|relpos=0/@Ballistics_Invalid|kind=BallisticTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "BallisticTB_Invalid");

            bool status = CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath);
            Assert.False(File.Exists(Path.Combine(OutputDir, generatedAsmFile)), "Failed to generate " + generatedAsmFile);
            Assert.False(File.Exists(Path.Combine(OutputDir, generatedBallisticConfig)), "Failed to generate " + generatedBallisticConfig);
        }

        [Fact]
        // This tests that the interpreters would not fail on null object
        public void CAD_CurrentObjectNull()
        {
            var XmePath = Path.GetFullPath(@"..\..\..\..\models\CADTeam\MSD_CAD.xme");
            UnpackXmes(XmePath);
            var mgaFile = XmePath.Replace(".xme", ".mga");

            string ProjectConnStr = "MGA=" + mgaFile;
            
            MgaProject project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);
            try
            {
                var PrepIFABInt = new CyPhyPrepareIFab.CyPhyPrepareIFabInterpreter();
                var CADAnalysisInt = new CyPhyCADAnalysis.CyPhyCADAnalysisInterpreter();
                var CyPhy2CAD = new CyPhy2CAD_CSharp.CyPhy2CAD_CSharpInterpreter();

                PrepIFABInt.Initialize(project);
                CADAnalysisInt.Initialize(project);
                CyPhy2CAD.Initialize(project);

                Assert.DoesNotThrow(() => PrepIFABInt.InvokeEx(project, null, null, 16));
                Assert.DoesNotThrow(() => CyPhy2CAD.InvokeEx(project, null, null, 16));
                Assert.DoesNotThrow(() => CADAnalysisInt.InvokeEx(project, null, null, 16));
            }
            finally
            {
                project.Close(true);
            }
        }

        #endregion

        [Fact]
        public void CFD_Success()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CFD\SolverSettings\wigley_hull.xme");
            string TestbenchPath = "/@CFD_Testing|kind=Testing|relpos=0/@CFD_Success|kind=CFDTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "CFD_Success_Output");

            string cfdGeneratedDir = Path.Combine(OutputDir, "CFD");
            string cfdOutputJson = Path.Combine(cfdGeneratedDir, "CFDConfig.json");
            string generatedAssemblyFilePath = Path.Combine(OutputDir, generatedAsmFile);

            bool status = CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath);
            Assert.True(File.Exists(generatedAssemblyFilePath), "Failed to generate " + generatedAsmFile);
            Assert.True(File.Exists(cfdOutputJson), "Failed to generate CFDConfig.json!");

            new AsmXmlChecker(Path.Combine(OutputDir, generatedAsmFile));
        }

        [Fact]
        public void CFD_Failure_CalmWaterSolverVelocityInvalid()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CFD\SolverSettings\wigley_hull.xme");
            string TestbenchPath = 
                "/@CFD_Testing|kind=Testing|relpos=0/@FAILURES|kind=Testing|relpos=0/@CalmWaterVelocityInvalid|kind=CFDTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "CFD_CalmWaterSolverVelocityInvalid");

            string cfdGeneratedDir = Path.Combine(OutputDir, "CFD");
            string cfdOutputJson = Path.Combine(cfdGeneratedDir, "CFDConfig.json");
            string generatedAssemblyFilePath = Path.Combine(OutputDir, generatedAsmFile);

            bool status = CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath);
            Assert.False(File.Exists(generatedAssemblyFilePath), generatedAsmFile + " was generated");
            Assert.False(File.Exists(cfdOutputJson), "CFDConfig.json was generated");
        }

        [Fact]
        public void CFD_Failure_CorrelationVelocityInvalid()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CFD\SolverSettings\wigley_hull.xme");
            string TestbenchPath = 
                "/@CFD_Testing|kind=Testing|relpos=0/@FAILURES|kind=Testing|relpos=0/@CorrelationVelocityInvalid|kind=CFDTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "CFD_CorrelationVelocityInvalid");

            string cfdGeneratedDir = Path.Combine(OutputDir, "CFD");
            string cfdOutputJson = Path.Combine(cfdGeneratedDir, "CFDConfig.json");
            string generatedAssemblyFilePath = Path.Combine(OutputDir, generatedAsmFile);

            bool status = CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath);
            Assert.False(File.Exists(generatedAssemblyFilePath), generatedAsmFile + " was generated");
            Assert.False(File.Exists(cfdOutputJson), "CFDConfig.json was generated");
        }

        [Fact]
        public void CFD_Failure_HydrostaticTempInvalidAngleInvalid()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CFD\SolverSettings\wigley_hull.xme");
            string TestbenchPath = 
                "/@CFD_Testing|kind=Testing|relpos=0/@FAILURES|kind=Testing|relpos=0/@HydrostaticTempInvalidAngleInvalid|kind=CFDTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "CFD_HydrostaticTempInvalidAngleInvalid");

            string cfdGeneratedDir = Path.Combine(OutputDir, "CFD");
            string cfdOutputJson = Path.Combine(cfdGeneratedDir, "CFDConfig.json");
            string generatedAssemblyFilePath = Path.Combine(OutputDir, generatedAsmFile);

            bool status = CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath);
            Assert.False(File.Exists(generatedAssemblyFilePath), generatedAsmFile + " was generated");
            Assert.False(File.Exists(cfdOutputJson), "CFDConfig.json was generated");
        }

        [Fact]
        public void CFD_Failure_MultipleSolvers()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CFD\SolverSettings\wigley_hull.xme");
            string TestbenchPath =
                "/@CFD_Testing|kind=Testing|relpos=0/@FAILURES|kind=Testing|relpos=0/@MultipleSolvers|kind=CFDTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "CFD_MultipleSolvers");

            string cfdGeneratedDir = Path.Combine(OutputDir, "CFD");
            string cfdOutputJson = Path.Combine(cfdGeneratedDir, "CFDConfig.json");
            string generatedAssemblyFilePath = Path.Combine(OutputDir, generatedAsmFile);

            bool status = CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath);
            Assert.False(File.Exists(generatedAssemblyFilePath), generatedAsmFile + " was generated");
            Assert.False(File.Exists(cfdOutputJson), "CFDConfig.json was generated");
        }

        [Fact]
        public void CFD_Failure_WaveVelocityInvalidTempInvalidFreqInvalid()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CFD\SolverSettings\wigley_hull.xme");
            string TestbenchPath =
                "/@CFD_Testing|kind=Testing|relpos=0/@FAILURES|kind=Testing|relpos=0/@WaveVelocityInvalidTempInvalidFreqInvalid|kind=CFDTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "CFD_WaveVelocityInvalidTempInvalidFreqInvalid");

            string cfdGeneratedDir = Path.Combine(OutputDir, "CFD");
            string cfdOutputJson = Path.Combine(cfdGeneratedDir, "CFDConfig.json");
            string generatedAssemblyFilePath = Path.Combine(OutputDir, generatedAsmFile);

            bool status = CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath);
            Assert.False(File.Exists(generatedAssemblyFilePath), generatedAsmFile + " was generated");
            Assert.False(File.Exists(cfdOutputJson), "CFDConfig.json was generated");
        }

        [Fact]
        public void Connectors_Invalid()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\CADTeam\MSD_CAD.xme");
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "Connectors_Invalid");
            var generatedAsmPath = Path.Combine(OutputDir, generatedAsmFile);

            CyPhyGUIs.CyPhyDirectory.EnsureEmptyDirectory(OutputDir);

            string ProjectConnStr;
            MgaUtils.ImportXMEForTest(XmePath, Path.Combine(OutputDir, Path.GetFileNameWithoutExtension(XmePath) + "_CADtest.mga"), out ProjectConnStr);
            CyPhy2CADRun.CopyDirectory(Path.Combine(Path.GetDirectoryName(XmePath), "components"), Path.Combine(Path.Combine(OutputDir, "components")));

            MgaProject project = new MgaProject();
            bool ro_mode;
            project.Open(ProjectConnStr, out ro_mode);
            try
            {

                // testbenchpath doesn't work:(
                var terr = project.BeginTransactionInNewTerr();
                var testObj = project.RootFolder.GetObjectByPathDisp("/@Generated_configurations/MyMassSpringDamper/Config1/MyMassSpringDamper_cfg_broken");
                project.AbortTransaction();

                bool status = CyPhy2CADRun.Run(Path.GetDirectoryName(XmePath), project, (MgaFCO)testObj, false);

                string logfilename = Path.Combine(OutputDir, "log", CyPhy2CAD_CSharp.Logger.LogFileName);

                StreamReader r = new StreamReader(logfilename);
                string logcontent = r.ReadToEnd();

                Assert.True(logcontent.Contains("badname"));
            }
            finally
            {
                project.Close(abort: true);
            }

        }

        [Fact]
        public void KinematicTB_4Bar()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@Kinematics|kind=Testing|relpos=0/@Kinematic_FourBar|kind=KinematicTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "Kinematic_FourBar");

            bool status = CADTeamTest.CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath, true);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedAsmFile)), "Failed to generate " + CADTeamTest.CADTests.generatedAsmFile);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedMBDFile)), "Failed to generate " + CADTeamTest.CADTests.generatedMBDFile);
        }

        [Fact]
        public void FEATB_4Bar()
        {
            string XmePath = Path.GetFullPath(@"..\..\..\..\models\MBD\MBD.xme");
            string TestbenchPath = "/@Testing|kind=Testing|relpos=0/@FEA|kind=Testing|relpos=0/@FourBar|kind=Testing|relpos=0/@ModelBased|kind=Testing|relpos=0/@FEA_Fourbar|kind=CADTestBench|relpos=0";
            string OutputDir = Path.Combine(Path.GetDirectoryName(XmePath), "FEA_FourBar");

            bool status = CADTeamTest.CyPhy2CADRun.Run(OutputDir, XmePath, TestbenchPath, true);
            Assert.True(File.Exists(Path.Combine(OutputDir, CADTeamTest.CADTests.generatedAsmFile)), "Failed to generate " + CADTeamTest.CADTests.generatedAsmFile);
        }


    }
}
