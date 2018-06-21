using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;

namespace CyPhy2CAD_CSharp
{
    public class UtilityHelpers
    {

        public static string CleanString2(string original, int size = 31, string specialchars = "")
        {
	        StringBuilder sbuilder = new StringBuilder();
	        string Special_Char_String = ",.=()!@#$%&~`'+ ^*[]{}/?:;<>|";		// 11-28-2012: removed "-" from list

	        if (specialchars != "")
		        Special_Char_String += specialchars;
                                    
	        foreach (char c in original)
	        {
		        if (!Special_Char_String.Contains(c))
			        sbuilder.Append(c);
		        else
		        {
			        if (c == '<')
				        sbuilder.Append("<");
			        else if (c == '>')
				        sbuilder.Append(">");
			        else
				        sbuilder.Append("_");
		        }
	        }

            if (sbuilder.Length > size)
                sbuilder.Remove(size-1, sbuilder.Length - size);
            return sbuilder.ToString();
        }

        private static int UdmID = 0;
        public static string MakeUdmID()
        {
            UdmID++;
            return "id" + UdmID.ToString();
        }

        public static void CopyFiles(string srcDir,
                                     string dstDir)
        {
            if (Directory.Exists(srcDir))
            {
                if (!Directory.Exists(dstDir))
                    Directory.CreateDirectory(dstDir);

                DirectoryInfo dir = new DirectoryInfo(srcDir);
                FileInfo[] files = dir.GetFiles();
                foreach (FileInfo file in files)
                {
                    string temppath = Path.Combine(dstDir, file.Name);
                    file.CopyTo(temppath, true);
                }
            }
        }
    }
}
