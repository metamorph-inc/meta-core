using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using GME.CSharp;
using GME;
using GME.MGA;
using GME.MGA.Core;
using ISIS.GME.Common;

using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyMLClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhyMLMerge {
    class Program {

        private static MgaProject GetProject(String filename) {
            MgaProject result = null;

            if (filename != null && filename != "") {
                if (Path.GetExtension(filename) == ".mga") {
                    result = new MgaProject();
                    if (System.IO.File.Exists(filename)) {
                        Console.Out.Write("Opening {0} ... ", filename);
                        bool ro_mode;
                        result.Open("MGA=" + filename, out ro_mode);
                    } else {
                        Console.Out.Write("Creating {0} ... ", filename);
                        result.Create("MGA=" + filename, "CyPhyML");
                    }
                    Console.Out.WriteLine("Done.");
                } else {
                    Console.Error.WriteLine("{0} file must be an mga project.", filename);
                }
            } else {
                Console.Error.WriteLine("Please specify an Mga project.");
            }

            return result;
        }

        static void usage() {
            Console.Error.WriteLine("Usage: CyPhyMLMerge FILE-TO-MERGE-INTO.mga < FILE-TO-MERGE.mga/PATH/TO/ITEM/TO/MERGE ... >");
            Environment.Exit( 1 );
        }

        static int Main(string[] args) {

            if (args.Length < 2) usage();

            MgaProject mainMgaProject = GetProject( args[0] );

            if( mainMgaProject == null ) {
                Environment.Exit( 2 );
            }

            int retval = 0;
            MgaGateway mgaGateway = new MgaGateway(mainMgaProject);

            mgaGateway.PerformInTransaction(delegate {

                for (int ix = 1; ix < args.Length; ++ix) {
                    string[] mergeInfo = args[ix].Split(new string[] { ".mga/" }, 2, StringSplitOptions.None);

                    string filename = mergeInfo[0] + ".mga";
                    string path = "/" + mergeInfo[1];

                    MgaFCO currentObject = mainMgaProject.get_ObjectByPath(path) as MgaFCO;
                    if (currentObject == null) {
                        Console.Error.WriteLine("Error: could not find object of path \"" + path + "\" in model of file \"" + args[0] + "\", cannot merge file \"" + filename + "\"");
                        retval |= (int)SubTreeMerge.SubTreeMerge.Errors.PathError;
                        continue;
                    }

                    SubTreeMerge.SubTreeMerge subTreeMerge = new SubTreeMerge.SubTreeMerge();
                    subTreeMerge.gmeConsole = new SubTreeMerge.FlexConsole( SubTreeMerge.FlexConsole.ConsoleType.CONSOLE );

                    subTreeMerge.merge(currentObject, filename);
                    retval = (int)subTreeMerge.exitStatus;
                }

            }, abort: false);

            mainMgaProject.Save();

            return retval;
        }
    }
}
