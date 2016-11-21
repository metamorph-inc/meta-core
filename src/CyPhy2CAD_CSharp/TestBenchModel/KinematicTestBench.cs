using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using GME.MGA;
using GME.CSharp;
using GME;
using GME.MGA.Meta;
using META;

namespace CyPhy2CAD_CSharp.TestBenchModel
{
    public class KinematicTestBench : TestBenchBase
    {
        public string Name;
        public double SimulationTime; // How long the simnulation should be running (secs)
        public int SimulationStep; // How many steps the simulation should take (the more the finer will the simulation be)
        public KinematicGravity Gravity;
        List<Multibody.ModelLoadsMotion> MotionList;
        List<Multibody.ModelContact> ContactList;
        public string TerrainFileName; // Filename of terrain CAD file
        public string GroundCyphyID; // Cyphy ID of the ground component
        List<Multibody.ModelScript> Scripts;

        public KinematicTestBench (CyPhy2CADSettings cadSetting,
                                  string outputdir,
                                  string projectdir,
                                  bool auto = false) :
                                  base(cadSetting, outputdir, projectdir, auto)
        {
            MotionList = new List<Multibody.ModelLoadsMotion>();
        }

        private CyPhy.KinematicJoint GetJoint(ISIS.GME.Dsml.CyPhyML.Interfaces.Connector conn)
        {
            return conn.Children.KinematicJointCollection.FirstOrDefault();
        }

