using System;
using System.Collections.Generic;
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
    }
}
