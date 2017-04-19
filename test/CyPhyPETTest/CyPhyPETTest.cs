using GME.MGA;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xunit;

namespace CyPhyPETTest
{
    public class CyPhyPETTest
    {
        static void Main(string[] args)
        {
            int ret = Xunit.ConsoleClient.Program.Main(new string[] {
                System.Reflection.Assembly.GetAssembly(typeof(CyPhyPETTest)).CodeBase.Substring("file:///".Length),
                //"/noshadow",
            });
            Console.In.ReadLine();
            //System.Console.Out.WriteLine("HEllo World");
        }

        [Fact]
        public void TestCodeParameters()
        {
            Dictionary<string, object> assignment = CyPhyPET.PET.getPythonAssignment("x = 1\ny = '2'");
            Assert.Equal(1L, assignment["x"]);
            Assert.Equal("2", assignment["y"]);
        }

        [Fact]
        public void Test_CyPhyPET_unit_matcher()
        {

            string projectConnStr;
            MgaUtils.ImportXMEForTest(Path.Combine(
                Path.GetDirectoryName(new Uri(System.Reflection.Assembly.GetAssembly(typeof(CyPhyPETTest)).CodeBase).LocalPath),
                "..", "..", "..", "..", "models", "PET_simple_proof-of-concept", "WorkFlow_PET.xme"), out projectConnStr);
            var project = new MgaProject();
            project.OpenEx(projectConnStr, "CyPhyML", null);
            try
            {
                project.BeginTransactionInNewTerr();
                try
                {
                    var wrapper = project.RootFolder.ObjectByPath["/@Testing/@ParametricExploration/@TestPython/@PythonWrapper"];
                    var pythonBlock = ISIS.GME.Dsml.CyPhyML.Classes.PythonWrapper.Cast(wrapper);

                    var binDir = Path.GetDirectoryName(System.Reflection.Assembly.GetAssembly(typeof(CyPhyPETTest)).CodeBase.Substring("file:///".Length));
                    var path = Path.GetFullPath(binDir + "/../../paraboloid.py");
                    var pet = CyPhyPET.CyPhyPETInterpreter.GetParamsAndUnknownsForPythonOpenMDAO(path, pythonBlock);


                    // "{\"unknowns\": {\"f_xy\": {\"units\": \"m**3\", \"gme_unit_id\": \"id-0065-00000179\", \"shape\": 1, \"val\": 0.0, \"size\": 1}}, \"params\": {\"x\": {\"units\": \"m\", \"gme_unit_id\": \"id-0066-00000014\", \"shape\": 1, \"val\": 0.0, \"size\": 1}, \"y\": {\"units\": \"m**3\", \"gme_unit_id\": \"id-0065-00000179\", \"shape\": 1, \"val\": 0.0, \"size\": 1}}}"
                    var unknowns = pet["unknowns"];
                    var params_ = pet["params"];
                    var x = params_["x"];
                    var y = params_["y"];
                    var f_xy = unknowns["f_xy"];
                    Assert.Equal("Meter", project.GetFCOByID((string)x["gme_unit_id"]).Name);
                    Assert.Equal("Cubic Meter", project.GetFCOByID((string)y["gme_unit_id"]).Name);
                    Assert.Equal("Cubic Meter", project.GetFCOByID((string)f_xy["gme_unit_id"]).Name);
                }
                finally
                {
                    project.CommitTransaction();
                }
            }
            finally
            {
                project.Close(abort: true);
            }
        }
    }
}
