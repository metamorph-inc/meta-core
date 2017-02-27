namespace CyPhyElaborateCS
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using GME.MGA;

    /// <summary>
    /// Elaborator logic for any test bench types.
    /// </summary>
    public class TestBenchTypeElaborator : Elaborator
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="TestBenchTypeElaborator"/> class with a subject
        /// object on which the elaboration will be performed.
        /// </summary>
        /// <param name="subject">Component Assembly object.</param>
        /// <exception cref="ArgumentNullException">If any arguments are null</exception>
        public TestBenchTypeElaborator(MgaModel subject, bool UnrollConnectors)
            : base(UnrollConnectors)
        {
            if (subject == null)
            {
                throw new ArgumentNullException("subject");
            }

            // initialize object with the given parameters
            this.Subject = subject;

            // initialize collections
            this.Traceability = new Dictionary<string, string>();
            this.ComponentGUIDs = new HashSet<string>();
        }

        /// <summary>
        /// Elaborates the give test bench.
        /// </summary>
        public override void Elaborate()
        {
            // gets all objects within the test bench in any depth.
            MgaFilter filter = this.Subject.Project.CreateFilter();
            var allObjects = this.Subject.GetDescendantFCOs(filter);

            // list of test injection points in the test bench
            List<MgaReference> tips = new List<MgaReference>();
            
            foreach (MgaFCO obj in allObjects)
            {
                // add object to traceability
                this.Traceability.Add(obj.ID, obj.ID);

                if (this.Factory.TestInjectionPointsMeta.Contains(obj.MetaBase.MetaRef))
                {
                    // if object is a kind of test injection point
                    if (obj is MgaReference)
                    {
                        // if it is a reference
                        if ((obj as MgaReference).Referred == null)
                        {
                            this.Logger.WriteWarning(string.Format("{0} [{1}] was ignored and skipped, because it points to a null reference.", obj.Name, obj.MetaBase.Name));
                        }
                        else
                        {
                            // has a valid reference which is not null
                            tips.Add(obj as MgaReference);
                        }
                    }
                    else
                    {
                        this.Logger.WriteError(string.Format("{0} [{1}] is not a reference, therefore it was ignored and skipped.", obj.Name, obj.MetaBase.Name));
                    }
                }
            }

            // get the top level system under test object for this test bench
            var tlsut = allObjects.OfType<MgaReference>().FirstOrDefault(x => x.MetaBase.Name == "TopLevelSystemUnderTest");
            
            // ASSUME we have exactly one
            
            // make sure it is not null
            if (tlsut == null)
            {
                this.Logger.WriteWarning(string.Format("No top level system under test object in {0} [{1}]", this.Subject.Name, this.Subject.MetaBase.Name));
                this.Logger.WriteWarning("Assumes [{0}] has been elaborated...", this.Subject.MetaBase.Name);
                this.IsElaborated = true;
                return;
            }

            // it has a null reference
            if (tlsut.Referred == null)
            {
                throw new ElaboratorException(string.Format("Top level system under test object in {0} [{1}] is a null reference.", this.Subject.Name, this.Subject.MetaBase.Name));
            }

            // switch the top level system under test to a Component Assembly
            // FIXME: what if it is not a component assembly ???
            string tlsu_referred_id = tlsut.Referred.ID;
            var ca_tlsut = this.SwitchReferenceToModel(this.Subject, tlsut, false);

            // delete the reference object.
            tlsut.DestroyObject();

            // component assembly elaborator used to elaborate the Top Level System Under Test object.
            ComponentAssemblyElaborator componentAssemblyElaborator = null;

            // get a Componenet assembly elaborator for the top level system under test object.
            componentAssemblyElaborator = Elaborator.GetElaborator<ComponentAssemblyElaborator>(ca_tlsut, this.Logger, UnrollConnectors);

            // pass our current traceability information
            componentAssemblyElaborator.Traceability = this.Traceability;
            componentAssemblyElaborator.ComponentGUIDs = this.ComponentGUIDs;

            // elaborate the top level system under test object
            componentAssemblyElaborator.Elaborate();

            // TODO: Elaborate test components

            // get a look up map for all test injection point references
            var map = componentAssemblyElaborator.GetReverseLookupMap(tips.Select(x => x.Referred.ID));
            map[tlsu_referred_id] = new MgaFCO[] { (MgaFCO)ca_tlsut }.ToList();

            // gather all information about test injection points, pretend everything is ok.
            bool success = true;

            foreach (MgaReference tip in tips)
            {
                // get the new targets for this test injection point
                var tipTargets = map[tip.Referred.ID];

                // looking for exactly one target
                if (tipTargets.Count == 0)
                {
                    // no target mark it as failure
                    success = false;
                    this.Logger.WriteFailed("{0} [{1}] --> {2} [{3}] was not found in traceability.", tip.Name, tip.MetaBase.Name, tip.Referred.Name, tip.Referred.MetaBase.Name);
                }
                else if (tipTargets.Count == 1)
                {
                    // exactly one target
                    try
                    {
                        // redirect test injection point to the new target
                        var switcher = new ReferenceSwitcher.ReferenceSwitcherInterpreter();
                        switcher.SwitchReference(tipTargets[0] as IMgaFCO, tip as IMgaReference);
                    }
                    catch (Exception ex)
                    {
                        success = false;

                        // handle failures for this (use case we can lose ports/connections/
                        // what if something is an instance/subtype/readonly etc...
                        this.Logger.WriteFailed("{0} [{1}] --> {2} [{3}] redirecting to --> {4} [{5}]", tip.Name, tip.MetaBase.Name, tip.Referred.Name, tip.Referred.MetaBase.Name, tipTargets[0].Name, tipTargets[0].MetaBase.Name);
                        this.Logger.WriteDebug(ex.ToString());
                    }
                }
                else
                {
                    // tipTarget.Count > 1
                    // more than one possible targets, this case is ambiguous, therefore mark it as failure.
                    success = false;
                    this.Logger.WriteFailed("{0} [{1}] --> {2} [{3}] was found more than once in the system - the choice is ambiguous.", tip.Name, tip.MetaBase.Name, tip.Referred.Name, tip.Referred.MetaBase.Name);
                }
            }

            if (tips.Any())
            {
                if (success)
                {
                    // check if no problems
                    this.Logger.WriteInfo("All test injection points were redirected successfully.");
                }
                else
                {
                    // some problems occured, must raise an exception
                    throw new ElaboratorException("At least one test injection point was not redirected successfully. See log messages above.");
                }
            }

            // if this point is reached, mark it successful.
            this.IsElaborated = true;
        }
    }
}