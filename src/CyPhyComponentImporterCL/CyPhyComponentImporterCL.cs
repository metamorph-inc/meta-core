using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Linq;
using System.Xml;
using System.Xml.Linq;
using CyPhy2ComponentModel.Validation;
using GME.CSharp;
using GME;
using GME.MGA;
using GME.MGA.Core;
using ISIS.GME.Common;
using System.Windows.Forms;
using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyMLClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using avm;
using CyPhy2ComponentModel;

using System.Reflection;
using Microsoft.Win32;

namespace CyPhyComponentImporterCL {

    public class CyPhyComponentImporterCL {

        private static void WriteLine(Func<string, string, string> f, IMgaFCO a, IMgaFCO b) {
            //if (GMEConsole != null)
            //{
            //    GMEConsole.Out.WriteLine(f(GetLink(a, a.Name), GetLink(b, b.Name)));
            //}
            //else
            {
                Console.Out.WriteLine(  f( a.AbsPath, b.AbsPath )  );
            }
        }

        public static string Meta_Path
        {
            get
            {
                const string keyName = "Software\\META";

                RegistryKey metaKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry32)
                    .OpenSubKey(keyName, false);

                string metaPath = "C:\\Program Files (x86)\\META";
                if (metaKey != null)
                {
                    metaPath = (string)metaKey.GetValue("META_PATH", metaPath);
                }

                return metaPath;
            }
        }

        private static MgaProject GetProject( String filename ) {
            MgaProject result = null;

            if( filename != null && filename != "" ) {
                if( Path.GetExtension( filename ) == ".mga" ) {
                    result = new MgaProject();
                    if( System.IO.File.Exists( filename ) ) {
                        Console.Out.Write( "Opening {0} ... ", filename );
                        bool ro_mode;
                        result.Open( "MGA=" + Path.GetFullPath(filename), out ro_mode );
                    } else {
                        Console.Out.Write( "Creating {0} ... ", filename );
                        result.Create( "MGA=" + filename, "CyPhyML" );
                    }
                    Console.Out.WriteLine( "Done." );
                } else {
                    Console.Error.WriteLine( "{0} file must be an mga project.", filename );
                }
            } else {
                Console.Error.WriteLine( "Please specify an Mga project." );
            }

            return result;
        }

        private static void usage() {
            Console.Out.WriteLine( "Usage: <program> AVMFile [ -p <path-to-component-to-replace> ] CyPhyMLFile.mga" );
            Console.Out.WriteLine( "Usage: <program> -r <path> CyPhyMLFile.mga" );
        }

