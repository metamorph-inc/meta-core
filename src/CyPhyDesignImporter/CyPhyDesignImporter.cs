using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using CyPhyComponentImporter;
using GME.CSharp;
using GME.MGA;
using GME.MGA.Core;
using GME.MGA.Meta;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using ISIS.GME.Common.Interfaces;

namespace CyPhyDesignImporter
{
    public class AVMDesignImporter : AVM2CyPhyML.AVM2CyPhyMLBuilder
    {
        string projroot;

        public AVMDesignImporter(GMEConsole console, IMgaProject project, object messageConsoleParameter = null)
            : base(CyPhyClasses.RootFolder.GetRootFolder((MgaProject)project), messageConsoleParameter)
        {
            projroot = Path.GetDirectoryName(project.ProjectConnStr.Substring("MGA=".Length));
            init(true);
        }

        public Dictionary<string, CyPhy.Component> avmidComponentMap
        {
            get
            {
                // TODO memoize
                CyPhy.RootFolder rootFolder = ISIS.GME.Common.Utils.CreateObject<CyPhyClasses.RootFolder>(project.RootFolder as MgaObject);
                return CyPhyComponentImporterInterpreter.getCyPhyMLComponentDictionary_ByAVMID(rootFolder);
            }
        }

        public Model[] ImportFiles(string[] fileNames, DesignImportMode mode = AVMDesignImporter.DesignImportMode.CREATE_DS)
        {
            List<Model> ret = new List<Model>();
            CyPhy.RootFolder rootFolder = ISIS.GME.Common.Utils.CreateObject<CyPhyClasses.RootFolder>(project.RootFolder as MgaObject);
            Dictionary<string, CyPhy.Component> avmidComponentMap = CyPhyComponentImporterInterpreter.getCyPhyMLComponentDictionary_ByAVMID(rootFolder);

            foreach (var inputFilePath in fileNames)
            {
                var container = ImportFile(inputFilePath, mode);
                ret.Add(container);
            }
            return ret.ToArray();
        }

        public Model ImportFile(string inputFilePath, DesignImportMode mode = DesignImportMode.CREATE_DS)
        {
            writeMessage(String.Format("Importing {0}", inputFilePath), MessageType.INFO);

            avm.Design ad_import;
            using (StreamReader streamReader = new StreamReader(inputFilePath))
            {
                ad_import = CyPhyDesignImporterInterpreter.DeserializeAvmDesignXml(streamReader);
            }
            if (ad_import == null)
            {
                throw new Exception("Could not load ACM file.");
            }

            return ImportDesign(ad_import, mode);
        }

        public enum DesignImportMode
        {
            CREATE_CAS,
            CREATE_DS,
            CREATE_CA_IF_NO_DS_CONCEPTS,
        }

