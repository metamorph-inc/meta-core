using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using META;
using Newtonsoft.Json;


namespace CyPhyMasterInterpreter
{
    public class TestBenchTypeProcessor : AnalysisModelProcessor
    {
        public static HashSet<string> SupportedTestBenchTypes = new HashSet<string>()
        {
            typeof(CyPhy.TestBenchType).Name,
            typeof(CyPhy.TestBench).Name,
            typeof(CyPhy.BallisticTestBench).Name,
            typeof(CyPhy.BlastTestBench).Name,
            typeof(CyPhy.CADTestBench).Name,
            typeof(CyPhy.KinematicTestBench).Name,
            typeof(CyPhy.CarTestBench).Name,
            typeof(CyPhy.CFDTestBench).Name,
        };

        public CyPhy.TestBenchType testBenchType { get; set; }
        public CyPhy.TestBenchType expandedTestBenchType { get; set; }

        public TestBenchTypeProcessor(CyPhy.TestBenchType testBenchType)
        {
            this.testBenchType = testBenchType;
            this.OriginalSystemUnderTest = testBenchType.Children.TopLevelSystemUnderTestCollection.FirstOrDefault();

            // HACK: used by PET, SoT and CyPhy2Modelica_v2 [old code]
            this.GetInvokedObject().RegistryValue["TestBenchUniqueName"] = this.testBenchType.Name;
        }

        public override MgaModel GetInvokedObject()
        {
            return this.testBenchType.Impl as MgaModel;
        }

        public override MgaModel GetExpandedObject()
        {
            return this.expandedTestBenchType.Impl as MgaModel;
        }

        public override void Expand(CyPhy.ComponentAssembly configuration)
        {
            this.Configuration = configuration;

            if (this.OriginalSystemUnderTest.Referred.DesignEntity.ID == configuration.ID)
            {
                this.expandedTestBenchType = this.testBenchType;
            }
            else
            {
                // create temp folder for test bench
                CyPhy.Testing testing = CyPhyClasses.Testing.Cast(this.testBenchType.ParentContainer.Impl);

                var tempFolderName = AnalysisModelProcessor.GetTemporaryFolderName(this.testBenchType.Impl);

                CyPhy.Testing tempFolder = testing.Children.TestingCollection.FirstOrDefault(x => x.Name == tempFolderName);
                if (tempFolder == null)
                {
                    tempFolder = CyPhyClasses.Testing.Create(testing);
                    tempFolder.Name = tempFolderName;

                    this.AddToTraceabilityAndTemporary(tempFolder.Impl, testing.Impl, recursive: false);
                }

                // copy test bench
                var tempCopy = (tempFolder.Impl as MgaFolder).CopyFCODisp(this.testBenchType.Impl as MgaFCO);
                // fix name
                tempCopy.Name = AnalysisModelProcessor.GetTemporaryObjectName(this.testBenchType.Impl, configuration.Impl);

                this.AddToTraceabilityAndTemporary(tempCopy, this.testBenchType.Impl);

                // set expanded property to the expanded element
                this.expandedTestBenchType = CyPhyClasses.TestBenchType.Cast(tempCopy);

                var tlsut = this.expandedTestBenchType.Children.TopLevelSystemUnderTestCollection.FirstOrDefault();

                // switch references
                try
                {
                    // redirect SUT
                    var switcher = new ReferenceSwitcher.ReferenceSwitcherInterpreter();
                    switcher.SwitchReference(configuration.Impl as MgaFCO, tlsut.Impl as IMgaReference);
                }
                catch (Exception ex)
                {
                    // handle failures for this (use case we can lose ports/connections/
                    // what if something is an instance/subtype/readonly etc...
                    throw new AnalysisModelExpandFailedException("ReferenceSwitcher failed.", ex);
                }

                // redirect TIPs
                this.SwitchAllTipReferences();
            }
        }

