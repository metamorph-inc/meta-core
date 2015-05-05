using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using GME.MGA;
using Xunit;
using System.Xml;
using System.Collections.Generic;
using System.Reflection;
using GME.CSharp;

namespace DesignExporterUnitTests
{
    public class Program
    {
        private int processCommon(Process process)
        {
            process.StartInfo.UseShellExecute = false;
            process.StartInfo.CreateNoWindow = true;

            process.Start();
            process.WaitForExit();

            return process.ExitCode;
        }

        private void UnpackXmes(string xmeFile)
        {
            unpackXme(xmeFile);
        }

        private void unpackXme(string xmeFilename)
        {
            if (!File.Exists(xmeFilename)) return;
            var mgaFilename = Path.ChangeExtension(xmeFilename, "mga");
            MgaUtils.ImportXME(xmeFilename, mgaFilename);
        }

        private int RunCyPhyDesignExporter(string mgaFile, string designName = "")
        {
            // Environment.CurrentDirectory = (new FileInfo(mgaFile)).DirectoryName;
            // return CyPhyDesignExporterCL.Program.Main(new string[] { "-m", mgaFile, "-d", designName });

            var process = new Process
                          {
                              StartInfo =
                              {
                                  FileName = Path.Combine(META.VersionInfo.MetaPath, "src", "CyPhyDesignExporterCL", "bin", "Release", "CyPhyDesignExporterCL.exe"),
                                  WorkingDirectory = (new FileInfo(mgaFile)).DirectoryName
                              }
                          };


            process.StartInfo.Arguments += String.Format(" -m {0}", mgaFile) + (string.IsNullOrEmpty(designName) ? string.Empty : String.Format(" -d {0}", designName));
            string output;
            Assert.True(0 == ComponentExporterUnitTests.Common.runProcessAndGetOutput(process, out output, err_only: true), process.StartInfo.FileName + " failed:" + output);
            return 0;
        }

        private int RunXmlComparator(string exported, string desired)
        {
            string output;
            int retval = RunXmlComparator(exported, desired, out output);
            Assert.True(retval == 0, "Output did not match expected: " + output);
            return retval;
        }

        private int RunXmlComparator(string exported, string desired, out string output)
        {
            var process = new Process
            {
                StartInfo =
                {
                    FileName = Path.Combine(_interchangeBinDirectory, "InterchangeXmlComparator.exe"),
                    WorkingDirectory = new FileInfo(exported).DirectoryName
                }
            };


            process.StartInfo.Arguments += String.Format(" -e {0} -d {1} -m Design", exported, desired);
            return ComponentExporterUnitTests.Common.runProcessAndGetOutput(process, out output, err_only: true);
        }

        [Fact]
        [Trait("Interchange", "JoinData")]
        [Trait("Interchange", "Design Export")]
        public void ExportJoinData_CA_with_Instances()
        {
            var fullTestPath = Path.Combine(_exportModelDirectory, "JoinDataSimpleTest");
            var xmeFilePath = Path.Combine(fullTestPath, "JoinDataSimpleExportTest.xme");
            UnpackXmes(xmeFilePath);
            var mgaFile = xmeFilePath.Replace(".xme", ".mga");
            var designName = "CA_with_Instances";

            Assert.True(RunCyPhyDesignExporter(mgaFile, designName) == 0, "Design Export failed");

            var exportedXml = Path.Combine(fullTestPath, designName + ".adm");
            var desiredResultsXml = Path.Combine(fullTestPath, "CA_with_Instances_expected.xml");
            Assert.True(RunXmlComparator(exportedXml, desiredResultsXml) == 0, "Output did not match expected");
        }

