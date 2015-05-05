using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using GME.MGA;
using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyMLClasses = ISIS.GME.Dsml.CyPhyML.Classes;


namespace CyPhy2ComponentModel {

    public class ComponentLister {

        public static HashSet<CyPhyML.Component> getCyPhyMLComponentSet(CyPhyML.RootFolder cyPhyMLRootFolder) {

            HashSet<CyPhyML.Component> cyPhyMLComponentSet = new HashSet<CyPhyML.Component>();

            foreach (CyPhyML.Components childComponentsFolder in cyPhyMLRootFolder.Children.ComponentsCollection) {
                cyPhyMLComponentSet.UnionWith(getCyPhyMLComponentSet(childComponentsFolder));
            }

            foreach (CyPhyML.ComponentAssemblies childComponentAssembliesFolder in cyPhyMLRootFolder.Children.ComponentAssembliesCollection) {
                cyPhyMLComponentSet.UnionWith(getCyPhyMLComponentSet(childComponentAssembliesFolder));
            }

            return cyPhyMLComponentSet;

        }

        public static HashSet<CyPhyML.Component> getCyPhyMLComponentSet(CyPhyML.TestBench cyPhyMLTestBench) {

            HashSet<CyPhyML.Component> cyPhyMLComponentSet = new HashSet<CyPhyML.Component>();

            foreach( CyPhyML.TopLevelSystemUnderTest cyPhyMLTopLevelSystemUnderTest in cyPhyMLTestBench.Children.TopLevelSystemUnderTestCollection ) {
                CyPhyML.DesignEntity cyPhyMLDesignEntity = cyPhyMLTopLevelSystemUnderTest.Referred as CyPhyML.DesignEntity;
                if (cyPhyMLDesignEntity is CyPhyML.ComponentAssembly) {
                    cyPhyMLComponentSet.UnionWith(getCyPhyMLComponentSet(cyPhyMLDesignEntity as CyPhyML.ComponentAssembly));
                }
            }

            foreach (CyPhyML.ComponentAssembly cyPhyMLComponentAssembly in cyPhyMLTestBench.Children.ComponentAssemblyCollection) {
                if ((cyPhyMLComponentAssembly.Impl as MgaFCO).MetaRole.Name != "TopLevelSystemUnderTest") continue;
                cyPhyMLComponentSet.UnionWith(getCyPhyMLComponentSet(cyPhyMLComponentAssembly));
            }

            return cyPhyMLComponentSet;
        }

        public static HashSet<CyPhyML.Component> getCyPhyMLComponentSet(CyPhyML.Components cyPhyMLComponents) {

            HashSet<CyPhyML.Component> cyPhyMLComponentSet = new HashSet<CyPhyML.Component>();

            foreach (CyPhyML.Components childComponentsFolder in cyPhyMLComponents.Children.ComponentsCollection) {
                cyPhyMLComponentSet.UnionWith(getCyPhyMLComponentSet(childComponentsFolder));
            }

            foreach (CyPhyML.ComponentAssemblies childComponentAssembliesFolder in cyPhyMLComponents.Children.ComponentAssembliesCollection) {
                cyPhyMLComponentSet.UnionWith(getCyPhyMLComponentSet(childComponentAssembliesFolder));
            }

            foreach (CyPhyML.Component childComponent in cyPhyMLComponents.Children.ComponentCollection) {
                cyPhyMLComponentSet.Add(childComponent);
            }

            return cyPhyMLComponentSet;
        }

        public static HashSet<CyPhyML.Component> getCyPhyMLComponentSet(CyPhyML.ComponentAssemblies cyPhyMLComponentAssemblies) {

            HashSet<CyPhyML.Component> cyPhyMLComponentSet = new HashSet<CyPhyML.Component>();

            foreach (CyPhyML.ComponentAssemblies childComponentAssembliesFolder in cyPhyMLComponentAssemblies.Children.ComponentAssembliesCollection) {
                cyPhyMLComponentSet.UnionWith(getCyPhyMLComponentSet(childComponentAssembliesFolder));
            }

            foreach (CyPhyML.ComponentAssembly childComponentAssemblyFolder in cyPhyMLComponentAssemblies.Children.ComponentAssemblyCollection) {
                cyPhyMLComponentSet.UnionWith(getCyPhyMLComponentSet(childComponentAssemblyFolder));
            }

            return cyPhyMLComponentSet;
        }

