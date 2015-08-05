using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ISIS.GME.Common.Interfaces;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using avm;
using iFAB;
using Container = avm.Container;
using GME.MGA;
using System.Text.RegularExpressions;
using System.Reflection;
using System.Diagnostics;

namespace CyPhy2DesignInterchange
{
    using ConnectorRefport = CyPhy2DesignInterchange.RefportPair<CyPhy.Connector>;
    using PortRefport = CyPhy2DesignInterchange.RefportPair<CyPhy.Port>;

    [System.Runtime.InteropServices.GuidAttribute("39ECE2C1-4844-46DA-998C-9A4D44073376")]
    public class CyPhy2DesignInterchange
    {
        private static void SetLayoutData(object avmObj, IMgaObject cyphyObj)
        {
            if (cyphyObj is IMgaFCO)
            {
                var cyphyFCO = cyphyObj as IMgaFCO;

                var objWrapper = new LayoutDataWrapper(avmObj);

                // Look for layout data associated with the All aspect.
                // If we find that aspect, set all the values.
                // Otherwise, we expect that avmObj's positional values will remain unset.
                Boolean hasAllAspect = false;
                foreach (IMgaPart part in cyphyFCO.Parts)
                {
                    if (part.MetaAspect.Name == "All")
                    {
                        hasAllAspect = true;
                        break;
                    }
                }

                foreach (IMgaPart part in cyphyFCO.Parts)
                {
                    if (part.MetaAspect.Name == "All" || hasAllAspect == false)
                    {
                        String icon;
                        int xpos = 0;
                        int ypos = 0;
                        part.GetGmeAttrs(out icon, out xpos, out ypos);

                        if (xpos >= 0 && ypos >= 0)
                        {
                            objWrapper.xpos = System.Convert.ToUInt32(xpos);
                            objWrapper.ypos = System.Convert.ToUInt32(ypos);
                        }

                        //  We got what we came for. Terminate the loop.
                        break;
                    }
                }
            }
        }

        private ISet<string> designEntityIds = new HashSet<string>();
        private String GetOrSetID(CyPhy.DesignEntity de)
        {
            int id = de.Attributes.ID;
            if (id == 0)
            {
                de.Attributes.ID = de.Guid.GetHashCode();
                foreach (CyPhy.BuiltDesignEntityRef bdef in de.ReferencedBy.BuiltDesignEntityRef)
                {
                    foreach (CyPhy.Built built in bdef.SrcConnections.BuiltCollection)
                    {
                        CyPhy.DesignEntityRef der = built.SrcEnds.DesignEntityRef;
                        id = der.Guid.GetHashCode();
                    }
                }
            }
            if (id == 0)
            {
                id = de.Guid.GetHashCode();
            }
            if (id == 0 || designEntityIds.Add(id.ToString()) == false)
            {
                id = Guid.NewGuid().GetHashCode();
            }
            
            de.Attributes.ID = id;
            return id.ToString();
        }

        /// <summary>
        /// For a given ComponentRef inside a design that is derived from a Design Space, 
        /// find the corresponding ComponentRef from the Design Space.
        /// </summary>
        /// <param name="cr">A ComponentRef with a Design that is derived from a Design Space</param>
        /// <returns>The InstanceID of the corresponding object, if it can be located. Returns "" otherwise.</returns>
        private static String GetDesignSpaceSourceObjectID(CyPhy.ComponentRef cr)
        {
            string rtn = "";

            // For this entity, we are looking for "BuiltDesignEntityRef" objects that point to this.
            // Those will be linked by a "Built" connection to another DesignEntityRef.
            // This latter DesignEntityRef will point to the original Component/ComponentRef in the DesignSpace.
            try
            {
                var sourceDesignEntity = cr.ReferencedBy.BuiltDesignEntityRef
                                         .SelectMany(x => x.SrcConnections.BuiltCollection)
                                         .Select(x => x.SrcEnds.DesignEntityRef)
                                         .Where(x => x != null)
                                         .Select(x => x.Referred.DesignEntity)
                                         .FirstOrDefault();

                if (sourceDesignEntity != null)
                {
                    if (sourceDesignEntity is CyPhy.ComponentRef || sourceDesignEntity is CyPhy.Component)
                        rtn = GetAndEnsureInstanceID(sourceDesignEntity);
                    else
                        rtn = sourceDesignEntity.Guid.ToString("B");
                }
            }
            catch (NullReferenceException)
            {
                // Could not find source entity -- >=1 broken link in chain
            }

            return rtn;
        }

        /// <summary>
        /// For a ComponentRef or Component, return the InstanceGUID.
        /// If it does not have one, it will assign one, and then return.
        /// </summary>
        /// <param name="de">A ComponentRef or Component</param>
        /// <returns>The InstanceGUID of the object, or "" if it is not a ComponentRef or Component</returns>
        private static String GetAndEnsureInstanceID(CyPhy.DesignEntity de)
        {
            if (de is CyPhy.ComponentRef)
            {
                CyPhy.ComponentRef cr = de as CyPhy.ComponentRef;
                if (String.IsNullOrWhiteSpace(cr.Attributes.InstanceGUID) && cr.IsLib == false)
                    cr.Attributes.InstanceGUID = cr.Guid.ToString("B");
                return cr.Attributes.InstanceGUID;
            }
            else if (de is CyPhy.Component)
            {
                CyPhy.Component c = de as CyPhy.Component;
                if (string.IsNullOrWhiteSpace(c.Attributes.InstanceGUID) && c.IsLib == false)
                    c.Attributes.InstanceGUID = c.Guid.ToString("B");
                return c.Attributes.InstanceGUID;
            }
            return "";
        }

        private Dictionary<string, CyPhy.Port> _PortIdMap = new Dictionary<string, CyPhy.Port>();
        private String GetOrSetID(CyPhy.Port p, ComponentInstance component=null)
        {
            return GetOrSetID(p, _PortIdMap, component);
        }

        private Dictionary<string, CyPhy.Connector> _ConnectorIdMap = new Dictionary<string,CyPhy.Connector>();
        private String GetOrSetID(CyPhy.Connector c, ComponentInstance component=null)
        {
            return GetOrSetID(c, _ConnectorIdMap, component);
        }

        public String GetOrSetID<T>(T target, Dictionary<string, T> idMap, ComponentInstance component=null) where T : ISIS.GME.Common.Interfaces.Base
        {
            MgaFCO targetFCO = (MgaFCO)target.Impl;
            var attr = targetFCO.get_Attribute(getMgaIDAttribute(targetFCO.Meta));
            var id = attr.StringValue;

            if (component == null && (String.IsNullOrWhiteSpace(id) || attr.Status != (int)GME.MGA.attstatus_enum.ATTSTATUS_HERE))
            {
                id = "id-" + target.Guid.ToString("D");
                attr.StringValue = id;
            }
            if (component != null)
            {
                if (string.IsNullOrWhiteSpace(id))
                {
                    throw new ApplicationException(String.Format("{0}/{1} has no ID. An exported design would not import correctly. " +
                                                "To fix this, run the Component Exporter on the component.", target.ParentContainer.Name, target.Name));
                }
                id = component.ID + "-" + id;
            }
            T dup;
            if (idMap.TryGetValue(id, out dup))
            {
                if (dup.ID != target.ID) // i.e. different objects (not ID attribute)
                {
                    if (component != null)
                    {
                        // TODO: get traceability for link
                        //throw new ApplicationException(String.Format("<a href=\"mga:{0}\">{1}</a> has a duplicate ID. An exported design would not import correctly. " +
                        throw new ApplicationException(String.Format("{0}/{1} has a duplicate ID. An exported design would not import correctly. " +
                            "To fix this, run the Component Exporter on the component.", target.ParentContainer.Name, target.Name));
                    }
                    // FIXME: often, this is done on an elaborated model, so these IDs are lost in the mga
                    id = target.Guid.ToString("D");
                    if (id == getIDAttribute(dup))
                    {
                        id = Guid.NewGuid().ToString("D");
                    }
                    setIDAttribute(target, id);
                }
            }
            idMap[id] = target;

            return id;
        }

