using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using GME.MGA;
using Cyber = ISIS.GME.Dsml.CyberComposition.Interfaces;
using CyberClasses = ISIS.GME.Dsml.CyberComposition.Classes;

namespace CyberModelUtil
{
    public class ComponentLister
    {
        public static HashSet<Cyber.ModelicaComponent> getCyberComponentSet(Cyber.RootFolder CyberRootFolder)
        {

            HashSet<Cyber.ModelicaComponent> CyberComponentSet = new HashSet<Cyber.ModelicaComponent>();

            foreach (Cyber.LibraryFolder childLibraryFolder in CyberRootFolder.Children.LibraryFolderCollection)
            {
                CyberComponentSet.UnionWith(getCyberComponentSet(childLibraryFolder));
            }

            foreach (Cyber.Components childComponentFolder in CyberRootFolder.Children.ComponentsCollection)
            {
                CyberComponentSet.UnionWith(getCyberComponentSet(childComponentFolder));
            }

            return CyberComponentSet;

        }

        public static HashSet<Cyber.ModelicaComponent> getCyberComponentSet(Cyber.LibraryFolder CyberLibraryFolder)
        {

            HashSet<Cyber.ModelicaComponent> CyberComponentSet = new HashSet<Cyber.ModelicaComponent>();

            // A LibraryFolder has self-containment relationship
            foreach (Cyber.LibraryFolder libFolder in CyberLibraryFolder.Children.LibraryFolderCollection)
            {
                CyberComponentSet.UnionWith(getCyberComponentSet(libFolder));
            }

            foreach (Cyber.ModelicaComponent component in CyberLibraryFolder.Children.ModelicaComponentCollection)
            {
                CyberComponentSet.Add(component);
            }

            return CyberComponentSet;
        }

        public static HashSet<Cyber.ModelicaComponent> getCyberComponentSet(Cyber.Components CyberComponents)
        {

            HashSet<Cyber.ModelicaComponent> CyberComponentSet = new HashSet<Cyber.ModelicaComponent>();

            foreach (Cyber.ModelicaComponent childComponent in CyberComponents.Children.ModelicaComponentCollection)
            {
                CyberComponentSet.Add(childComponent);
            }

            return CyberComponentSet;
        }

        public static HashSet<Cyber.ModelicaComponent> getCyberComponentSet(Cyber.ModelicaComponent CyberComponent)
        {
            HashSet<Cyber.ModelicaComponent> CyberComponentSet = new HashSet<Cyber.ModelicaComponent>();
            CyberComponentSet.Add(CyberComponent);
            return CyberComponentSet;
        }

        public static HashSet<Cyber.ModelicaComponent> getCyberComponentSet(IMgaObject iMgaObject)
        {
            //GME.CSharp.GMEConsole console = GME.CSharp.GMEConsole.CreateFromProject(iMgaObject.Project);
            //console.Info.WriteLine("getCyberComponentSet: iMgaObject Overload!");

            if (iMgaObject == null) return new HashSet<Cyber.ModelicaComponent>();

            string metaName = iMgaObject.MetaBase.Name;

            HashSet<Cyber.ModelicaComponent> CyberComponentSet = null;
            if (metaName == "Components")
            {

                CyberComponentSet = getCyberComponentSet(CyberClasses.Components.Cast(iMgaObject));

            }
            else if (metaName == "LibraryFolder")
            {

                CyberComponentSet = getCyberComponentSet(CyberClasses.LibraryFolder.Cast(iMgaObject));

            }
            else if (metaName == "SimulinkWrapper" || metaName == "SignalFlowWrapper")
            {

                //console.Info.WriteLine("getCyberComponentSet: A ModelicaComponent! ");
                CyberComponentSet = getCyberComponentSet(CyberClasses.ModelicaComponent.Cast(iMgaObject));

            }
            else if (metaName == "RootFolder")
            {

                CyberComponentSet = getCyberComponentSet(CyberClasses.RootFolder.GetRootFolder(iMgaObject.Project));
            }

            return CyberComponentSet;
        }
    }
}
