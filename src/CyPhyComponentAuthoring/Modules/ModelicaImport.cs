using System;
using System.Text;
using System.IO;
using System.Windows.Forms;
using GME.CSharp;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;

using CyPhyComponentAuthoring;


namespace CyPhyComponentAuthoring.Modules
{
    [CyPhyComponentAuthoringInterpreter.IsCATModule(ContainsCATmethod = true)]
    public class ModelicaImport : CATModule
    {
        public CyPhyGUIs.GMELogger Logger { get; set; }
        private bool Close_Dlg;

        [CyPhyComponentAuthoringInterpreter.CATName(
                NameVal = "Add Modelica",
                DescriptionVal = "An existing Modelica model gets imported and associated with this CyPhy Component.",
                RoleVal = CyPhyComponentAuthoringInterpreter.Role.Construct,
                IconResourceKey = "add_modelica",
                SupportedDesignEntityTypes = CyPhyComponentAuthoringInterpreter.SupportedDesignEntityType.Component
           )
        ]
        public void ImportModelicaModel(object sender, EventArgs e)
        {
            import_modelica_model();

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

        void import_modelica_model()
        {
            this.Logger = new CyPhyGUIs.GMELogger(CurrentProj, this.GetType().Name);
            this.Logger.WriteDebug("Starting Import Modelica Model module...");

            var component = this.GetCurrentDesignElement();

            Type type = Type.GetTypeFromProgID("MGA.Interpreter.ModelicaImporter");
            GME.MGA.IMgaComponentEx modelicaImporter = Activator.CreateInstance(type) as GME.MGA.IMgaComponentEx;

            modelicaImporter.Initialize(component.Impl.Project);
            var selectedFCOs = (GME.MGA.MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
            modelicaImporter.InvokeEx(component.Impl.Project, component.Impl as GME.MGA.MgaFCO, selectedFCOs, 0);

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
