using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using GME.CSharp;
using GME;
using GME.MGA;
using GME.MGA.Core;
// ICyPhyInterpreter and DSML
using CyPhyGUIs;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using Newtonsoft.Json;

namespace CyPhyCar
{
    class TestBenchProcessor
    {
        public static readonly Dictionary<string, AdamsCarFileReplacement> Replacements = new Dictionary<string, AdamsCarFileReplacement>();
        public static string acardbname = "isis_test";

        static TestBenchProcessor()
        {
            Replacements.Add("powertrain_subsys", new AdamsCarFileReplacement() { EntryType = "SUBSYSTEM", IdStrings = new string[] { "Major Role : powertrain" }, KeyToReplace = "USAGE" });
            Replacements.Add("body_subsys", new AdamsCarFileReplacement() { EntryType = "SUBSYSTEM", IdStrings = new string[] { "Major Role : body" }, KeyToReplace = "USAGE" });
            Replacements.Add("suspension_front_subsys", new AdamsCarFileReplacement() { EntryType = "SUBSYSTEM", IdStrings = new string[] { "Major Role : suspension", "Minor Role : front" }, KeyToReplace = "USAGE" });
            Replacements.Add("suspension_rear_subsys", new AdamsCarFileReplacement() { EntryType = "SUBSYSTEM", IdStrings = new string[] { "Major Role : body", "Minor Role : rear" }, KeyToReplace = "USAGE" });
            Replacements.Add("wheel_front_subsys", new AdamsCarFileReplacement() { EntryType = "SUBSYSTEM", IdStrings = new string[] { "Major Role : wheel", "Minor Role : front" }, KeyToReplace = "USAGE" });
            Replacements.Add("wheel_rear_subsys", new AdamsCarFileReplacement() { EntryType = "SUBSYSTEM", IdStrings = new string[] { "Major Role : wheel", "Minor Role : rear" }, KeyToReplace = "USAGE" });
            Replacements.Add("steering_subsys", new AdamsCarFileReplacement() { EntryType = "SUBSYSTEM", IdStrings = new string[] { "Major Role : steering", "Minor Role : front" }, KeyToReplace = "USAGE" });
            Replacements.Add("environment_subsys", new AdamsCarFileReplacement() { EntryType = "SUBSYSTEM", IdStrings = new string[] { "Major Role : environment", "Minor Role : any" }, KeyToReplace = "USAGE" });
            Replacements.Add("engine_torque", new AdamsCarFileReplacement() { EntryType = "GENERAL_SPLINE", IdKeys = new Dictionary<string, string>() { { "USAGE", "engine_torque" } }, KeyToReplace = "PROPERTY_FILE" });
            Replacements.Add("body_template", new AdamsCarFileReplacement() { EntryType = "SUBSYSTEM_HEADER", IdKeys = new Dictionary<string, string>() { { "MAJOR_ROLE", "body" } }, KeyToReplace = "TEMPLATE_NAME" });
            Replacements.Add("suspension_template", new AdamsCarFileReplacement() { EntryType = "SUBSYSTEM_HEADER", IdKeys = new Dictionary<string, string>() { { "MAJOR_ROLE", "suspension" } }, KeyToReplace = "TEMPLATE_NAME" });
            Replacements.Add("wheel_template", new AdamsCarFileReplacement() { EntryType = "SUBSYSTEM_HEADER", IdKeys = new Dictionary<string, string>() { { "MAJOR_ROLE", "wheel" } }, KeyToReplace = "TEMPLATE_NAME" });
        }
        public struct CarMetric
        {
            public enum LookupType
            {
                Value,
                Max,
                Min,
                Range
            }
            public string MetricID;
            public string AdamsResultName;
            public double LookupValue;
            public bool ReverseLookup;
            public string SimName;
            public LookupType Type;
        }

        public GMEConsole GMEConsole { get; set; }
        public string OutputDir;
        public string ProjectDir;
        private static readonly string CarModelDir = "carmodel.cdb";
        private static readonly string AssembliesDir = "assemblies.tbl";
        private static readonly string SimFileDir = "AdamsSimulations";
        private List<SimulationResult> Results = new List<SimulationResult>();
        private List<CarMetric> OutputMetrics = new List<CarMetric>();
        private List<CarParamMapping> CarParamMap = new List<CarParamMapping>();