        [Fact]
        [Trait("Interchange", "JoinData")]
        [Trait("Interchange", "Design Export")]
        public void ExportJoinData_CA_with_References()
        {
            var fullTestPath = Path.Combine(_exportModelDirectory, "JoinDataSimpleTest");
            var xmeFilePath = Path.Combine(fullTestPath, "JoinDataSimpleExportTest.xme");
            UnpackXmes(xmeFilePath);
            var mgaFile = xmeFilePath.Replace(".xme", ".mga");
            var designName = "CA_with_References";

            Assert.True(RunCyPhyDesignExporter(mgaFile, designName) == 0, "Design Export failed");

            var exportedXml = Path.Combine(fullTestPath, designName + ".adm");
            var desiredResultsXml = Path.Combine(fullTestPath, "CA_with_References_expected.xml");
            Assert.True(RunXmlComparator(exportedXml, desiredResultsXml) == 0, "Output did not match expected");
        }

        [Fact]
        [Trait("Interchange", "JoinData")]
        [Trait("Interchange", "Design Export")]
        public void ExportJoinData_ExposedConnectorTest()
        {
            var fullTestPath = Path.Combine(_exportModelDirectory, "JoinDataUnitTests");
            var xmeFilePath = Path.Combine(fullTestPath, "JoinDataUnitTests.xme");
            UnpackXmes(xmeFilePath);
            var mgaFile = xmeFilePath.Replace(".xme", ".mga");
            var designName = "ExposedConnector_TopAsm";

            Assert.True(RunCyPhyDesignExporter(mgaFile, designName) == 0, "Design Export failed");

            var exportedXml = Path.Combine(fullTestPath, designName + ".adm");
            var desiredResultsXml = Path.Combine(fullTestPath, "ExposedConnector_expected.xml");
            Assert.True(RunXmlComparator(exportedXml, desiredResultsXml) == 0, "Output did not match expected");
        }

        [Fact]
        [Trait("Interchange", "JoinData")]
        [Trait("Interchange", "Design Export")]
        public void All_5_JoinTypes_Complex_CATest()
        {
            var fullTestPath = Path.Combine(_exportModelDirectory, "JoinDataUnitTests");
            var xmeFilePath = Path.Combine(fullTestPath, "JoinDataUnitTests.xme");
            UnpackXmes(xmeFilePath);
            var mgaFile = xmeFilePath.Replace(".xme", ".mga");
            var designName = "All_5_JoinTypes_TopAsm_CA";

            Assert.True(RunCyPhyDesignExporter(mgaFile, designName) == 0, "Design Export failed");

            var exportedXml = Path.Combine(fullTestPath, designName + ".adm");
            var desiredResultsXml = Path.Combine(fullTestPath, "All5_CA_Expected.xml");
            Assert.True(RunXmlComparator(exportedXml, desiredResultsXml) == 0, "Output did not match expected");
        }

        [Fact]
        [Trait("Interchange", "JoinData")]
        [Trait("Interchange", "Design Export")]
        public void All_5_JoinTypes_Complex_DSTest()
        {
            var fullTestPath = Path.Combine(_exportModelDirectory, "JoinDataUnitTests");
            var xmeFilePath = Path.Combine(fullTestPath, "JoinDataUnitTests.xme");
            UnpackXmes(xmeFilePath);
            var mgaFile = xmeFilePath.Replace(".xme", ".mga");
            var designName = "All_5_JoinTypes_TopAsm_DS";

            Assert.True(RunCyPhyDesignExporter(mgaFile, designName) == 0, "Design Export failed");

            var exportedXml = Path.Combine(fullTestPath, designName + ".adm");
            var desiredResultsXml = Path.Combine(fullTestPath, "All5_DS_Expected.xml");
            Assert.True(RunXmlComparator(exportedXml, desiredResultsXml) == 0, "Output did not match expected");
        }


        [Fact]
        [Trait("Interchange", "Design Export")]
        public void META1974()
        {
            var fullTestPath = Path.Combine(_exportModelDirectory, "META-1974");
            var xmeFilePath = Path.Combine(fullTestPath, "META-1974.xme");
            UnpackXmes(xmeFilePath);
            var mgaFile = xmeFilePath.Replace(".xme", ".mga");
            var designName = "GroundedRI_cfg1";

            Assert.True(RunCyPhyDesignExporter(mgaFile, designName) == 0, "Design Export failed");

            var exportedXml = Path.Combine(fullTestPath, designName + ".adm");
            var desiredResultsXml = Path.Combine(fullTestPath, "expected.adm");
            Assert.True(RunXmlComparator(exportedXml, desiredResultsXml) == 0, "Output did not match expected");
        }

