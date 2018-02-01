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
                    throw;
                }
                if (Directory.GetFileSystemEntries(path).Length > 0)
                {
                    throw;
                }
                return;
            }
            Directory.CreateDirectory(path);
        }
    }
}
