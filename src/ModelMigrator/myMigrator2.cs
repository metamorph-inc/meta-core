using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;
using GME.MGA;
using GME.CSharp;
using GME;
using GME.MGA.Meta;
//using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
//using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;

namespace ModelMigrator
{
    class myMigrator
    {
        private MgaProject project;
        GMEConsole GMEConsole { get; set; }
        private Dictionary<string, string> oldToNewRole; //map of obselete to replacement FCO roles
        private Dictionary<MgaFCO, MgaFCO> oldToNewFCO; //map of old to replacement FCOs
        private List<MgaFCO> oldConnsAndRefs; //for all connections/references
        private List<MgaFCO> oldSubsAndInstances; //for all largest instances/subtypes
        private List<MgaModel> migratedModels; 
        private MgaFolder RootFolder;
        private MgaModel RootModel;

        public myMigrator(MgaProject p)
        {
            project = p;
            GMEConsole = GMEConsole.CreateFromProject(p);
            oldToNewRole = new Dictionary<string, string>()
            {
                { "StructuralInterface" , "Connector" },
                {"AxisGeometry", "Axis"},
                {"SurfaceGeometry", "Surface"},
                {"PointGeometry", "Point"},
                {"CoordinateSystemGeometry", "CoordinateSystem"},
                {"StructuralInterfaceForwarder", "Connector"},
                {"FeatureMap", "PortComposition"},
                {"AnalysisPointMap", "PortComposition"},
                {"AnalysisPoint", "Point"},
                {"JoinStructures", "ConnectorComposition"},
                //{"FeatureMap", "SurfaceReverseMap"}, //special, not general case
                //{"CADModel", ""}, //special, addition not replacement
            };
            oldToNewFCO = new Dictionary<MgaFCO, MgaFCO>();
            oldConnsAndRefs = new List<MgaFCO>();
            oldSubsAndInstances = new List<MgaFCO>();
            migratedModels = new List<MgaModel>();

            foreach (MgaFolder child in project.RootFolder.ChildFolders)
                if (child.MetaFolder.Name == "Components")
                {
                    List<MgaFolder> folders = getAllFolders(child);
                    foreach (MgaFolder folder in folders)
                        foreach (MgaModel model in folder.ChildObjects.OfType<MgaModel>())
                            createNewFCOs(model);
                }
        }

        public void migrateProject()
        {
            RootFolder = project.RootFolder;
            RootModel = null;

            List<MgaFolder> folders = getAllFolders(project.RootFolder);
            List<MgaFolder> C = new List<MgaFolder>(); //components
            List<MgaFolder> DS = new List<MgaFolder>(); //design spaces
            List<MgaFolder> CA = new List<MgaFolder>(); // component assemblies
            List<MgaFolder> TC = new List<MgaFolder>(); //test components
            List<MgaFolder> T = new List<MgaFolder>(); //testing
            List<MgaFolder> other = new List<MgaFolder>();

            foreach (MgaFolder folder in folders)
            {
                if (folder == null)
                    continue;
                if (folder.MetaFolder.DisplayedName == "Components")
                    C.Add(folder);
                else if (folder.MetaFolder.DisplayedName == "Design Spaces")
                    DS.Add(folder);
                else if (folder.MetaFolder.DisplayedName == "Component Assemblies")
                    CA.Add(folder);
                else if (folder.MetaFolder.DisplayedName == "Test Components")
                    TC.Add(folder);
                else if (folder.MetaFolder.DisplayedName == "Testing")
                    T.Add(folder);
                else
                    other.Add(folder);
            }
            
            foreach (MgaFolder folder in C)
                foreach (MgaModel model in folder.ChildObjects.OfType<MgaModel>())
                    createNewFCOs(model);
            foreach (MgaFolder folder in DS)
                foreach (MgaModel model in folder.ChildObjects.OfType<MgaModel>())
                    createNewFCOs(model);
            foreach (MgaFolder folder in CA)
                foreach (MgaModel model in folder.ChildObjects.OfType<MgaModel>())
                    createNewFCOs(model);
            foreach (MgaFolder folder in TC)
                foreach (MgaModel model in folder.ChildObjects.OfType<MgaModel>())
                    createNewFCOs(model);
            foreach (MgaFolder folder in T)
                foreach (MgaModel model in folder.ChildObjects.OfType<MgaModel>())
                    createNewFCOs(model);
            foreach (MgaFolder folder in other)
                foreach (MgaModel model in folder.ChildObjects.OfType<MgaModel>())
                    createNewFCOs(model);
            directFCOs();
            copyOldLayout();
            deleteOldFCOs();
        }