        public Model ImportDesign(avm.Design ad_import, DesignImportMode mode = DesignImportMode.CREATE_DS)
        {
            TellCyPhyAddonDontAssignIds();

            // TODO: check ad_import.SchemaVersion
            CyPhy.DesignEntity cyphy_container;

            if (mode == DesignImportMode.CREATE_CA_IF_NO_DS_CONCEPTS)
            {
                bool containsNonCompound = false;
                Queue<avm.Container> containers = new Queue<avm.Container>();
                containers.Enqueue(ad_import.RootContainer);
                while (containers.Count > 0)
                {
                    avm.Container container = containers.Dequeue();
                    containsNonCompound |= container is avm.Optional || container is avm.Alternative;
                    foreach (var subcontainer in container.Container1)
                    {
                        containers.Enqueue(subcontainer);
                    }
                }
                if (containsNonCompound)
                {
                    cyphy_container = CreateDesignSpaceRoot(ad_import);
                }
                else
                {
                    cyphy_container = CreateComponentAssemblyRoot(ad_import);
                }
            }
            else if (mode == DesignImportMode.CREATE_CAS)
            {
                cyphy_container = CreateComponentAssemblyRoot(ad_import);
            }
            else if (mode == DesignImportMode.CREATE_DS)
            {
                cyphy_container = CreateDesignSpaceRoot(ad_import);
            }
            else
            {
                throw new ArgumentOutOfRangeException("Unrecognized mode " + mode.ToString());
            }

            var ad_container = ad_import.RootContainer;

            ImportContainer(cyphy_container, ad_container);

            processValues();
            processPorts();

            Dictionary<avm.ConnectorCompositionTarget, avm.ConnectorCompositionTarget> connectorMap = new Dictionary<avm.ConnectorCompositionTarget, avm.ConnectorCompositionTarget>();
            foreach (var obj in this._avmCyPhyMLObjectMap)
            {
                if (obj.Key is avm.ConnectorCompositionTarget)
                {
                    avm.ConnectorCompositionTarget ad_compositionTarget1 = (avm.ConnectorCompositionTarget)obj.Key;
                    foreach (var ad_compositionTarget2ID in ad_compositionTarget1.ConnectorComposition.Where(id => string.IsNullOrEmpty(id) == false))
                    {
                        var ad_compositionTarget2 = _idConnectorMap[ad_compositionTarget2ID];
                        var cyphy_target = _avmCyPhyMLObjectMap[ad_compositionTarget2]; // TODO: handle lookup failure
                        if (string.Compare(ad_compositionTarget1.ID, ad_compositionTarget2.ID) < 0)
                        {
                            continue;
                        }
                        ConnectConnectorsAcrossHierarchy(obj.Value, cyphy_target);
                    }
                }
            }

            AddReferenceCoordinateSystemForAssemblyRoot(ad_import, cyphy_container);

            DoLayout();

            return (Model)cyphy_container;
        }

        private IEnumerable<Model> getParents(Model fco)
        {
            yield return fco;
            Model parent = fco.ParentContainer as Model;
            while (parent != null)
            {
                yield return parent;
                parent = parent.ParentContainer as Model;
            }
        }

        private void ConnectConnectorsAcrossHierarchy(object cyphy_source, object cyphy_target)
        {
            Model source_parent;
            Model source_connector;
            if (cyphy_source is KeyValuePair<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>)
            {
                source_parent = (Model)((KeyValuePair<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>)cyphy_source).Key.ParentContainer;
                source_connector = (Model)((KeyValuePair<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>)cyphy_source).Value;
            }
            else
            {
                source_connector = ((Model)cyphy_source);
                source_parent = (Model)source_connector.ParentContainer;
            }
            Model target_parent;
            Model target_connector;
            if (cyphy_target is KeyValuePair<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>)
            {
                target_parent = (Model)((KeyValuePair<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>)cyphy_target).Key.ParentContainer;
                target_connector = (Model)((KeyValuePair<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>)cyphy_target).Value;
            }
            else
            {
                target_connector = ((Model)cyphy_target);
                target_parent = (Model)target_connector.ParentContainer;
            }

            if (target_parent.ID == source_parent.ID
                || (AVM2CyPhyML.AVM2CyPhyMLBuilder.GetFCOObjectReference(cyphy_target) == null && source_parent.ID == target_parent.ParentContainer.ID)
                || (AVM2CyPhyML.AVM2CyPhyMLBuilder.GetFCOObjectReference(cyphy_source) == null && target_parent.ID == source_parent.ParentContainer.ID)
                )
            {
                makeConnection(cyphy_source, cyphy_target, typeof(CyPhy.ConnectorComposition).Name);
                return;
            }
            // AVM2CyPhyML.AVM2CyPhyMLBuilder.GetFCOObject(
            List<Model> source_parents = getParents(source_parent).ToList();
            List<Model> target_parents = getParents(target_parent).ToList();
            if (source_parents.Last().ID != target_parents.Last().ID)
            {
                throw new ApplicationException(String.Format("'{0}' and '{1}' cannot be connected", source_parent.Path, target_parent.Path));
            }
            Model commonAncestor = source_parent;
            // remove common ancestors from source_ and target_parents
            while (true)
            {
                if (source_parents[source_parents.Count - 1].ID == target_parents[target_parents.Count - 1].ID)
                {
                    commonAncestor = source_parents[source_parents.Count - 1];
                    source_parents.RemoveAt(source_parents.Count - 1);
                    target_parents.RemoveAt(target_parents.Count - 1);
                }
                else
                {
                    break;
                }
                if (source_parents.Count == 0 || target_parents.Count == 0)
                {
                    break;
                }
            }
            var source_intermediary = ConnectCompositionThroughHierarchy(cyphy_source, source_connector, source_parents);
            var target_intermediary = ConnectCompositionThroughHierarchy(cyphy_target, target_connector, target_parents);

            MgaMetaRole connectorRole = ((MgaMetaModel)commonAncestor.Impl.MetaBase).RoleByName["Connector"];
            var lastConnector = CyPhyClasses.Connector.Cast(((MgaModel)commonAncestor.Impl).DeriveChildObject((MgaFCO)source_connector.Impl, connectorRole, true));

            makeConnection(lastConnector, source_intermediary, typeof(CyPhy.ConnectorComposition).Name);
            makeConnection(lastConnector, target_intermediary, typeof(CyPhy.ConnectorComposition).Name);
        }