        public static HashSet<CyPhyML.Component> getCyPhyMLComponentSet(CyPhyML.ComponentAssembly cyPhyMLComponentAssembly) {

            HashSet<CyPhyML.Component> cyPhyMLComponentSet = new HashSet<CyPhyML.Component>();

            foreach (CyPhyML.ComponentAssembly childComponentAssemblyFolder in cyPhyMLComponentAssembly.Children.ComponentAssemblyCollection) {
                cyPhyMLComponentSet.UnionWith(getCyPhyMLComponentSet(childComponentAssemblyFolder));
            }

            foreach (CyPhyML.Component childComponent in cyPhyMLComponentAssembly.Children.ComponentCollection) {
                cyPhyMLComponentSet.Add(childComponent);
            }

            return cyPhyMLComponentSet;
        }

        public static HashSet<CyPhyML.Component> getCyPhyMLComponentSet(CyPhyML.Component cyPhyMLComponent) {
            HashSet<CyPhyML.Component> cyPhyMLComponentSet = new HashSet<CyPhyML.Component>();
            cyPhyMLComponentSet.Add(cyPhyMLComponent);
            return cyPhyMLComponentSet;
        }

        public static HashSet<CyPhyML.Component> getCyPhyMLComponentSet(IMgaObject iMgaObject) {

            if ( iMgaObject == null ) return new HashSet<CyPhyML.Component>();

            string metaName = iMgaObject.MetaBase.Name;

            HashSet<CyPhyML.Component> cyPhyMLComponentSet = null;
            if (metaName == "Component") {

                cyPhyMLComponentSet = new HashSet<CyPhyML.Component>();
                cyPhyMLComponentSet.Add(CyPhyMLClasses.Component.Cast(iMgaObject));

            } else if (metaName == "ComponentRef") {

                CyPhyML.ComponentRef cyPhyMLComponentRef = CyPhyMLClasses.ComponentRef.Cast(iMgaObject);
                cyPhyMLComponentSet = new HashSet<CyPhyML.Component>();
                cyPhyMLComponentSet.Add(cyPhyMLComponentRef.Referred as CyPhyML.Component);

            } else if (metaName == "ComponentAssembly") {

                cyPhyMLComponentSet = CyPhy2ComponentModel.ComponentLister.getCyPhyMLComponentSet(CyPhyMLClasses.ComponentAssembly.Cast(iMgaObject));

            } else if (metaName == "ComponentAssemblyRef") {

                CyPhyML.ComponentAssemblyRef cyPhyMLComponentAssemblyRef = CyPhyMLClasses.ComponentAssemblyRef.Cast(iMgaObject);
                cyPhyMLComponentSet = new HashSet<CyPhyML.Component>();
                cyPhyMLComponentSet = CyPhy2ComponentModel.ComponentLister.getCyPhyMLComponentSet(cyPhyMLComponentAssemblyRef.Referred as CyPhyML.ComponentAssembly);

            } else if (metaName == "TestBench") {

                cyPhyMLComponentSet = CyPhy2ComponentModel.ComponentLister.getCyPhyMLComponentSet(CyPhyMLClasses.TestBench.Cast(iMgaObject));

            } else if (metaName == "RootFolder") {

                cyPhyMLComponentSet = CyPhy2ComponentModel.ComponentLister.getCyPhyMLComponentSet(CyPhyMLClasses.RootFolder.GetRootFolder(iMgaObject.Project));

            }
            else {
                cyPhyMLComponentSet = new HashSet<CyPhyML.Component>();
            }

            return cyPhyMLComponentSet;
        }

    }
}
