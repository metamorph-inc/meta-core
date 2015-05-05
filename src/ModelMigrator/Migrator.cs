using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using GME.CSharp;
using GME;
using GME.MGA.Meta;
//using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
//using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;

namespace ModelMigrator
{
    class Migrator
    {
        // For each obsolete CyPhyML concept, 
        // store the object in the PortLib that should replace it.
        private Dictionary<String, MgaFCO> d_OldKindToNewPortArchetype;
        private Dictionary<String, MgaFCO> d_OldKindToNewPortArchetype_jk;

        // During migration, map old port objects to their replacements.
        private Dictionary<MgaFCO, MgaFCO> dfco_OldToNewMap;

        // Dictionary of archetype ports from old Paradigm, name_FCO
        private Dictionary<string, MgaFCO> d_longName2MgaFCO;

        // List of old Port FCOs
        private List<MgaFCO> oldPortList;
        private List<MgaFCO> oldPortBasetypeList;

        private MgaProject project;
        GMEConsole GMEConsole { get; set; }

        #region JamesMethods

        public void objectInfo(MgaFCO currentObj)
        {
            string debug = "yeah";
        }

        Dictionary<string, MgaFCO> modelicaPortLib = new Dictionary<string, MgaFCO>();
        List<MgaFCO> componentArchetypes = new List<MgaFCO>();
        List<MgaFCO> componentAssemblies = new List<MgaFCO>();
        List<MgaFCO> designContainers = new List<MgaFCO>();
        List<MgaFCO> testComponents = new List<MgaFCO>();
        List<MgaFCO> testbenches = new List<MgaFCO>();

        public void convertFluidParameters(MgaProject project)
        {
            MgaFilter modelicaFluidPortFilter = project.CreateFilter();
            modelicaFluidPortFilter.ObjType = "OBJTYPE_MODEL";
            modelicaFluidPortFilter.Kind = "ModelicaConnector";
            var modelicaFluidPortArchetypes = 
                project
                .AllFCOs(modelicaFluidPortFilter)
                .Cast<MgaModel>()
                .Where(x => x.ChildFCOs.OfType<MgaAtom>().ToList().Count() != 0 &&
                     x.ParentModel != null &&
                    x.ArcheType == null); //.ToList()

            //GMEConsole.Info.WriteLine("Number of FluidPort Archetypes: {0}", modelicaFluidPortArchetypes.Count());

            foreach (var port in modelicaFluidPortArchetypes)
            {
                //GMEConsole.Out.WriteLine("{0}", port.StrAttrByName["Class"]);

                try
                {
                    MgaAtom packageParameter =
                        port.ChildFCOs.OfType<MgaAtom>()
                        .Where(x => x.Name == "redeclare package Medium").FirstOrDefault();
                    if (packageParameter != null)
                    {
                        MgaFCO modelicaRedeclare = makeFCO(port, "ModelicaRedeclare");
                        modelicaRedeclare.Name = "Medium";
                        modelicaRedeclare.StrAttrByName["ModelicaRedeclareType"] = "Package";
                        modelicaRedeclare.StrAttrByName["Value"] = packageParameter.StrAttrByName["Value"];
                    }
                }
                catch (Exception ex)
                {
                    GMEConsole.Warning.WriteLine(
                        "Could not make ModelicaRedeclare block within {0}.{1}",
                        port.ParentModel.Name,
                        port.Name);
                    GMEConsole.Error.WriteLine("{0}", ex.Message);
                    GMEConsole.Info.WriteLine("{0}", ex.StackTrace);
                }
            }
        }