        public override void TraverseTestBench(CyPhy.TestBenchType testBenchBase)
        {
            CyPhy.KinematicTestBench testBench = testBenchBase as CyPhy.KinematicTestBench;
            if (testBench == null)
                testBench = CyPhyClasses.KinematicTestBench.Cast(testBenchBase.Impl);

            base.TraverseTestBench(testBenchBase);

            //STLDataExchangeFormats.Add("Parasolid");

            Name = testBench.Name;
            SimulationStep = testBench.Attributes.SimulationResolution;
            SimulationTime = testBench.Attributes.Duration;

            if (testBench.Children.GravityLoadCollection.Any())
            {
                CyPhy.GravityLoad load = testBench.Children.GravityLoadCollection.First();
                double x = 0;
                double y = 0;
                double z = 0;

                GetGravityLoadParam(load, "X", out x);
                GetGravityLoadParam(load, "Y", out y);
                GetGravityLoadParam(load, "Z", out z);
                Gravity = new KinematicGravity(x, y, z, load.Attributes.Active);

            }

            var terrain = testBench.Children.ComponentRefCollection.Where(c => ((MgaFCO)c.Impl).MetaRole.Name == "Terrain");

            if (terrain.Any())
            {
                CyPhy.Component terraincomp = terrain.First().AllReferred as CyPhy.Component;
                if (terraincomp == null)
                {
                    Logger.Instance.AddLogMessage("Terrain component reference must refer to a cyphy component.", Severity.Error);
                    return;
                }
                if (!terraincomp.Children.CADModelCollection.Any())
                {
                    Logger.Instance.AddLogMessage("Terrain component must contain a CAD model.", Severity.Error);
                    return;
                }
                TerrainFileName = terraincomp.Children.ResourceCollection.First().Attributes.Path;
                try
                {
                    File.Copy(Path.Combine(terraincomp.GetDirectoryPath(), TerrainFileName), Path.Combine(OutputDirectory, Path.GetFileName(TerrainFileName)));
                }
                catch (Exception e)
                {
                    Logger.Instance.AddLogMessage("Unable to copy terrain file: " + e.Message, Severity.Warning);
                }
                TerrainFileName = Path.GetFileNameWithoutExtension(TerrainFileName);
            }
            else
            {
                if (testBench.Children.AnchorCollection.Any())
                {
                    CyPhy.Anchor anchor = testBench.Children.AnchorCollection.First();
                    var pointList = anchor
                        .DstConnections
                        .SetAnchorCollection
                        .Select(x => x.DstEnds.Point)
                       .Concat(anchor
                       .SrcConnections
                       .SetAnchorCollection
                       .Select(x => x.SrcEnds.Point));

                    if (!pointList.Any())
                    {
                        Logger.Instance.AddLogMessage("Anchor is not connected to a point. Anchor should be connected to a datum point on one of the components.", Severity.Error);
                        return;
                    }

                    var point = pointList.First();
                    PointMetricTraversal traverser = new PointMetricTraversal(point);
                    foreach (var pointFound in traverser.portsFound)
                    {
                        string name = (pointFound as CyPhy.Point).Attributes.DatumName;
                        string componentID = CyPhyClasses.Component.Cast((pointFound as CyPhy.Point).ParentContainer.ParentContainer.Impl).Attributes.InstanceGUID;
                        StaticAnalysisMetrics.Add(new TBComputation() { ComponentID = componentID, Details = name, MetricID = "Anchor", ComputationType = TBComputation.Type.POINTCOORDINATES, RequestedValueType = "Vector" });
                        //Computations.Add(new TBComputation() { ComponentID = componentID, FeatureDatumName = name, MetricID = "Anchor", ComputationType = TBComputation.Type.POINTCOORDINATES, RequestedValueType = "Vector" });
                        GroundCyphyID = componentID;
                    }
                }
                else
                {
                    Logger.Instance.AddLogMessage("Anchor not found. There should be exactly one anchor in the testbench, to specify which body is fixed to the ground in the kinematic simnulation.", Severity.Error);
                    return;
                }
            }

            foreach (var motion in testBench.Children.MotionLoadCollection)
            {
                var connectorList = motion
                                .SrcConnections
                                .ApplyMotionToJointCollection
                                .Select(x => x.SrcEnds.Connector)
                                .Concat(motion
                                .DstConnections
                                .ApplyMotionToJointCollection
                                .Select(x => x.DstEnds.Connector)
                                );

                var toplevelSUT = testBench.Children.ComponentAssemblyCollection.FirstOrDefault();

                foreach (var connector in connectorList)
                {
                    OriginatingConnectorTraversal traverser = new OriginatingConnectorTraversal(connector);

                    if (traverser.result != null)
                    {
                        CyPhy.KinematicJoint joint = GetJoint(traverser.result);
                        if (joint != null)
                        {
                            Multibody.ModelLoadsMotion mbdMotion = new Multibody.ModelLoadsMotion();
                            mbdMotion.Active = (motion.Attributes.Active) ? Multibody.ModelLoadsMotionActive.on : Multibody.ModelLoadsMotionActive.off;
                            mbdMotion.FreedomType = (Multibody.ModelLoadsMotionFreedomType)((int)motion.Attributes.JointFreedomType);
                            mbdMotion.Function = motion.Attributes.CustomFunction;
                            mbdMotion.JointID = cadDataContainer.GetJointIDFromGMEID(joint.ID);
                            mbdMotion.MotionID = motion.Name;
                            mbdMotion.TimeDerivative = (Multibody.ModelLoadsMotionTimeDerivative)((int)motion.Attributes.TimeDerivative);
                            mbdMotion.Type = "JointMotion";

                            MotionList.Add(mbdMotion);
                        }
                        else
                        {
                            Logger.Instance.AddLogMessage("Kinematic motion is connected to a connector without joint information: " + connector.ToHyperLink(), Severity.Error);
                        }
                    }
                }
            }

            if (testBench.Children.AdamsScriptCollection.Any())
            {
                Scripts = new List<Multibody.ModelScript>();
                foreach (var script in testBench.Children.AdamsScriptCollection)
                {
                    Scripts.Add(new Multibody.ModelScript() { Path = script.Attributes.Path });
                }
            }

            ProcessContacts(testBench);
        }