        public void migrateFolder(MgaFolder rootfolder)
        {
            RootFolder = rootfolder;
            RootModel = null;

            List<MgaFolder> folders = getAllFolders(rootfolder);
            foreach (MgaFolder folder in folders)
                foreach (MgaModel model in folder.ChildObjects.OfType<MgaModel>())
                    createNewFCOs(model);
            directFCOs();
            copyOldLayout();
            deleteOldFCOs();
        }

        public void migrateModel(MgaModel rootmodel)
        {
            RootFolder = null;
            RootModel = rootmodel;

            createNewFCOs(rootmodel);
            directFCOs();
            copyOldLayout();
            deleteOldFCOs();
        }

        private List<MgaFolder> getAllFolders(MgaFolder folder)
        {
            List<MgaFolder> rtn = new List<MgaFolder>();
            rtn.Add(folder);
            foreach (MgaFolder subfolder in folder.ChildFolders)
                rtn.AddRange(getAllFolders(subfolder));
            return rtn;
        }

        //assume root model (located directly in folder) will not need replacing
        //creates all model, atoms, and reference replacements (not connections)
        //Parents of new/old objects is set to the new object if model is being replaced
        //properties are copied over
        private void createNewFCOs(MgaModel model)
        {
            bool newParent = model.ParentFolder == null && oldToNewFCO.ContainsKey(model as MgaFCO);
            if (model == null || migratedModels.Contains(model) || (model.IsInstance && !newParent)) //if model already migrated, stop //INSTANCE BUG SOLUTION
                return;

            
            //INSTANCE BUG SOLUTION
            if (model.IsInstance) //newParent = true
            {
                foreach (var childObject in model.ChildObjects.OfType<MgaFCO>())
                {
                    oldToNewFCO.Add(childObject, null);
                    if (childObject is MgaModel)
                        createNewFCOs(childObject as MgaModel);
                }
                migratedModels.Add(model);
                return;
            }
            //END SOLUTION

            foreach (var childObject in model.ChildObjects.OfType<MgaFCO>())
            {
                string role = (childObject as MgaFCO).MetaRole.Name;

                //special case (cadmodel): assumes cadmodel is not replaced
                if (role == "CADModel")
                {
                    string cadFileType = childObject.get_StrAttrByName("FileType");
                    string cadFileExtension = (cadFileType == "Part") ? ".prt" : ".asm";

                    //check already has resource?
                    bool makeFCOs = true;
                    foreach (MgaConnPoint point in childObject.PartOfConns)
                    {
                        if (point.Owner.MetaRole.Name == "UsesResource")
                        {
                            makeFCOs = false;

                            MgaConnection conn = point.Owner;
                            MgaFCO resourceObj = null;
                            if ((conn as IMgaSimpleConnection).Dst.Meta.Name == "Resource")
                            {
                                resourceObj = (conn as IMgaSimpleConnection).Dst;
                            }
                            else if ((conn as IMgaSimpleConnection).Src.Meta.Name == "Resource")
                            {
                                resourceObj = (conn as IMgaSimpleConnection).Src;
                            }

                            if (resourceObj != null)
                                UpdateResourcePath(resourceObj,
                                                   cadFileExtension);
                        }
                    }

                    if (makeFCOs)
                    {
                        MgaFCO resource;
                        MgaFCO conn;
                        if (newParent)
                        {
                            MgaFCO newCAD = makeFCO(oldToNewFCO[model as MgaFCO] as MgaModel, childObject.MetaRole.Name);
                            oldToNewFCO.Add(childObject, newCAD);
                            copyFCO(childObject, newCAD);
                            resource = makeFCO(oldToNewFCO[model as MgaFCO] as MgaModel, "Resource");
                            conn = makeConn(oldToNewFCO[model as MgaFCO] as MgaModel, newCAD, resource, "UsesResource");
                        }
                        else
                        {
                            resource = makeFCO(model, "Resource");
                            conn = makeConn(model, childObject, resource, "UsesResource");
                        }
                        resource.set_StrAttrByName("Path", childObject.get_StrAttrByName("FilePathWithinResource"));
                        childObject.set_StrAttrByName("FilePathWithinResource", "");
                        UpdateResourcePath(resource,
                                           cadFileExtension);
                    }
                }

                if (childObject.ArcheType != null && RootFolder != project.RootFolder 
                    && !oldToNewFCO.ContainsKey(childObject.ArcheType) && !willBeMigrated(childObject.ArcheType)) //migrate archetype if not going to be/hasn't been migrated 
                        createNewFCOs(childObject.ArcheType.ParentModel);
                if (oldToNewRole.ContainsKey(role) 
                    && !oldToNewFCO.ContainsKey(childObject)
                    && !(childObject is MgaConnection)) //if child type is obselete, not yet migrated, and not a connection
                {
                    //general case for models/atoms/references
                    if (newParent)
                        oldToNewFCO.Add(childObject, makeFCO(oldToNewFCO[model as MgaFCO] as MgaModel, oldToNewRole[role]));
                    else
                        oldToNewFCO.Add(childObject, makeFCO(model, oldToNewRole[role]));
                    if(childObject.Name != childObject.MetaRole.Name && childObject.Name != childObject.Meta.Name && childObject.Name != childObject.Meta.DisplayedName) //name default?
                        oldToNewFCO[childObject].Name = childObject.Name;

                    //copy over specified properties to new FCO
                    if (role == "StructuralInterface")
                    {
                        oldToNewFCO[childObject].set_StrAttrByName("Definition", childObject.get_StrAttrByName("Type"));
                        oldToNewFCO[childObject].Name = childObject.Name;
                    }
                    else if (role == "SurfaceGeometry" || role == "AxisGeometry" || role == "PointGeometry" || role == "CoordinateSystemGeometry")
                    {
                        oldToNewFCO[childObject].set_StrAttrByName("DatumName", childObject.get_StrAttrByName("Datum"));
                    } 
                }
                else if (newParent && !oldToNewFCO.ContainsKey(childObject) && !(childObject is MgaConnection)) //if child type not obselete, not yet migrated, needs a new parent (and not a connection)
                {
                    MgaFCO copy = makeFCO(oldToNewFCO[model as MgaFCO] as MgaModel, childObject.MetaRole.Name);
                    copyFCO(childObject, copy);
                    oldToNewFCO.Add(childObject, copy);
                }

                //for directFCOs
                if (childObject is MgaConnection || childObject is MgaReference)
                    oldConnsAndRefs.Add(childObject);
                if (childObject.ArcheType != null)
                    oldSubsAndInstances.Add(childObject);
                //recursion
                if (childObject is MgaModel)
                    createNewFCOs(childObject as MgaModel);
            }
            migratedModels.Add(model);
        }