        private Object ConnectCompositionThroughHierarchy(object cyphy_source, Model source_connector, List<Model> source_parents)
        {
            object srcIntermediary = cyphy_source;
            for (int i = 0; i < source_parents.Count; i++)
            {
                Model parent = source_parents[i];
                MgaMetaRole connectorRole = ((MgaMetaModel)parent.Impl.MetaBase).RoleByName["Connector"];
                var newIntermediary = CyPhyClasses.Connector.Cast(((MgaModel)parent.Impl).DeriveChildObject((MgaFCO)source_connector.Impl, connectorRole, true));
                makeConnection(srcIntermediary, newIntermediary, typeof(CyPhy.ConnectorComposition).Name);
                srcIntermediary = newIntermediary;
            }

            return srcIntermediary;
        }

        private void TellCyPhyAddonDontAssignIds()
        {
            var cyPhyAddon = project.AddOnComponents.Cast<IMgaComponentEx>().Where(x => x.ComponentName.ToLowerInvariant() == "CyPhyAddOn".ToLowerInvariant()).FirstOrDefault();
            if (cyPhyAddon != null)
            {
                cyPhyAddon.ComponentParameter["DontAssignGuidsOnNextTransaction".ToLowerInvariant()] = true;
            }
        }

        private void AddReferenceCoordinateSystemForAssemblyRoot(avm.Design ad_import, CyPhy.DesignEntity cyphy_container)
        {
            foreach (var root in ad_import.DomainFeature.OfType<avm.cad.AssemblyRoot>())
            {
                CyPhyML.ComponentRef componentRef;
                if (idToComponentInstanceMap.TryGetValue(root.AssemblyRootComponentInstance, out componentRef))
                {
                    MgaFCO rcs = CreateChild((ISIS.GME.Common.Interfaces.Model)componentRef.ParentContainer, typeof(CyPhyML.ReferenceCoordinateSystem));
                    rcs.Name = "AssemblyRoot";
                    CyPhyML.ReferenceCoordinateSystem componentRcs = componentRef.Referred.Component.Children.ReferenceCoordinateSystemCollection.FirstOrDefault();
                    if (componentRcs == null)
                    {
                        componentRcs = CyPhyClasses.ReferenceCoordinateSystem.Create(componentRef.Referred.Component);
                    }

                    ((MgaModel)componentRef.ParentContainer.Impl).CreateSimpleConnDisp(((MgaMetaModel)componentRef.ParentContainer.Impl.MetaBase).RoleByName[typeof(CyPhyML.RefCoordSystem2RefCoordSystem).Name],
                        rcs, (MgaFCO)componentRcs.Impl, null, (MgaFCO)componentRef.Impl);

                    while (rcs.ParentModel.ID != cyphy_container.ID)
                    {
                        var oldrcs = rcs;
                        rcs = CreateChild(rcs.ParentModel.ParentModel, typeof(CyPhyML.ReferenceCoordinateSystem));
                        rcs.Name = "AssemblyRoot";
                        ((MgaModel)rcs.ParentModel).CreateSimplerConnDisp(((MgaMetaModel)rcs.ParentModel.Meta).RoleByName[typeof(CyPhyML.RefCoordSystem2RefCoordSystem).Name],
                            rcs, oldrcs);
                    }
                }
            }
        }

