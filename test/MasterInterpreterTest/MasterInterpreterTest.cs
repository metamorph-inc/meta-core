using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using GME.MGA;
using GME.CSharp;
using System.IO;
using Xunit;
using System.Reflection;
using GME.MGA.Parser;
using GME.Util;

namespace MasterInterpreterTest
{
    public static class MgaHelper
    {
        public static void CheckParadigmVersionUpgrade(MgaProject project)
        {
            GME.Util.MgaRegistrar registar = new GME.Util.MgaRegistrar();
            string MetaConnStr;
            object MetaGuid = null;
            object ProjectMetaGuid;
            project.BeginTransactionInNewTerr();
            try
            {
                registar.QueryParadigm(project.MetaName, out MetaConnStr, ref MetaGuid, GME.Util.regaccessmode_enum.REGACCESS_BOTH);
                ProjectMetaGuid = project.MetaGUID;
            }
            finally
            {
                project.AbortTransaction();
            }
            if (((Array)ProjectMetaGuid).Cast<byte>().SequenceEqual(((Array)MetaGuid).Cast<byte>()) == false)
            {
                Xunit.Assert.True(false, string.Format("Please upgrade {0} to the latest registered {1} paradigm", project.ProjectConnStr, project.MetaName));
            }
        }
    }

    public class Test
    {
        [STAThread]
        static int Main(string[] args)
        {
            int ret = Xunit.ConsoleClient.Program.Main(new string[] {
                Assembly.GetAssembly(typeof(Test)).CodeBase.Substring("file:///".Length),
                //"/noshadow",
                // "/trait", "THIS=ONE",
            });
            Console.In.ReadLine();
            return ret;
        }

        public static string ImportXME2Mga(string projectDir, string xmeFileName)
        {
            var xmePath = Path.Combine("..", "..", "..", "..", "models", "DynamicsTeam", projectDir, xmeFileName);
            string projectConnStr;
            MgaUtils.ImportXMEForTest(xmePath, out projectConnStr);
            return projectConnStr.Substring("MGA=".Length);
        }
    }
}
