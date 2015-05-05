using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using System.Collections.Concurrent;
using System.Threading.Tasks;

namespace ComponentImporterUnitTests
{
    public class PyLibTestFixture
    {
        public PyLibTestFixture()
        {
            // Import Driveline XME file
            if (Directory.Exists(PyLibTest.acmFilesPath))
                Directory.Delete(PyLibTest.acmFilesPath, true);
            Directory.CreateDirectory(PyLibTest.acmFilesPath);

            File.Delete(PyLibTest.mgaPath);
            GME.MGA.MgaUtils.ImportXME(PyLibTest.xmePath, PyLibTest.mgaPath);
            Assert.True(File.Exists(PyLibTest.mgaPath));

            var args = (PyLibTest.mgaPath + " -f " + PyLibTest.acmFilesPath).Split();
            var rtnCode = CyPhyComponentExporterCL.CyPhyComponentExporterCL.Main(args);
            Assert.True(rtnCode == 0, "Component Exporter had non-zero return code.");
        }
    }

    public class PyLibTest : IUseFixture<PyLibTestFixture>
    {
        #region Path Variables
        public static readonly string testPath = Path.Combine(
            META.VersionInfo.MetaPath,
            "models",
            "DynamicsTeam",
            "DriveLine_v3"
            );
        public static readonly string xmePath = Path.Combine(
            testPath,
            "DriveLine_v3.xme"
            );
        public static readonly string mgaPath = Path.Combine(
            testPath,
            "DriveLine_v3_PyImportTest.mga"
            );
        public static readonly string acmFilesPath = Path.Combine(
            testPath,
            "ExportedComponents"
            );
        #endregion

        #region Fixture
        PyLibTestFixture fixture;
        public void SetFixture(PyLibTestFixture data)
        {
            fixture = data;
        }
        #endregion
        
        [Fact]
        public void PythonLibraryTest()
        {
            // Find all exported ACM files
            var acms = Directory.EnumerateFiles(acmFilesPath, "*.acm", SearchOption.AllDirectories);
            ConcurrentBag<String> cb_Failures = new ConcurrentBag<String>();
            Parallel.ForEach(acms, pathACM =>
            {
                var absPathACM = Path.Combine(acmFilesPath, pathACM);
                String output;
                int rtnCode = PyLibUtils.TryImportUsingPyLib(absPathACM, out output);

                if (rtnCode != 0)
                    cb_Failures.Add(pathACM);
            });

            if (cb_Failures.Any())
            {
                var msg = "AVM PyLib failed to parse:" + Environment.NewLine;
                foreach (var acmPath in cb_Failures)
                {
                    msg += String.Format("{0}" + Environment.NewLine, acmPath);
                }
                Assert.False(true, msg);
            }
        }
    }
}
