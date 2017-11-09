using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Xml;
using Xunit;

namespace CADCreoTest
{
    public class CreoImportTest : IUseFixture<CADCreoTest.Cyphy2CADCreoTest.ExtractACMXMLFixture>
    {

        string CADCreoTestPath = Path.GetFullPath(Path.Combine(Path.GetDirectoryName(new Uri(Assembly.GetExecutingAssembly().CodeBase).LocalPath), "..", ".."));
        private Cyphy2CADCreoTest.ExtractACMXMLFixture fixture;

        public void SetFixture(Cyphy2CADCreoTest.ExtractACMXMLFixture data)
        {
            this.fixture = data;
        }

        [Fact]
        public void TestImport()
        {
            var inputPrt = Path.Combine(CADCreoTestPath, "damperimporttest.prt.1");
            var outputXml = inputPrt + ".xml";
            if (File.Exists(outputXml))
            {
                File.Delete(outputXml);
            }
            ProcessStartInfo info = new ProcessStartInfo()
            {
                FileName = fixture.extractXmlExe,
                Arguments = string.Format("-c \"{0}\"", inputPrt),
                UseShellExecute = false,
                RedirectStandardInput = true,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
            };
            Process extractXml = new Process();
            extractXml.StartInfo = info;

            extractXml.Start();
            IntPtr createAssemblyJob = CyPhyMetaLink.JobObjectPinvoke.AssignProcessToKillOnCloseJob(extractXml);

            try
            {
                using (extractXml)
                {
                    extractXml.StandardInput.Close();
                    var stdout = extractXml.StandardOutput.ReadToEnd();
                    var stderr = extractXml.StandardError.ReadToEnd();

                    Console.Out.Write(stdout);
                    Console.Error.Write(stderr);
                    Assert.True(extractXml.ExitCode == 0, "XML export failed");
                }

                XmlDocument doc = new XmlDocument();

                doc.Load(outputXml);

                var names = new XmlNamespaceManager(doc.NameTable);
                names.AddNamespace("xsi", "http://www.w3.org/2001/XMLSchema-instance");
                names.AddNamespace("avm", "avm");
                names.AddNamespace("cad", "cad");

                var damper_value = doc.SelectSingleNode("/avm:Component/DomainModel/Parameter[@Name='DAMPER_D']/Value", names);
                Assert.Equal("m", damper_value.Attributes["Unit"].Value);
                Assert.Equal("3558.69", damper_value.SelectSingleNode("./ValueExpression/Value").InnerText);

                Assert.Equal("PVC", doc.SelectSingleNode("/avm:Component/DomainModel/Parameter[@Name='PTC_MATERIAL_NAME']/Value/ValueExpression/Value", names).InnerText);

                //<Datum Name="" ID="cad.COMMON_AXIS" DatumName="COMMON_AXIS" xsi:type="ns3:Axis"/>
                Assert.Equal("ns3:Axis", doc.SelectSingleNode("/avm:Component/DomainModel/Datum[@ID='cad.COMMON_AXIS']", names).Attributes["xsi:type"].Value);
                Assert.Equal("ns3:Plane", doc.SelectSingleNode("/avm:Component/DomainModel/Datum[@ID='cad.COMMON_PLANE_1_TOP']", names).Attributes["xsi:type"].Value);


                var resourceDependency = doc.SelectSingleNode("/avm:Component/ResourceDependency", names);
                Assert.Equal(inputPrt.ToLowerInvariant(), Path.Combine(resourceDependency.Attributes["Path"].Value, resourceDependency.Attributes["Name"].Value + ".1").ToLowerInvariant());
            }
            finally
            {
                CyPhyMetaLink.JobObjectPinvoke.CloseHandle(createAssemblyJob);
            }
        }
    }
}
