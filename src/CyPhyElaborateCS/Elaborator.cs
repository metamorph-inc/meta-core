namespace CyPhyElaborateCS
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using GME.MGA;
    using System.Runtime.InteropServices;

    /// <summary>
    /// Represents a type of elaborator. Use the <see cref="GetElaborator"/> to create an instance of an elaborator based on
    /// a given context.
    /// </summary>
    /// <remarks>NOTE: collapse CANNOT be implemented because of MetaLink assumptions. Limitations are as follows:
    /// <list type="bullet">
    /// <item>Component assemblies cannot be library objects</item>
    /// <item>Component assemblies cannot be read-only objects</item>
    /// <item>Component assemblies cannot be instances</item>
    /// <item>Component assemblies cannot be subtypes</item>
    /// <item>Test benches cannot be library objects</item>
    /// <item>Test benches cannot be read-only objects</item>
    /// <item>Test benches cannot be instances</item>
    /// <item>Test benches cannot be subtypes</item>
    /// </list>
    /// </remarks>
    public abstract class Elaborator
    {
        /// <summary>
        /// Type of the <see cref="MgaFCOs"/>, use this type when creating a new instance of it.
        /// </summary>
        protected static readonly Type MgaFCOsType = Type.GetTypeFromProgID("Mga.MgaFCOs");

        /// <summary>
        /// Component reference objects have this attribute and used by MetaLink
        /// </summary>
        private const string AttributeNameInstanceGuid = "InstanceGUID";

        /// <summary>
        /// Bookkeeping for elaborator
        /// </summary>
        private const string RegistryNameElaborator = "Elaborator";

        /// <summary>
        /// Stores a chain of GUID through which references the elaboration happened. Used by MetaLink
        /// </summary>
        public const string RegistryNameInstanceGuidChain = RegistryNameElaborator + "/" + AttributeNameInstanceGuid + "_Chain";

        /// <summary>
        /// Stores a chain of GME IDs through which references the elaboration happened. Might be used in interpreters.
        /// </summary>
        private const string RegistryNameGmeIDChain = RegistryNameElaborator + "/ID_Chain";

        /// <summary>
        /// Stores the original GME ID of the object that the reference was pointing to before elaboration.
        /// </summary>
        /// <remarks>NOTE: Will be handy on collapse, but collapse CANNOT be implemented because of MetaLink assumptions.</remarks>
        private const string RegistryNameOriginalReferredID = RegistryNameElaborator + "/ReferredID";

        protected bool UnrollConnectors;

        /// <summary>
        /// Gets meta reference id factory.
        /// </summary>
        public CyPhyTypeFactory Factory { get; private set; }

        /// <summary>
        /// Gets or sets a value indicating whether the model is already elaborated or not.
        /// </summary>
        public bool IsElaborated { get; protected set; }

        /// <summary>
        /// Gets the Logger.
        /// </summary>
        public CyPhyGUIs.GMELogger Logger { get; private set; }

        /// <summary>
        /// Gets or sets Component Assembly on which the elaboration will be performed.
        /// </summary>
        public MgaModel Subject { get; set; }

        /// <summary>
        /// Gets or sets traceability between the elaborated (copied) objects and the original objects. Contains GME ID pairs.
        /// </summary>
        public Dictionary<string, string> Traceability { get; set; }

        /// <summary>
        /// Contains InstanceGUIDs of Components that have been elaborated thus far
        /// </summary>
        public ISet<string> ComponentGUIDs { get; set; }

        public Dictionary<MgaFCO, MgaFCO> ComponentCopyMap { get; set; }

        public Elaborator(bool UnrollConnectors)
        {
            ComponentCopyMap = new Dictionary<MgaFCO, MgaFCO>();
            this.UnrollConnectors = UnrollConnectors;
        }

        /// <summary>
        /// Gets a new instance of an elaborator based on a given context.
        /// </summary>
        /// <param name="subject">Given context</param>
        /// <param name="logger">Logger which can be used to log messages.</param>
        /// <returns>A new instance of a context aware elaborator.</returns>
        /// <exception cref="ArgumentNullException">If subject or logger null.</exception>
        /// <exception cref="NotSupportedException">If subject does not have an associated elaborator class.</exception>
        public static Elaborator GetElaborator(MgaModel subject, CyPhyGUIs.GMELogger logger, bool UnrollConnectors)
        {
            if (subject == null)
            {
                throw new ArgumentNullException("subject");
            }

            if (logger == null)
            {
                throw new ArgumentNullException("logger");
            }

            var factory = new CyPhyTypeFactory(subject.Project.RootMeta);
            Elaborator elaborator = null;

            if (subject.MetaBase.MetaRef == factory.TestBenchMeta ||
                subject.MetaBase.MetaRef == factory.CADTestBenchMeta ||
                subject.MetaBase.MetaRef == factory.BlastTestBenchMeta ||
                subject.MetaBase.MetaRef == factory.BallisticTestBenchMeta ||
                subject.MetaBase.MetaRef == factory.CFDTestBenchMeta ||
                subject.MetaBase.MetaRef == factory.KinematicTestBenchMeta ||
                subject.MetaBase.MetaRef == factory.CarTestBenchMeta)
            {
                elaborator = new TestBenchTypeElaborator(subject, UnrollConnectors);
            }
            else if (subject.MetaBase.MetaRef == factory.ComponentAssemblyMeta)
            {
                elaborator = new ComponentAssemblyElaborator(subject, UnrollConnectors);
            }
            else if (subject.MetaBase.MetaRef == factory.DesignContainerMeta)
            {
                elaborator = new DesignContainerElaborator(subject, UnrollConnectors);
            }
            else
            {
                throw new NotSupportedException(string.Format("Not supported context: {0} [{1}]", subject.Name, subject.MetaBase.Name));
            }

            if (subject.IsLibObject)
            {
                throw new NotSupportedException(string.Format("{0} cannot be a library object.", subject.MetaBase.Name));
            }

            elaborator.Logger = logger;
            elaborator.Factory = factory;
            // TODO: how can we determine this?
            elaborator.IsElaborated = false;

            return elaborator;
        }

        /// <summary>
        /// Gets a new instance of an elaborator based on a given context.
        /// </summary>
        /// <typeparam name="T">Specific type of elaborator</typeparam>
        /// <param name="subject">Given context</param>
        /// <param name="logger">Logger which can be used to log messages.</param>
        /// <returns>A new instance of a context aware elaborator.</returns>
        /// <exception cref="ArgumentNullException">If subject or logger null.</exception>
        /// <exception cref="NotSupportedException">If subject does not have an associated elaborator class.</exception>
        /// <exception cref="InvalidCastException">If the created elaborator cannot be casted to the requested type.</exception>
        public static T GetElaborator<T>(MgaModel subject, CyPhyGUIs.GMELogger logger, bool UnrollConnectors)
            where T : Elaborator
        {
            return (T)GetElaborator(subject, logger, UnrollConnectors);
        }

        /// <summary>
        /// Elaborates the current context. Updates the <see cref="IsElaborated"/> flag.
        /// </summary>
        public abstract void Elaborate();

        /// <summary>
        /// Logs debugging information about the current status of elaboration.
        /// </summary>
        /// <param name="message">Message to log.</param>
        /// <param name="obj">Object on which the action was performed.</param>
        protected void LogDebug(string message, IMgaObject obj)
        {
            this.Logger.WriteDebug("{0} - {1}: {2} [{3}]", obj.ID, message, obj.Name, obj.MetaBase.Name);
        }

        /// <summary>
        /// Utility function to switch a reference to a model copy or instance. Reference should be deleted after this call.
        /// </summary>
        /// <remarks>In case copy is requested and the target element is a derived type (i.e. subtype or instance) inheritance chain will be cut. To make further elaboration possible.</remarks>
        /// <param name="parentModel">Parent model of the reference</param>
        /// <param name="reference">Reference to switch to a model</param>
        /// <param name="createInstance">If true creates an instance otherwise makes a copy of the target object. See remarks.</param>
        /// <returns>Model which was created instead of the reference.</returns>
        protected MgaModel SwitchReferenceToModel(MgaModel parentModel, MgaReference reference, bool createInstance)
        {
            this.LogDebug("Switching reference to model", reference);

            if (reference.Referred == null)
            {
                // failed to elaborate
                this.Logger.WriteFailed("Reference is null. {0} {1} [{2}]", reference.ID, reference.Name, reference.MetaBase.Name);
                return null;
            }

            var referred = reference.Referred;

            // FIXME: this could fail, e.g. if componentref refers to a test component and we are in a CA
            GME.MGA.Meta.MgaMetaRole role = null;

            foreach (GME.MGA.Meta.MgaMetaRole roleItem in (parentModel.Meta as GME.MGA.Meta.MgaMetaModel)
                .Roles)
            {
                if (roleItem.Kind.MetaRef == referred.MetaBase.MetaRef)
                {
                    role = roleItem;
                    break;
                }
            }

            if (role == null)
            {
                throw new ElaboratorException(string.Format("Role was not found for {0} [{1}] in {2} [{3}]", referred.Name, referred.MetaBase.Name, parentModel.Name, parentModel.MetaBase.Name));
            }

            MgaFCO copiedObj = null;

            Func<MgaFCO, MgaFCO, MgaFCO> getOriginalChildObject = null;

            if (createInstance)
            {
                this.LogDebug("Creating instance from", referred);

                MgaFCO originalReferred = null;
                if (UnrollConnectors)
                {
                    originalReferred = referred;
                    MgaFCO referredCopy;
                    if (ComponentCopyMap.TryGetValue(referred, out referredCopy))
                    {
                    }
                    else
                    {
                        // FIXME update traceability
                        referredCopy = referred.ParentFolder.CopyFCODisp(referred);
                        ComponentCopyMap[referred] = referredCopy;
                    }
                    referred = referredCopy;
                }

                copiedObj = parentModel.DeriveChildObject(referred, role, true);

                Func<IMgaFCO, int> getRelID = (fco) => {
                    if (fco.DerivedFrom != null && fco.IsPrimaryDerived == false)
                    {
                        // workaround GME bug fixed 2/27/2017
                        const int RELID_BASE_MAX = 0x07FFFFFF; // Mga.idl
                        return (~RELID_BASE_MAX & fco.RelID) | (RELID_BASE_MAX & fco.DerivedFrom.RelID);

                    }
                    return fco.RelID;
                };
                Func<MgaFCO, string> getFCORelIds = fco =>
                {
                    string ret = "/#" + getRelID(fco);
                    MgaModel parent = fco.ParentModel;
                    while (parent != null)
                    {
                        ret = "/#" + getRelID(parent) + ret;
                        parent = parent.ParentModel;
                    }
                    return ret;
                };

                getOriginalChildObject = new Func<MgaFCO, MgaFCO, MgaFCO>((parent, copied) =>
                {
                    if (UnrollConnectors == false)
                    {
                        return copied.DerivedFrom;
                    }
                    string relPath = getFCORelIds(copied.DerivedFrom).Substring(getFCORelIds(referred).Length);
                    var ret = (MgaFCO)originalReferred.GetObjectByPathDisp(relPath);
                    if (ret == null)
                    {
                        ret = originalReferred;
                        // GME bug workaround continued
                        foreach (var relID in relPath.Substring(2).Split(new[] { "/#" }, StringSplitOptions.None))
                        {
                            ret = ret.ChildObjects.Cast<MgaFCO>().Where(f => getRelID(f).ToString() == relID).First();
                        }
                        if (ret == null)
                        {
                            throw new ArgumentNullException();
                        }
                    }
                    return ret;
                });

                var guid = reference.StrAttrByName[AttributeNameInstanceGuid];
                if (string.IsNullOrWhiteSpace(guid))
                {
                    reference.set_StrAttrByName(AttributeNameInstanceGuid, reference.GetGuidDisp());
                    guid = reference.StrAttrByName[AttributeNameInstanceGuid];
                    this.LogDebug("InstanceGUID was empty for reference, assigned new : " + guid, reference);
                }

                // parent model should contain the concatenated InstanceGUID
                string guidConcat = parentModel.RegistryValue[RegistryNameInstanceGuidChain] + guid;

                copiedObj.RegistryValue[RegistryNameInstanceGuidChain] = guidConcat;
                copiedObj.RegistryValue[RegistryNameOriginalReferredID] = reference.Referred.ID;

                this.LogDebug(string.Format("Overwriting InstanceGUID to {0}", guidConcat), referred);

                copiedObj.StrAttrByName[AttributeNameInstanceGuid] = guidConcat;

                copiedObj.RegistryValue[RegistryNameGmeIDChain] = parentModel.RegistryValue[RegistryNameGmeIDChain] + "," + reference.ID + "," + referred.ID;
            }
            else
            {
                this.LogDebug("Creating copy from", referred);

                copiedObj = parentModel.CopyFCODisp(referred, role);

                if (copiedObj.ArcheType != null)
                {
                    // instance or subtype
                    this.LogDebug("Cutting inheritance for copied object", copiedObj);
                    copiedObj.DetachFromArcheType();
                }

                getOriginalChildObject = new Func<MgaFCO, MgaFCO, MgaFCO>((parent, copied) =>
                {
                    // only chance to find objects are based on relids, which might not be unique
                    return parent.ChildObjectByRelID[copied.RelID] as MgaFCO;
                });

                copiedObj.RegistryValue[RegistryNameGmeIDChain] = parentModel.RegistryValue[RegistryNameGmeIDChain] + "," + reference.ID + "," + referred.ID;

                if (reference.MetaBase.MetaRef == this.Factory.ComponentRefMeta)
                {
                    var guid = reference.StrAttrByName[AttributeNameInstanceGuid];

                    copiedObj.RegistryValue[RegistryNameInstanceGuidChain] = parentModel.RegistryValue[RegistryNameInstanceGuidChain] + guid;
                    copiedObj.RegistryValue[RegistryNameOriginalReferredID] = reference.Referred.ID;

                    // push current instance GUID down to all component assembly elements
                    MgaFilter filter = copiedObj.Project.CreateFilter();
                    filter.ObjType = GME.MGA.Meta.objtype_enum.OBJTYPE_MODEL.ToString();
                    // filter.Kind = "ComponentAssembly";

                    foreach (MgaFCO obj in (copiedObj as MgaModel).GetDescendantFCOs(filter))
                    {
                        if (obj.MetaBase.MetaRef == this.Factory.ComponentAssemblyMeta)
                        {
                            obj.RegistryValue[RegistryNameInstanceGuidChain] = copiedObj.RegistryValue[RegistryNameInstanceGuidChain];

                            obj.RegistryValue[RegistryNameGmeIDChain] = copiedObj.RegistryValue[RegistryNameGmeIDChain];
                        }
                    }
                }
            }

            copiedObj.Name = reference.Name;
            if (reference.Meta.MetaRef == this.Factory.ComponentRefMeta && copiedObj.Meta.MetaRef == this.Factory.ComponentAssemblyMeta)
            {
                // TODO what should happen here
                // var managedGuid = reference.StrAttrByName["ManagedGUID"]; ComponentRef does not have ManagedGUID
                var managedGuid = reference.StrAttrByName["InstanceGUID"];
                if (string.IsNullOrEmpty(managedGuid) == false)
                {
                //    copiedObj.StrAttrByName["ManagedGUID"] = managedGuid;
                }
            }

            foreach (MgaPart part in reference.Parts)
            {
                int x;
                int y;
                string icon;
                part.GetGmeAttrs(out icon, out x, out y);

                try
                {
                    copiedObj.Part[part.MetaAspect].SetGmeAttrs(icon, x, y);
                }
                catch (System.Runtime.InteropServices.COMException)
                {
                    // It's okay. This means that Reference is visible in this aspect, but copiedObj isn't
                }
            }

            // add / update to traceability
            string baseId = null;

            if (this.Traceability.TryGetValue(reference.ID, out baseId))
            {
                //// if it is already in the traceability it is a temporary object.
                //// we need our final object in the mapping

                this.Traceability.Remove(reference.ID);
                this.Traceability.Add(copiedObj.ID, baseId);
            }
            else
            {
                this.Traceability.Add(copiedObj.ID, referred.ID);
            }

            // safe connection information for switching connections later.
            Dictionary<MgaFCO, MgaFCO> original2Copied = new Dictionary<MgaFCO, MgaFCO>();

            original2Copied.Add(referred, copiedObj);

            MgaFCOs objects = null;

            if (createInstance)
            {
                objects = (copiedObj as MgaModel).GetDescendantFCOs(copiedObj.Project.CreateFilter());
            }
            else
            {
                objects = (copiedObj as MgaModel).ChildFCOs;
            }

            foreach (MgaFCO copied in objects)
            {
                if (copied == copiedObj)
                {
                    continue;
                }

                var original = getOriginalChildObject(referred, copied);
                original2Copied.Add(original, copied);

                if (this.Traceability.TryGetValue(original.ID, out baseId))
                {
                    // if it is already in the traceability it is a temporary object.
                    // we need our final object in the mapping
                    this.Traceability.Remove(original.ID);
                    this.Traceability.Add(copied.ID, baseId);
                }
                else
                {
                    this.Traceability.Add(copied.ID, original.ID);
                }

                if (!createInstance && copied.ObjType == GME.MGA.Meta.objtype_enum.OBJTYPE_MODEL)
                {
                    this.AddRecursivelyTraceability(copied, original);
                }
            }

            // switch connections to refport children
            foreach (MgaConnPoint connPoint in reference.UsedByConns)
            {
                var connection = connPoint.Owner as IMgaSimpleConnection;

                if (connPoint.ConnRole == "src" &&
                    connection.SrcReferences.Count > 0 &&
                    connection.SrcReferences[1] == reference)
                {
                    // what if this is instance
                    var newEndPoint = original2Copied[connection.Src];

                    if (newEndPoint == null)
                    {
                        throw new Exception("null connection endpoint");
                    }

                    // create an empty array for the connection references
                    MgaFCOs emptyArray = (MgaFCOs)Activator.CreateInstance(Elaborator.MgaFCOsType);
                    connection.SetSrc(emptyArray, newEndPoint);
                } 
                
                if (connPoint.ConnRole == "dst" &&
                    connection.DstReferences.Count > 0 &&
                    connection.DstReferences[1] == reference)
                {
                    // what if this is instance
                    var newEndPoint = original2Copied[connection.Dst];

                    if (newEndPoint == null)
                    {
                        throw new Exception("null connection endpoint");
                    }

                    // create an empty array for the connection references
                    MgaFCOs emptyArray = (MgaFCOs)Activator.CreateInstance(Elaborator.MgaFCOsType);
                    connection.SetDst(emptyArray, newEndPoint);
                }
            }

            // switch connections to original reference object itself
            foreach (MgaConnPoint connPoint in reference.PartOfConns)
            {
                var connection = connPoint.Owner as IMgaSimpleConnection;

                if (connPoint.ConnRole == "src")
                {
                    // create an empty array for the connection references
                    MgaFCOs emptyArray = (MgaFCOs)Activator.CreateInstance(Elaborator.MgaFCOsType);
                    try
                    {
                        connection.SetSrc(emptyArray, copiedObj);
                    }
                    catch (COMException ex)
                    {
                        if (ex.Message.Contains("Illegal connection") &&
                            ex.Message.Contains("meta violation"))
                        {
                            var refID = reference.ID;
                            while (Traceability.ContainsKey(refID))
                            {
                                refID = Traceability[refID];
                            }

                            Logger.WriteWarning("<a href=\"mga:{0}\">{1}</a>'s referred type ({2}) isn't a legal SRC target for {3} connections. Skipping re-creation of this connection.",
                                                refID,
                                                reference.Name,
                                                reference.Referred.MetaBase.Name,
                                                connection.MetaBase.Name);
                        }
                        else
                        {
                            throw ex;
                        }
                    }
                }
                else if (connPoint.ConnRole == "dst")
                {
                    // create an empty array for the connection references
                    MgaFCOs emptyArray = (MgaFCOs)Activator.CreateInstance(Elaborator.MgaFCOsType);
                    try
                    {
                        connection.SetDst(emptyArray, copiedObj);
                    }
                    catch (COMException ex)
                    {
                        if (ex.Message.Contains("Illegal connection") &&
                            ex.Message.Contains("meta violation"))
                        {
                            var refID = reference.ID;
                            while (Traceability.ContainsKey(refID))
                            {
                                refID = Traceability[refID];
                            }

                            Logger.WriteWarning("<a href=\"mga:{0}\">{1}</a>'s referred type ({2}) isn't a legal DST target for {3} connections. Skipping re-creation of this connection.",
                                                refID,
                                                reference.Name,
                                                reference.Referred.MetaBase.Name,
                                                connection.MetaBase.Name);
                        }
                        else
                        {
                            throw ex;
                        }
                    }
                }
            }

            return copiedObj as MgaModel;
        }

        /// <summary>
        /// Adds all child objects recursively to the traceability map. Relies on unique relative IDs.
        /// </summary>
        /// <param name="copied">Object that was copied from the original one.</param>
        /// <param name="original">Original object from which the copy was made.</param>
        private void AddRecursivelyTraceability(MgaFCO copied, MgaFCO original)
        {
            var componentAssembly = copied as MgaModel;

            foreach (MgaFCO child in componentAssembly.ChildFCOs)
            {
                var originalChild = original.ChildObjectByRelID[child.RelID] as MgaFCO;
                this.Traceability.Add(child.ID, originalChild.ID);

                if (child.ObjType == GME.MGA.Meta.objtype_enum.OBJTYPE_MODEL)
                {
                    this.AddRecursivelyTraceability(child, originalChild);
                }
            }
        }
    }
}