        public void getObjectsByFilter(MgaProject project)
        {
            //MgaFilter componentFilter = project.CreateFilter();
            //componentFilter.ObjType = "OBJTYPE_MODEL";
            //componentFilter.Kind = "Component";
            //componentArchetypes = project.AllFCOs(componentFilter).Cast<MgaFCO>().Where(x => x.ArcheType == null).ToList();

            //foreach (var c in componentArchetypes)
            //{
            //    string path = c.AbsPath;
            //    string[] splitPath = path.Split('/');
            //    string newPath = "";
            //    foreach (var s in splitPath)
            //    {
            //        newPath = newPath + s.Split('|').FirstOrDefault();
            //    }
            //    GMEConsole.Out.WriteLine("{0}", newPath);
            //}

            //MgaFilter assemblyFilter = project.CreateFilter();
            //assemblyFilter.ObjType = "OBJTYPE_MODEL";
            //assemblyFilter.Kind = "ComponentAssembly";
            //componentAssemblies = project.AllFCOs(assemblyFilter).Cast<MgaFCO>().ToList();

            //MgaFilter designContainerFilter = project.CreateFilter();
            //designContainerFilter.ObjType = "OBJTYPE_MODEL";
            //designContainerFilter.Kind = "DesignContainer";
            //designContainers = project.AllFCOs(designContainerFilter).Cast<MgaFCO>().ToList();

            //MgaFilter testCompFilter = project.CreateFilter();
            //testCompFilter.ObjType = "OBJTYPE_MODEL";
            //testCompFilter.Kind = "TestComponent";
            //testComponents = project.AllFCOs(testCompFilter).Cast<MgaFCO>().Where(x => x.ArcheType == null).ToList();

            //MgaFilter testbenchFilter = project.CreateFilter();
            //testbenchFilter.ObjType = "OBJTYPE_MODEL";
            //testbenchFilter.Kind = "TestBench";
            //testbenches = project.AllFCOs(testbenchFilter).Cast<MgaFCO>().ToList();

            MgaFilter portLibFilter = project.CreateFilter();
            portLibFilter.ObjType = objtype_enum.OBJTYPE_MODEL.ToString();
            portLibFilter.Kind = "ModelicaConnector";

            var newPortTypes = project.AllFCOs(portLibFilter).Cast<MgaFCO>().Where(x => x.ParentFolder != null);

            foreach (var pType in newPortTypes)
            {
                if (modelicaPortLib.ContainsKey(pType.Name))
                {
                    continue;
                }
                modelicaPortLib.Add(pType.Name, pType);
            }

            d_OldKindToNewPortArchetype_jk = new Dictionary<string, MgaFCO>() 
            {
                { "MultibodyFramePowerPort" , modelicaPortLib["MultibodyFrame"] },
                { "MultibodyFrame" , modelicaPortLib["MultibodyFrame"] },
                { "ModelicaAggregateInterface" , modelicaPortLib["FlangeWithBearing"] },
                { "AggregatePort" , modelicaPortLib["FlangeWithBearing"] },
                { "HydraulicPowerPort" , modelicaPortLib["FluidPort"] },
                { "FluidPort" , modelicaPortLib["FluidPort"] },
                { "HeatPort" , modelicaPortLib["HeatPort"] },
                { "ThermalPowerPort" , modelicaPortLib["HeatPort"] },
                { "ElectricalPowerPort" , modelicaPortLib["ElectricalPin"] },
                { "ElectricalPin" , modelicaPortLib["ElectricalPin"] },
                { "InputSignalPort" , modelicaPortLib["RealInput"] },
                { "OutputSignalPort" , modelicaPortLib["RealOutput"] },
                { "RealInput" , modelicaPortLib["RealInput"] },
                { "RealOutput" , modelicaPortLib["RealOutput"] },
                { "BooleanInput" , modelicaPortLib["BooleanInput"] },
                { "BooleanOutput" , modelicaPortLib["BooleanOutput"] },
                { "IntegerInput" , modelicaPortLib["IntegerInput"] },
                { "IntegerOutput" , modelicaPortLib["IntegerOutput"] },
                { "TranslationalPowerPort" , modelicaPortLib["TranslationalFlange"] },
                { "TranslationalFlange" , modelicaPortLib["TranslationalFlange"] },
                { "RotationalPowerPort" , modelicaPortLib["RotationalFlange"] },
                { "RotationalFlange" , modelicaPortLib["RotationalFlange"] },
                { "Modelica.Blocks.Interfaces.BooleanInput" , modelicaPortLib["BooleanInput"] },
                { "Modelica.Blocks.Interfaces.BooleanOutput" , modelicaPortLib["BooleanOutput"] },
                { "Modelica.Blocks.Interfaces.IntegerInput" , modelicaPortLib["IntegerInput"] },
                { "Modelica.Blocks.Interfaces.IntegerOutput" , modelicaPortLib["IntegerOutput"] },
                { "C2M2L_Ext.Electrical.Interfaces.Battery_Bus" , modelicaPortLib["BatteryBus"] },
                { "C2M2L_Ext.Interfaces.Context_Interfaces.Driver.Driver_Bus" , modelicaPortLib["DriverBus"] },
                { "Modelica.Mechanics.MultiBody.Interfaces.FlangeWithBearing" , modelicaPortLib["FlangeWithBearing"] },
                { "C2M2L_Ext.Fluid.Hydraulics_Simple.Interfaces.Hydraulic_Port" , modelicaPortLib["HydraulicPower"] },
                { "C2M2L_Ext.C2M2L_Component_Building_Blocks.Drive_Line.Brakes_Clutch.Common_Controls.Brake_Control_Bus" , modelicaPortLib["BrakeControlBus"] },
                { "C2M2L_Ext.Electrical.Interfaces.Electric_Machine_Bus" , modelicaPortLib["ElectricMachineBus"] },
                { "C2M2L_Ext.C2M2L_Component_Building_Blocks.Prime_Movers.Common_Controls.Engine_Control_Bus" , modelicaPortLib["EngineControlBus"] },
                { "C2M2L_Ext.C2M2L_Component_Building_Blocks.Drive_Line.Range_Packs.Common_Controls.Range_Pack_Control_Bus" , modelicaPortLib["RangePackControlBus"] },
                { "C2M2L_Ext.C2M2L_Component_Building_Blocks.Drive_Line.Cross_Drive_Steering.Common_Controls.Steering_Control_Bus" , modelicaPortLib["SteeringControlBus"] },
                { "C2M2L_Ext.C2M2L_Component_Building_Blocks.Drive_Line.Torque_Converters.Common_Controls.Torque_Converter_Control_Bus" , modelicaPortLib["TorqueConverterControlBus"] },
                { "C2M2L_Ext.Interfaces.Context_Interfaces.Atmospheric_Environment.Atmospheric_Environment_Bus" , modelicaPortLib["AtmosphericEnvironmentBus"] },
            };
        }

        private MgaModel makeCopyOfModel(MgaModel originalModel, string nameModifier)
        {
            MgaModel copyForMigration = null;

            if (originalModel.ParentFolder != null)
            {
                MgaFolder parentFolder = originalModel.ParentFolder;
                copyForMigration = (MgaModel)parentFolder.CopyFCODisp(originalModel as MgaFCO);
                copyForMigration.Name = originalModel.Name + nameModifier;
            }
            //else if (originalModel.ParentModel != null)
            //{
            //    GMEConsole.Info.WriteLine(
            //        "Did not copy: <a href=\"mga:{0}\">{1}</a>. (not a Root Object)",
            //        originalModel.ID,
            //        originalModel.Name);
            //}

            return copyForMigration;
        }

        #endregion

        public Migrator(MgaProject project)
        {
            GMEConsole = GMEConsole.CreateFromProject(project);
            dfco_OldToNewMap = new Dictionary<MgaFCO, MgaFCO>();
            //SeedPortMap(project);
            getObjectsByFilter(project);
        }

