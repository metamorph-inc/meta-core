using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CADTeamTest
{
    public class Program
    {
        public static int Main(string[] args)
        {
            int ret = Xunit.ConsoleClient.Program.Main(new string[] {
                System.Reflection.Assembly.GetAssembly(typeof(CADTeamTest.CADTests)).CodeBase.Substring("file:///".Length),
                //"/noshadow",
            });
            Console.In.ReadLine();
            return 0;
        }
    }
}