        //for any objects dependant on other objects
        //creates new connections and redirects src and dst
        //redirects references
        //redirects attachments for instances/subtypes
        private void directFCOs()
        {
            foreach (MgaFCO entry in oldSubsAndInstances)
            {
                //attach masters/slaves that changed parents
                MgaFCO arch = entry.ArcheType;
                if (oldToNewFCO.ContainsKey(entry.ArcheType))
                    arch = oldToNewFCO[entry.ArcheType];

                if (oldToNewFCO.ContainsKey(entry))
                    oldToNewFCO[entry].AttachToArcheType(arch, entry.IsInstance);
                else
                {
                    bool tmp = entry.IsInstance;
                    entry.DetachFromArcheType();
                    entry.AttachToArcheType(arch, tmp);
                }
                
                //TODO: some recursive action with children, replace instanceSoln
                if(entry.IsInstance)
                    directInstance(entry);
            }

            foreach (MgaFCO entry in oldConnsAndRefs)
            {
                MgaFCO curr = entry;

                if (entry is MgaConnection)
                {
                    bool replaceConn = false;
                    MgaModel parent = entry.ParentModel;
                    MgaFCO src = (entry as IMgaSimpleConnection).Src;
                    MgaFCO dst = (entry as IMgaSimpleConnection).Dst;
                    MgaFCOs srcRefs = (entry as IMgaSimpleConnection).SrcReferences;
                    MgaFCOs dstRefs = (entry as IMgaSimpleConnection).DstReferences;
                    string role = entry.MetaRole.Name;

                    //instanceSoln(parent as MgaFCO);
                    //instanceSoln(src);
                    //instanceSoln(dst);
                    

                    if (oldToNewFCO.ContainsKey(entry.ParentModel as MgaFCO))
                    {
                        parent = oldToNewFCO[entry.ParentModel as MgaFCO] as MgaModel;
                        replaceConn = true;
                    }
                    if (oldToNewFCO.ContainsKey((entry as IMgaSimpleConnection).Src))
                    {
                        src = oldToNewFCO[(entry as IMgaSimpleConnection).Src];
                        replaceConn = true;
                    }
                    if (oldToNewFCO.ContainsKey((entry as IMgaSimpleConnection).Dst))
                    {
                        dst = oldToNewFCO[(entry as IMgaSimpleConnection).Dst];
                        replaceConn = true;
                    }
                    if (srcRefs != null)
                    {
                        List<int> temp = new List<int>();
                        for (int i = srcRefs.Count; i > 0; --i)
                        {
                            //instanceSoln(srcRefs[i]);
                            if (oldToNewFCO.ContainsKey(srcRefs[i]))
                            {
                                temp.Add(i);
                                replaceConn = true;
                            }
                        }
                        foreach (int i in temp)
                        {
                            srcRefs.Append(oldToNewFCO[srcRefs[i]]);
                        }
                        foreach (int i in temp)
                        {
                            srcRefs.Remove(i);
                        }
                    }
                    if (dstRefs != null)
                    {
                        List<int> temp = new List<int>();
                        for (int i = dstRefs.Count; i > 0; --i)
                        {
                            //instanceSoln(dstRefs[i]);
                            if (oldToNewFCO.ContainsKey(dstRefs[i]))
                            {
                                temp.Add(i);
                                replaceConn = true;
                            }
                        }
                        foreach (int i in temp)
                        {
                            dstRefs.Append(oldToNewFCO[dstRefs[i]]);
                        }
                        foreach (int i in temp)
                        {
                            dstRefs.Remove(i);
                        }
                    }
                    if (oldToNewRole.ContainsKey(entry.MetaRole.Name))
                    {
                        role = oldToNewRole[entry.MetaRole.Name];
                        replaceConn = true;
                    }
                    //special case (featuremap): 
                    if (entry.MetaRole.Name == "FeatureMap")
                    {
                        if (((entry as IMgaSimpleConnection).Src.ParentModel.MetaRole.Name == "CADModel"
                            && (entry as IMgaSimpleConnection).Src.MetaRole.Name == "SurfaceGeometry"
                            && (entry as IMgaSimpleConnection).Src.get_StrAttrByName("Orientation") != "SIDE_A")
                            ||
                            ((entry as IMgaSimpleConnection).Dst.ParentModel.MetaRole.Name == "CADModel"
                            && (entry as IMgaSimpleConnection).Dst.MetaRole.Name == "SurfaceGeometry"
                            && (entry as IMgaSimpleConnection).Dst.get_StrAttrByName("Orientation") != "SIDE_A"))
                        {
                            role = "SurfaceReverseMap";
                            replaceConn = true;
                        }
                    }

                    if (replaceConn)
                    {
                        try{
                            MgaFCO newConn = makeConn(parent, src, dst, role, srcRefs, dstRefs);
                            oldToNewFCO.Add(entry, newConn);
                            if (entry.Name != entry.MetaRole.Name && entry.Name != entry.Meta.Name && entry.Name != entry.Meta.DisplayedName) //name default??
                                newConn.Name = entry.Name;
                            copyFCO(entry, newConn);
                        }
                        catch (Exception ex)
                        {
                            GMEConsole.Error.WriteLine(
                                    "Exception creating connection in <a href=\"mga:{0}\">{1}</a> between <a href=\"mga:{2}\">{3}</a> and <a href=\"mga:{4}\">{5}</a>: {6}",
                                    parent.ID,
                                    parent.Name,
                                    src.ID,
                                    src.Name,
                                    dst.ID,
                                    dst.Name,
                                    ex.Message
                                    );
                        }
                    }
                }
                else if (entry is MgaReference)
                {
                    if (oldToNewFCO.ContainsKey(entry))
                    {
                        curr = oldToNewFCO[entry];
                        (curr as MgaReference).Referred = (entry as MgaReference).Referred;
                    }
                    if ((entry as MgaReference).Referred != null && oldToNewFCO.ContainsKey((entry as MgaReference).Referred))
                    {
                        (curr as MgaReference).Referred = oldToNewFCO[(entry as MgaReference).Referred];
                    }
                }
            }
        }