        private void SwitchAllTipReferences()
        {
            HashSet<string> tipKinds = new HashSet<string>()
            {
               typeof(CyPhy.TestInjectionPoint).Name,
               typeof(CyPhy.BallisticTarget).Name,
               typeof(CyPhy.CriticalComponent).Name
            };

            List<IMgaReference> tips = (this.expandedTestBenchType.Impl as MgaModel)
                .ChildFCOs
                .OfType<IMgaReference>()
                .Where(x => tipKinds.Contains(x.MetaBase.Name))
                .ToList();

            foreach (var tip in tips)
            {
                // find new destination
                IMgaFCO newTipTarget = FindTestInjectionTarget(
                    this.testBenchType.Children.TopLevelSystemUnderTestCollection.FirstOrDefault(),
                    tip,
                    this.expandedTestBenchType.Children.TopLevelSystemUnderTestCollection.FirstOrDefault());

                if (newTipTarget == null)
                {
                    throw new AnalysisModelTipNotFoundException(string.Format("Referenced test injection entity {0} [{1}] was not found in the generated component assembly (design). {2}", tip.Name, tip.Meta.Name, this.Configuration.Path));
                }

                try
                {
                    var numObjectsBefore = (this.expandedTestBenchType.Impl as MgaModel).ChildObjects.Count;

                    var switcher = new ReferenceSwitcher.ReferenceSwitcherInterpreter();
                    switcher.SwitchReference(newTipTarget, tip);

                    var numObjectsAfter = (this.expandedTestBenchType.Impl as MgaModel).ChildObjects.Count;
                    if (numObjectsBefore != numObjectsAfter)
                    {
                        var message = string.Format("Some connections were lost for test injection point. Most likely becasue in the generated design the target object does not have the same interface (that are connected in the test bench) compare interfaces {0} [{1}] -> {2}", tip.Name, tip.Meta.Name, newTipTarget.AbsPath);

                        throw new AnalysisModelExpandFailedException(message);
                    }
                }
                catch (Exception ex)
                {
                    throw new AnalysisModelExpandFailedException("ReferenceSwitcher failed.", ex);
                }
            }

        }


