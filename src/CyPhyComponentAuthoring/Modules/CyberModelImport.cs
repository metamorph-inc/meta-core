using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using System.Xml;
using CyPhyComponentExporter;
using GME.MGA;
using GME.MGA.Meta;
using Microsoft.Win32;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

using CyberComposition = ISIS.GME.Dsml.CyberComposition.Interfaces;
using CyberCompositionClass = ISIS.GME.Dsml.CyberComposition.Classes;
using META;

namespace CyPhyComponentAuthoring.Modules
{
    [CyPhyComponentAuthoringInterpreter.IsCATModule(ContainsCATmethod = true)]
    public class CyberModelImport : CATModule
    {
        // constants for adding new parameters to the GME display at the right coordinates
        private const int PARAMETER_START_X = 20;
        private const int PARAMETER_START_Y = 200;
        private const int PARAMETER_ADJUST_Y = 40;

        // constants for adding new resources to the GME display at the right coordinates
        private const int RESOURCE_START_X = 650;
        private const int RESOURCE_START_Y = 200;

        // constants for adding new Cyber models to the GME display at the right coordinates
        private const int Cyber_MODEL_START_X = 270;
        private const int Cyber_MODEL_START_Y = 200;

        // used to determine the element that is the farthest "south" on the display
        private int greatest_current_y = 0;

        public CyPhyGUIs.GMELogger Logger { get; set; }
        private bool Close_Dlg;

        [CyPhyComponentAuthoringInterpreter.CATName(
            NameVal = "Add Cyber",
            DescriptionVal = "Import an existing Creo Cyber model and associate it with this CyPhy Component.  You must have Creo installed on this machine to convert the model file.",
            RoleVal = CyPhyComponentAuthoringInterpreter.Role.Construct
            )
        ]
        public void callImportCyberModel(object sender, EventArgs e)
        {
            ImportCyberModel();

            // Close the calling dialog box if the module ran successfully
            if (Close_Dlg)
            {
                // calling object is a button
                Button callerBtn = (Button)sender;
                // the button is in a layout panel
                TableLayoutPanel innerTLP = (TableLayoutPanel)callerBtn.Parent;
                // the layout panel is a table within a table
                TableLayoutPanel outerTLP = (TableLayoutPanel)innerTLP.Parent;
                // the TLP is in the dialog box
                Form parentDB = (Form)outerTLP.Parent;
                parentDB.Close();
            }
        }

