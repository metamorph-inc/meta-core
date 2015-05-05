using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace CyPhy2CAD_CSharp
{
    class EdgeWalker
    {

        public string OutputDirectory { get; set; }

        private long newAssemblyIDSeed;
        private string assemblyID;
        private string assemblyName;
        private Dictionary<string, string> component2assemblyTable;

        private List<string> debugMessages;

        public EdgeWalker(string assemblyid,
                          string name,
                          string outputdir)
        {
            //assemblies = new Dictionary<string, DataRep.CADAssembly>();
            newAssemblyIDSeed = 1;
            assemblyID = assemblyid;
            assemblyName = name;
            component2assemblyTable = new Dictionary<string, string>();
            OutputDirectory = outputdir;
            debugMessages = new List<string>();
        }
        
        public DataRep.CADContainer WalkAssembly(Dictionary<string, DataRep.CADComponent> regularComponents,
                                 List<DataRep.CADEdge> regularEdges,
                                 Dictionary<string, DataRep.CADComponent> size2fitComponents,
                                 List<DataRep.CADEdge> size2fitEdges,
                                 List<DataRep.CADComponent> orphans,
                                 List<string> RefCoordIDs)
        {
            //Dictionary<string, DataRep.CADAssembly> tmpassemblies = new Dictionary<string, DataRep.CADAssembly>();
    
            DataRep.CADContainer assembliesContainer = new DataRep.CADContainer("jklmn");

            List<DataRep.CADEdge> openEdges = new List<DataRep.CADEdge>();
            openEdges = regularEdges;
            List<string> openComponents = regularComponents.Keys.ToList();
            Queue<string> openQ = new Queue<string>();

            assembliesContainer.orphans.AddRange(orphans);

            if (openComponents.Count() == 1 && !openEdges.Any())
            {
                string ID = assemblyID + "|" + newAssemblyIDSeed.ToString();
                DataRep.CADAssembly anassembly = new DataRep.CADAssembly(ID, assemblyName);               //DataRep.CADAssembly anassembly = new DataRep.CADAssembly(assemblyID, assemblyName);
                anassembly.ChildComponents.Add(regularComponents.First().Key, regularComponents.First().Value);
                anassembly.ChildComponents = regularComponents;
                anassembly.ChildEdges = regularEdges;
                ToDAG(anassembly, RefCoordIDs);     // EXPERIMENT!!!
                assembliesContainer.assemblies.Add(assemblyID, anassembly);
            }
            else
            {
                int z = 1;
                while (openEdges.Any())
                {
                    List<DataRep.CADEdge> closedEdges = new List<DataRep.CADEdge>();
                    List<string> closedComponents = new List<string>();

                    DataRep.CADEdge lastEdge = openEdges.Last();
                    string srcid = lastEdge.Src.Id;
                    FindComponentID(srcid,
                                    openComponents,
                                    openQ);
                    
                    WalkIncidentEdges(openQ, 
                                      openComponents, 
                                      closedComponents, 
                                      openEdges, 
                                      closedEdges);
                    
                    // create a new island CAssembly;
                    string ID = assemblyID + "|" + newAssemblyIDSeed.ToString();
                    DataRep.CADAssembly anassembly = new DataRep.CADAssembly(ID, assemblyName + "_" + newAssemblyIDSeed.ToString());
                    foreach (var key in closedComponents)
                    {
                        if (regularComponents.ContainsKey(key))
                        {
                            anassembly.ChildComponents.Add(key, regularComponents[key]);
                            component2assemblyTable[key]= ID;
                        }
                    }

                    anassembly.ChildEdges.AddRange(closedEdges);
                    ToDAG(anassembly, RefCoordIDs);
                    assembliesContainer.assemblies.Add(ID, anassembly);

                    newAssemblyIDSeed++;
                    z++;
                }

                if (openComponents.Any())
                {
                    foreach (string id in openComponents)
                    {
                        DataRep.CADComponent component;
                        if (regularComponents.TryGetValue(id, out component))
                            assembliesContainer.orphans.Add(component);
                    }
                }
                
                // size2fit
                // iterate through Size2Fit (are they orphans or belong to a CAssembly?)
		        // for each size2fit, find its size2fit edges
		        // find which assembly contains the edge's source
		        // if they are in the same 
		        foreach (var item in size2fitComponents)
		        {
			        List<DataRep.CADEdge> s2fclosedEdges = new List<DataRep.CADEdge>();				// <source, CEdge*>
			        string assemblyParent_ID = FindSize2FitEdges(item.Key, size2fitEdges, s2fclosedEdges);

			        // orphan if no edges or parentID is -1
			        if (!s2fclosedEdges.Any() || assemblyParent_ID == "-1")
			        {
                        assembliesContainer.orphans.Add(item.Value);
			        }
			        else
			        {
				        // add size2fit + size2fit edges to island assembly
                        if (assembliesContainer.assemblies.ContainsKey(assemblyParent_ID))
                        {
                            assembliesContainer.assemblies[assemblyParent_ID].ChildSize2FitComponents[item.Key] = item.Value;
                            assembliesContainer.assemblies[assemblyParent_ID].ChildSize2FitEdges.AddRange(s2fclosedEdges);
				        }
			        }
		        }

                // Testing
                //assembliesContainer.ToString();

                // Convert to DAG                
                //ToDAG(assembliesContainer.assemblies, RefCoordIDs);

                Print();

            } // else


            
            return assembliesContainer;
        }

        public void WalkIncidentEdges(Queue<string> openQ,
                                      List<string> openComponents,
                                      List<string> closedComponents,
                                      List<DataRep.CADEdge> openEdges,
                                      List<DataRep.CADEdge> closedEdges)
        {
            while (openQ.Any())
            {
                string srcid = openQ.Dequeue(), dstid = "";
                closedComponents.Add(srcid);

                IEnumerable<DataRep.CADEdge> matchSrc = openEdges.Where(x => x.Src.Id == srcid);
                foreach (var edge in matchSrc)
                {
                    dstid = edge.Dst.Id;
                    FindComponentID(dstid, openComponents, openQ);
                    closedEdges.Add(edge);
                }

                IEnumerable<DataRep.CADEdge> matchDst = openEdges.Where(x => x.Dst.Id == srcid);
                foreach (var edge in matchDst)
                {
                    dstid = edge.Src.Id;
                    FindComponentID(dstid, openComponents, openQ);
                    closedEdges.Add(edge);
                }

                openEdges.RemoveAll(x => x.Src.Id == srcid);
                openEdges.RemoveAll(x => x.Dst.Id == srcid);
                WalkIncidentEdges(openQ, openComponents, closedComponents, openEdges, closedEdges);
            }
        }


        string FindSize2FitEdges(string ID, 
                               List<DataRep.CADEdge> openEdges,
                               List<DataRep.CADEdge> closedEdges)
        {
	        string assemblyParentID = "-1";
	        foreach (var item in openEdges)
	        {
                if (item.Dst.Id == ID)
		        {
			        closedEdges.Add(item);
			        string tmp = item.Src.Id;

                    if (component2assemblyTable.ContainsKey(tmp))
                    {
                        tmp = component2assemblyTable[tmp];

                        if (assemblyParentID == "-1")
					        assemblyParentID = tmp;
				        else
				        {
					        if (assemblyParentID != tmp)
						        assemblyParentID = "-1";
				        }
			        }
		        }
	        }

	        return assemblyParentID;
        }



        // [1] find CComponent of ID_in in destinationSet_inout. if found, do nothing
        // [2] find CComponent of ID_in in openComponents_inout
        //			if found, add to destinationSet_inout and remove from openComponents_inout
        private void FindComponentID(string id,
                          List<string> openComponents,
                          Queue<string> destination)
        {
            if (destination.Contains(id))
                return;

            if (openComponents.Contains(id))
            {
                destination.Enqueue(id);
                openComponents.Remove(id);
            }
        }


        /***********************************************
         *          CONVERT TO DAG SECTION
        ***********************************************/

        public void ToDAG(Dictionary<string, DataRep.CADAssembly> assemblies,
                          List<string> RefCoordComponents)
        {
            foreach (var item in assemblies.Values)
                ToDAG(item, RefCoordComponents);
        }

        public void ToDAG(DataRep.CADAssembly assembly,
                          List<string> RefCoordComponents)
        {
            string start_component_id = "-1";
            List<string> openComponents = new List<string>();
            List<string> closedComponents = new List<string>();


            // [1] Create openEdges
            List<DataRep.CADEdge> openEdges = new List<DataRep.CADEdge>();
            openEdges.AddRange(assembly.ChildEdges);

            // [2] Find starting component and create openComponents set
            /*
            // META-2885
            start_component_id = FindStartingComponent(assembly.ChildComponents.Keys.ToList(),
                                                       RefCoordComponents,
                                                       openEdges,
                                                       openComponents);
            */
            start_component_id = FindStartingComponent(assembly.ChildComponents.OrderBy(x => x.Value.Name).ToDictionary(x => x.Key, x => x.Value).Keys.ToList(),
                                                       RefCoordComponents,
                                                       openEdges,
                                                       openComponents);
            assembly.RootComponentID = start_component_id;
            closedComponents.Add(start_component_id);
            // cassemblyParent_in->SetRootComponentID(start_component_id);                                            
            int degree = 6, openSize = openComponents.Count();

            while (openComponents.Any())
            {
                ConstrainComponents(closedComponents, openEdges, degree, openComponents);
                //ConstrainComponents(closedComponents, assembly.ChildEdges, degree, openComponents);
                int tmp_open_size = openComponents.Count();

                if (tmp_open_size == openSize)			// was not able to find components with edges of dof 6
                    degree--;
                else
                    degree = 6;

                openSize = tmp_open_size;
            }

            if (openEdges.Any())
            {
                debugMessages.Add("+++++ Left Over Edges +++++\n");
                foreach (var edge in openEdges)
                    debugMessages.Add("		Source: " + edge.Src.Id + "Dst: " + edge.Dst.Id + "\n");
            }

        }


        private void ConstrainComponents(List<string> closedComponents,
                                         List<DataRep.CADEdge> openEdges,
                                         int degree_in,
                                         List<string> openComponents)
        {
            bool search = true;
            List<string> newOpenComponents = new List<string>();
            foreach (var item in openComponents)
            {
                string my_component_id = item;
                if (search)
                {
                    if (!FindEdges(my_component_id, degree_in, closedComponents, openEdges))
                        newOpenComponents.Add(my_component_id);
                    else
                    {
                        if (degree_in < 6)
                            search = false;
                    }
                }
                else
                    newOpenComponents.Add(my_component_id);
            }

            openComponents.RemoveRange(0, openComponents.Count());
            openComponents.AddRange(newOpenComponents);
        }


        private bool FindEdges(string my_component_id,
                               int degree_in,
                               List<string> closedComponents,
                               List<DataRep.CADEdge> openEdges)
        {
            bool status;
            List<DataRep.CADEdge> newOpenEdges = new List<DataRep.CADEdge>();
            List<DataRep.CADEdge> found_edges_set = new List<DataRep.CADEdge>();

            int totalDegrees = 0;

            foreach (var item in openEdges)
            {
                string srcID = item.Src.Id, dstID = item.Dst.Id;
                if (my_component_id == srcID && closedComponents.Contains(dstID))
                {
                    // Reverse Src and Dst in CEdge
                    DataRep.CADData tmpCADData = item.Src;
                    string tmpPort = item.SrcPortID, tmpPortName = item.SrcPortName;
                    item.Src = item.Dst;
                    item.SrcPortID = item.DstPortID;
                    item.SrcPortName = item.DstPortName;
                    item.Dst = tmpCADData;
                    item.DstPortID = tmpPort;
                    item.DstPortName = tmpPortName;
                    // end

                    totalDegrees += item.DegreeFreedom;
                    found_edges_set.Add(item);
                }
                else if (my_component_id == dstID && closedComponents.Contains(srcID))
                {
                    totalDegrees += item.DegreeFreedom;
                    found_edges_set.Add(item);
                }
                else
                    newOpenEdges.Add(item);
            }

            if (totalDegrees < degree_in)
            {
                foreach (var item in found_edges_set)
                    newOpenEdges.Add(item);		// insert edges back into newOpenEdges
                status = false;
            }
            else
            {
                debugMessages.Add(my_component_id + " - DOF(" + totalDegrees + ")\n");
                foreach (var item in found_edges_set)
                    debugMessages.Add("		Source: " + item.Src.Id + " Dst: " + item.Dst.Id + "\n");

                closedComponents.Add(my_component_id);
                status = true;
            }

            openEdges.RemoveRange(0, openEdges.Count());
            openEdges.AddRange(newOpenEdges);

            return status;

            /*
            bool status;
            List<DataRep.CADEdge> newOpenEdges = new List<DataRep.CADEdge>();
            List<DataRep.CADEdge> found_edges_set = new List<DataRep.CADEdge>();

            int totalDegrees = 0;

            foreach (var item in openEdges)
            {
                string srcID = item.Src.Id, dstID = item.Dst.Id;
                if (my_component_id == srcID && closedComponents.Contains(dstID))
                {
                    // Reverse Src and Dst in CEdge
                    DataRep.CADData tmpCADData = item.Src;
                    string tmpPort = item.SrcPortID;
                    item.Src = item.Dst;
                    item.SrcPortID = item.DstPortID;
                    item.Dst = tmpCADData;
                    item.DstPortID = tmpPort;
                    // end

                    totalDegrees += item.DegreeFreedom;
                    found_edges_set.Add(item);
                }
                else if (my_component_id == dstID && closedComponents.Contains(srcID))
                {
                    totalDegrees += item.DegreeFreedom;
                    found_edges_set.Add(item);
                }
                else
                    newOpenEdges.Add(item);
            }

            if (totalDegrees < degree_in)
            {
                foreach (var item in found_edges_set)
                    newOpenEdges.Add(item);		// insert edges back into newOpenEdges
                status = false;
            }
            else
            {
                debugMessages.Add(my_component_id + " - DOF(" + totalDegrees + ")\n");
		        foreach (var item in found_edges_set)
                    debugMessages.Add("		Source: " + item.Src.Id + " Dst: " + item.Dst.Id + "\n");

                closedComponents.Add(my_component_id);
                status = true;
            }

            openEdges.RemoveRange(0, openEdges.Count());
            openEdges.AddRange(newOpenEdges);

            return status;
            */
        }


        // Find root component ID
        // Populate openComponents set
        public string FindStartingComponent(List<string> components,
                                     List<string> RefCoordSystemComponentIDs,
                                     List<DataRep.CADEdge> openEdges,
                                     List<string> openComponents
            )
        {
            string start_id = "-1";

            foreach (var item in components)
            {
                if (RefCoordSystemComponentIDs.Contains(item))
                {
                    start_id = item;
                    break;
                }
            }

            int degrees = 6;
            while (start_id == "-1" && degrees > 2)		// if not in RefCoordSystemComponentIDs_in
            {
                //Dictionary<string, int> id_degree_table = new Dictionary<string, int>();

                Dictionary<string, KeyValuePair<string, int>> id_degree_table = new Dictionary<string, KeyValuePair<string, int>>();
                foreach (var edge in openEdges)
                {
                    if (edge.DegreeFreedom == degrees)
                    {
                        string src_id = edge.Src.Id, dst_id = edge.Dst.Id;
                        if (!id_degree_table.ContainsKey(src_id))
                        {
                            //id_degree_table.Add(src_id, 1);
                            id_degree_table.Add(src_id, new KeyValuePair<string, int>(edge.Src.Name, 1));
                        }
                        else
                        {
                            //int total = id_degree_table[src_id];
                            //id_degree_table[src_id] = total + 1;
                            int total = id_degree_table[src_id].Value;
                            id_degree_table[src_id] = new KeyValuePair<string, int>(id_degree_table[src_id].Key, total + 1);
                        }

                        if (!id_degree_table.ContainsKey(dst_id))
                        {
                            //id_degree_table.Add(dst_id, 1);
                            id_degree_table.Add(dst_id, new KeyValuePair<string, int>(edge.Dst.Name, 1));
                        }
                        else
                        {
                            //int total = id_degree_table[dst_id];
                            //id_degree_table[dst_id] = total + 1;
                            int total = id_degree_table[dst_id].Value;
                            id_degree_table[dst_id] = new KeyValuePair<string, int>(id_degree_table[dst_id].Key, total + 1);
                        }
                    }
                }

                int max_dof = 0;
                string max_dof_component_id = "-1";

                //Dictionary<string, KeyValuePair<string, int>> sortedDict = id_degree_table.OrderByDescending(x => x.Value.Value).OrderBy(x => x.Value.Key).ToDictionary(t => t.Key, t => t.Value);
                Dictionary<string, KeyValuePair<string, int>> sortedDict = id_degree_table.OrderByDescending(x => x.Value.Value).ThenBy(x => x.Value.Key).ToDictionary(t => t.Key, t => t.Value);
                if (sortedDict.Any())
                {
                    max_dof = sortedDict.First().Value.Value;
                    max_dof_component_id = sortedDict.First().Key;
                }

                /*
                int max_dof = 0;
                string max_dof_component_id = "-1";

                foreach (var item in id_degree_table)
                {
                    if (item.Value > max_dof)
                    {
                        max_dof = item.Value;
                        max_dof_component_id = item.Key;
                    }
                }
                */

                if (max_dof_component_id != "-1")
                    start_id = max_dof_component_id;
                else
                    degrees--;
            }

            if (start_id == "-1")
                start_id = components.First();

            foreach (var item in components)
            {
                string tmp = item;
                if (start_id != tmp)
                    openComponents.Add(tmp);
            }

            return start_id;
        }

        public void Print()
        {
            using (StreamWriter writer = new StreamWriter(Path.Combine(OutputDirectory, "CyPhy2CAD_Convert2DAG.log")))
            {
                foreach (var line in debugMessages)
                    writer.WriteLine(line);
            }
        }


    }   // end class
}   // end namespace