        public static IMgaFCO FindTestInjectionTarget(
            CyPhy.TopLevelSystemUnderTest originalSut,
            IMgaReference originalTip,
            CyPhy.TopLevelSystemUnderTest targetSut)
        {
            if (originalSut == null)
            {
                throw new ArgumentNullException("originalSut");
            }

            if (originalTip == null)
            {
                throw new ArgumentNullException("originalTip");
            }

            if (targetSut == null)
            {
                throw new ArgumentNullException("targetSut");
            }

            // target that the reference has to be redirected
            IMgaFCO targetTipByBuilt = null;
            IMgaFCO targetTipByAbsPath = null;

            // get tip reference

            // relative path
            var relativePath = string.Empty;

            // ASSUME original SUT is a design container
            if (originalSut.Referred.DesignContainer == null &&
                targetSut.Referred.ComponentAssembly == null)
            {
                throw new NotImplementedException();
            }

            var sutDesignContainer = originalSut.Referred.DesignContainer;
            var sutComponentAssemblyTarget = targetSut.Referred.ComponentAssembly;

            // TOOD: Double check this cast
            CyPhy.DesignEntity originalDesignEntity = null;

            if (originalTip.Referred.Meta.Name == typeof(CyPhy.DesignContainer).Name)
            {
                originalDesignEntity = CyPhyClasses.DesignContainer.Cast(originalTip.Referred);
            }
            else if (originalTip.Referred.Meta.Name == typeof(CyPhy.Component).Name)
            {
                originalDesignEntity = CyPhyClasses.Component.Cast(originalTip.Referred);
            }
            else if (originalTip.Referred.Meta.Name == typeof(CyPhy.ComponentRef).Name)
            {
                originalDesignEntity = CyPhyClasses.ComponentRef.Cast(originalTip.Referred);
            }
            else if (originalTip.Referred.Meta.Name == typeof(CyPhy.ComponentAssembly).Name)
            {
                originalDesignEntity = CyPhyClasses.ComponentAssembly.Cast(originalTip.Referred);
            }


            List<CyPhy.DesignEntity> possibleDesignEntities = new List<CyPhy.DesignEntity>();

            possibleDesignEntities.Add(originalDesignEntity);

            // TODO: abspath for component assemblies / Component assembly refs too. Call function recursively
            if (originalTip.Referred.Meta.Name == typeof(CyPhy.DesignContainer).Name)
            {
                var designContainer = CyPhyClasses.DesignContainer.Cast(originalTip.Referred);
                if (designContainer.Attributes.ContainerType ==
                    CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum.Compound)
                {
                    // there could be a referece to the compound container

                    // BUG: no tracking of compound containers attempt to get it by AbsPath
                    // this implementation will work as long as
                    //    names are unqiue to Compound containes
                    //    structure as follows
                    //
                    // == PASS ==
                    // | Compound
                    // +-- Compound
                    //   |
                    //   +-- Compound
                    //
                    // == FAIL 1 == Have no idea how to get the path correctly
                    // | Compound
                    // +-- Alternative
                    //   |
                    //   +-- Compound
                    //
                    // == FAIL 2 == Have no idea how to get the path correctly
                    // | Compound
                    // +-- Optional
                    //   |
                    //   +-- Compound

                    // TODO: would be nice to use a regexp or something like that
                    Func<string, string> shortenAbsPath = new Func<string, string>((absPath) =>
                    {
                        // Example:
                        // input:  /@NewTesting|kind=Testing|relpos=0/@FEA_CompoundTIP_DS|kind=CADTestBench|relpos=0/@DesignContainer|kind=TopLevelSystemUnderTest|relpos=0
                        // output: /@NewTesting/@FEA_CompoundTIP_DS/@DesignContainer
                        return string.Join("/", absPath.Split('/').Select(x => string.Join("", x.TakeWhile(y => y != '|'))));
                    });

                    var sutPath = shortenAbsPath(originalSut.AllReferred.Impl.AbsPath);
                    var originalTipPath = shortenAbsPath(originalTip.Referred.AbsPath);
                    relativePath = originalTipPath.Substring(sutPath.Length);

                    // TODO: meta base name [kind] can be different if there are no unique path to the object.
                    targetTipByAbsPath = (targetSut.AllReferred.Impl as MgaModel).get_ObjectByPath(relativePath) as MgaFCO;
                }
                else
                {
                    // while optional and alternative containers are not tracked, get all ComponentAssembly, Component, ComponentRef recursively.
                    possibleDesignEntities.AddRange(ExpandInnerOptionalAndAlternativesRecursively(designContainer));
                }
            }

            Exception ex = null;

            foreach (var designEntity in possibleDesignEntities)
            {
                // Look for a DesignEntityRef, within a CWC, that points to this element in the Design Space.
                foreach (CyPhy.DesignEntityRef designEntityRef in designEntity.ReferencedBy.DesignEntityRef)
                {
                    var cwc = CyPhyClasses.CWC.Cast(designEntityRef.ParentContainer.Impl);

                    bool hasMatchingCARoot = false;

                    var caRefs = cwc.DstConnections.Config2CACollection.Select(x => x.DstEnds.ComponentAssemblyRef);
                    foreach (var caRef in caRefs)
                    {
                        if (caRef.AllReferred != null)
                        {
                            if ((caRef.AllReferred.Impl as MgaFCO).RootFCO.ID == (sutComponentAssemblyTarget.Impl as MgaFCO).RootFCO.ID)
                            {
                                hasMatchingCARoot = true;
                                break;
                            }
                        }
                    }

                    if (hasMatchingCARoot == false)
                    {
                        // do not log any messages since they irrelevant.
                        continue;
                    }

                    // TODO: log if design is dirty.


                    // get design entity ref where the refered object is the test injection point reference
                    //
                    // Multiple configuration (1, 2, 3, ...) can have the same design entity refs
                    //
                    //      design entity ref ----> DesignSpace element
                    //                                       ^
                    //                                       |
                    //      original test injection ---------

                    // verify root fco do match
                    if ((designEntityRef.Impl as MgaFCO).RootFCO != (sutDesignContainer.Impl as MgaFCO).RootFCO)
                    {
                        // in case user accidentally moves these object in the tree
                        string message = string.Format("Model does not contain traceablity information between the generated configurations and the design space. {0} will not be found in the original design space. Please try to export the configurations again using CAExporter or run the design space exploration tool.", originalTip.Meta.Name);

                        ex = new AnalysisModelTipNotFoundException(message);
                        continue;
                    }

                    var builtConnections = designEntityRef.DstConnections.BuiltCollection.Count();

                    if (builtConnections == 0)
                    {
                        // no built connection -> not good
                        string message = string.Format("No 'built' connection. Model does not contain traceablity information between the generated configurations and the design space. {0} will not be found in the original design space. Please try to export the configurations again using CAExporter or run the design space exploration tool.", originalTip.Meta.Name);

                        ex = new AnalysisModelTipNotFoundException(message);
                        continue;
                    }

                    if (builtConnections > 1)
                    {
                        // more than one built connection -> not good
                        string message = string.Format("There are more than one 'built' connections traceability is ambiguous. Model has to be fixed first. {0} will not be found in the original design space. Please try to export the configurations again using CAExporter or run the design space exploration tool.", originalTip.Meta.Name);

                        ex = new AnalysisModelTipNotFoundException(message);
                        continue;
                    }

                    // Get the Built connection and then the BuildDesignEntityRef.
                    // The BuildDesignEntityRef will point to our source object within a Component Assembly / configuration.
                    foreach (CyPhy.Built built in designEntityRef.DstConnections.BuiltCollection)
                    {
                        var builtDesignEntityRef = built.DstEnds.BuiltDesignEntityRef;
                        if (builtDesignEntityRef.AllReferred == null)
                        {
                            // in case user clears the reference

                            string message = string.Format("Model does not contain traceablity information between the generated configurations and the design space. {0} will not be found in the original design space. Please try to export the configurations again using CAExporter or run the design space exploration tool.", originalTip.Meta.Name);

                            ex = new AnalysisModelTipNotFoundException(message);
                            continue;
                        }

                        if ((builtDesignEntityRef.AllReferred.Impl as MgaFCO).RootFCO.ID == (sutComponentAssemblyTarget.Impl as MgaFCO).RootFCO.ID)
                        {
                            // verify that the component is inside the target design
                            // TODO: how this would work if it points to a component in a CA which is referred in the design???
                            targetTipByBuilt = builtDesignEntityRef.AllReferred.Impl as MgaFCO;
                        }
                        
                    }
                }
            }
            

            if (targetTipByBuilt != null)
            {
                // prefer by built
                return targetTipByBuilt;
            }
            else if (targetTipByAbsPath != null)
            {
                // fallback to by abs path
                return targetTipByAbsPath;
            }
            else if (ex != null)
            {
                // not found
                throw ex;
            }
            else
            {
                return null;
            }
        }

