namespace CyPhyElaborateCS
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using GME.MGA;

    /// <summary>
    /// Elaborator for a DesignContainer.
    /// </summary>
    public class DesignContainerElaborator : Elaborator
    {
        public DesignContainerElaborator(MgaModel subject)
        {
            if (subject == null)
            {
                throw new ArgumentNullException("subject");
            }

            // initialize subject
            this.Subject = subject;

            // initialize collections
            this.Traceability = new Dictionary<string, string>();
            this.ComponentGUIDs = new HashSet<string>();
            //this.InnerElaborators = new List<ComponentAssemblyElaborator>();
            this.InnerElaborators = new List<Elaborator>();
            this.ComponentAssemblyReferences = new Queue<MgaFCO>();
            this.DesignSpaceReferences = new Queue<MgaFCO>();
        }

        /// <summary>
        /// Gets or sets elaborators for contained component assembly references or
        /// DesignContainer references that need further elaboration.
        /// </summary>
        public List<Elaborator> InnerElaborators { get; set; }
        //public List<ComponentAssemblyElaborator> InnerElaborators { get; set; }

        /// <summary>
        /// Gets or sets the already processed Component Assembly references from the parent to this object. Does not
        /// contain any branches. Used to detect circular dependencies between tree branches.
        /// </summary>
        public Queue<MgaFCO> ComponentAssemblyReferences { get; set; }

        /// <summary>
        /// Gets or sets the already processed DesignSpace references from the parent to this object. Does not
        /// contain any branches. Used to detect circular dependencies between tree branches.
        /// </summary>
        public Queue<MgaFCO> DesignSpaceReferences { get; set; }

        /// <summary>
        /// Returns true if parent is either a Configurations or CWC
        /// </summary>
        /// <param name="parent"></param>
        /// <returns></returns>
        protected bool IsConfigurationType(MgaObject parent)
        {
            if (parent.MetaBase.MetaRef == this.Factory.ConfigurationsMeta ||
                parent.MetaBase.MetaRef == this.Factory.CWCMeta)
            {
                return true;
            }

            return false;
        }

        /// <summary>
        /// Elaborates a DesignContainer object.
        /// </summary>
        public override void Elaborate()
        {
            MgaFilter filter = this.Subject.Project.CreateFilter();

            var allObjects = this.Subject.GetDescendantFCOs(filter);
            foreach (MgaFCO obj in allObjects)
            {
                if (obj.IsPrimaryDerived)
                {
                    obj.DetachFromArcheType();
                }
            }

            foreach (MgaFCO obj in allObjects)
            {
                if (this.Traceability.ContainsKey(obj.ID) == false)
                {
                    // add to traceability
                    this.Traceability.Add(obj.ID, obj.ID);
                }

                if (obj is MgaReference)
                {
                    var reference = obj as MgaReference;
                    if (reference.Referred == null)
                    {
                        if (reference.MetaBase.MetaRef == this.Factory.ComponentRefMeta)
                        {
                            this.Logger.WriteWarning(string.Format("Null {0} [{1}] was ignored and skipped.", reference.Name, reference.MetaBase.Name));
                        }

                        continue;
                    }

                    var referred = reference.Referred;

                    MgaObject parent = null;
                    GME.MGA.Meta.objtype_enum type;
                    reference.GetParent(out parent, out type);

                    if (IsConfigurationType(parent))
                    {
                        continue;
                    }
                    if (parent.MetaBase.MetaRef == Factory.DecisionGroupMeta || parent.MetaBase.MetaRef == Factory.VisualConstraintMeta
                        || parent.MetaBase.MetaRef == Factory.And_operatorMeta || parent.MetaBase.MetaRef == Factory.Or_operatorMeta
                        || parent.MetaBase.MetaRef == Factory.Not_operatorMeta)
                    {
                        // DecisionGroup can hold only ComponentRefs, so we can't do anything with its children (META-3595)
                        continue;
                    }

                    if (referred.MetaBase.MetaRef == this.Factory.ComponentMeta)
                    {
                        var copied = this.SwitchReferenceToModel(parent as MgaModel, reference, true);

                        // delete reference
                        reference.DestroyObject();
                    }
                    else if (referred.MetaBase.MetaRef == this.Factory.ComponentAssemblyMeta)
                    {
                        MgaObject parent2 = parent;
                        GME.MGA.Meta.objtype_enum type2;

                        // worst case this will terminate at the root folder level
                        while (parent2 != null && parent2 is MgaModel)
                        {
                            // FIXME: is this safe? should we compare IDs?
                            if (parent2 == reference.Referred)
                            {
                                string message = string.Format("Circular dependency: {0} --> {1}", parent2.Name, reference.Referred.Name);
                                throw new ElaboratorCircularReferenceException(message);
                            }

                            parent2.GetParent(out parent2, out type2);
                        }

                        if (this.ComponentAssemblyReferences.Any(x => x.ID == reference.Referred.ID))
                        {
                            string message = string.Format("Circular dependency: {0} --> {1}", string.Join(" -> ", this.ComponentAssemblyReferences.Select(x => x.Name)), reference.Referred.Name);
                            throw new ElaboratorCircularReferenceException(message);
                        }

                        var copied = this.SwitchReferenceToModel(parent as MgaModel, reference, false);

                        // prevent circular dependency
                        var innerElaborator = Elaborator.GetElaborator(copied, this.Logger) as ComponentAssemblyElaborator;

                        // use only one map
                        innerElaborator.Traceability = this.Traceability;
                        innerElaborator.ComponentGUIDs = this.ComponentGUIDs;

                        // hold only one queue
                        foreach (var item in this.ComponentAssemblyReferences)
                        {
                            innerElaborator.ComponentAssemblyReferences.Enqueue(item);
                        }

                        innerElaborator.ComponentAssemblyReferences.Enqueue(reference.Referred);
                        this.InnerElaborators.Add(innerElaborator);
                        innerElaborator.Elaborate();

                        // delete reference
                        reference.DestroyObject();
                    }
                    else if (referred.MetaBase.MetaRef == this.Factory.DesignContainerMeta)
                    {
                        MgaObject parent2 = parent;
                        GME.MGA.Meta.objtype_enum type2;

                        // worst case this will terminate at the root folder level
                        while (parent2 != null && parent2 is MgaModel)
                        {
                            // FIXME: is this safe? should we compare IDs?
                            if (parent2 == reference.Referred)
                            {
                                string message = string.Format("Circular dependency: {0} --> {1}", parent2.Name, reference.Referred.Name);
                                throw new ElaboratorCircularReferenceException(message);
                            }

                            parent2.GetParent(out parent2, out type2);
                        }

                        if (this.DesignSpaceReferences.Any(x => x.ID == reference.Referred.ID))
                        {
                            string message = string.Format("Circular dependency: {0} --> {1}", string.Join(" -> ", this.DesignSpaceReferences.Select(x => x.Name)), reference.Referred.Name);
                            throw new ElaboratorCircularReferenceException(message);
                        }

                        var copied = this.SwitchReferenceToModel(parent as MgaModel, reference, false);

                        var innerElaborator = Elaborator.GetElaborator(copied, this.Logger) as DesignContainerElaborator;

                        // use only one map
                        innerElaborator.Traceability = this.Traceability;
                        innerElaborator.ComponentGUIDs = this.ComponentGUIDs;

                        // hold only one queue
                        foreach (var item in this.DesignSpaceReferences)
                        {
                            innerElaborator.DesignSpaceReferences.Enqueue(item);
                        }

                        innerElaborator.DesignSpaceReferences.Enqueue(reference.Referred);
                        this.InnerElaborators.Add(innerElaborator);
                        innerElaborator.Elaborate();

                        // delete reference
                        reference.DestroyObject();
                    }
                }
            }
        }
    }
}