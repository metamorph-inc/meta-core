using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CADTeamTest
{
    public class Program
    {
        [STAThread]
        public static int Main(string[] args)
        {
            int ret = Xunit.ConsoleClient.Program.Main(new string[] {
                System.Reflection.Assembly.GetAssembly(typeof(CADTeamTest.CADTests)).CodeBase.Substring("file:///".Length),
                //"/noshadow",
                // [Trait("THIS", "ONE")]
                // "/trait", "THIS=ONE",
            });
            Console.In.ReadLine();
            return 0;
        }
    }
}
