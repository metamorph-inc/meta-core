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
using CyPhyComponentAuthoring;

namespace CyPhyComponentAuthoring.Modules
{
    [CyPhyComponentAuthoringInterpreter.IsCATModule(ContainsCATmethod = true)]
    public class CustomIconAdd : CATModule
    {
        public CyPhyGUIs.GMELogger Logger { get; set; }
        private bool Close_Dlg;

        [CyPhyComponentAuthoringInterpreter.CATName(
            NameVal = "Add Icon",
            DescriptionVal = "An custom icon is imported from a file into a resource object in this CyPhy Component and set as the icon.",
            RoleVal = CyPhyComponentAuthoringInterpreter.Role.Construct
            )
        ]
        public void callAddCustomIcon(object sender, EventArgs e)
        {
            AddCustomIcon();

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

        public void AddCustomIcon(string IconFileSourcePath = null)
        {
            this.Logger = new CyPhyGUIs.GMELogger(CurrentProj, this.GetType().Name);
            string IconFileSourceName = "";

            //            To support this, Components can have custom icons. A Component's custom icon must: 
            // - be in the Component's folder 
            // - have a Resource reference to the file (with path relative to the component folder itself) 
            // - have a path that includes "Icon.png" ("myIcon.png", "images/theIcon.png", "Icon.png", etc) 
            // You can take a look at a Component's custom icon by opening a ComponentAssembly and creating a reference to the Component. 

            // The goal of this ticket is to allow the user to add a custom icon for the component via the CAT. The user should be allowed to browse for a *.png file. 
            #region choose_icon_file
            //  - Display a dialog box to let the user choose the Custom Icon file
            bool icon_file_chosen = false;
            bool test_mode = false;
            if (string.IsNullOrWhiteSpace(IconFileSourcePath))
            {
                icon_file_chosen = get_icon_file(out IconFileSourceName);
            }
            else
            {
                test_mode = true;
                IconFileSourceName = IconFileSourcePath;
                if (File.Exists(IconFileSourceName))
                {
                    icon_file_chosen = true;
                }
                else
                {
                    this.Logger.WriteError("Invalid Custom Icon file path passed in: " + IconFileSourcePath);
                }
            }
            #endregion

            // When that file is selected: - it should be copied into the component's folder, and have its name changed to "Icon.png". 
            #region Copy files to backend folder

            // used in creating the resource object below
            string IconFileDestPath = null;
            string IconFileDestName = "";

            if (icon_file_chosen)
            {
                try
                {
                    // Find the path of the current component
                    IconFileDestPath = META.ComponentLibraryManager.EnsureComponentFolder(GetCurrentComp());
                    IconFileDestPath = GetCurrentComp().GetDirectoryPath(ComponentLibraryManager.PathConvention.ABSOLUTE);

                    // copy the selected file
                    IconFileDestName = System.IO.Path.Combine(IconFileDestPath, "Icon.png");
                    System.IO.File.Copy(IconFileSourceName, IconFileDestName, true);
                }
                catch (Exception err_copy_icon_file)
                {
                    this.Logger.WriteError("Error copying Icon file" + err_copy_icon_file.Message);
                    clean_up(false);
                    return;
                }
            }
            #endregion

            //- A Resource object should be created in the CyPhy Component which points to the file. 
            #region create_resource
            if (icon_file_chosen)
            {
                CyPhy.Resource ResourceObj = CyPhyClasses.Resource.Create(GetCurrentComp());
                ResourceObj.Attributes.ID = Guid.NewGuid().ToString("B");
                ResourceObj.Attributes.Path = "Icon.png";
                ResourceObj.Attributes.Notes = "Custom icon for this component";
                ResourceObj.Name = Path.GetFileName(IconFileDestName);

                String iconPath_RelativeToProjRoot = Path.Combine(GetCurrentComp().GetDirectoryPath(ComponentLibraryManager.PathConvention.REL_TO_PROJ_ROOT),
                                                                  "Icon.png");

                //- Finally, it must be set as the CyPhy Component's icon
                try
                {
                    (GetCurrentComp().Impl as GME.MGA.IMgaFCO).set_RegistryValue("icon", iconPath_RelativeToProjRoot);
                }
                catch (Exception err_set_registry)
                {
                    this.Logger.WriteError("Error setting Icon Registry Value" + err_set_registry.Message);
                    clean_up(false);
                    return;
                }
            }
            #endregion

            clean_up( icon_file_chosen && !test_mode );
        }

        // Display a file dialog box to select the Custom Icon file to import
        // parameter will contain the filename, returns true or false based on if a file is returned or not
        bool get_icon_file(out string FileName)
        {
            // Open file dialog box
            DialogResult dr;
            using (OpenFileDialog ofd = new OpenFileDialog())
            {
                ofd.CheckFileExists = true;
                ofd.DefaultExt = "test.png";
                ofd.Multiselect = false;
                ofd.Filter = "PNG files (*.png)|*.png*|All files (*.*)|*.*";
                dr = ofd.ShowDialog();
                if (dr == DialogResult.OK)
                {
                    FileName = ofd.FileName;
                    return true;
                }
            }
            FileName = "";
            this.Logger.WriteError("No file was selected.  Add Custom Icon will not complete.");
            return false;
        }

        // clean up loose ends on leaving this module
        void clean_up(bool close_dlg)
        {
            Close_Dlg = close_dlg;
            this.Logger.Dispose();
        }
    }
}
