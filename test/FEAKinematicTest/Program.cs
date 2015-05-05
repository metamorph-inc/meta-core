using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FEAKinematicTest
{
    class Program
    {
        static void Main(string[] args)
        {
            int ret = Xunit.ConsoleClient.Program.Main(new string[] {
                System.Reflection.Assembly.GetAssembly(typeof(FEAKinematicTest)).CodeBase.Substring("file:///".Length),
                //"/noshadow",
            });
            Console.In.ReadLine();
        }
    }
}