        [Fact]
        [Trait("Interchange", "Design Export")]
        public void DesignSpaceTracing()
        {
            var fullTestPath = Path.Combine(_exportModelDirectory, "DesignSpaceTracing");
            var xmeFilePath = Path.Combine(fullTestPath, "DesignSpaceTracing.xme");
            UnpackXmes(xmeFilePath);
            var mgaFile = xmeFilePath.Replace(".xme", ".mga");

            Assert.True(RunCyPhyDesignExporter(mgaFile) == 0, "Design Export failed");

            var exportedADMs = Directory.GetFiles(fullTestPath, "*expected.adm");
            foreach (var p_expected in exportedADMs)
            {
                var p_output = p_expected.Replace(".expected", "");
                Assert.True(File.Exists(p_output), "Output file missing: " + p_output);
                Assert.True(RunXmlComparator(p_expected, p_output) == 0, "Output did not match expected for " + p_expected);
            }
        }

        [Fact]
        [Trait("Interchange", "Design Export")]
        public void Connector()
        {
            var fullTestPath = Path.Combine(_exportModelDirectory, "Connector");
            var xmeFilePath = Path.Combine(fullTestPath, "Connector.xme");
            string connStr;
            MgaUtils.ImportXMEForTest(xmeFilePath, out connStr);
            var mgaFile = connStr.Substring("MGA=".Length);

            Assert.True(RunCyPhyDesignExporter(mgaFile) == 0, "Design Export failed");

            var exportedADMs = Directory.GetFiles(fullTestPath, "*expected.adm");
            foreach (var p_expected in exportedADMs)
            {
                var p_output = p_expected.Replace(".expected", "");
                Assert.True(File.Exists(p_output), "Output file missing: " + p_output);
                Assert.True(RunXmlComparator(p_expected, p_output) == 0, "Output did not match expected for " + p_expected);
            }
        }

        [Fact]
        public void ComponentAssemblyRefs()
        {
            var fullTestPath = Path.Combine(_exportModelDirectory, "CARefs");
            var xmeFilePath = Path.Combine(fullTestPath, "CARefs.xme");
            UnpackXmes(xmeFilePath);
            var mgaFile = xmeFilePath.Replace(".xme", ".mga");

            Assert.True(RunCyPhyDesignExporter(mgaFile) == 0, "Design Export Failed");

            var exportedADMs = Directory.GetFiles(fullTestPath, "*.adm");
            var results = new Dictionary<String, String>();
            foreach (var adm in exportedADMs)
            {
                var doc = new XmlDocument();
                using (var reader = new XmlTextReader(adm))
                {
                    doc.Load(reader);
                    XmlNode root = doc.DocumentElement;

                    var ls_EncounteredIDs = new List<String>();
                    foreach (XmlAttribute node in root.SelectNodes("//@ID"))
                    {
                        ls_EncounteredIDs.Add(node.Value);
                    }

                    // Get all duplicate IDs that aren't empty/whitespace
                    var duplicates = ls_EncounteredIDs.Where(s => !String.IsNullOrWhiteSpace(s))
                                                      .GroupBy(s => s)
                                                      .Where(g => g.Count() > 1)
                                                      .Select(g => g.Key)
                                                      .ToList();
                    if (duplicates.Any())
                    {
                        String msg = "Duplicate IDs found in exported design: ";
                        foreach (var dupe in duplicates)
                            msg += String.Format("{0}\"{1}\", ", Environment.NewLine, dupe);

                        results.Add(Path.GetFileName(adm), msg);
                    }
                }
            }

            if (results.Any())
            {
                String finalMsg = String.Format("{0} exported ADM files had duplicate IDs" + Environment.NewLine, results.Count);
                foreach (var kvp in results)
                    finalMsg += String.Format("{0}: {1}" + Environment.NewLine, kvp.Key, kvp.Value);
                Assert.True(false, finalMsg);
            }
        }