        // Seeds our map of old port classes to new port archetypes.
        // These new archetypes will be instantiated to replace the old port types.
        private void SeedPortMap(MgaProject project)
        {
            this.project = project;

            // Find the PortLib folder with the port definitions.
            // Add each object to the local dictionary, with the last
            // two nodes of its path as the key.
            // This will give us what we need: <Domain>\<Port>

            //MgaFilter newFilter = project.CreateFilter();
            //var allFCOs = project.AllFCOs(newFilter).Cast<IMgaFCO>();

            Dictionary<String, MgaFCO> d_AllObjectsInPortFolder = new Dictionary<string, MgaFCO>();
            Dictionary<String, MgaFCO> d_ModelicaPortLib = new Dictionary<string, MgaFCO>();

            foreach (MgaFolder folder in project.RootFolder.ChildFolders)
            {
                if (folder.Name.Contains("CyPhy_PortLib"))
                {
                    List<MgaFolder> lf_Folders = RecursivelyGetAllFolders(folder);
                    foreach (MgaFolder subfolder in lf_Folders)
                    {
                        foreach (MgaFCO fco in subfolder.ChildFCOs)
                        {
                            string s_QualifiedName = fco.ParentFolder.Name + '\\' + fco.Name;
                            d_AllObjectsInPortFolder[s_QualifiedName] = fco;
                            if (subfolder.Name.Contains("Modelica"))
                            {
                                string portName = fco.Name;
                                d_ModelicaPortLib[portName] = fco;
                            }
                        }
                    }
                }
            }

            // Map obsolete port classes to archetypes found in the port library.
            //d_OldKindToNewPortArchetype = new Dictionary<String,MgaFCO>() {
            //    { "MultibodyFramePowerPort" , d_AllObjectsInPortFolder["Modelica Ports\\MultibodyFrame"] },
            //    { "MultibodyFrame" , d_AllObjectsInPortFolder["Modelica Ports\\MultibodyFrame"] },
            //    { "ModelicaBusPortInterface" , d_AllObjectsInPortFolder["Modelica Ports\\Bus"] },
            //    { "BusPort" , d_AllObjectsInPortFolder["Abstract Ports\\Bus"] },
            //    { "ModelicaAggregateInterface" , d_AllObjectsInPortFolder["Modelica Ports\\FlangeWithBearing"] },
            //    { "AggregatePort" , d_AllObjectsInPortFolder["Modelica Ports\\FlangeWithBearing"] },
            //    { "HydraulicPowerPort" , d_AllObjectsInPortFolder["Modelica Ports\\FluidPort"] },
            //    { "FluidPort" , d_AllObjectsInPortFolder["Modelica Ports\\FluidPort"] },
            //    { "HeatPort" , d_AllObjectsInPortFolder["Modelica Ports\\HeatPort"] },
            //    { "ThermalPowerPort" , d_AllObjectsInPortFolder["Modelica Ports\\HeatPort"] },
            //    { "ElectricalPowerPort" , d_AllObjectsInPortFolder["Modelica Ports\\ElectricalPin"] },
            //    { "ElectricalPin" , d_AllObjectsInPortFolder["Modelica Ports\\ElectricalPin"] },
            //    { "InputSignalPort" , d_AllObjectsInPortFolder["Modelica Ports\\RealInput"] },
            //    { "OutputSignalPort" , d_AllObjectsInPortFolder["Modelica Ports\\RealOutput"] },
            //    { "RealInput" , d_AllObjectsInPortFolder["Modelica Ports\\RealInput"] },
            //    { "RealOutput" , d_AllObjectsInPortFolder["Modelica Ports\\RealOutput"] },
            //    { "BooleanInput" , d_AllObjectsInPortFolder["Modelica Ports\\BooleanInput"] },
            //    { "BooleanOutput" , d_AllObjectsInPortFolder["Modelica Ports\\BooleanOutput"] },
            //    { "IntegerInput" , d_AllObjectsInPortFolder["Modelica Ports\\IntegerInput"] },
            //    { "IntegerOutput" , d_AllObjectsInPortFolder["Modelica Ports\\IntegerOutput"] },
            //    { "TranslationalPowerPort" , d_AllObjectsInPortFolder["Modelica Ports\\TranslationalFlange"] },
            //    { "TranslationalFlange" , d_AllObjectsInPortFolder["Modelica Ports\\TranslationalFlange"] },
            //    { "RotationalPowerPort" , d_AllObjectsInPortFolder["Modelica Ports\\RotationalFlange"] },
            //    { "RotationalFlange" , d_AllObjectsInPortFolder["Modelica Ports\\RotationalFlange"] },
            //};
        }

        // Your basic recursive folder-gatherer.
        private List<MgaFolder> RecursivelyGetAllFolders(MgaFolder folder)
        {
            List<MgaFolder> rtn = new List<MgaFolder>();

            if (folder.MetaBase.Name == "RootFolder")
            {
                GMEConsole.Info.WriteLine(
                    "Omitted {0} - it is a RootFolder and contains no valid models",
                    folder.Name);
            }
            else
            {
                rtn.Add(folder);
            }

            foreach (MgaFolder subfolder in folder.ChildFolders)
                rtn.AddRange(RecursivelyGetAllFolders(subfolder));

            return rtn;
        }

        // Your basic recursive FCO-gatherer.
        private Dictionary<string, List<MgaFCO>> RecursivelyGetAllChildren(MgaModel root)
        {
            if (root == null)
            {
                return new Dictionary<string, List<MgaFCO>>();
            }

            var result = new Dictionary<string, List<MgaFCO>>();
            var models = new List<MgaModel>();

            foreach (var childObject in root.ChildObjects.OfType<MgaFCO>())
            {
                var kind = childObject.Meta.Name;

                if (!result.ContainsKey(kind))
                {
                    result[kind] = new List<MgaFCO>();
                }

                result[kind].Add(childObject);

                if (childObject is MgaModel)
                {
                    models.Add((MgaModel)childObject);
                }
            }

            foreach (var model in models)
            {
                var tmp = RecursivelyGetAllChildren(model);

                foreach (var tmpChild in tmp.Where(tmpChild => tmpChild.Value != null))
                {
                    if (!result.ContainsKey(tmpChild.Key))
                    {
                        result[tmpChild.Key] = new List<MgaFCO>();
                    }

                    result[tmpChild.Key].AddRange(tmpChild.Value);
                }
            }

            return result;
        }

        // Entry point for the migrator.
        // Gather all folders, then recursively migrate each model found within.
        public void MigrateProject()
        {
            List<MgaFolder> lf_Folders = RecursivelyGetAllFolders(project.RootFolder);
            foreach (MgaFolder folder in lf_Folders)
            {
                foreach (MgaModel model in folder.ChildObjects.OfType<MgaModel>())
                    MigrateAllChildren(model);
            }
        }

