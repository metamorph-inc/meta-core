using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using System.Threading.Tasks;

namespace CyPhy2CAD_CSharp
{
    public class CADFlatDataCreator
    {
        //public List<Message> messages { get; set; }
        public int errorCnt { get; set; }
        public int warningCnt { get; set; }
        public string OutputDir { get; set; }
        public string ProjectDirectory { get; set; }


       // private DataRep.CADAssembly assembly;
        private Dictionary<string, DataRep.CADComponent> regularComponents;
        private Dictionary<string, DataRep.CADComponent> size2fitComponents;
        private List<DataRep.CADEdge> regularEdges;
        private List<DataRep.CADEdge> size2fitEdges;
        private List<DataRep.CADComponent> orphans;
        private CyPhyClasses.CADModel.AttributesClass.FileFormat_enum cadFormat = CyPhyClasses.CADModel.AttributesClass.FileFormat_enum.Creo;
        private bool MetaLink;

        private List<string> referenceCoordComponents;

        //  Book keeping
        private List<string> structuralInfProcessed;
        private Dictionary<string, string> GMEIDJointIDMap;

        public CADFlatDataCreator(string outputdir, string projectDirectory, CyPhyClasses.CADModel.AttributesClass.FileFormat_enum cadFormat, bool metalink = false)
        {
           // messages = new List<Message>();
            regularComponents = new Dictionary<string, DataRep.CADComponent>();
            size2fitComponents = new Dictionary<string, DataRep.CADComponent>();
            regularEdges = new List<DataRep.CADEdge>();
            size2fitEdges = new List<DataRep.CADEdge>();
            orphans = new List<DataRep.CADComponent>();
            referenceCoordComponents = new List<string>();
            OutputDir = outputdir;
            ProjectDirectory = projectDirectory;
            this.cadFormat = cadFormat;
            errorCnt = 0;
            warningCnt = 0;
            MetaLink = metalink;

            structuralInfProcessed = new List<string>();
            GMEIDJointIDMap = new Dictionary<string, string>();
        }


        public void CreateFlatData(CyPhy.ComponentAssembly cyphyasm)
        {
            List<CyPhy.Component> regular = new List<CyPhy.Component>();
            List<CyPhy.Component> size2fitcomponents = new List<CyPhy.Component>();

            // [1] Grabs all components
            FindComponents(cyphyasm, regular, size2fitcomponents);

            ProcessComponents(regular, size2fitcomponents, cyphyasm);
            ProcessReferenceCoordinateSystems(cyphyasm);
        }

        public void ProcessReferenceCoordinateSystems(CyPhy.ComponentAssembly cyphyasm)
        {
            if (cyphyasm != null)
            {
                // [4] Find ReferenceCoordinateComponents
                foreach (CyPhy.ReferenceCoordinateSystem refCoord in cyphyasm.Children.ReferenceCoordinateSystemCollection)
                {
                    ReferenceCoordinateSystemTraversal traverser = new ReferenceCoordinateSystemTraversal(refCoord);
                    referenceCoordComponents.AddRange(traverser.referenceCoordComponents);
                }

            }
        }

