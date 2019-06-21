using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using CommandLine;
using GME.CSharp;
using GME.MGA;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhyDesignExporterCL
{
    public class Program
    {
        private static String Safeify(String test)
        {
            return string.IsNullOrEmpty(test) ? string.Empty : test.Replace("\\", "_").Replace("/", "_");
        }

        [STAThread]
        public static int Main(string[] args)
        {
            var options = new Options();
            if (!Parser.Default.ParseArguments(args, options)) return -1;

            var project = GetProject(options.MgaFile);

            if (project == null)
            {
                return 1;
            }

            try
            {
                var mgaGateway = new MgaGateway(project);

                var designList = new List<CyPhy.DesignEntity>();
                var designName = Safeify(options.DesignName);


                bool bExceptionOccurred = false;
                mgaGateway.PerformInTransaction(delegate
                {
                    try
                    {
                        #region Collect DesignEntities

                        MgaFilter filter = project.CreateFilter();
                        filter.Kind = "ComponentAssembly";
                        foreach (var item in project.AllFCOs(filter).Cast<MgaFCO>().Where(x => x.ParentFolder != null))
                        {
                            designList.Add(CyPhyClasses.ComponentAssembly.Cast(item));
                        }

                        filter = project.CreateFilter();
                        filter.Kind = "DesignContainer";
                        foreach (var item in project.AllFCOs(filter).Cast<MgaFCO>().Where(x => x.ParentFolder != null))
                        {
                            designList.Add(CyPhyClasses.DesignContainer.Cast(item));
                        }

                        #endregion

                        #region Process DesignEntities

                        foreach (CyPhy.DesignEntity de in designList)
                        {
                            var currentDesignName = Safeify(de.Name);

                            if (!string.IsNullOrEmpty(options.DesignName) && currentDesignName != designName) continue;

                            var dm = CyPhy2DesignInterchange.CyPhy2DesignInterchange.Convert(de);
                            var outFilePath = String.Format("{0}\\{1}.adm", new FileInfo(options.MgaFile).DirectoryName, currentDesignName);
                            OpenMETA.Interchange.AvmXmlSerializer.SaveToFile(outFilePath, dm);
                        }

                        #endregion
                    }
                    catch (Exception ex)
                    {
                        Console.Error.WriteLine("Exception: {0}", ex.Message.ToString());
                        Console.Error.WriteLine("Stack: {0}", ex.StackTrace.ToString());
                        bExceptionOccurred = true;
                    }
                }, abort: false);

                if (bExceptionOccurred)
                    return -1;

                return 0;
            }
            finally
            {
                project.Close(true);
            }
        }

        private static MgaProject GetProject(String filename)
        {
            MgaProject result = null;

            if (!string.IsNullOrEmpty(filename))
            {
                if (Path.GetExtension(filename) == ".mga")
                {
                    result = new MgaProject();
                    if (File.Exists(filename))
                    {
                        Console.Out.Write("Opening {0} ... ", filename);
                        var roMode = true;
                        result.Open("MGA=" + filename, out roMode);
                        Console.Out.WriteLine("Done.");
                    }
                    else
                    {
                        Console.Error.WriteLine("{0} file must be an existing mga project.", filename);
                    }
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
    }
}
