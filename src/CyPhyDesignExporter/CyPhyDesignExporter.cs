using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using GME.CSharp;
using GME;
using GME.MGA;
using GME.MGA.Core;
using System.Windows.Forms;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using CyPhy2DesignInterchange;
using System.Linq;
using CyPhyGUIs;
using System.Reflection;
using System.Xml;

namespace CyPhyDesignExporter
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyPhyDesignExporterInterpreter : IGMEVersionInfo, IMgaComponentEx, ICyPhyInterpreter
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
        /// Parameter of this run.
        /// </summary>
        private InterpreterMainParameters mainParameters { get; set; }

        public string InterpreterConfigurationProgId
        {
            get
            {
                return (typeof(CyPhyGUIs.NullInterpreterConfiguration).GetCustomAttributes(typeof(ProgIdAttribute), false)[0] as ProgIdAttribute).Value;
            }
        }

        public IInterpreterPreConfiguration PreConfig(IPreConfigParameters parameters)
        {
            return null;
        }

        public IInterpreterConfiguration DoGUIConfiguration(IInterpreterPreConfiguration preConfig, IInterpreterConfiguration previousConfig)
        {
            return new CyPhyGUIs.NullInterpreterConfiguration();
        }

        public IInterpreterResult Main(IInterpreterMainParameters parameters)
        {
            this.mainParameters = (InterpreterMainParameters)parameters;

            try
            {
                MgaGateway = new MgaGateway(mainParameters.Project);
                parameters.Project.CreateTerritoryWithoutSink(out MgaGateway.territory);

                var result = new InterpreterResult() { Success = true, RunCommand = "" };

                MgaGateway.PerformInTransaction(delegate
                {
                    MainInTransaction((InterpreterMainParameters)parameters);


                    // TODO: this part needs to be refactored!
                    var workflowRef = this.mainParameters
                        .CurrentFCO
                        .ChildObjects
                        .OfType<MgaReference>()
                        .FirstOrDefault(x => x.Meta.Name == "WorkflowRef");
                    
                    if (workflowRef != null)
                    {
                        string Parameters = workflowRef
                            .Referred
                            .ChildObjects
                            .OfType<MgaAtom>()
                            .FirstOrDefault(fco => fco.Meta.Name == typeof(CyPhy.Task).Name
                                && String.Equals(CyPhyClasses.Task.Cast(fco).Attributes.COMName, this.ComponentProgID, StringComparison.InvariantCultureIgnoreCase))
                            .StrAttrByName["Parameters"];

                        Dictionary<string, string> workflowParameters = new Dictionary<string, string>();

                        try
                        {
                            workflowParameters = (Dictionary<string, string>)Newtonsoft.Json.JsonConvert.DeserializeObject(Parameters, typeof(Dictionary<string, string>));
                            if (workflowParameters == null)
                            {
                                workflowParameters = new Dictionary<string, string>();
                            }
                        }
                        catch (Newtonsoft.Json.JsonReaderException)
                        {
                        }

                        META.AnalysisTool.ApplyToolSelection(this.ComponentProgID, workflowParameters, result, this.mainParameters);
                    }
                });

                return result;
            }
            finally
            {
                if (MgaGateway.territory != null)
                {
                    MgaGateway.territory.Destroy();
                }
                MgaGateway = null;                
                GC.Collect();
                GC.WaitForPendingFinalizers();
            }
        }

        public void MainInTransaction(InterpreterMainParameters parameters)
        {
            this.mainParameters = (InterpreterMainParameters)parameters;
            if (GMEConsole == null)
            {
                GMEConsole = GMEConsole.CreateFromProject(mainParameters.Project);
            }

            var currentObject = mainParameters.CurrentFCO;
            var currentOutputDirectory = mainParameters.OutputDirectory;
            string artifactName = string.Empty;
            string metaBaseName = currentObject.MetaBase.Name;

            if (metaBaseName == typeof(CyPhyClasses.DesignContainer).Name)
            {
                artifactName = ExportToFile(CyPhyClasses.DesignContainer.Cast(currentObject), currentOutputDirectory);
            }
            else if (metaBaseName == typeof(CyPhyClasses.ComponentAssembly).Name)
            {
                artifactName = ExportToFile(CyPhyClasses.ComponentAssembly.Cast(currentObject), currentOutputDirectory);
            }
            else if (IsTestBenchType(metaBaseName))
            {
                artifactName = ExportToFile(CyPhyClasses.TestBenchType.Cast(currentObject), currentOutputDirectory);
            }

            if (!string.IsNullOrWhiteSpace(artifactName))
            {
                var manifest = AVM.DDP.MetaTBManifest.OpenForUpdate(currentOutputDirectory);
                manifest.AddArtifact(Path.GetFileName(artifactName), "Design Model");
                manifest.Serialize(currentOutputDirectory);
            }

        }

        private static Lazy<string[]> m_TestBenchTypeNames = new Lazy<string[]>(() => 
        {
            var tbt = typeof(CyPhy.TestBenchType);
            return Assembly.
                        GetAssembly(tbt).
                        GetTypes().
                        Where(t => tbt.IsAssignableFrom(t)).
                        Select(x => x.Name).ToArray();
        });

        private static string[] TestBenchTypeNames 
        {
            get
            {
                return m_TestBenchTypeNames.Value;
            }
        }

        private bool IsDesignType(string typeName)
        {
            if (typeName == "ComponentAssembly" ||
                typeName == "DesignContainer")
            { 
                return true;
            }

            return false;
        }

        private bool IsTestBenchType(string typeName)
        {
            return TestBenchTypeNames.Contains(typeName);
        }

        private string ExportToFile(CyPhy.TestBenchType testBench, string outputDirectory)
        {
            var topLevelSystem = testBench.Children.TopLevelSystemUnderTestCollection.FirstOrDefault();
            if (topLevelSystem != null)
            {
                var design = topLevelSystem.Referred.DesignEntity;
                if (design != null)
                {
                    return ExportToFile(design, outputDirectory);
                }
            }
            else
            {
                var tlsut = (CyPhy.DesignEntity)testBench.AllChildren
                                    .Where(x => ((IMgaFCO)x.Impl).MetaRole.Name == "TopLevelSystemUnderTest"
                                        || ((IMgaFCO)x.Impl).MetaRole.Name == x.Impl.MetaBase.Name + "TopLevelSystemUnderTest")
                                    .FirstOrDefault();
                if (tlsut != null)
                    return ExportToFile(tlsut, outputDirectory);
            }
            throw new NotSupportedException("No TopLevelSystemUnderTest found");
        }

        private String Safeify(String s_in)
        {
            String rtn = s_in;
            rtn = rtn.Replace("\\", "_");
            rtn = rtn.Replace("/", "_");
            return rtn;
        }

        private string ExportToFile(CyPhy.DesignEntity de, String s_outFolder)
        {
            // Elaborate first
            CallElaborator(de.Impl.Project, de.Impl as MgaFCO, null, 128, true);
            
            var dm = CyPhy2DesignInterchange.CyPhy2DesignInterchange.Convert(de);
            String s_outFilePath = String.Format("{0}\\{1}.adm", s_outFolder, Safeify(de.Name));
            //dm.SaveToFile(s_outFilePath);
            XSD2CSharp.AvmXmlSerializer.SaveToFile(Path.GetFullPath(Path.Combine(s_outFolder, Safeify(de.Name) + ".adm")), dm);

            CheckForDuplicateIDs(dm);

            return s_outFilePath;   
        }

        public bool CheckForDuplicateIDs(avm.Design d)
        {
            //String str = d.Serialize();
            String str = XSD2CSharp.AvmXmlSerializer.Serialize(d);

            XmlDocument doc = new XmlDocument();
            doc.LoadXml(str);
            XmlNode root = doc.DocumentElement;

            var ls_EncounteredIDs = new List<String>();
            foreach (XmlAttribute node in root.SelectNodes("//@ID"))
            {
                ls_EncounteredIDs.Add(node.Value);
            }
            
            // Get all duplicate IDs that aren't empty/whitespace
            var duplicates = ls_EncounteredIDs.Where(s => !String.IsNullOrWhiteSpace(s))
                                              .GroupBy(s => s)
                                              .Where(g => g.Count() > 1)
                                              .Select(g => g.Key)
                                              .ToList();
            if (duplicates.Any())
            {
                String msg = "Duplicate IDs found in exported design: ";
                foreach (var dupe in duplicates)
                    msg += String.Format("{0}\"{1}\", ", Environment.NewLine, dupe);

                if (GMEConsole != null)
                    GMEConsole.Error.WriteLine(msg);
                return true;
            }
            
            return false;
        }

        private bool CallElaborator(
            MgaProject project,
            MgaFCO currentobj,
            MgaFCOs selectedobjs,
            int param,
            bool expand = true)
        {
            bool result = false;
            try
            {
                GMEConsole.Info.WriteLine("Elaborating model...");
                var elaborator = new CyPhyElaborateCS.CyPhyElaborateCSInterpreter();
                elaborator.Initialize(project);
                int verbosity = 128;
                //elaborator.UnrollConnectors = false;
                result = elaborator.RunInTransaction(project, currentobj, selectedobjs, verbosity);

                GMEConsole.Info.WriteLine("Elaboration is done.");
            }
            catch (Exception ex)
            {
                GMEConsole.Error.WriteLine("Exception occurred in Elaborator : {0}", ex.ToString());
                result = false;
            }

            return result;
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
            GMEConsole.Out.WriteLine("Running Design Exporter...");

            #region Prompt for Output Path
            // Get an output path from the user.
            if (this.OutputDir == null)
            {
                using (META.FolderBrowserDialog fbd = new META.FolderBrowserDialog()
                {
                    Description = "Choose a path for the generated files.",
                    //ShowNewFolderButton = true,
                    SelectedPath = Environment.CurrentDirectory,
                })
                {

                    DialogResult dr = fbd.ShowDialog();
                    if (dr == DialogResult.OK)
                    {
                        OutputDir = fbd.SelectedPath;
                    }
                    else
                    {
                        GMEConsole.Warning.WriteLine("Design Exporter cancelled");
                        return;
                    }
                }
            }
            #endregion

            GMEConsole.Info.WriteLine("Beginning Export...");
            List<CyPhy.DesignEntity> lde_allCAandDC = new List<CyPhy.DesignEntity>();
            List<CyPhy.TestBenchType> ltbt_allTB = new List<CyPhy.TestBenchType>();
            
            if (currentobj != null && 
                currentobj.Meta.Name == "ComponentAssembly")
            {
                lde_allCAandDC.Add(CyPhyClasses.ComponentAssembly.Cast(currentobj));
            }
            else if (currentobj != null &&
                     currentobj.Meta.Name == "DesignContainer")
            {
                lde_allCAandDC.Add(CyPhyClasses.DesignContainer.Cast(currentobj));
            }
            else if (currentobj != null &&
                IsTestBenchType(currentobj.MetaBase.Name))
                //CyPhyClasses.TestBenchType.Cast(currentobj) != null)
            {
                ltbt_allTB.Add(CyPhyClasses.TestBenchType.Cast(currentobj));
            }
            else if (selectedobjs != null && selectedobjs.Count > 0)
            {
                foreach (MgaFCO mf in selectedobjs)
                {
                    if (mf.Meta.Name == "ComponentAssembly")
                    {
                        lde_allCAandDC.Add(CyPhyClasses.ComponentAssembly.Cast(mf));
                    }
                    else if (mf.Meta.Name == "DesignContainer")
                    
                    {
                        lde_allCAandDC.Add(CyPhyClasses.DesignContainer.Cast(mf));
                    }
                    else if (IsTestBenchType(mf.MetaBase.Name))
                    {
                        ltbt_allTB.Add(CyPhyClasses.TestBenchType.Cast(mf));
                    }
                }
            }
            else
            {
                CyPhy.RootFolder rootFolder = ISIS.GME.Common.Utils.CreateObject<CyPhyClasses.RootFolder>(project.RootFolder as MgaObject);

                MgaFilter filter = project.CreateFilter();
                filter.Kind = "ComponentAssembly";
                foreach (var item in project.AllFCOs(filter).Cast<MgaFCO>())
                {
                    if (item.ParentFolder != null)
                    {
                        lde_allCAandDC.Add(CyPhyClasses.ComponentAssembly.Cast(item));
                    }
                }

                filter = project.CreateFilter();
                filter.Kind = "DesignContainer";
                foreach (var item in project.AllFCOs(filter).Cast<MgaFCO>())
                {
                    if (item.ParentFolder != null)
                    {
                        lde_allCAandDC.Add(CyPhyClasses.DesignContainer.Cast(item));
                    }
                }

                filter = project.CreateFilter();
                filter.Kind = "TestBenchType";
                foreach (var item in project.AllFCOs(filter).Cast<MgaFCO>())
                {
                    if (item.ParentFolder != null)
                    {
                        ltbt_allTB.Add(CyPhyClasses.TestBenchType.Cast(item));
                    }
                }
            }

            foreach (CyPhy.DesignEntity de in lde_allCAandDC)
            {
                System.Windows.Forms.Application.DoEvents();
                try
                {
                    ExportToFile(de, OutputDir);
                } 
                catch (Exception ex) 
                {
                    GMEConsole.Error.WriteLine("{0}: Exception encountered ({1})",de.Name,ex.Message);
                }
                GMEConsole.Info.WriteLine("{0}: {1}", de.Name, OutputDir);
            }

            foreach (CyPhy.TestBenchType tbt in ltbt_allTB)
            {
                System.Windows.Forms.Application.DoEvents();
                try
                {
                    ExportToFile(tbt, OutputDir);
                }
                catch (Exception ex)
                {
                    GMEConsole.Error.WriteLine("{0}: Exception encountered ({1})", tbt.Name, ex.Message);
                }
                GMEConsole.Info.WriteLine("{0}: {1}", tbt.Name, OutputDir);
            }

            GMEConsole.Info.WriteLine(String.Format("{0} model(s) exported", lde_allCAandDC.Count + ltbt_allTB.Count));
            GMEConsole.Info.WriteLine("Design Exporter finished");			
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

                MgaGateway.BeginTransaction();
                Main(project, currentobj, selectedobjs, Convert(param));
                MgaGateway.AbortTransaction();
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

        public string OutputDir;
    }
}
