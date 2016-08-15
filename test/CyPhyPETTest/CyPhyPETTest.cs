using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CyPhyPETTest
{
    class CyPhyPETTest
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
    }
}