        private void ProcessComponents(List<CyPhy.Component> regular, List<CyPhy.Component> size2fitcomponents, CyPhy.ComponentAssembly cyphyasm)
        {
            if (regular.Count < 1)
            {
                if (!MetaLink)
                {
                    if (cyphyasm != null)
                        Logger.Instance.AddLogMessage("Assembly do not contain components with Creo CADModels [" + cyphyasm.Path + "]", Severity.Error);
                    else
                        Logger.Instance.AddLogMessage("CAD TestBench does not contain any valid TIPs!", Severity.Error);
                }
                return;
            }

            List<Task<string>> missingFiles = new List<Task<string>>();
            // [2] Create CADComponent, size2fit, and edges
            foreach (var item in regular)
            {
                DataRep.CADComponent component = new DataRep.CADComponent(item, ProjectDirectory: this.ProjectDirectory, Traceability: Traceability, cadFormat: cadFormat);
                this.regularComponents[item.ID] = component;
                missingFiles.Add(component.missingFile);
            }

            foreach (var item in size2fitcomponents)
            {
                DataRep.CADComponent component = new DataRep.CADComponent(item, ProjectDirectory: this.ProjectDirectory, size2fit: true, Traceability: Traceability, cadFormat: cadFormat);
                this.size2fitComponents[item.ID] = component;
                missingFiles.Add(component.missingFile);
            }

            foreach (var task in missingFiles.Where(t => t != null))
            {
                task.Wait();
                if (task.Result != null)
                {
                    Logger.Instance.AddLogMessage(task.Result, Severity.Error);
                }
            }

            foreach (var component in regularComponents)
            {
                foreach (var structintf in component.Value.StructuralInterfaceNodes)
                {
                    if (structintf.Value.Joint != null)
                    {
                        structintf.Value.Joint.FindTopLevelPort(cyphyasm);
                    }
                }
            }

            // [3] Create Edges
            foreach (KeyValuePair<string, DataRep.CADComponent> item in size2fitComponents)
            {
                CreateEdges(item.Value, cyphyasm, true);
            }

            foreach (KeyValuePair<string, DataRep.CADComponent> item in regularComponents)
            {
                CreateEdges(item.Value, cyphyasm);
            }

            // R. Owens 1/18/2017
            // The following code was moved to ProcessReferenceCoordinateSystems
            // This is because there are cases where ProcessReferenceCoordinateSystems should be called independent
            // of calling process components
            //if (cyphyasm != null)
            //{
            //    // [4] Find ReferenceCoordinateComponents
            //    foreach (CyPhy.ReferenceCoordinateSystem refCoord in cyphyasm.Children.ReferenceCoordinateSystemCollection)
            //    {
            //        ReferenceCoordinateSystemTraversal traverser = new ReferenceCoordinateSystemTraversal(refCoord);
            //        referenceCoordComponents.AddRange(traverser.referenceCoordComponents);
            //    }
            //}

            //if (referenceCoordComponents.Count == 0)
            //{
            //    Logger.Instance.AddLogMessage("No referencecoordinatesystem was found. Root component of assembly will be selected arbitrarily.", Severity.Warning);
            //}

            // [5] Print Assembly
            PrintAssembly();
        }

        public void FindComponents(CyPhy.ComponentAssembly cyphyasm,
                                   List<CyPhy.Component> regular,
                                   List<CyPhy.Component> size2fit)
        {

            CheckAssembly(cyphyasm);

            foreach (var item in cyphyasm.Children.ComponentAssemblyCollection)
            {
                FindComponents(item, regular, size2fit);
            }

            foreach (var item in cyphyasm.Children.ComponentCollection)
            {
                if (CheckComponent(item))
                {
                    // size2fit or regular
                    if (item.Children.SizedToFitCollection.Any())
                    {
                        size2fit.Add(item);
                    }
                    else
                    {
                        regular.Add(item);
                    }
                }
            }
        }


#region COMMON_FCN
        private void CheckAssembly(CyPhy.ComponentAssembly cyphyasm)
        {
            foreach (CyPhy.ComponentRef cref in cyphyasm.Children.ComponentRefCollection)
            {
                if (cref.AllReferred == null)
                {
                    string originalId = null;
                    if (Traceability != null && Traceability.TryGetMappedObject(cref.ID, out originalId))
                    {
                        throw new Exception("Null ComponentRef <a href=\"mga:" + originalId + "\">[" + cref.Path + "]</a>.");
                    }
                    throw new Exception("Null ComponentRef [" + cref.Path + "].");
                }
                else
                {
                    throw new Exception("Model not fully elaborated, contains ComponentRef [" + cref.Path + "]");
                }
            }
        }