        private void ProcessContacts(CyPhy.KinematicTestBench testBench)
        {
            List<Multibody.ModelContact> contactlist = new List<Multibody.ModelContact>();
            string cyphyid1, cyphyid2;
            foreach (CyPhy.Contact contact in testBench.Children.ContactCollection)
            {
                CyPhy.ComponentRef terrain = null;
                var tip1 = contact.SrcEnds.TestInjectionPoint;
                if (tip1 == null)
                    terrain = contact.SrcEnds.ComponentRef;
                var tip2 = contact.DstEnds.TestInjectionPoint;
                if (tip2 == null)
                    terrain = contact.SrcEnds.ComponentRef;
                if (terrain != null)
                {
                    var tip = tip1 == null ? tip2 : tip1;
                    if (tip.AllReferred == null)
                    {
                        Logger.Instance.AddLogMessage("TIP doesn't refer anything: " + tip.Path, Severity.Error);
                        continue;
                    }
                    if (!(tip.AllReferred is CyPhy.Component))
                    {
                        Logger.Instance.AddLogMessage("TIPs participating in contacts can only refer components: " + tip.Path, Severity.Error);
                        continue;
                    }
                    cyphyid1 = (tip.AllReferred as CyPhy.Component).Attributes.InstanceGUID;
                    cyphyid2 = "TERRAIN";
                }
                else
                {
                    if (tip1.AllReferred == null)
                    {
                        Logger.Instance.AddLogMessage("TIP doesn't refer anything: " + tip1.Path, Severity.Error);
                        continue;
                    }
                    if (tip2.AllReferred == null)
                    {
                        Logger.Instance.AddLogMessage("TIP doesn't refer anything: " + tip2.Path, Severity.Error);
                        continue;
                    }
                    if (!(tip1.AllReferred is CyPhy.Component))
                    {
                        Logger.Instance.AddLogMessage("TIPs participating in contacts can only refer components: " + tip1.Path, Severity.Error);
                        continue;
                    }
                    if (!(tip2.AllReferred is CyPhy.Component))
                    {
                        Logger.Instance.AddLogMessage("TIPs participating in contacts can only refer components: " + tip2.Path, Severity.Error);
                        continue;
                    }
                    cyphyid1 = (tip1.AllReferred as CyPhy.Component).Attributes.InstanceGUID;
                    cyphyid2 = (tip2.AllReferred as CyPhy.Component).Attributes.InstanceGUID;
                }
                contactlist.Add(new Multibody.ModelContact() { CyphyId1 = cyphyid1, CyphyId2 = cyphyid2 });
            }
            if (contactlist.Count > 0)
            {
                ContactList = contactlist;
            }
        }

        private CyPhy.Parameter GetGravityLoadParam(CyPhy.GravityLoad gravityload, string name, out double param)
        {
            try
            {
                CyPhy.Parameter prm = gravityload.Children.ParameterCollection.Where(p => ((MgaFCO)p.Impl).MetaRole.Name == name).First();
                param = String.IsNullOrEmpty(prm.Attributes.Value) ? 0 : double.Parse(prm.Attributes.Value);
                return prm;
            }
            catch (InvalidOperationException)
            {
                Logger.Instance.AddLogMessage("Parameter" + name + " was not found in AccelerationLoad.", Severity.Error);
            }
            catch (FormatException)
            {
                Logger.Instance.AddLogMessage("Parameter" + name + " is not a valid floating point value.", Severity.Error);
            }
            param = 0;
            return null;
        }

        public override bool GenerateOutputFiles()
        {
            if (!HasErrors())
            {
                GenerateMultiBodyOutput();
                GenerateCADXMLOutput();
                GenerateRunBat();
                GenerateScriptFiles();
                return true;
            }
            return false;
        }

        public override void GenerateCADXMLOutput()
        {
            CAD.AssembliesType assembliesoutroot = cadDataContainer.ToCADXMLOutput(this, MetaLink);
            CAD.ProcessingInstructionsType instr = new CAD.ProcessingInstructionsType();
            CAD.ProcessingInstructionType[] instructions = new CAD.ProcessingInstructionType[2];
            instructions[0] = new CAD.ProcessingInstructionType();
            instructions[1] = new CAD.ProcessingInstructionType();
            //instructions[2] = new CAD.ProcessingInstructionType();
            instructions[0].Primary = "COMPLETE_THE_HIERARCHY_FOR_LEAF_ASSEMBLIES";
            //instructions[1].Primary = "UNIQUELY_NAME_ALL_CAD_MODEL_INSTANCES";
            instructions[1].Primary = "OUTPUT_JOINT_INFORMATION";
            instructions[0].Secondary = instructions[1].Secondary = "";
            instructions[1].Secondary = "VALIDATE_JOINT_INFORMATION";
            instr.ProcessingInstruction = instructions;
            assembliesoutroot.ProcessingInstructions = instr;
            AddDataExchangeFormatToXMLOutput(assembliesoutroot);
            if (assembliesoutroot.Assembly.Length>0)
                AddStaticAnalysisMetrics(assembliesoutroot.Assembly[0]);
            assembliesoutroot.SerializeToFile(Path.Combine(OutputDirectory, TestBenchBase.CADAssemblyFile));
        }