        private static IEnumerable<CyPhy.DesignEntity> ExpandInnerOptionalAndAlternativesRecursively(CyPhy.DesignEntity designEntity)
        {
            // FIXME: Recursive function, when this will terminate?
            List<CyPhy.DesignEntity> possibleTargets = new List<CyPhy.DesignEntity>();
            if (designEntity is CyPhy.DesignContainer)
            {
                foreach (var childDesignContainer in (designEntity as CyPhy.DesignContainer).Children.DesignContainerCollection)
                {
                    if (childDesignContainer.Attributes.ContainerType == CyPhyClasses.DesignContainer.AttributesClass.ContainerType_enum.Compound)
                    {
                        possibleTargets.Add(childDesignContainer);
                    }
                    else
                    {
                        possibleTargets.AddRange(ExpandInnerOptionalAndAlternativesRecursively(childDesignContainer));
                    }
                }

                foreach (var item in (designEntity as CyPhy.DesignContainer).Children.ComponentAssemblyCollection)
                {
                    possibleTargets.Add(item);
                }

                foreach (var item in (designEntity as CyPhy.DesignContainer).Children.ComponentRefCollection)
                {
                    possibleTargets.Add(item);
                }

                foreach (var item in (designEntity as CyPhy.DesignContainer).Children.ComponentCollection)
                {
                    possibleTargets.Add(item);
                }
            }

            return possibleTargets;
        }

