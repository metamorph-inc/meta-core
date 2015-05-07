using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using avm;
using DesignConsistencyChecker.DesignRule;
using GME.CSharp;
using GME.MGA;
using GME.MGA.Core;
using Ionic.Zip;
using META;
using Newtonsoft.Json;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhyComponentExporter
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyPhyComponentExporterInterpreter : IMgaComponentEx, IGMEVersionInfo
    {
        protected static readonly Regex cadResourceRegex = new Regex("^(.*)(\\.prt|\\.asm)\\.([0-9]*)$", RegexOptions.IgnoreCase);

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

        public static string previousExportPath = null; // MOT-387
        
        /// <summary>
        /// This function is called for each interpreter invocation before Main.
        /// Don't perform MGA operations here unless you open a tansaction.
        /// </summary>
        /// <param name="project">The handle of the project opened in GME, for which the interpreter was called.</param>
        public void Initialize(MgaProject project)
        {
            // TODO: Add your initialization code here...
            GMEConsole = GMEConsole.CreateFromProject(project);
        }

        List<CyPhy.Component> GetAllComponents(CyPhy.Components componentsFolder)
        {
            List<CyPhy.Component> componentList = new List<CyPhy.Component>();
            foreach (CyPhy.Components childComponentsFolder in componentsFolder.Children.ComponentsCollection)
            {
                componentList.AddRange(GetAllComponents(childComponentsFolder));
            }

            foreach (CyPhy.Component childComponent in componentsFolder.Children.ComponentCollection)
            {
                componentList.Add(childComponent);
            }
            return componentList;
        }

        private static String Safeify(String s_in)
        {
            String rtn = s_in;
            rtn = rtn.Replace("\\", "_");
            rtn = rtn.Replace("/", "_");
            return rtn;
        }

        public static String ExportToFile(CyPhy.Component c, String s_outFolder)
        {
            try
            {
                avm.Component avmComponent = CyPhy2ComponentModel.Convert.CyPhyML2AVMComponent(c);
                String s_outFilePath = String.Format("{0}\\{1}.component.acm", s_outFolder, System.IO.Path.GetRandomFileName());
                SerializeAvmComponent(avmComponent, s_outFilePath);

                return s_outFilePath;
            }
            catch (Exception ex)
            {
                var console = GMEConsole.CreateFromProject(c.Impl.Project);
                console.Error.WriteLine("Exception Parsing {0}: {1}", Safeify(c.Name), ex.Message);
                return null;
            }
        }

        public static void SerializeAvmComponent(avm.Component avmComponent, String s_outFilePath)
        {
            avmComponent.SchemaVersion = "2.5";

            FileStream stream = new FileStream(s_outFilePath, FileMode.Create);
            using (stream)
            {
                XSD2CSharp.AvmXmlSerializer.Serialize(avmComponent, stream);
                stream.Close();
            }
        }



        // RB 8/16/13
        public void TraverseTestBenchForComponentExport(MgaFCO currentobj, string OutputDirectory, string ProjectDirectory)
        {
            string type = currentobj.MetaBase.Name;
            if (type != "TestBench" && type != "BallisticTestBench" && type != "BlastTestBench")
            {
                GMEConsole.Info.WriteLine("ERROR: " + type + " != TestBench or BlastTestBench or BallisticTestBench");
                return;
            }

            //CyPhy.TestBench tb = CyPhyClasses.TestBench.Cast(currentobj);     // DY: META-1971
            CyPhy.TestBenchType tb = CyPhyClasses.TestBenchType.Cast(currentobj);
            var catlsut = tb.Children.ComponentAssemblyCollection.FirstOrDefault();
            if (catlsut == null)
            {
                throw new Exception("There is no elaborated system under test component assembly in the model!");
            }

            ComponentIndex compindex = new ComponentIndex(); //create index
            TraverseComponentAssemblyForComponentExport(compindex, catlsut, OutputDirectory, ProjectDirectory);
        }

        // 8/16/13
        private void TraverseComponentAssemblyForComponentExport(ComponentIndex compindex, CyPhy.ComponentAssembly componentasm, string OutputDirectory, string ProjectDirectory)
        {
            foreach (CyPhy.ComponentRef cref in componentasm.Children.ComponentRefCollection)
            {
                throw new Exception("Model not fully elaborated, contains ComponentRef [" + cref.Path + "]");
            }

            foreach (CyPhy.ComponentAssembly cainst in componentasm.Children.ComponentAssemblyCollection)
            {
                TraverseComponentAssemblyForComponentExport(compindex, cainst, OutputDirectory, ProjectDirectory);
            }


            foreach (CyPhy.Component cint in componentasm.Children.ComponentCollection)
            {
                var console = GMEConsole.CreateFromProject(cint.Impl.Project);
                string comp_inst_loc = cint.Attributes.InstanceGUID; //Component Instance 
                char[] charsToTrim = { '{', '}' };

                string ACMDir = Path.Combine(OutputDirectory, "ComponentACMs");  //create folder for component acm's
                if (!Directory.Exists(ACMDir))
                    Directory.CreateDirectory(ACMDir);

                string avmid = cint.Attributes.AVMID;

                string newACMComponentFileLocation;
                char[] charsToTrim1 = { '{', '}' };
                string RandACMFileName = System.IO.Path.GetRandomFileName() + ".component.acm";
                newACMComponentFileLocation = Path.Combine(ACMDir, RandACMFileName);
                String classification = "";
                try
                {
                    // First, try to copy the original ACM file, if found.
                    // If not, then export a new one. CAD model must be present.
                    string originalACMFilePath = "";
                    if (META.ComponentLibraryManager.TryGetOriginalACMFilePath(cint, out originalACMFilePath, META.ComponentLibraryManager.PathConvention.ABSOLUTE, ProjectDirectory: ProjectDirectory))
                    {
                        File.Copy(originalACMFilePath, newACMComponentFileLocation);
                        classification = cint.Attributes.Classifications.Split('\n').FirstOrDefault();
                    }
                    else
                    {
                        const string cadFormat = "Creo";
                        CyPhy.CADModel cadmodel = cint.Children.CADModelCollection.FirstOrDefault(x => x.Attributes.FileFormat.ToString() == cadFormat);
                        if (cadmodel == null || cadmodel.Attributes.FileFormat.ToString() != cadFormat)
                        {
                            console.Warning.WriteLine("Skipping component export for {0}. No original ACM file found and no CAD model present in component.", Safeify(cint.Name));
                            continue;
                        }
                        else
                        {
                            var avmComponentModel = CyPhyML2AVM.AVMComponentBuilder.CyPhyML2AVM(cint);
                            SerializeAvmComponent(avmComponentModel, newACMComponentFileLocation);
                            classification = avmComponentModel.Classifications.FirstOrDefault();
                        }
                    }

                }
                catch (Exception ex)
                {
                    //var console = GMEConsole.CreateFromProject(cint.Impl.Project);
                    console.Warning.WriteLine("Unable to find original ACM file or export a new one for {0}. Reason: {1} Moving to next component.", Safeify(cint.Name), Safeify(ex.Message));
                    continue;
                }

                System.Uri acm = new Uri(newACMComponentFileLocation);
                System.Uri acm_folder = new Uri(ACMDir);
                string relative_path = Uri.UnescapeDataString(acm_folder.MakeRelativeUri(acm).ToString());

                compindex.AddMember(cint.Name, cint.Attributes.AVMID, cint.Attributes.InstanceGUID, relative_path, classification);  //add component to index
                compindex.Serialize();
                compindex.SerializeToFile(Path.Combine(OutputDirectory, "component_index.json"));
                
                
            }

        }

        // RB 8/16/13 - Create Component Index
        private class ComponentIndex
        {
            // Choose some settings for the Newtonsoft JSON serializer
            private static JsonSerializerSettings jss_defaults = new JsonSerializerSettings()
            {
                PreserveReferencesHandling = PreserveReferencesHandling.None,
                TypeNameHandling = TypeNameHandling.None,
                NullValueHandling = NullValueHandling.Ignore,

            };

            // This data structure defines the contents of the elements of the index.
            private class ComponentIndexMember
            {
                public String Name;
                public String AVMID;
                public String InstanceID;
                public String ModelPath;
                public String Classification;
            }

            private List<ComponentIndexMember> members;

            // Constructor
            public ComponentIndex()
            {
                // Create a new, empty list
                this.members = new List<ComponentIndexMember>();
            }

            public void AddMember(String Name, String AVMID, String InstanceID, String ModelPath, String Classification)
            {
                ComponentIndexMember cim = new ComponentIndexMember();
                cim.Name = Name;
                cim.AVMID = AVMID;
                cim.InstanceID = InstanceID;
                cim.ModelPath = ModelPath;
                cim.Classification = Classification;
                this.members.Add(cim);
            }

            // Take this object and serialize it to JSON format.
            public string Serialize()
            {
                string json = JsonConvert.SerializeObject(
                    this.members,
                    Formatting.Indented,
                    jss_defaults
                );

                return json;
            }

            // Serialize this object to JSON format and write it to disk
            public void SerializeToFile(String path)
            {
                string jsonString = this.Serialize();
                using (System.IO.StreamWriter sw = new System.IO.StreamWriter(path)) { sw.WriteLine(jsonString); }
            }
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
            GMEConsole.Info.WriteLine("Running Component Exporter...");

            #region Prompt for Output Path

            GMEConsole.Info.WriteLine("Beginning Export...");
            int i_count = 0;
            HashSet<CyPhy.Component> cyPhyComponentSet = null;
            if (currentobj != null)
            {
                cyPhyComponentSet = CyPhy2ComponentModel.ComponentLister.getCyPhyMLComponentSet(currentobj);
            }
            else if (selectedobjs != null && selectedobjs.Count > 0)
            {
                cyPhyComponentSet = new HashSet<CyPhy.Component>();
                foreach (MgaFCO mf in selectedobjs)
                {
                    cyPhyComponentSet.UnionWith(CyPhy2ComponentModel.ComponentLister.getCyPhyMLComponentSet(mf));
                }
            }
            else
            {
                cyPhyComponentSet = CyPhy2ComponentModel.ComponentLister.getCyPhyMLComponentSet(project.RootFolder);
            }

            if (cyPhyComponentSet.Count == 0)
            {
                GMEConsole.Warning.WriteLine("Nothing to export.");
                return;
            }

            string startupPath;

            if (false == String.IsNullOrWhiteSpace(previousExportPath)
                 && Directory.Exists(previousExportPath))
            {
                 startupPath = previousExportPath;
            }
            else
            {
                startupPath = Path.GetDirectoryName(project.ProjectConnStr.Substring("MGA=".Length));
            }

            // Get an output path from the user.
            if (this.OutputDir == null)
            {
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
                        OutputDir = fbd.SelectedPath;
                        previousExportPath = OutputDir;
                    }
                    else
                    {
                        GMEConsole.Warning.WriteLine("Component Exporter cancelled");
                        return;
                    }
                }
            }
            #endregion

            var ruleChecker = new DesignConsistencyChecker.Framework.Checker(currentobj, project);
            ruleChecker.RegisterRuleDll("DesignConsistencyChecker.dll");
            var uniqueNameRule = ruleChecker.GetRegisteredRules.FirstOrDefault(x => x.Name == "UniquePPMNames");

            foreach (CyPhy.Component c in cyPhyComponentSet)
            {
                if (uniqueNameRule != null)
                {
                    var feedBacks = uniqueNameRule.Check((MgaFCO)c.Impl).ToList();
                    if (feedBacks.Any(x => x.FeedbackType == FeedbackTypes.Error))
                    {
                        foreach (var fb in feedBacks.Where(x => x.FeedbackType == FeedbackTypes.Error))
                        {
                            GMEConsole.Error.WriteLine("Rule {0} failed: {1}", uniqueNameRule.Name, fb.Message);
                        }
                        continue;
                    }
                }

                try
                {
                    ExportComponentPackage(c, OutputDir);                    
                    i_count++;
                }
                catch (Exception ex)
                {
                    Console.Error.WriteLine("{0} encountered while exporting {1}: {2}", ex.GetType().Name, c.Name, ex.Message);
                }
            }

            GMEConsole.Info.WriteLine(String.Format("{0} Component(s) exported", i_count));
            GMEConsole.Info.WriteLine("Component Exporter finished");			
        }

        /// <summary>
        /// Given a CyPhy Component, builds a .ZIP-format Component Package, which includes
        /// an ACM version, as well as all artifacts from the component's
        /// backend folder. Note that it will grab ALL artifacts from that folder, not just
        /// those called out as Resources by the Component. It will also exclude any
        /// *.acm file found in that folder, opting to create a new ACM file from the
        /// current state of the component.
        /// </summary>
        /// <param name="component"></param>
        /// <param name="outputFolder">The folder where the component package should be created. If null, a temporary folder will be used.</param>
        /// <returns>The absolute path of the component package.</returns>
        public static String ExportComponentPackage(CyPhy.Component component, String outputFolder = null)
        {
            if (String.IsNullOrWhiteSpace(outputFolder))
            {
                // Assign a temp folder
                outputFolder = Path.Combine(System.IO.Path.GetTempPath(), Path.GetRandomFileName());
                Directory.CreateDirectory(outputFolder);
            }

            String acmFilePath = String.Format("{0}\\{1}.component.acm", outputFolder, System.IO.Path.GetRandomFileName());
            if (acmFilePath == null)
            {
                return null;
            }

            avm.Component avmComponent;

            var componentBuilder = new CyPhyML2AVM.AVMComponentBuilder();
            avmComponent = componentBuilder.CyPhyML2AVMNonStatic(component);

            // Create a ZIP filename
            String filename = String.Format("{0}.zip", component.Name);
            
            // If not unique, try a few different ones
            int counter = 1;
            while (File.Exists(Path.Combine(outputFolder, filename)))
            {
                filename = String.Format("{0}({1}).zip", component.Name, counter++);
            }

            String zipFileAbsPath = Path.Combine(outputFolder, filename);
            using (ZipFile zip = new ZipFile(zipFileAbsPath)
            {
                CompressionLevel = Ionic.Zlib.CompressionLevel.BestCompression
            })
            {
                String compDirAbsPath = component.GetDirectoryPath(META.ComponentLibraryManager.PathConvention.ABSOLUTE);
                foreach (var filePath in Directory
                                    .EnumerateFiles(compDirAbsPath,"*.*",SearchOption.AllDirectories)
                                    .Where(f => Path.GetExtension(f).ToLower() != ".acm"
                                             && Path.GetFileName(f).ToLower() != "componentdata.xml"))
                {
                    String fileRelDir;
                    if (compDirAbsPath.EndsWith("/"))
                    {
                        fileRelDir = Path.GetDirectoryName(
                                                ComponentLibraryManager.MakeRelativePath(compDirAbsPath, filePath));
                    }
                    else
                    {
                        fileRelDir = Path.GetDirectoryName(
                                                ComponentLibraryManager.MakeRelativePath(compDirAbsPath + "/", filePath));
                    }


                    zip.AddFile(filePath, fileRelDir);
                    Match match = cadResourceRegex.Match(Path.Combine(fileRelDir, Path.GetFileName(filePath)));
                    if (match.Success)
                    {
                        Func<Resource, bool> sameFile = delegate (Resource x) {
                            if (x.Path == match.Groups[1].Value + match.Groups[2].Value)
                                return true;
                            Match m = cadResourceRegex.Match(x.Path);
                            return m.Success && m.Groups[1].Value == match.Groups[1].Value && m.Groups[2].Value == match.Groups[2].Value;
                        };
                        foreach (var resource in avmComponent.ResourceDependency.Where(sameFile))
                        {
                            Match currentMatch = cadResourceRegex.Match(resource.Path);
                            if (currentMatch.Success == false || (Int32.Parse(match.Groups[3].Value) > Int32.Parse(currentMatch.Groups[3].Value)))
                            {
                                resource.Path = Path.Combine(fileRelDir, Path.GetFileName(filePath));
                            }
                        }
                    }
                }
                // Add the ACM file.
                SerializeAvmComponent(avmComponent, acmFilePath);
                zip.AddFile(acmFilePath, "").FileName = String.Format("{0}.acm", component.Name);

                zip.Save();
            }

            // Delete that ACM file
            File.Delete(acmFilePath);

            return zipFileAbsPath;
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

        public string OutputDir;
    }
}