        public void ImportCyberModel(string Cyberpath = null)
        {
            string CyberFilename = "";
            this.Logger = new CyPhyGUIs.GMELogger(CurrentProj, this.GetType().Name);

            //  - Display a dialog box to let the user choose their Cyber model file
            bool Cyber_file_chosen = false;
            //bool test_copy_and_path_only = false;
            if (string.IsNullOrWhiteSpace(Cyberpath))
            {
                Cyber_file_chosen = get_Cyber_file(out CyberFilename);
            }
            else
            {
                //test_copy_and_path_only = true;
                CyberFilename = Cyberpath;
                if (File.Exists(CyberFilename))
                {
                    Cyber_file_chosen = true;
                }
                else
                {
                    this.Logger.WriteError("Invalid Cyber file path passed in: " + Cyberpath);
                }
            }

            //  - Run the extractor on the Creo model file
            #region Run the Extractor

            List<string> componentList = new List<string>();
            IMgaProject project = (IMgaProject)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaProject"));
            string compName;
            project.OpenEx("MGA=" + CyberFilename, "CyberComposition", null);
            try
            {
                project.BeginTransactionInNewTerr();
                IMgaFCO currentObj;
                try
                {
                    IMgaFolder currentFolder = (IMgaFolder)project.RootFolder;
                    IMgaFolders cFolders = currentFolder.ChildFolders;
                    foreach (IMgaFolder f in cFolders)
                    {
                        if (f.MetaFolder.Name == "Components")
                        {
                            IMgaFCOs objects = f.ChildFCOs;
                            foreach (IMgaFCO o in objects)
                            {
                                componentList.Add("/" + f.Name + "/" + o.Name);
                            }
                        }
                    }  
                }
                finally
                {
                    project.CommitTransaction();
                }


                // Adapted from ModelicaImporter.cs 
                string result = "";
                using (CyberComponentPicker cyberpicker = new CyberComponentPicker(componentList))
                {
                    var dialogResult = cyberpicker.ShowDialog();
                    if (dialogResult != System.Windows.Forms.DialogResult.OK)
                    {
                        this.Logger.WriteInfo("Modelica import was cancelled by the user.");
                        return;
                    }

                    result = cyberpicker.compResult;
                }

                var halves = result.Split('/');
                compName = halves[1];

                project.BeginTransactionInNewTerr();
                try
                {
                    currentObj = (IMgaFCO)project.RootFolder.ObjectByPath[result];
                }
                finally
                {
                    project.CommitTransaction();
                }

                IMgaComponentEx comp = (IMgaComponentEx)Activator.CreateInstance(Type.GetTypeFromProgID("MGA.Interpreter.CyberComponentExporter"));
                comp.Initialize((MgaProject)project);
                comp.InvokeEx((MgaProject)project, (MgaFCO)currentObj, null, 128);

                project.Save();
            }
            finally
            {
                project.Close(true);
            }
 
            #endregion

            //  - Use a function from CyPhy2ComponentModel to convert the extractor's XML format into a CyPhy model fragment
            #region Convert_to_XML
            // used in creating the resource object below
            CyPhy.CyberModel ProcessedCyberModel = null;

            if (true)
            {
                this.Logger.WriteDebug("About to call DeserializeAvmComponentXml...");
                StreamReader streamReader = new StreamReader(compName + ".component.acm");
                avm.Component ac_import = CyPhyComponentImporter.CyPhyComponentImporterInterpreter.DeserializeAvmComponentXml(streamReader);
                streamReader.Close();
                this.Logger.WriteDebug("... finished DeserializeAvmComponentXml call.");

                foreach (avm.cyber.CyberModel Cybermodel in ac_import.DomainModel.Where(dm => dm is avm.cyber.CyberModel)
                                                                          .Cast<avm.cyber.CyberModel>())
                {
                    var rf = CyPhyClasses.RootFolder.GetRootFolder(CurrentProj);

                    Dictionary<string, CyPhy.Component> avmidComponentMap = new Dictionary<string, CyPhy.Component>();
                    AVM2CyPhyML.CyPhyMLComponentBuilder newComponent = new AVM2CyPhyML.CyPhyMLComponentBuilder(rf);
                    ProcessedCyberModel = newComponent.process(Cybermodel, GetCurrentComp());
                    ProcessedCyberModel.Name = Path.GetFileNameWithoutExtension(CyberFilename);
                }

                // find the largest current Y value so our new elements are added below the existing design elements
                foreach (var child in GetCurrentComp().AllChildren)
                {
                    foreach (MgaPart item in (child.Impl as MgaFCO).Parts)
                    {
                        int read_x, read_y;
                        string read_str;
                        item.GetGmeAttrs(out read_str, out read_x, out read_y);
                        greatest_current_y = (read_y > greatest_current_y) ? read_y : greatest_current_y;
                    }
                }

                // layout Cyber model to the "south" of existing elements
                foreach (MgaPart item in (ProcessedCyberModel.Impl as MgaFCO).Parts)
                {
                    item.SetGmeAttrs(null, Cyber_MODEL_START_X, greatest_current_y + Cyber_MODEL_START_Y);
                }

                // Extend it's properties out to the component level.
                this.CyberModuleImportExtension(ProcessedCyberModel);
            }

            #endregion

            //  - Copy the Cyber Model files into the component's backend folder
            //      - Note: The solution includes a function that can find this folder using the project.manifest.json file.
            //      - For nice organization, create them in a subfolder called "Cyber"
            // create avmproj
            #region Copy files to backend folder

            // used in creating the resource object below
            string PathforComp = null;

            var importedCyberFiles = new List<String>();
            if (true)
            {
                try
                {
                    // create the destination path
                    PathforComp = GetCurrentComp().GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE);
                    
                    string finalPathName = Path.Combine(PathforComp, "Cyber");

                    Directory.CreateDirectory(finalPathName);

                    // determine if one part file or all part and assembly files need to be copied
                    string cpsrcfile = System.IO.Path.GetFileName(CyberFilename);

                    // copy the selected file
                    string CyberFileCopyPath = System.IO.Path.Combine(finalPathName, cpsrcfile);
                    System.IO.File.Copy(CyberFilename, CyberFileCopyPath, true);

                    // Set "primary" file as the first in the list.
                    importedCyberFiles.Add(Path.Combine("Cyber", Path.GetFileName(CyberFileCopyPath)));

                    if (true)
                    {
                        // get a string of the XML contents
                        this.Logger.WriteDebug("About to read contents of XML file using class XmlDocument: " + compName + ".component.acm");

                        XmlDocument doc = new XmlDocument();
                        doc.Load(compName + ".component.acm");
                        string xmlcontents = doc.InnerXml;

                        // mine down to the Resource dependencies
                        using (XmlReader reader = XmlReader.Create(new StringReader(xmlcontents)))
                        {
                            // iterate through each file listed in the resourcedependency section
                            while (reader.ReadToFollowing("ResourceDependency") == true)
                            {
                                string res_name = reader.GetAttribute("Name");
                                string res_path = reader.GetAttribute("Path");
                                this.Logger.WriteDebug("Copying this file: " + res_path + "\\" + res_name);

                                // Cyber files end in .1 .2 etc. Pick the latest ones
                                var allFiles = Directory.EnumerateFiles(Path.GetDirectoryName(res_path), "*mga." /*n.b. literal dot*/ + "*")
                                    .Select(Path.GetFileName)
                                    .Select(filename => new { basename = filename.Substring(0, filename.LastIndexOf('.')), version = filename.Substring(filename.LastIndexOf('.') + 1) })
                                    .Where(p => { int val; return Int32.TryParse(p.version, out val); })
                                    .OrderByDescending(p => Int32.Parse(p.version))
                                    .ToArray();
                                foreach (var basename in allFiles.Select(p => p.basename).Distinct())
                                {
                                    var latest = allFiles.Where(p => p.basename == basename).FirstOrDefault();
                                    if (latest != null)
                                    {
                                        string latestFilename = latest.basename + "." + latest.version;
                                        // Need to limit this down to just the filename in question
                                        // The XML file changes the name to all caps, so compare apples to apples
                                        if (latestFilename.ToUpper().StartsWith(res_name.ToUpper()))
                                        {
                                            string destpathandname = Path.Combine(finalPathName, latestFilename);
                                            if (!importedCyberFiles.Contains(Path.Combine("Cyber", Path.GetFileName(destpathandname))))
                                            {
                                                importedCyberFiles.Add(Path.Combine("Cyber", Path.GetFileName(destpathandname)));
                                            }
                                            File.Copy(Path.Combine(Path.GetDirectoryName(res_path), latestFilename), destpathandname, true);
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                catch (Exception err_create_proj)
                {
                    this.Logger.WriteError("Error creating AVM project: " + err_create_proj.Message, " - Extraction Failed. Possible ComponentManagement issue.");
                    //cleanup(tempXMLfile, true);
                    return;
                }
            }
            #endregion

            //  - Create Resource objects in the CyPhy Component model that point to these Cyber Model files
            //      - Note: The "paths" of these should be the relative path from that component's root folder
            if (true)
            {
                foreach (var CyberFile in importedCyberFiles)
                {

                    CyPhy.Resource ResourceObj = CyPhyClasses.Resource.Create(GetCurrentComp());
                    ResourceObj.Attributes.ID = Guid.NewGuid().ToString("B");
                    ResourceObj.Attributes.Path = CyberFile;
                    ResourceObj.Attributes.Notes = "Cyber Model Import tool added this resource object for the imported Cyber file";
                    ResourceObj.Name = Path.GetFileName(CyberFile);

                    // layout Resource just to the side of the Cyber model
                    foreach (MgaPart item in (ResourceObj.Impl as MgaFCO).Parts)
                    {
                        item.SetGmeAttrs(null, RESOURCE_START_X, greatest_current_y + RESOURCE_START_Y);
                    }

                    // The "primary" Cyber model is the first one -- associate it with the CyPhy CyberModel object
                    if (importedCyberFiles.IndexOf(CyberFile) == 0)
                    {
                        //  - Create a UsesResource association between the CyPhy CyberModel object and the Resource object that represents the top-level Creo Model file.
                        CyPhyClasses.UsesResource.Connect(ResourceObj, ProcessedCyberModel, null, null, GetCurrentComp());
                    }
                }
            }

            // throw in an ACM file for the current state of the component.
            if (true)
            {
                var exporter = new CyPhyComponentExporter.CyPhyComponentExporterInterpreter();
                String acmPath = Path.Combine(PathforComp,GetCurrentComp().Name + ".component.acm");
                CyPhyComponentExporterInterpreter.ExportToFile(GetCurrentComp(), Path.GetDirectoryName(acmPath));
            }

            // Clean up
            //cleanup(tempXMLfile, true);
        }

        // Clean up resources on all return paths
        void cleanup(string tempfile, bool close_dlg = false)
        {
            Close_Dlg = close_dlg;
            this.Logger.Dispose();
            File.Delete(tempfile);
        }

        // Display a file dialog box to select the Cyber model file to import
        // parameter will contain the filename, returns true or false based on if a file is returned or not
        bool get_Cyber_file(out string FileName)
        {
            // Open file dialog box
            DialogResult dr;
            using (OpenFileDialog ofd = new OpenFileDialog())
            {
                ofd.CheckFileExists = true;
                ofd.DefaultExt = "test.mdl";
                ofd.Multiselect = false;
                ofd.Filter = "MGA files (*.mga)|*.mga*|All files (*.*)|*.*";
                dr = ofd.ShowDialog();
                if (dr == DialogResult.OK)
                {
                    FileName = ofd.FileName;
                    return true;
                }
            }
            FileName = "";
            this.Logger.WriteError("No file was selected.  Cyber Import will not complete.");
            return false;
        }


        // Extend Cyber CAT module to create component-level Parameters for Cyber Model's Parameters
        void CyberModuleImportExtension(CyPhy.CyberModel ProcessedCyberModel)
        {
            // When the user imports a Cyber Model, the current implementation creates a CyPhy CyberModel object. After this point: 
            int numComponentParams = 0;
            foreach (CyPhy.ModelicaParameter parm in ProcessedCyberModel.Children.ModelicaParameterCollection)
            {
                // - For each ModelicaParameter object, create a corresponding Parameter object under the parent Component. 
                //   - Give the Parameter the same name 
                CyPhy.Parameter newparam = CyPhyClasses.Parameter.Create(GetCurrentComp());
                newparam.Name = parm.Name;
                if ( ! String.IsNullOrWhiteSpace(parm.Attributes.Value) )
                    newparam.Attributes.Value = parm.Attributes.Value;

                // - Create a CyberParameterPortMap connection from the Property to the CyberParameter 
                CyPhy.ModelicaParameterPortMap ppmap = CyPhyClasses.ModelicaParameterPortMap.Connect(newparam, parm);
                ppmap.Name = parm.Name;

                // - Perform some layout "niceification" on the resulting objects. 
                foreach (MgaPart item in (newparam.Impl as MgaFCO).Parts)
                {
                    item.SetGmeAttrs(null, PARAMETER_START_X, greatest_current_y + PARAMETER_START_Y + (numComponentParams * PARAMETER_ADJUST_Y));
                }
                numComponentParams++;
            }
        }
    }
}