        // This method should only operate on 'obsolete' object kinds (?)
        private void MigrateObject(MgaFCO fco_Old)
        {
            // Skip if it's already been migrated.
            if (dfco_OldToNewMap.ContainsKey(fco_Old))
            {
                return;
            }

            String s_Kind = fco_Old.Meta.Name;

            // Check if old port is FluidPort; get associated Package
            bool isModelicaFluidPort = false;
            string fluidPackage = null;
            string materialSpecName = null;
            if (s_Kind == "FluidPort" &&
                fco_Old.ParentModel.Meta.Name == "ModelicaModel")
            {
                isModelicaFluidPort = true;
                MgaConnPoints connPoints = fco_Old.PartOfConns;
                foreach (MgaConnPoint cp in connPoints)
                {
                    MgaFCO otherEnd = null;
                    MgaConnection conn = cp.Owner;
                    if ((conn as IMgaSimpleConnection).Dst.Name == fco_Old.Name)
                    {
                        otherEnd = (conn as IMgaSimpleConnection).Src;
                    }
                    else if ((conn as IMgaSimpleConnection).Src.Name == fco_Old.Name)
                    {
                        otherEnd = (conn as IMgaSimpleConnection).Dst;
                    }

                    if (otherEnd != null &&
                        otherEnd.Meta.Name == "ModelicaMaterialSpec")
                    {
                        materialSpecName = otherEnd.Name;
                        fluidPackage = otherEnd.StrAttrByName["CompatibleMaterials"];
                    }
                }
            }

            // if port is AggregatePort or BusPort, it might be one of several types, based on URI
            //      -it also might contain a parameter...
            string portURI = null;
            MgaFCO portsFirstOrDefaultChild = null;
            string portParamName = null;
            string portParamValue = null;
            string portParamDefault = null;

            if (s_Kind == "AggregatePort" ||
                s_Kind == "ModelicaAggregateInterface" ||
                s_Kind == "BusPort" ||
                s_Kind == "ModelicaBusPortInterface")
            {
                portURI = fco_Old.StrAttrByName["Type"];
                int childObjectsCount = fco_Old.ChildObjects.Count;
                if (childObjectsCount != 0)
                {
                    portsFirstOrDefaultChild = fco_Old.ChildObjects.Cast<MgaFCO>().FirstOrDefault();
                    if (portsFirstOrDefaultChild.Meta.Name == "ModelicaParameter")
                    {
                        portParamName = portsFirstOrDefaultChild.Name;
                        portParamValue = portsFirstOrDefaultChild.StrAttrByName["Value"];
                        portParamDefault = portsFirstOrDefaultChild.StrAttrByName["DefaultValue"];
                    }
                }
                s_Kind = portURI;
            }

            // Check to see if we have a mapping rule for this object type.
            MgaFCO cp_Declaration;
            d_OldKindToNewPortArchetype_jk.TryGetValue(s_Kind, out cp_Declaration);

            // if it wasn't in the dictionary, and it wasn't an 'Aggregate' type port, skip it
            if (cp_Declaration == null &&
                portURI == null)
            {
                return;
            }

            MgaModel m_Parent = fco_Old.ParentModel;
            String s_Name = fco_Old.Name;

            // Is this a subtype or instance? 
            if (fco_Old.DerivedFrom != null &&
                m_Parent.DerivedFrom != null)
            {
                // Yes. Migrate the archetype, and then find the auto-generated equivalent at this level.
                MigrateObject(fco_Old.ArcheType);
                MgaFCO fco_Equivalent = m_Parent.ChildFCOs.OfType<MgaFCO>().Where(f => f.Name == fco_Old.Name).FirstOrDefault();

                // If the old port had a child object (ModelicaParameter), create it.
                if (portParamName != null)
                {
                    try
                    {
                        MgaFCO portParam = fco_Equivalent.get_ObjectByPath(portParamName) as MgaFCO;
                        if (portParam != null)
                        {
                            portParam.StrAttrByName["Value"] = portParamValue;
                            portParam.StrAttrByName["DefaultValue"] = portParamDefault;
                        }
                    }
                    catch (Exception ex)
                    {
                        GMEConsole.Error.WriteLine(
                            "Could not modify PortParameter {0} (Value: {1}, Default: {2}) within <a href=\"mga:{3}\">{4}</a>",
                            portParamName,
                            portParamValue,
                            portParamDefault,
                            fco_Equivalent.ID,
                            fco_Equivalent.Name);
                        GMEConsole.Error.WriteLine("Exception: {0}", ex.StackTrace);
                    }
                }

                // if the old port was a FluidPort, get the parameter within the new port and modify "Value"
                if (isModelicaFluidPort)
                {
                    try
                    {
                        MgaFCO portParam = fco_Equivalent.get_ObjectByPath(materialSpecName) as MgaFCO;
                        if (portParam != null)
                        {
                            portParam.StrAttrByName["Value"] = fluidPackage;
                        }
                    }
                    catch (Exception ex)
                    {
                        GMEConsole.Error.WriteLine(
                            "Could not modify FluidPortParameter {0} (Value: {1}) within <a href=\"mga:{2}\">{3}</a>",
                            materialSpecName,
                            fluidPackage,
                            fco_Equivalent.ID,
                            fco_Equivalent.Name);
                        GMEConsole.Error.WriteLine("Exception: {0}", ex.StackTrace);
                    }
                }

                // Log in migration map.
                dfco_OldToNewMap[fco_Old] = fco_Equivalent;
            }
            else
            {
                bool isParentModelicaModel = true;
                if (m_Parent.Meta.Name == "ComponentAssembly" ||
                    m_Parent.Meta.Name == "DesignContainer" ||
                    m_Parent.Meta.Name == "Component" ||
                    m_Parent.Meta.Name == "TestComponent")
                {
                    isParentModelicaModel = false;
                }

                // No, it's an archetype. Create a new port.
                MgaFCO fco_New = null;

                if (cp_Declaration != null)
                {
                    try
                    {
                        GME.MGA.Meta.MgaMetaRole role = (m_Parent.Meta as GME.MGA.Meta.MgaMetaModel).RoleByName[cp_Declaration.Meta.Name];
                        fco_New = m_Parent.CopyFCODisp(cp_Declaration, role);

                        // ALWAYS DELETE CHILD OBJECTS. MAYBE NOT A PERMANENT SOLUTION
                        foreach (MgaFCO childObj in fco_New.ChildObjects.Cast<MgaFCO>()) //.ToList()
                        {
                            childObj.DestroyObject();
                        }
                    }
                    catch (Exception ex)
                    {
                        GMEConsole.Out.WriteLine("{0}: {1}", ex.Message, ex.StackTrace);
                    }
                }
                else
                {
                    try
                    {
                        fco_New = makeFCO(m_Parent, "ModelicaConnector");
                        fco_New.StrAttrByName["Class"] = s_Kind;
                    }
                    catch (Exception ex)
                    {
                        GMEConsole.Out.WriteLine("{0}: {1}", ex.Message, ex.StackTrace);
                    }
                }

                if (fco_New != null)
                {
                    fco_New.Name = s_Name;
                }

                // If the parent model is a ModelicaModel, put port parameters.
                if (isParentModelicaModel == true)
                {
                    // If the old port had a child object (ModelicaParameter), create it.
                    if (portParamName != null)
                    {
                        try
                        {
                            MgaFCO portParam = makeFCO((fco_New as MgaModel), "ModelicaParameter");
                            if (portParam != null)
                            {
                                portParam.Name = portParamName;
                                portParam.StrAttrByName["Value"] = portParamValue;
                                portParam.StrAttrByName["DefaultValue"] = portParamDefault;
                            }
                        }
                        catch (Exception ex)
                        {
                            GMEConsole.Error.WriteLine(
                                "Could not modify PortParameter {0} (Value: {1}, Default: {2}) within <a href=\"mga:{3}\">{4}</a>",
                                portParamName,
                                portParamValue,
                                portParamDefault,
                                fco_New.ID,
                                fco_New.Name);
                            GMEConsole.Error.WriteLine("Exception: {0}", ex.StackTrace);
                        }
                    }

                    // if the old port was a FluidPort, make the ModelicaRedeclare
                    if (isModelicaFluidPort)
                    {
                        try
                        {
                            var modelicaRedeclare = makeFCO((fco_New as MgaModel), "ModelicaRedeclare");
                            if (modelicaRedeclare != null)
                            {
                                modelicaRedeclare.Name = materialSpecName;
                                modelicaRedeclare.StrAttrByName["Value"] = fluidPackage;
                            }
                        }
                        catch (Exception ex)
                        {
                            GMEConsole.Error.WriteLine(
                                "Could not modify FluidPortParameter {0} (Value: {1}) within <a href=\"mga:{2}\">{3}</a>",
                                materialSpecName,
                                fluidPackage,
                                fco_New.ID,
                                fco_New.Name);
                            GMEConsole.Error.WriteLine("Exception: {0}", ex.StackTrace);
                        }
                    }
                }

                // Copy position (from first found aspect)
                var part = fco_Old.Parts.Cast<IMgaPart>().FirstOrDefault();
                string icon;
                int x;
                int y;
                if (part != null)
                {
                    part.GetGmeAttrs(out icon, out x, out y);
                    foreach (var part_New in fco_New.Parts.Cast<IMgaPart>())
                    {
                        part_New.SetGmeAttrs(icon, x, y);
                    }
                }

                // Log in migration map.
                dfco_OldToNewMap[fco_Old] = fco_New;
            }

            // Migrate connections involving this (now-migrated) port.
            MigrateConnections(fco_Old);
        }

