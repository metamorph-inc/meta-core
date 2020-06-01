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
using CyPhyGUIs;
using System.Text;
using System.Text.RegularExpressions;
using AVM2CyPhyML;

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
        private object sender;

        private bool cadImportProgressDialogCancelled;

        [CyPhyComponentAuthoringInterpreter.CATName(
                NameVal = "Add CAD",
                DescriptionVal = "Import an existing Creo CAD model and associate it with this CyPhy Component.  You must have Creo installed on this machine to convert the model file.",
                RoleVal = CyPhyComponentAuthoringInterpreter.Role.Construct,
                IconResourceKey = "add_cad",
                SupportedDesignEntityTypes = CyPhyComponentAuthoringInterpreter.SupportedDesignEntityType.Component
            )
        ]
        public void callImportCADModel(object sender, EventArgs e)
        {
            this.sender = sender;
            ImportCADModel();

            // Close the calling dialog box if the module ran successfully
            if (Close_Dlg)
            {
                if (sender is Form)
                {
                    // the TLP is in the dialog box
                    Form parentDB = (Form)sender;
                    parentDB.Close();
                }
            }
        }

        public void ImportCADModel(string CADpath = null)
        {
            string cadFilename = "";
            this.Logger = new CyPhyGUIs.GMELogger(CurrentProj, this.GetType().Name);
            using (this.Logger)
            {
                //The CAT module will perform these steps, in this order:
                //  - Check that the user has Creo (so that the extraction will be successful)
                //      - Implementation Suggestion: The extraction utility may have special flag to have it indicates if all the dependencies are met

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

                if (cad_file_chosen && !test_copy_and_path_only)
                {
                    try
                    {
                        // assemble the arg string
                        // first the input CAD file
                        // include quotation marks to handle paths with white spaces
                        string argstring = "ExtractACM-XMLfromCreoModels -c \"";
                        argstring += cadFilename;
                        argstring += "\"";

                        // add the XML output file name
                        argstring += " -x \"";
                        argstring += tempXMLfile;
                        argstring += "\"";

                        // Debug only: pause before exit, graphics mode.
                        //argstring += " -p -g";

                        Process firstProc = new Process();

                        string path = Path.Combine(META.VersionInfo.MetaPath, "bin\\CAD\\Creo\\bin\\CADCreoParametricCreateAssembly.exe");
                        if (!File.Exists(path))
                        {
                            this.Logger.WriteError(String.Format("Cannot find '{0}'", path));
                            throw new Exception("CADCreoParametricCreateAssembly.exe not found.");
                        }

                        firstProc.StartInfo.FileName = path;
                        firstProc.StartInfo.Arguments = argstring;
                        firstProc.StartInfo.CreateNoWindow = true;
                        firstProc.StartInfo.UseShellExecute = false;
                        firstProc.StartInfo.RedirectStandardOutput = true;
                        firstProc.StartInfo.RedirectStandardError = true;
                        firstProc.StartInfo.RedirectStandardInput = true;

                        int streamsClosed = 0;
                        StringBuilder exeConsoleOutput = new StringBuilder();

                        object cancelledLock = new Object();
                        cadImportProgressDialogCancelled = false;

                        DataReceivedEventHandler handler = (sender, e) =>
                        {
                            lock (cancelledLock)
                            {
                                if (!cadImportProgressDialogCancelled)
                                {
                                    lock (Logger)
                                    {
                                        if (e.Data == null)
                                        {
                                            streamsClosed += 1;
                                            if (streamsClosed == 2)
                                            {
                                            }
                                            return;
                                        }
                                        Logger.WriteDebug(e.Data);
                                        exeConsoleOutput.AppendLine(e.Data);
                                    }
                                }
                            }
                        };
                        firstProc.OutputDataReceived += handler;
                        firstProc.ErrorDataReceived += handler;
                        firstProc.EnableRaisingEvents = true;
                        bool showProgressBar = sender != null && sender is IWin32Window;
                        bool done = false;
                        Object doneSyncObject = new object();
                        GUIs.CADProgress progress = null;
                        EventHandler closeDialogWithSuccess = (e, o) =>
                        {
                            lock (doneSyncObject)
                            {
                                if (done == false)
                                {
                                    progress.BeginInvoke((Action)(() =>
                                    {
                                        done = true;
                                        progress.DialogResult = DialogResult.OK;
                                        progress.Close();
                                    }));
                                }
                            }
                        };
                        if (showProgressBar)
                        {
                            progress = new GUIs.CADProgress();
                            firstProc.Exited += closeDialogWithSuccess;
                        }
                        IntPtr job = JobObjectPinvoke.CreateKillOnCloseJob();
                        firstProc.Exited += (s, o) =>
                        {
                            if (job != JobObjectPinvoke.INVALID_HANDLE_VALUE)
                            {
                                JobObjectPinvoke.CloseHandle(job);
                            }
                            if (progress != null && progress.DialogResult == DialogResult.Cancel)
                            {
                                File.Delete(tempXMLfile);
                            }
                        };

                        this.Logger.WriteDebug("Calling CADCreoParametricCreateAssembly.exe with argument string: " + argstring);

                        firstProc.Start();
                        JobObjectPinvoke.AssignProcessToJobObject(firstProc, job);
                        firstProc.StandardInput.Close();
                        firstProc.BeginOutputReadLine();
                        firstProc.BeginErrorReadLine();

                        if (showProgressBar)
                        {
                            progress.FormClosing += (e, o) =>
                            {
                                lock (doneSyncObject)
                                {
                                    firstProc.Exited -= closeDialogWithSuccess;
                                    done = true;
                                }
                            };
                            var result = progress.ShowDialog((IWin32Window)sender);
                            if (result == DialogResult.Cancel)
                            {
                                lock (cancelledLock)
                                {
                                    cadImportProgressDialogCancelled = true;
                                }
                                this.Logger.WriteInfo("CAD model import cancelled by user.");
                                cleanup(null, true);
                                return;
                            }
                        }
                        else
                        {
                            firstProc.WaitForExit();
                        }

                        this.Logger.WriteDebug("CADCreoParametricCreateAssembly.exe ExtractACM-XMLfromCreoModels has completed.");

                        if (firstProc.ExitCode == 0)
                        {
                            extractor_ran_success = true;
                        }
                        else
                        {
                            this.Logger.WriteDebug("CADCreoParametricCreateAssembly.exe ExtractACM-XMLfromCreoModels returned error code " + firstProc.ExitCode.ToString());
                            // Warn with last non-empty line, it should contain a useful error message
                            Regex lastLine = new Regex("[\\n^]([^\\n]+)(\\n|\\s)*$", RegexOptions.Singleline);
                            var match = lastLine.Match(exeConsoleOutput.ToString());
                            if (match != null && match.Success)
                            {
                                this.Logger.WriteWarning(match.Groups[1].Value);
                            }
                            throw new Exception("Extract executable returned error code " + firstProc.ExitCode.ToString() + ". Detailed log at " + this.Logger.LogFilenames.FirstOrDefault());
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

                if (extractor_ran_success && !test_copy_and_path_only)
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

                        AVM2CyPhyML.CyPhyMLComponentBuilder newComponent = new AVM2CyPhyML.CyPhyMLComponentBuilder(rf);
                        ProcessedCADModel = newComponent.process(cadmodel, (CyPhy.Component)GetCurrentDesignElement());
                        ProcessedCADModel.Name = Path.GetFileName(AVM2CyPhyML.CyPhyMLComponentBuilder.GetCreoFileWithoutVersion(cadFilename));
                    }

                    // find the largest current Y value so our new elements are added below the existing design elements
                    foreach (var child in GetCurrentDesignElement().AllChildren)
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
                    ProcessedCADModel = CyPhyClasses.CADModel.Create((CyPhy.Component)GetCurrentDesignElement());
                    ProcessedCADModel.Name = Path.GetFileName(AVM2CyPhyML.CyPhyMLComponentBuilder.GetCreoFileWithoutVersion(CADpath));
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
                        PathforComp = META.ComponentLibraryManager.EnsureComponentFolder((CyPhy.Component)GetCurrentDesignElement());
                        PathforComp = ((CyPhy.Component)GetCurrentDesignElement()).GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE);

                        string finalPathName = Path.Combine(PathforComp, "CAD");

                        Directory.CreateDirectory(finalPathName);

                        // determine if one part file or all part and assembly files need to be copied
                        string cpsrcfile = System.IO.Path.GetFileName(cadFilename);

                        // copy the selected file
                        string cadFileCopyPath = System.IO.Path.Combine(finalPathName, cpsrcfile);
                        if (Path.GetFullPath(cadFilename) != Path.GetFullPath(cadFileCopyPath))
                        {
                            System.IO.File.Copy(cadFilename, cadFileCopyPath, true);
                        }

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
                                    string res_path = reader.GetAttribute("Path");
                                    this.Logger.WriteDebug("Copying this file: " + res_path);

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
                                            if (latestFilename.ToUpperInvariant().StartsWith(Path.GetFileName(res_path).ToUpperInvariant()))
                                            {
                                                string destpathandname = Path.Combine(finalPathName, latestFilename);
                                                if (!importedCADFiles.Contains(Path.Combine("CAD", Path.GetFileName(destpathandname))))
                                                {
                                                    importedCADFiles.Add(Path.Combine("CAD", Path.GetFileName(destpathandname)));
                                                }
                                                var sourcepathandname = Path.Combine(Path.GetDirectoryName(res_path), latestFilename);
                                                if (Path.GetFullPath(sourcepathandname) != Path.GetFullPath(destpathandname))
                                                {
                                                    File.Copy(sourcepathandname, destpathandname, true);
                                                }
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

                        CyPhy.Resource ResourceObj = CyPhyClasses.Resource.Create((CyPhy.Component)GetCurrentDesignElement());
                        ResourceObj.Attributes.ID = Guid.NewGuid().ToString("B");
                        ResourceObj.Attributes.Path = AVM2CyPhyML.CyPhyMLComponentBuilder.GetCreoFileWithoutVersion(cadFile);
                        ResourceObj.Attributes.Notes = "CAD Model Import tool added this resource object for the imported CAD file";
                        ResourceObj.Name = Path.GetFileName(AVM2CyPhyML.CyPhyMLComponentBuilder.GetCreoFileWithoutVersion(cadFile));

                        // layout Resource just to the side of the CAD model
                        foreach (MgaPart item in (ResourceObj.Impl as MgaFCO).Parts)
                        {
                            item.SetGmeAttrs(null, RESOURCE_START_X, greatest_current_y + RESOURCE_START_Y);
                        }

                    // The "primary" CAD model is the first one -- associate it with the CyPhy CADModel object
                    if (importedCADFiles.IndexOf(cadFile) == 0)
                    {
                        //  - Create a UsesResource association between the CyPhy CADModel object and the Resource object that represents the top-level Creo Model file.
                        CyPhyClasses.UsesResource.Connect(ResourceObj, ProcessedCADModel, null, null, (CyPhy.Component) GetCurrentDesignElement());
                        }
                    }
                }

            // throw in an ACM file for the current state of the component.
            if (extractor_ran_success && !test_copy_and_path_only)
            {
                var exporter = new CyPhyComponentExporter.CyPhyComponentExporterInterpreter();
                String acmPath = Path.Combine(PathforComp,GetCurrentDesignElement().Name + ".component.acm");
                CyPhyComponentExporterInterpreter.ExportToFile((CyPhy.Component) GetCurrentDesignElement(), Path.GetDirectoryName(acmPath));
            }

                // Clean up
                cleanup(tempXMLfile, (extractor_ran_success && !test_copy_and_path_only));
            }
        }

        // Clean up resources on all return paths
        void cleanup(string tempfile, bool close_dlg = false)
        {
            Close_Dlg = close_dlg;
            if (tempfile != null)
            {
                File.Delete(tempfile);
            }
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

        CyPhyUnitMap map;

        // META-2043 Extend CAD CAT module to create component-level Parameters for CAD Model's Parameters
        void CADModuleImportExtension(CyPhy.CADModel ProcessedCADModel)
        {
            // When the user imports a CAD Model, the current implementation creates a CyPhy CADModel object. After this point: 
            int numComponentParams = 0;
            foreach (CyPhy.CADParameter parm in ProcessedCADModel.Children.CADParameterCollection)
            {
                // - For each CADParameter object, create a corresponding Property object under the parent Component. 
                //   - Give the Property the same name 
                CyPhy.Property newprop = CyPhyClasses.Property.Create((CyPhy.Component)GetCurrentDesignElement());
                newprop.Name = parm.Name;
                if (!String.IsNullOrWhiteSpace(parm.Attributes.Value))
                {
                    newprop.Attributes.Value = parm.Attributes.Value;
                }

                if (String.IsNullOrWhiteSpace(parm.Attributes.Unit) == false)
                {
                    if (map == null)
                    {
                        map = new CyPhyUnitMap();
                        map._cyPhyMLRootFolder = CyPhyClasses.RootFolder.GetRootFolder(ProcessedCADModel.Impl.Project);
                        map.init(true);
                    }
                    CyPhy.unit unit;
                    if (map._unitSymbolCyPhyMLUnitMap.TryGetValue(parm.Attributes.Unit, out unit))
                    {
                        newprop.Referred.unit = unit;
                    }
                }

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