        private bool CheckComponent(CyPhy.Component cyphycomp)
        {
            bool status = true;
            string comppath = cyphycomp.Path;

            CyPhy.CADModel cadmodel = cyphycomp.Children.CADModelCollection.FirstOrDefault(x => x.Attributes.FileFormat == cadFormat);
            if (cadmodel == null)
            {
                Logger.Instance.AddLogMessage("Component is missing CADModel object, it will be excluded from assembly xml: [" + comppath + "]", Severity.Normal);
                return false;
            }

            return status;
        }

        private void CreateEdges (DataRep.CADComponent component, CyPhy.ComponentAssembly topassembly, bool size2fit = false)
        {
            foreach (KeyValuePair<string, DataRep.StructuralInterfaceConstraint> item in component.StructuralInterfaceNodes)
            {
                // [1] find matching structural interface
                // [2] CreateEdge(a, b) 

                var found = structuralInfProcessed.Find(x => x == item.Key);
                if (found == null)
                {
                    CommonTraversal traverser = new CommonTraversal(item.Value.CyPhyImpl, topassembly);
                    if (traverser.FoundConnectedNodes.Count() > 1)
                    {
                        Logger.Instance.AddLogMessage("StructuralInterface connected to >1 StructuralInterface [" + item.Value.CyPhyImpl.Path + "]", Severity.Error);
                        continue;
                    }
                    else if (traverser.FoundConnectedNodes.Count() < 1)
                    {
                        if (traverser.GetVisitedConnectionCount() > 0)
                            Logger.Instance.AddLogMessage("StructuralInterface not connected to another StructuralInterface [" + item.Value.CyPhyImpl.Path + "]", Severity.Warning);
                        continue;
                    }

                    string dstparentid = traverser.FoundConnectedNodes.First().ParentContainer.ID;
                    string dstsiid = traverser.FoundConnectedNodes.First().ID;
                    if (!size2fitComponents.ContainsKey(dstparentid))
                    {
                        if (regularComponents.ContainsKey(dstparentid))
                        {
                            if (regularComponents[dstparentid].StructuralInterfaceNodes.ContainsKey(dstsiid))
                            {
                                // META-3124
                                if (item.Value.Joint != null && regularComponents[dstparentid].StructuralInterfaceNodes[dstsiid].Joint != null)
                                {
                                    if (item.Value.Joint.Name == regularComponents[dstparentid].StructuralInterfaceNodes[dstsiid].Joint.Name)
                                        CreateEdge(component,
                                                   item.Value,
                                                   regularComponents[dstparentid],
                                                   regularComponents[dstparentid].StructuralInterfaceNodes[dstsiid]);
                                    else
                                    {
                                        Logger.Instance.AddLogMessage("Mismatched joints in connected Connectors: " + component.CyPhyModelPath + item.Value.CyPhyImpl.Name + "(" + item.Value.Joint + "), " + regularComponents[dstparentid].CyPhyModelPath + regularComponents[dstparentid].StructuralInterfaceNodes[dstsiid].CyPhyImpl.Name + "(" + regularComponents[dstparentid].StructuralInterfaceNodes[dstsiid].Joint + ")", Severity.Error);
                                    }
                                }
                                else
                                {
                                    CreateEdge(component,
                                               item.Value,
                                               regularComponents[dstparentid],
                                               regularComponents[dstparentid].StructuralInterfaceNodes[dstsiid]);

                                }
                            }
                        }
                    }
                }
            }
        }