        private void MigrateConnections(MgaFCO fco_Old)
        {
            MgaFCO fco_New = dfco_OldToNewMap[fco_Old];

            // For each other port that's been migrated.
            foreach (var kvp in dfco_OldToNewMap.Where(kvp => kvp.Key != fco_Old))
            {
                MgaFCO fco_OtherOld = kvp.Key;
                MgaFCO fco_OtherNew = kvp.Value;

                // Look for connections between fco_Old and fco_OtherOld
                var conns = fco_Old
                    .PartOfConns
                    .Cast<IMgaConnPoint>()
                    .Select(x => x.Owner)
                    .Cast<IMgaSimpleConnection>()
                    .Where(x => x.Dst.ID == fco_OtherOld.ID || x.Src.ID == fco_OtherOld.ID);

                foreach (var conn in conns)
                {
                    // If connection is an instance or subtype, skip it.
                    if (conn.ParentModel.IsInstance)
                    {
                        continue;
                    }
                    if (conn.ParentModel.DerivedFrom != null)
                    {
                        continue;
                    }
                    // Function MigrateCompAssmConns(MgaModel does this job)
                    if (conn.ParentModel.Meta.Name == "ComponentAssembly" ||
                        conn.ParentModel.Meta.Name == "DesignContainer")
                    {
                        continue;
                    }

                    try
                    {
                        // Create new connection
                        var connRole = (conn.ParentModel.Meta as GME.MGA.Meta.MgaMetaModel).RoleByName["PortComposition"];
                        conn.ParentModel.CreateSimplerConnDisp(connRole, fco_New, fco_OtherNew);

                        // Delete old connection
                        //conn.DestroyObject();
                    }
                    catch (Exception ex)
                    {
                        GMEConsole.Error.WriteLine(
                            "Exception creating connection in <a href=\"mga:{0}\">{1}</a> between <a href=\"mga:{2}\">{3}</a> and <a href=\"mga:{4}\">{5}</a>: {6}",
                            conn.ParentModel.ID,
                            conn.ParentModel.Name,
                            fco_New.ID,
                            fco_New.Name,
                            fco_OtherNew.ID,
                            fco_OtherNew.Name,
                            ex.Message
                            );
                    }
                }
            }
        }