        private void copyOldLayout()
        {
            foreach (KeyValuePair<MgaFCO, MgaFCO> pair in oldToNewFCO)
            {
                var part = pair.Key.Parts.Cast<IMgaPart>().FirstOrDefault();
                string icon;
                int x;
                int y;
                part.GetGmeAttrs(out icon, out x, out y);
                foreach (var part_New in pair.Value.Parts.Cast<IMgaPart>())
                {
                    part_New.SetGmeAttrs(icon, x, y);
                }
            }
        }

        private void deleteOldFCOs()
        {
            List<MgaFCO> temp = new List<MgaFCO>();
            foreach (MgaFCO fco in oldToNewFCO.Keys)
                if (oldToNewFCO.ContainsKey(fco.ParentModel as MgaFCO) || (fco.ArcheType != null && oldToNewFCO.ContainsKey(fco.ArcheType))) //TODO: instance/subtype
                    temp.Add(fco);
                else if (fco is MgaConnection)
                {
                    fco.DestroyObject();
                    temp.Add(fco);
                }

            foreach (MgaFCO fco in temp)
                oldToNewFCO.Remove(fco);
            foreach (MgaFCO fco in oldToNewFCO.Keys)
                fco.DestroyObject();

            oldToNewFCO.Clear();
            oldConnsAndRefs.Clear();
        }