        /*
        // This code is copy/pasted from TestBench. Since this class is not a subclass of TestBench,
        // it can't be re-used. Can this code be re-used from there by moving it to TestBenchBase?
        private void AddStaticAnalysis(CAD.AssemblyType assemblyRoot, List<TBComputation> computations)
        {
            if (computations.Any())
            {
                CAD.AnalysesType cadanalysis = GetCADAnalysis(assemblyRoot);

                CAD.StaticType staticanalysis = new CAD.StaticType();
                staticanalysis._id = UtilityHelpers.MakeUdmID();
                staticanalysis.AnalysisID = AnalysisID;

                List<CAD.MetricType> metriclist = new List<CAD.MetricType>();
                foreach (var item in computations)
                {
                    if (item.ComputationType == TBComputation.Type.POINTCOORDINATES)
                    {
                        CAD.MetricType ptout = new CAD.MetricType();
                        ptout._id = UtilityHelpers.MakeUdmID();
                        ptout.ComponentID = item.ComponentID;
                        ptout.MetricID = item.MetricID;
                        ptout.MetricType1 = item.ComputationType.ToString();
                        ptout.RequestedValueType = item.RequestedValueType;
                        ptout.Details = item.FeatureDatumName;
                        ptout.ComponentID = String.IsNullOrEmpty(item.ComponentID) ? "" : item.ComponentID;     // PointCoordinate metric is tied to a specific Component  
                        ptout.MetricName = item.MetricName ?? "";
                        metriclist.Add(ptout);
                    }
                    else
                    {
                        CAD.MetricType metric = new CAD.MetricType();
                        metric._id = UtilityHelpers.MakeUdmID();
                        metric.MetricID = item.MetricID;
                        metric.MetricType1 = item.ComputationType.ToString();
                        metric.RequestedValueType = item.RequestedValueType;
                        metric.ComponentID = assemblyRoot.ConfigurationID;
                        metric.Details = "";
                        metric.MetricName = item.MetricName ?? "";
                        metriclist.Add(metric);
                    }
                }

                staticanalysis.Metrics = new CAD.MetricsType();
                staticanalysis.Metrics._id = UtilityHelpers.MakeUdmID();
                staticanalysis.Metrics.Metric = metriclist.ToArray();

                cadanalysis.Static = new CAD.StaticType[] { staticanalysis };
            }
        }
        */