        public interface SimulationResult
        {
            string ResultName
            {
                get;
            }
            double Time
            {
                get;
            }
        }

        private class AccelerationResult : SimulationResult
        {
            public AccelerationResult(double t)
            {
                time = t;
            }
            private double time;
            public string ResultName
            {
                get
                {
                    return "chassis_accelerations.longitudinal";
                }

            }
            public double Time
            {
                get
                {
                    return time;
                }
            }

        }

        private class CarParamMapping
        {
            public string FileName;
            public string ParamID;
            public string TBParamName;
        }

        private class CarFileParam
        {
            public string Name;
            public string Value;
            public AdamsCarFileReplacement Replacement;
        }

        private class CarResource
        {
            public string FileName;
            public List<CarFileParam> Parameters = new List<CarFileParam>();
            public Dictionary<string, CarResource> References = new Dictionary<string, CarResource>();
        }

        private static void DirectoryCopy(string sourceDirName, string destDirName, bool copySubDirs)
        {
            // Get the subdirectories for the specified directory.
            DirectoryInfo dir = new DirectoryInfo(sourceDirName);
            DirectoryInfo[] dirs = dir.GetDirectories();

            if (!dir.Exists)
            {
                throw new DirectoryNotFoundException(
                    "Source directory does not exist or could not be found: "
                    + sourceDirName);
            }

            // If the destination directory doesn't exist, create it. 
            if (!Directory.Exists(destDirName))
            {
                Directory.CreateDirectory(destDirName);
            }

            // Get the files in the directory and copy them to the new location.
            FileInfo[] files = dir.GetFiles();
            foreach (FileInfo file in files)
            {
                string temppath = Path.Combine(destDirName, file.Name);
                file.CopyTo(temppath, false);
            }

            // If copying subdirectories, copy them and their contents to new location. 
            if (copySubDirs)
            {
                foreach (DirectoryInfo subdir in dirs)
                {
                    string temppath = Path.Combine(destDirName, subdir.Name);
                    DirectoryCopy(subdir.FullName, temppath, copySubDirs);
                }
            }
        }

        private List<CyPhy.CarModel> CollectCarModels(CyPhy.CarTestBench testBench)
        {
            List<CyPhy.CarModel> result = new List<CyPhy.CarModel>();
            var topSUT = testBench.Children.ComponentAssemblyCollection.FirstOrDefault();
            if (topSUT == null)
            {
                GMEConsole.Error.WriteLine("No Top System Under Test found, nothing to be done.");
                return null;
            }
            /*            if (topSUT.AllReferred == null)
                        {
                            GMEConsole.Error.WriteLine("Top System Under Test is not referring anything. Nothing to be done.");
                            return null;
                        }*/
            //if (topSUT.AllReferred is CyPhy.ComponentAssembly)
            //{
            foreach (var comp in topSUT.Children.ComponentCollection)
            {
                if (comp.Children.CarModelCollection.Any())
                {
                    foreach (var carc in comp.Children.CarModelCollection)
                    {
                        result.Add(carc);
                    }
                }
            }

            //}
            return result;
        }

