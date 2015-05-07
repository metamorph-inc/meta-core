using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.IO;
using Microsoft.Win32;
using Xunit;
using System.Reflection;

using GME.MGA;
using GME.CSharp;
using System.Text.RegularExpressions;

namespace ComponentImporterUnitTests
{

    public class Tests
    {
        private int processCommon(Process process)
        {
            process.StartInfo.UseShellExecute = false;
            process.StartInfo.CreateNoWindow = true;

            process.Start();
            process.WaitForExit();

            return process.ExitCode;
        }

        private void unpackXmes(string testName) {
            unpackXme(Path.Combine(Common._importModelDirectory, testName, "DesiredResult.xme"));
            unpackXme(Path.Combine(Common._importModelDirectory, testName, "InputModel.xme"));
        }

        private void unpackXme(string xmeFilename)
        {
            if (!File.Exists(xmeFilename)) return;
            string mgaFilename = Path.ChangeExtension(xmeFilename, "mga");
            GME.MGA.MgaUtils.ImportXME(xmeFilename, mgaFilename);
        }

        private int RunCyPhyComponentImporterCL(string testName) {
            var process = new Process
                          {
                              StartInfo =
                              {
                                  FileName = Path.Combine(META.VersionInfo.MetaPath, "src", "CyPhyComponentImporterCL", "bin", "Release", "CyPhyComponentImporterCL.exe"),
                                  WorkingDirectory = Path.Combine(Common._importModelDirectory, testName)
                              }
                          };

            process.StartInfo.Arguments += "InputModel.component.acm";
            process.StartInfo.Arguments += " InputModel.mga";
            
            return processCommon(process);
        }

        private int RunCyPhyMLComparator(string testName)
        {
            var process = new Process
                          {
                              StartInfo =
                              {
                                  FileName = Path.Combine(META.VersionInfo.MetaPath, "src", "bin", "CyPhyMLComparator.exe"),
                                  WorkingDirectory = Path.Combine(Common._importModelDirectory, testName)
                              }
                          };

            process.StartInfo.Arguments += "DesiredResult.mga";
            process.StartInfo.Arguments += " InputModel.mga";

            return processCommon(process);
        }

        [Fact]
        public void ImportTest()
        {
            const string testName = "ImportTest";
            unpackXmes(testName);
            Assert.True(0 == RunCyPhyComponentImporterCL(testName), "Importer exited with non-zero return code.");
            Assert.True(0 == RunCyPhyMLComparator(testName), "Imported model doesn't match expected.");
        }

        [Fact]
        public void ImportWithResourceTest()
        {
            const string testName = "ImportWithResource";
            unpackXmes(testName);
            Assert.True(0 == RunCyPhyComponentImporterCL(testName), "Importer exited with non-zero return code.");
            Assert.True(0 == RunCyPhyMLComparator(testName), "Imported model doesn't match expected.");
        }

		[Fact]
        public void HierarchicalPropertyFlattenTest()
        {
            string testName = "HierarchicalPropertyFlatten";
            unpackXmes(testName);
            Assert.True(0 == RunCyPhyComponentImporterCL(testName), "Importer exited with non-zero return code.");
            Assert.True(0 == RunCyPhyMLComparator(testName), "Imported model doesn't match expected.");
        }

        [Fact]
        public void PropertiesWithinConnectorsTest()
        {
            string testName = "PropertiesWithinConnectors";
            unpackXmes(testName);
            Assert.True(0 == RunCyPhyComponentImporterCL(testName), "Importer exited with non-zero return code.");
            Assert.True(0 == RunCyPhyMLComparator(testName), "Imported model doesn't match expected.");
        }

        [Fact]
        public void CADPropertyTest()
        {
            string testName = "CADProperty";
            unpackXmes(testName);
            Assert.True(0 == RunCyPhyComponentImporterCL(testName),"Importer exited with non-zero return code.");
            Assert.True(0 == RunCyPhyMLComparator(testName),"Imported model doesn't match expected.");
        }