        private CyPhy.DesignContainer CreateDesignSpaceRoot(avm.Design ad_import)
        {
            CyPhy.DesignSpace ds;
            CyPhy.RootFolder rf = CyPhyClasses.RootFolder.GetRootFolder((MgaProject)project);
            ds = rf.Children.DesignSpaceCollection.Where(d => d.Name == "DesignSpaces").FirstOrDefault();
            if (ds == null)
            {
                ds = CyPhyClasses.DesignSpace.Create(rf);
                ds.Name = "DesignSpaces";
            }

            CyPhy.DesignContainer cyphy_container = CyPhyClasses.DesignContainer.Create(ds);
            // container.Name = ad_import.Name; RootContainer has a name too
            int designID;
            if (int.TryParse(ad_import.DesignID, out designID))
            {
                cyphy_container.Attributes.ID = designID;
            }
            cyphy_container.Attributes.ContainerType = CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum.Compound;
            return cyphy_container;
        }

        private CyPhy.ComponentAssembly CreateComponentAssemblyRoot(avm.Design ad_import)
        {
            CyPhy.ComponentAssemblies cyphy_cas;
            CyPhy.RootFolder rf = CyPhyClasses.RootFolder.GetRootFolder((MgaProject)project);
            cyphy_cas = rf.Children.ComponentAssembliesCollection.Where(d => d.Name == typeof(CyPhyClasses.ComponentAssemblies).Name).FirstOrDefault();
            if (cyphy_cas == null)
            {
                cyphy_cas = CyPhyClasses.ComponentAssemblies.Create(rf);
                cyphy_cas.Name = typeof(CyPhyClasses.ComponentAssemblies).Name;
            }
            CyPhy.ComponentAssembly cyphy_container = CyPhyClasses.ComponentAssembly.Create(cyphy_cas);
            // container.Name = ad_import.Name; RootContainer has a name too
            // TODO: check ad_import.SchemaVersion
            int designID;
            if (int.TryParse(ad_import.DesignID, out designID))
            {
                cyphy_container.Attributes.ID = designID;
            }
            return cyphy_container;
        }