        [STAThread]
        public static int Main( String[] args ) {

            if (args.Length < 2 || args.Length > 4)
            {
                usage();
                return 1;
            }
            MgaProject mgaProject;
            List<String> lp_FilesToImport = new List<string>();
            string avmFilePath = "";
            string mgaProjectPath = "";
            string componentReplacementPath = "";

            bool rOptionUsed = false;
            bool pOptionUsed = false;

            for( int ix = 0 ; ix < args.Length ; ++ix ) {

                if( args[ ix ].ToLower() == "-r" ) {

                    if (pOptionUsed)
                    {
                        usage();
                        return 1;
                    }
                    rOptionUsed = true;

                    if (++ix >= args.Length)
                    {
                        usage();
                        return 1;
                    }

                    if( avmFilePath != null && avmFilePath != "" ) {
                        if (mgaProjectPath != null && mgaProjectPath != "")
                        {
                            usage();
                            return 1;
                        }
                        mgaProjectPath = avmFilePath;
                        avmFilePath = "";
                        lp_FilesToImport.Clear();
                    }

                    String sImportDirectory = args[ ix ];

                    String startingDirectory = Path.GetFullPath( sImportDirectory );
                    string[] xmlFiles = Directory.GetFiles( startingDirectory, "*.acm", SearchOption.AllDirectories );

                    foreach( String p_XMLFile in xmlFiles ) {
                        lp_FilesToImport.Add( Path.GetFullPath( p_XMLFile ) );
                    }

                } else if( args[ ix ].ToLower() == "-p" ) {

                    if (rOptionUsed)
                    {
                        usage();
                        return 1;
                    }
                    pOptionUsed = true;

                    if (++ix >= args.Length)
                    {
                        usage();
                        return 1;
                    }
                    componentReplacementPath = args[ ix ];

                } else if ( lp_FilesToImport.Count == 0 && avmFilePath == "" ) {

                    avmFilePath = args[ ix ];
                    try
                    {
                        lp_FilesToImport.Add(Path.GetFullPath(avmFilePath));
                    }
                    catch (System.ArgumentException ex)
                    {
                        Console.Out.WriteLine(ex.Message);
                        Console.Out.WriteLine(avmFilePath);
                        throw ex;
                    }

                } else {

                    if (mgaProjectPath != null && mgaProjectPath != "")
                    {
                        usage();
                        return 1;
                    }
                    mgaProjectPath = args[ ix ];
                }

            }


            mgaProject = GetProject( mgaProjectPath );
            try
            {
                bool bExceptionOccurred = false;
                if (mgaProject != null)
                {
                    MgaGateway mgaGateway = new MgaGateway(mgaProject);

                    mgaGateway.PerformInTransaction(delegate
                    {

                        string libroot = Path.GetDirectoryName(Path.GetFullPath(mgaProjectPath));

                        CyPhyML.RootFolder cyPhyMLRootFolder = ISIS.GME.Common.Utils.CreateObject<CyPhyMLClasses.RootFolder>(mgaProject.RootFolder as MgaObject);

                        #region Attach QUDT library if needed
                        IMgaFolder oldQudt = mgaProject.RootFolder.ChildFolders.Cast<IMgaFolder>().Where(x => x.LibraryName != "" && (x.Name.ToLower().Contains("qudt"))).FirstOrDefault();
                        string mgaQudtPath = Meta_Path + "\\meta\\CyPhyMLQudt.mga";

                        bool needAttach = false;
                        if (oldQudt == null)
                        {

                            needAttach = true;

                        }
                        else
                        {

                            long loldModTime;
                            DateTime oldModTime = long.TryParse(oldQudt.RegistryValue["modtime"], out loldModTime) ? DateTime.FromFileTimeUtc(loldModTime) : DateTime.MinValue;
                            needAttach = System.IO.File.GetLastWriteTimeUtc(mgaQudtPath).CompareTo(oldModTime) > 0;
                            if (!needAttach)
                            {
                                Console.Error.WriteLine("QUDT is up-to-date: embedded library modified " + oldModTime.ToString() + ", CyPhyMLQudt.mga modified " + System.IO.File.GetLastWriteTimeUtc(mgaQudtPath).ToString());
                            }
                        }

                        if (needAttach)
                        {

                            Console.Error.WriteLine("Attaching library " + mgaQudtPath);
                            ISIS.GME.Common.Interfaces.RootFolder newQudt = ISIS.GME.Common.Classes.RootFolder.GetRootFolder(mgaProject).AttachLibrary("MGA=" + mgaQudtPath);
                            DateTime modtime = System.IO.File.GetLastWriteTimeUtc(mgaQudtPath);
                            ((newQudt as ISIS.GME.Common.Classes.RootFolder).Impl as GME.MGA.IMgaFolder).RegistryValue["modtime"] =
                                    modtime.ToFileTimeUtc().ToString();

                            if (oldQudt != null)
                            {
                                ReferenceSwitcher.Switcher sw = new ReferenceSwitcher.Switcher(oldQudt, newQudt.Impl, null);
                                sw.UpdateSublibrary();
                                oldQudt.DestroyObject();
                            }
                            ((newQudt as ISIS.GME.Common.Classes.RootFolder).Impl as GME.MGA.IMgaFolder).LibraryName = "UnitLibrary QUDT";
                            Console.Error.WriteLine((oldQudt == null ? "Attached " : "Refreshed") + " Qudt library.");
                        }
                        #endregion

                        var importer = new CyPhyComponentImporter.CyPhyComponentImporterInterpreter();
                        importer.Initialize(cyPhyMLRootFolder.Impl.Project);
                        importer.ImportFiles(cyPhyMLRootFolder.Impl.Project, libroot, lp_FilesToImport.ToArray(), true);
                        bExceptionOccurred = importer.Errors.Count > 0;
                    }, abort: false);

                    mgaProject.Save();

                    if (bExceptionOccurred)
                    {
                        return -1;
                    }
                }
            }
            finally
            {
                mgaProject.Close(true);
            }
            return 0;
        }
    }
}