        private void migrateSystemConns(MgaModel SystemModel)
        {
            string parName = (SystemModel.ParentFolder == null) ? SystemModel.ParentModel.Name : SystemModel.ParentFolder.Name;
            //GMEConsole.Info.WriteLine("migrateSystemConns({0}.{1})", parName, SystemModel.Name);

            var connections = SystemModel.ChildObjects.OfType<MgaSimpleConnection>();
            string systemName = SystemModel.Name;
            string systemID = SystemModel.ID;

            foreach (var conn in connections)
            {
                if (conn.Meta.Name == "JoinStructures" ||
                    conn.Meta.Name == "ValueFlow" ||
                    conn.Meta.Name == "PortComposition")
                {
                    continue;
                }

               
                string srcPortName = conn.Src.Name;
                string srcParentName = conn.Src.ParentModel.Name;
                string srcPortID = conn.Src.ID;
                string srcParentID = conn.Src.ParentModel.ID;

                var srcReferences = conn.SrcReferences;
                if (srcReferences.Count != 0)
                {
                    srcParentName = srcReferences.Cast<IMgaFCO>().FirstOrDefault().Name;
                    srcParentID = srcReferences.Cast<IMgaFCO>().FirstOrDefault().ID;
                }

                string dstPortName = conn.Dst.Name;
                string dstParentName = conn.Dst.ParentModel.Name;
                string dstPortID = conn.Dst.ID;
                string dstParentID = conn.Dst.ParentModel.ID;

                var dstReferences = conn.DstReferences;
                if (dstReferences.Count != 0)
                {
                    dstParentName = dstReferences.Cast<IMgaFCO>().FirstOrDefault().Name;
                    dstParentID = dstReferences.Cast<IMgaFCO>().FirstOrDefault().ID;
                }

                try
                {
                    MgaFCO newConnSrcParent =
                        (srcParentID == systemID) ?
                        SystemModel as MgaFCO :
                        SystemModel.get_ChildFCO(srcParentName);
                    if (newConnSrcParent == null)
                    {
                        newConnSrcParent = SystemModel.ChildObjects.OfType<MgaReference>()
                            .FirstOrDefault(x => x.Referred != null &&
                                x.Referred.Name == srcParentName) as MgaFCO;
                    }
                    if (newConnSrcParent == null)
                    {
                        GMEConsole.Warning.WriteLine(
                            "Could not find 'srcParent'; Try to make manual connection: {0}.{1}<=>{2}.{3} within <a href=\"mga:{4}\">{5}</a>",
                            srcParentName,
                            srcPortName,
                            dstParentName,
                            dstPortName,
                            systemID,
                            systemName);
                        continue;
                    }

                    MgaFCO newConnSrcPort = null;
                    if (newConnSrcParent is MgaReference &&
                        (newConnSrcParent as MgaReference).Referred != null)
                    {
                        MgaFCO referredComp = (newConnSrcParent as MgaReference).Referred;
                        newConnSrcPort = referredComp.ChildObjects.OfType<MgaModel>()
                            .FirstOrDefault(x => x.Name == srcPortName &&
                                x.Meta.Name == "ModelicaConnector") as MgaFCO;
                    }
                    else
                    {
                        newConnSrcPort = newConnSrcParent.ChildObjects.OfType<MgaFCO>()
                            .FirstOrDefault(x => x.Name == srcPortName &&
                                x.Meta.Name == "ModelicaConnector");
                    }

                    MgaFCO newConnDstParent =
                        (dstParentID == systemID) ?
                        SystemModel as MgaFCO :
                        SystemModel.get_ChildFCO(dstParentName);
                    if (newConnDstParent == null)
                    {
                        newConnDstParent = SystemModel.ChildObjects.OfType<MgaReference>()
                            .FirstOrDefault(x => x.Referred != null &&
                                x.Referred.Name == dstParentName) as MgaFCO;
                    }
                    if (newConnDstParent == null)
                    {
                        GMEConsole.Warning.WriteLine(
                            "Could not find 'dstParent'; Try to make manual connection: {0}.{1}<=>{2}.{3} within <a href=\"mga:{4}\">{5}</a>",
                            srcParentName,
                            srcPortName,
                            dstParentName,
                            dstPortName,
                            systemID,
                            systemName);
                        continue;
                    }

                    MgaFCO newConnDstPort = null;
                    if (newConnDstParent is MgaReference &&
                        (newConnDstParent as MgaReference).Referred != null)
                    {
                        MgaFCO referredComp = (newConnDstParent as MgaReference).Referred;
                        newConnDstPort = referredComp.ChildObjects.OfType<MgaFCO>()
                            .FirstOrDefault(x => x.Name == dstPortName &&
                                x.Meta.Name == "ModelicaConnector");
                    }
                    else
                    {
                        newConnDstPort = newConnDstParent.ChildObjects.OfType<MgaFCO>()
                            .FirstOrDefault(x => x.Name == dstPortName &&
                                x.Meta.Name == "ModelicaConnector");
                    }

                    if (newConnSrcPort == null)
                    {
                        string systemParentName =
                            (SystemModel.ParentModel == null) ?
                            SystemModel.ParentFolder.Name :
                            SystemModel.ParentModel.Name;
                        GMEConsole.Warning.WriteLine(
                            "Could not find 'srcPort'; Try to make manual connection: {0}.{1}<=>{2}.{3} within <a href=\"mga:{4}\">{5}</a>",
                            srcParentName,
                            srcPortName,
                            dstParentName,
                            dstPortName,
                            systemID,
                            systemName);
                        continue;
                    }
                    if (newConnDstPort == null)
                    {
                        string systemParentName =
                            (SystemModel.ParentModel == null) ?
                            SystemModel.ParentFolder.Name :
                            SystemModel.ParentModel.Name;

                        GMEConsole.Warning.WriteLine(
                            "Could not find 'dstPort'; Try to make manual connection: {0}.{1}<=>{2}.{3} within <a href=\"mga:{4}\">{5}</a>",
                            srcParentName,
                            srcPortName,
                            dstParentName,
                            dstPortName,
                            systemID,
                            systemName);
                        continue;
                    }


                    if (newConnDstParent != null &&
                        newConnDstPort != null &&
                        newConnSrcParent != null &&
                        newConnSrcPort != null)
                    {
                        try
                        {
                            var newConnection = SystemModel.CreateSimpleConnDisp(
                                (SystemModel.Meta as MgaMetaModel).RoleByName["PortComposition"],
                                newConnSrcPort,
                                newConnDstPort,
                                newConnSrcParent,
                                newConnDstParent);
                        }
                        catch (Exception ex)
                        {
                            GMEConsole.Warning.WriteLine(
                                "Exception making connection between {0}.{1}<=>{2}.{3} within <a href=\"mga:{4}\">{5}</a>",
                                newConnSrcParent.Name,
                                newConnSrcPort.Name,
                                newConnDstParent.Name,
                                newConnDstPort.Name,
                                systemID,
                                systemName);
                            GMEConsole.Error.WriteLine("trace: {0}",
                                ex.StackTrace);
                            GMEConsole.Error.WriteLine("message: {0}",
                                ex.Message);
                        }
                    }
                    else
                    {
                        GMEConsole.Warning.WriteLine(
                            "Object missing for connection between {0}.{1}<=>{2}.{3} within <a href=\"mga:{4}\">{5}</a>",
                            srcParentName,
                            srcPortName,
                            dstParentName,
                            dstPortName,
                            systemID,
                            systemName);
                    }
                }
                catch (Exception ex)
                {
                    GMEConsole.Warning.WriteLine(
                        "Unknown Exception making connection between {0}.{1}<=>{2}.{3} within <a href=\"mga:{4}\">{5}</a>",
                        srcParentName,
                        srcPortName,
                        dstParentName,
                        dstPortName,
                        systemID,
                        systemName);
                    GMEConsole.Error.WriteLine("trace: {0}",
                        ex.StackTrace);
                    GMEConsole.Error.WriteLine("message: {0}",
                        ex.Message);
                }
            }
        }