        [Fact]
        public void ParametricProperty()
        {
            string p_TestFolder = Path.Combine(Common.InterchangeTestDirectory, 
                                                "ComponentInterchangeTest", 
                                                "SharedModels", 
                                                "CompParametricProperty");

            string p_InputModelXME = Path.Combine(p_TestFolder,"InputModel.xme");
            string p_InputModelMGA;
            GME.MGA.MgaUtils.ImportXMEForTest(p_InputModelXME, Path.GetFileNameWithoutExtension(p_InputModelXME) + "_importertest.mga", out p_InputModelMGA);
            p_InputModelMGA = p_InputModelMGA.Replace("MGA=", "");
            Assert.True(File.Exists(p_InputModelMGA), String.Format("{0} not found. Import may have failed.",Path.GetFileName(p_InputModelMGA)));

            string p_DesiredResultXME = Path.Combine(p_TestFolder, "DesiredResult.xme");
            string p_DesiredResultMGA;
            GME.MGA.MgaUtils.ImportXMEForTest(p_DesiredResultXME, Path.GetFileNameWithoutExtension(p_InputModelXME) + "_importertest.mga", out p_DesiredResultMGA);
            p_DesiredResultMGA = p_DesiredResultMGA.Replace("MGA=", "");
            Assert.True(File.Exists(p_DesiredResultMGA), "DesiredResult.mga not found. Import may have failed.");

            String p_InputACM = Path.Combine(p_TestFolder, "InputModel.component.acm");
            Assert.True(0 == Common.runCyPhyComponentImporterCL(p_InputModelMGA, p_InputACM),
                        "Component Importer failed.");

            Assert.True(0 == Common.RunCyPhyMLComparator(p_DesiredResultMGA, p_InputModelMGA),
                        "Imported model doesn't match expected");
        }
		
        [Fact]
        public void ProjectManifestPopulationTest()
        {
            var testPath = Path.Combine(Common._importModelDirectory, "ProjectManifestPopulation");
            var xmePath = Path.Combine(testPath, "InputModel.xme");

            var pathGeneratedManifest = Path.Combine(testPath, "manifest.project.json");
            // Delete manifest, if it exists
            if (File.Exists(pathGeneratedManifest))
                File.Delete(pathGeneratedManifest);

            var mgaFilename = Path.ChangeExtension(xmePath, "mga");
            GME.MGA.MgaUtils.ImportXME(xmePath, mgaFilename);

            var mgaProject = Common.GetProject( mgaFilename );
            Assert.True(mgaProject != null,"Could not load MGA project.");

            AVM.DDP.MetaAvmProject proj = null;
            bool resultIsNull = false;
            var mgaGateway = new MgaGateway(mgaProject);
            mgaProject.CreateTerritoryWithoutSink(out mgaGateway.territory);
            mgaGateway.PerformInTransaction(delegate {
                var importer = new CyPhyComponentImporter.CyPhyComponentImporterInterpreter();
                importer.Initialize(mgaProject);

                var result = importer.ImportFile(mgaProject,testPath,Path.Combine(testPath,"InputModel.component.acm"));
                if (result == null)
                    resultIsNull = true;

                // Load manifest while we're in a transaction
                proj = AVM.DDP.MetaAvmProject.Create(mgaProject);
            });
            Assert.False(resultIsNull,"Exception occurred during import.");
            Assert.False(File.Exists(pathGeneratedManifest), "Manifest erroneously generated");
        }

        
        
    }

    class Program
    {
        [STAThread]
        static int Main(string[] args)
        {
            int ret = Xunit.ConsoleClient.Program.Main(new string[] {
                Assembly.GetAssembly(typeof(Tests)).CodeBase.Substring("file:///".Length),
                //"/noshadow",
            });
            Console.In.ReadLine();
            return ret;
        }
    }
}
