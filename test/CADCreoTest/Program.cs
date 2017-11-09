using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CADCreoTest
{
    class Program
    {
        static void Main(string[] args)
        {
            int ret = Xunit.ConsoleClient.Program.Main(new string[] {
                System.Reflection.Assembly.GetAssembly(typeof(CyPhyPropagateTest.MetaLinkCreoTest)).CodeBase.Substring("file:///".Length),
                //"/noshadow",
                // [Trait("THIS", "ONE")]
                // "/trait", "THIS=ONE",
            });
            Console.In.ReadLine();
        }
    }
}