        Dictionary<MgaModel, MgaModel> mapModel_migrated2original = new Dictionary<MgaModel, MgaModel>();
        List<MgaModel> migratedModels = new List<MgaModel>();

        // Recursive function that will keep looking for:
        //      -DesignContainers
        //      -ComponentAssemblies
        //      -ComponentRefs => referred Component
        // and migrate the deepest level first
        public void migrateModel(MgaModel model)
        {
            try
            {
                MgaModel archetype =
                    (model.ArcheType == null) ?
                    model :
                    model.ArcheType as MgaModel;

                // Skip the model if its archetype has already been migrated
                if (migratedModels.Contains(archetype))
                {
                    return;
                }
                // Skip the model if it contains 'new' port types
                if (archetype.ChildObjects.OfType<MgaModel>().Where(x => x.Meta.Name == "ModelicaConnector").FirstOrDefault() != null)
                {
                    GMEConsole.Info.WriteLine(
                        "Found new connector type in MgaModel, assumed to be migrated: <a href=\"mga:{0}\">{1}</a>",
                        archetype.ID,
                        archetype.Name);

                    return;
                }

                string parName = (archetype.ParentFolder == null) ? archetype.ParentModel.Name : archetype.ParentFolder.Name;
                //GMEConsole.Info.WriteLine("migrateModel(<a href=\"mga:{0}\">{1}</a>)", archetype.ID, archetype.Name);

                // if this model is an archetype, make a copy to preserve original
                MgaModel untouchedOriginal = makeCopyOfModel(archetype, "_original");
                mapModel_migrated2original.Add(archetype, untouchedOriginal);

                // get all the children of this model, and migrate them also. Assume only on TLSUT within a model
                var topLevelSUT = archetype.ChildObjects.OfType<MgaReference>().Where(x => x.Meta.Name == "TopLevelSystemUnderTest").FirstOrDefault();
                var testComponents = archetype.ChildObjects.OfType<MgaModel>().Where(x => x.Meta.Name == "TestComponent");
                var components = archetype.ChildObjects.OfType<MgaModel>().Where(x => x.Meta.Name == "Component");
                var childDesignContainers = archetype.ChildObjects.OfType<MgaModel>().Where(x => x.Meta.Name == "DesignContainer");
                var childComponentAssemblies = archetype.ChildObjects.OfType<MgaModel>().Where(x => x.Meta.Name == "ComponentAssembly");
                var childComponentRefs = archetype.ChildObjects.OfType<MgaReference>().Where(x => x.Meta.Name == "ComponentRef");
                var childComponentAssemblyRefs = archetype.ChildObjects.OfType<MgaReference>().Where(x => x.Meta.Name == "ComponentAssemblyRef");
                var childModelicaModels = archetype.ChildFCOs.OfType<MgaModel>().Where(x => x.Meta.Name == "ModelicaModel");
                var childEnvironments = archetype.ChildFCOs.OfType<MgaModel>().Where(x => x.Meta.Name == "Environment");

                foreach (var mm in childModelicaModels)
                {
                    migrateModel(mm);
                }

                foreach (var env in childEnvironments)
                {
                    migrateModel(env);
                }

                foreach (MgaModel tComp in testComponents)
                {
                    migrateModel(tComp);
                }

                foreach (MgaModel comp in components)
                {
                    migrateModel(comp);
                }

                foreach (MgaReference compRef in childComponentRefs)
                {
                    if (compRef.Referred != null)
                    {
                        MgaModel referred = compRef.Referred as MgaModel;
                        migrateModel(referred);
                    }
                }

                foreach (MgaModel dc in childDesignContainers)
                {
                    migrateModel(dc);
                }

                foreach (MgaModel ca in childComponentAssemblies)
                {
                    migrateModel(ca);
                }

                foreach (MgaReference compAssemblyRef in childComponentAssemblyRefs)
                {
                    if (compAssemblyRef.Referred != null)
                    {
                        MgaModel referred = compAssemblyRef.Referred as MgaModel;
                        migrateModel(referred);
                    }
                }

                if (topLevelSUT != null)
                {
                    if (topLevelSUT.Referred != null)
                    {
                        MgaModel referredSystem = topLevelSUT.Referred as MgaModel;
                        migrateModel(referredSystem);
                    }
                }

                migratePorts(archetype);
                migratedModels.Add(archetype);
            }
            catch (Exception ex)
            {
                GMEConsole.Info.WriteLine(
                    "Could not migrate: <a href=\"mga:{0}\">{1}</a>. ({2})",
                    model.ID,
                    model.Name,
                    model.AbsPath);

                GMEConsole.Error.WriteLine(ex.Message);
            }
        }

        public void migratePorts(MgaModel rootModel)
        {
            string parName = (rootModel.ParentFolder == null) ? rootModel.ParentModel.Name : rootModel.ParentFolder.Name;
            //GMEConsole.Info.WriteLine("migratePorts({0}.{1})", parName, rootModel.Name);
            string kind = rootModel.Meta.Name;

            var childFCOs = rootModel.ChildFCOs.Cast<MgaFCO>().Where(x =>
                x.Meta.Name != "Property" &&
                x.Meta.Name != "ModelicaParameter" &&
                x.Meta.Name != "ModelicaMaterialSpec");

            foreach (var fco in childFCOs)
            {
                MigrateObject(fco);
            }

            if (kind == "ComponentAssembly" ||
                kind == "DesignContainer" ||
                kind == "TestBench")
            {
                migrateSystemConns(rootModel);
            }
        }

