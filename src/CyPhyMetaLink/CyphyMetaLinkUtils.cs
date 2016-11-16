using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyMLClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using GME.MGA;
using System.IO;
using Newtonsoft.Json;
using System.Xml.XPath;
using META;

namespace CyPhyMetaLink
{
    public static class CyphyMetaLinkUtils
    {
        public static bool IsComponent(MgaObject obj)
        {
            return obj.MetaBase.Name == "Component";
        }
        public static bool IsComponentRef(MgaObject obj)
        {
            return obj.MetaBase.Name == "ComponentRef";
        }

        public static bool IsValidComponentRef(MgaObject obj)
        {
            return obj.MetaBase.Name == "ComponentRef" && ((MgaReference)obj).Referred != null
                && ((MgaReference)obj).Referred.Status == (int)objectstatus_enum.OBJECT_EXISTS && ((MgaReference)obj).Referred.Meta.Name == "Component";
        }

        public static void SetOrCreateXmlAttribute(XPathNavigator node, string localName, string namespaceURI, string value)
        {
            if (node.MoveToAttribute(localName, namespaceURI))
            {
                node.SetValue(value);
                node.MoveToParent();
            }
            else
            {
                node.CreateAttribute("", localName, namespaceURI, value);
            }
        }

        public static void SetCADModelTypesFromFilenames(CyPhyML.Component c)
        {
            foreach (CyPhyML.CADModel cadModel in c.Children.CADModelCollection)
            {
                List<ISIS.GME.Common.Interfaces.FCO> resources = new List<ISIS.GME.Common.Interfaces.FCO>();
                foreach (var usesResource in cadModel.DstConnections.UsesResourceCollection)
                {
                    resources.Add(usesResource.DstEnd);
                }
                foreach (var usesResource in cadModel.SrcConnections.UsesResourceCollection)
                {
                    resources.Add(usesResource.SrcEnd);
                }
                foreach (var resource in resources.Where(f => f.Impl.MetaBase.Name == "Resource")
                    .Select(f => CyPhyMLClasses.Resource.Cast(f.Impl)))
                {
                    if (resource.Attributes.Path.EndsWith(".prt") || resource.Attributes.Path.EndsWith(".PRT"))
                    {
                        cadModel.Attributes.FileType = CyPhyMLClasses.CADModel.AttributesClass.FileType_enum.Part;
                    }
                    if (resource.Attributes.Path.EndsWith(".asm") || resource.Attributes.Path.EndsWith(".ASM"))
                    {
                        cadModel.Attributes.FileType = CyPhyMLClasses.CADModel.AttributesClass.FileType_enum.Assembly;
                    }
                }
            }
        }

        public static String GetResourceID(CyPhyML.CADModel cadModel)
        {
            // Starting with the DomainModel object, we'll find a Resource object to which it has a relation.
            // Then we'll return the path of that Resource object.
            List<CyPhyML.Resource> l_Resources = new List<CyPhyML.Resource>();
            foreach (CyPhyML.UsesResource ur in cadModel.DstConnections.UsesResourceCollection)
                l_Resources.Add(ur.DstEnds.Resource);
            foreach (CyPhyML.UsesResource ur in cadModel.SrcConnections.UsesResourceCollection)
                l_Resources.Add(ur.SrcEnds.Resource);

            // If no Resources found, return false.
            // If at least one Resource was found, return the path of the first.
            if (l_Resources.Count == 0)
            {
                return "";
            }
            else
            {
                return l_Resources.FirstOrDefault().Attributes.ID;
            }
        }

        static bool stringRoughlyEqual(string a, string b)
        {
            if (string.IsNullOrWhiteSpace(a) && string.IsNullOrWhiteSpace(b))
            {
                return true;
            }
            if (string.IsNullOrWhiteSpace(a) || string.IsNullOrWhiteSpace(b))
            {
                return false;
            }
            return a.Equals(b);
        }

        public static string GetProjectDir(MgaProject project)
        {
            string workingDir = Path.GetTempPath();
            if (project.ProjectConnStr.StartsWith("MGA="))
            {
                workingDir = Path.GetDirectoryName(project.ProjectConnStr.Substring("MGA=".Length));
            }
            return workingDir;
        }

