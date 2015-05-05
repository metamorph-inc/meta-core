using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using Newtonsoft.Json;

namespace CyPhy2CAD_CSharp.DataRep
{
    public class CADContainer
    {
        public Dictionary<string, DataRep.CADAssembly> assemblies { get; set; }
        public List<DataRep.CADComponent> orphans { get; set; }
        public string ConfigID { get; set; }
        //public string OutputDirectory { get; set; }
        public Dictionary<string, string> GMEIDToJointIDMap { get; set; }

        public Dictionary<string, CadManifestObject> CADManifestList { get; set; }


        public CADContainer(string configid)
        {
            assemblies = new Dictionary<string, DataRep.CADAssembly>();
            orphans = new List<CADComponent>();
            ConfigID = configid;
            CADManifestList = new Dictionary<string, CadManifestObject>();
            GMEIDToJointIDMap = new Dictionary<string, string>();
        }

        public CAD.AssembliesType ToCADXMLOutput(TestBenchModel.TestBenchBase tb, bool metalink = false)
        {
            CAD.AssembliesType outputAssembliesType = new CAD.AssembliesType();
            outputAssembliesType.VersionInfo = "";

            List<CAD.AssemblyType> asmlist = new List<CAD.AssemblyType>();
            foreach (var item in assemblies.Values)
            {
                CAD.AssemblyType assembly = item.ToCADXMLOutput(tb);
                asmlist.Add(assembly);
            }

            if (asmlist.Count > 1 && metalink)
            {
                Logger.Instance.AddLogMessage("There's more than one unconnected assembly in the model. Meta-Link may not work with this model.", Severity.Warning);
            }

            if (orphans.Any())
            {
                if (metalink)
                {
                    CAD.AssemblyType topAssembly = asmlist.First();
                    List<CAD.CADComponentType> cadcomponentlist = topAssembly.CADComponent.CADComponent.ToList();
                    foreach (var orphan in orphans)
                    {
                        cadcomponentlist.Add(orphan.ToCADXMLOutput(tb.GetRepresentation(orphan)));
                    }
                    topAssembly.CADComponent.CADComponent = cadcomponentlist.ToArray();
                }
                else
                {
                    List<CAD.CADComponentType> orphanlist = new List<CAD.CADComponentType>();
                    CAD.UnassembledComponentsType orphancomponentParent = new CAD.UnassembledComponentsType();
                    orphancomponentParent._id = UtilityHelpers.MakeUdmID();

                    foreach (var orphan in orphans)
                    {
                        orphanlist.Add(orphan.ToCADXMLOutput(tb.GetRepresentation(orphan)));
                    }
                    orphancomponentParent.CADComponent = orphanlist.ToArray();
                    outputAssembliesType.UnassembledComponents = orphancomponentParent;
                }
            }

            outputAssembliesType.Assembly = asmlist.ToArray();

            return outputAssembliesType;
        }

        /// <summary>
        /// Injects Meta-Link related data into a pre-existing structure
        /// </summary>
        /// <param name="assembly"></param>
        public void InjectMetaLinkData(CyPhy.ComponentAssembly assembly, MetaLinkData data)
        {
            // There's only one cadassembly in case of Meta-Link!!
            foreach (DataRep.CADAssembly a in assemblies.Values)
            {
                a.MetaLinkData = data;
            }
        }

        public void CollectDirectories(string projectDir, string outputDir)
        {
            foreach (var item in assemblies.Values)
            {
                item.CollectDirectories(CADManifestList,
                                                  projectDir,
                                                  outputDir);
            }
        }

        public List<string> GetRemoteCadFolders()
        {
            List<string> list = new List<string>();
            foreach (var item in CADManifestList.Values)
            {
                list.Add(item.RemoteMETAProPath);
            }

            return list;
        }

        public List<string> GetLocalCadFolders()
        {
            List<string> list = new List<string>();
            foreach (var item in CADManifestList.Values)
            {
                list.Add(item.LocalFolderPath);
            }

            return list;
        }


        /// <summary>
        /// Meta-Link feature: If there's no components in an assembly still need to add one empty root component
        /// </summary>
        /// <param name="?"></param>
        public void AddRootComponent(CyPhy.ComponentAssembly assembly)
        {
            CADAssembly a = new CADAssembly();
            a.Id = a.ConfigID = assembly.Guid.ToString();
            a.Name = a.DisplayName = assembly.Name;
            assemblies.Add(UtilityHelpers.CleanString2(assembly.Name), a);
        }

        // META-1882
        public bool IsAnalyzable()
        {
            return (assemblies.Count == 1 && orphans.Count == 0);
        }

        // Merge all components into one (the first) assembly
        // Meta-Link is using this for unconnected assemblies
        public void MergeAssemblies()
        {
            if (assemblies.Count <= 1) return;
            int i = 0;
            CADAssembly first = null;
            foreach (var assembly in assemblies.Values)
            {
                if (i == 0)
                {
                    first = assembly;
                    i++;
                    continue;
                }
                else
                {
                    first.ChildComponents = first.ChildComponents.Union(assembly.ChildComponents).ToDictionary(k => k.Key, v => v.Value);
                    first.ChildEdges.AddRange(assembly.ChildEdges);
                }
            }
            assemblies.Clear();
            assemblies.Add(first.Id, first);

        }

        // snyako@isis.vanderbilt.edu: This cod eis not used anymore
        /*
        public Multibody.ModelAssembly ToMultibodyOutput()
        {
            if (assemblies.Count > 0)
            {
                return assemblies.First().Value.ToMultibodyOutput();
            }
            else
                return null;
        }
        */

        public string GetJointIDFromGMEID(string gmeID)
        {
            if (GMEIDToJointIDMap.ContainsKey(gmeID))
                return GMEIDToJointIDMap[gmeID];
            else
                return null;
        }

        /*
        // META-1544: removed
        public Dictionary<string, string> CreateMaterialList()
        {
            Dictionary<string, string> MaterialList = new Dictionary<string, string>();
            foreach (var item in assemblies)
            {
                foreach (var subitem in item.Value.ChildComponents)
                {
                    if (!MaterialList.ContainsKey(subitem.Value.MaterialID))
                        MaterialList.Add(subitem.Value.MaterialID, subitem.Value.MaterialName);
                }

                foreach (var subitem in item.Value.ChildSize2FitComponents)
                {
                    if (!MaterialList.ContainsKey(subitem.Value.MaterialID))
                        MaterialList.Add(subitem.Value.MaterialID, subitem.Value.MaterialName);
                }
            }

            foreach (var item in orphans)
            {
                if (!MaterialList.ContainsKey(item.MaterialID))
                    MaterialList.Add(item.MaterialID, item.MaterialName);
            }
            return MaterialList;
        }
        */

        public override string ToString()
        {
            StringBuilder sbuilder = new StringBuilder();

            foreach (var item in assemblies.Values)
            {
                sbuilder.Append(item.ToString()).AppendLine();
            }
            
            return sbuilder.ToString();
        }

        public string ToGraphviz(bool directed = false)
        {
            StringBuilder sbuilder = new StringBuilder();

            sbuilder.Append("digraph G {\n").AppendLine();
	        sbuilder.Append("node[style=filled];\n").AppendLine();

            foreach (var item in assemblies.Values)
            {
                sbuilder.Append(item.ToGraphViz(directed)).AppendLine();
            }
            foreach (var item in orphans)
            {
                sbuilder.Append(item.ToGraphViz(directed)).AppendLine();
            }
            sbuilder.Append("}\n").AppendLine();

            return sbuilder.ToString();
        }
    }
}
