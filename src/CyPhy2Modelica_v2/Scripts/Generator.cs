using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhy2Modelica_v2;
using System.IO;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2Modelica_v2.Scripts
{
    public class Generator
    {
        private CyPhyGUIs.IInterpreterMainParameters mainParameters { get; set; }

        //public static GME.CSharp.GMEConsole GMEConsole { get; set; }
        public CyPhyGUIs.GMELogger Logger { get; set; }
        public string RunCommand { get; set; }
        public Modelica.LimitDefinition LimitDefinition { get; set; }
        public ModelConfig CurrentModelConfig { get; set; }
        public Generator(CyPhyGUIs.IInterpreterMainParameters parameters, CyPhyGUIs.GMELogger logger)
        {
            this.Logger = logger;
            this.mainParameters = parameters;
            this.RunCommand = "";
        }

        public bool Generate(string originalDesignName)
        {
            var testBench = CyPhyClasses.TestBench.Cast(this.mainParameters.CurrentFCO);

            // generate new manifest file
            AVM.DDP.MetaTBManifest manifest = new AVM.DDP.MetaTBManifest();
            manifest.MakeManifest(testBench, this.mainParameters.OutputDirectory);
            if (string.IsNullOrWhiteSpace(originalDesignName) == false)
            {
                manifest.DesignName = originalDesignName;
            }
            else
            {
                this.Logger.WriteWarning("Could not find an original design. Manifest file will use the name of the test-bench, i.e. '{0}'.", testBench.Name);
            }

            manifest.Serialize(this.mainParameters.OutputDirectory);
            // create scripts directory
            string scriptDir = Path.Combine(this.mainParameters.OutputDirectory, "scripts");
            Directory.CreateDirectory(scriptDir);

            // create modelica libraries directory
            string modelicaDir = Path.Combine(this.mainParameters.OutputDirectory, "Libraries");
            Directory.CreateDirectory(modelicaDir);

            // get CyPhy directory
            string cyphyDir = Path.Combine(this.mainParameters.OutputDirectory, Modelica.CodeGenerator.MainPackage);

            // copy libraries into the output directory
            var settings = this.mainParameters.config as CyPhy2Modelica_v2Settings;

            foreach (var library in settings.IncludeDirectoryPath)
            {
                var sourcePath = library;
                if (Path.GetFileName(library) == "package.mo")
                {
                    sourcePath = Path.GetDirectoryName(library);
                }

                if (Path.IsPathRooted(sourcePath) == false)
                {
                    // Relative paths are defined relative to the project!
                    sourcePath = Path.Combine(this.mainParameters.ProjectDirectory, sourcePath);
                }

                // TODO: Move this to the Checker
                if ((File.Exists(sourcePath) || Directory.Exists(sourcePath)) == false)
                {
                    this.Logger.WriteError("External Modelica Library does not exist at {0}.", sourcePath);
                    continue;
                }

                var destinationPath = Path.Combine(modelicaDir, Path.GetFileName(sourcePath));
                if (sourcePath.EndsWith(".mo") == false)
                {
                    Directory.CreateDirectory(destinationPath);
                    // Create all of the directories
                    foreach (string dirPath in Directory.GetDirectories(sourcePath, "*", SearchOption.AllDirectories))
                    {
                        if (dirPath.Split(Path.DirectorySeparatorChar)
                            .Where(dir => dir == ".svn" || dir == ".git").Count() > 0)
                        {
                            continue;
                        }

                        Directory.CreateDirectory(dirPath.Replace(sourcePath, destinationPath));
                    }

                    // Copy all the files
                    foreach (string newPath in Directory.GetFiles(sourcePath, "*.*", SearchOption.AllDirectories))
                    {
                        if (newPath.Split(Path.DirectorySeparatorChar)
                            .Where(dir => dir == ".svn" || dir == ".git").Count() > 0)
                        {
                            continue;
                        }

                        File.Copy(newPath, newPath.Replace(sourcePath, destinationPath), true);
                    }
                }
                else
                {
                    File.Copy(sourcePath, destinationPath, true);
                }

                this.Logger.WriteDebug("Copied library from {0} to {1}", sourcePath, destinationPath);
            }

            Templates.open_package_in_dymola dymola_open = new Templates.open_package_in_dymola();
            File.WriteAllText(Path.Combine(this.mainParameters.OutputDirectory, "open_in_dymola.cmd"), dymola_open.TransformText());

            Templates.open_package_in_om om_open = new Templates.open_package_in_om();
            File.WriteAllText(Path.Combine(this.mainParameters.OutputDirectory, "open_in_om.cmd"), om_open.TransformText());


            // generate scripts
            File.WriteAllText(Path.Combine(this.mainParameters.OutputDirectory, "simulate_dymola.cmd"), 
                CyPhy2Modelica_v2.Properties.Resources._simulate_cmd.Replace("TOOL_TO_BE_REPLACED", "Dymola"));

            File.WriteAllText(Path.Combine(this.mainParameters.OutputDirectory, "simulate_om.cmd"),
                CyPhy2Modelica_v2.Properties.Resources._simulate_cmd.Replace("TOOL_TO_BE_REPLACED", "OpenModelica"));

            File.WriteAllText(Path.Combine(this.mainParameters.OutputDirectory, "simulate_jmodelica.cmd"),
                CyPhy2Modelica_v2.Properties.Resources._simulate_cmd.Replace("TOOL_TO_BE_REPLACED", "JModelica"));

            var solver = new Modelica.SolverSettings(testBench.Children.SolverSettingsCollection.FirstOrDefault());

            if (solver.ToolSelection.Contains("OpenModelica"))
            {
                this.RunCommand = "simulate_om.cmd";
            }
            else if (solver.ToolSelection.Contains("Dymola"))
            {
                this.RunCommand = "simulate_dymola.cmd";
            }
            else if (solver.ToolSelection.Contains("JModelica"))
            {
                this.RunCommand = "simulate_jmodelica.cmd";

                Templates.jmodelica_compile_py jmodelica_compile = new Templates.jmodelica_compile_py();
                File.WriteAllText(Path.Combine(scriptDir, "jmodelica_compile.py"), jmodelica_compile.TransformText());

                Templates.jmodelica_simulate_py jmodelica_simulate = new Templates.jmodelica_simulate_py();
                File.WriteAllText(Path.Combine(scriptDir, "jmodelica_simulate.py"), jmodelica_simulate.TransformText());
            }
            else
            {
                throw new NotSupportedException(solver.ToolSelection);
            }

            Templates.simulate_py simulate_py = new Templates.simulate_py();
            File.WriteAllText(Path.Combine(scriptDir, "simulate.py"), simulate_py.TransformText());

            //Templates.compile_and_run_jmodelica_py run_jmodelica = new Templates.compile_and_run_jmodelica_py();
            //File.WriteAllText(Path.Combine(scriptDir, "compile_and_run_jmodelica.py"), run_jmodelica.TransformText());

            this.CurrentModelConfig = this.GenerateModelConfig(Path.Combine(this.mainParameters.OutputDirectory, Modelica.CodeGenerator.MainPackage), testBench, settings.IncludeDirectoryPath);

            this.CopyOverPostProcessingScripts(Path.Combine(this.mainParameters.OutputDirectory, Modelica.CodeGenerator.MainPackage), testBench);

            this.LimitChecks(Path.Combine(this.mainParameters.OutputDirectory, Modelica.CodeGenerator.MainPackage), this.CurrentModelConfig.result_file);

            return true;
        }

        private void LimitChecks(string outputDirectory, string resultFile)
        {
            LimitDefinition.PathToMatFile = resultFile;

            string limit_filename = Path.Combine(outputDirectory, "limits.json");

            using (StreamWriter writer = new StreamWriter(limit_filename))
            {
                var jsonString = Newtonsoft.Json.JsonConvert.SerializeObject(
                    LimitDefinition,
                    Newtonsoft.Json.Formatting.Indented);

                writer.WriteLine(jsonString);
            }
        }


        private ModelConfig GenerateModelConfig(
            string outputDirectory,
            CyPhy.TestBench testBench,
            List<string> includeLibs)
        {
            Modelica.SolverSettings solver = new Modelica.SolverSettings(testBench.Children.SolverSettingsCollection.FirstOrDefault());

            ModelConfig modelConfig = new ModelConfig()
            {
                model_name = "CyPhy.TestBenches." + testBench.Name,
                result_file = testBench.Name,
                model_file_name = "package.mo",
                verification_model_name = "CyPhy.TestBenches.verif_" + testBench.Name
            };

            if (solver != null)
            {
                modelConfig.experiment.StartTime = solver.StartTime.ToString();
                modelConfig.experiment.StopTime = solver.StopTime.ToString();

                if (solver.Tolerance > 0)
                {
                    modelConfig.experiment.Tolerance = solver.Tolerance.ToString();
                }

                if (solver.UsesNumberOfIntervals)
                {
                    modelConfig.experiment.IntervalMethod = "NumberOfIntervals";
                    if (solver.NumberOfIntervals > 0)
                    {
                        modelConfig.experiment.NumberOfIntervals = solver.NumberOfIntervals.ToString();
                    }
                }
                else
                {
                    modelConfig.experiment.IntervalMethod = "Interval";
                    modelConfig.experiment.Interval = solver.IntervalLength.ToString();
                }

                modelConfig.experiment.Algorithm.Dymola = solver.DymolaSolver;
                modelConfig.experiment.Algorithm.OpenModelica = solver.OpenModelicaSolver;
                modelConfig.experiment.Algorithm.JModelica = solver.JModelicaSolver;
                modelConfig.experiment.ToolSelection = solver.ToolSelection;
            }

            modelConfig.lib_package_paths.Add("../Libraries");
            modelConfig.MSL_version = "3.2";
            foreach (var libPath in includeLibs)
            {
                var library = ModelicaLibrary.GetLibraryFromPath(libPath);
                if (library.Name == "Modelica")
                {
                    modelConfig.MSL_version = library.Version;
                }
                else
                {
                    modelConfig.lib_package_names.Add(library.Name);
                }
            }

            var model_config_filename = Path.Combine(outputDirectory, "model_config.json");

            using (StreamWriter writer = new StreamWriter(model_config_filename))
            {
                var jsonString = Newtonsoft.Json.JsonConvert.SerializeObject(
                    modelConfig,
                    Newtonsoft.Json.Formatting.Indented);

                writer.WriteLine(jsonString);
            }

            return modelConfig;
        }

        private void CopyOverPostProcessingScripts(string outputDirectory, CyPhy.TestBench testBench)
        {
            string outputPostProcDir = Path.Combine(outputDirectory, "PostProcessing");
            List<string> postProcFileNames = new List<string>();

            foreach (var postProc in testBench.Children.PostProcessingCollection)
            {
                Directory.CreateDirectory(outputPostProcDir);
                var scriptFilePath = postProc.Attributes.ScriptPath;

                if (string.IsNullOrWhiteSpace(scriptFilePath))
                {
                    continue;
                }

                if (Path.IsPathRooted(scriptFilePath) == false) // if path starts with / it is rooted. Use ./ or nothing
                {
                    scriptFilePath = Path.Combine(this.mainParameters.ProjectDirectory, scriptFilePath);
                }

                if (File.Exists(scriptFilePath))
                {

                    // Copy postScript file
                    FileInfo postScript = new FileInfo(scriptFilePath);
                    string destFile = Path.Combine(outputPostProcDir, postScript.Name);
                    File.Copy(postScript.FullName, destFile, true);
                    // META-3580
                    postProcFileNames.Add(Path.GetFileNameWithoutExtension(postScript.Name));

                    // If common folder exist copy entire folder.
                    DirectoryInfo commonSrc = new DirectoryInfo(Path.Combine(postScript.DirectoryName, "common"));
                    DirectoryInfo commonTarget = new DirectoryInfo(Path.Combine(outputPostProcDir, "common"));
                    if (commonSrc.Exists && commonTarget.Exists == false)
                    {
                        commonTarget.Create();
                        CopyFilesRecursively(commonSrc, commonTarget);
                    }
                }
            }

            // META-3580
            if (postProcFileNames.Any())
            {
                StringBuilder sb = new StringBuilder();

                sb.AppendLine("# Generated file using the META tools");
                sb.AppendLine("# Exposes all get_metrics functions for external usage");
                sb.AppendLine();

                // import all post processing files
                foreach (var postScriptFileName in postProcFileNames)
                {
                    sb.AppendFormat("import {0}", postScriptFileName);
                    sb.AppendLine();
                }

                sb.AppendLine();

                // expose get_metrics functions
                sb.AppendFormat("post_processing_modules = [{0}]", string.Join(", ", postProcFileNames));
                sb.AppendLine();

                sb.AppendLine();

                File.WriteAllText(Path.Combine(outputPostProcDir, "__init__.py"), sb.ToString(), Encoding.UTF8);
            }

        }

        /// <summary>
        /// Copies entire directory to another.
        /// </summary>
        /// <param name="source"></param>
        /// <param name="target"></param>
        private static void CopyFilesRecursively(DirectoryInfo source, DirectoryInfo target)
        {
            foreach (DirectoryInfo dir in source.GetDirectories())
            {
                if (dir.FullName.Split(Path.DirectorySeparatorChar)
                    .Where(d => d == ".svn" || d == ".git").Count() > 0)
                {
                    continue;
                }

                CopyFilesRecursively(dir, target.CreateSubdirectory(dir.Name));
            }

            foreach (FileInfo file in source.GetFiles())
            {
                if (file.Directory.FullName.Split(Path.DirectorySeparatorChar)
                    .Where(d => d == ".svn" || d == ".git").Count() > 0)
                {
                    continue;
                }

                file.CopyTo(Path.Combine(target.FullName, file.Name), true);
            }
        }
    }
}
