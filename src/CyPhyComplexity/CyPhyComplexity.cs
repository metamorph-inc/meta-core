using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using GME.CSharp;
using GME;
using GME.MGA;
using GME.MGA.Core;
using System.Linq;
using System.Drawing;
using System.Windows.Forms;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using CyPhyGUIs;
//using Complexity;

namespace CyPhyComplexity
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyPhyComplexityInterpreter : IMgaComponentEx, IGMEVersionInfo, ICyPhyInterpreter
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
            MgaGateway = new MgaGateway(project);
            project.CreateTerritoryWithoutSink(out MgaGateway.territory);
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
            if (currentobj.Meta.Name == "TestBench")
            {
                CyPhy.TestBench tb = ISIS.GME.Common.Utils.CreateObject<CyPhyClasses.TestBench>(currentobj as MgaObject);

                //var sut = tb.Children.TopLevelSystemUnderTestCollection.FirstOrDefault();
                //String s_sutName = sut.Name;

                CyPhy.ComponentAssembly sut = null;
                foreach (CyPhy.ComponentAssembly ca in tb.Children.ComponentAssemblyCollection)
                {
                    if ((ca.Impl as MgaFCO).get_RegistryValue("ElaboratedModel") != "")
                        sut = ca;
                }

                if (sut != null)
                {
                    var dm = CyPhy2DesignInterchange.CyPhy2DesignInterchange.Convert(sut);
                    //dm = MakeFakeModel();
                    //String json = dm.Serialize();
                    String json = XSD2CSharp.AvmXmlSerializer.Serialize(dm);
                    int json_len = json.Length;
                    dm.SaveToFile(Path.Combine(OutputDirectory, dm.Name + ".adm"));

                    Complexity.Design cd = METADesignInterchange2ComplexityLib.METADesign2Complexity.Design2Complexity(dm);
                    Dictionary<string,string> dCSV_output = cd.SerializeToCSVFormat();

                    foreach (KeyValuePair<string, string> kvp in dCSV_output)
                    {
                        String s_filePath = Path.Combine(OutputDirectory, kvp.Key);
                        using (System.IO.StreamWriter sw = new System.IO.StreamWriter(s_filePath))
                        {
                            sw.Write(kvp.Value);
                        }
                    }

                    string mainPythonFilename = Path.Combine(OutputDirectory, "ComplexityMain.py");
                    string mainCmdFilename = Path.Combine(OutputDirectory, "RunComplexityEvaluator.cmd");

                    using (StreamWriter writer = new StreamWriter(mainPythonFilename))
                    {
                        writer.WriteLine(CyPhyComplexity.Properties.Resources.ComplexityMain);
                    }
                    using (StreamWriter writer = new StreamWriter(mainCmdFilename))
                    {
                        writer.WriteLine("call \"{0}\"", META_PATH_PYTHON_ACTIVATE);
                        writer.WriteLine("python {0}  Components.csv Connections.csv 1", mainPythonFilename);

                        //writer.WriteLine(CyPhyComplexity.Properties.Resources.RunComplexityEvaluator);
                    }

                    if (dCSV_output.Count == 2)
                    {
                        result.RunCommand = "RunComplexityEvaluator.cmd";
                    }

                    GMEConsole.Info.WriteLine(
                        "Result files are here: <a href=\"file:///{0}\" target=\"_blank\">{0}</a>.",
                        Path.GetFullPath(OutputDirectory));

                }
                                        
            }
        }

        public string META_PATH { get; set; }
        public string META_PATH_PYTHON_ACTIVATE
        {
            get
            {
                return Path.Combine(META_PATH, "bin", "Python27", "Scripts", "activate.bat");
            }
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
            if (currentobj == null)
            {
                GMEConsole = GMEConsole.CreateFromProject(project);
                GMEConsole.Warning.WriteLine("Please run in the context of a TestBench");
                return;
            }

            // Make sure running within the context of a testbench
            bool inTBContext = false;
            MgaGateway.PerformInTransaction(delegate
            {
                if (currentobj.Meta.Name == "TestBench")
                {
                    inTBContext = true;
                }
            });
            if (!inTBContext)
            {
                GMEConsole = GMEConsole.CreateFromProject(project);
                GMEConsole.Warning.WriteLine("Please run in the context of a TestBench");
                return;
            }

            // Need to get an output path from the user.
            using (META.FolderBrowserDialog fbd = new META.FolderBrowserDialog())
            {
                fbd.Description = "Choose a path for the generated files.";
                //fbd.ShowNewFolderButton = true;
                DialogResult dr = fbd.ShowDialog();
                if (dr == DialogResult.OK)
                {
                    OutputDirectory = fbd.SelectedPath;
                }
                else
                {
                    GMEConsole = GMEConsole.CreateFromProject(project);
                    GMEConsole.Out.WriteLine("CyPhyComplexity: Operation cancelled");
                    return;
                }
            }

            try
            {
                CallElaboratorAndMain(project, currentobj, selectedobjs, param);
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

        private void CallElaboratorAndMain(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, int param)
        {
            string keyName = @"HKEY_LOCAL_MACHINE\Software\META";
            string value = @"META_PATH";

            META_PATH = (string)Microsoft.Win32.Registry.GetValue(
                keyName,
                value,
                "ERROR: " + keyName + value + " does not exist!");

            GMEConsole = GMEConsole.CreateFromProject(project);

            #region Elaborate the TestBench
            // call elaborator and expand the references
            Type t = Type.GetTypeFromProgID("MGA.Interpreter.CyPhyElaborate");
            IMgaComponentEx elaborator = Activator.CreateInstance(t) as IMgaComponentEx;
            elaborator.Initialize(project);
            elaborator.ComponentParameter["automated_expand"] = "true";
            elaborator.ComponentParameter["console_messages"] = "off";
            elaborator.InvokeEx(project, currentobj, selectedobjs, (int)ComponentStartMode.GME_SILENT_MODE);
            #endregion

            MgaGateway.PerformInTransaction(delegate
            {
                Main(project, currentobj, selectedobjs, Convert(param));
            });
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
        SortedDictionary<string, object> componentParameters = new SortedDictionary<string,object>();

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



        public string InterpreterConfigurationProgId
        {
            get { return null; }
        }

        public IInterpreterPreConfiguration PreConfig(IPreConfigParameters parameters)
        {
            return null;
        }

        public IInterpreterConfiguration DoGUIConfiguration(IInterpreterPreConfiguration preConfig, IInterpreterConfiguration previousConfig)
        {
            return new CyPhyGUIs.NullInterpreterConfiguration();
        }

        private string OutputDirectory;
        private InterpreterResult result = new InterpreterResult();
        public IInterpreterResult Main(IInterpreterMainParameters parameters)
        {
            OutputDirectory = parameters.OutputDirectory;
            CallElaboratorAndMain(parameters.Project, parameters.CurrentFCO, parameters.SelectedFCOs, parameters.StartModeParam);

            result.Success = true;
            return result;
        }
    }
}
