using System;
using System.IO;
using System.Windows.Forms;
using GME.CSharp;
using Exp = CyPhyComponentExporter;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

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
                RoleVal = CyPhyComponentAuthoringInterpreter.Role.Share,
                IconResourceKey = "export_component_folder",
                SupportedDesignEntityTypes = CyPhyComponentAuthoringInterpreter.SupportedDesignEntityType.Component
            )
        ]
        public void ExportComponentPackage(object sender, EventArgs e)
        {
            export_component_package();

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

        void export_component_package()
        {
            var component = (CyPhy.Component) this.GetCurrentDesignElement();

            this.Logger = new CyPhyGUIs.GMELogger(CurrentProj, this.GetType().Name);
            this.Logger.WriteDebug("Starting Component Export module...");

            // check for valid component and bail if there is not one
            try
            {
                string checkforinvalidcomponent = Path.GetDirectoryName(component.Impl.Project.ProjectConnStr.Substring("MGA=".Length));
            }
            catch(Exception)
            {
                this.Logger.WriteError("Error, No Current Component");
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

            // FIXME: this could throw an exception (but this code is currently unreachable)
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
