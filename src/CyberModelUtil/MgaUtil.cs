using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using GME;
using GME.MGA;
using GME.MGA.Core;
namespace CyberModelUtil
{
    public class MgaUtil
    {
        /* Copied/Adapted from CyPhyCompomponentExporterCL Proj */
        public static MgaProject GetProject(String mgaFilename)
        {
            MgaProject result = null;

            if (mgaFilename != null && mgaFilename != "")
            {
                if (Path.GetExtension(mgaFilename) == ".mga")
                {
                    result = new MgaProject();
                    if (System.IO.File.Exists(mgaFilename))
                    {
                        Console.Out.Write("Opening {0} ... ", mgaFilename);
                        bool ro_mode = true;
                        result.Open("MGA=" + Path.GetFullPath(mgaFilename), out ro_mode);
                        Console.Out.WriteLine("Done.");
                    }
                    else
                    {
                        Console.Error.WriteLine("{0} file must be an existing mga project.", mgaFilename);
                    }
                }
                else
                {
                    Console.Error.WriteLine("{0} file must be an mga project.", mgaFilename);
                }
            }
            else
            {
                Console.Error.WriteLine("Please specify an Mga project.");
            }

            return result;
        }
        /* copy end */
    }
}
