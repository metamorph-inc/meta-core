using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhyMasterInterpreter.Rules
{
    /// <summary>
    /// Base class for any test bench context checker, which inherits from TestBenchType.
    /// </summary>
    /// <typeparam name="T">Specific test bench type, e.g. BallisticTestBench</typeparam>
    public class TestBenchTypeChecker<T> : ContextChecker where T : CyPhy.TestBenchType
    {
        /// <summary>
        /// Test bench context on which the checker runs.
        /// </summary>
        public T testBench { get; set; }

        public TestBenchTypeChecker(T testBench)
        {
            this.testBench = testBench;
        }

        /// <summary>
        /// Gets the model element where the context check starts.
        /// </summary>
        /// <returns>Model context that the context checker can check.</returns>
        public override GME.MGA.IMgaModel GetContext()
        {
            return this.testBench.Impl as GME.MGA.IMgaModel;
        }

        /// <summary>
        /// Checks the current context without throwing any exceptions.
        /// Clears and saves the context check results.
        /// <para><remarks>Execute within transaction.</remarks></para>
        /// </summary>
        public override void CheckNoThrow()
        {
            base.CheckNoThrow();

            this.m_details.AddRange(this.TopLevelSystemUnderTest());
            this.m_details.AddRange(this.NoComponentRefOrComponents());
        }

        /// <summary>
        /// TODO: description what we check...
        /// <para><remarks>Execute within transaction.</remarks></para>
        /// </summary>
        /// <returns>Checker results.</returns>
        protected IEnumerable<ContextCheckerResult> TopLevelSystemUnderTest()
        {
            List<ContextCheckerResult> results = new List<ContextCheckerResult>();

            foreach (var tlsut in this.testBench.Children.TopLevelSystemUnderTestCollection)
            {
                if ((tlsut.Impl as GME.MGA.IMgaReference).Referred == null)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = tlsut.Impl,
                        Message = "TopLevelSystemUnderTest object is null reference."
                    };

                    results.Add(feedback);
                }
                else
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = tlsut.Impl,
                        Message = "TopLevelSystemUnderTest object is not a null reference."
                    };

                    results.Add(feedback);
                }
            }

            var tlsutCount = this.testBench.Children.TopLevelSystemUnderTestCollection.Count();

            if (tlsutCount == 0)
            {
                var feedback = new ContextCheckerResult()
                {
                    Success = false,
                    Subject = this.testBench.Impl,
                    Message = "No TopLevelSystemUnderTest object."
                };

                results.Add(feedback);
            }
            else if (tlsutCount == 1)
            {
                var tlsut = this.testBench.Children.TopLevelSystemUnderTestCollection.FirstOrDefault();
                var tlsutRefered = tlsut.Referred.DesignEntity;

                if (tlsutRefered == null)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = tlsut.Impl,
                        Message = string.Format("TopLevelSystemUnderTest is null.")
                    };

                    results.Add(feedback);
                }
                else if (tlsutRefered is CyPhy.TestComponent ||
                    tlsutRefered is CyPhy.ComponentRef ||
                    tlsutRefered is CyPhy.Component)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = tlsut.Impl,
                        Message = string.Format("TopLevelSystemUnderTest cannot refer to a {0}.", tlsutRefered.Kind)
                    };

                    results.Add(feedback);
                }
                else
                {
                    if (tlsutRefered is CyPhy.ComponentAssembly)
                    {
                        var feedback = new ContextCheckerResult()
                        {
                            Success = true,
                            Subject = tlsut.Impl,
                            Message = string.Format("TopLevelSystemUnderTest refers to {0}.", tlsutRefered.Kind)
                        };

                        results.Add(feedback);
                    }
                    else if (tlsutRefered is CyPhy.DesignContainer)
                    {
                        var designContainer = tlsutRefered as CyPhy.DesignContainer;
                        var feedback = new ContextCheckerResult()
                        {
                            Success = true,
                            Subject = tlsut.Impl,
                            Message = string.Format("TopLevelSystemUnderTest refers to {0} {1}.",
                                designContainer.Attributes.ContainerType,
                                designContainer.Kind)
                        };

                        results.Add(feedback);
                    }

                    if (tlsutRefered.Impl.ID == (tlsutRefered.Impl as GME.MGA.MgaModel).RootFCO.ID)
                    {
                        var feedback = new ContextCheckerResult()
                        {
                            Success = true,
                            Subject = tlsutRefered.Impl,
                            Message = string.Format("'{0}' [TopLevelSystemUnderTest] refers to a valid object [{1}] and the {1} is in a folder.", tlsut.Name, tlsutRefered.Kind)
                        };

                        results.Add(feedback);
                    }
                    else
                    {
                        var feedback = new ContextCheckerResult()
                        {
                            Success = false,
                            Subject = tlsutRefered.Impl,
                            Message = string.Format("'{0}' [TopLevelSystemUnderTest] refers to a valid object [{1}] but the {1} is not in a folder.", tlsut.Name, tlsutRefered.Kind)
                        };

                        results.Add(feedback);
                    }
                }
            }
            else if (tlsutCount > 1)
            {
                foreach (var tlsut in this.testBench.Children.TopLevelSystemUnderTestCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = tlsut.Impl,
                        Message = "Only one TopLevelSystemUnderTest object allowed."
                    };

                    results.Add(feedback);
                }
            }
            else
            {
                throw new NotImplementedException();
            }


            return results;
        }

        /// <summary>
        /// TODO: description what we check...
        /// <para><remarks>Execute within transaction.</remarks></para>
        /// </summary>
        /// <returns>Checker results.</returns>
        protected IEnumerable<ContextCheckerResult> ValidateTestInjectionPoints()
        {
            List<ContextCheckerResult> result = new List<ContextCheckerResult>();
            foreach (var tip in this.testBench.Children.TestInjectionPointCollection)
            {
                result.AddRange(this.ValidateTestInjectionPoint(tip.Impl as GME.MGA.MgaReference));
            }
            return result;
        }

        /// <summary>
        /// TODO: description what we check...
        /// <para><remarks>Execute within transaction.</remarks></para>
        /// </summary>
        /// <returns>Checker results.</returns>
        protected IEnumerable<ContextCheckerResult> NoTestInjectionPoints()
        {
            List<ContextCheckerResult> results = new List<ContextCheckerResult>();

            if (this.testBench.Children.TestInjectionPointCollection.Any())
            {
                foreach (var tip in this.testBench.Children.TestInjectionPointCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = tip.Impl,
                        Message = "Test injection points are not allowed in this type of test bench."
                    };

                    results.Add(feedback);
                }
            }
            else
            {
                var feedback = new ContextCheckerResult()
                {
                    Success = true,
                    Subject = this.testBench.Impl,
                    Message = "No test injection points."
                };

                results.Add(feedback);
            }

            return results;
        }

        protected IEnumerable<ContextCheckerResult> NoComponentRefOrComponents()
        {
            List<ContextCheckerResult> results = new List<ContextCheckerResult>();

            if (this.testBench.Children.ComponentRefCollection.Any())
            {
                foreach (var tip in this.testBench.Children.ComponentRefCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = tip.Impl,
                        Message = string.Format("Component references are not allowed in test bench.", this.testBench.Kind)
                    };

                    results.Add(feedback);
                }
            }
            else
            {
                var feedback = new ContextCheckerResult()
                {
                    Success = true,
                    Subject = this.testBench.Impl,
                    Message = string.Format("No component references in {0}.", this.testBench.Kind)
                };

                results.Add(feedback);
            }

            if (this.testBench.Children.ComponentCollection.Any())
            {
                foreach (var tip in this.testBench.Children.ComponentCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = tip.Impl,
                        Message = string.Format("Components are not allowed in test bench.", this.testBench.Kind)
                    };

                    results.Add(feedback);
                }
            }
            else
            {
                var feedback = new ContextCheckerResult()
                {
                    Success = true,
                    Subject = this.testBench.Impl,
                    Message = string.Format("No component in {0}.", this.testBench.Kind)
                };

                results.Add(feedback);
            }

            return results;
        }

        /// <summary>
        /// TODO: description what we check...
        /// <para><remarks>Execute within transaction.</remarks></para>
        /// </summary>
        /// <returns>Checker results.</returns>
        protected IEnumerable<ContextCheckerResult> AtLeastOneTestInjectionPoints()
        {
            List<ContextCheckerResult> results = new List<ContextCheckerResult>();

            if (this.testBench.Children.TestInjectionPointCollection.Any())
            {
                foreach (var tip in this.testBench.Children.TestInjectionPointCollection)
                {
                    var feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = tip.Impl,
                        Message = "Test bench has test injection point."
                    };

                    results.Add(feedback);

                    results.AddRange(this.ValidateTestInjectionPoint(tip.Impl as GME.MGA.MgaReference));
                }
            }
            else
            {
                var feedback = new ContextCheckerResult()
                {
                    Success = false,
                    Subject = this.testBench.Impl,
                    Message = "No test injection points. Test bench must have at least one test injection point."
                };

                results.Add(feedback);
            }

            return results;
        }

        /// <summary>
        /// TODO: description what we check...
        /// <para><remarks>Execute within transaction.</remarks></para>
        /// </summary>
        /// <returns>Checker results.</returns>
        protected IEnumerable<ContextCheckerResult> ValidateTestInjectionPoint(GME.MGA.IMgaReference tip)
        {
            List<ContextCheckerResult> results = new List<ContextCheckerResult>();

            CyPhy.TopLevelSystemUnderTest tlsut = null;
            if (this.testBench.Children.TopLevelSystemUnderTestCollection.Count() == 1)
            {
                // if there is only one tlsut we can check safely the Test injection point references, otherwise do not.
                tlsut = this.testBench.Children.TopLevelSystemUnderTestCollection.FirstOrDefault();
            }
            else
            {
                return results;
            }

            var tipRefersTo = tip.Referred;
            if (tipRefersTo == null)
            {
                var feedback = new ContextCheckerResult()
                {
                    Success = false,
                    Subject = tip,
                    Message = string.Format("{0} [{1}] object is null reference.", tip.Name, tip.Meta.Name)
                };

                results.Add(feedback);
            }
            else
            {
                var feedback = new ContextCheckerResult()
                {
                    Success = true,
                    Subject = tip,
                    Message = string.Format("{0} [{1}] object is not null reference.", tip.Name, tip.Meta.Name)
                };

                results.Add(feedback);


                // descendant of top level system under test

                var referred = (tlsut.Impl as GME.MGA.IMgaReference).Referred;
                if (referred != null &&
                    referred is GME.MGA.MgaModel)
                {
                    var visitedCAs = new HashSet<string>() { referred.ID };
                    var referenceFilter = referred.Project.CreateFilter();
                    referenceFilter.Kind = typeof(CyPhy.ComponentRef).Name;

                    var matchingTarget = this.LookForObjectIDInReferencedCAsRecursively(
                        referenceFilter,
                        referred.Project.CreateFilter(),
                        visitedCAs,
                        referred as GME.MGA.IMgaModel,
                        tip.Referred.ID);

                    if (matchingTarget == false)
                    {
                        // tip does not point to a descendant of the tlsut

                        feedback = new ContextCheckerResult()
                        {
                            Success = false,
                            Subject = tip,
                            Message = string.Format("{0} [{1}] does not point to a descendant of the TopLevelSystemUnderTest model.", tip.Name, tip.Meta.Name)
                        };

                        results.Add(feedback);
                    }
                    else
                    {
                        feedback = new ContextCheckerResult()
                        {
                            Success = true,
                            Subject = tip,
                            Message = string.Format("{0} [{1}] points to a descendant of the TopLevelSystemUnderTest model.", tip.Name, tip.Meta.Name)
                        };

                        results.Add(feedback);
                    }
                }

                if (tipRefersTo.Meta.Name == typeof(CyPhy.DesignContainer).Name)
                {
                    // can point to design container
                    var designContainer = CyPhyClasses.DesignContainer.Cast(tipRefersTo);

                    feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = tip,
                        Message = string.Format("{0} [{1}] can point to {2}.",
                            tip.Name,
                            tip.Meta.Name,
                            designContainer.Attributes.ContainerType)
                    };

                    results.Add(feedback);
                }
                else if (tipRefersTo.Meta.Name == typeof(CyPhy.ComponentAssembly).Name)
                {
                    // can point to component assembly
                    feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = tip,
                        Message = string.Format("{0} [{1}] point can point to Component Assembly.", tip.Name, tip.Meta.Name)
                    };

                    results.Add(feedback);
                }
                else if (tipRefersTo.Meta.Name == typeof(CyPhy.Component).Name)
                {
                    // cannot point to component
                    feedback = new ContextCheckerResult()
                    {
                        Success = false,
                        Subject = tip,
                        Message = string.Format("{0} [{1}] point cannot point to Component.", tip.Name, tip.Meta.Name)
                    };

                    results.Add(feedback);
                }
                else if (tipRefersTo.Meta.Name == typeof(CyPhy.ComponentRef).Name)
                {
                    var componentRef = CyPhyClasses.ComponentRef.Cast(tipRefersTo);

                    // can point to component ref
                    feedback = new ContextCheckerResult()
                    {
                        Success = true,
                        Subject = tip,
                        Message = string.Format("{0} [{1}] can point to Component reference.", tip.Name, tip.Meta.Name)
                    };

                    results.Add(feedback);

                    if ((componentRef.Impl as GME.MGA.IMgaReference).Referred == null)
                    {
                        // component ref cannot be null
                        feedback = new ContextCheckerResult()
                        {
                            Success = false,
                            Subject = tip,
                            Message = string.Format("{0} [{1}] can point to Component reference, but the Component reference cannot be null.", tip.Name, tip.Meta.Name)
                        };

                        results.Add(feedback);
                    }
                    else
                    {
                        if (componentRef.AllReferred is CyPhy.TestComponent)
                        {
                            // component ref cannot be test component
                            feedback = new ContextCheckerResult()
                            {
                                Success = false,
                                Subject = tip,
                                Message = string.Format("{0} [{1}] can point to Component reference, but the Component reference cannot be a Test Component.", tip.Name, tip.Meta.Name)
                            };

                            results.Add(feedback);
                        }
                        else if (componentRef.AllReferred is CyPhy.Component)
                        {
                            // component ref can be component
                            feedback = new ContextCheckerResult()
                            {
                                Success = true,
                                Subject = tip,
                                Message = string.Format("{0} [{1}] can point to Component reference, which points to a Component.", tip.Name, tip.Meta.Name)
                            };

                            results.Add(feedback);
                        }
                        else if (componentRef.AllReferred is CyPhy.ComponentAssembly)
                        {
                            // component ref can be component assembly
                            feedback = new ContextCheckerResult()
                            {
                                Success = true,
                                Subject = tip,
                                Message = string.Format("{0} [{1}] can point to Component reference, which points to a Component Assembly.", tip.Name, tip.Meta.Name)
                            };

                            results.Add(feedback);
                        }
                        else
                        {
                            // this cannot happen, but if CyPhyML changes it may happen.
                            throw new NotImplementedException();
                        }
                    }
                }
            }

            return results;
        }

        /// <summary>
        /// Elaborates all referenced ComponentsRefs pointing to Component Assemblies and looks for all descendent FCOs within these for a match with ID.
        /// </summary>
        /// <param name="referenceFilter">Filter looking for ComponentRefs passed along.</param>
        /// <param name="filter">Empty filter passed along.</param>
        /// <param name="visitedCAs">Keeps track of visited Component Assemblies.</param>
        /// <param name="parent">MgaModel to start from.</param>
        /// <param name="id">The ID string of the Object to look for.</param>
        /// <returns>True if the ID could be matched with any of the objects.</returns>
        private bool LookForObjectIDInReferencedCAsRecursively(
            GME.MGA.MgaFilter referenceFilter,
            GME.MGA.MgaFilter filter,
            HashSet<string> visitedCAs,
            GME.MGA.IMgaModel parent,
            string id)
        {
            if (parent.GetDescendantFCOs(filter).Cast<GME.MGA.MgaFCO>().Any(x => x.ID == id))
            {
                return true;
            }

            foreach (GME.MGA.MgaReference reference in parent.GetDescendantFCOs(referenceFilter))
            {
                var potentialCA = reference.Referred;
                if (potentialCA != null && potentialCA.MetaBase.Name == typeof(CyPhy.ComponentAssembly).Name)
                {
                    if (visitedCAs.Contains(potentialCA.ID))
                    {
                        continue;
                    }
                    else
                    {
                        visitedCAs.Add(potentialCA.ID);
                        if (this.LookForObjectIDInReferencedCAsRecursively(referenceFilter, filter, visitedCAs, potentialCA as GME.MGA.IMgaModel, id))
                        {
                            return true;
                        }
                    }
                }
            }

            return false;
        }
    }
}
