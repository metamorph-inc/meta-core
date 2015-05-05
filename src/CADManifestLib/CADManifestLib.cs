namespace CADManifestLib
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.IO;
    using System.Collections.Generic;
    using Newtonsoft.Json;

    public static class CADManifestLib
    {

        public static void CreateCADManifest(string projectJsonPath)
        {
            // [1] Parse project json file
            // [2] Go through Component list
            // [3] Parse each ComponentData.component.json
           
            if (!File.Exists(projectJsonPath))
                return;

            string projectRoot = Path.GetDirectoryName(projectJsonPath);
            AVM.DDP.MetaAvmProject avmProj = new AVM.DDP.MetaAvmProject();

            if (File.Exists(projectJsonPath))
            {
                string sjson = "{}";
                using (StreamReader reader = new StreamReader(projectJsonPath))
                {
                    sjson = reader.ReadToEnd();
                    avmProj = JsonConvert.DeserializeObject<AVM.DDP.MetaAvmProject>(sjson);
                }
            }

            List<string> cadFileDirs = FindComponentCreoFiles(avmProj.Project.Components, projectRoot);
            if (cadFileDirs.Any())
            {
                using (StreamWriter writer = new StreamWriter(Path.Combine(projectRoot, "cadmanifest.txt")))
                {
                    foreach (string dir in cadFileDirs)
                    {
                        writer.WriteLine(dir);
                    }
                }
            }
        }

        public static List<string> FindComponentCreoFiles(List<AVM.DDP.MetaAvmProject.Component> avmComponents,
                                                          string projectRoot)
        {
            List<string> cadFileDirs = new List<string>();
            foreach (AVM.DDP.MetaAvmProject.Component component in avmComponents)
            {
                string cadModelPath = component.modelpath.TrimStart('/');      // Path.GetDirectoryName(component.modelpath);
                string componentModelPath = Path.Combine(projectRoot, cadModelPath);
                if (Path.GetFileName(cadModelPath).Contains(".json"))
                    cadModelPath = Path.GetDirectoryName(cadModelPath);

                if (File.Exists(componentModelPath))
                {
                    AVM.Component acimport = AVM.Component.DeserializeFromFile(componentModelPath);
                    foreach (AVM.Feature feature in acimport.Features.Where(cm => cm is AVM.META.CADModel))
                    {
                        AVM.META.CADModel acimport_cadModel = (AVM.META.CADModel)feature;
                        if (String.IsNullOrEmpty(acimport_cadModel.FileFormat))
                        {
                            cadFileDirs.Add(Path.Combine(cadModelPath, acimport_cadModel.Location));
                            break;
                        }
                    }
                }
            }

            return cadFileDirs;
        }
    }
}
