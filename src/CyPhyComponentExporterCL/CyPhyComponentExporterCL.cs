using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Linq;
using DesignConsistencyChecker.DesignRule;
using GME.CSharp;
using GME;
using GME.MGA;
using GME.MGA.Core;
using System.Windows.Forms;
using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyMLClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using avm;
using CyPhy2ComponentModel;

using System.Threading.Tasks;
using System.Collections.Concurrent;

namespace CyPhyComponentExporterCL {

    public class CyPhyComponentExporterCL {

        private static MgaProject GetProject( String filename ) {
            MgaProject result = null;

            if( filename != null && filename != "" ) {
                if( Path.GetExtension( filename ) == ".mga" ) {
                    result = new MgaProject();
                    if( System.IO.File.Exists( filename ) ) {
                        Console.Out.Write( "Opening {0} ... ", filename );
                        bool ro_mode = true;
                        result.Open( "MGA=" + Path.GetFullPath(filename), out ro_mode );
                        Console.Out.WriteLine( "Done." );
                    } else {
                        Console.Error.WriteLine( "{0} file must be an existing mga project.", filename );
                    }
                } else {
                    Console.Error.WriteLine( "{0} file must be an mga project.", filename );
                }
            } else {
                Console.Error.WriteLine( "Please specify an Mga project." );
            }

            return result;
        }

        private static String Safeify( String s_in ) {
            String rtn = s_in;
            rtn = rtn.Replace( "\\", "_" );
            rtn = rtn.Replace( "/", "_" );
            return rtn;
        }

        [STAThread]
        public static int Main( String[] args ) {

            if( args.Length < 1 ) {
                Console.Out.WriteLine( "Usage: <program> CyPhyMLFile.mga [-f] [outputDirectory]" );
                Environment.Exit( 1 );
            }

            MgaProject mgaProject = null;

            string outputDirectory = ".";
            bool flat = false;

            for( int ix = 0; ix < args.Length; ++ix ) {
                if( args[ ix ] == "-f" ) {
                    flat = true;
                } else if ( mgaProject == null ) {
                    mgaProject = GetProject( args[ ix ] );
                } else {
                    outputDirectory = args[ ix ];
                }
            }

            if ( mgaProject == null ) {
                Console.Out.WriteLine( "Usage: <program> CyPhyMLFile.mga [-f] [outputDirectory]" );
                Environment.Exit( 1 );
            }

            MgaGateway mgaGateway = new MgaGateway( mgaProject );

            bool bExceptionOccurred = false;

            mgaGateway.PerformInTransaction( delegate {
                try {
                    CyPhyML.RootFolder cyPhyMLRootFolder = ISIS.GME.Common.Utils.CreateObject<CyPhyMLClasses.RootFolder>( mgaProject.RootFolder );
                    HashSet<CyPhyML.Component> cyPhyMLComponentSet = CyPhy2ComponentModel.ComponentLister.getCyPhyMLComponentSet( cyPhyMLRootFolder );

                    var ruleChecker = new DesignConsistencyChecker.Framework.Checker(null, mgaProject);
                    ruleChecker.RegisterRuleDll("DesignConsistencyChecker.dll");
                    var uniqueNameRule = ruleChecker.GetRegisteredRules.FirstOrDefault(x => x.Name == "UniquePPMNames");

                    int i_Counter = 1;
                    foreach (var cyPhyMLComponent  in cyPhyMLComponentSet)
                        {
                            try
                            {
                                if (uniqueNameRule != null)
                                {
                                    var feedBacks = uniqueNameRule.Check((MgaFCO)cyPhyMLComponent.Impl).ToList();
                                    if (feedBacks.Any(x => x.FeedbackType == FeedbackTypes.Error))
                                    {
                                        foreach (var fb in feedBacks.Where(x => x.FeedbackType == FeedbackTypes.Error))
                                        {
                                            Console.Error.WriteLine("Rule {0} failed: {1}", uniqueNameRule.Name, fb.Message);
                                        }
                                        continue;
                                    }
                                }

                                //foreach (CyPhyML.Component cyPhyMLComponent in cyPhyMLComponentList) {
                                avm.Component avmComponent = CyPhy2ComponentModel.Convert.CyPhyML2AVMComponent(cyPhyMLComponent);
                                string componentPath = null;

                                if (flat) {
                                    componentPath = outputDirectory;
                                } else {
                                    componentPath = cyPhyMLComponent.Path;
                                    componentPath = outputDirectory + "/" + componentPath.Substring(componentPath.IndexOf("/"));
                                }

                                Directory.CreateDirectory(componentPath);

                                if (!flat) {
                                    Directory.CreateDirectory(componentPath + "/images");
                                    //Directory.CreateDirectory( componentPath + "/CyPhyML" );
                                    Directory.CreateDirectory(componentPath + "/doc");
                                    Directory.CreateDirectory(componentPath + "/CAD");
                                }

                                String s_outFilePath = String.Format("{0}/{1}.component.acm", componentPath, Safeify(cyPhyMLComponent.Name));
                                using (FileStream stream = new FileStream(s_outFilePath, FileMode.Create))
                                {
                                    OpenMETA.Interchange.AvmXmlSerializer.Serialize(avmComponent, stream);
                                    stream.Close();
                                }
                                    
                                Console.Out.WriteLine(string.Format("({0}/{1}) {2}", i_Counter++, cyPhyMLComponentSet.Count, Safeify(cyPhyMLComponent.Name)));
                                //}
                            } catch( Exception ex ) {
                                Console.Error.WriteLine( "Exception: {0}", ex.Message.ToString() );
                                Console.Error.WriteLine( "Stack: {0}", ex.StackTrace.ToString() );
                                bExceptionOccurred = true;
                            }
                        }
                    
                } catch( Exception ex ) {
                    Console.Error.WriteLine( "Exception: {0}", ex.Message.ToString() );
                    Console.Error.WriteLine("Stack: {0}", ex.StackTrace.ToString());
                    bExceptionOccurred = true;
                }
            }, abort: false );

            mgaProject.Close();

            if (bExceptionOccurred)
                return -1;
            return 0;
        }

    }
}