        public override void GenerateRunBat()
        {
            // Adams specific script
            StringBuilder sbuilder = new StringBuilder();
            string scriptparams = "";
            if (!String.IsNullOrEmpty(TerrainFileName))
            {
                scriptparams += "-terrain " + TerrainFileName + " -firstpass 1";
            }
            sbuilder.AppendLine("set ADAMS_SCRIPT=\"%MetaPath%bin\\CAD\\Adams\\CreateAdamsModel.py\"\n");

            sbuilder.AppendLine("if exist %ADAMS_SCRIPT% goto  :ADAMS_SCRIPT_FOUND");
            sbuilder.AppendLine("@echo off");
            sbuilder.AppendLine("echo		Error: Could not find %ADAMS_SCRIPT%.");
            sbuilder.AppendLine("set ERROR_CODE=2");
            sbuilder.AppendLine("set ERROR_MSG=\"Error: Could not find %ADAMS_SCRIPT%.\"");
            sbuilder.AppendLine("goto :ERROR_SECTION\n");

            sbuilder.AppendLine(":ADAMS_SCRIPT_FOUND");
            sbuilder.AppendLine("del adams.log\n");
            sbuilder.AppendLine("\"%MetaPath%bin\\Python27\\Scripts\\Python.exe\" %ADAMS_SCRIPT% " + scriptparams + " >> adams.log\n");

            if (!String.IsNullOrEmpty(TerrainFileName))
            {
                sbuilder.AppendLine("call %ADAMS_PATH%\\bin\\adams2013_2_x64.bat aview ru-st b adams_output.cmd\n");
                sbuilder.AppendLine("set PYTHONHOME=");
                scriptparams = "-terrain " + TerrainFileName;
                sbuilder.AppendLine("%MetaPath%bin\\Python27\\Scripts\\Python.exe\" %ADAMS_SCRIPT% " + scriptparams + " >> adams.log\n");
            }

            sbuilder.AppendLine("if %ERRORLEVEL% NEQ 0 (\n");
            sbuilder.AppendLine("set ERROR_CODE=%ERRORLEVEL%\n");
            sbuilder.AppendLine("set ERROR_MSG=\"Error during running the CreateAdamsModel script: error code is %ERROR_CODE%\"\n");
            sbuilder.AppendLine("echo %ERROR_MSG% >>_FAILED.txt\n");
            sbuilder.AppendLine("type adams.log >> _FAILED.txt\n");
            sbuilder.AppendLine("echo \"\"\n");
            sbuilder.AppendLine("echo \"See Error Log: _FAILED.txt\"\n");
            sbuilder.AppendLine("ping -n 8 127.0.0.1 > nul\n");
            sbuilder.AppendLine("exit /b %ERROR_CODE%\n");
            sbuilder.AppendLine(")\n");

            sbuilder.AppendLine("if exist %ADAMS_PATH%\\bin\\adams2013_2_x64.bat goto  :ADAMS_FOUND");
            
            sbuilder.AppendLine("@echo off");
            sbuilder.AppendLine("echo		Error: Adams is not installed or ADAMS_PATH environment variable is not set.");
            sbuilder.AppendLine("set ERROR_CODE=2");
            sbuilder.AppendLine("set ERROR_MSG=\"Error: Adams is not installed or ADAMS_PATH environment variable is not set.\"");
            sbuilder.AppendLine("goto :ERROR_SECTION\n");

            sbuilder.AppendLine(":ADAMS_FOUND");
            sbuilder.AppendLine("call %ADAMS_PATH%\\bin\\adams2013_2_x64.bat aview ru-st b adams_output.cmd");

            sbuilder.AppendLine("type aview.log | find /c \"ERROR:\" > tmpfile");
            sbuilder.AppendLine("FOR /F %%p IN ('type tmpfile') DO set crap=%%p");
            sbuilder.AppendLine("del tmpfile");

            sbuilder.AppendLine("if %crap% NEQ 0 (\n");
            sbuilder.AppendLine("set ERROR_CODE=2\n");
            sbuilder.AppendLine("set ERROR_MSG=\"Adams log contains errors.\n");
            sbuilder.AppendLine("echo %ERROR_MSG% >>_FAILED.txt\n");
            sbuilder.AppendLine("type aview.log >> _FAILED.txt\n");
            sbuilder.AppendLine("echo \"See Error Log: _FAILED.txt\"\n");
            sbuilder.AppendLine("ping -n 8 127.0.0.1 > nul\n");
            sbuilder.AppendLine("exit /b %ERROR_CODE%\n");
            sbuilder.AppendLine(")\n");


            Template.run_bat searchmeta = new Template.run_bat()
            {
                Automation = false,
                XMLFileName = "CADAssembly",
                ComputedMetricsPath = "\"Analysis\\Abaqus\\ComputedValues.xml\"",
                AdditionalOptions = CADOptions??"",
                Assembler = "CREO",
                Mesher = "NONE",
                Analyzer = "NONE",
                CallDomainTool = sbuilder.ToString()
            };
            using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "runCADJob.bat")))
            {
                writer.WriteLine(searchmeta.TransformText());
            }
        }

        public void GenerateMultiBodyOutput()
        {
            Multibody.Model mbdModel = new Multibody.Model();
            mbdModel.Name = Name;

            // Units
            mbdModel.Units = new Multibody.ModelUnits();
            mbdModel.Units.Angle = Multibody.ModelUnitsAngle.rad;
            mbdModel.Units.Force = "newton";
            mbdModel.Units.Length = "mm";
            mbdModel.Units.Mass = "kg";
            mbdModel.Units.Time = "sec";

            // Loads
            mbdModel.Loads = new Multibody.ModelLoads();
            mbdModel.Loads.Motion = MotionList.ToArray();
            if (Gravity != null)
            {
                mbdModel.Loads.Gravity = new Multibody.ModelLoadsGravity();
                mbdModel.Loads.Gravity.Active = Multibody.ModelLoadsGravityActive.on;
                mbdModel.Loads.Gravity.Value = String.Format("{0};{1};{2}", Gravity.X, Gravity.Y, Gravity.Z);
            }

            // Simulation
            mbdModel.Simulation = new Multibody.ModelSimulation();
            mbdModel.Simulation.Steps = SimulationStep.ToString();
            mbdModel.Simulation.Time = (float)SimulationTime;

            // Terrain
            if (!String.IsNullOrEmpty(TerrainFileName))
            {
                mbdModel.Terrain = new Multibody.ModelTerrain();
                mbdModel.Terrain.FileName = TerrainFileName;
            }

            // Ground
            if (!String.IsNullOrEmpty(GroundCyphyID))
            {
                mbdModel.Ground = new Multibody.ModelGround();
                mbdModel.Ground.CyphyId = GroundCyphyID;
            }

            if (ContactList != null)
                mbdModel.Contact = ContactList.ToArray();

            if (Scripts != null)
                mbdModel.Script = Scripts.ToArray();

            mbdModel.SerializeToFile(Path.Combine(OutputDirectory, "Analysis_MBD.xml"));

        }
    }
}