        public static Func<A, R> Memoize<A, R>(Func<A, R> f)
        {
            var map = new Dictionary<A, R>();
            return a =>
            {
                R value;
                if (map.TryGetValue(a, out value))
                    return value;
                value = f(a);
                map.Add(a, value);
                return value;
            };
        }

        Func<GME.MGA.Meta.MgaMetaFCO, GME.MGA.Meta.MgaMetaAttribute> getMgaIDAttribute = Memoize(delegate(GME.MGA.Meta.MgaMetaFCO meta) { return meta.AttributeByName["ID"]; });

        private Dictionary<string, CyPhy.ValueFlowTarget> _ValueFlowTargetIdMap = new Dictionary<string, CyPhy.ValueFlowTarget>();
        private String GetOrSetID(CyPhy.ValueFlowTarget vft)
        {
            return GetOrSetID(vft, _ValueFlowTargetIdMap);
        }

        private void ResetCache()
        {
            _dstConnectionCache = new Dictionary<CyPhy.Port, List<Connection>>();
            _srcConnectionCache = new Dictionary<CyPhy.Port, List<Connection>>();
            _reachablePortsCache = new Dictionary<CyPhy.Port, Dictionary<ConnectionTypes, List<CyPhy.Port>>>();
        }

        public static Design Convert(CyPhy.DesignEntity de)
        {
            return new CyPhy2DesignInterchange()._Convert(de);
        }