        public static CyPhyML.Components GetImportedComponentsFolder(MgaProject project)
        {
            var rf = CyPhyMLClasses.RootFolder.GetRootFolder(project);
            return rf.Children.ComponentsCollection.Where(cs => cs.Name == CyPhyComponentImporter.CyPhyComponentImporterInterpreter.ImportedComponentsFolderName).FirstOrDefault();
        }

        public static String CleanPath(String path)
        {
            return path.Replace('/', '\\');
        }

        public static CyPhyML.Component CopyComponent(CyPhyML.Component original, bool createNewMgaComponent)
        {
            // This will fixup any errors/sync issues
            string componentDirectory = original.GetDirectoryPath();
            if (String.IsNullOrWhiteSpace(componentDirectory))
            {
                throw new ApplicationException(String.Format("Could not find component definition"));
            }

            CyPhyML.Component newComponent;
            if (createNewMgaComponent)
            {
                if (original.ParentContainer.Impl.ObjType == GME.MGA.Meta.objtype_enum.OBJTYPE_FOLDER)
                {
                    newComponent = CyPhyMLClasses.Component.Cast(((MgaFolder)original.ParentContainer.Impl).CopyFCODisp((MgaFCO)original.Impl));
                }
                else
                {
                    newComponent = CyPhyMLClasses.Component.Cast(((MgaModel)original.ParentContainer.Impl).CopyFCODisp((MgaFCO)original.Impl, ((MgaModel)original.Impl).MetaRole));
                }
                string basename = newComponent.Name;
                newComponent.Name = basename + "_new";
                HashSet<string> childNames = new HashSet<string>();
                foreach (var child in newComponent.ParentContainer.AllChildren)
                {
                    if (child.ID != newComponent.ID)
                    {
                        childNames.Add(child.Name);
                    }
                }
                int i = 2;
                while (childNames.Contains(newComponent.Name))
                {
                    newComponent.Name = basename + "_new_" + i;
                    i++;
                    if (i > 100)
                    {
                        break;
                    }
                }
                newComponent.Attributes.AVMID = "";
                newComponent.Attributes.Path = "";
                // this will also assign AVMID and Path, add to project manifest, etc
                string newModelpathDirectory = newComponent.GetDirectoryPath();

                CopyDirectory(Path.Combine(GetProjectDir(original.Impl.Project), componentDirectory), Path.Combine(GetProjectDir(original.Impl.Project), newModelpathDirectory));
            }
            else
            {
                newComponent = original;
            }

            return newComponent;
        }

        public static void CopyDirectory(string sourcePath, string destPath)
        {
            if (!Directory.Exists(destPath))
            {
                Directory.CreateDirectory(destPath);
            }

            foreach (string file in Directory.GetFiles(sourcePath))
            {
                string dest = Path.Combine(destPath, Path.GetFileName(file));
                File.Copy(file, dest);
            }

            foreach (string folder in Directory.GetDirectories(sourcePath))
            {
                string dest = Path.Combine(destPath, Path.GetFileName(folder));
                CopyDirectory(folder, dest);
            }
        }

        public static CyPhyML.Connector FindConnector(CyPhyML.ComponentRef compref, string guid)
        {
            if (compref.Referred != null && compref.Referred.Component != null)
            {
                CyPhyML.Component comp = compref.Referred.Component;
                foreach (var connector in comp.Children.ConnectorCollection)
                {
                    if (connector.Guid.ToString() == guid)
                    {
                        return connector;
                    }
                }
            }
            return null;
        }

        public static CyPhyML.Connector FindConnector(CyPhyML.Component component, string guid)
        {
            foreach (var connector in component.Children.ConnectorCollection)
            {
                if (connector.Guid.ToString() == guid)
                {
                    return connector;
                }
            }
            return null;
        }

        public static CyPhyML.CADDatum FindDatum(CyPhyML.CADModel cadmodel, string name)
        {
            foreach (var datum in cadmodel.Children.CADDatumCollection)
            {
                if (datum.Attributes.DatumName.ToLower() == name.ToLower())
                {
                    return datum;
                }
            }
            return null;
        }