        public override Queue<ComComponent> GetWorkflow()
        {
            // TODO: implement this method appropriately
            Queue<ComComponent> workflow = new Queue<ComComponent>();

            this.ExecuteInTransaction(() =>
            {
                var workflowRef = this.testBenchType
                    .Children
                    .WorkflowRefCollection
                    .ToList();

                if (workflowRef.Count == 0)
                {
                    if (this.testBenchType.Kind == typeof(CyPhy.CADTestBench).Name)
                    {
                        // use CyPhy2CAD by default
                        workflow.Enqueue(new ComComponent("MGA.Interpreter.CyPhy2CAD_CSharp", true));
                    }
                    else if (this.testBenchType.Kind == typeof(CyPhy.KinematicTestBench).Name)
                    {
                        // use CyPhy2CAD_CSharp by default
                        workflow.Enqueue(new ComComponent("MGA.Interpreter.CyPhy2CAD_CSharp", true));
                    }
                    else if (this.testBenchType.Kind == typeof(CyPhy.BlastTestBench).Name)
                    {
                        // use CyPhy2CAD_CSharp by default
                        workflow.Enqueue(new ComComponent("MGA.Interpreter.CyPhy2CAD_CSharp", true));
                    }
                    else if (this.testBenchType.Kind == typeof(CyPhy.BallisticTestBench).Name)
                    {
                        // use CyPhy2CAD_CSharp by default
                        workflow.Enqueue(new ComComponent("MGA.Interpreter.CyPhy2CAD_CSharp", true));
                    }
                    else if (this.testBenchType.Kind == typeof(CyPhy.CFDTestBench).Name)
                    {
                        // use CyPhy2CAD_CSharp by default
                        workflow.Enqueue(new ComComponent("MGA.Interpreter.CyPhy2CAD_CSharp", true));
                    }
                    else
                    {
                        // use NOTHING by default see JIRA
                    }
                }
                else if (workflowRef.Count == 1)
                {
                    workflow.Clear();

                    Queue<string> items = new Queue<string>();
                    var tasks = new List<CyPhy.Task>();
                    var processed = new List<CyPhy.TaskBase>();

                    if (workflowRef[0].Referred.Workflow != null)
                    {
                        //tasks.AddRange(workflowRef[0].Referred.Workflow.Children.TaskCollection);

                        //CyPhy.Task startTask = null;

                        //startTask = tasks
                        //    .Where(x => x.AllSrcConnections.Count() == 0)
                        //    .FirstOrDefault();

                        // TODO: signal a warning/error if there are multiple sources

                        CyPhy.Task nextTask = this.GetInitialTask(workflowRef[0].Referred.Workflow);

                        // avoid loops
                        while (nextTask != null &&
                                processed.Contains(nextTask) == false)
                        {
                            processed.Add(nextTask);
                            ComComponent component = new ComComponent(nextTask.Attributes.COMName);
                            if (component.isValid == false)
                            {
                                throw new ApplicationException("Could not create " + nextTask.Attributes.COMName);
                            }
                            string parameters = nextTask.Attributes.Parameters;

                            try
                            {
                                component.WorkflowParameters = (Dictionary<string, string>)JsonConvert.DeserializeObject(parameters, typeof(Dictionary<string, string>));
                                if (component.WorkflowParameters == null)
                                {
                                    component.WorkflowParameters = new Dictionary<string, string>();
                                }
                            }
                            catch (JsonException ex)
                            {
                                throw new ApplicationException(String.Format("Could not parse Parameters for '{0}'", nextTask.Name), ex);
                            }
                            workflow.Enqueue(component);                            

                            var flow = nextTask.DstConnections.FlowCollection.FirstOrDefault();
                            if (flow == null)
                            {
                                nextTask = null;
                            }
                            else // skip all execution tasks
                            {
                                var taskBase = flow.DstEnds.TaskBase;
                                while (taskBase != null &&
                                        processed.Contains(taskBase) == false)
                                {
                                    if (taskBase.Impl.MetaBase.Name == (typeof(CyPhy.Task).Name))
                                    {
                                        nextTask = CyPhyClasses.Task.Cast(taskBase.Impl);
                                        break;
                                    }

                                    processed.Add(taskBase);
                                    flow = taskBase.DstConnections.FlowCollection.FirstOrDefault();
                                    if (flow == null)
                                    {
                                        taskBase = null;
                                        nextTask = null;
                                        break;
                                    }
                                    else
                                    {
                                        taskBase = flow.DstEnds.TaskBase;
                                    }
                                }
                            }
                        }
                    }
                }
                else if (workflowRef.Count > 1)
                {
                    // not supported
                    throw new NotImplementedException("WorkflowRef is greater than 1.");
                }
            });
            return workflow;
        }

