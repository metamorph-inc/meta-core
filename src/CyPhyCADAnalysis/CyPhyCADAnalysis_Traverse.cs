namespace CyPhyCADAnalysis
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.IO;
    using System.Diagnostics;

    using GME;
    using GME.CSharp;
    using GME.MGA;
    using GME.MGA.Core;
    using Common = ISIS.GME.Common;
    using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
    using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
    using Newtonsoft.Json;
    using Newtonsoft.Json.Converters;
    using META;

    public partial class CyPhyCADAnalysisInterpreter
    {

        private string ManufacturingGeneration(MgaFCO currentobj)
        {

            if (currentobj.MetaBase.Name == "TestBench")
            {
                // DDP Generation
                CyPhy.TestBench tb = CyPhyClasses.TestBench.Cast(currentobj);
                var catlsut = tb.Children.ComponentAssemblyCollection.FirstOrDefault();     // should be an instance b/c elaborate was called earlier
                if (catlsut == null)
                {
                    throw new Exception("There is no elaborated system under test component assembly in the model!");
                }

                var design = CyPhy2DesignInterchange.CyPhy2DesignInterchange.Convert(catlsut);
                this.TestBenchName = tb.Name;
                this.AssemblyName = design.Name;
                design.SaveToFile(Path.Combine(this.OutputDirectory, this.TestBenchName + ".adm"));
                

                foreach (var wf_item in tb.Children.WorkflowRefCollection)
                {
                    string TB_child_item_name = wf_item.Name;

                    if (wf_item.Kind == "WorkflowRef")  // not needed
                    {
                        var workflow_item = wf_item.ReferencedBy;
                        // look for child task...
                        foreach (var task_item in wf_item.Referred.Workflow.Children.TaskCollection)
                        {
                            string TaskName = task_item.Name;
                            //wkflow_param = (task_item.Attributes.Parameters).ToLower(); //Grab parameter name -- which 'type' of test bench                                                       
                            wkflow_param = task_item.Attributes.Parameters;
                        }
                    }
                }
                // End RB 7/17/13   




                if (catlsut.Attributes.ConfigurationUniqueID.Contains("{"))
                    this.ManufacturingManifestData.DesignID = catlsut.Attributes.ConfigurationUniqueID;
                else
                    this.ManufacturingManifestData.DesignID = "{" + catlsut.Attributes.ConfigurationUniqueID + "}";
                this.ManufacturingManifestData.Name = catlsut.Name;
                PartManufacturingGeneration(catlsut);

            }

            else if (currentobj.MetaBase.Name == "ComponentAssembly")
            {
                // DDP Generation
                CyPhy.ComponentAssembly assembly = CyPhyClasses.ComponentAssembly.Cast(currentobj);

                var design = CyPhy2DesignInterchange.CyPhy2DesignInterchange.Convert(assembly);
                this.AssemblyName = design.Name;
                design.SaveToFile(Path.Combine(this.OutputDirectory, this.TestBenchName + ".adm"));
                
                this.ManufacturingManifestData.DesignID = "{" + assembly.Attributes.ConfigurationUniqueID + "}";
                this.ManufacturingManifestData.Name = assembly.Name;
                PartManufacturingGeneration(assembly);
            }
            else
            {
                throw new NotImplementedException();
            }
            return wkflow_param;

        }


        private void PartManufacturingGeneration(CyPhy.ComponentAssembly componentasm)
        {
            // [1] Create ManufacturingData
            // [2] Regenerate manufacturing xml file for each ManufacturingData
            // [3] Generate manifest file

            if (this.AVMComponentList == null)
                return;

            TraverseComponentAssembly(componentasm);
            //foreach (ComponentManufacturingData data in this.ComponentManufacturingDataList)
            //{
            //    data.UpdateManufacturingSpec();
            //}

            // manifest
            string reportJson = Newtonsoft.Json.JsonConvert.SerializeObject(
                this.ManufacturingManifestData,
                Newtonsoft.Json.Formatting.Indented);

            using (StreamWriter writer = new StreamWriter(Path.Combine(this.OutputDirectory, "manufacturing.manifest.json")))
            {
                writer.WriteLine(reportJson);
            }

        }
        private void TraverseComponentAssembly(CyPhy.ComponentAssembly componentasm)
        {
            foreach (CyPhy.ComponentRef cref in componentasm.Children.ComponentRefCollection)
            {
                throw new Exception("Model not fully elaborated, contains ComponentRef [" + cref.Path + "]");
            }

            foreach (CyPhy.ComponentAssembly cainst in componentasm.Children.ComponentAssemblyCollection)
            {
                TraverseComponentAssembly(cainst);
            }

            foreach (CyPhy.Component cint in componentasm.Children.ComponentCollection)
            {
                foreach (CyPhy.ManufacturingModel manModel in cint.Children.ManufacturingModelCollection)
                {
                    string manfilename;
                    if (! manModel.TryGetResourcePath(out manfilename) ||
                           String.IsNullOrWhiteSpace(manfilename))
                    {
                        GMEConsole.Warning.Write("ManufacturingModel's FileLocation does not have a valid file name [" + manModel.Path + "]");
                        continue;
                    }

                    string avmid = cint.Attributes.AVMID;
                    string ComponentPath = cint.GetDirectoryPath();
                    string RandnewMfgFileName;
                    
                    {
                        string MfgDir = Path.Combine(this.OutputDirectory, "ManufacturingModels"); 
                        if (!Directory.Exists(MfgDir))
                            Directory.CreateDirectory(MfgDir);

                        string MfgComponentFileLocation;
                        char[] charsToTrim1 = { '{', '}' };
                        string newMfgFileNameGUID = cint.Attributes.ID + ".xml";
                        string newMfgFileNameAVMID = cint.Attributes.AVMID.Trim(charsToTrim1) + ".xml";
                        RandnewMfgFileName = System.IO.Path.GetRandomFileName() + ".xml";                // RB META-3152
                        //MfgComponentFileLocation = Path.Combine(MfgDir, newMfgFileNameGUID); // manfilename); 
                        MfgComponentFileLocation = Path.Combine(MfgDir, RandnewMfgFileName); // manfilename);   // RB META-3152
                        string resource_path;
                        manModel.TryGetResourcePath(out resource_path, ComponentLibraryManager.PathConvention.ABSOLUTE);
                        File.Copy(resource_path, MfgComponentFileLocation, true);
                    }

                    // for xml data
                    ComponentManufacturingData mfdata = new ComponentManufacturingData();
                    mfdata.AVMID = cint.Attributes.AVMID;
                    mfdata.RevID = cint.Attributes.Revision;
                    mfdata.VerID = cint.Attributes.Version;
                    mfdata.GUID = cint.Attributes.InstanceGUID;
                    mfdata.Name = cint.Name;
                    mfdata.Location = Path.Combine(this.ProjectRootDirectory, ComponentPath, manfilename);         // form path
                    mfdata.NewLocation = Path.Combine(this.OutputDirectory, "ManufacturingModels");

                    foreach (CyPhy.ManufacturingModelParameter param in manModel.Children.ManufacturingModelParameterCollection)
                    {
                        int srcCount = param.AllSrcConnections.Count();
                        if (srcCount > 1)
                            throw new Exception("ManufacturingModelParameter is connected to >1 value flow targets [" + param.Path + "]");

                        if (String.IsNullOrEmpty(param.Attributes.Value))
                        {
                            if (String.IsNullOrEmpty(param.Attributes.DefaultValue))
                                continue;
                            else
                                mfdata.ManufacturingParamters.Add(param.Name, param.Attributes.DefaultValue);
                        }
                        else
                            mfdata.ManufacturingParamters.Add(param.Name, param.Attributes.Value);
                    }

                    this.ComponentManufacturingDataList.Add(mfdata);

                    // for manifest data
                    char[] charsToTrim = { '{', '}' };
                    var componentmanifestdata = new DesignManufactureManifest.ComponentManufactureManifestData(
                                                                                        cint.Attributes.InstanceGUID, 
                                                                                        cint.Attributes.ID.ToString(), 
                                                                                        mfdata.NewLocation);
                    string fileName = cint.Attributes.ID + ".xml";
                    componentmanifestdata.ManufacturingModel = "ManufacturingModels/" + RandnewMfgFileName;       // RB META-3152
                    //componentmanifestdata.ManufacturingModel = "ManufacturingModels/" + fileName;
                    this.ManufacturingManifestData.ComponentManufactureList.Add(componentmanifestdata);


                }               

            }
        }

        public Process process { get; set; }
    }
}