        private List<CarResource> CollectCarResources(List<CyPhy.CarModel> carcomps)
        {
            List<CarResource> result = new List<CarResource>();
            foreach (var comp in carcomps)
            {
                foreach (var res in comp.Children.CarResourceCollection)
                {
                    // Is the resource already listed?
                    CarResource currres = null;
                    var existingres = result.Where(r => r.FileName == res.Attributes.ResourcePath);
                    if (existingres.Any())
                    {
                        if (existingres.Count() > 1)
                        {
                            throw new Exception("Internal error: car resource found more than once.");
                        }
                        currres = existingres.First();

                    }
                    else
                    {
                        currres = new CarResource() { FileName = res.Attributes.ResourcePath };
                        result.Add(currres);
                    }
                    foreach (var param in res.SrcConnections.CarResourceParameterCollection)
                    {
                        if (param.SrcEnds.CarParameter != null)
                        {
                            if (param.SrcEnds.CarParameter.Attributes.Value.Length == 0)
                            {
                                GMEConsole.Warning.WriteLine("Empty parameter value for '" + param.SrcEnds.CarParameter.Name + "', ignoring.");
                            }
                            else
                            {
                                var existingparam = currres.Parameters.Where(p => p.Name == param.SrcEnds.CarParameter.Name);
                                if (existingparam.Any())
                                {
                                    GMEConsole.Warning.WriteLine("Parameter '" + existingparam.First().Name + "' set twice on resource '" + currres.FileName + "'. Ignoring second setting.");
                                }
                                else
                                {
                                    var newparam = new CarFileParam() { Name = param.SrcEnds.CarParameter.Name, Value = param.SrcEnds.CarParameter.Attributes.Value, Replacement = ((param.Attributes.ParameterKey.Length > 0) ? AdamsCarFileReplacement.FromString(param.Attributes.ParameterKey) : null) };
                                    currres.Parameters.Add(newparam);
                                }
                            }
                        }
                    }
                }
            }
            // Do a 2nd round to discover resource replacements
            foreach (var comp in carcomps)
            {
                foreach (var res in comp.Children.CarResourceCollection)
                {
                    foreach (var conn in res.SrcConnections.ReferenceSwapCollection)
                    {
                        if (conn.SrcEnds.CarResource != null)
                        {
                            var referenced = result.Where(r => r.FileName == res.Attributes.ResourcePath);
                            var referer = result.Where(r => r.FileName == conn.SrcEnds.CarResource.Attributes.ResourcePath);
                            if (referer.Any() && referenced.Any())
                            {
                                referenced.First().References.Add(conn.Attributes.ReferenceName, referer.First());
                            }
                            else
                            {
                                GMEConsole.Error.WriteLine("Internal error, resource was not found: " + conn.DstEnds.CarResource.Attributes.ResourcePath);
                            }
                        }
                    }
                }
            }

            return result;
        }

        private int[] ProcessParamColumn(string paramcolumn)
        {
            List<int> column = new List<int>();
            column.Add(paramcolumn.IndexOf("parameter_name"));
            column.Add(paramcolumn.IndexOf("symmetry"));
            column.Add(paramcolumn.IndexOf("type"));
            column.Add(paramcolumn.IndexOf("value"));
            return column.ToArray();
        }

        private string ProcessParamLine(string paramline, List<CarFileParam> paramlist, int[] paramcolumn)
        {
            string tmp = paramline.Trim();
            if (tmp.StartsWith("{")) return paramline;
            string paramname = paramline.Substring(paramcolumn[0] + 1, paramline.IndexOf('\'', paramcolumn[0] + 1) - paramcolumn[0] - 1);
            int paramlen = paramname.Length;
            var replaceparam = paramlist.Find(p => p.Name.ToLower() == paramname.Trim().ToLower() && p.Replacement == null);
            if (replaceparam != null)
            {
                return paramline.Substring(0, paramcolumn[3]) + replaceparam.Value;
            }
            else
            {
                return paramline;
            }
        }

        private string PrepareResource(CarResource res)
        {
            string content = null;
            List<string> prelines = new List<string>();
            List<string> paramlines = new List<string>();
            List<string> postlines = new List<string>();
            bool paramfound = false;
            bool paramfinished = false;
            int[] lastparamcolumn = null;
            using (StreamReader reader = new StreamReader(Path.Combine(ProjectDir, CarModelDir, res.FileName)))
            {
                while (!reader.EndOfStream)
                {
                    string line = reader.ReadLine();
                    if (!paramfound)
                    {
                        prelines.Add(line);
                    }
                    else if (paramfinished)
                    {
                        postlines = new List<string>();
                    }
                    else
                    {
                        string tmp = line.Trim();
                        if (tmp.StartsWith("{"))
                        {
                            lastparamcolumn = ProcessParamColumn(line);
                            paramlines.Add(line);
                        }
                        else if (tmp.StartsWith("'"))
                        {
                            paramlines.Add(line);
                        }
                        else
                        {
                            paramfinished = true;
                            postlines.Add(line);
                        }
                    }
                    if (line == "[PARAMETER]")
                    {
                        paramfound = true;
                    }
                }

            }
            StringBuilder b = new StringBuilder();
            using (StringWriter writer = new StringWriter(b))
            {
                foreach (string s in prelines)
                {
                    writer.WriteLine(s);
                }
                foreach (string s in paramlines)
                {
                    writer.WriteLine(ProcessParamLine(s, res.Parameters, lastparamcolumn));
                }
                foreach (string s in postlines)
                {
                    writer.WriteLine(s);
                }
            }
            return b.ToString();

        }

