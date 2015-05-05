using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

using CyPhyGUIs;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2Modelica_v2
{
    public static class FolderFilter
    {
        public static void PopulateWithFilteredNodes(ref FolderTreeNode folder)
        {
            var components = folder.Folder;
            foreach (var component in components.Children.ComponentCollection
                .Where(c => (c.Impl as GME.MGA.IMgaModel).GetChildrenOfKind(typeof(CyPhy.ModelicaModel).Name).Count +
                            (c.Impl as GME.MGA.IMgaModel).GetChildrenOfKind(typeof(CyPhy.CyberModel).Name).Count > 0))
            {
                folder.Children.Add(new ComponentTreeNode(component));
            }

            foreach (var subComponents in components.Children.ComponentsCollection.Where(f => FolderHasModelicaComponents(f)))
            {
                var subFolder = new FolderTreeNode(subComponents);
                folder.Children.Add(subFolder);
                PopulateWithFilteredNodes(ref subFolder);
            }
        }

        public static bool FolderHasModelicaComponents(CyPhy.Components folder)
        {
            foreach (var component in folder.Children.ComponentCollection)
            {
                if ((component.Impl as GME.MGA.IMgaModel).GetChildrenOfKind(typeof(CyPhy.ModelicaModel).Name).Count +
                    (component.Impl as GME.MGA.IMgaModel).GetChildrenOfKind(typeof(CyPhy.CyberModel).Name).Count > 0)
                {
                    return true;
                }
            }

            bool subFolderHasModelicaComponents = false;

            foreach (var subFolder in folder.Children.ComponentsCollection)
            {
                if (FolderHasModelicaComponents(subFolder))
                {
                    subFolderHasModelicaComponents = true;
                    break;
                }
            }

            return subFolderHasModelicaComponents;
        }
    }

    public class TreeNodeBase
    {
        public string Name;
    }

    public class ComponentTreeNode : TreeNodeBase
    {
        public CyPhy.Component Component { get; set; }

        public ComponentTreeNode(CyPhy.Component component)
        {
            this.Component = component;
            this.Name = component.Name;
        }
    }

    public class FolderTreeNode : TreeNodeBase
    {
        public CyPhy.Components Folder { get; set; }
        public List<TreeNodeBase> Children { get; set; }

        public FolderTreeNode(CyPhy.Components folder)
        {
            this.Folder = folder;
            this.Children = new List<TreeNodeBase>();
            this.Name = folder.Name;
        }
    }
}