        public static CyPhyML.ComponentRef GetComponentRefInAssemblyById(CyPhyML.ComponentAssembly root, string amalgamatedId)
        {
            List<string> ids = new List<string>();
            int ind = 0;
            int guid_length = new Guid().ToString("B").Length;
            for (; ind < amalgamatedId.Length; ind += guid_length)
            {
                if (amalgamatedId.Length < ind + guid_length)
                {
                    return null;
                }
                ids.Add(amalgamatedId.Substring(ind, guid_length));
            }
            CyPhyML.ComponentAssembly ca = root;
            CyPhyML.ComponentRef componentRef = null;
            foreach (string componentRefInstanceGuid in ids)
            {
                if (ca == null)
                {
                    return null;
                }
                componentRef = GetComponentRefInAssemblyByIdWithoutFollowingReferences(ca, componentRefInstanceGuid);
                if (componentRef == null)
                {
                    return null;
                }
                if (componentRef.AllReferred is CyPhyML.ComponentAssembly)
                {
                    ca = (CyPhyML.ComponentAssembly)componentRef.AllReferred;
                }
                else
                {
                    ca = null;
                }
            }
            return componentRef;
        }

        public static CyPhyML.ComponentRef GetComponentRefInAssemblyByIdWithoutFollowingReferences(CyPhyML.ComponentAssembly root, string instanceId)
        {
            foreach (CyPhyML.ComponentAssembly assembly in VisitAllComponentAssembly(root))
            {
                var compRef = assembly.Children.ComponentRefCollection.Where(c => c.Attributes.InstanceGUID == instanceId).FirstOrDefault();
                if (compRef != null)
                {
                    return compRef;
                }
            }
            return null;
        }

        private static IEnumerable<CyPhyML.ComponentAssembly> VisitAllComponentAssembly(CyPhyML.ComponentAssembly root)
        {
            Queue<CyPhyML.ComponentAssembly> assemblyQueue = new Queue<CyPhyML.ComponentAssembly>();
            assemblyQueue.Enqueue(root);
            while (assemblyQueue.Count > 0)
            {
                var assembly = assemblyQueue.Dequeue();
                yield return assembly;
                foreach (var childAssemblies in assembly.Children.ComponentAssemblyCollection)
                {
                    assemblyQueue.Enqueue(childAssemblies);
                }
            }
        }

        private static IEnumerable<CyPhyML.Components> VisitComponentsFolders(MgaProject project)
        {
            var rf = CyPhyMLClasses.RootFolder.GetRootFolder(project);
            Queue<CyPhyML.Components> componentsQueue = new Queue<CyPhyML.Components>();
            foreach (var components in rf.Children.ComponentsCollection)
            {
                componentsQueue.Enqueue(components);
            }
            while (componentsQueue.Count > 0)
            {
                var components = componentsQueue.Dequeue();
                yield return components;
                foreach (var childComponents in components.Children.ComponentsCollection)
                {
                    componentsQueue.Enqueue(childComponents);
                }
            }
        }

        public static CyPhyML.Component GetComponentByAvmId(MgaProject project, string avmId)
        {
            foreach (var components in VisitComponentsFolders(project))
            {
                foreach (var component in components.Children.ComponentCollection)
                {
                    if (component.Attributes.AVMID == avmId)
                    {
                        return component;
                    }
                }
            }
            return null;
        }

        public static CyPhyML.Component FindAVMComponent(MgaProject project, string avmid, ISIS.GME.Dsml.CyPhyML.Interfaces.Components root)
        {
            if (root == null)
            {
                var rf = CyPhyMLClasses.RootFolder.GetRootFolder(project);
                foreach (var components in rf.Children.ComponentsCollection)
                {
                    CyPhyML.Component comp = FindAVMComponent(project, avmid, components);
                    if (comp != null)
                    {
                        return comp;
                    }
                }
            }
            else
            {
                foreach (var components in root.Children.ComponentsCollection)
                {
                    CyPhyML.Component comp = FindAVMComponent(project, avmid, components);
                    if (comp != null)
                    {
                        return comp;
                    }
                }
                foreach (var component in root.Children.ComponentCollection)
                {
                    if (component.Attributes.AVMID == avmid)
                    {
                        return component;
                    }
                }
            }
            return null;
        }