        private bool CreateEdge(DataRep.CADComponent aparent,
                                DataRep.StructuralInterfaceConstraint a,
                                DataRep.CADComponent bparent,
                                DataRep.StructuralInterfaceConstraint b,
                                bool size2fit = false)
        {
            structuralInfProcessed.Add(a.PortID);
            structuralInfProcessed.Add(b.PortID);

            // Create Edge
            DataRep.CADEdge edge = new DataRep.CADEdge(aparent, bparent)
            {
                SrcPortID = a.PortID,
                DstPortID = b.PortID,
                SrcPortName = a.Name,
                DstPortName = b.Name,
                DegreeFreedom = a.DegreeFreedom,
                SrcIntf = a,
                DstIntf = b
            };

            List<Tuple<DataRep.Datum, DataRep.Datum>> constraintPairs = new List<Tuple<DataRep.Datum,DataRep.Datum>>();
            bool result = DataRep.StructuralInterfaceConstraint.MatchStructuralInterfaceDatums(a, b, constraintPairs, Traceability);
            edge.ConstraintPairs.AddRange(constraintPairs);

            // TODO: Add more checking for this
            if (a.Joint != null && b.Joint != null)
            {
                var aJointType = a.Joint.JointType;
                var bJointType = b.Joint.JointType;
                if (aJointType != bJointType)
                {
                    Logger.Instance.AddLogMessage("Mismatched joint type in connected Connectors.", Severity.Error);
                }
                else
                {
                    edge.KinematicJointPair = new DataRep.KinematicJointPair(a.Joint, b.Joint);
                    GMEIDJointIDMap.Add(a.Joint.CyPhyID, edge.KinematicJointPair.XmlID);
                    GMEIDJointIDMap.Add(b.Joint.CyPhyID, edge.KinematicJointPair.XmlID);
                }
            }


            if (size2fit)
                size2fitEdges.Add(edge);
            else
                regularEdges.Add(edge);

            // TODO: !!!!!!!!!!!!!! NEED TO IMPLEMENT !!!!!!!!!!!!!!
            // Check for coordinatesystem datum should be the only 1

            return !result;
        }

 
#endregion

        public DataRep.CADContainer CreateCADDataContainer(string configID,
                                                           string assemblyName)
        {

            EdgeWalker edgewalker = new EdgeWalker(configID,
                                                   assemblyName,
                                                   OutputDir);

            DataRep.CADContainer container = edgewalker.WalkAssembly(regularComponents,
                                           regularEdges,
                                           size2fitComponents,
                                           size2fitEdges,
                                           orphans,
                                           referenceCoordComponents);
            container.GMEIDToJointIDMap = GMEIDJointIDMap;

            return container;
        }




