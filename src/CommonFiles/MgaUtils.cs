using System.IO;
using GME.MGA.Parser;
using GME.MGA;
using GME.Util;

namespace GME.MGA
{
    public static class MgaUtils
    {
        public static void ImportXMEForTest(string xmePath, out string connectionString)
        {
            string mgaPath = Path.Combine(Path.GetDirectoryName(xmePath), Path.GetFileNameWithoutExtension(xmePath) + "_test.mga");

            ImportXMEForTest(xmePath, mgaPath, out connectionString);
        }

        public static void ImportXMEForTest(string xmePath, string mgaPath, out string connectionString)
        {
            connectionString = "MGA=" + mgaPath;
            if (File.Exists(mgaPath))
            {
                // for faster debug cycles, skip importing .xme
                // if (xmePath.Contains("ToyDS.xme") && File.GetLastWriteTimeUtc(xmePath) < File.GetLastWriteTimeUtc(mgaPath))
                // {
                //     return;
                // }
                // delete the file if exists.
                // it could be a test to check if the importer has generated the mga file or not.
                File.Delete(mgaPath);
            }

            ImportXME(xmePath, mgaPath);
        }

        public static void ImportXME(string xmePath, string mgaPath, bool enableAutoAddons=false)
        {
            MgaParser parser = new MgaParser();
            string paradigm;
            string paradigmVersion;
            object paradigmGuid;
            string basename;
            string version;
            parser.GetXMLInfo(xmePath, out paradigm, out paradigmVersion, out paradigmGuid, out basename, out version);

            parser = new MgaParser();
            MgaProject project = new MgaProject();
            MgaResolver resolver = new MgaResolver();
            resolver.IsInteractive = false;
            dynamic dynParser = parser;
            dynParser.Resolver = resolver;
            project.Create("MGA=" + Path.GetFullPath(mgaPath), paradigm);
            if (enableAutoAddons)
            {
                project.EnableAutoAddOns(true);
            }
            try
            {
                parser.ParseProject(project, xmePath);
                project.Save();
            }
            finally
            {
                project.Close();
            }
        }
    }
}