        /// <summary>
        /// Gets ComponentAssemblys that contain component directly, or through references to ComponentAssemblys
        /// </summary>
        public static IEnumerable<CyPhyML.ComponentAssembly> GetContainingAssemblies(CyPhyML.Component component)
        {
            Queue<CyPhyML.ComponentAssembly> containingAssemblies = new Queue<CyPhyML.ComponentAssembly>();
            foreach (CyPhyML.ComponentRef compref in component.ReferencedBy.ComponentRef)
            {
                if (compref.ParentContainer is CyPhyML.ComponentAssembly)
                {
                    containingAssemblies.Enqueue(compref.ParentContainer as CyPhyML.ComponentAssembly);
                }
            }
            while (containingAssemblies.Count > 0)
            {
                CyPhyML.ComponentAssembly asm = containingAssemblies.Dequeue();
                yield return asm;
                foreach (CyPhyML.ComponentRef compref in asm.ReferencedBy.ComponentRef)
                {
                    if (compref.ParentContainer is CyPhyML.ComponentAssembly)
                    {
                        containingAssemblies.Enqueue(compref.ParentContainer as CyPhyML.ComponentAssembly);
                    }
                }
                if (asm.ParentContainer is CyPhyML.ComponentAssembly)
                {
                    containingAssemblies.Enqueue(asm.ParentContainer as CyPhyML.ComponentAssembly);
                }
            }
        }

        /// <summary>
        /// Gets assembly and ComponentAssemblys that contain assembly directly, or through references to ComponentAssemblys
        /// </summary>
        public static IEnumerable<CyPhyML.ComponentAssembly> GetContainingAssemblies(CyPhyML.ComponentAssembly assembly)
        {
            Queue<CyPhyML.ComponentAssembly> containingAssemblies = new Queue<CyPhyML.ComponentAssembly>();
            containingAssemblies.Enqueue(assembly);
            while (containingAssemblies.Count > 0)
            {
                CyPhyML.ComponentAssembly asm = containingAssemblies.Dequeue();
                yield return asm;
                foreach (CyPhyML.ComponentRef compref in asm.ReferencedBy.ComponentRef)
                {
                    if (compref.ParentContainer is CyPhyML.ComponentAssembly)
                    {
                        containingAssemblies.Enqueue(compref.ParentContainer as CyPhyML.ComponentAssembly);
                    }
                }
                if (asm.ParentContainer is CyPhyML.ComponentAssembly)
                {
                    containingAssemblies.Enqueue(asm.ParentContainer as CyPhyML.ComponentAssembly);
                }
            }
        }

        public static CyPhyML.ComponentAssembly GetComponentAssemblyByGuid(MgaProject project, string componentAssemblyGuid)
        {
            CyPhyML.ComponentAssembly topicAssembly = null;
            foreach (CyPhyML.ComponentAssemblies assemblies in VisitComponentAssemblies(project))
            {
                foreach (CyPhyML.ComponentAssembly assembly in assemblies.Children.ComponentAssemblyCollection)
                {
                    if (assembly.Guid.ToString().Contains(componentAssemblyGuid))
                    {
                        topicAssembly = assembly;
                        break;
                    }
                }
                if (topicAssembly != null)
                {
                    break;
                }
            }
            return topicAssembly;
        }

        public static string GetComponentNameByAnyId(MgaProject project, string id)
        {
            IEnumerable<CyPhyML.ComponentAssemblies> assemblyList = VisitComponentAssemblies(project);
            foreach (CyPhyML.ComponentAssemblies assemblies in assemblyList)
            {
                foreach (CyPhyML.ComponentAssembly assembly in assemblies.Children.ComponentAssemblyCollection)
                {
                    if (assembly.Guid.ToString() == id)
                    {
                        return assembly.Name;
                    }

                    foreach (var componentref in assembly.Children.ComponentRefCollection)
                    {
                        if (componentref.Guid.ToString() == id || componentref.Attributes.InstanceGUID == id)
                        {
                            return componentref.Name;
                        }
                    }
                }
            }

            return null;
        }