        private static MgaFCO makeFCO(MgaModel parent, string role)
        {
            MgaFCO newFCO = parent.CreateChildObject((parent.Meta as MgaMetaModel).RoleByName[role]);
            newFCO.Name = role;
            return newFCO;
        }

        private static MgaFCO makeConn(MgaModel parent, MgaFCO src, MgaFCO dst, string role, MgaFCOs srcRef=null, MgaFCOs dstRef=null)
        {
            MgaFCO newConn;
            if (srcRef == null && dstRef == null)
                newConn = parent.CreateSimplerConnDisp((parent.Meta as MgaMetaModel).RoleByName[role], src, dst);
            else
                newConn = parent.CreateSimpleConn((parent.Meta as MgaMetaModel).RoleByName[role], src, dst, srcRef, dstRef);
            newConn.Name = role;
            return newConn;
        }

        private static bool copyFCO(MgaFCO from, MgaFCO to)
        {
            if (from.MetaRole.Name == to.MetaRole.Name)
            {
                to.Name = from.Name;
                foreach (MgaAttribute attrib in from.Attributes)
                    to.SetAttributeByNameDisp((attrib as IMgaAttribute).Meta.Name, from.GetAttributeByNameDisp((attrib as IMgaAttribute).Meta.Name));
                return true;
            }
            return false;
        }

        private bool willBeMigrated(MgaFCO fco)
        {
            if (fco == RootModel)
                return true;
            if (fco.ParentModel != null)
                return willBeMigrated(fco.ParentModel as MgaFCO);
            else if (RootFolder != null)
                return willBeMigrated(fco.ParentFolder);
            return false;
        }

        private bool willBeMigrated(MgaFolder folder)
        {
            if (folder == RootFolder)
                return true;
            return folder.ParentFolder != null && willBeMigrated(folder.ParentFolder);
        }

