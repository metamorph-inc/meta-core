using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

using GME.CSharp;
using GME;
using GME.MGA;
using GME.MGA.Core;
using Cyber = ISIS.GME.Dsml.CyberComposition.Interfaces;
using CyberClasses = ISIS.GME.Dsml.CyberComposition.Classes;
using CyberModelUtil;
using Cyber2AVM;

namespace CyberComponentExporterCL
{
    public class CyberComponentExporterCL
    {
        /* Copied/Adapted from CyPhyCompomponentExporterCL Proj */

        [STAThread]
        public static int Main(String[] args)
        {

            if (args.Length < 1)
            {
                Console.Out.WriteLine("Usage: <program> CyberFile.mga [-f] [outputDirectory]");
                Environment.Exit(1);
            }

            MgaProject mgaProject = null;

            string outputDirectory = ".";
            bool flat = false;

            String mgaFilePath = null;
            for (int ix = 0; ix < args.Length; ++ix)
            {
                if (args[ix] == "-f")
                {
                    flat = true;
                }
                else if (mgaProject == null)
                {
                    mgaProject = CyberModelUtil.MgaUtil.GetProject(args[ix]);
                    mgaFilePath = args[ix];
                }
                else
                {
                    outputDirectory = args[ix];
                }
            }

            if (mgaProject == null)
            {
                Console.Out.WriteLine("Usage: <program> CyberFile.mga [-f] [outputDirectory]");
                Environment.Exit(1);
            }

            MgaGateway mgaGateway = new MgaGateway(mgaProject);
            mgaProject.CreateTerritoryWithoutSink(out mgaGateway.territory);

            bool bExceptionOccurred = false;

            mgaGateway.PerformInTransaction(delegate
            {
                try
                {
                    Cyber.RootFolder cyberRootFolder = ISIS.GME.Common.Utils.CreateObject<CyberClasses.RootFolder>(mgaProject.RootFolder);
                    HashSet<Cyber.ModelicaComponent> cyberComponentSet = ComponentLister.getCyberComponentSet(cyberRootFolder);

                    //ParallelOptions options = new ParallelOptions();
                    //options.MaxDegreeOfParallelism = 4;

                    //var ruleChecker = new DesignConsistencyChecker.Framework.Checker(null, mgaProject);
                    //ruleChecker.RegisterRuleDll("DesignConsistencyChecker.dll");
                    //var uniqueNameRule = ruleChecker.GetRegisteredRules.FirstOrDefault(x => x.Name == "UniquePPMNames");

                    int i_Counter = 1;
                    foreach (var cyberComponent in cyberComponentSet)
                    {
                        try
                        {
                            //if (uniqueNameRule != null)
                            //{
                            //    var feedBacks = uniqueNameRule.Check((MgaFCO)cyberComponent.Impl).ToList();
                            //    if (feedBacks.Any(x => x.FeedbackType == FeedbackTypes.Error))
                            //    {
                            //        foreach (var fb in feedBacks.Where(x => x.FeedbackType == FeedbackTypes.Error))
                            //        {
                            //            Console.Error.WriteLine("Rule {0} failed: {1}", uniqueNameRule.Name, fb.Message);
                            //        }
                            //        continue;
                            //    }
                            //}

                            //foreach (Cyber.ModelicaComponent cyberComponent in cyberComponentList) {
                            AVMComponentBuilder avmComponentBuilder = new AVMComponentBuilder();
                            avmComponentBuilder.createAVMCyberModel(cyberComponent, mgaFilePath);
                            avm.Component avmComponent = avmComponentBuilder.getAVMComponent();
                            string componentPath = null;

                            if (flat)
                            {
                                componentPath = outputDirectory;
                            }
                            else
                            {
                                componentPath = cyberComponent.Path;
                                componentPath = outputDirectory + "/" + componentPath.Substring(componentPath.IndexOf("/"));
                            }

                            Directory.CreateDirectory(componentPath);

                            // delete below
                            //if (!flat)
                            //{
                            //    Directory.CreateDirectory(componentPath + "/images");
                            //    //Directory.CreateDirectory( componentPath + "/Cyber" );
                            //    Directory.CreateDirectory(componentPath + "/doc");
                            //    Directory.CreateDirectory(componentPath + "/CAD");
                            //}

                            String s_outFilePath = String.Format("{0}/{1}.component.acm", componentPath, META2AVM_Util.UtilFuncs.Safeify(cyberComponent.Name));
                            META2AVM_Util.UtilFuncs.SerializeAvmComponent(avmComponent, s_outFilePath);
                            //StreamWriter streamWriter = new StreamWriter(s_outFilePath);

                            //System.Xml.Serialization.XmlSerializer serializer = new System.Xml.Serialization.XmlSerializer(typeof(avm.Component), META2AVM_Util.UtilFuncs.getAVMClasses());

                            //serializer.Serialize(streamWriter, avmComponent);
                            //streamWriter.Close();

                            Console.Out.WriteLine(string.Format("({0}/{1}) {2}", i_Counter++, cyberComponentSet.Count, META2AVM_Util.UtilFuncs.Safeify(cyberComponent.Name)));
                            //}
                        }
                        catch (Exception ex)
                        {
                            Console.Error.WriteLine("Exception: {0}", ex.Message.ToString());
                            Console.Error.WriteLine("Stack: {0}", ex.StackTrace.ToString());
                            bExceptionOccurred = true;
                        }
                    }

                }
                catch (Exception ex)
                {
                    Console.Error.WriteLine("Exception: {0}", ex.Message.ToString());
                    Console.Error.WriteLine("Stack: {0}", ex.StackTrace.ToString());
                    bExceptionOccurred = true;
                }
            });

            mgaProject.Close();

            if (bExceptionOccurred)
                return -1;
            return 0;
        }
        /* copy end */
    }
}