        [Fact]
        public void DupIDs()
        {
            var fullTestPath = Path.Combine(_exportModelDirectory, "DupIDs");
            var xmeFilePath = Path.Combine(fullTestPath, "DupIDs.xme");
            string connStr;
            MgaUtils.ImportXMEForTest(xmeFilePath, out connStr);
            var mgaFile = connStr.Substring("MGA=".Length);

            Assert.True(RunCyPhyDesignExporter(mgaFile) == 0, "Design Export Failed");

            var exportedADMs = Directory.GetFiles(fullTestPath, "*.adm");
            var results = new Dictionary<String, String>();
            foreach (var adm in exportedADMs)
            {
                var doc = new XmlDocument();
                using (var reader = new XmlTextReader(adm))
                {
                    doc.Load(reader);
                    XmlNode root = doc.DocumentElement;

                    var ls_EncounteredIDs = new List<String>();
                    foreach (XmlAttribute node in root.SelectNodes("//@ID"))
                    {
                        ls_EncounteredIDs.Add(node.Value);
                    }

                    // Get all duplicate IDs that aren't empty/whitespace
                    var duplicates = ls_EncounteredIDs.Where(s => !String.IsNullOrWhiteSpace(s))
                                                      .GroupBy(s => s)
                                                      .Where(g => g.Count() > 1)
                                                      .Select(g => g.Key)
                                                      .ToList();
                    if (duplicates.Any())
                    {
                        String msg = "Duplicate IDs found in exported design: ";
                        foreach (var dupe in duplicates)
                            msg += String.Format("{0}\"{1}\", ", Environment.NewLine, dupe);

                        results.Add(Path.GetFileName(adm), msg);
                    }
                }
            }

            if (results.Any())
            {
                String finalMsg = String.Format("{0} exported ADM files had duplicate IDs" + Environment.NewLine, results.Count);
                foreach (var kvp in results)
                    finalMsg += String.Format("{0}: {1}" + Environment.NewLine, kvp.Key, kvp.Value);
                Assert.True(false, finalMsg);
            }
        }

        [Fact]
        [Trait("Interchange", "Design Export")]
        public void Refports()
        {
            var fullTestPath = Path.Combine(_exportModelDirectory, "Refports");
            var xmeFilePath = Path.Combine(fullTestPath, "Refports.xme");
            string connstr;
            MgaUtils.ImportXMEForTest(xmeFilePath, out connstr);
            var mgaFile = connstr.Substring("MGA=".Length);
            var designName = "CA";

            Assert.True(RunCyPhyDesignExporter(mgaFile, designName) == 0, "Design Export failed");

            var exportedXml = Path.Combine(fullTestPath, designName + ".adm");

            var design = XSD2CSharp.AvmXmlSerializer.Deserialize<avm.Design>(File.ReadAllText(exportedXml));
            var valueExpression = design.RootContainer.Property.Cast<avm.PrimitiveProperty>().First().Value.ValueExpression;
            Assert.True(valueExpression is avm.FixedValue, valueExpression.GetType().Name);
        }

