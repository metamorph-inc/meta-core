using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


using AVM.META.Design;
using AVM;
using ISIS.GME.Common.Interfaces;
using Container = AVM.META.Design.Container;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2DesignInterchange_ddp1format
{

    public static class CyPhy2DesignInterchange
    {
        private static String GetIDOfDesignSpaceSourceObject(CyPhy.DesignElement de)
        {
            // Look for a DesignEntityRef, within a CWC, that points to this.
            foreach (CyPhy.DesignEntityRef der in de.ReferencedBy.DesignEntityRef)
            {
                // Get the Built connection and then the BuildDesignEntityRef.
                // The BuildDesignEntityRef will point to our source object.
                foreach (CyPhy.Built b in der.DstConnections.BuiltCollection)
                {
                    ISIS.GME.Common.Interfaces.FCO fco = b.SrcEnd;
                    if (fco is CyPhy.BuiltDesignEntityRef)
                    {
                        return GetOrSetID((fco as CyPhy.BuiltDesignEntityRef).Referred.DesignEntity);
                    }
                }
            }

            return "";
        }

        private static String GetOrSetID(CyPhy.DesignEntity de)
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
                id = Guid.NewGuid().GetHashCode();
            
            de.Attributes.ID = id;
            return id.ToString();
        }

        private static String GetDesignSpaceSourceObjectID(CyPhy.ComponentRef cr)
        {
            foreach (CyPhy.BuiltDesignEntityRef bdef in cr.ReferencedBy.BuiltDesignEntityRef)
            {
                foreach (CyPhy.Built built in bdef.SrcConnections.BuiltCollection)
                {
                    CyPhy.DesignEntityRef der = built.SrcEnds.DesignEntityRef;
                    CyPhy.DesignEntity de_Source = der.Referred.DesignEntity;
                    return de_Source.Guid.ToString();
                }
            }
            return "";
        }
        
        private static String GetOrSetID(CyPhy.Port p)
        {
            String id = p.Attributes.ID;
            if (id == "" &&
                p.IsLib == false)
            {
                // set it only if it is not an attached Library object
                id = p.Impl.GetGuidDisp();
                p.Attributes.ID = id;
            }
            return id;
        }

        private static void ResetCache()
        {
            _dstConnectionCache = new Dictionary<CyPhy.Port, List<Connection>>();
            _srcConnectionCache = new Dictionary<CyPhy.Port, List<Connection>>();
            _reachablePortsCache = new Dictionary<CyPhy.Port, Dictionary<ConnectionTypes, List<CyPhy.Port>>>();
        }

        // Only accept ComponentAssembly and DesignContainer
        public static DesignModel Convert(CyPhy.DesignEntity de)
        {            
            if (!(de.Kind == "ComponentAssembly" || de.Kind == "DesignContainer"))
                return null;

            ResetCache();

            var dm = new DesignModel {Name = de.Name};
            Container rootContainerType = null;


            if (de is CyPhy.ComponentAssembly)
            {
                String s_ID = (de as CyPhy.ComponentAssembly).Attributes.ConfigurationUniqueID;
                if (string.IsNullOrEmpty(s_ID))
                {
                    s_ID = Guid.NewGuid().ToString("B");
                    (de as CyPhy.ComponentAssembly).Attributes.ConfigurationUniqueID = s_ID;
                }
                if (s_ID.IndexOf("{") != 0)
                {
                    // Add curly braces
                    s_ID = string.Concat( "{", s_ID, "}" );
                }
                dm.DesignID = s_ID;

                rootContainerType = new Compound();
            }
            else if (de is CyPhy.DesignContainer)
            {
                dm.DesignID = (de as CyPhy.DesignContainer).Impl.GetGuidDisp();
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


            // Add single ContainerType for the root level Container
            
            dm.Containers.AddItem(rootContainerType);
            rootContainerType.Name = de.Name;
            rootContainerType.id = GetOrSetID(de);

            // Update DesignModel by calling recursive update method
            var innerPortInstances = new Dictionary<CyPhy.Port, PortInstance>();
            var containerPortInstances = new Dictionary<CyPhy.Port, ContainerPort>();

            var innerStructuralPortInstances = new Dictionary<CyPhy.Port, StructuralInterfaceInstance>();
            var containerStructuralPortInstances = new Dictionary<CyPhy.Port, ContainerStructuralInterface>();

            var connections = new List<Connection>();
            var simpleFormulas = new Dictionary<CyPhy.SimpleFormula, ContainerCalculation>();

            // Can contain: ContainerNamedValue/ContainerCalculation/ComponentNamedValueInstance
            var valueInstances = new Dictionary<CyPhy.ValueFlowTarget, AVM.META.Design.ValueType>();


            UpdateSubContainers(new DesignPrimitivesWrapper(de), rootContainerType, dm, innerPortInstances, containerPortInstances, connections, simpleFormulas, valueInstances, innerStructuralPortInstances, containerStructuralPortInstances);


            #region Create AllPorts dictionary
            var allPorts = new Dictionary<CyPhy.Port, AbstractPort>();

            foreach (var portInstance in innerPortInstances)
            {
                allPorts.Add(portInstance.Key, portInstance.Value);
            }

            foreach (var containerPort in containerPortInstances)
            {
                allPorts.Add(containerPort.Key, containerPort.Value);
            }

            #endregion

            #region Create AllStructuralPorts dictionary
            var allStructuralPorts = new Dictionary<CyPhy.Port, AbstractStructuralInterface>();

            foreach (var abstractPortInstance in innerStructuralPortInstances)
            {
                allStructuralPorts.Add(abstractPortInstance.Key, abstractPortInstance.Value);
            }

            foreach (var abstractContainerPort in containerStructuralPortInstances)
            {
                allStructuralPorts.Add(abstractContainerPort.Key, abstractContainerPort.Value);
            }

            #endregion


            #region Join Structures -> PortConnectors

            // Pair construct
            //ConvertJoinStructures(dm, innerPortInstances, connections.OfType<CyPhy.JoinStructures>());


            // Grouping construct
            //ConvertJoinStrucutresInGroups(GetStructuralPortGroups(allPorts, connections.OfType<CyPhy.JoinStructures>()), dm, allPorts);

            // Pair construct 

            #endregion

            #region ValueFlow -> ValueConnector

            ConvertValueFlows(dm, connections.OfType<CyPhy.ValueFlow>(), simpleFormulas, allPorts, valueInstances);

            #endregion

            return dm;
        }

        // Add ComponentNamedValueInstances to the design model 
        private static void ConvertValueFlows(DesignModel dm, IEnumerable<CyPhy.ValueFlow> valueFlowConnectors, Dictionary<CyPhy.SimpleFormula, ContainerCalculation> simpleFormulaCache, Dictionary<CyPhy.Port, AbstractPort> allPorts, Dictionary<CyPhy.ValueFlowTarget, AVM.META.Design.ValueType> valueInstances)
        {
            #region Param -> SimpleFormula

            foreach (var valueFlowConnector in valueFlowConnectors.Where(x => x.GenericSrcEnd.Kind == "Parameter" && x.GenericDstEnd.Kind == "SimpleFormula"))
            {
                var src = CyPhyClasses.Parameter.Cast(valueFlowConnector.GenericSrcEnd.Impl);
                var dst = CyPhyClasses.SimpleFormula.Cast(valueFlowConnector.GenericDstEnd.Impl);

                if (!valueInstances.ContainsKey(src) || !simpleFormulaCache.ContainsKey(dst)) continue;

                dm.Connectors.AddItem(new ValueConnector
                {
                    SourceValue = valueInstances[src],
                    DestinationValue = simpleFormulaCache[dst]
                });
            }

            #endregion

            #region SimpleFormula -> Param

            foreach (var valueFlowConnector in valueFlowConnectors.Where(x => x.GenericDstEnd.Kind == "Parameter" && x.GenericSrcEnd.Kind == "SimpleFormula"))
            {
                var src = CyPhyClasses.SimpleFormula.Cast(valueFlowConnector.GenericSrcEnd.Impl);
                var dst = CyPhyClasses.Parameter.Cast(valueFlowConnector.GenericDstEnd.Impl);

                if (!valueInstances.ContainsKey(dst) || !simpleFormulaCache.ContainsKey(src)) continue;

                dm.Connectors.AddItem(new ValueConnector
                {
                    SourceValue = simpleFormulaCache[src],
                    DestinationValue = valueInstances[dst]
                });
            }

            #endregion

            #region Property -> SimpleFormula

            foreach (var valueFlowConnector in valueFlowConnectors.Where(x => x.GenericSrcEnd.Kind == "Property" && x.GenericDstEnd.Kind == "SimpleFormula"))
            {
                var src = CyPhyClasses.Property.Cast(valueFlowConnector.GenericSrcEnd.Impl);
                var dst = CyPhyClasses.SimpleFormula.Cast(valueFlowConnector.GenericDstEnd.Impl);

                if (!valueInstances.ContainsKey(src) || !simpleFormulaCache.ContainsKey(dst)) continue;

                dm.Connectors.AddItem(new ValueConnector
                {
                    SourceValue = valueInstances[src],
                    DestinationValue = simpleFormulaCache[dst]
                });
            }

            #endregion

            #region SimpleFormula -> Property

            foreach (var valueFlowConnector in valueFlowConnectors.Where(x => x.GenericDstEnd.Kind == "Property" && x.GenericSrcEnd.Kind == "SimpleFormula"))
            {
                var src = CyPhyClasses.SimpleFormula.Cast(valueFlowConnector.GenericSrcEnd.Impl);
                var dst = CyPhyClasses.Property.Cast(valueFlowConnector.GenericDstEnd.Impl);

                if (!valueInstances.ContainsKey(dst) || !simpleFormulaCache.ContainsKey(src)) continue;

                dm.Connectors.AddItem(new ValueConnector
                {
                    SourceValue = simpleFormulaCache[src],
                    DestinationValue = valueInstances[dst]
                });
            }

            #endregion

            #region Param -> Param

            foreach (var valueFlowConnector in valueFlowConnectors.Where(x => x.GenericDstEnd.Kind == "Parameter" && x.GenericSrcEnd.Kind == "Parameter"))
            {
                var src = CyPhyClasses.Parameter.Cast(valueFlowConnector.GenericSrcEnd.Impl);
                var dst = CyPhyClasses.Parameter.Cast(valueFlowConnector.GenericDstEnd.Impl);

                if (!valueInstances.ContainsKey(dst) || !valueInstances.ContainsKey(src)) continue;

                dm.Connectors.AddItem(new ValueConnector
                {
                    SourceValue = valueInstances[src],
                    DestinationValue = valueInstances[dst]
                });
            }

            #endregion
        }

       

        private static Dictionary<CyPhyClasses.WeldedJoin.AttributesClass.InspectionRequirement_enum,Assembly.inspectionRequirement> d_InspectionRequirement = new Dictionary<CyPhyClasses.WeldedJoin.AttributesClass.InspectionRequirement_enum,Assembly.inspectionRequirement>() {
            { CyPhyClasses.WeldedJoin.AttributesClass.InspectionRequirement_enum.Visual, Assembly.inspectionRequirement.Visual },
            { CyPhyClasses.WeldedJoin.AttributesClass.InspectionRequirement_enum.X_dash_Ray, Assembly.inspectionRequirement.XRay },
            //{ CyPhyClasses.WeldedJoin.AttributesClass.InspectionRequirement_enum.Ultrasonic, Assembly.inspectionRequirement.Ultrasonic }
        };

        private static Dictionary<CyPhyClasses.MechanicalJoin.AttributesClass.TorqueUnit_enum, Assembly.torqueUnit> d_TorqueUnits = new Dictionary<CyPhyClasses.MechanicalJoin.AttributesClass.TorqueUnit_enum, Assembly.torqueUnit>() {
            { CyPhyClasses.MechanicalJoin.AttributesClass.TorqueUnit_enum.N_m, Assembly.torqueUnit.Nm }
        };

        private static Dictionary<CyPhyClasses.MechanicalJoin.AttributesClass.FasteningMethod_enum, Assembly.fasteningMethod> d_FasteningMethod = new Dictionary<CyPhyClasses.MechanicalJoin.AttributesClass.FasteningMethod_enum,Assembly.fasteningMethod>() {
            { CyPhyClasses.MechanicalJoin.AttributesClass.FasteningMethod_enum.Bolted, Assembly.fasteningMethod.Bolted }
            //{ CyPhyClasses.MechanicalJoin.AttributesClass.FasteningMethod_enum.Bolted__Blind_Hole_, Assembly.fasteningMethod.BoltedBlindHole},
            //{ CyPhyClasses.MechanicalJoin.AttributesClass.FasteningMethod_enum.Bolted__Through_Hole_, Assembly.fasteningMethod.BoltedThroughHole},
            //{ CyPhyClasses.MechanicalJoin.AttributesClass.FasteningMethod_enum.Machine_Screwed, Assembly.fasteningMethod.MachineScrewed},
            //{ CyPhyClasses.MechanicalJoin.AttributesClass.FasteningMethod_enum.Press_Fitted, Assembly.fasteningMethod.PressFitted},
            //{ CyPhyClasses.MechanicalJoin.AttributesClass.FasteningMethod_enum.Snap_Fitted, Assembly.fasteningMethod.SnapFitted},
        };

        //private static AVM.iFAB.Join BuildJoinData(CyPhy.JoinData jd)
        //{
        //    AVM.iFAB.Join j_rtn = new AVM.iFAB.Join();

        //    if (jd.Attributes.OverrideSemantics == CyPhyClasses.JoinData.AttributesClass.OverrideSemantics_enum.Override)
        //        j_rtn.OverrideSemantics = AVM.iFAB.JoinOverrideSemantics.Override;
        //    else if (jd.Attributes.OverrideSemantics == CyPhyClasses.JoinData.AttributesClass.OverrideSemantics_enum.Union)
        //        j_rtn.OverrideSemantics = AVM.iFAB.JoinOverrideSemantics.Union;

        //    Assembly.assemblyDetails ad = new Assembly.assemblyDetails();
            
        //    if (jd is CyPhy.WeldedJoin || jd.Kind == "WeldedJoin")
        //    {
        //        CyPhy.WeldedJoin wj = jd as CyPhy.WeldedJoin;
        //        if (wj == null)
        //            wj = CyPhyClasses.WeldedJoin.Cast(jd.Impl);
                
        //        Assembly.welded w = new Assembly.welded();                

        //        w.inspectionRequirement = d_InspectionRequirement[wj.Attributes.InspectionRequirement];
        //        w.jointType = (Assembly.jointType)Enum.Parse(typeof(Assembly.jointType),wj.Attributes.JointType.ToString());

        //        w.length = new Assembly.length();
        //        w.length.Value = decimal.Parse(wj.Attributes.Length.ToString());
        //        w.length.unit = (Assembly.lengthUnit)Enum.Parse(typeof(Assembly.lengthUnit),wj.Attributes.LengthUnit.ToString());
                
        //        w.weldPenetration = (Assembly.weldPenetration)Enum.Parse(typeof(Assembly.weldPenetration), wj.Attributes.WeldPenetration.ToString());
        //        w.weldType = (Assembly.weldType)Enum.Parse(typeof(Assembly.weldType), wj.Attributes.WeldType.ToString());

        //        w.twoSided = wj.Attributes.TwoSided;

        //        ad.Item = w;
        //    }
        //    else if (jd is CyPhy.BrazedJoin || jd.Kind == "BrazedJoin")
        //    {
        //        CyPhy.BrazedJoin bj = jd as CyPhy.BrazedJoin;
        //        if (bj == null)
        //            bj = CyPhyClasses.BrazedJoin.Cast(jd.Impl);
        //        Assembly.brazed b = new Assembly.brazed();

        //        b.fillerMaterial = new Assembly.material();
        //        b.fillerMaterial.Value = bj.Attributes.FillerMaterial;
        //        b.fluxMaterial = new Assembly.material();
        //        b.fluxMaterial.Value = bj.Attributes.FluxMaterial;

        //        b.length = new Assembly.length();
        //        b.length.Value = decimal.Parse(bj.Attributes.Length.ToString());
        //        b.length.unit = (Assembly.lengthUnit)Enum.Parse(typeof(Assembly.lengthUnit), bj.Attributes.LengthUnit.ToString());

        //        ad.Item = b;
        //    }
        //    else if (jd is CyPhy.SolderedJoin || jd.Kind == "SolderedJoin")
        //    {
        //        CyPhy.SolderedJoin sj = jd as CyPhy.SolderedJoin;
        //        if (sj == null)
        //            sj = CyPhyClasses.SolderedJoin.Cast(jd.Impl);
        //        Assembly.soldered s = new Assembly.soldered();

        //        s.fillerMaterial = new Assembly.material();
        //        s.fillerMaterial.Value = sj.Attributes.FillerMaterial;
        //        s.fluxMaterial = new Assembly.material();
        //        s.fluxMaterial.Value = sj.Attributes.FluxMaterial;

        //        s.length = new Assembly.length();
        //        s.length.Value = decimal.Parse(sj.Attributes.Length.ToString());
        //        s.length.unit = (Assembly.lengthUnit)Enum.Parse(typeof(Assembly.lengthUnit), sj.Attributes.LengthUnit.ToString());

        //        ad.Item = s;
        //    }
        //    else if (jd is CyPhy.GluedJoin || jd.Kind == "GluedJoin")
        //    {
        //        CyPhy.GluedJoin gj = jd as CyPhy.GluedJoin;
        //        if (gj == null)
        //            gj = CyPhyClasses.GluedJoin.Cast(jd.Impl);
        //        Assembly.glued g = new Assembly.glued();

        //        g.length = new Assembly.length();
        //        g.length.Value = decimal.Parse(gj.Attributes.Length.ToString());
        //        g.length.unit = (Assembly.lengthUnit)Enum.Parse(typeof(Assembly.lengthUnit), gj.Attributes.LengthUnit.ToString());

        //        g.material = new Assembly.material();
        //        g.material.Value = gj.Attributes.Material;

        //        g.volume = new Assembly.volume();
        //        g.volume.Value = decimal.Parse(gj.Attributes.Volume.ToString());
        //        g.volume.unit = (Assembly.volumeUnit)Enum.Parse(typeof(Assembly.volumeUnit), gj.Attributes.LengthUnit.ToString());

        //        ad.Item = g;
        //    }
        //    else if (jd is CyPhy.MechanicalJoin || jd.Kind == "MechanicalJoin")
        //    {
        //        CyPhy.MechanicalJoin mj = jd as CyPhy.MechanicalJoin;
        //        if (mj == null)
        //            mj = CyPhyClasses.MechanicalJoin.Cast(jd.Impl);
        //        Assembly.mechanical m = new Assembly.mechanical();

        //        m.fasteningMethod = d_FasteningMethod[mj.Attributes.FasteningMethod];
        //        m.fasteningQuantity = mj.Attributes.FasteningQuantity.ToString();
                
        //        m.torque = new Assembly.torque();
        //        m.torque.Value = decimal.Parse( mj.Attributes.Torque.ToString() );
        //        m.torque.unit = d_TorqueUnits[mj.Attributes.TorqueUnit];

        //        List<Assembly.component> lc_Fasteners = new List<Assembly.component>();
        //        foreach (CyPhy.Fastener f in mj.Children.FastenerCollection)
        //        {
        //            Assembly.component c = new Assembly.component();
        //            c.id = f.Referred.Component.Attributes.AVMID;
        //            c.quantity = f.Attributes.Quantity.ToString();

        //            lc_Fasteners.Add(c);
        //        }
        //        m.components = lc_Fasteners.ToArray();

        //        ad.Item = m;
        //    }
        //    ad.Item.description = jd.Attributes.Description;

        //    j_rtn.Data = Serializer.Serialize(ad);

        //    return j_rtn;
        //}

        //Add port group to the design model
        private static void ConvertPowerFlows(IEnumerable<HashSet<CyPhy.Port>> portGroups, DesignModel dm, Dictionary<CyPhy.Port, AbstractPort> allPorts)
        {
            foreach (var currentGroup in portGroups)
            {
                var pc = new PortConnector { EndPoints = new List<AbstractPort>() };
                foreach (var h in currentGroup.Where(allPorts.ContainsKey))
                {
                    var a = allPorts[h];
                    pc.EndPoints.AddItem(a);
                }
                dm.Connectors.AddItem(pc);
            }
        }

        //Add port group to the design model from joinstructures
        private static void ConvertJoinStrucutresInGroups(IEnumerable<HashSet<CyPhy.Port>> portGroups, DesignModel dm, Dictionary<CyPhy.Port, AbstractPort> allPorts)
        {
            foreach (var currentGroup in portGroups)
            {
                var pc = new PortConnector { EndPoints = new List<AbstractPort>() };
                foreach (var h in currentGroup.Where(allPorts.ContainsKey))
                {
                    var a = allPorts[h];
                    pc.EndPoints.AddItem(a);
                }
                dm.Connectors.AddItem(pc);
            }
        }

        

        
        private static void UpdateSubContainers(DesignPrimitivesWrapper de,
                                                Container rootContainerType,
                                                DesignModel rootDesignModel,
                                                Dictionary<CyPhy.Port, PortInstance> cyphyPorts2PortInstances,
                                                Dictionary<CyPhy.Port, ContainerPort> containerPorts,
                                                List<Connection> connectionsToBeVisited,
                                                Dictionary<CyPhy.SimpleFormula, ContainerCalculation> simpleFormulas,
                                                Dictionary<CyPhy.ValueFlowTarget, AVM.META.Design.ValueType> valueInstances,
                                                Dictionary<CyPhy.Port, StructuralInterfaceInstance> innerStructuralPortInstances,
                                                Dictionary<CyPhy.Port, ContainerStructuralInterface> containerStructuralPortInstances)
        {
            System.Diagnostics.Debug.WriteLine(de.Path);

            // Init list
            if (rootContainerType.ContainerPorts==null) rootContainerType.ContainerPorts=new List<ContainerPort>();
            rootContainerType.ComponentInstances = new List<ComponentInstance>();

            // Add Powerflows, JointStrucutres and ValueFlows for later steps
            connectionsToBeVisited.AddRange(de.ValueFlows);

            
            #region Handle ComponentRefs
            var componentRefs = de.ComponentRefs;
            foreach (var compRef in componentRefs)
            {
                var isComponent = false;
                try
                {
                    var c = compRef.Referred.Component;
                    // line above throws an exception if it is not a component
                    isComponent = true;
                }
                catch
                {
                }

                if (compRef != null && isComponent)
                {
                    CyPhy.Component comp = compRef.Referred.Component;
                    System.Diagnostics.Debug.WriteLine(comp.Path);

                    ComponentInstance compInst = Component2ComponentInstance(comp);
                    rootContainerType.ComponentInstances.AddItem(compInst);
                    compInst.Name = compRef.Name;
                    //compInst.id = GetOrSetID(compRef);
                    //compInst.id = compRef.Guid.ToString();

                    String instanceGUID = compRef.Attributes.InstanceGUID;
                    // Set this value if not set
                    if (String.IsNullOrWhiteSpace(instanceGUID))
                    {
                        compRef.Attributes.InstanceGUID = compRef.Guid.ToString();
                        instanceGUID = compRef.Attributes.InstanceGUID;
                    }
                    compInst.id = compRef.Attributes.InstanceGUID;


                    String s_SrcDesignSpaceContainerID = GetDesignSpaceSourceObjectID(compRef);
                    if ( ! String.IsNullOrWhiteSpace(s_SrcDesignSpaceContainerID))
                        compInst.SrcDesignSpaceContainerID = s_SrcDesignSpaceContainerID;

                    // Update Component's PortInstances
                    compInst.PortInstances = new List<PortInstance>();
                    foreach (CyPhy.Port port in comp.Children.PortCollection)
                    {
#if TRACK_PORT_ID
                        var portInst = new PortInstance { IDinSourceModel = port.ID };
#else

                        var portInst = new PortInstance { IDinSourceModel = port.Attributes.ID };
#endif
                        //var portInst = new PortInstance { IDinSourceModel = port.ID };
                        compInst.PortInstances.AddItem(portInst);

                        if (!cyphyPorts2PortInstances.ContainsKey(port))
                        {
                            cyphyPorts2PortInstances.Add(port, portInst); // Update Ports list
                        }
                    }

                    // Convert parameters
                    foreach (var p in comp.Children.ParameterCollection)
                    {
                        double result=0;
                        double.TryParse(p.Attributes.Value, out result);
                        var componentNamedValueInstance = new ComponentNamedValueInstance { IDinComponentModel = p.Attributes.ID, Value = result};
                        compInst.NamedValueInstances.AddItem(componentNamedValueInstance);
                        if (valueInstances.ContainsKey(p)) continue;

                        valueInstances.Add(p, componentNamedValueInstance);
                    }

                    // Convert properties
                    foreach (var p in comp.Children.PropertyCollection)
                    {
                        double result = 0;
                        double.TryParse(p.Attributes.Value, out result);
                        var componentNamedValueInstance = new ComponentNamedValueInstance { IDinComponentModel = p.Attributes.ID, Value = result };
                        compInst.NamedValueInstances.AddItem(componentNamedValueInstance);
                        if (valueInstances.ContainsKey(p)) continue;

                        valueInstances.Add(p, componentNamedValueInstance);
                    }
                }
            }
            #endregion

            #region Handle Component instances
            List<CyPhy.Component> lcChildComps = de.Components;

            foreach (CyPhy.Component comp in lcChildComps)
            {
                System.Diagnostics.Debug.WriteLine(comp.Path);

                ComponentInstance compInst = Component2ComponentInstance(comp);
                rootContainerType.ComponentInstances.AddItem(compInst);
                
                // Update Component's PortInstances
                compInst.PortInstances = new List<PortInstance>();
                foreach (CyPhy.Port port in comp.Children.PortCollection)
                {

#if TRACK_PORT_ID
                    var structPort = new StructuralInterfaceInstance {IDinSourceModel = port.ID};
#else
                    var structPort = new StructuralInterfaceInstance { IDinSourceModel = port.Attributes.ID };
#endif
                    if (!innerStructuralPortInstances.ContainsKey(port))
                    {
                        innerStructuralPortInstances.Add(port, structPort);
                        compInst.StructuralInterfaceInstances.Add(structPort);
                    }

#if TRACK_PORT_ID
                    var portInst = new PortInstance { IDinSourceModel = port.ID };
#else
                    var portInst = new PortInstance { IDinSourceModel = port.Attributes.ID };
#endif
                    compInst.PortInstances.AddItem(portInst);

                    if (!cyphyPorts2PortInstances.ContainsKey(port))
                    {
                        cyphyPorts2PortInstances.Add(port, portInst); // Update Ports list
                    }
                }

                // Convert parameters
                foreach (var p in comp.Children.ParameterCollection)
                {
                    double result = 0;
                    double.TryParse(p.Attributes.Value, out result);
                    var componentNamedValueInstance = new ComponentNamedValueInstance { IDinComponentModel = p.Attributes.ID, Value = result};
                    compInst.NamedValueInstances.AddItem(componentNamedValueInstance);
                    if (valueInstances.ContainsKey(p)) continue;

                    valueInstances.Add(p, componentNamedValueInstance);
                }

                // Convert properties
                foreach (var p in comp.Children.PropertyCollection)
                {
                    double result = 0;
                    double.TryParse(p.Attributes.Value, out result);
                    var componentNamedValueInstance = new ComponentNamedValueInstance { IDinComponentModel = p.Attributes.ID, Value = result};
                    compInst.NamedValueInstances.AddItem(componentNamedValueInstance);
                    if (valueInstances.ContainsKey(p)) continue;

                    valueInstances.Add(p, componentNamedValueInstance);
                }
            }
            #endregion

            // Recursively add all ContainerTypes from sub-DesignEntities
            rootContainerType.Containers = new List<Container>();
            var ldeDesignEntityChildren = de.ComponentAssemblies.Cast<CyPhy.DesignEntity>().ToList();
            ldeDesignEntityChildren.AddRange(de.DesignContainers);

            foreach (CyPhy.DesignEntity subDE in ldeDesignEntityChildren)
            {
                Container subRootContainerType;
                if (subDE is CyPhy.DesignContainer &&
                    (subDE as CyPhy.DesignContainer).Attributes.ContainerType == CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum.Compound)
                {
                    subRootContainerType = new Compound();
                }
                else if (subDE is CyPhy.DesignContainer &&
                         (subDE as CyPhy.DesignContainer).Attributes.ContainerType == CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum.Alternative)
                {
                    subRootContainerType = new Alternative();
                }
                else if (subDE is CyPhy.DesignContainer &&
                         (subDE as CyPhy.DesignContainer).Attributes.ContainerType == CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum.Optional)
                {
                    subRootContainerType = new Optional();
                }
                else if (subDE is CyPhy.ComponentAssembly)
                {
                    subRootContainerType = new Compound();
                }
                else
                {
                    continue;
                }

                subRootContainerType.Name = subDE.Name;
                subRootContainerType.id = GetOrSetID(subDE);
                
                rootContainerType.Containers.AddItem(subRootContainerType);
                
                UpdateSubContainers(new DesignPrimitivesWrapper(subDE), subRootContainerType, rootDesignModel, cyphyPorts2PortInstances, containerPorts, connectionsToBeVisited, simpleFormulas, valueInstances, innerStructuralPortInstances, containerStructuralPortInstances);
            }

            #region Elements on the current container

            // Convert simple formulas
            foreach (var sf in de.SimpleFormulas)
            {
                var containerCalculation = new ContainerCalculation();
                rootContainerType.ContainerValues.AddItem(containerCalculation);
                simpleFormulas[sf] = containerCalculation;
            }

            // Add container ports (from ports)
            foreach (var port in de.Ports)
            {
                if (containerPorts.ContainsKey(port)) continue;

                var pi = new ContainerPort();
                containerPorts.Add(port, pi);
                rootContainerType.ContainerPorts.AddItem(pi);
            }

            // Convert parameters
            foreach (var p in de.Parameters)
            {
                var containerNamedValue = new ContainerNamedValue {IsParameter = true, Value = p.Attributes.Value};
                rootContainerType.ContainerValues.AddItem(containerNamedValue);
                if (valueInstances.ContainsKey(p)) continue;

                valueInstances.Add(p, containerNamedValue);
            }

            // Convert properties
            foreach (var p in de.Properties)
            {
                var containerNamedValue = new ContainerNamedValue { IsParameter = false, Value = p.Attributes.Value };
                rootContainerType.ContainerValues.AddItem(containerNamedValue);
                if (valueInstances.ContainsKey(p)) continue;

                valueInstances.Add(p, containerNamedValue);
            }

            #endregion
        }

        public static ComponentInstance Component2ComponentInstance(CyPhy.Component comp)
        {
            ComponentInstance compInst = new ComponentInstance();
            if (comp.Attributes.AVMID != "")
                compInst.ComponentID = comp.Attributes.AVMID;
            else
            {
                // If there's no AVMID, then make one up, and save it to the component
                String s_GUID = Guid.NewGuid().ToString();
                compInst.ComponentID = s_GUID;
                comp.Attributes.AVMID = s_GUID;
            }
            compInst.ComponentRevision = comp.Attributes.Revision;
            compInst.ComponentVersion = comp.Attributes.Version;
            compInst.Name = comp.Name;
            compInst.NamedValueInstances = new List<ComponentNamedValueInstance>();
            compInst.id = comp.Attributes.InstanceGUID;

            return compInst;
        }
        
        private static Dictionary<CyPhy.Port, List<Connection>> _srcConnectionCache = new Dictionary<CyPhy.Port, List<Connection>>();
        private static IEnumerable<Connection> GetAllSrcConnection(CyPhy.Port port)
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

        private static Dictionary<CyPhy.Port, List<Connection>> _dstConnectionCache = new Dictionary<CyPhy.Port, List<Connection>>();
        private static IEnumerable<Connection> GetAllDstConnection(CyPhy.Port port)
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

        private static Dictionary<CyPhy.Port, Dictionary<ConnectionTypes, List<CyPhy.Port>>> _reachablePortsCache = new Dictionary<CyPhy.Port, Dictionary<ConnectionTypes, List<CyPhy.Port>>>();
        private static IEnumerable<CyPhy.Port> GetReachablePorts(CyPhy.Port port, ConnectionTypes conType, IList<PortTypes> portTypes, IEnumerable<Connection> connections = null)
        {
            if (!_reachablePortsCache.ContainsKey(port) || !_reachablePortsCache[port].ContainsKey(conType))
            {
                var result = new List<CyPhy.Port>();
                var processedConnection = new List<Connection>();
                foreach (var connection in GetAllDstConnection(port)
                                           .Where(x=> connections==null||connections.Any(y=>y.ID==x.ID))
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

        private static IEnumerable<CyPhy.Port> GetReachablePorts(CyPhy.Port port, ConnectionTypes conType, IEnumerable<Connection> connections = null)
        {
            return GetReachablePorts(port, conType, new List<PortTypes> {PortTypes.All}, connections);
        }

        private static IEnumerable<CyPhy.Port> GetReachableStructuralPorts(CyPhy.Port port, ConnectionTypes conType, IEnumerable<Connection> connections = null)
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
        private static void GroupPorts(CyPhy.Port port, ICollection<CyPhy.Port> results, ConnectionTypes connectionType, IEnumerable<Connection> connections = null)
        {
            foreach (var nextPort in GetReachablePorts(port, connectionType, connections))
            {
                if(results.Any(x=>x.ID == nextPort.ID)) continue;
                
                results.Add(nextPort);
                GroupPorts(nextPort, results, connectionType, connections);
            }
        }

        private static void GroupStructuralPorts(CyPhy.Port port, ICollection<CyPhy.Port> results, ConnectionTypes connectionType, IEnumerable<Connection> connections = null)
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
}
