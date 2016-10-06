using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using GME.CSharp;
using GME;
using GME.MGA;
using GME.MGA.Core;
using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using System.Windows.Forms;
using CyPhyComponentImporter;
using ISIS.GME.Common.Interfaces;


namespace CyPhyDesignImporter
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyPhyDesignImporterInterpreter : IMgaComponentEx, IGMEVersionInfo
    {
        /// <summary>
        /// Contains information about the GUI event that initiated the invocation.
        /// </summary>
        public enum ComponentStartMode
        {
            GME_MAIN_START = 0, 		// Not used by GME
            GME_BROWSER_START = 1,      // Right click in the GME Tree Browser window
            GME_CONTEXT_START = 2,		// Using the context menu by right clicking a model element in the GME modeling window
            GME_EMBEDDED_START = 3,		// Not used by GME
            GME_MENU_START = 16,		// Clicking on the toolbar icon, or using the main menu
            GME_BGCONTEXT_START = 18,	// Using the context menu by right clicking the background of the GME modeling window
            GME_ICON_START = 32,		// Not used by GME
            GME_SILENT_MODE = 128 		// Not used by GME, available to testers not using GME
        }

        /// <summary>
        /// This function is called for each interpreter invocation before Main.
        /// Don't perform MGA operations here unless you open a transaction.
        /// </summary>
        /// <param name="project">The handle of the project opened in GME, for which the interpreter was called.</param>
        public void Initialize(MgaProject project)
        {
        }

        /// <summary>
        /// The main entry point of the interpreter.
        /// GMEConsole is available. A general try-catch block catches all the exceptions
        /// coming from this function, you don't need to add it. For more information, see InvokeEx.
        /// </summary>
        /// <param name="project">The handle of the project opened in GME, for which the interpreter was called.</param>
        /// <param name="currentobj">The model open in the active tab in GME. Its value is null if no model is open (no GME modeling windows open). </param>
        /// <param name="selectedobjs">
        /// A collection for the selected model elements. It is never null.
        /// If the interpreter is invoked by the context menu of the GME Tree Browser, then the selected items in the tree browser. Folders
        /// are never passed (they are not FCOs).
        /// If the interpreter is invoked by clicking on the toolbar icon or the context menu of the modeling window, then the selected items 
        /// in the active GME modeling window. If nothing is selected, the collection is empty (contains zero elements).
        /// </param>
        /// <param name="startMode">Contains information about the GUI event that initiated the invocation.</param>
        [ComVisible(false)]
        public void Main(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, ComponentStartMode startMode)
        {
            string[] fileNames = null;
            DialogResult dr;
            using (OpenFileDialog ofd = new OpenFileDialog())
            {
                ofd.CheckFileExists = true;
                ofd.DefaultExt = "design.adm";
                ofd.Multiselect = true;
                ofd.Filter = "AVM design files (*.adm)|*.adm|All files (*.*)|*.*";
                ofd.RestoreDirectory = true;
                if (project.ProjectConnStr.StartsWith("MGA=", true, System.Globalization.CultureInfo.InvariantCulture))
                {
                    ofd.InitialDirectory = Path.GetDirectoryName(project.ProjectConnStr.Substring("MGA=".Length));
                }

                dr = ofd.ShowDialog();
                if (dr == DialogResult.OK)
                {
                    fileNames = ofd.FileNames;
                }
            }
            if (dr == DialogResult.OK)
            {
                Model[] result = null;
                MgaGateway.PerformInTransaction(delegate
                {
                    var importer = new AVMDesignImporter(GMEConsole, project);
                    result  = importer.ImportFiles(fileNames, mode: AVMDesignImporter.DesignImportMode.CREATE_CA_IF_NO_DS_CONCEPTS);
                }, transactiontype_enum.TRANSACTION_NON_NESTED, abort: false);

                if (result.Length > 0 && GMEConsole.gme != null)
                {
                    GMEConsole.gme.ShowFCO((MgaFCO)result[0].Impl);
                }
                return;
            }
            else
            {
                GMEConsole.Warning.WriteLine("Design Importer canceled");
                return;
            }
        }


        public static avm.Design DeserializeAvmDesignXml(TextReader reader)
        {
            return XSD2CSharp.AvmXmlSerializer.Deserialize<avm.Design>(reader);
        }

        public IMgaFCO ImportDesign(IMgaProject project, string filename)
        {
            var importer = new AVMDesignImporter(null, project);
            var result = importer.ImportFile(filename);
            return (IMgaFCO)result.Impl;
        }

        public IMgaFCO ImportDesignToDesignSpace(IMgaProject project, string filename)
        {
            var importer = new AVMDesignImporter(null, project);
            var result = importer.ImportFile(filename, AVMDesignImporter.DesignImportMode.CREATE_DS);
            return (IMgaFCO)result.Impl;
        }

        public IMgaFCO ImportDesignToComponentAssembly(IMgaProject project, string filename)
        {
            var importer = new AVMDesignImporter(null, project);
            var result = importer.ImportFile(filename, AVMDesignImporter.DesignImportMode.CREATE_CAS);
            return (IMgaFCO)result.Impl;
        }

        /*
         * returns: ComponentAssembly if the design contains no design space concepts
         */
        public IMgaFCO ImportDesignToDesignSpaceIfApplicable(IMgaProject project, string filename)
        {
            var importer = new AVMDesignImporter(null, project);
            var result = importer.ImportFile(filename, AVMDesignImporter.DesignImportMode.CREATE_CA_IF_NO_DS_CONCEPTS);
            return (IMgaFCO)result.Impl;
        }

        #region IMgaComponentEx Members

        MgaGateway MgaGateway { get; set; }
        GMEConsole GMEConsole { get; set; }

        public void InvokeEx(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, int param)
        {
            if (!enabled)
            {
                return;
            }

            try
            {
                GMEConsole = GMEConsole.CreateFromProject(project);
                MgaGateway = new MgaGateway(project);

                Main(project, currentobj, selectedobjs, Convert(param));
            }
            finally
            {
                MgaGateway = null;
                project = null;
                currentobj = null;
                selectedobjs = null;
                GMEConsole = null;
                GC.Collect();
                GC.WaitForPendingFinalizers();
            }
        }

        private ComponentStartMode Convert(int param)
        {
            switch (param)
            {
                case (int)ComponentStartMode.GME_BGCONTEXT_START:
                    return ComponentStartMode.GME_BGCONTEXT_START;
                case (int)ComponentStartMode.GME_BROWSER_START:
                    return ComponentStartMode.GME_BROWSER_START;

                case (int)ComponentStartMode.GME_CONTEXT_START:
                    return ComponentStartMode.GME_CONTEXT_START;

                case (int)ComponentStartMode.GME_EMBEDDED_START:
                    return ComponentStartMode.GME_EMBEDDED_START;

                case (int)ComponentStartMode.GME_ICON_START:
                    return ComponentStartMode.GME_ICON_START;

                case (int)ComponentStartMode.GME_MAIN_START:
                    return ComponentStartMode.GME_MAIN_START;

                case (int)ComponentStartMode.GME_MENU_START:
                    return ComponentStartMode.GME_MENU_START;
                case (int)ComponentStartMode.GME_SILENT_MODE:
                    return ComponentStartMode.GME_SILENT_MODE;
            }

            return ComponentStartMode.GME_SILENT_MODE;
        }

        #region Component Information
        public string ComponentName
        {
            get { return GetType().Name; }
        }

        public string ComponentProgID
        {
            get
            {
                return ComponentConfig.progID;
            }
        }

        public componenttype_enum ComponentType
        {
            get { return ComponentConfig.componentType; }
        }
        public string Paradigm
        {
            get { return ComponentConfig.paradigmName; }
        }
        #endregion

        #region Enabling
        bool enabled = true;
        public void Enable(bool newval)
        {
            enabled = newval;
        }
        #endregion

        #region Interactive Mode
        protected bool interactiveMode = true;
        public bool InteractiveMode
        {
            get
            {
                return interactiveMode;
            }
            set
            {
                interactiveMode = value;
            }
        }
        #endregion

        #region Custom Parameters
        SortedDictionary<string, object> componentParameters = null;

        public object get_ComponentParameter(string Name)
        {
            if (Name == "type")
                return "csharp";

            if (Name == "path")
                return GetType().Assembly.Location;

            if (Name == "fullname")
                return GetType().FullName;

            object value;
            if (componentParameters != null && componentParameters.TryGetValue(Name, out value))
            {
                return value;
            }

            return null;
        }

        public void set_ComponentParameter(string Name, object pVal)
        {
            if (componentParameters == null)
            {
                componentParameters = new SortedDictionary<string, object>();
            }

            componentParameters[Name] = pVal;
        }
        #endregion

        #region Unused Methods
        // Old interface, it is never called for MgaComponentEx interfaces
        public void Invoke(MgaProject Project, MgaFCOs selectedobjs, int param)
        {
            throw new NotImplementedException();
        }

        // Not used by GME
        public void ObjectsInvokeEx(MgaProject Project, MgaObject currentobj, MgaObjects selectedobjs, int param)
        {
            throw new NotImplementedException();
        }

        #endregion

        #endregion

        #region IMgaVersionInfo Members

        public GMEInterfaceVersion_enum version
        {
            get { return GMEInterfaceVersion_enum.GMEInterfaceVersion_Current; }
        }

        #endregion

        #region Registration Helpers

        [ComRegisterFunctionAttribute]
        public static void GMERegister(Type t)
        {
            Registrar.RegisterComponentsInGMERegistry();

        }

        [ComUnregisterFunctionAttribute]
        public static void GMEUnRegister(Type t)
        {
            Registrar.UnregisterComponentsInGMERegistry();
        }

        #endregion


    }
}
