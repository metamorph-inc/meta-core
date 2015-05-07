using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Newtonsoft.Json;
using GME.CSharp;
using GME;
using GME.MGA;
using GME.MGA.Core;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;


namespace CyPhy2CAD_CSharp
{
    public partial class CyPhy2CAD_CSharpInterpreter
    {
        private void ProcessCAD(MgaFCO currentobj)
        {
            string outputdir = this.mainParameters.OutputDirectory;
            if (!Directory.Exists(outputdir))
                Directory.CreateDirectory(outputdir);

            string debuglogdir = Path.Combine(outputdir, "log");
            if (!Directory.Exists(debuglogdir))
                Directory.CreateDirectory(debuglogdir);

            string projectdir = this.mainParameters.ProjectDirectory;

            CyPhy2CADSettings cadSetting = (CyPhy2CADSettings)this.mainParameters.config;
            string cadauxdir = cadSetting.AuxiliaryDirectory;

            CyPhy.ComponentAssembly curassembly;

            
            string curObjMetaBase = currentobj.MetaBase.Name;
            if (curObjMetaBase == "TestBench" || curObjMetaBase == "BallisticTestBench" || curObjMetaBase == "CFDTestBench" || curObjMetaBase == "BlastTestBench" || curObjMetaBase == "KinematicTestBench")
            {
                CyPhy.TestBenchType tb = CyPhyClasses.TestBenchType.Cast(currentobj);
                if (result != null && curObjMetaBase == "BlastTestBench")
                {
                    result.Labels = JobManager.Job.DefaultLabels + "&&SwRI_Blast";
                }
                if (result != null && curObjMetaBase == "KinematicTestBench")
                {
                    result.Labels = JobManager.Job.DefaultLabels + "&&Adams";
                }

                var catlsut = tb.Children.ComponentAssemblyCollection.FirstOrDefault();     // should be an instance b/c elaborate was called earlier
                if (catlsut == null)
                {
                    result.Success = false;
                    throw new Exception("There is no elaborated system under test component assembly in the model!");
                }

                curassembly = catlsut;

                if (curassembly == null)
                {
                    result.Success = false;
                    throw new Exception("No Valid Component Assembly.");
                }

                // META-1971: ADM + ACM file export for blast + ballistics
                if (curObjMetaBase == "BallisticTestBench" || curObjMetaBase == "BlastTestBench")
                {
                    // ADM: Export DDP 2.x version (XML)
                    {
                        var design = CyPhy2DesignInterchange.CyPhy2DesignInterchange.Convert(catlsut);
                        string TestBenchName = tb.Name;

                        string admDir = Path.Combine(cadSetting.OutputDirectory, "DesignADM");
                        if (!Directory.Exists(admDir))
                            Directory.CreateDirectory(admDir);
                        XSD2CSharp.AvmXmlSerializer.SaveToFile(Path.Combine(admDir, TestBenchName + ".adm"), design);
                    }
                }
                // end META-1971

                // META-2987 
                GenerateCADParameterMapping(tb,
                                            outputdir);

                CADFlatDataCreator datacreator = new CADFlatDataCreator(debuglogdir, this.mainParameters.ProjectDirectory);
                datacreator.Traceability = this.result.Traceability;
                datacreator.CreateFlatData(curassembly);
                DataRep.CADContainer cadcontainer = datacreator.CreateCADDataContainer(curassembly.Attributes.ConfigurationUniqueID,
                                                                                       UtilityHelpers.CleanString2(curassembly.Name));

                using (StreamWriter writer = new StreamWriter(Path.Combine(outputdir, "log", "CyPhy2CAD_Graphviz_Representation.gv")))
                {
                    writer.WriteLine(cadcontainer.ToGraphviz(true));
                }

                if (!cadcontainer.IsAnalyzable() && curObjMetaBase != "TestBench")
                {                    
                    result.Success = false;
                    //Logger.Instance.AddLogMessage("Test Bench can not proceed because the test bench contains islands and/or orphans! Please see graphviz file in /log directory for details.", Severity.Error);
                    throw new META.InterpreterException("Test Bench can not proceed because the test bench contains islands and/or orphans, see graphviz file in /log/CyPhy2CAD_Graphviz_Representation.gv for details. Please remove islands/orphans and try again.");
                }

                TestBenchModel.TestBenchBase testBenchRep = TestBenchModelFactory.CreateTestBenchModel(curObjMetaBase,
                                                                                                     cadSetting,
                                                                                                     outputdir,
                                                                                                     projectdir,
                                                                                                     Automation);

                testBenchRep.cadDataContainer = cadcontainer;
                testBenchRep.CopySTL = this.CopySTL;

                if (curObjMetaBase != "ComponentAssembly")
                    testBenchRep.TraverseTestBench(tb);

                testBenchRep.CollectDirectories();
                result.Success = testBenchRep.GenerateOutputFiles();   

            }
            else if (curObjMetaBase == "CADTestBench")
            {
                CyPhy.CADTestBench tb = CyPhyClasses.CADTestBench.Cast(currentobj);
                // META-1701: label for Abaqus
                if (result != null)
                {
                    Dictionary<CyPhyClasses.CADTestBench.AttributesClass.SolverType_enum, string> labelMap = new Dictionary<CyPhyClasses.CADTestBench.AttributesClass.SolverType_enum,string>()
                    {
                        { CyPhyClasses.CADTestBench.AttributesClass.SolverType_enum.ABAQUS_Deck_Based, "Abaqus"},
                        { CyPhyClasses.CADTestBench.AttributesClass.SolverType_enum.ABAQUS_Model_Based, "Abaqus"},
                        { CyPhyClasses.CADTestBench.AttributesClass.SolverType_enum.NASTRAN, "Nastran"},
                    };
                    string label = "Abaqus";
                    labelMap.TryGetValue(tb.Attributes.SolverType, out label);
                    result.Labels += "&&" + label;
                }
                var toplevelSUT = tb.Children.ComponentAssemblyCollection.FirstOrDefault();     // should be an instance b/c elaborate was called earlier
                if (toplevelSUT == null)
                {
                    result.Success = false;
                    throw new Exception("There is no elaborated system under test component assembly in the model!");
                }

                // META-2987
                GenerateCADParameterMapping(tb,
                                            outputdir);

                CADFlatDataCreator datacreator = new CADFlatDataCreator(debuglogdir, this.mainParameters.ProjectDirectory);
                datacreator.Traceability = this.result.Traceability;
                datacreator.CreateFlatData(tb,
                                           toplevelSUT.Path);
                DataRep.CADContainer cadcontainer = datacreator.CreateCADDataContainer(tb.Guid.ToString(),
                                                                                       UtilityHelpers.CleanString2(tb.Name));

                using (StreamWriter writer = new StreamWriter(Path.Combine(outputdir, "log", "CyPhy2CAD_Graphviz_Representation.gv")))
                {
                    writer.WriteLine(cadcontainer.ToGraphviz(true));
                }
                result.Success = false;

                if (!cadcontainer.IsAnalyzable())
                {
                    //Logger.Instance.AddLogMessage("FEA Test Bench can not proceed because the test bench contains islands and/or orphans! Please see graphviz file in /log directory for details.", Severity.Error);
                    throw new META.InterpreterException("FEA Test Bench can not proceed because the test bench contains islands and/or orphans, see graphviz file in /log/CyPhy2CAD_Graphviz_Representation.gv for details. Please remove islands/orphans and try again.");
                }

                TestBenchModel.TestBenchBase testBenchRep = TestBenchModelFactory.CreateTestBenchModel(curObjMetaBase,
                                                                                                       cadSetting,
                                                                                                       outputdir,
                                                                                                       projectdir,
                                                                                                       Automation);
                testBenchRep.cadDataContainer = cadcontainer;
                testBenchRep.CopySTL = this.CopySTL;
                testBenchRep.TraverseTestBench(tb);
                testBenchRep.CollectDirectories();
                result.Success = testBenchRep.GenerateOutputFiles();
            }
            else if (curObjMetaBase == "ComponentAssembly")
            {
                curassembly = CyPhyClasses.ComponentAssembly.Cast(currentobj);
                CADFlatDataCreator datacreator = new CADFlatDataCreator(debuglogdir, this.mainParameters.ProjectDirectory);
                datacreator.Traceability = this.result.Traceability;
                datacreator.CreateFlatData(curassembly);
                DataRep.CADContainer cadcontainer = datacreator.CreateCADDataContainer(curassembly.Guid.ToString(),
                                                                                       UtilityHelpers.CleanString2(curassembly.Name));

                TestBenchModel.TestBenchBase testbenchbase = new TestBenchModel.TestBenchBase(cadSetting,
                                                                                              outputdir,
                                                                                              projectdir);
                testbenchbase.cadDataContainer = cadcontainer;
                testbenchbase.CopySTL = this.CopySTL;
                testbenchbase.CollectDirectories();
            }
            else
            {
                result.Success = false;
                throw new NotImplementedException(String.Format("Running Cyphy2CAD on {0} is not implemented.", curObjMetaBase));
            }  

        }

