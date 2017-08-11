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
using Microsoft.VisualBasic;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using META;

namespace CyPhyComponentAuthoring.Modules
{
    [CyPhyComponentAuthoringInterpreter.IsCATModule(ContainsCATmethod = true)]
    public class CADFileRename : CATModule
    {
        public CyPhyGUIs.GMELogger Logger { get; set; }
        private bool Close_Dlg;

        [CyPhyComponentAuthoringInterpreter.CATName(
            NameVal = "Rename CAD File",
            DescriptionVal = "Rename a previously imported Creo CAD model in this CyPhy Component.",
            RoleVal = CyPhyComponentAuthoringInterpreter.Role.Modify
            )
        ]
        public void callRenameCADFile(object sender, EventArgs e)
        {
            RenameCADFile();

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
                // the dialog box is what we want to close
                parentDB.Close();
            }
        }

        public void RenameCADFile(string CADpath = null, string NewName = null)
        {
            string StartingCadFilename = "";
            string CadFilenamePath = "";
            string cad_extension = "";
            string RenamedCadFilename = "";

            this.Logger = new CyPhyGUIs.GMELogger(CurrentProj, this.GetType().Name);

            // Get the file name desirous of changing
            #region get_name
            //  - Display a dialog box to let the user choose their Creo model file
            bool cad_file_chosen = false;
            bool test_mode_only = false;
            if (string.IsNullOrWhiteSpace(CADpath))
            {
                cad_file_chosen = get_cad_file(out StartingCadFilename);
            }
            else
            {
                test_mode_only = true;
                StartingCadFilename = CADpath.Replace("/", "\\");
                if (File.Exists(StartingCadFilename))
                {
                    cad_file_chosen = true;
                }
                else
                {
                    if (!test_mode_only)
                    {
                        this.Logger.WriteError("Invalid CAD file path passed in: " + CADpath);
                    }
                }
            }
            #endregion

            // check chosen file constraints
            #region check_constraints
            if (cad_file_chosen)
            {
                // -    file chosen is in the component folder structure
                string must_be_in_dir = GetCurrentComp().GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE).Replace("/", "\\");
                if (!StartingCadFilename.Contains(must_be_in_dir))
                {
                    if (!test_mode_only)
                    {
                        this.Logger.WriteError("File to rename must reside in current component's folder:" + must_be_in_dir);
                    }
                    //cleanup
                    cleanup(false);
                    return;
                }
                // -    file chosen is a CREO .prt or .asm file
                if (!(StartingCadFilename.Contains(".prt") || StartingCadFilename.Contains(".asm")))
                {
                    if (!test_mode_only)
                    {
                        this.Logger.WriteError("File to rename must be a CAD .prt or .asm file only");
                    }
                    //cleanup
                    cleanup(false);
                    return;
                }
            }
            #endregion

            // Get new desired name
            #region get_new_name
            if (cad_file_chosen)
            {
                string new_file_name = "";

                if (test_mode_only)
                {
                    new_file_name = NewName;
                }
                else
                {
                    new_file_name = GetNewFilename();
                }

                if (!String.IsNullOrEmpty(new_file_name))
                {
                    // massage new name
                    CadFilenamePath = Path.GetDirectoryName(StartingCadFilename);
                    //  -   strip off the CREO version extension
                    string just_the_name = Path.GetFileNameWithoutExtension(new_file_name);
                    // -    retain the original extension type (.prt or .asm)
                    cad_extension = FileExtensionType(StartingCadFilename);

                    // construct the new file name
                    RenamedCadFilename = Path.Combine(CadFilenamePath, just_the_name) + cad_extension + ".1";
                    // -    verify chosen name does not alreay exist
                    if (File.Exists(RenamedCadFilename))
                    {
                        if (!test_mode_only)
                        {
                            this.Logger.WriteError("Chosen new filename already exists: ", RenamedCadFilename);
                        }
                        //cleanup
                        cleanup(false);
                        return;
                    }
                }
                else
                {
                    if (!test_mode_only)
                    {
                        this.Logger.WriteError("Chosen new filename is invalid or null: ", RenamedCadFilename);
                    }
                    //cleanup
                    cleanup(false);
                    return;
                }
            }
            #endregion

