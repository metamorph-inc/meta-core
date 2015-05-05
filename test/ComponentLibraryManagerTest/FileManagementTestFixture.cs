using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Xunit;
using GME.MGA;
using GME.CSharp;

namespace ComponentLibraryManagerTest
{
    public class FileManagementTestFixture : IDisposable
    {
        public FileManagementTestFixture()
        {
            // Delete any MGA and import from scratch
            File.Delete(FileManagementTest.path_MGA);
            GME.MGA.MgaUtils.ImportXME(FileManagementTest.path_XME, FileManagementTest.path_MGA);
            Assert.True(File.Exists(FileManagementTest.path_MGA));

            proj = GetProject(FileManagementTest.path_MGA);
            Assert.True(proj != null, "Could not load MGA project.");

            File.Copy(FileManagementTest.path_Manifest_Original, FileManagementTest.path_Manifest, true);
        }

        public IMgaProject proj { get; private set; }

        public static MgaProject GetProject(String filename)
        {
            MgaProject result = null;

            if (filename != null && filename != "")
            {
                if (Path.GetExtension(filename) == ".mga")
                {
                    result = new MgaProject();
                    if (System.IO.File.Exists(filename))
                    {
                        Console.Out.Write("Opening {0} ... ", filename);
                        bool ro_mode;
                        result.Open("MGA=" + Path.GetFullPath(filename), out ro_mode);
                    }
                    else
                    {
                        Console.Out.Write("Creating {0} ... ", filename);
                        result.Create("MGA=" + filename, "CyPhyML");
                    }
                    Console.Out.WriteLine("Done.");
                }
                else
                {
                    Console.Error.WriteLine("{0} file must be an mga project.", filename);
                }
            }
            else
            {
                Console.Error.WriteLine("Please specify an Mga project.");
            }

            return result;
        }

        public void Dispose()
        {
            proj.Save();
            proj.Close();
        }
    }
}