        private void CopyAndPrepareResources(List<CarResource> resources)
        {
            Directory.CreateDirectory(Path.Combine(OutputDir, CarModelDir));
            Directory.CreateDirectory(Path.Combine(OutputDir, CarModelDir, AssembliesDir));
            // By default, put the default car assembly there
            DefaultCarAssembly defaultasy = new DefaultCarAssembly();
            using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDir, CarModelDir, AssembliesDir, "DefaultCar.asy"), false))
            {
                writer.WriteLine(defaultasy.TransformText());
            }
            foreach (var res in resources)
            {
                try
                {
                    if (res.FileName.EndsWith(".tpl"))
                    {
                        File.Copy(Path.Combine(ProjectDir, CarModelDir, res.FileName), Path.Combine(OutputDir, CarModelDir, res.FileName), true);
                    }
                    else
                    {
                        string rescontent = PrepareResource(res);
                        string dirpath = Path.GetDirectoryName(Path.Combine(OutputDir, CarModelDir, res.FileName));

                        if (!Directory.Exists(dirpath))
                        {
                            Directory.CreateDirectory(dirpath);
                        }
                        AdamsCarFile test = new AdamsCarFile(rescontent);
                        foreach (var resconnect in res.References)
                        {
                            if (Replacements.ContainsKey(resconnect.Key))
                            {
                                test.Replace(Replacements[resconnect.Key], resconnect.Value.FileName);
                            }
                            else
                            {
                                GMEConsole.Warning.WriteLine("Replacement with key '" + resconnect.Key + "' was not found. Ignoring.");
                            }
                        }
                        foreach (var param in res.Parameters)
                        {
                            if (param.Replacement != null)
                            {
                                test.Replace(param.Replacement, param.Value);
                            }
                        }
                        using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDir, CarModelDir, res.FileName), false))
                        {
                            writer.WriteLine(test.ToString());
                        }
                        //GMEConsole.Info.WriteLine(test.ToString());
                    }
                }
                catch (System.IO.DirectoryNotFoundException ex)
                {
                    GMEConsole.Error.WriteLine("Car resource '" + res.FileName + "' was not found. This file will not be included in the ADAMS model.");
                    throw ex;
                }
                catch (System.IO.FileNotFoundException ex)
                {
                    GMEConsole.Error.WriteLine("Car resource '" + res.FileName + "' was not found. This file will not be included in the ADAMS model.");
                    throw ex;
                }
            }
        }

        private CyPhy.Parameter FollowParam(CyPhy.Parameter input)
        {
            CyPhy.ValueFlow conn = null;
            while ((conn = input.SrcConnections.ValueFlowCollection.FirstOrDefault()) != null)
            {
                input = conn.SrcEnds.Parameter;
            }
            return input;
        }

        private int GetParamOrDefault(CyPhy.Parameter param, int def)
        {
            if (param == null || param.Attributes.Value.Length == 0)
            {
                GMEConsole.Warning.WriteLine("No parameter was specified, assuming " + def);
                return def;
            }
            else
            {
                return Int32.Parse(param.Attributes.Value);
            }
        }

        private string GetSimulationScriptEntry(CyPhy.CarSimulation sim)
        {
            if (sim.Kind == "Acceleration")
            {
                AccelSim outp = new AccelSim();
                CyPhy.Acceleration accelsim = sim as CyPhy.Acceleration;
                CyPhy.Parameter endtimeparam = accelsim.Children.ParameterCollection.Where(p => ((MgaFCO)p.Impl).MetaRole.Name == "EndTime").FirstOrDefault();
                int endtime = GetParamOrDefault(endtimeparam, 20);
                outp.ModelName = "DefaultCar";
                outp.InitialVelocity = "3";
                outp.NumSteps = (endtime * 10).ToString();
                outp.EndTime = endtime.ToString();
                outp.FinalThrottle = "100.0";
                outp.StepDuration = "1.0";
                outp.StartTime = "0.0";
                outp.ShiftGears = "no";
                outp.OutputPrefix = "test";
                outp.OutputPostfix = sim.Kind;
                outp.GearPosition = "2";
                outp.Acceleration = "0";
                outp.RoadData = "acar_shared/roads.tbl/2d_flat.rdf";
                CarParamMap.Add(new CarParamMapping() { FileName = "adams_car.cmd", TBParamName = FollowParam(endtimeparam).Name, ParamID = "EndTime" });
                return outp.TransformText();
            }
            else if (sim.Kind == "Braking")
            {
                BrakingSim outp = new BrakingSim();
                outp.ModelName = "DefaultCar";
                outp.InitialVelocity = "20";
                outp.NumSteps = "300";
                outp.EndTime = "30";
                outp.FinalBrake = "100.0";
                outp.StepDuration = "1.0";
                outp.StartValue = "0.0";
                outp.OutputPrefix = "test";
                outp.OutputPostfix = sim.Kind;
                outp.GearPosition = "5";
                return outp.TransformText();
            }
            else if (sim.Kind == "ConstantSpeed")
            {
                ConstantSpeedSim outp = new ConstantSpeedSim();
                CyPhy.ConstantSpeed csim = sim as CyPhy.ConstantSpeed;
                CyPhy.Parameter endtimeparam = csim.Children.ParameterCollection.Where(p => ((MgaFCO)p.Impl).MetaRole.Name == "EndTime").FirstOrDefault();
                CyPhy.Parameter speedparam = csim.Children.ParameterCollection.Where(p => ((MgaFCO)p.Impl).MetaRole.Name == "Speed").FirstOrDefault();
                CyPhy.Parameter gearparam = csim.Children.ParameterCollection.Where(p => ((MgaFCO)p.Impl).MetaRole.Name == "Gear").FirstOrDefault();
                int endtime = GetParamOrDefault(endtimeparam, 20);
                int speed = GetParamOrDefault(speedparam, 15);
                int gear = GetParamOrDefault(gearparam, 2);
                outp.ModelName = "DefaultCar";
                outp.InitialVelocity = speed.ToString();
                outp.NumSteps = (endtime * 10).ToString();
                outp.GearPosition = gear.ToString();
                outp.OutputPrefix = "test";
                outp.OutputPostfix = sim.Kind;
                outp.EndTime = endtime.ToString();
                outp.RoadData = "roads.tbl\\pothole_4in.rdf";
                CarParamMap.Add(new CarParamMapping() { FileName = "adams_car.cmd", TBParamName = FollowParam(endtimeparam).Name, ParamID = "EndTime" });
                CarParamMap.Add(new CarParamMapping() { FileName = "adams_car.cmd", TBParamName = FollowParam(speedparam).Name, ParamID = "Speed" });
                CarParamMap.Add(new CarParamMapping() { FileName = "adams_car.cmd", TBParamName = FollowParam(gearparam).Name, ParamID = "Gear" });
                /*AccelSim outp = new AccelSim();
                CyPhy.ConstantSpeed csim = sim as CyPhy.ConstantSpeed;
                CyPhy.Parameter endtimeparam = csim.Children.ParameterCollection.Where(p => ((MgaFCO)p.Impl).MetaRole.Name == "EndTime").FirstOrDefault();
                int endtime = GetParamOrDefault(endtimeparam, 20);
                outp.ModelName = "DefaultCar";
                outp.InitialVelocity = "3";
                outp.NumSteps = (endtime * 10).ToString();
                outp.EndTime = endtime.ToString();
                outp.FinalThrottle = "100.0";
                outp.StepDuration = "5.0";
                outp.StartTime = "0.0";
                outp.ShiftGears = "no";
                outp.OutputPrefix = "test";
                outp.OutputPostfix = sim.Kind;
                outp.GearPosition = "2";
                outp.Acceleration = "1";
                outp.RoadData = "isis_test\\roads.tbl\\pothole_4in.rdf";
                CarParamMap.Add(new CarParamMapping() { FileName = "adams_car.cmd", TBParamName = FollowParam(endtimeparam).Name, ParamID = "EndTime" });
                 */
                return outp.TransformText();
            }
            else if (sim.Kind == "Steering")
            {
                SteerSim outp = new SteerSim();
                CyPhy.Steering ssim = sim as CyPhy.Steering;
                CyPhy.Parameter endtimeparam = ssim.Children.ParameterCollection.Where(p => ((MgaFCO)p.Impl).MetaRole.Name == "EndTime").FirstOrDefault();
                CyPhy.Parameter speedparam = ssim.Children.ParameterCollection.Where(p => ((MgaFCO)p.Impl).MetaRole.Name == "Speed").FirstOrDefault();
                CyPhy.Parameter angleparam = ssim.Children.ParameterCollection.Where(p => ((MgaFCO)p.Impl).MetaRole.Name == "Angle").FirstOrDefault();
                int endtime = GetParamOrDefault(endtimeparam, 20);
                int speed = GetParamOrDefault(speedparam, 50);
                int angle = GetParamOrDefault(angleparam, 20);
                outp.ModelName = "DefaultCar";
                outp.OutputPrefix = "test";
                outp.OutputPostfix = sim.Kind;
                outp.EndTime = endtime.ToString();
                outp.InitialVelocity = speed.ToString();
                outp.NumSteps = (endtime * 10).ToString();
                outp.SteerAngle = angle.ToString();
                outp.GearPosition = "1";
                CarParamMap.Add(new CarParamMapping() { FileName = "adams_car.cmd", TBParamName = FollowParam(endtimeparam).Name, ParamID = "EndTime" });
                CarParamMap.Add(new CarParamMapping() { FileName = "adams_car.cmd", TBParamName = FollowParam(speedparam).Name, ParamID = "Speed" });
                CarParamMap.Add(new CarParamMapping() { FileName = "adams_car.cmd", TBParamName = FollowParam(angleparam).Name, ParamID = "Angle" });
                return outp.TransformText();
            }
            else
            {
                GMEConsole.Warning.WriteLine("Unknown simulation type:" + sim.Kind);
            }
            return null;
        }

        private void GenerateAdamsCarCmd(string filename, CyPhy.CarTestBench testBench)
        {
            string dbpath = Path.Combine(OutputDir, CarModelDir).Replace("\\", "\\\\");
            using (StreamWriter writer = new StreamWriter(filename, false))
            {

                writer.WriteLine("acar toolkit database add  &");
                writer.WriteLine("database_name = \"" + acardbname + "\"  &");
                writer.WriteLine("database_path = \"" + dbpath + "\"  &");
                writer.WriteLine("alert = yes  &");
                writer.WriteLine("error_variable = .ACAR.variables.errorFlag");
                writer.WriteLine("!");

                writer.WriteLine("acar files assembly open  &");
                writer.WriteLine("assembly_name = \"mdids://" + acardbname + "/assemblies.tbl/DefaultCar.asy\"  &");
                writer.WriteLine("error_variable = .ACAR.variables.errorFlag");
                writer.WriteLine("!");

                foreach (var sim in testBench.Children.CarSimulationCollection)
                {
                    writer.WriteLine(GetSimulationScriptEntry(sim));
                    writer.WriteLine("!");
                }

                writer.WriteLine("acar toolkit database remove  &");
                writer.WriteLine("database_name = \"" + acardbname + "\"  &");
                writer.WriteLine("error_variable = .ACAR.variables.errorFlag");


            }
        }

        private void GenerateAdamsViewCmd(string path, CyPhy.CarTestBench testBench)
        {
            using (StreamWriter writer = new StreamWriter(path, false))
            {
                foreach (var sim in testBench.Children.CarSimulationCollection)
                {

                    writer.WriteLine("file analysis read  &");
                    writer.WriteLine("file_name = \"test_" + sim.Kind + "\"");
                    writer.WriteLine("!");

                    HashSet<string> datatoextract = new HashSet<string>();
                    foreach (var metricconn in sim.DstConnections.CarSimulationToMetricCollection)
                    {
                        if (metricconn.DstEnds.Metric != null)
                        {
                            CarMetric m = new CarMetric();
                            m.MetricID = metricconn.DstEnds.Metric.Name;
                            m.SimName = sim.Name;
                            if (metricconn.DstEnds.Metric.Name == "Acc0_15")
                            {
                                datatoextract.Add("test_" + sim.Name + ".chassis_velocities.longitudinal");
                                m.AdamsResultName = "chassis_velocities.longitudinal";
                                m.LookupValue = 15;
                                m.ReverseLookup = true;
                            }
                            else if (metricconn.DstEnds.Metric.Name == "Acc0_50")
                            {
                                datatoextract.Add("test_" + sim.Name + ".chassis_velocities.longitudinal");
                                m.AdamsResultName = "chassis_velocities.longitudinal";
                                m.LookupValue = 50;
                                m.ReverseLookup = true;
                            }
                            else if (metricconn.DstEnds.Metric.Name == "Acc0_65")
                            {
                                datatoextract.Add("test_" + sim.Name + ".chassis_velocities.longitudinal");
                                m.AdamsResultName = "chassis_velocities.longitudinal";
                                m.LookupValue = 65;
                                m.ReverseLookup = true;
                            }
                            else if (metricconn.DstEnds.Metric.Name == "Braking_Dist")
                            {
                                datatoextract.Add("test_" + sim.Name + ".chassis_displacements.longitudinal");
                                m.AdamsResultName = "chassis_displacements.longitudinal";
                                m.LookupValue = 300; // Last number
                            }
                            else if (metricconn.DstEnds.Metric.Name == "Braking_Accel")
                            {
                                datatoextract.Add("test_" + sim.Name + ".chassis_accelerations.longitudinal");
                                m.AdamsResultName = "chassis_accelerations.longitudinal";
                                m.Type = CarMetric.LookupType.Min;
                            }
                            else if (metricconn.DstEnds.Metric.Name == "TopSpeed")
                            {
                                datatoextract.Add("test_" + sim.Name + ".chassis_velocities.longitudinal");
                                m.AdamsResultName = "chassis_velocities.longitudinal";
                                m.Type = CarMetric.LookupType.Max;
                            }
                            else if (metricconn.DstEnds.Metric.Name == "RideQuality")
                            {
                                datatoextract.Add("test_" + sim.Name + ".absorbed_power.driver_seat_accel_var.Q");
                                m.AdamsResultName = "absorbed_power.driver_seat_accel_var.Q";
                                m.Type = CarMetric.LookupType.Max;
                            }
                            else if (metricconn.DstEnds.Metric.Name == "TurnCircle")
                            {
                                datatoextract.Add("test_" + sim.Name + ".til_wheel_contact_patch.y_front");
                                m.AdamsResultName = "til_wheel_contact_patch.y_front";
                                m.Type = CarMetric.LookupType.Range;
                            }
                            else
                            {
                                GMEConsole.Warning.WriteLine("Unknown Metric ID: " + metricconn.DstEnds.Metric.Name + ", ignoring.");
                                continue;
                            }
                            OutputMetrics.Add(m);
                        }
                    }
                    foreach (string data in datatoextract)
                    {
                        var resext = new NumericResultExtract();
                        resext.Name = data;
                        resext.FileName = data + "_res.txt";
                        writer.WriteLine(resext.TransformText());
                        writer.WriteLine("!");
                    }

                    writer.WriteLine("numeric_results list_info all  &");
                    writer.WriteLine("result_set_component_name = test_" + sim.Kind + ".TIME  &");
                    writer.WriteLine("write_to_terminal = off &");
                    writer.WriteLine("file_name = \"TIME_" + sim.Name + "_res.txt\"");
                    writer.WriteLine("!");
                }

            }
        }

        private void CreateAcarDatabase()
        {
            DirectoryCopy(Path.Combine(ProjectDir, CarModelDir), Path.Combine(OutputDir, CarModelDir), true);
            /*foreach (string d in Directory.GetDirectories(Path.Combine(ProjectDir, CarModelDir)))
            {
                string s = Path.GetFileName(d);
                Directory.CreateDirectory(Path.Combine(OutputDir, CarModelDir, s));
            }*/
        }

        public void Process(CyPhy.CarTestBench testBench)
        {
            GMEConsole.Info.WriteLine("Processing Test Bench: {0}", testBench.Name);
            List<CyPhy.CarModel> carcomps = CollectCarModels(testBench);
            CreateAcarDatabase();
            CopyAndPrepareResources(CollectCarResources(carcomps));
            GenerateAdamsCarCmd(Path.Combine(OutputDir, "adams_car.cmd"), testBench);
            //PrepareResults(testBench);
            GenerateAdamsViewCmd(Path.Combine(OutputDir, "adams_view.cmd"), testBench);
            string outmetrics = Newtonsoft.Json.JsonConvert.SerializeObject(OutputMetrics, Formatting.Indented);
            using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDir, "metrics_tmp.json"), false))
            {
                writer.WriteLine(outmetrics);
            }
            string outcarparammap = Newtonsoft.Json.JsonConvert.SerializeObject(CarParamMap, Formatting.Indented);
            using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDir, "parammap.json"), false))
            {
                writer.WriteLine(outcarparammap);
            }
            runCarSim_bat bat = new runCarSim_bat();
            using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDir, "runCarSim.bat"), false))
            {
                writer.WriteLine(bat.TransformText());
            }
            if (carcomps == null || !carcomps.Any()) return;

        }
    }
}
