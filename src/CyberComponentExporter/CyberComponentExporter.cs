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
using System.Windows.Forms;

using Cyber = ISIS.GME.Dsml.CyberComposition.Interfaces;
using CyberClasses = ISIS.GME.Dsml.CyberComposition.Classes;
using CyberModelUtil;
using Cyber2AVM;

namespace CyberComponentExporter
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyberComponentExporterInterpreter : IMgaComponentEx, IGMEVersionInfo
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
        /// Don't perform MGA operations here unless you open a tansaction.
        /// </summary>
        /// <param name="project">The handle of the project opened in GME, for which the interpreter was called.</param>
        public void Initialize(MgaProject project)
        {
            // TODO: Add your initialization code here...            
        }

        /// <summary>
        /// The main entry point of the interpreter. A transaction is already open,
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
            // TODO: Add your interpreter code
            GMEConsole.Out.WriteLine("Running Cyber Component Exporter...");
			
            //// Get RootFolder
            //IMgaFolder rootFolder = project.RootFolder;
            //GMEConsole.Out.WriteLine(rootFolder.Name);
            
            // To use the domain-specific API:
            //  Create another project with the same name as the paradigm name
            //  Copy the paradigm .mga file to the directory containing the new project
            //  In the new project, install the GME DSMLGenerator NuGet package (search for DSMLGenerator)
            //  Add a Reference in this project to the other project
            //  Add "using [ParadigmName] = ISIS.GME.Dsml.[ParadigmName].Classes.Interfaces;" to the top of this file
            // if (currentobj.Meta.Name == "KindName")
            // [ParadigmName].[KindName] dsCurrentObj = ISIS.GME.Dsml.[ParadigmName].Classes.[KindName].Cast(currentobj);			

            // Adapted from CyPhyComponentExporter.cs
            //#region Prompt for Output Path

            //string startupPath = Path.GetDirectoryName(project.ProjectConnStr.Substring("MGA=".Length));

            // Make sure Cyber output paths exist.
            String s_outPath = ".";
            if (!Directory.Exists(s_outPath + "\\Cyber"))
            {
                Directory.CreateDirectory(s_outPath + "\\Cyber");
            }

            
            //#endregion

            GMEConsole.Info.WriteLine("Beginning Export...");

            string filename = project.ProjectConnStr;
            if (filename.Length > 4)
            {
                filename = filename.Substring(4);
            }
            else
            {
                GMEConsole.Info.WriteLine("Invalid MGA connection string for project. Can not determine file name. Bailing out...");
                throw new System.Exception("Invalid MGA connection string.");
            }

            HashSet<Cyber.ModelicaComponent> cyberComponentSet = null;
            if (currentobj != null)
            {
                cyberComponentSet = ComponentLister.getCyberComponentSet(currentobj);
            }
            else if (selectedobjs.Count > 0)
            {
                cyberComponentSet = new HashSet<Cyber.ModelicaComponent>();
                foreach (MgaFCO mf in selectedobjs)
                {
                    cyberComponentSet.UnionWith(ComponentLister.getCyberComponentSet(mf));
                }
            }
            else
            {
                cyberComponentSet = ComponentLister.getCyberComponentSet(project.RootFolder);
            }

            // Debug
            foreach (Cyber.ModelicaComponent cyberComponent in cyberComponentSet)
            {
                GMEConsole.Info.WriteLine("");
                GMEConsole.Info.Write("Processing Component ... " + cyberComponent.Path);
                AVMComponentBuilder avmComponentBuilder = new AVMComponentBuilder();
                avmComponentBuilder.GMEConsole = GMEConsole;
                avmComponentBuilder.createAVMCyberModel(cyberComponent, filename);
                String s_outFilePath = String.Format("{0}\\{1}.component.acm", s_outPath, META2AVM_Util.UtilFuncs.Safeify(cyberComponent.Name));
                META2AVM_Util.UtilFuncs.SerializeAvmComponent(avmComponentBuilder.getAVMComponent(), s_outFilePath);
                GMEConsole.Info.Write("... Exported " + cyberComponent.Path + " to " + s_outFilePath);
            }

            try
            {
                string[] path_parts = filename.Split('\\');
                string filename_part = path_parts[path_parts.Length - 1];
                path_parts[path_parts.Length - 1] = "Cyber";
                string path_part = System.IO.Path.Combine(path_parts);
                //GMEConsole.Info.Write("Copying " + filename + " to " + System.IO.Path.Combine(path_part, filename_part) );
                GMEConsole.Info.Write("Copying " + filename_part + " to " + System.IO.Path.Combine("Cyber", filename_part));
                // copy the project file to the Cyber subdirectory
                System.IO.File.Copy(filename_part, System.IO.Path.Combine("Cyber", filename_part), true);
            }
            catch (Exception err_copy_model_file)
            {
                GMEConsole.Error.Write("Error copying model file to Cyber subdirectory: " + err_copy_model_file.Message);
                return;
            }

            // Try to create the corresponding XML file
            InvokeUDMCopy invokeUdmCopy = new InvokeUDMCopy();
            if (invokeUdmCopy.GenerateXML(project, filename) == true)
            {
                GMEConsole.Out.WriteLine("Finished XML generation.");
            }
            else
            {
                GMEConsole.Out.WriteLine("XML not generated.");
            }

            GMEConsole.Info.WriteLine("Cyber Component Exporter finished!");
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
                project.CreateTerritoryWithoutSink(out MgaGateway.territory);

                MgaGateway.PerformInTransaction(delegate
                {
                    Main(project, currentobj, selectedobjs, Convert(param));
                });
            }
            finally
            {
                if (MgaGateway.territory != null)
                {
                    MgaGateway.territory.Destroy();
                }
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
