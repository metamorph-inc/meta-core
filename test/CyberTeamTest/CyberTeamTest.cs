using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using Xunit;
using System.IO;

namespace CyberTeamTest
{
    public class Test
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

        /// <summary>
        /// Call this method for each test-case.
        /// </summary>
        /// <param name="projectDir"> e.g. DriveTrain_v3</param>
        /// <param name="xmeFileName">e.g. DriveLine_v3.xme</param>
        /// <returns>e.g. C:\META\meta_trunk\models\DynamicsTeam\DriveTrain_v3\DriveLine_v3_test.mga</returns>
        public static string ImportXME2Mga(string projectDir, string xmeFileName)
        {
            var xmePath = Path.Combine("..", "..", "..", "..", "models", "Cyber", projectDir, xmeFileName);
            string projectConnStr;
            GME.MGA.MgaUtils.ImportXMEForTest(xmePath, out projectConnStr);
            return projectConnStr.Substring("MGA=".Length);
        }
    }

    public class CyberToolsTest
    {
        [Fact]
        public void CreateCyber2SLC()
        {
            Assert.DoesNotThrow(() => {
                var cyberInterpreter = 
                    Activator.CreateInstance(Type.GetTypeFromProgID("MGA.Interpreter.Cyber2SLC_CodeGen"));

                if (cyberInterpreter == null)
                {
                    throw new NullReferenceException("Error creating Cyber2SLC interpreter!");
                }
            });
        }
    }
}