        private static IEnumerable<CyPhyML.ComponentAssemblies> VisitComponentAssemblies(MgaProject project)
        {
            var rf = CyPhyMLClasses.RootFolder.GetRootFolder(project);
            Queue<CyPhyML.ComponentAssemblies> assembliesQueue = new Queue<CyPhyML.ComponentAssemblies>();
            foreach (var assemblies in rf.Children.ComponentAssembliesCollection)
            {
                assembliesQueue.Enqueue(assemblies);
            }
            while (assembliesQueue.Count > 0)
            {
                var assemblies = assembliesQueue.Dequeue();
                yield return assemblies;
                foreach (var childAssemblies in assemblies.Children.ComponentAssembliesCollection)
                {
                    assembliesQueue.Enqueue(childAssemblies);
                }
            }
        }

        public static CyPhyML.CADModel FindCADModelObject(CyPhyML.Component component)
        {
            foreach (CyPhyML.CADModel item in component.Children.CADModelCollection)
            {
                if (item.Attributes.FileFormat.ToString() == "Creo")
                {
                    return item;
                }
            }
            return null;
        }

        private static void CollectComponents(HashSet<CyPhyML.Component> complist, CyPhyML.ComponentAssembly assembly)
        {
            // Get all the components from here
            foreach (var compref in assembly.Children.ComponentRefCollection)
            {
                if (compref.AllReferred is CyPhyML.Component)
                {
                    complist.Add(compref.AllReferred as CyPhyML.Component);
                }
                else if (compref.AllReferred is CyPhyML.ComponentAssembly)
                {
                    CollectComponents(complist, compref.AllReferred as CyPhyML.ComponentAssembly);
                }
            }
        }

        // Collects all the components this assembly contains, including multi-level hierarchies
        public static HashSet<CyPhyML.Component> CollectComponentsRecursive(CyPhyML.ComponentAssembly assembly)
        {
            HashSet<CyPhyML.Component> complist = new HashSet<CyPhyML.Component>();
            CollectComponents(complist, assembly);
            return complist;
        }

        private static void CollectComponentRefs(List<CyPhyML.ComponentRef> complist, CyPhyML.ComponentAssembly assembly)
        {
            // Get all the components from here
            foreach (var compref in assembly.Children.ComponentRefCollection)
            {
                if (compref.AllReferred is CyPhyML.Component)
                {
                    complist.Add(compref);
                }
                else if (compref.AllReferred is CyPhyML.ComponentAssembly)
                {
                    CollectComponentRefs(complist, compref.AllReferred as CyPhyML.ComponentAssembly);
                }
            }
        }

        // Collects all the component references this assembly contains, including multi-level hierarchies
        public static List<CyPhyML.ComponentRef> CollectComponentRefsRecursive(CyPhyML.ComponentAssembly assembly)
        {
            List<CyPhyML.ComponentRef> complist = new List<CyPhyML.ComponentRef>();
            CollectComponentRefs(complist, assembly);
            return complist;
        }

        public static bool IsCADResource(CyPhyML.Resource res)
        {
            return Path.GetExtension(res.Attributes.Path).ToLower() == ".prt" || Path.GetExtension(res.Attributes.Path).ToLower() == ".asm";
        }

        public static void CollectInstanceGUIDs(Dictionary<string, CyPhyML.ComponentRef> dict, string prefix, CyPhyML.ComponentAssembly assembly)
        {
            foreach (CyPhyML.ComponentRef cref in assembly.Children.ComponentRefCollection)
            {
                if (cref.AllReferred != null)
                {
                    if (cref.AllReferred is CyPhyML.Component)
                    {
                        dict.Add(prefix + "{" + cref.Guid + "}", cref);
                    }
                    else if (cref.AllReferred is CyPhyML.ComponentAssembly)
                    {
                        CollectInstanceGUIDs(dict, prefix + "{" + cref.Guid + "}", cref.AllReferred as CyPhyML.ComponentAssembly);
                    }
                }
            }
        }
    }
}
