using System.IO;

namespace CyPhyGUIs
{
    public class CyPhyDirectory
    {
        public static void EnsureEmptyDirectory(string path)
        {
            try
            {
                Directory.Delete(path, true);
            }
            catch (DirectoryNotFoundException)
            {
            }
            catch (IOException e)
            {
                if (e.Message.Contains("The directory is not empty") == false)
                {
                    throw new IOException("Could not delete " + path, e);
                }
                if (Directory.GetFileSystemEntries(path).Length > 0)
                {
                    throw new IOException("Could not delete " + path, e);
                }
                return;
            }
            Directory.CreateDirectory(path);
        }
    }
}