        public void MigrateAllChildren(MgaModel root)
        {
            // Recursively get all children of this object, indexed by kind.
            Dictionary<string, List<MgaFCO>> dlf_AllFCOs = RecursivelyGetAllChildren(root);

            // Get self too.
            var kind = root.Meta.Name;
            if (!dlf_AllFCOs.ContainsKey(kind))
                dlf_AllFCOs[kind] = new List<MgaFCO>();
            dlf_AllFCOs[kind].Add(root as MgaFCO);

            // For each found FCO, migrate.
            // The key (kind name) isn't used at all

            #region ObsoleteCode

            foreach (KeyValuePair<string, List<MgaFCO>> kvp in dlf_AllFCOs)
            {
                if (kvp.Key == "ComponentRef")
                {
                    foreach (MgaFCO fco in kvp.Value)
                    {
                        if ((fco as MgaReference).Referred != null)
                        {
                            MgaFCO referredComponent = (fco as MgaReference).Referred;
                            MigrateAllChildren(referredComponent as MgaModel);
                        }
                    }
                }
                else
                {
                    foreach (MgaFCO fco in kvp.Value)
                    {
                        MigrateObject(fco);
                    }
                }
            }

            #endregion

            foreach (KeyValuePair<string, List<MgaFCO>> kvp in dlf_AllFCOs)
            {
                foreach (MgaFCO fco in kvp.Value)
                {
                    MigrateObject(fco);
                }
            }

            if (root.Meta.Name == "ComponentAssembly" ||
                root.Meta.Name == "DesignContainer")
            {
                migrateSystemConns(root);
            }

            // ModelicaModel objects aren't replaced, but are changing to a new aspect.
            // Copy its position from the current Behavior aspect to the new Modelica aspect.
            if (dlf_AllFCOs.ContainsKey("ModelicaModel"))
            {
                foreach (MgaFCO fco_ModelicaModel in dlf_AllFCOs["ModelicaModel"])
                {
                    var part = fco_ModelicaModel.Parts.Cast<IMgaPart>().Where(p => p.MetaAspect.Name == "BehaviorModelsAspect").FirstOrDefault();
                    string icon;
                    int x;
                    int y;
                    part.GetGmeAttrs(out icon, out x, out y);

                    foreach (var part_ in fco_ModelicaModel.Parts.Cast<IMgaPart>())
                    {
                        part_.SetGmeAttrs(icon, x, y);
                    }
                }
            }
        }

        private static MgaFCO makeFCO(MgaModel parent, string role)
        {
            var newFCO = parent.CreateChildObject(
                (parent.Meta as MgaMetaModel).RoleByName[role]) as MgaFCO;
            return newFCO;
        }

        internal void findOldPortTypes(MgaProject project, bool printHyperlinks)
        {
            List<string> oldPortKinds = new List<string>() 
            {
                "MultibodyFramePowerPort",
                "MultibodyFrame", 
                "ModelicaAggregateInterface",
                "AggregatePort", 
                "HydraulicPowerPort",
                "FluidPort", 
                "HeatPort",
                "ThermalPowerPort",
                "ElectricalPowerPort",
                "ElectricalPin",
                "InputSignalPort",
                "OutputSignalPort",
                "RealInput",
                "RealOutput",
                "BooleanInput",
                "BooleanOutput",
                "IntegerInput",
                "IntegerOutput",
                "TranslationalPowerPort",
                "TranslationalFlange",
                "RotationalPowerPort",
                "RotationalFlange",
                "ModelicaMaterialSpec",
            };

            MgaFilter filter = project.CreateFilter();
            var oldPorts = project.AllFCOs(filter).Cast<MgaFCO>().Where(x => oldPortKinds.Contains(x.Meta.Name));

            if (printHyperlinks)
            {
                foreach (var port in oldPorts)
                {
                    GMEConsole.Info.WriteLine(
                        "Obsolete Port found: <a href=\"mga:{0}\">{1}</a>: {2}",
                        port.ID,
                        port.Name,
                        port.Meta.Name);
                }
            }

            GMEConsole.Info.WriteLine("Number of Obsolete ports found in project: {0}", oldPorts.Count());
        }

        internal void DeleteAllOldTypes(MgaProject project)
        {
            foreach (var kvp in dfco_OldToNewMap)
            {
                try
                {
                    if (kvp.Key == null)
                    {
                        if (kvp.Value != null)
                        {
                            GMEConsole.Warning.WriteLine(
                                "Migrated? <a href=\"mga:{0}\">{1}</a>",
                                kvp.Value.ID,
                                kvp.Value.Name);
                        }
                        continue;
                    }
                    if (kvp.Key.IsLibObject == false &&
                        kvp.Key.ParentModel != null &&
                        kvp.Key.ParentModel.DerivedFrom == null)
                    {
                        kvp.Key.DestroyObject();
                    }
                    else
                    {
                        GMEConsole.Info.WriteLine(
                            "Skipped <a href=\"mga:{0}\">{1}</a>",
                            kvp.Key.ID,
                            kvp.Key.Name);
                    }
                }
                catch (Exception ex)
                {
                    GMEConsole.Error.WriteLine(
                        "Cannot delete object <a href=\"mga:{0}\">{1}</a>. Exception: {2} ({3})",
                        kvp.Key.ID,
                        kvp.Key.Name,
                        ex.Message,
                        ex.StackTrace);
                }

            }

            foreach (var kvp in mapModel_migrated2original)
            {
                try
                {
                    if (kvp.Value != null)
                    {
                        kvp.Value.DestroyObject();
                    }
                }
                catch (Exception ex)
                {
                    GMEConsole.Error.WriteLine(
                        "Failed to Delete <a href=\"mga:{1}\">{2}</a>: {0}",
                        ex.Message,
                        kvp.Value.ID,
                        kvp.Value.Name);
                }
            }

            MgaFilter materialSpecFilter = project.CreateFilter();
            materialSpecFilter.Kind = "ModelicaMaterialSpec";

            var materialSpecBlocks = project.AllFCOs(materialSpecFilter).Cast<MgaFCO>(); //.ToList()
            foreach (MgaFCO msb in materialSpecBlocks)
            {
                try
                {
                    msb.DestroyObject();
                }
                catch (Exception ex)
                {
                    GMEConsole.Error.WriteLine(
                        "Failed to Delete <a href=\"mga:{1}\">{2}</a>: {0}",
                        ex.Message,
                        msb.ID,
                        msb.Name);
                }
            }
        }

        internal void migrateCyPhy2ModelicaWorkflow(MgaProject project)
        {
            MgaFilter workflowFilter = project.CreateFilter();

            workflowFilter.ObjType = objtype_enum.OBJTYPE_ATOM.ToString();
            workflowFilter.Kind = "Task";

            var workflowTasks = project.AllFCOs(workflowFilter).OfType<MgaAtom>();

            foreach (var task in workflowTasks)
            {
                if (task.IsLibObject == false &&
                    task.StrAttrByName["COMName"] == "MGA.Interpreter.CyPhy2Modelica")
                {
                    task.StrAttrByName["COMName"] = task.StrAttrByName["COMName"] + "_v2";
                }
            }

        }
    }
}