        private void directInstance(MgaFCO parent) //INSTANCE BUG SOLUTION
        {
            foreach (MgaFCO fco in parent.ChildObjects.OfType<MgaFCO>())
            {
                if (fco.IsInstance //if fco is instance (not needed)
                    && ((!oldToNewFCO.ContainsKey(fco as MgaFCO) && oldToNewRole.ContainsKey(fco.MetaRole.Name)) //and is obselete type
                        || (oldToNewFCO.ContainsKey(fco as MgaFCO) && oldToNewFCO[fco] == null))) //or has new parent
                {
                    MgaFCO arch = fco.ArcheType;
                    if (oldToNewFCO.ContainsKey(fco.ArcheType))
                        arch = oldToNewFCO[fco.ArcheType];
                    foreach (MgaFCO curr in arch.DerivedObjects)
                    {
                        if (oldToNewFCO.ContainsKey(parent as MgaFCO))
                            parent = oldToNewFCO[parent];
                        if (curr.ParentModel == parent && !oldToNewFCO.ContainsValue(curr))
                        {
                            oldToNewFCO.Remove(fco);
                            oldToNewFCO.Add(fco, curr);

                            copyFCO(fco, curr); //if same role

                            //if different role
                            if (fco.Name != fco.MetaRole.Name && fco.Name != fco.Meta.Name && fco.Name != fco.Meta.DisplayedName) //name default?
                                oldToNewFCO[fco].Name = fco.Name;
                            //copy over specified properties to new FCO
                            string role = fco.MetaRole.Name;
                            if (role == "StructuralInterface")
                            {
                                curr.set_StrAttrByName("Definition", fco.get_StrAttrByName("Type"));
                                curr.Name = fco.Name;
                            }
                            else if (role == "SurfaceGeometry" || role == "AxisGeometry" || role == "PointGeometry" || role == "CoordinateSystemGeometry")
                            {
                                curr.set_StrAttrByName("DatumName", fco.get_StrAttrByName("Datum"));
                            }
                            break;
                        }
                    }
                }
                if (fco is MgaModel)
                    directInstance(fco);
            }
        }

        private void UpdateResourcePath(MgaFCO resource, string cadFileExtension)
        {
            string path = resource.get_StrAttrByName("Path");
            if (!String.IsNullOrEmpty(path))
            {
                string filename = Path.GetFileName(path);
                if (!String.IsNullOrEmpty(filename))
                {
                    Match match = Regex.Match(filename, @"((\.prt\.*)|(\.asm\.*))", RegexOptions.IgnoreCase);
                    if (!match.Success)
                    {
                        path += cadFileExtension;
                        resource.set_StrAttrByName("Path", path);

                        GMEConsole.Info.WriteLine("Added file extension ({0}) to Resource's Path attribute: <a href=\"mga:{1}\">{2}</a>.",
                                                  cadFileExtension,
                                                  resource.ID,
                                                  resource.Name);
                    }
                    // might need to do this later: check if existing file extension of resource matches cadFileExtension.
                }
            }
        }


        /*
        private void instanceSoln(MgaFCO fco) //INSTANCE BUG SOLUTION (replaced by directInstance)
        {
            if (fco.IsInstance && !oldToNewFCO.ContainsKey(fco as MgaFCO) && (oldToNewRole.ContainsKey(fco.MetaRole.Name) || oldToNewFCO.ContainsKey(fco.ParentModel as MgaFCO)))
            {
                MgaFCO arch = fco.ArcheType;
                if(oldToNewFCO.ContainsKey(fco.ArcheType))
                    arch = oldToNewFCO[fco.ArcheType];
                MgaModel parent = fco.ParentModel;
                foreach (MgaFCO curr in arch.DerivedObjects)
                {
                    if (oldToNewFCO.ContainsKey(fco.ParentModel as MgaFCO) && oldToNewFCO[fco.ParentModel as MgaFCO] == null)
                        instanceSoln(fco.ParentModel as MgaFCO);
                    if (oldToNewFCO.ContainsKey(fco.ParentModel as MgaFCO))
                        parent = oldToNewFCO[fco.ParentModel as MgaFCO] as MgaModel;
                    if(curr.ParentModel == parent && !oldToNewFCO.ContainsValue(curr))
                    {
                        oldToNewFCO.Add(fco, curr);

                        copyFCO(fco, curr); //if same role
                        //if different role
                        if (fco.Name != fco.MetaRole.Name && fco.Name != fco.Meta.Name && fco.Name != fco.Meta.DisplayedName) //name default?
                            oldToNewFCO[fco].Name = fco.Name;

                        //copy over specified properties to new FCO
                        string role = fco.MetaRole.Name;
                        if (role == "StructuralInterface")
                        {
                            oldToNewFCO[fco].set_StrAttrByName("Definition", fco.get_StrAttrByName("Type"));
                            oldToNewFCO[fco].Name = fco.Name;
                        }
                        else if (role == "SurfaceGeometry" || role == "AxisGeometry" || role == "PointGeometry" || role == "CoordinateSystemGeometry")
                        {
                            oldToNewFCO[fco].set_StrAttrByName("DatumName", fco.get_StrAttrByName("Datum"));
                        } 
                        return;
                    }
                }
            }
        }
        */
    }
}
