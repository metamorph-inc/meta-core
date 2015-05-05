using System;
using System.IO;
using System.Windows.Forms;
using GME.CSharp;
using Exp = CyPhyComponentExporter;

namespace CyPhyComponentAuthoring.Modules
{
    [CyPhyComponentAuthoringInterpreter.IsCATModule(ContainsCATmethod = true)]
    public class ComponentExport : CATModule
    {
        public CyPhyGUIs.GMELogger Logger { get; set; }
        private bool Close_Dlg;

        [CyPhyComponentAuthoringInterpreter.CATName(
            NameVal = "Export Component Package",
            DescriptionVal = "Export an AVM Component Package in ZIP format, which will include any of this model's dependent artifacts.",
            RoleVal = CyPhyComponentAuthoringInterpreter.Role.Share
            )
        ]
        public void ExportComponentPackage(object sender, EventArgs e)
        {
            export_component_package();

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

        void export_component_package()
        {
            var component = this.GetCurrentComp();

            this.Logger = new CyPhyGUIs.GMELogger(CurrentProj, this.GetType().Name);
            this.Logger.WriteDebug("Starting Component Export module...");

            // check for valid component and bail if there is not one
            try
            {
                string checkforinvalidcomponent = Path.GetDirectoryName(component.Impl.Project.ProjectConnStr.Substring("MGA=".Length));
            }
            catch(Exception)
            {
                this.Logger.WriteError("Error, No Current Componet");
                cleanup(true);
                return;
            }

            #region Prompt for Output Path
            string startupPath = Path.GetDirectoryName(component.Impl.Project.ProjectConnStr.Substring("MGA=".Length));

            // Get an output path from the user.
            String s_outPath;
            using (META.FolderBrowserDialog fbd = new META.FolderBrowserDialog()
            {
                Description = "Choose a path for the generated files.",
                //ShowNewFolderButton = true,
                SelectedPath = startupPath,
            })
            {

                DialogResult dr = fbd.ShowDialog();
                if (dr == DialogResult.OK)
                {
                    s_outPath = fbd.SelectedPath;
                }
                else
                {
                    this.Logger.WriteWarning("Component Exporter cancelled");
                    cleanup(false);
                    return;
                }
            }
            #endregion

            var pkgPath = Exp.CyPhyComponentExporterInterpreter.ExportComponentPackage(component, s_outPath);
            this.Logger.WriteInfo("Component package created: {0}", pkgPath);
            cleanup(true);
        }

        // Clean up resources on all return paths
        void cleanup(bool close_dlg = true)
        {
            Close_Dlg = close_dlg;
            this.Logger.Dispose();
        }
    }
}