        //[Fact]
        public void DesignSpaceWithRefs()
        {
            var fullTestPath = Path.Combine(_exportModelDirectory, "DesignSpaceWithRefs");
            var xmeFilePath = Path.Combine(fullTestPath, "DesignSpaceWithRefs.xme");
            var pathExportedDesign = Path.Combine(fullTestPath, "DesignSpace.adm");

            // delete design file if it already exists
            if (File.Exists(pathExportedDesign))
            {
                File.Delete(pathExportedDesign);
            }

            String connectionString;
            GME.MGA.MgaUtils.ImportXMEForTest(xmeFilePath, out connectionString);

            var proj = new MgaProject();
            bool ro_mode;
            proj.Open(connectionString, out ro_mode);
            proj.EnableAutoAddOns(true);

            try
            {
                var designExporter = new CyPhyDesignExporter.CyPhyDesignExporterInterpreter();
                designExporter.Initialize(proj);

                var mgaGateway = new MgaGateway(proj);
                proj.CreateTerritoryWithoutSink(out mgaGateway.territory);
                MgaFCO currentFCO = null;
                mgaGateway.PerformInTransaction(delegate
                {
                    currentFCO = proj.get_ObjectByPath("/@DesignSpaces|kind=DesignSpace|relpos=0/@DesignSpace|kind=DesignContainer|relpos=0") as MgaFCO;
                    Assert.NotNull(currentFCO);
                });

                var parameters = new CyPhyGUIs.InterpreterMainParameters()
                {
                    CurrentFCO = currentFCO,
                    Project = proj,
                    OutputDirectory = fullTestPath
                };

                var result = designExporter.Main(parameters);

                var design = avm.Design.LoadFromFile(pathExportedDesign);

                /// Add assert statements to check structure
                var root = design.RootContainer;
                Assert.NotNull(root);

                Assert.Equal(1, root.Container1.Count(c => c.Name == "AltContainer"));
                var AltContainer = root.Container1.First(c => c.Name == "AltContainer");

                Assert.Equal(1, AltContainer.ComponentInstance.Count(ci => ci.Name == "CompA_5"));
                Assert.Equal(1, AltContainer.ComponentInstance.Count(ci => ci.Name == "CompA_6"));
                Assert.Equal(1, AltContainer.ComponentInstance.Count(ci => ci.Name == "CompA_7"));

                Assert.Equal(1, root.Container1.Count(c => c.Name == "DesignContainer"));
                var DesignContainer = root.Container1.First(c => c.Name == "DesignContainer");

                Assert.Equal(1, DesignContainer.ComponentInstance.Count(ci => ci.Name == "CompA3"));

                Assert.Equal(1, DesignContainer.Container1.Count(c => c.Name == "Assembly1"));
                var Assembly1 = DesignContainer.Container1.First(c => c.Name == "Assembly1");

                Assert.Equal(1, Assembly1.ComponentInstance.Count(ci => ci.Name == "CompA1"));
                Assert.Equal(1, Assembly1.ComponentInstance.Count(ci => ci.Name == "CompA2"));

                Assert.Equal(1, root.Container1.Count(c => c.Name == "ReusableContainer"));
                var ReusableContainer = root.Container1.First(c => c.Name == "ReusableContainer");

                Assert.Equal(1, ReusableContainer.Container1.Count(c => c.Name == "Assembly4"));
                var Assembly4 = ReusableContainer.Container1.First(c => c.Name == "Assembly4");

                Assert.Equal(1, Assembly4.ComponentInstance.Count(ci => ci.Name == "CompA1"));
                Assert.Equal(1, Assembly4.ComponentInstance.Count(ci => ci.Name == "CompA2"));

                Assert.Equal(1, ReusableContainer.ComponentInstance.Count(ci => ci.Name == "CompA4"));

                Assert.Equal(1, root.ComponentInstance.Count(ci => ci.Name == "CompA"));

                Assert.Equal(1, root.ComponentInstance.Count(ci => ci.Name == "CompA"));
            }
            finally
            {
                proj.Close();
            }
        }

        private static readonly string InterchangeTestDirectory = Path.Combine(META.VersionInfo.MetaPath, "test", "InterchangeTest");
        private static string _interchangeBinDirectory = Path.Combine(InterchangeTestDirectory, @"InterchangeXmlComparator\bin\Release");
        private static string _exportModelDirectory = Path.Combine(InterchangeTestDirectory, "DesignInterchangeTest", "ExportTestModels");
        private static string _importModelDirectory = Path.Combine(InterchangeTestDirectory, "DesignInterchangeTest", "ImportTestModels");

        [STAThread]
        static int Main(string[] args)
        {
            int ret = Xunit.ConsoleClient.Program.Main(new string[] {
                Assembly.GetExecutingAssembly().CodeBase.Substring("file:///".Length),
                //"/noshadow",
            });
            Console.In.ReadLine();
            return ret;
        }
    }
}