            // change model and resource names and path to match new name
            if (cad_file_chosen)
            {
                // Step 1 - Look for a Resource object that has a "Path" attribute that matches the "old path". 
                //- Does the file path chosen match the "Path" attribute of a Resource? 
                //  - If not, quit. 
                CyPhy.Resource ResourceObj = null;
                var resourcePath = ComponentLibraryManager.MakeRelativePath(GetCurrentComp().GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE),
                        AVM2CyPhyML.CyPhyMLComponentBuilder.GetCreoFileWithoutVersion(StartingCadFilename));
                // n.b. current dir doesn't matter, we just want to canonicalize .. . / et al
                resourcePath = Path.GetFullPath(resourcePath);
                try
                {
                    ResourceObj = GetCurrentComp().Children.ResourceCollection
                        .Where(p => Path.GetFullPath(AVM2CyPhyML.CyPhyMLComponentBuilder.GetCreoFileWithoutVersion(p.Attributes.Path))
                            == resourcePath).First();
                }
                catch (InvalidOperationException ex)
                {
                    if (!test_mode_only)
                    {
                        this.Logger.WriteError("No resource found with that CAD file path", ComponentLibraryManager.MakeRelativePath(GetCurrentComp().GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE), StartingCadFilename), ex.Message);
                    }
                    cleanup(false);
                    return;
                }

                // Step 2 - Check if the Resource is attached to a CADModel. 
                //  - If not, quit. 
                bool ResourceConnected2Model = true;
                // first check if Resource is Source, Model is Destination
                try
                {
                    ResourceConnected2Model = ResourceObj.DstConnections.UsesResourceCollection.Where(c => c.DstEnds.CADModel != null).Any();
                }
                catch (Exception ex)
                {
                    ResourceConnected2Model = false;
                    if (!test_mode_only)
                    {
                        this.Logger.WriteError("No connection from this Resource to a CAD model", ex.Message);
                    }
                }
                // check opposite direction if connection not found
                if (!ResourceConnected2Model)
                {
                    // next check if Resource is Destination, Model is Source
                    try
                    {
                        ResourceConnected2Model = ResourceObj.SrcConnections.UsesResourceCollection.Where(c => c.SrcEnds.CADModel != null).Any();
                        ResourceConnected2Model = true;
                    }
                    catch (Exception ex)
                    {
                        if (!test_mode_only)
                        {
                            this.Logger.WriteError("No connection from this Resource to a CAD model", ex.Message);
                        }
                        cleanup(true);
                        return;
                    }
                }

                // Step 3 - rename the file in the folder
                try
                {
                    //TODO commented out for debug                    
                    File.Move(StartingCadFilename, RenamedCadFilename);
                }
                catch (Exception ex)
                {
                    // the rename failed.  bail out
                    this.Logger.WriteError("Error renaming CAD file:", Path.GetFileName(StartingCadFilename), ex.Message);
                    cleanup(true);
                    return;
                }

                // Step 4 - Change that "Path" attribute to the new name. 
                // change the resource name and path
                // Path name needs to be relative to component folder
                string new_path = ComponentLibraryManager.MakeRelativePath(GetCurrentComp().GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE),
                    AVM2CyPhyML.CyPhyMLComponentBuilder.GetCreoFileWithoutVersion(RenamedCadFilename));
                ResourceObj.Attributes.Path = new_path;

                // Step 5 - If the Resource name happens to be the filename, change it too. 
                if (AVM2CyPhyML.CyPhyMLComponentBuilder.GetCreoFileWithoutVersion(ResourceObj.Name)
                    == Path.GetFileName(AVM2CyPhyML.CyPhyMLComponentBuilder.GetCreoFileWithoutVersion(StartingCadFilename)))
                {
                    ResourceObj.Name = Path.GetFileName(AVM2CyPhyML.CyPhyMLComponentBuilder.GetCreoFileWithoutVersion(RenamedCadFilename));
                }

                // Step 6 - If the CAD Model name happens to be the filename, change it too. 
                // change the model name
                CyPhy.CADModel ModelObj = null;
                bool ModelObjExists = true;
                var modelObjName = AVM2CyPhyML.CyPhyMLComponentBuilder.GetCreoFileWithoutVersion(Path.GetFileName(StartingCadFilename));
                try
                {
                    // FIXME: check that it is connected to ResourceObj
                    ModelObj = GetCurrentComp().Children.CADModelCollection.Where(p => p.Name == modelObjName).First();
                }
                // check if ModelObj exists
                catch (InvalidOperationException)
                {
                    ModelObjExists = false;
                    if (!test_mode_only)
                    {
                        // this doesn't matter, don't confuse the user
                        // this.Logger.WriteWarning("Cannot rename CADModel, because there is no CADModel named '{0}'", modelObjName, ex.Message);
                    }
                }
                if (ModelObjExists)
                {
                    ModelObj.Name = AVM2CyPhyML.CyPhyMLComponentBuilder.GetCreoFileWithoutVersion(Path.GetFileName(RenamedCadFilename));
                }
            }
            //cleanup
            cleanup(cad_file_chosen && !test_mode_only);
            return;
        }

        // clean up loose ends on leaving this module
        void cleanup(bool close_dlg = false)
        {
            Close_Dlg = close_dlg;
            this.Logger.Dispose();
        }

        // Display a file dialog box to select the CAD model file to rename
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
                // Explorer doesn't like paths with mixed seperators, make them all the same
                string initial_directory = GetCurrentComp().GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE).Replace("/", "\\");
                ofd.InitialDirectory = initial_directory;
                ofd.Filter = "ASM and PRT files (*.asm,*prt)|*.asm*;*.prt*|ASM files (*.asm)|*.asm*|PRT files (*.prt)|*.prt*|All files (*.*)|*.*";
                dr = ofd.ShowDialog();
                if (dr == DialogResult.OK)
                {
                    FileName = ofd.FileName;
                    return true;
                }
            }
            FileName = "";
            this.Logger.WriteError("No file was selected.  Rename CAD File will not complete.");
            return false;
        }

        // Opens a dialog box for the user to select a new file name
        string GetNewFilename()
        {
            string retval = Microsoft.VisualBasic.Interaction.InputBox("Enter the new file name (without extension)", "Enter File Name", "new_file_name");
            return retval;
        }

        // determine if the creo file name is a part file or an assembly file
        // kind of tricky since creo adds a ".number" extension for revision control
        // returns ".asm" if the file is ASM, ".prt" if not (i.e. it is a PRT or unknown type)
        string FileExtensionType(string sourcefile)
        {
            int locateASM = sourcefile.LastIndexOf(".asm", System.StringComparison.CurrentCultureIgnoreCase);
            int locatePRT = sourcefile.LastIndexOf(".prt", System.StringComparison.CurrentCultureIgnoreCase);
            string retval = ".prt";

            if (locateASM > locatePRT)
            {
                retval = ".asm";
            }
            return retval;
        }

    }
}