        private void ImportContainer(CyPhy.DesignEntity cyphy_container, avm.Container ad_container)
        {
            cyphy_container.Name = ad_container.Name;
            AVM2CyPhyML.CyPhyMLComponentBuilder.SetLayoutData(ad_container, cyphy_container.Impl);

            Dictionary<Type, CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum> typeToAttribute = new Dictionary<Type, CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum>()
            {
                {typeof(avm.DesignSpaceContainer), CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum.Compound},
                {typeof(avm.Alternative), CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum.Alternative},
                {typeof(avm.Optional), CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum.Optional},
                {typeof(avm.Compound), CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum.Compound},
            };
            if (cyphy_container is CyPhy.DesignContainer)
            {
                ((CyPhy.DesignContainer)cyphy_container).Attributes.ContainerType = typeToAttribute[ad_container.GetType()];
                if (ad_container is avm.Alternative)
                {
                    ((IMgaFCO)cyphy_container.Impl).SetRegistryValueDisp("icon", "alternative_ds.png");
                }
                if (ad_container is avm.Optional)
                {
                    ((IMgaFCO)cyphy_container.Impl).SetRegistryValueDisp("icon", "optional_ds");
                }
            }
            if (ad_container is avm.Alternative)
            {
                foreach (var ad_mux in ((avm.Alternative)ad_container).ValueFlowMux)
                {
                    processMux((CyPhy.DesignContainer)cyphy_container, ad_mux);
                }
            }

            foreach (avm.Port avmPort in ad_container.Port)
            {
                if (cyphy_container is CyPhy.DesignContainer)
                {
                    process((CyPhy.DesignContainer)cyphy_container, avmPort);
                }
                else
                {
                    process((CyPhy.ComponentAssembly)cyphy_container, avmPort);
                }
            }
            foreach (var ad_connector in ad_container.Connector)
            {
                var cyphy_connector = CyPhyClasses.Connector.Cast(CreateChild((ISIS.GME.Common.Interfaces.Model)cyphy_container, typeof(CyPhyClasses.Connector)));
                processConnector(ad_connector, cyphy_connector);
            }

            foreach (var ad_prop in ad_container.Property)
            {
                if (cyphy_container is CyPhy.DesignContainer)
                {
                    process((CyPhy.DesignContainer)cyphy_container, ad_prop);
                }
                else
                {
                    process((CyPhy.ComponentAssembly)cyphy_container, ad_prop);
                }
            }

            foreach (var ad_componentinstance in ad_container.ComponentInstance)
            {
                CyPhy.ComponentRef cyphy_componentref;
                if (cyphy_container is CyPhy.DesignContainer)
                {
                    cyphy_componentref = CyPhyClasses.ComponentRef.Create((CyPhy.DesignContainer)cyphy_container);
                }
                else
                {
                    cyphy_componentref = CyPhyClasses.ComponentRef.Create((CyPhy.ComponentAssembly)cyphy_container);
                }
                ImportComponentInstance(ad_componentinstance, cyphy_componentref);
            }

            foreach (var ad_childcontainer in ad_container.Container1)
            {
                CyPhy.DesignEntity cyphy_childcontainer;
                if (cyphy_container is CyPhy.DesignContainer)
                {
                    cyphy_childcontainer = CyPhyClasses.DesignContainer.Create((CyPhy.DesignContainer)cyphy_container);
                }
                else
                {
                    cyphy_childcontainer = CyPhyClasses.ComponentAssembly.Create((CyPhy.ComponentAssembly)cyphy_container);
                }
                ImportContainer(cyphy_childcontainer, ad_childcontainer);
            }

            foreach (var simpleFormula in ad_container.Formula.OfType<avm.SimpleFormula>())
            {
                CyPhyML.SimpleFormula cyphy_simpleFormula = CyPhyClasses.SimpleFormula.Cast(CreateChild((ISIS.GME.Common.Interfaces.Model)cyphy_container, typeof(CyPhyClasses.SimpleFormula)));
                process(simpleFormula, cyphy_simpleFormula);
            }

            foreach (var complexFormula in ad_container.Formula.OfType<avm.ComplexFormula>())
            {
                var cyphy_customFormula = CyPhyClasses.CustomFormula.Cast(CreateChild((ISIS.GME.Common.Interfaces.Model)cyphy_container, typeof(CyPhyClasses.CustomFormula)));
                processComplexFormula(complexFormula, cyphy_customFormula);
            }
        }

        private void processMux(CyPhyML.DesignContainer designContainer, avm.ValueFlowMux ad_mux)
        {
            _avmValueNodeIDMap.Add(ad_mux.ID, new KeyValuePair<avm.ValueNode, object>(null, ad_mux));
        }