        private CyPhy.Task GetInitialTask(CyPhy.Workflow workflow)
        {
            if (workflow != null)
            {
                var allTasks = new List<CyPhy.TaskBase>();
                allTasks.AddRange(workflow.Children.TaskBaseCollection);

                var sourceTasks = allTasks
                    .Where(x => x.AllSrcConnections.Count() == 0);

                if (sourceTasks.Count() != 1)
                {
                    // Log warning or error
                    return null;
                }

                var startTask = sourceTasks.FirstOrDefault();
                var processed = new List<CyPhy.TaskBase>();

                while (startTask != null &&
                        startTask.Impl.MetaBase.Name != (typeof(CyPhy.Task).Name) &&
                        processed.Contains(startTask) == false)
                {
                    processed.Add(startTask);

                    var flow = startTask.DstConnections.FlowCollection.FirstOrDefault();
                    if (flow == null)
                    {
                        startTask = null;
                    }
                    else
                    {
                        startTask = flow.DstEnds.TaskBase;
                    }
                }

                if (startTask != null &&
                    startTask.Impl.MetaBase.Name == (typeof(CyPhy.Task).Name))
                {
                    return CyPhyClasses.Task.Cast(startTask.Impl);
                }
            }

            return null;
        }

        public override bool PostToJobManager(JobManagerDispatch manager = null)
        {
            if (this.Interpreters == null)
            {
                throw new InvalidOperationException("Call RunInterpreters method first.");
            }

            if (manager == null)
            {
                manager = new JobManagerDispatch();
            }

            bool success = true;

            foreach (var interpreter in this.Interpreters)
            {
                // TODO: what if some of them failed ???
                if (interpreter.result.Success)
                {
                    string runCommand = interpreter.result.RunCommand;
                    string title = string.Empty;
                    string testbenchName = string.Empty;
                    string workingDirectory = interpreter.MainParameters.OutputDirectory;


                    string interpreterName = interpreter.Name.StartsWith("MGA.Interpreter.") ?
                        interpreter.Name.Substring("MGA.Interpreter.".Length) :
                        interpreter.Name;

                    title = String.Format("{0}_{1}", interpreterName, this.expandedTestBenchType.Name).Replace(" ", "_");
                    testbenchName = this.testBenchType.Name;

                    success = success && manager.EnqueueJob(runCommand, title, testbenchName, workingDirectory, interpreter);
                }
            }

            // TODO: should this be inside the Dispatch and triggred by a timer ???
            manager.AddJobs();


            if (this.Interpreters.Count == 0)
            {
                success = false;
            }

            return success;
        }

        public override bool SaveTestBenchManifest(AVM.DDP.MetaAvmProject project, string configurationName, DateTime analysisStartTime)
        {
            if (project == null)
            {
                throw new ArgumentNullException("project");
            }

            this.ThrowIfNotExpanded();

            this.EnsureOutputDirectory();

            bool success = false;
            try
            {
                success = project.SaveTestBenchManifest(
                this.Configuration.Name,
                configurationName,
                this.expandedTestBenchType,
                this.OutputDirectory,
                this.testBenchType,
                analysisStartTime);
            }
            catch (Exception ex)
            {
                throw new AnalysisModelProcessorException("Saving test bench manifest failed.", ex);
            }

            return success;
        }

        public override bool SaveTestBench(AVM.DDP.MetaAvmProject project)
        {
            if (project == null)
            {
                throw new ArgumentNullException("project");
            }

            this.ThrowIfNotExpanded();

            this.EnsureOutputDirectory();

            bool success = false;
            try
            {
                success = project.SaveTestBench(this.testBenchType);
            }
            catch (System.IO.IOException ex)
            {
                // ignore sharing violation:
                // if another process is writing the same testbench file, it will have the same contents
                int HResult = System.Runtime.InteropServices.Marshal.GetHRForException(ex);
                const int SharingViolation = 32;
                if ((HResult & 0xFFFF) == SharingViolation)
                {
                }
                else
                {
                    throw new AnalysisModelProcessorException("Saving test bench failed.", ex);
                }
            }
            catch (Exception ex)
            {
                throw new AnalysisModelProcessorException("Saving test bench failed.", ex);
            }

            return success;
        }

        public override bool UpdateTestBenchManifestExecutionSteps(AVM.DDP.MetaTBManifest manifest)
        {
            this.ExecuteInTransaction(() =>
            {
                    try
                    {
                        manifest.AddAllTasks(this.testBenchType, this.Interpreters);
                    }
                    catch (Exception ex)
                    {
                        throw new AnalysisModelProcessorException("Updating test bench execution steps failed.", ex);
                    }
            });

            return true;
        }
    }
}