        /***************************************************************************/
        /************************* Test Injection Points ***************************/
        /***************************************************************************/
        public void CreateFlatData(CyPhy.CADTestBench testBench,
                                   string catlsut)
        {
            List<CyPhy.Component> regular = new List<CyPhy.Component>();
            List<CyPhy.Component> size2fit = new List<CyPhy.Component>();

            bool asmprocessed = false;


            var toplevelSUT = testBench.Children.ComponentAssemblyCollection.FirstOrDefault();     // should be an instance b/c elaborate was called earlier
            // CyPhy2CAD_CSharp::ProcessCAD already validated that toplevelSUT != null.  Check again in case some removes the check in  CyPhy2CAD_CSharp::ProcessCAD
            if (toplevelSUT != null)
            {
                // We must cover the case where the TestBench (TB) only consists of TestInjectionPoints that are 
                // only Components and not ComponentAssemblies.  The following code in this function does not accomodate
                // that case.  Adding the ReferenceCoordinateSystems now will assure that if a ReferenceCoordinateSystem is set for 
                // a Component (i.e. part not assembly) that the ReferenceCoordinateSystem will be used and will force that 
                // component to be added first.
                // Also, note that the FindStartingComponent function ignores components with ReferenceCoordinateSystems if those components
                // are not actually referenced by the TB.  In other words, adding ReferenceCoordinateSystems to 
                // CyPhy2CAD_CSharp::referenceCoordComponents that are not actually referenced by the TB is not a problem.
                ProcessReferenceCoordinateSystems(toplevelSUT);
            }

            foreach (var tip in testBench.Children.TestInjectionPointCollection)
            {
                if (tip.Referred != null)
                {                  
                    string tipKind = tip.Referred.TIPRefBase.Kind;
                    Logger.Instance.AddLogMessage("Test Injection Point kind is: [" + tipKind + "]", Severity.Info);
                    if (tipKind == "Component")
                    {
                        CyPhy.Component component = tip.Referred.Component;
                        if (CheckComponent(component))
                        {
                            // size2fit or regular
                            if (component.Children.SizedToFitCollection.Any())
                                size2fit.Add(component);
                            else
                                regular.Add(component);
                        }
                    }
                    else if (tipKind == "ComponentAssembly")
                    {
                        List<CyPhy.Component> asmregular = new List<CyPhy.Component>();
                        List<CyPhy.Component> asmsize2fitcomponents = new List<CyPhy.Component>();
                        CyPhy.ComponentAssembly componentAsm = tip.Referred.ComponentAssembly;
                        FindComponents(componentAsm, 
                                       asmregular,
                                       asmsize2fitcomponents);
                        ProcessComponents(asmregular, asmsize2fitcomponents, componentAsm);
                        ProcessReferenceCoordinateSystems(componentAsm);
                        asmprocessed = true;
                    }
                    else
                    {
                        Logger.Instance.AddLogMessage("Not supported Test Injection Point kind [" + tipKind + "]", Severity.Error);
                    }
                }
                else
                {
                    throw new META.InterpreterException("Null Test Injection Point in FEA Test Bench!");
                }

            }
            /*
            if (regular.Count < 1)
            {
            }

            // [] Create CADComponent, size2fit, and edges
            foreach (var item in regular)
            {
                DataRep.CADComponent component = new DataRep.CADComponent(item, ProjectDirectory: this.ProjectDirectory);
                this.regularComponents[item.ID] = component;

            }

            foreach (var item in size2fit)
            {
                DataRep.CADComponent component = new DataRep.CADComponent(item, ProjectDirectory: this.ProjectDirectory, size2fit: true);
                this.size2fitComponents[item.ID] = component;
            }

            // [] Create Edges
            foreach (KeyValuePair<string, DataRep.CADComponent> item in size2fitComponents)
            {
                CreateEdges(item.Value, null, true); // snyako: what's the top assembly here?
            }

            foreach (KeyValuePair<string, DataRep.CADComponent> item in regularComponents)
            {
                CreateEdges(item.Value, null); // snyako: what's the top assembly here?
            }

            // [] Print Assembly
            PrintAssembly();*/

            if (!asmprocessed || (regular.Count!=0 && size2fit.Count!=0))
                ProcessComponents(regular, size2fit, null);

        }


        #region PRINT_SECTION
        private void PrintAssembly()
        {
            using (StreamWriter file = new StreamWriter(Path.Combine(OutputDir, "CyPhy2CAD_Flat_Representation.txt"), false))
            {
                file.WriteLine("<<<<<   Components  >>>>>");
                foreach (var item in regularComponents.Values)
                    file.WriteLine(item.ToString());

                file.WriteLine("<<<<<   Edges  >>>>>");
                foreach (var item in regularEdges)
                    file.WriteLine(item.ToString());

                file.WriteLine("<<<<<   Size2Fit Components  >>>>>");
                foreach (var item in size2fitComponents.Values)
                    file.WriteLine(item.ToString());

                file.WriteLine("<<<<<   Size2Fit Edges  >>>>>");
                foreach (var item in size2fitEdges)
                    file.WriteLine(item.ToString());
            }  
        }

        private void PrintAssembly_Graphviz(bool directed = false)
        {
            string filename = "CyPhy2CAD_Graphviz_Representation";

            if (directed)
                filename += "_Directed.gv";
            else
                filename += ".gv";

                using (StreamWriter file = new StreamWriter(Path.Combine(OutputDir, filename), false))
                {
                    foreach (var item in regularComponents.Values)
                        file.WriteLine(item.ToGraphViz());

                    foreach (var item in size2fitComponents.Values)
                        file.WriteLine(item.ToGraphViz());

                    foreach (var item in regularEdges)
                        file.WriteLine(item.ToGraphViz());

                    foreach (var item in size2fitEdges)
                        file.WriteLine(item.ToGraphViz());
                }
        }
#endregion



        public CyPhyCOMInterfaces.IMgaTraceability Traceability { get; set; }
    }   // end class
}       // end namespace