        Dictionary<string, CyPhy.ComponentRef> idToComponentInstanceMap = new Dictionary<string, CyPhy.ComponentRef>();
        private void ImportComponentInstance(avm.ComponentInstance ad_componentinstance, CyPhy.ComponentRef cyphy_componentref)
        {
            AVM2CyPhyML.CyPhyMLComponentBuilder.SetLayoutData(ad_componentinstance, cyphy_componentref.Impl);

            ISIS.GME.Dsml.CyPhyML.Interfaces.Component component;
            if (avmidComponentMap.TryGetValue(ad_componentinstance.ComponentID, out component) == false)
            {
                throw new ApplicationException(String.Format("Cannot find Component with ID {0}. Has it been imported?", ad_componentinstance.ComponentID));
            }
            cyphy_componentref.Referred.Component = component;
            cyphy_componentref.Name = ad_componentinstance.Name;
            //cyphy_componentref.Attributes.ID = ad_componentinstance.ID;
            cyphy_componentref.Attributes.InstanceGUID = ad_componentinstance.ID;
            idToComponentInstanceMap[ad_componentinstance.ID] = cyphy_componentref;

            foreach (var ad_propinstance in ad_componentinstance.PrimitivePropertyInstance)
            {
                Func<IMgaObject, string> getID = o =>
                {
                    var id = ((IMgaFCO)o).StrAttrByName["ID"];
                    if (id == "")
                    {
                        id = "id-" + Guid.Parse(((IMgaFCO)o).GetGuidDisp()).ToString("D");
                    }
                    return id;
                };

                var cyphy_component = this.avmidComponentMap[ad_componentinstance.ComponentID];
                CyPhy.ValueFlowTarget cyphy_componentPort;
                try
                {
                    cyphy_componentPort = cyphy_component.AllChildren.OfType<CyPhy.ValueFlowTarget>()
                        .Where(x => getID(x.Impl) == ad_propinstance.IDinComponentModel).SingleOrDefault();
                }
                catch (System.InvalidOperationException e)
                {
                    throw new ApplicationException(String.Format("Error: more than one PrimitivePropertyInstance with ID '{0}' in '{1}'. Run the ComponentExporter to fix.",
                        ad_propinstance.IDinComponentModel, ad_componentinstance.Name));
                }

                _avmCyPhyMLObjectMap.Add(ad_propinstance, new KeyValuePair<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>(cyphy_componentref, cyphy_componentPort));
                    registerValueNode(ad_propinstance.Value, ad_propinstance);
            }

            foreach (var ad_connectorInstance in ad_componentinstance.ConnectorInstance)
            {
                _idConnectorMap.Add(ad_connectorInstance.ID, ad_connectorInstance); // FIXME could be dup

                var cyphy_component = this.avmidComponentMap[ad_componentinstance.ComponentID];
                CyPhy.Connector cyphy_componentConnector;
                try
                {
                    cyphy_componentConnector = cyphy_component.AllChildren.OfType<CyPhy.Connector>()
                    .Where(x => ((MgaFCO)x.Impl).StrAttrByName["ID"] == ad_connectorInstance.IDinComponentModel).SingleOrDefault();
                }
                catch (System.InvalidOperationException e)
                {
                    throw new ApplicationException(String.Format("Error: more than one ConnectorInstance with ID '{0}' in '{1}'. Run the ComponentExporter to fix.",
                        ad_connectorInstance.IDinComponentModel, ad_componentinstance.Name));
                }

                if (cyphy_componentConnector == null)
                {
                    throw new ApplicationException("adm error: component instance " + ad_componentinstance.ID + " has connector with IDinComponentModel "
                        + ad_connectorInstance.IDinComponentModel + " that has no matching Connector in the Component");
                }

                _avmCyPhyMLObjectMap.Add(ad_connectorInstance, new KeyValuePair<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>(cyphy_componentref, cyphy_componentConnector));
            }

            foreach (var ad_port in ad_componentinstance.PortInstance)
            {
                registerPort(ad_port);

                var cyphy_component = this.avmidComponentMap[ad_componentinstance.ComponentID];
                var cyphy_componentConnector = cyphy_component.AllChildren.OfType<CyPhy.Port>()
                    .Where(x => ((MgaFCO)x.Impl).StrAttrByName["ID"] == ad_port.IDinComponentModel).FirstOrDefault();
                if (cyphy_componentConnector == null)
                {
                    throw new ApplicationException("adm error: component instance " + ad_componentinstance.ID + " has connector with IDinComponentModel "
                        + ad_port.IDinComponentModel + " that has no matching Connector in the Component");
                }
                _avmCyPhyMLObjectMap.Add(ad_port, new KeyValuePair<ISIS.GME.Common.Interfaces.Reference, ISIS.GME.Common.Interfaces.FCO>(cyphy_componentref, cyphy_componentConnector));
            }
        }

        private MgaFCO CreateChild(ISIS.GME.Common.Interfaces.Model parent, Type type)
        {
            var role = ((MgaMetaModel)parent.Impl.MetaBase).RoleByName[type.Name];
            return (MgaFCO)((MgaModel)parent.Impl).CreateChildObject(role);
        }

        private MgaFCO CreateChild(MgaModel parent, Type type)
        {
            var role = ((MgaMetaModel)parent.MetaBase).RoleByName[type.Name];
            return (MgaFCO)((MgaModel)parent).CreateChildObject(role);
        }
    }
}