        // META-2987
        private void GenerateCADParameterMapping(CyPhy.TestBenchType testBench,
                                                 string outputDir)
        {
            List<CyPhy2CAD_CSharp.TestBenchModel.TBCadParameterMapping> cadParamList = new List<TestBenchModel.TBCadParameterMapping>();
            if (testBench.Children.ParameterCollection.Any())
            {
                foreach (var parameter in testBench.Children.ParameterCollection)
                {
                    List<CyPhy.CADParameter> vftFound = new List<CyPhy.CADParameter>();
                    List<CyPhy.ValueFlowTarget> vftVisited = new List<CyPhy.ValueFlowTarget>();
 
                    FindValueFlowTargets(parameter, 
                                         vftFound,
                                         vftVisited);

                    foreach (var cadParam in vftFound)
                    {
                        CyPhy2CAD_CSharp.TestBenchModel.TBCadParameterMapping cadParameterMapping = new TestBenchModel.TBCadParameterMapping(cadParam, parameter.Name);
                        if (!String.IsNullOrEmpty(cadParameterMapping.ComponentInstanceGUID))
                            cadParamList.Add(cadParameterMapping);
                    }
                }
            }

            if (cadParamList.Any())
            {
                string mappingFileContent = Newtonsoft.Json.JsonConvert.SerializeObject(cadParamList);
                using (StreamWriter writer = new StreamWriter(Path.Combine(outputDir, "CADParamTestBenchMapping.json")))
                {
                    writer.WriteLine(mappingFileContent);
                }
            }            

            // generate testbench_manifest.json if it does not exist
            
            if (true/*!File.Exists(Path.Combine(outputDir, "testbench_manifest.json"))*/)
            {
                AVM.DDP.MetaTBManifest tbManifest = new AVM.DDP.MetaTBManifest();
                tbManifest.MakeManifest(testBench,
                                        outputDir);
                tbManifest.Serialize(outputDir);
            }
            
        }

