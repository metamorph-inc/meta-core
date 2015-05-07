using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Xml;
using avm;
using AVM.DDP;
using GME.CSharp;
using GME.MGA;
using GME.MGA.Core;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhyComponentImporter
{
    /// <summary>
    /// This class implements the necessary COM interfaces for a GME interpreter component.
    /// </summary>
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyPhyComponentImporterInterpreter : IMgaComponentEx, IGMEVersionInfo
    {
        /// <summary>
        /// The version of the ACM schema expected by the importer. If an input file has a different version,
        /// the importer will warn the user, but try to import the component anyway.
        /// </summary>
        public static double ExpectedSchemaVersion = 2.5;
        
        /// <summary>
        /// The default name of the folder in which imported components are placed.
        /// </summary>
        public readonly static string ImportedComponentsFolderName = "Imported_Components";

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

        private static void WriteLine(Func<string, string, string> f, IMgaFCO a, IMgaFCO b)
        {
            //if (GMEConsole != null)
            //{
            //    GMEConsole.Out.WriteLine(f(GetLink(a, a.Name), GetLink(b, b.Name)));
            //}
            //else
            {
                Console.Out.WriteLine(f(a.AbsPath, b.AbsPath));
            }
        }

        /// <summary>
        /// This function is called for each interpreter invocation before Main.
        /// Don't perform MGA operations here unless you open a transaction.
        /// </summary>
        /// <param name="project">The handle of the project opened in GME, for which the interpreter was called.</param>
        public void Initialize(MgaProject project)
        {
            // TODO: Add your initialization code here...  

            GMEConsole = GMEConsole.CreateFromProject(project);
            MgaGateway = new MgaGateway(project);
            project.CreateTerritoryWithoutSink(out MgaGateway.territory);
        }

        private CyPhy.Components GetImportFolder(CyPhy.RootFolder rf)
        {
            CyPhy.Components rtn = null;
            foreach (CyPhy.Components c in rf.Children.ComponentsCollection)
            {
                if (c.Name == ImportedComponentsFolderName)
                {
                    rtn = c;
                    break;
                }
            }

            if (rtn == null)
            {
                rtn = CyPhyClasses.Components.Create(rf);
                rtn.Name = ImportedComponentsFolderName;
            }

            return rtn;
        }

        #region getCyPhyMLComponentDictionary Functions
        public static Dictionary<string, CyPhy.Component> getCyPhyMLComponentDictionary_ByAVMID(CyPhy.RootFolder cyPhyMLRootFolder)
        {
            var rtn = new Dictionary<string, CyPhy.Component>();

            var project = cyPhyMLRootFolder.Impl.Project;
            MgaFilter filter = project.CreateFilter();
            filter.Kind = "Component";

            // The only folder that can contain components is "Components"
            foreach (var componentsFolder in cyPhyMLRootFolder.Children.ComponentsCollection)
            {
                IMgaFolder mgaComponentsFolder = (IMgaFolder)componentsFolder.Impl;

                foreach (var item in mgaComponentsFolder.GetDescendantFCOs(filter).Cast<MgaFCO>().Where(x => x.ParentFolder != null))
                {
                    var comp = CyPhyClasses.Component.Cast(item);
                    rtn[comp.Attributes.AVMID] = comp;
                }
            }
            return rtn;
        }

        private static Dictionary<string, CyPhy.Component> getCyPhyMLComponentDictionary_ByName(CyPhy.RootFolder cyPhyMLRootFolder)
        {
            var rtn = new Dictionary<string, CyPhy.Component>();

            var project = cyPhyMLRootFolder.Impl.Project;
            MgaFilter filter = project.CreateFilter();
            filter.Kind = "Component";
            foreach (var item in project.AllFCOs(filter).Cast<MgaFCO>().Where(x => x.ParentFolder != null))
            {
                var comp = CyPhyClasses.Component.Cast(item);
                rtn[comp.Name] = comp;
            }

            return rtn;
        }
        #endregion

        public static string Meta_Path
        {
            get
            {
                return META.VersionInfo.MetaPath;
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
            string projroot = Path.GetDirectoryName(project.ProjectConnStr.Substring("MGA=".Length));

            // TODO: Add your interpreter code
            GMEConsole.Out.WriteLine("Running Component Importer...");

            string[] FileNames = null;
            DialogResult dr;
            using (OpenFileDialog ofd = new OpenFileDialog())
            {
                ofd.CheckFileExists = true;
                ofd.DefaultExt = "component.acm";
                ofd.Multiselect = true;
                ofd.Filter = "component files (*.acm;*.component.acm;*.zip)|*.acm;*.component.acm;*.zip|All files (*.*)|*.*";

                dr = ofd.ShowDialog();
                if (dr == DialogResult.OK)
                {
                    FileNames = ofd.FileNames;
                }
            }
            if (dr == DialogResult.OK)
            {
                MgaGateway.PerformInTransaction(delegate
                {
                    ImportFiles(project, projroot, FileNames);
                }, transactiontype_enum.TRANSACTION_NON_NESTED);
                return;
            }
            else
            {
                GMEConsole.Warning.WriteLine("Component Importer canceled");
                return;
            }

            // Get RootFolder
            //IMgaFolder rootFolder = project.RootFolder;
            //GMEConsole.Out.WriteLine(rootFolder.Name);

        }

        public IMgaFCO ImportFile(MgaProject project, string projroot, string FileName)
        {
            // this function can be called from python
            return this.ImportFiles(project, projroot, new string[] { FileName }).Cast<IMgaFCO>().FirstOrDefault();
        }

        public IMgaFCOs ImportFiles(MgaProject project, string projRootPath, string[] FileNames, bool alwaysReplace = false)
        {
            Boolean b_CLMAddOnStatus = META.ComponentLibraryManagerAddOn.GetEnabled(project);
            META.ComponentLibraryManagerAddOn.Enable(false, project);
            var addons = project.AddOnComponents.Cast<IMgaComponentEx>();
            foreach (var addon in addons)
            {
                if (addon.ComponentName.ToLowerInvariant() == "CyPhyAddOn".ToLowerInvariant())
                {
                    addon.ComponentParameter["DontAssignGuidsOnNextTransaction".ToLowerInvariant()] = true;
                }
            }
			
            IMgaFCOs importedComponents = (IMgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));

            try
            {
                bool replaceAll = alwaysReplace;
                bool doNotReplaceAll = false;
                CyPhy.RootFolder rootFolder = ISIS.GME.Common.Utils.CreateObject<CyPhyClasses.RootFolder>(project.RootFolder as MgaObject);
                Dictionary<string, CyPhy.Component> avmidComponentMap = getCyPhyMLComponentDictionary_ByAVMID(rootFolder);
                Dictionary<string, CyPhy.Component> nameComponentMap = getCyPhyMLComponentDictionary_ByName(rootFolder);
                Boolean b_FirstComponent = true;

                foreach (String inputFile in FileNames)
                {
                    var inputFilePath = inputFile;

                    /* Disable validation for now
                    #region Check validation rules

                    var errorMessages = new List<string>();
                    ComponentImportRules.CheckUniqueNames(inputXMLFile, ref errorMessages);
                    var isValid = ComponentImportRules.ValidateXsd(inputXMLFile, ref errorMessages);

                    foreach (var errorMessage in errorMessages)
                    {
                        GMEConsole.Error.WriteLine(errorMessage);
                    }

                    if (!isValid)
                    {
                        GMEConsole.Info.WriteLine("XML is not valid. Skip file: {0}", inputXMLFile);
                        continue;
                    }
                    #endregion
                    */

                    GMEConsole.Info.WriteLine("Importing {0}", inputFilePath);

                    // If ZIP file, unzip it to a temporary directory, then import as usual
                    UnzipToTemp unzip = null;
                    bool bZipArchive = (Path.GetExtension(inputFilePath) == ".zip");
                    if (bZipArchive)
                    {
                        unzip = new UnzipToTemp(GMEConsole);
                        List<string> entries = unzip.UnzipFile(inputFilePath);
                        inputFilePath = entries.Where(entry => Path.GetDirectoryName(entry) == "" && entry.EndsWith(".acm")).FirstOrDefault();
                        if (inputFilePath != null)
                        {
                            inputFilePath = Path.Combine(unzip.TempDirDestination, inputFilePath);
                        }
                    }

                    using (unzip)
                    try
                    {
                        if (inputFilePath == null) // may be null if .zip didn't contain .acm
                        {
                            throw new FileNotFoundException("No ACM file not found in root directory of ZIP file.");
                        }
                        avm.Component ac_import = null;
                        
                        /* META-3003: Check for old-style ITAR statement */
                        bool isDistributionD = false;
                        bool isNotItar = false;

                        #region Check for old-style ITAR statements

                        XmlDocument doc = new XmlDocument();
                        doc.Load(inputFilePath);
                        XmlNode root = doc.DocumentElement;

                        XmlNodeList nodeList = root.SelectNodes("//*[local-name()='DistributionRestriction' and @Level='ITARDistributionD']");
                        if (nodeList.Count > 0)
                        {
                            isDistributionD = true;
                        }
                        else
                        {
                            nodeList = root.SelectNodes("//*[local-name()='DistributionRestriction' and @Level='NotITAR']");
                            if (nodeList.Count > 0)
                            {
                                isNotItar = true;
                            }
                        }

                        #endregion
                        
                        using (StreamReader streamReader = new StreamReader(inputFilePath))
                        {
                            ac_import = DeserializeAvmComponentXml(streamReader);
                        }
                        if (ac_import == null)
                        {
                            throw new Exception("Could not load ACM file.");
                        }

                        /* Throw warning if from an unexpected schema version */
                        CheckAndWarnOnSchemaVersion(ac_import);

                        /* META-3003: Strip old-style ITAR statement */
                        #region Strip old-style ITAR statements

                        if (isDistributionD)
                        {
                            // Correct this.
                            if (ac_import.DistributionRestriction == null)
                            {
                                ac_import.DistributionRestriction = new List<avm.DistributionRestriction>();
                            }

                            ac_import.DistributionRestriction.Add(new avm.DoDDistributionStatement()
                            {
                                Type = DoDDistributionStatementEnum.StatementD
                            });
                        }
                        else if (isNotItar)
                        {
                            var itar = ac_import.DistributionRestriction.OfType<avm.ITAR>().FirstOrDefault();
                            if (itar != null)
                            {
                                ac_import.DistributionRestriction.Remove(itar);
                            }
                        }

                        #endregion

                        String acmDir = Path.GetDirectoryName(inputFilePath);

                        #region File Management

                        // Create a new backend folder
                        String compDirAbsPath = META.ComponentLibraryManager.CreateComponentFolder(project);
                        String compDirRelPath = MetaAvmProject.MakeRelativePath(projRootPath, compDirAbsPath);

                        // Copy the ACM file to the new path
                        String newACMRelPath = Path.Combine(compDirRelPath, ac_import.Name + ".acm");
                        string newACMAbsPath = Path.Combine(compDirAbsPath, ac_import.Name + ".acm");
                        File.Copy(inputFilePath, newACMAbsPath, true);

                        // Now we have to copy in his resources
                        foreach (var resource in ac_import.ResourceDependency)
                        {
                            try
                            {
                                var dirRelPath = Path.GetDirectoryName(resource.Path);
                                var dirAbsPath = Path.Combine(compDirAbsPath, dirRelPath);

                                var orgAbsPath = Path.Combine(acmDir, resource.Path);
                                var dstAbsPath = Path.Combine(compDirAbsPath, resource.Path);

                                Directory.CreateDirectory(dirAbsPath);
                                File.Copy(orgAbsPath, dstAbsPath, true);
                            }
                            catch (FileNotFoundException)
                            {
                                var message = String.Format("This Component depends on a file which cannot be found in the Component package: {0}", resource.Path);
                                GMEConsole.Warning.WriteLine(message);
                            }
                            catch (DirectoryNotFoundException)
                            {
                                var message = String.Format("This Component depends on a file which cannot be found in the Component package: {0}", resource.Path);
                                GMEConsole.Warning.WriteLine(message);
                            }
                            catch (PathTooLongException)
                            {
                                var message = String.Format("This Component has a Resource that results in a path that is too long: {0}", resource.Path);
                                GMEConsole.Warning.WriteLine(message);
                            }
                            catch (NotSupportedException)
                            {
                                var message = String.Format("This Component has a Resource that could not be loaded: {0}", resource.Path);
                                GMEConsole.Warning.WriteLine(message);
                            }
                            catch (Exception ex)
                            {
                                var message = String.Format("Exception while copying Resource {0}: {1}", resource.Path, ex);
                                GMEConsole.Warning.WriteLine(message);
                            }
                        }
                        #endregion

                        CyPhy.ComponentAssembly cyPhyMLComponentAssembly = null;
                        CyPhy.Components cyPhyMLComponentsFolder = null;

                        CyPhy.Component cyPhyReplaceComponent = null;

                        #region Search for Components that should be Replaced by this new one
                        if (nameComponentMap.TryGetValue(ac_import.Name, out cyPhyReplaceComponent))
                        {
                            bool replace = false;
                            if (!doNotReplaceAll && !replaceAll)
                            {
                                // Present dialog to see if user wants to replace component with AVMID avmid
                                // Maybe have a "do all" checkbox (which sets "replaceAll" to "true") if many items are being imported.
                                // If yes, replace = true;

                                String s_ExistingName = cyPhyReplaceComponent.Name;
                                String s_ExistingAVMID = cyPhyReplaceComponent.Attributes.AVMID;
                                String s_ExistingVersion = cyPhyReplaceComponent.Attributes.Version;
                                String s_ExistingRevision = cyPhyReplaceComponent.Attributes.Revision;

                                String s_ExistingDescriptor = cyPhyReplaceComponent.Name;
                                if (s_ExistingAVMID != "")
                                    s_ExistingDescriptor += "\nAVM ID: " + s_ExistingAVMID;
                                if (s_ExistingVersion != "")
                                    s_ExistingDescriptor += "\nVersion: " + s_ExistingVersion;
                                if (s_ExistingRevision != "")
                                    s_ExistingDescriptor += "\nRevision: " + s_ExistingRevision;

                                String s_NewName = ac_import.Name;
                                //String s_NewAVMID = ac_import.AVMID;
                                String s_NewVersion = ac_import.Version;
                                //String s_NewRevision = ac_import.Revision;

                                String s_NewDescriptor = ac_import.Name;
                                //if (s_NewAVMID != "")
                                //    s_NewDescriptor += "\nAVM ID: " + s_NewAVMID;
                                if (s_NewVersion != "")
                                    s_NewDescriptor += "\nVersion: " + s_NewVersion;
                                //if (s_NewRevision != "")
                                //    s_NewDescriptor += "\nRevision: " + s_NewRevision;

                                String s_MessageBoxPromptTemplate = "Would you like to replace\n\n{0}\n\nwith\n\n{1}";
                                String s_MessageBoxPrompt = String.Format(s_MessageBoxPromptTemplate, s_ExistingDescriptor, s_NewDescriptor);

                                using (UpdatePrompt up = new UpdatePrompt())
                                {
                                    up.DialogText.Text = s_MessageBoxPrompt;
                                    up.ShowDialog();

                                    if (up.DialogResult == DialogResult.Cancel
                                        || up.DialogResult == DialogResult.None)
                                    {
                                        // Skip this component; Continues the "foreach" loop above.
                                        GMEConsole.Error.WriteLine("Import canceled for {0}", inputFile);
                                        continue;
                                    }

                                    Dictionary<DialogResult, string> d_TranslateResult = new Dictionary<DialogResult, string>()
                                    {
                                        { DialogResult.OK, "Replace" },
                                        { DialogResult.Abort, "Import as New"},
                                    };

                                    if (d_TranslateResult[up.DialogResult] == "Replace")
                                    {
                                        replace = true;
                                        if (up.applyToAll.Checked)
                                        {
                                            replaceAll = true;
                                        }
                                    }
                                    else if (d_TranslateResult[up.DialogResult] == "Import as New")
                                    {
                                        if (up.applyToAll.Checked)
                                        {
                                            doNotReplaceAll = true;
                                        }
                                    }
                                }
                            }

                            if (!replace && !replaceAll || doNotReplaceAll)
                            {
                                cyPhyReplaceComponent = null;
                            }
                        }
                        #endregion

                        if (cyPhyReplaceComponent != null)
                        {
                            Object replaceComponentParent = cyPhyReplaceComponent.ParentContainer;
                            if (replaceComponentParent is CyPhy.ComponentAssembly)
                            {
                                cyPhyMLComponentAssembly = replaceComponentParent as CyPhy.ComponentAssembly;
                            }
                            else
                            {
                                cyPhyMLComponentsFolder = replaceComponentParent as CyPhy.Components;
                            }
                        }
                        else
                        {
                            cyPhyMLComponentsFolder = GetImportFolder(rootFolder);
                        }

                        // The importer uses a map to resolve unit references.
                        // If this is our second run, we shouldn't waste time rebuilding it.
                        Boolean b_ResetUnitMap;
                        if (b_FirstComponent)
                            b_ResetUnitMap = true;
                        else
                            b_ResetUnitMap = false;

                        CyPhy.Component c = null;
                        if (cyPhyMLComponentAssembly != null)
                        {
                            c = CyPhy2ComponentModel.Convert.AVMComponent2CyPhyML(cyPhyMLComponentAssembly, ac_import, b_ResetUnitMap, GMEConsole);
                        }
                        else
                        {
                            c = CyPhy2ComponentModel.Convert.AVMComponent2CyPhyML(cyPhyMLComponentsFolder, ac_import, b_ResetUnitMap, GMEConsole);
                        }

                        if (c != null)
                        {
                            importedComponents.Append(c.Impl as MgaFCO);
                            c.Attributes.Path = compDirRelPath;
                        }

                        if (cyPhyReplaceComponent != null)
                        {
                            foreach (IMgaReference reference in (cyPhyReplaceComponent.Impl as IMgaFCO).ReferencedBy)
                            {
                                ReferenceSwitcher.Switcher.MoveReferenceWithRefportConnections(c.Impl as IMgaFCO, reference, WriteLine);
                            }
                            cyPhyReplaceComponent.Delete();
                        }

                        // If icon available, set it in Preferences.
                        // Relative paths here are from our project root.
                        var iconResource = ac_import.ResourceDependency.Where(x => x.Path.Contains("Icon.png")).FirstOrDefault();
                        //foreach (AVM.File file in ac_import.Files)
                        //{
                        //    if (file.Location.Contains("Icon.png"))
                        //        iconRelativePath = componentModelRelativePath + "/" + file.Location;
                        //}
                        if (iconResource != null)
                        {
                            var iconRelativePath = Path.Combine(compDirRelPath, iconResource.Path);

                            // Remove leading "\"
                            if (iconRelativePath.IndexOf("\\") == 0)
                                iconRelativePath = iconRelativePath.Remove(0, 1);

                            // Shrink double backslash to single
                            iconRelativePath = iconRelativePath.Replace("\\\\", "\\");

                            // Replace / with \
                            iconRelativePath = iconRelativePath.Replace('/', '\\');

                            // If icon exists, set it
                            if (System.IO.File.Exists(iconRelativePath))
                            {
                                // Set "icon" regnode
                                (c.Impl as GME.MGA.IMgaFCO).set_RegistryValue("icon", iconRelativePath);
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        string error = string.Format("{0} while importing {1}: {2}", ex.GetType().Name, inputFile, ex.Message);
                        GMEConsole.Error.WriteLine(error);
                        this.Errors.Add(error);
                        GMEConsole.Error.WriteLine(ex.StackTrace);
                    }
                    finally
                    {
                    }
                    b_FirstComponent = false;
                }
            }
            finally
            {
                // Reset the "enabled" state of the CLM AddOn
                META.ComponentLibraryManagerAddOn.Enable(b_CLMAddOnStatus, project);
            }

            return importedComponents;
        }

        public List<string> Errors = new List<string>();

        private void CheckAndWarnOnSchemaVersion(avm.Component ac_import)
        {
            if (!String.IsNullOrWhiteSpace(ac_import.SchemaVersion))
            {
                double version;
                if (double.TryParse(ac_import.SchemaVersion, out version))
                {
                    if (version == ExpectedSchemaVersion)
                    {
                        // Okay.
                    }
                    else
                    {
                        GMEConsole.Warning.WriteLine("{0} has SchemaVersion of {1}. This version of CyPhy expects version {2}. Component may not import correctly.", ac_import.Name, version, ExpectedSchemaVersion);
                    }
                }
                else
                {
                    GMEConsole.Warning.WriteLine("{0} has an unknown SchemaVersion of {1}. This version of CyPhy expects version {2}. Component may not import correctly.", ac_import.Name, ac_import.SchemaVersion, ExpectedSchemaVersion);
                }
            }
            else
            {
                GMEConsole.Warning.WriteLine("{0} has an unknown SchemaVersion. This version of CyPhy expects version {1}. Component may not import correctly.", ac_import.Name, ExpectedSchemaVersion);
            }
        }

        public static avm.Component DeserializeAvmComponentXml(TextReader reader)
        {
            return XSD2CSharp.AvmXmlSerializer.Deserialize<Component>(reader);
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
                Main(project, currentobj, selectedobjs, Convert(param));
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

    public class UnzipToTemp : IDisposable
    {
        public UnzipToTemp(GMEConsole console)
        {
            GMEConsole = console;
        }

        GMEConsole GMEConsole;
        public string TempDirDestination { get; private set; }

        public List<string> UnzipFile(string inputFilePath)
        {
            List<string> ret = new List<string>();
            try
            {

                var tempPath = System.IO.Path.GetTempPath();
                TempDirDestination = Path.Combine(tempPath, Path.GetRandomFileName());
                Directory.CreateDirectory(TempDirDestination);

                using (var zip = ZipStorer.Open(inputFilePath, FileAccess.Read))
                {
                    foreach (var record in zip.ReadCentralDir())
                    {
                        var relDir = Path.GetDirectoryName(record.FilenameInZip);
                        var absDir = Path.Combine(TempDirDestination, relDir);
                        Directory.CreateDirectory(absDir);

                        var absPath = Path.Combine(TempDirDestination, record.FilenameInZip);
                        zip.ExtractFile(record, absPath);
                        
                        ret.Add(record.FilenameInZip);
                    }
                }

            }
            catch (Exception ex)
            {
                throw new IOException("Failed to extract contents from ZIP file.", ex);
            }

            return ret;
        }


        public void Dispose()
        {
                // Clean up temporary directory
                if (!String.IsNullOrWhiteSpace(TempDirDestination))
                {
                    try
                    {
                        Directory.Delete(TempDirDestination, true);
                    }
                    catch (Exception ex)
                    {
                        GMEConsole.Warning.WriteLine("{0} while deleting temporary directory: {1}", ex.GetType().Name, ex.Message);
                    }
                }

        }
    }
}
