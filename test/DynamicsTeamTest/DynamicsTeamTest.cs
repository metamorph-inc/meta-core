using System;
using GME.MGA;
using System.IO;
using System.Reflection;

namespace DynamicsTeamTest
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
            var xmePath = Path.Combine("..", "..", ".." , "..", "models", "DynamicsTeam", projectDir, xmeFileName);
            string projectConnStr;
            MgaUtils.ImportXMEForTest(xmePath, out projectConnStr);
            return projectConnStr.Substring("MGA=".Length);
        }

        public static void DeleteDirectory(string target_dir)
        {
            string[] files = Directory.GetFiles(target_dir);
            string[] dirs = Directory.GetDirectories(target_dir);

            foreach (string file in files)
            {
                File.SetAttributes(file, FileAttributes.Normal);
                File.Delete(file);
            }

            foreach (string dir in dirs)
            {
                DeleteDirectory(dir);
            }

            Directory.Delete(target_dir, false);
        }
    }

    public abstract class XmeImportFixture
    {
        protected abstract string xmeFilename { get; }
        private Exception importException;
        private string _mgaFile;
        public string mgaFile
        {
            get
            {
                if (importException != null)
                {
                    throw new Exception("Xme import failed", importException);
                }
                return _mgaFile;
            }
        }

        public XmeImportFixture()
        {
            try
            {
                this._mgaFile = ImportXME2Mga();
            }
            catch (Exception e)
            {
                importException = e;
            }
        }

        public string ImportXME2Mga()
        {
            string projectConnStr;
            MgaUtils.ImportXMEForTest(Path.Combine(
                Path.GetDirectoryName(new Uri(System.Reflection.Assembly.GetAssembly(typeof(XmeImportFixture)).CodeBase).LocalPath),
                "..", "..", "..", "..", "models", "DynamicsTeam", xmeFilename), out projectConnStr);
           return projectConnStr.Substring("MGA=".Length);
        }
    }
}