        private void FindValueFlowTargets(CyPhy.ValueFlowTarget start, 
                                          List<CyPhy.CADParameter> vftFound,
                                          List<CyPhy.ValueFlowTarget> vftVisited)
        {
            IEnumerable<CyPhy.ValueFlowTarget> ends;
            vftVisited.Add(start);

            ends = start
                    .DstConnections
                    .ValueFlowCollection
                    .Select(x => x.DstEnds.ValueFlowTarget)
                  .Concat(
                    start
                    .SrcConnections
                    .ValueFlowCollection
                    .Select(x => x.SrcEnds.ValueFlowTarget))
                  .Where(x => vftVisited.Any(y => y.ID == x.ID) == false);

            foreach (var vftEnd in ends)
            {
                if (vftEnd.DstConnections.CADParameterPortMapCollection.Any())
                {
                    vftFound.AddRange(vftEnd
                                      .DstConnections
                                      .CADParameterPortMapCollection
                                      .Select(x => x.DstEnds.CADParameter)
                                      .Where(x => vftFound.Any(y => y.ID == x.ID) == false)); 
                    
                }

                FindValueFlowTargets(vftEnd,
                                     vftFound,
                                     vftVisited);
            }


        }
   }

    public static class TestBenchModelFactory
    {
        public static TestBenchModel.TestBenchBase CreateTestBenchModel(string type,
                                                                        CyPhy2CADSettings cadSetting,
                                                                        string outputdir,
                                                                        string projectdir,
                                                                        bool Automation)
        {
            TestBenchModel.TestBenchBase testBenchModel = null;

            if (type == "TestBench")
            {
                testBenchModel = new TestBenchModel.TestBench(cadSetting,
                                                              outputdir,
                                                              projectdir,
                                                              Automation);
                return testBenchModel;
            }
            
            if (type == "BallisticTestBench")
            {
                testBenchModel = new TestBenchModel.BallisticTestBench(cadSetting,
                                                              outputdir,
                                                              projectdir,
                                                              Automation);
                return testBenchModel;
            }
            
            if (type == "CFDTestBench")
            {
                testBenchModel = new TestBenchModel.CFDTestBenchData(cadSetting,
                                                              outputdir,
                                                              projectdir,
                                                              Automation);
                return testBenchModel;
            }
            
            if (type == "BlastTestBench")
            {
                testBenchModel = new TestBenchModel.BlastTestBench(cadSetting,
                                                              outputdir,
                                                              projectdir,
                                                              Automation);
                return testBenchModel;
            }
            if (type == "CADTestBench")
            {
                testBenchModel = new TestBenchModel.FEATestBench(cadSetting,
                                                             outputdir,
                                                             projectdir,
                                                             Automation);
                return testBenchModel;
            }

            
            if (type == "KinematicTestBench")
            {
                testBenchModel = new TestBenchModel.KinematicTestBench(cadSetting,
                                                             outputdir,
                                                             projectdir,
                                                             Automation);
                return testBenchModel;
            }
            

            return testBenchModel;
        }
    }

}