        // Only accept ComponentAssembly and DesignContainer
        public Design _Convert(CyPhy.DesignEntity de)
        {
            if (!(de.Kind == "ComponentAssembly" || de.Kind == "DesignContainer"))
                return null;

            ResetCache();

            var dm = new Design { Name = de.Name };
            avm.Container rootContainerType = null;

            if (de.Kind == typeof(CyPhy.ComponentAssembly).Name)
            {
                // get traceability info about source design space for this design if any.
                var ca = de as CyPhy.ComponentAssembly;
                if (ca.ReferencedBy.ComponentAssemblyRef.Any())
                {
                    // looking for a component assembly ref, where the parent is a Configurations object
                    var caRef = ca.ReferencedBy.ComponentAssemblyRef.FirstOrDefault(x => x.ParentContainer.Kind == typeof(CyPhy.Configurations).Name);
                    if (caRef != null)
                    {
                        // get GUID in the same way as below dm.DesignID = (de as CyPhy.DesignContainer).Impl.GetGuidDisp();
                        // caref is always at least in a 2 depth in the hierarchy, safely can go up 2 levels.
                        dm.DesignSpaceSrcID = caRef.ParentContainer.ParentContainer.Impl.GetGuidDisp();
                    }
                }
            }

            var containerMapping = new Dictionary<CyPhy.DesignEntity, avm.Container>();

            if (de is CyPhy.ComponentAssembly)
            {
                var s_ID = (de as CyPhy.ComponentAssembly).Attributes.ConfigurationUniqueID;
                if (string.IsNullOrEmpty(s_ID))
                {
                    s_ID = de.Guid.ToString("B");
                    (de as CyPhy.ComponentAssembly).Attributes.ConfigurationUniqueID = s_ID;
                }
                dm.DesignID = s_ID;

                rootContainerType = new Compound();
            }
            else if (de is CyPhy.DesignContainer)
            {
                dm.DesignID = "id-" + (de as CyPhy.DesignContainer).Guid.ToString();
                switch (((CyPhy.DesignContainer)de).Attributes.ContainerType)
                {
                    case CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum.Compound:
                        rootContainerType = new Compound();
                        break;
                    case CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum.Alternative:
                        rootContainerType = new Alternative();
                        break;
                    case CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum.Optional:
                        rootContainerType = new Optional();
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }

            containerMapping.Add(de, rootContainerType);

            // Add single ContainerType for the root level Container
            dm.RootContainer = rootContainerType;
            dm.RootContainer.Name = de.Name;

            // Note: do we need for the id? I don't think so...
            //dm.RootContainer. = GetOrSetID(de);

            var componentPortMapping = new Dictionary<CyPhy.Port, ComponentPortInstance>();
            var connectorMapping = new Dictionary<ConnectorRefport, avm.ConnectorCompositionTarget>();
            var componentValueflowTargetMapping = new Dictionary<CyPhy.ValueFlowTarget, ComponentPrimitivePropertyInstance>();
            var vftIdCache = new Dictionary<CyPhy.ValueFlowTarget, String>();
            var wrapper = new DesignPrimitivesWrapper(de);
            UpdateSubContainers(wrapper, dm.RootContainer, componentPortMapping, connectorMapping, containerMapping, componentValueflowTargetMapping, vftIdCache);

            AddAssemblyRoot(dm, wrapper);

            ConvertJoinStructures(dm, connectorMapping, containerMapping);


            return dm;
        }

        private static void AddAssemblyRoot(Design dm, DesignPrimitivesWrapper wrapper)
        {
            HashSet<CyPhy.ReferenceCoordinateSystem> visitedRCS = new HashSet<CyPhy.ReferenceCoordinateSystem>();
            Queue<CyPhy.ReferenceCoordinateSystem> todoRCS = new Queue<CyPhy.ReferenceCoordinateSystem>();
            List<string> rcsComponentIds = new List<string>();
            foreach (var rcs in wrapper.ReferenceCoordinateSystems)
            {
                todoRCS.Enqueue(rcs);
            }
            while (todoRCS.Count > 0)
            {
                var rcs = todoRCS.Dequeue();
                visitedRCS.Add(rcs);
                foreach (var rcsConnection in rcs.DstConnections.RefCoordSystem2RefCoordSystemCollection)
                {
                    if (visitedRCS.Contains(rcsConnection.DstEnd))
                    {
                        continue;
                    }
                    var ref_ = rcsConnection.DstRef();
                    if (ref_ != null && ref_.Meta.Name == "ComponentRef" && ref_.Referred != null && ref_.Referred.Meta.Name == "Component")
                    {
                        rcsComponentIds.Add(GetOrSetID(CyPhyClasses.ComponentRef.Cast(ref_)));
                    }
                    if (ref_ == null)
                    {
                        if (rcsConnection.DstEnd.ParentContainer.Impl.MetaBase.Name == "Component")
                        {
                            rcsComponentIds.Add(CyPhyClasses.Component.Cast(rcsConnection.DstEnd.ParentContainer.Impl).Attributes.InstanceGUID);
                        }
                        else
                        {
                            todoRCS.Enqueue(rcsConnection.DstEnds.ReferenceCoordinateSystem);
                        }
                    }
                }
                foreach (var rcsConnection in rcs.SrcConnections.RefCoordSystem2RefCoordSystemCollection)
                {
                    if (visitedRCS.Contains(rcsConnection.SrcEnd))
                    {
                        continue;
                    }
                    var ref_ = rcsConnection.SrcRef();
                    if (ref_ != null && ref_.Meta.Name == "ComponentRef" && ref_.Referred != null && ref_.Referred.Meta.Name == "Component")
                    {
                        rcsComponentIds.Add(GetOrSetID(CyPhyClasses.ComponentRef.Cast(ref_)));
                    }
                    if (ref_ == null)
                    {
                        if (rcsConnection.SrcEnd.ParentContainer.Impl.MetaBase.Name == "Component")
                        {
                            rcsComponentIds.Add(CyPhyClasses.Component.Cast(rcsConnection.SrcEnd.ParentContainer.Impl).Attributes.InstanceGUID);
                        }
                        else
                        {
                            todoRCS.Enqueue(rcsConnection.SrcEnds.ReferenceCoordinateSystem);
                        }
                    }
                }
            }
            if (rcsComponentIds.Count == 1)
            {
                dm.DomainFeature.Add(new avm.cad.AssemblyRoot()
                {
                    AssemblyRootComponentInstance = rcsComponentIds[0]
                });
            }
        }

        public class RefportPair<T> where T : ISIS.GME.Common.Interfaces.FCO
        {
            public T connector;
            public ISIS.GME.Common.Interfaces.Reference reference;

            public RefportPair(T connector, ISIS.GME.Common.Interfaces.Reference reference)
            {
                this.connector = connector;
                this.reference = reference;
            }

            public RefportPair(T connector, IMgaReference reference, bool mga)
            {
                this.connector = connector;
                if (reference != null)
                {
                    ISIS.GME.Common.Classes.Reference ref_ = new ISIS.GME.Common.Classes.Reference();
                    ref_.Impl = reference;
                    this.reference = ref_;
                }
            }

            public override int GetHashCode()
            {
                return connector.ID.GetHashCode() + (this.reference != null ? this.reference.ID : "").GetHashCode();
            }

            public override bool Equals(object o)
            {
                if (!(o is RefportPair<T>))
                {
                    return false;
                }
                RefportPair<T> that = (RefportPair<T>)o;
                if (this.connector.ID != that.connector.ID)
                {
                    return false;
                }
                if (this.reference == null ^ that.reference == null)
                {
                    return false;
                }
                if (this.reference == null && that.reference == null)
                {
                    return true;
                }
                return this.reference.ID == that.reference.ID;
            }
        }

        // Add JoinData to the Design model
        private static void ConvertJoinStructures(Design dm,
            Dictionary<ConnectorRefport, avm.ConnectorCompositionTarget> connectorMapping,
            Dictionary<CyPhy.DesignEntity, avm.Container> containerMapping)
        {
            foreach (var kvp in containerMapping.Where(kvp => kvp.Key is CyPhy.DesignContainer || kvp.Key is CyPhy.ComponentAssembly))
            {
                var orgContainer = kvp.Key;
                var genContainer = kvp.Value;

                IEnumerable<CyPhy.JoinData> e_jd = new List<CyPhy.JoinData>();
                if (kvp.Key is CyPhy.DesignContainer)
                    e_jd = (kvp.Key as CyPhy.DesignContainer).Children.JoinDataCollection;
                else
                    e_jd = (kvp.Key as CyPhy.ComponentAssembly).Children.JoinDataCollection;

                foreach (var jd in e_jd)
                {
                    var genJD = CyPhy2ComponentModel.JoinDataTransform.TransformJoinData(jd);
                    genContainer.JoinData.Add(genJD);

                    foreach (var ajd in jd.DstConnections.AssignJoinDataCollection.Where(c => c.IsRefportConnection() == false))
                    {
                        var orgAssignedConnector = ajd.DstEnds.Connector;
                        var genAssignedConnector = connectorMapping[new ConnectorRefport(orgAssignedConnector, null)];

                        genAssignedConnector.ApplyJoinData.Add(genJD.id);
                    }
                }
            }
        }




        private void UpdateSubContainers(DesignPrimitivesWrapper de, avm.Container rootContainer,
                                                Dictionary<CyPhy.Port, ComponentPortInstance> componentPortMapping,
                                                Dictionary<ConnectorRefport, avm.ConnectorCompositionTarget> connectorMapping,
                                                Dictionary<CyPhy.DesignEntity, avm.Container> containerMapping,
                                                Dictionary<CyPhy.ValueFlowTarget, ComponentPrimitivePropertyInstance> componentValueflowTargetMapping,
                                                Dictionary<CyPhy.ValueFlowTarget, String> vftIdCache,
                                                Dictionary<PortRefport, PortMapTarget> portMapping = null)
        {
            if (portMapping == null)
            {
                portMapping = new Dictionary<PortRefport, PortMapTarget>();
            }
            System.Diagnostics.Debug.WriteLine(de.Path);

            #region Get ComponentRefs

            var childComponents = new Dictionary<Object, avm.ComponentInstance>();
            foreach (var compRef in de.ComponentRefs)
            {
                CyPhy.Component comp = null;
                try
                {
                    comp = compRef.Referred.Component;
                }
                catch
                {
                    continue;
                }

                if (comp == null) continue;

                System.Diagnostics.Debug.WriteLine(comp.Path);

                var ci = Component2ComponentInstance(comp);
                if (!String.IsNullOrWhiteSpace(compRef.Attributes.InstanceGUID))
                    ci.ID = compRef.Attributes.InstanceGUID; // FIXME must not contain {
                else
                    ci.ID = compRef.Attributes.ID.ToString();
                ci.Name = compRef.Name;

                SetLayoutData(ci, compRef.Impl);

                // moving the old code/implementation to here.
                String s_SrcDesignSpaceContainerID = GetDesignSpaceSourceObjectID(compRef);
                if ( ! String.IsNullOrWhiteSpace(s_SrcDesignSpaceContainerID))
                    ci.DesignSpaceSrcComponentID = s_SrcDesignSpaceContainerID;

                childComponents.Add(compRef, ci);


                //var compInst = Component2ComponentInstance(comp);
                //rootContainer.ComponentInstance.Add(compInst);
                //compInst.Name = compRef.Name;
                //compInst.id = GetOrSetID(compRef);
                //compInst.id = compRef.Guid.ToString();

                //var instanceGUID = compRef.Attributes.InstanceGUID;
                //// Set this value if not set
                //if (String.IsNullOrWhiteSpace(instanceGUID))
                //{
                //    compRef.Attributes.InstanceGUID = compRef.Guid.ToString();
                //    instanceGUID = compRef.Attributes.InstanceGUID;
                //}
            }

            #endregion

            #region Get Component instances
            foreach (var comp in de.Components)
            {
                var ci = Component2ComponentInstance(comp);
                childComponents.Add(comp, ci);

                SetLayoutData(ci, comp.Impl);
            }
            #endregion

            #region Handle all Components
            foreach (var kvp in childComponents)
            {
                var compInst = kvp.Value;
                CyPhy.Component comp;
                CyPhy.ComponentRef reference;
                if (kvp.Key is CyPhy.Component)
                {
                    comp = kvp.Key as CyPhy.Component;
                    reference = null;
                }
                else
                {
                    reference = (CyPhy.ComponentRef)kvp.Key;
                    comp = (kvp.Key as CyPhy.ComponentRef).Referred.Component;
                }
                
                System.Diagnostics.Debug.WriteLine(comp.Path);
                rootContainer.ComponentInstance.Add(compInst);

                #region Component ports
                foreach (var port in comp.Children.DomainModelPortCollection)
                {
                    var cpi = new ComponentPortInstance
                    {
                        ID = GetOrSetID(port, compInst),
                        IDinComponentModel = port.Attributes.ID,
                    };
                    //portIds[port] = cpi.ID;
                    compInst.PortInstance.Add(cpi);
                    componentPortMapping[port] = cpi;

                    portMapping.Add(new PortRefport(port, reference), cpi);
                }
                #endregion

                #region Component properties

                foreach (var prop in comp.Children.PropertyCollection)
                {
                    DataTypeEnum xDataType;
                    if (!ConvertCyPhyDataTypeEnum(prop.Attributes.DataType, out xDataType)) continue;
                    var cppi = new ComponentPrimitivePropertyInstance
                                   {
                                       IDinComponentModel = prop.Attributes.ID,
                                       Value = new Value
                                                   {
                                                       DataType = xDataType,
                                                       DataTypeSpecified = true,
                                                       DimensionType = DimensionTypeEnum.Scalar,
                                                       DimensionTypeSpecified = true,
                                                       Dimensions = prop.Attributes.Dimension,
                                                       ID = prop.Guid.ToString("D")
                                                   }
                                   };
                    componentValueflowTargetMapping[prop] = cppi;
                    vftIdCache[prop] = cppi.Value.ID;
                    compInst.PrimitivePropertyInstance.Add(cppi);
                }

                #endregion

                #region Component parameters
                
                foreach (var param in comp.Children.ParameterCollection)
                {
                    DataTypeEnum xDataType;
                    if (!ConvertCyPhyDataTypeEnum(param.Attributes.DataType, out xDataType)) continue;
                    var cppi = new ComponentPrimitivePropertyInstance
                    {
                        IDinComponentModel = param.Attributes.ID,
                        Value = new Value
                        {
                            DataType = xDataType,
                            DataTypeSpecified = true,
                            DimensionType = DimensionTypeEnum.Scalar,
                            DimensionTypeSpecified = true,
                            Dimensions = param.Attributes.Dimension,
                            ID = param.Guid.ToString("D")
                        }
                    };
                    componentValueflowTargetMapping[param] = cppi;
                    vftIdCache[param] = cppi.Value.ID;
                    compInst.PrimitivePropertyInstance.Add(cppi);
                }

                #endregion

                #region Component connectors

                Dictionary<string, CyPhy.Connector> componentConenctors = new Dictionary<string, CyPhy.Connector>();
                foreach (var connector in comp.Children.ConnectorCollection)
                {
                    var xConnector = new ComponentConnectorInstance
                    {
                        IDinComponentModel = connector.Attributes.ID,
                        ID = GetOrSetID(connector, compInst),
                    };
                    try
                    {
                        componentConenctors.Add(xConnector.ID, connector);
                    }
                    catch (System.ArgumentException e)
                    {
                        throw new ApplicationException("Component " + comp.Name + " has two or more Connectors with the same ID " + connector.Attributes.ID + 
                            ". To fix this error, run the Component Exporter on " + comp.Name);
                    }
                    //connectorIds[connector] = xConnector.ID;
                    compInst.ConnectorInstance.Add(xConnector);

                    connectorMapping.Add(new ConnectorRefport(connector, reference), xConnector as avm.ComponentConnectorInstance);
                }

                #endregion
            }
            #endregion

            var ldeDesignEntityChildren = de.ComponentAssemblies.Cast<CyPhy.DesignEntity>().ToList();
            ldeDesignEntityChildren.AddRange(de.DesignContainers);

            #region Containers
            foreach (CyPhy.DesignEntity subDE in ldeDesignEntityChildren)
            {
                Container subContainer;
                if (subDE is CyPhy.DesignContainer &&
                    (subDE as CyPhy.DesignContainer).Attributes.ContainerType ==
                    CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum.Compound)
                {
                    subContainer = new Compound();
                }
                else if (subDE is CyPhy.DesignContainer &&
                         (subDE as CyPhy.DesignContainer).Attributes.ContainerType ==
                         CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum.Alternative)
                {
                    subContainer = new Alternative();
                }
                else if (subDE is CyPhy.DesignContainer &&
                         (subDE as CyPhy.DesignContainer).Attributes.ContainerType ==
                         CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum.Optional)
                {
                    subContainer = new Optional();
                }
                else if (subDE is CyPhy.ComponentAssembly)
                {
                    subContainer = new Compound();
                }
                else
                {
                    continue;
                }

                subContainer.Name = subDE.Name;
                //subRootContainerType.id = GetOrSetID(subDE);

                rootContainer.Container1.Add(subContainer);
                containerMapping.Add(subDE, subContainer);

                SetLayoutData(subContainer, subDE.Impl);

                UpdateSubContainers(new DesignPrimitivesWrapper(subDE), subContainer, componentPortMapping, connectorMapping, containerMapping, componentValueflowTargetMapping, vftIdCache, portMapping);
            }
            #endregion

            #region Ports

            // ModelicaConnector
            foreach (var mc in de.ModelicaConnectors)
            {
                var con = new avm.modelica.Connector
                              {
                                  ID = GetOrSetID(mc),
                                  Name = mc.Name,
                                  Class = mc.Attributes.Class,
                                  Locator = mc.Attributes.Locator,
                              };

                rootContainer.Port.Add(con);
                portMapping.Add(new PortRefport(mc, null), con);

                SetLayoutData(con, mc.Impl);
            }

            foreach (var cad in de.CADData)
            {
                var datum = CreateAvmCADDatum(cad);
                rootContainer.Port.Add(datum);
                portMapping.Add(new PortRefport(cad, null), datum);

                SetLayoutData(datum, cad.Impl);
            }
            foreach (var refsys in de.ReferenceCoordinateSystems)
            {
                // TODO: not yet in AVM spec: new avm.cad.ReferenceCoordinateSystem()
            }
            
            foreach (var ap in de.AbstractPorts)
            {
                var xAp = new AbstractPort
                              {
                                  ID = GetOrSetID(ap),
                                  Definition = ap.Attributes.Definition,
                                  Name = ap.Name,
                                  Notes = ap.Attributes.InstanceNotes,
                              };

                rootContainer.Port.Add(xAp);

                SetLayoutData(xAp, ap.Impl);
            }           

            #endregion

            #region Properties

            foreach (var prop in de.Properties)
            {
                DataTypeEnum xDataType;
                if (!ConvertCyPhyDataTypeEnum(prop.Attributes.DataType, out xDataType)) continue;


                // TODO: CompoundProperty
                String idProp = GetOrSetID(prop);
                var xProp = new PrimitiveProperty
                                {
                                    ID = "property." + idProp,
                                    Name = prop.Name,
                                    Value = new Value
                                                {
                                                    ID = idProp,
                                                    DataType = xDataType,
                                                    DataTypeSpecified = true,
                                                    DimensionType = DimensionTypeEnum.Scalar,
                                                    DimensionTypeSpecified = true,
                                                    Dimensions = prop.Attributes.Dimension,
                                                    Unit = GetUnitAbbr(prop.AllReferred)
                                                }
                                };

                var incomingValueFlows = prop.SrcConnections.ValueFlowCollection.Where(c => c.IsRefportConnection() == false);
                if (incomingValueFlows.Count() > 1)
                {
                    var mux = CreateDSMux(componentValueflowTargetMapping, "mux" + idProp, incomingValueFlows);

                    ((avm.Alternative)rootContainer).ValueFlowMux.Add(mux);
                    vftIdCache[prop] = mux.ID;
                    xProp.Value.ValueExpression = new DerivedValue { ValueSource = mux.ID };
                }
                else if (incomingValueFlows.Count() == 1)
                {
                    var vft = incomingValueFlows.First().SrcEnds.ValueFlowTarget;

                    String idVft = null;
                    ComponentPrimitivePropertyInstance cppi = null;
                    if (componentValueflowTargetMapping.TryGetValue(vft, out cppi))
                    {
                        idVft = cppi.Value.ID;
                    }
                    else
                    {
                        idVft = GetOrSetID(vft);
                    }

                    xProp.Value.ValueExpression = new DerivedValue { ValueSource = idVft };
                }
                else
                {
                    var avmFixedValue = new FixedValue();
                    xProp.Value.ValueExpression = avmFixedValue;
                    avmFixedValue.Value = prop.Attributes.Value;
                }

                SetLayoutData(xProp, prop.Impl);

                rootContainer.Property.Add(xProp);
                vftIdCache[prop] = xProp.Value.ID;
            }

            #endregion

            #region Parameters
            
            foreach (var param in de.Parameters)
            {
                DataTypeEnum xDataType;
                if (!ConvertCyPhyDataTypeEnum(param.Attributes.DataType, out xDataType)) continue;

                /*
                var idParam = GetOrSetID(param);
                var valueExpression = new ParametricValue();
                var xProp = new PrimitiveProperty
                {
                    ID = "param." + idParam,
                    Name = param.Name,
                    Value = new Value
                    {
                        ID = idParam,
                        DataType = xDataType,
                        DataTypeSpecified = true,
                        DimensionType = DimensionTypeEnum.Scalar,
                        DimensionTypeSpecified = true,
                        Dimensions = param.Attributes.Dimension,
                        ValueExpression = valueExpression
                    }
                };*/

                var xProp = convertParameter(param);
                var valueExpression = xProp.Value.ValueExpression as avm.ParametricValue;

                var incomingValueFlows = param.SrcConnections.ValueFlowCollection.Where(c => c.IsRefportConnection() == false);
                if (incomingValueFlows.Count() > 1)
                {
                    var mux = CreateDSMux(componentValueflowTargetMapping, "mux-" + GetOrSetID(param),incomingValueFlows);

                    ((avm.Alternative)rootContainer).ValueFlowMux.Add(mux);
                    vftIdCache[param] = mux.ID;
                    valueExpression.AssignedValue = new avm.DerivedValue() { ValueSource = mux.ID };
                }
                else if (incomingValueFlows.Count() == 1)
                {
                    var vft = param.SrcConnections.ValueFlowCollection.First(c => c.IsRefportConnection() == false).SrcEnds.ValueFlowTarget;

                    String idVft = null;
                    ComponentPrimitivePropertyInstance cppi = null;
                    if (componentValueflowTargetMapping.TryGetValue(vft, out cppi))
                    {
                        idVft = cppi.Value.ID;
                    }
                    else
                    {
                        idVft = GetOrSetID(vft);
                    }

                    valueExpression.AssignedValue = new avm.DerivedValue() { ValueSource = idVft };
                }
                else
                {
                    valueExpression.AssignedValue = new avm.FixedValue() { Value = param.Attributes.Value };
                }

                SetLayoutData(xProp, param.Impl);

                rootContainer.Property.Add(xProp);
                vftIdCache[param] = xProp.Value.ID;
            }

            #endregion

            #region SimpleFormulas
            foreach (var sf in de.SimpleFormulas)
            {
                var xSF = new avm.SimpleFormula
                {
                    Name = sf.Name,
                    Operation = simpleFormulaOperationMap[sf.Attributes.Method],
                    OperationSpecified = true,
                    ID = GetOrSetID(sf),
                    Operand = new List<String>()
                };

                foreach (var vft in sf.SrcConnections.ValueFlowCollection.Where(c => c.IsRefportConnection() == false).Select(vf => vf.SrcEnds.ValueFlowTarget))
                {
                    String idVft = null;
                    ComponentPrimitivePropertyInstance cppi = null;
                    if (componentValueflowTargetMapping.TryGetValue(vft, out cppi))
                    {
                        idVft = cppi.Value.ID;
                    }
                    else
                    {
                        idVft = GetOrSetID(vft);
                    }

                    xSF.Operand.Add(idVft);
                }

                SetLayoutData(xSF, sf.Impl);
                rootContainer.Formula.Add(xSF);
                vftIdCache[sf] = xSF.ID;
            }
            #endregion

            #region CustomFormulas
            foreach (var cf in de.CustomFormulas)
            {
                var xCF = new avm.ComplexFormula
                {
                    Name = cf.Name,
                    ID = GetOrSetID(cf),
                    Expression = cf.Attributes.Expression,
                    Operand = new List<Operand>()
                };

                SetLayoutData(xCF, cf.Impl);

                // Fetch operands
                foreach (var vf in cf.SrcConnections.ValueFlowCollection.Where(c => c.IsRefportConnection() == false))
                {
                    // If the ValueFlow connection includes a FormulaVariableName,
                    //      we'll use that as the Symbol.
                    // Otherwise, we'll use the name of the source object.
                    var fvName = vf.Attributes.FormulaVariableName;
                    Boolean fvNameProvided = !String.IsNullOrWhiteSpace(fvName);                        

                    // Find the ID of the source object
                    var vftSrc = vf.SrcEnds.ValueFlowTarget;
                    String idVftSrc = null;
                    ComponentPrimitivePropertyInstance cppi = null;
                    if (componentValueflowTargetMapping.TryGetValue(vftSrc, out cppi))
                    {
                        idVftSrc = cppi.Value.ID;
                    }
                    else
                    {
                        idVftSrc = GetOrSetID(vftSrc);
                    }
                    
                    var op = new Operand()
                    {
                        ValueSource = idVftSrc,
                        Symbol = fvNameProvided
                                    ? fvName
                                    : vftSrc.Name
                    };
                    xCF.Operand.Add(op);
                }

                rootContainer.Formula.Add(xCF);
                vftIdCache[cf] = xCF.ID;
            }
            #endregion

            #region Connectors
            foreach (var connector in de.ConnectorList)
            {
                var xConnector = new Connector
                {
                    ID = GetOrSetID(connector),
                    Definition = connector.Attributes.Definition,
                    Name = connector.Name,
                };

                SetLayoutData(xConnector, connector.Impl);

                rootContainer.Connector.Add(xConnector);
                connectorMapping[new ConnectorRefport(connector, null)] = xConnector as avm.ConnectorCompositionTarget;

                // TODO: [META-3310] add any domain connectors here ... META-3310
                foreach (var domainConnector in connector.Children.ModelicaConnectorCollection)
                {
                    var dConnector = new avm.modelica.Connector
                    {
                        ID = GetOrSetID(domainConnector),
                        Locator = domainConnector.Attributes.Locator,
                        Class = domainConnector.Attributes.Class,
                        Name = domainConnector.Name,
                    };

                    // FIXME: do we need any internals of the port? Parameters/Redeclares?
                    SetLayoutData(dConnector, domainConnector.Impl);
                    portMapping.Add(new PortRefport(domainConnector, null), dConnector);

                    xConnector.Role.Add(dConnector);
                }

                foreach (var cyPhyMLDomainModelPort in connector.Children.CADDatumCollection)
                {
                    avm.DomainModelPort avmDomainModelPort = CreateAvmCADDatum(cyPhyMLDomainModelPort);
                    if (avmDomainModelPort == null)
                    {
                        continue;
                    }

                    SetLayoutData(avmDomainModelPort, cyPhyMLDomainModelPort.Impl);
                    portMapping.Add(new PortRefport(cyPhyMLDomainModelPort, null), avmDomainModelPort);
                    xConnector.Role.Add(avmDomainModelPort);
                }
            }
            #endregion
            
            #region Connector Composition
            foreach (var connectorComposition in de.ConnectorCompositions)
            {
                // FIXME: [Zsolt L.] See http://escher.isis.vanderbilt.edu/JIRA/browse/META-1581
                ConnectorCompositionTarget srcGenConnector;
                ConnectorCompositionTarget dstGenConnector;
                if (connectorMapping.TryGetValue(new ConnectorRefport(connectorComposition.SrcEnds.Connector, connectorComposition.SrcRef(), mga: true), out srcGenConnector) &&
                    connectorMapping.TryGetValue(new ConnectorRefport(connectorComposition.DstEnds.Connector, connectorComposition.DstRef(), mga: true), out dstGenConnector))
                {
                    srcGenConnector.ConnectorComposition.Add(dstGenConnector.ID);
                    dstGenConnector.ConnectorComposition.Add(srcGenConnector.ID);
                }
                else
                {
                    if (Debugger.IsAttached)
                    {
                        Debugger.Break();
                    }
                }
            }
            #endregion

            foreach (var pc in de.PortCompositions)
            {
                if (pc.SrcEnds.Port == null || pc.DstEnds.Port == null)
                {
                    continue;
                }
                PortMapTarget srcPort;
                PortMapTarget dstPort;
                if (portMapping.TryGetValue(new PortRefport(pc.SrcEnds.Port, pc.SrcRef(), mga: true), out srcPort) &&
                    portMapping.TryGetValue(new PortRefport(pc.DstEnds.Port, pc.DstRef(), mga: true), out dstPort))
                {
                    srcPort.PortMap.Add(dstPort.ID);
                    dstPort.PortMap.Add(srcPort.ID);
                }
                else
                 {
                    if (Debugger.IsAttached)
                    {
                        Debugger.Break();
                    }
                }

            }

            #region ValueFlows
            foreach (var vf in de.ValueFlows)
            {
                var srcValueFlow = vf.SrcEnds.ValueFlowTarget;
                var dstValueFlow = vf.DstEnds.ValueFlowTarget;

                if (componentValueflowTargetMapping.ContainsKey(dstValueFlow))
                {
                    componentValueflowTargetMapping[dstValueFlow].Value.ValueExpression = new avm.DerivedValue
                    {
                        ValueSource = vftIdCache[srcValueFlow]
                    };
                }
            }
            #endregion
        }

        private ValueFlowMux CreateDSMux(Dictionary<CyPhy.ValueFlowTarget, ComponentPrimitivePropertyInstance> componentValueflowTargetMapping, String idProp, IEnumerable<CyPhy.ValueFlow> incomingValueFlows)
        {
            var mux = new avm.ValueFlowMux()
            {
                ID = idProp,
                // Name = TODO
            };

            foreach (var vft in incomingValueFlows.Select(x => x.SrcEnds.ValueFlowTarget))
            {
                String idVft = null;
                ComponentPrimitivePropertyInstance cppi = null;
                if (componentValueflowTargetMapping.TryGetValue(vft, out cppi))
                {
                    idVft = cppi.Value.ID;
                }
                else
                {
                    idVft = GetOrSetID(vft);
                }
                mux.Source.Add(idVft);
            }
            return mux;
        }

        private avm.DomainModelPort CreateAvmCADDatum(CyPhy.CADDatum cyPhyMLDomainModelPort)
        {
            string cyPhyMLDomainModelPortTypeName = cyPhyMLDomainModelPort.GetType().ToString();

            CyPhyML2AVM.CreatorBase creator;
            if (CyPhyML2AVM.AVMComponentBuilder._avmNameCreateMethodMap.TryGetValue(cyPhyMLDomainModelPortTypeName, out creator) == false)
            {
                // writeMessage("WARNING:  No way to create AVM object from \"" + avmPortTypeName + "\" CyPhyML DomainModelPort.", MessageType.WARNING);
                return null;
            }

            avm.DomainModelPort avmDomainModelPort = null;
            creator.call(out avmDomainModelPort);

            PropertyInfo avmClassPropertyInfo = getPropertyInfo(avmDomainModelPort, "Class");

            if (avmClassPropertyInfo != null)
            {
                PropertyInfo cyPhyMLAttributesPropertyInfo = getPropertyInfo(getInterfaceType(cyPhyMLDomainModelPort), "Attributes");
                if (cyPhyMLAttributesPropertyInfo != null)
                {
                    PropertyInfo cyPhyMLClassPropertyInfo = cyPhyMLAttributesPropertyInfo.PropertyType.GetProperty("Class");
                    if (cyPhyMLClassPropertyInfo != null)
                    {
                        avmClassPropertyInfo.SetValue(avmDomainModelPort, cyPhyMLClassPropertyInfo.GetValue(cyPhyMLAttributesPropertyInfo.GetValue(cyPhyMLDomainModelPort, null), null), null);
                    }
                }
            }

            avmDomainModelPort.Name = cyPhyMLDomainModelPort.Name;
            if (avmDomainModelPort is avm.cad.Datum)
            {
                avm.cad.Datum datum = (avm.cad.Datum)avmDomainModelPort;
                datum.DatumName = cyPhyMLDomainModelPort.Attributes.DatumName;
                // datum.DatumMetric = cyPhyMLDomainModelPort.Attributes.
                // avm.cad.Surface s;
                // s.ReferencePlane =  TODO?
            }
            avmDomainModelPort.ID = GetOrSetID(cyPhyMLDomainModelPort);
            avmDomainModelPort.Definition = cyPhyMLDomainModelPort.Attributes.Definition;
            avmDomainModelPort.Notes = cyPhyMLDomainModelPort.Attributes.InstanceNotes;

            return avmDomainModelPort;
        }

        private string GetUnitAbbr(FCO fco)
        {
            if (fco != null && fco is CyPhy.unit)
            {
                CyPhy.unit unit = (CyPhy.unit)fco;
                return unit.Attributes.Abbreviation;
            }
            return null;
        }

        public avm.PrimitiveProperty convertParameter(CyPhy.Parameter cyPhyMLParameter)
        {
            var idParam = GetOrSetID(cyPhyMLParameter);

            return CyPhyML2AVM.AVMComponentBuilder.convertParameter(cyPhyMLParameter, idParam);
        }

        private static Dictionary<CyPhyClasses.SimpleFormula.AttributesClass.Method_enum, avm.SimpleFormulaOperation> simpleFormulaOperationMap = new Dictionary<CyPhyClasses.SimpleFormula.AttributesClass.Method_enum, SimpleFormulaOperation>()
        {
            { CyPhyClasses.SimpleFormula.AttributesClass.Method_enum.Addition, avm.SimpleFormulaOperation.Addition },
            { CyPhyClasses.SimpleFormula.AttributesClass.Method_enum.ArithmeticMean, avm.SimpleFormulaOperation.ArithmeticMean },
            { CyPhyClasses.SimpleFormula.AttributesClass.Method_enum.GeometricMean, avm.SimpleFormulaOperation.GeometricMean },
            { CyPhyClasses.SimpleFormula.AttributesClass.Method_enum.Maximum, avm.SimpleFormulaOperation.Maximum },
            { CyPhyClasses.SimpleFormula.AttributesClass.Method_enum.Minimum, avm.SimpleFormulaOperation.Minimum },
            { CyPhyClasses.SimpleFormula.AttributesClass.Method_enum.Multiplication, avm.SimpleFormulaOperation.Multiplication }
        };

        private static bool ConvertCyPhyDataTypeEnum(CyPhyClasses.Property.AttributesClass.DataType_enum cyphyDataType, out DataTypeEnum xDataType)
        {
            if (cyphyDataType == CyPhyClasses.Property.AttributesClass.DataType_enum.Float)
            {
                xDataType = DataTypeEnum.Real;
            }
            else
            {
                var valid = Enum.TryParse(cyphyDataType.ToString(), true, out xDataType);
                return valid;
            }
            return true;
        }

        private static bool ConvertCyPhyDataTypeEnum(CyPhyClasses.Parameter.AttributesClass.DataType_enum cyphyDataType, out DataTypeEnum xDataType)
        {
            if (cyphyDataType == CyPhyClasses.Parameter.AttributesClass.DataType_enum.Float)
            {
                xDataType = DataTypeEnum.Real;
            }
            else
            {
                var valid = Enum.TryParse(cyphyDataType.ToString(), true, out xDataType);
                return valid;
            }
            return true;
        }

        private static System.Reflection.PropertyInfo getPropertyInfo(Type type, string propertyName)
        {
            return type.GetProperty(propertyName);
        }

        private static System.Reflection.PropertyInfo getPropertyInfo(object object_var, string propertyName)
        {
            return getPropertyInfo(object_var.GetType(), propertyName);
        }

        private static Type getInterfaceType(Type type)
        {
            string typeName = type.UnderlyingSystemType.AssemblyQualifiedName.Replace(".Classes.", ".Interfaces.");
            return Type.GetType(typeName);
        }

        private static Type getInterfaceType(object object_var)
        {
            return getInterfaceType(object_var.GetType());
        }

        private static string getIDAttribute(ISIS.GME.Common.Interfaces.Base cyPhyMLObject)
        {
            string id = "";

            var cyPhyMLAttributesPropertyInfo = getPropertyInfo(getInterfaceType(cyPhyMLObject), "Attributes");
            if (cyPhyMLAttributesPropertyInfo != null)
            {
                var cyPhyMLIDPropertyInfo = cyPhyMLAttributesPropertyInfo.PropertyType.GetProperty("ID");
                if (cyPhyMLIDPropertyInfo != null)
                {
                    id = cyPhyMLIDPropertyInfo.GetValue(cyPhyMLAttributesPropertyInfo.GetValue(cyPhyMLObject, null), null) as string;
                }
            }

            return id;
        }

        private static void setIDAttribute(ISIS.GME.Common.Interfaces.Base cyPhyMLObject, String id)
        {
            var cyPhyMLAttributesPropertyInfo = getPropertyInfo(getInterfaceType(cyPhyMLObject), "Attributes");
            if (cyPhyMLAttributesPropertyInfo != null)
            {
                var cyPhyMLIDPropertyInfo = cyPhyMLAttributesPropertyInfo.PropertyType.GetProperty("ID");
                if (cyPhyMLIDPropertyInfo != null)
                {
                    cyPhyMLIDPropertyInfo.SetValue(cyPhyMLAttributesPropertyInfo.GetValue(cyPhyMLObject, null), id, null);
                }
            }
        }


        public static ComponentInstance Component2ComponentInstance(CyPhy.Component comp)
        {
            var compInst = new ComponentInstance();

            if (!String.IsNullOrWhiteSpace(comp.Attributes.InstanceGUID)) // FIXME could be inherited from archetype
                compInst.ID = comp.Attributes.InstanceGUID;
            else
                compInst.ID = comp.Attributes.ID.ToString();

            if (!String.IsNullOrWhiteSpace(comp.Attributes.AVMID))
            {
                compInst.ComponentID = comp.Attributes.AVMID;
            }
            else
            {
                // If there's no AVMID, then make one up, and save it to the component
                String s_GUID = Guid.NewGuid().ToString();
                compInst.ComponentID = comp.Attributes.AVMID = s_GUID;
            }
            compInst.Name = comp.Name;
            return compInst;
        }

        private Dictionary<CyPhy.Port, List<Connection>> _srcConnectionCache = new Dictionary<CyPhy.Port, List<Connection>>();
        private IEnumerable<Connection> GetAllSrcConnection(CyPhy.Port port)
        {
            if (!_srcConnectionCache.ContainsKey(port))
            {
                var result = new List<ISIS.GME.Common.Interfaces.Connection>();
                var obj = port.Impl as GME.MGA.MgaFCO;
                foreach (GME.MGA.MgaConnPoint connpoint in obj.PartOfConns)
                {
                    var conn = connpoint.Owner as GME.MGA.MgaSimpleConnection;
                    if (conn.Dst == obj)
                    {
                        result.Add(
                            ISIS.GME.Common.Utils.CreateObject<ISIS.GME.Common.Classes.Connection>(
                                conn as GME.MGA.MgaObject));
                    }
                }
                _srcConnectionCache[port] = result;
            }
            return _srcConnectionCache[port];
        }

        private Dictionary<CyPhy.Port, List<Connection>> _dstConnectionCache = new Dictionary<CyPhy.Port, List<Connection>>();
        private IEnumerable<Connection> GetAllDstConnection(CyPhy.Port port)
        {
            if (!_dstConnectionCache.ContainsKey(port))
            {
                var result = new List<ISIS.GME.Common.Interfaces.Connection>();
                var obj = port.Impl as GME.MGA.MgaFCO;
                foreach (GME.MGA.MgaConnPoint connpoint in obj.PartOfConns)
                {
                    var conn = connpoint.Owner as GME.MGA.MgaSimpleConnection;
                    if (conn.Src == obj)
                    {
                        result.Add(
                            ISIS.GME.Common.Utils.CreateObject<ISIS.GME.Common.Classes.Connection>(
                                conn as GME.MGA.MgaObject));
                    }
                }
                _dstConnectionCache[port] = result;
            }
            return _dstConnectionCache[port];
        }

        private Dictionary<CyPhy.Port, Dictionary<ConnectionTypes, List<CyPhy.Port>>> _reachablePortsCache = new Dictionary<CyPhy.Port, Dictionary<ConnectionTypes, List<CyPhy.Port>>>();
        private IEnumerable<CyPhy.Port> GetReachablePorts(CyPhy.Port port, ConnectionTypes conType, IList<PortTypes> portTypes, IEnumerable<Connection> connections = null)
        {
            if (!_reachablePortsCache.ContainsKey(port) || !_reachablePortsCache[port].ContainsKey(conType))
            {
                var result = new List<CyPhy.Port>();
                var processedConnection = new List<Connection>();
                foreach (var connection in GetAllDstConnection(port)
                                           .Where(x => connections == null || connections.Any(y => y.ID == x.ID))
                                           .Where(x => (conType == ConnectionTypes.All) || x.Kind == conType.ToString()))
                {
                    if (processedConnection.Any(x => x.Guid == connection.Guid)) continue;
                    processedConnection.Add(connection);

                    if (!portTypes.Contains(PortTypes.All) &&
                        !portTypes.Any(x => connection.GenericDstEnd.Kind == x.ToString())) continue;

                    //if (portTypes.All(x => x != PortTypes.All) || portTypes.All(x => connection.GenericDstEnd.Kind != x.ToString())) continue;

                    if (!result.Contains(connection.GenericDstEnd))
                        result.Add(CyPhyClasses.Port.Cast(connection.GenericDstEnd.Impl));
                }
                foreach (var connection in GetAllSrcConnection(port)
                                           .Where(x => connections == null || connections.Any(y => y.ID == x.ID))
                                           .Where(x => (conType == ConnectionTypes.All) || x.Kind == conType.ToString()))
                {
                    if (processedConnection.Any(x => x.Guid == connection.Guid)) continue;
                    processedConnection.Add(connection);

                    //if (portTypes.All(x => x != PortTypes.All) || portTypes.All(x => connection.GenericSrcEnd.Kind != x.ToString())) continue;
                    if (!portTypes.Contains(PortTypes.All) &&
                        !portTypes.Any(x => connection.GenericSrcEnd.Kind == x.ToString())) continue;

                    if (!result.Contains(connection.GenericSrcEnd))
                        result.Add(CyPhyClasses.Port.Cast(connection.GenericSrcEnd.Impl));
                }

                if (!_reachablePortsCache.ContainsKey(port))
                    _reachablePortsCache[port] = new Dictionary<ConnectionTypes, List<CyPhy.Port>>();
                _reachablePortsCache[port][conType] = result;
            }
            return _reachablePortsCache[port][conType];
        }

        private IEnumerable<CyPhy.Port> GetReachablePorts(CyPhy.Port port, ConnectionTypes conType, IEnumerable<Connection> connections = null)
        {
            return GetReachablePorts(port, conType, new List<PortTypes> { PortTypes.All }, connections);
        }

        private IEnumerable<CyPhy.Port> GetReachableStructuralPorts(CyPhy.Port port, ConnectionTypes conType, IEnumerable<Connection> connections = null)
        {
            return GetReachablePorts(port, conType, new List<PortTypes> { PortTypes.StructuralInterface, PortTypes.StructuralInterfaceForwarder }, connections);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="port"></param>
        /// <param name="results"></param>
        /// <param name="connectionType"> </param>
        /// <param name="connections">The search space is limited to the given connections. All connection is processed if NULL. Recommended to filter your connections previously.</param>
        private void GroupPorts(CyPhy.Port port, ICollection<CyPhy.Port> results, ConnectionTypes connectionType, IEnumerable<Connection> connections = null)
        {
            foreach (var nextPort in GetReachablePorts(port, connectionType, connections))
            {
                if (results.Any(x => x.ID == nextPort.ID)) continue;

                results.Add(nextPort);
                GroupPorts(nextPort, results, connectionType, connections);
            }
        }

        private void GroupStructuralPorts(CyPhy.Port port, ICollection<CyPhy.Port> results, ConnectionTypes connectionType, IEnumerable<Connection> connections = null)
        {
            foreach (var nextPort in GetReachableStructuralPorts(port, connectionType, connections))
            {
                if (results.Any(x => x.ID == nextPort.ID)) continue;

                results.Add(nextPort);
                GroupStructuralPorts(nextPort, results, connectionType, connections);
            }
        }

        private static T Cast<T>(GME.MGA.IMgaObject mgaObject)
            where T : ISIS.GME.Common.Classes.Base, new()
        {
            Type resultType = typeof(T);
            Type[] types = resultType.Assembly.GetTypes();

            // TODO: this is really slow
            foreach (var type in types.Where(x => x.Name == mgaObject.MetaBase.Name))
            {
                if (type.IsClass)
                {
                    var iType = type.GetInterfaces();
                    var iResultType = resultType.GetInterfaces();
                    bool isOk = iResultType.All(iType.Contains);
                    //foreach (var item in iResultType)
                    //{
                    //    if (iType.Contains(item) == false)
                    //    {
                    //        isOK = false;
                    //    }
                    //}
                    if (isOk)
                    {
                        return ISIS.GME.Common.Utils.CreateObject<T>(mgaObject as GME.MGA.MgaObject);
                    }
                }
            }
            return null;
        }
    }

    public enum ConnectionTypes
    {
        All,
        PowerFlow,
        JoinStructures
    }

    public enum PortTypes
    {
        All,
        StructuralInterface,
        StructuralInterfaceForwarder,
    }

    public static class RefportExtension
    {
        public static bool IsRefportConnection(this ISIS.GME.Common.Interfaces.Connection connection)
        {
            IMgaSimpleConnection conn = ((IMgaSimpleConnection)connection.Impl);
            var srcRefs = conn.SrcReferences;
            if (srcRefs != null && srcRefs.Count > 0)
                return true;
            var dstRefs = conn.DstReferences;
            if (dstRefs != null && dstRefs.Count > 0)
                return true;
            return false;
        }

        public static IMgaReference SrcRef(this ISIS.GME.Common.Interfaces.Connection connection)
        {
            IMgaSimpleConnection conn = ((IMgaSimpleConnection)connection.Impl);
            var srcRefs = conn.SrcReferences;
            if (srcRefs != null && srcRefs.Count > 0)
            {
                return (IMgaReference)srcRefs[1];
            }
            return null;
        }

        public static IMgaReference DstRef(this ISIS.GME.Common.Interfaces.Connection connection)
        {
            IMgaSimpleConnection conn = ((IMgaSimpleConnection)connection.Impl);
            var dstRefs = conn.DstReferences;
            if (dstRefs != null && dstRefs.Count > 0)
            {
                return (IMgaReference)dstRefs[1];
            }
            return null;
        }
    }
}
