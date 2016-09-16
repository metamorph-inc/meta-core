using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using System.Xml;
using CyPhyComponentExporter;
using GME.MGA;
using Microsoft.Win32;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using META;

namespace CyPhyComponentAuthoring.Modules
{
    [CyPhyComponentAuthoringInterpreter.IsCATModule(ContainsCATmethod = true)]
    public class CADModelImport : CATModule
    {
        // constants for adding new parameters to the GME display at the right coordinates
        private const int PARAMETER_START_X = 20;
        private const int PARAMETER_START_Y = 200;
        private const int PARAMETER_ADJUST_Y = 40;

        // constants for adding new resources to the GME display at the right coordinates
        private const int RESOURCE_START_X = 650;
        private const int RESOURCE_START_Y = 200;

        // constants for adding new CAD models to the GME display at the right coordinates
        private const int CAD_MODEL_START_X = 270;
        private const int CAD_MODEL_START_Y = 200;

        // used to determine the element that is the farthest "south" on the display
        private int greatest_current_y = 0;

        public CyPhyGUIs.GMELogger Logger { get; set; }
        private bool Close_Dlg;

        [CyPhyComponentAuthoringInterpreter.CATName(
            NameVal = "Add CAD",
            DescriptionVal = "Import an existing Creo CAD model and associate it with this CyPhy Component.  You must have Creo installed on this machine to convert the model file.",
            RoleVal = CyPhyComponentAuthoringInterpreter.Role.Construct
            )
        ]
        public void callImportCADModel(object sender, EventArgs e)
        {
            ImportCADModel();

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

        public void ImportCADModel(string CADpath = null)
        {
            string cadFilename = "";
            this.Logger = new CyPhyGUIs.GMELogger(CurrentProj, this.GetType().Name);

            //The CAT module will perform these steps, in this order:
            //  - Check that the user has Creo (so that the extraction will be successful)
            //      - Implementation Suggestion: The extraction utility may have special flag to have it indicates if all the dependencies are met
            bool creo_installed = this.CREO_present();

            //  - Display a dialog box to let the user choose their Creo model file
            bool cad_file_chosen = false;
            bool test_copy_and_path_only = false;
            if (string.IsNullOrWhiteSpace(CADpath))
            {
                cad_file_chosen = get_cad_file(out cadFilename);
            }
            else
            {
                test_copy_and_path_only = true;
                cadFilename = CADpath;
                if (File.Exists(cadFilename))
                {
                    cad_file_chosen = true;
                }
                else
                {
                    this.Logger.WriteError("Invalid CAD file path passed in: " + CADpath);
                }
            }

            //  - Run the extractor on the Creo model file
            #region Run the Extractor

            bool extractor_ran_success = test_copy_and_path_only;
            string tempXMLfile = Path.GetTempFileName(); 
            this.Logger.WriteDebug("Temporary XML file created: " + tempXMLfile);

            if (creo_installed && cad_file_chosen && !test_copy_and_path_only)
            {
                try
                {
                    // assemble the arg string
                    // first the input CAD file
                    // include quotation marks to handle paths with white spaces
                    string argstring = "-c \"";
                    argstring += cadFilename;
                    argstring += "\"";

                    // add the XML output file name
                    argstring += " -x ";
                    argstring += tempXMLfile;

                    // Debug only: pause before exit, graphics mode.
                    //argstring += " -p -g";

                    Process firstProc = new Process();

                    // NOTE: Process class does not expand environment variables, do it manually
                    string temp = Environment.GetEnvironmentVariable("PROE_ISIS_EXTENSIONS");
                    if (temp == null)
                    {
                        this.Logger.WriteError("Please set the PROE_ISIS_EXTENSIONS environment variable");
                        cleanup(tempXMLfile, true);
                        return;
                    }
                    string path = Path.Combine(temp, "bin\\ExtractACM-XMLfromCreoModels.exe");
                    if (!File.Exists(path))
                    {
                        this.Logger.WriteError("Cannot find ExtractACM-XMLfromCreoModels.exe - Check if CAD Assembler was built and files installed to PROE_ISIS_EXTENSIONS");
                        throw new Exception("ExtractACM-XMLfromCreoModels.exe not found.");
                    }

                    firstProc.StartInfo.FileName = path;
                    firstProc.StartInfo.Arguments = argstring;
                    this.Logger.WriteDebug("Calling ExtractACM-XMLfromCreoModels.exe with argument string: " + argstring);

                    firstProc.EnableRaisingEvents = true;

                    firstProc.Start();

                    firstProc.WaitForExit();

                    this.Logger.WriteDebug("ExtractACM-XMLfromCreoModels.exe has completed.");

                    if (firstProc.ExitCode == 0)
                    {
                        extractor_ran_success = true;
                    }
                    else
                    {
                        this.Logger.WriteDebug("ExtractACM-XMLfromCreoModels.exe returned error code " + firstProc.ExitCode.ToString());
                        throw new Exception("Extract executable returned error code " + firstProc.ExitCode.ToString());
                    }
                }
                catch (Exception ex)
                {
                    this.Logger.WriteError("An error occurred running Creo parametric: " + ex.Message + " - Extraction Failed. Insure you can access the Creo license server");
                    cleanup(tempXMLfile, true);
                    return;
                }
            }
            #endregion

            //  - Use a function from CyPhy2ComponentModel to convert the extractor's XML format into a CyPhy model fragment
            #region Convert_to_XML
            // used in creating the resource object below
            CyPhy.CADModel ProcessedCADModel = null;

            if (extractor_ran_success  && !test_copy_and_path_only)
            {
                this.Logger.WriteDebug("About to call DeserializeAvmComponentXml...");
                StreamReader streamReader = new StreamReader(tempXMLfile);
                avm.Component ac_import = CyPhyComponentImporter.CyPhyComponentImporterInterpreter.DeserializeAvmComponentXml(streamReader);
                streamReader.Close();
                this.Logger.WriteDebug("... finished DeserializeAvmComponentXml call.");

                foreach (var cadmodel in ac_import.DomainModel.Where(dm => dm is avm.cad.CADModel)
                                                                          .Cast<avm.cad.CADModel>())
                {
                    var rf = CyPhyClasses.RootFolder.GetRootFolder(CurrentProj);

                    Dictionary<string, CyPhy.Component> avmidComponentMap = new Dictionary<string, CyPhy.Component>();
                    AVM2CyPhyML.CyPhyMLComponentBuilder newComponent = new AVM2CyPhyML.CyPhyMLComponentBuilder(rf);
                    ProcessedCADModel = newComponent.process(cadmodel, GetCurrentComp());
                    ProcessedCADModel.Name = Path.GetFileNameWithoutExtension(cadFilename);
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

                // layout CAD model to the "south" of existing elements
                foreach (MgaPart item in (ProcessedCADModel.Impl as MgaFCO).Parts)
                {
                    item.SetGmeAttrs(null, CAD_MODEL_START_X, greatest_current_y + CAD_MODEL_START_Y);
                }

                // Extend it's properties out to the component level.
                this.CADModuleImportExtension(ProcessedCADModel);
            }
            else if (test_copy_and_path_only)
            {
                ProcessedCADModel = CyPhyClasses.CADModel.Create(GetCurrentComp());
                ProcessedCADModel.Name = Path.GetFileNameWithoutExtension(CADpath);
            }

            #endregion

            //  - Copy the Creo Model files into the component's backend folder
            //      - Note: The solution includes a function that can find this folder using the project.manifest.json file.
            //      - For nice organization, create them in a subfolder called "CAD"
            // create avmproj
            #region Copy files to backend folder

            // used in creating the resource object below
            string PathforComp = null;

            var importedCADFiles = new List<String>();
            if (extractor_ran_success)
            {
                try
                {
                    // create the destination path
                    PathforComp = META.ComponentLibraryManager.EnsureComponentFolder(GetCurrentComp());
                    PathforComp = GetCurrentComp().GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE);
                    
                    string finalPathName = Path.Combine(PathforComp, "CAD");

                    Directory.CreateDirectory(finalPathName);

                    // determine if one part file or all part and assembly files need to be copied
                    string cpsrcfile = System.IO.Path.GetFileName(cadFilename);

                    // copy the selected file
                    string cadFileCopyPath = System.IO.Path.Combine(finalPathName, cpsrcfile);
                    System.IO.File.Copy(cadFilename, cadFileCopyPath, true);

                    // Set "primary" file as the first in the list.
                    importedCADFiles.Add(Path.Combine("CAD", Path.GetFileName(cadFileCopyPath)));

                    if (fileISasm(cpsrcfile) && !test_copy_and_path_only)
                    {
                        // get a string of the XML contents
                        this.Logger.WriteDebug("About to read contents of XML file using class XmlDocument: " + tempXMLfile);

                        XmlDocument doc = new XmlDocument();
                        doc.Load(tempXMLfile);
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

                                // CAD files end in .1 .2 etc. Pick the latest ones
                                var allFiles = Directory.EnumerateFiles(Path.GetDirectoryName(res_path), "*prt." /*n.b. literal dot*/ + "*")
                                    .Concat(Directory.EnumerateFiles(Path.GetDirectoryName(res_path), "*asm.*"))
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
                                            if (!importedCADFiles.Contains(Path.Combine("CAD", Path.GetFileName(destpathandname))))
                                            {
                                                importedCADFiles.Add(Path.Combine("CAD", Path.GetFileName(destpathandname)));
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
                    cleanup(tempXMLfile, true);
                    return;
                }
            }
            #endregion

            //  - Create Resource objects in the CyPhy Component model that point to these Creo Model files
            //      - Note: The "paths" of these should be the relative path from that component's root folder
            if (extractor_ran_success)
            {
                foreach (var cadFile in importedCADFiles)
                {

                    CyPhy.Resource ResourceObj = CyPhyClasses.Resource.Create(GetCurrentComp());
                    ResourceObj.Attributes.ID = Guid.NewGuid().ToString("B");
                    ResourceObj.Attributes.Path = cadFile;
                    ResourceObj.Attributes.Notes = "CAD Model Import tool added this resource object for the imported CAD file";
                    ResourceObj.Name = Path.GetFileName(cadFile);

                    // layout Resource just to the side of the CAD model
                    foreach (MgaPart item in (ResourceObj.Impl as MgaFCO).Parts)
                    {
                        item.SetGmeAttrs(null, RESOURCE_START_X, greatest_current_y + RESOURCE_START_Y);
                    }

                    // The "primary" CAD model is the first one -- associate it with the CyPhy CADModel object
                    if (importedCADFiles.IndexOf(cadFile) == 0)
                    {
                        //  - Create a UsesResource association between the CyPhy CADModel object and the Resource object that represents the top-level Creo Model file.
                        CyPhyClasses.UsesResource.Connect(ResourceObj, ProcessedCADModel, null, null, GetCurrentComp());
                    }
                }
            }

            // throw in an ACM file for the current state of the component.
            if (extractor_ran_success && !test_copy_and_path_only)
            {
                var exporter = new CyPhyComponentExporter.CyPhyComponentExporterInterpreter();
                String acmPath = Path.Combine(PathforComp,GetCurrentComp().Name + ".component.acm");
                CyPhyComponentExporterInterpreter.ExportToFile(GetCurrentComp(), Path.GetDirectoryName(acmPath));
            }

            // Clean up
            cleanup(tempXMLfile, (extractor_ran_success && !test_copy_and_path_only));
        }

        // Clean up resources on all return paths
        void cleanup(string tempfile, bool close_dlg = false)
        {
            Close_Dlg = close_dlg;
            this.Logger.Dispose();
            File.Delete(tempfile);
        }

        // Find out if Creo is installed on this machine.
        //  The method we use is to look through the uninstall list in the registry, 
        //    since it has been verified that Creo updates this registry when it is
        //    installed
        bool CREO_present()
        {
            string registry_key = @"SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall";
            using (Microsoft.Win32.RegistryKey key = Registry.LocalMachine.OpenSubKey(registry_key))
            {
                foreach (string subkey_name in key.GetSubKeyNames())
                {
                    using (RegistryKey subkey = key.OpenSubKey(subkey_name))
                    {
                        try
                        {
                            string str = subkey.GetValue("DisplayName", "").ToString();

                            if (new string[] { "Creo Parametric", "PTC Creo Parametric" }.Any(name => str.StartsWith(name)))
                            {
                                return true;
                            }
                        }
                        catch (Exception)
                        {
                            // exceptions are thrown on every app in the registry without a proper display name
                            // we just want to skip these
                        }
                    }
                }
            }
            this.Logger.WriteError("CREO Parametric was NOT found on this Machine");
            return false;
        }

        // Display a file dialog box to select the CAD model file to import
        // parameter will contain the filename, returns true or false based on if a file is returned or not
        bool get_cad_file(out string FileName)
        {
            // Open file dialog box
            DialogResult dr;
            using (OpenFileDialog ofd = new OpenFileDialog())
            {
                ofd.CheckFileExists = true;
                ofd.DefaultExt = "test.mdl";
                ofd.Multiselect = false;
                ofd.Filter = "ASM and PRT files (*.asm,*prt)|*.asm*;*.prt*|ASM files (*.asm)|*.asm*|PRT files (*.prt)|*.prt*|All files (*.*)|*.*";
                dr = ofd.ShowDialog();
                if (dr == DialogResult.OK)
                {
                    FileName = ofd.FileName;
                    return true;
                }
            }
            FileName = "";
            this.Logger.WriteError("No file was selected.  CAD Import will not complete.");
            return false;
        }

        // determine if the creo file name is a part file or an assembly file
        // kind of tricky since creo adds a ".number" extension for revision control
        // returns true if the file is ASM, false if not (i.e. it is a PRT or unknown type)
        bool fileISasm(string sourcefile)
        {
            int locateASM = sourcefile.LastIndexOf(".asm", System.StringComparison.CurrentCultureIgnoreCase);
            int locatePRT = sourcefile.LastIndexOf(".prt", System.StringComparison.CurrentCultureIgnoreCase);

            if (locateASM > locatePRT)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        // META-2043 Extend CAD CAT module to create component-level Parameters for CAD Model's Parameters
        void CADModuleImportExtension(CyPhy.CADModel ProcessedCADModel)
        {
            // When the user imports a CAD Model, the current implementation creates a CyPhy CADModel object. After this point: 
            int numComponentParams = 0;
            foreach (CyPhy.CADParameter parm in ProcessedCADModel.Children.CADParameterCollection)
            {
                // - For each CADParameter object, create a corresponding Property object under the parent Component. 
                //   - Give the Property the same name 
                CyPhy.Property newprop = CyPhyClasses.Property.Create(GetCurrentComp());
                newprop.Name = parm.Name;
                if ( ! String.IsNullOrWhiteSpace(parm.Attributes.Value) )
                    newprop.Attributes.Value = parm.Attributes.Value;

                // - Create a CADParameterPortMap connection from the Property to the CADParameter 
                CyPhy.CADParameterPortMap ppmap = CyPhyClasses.CADParameterPortMap.Connect(newprop, parm);
                ppmap.Name = parm.Name;

                // - Perform some layout "niceification" on the resulting objects. 
                foreach (MgaPart item in (newprop.Impl as MgaFCO).Parts)
                {
                    item.SetGmeAttrs(null, PARAMETER_START_X, greatest_current_y + PARAMETER_START_Y + (numComponentParams * PARAMETER_ADJUST_Y));
                }
                numComponentParams++;
            }
        }
    }
}
