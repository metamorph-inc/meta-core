using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;
using GME.MGA;
using System.Net;
using GME.CSharp;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using Microsoft.Win32;

namespace ModelicaImporter
{
    public partial class ModelicaModelPicker : Form
    {
        private CyPhyGUIs.GMELogger m_Logger { get; set; }
        private CyPhy.ComponentType m_CyPhyComponent { get; set; }
        private MgaProject m_Project { get; set; }

        private bool m_createMultipleComponents;

        private bool debugLoadLibraries = false;
        private bool debugImportModels = false;

        private BackgroundWorker backgroundWorkerTreeImport = new BackgroundWorker();
        private BackgroundWorker backgroundWorkerComponentImport = new BackgroundWorker();

        private Dictionary<string, CyPhy.ModelicaConnector> localPortMap;

        private int numModelicaModels = 0;

        private int numComponentParams = 0;
        private int numModelicaParams = 0;

        private int numComponentPorts = 0;
        private int numModelicaPorts = 0;

        private const int PARAMETER_START_X = 100;
        private const int PARAMETER_START_Y = 100;
        private const int PARAMETER_ADJUST_Y = 70;

        private const int PORT_START_X = 900;
        private const int PORT_START_Y = 50;
        private const int PORT_ADJUST_Y = 120;

        private const int MODELICA_MODEL_START_X = 450;
        private const int MODELICA_MODEL_ADJUST_Y = 450;

        private CyPhy.RootFolder rootFolder = null;
        private CyPhy.Components ImportedComponentsFolder = null;
        private CyPhy.TestComponents ImportedTestComponentsFolder = null;
        private CyPhy.Ports localPortLibrary { get; set; }
        private CyPhy.Connectors localConnectorLibrary { get; set; }

        private List<ComponentDetail> componentDetails { get; set; }
        private List<string> externalPackagePaths { get; set; }
        private List<string> externalPackageNames { get; set; }
        private Dictionary<string, List<string>> PyModelicaExporterConfig { get; set; } 

        public ModelicaModelPicker(
            bool createMultipleComponents, 
            MgaProject thisProject, 
            CyPhyGUIs.GMELogger logger, 
            CyPhy.ComponentType cyphyComponent=null)
        {
            this.InitializeComponent();

            this.componentDetails = new List<ComponentDetail>();
            this.externalPackagePaths = new List<string>();
            this.externalPackageNames = new List<string>();
            this.UpdateModelPickerDataSource_detailed();

            this.m_Logger = logger;
            this.m_Project = thisProject;
            this.m_createMultipleComponents = createMultipleComponents;
            this.dgvModelPicker.MultiSelect = true;

            //string testFilePath = "";

            //if (File.Exists(testFilePath))
            //{
            //    this.ReadFileAndImportComponents_Flat(testFilePath);
            //    return;
            //}
            
            if (this.m_createMultipleComponents)
            {
                this.gbComponentType.Visible = true;
                this.btnImportSelected.Text = "Import Selected Component(s)";
            }

            if (!this.m_createMultipleComponents && 
                cyphyComponent != null)
            {
                this.m_CyPhyComponent = cyphyComponent;
            }

            this.pictureBoxLoader.Visible = false;
            this.btnImportSelected.Enabled = false;
            this.btnViewWebDoc.Enabled = false;
            this.cbIncludeMSL.Checked = true;
            this.cbIncludeMSL.Enabled = false;

            lbExternalLibList.Items.Add("ModelicaStandardLibrary");

            this.rootFolder = CyPhyClasses.RootFolder.GetRootFolder(this.m_Project);

            if (this.rootFolder != null)
            {
                this.localConnectorLibrary =
                    this.rootFolder.Children.ConnectorsCollection.FirstOrDefault(x => x.Name == "ImportedConnectors");

                if (this.localConnectorLibrary != null)
                {
                    this.localPortLibrary =
                        this.localConnectorLibrary.Children.PortsCollection.FirstOrDefault(x =>
                        x.Name == "ImportedPortTypes");

                    if (this.localPortLibrary != null)
                    {
                        this.localPortMap =
                            this.localPortLibrary.Children.ModelicaConnectorCollection.ToDictionary(mc => mc.Attributes.Class);
                    }
                    else
                    {
                        this.localPortLibrary = CyPhyClasses.Ports.Create(this.localConnectorLibrary);
                        this.localPortLibrary.Name = "ImportedPortTypes";
                        this.localPortMap = new Dictionary<string, CyPhy.ModelicaConnector>();
                    }
                }
                else
                {
                    this.localConnectorLibrary = CyPhyClasses.Connectors.Create(this.rootFolder);
                    this.localConnectorLibrary.Name = "ImportedConnectors";
                    this.localPortLibrary = CyPhyClasses.Ports.Create(this.localConnectorLibrary);
                    this.localPortLibrary.Name = "ImportedPortTypes";
                    this.localPortMap = new Dictionary<string, CyPhy.ModelicaConnector>();
                }
            }
            else
            {
                this.m_Logger.WriteError("Could not get the root folder!");
                return;
            }

            this.backgroundWorkerTreeImport.DoWork += backgroundWorkerTreeImport_DoWork;
            this.backgroundWorkerTreeImport.RunWorkerCompleted += backgroundWorkerTreeImport_RunWorkerCompleted;

            this.backgroundWorkerComponentImport.DoWork += backgroundWorkerComponentImport_DoWork;
            this.backgroundWorkerComponentImport.RunWorkerCompleted += backgroundWorkerComponentImport_RunWorkerCompleted;

            this.numModelicaModels = 0;

            this.numComponentParams = 0;
            this.numModelicaParams = 0;

            this.numComponentPorts = 0;
            this.numModelicaPorts = 0;
        }

        delegate string GetSelectedItemCallback_detailed();
        delegate List<string> GetMultipleSelectedItemsCallback_detailed();

        private string GetSelectedItem_detailed()
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (this.dgvModelPicker.InvokeRequired)
            {
                GetSelectedItemCallback_detailed d = new GetSelectedItemCallback_detailed(GetSelectedItem_detailed);
                return this.Invoke(d, new object[] { }) as string;
            }
            else
            {
                ComponentDetail selectedComponent = null;
                foreach (System.Windows.Forms.DataGridViewRow dgvRow in this.dgvModelPicker.SelectedRows)
                {
                    selectedComponent = dgvRow.DataBoundItem as ComponentDetail;
                    break;
                }

                return selectedComponent.ComponentName;
            }
        }

        private List<string> GetMultipleSelectedItems_detailed()
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (this.dgvModelPicker.InvokeRequired)
            {
                GetMultipleSelectedItemsCallback_detailed d = 
                    new GetMultipleSelectedItemsCallback_detailed(GetMultipleSelectedItems_detailed);
                return this.Invoke(d, new object[] { }) as List<string>;
            }
            else
            {
                List<string> selectedComponentNames = new List<string>();

                foreach (System.Windows.Forms.DataGridViewRow dgvRow in this.dgvModelPicker.SelectedRows)
                {
                    selectedComponentNames.Add((dgvRow.DataBoundItem as ComponentDetail).ComponentName);
                }

                return selectedComponentNames;
            }
        }

        public string GetChosenComponent_detailed()
        {
            return this.GetSelectedItem_detailed();
        }

        private List<string> GetMultipleChosenComponents_detailed()
        {
            return this.GetMultipleSelectedItems_detailed();
        }

        public void ClearComponents_detailed()
        {
            lock (this.componentDetails)
            {
                this.componentDetails.Clear();
            }
            this.UpdateModelPickerDataSource_detailed();
        }

        public void SetComponents_detailed(List<ComponentDetail> componentDetails)
        {
            this.ClearComponents_detailed();
            lock (this.componentDetails)
            {
                this.componentDetails = componentDetails;
            }
            this.UpdateModelPickerDataSource_detailed();
        }

        private void tbSearch_TextChanged(object sender, EventArgs e)
        {
            this.UpdateModelPickerDataSource_detailed();
        }

        delegate void SetDataSourceCallback(IEnumerable<string> components);
        delegate void SetDataSource_Detailed_Callback(IEnumerable<ComponentDetail> componentDetails);

        private void SetDataSource_detailed(IEnumerable<ComponentDetail> componentDetails)
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (this.dgvModelPicker.InvokeRequired)
            {
                SetDataSource_Detailed_Callback d = new SetDataSource_Detailed_Callback(SetDataSource_detailed);
                this.Invoke(d, new object[] { componentDetails });
            }
            else
            {
                this.dgvModelPicker.DataSource = componentDetails;
            }
        }

        private void UpdateModelPickerDataSource_detailed()
        {
            lock (this.componentDetails)
            {
                this.SetDataSource_detailed(
                    this.componentDetails.
                    Where(x => x.ComponentName.IndexOf(this.tbSearch.Text, StringComparison.OrdinalIgnoreCase) >= 0).
                    ToList());
            }
        }

        private void btnImportSelected_Click(object sender, EventArgs e)
        {
            if (ModifierKeys == Keys.Control)
            {
                this.debugImportModels = true;
            }

            if (this.dgvModelPicker.SelectedRows.Count < 1)
            {
                return;
            }

            this.ImportComponentAsync();
        }

        private void ImportComponentAsync()
        {
            if (backgroundWorkerComponentImport.IsBusy)
            {
                return;
            }

            this.pictureBoxLoader.Visible = true;
            this.btnLoadLibraries.Enabled = false;
            this.btnImportSelected.Enabled = false;
            this.btnAddCustomLibrary.Enabled = false;
            this.btnViewWebDoc.Enabled = false;

            this.backgroundWorkerComponentImport.RunWorkerAsync();
        }

        private void btnLoadLibraries_Click(object sender, EventArgs e)
        {
            if (ModifierKeys == Keys.Control)
            {
                this.debugLoadLibraries = true;
            }

            if (backgroundWorkerTreeImport.IsBusy)
            {
                return;
            }

            this.LoadLibrariesAsync();
        }

        private void LoadLibrariesAsync()
        {
            this.pictureBoxLoader.Visible = true;
            this.btnLoadLibraries.Enabled = false;
            this.btnImportSelected.Enabled = false;
            this.btnAddCustomLibrary.Enabled = false;
            this.btnViewWebDoc.Enabled = false;

            this.backgroundWorkerTreeImport.RunWorkerAsync();
        }

        private void btnAddCustomLibrary_Click(object sender, EventArgs e)
        {
            // TODO: The MainForm from CyPhy2Modelieca_v2 needs to be modified/moved to CyPhyGUIs
            // Show the MainForm dialog from CyPhyGUIs... 

            #region RemoveThisBlock
            // This is a temporary solution to add an external library; they are all loaded in the TreeImportWorker

            var fileBrowserDialog = new OpenFileDialog()
            {
                Multiselect = false,
                Filter = "Modelica files (*.mo)|*.mo",
                InitialDirectory = Environment.CurrentDirectory,
                Title = "Select Modelica package. If multi-file package; select the package.mo highest up in the file hierarchy."
            };

            var dialogResult = fileBrowserDialog.ShowDialog();

            if (dialogResult == System.Windows.Forms.DialogResult.OK)
            {
                if (fileBrowserDialog.FileName.EndsWith(".mo") == false)
                {
                    MessageBox.Show(
                        "Modelica packages must have a '.mo' extension. Please try another file.",
                        "Error",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Error);

                    return;
                }

                string path = fileBrowserDialog.FileName;
                string packageDirectory = System.IO.Path.GetDirectoryName(fileBrowserDialog.FileName);
                string packageName = System.IO.Path.GetFileName(path);

                if (packageName == "package.mo")
                {
                    packageName = System.IO.Path.GetFileName(packageDirectory);
                }
                else
                {
                    packageName = System.IO.Path.GetFileNameWithoutExtension(packageName);
                }

                if (!externalPackageNames.Contains(packageName))
                {
                    this.externalPackageNames.Add(packageName);
                    this.externalPackagePaths.Add(path);
                    this.lbExternalLibList.Items.Add(packageName);
                }

                this.cbIncludeMSL.Enabled = true;
            }
            else
            {
                return;
            }
            #endregion
        }

        private void btnViewWebDoc_Click(object sender, EventArgs e)
        {
            try
            {
                if (this.dgvModelPicker.SelectedRows.Count < 1)
                {
                    return;
                }

                string selectedComponent = this.GetSelectedItem_detailed();

                if (selectedComponent.Split('.').FirstOrDefault() != "Modelica")
                {
                    MessageBox.Show("No Web documentation available for this component.");
                }
                else
                {
                    string docUrl = "https://build.openmodelica.org/Documentation/";
                    OpenWebsite(docUrl + selectedComponent + ".html");
                }
            }
            catch (WebException)
            {
                this.m_Logger.WriteError(
                    "Could not open Documentation page; it might not exist for this component, or you might not have an internet connection");
            }
        }

        System.Windows.Forms.ToolTip docBtnToolTip = new System.Windows.Forms.ToolTip();

        private void btnViewWebDoc_MouseHover(object sender, EventArgs e)
        {
            if (this.dgvModelPicker.SelectedRows.Count < 1)
            {
                return;
            }

            string docUrl = "https://build.openmodelica.org/Documentation/";
            string selectedComponent = this.GetSelectedItem_detailed();

            docBtnToolTip.SetToolTip(btnViewWebDoc, string.Format(docUrl + "{0}.html", selectedComponent));
        }

        public static void OpenWebsite(string url)
        {
            Process.Start(GetDefaultBrowserPath(), url);
        }

        private static string GetDefaultBrowserPath()
        {
            string key = @"http\shell\open\command";
            RegistryKey registryKey =
            Registry.ClassesRoot.OpenSubKey(key, false);
            return ((string)registryKey.GetValue(null, null)).Split('"')[1];
        }

        #region BackgroundWorkers

        private List<string> successfullyLoadedPackages = new List<string>();

        private void backgroundWorkerTreeImport_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;

            var metaPythonExePath = META.VersionInfo.PythonVEnvExe;
            var arguments = "-m py_modelica_exporter --standard --json";

            var jsonToImport = "ModelicaPackages.tree.json";

            if (externalPackagePaths.Count != 0)
            {
                if (cbIncludeMSL.Checked == true)
                {
                    arguments = string.Format(
                        "-m py_modelica_exporter --packages \"{0}\" --standard --json", string.Join(";", externalPackagePaths));
                }
                else
                {
                    arguments = string.Format(
                        "-m py_modelica_exporter --packages \"{0}\" --json", string.Join(";", externalPackagePaths));
                }
            }

            this.CallCommand(metaPythonExePath, arguments, Environment.CurrentDirectory);

            var modelicaModelDetails = new List<ComponentDetail>();

            if (File.Exists(jsonToImport))
            {
                var models = Newtonsoft.Json.JsonConvert.DeserializeObject<Dictionary<string, object>>(File.ReadAllText(jsonToImport));
                successfullyLoadedPackages = (models["topLevelPackages"] as Newtonsoft.Json.Linq.JArray).ToObject<List<string>>();
                var modelDetails = (models["classDetails"] as Newtonsoft.Json.Linq.JArray).ToObject<List<ComponentDetail>>();

                modelicaModelDetails.AddRange(modelDetails);
            }
            else
            {
                var message = string.Format("json file was not generated by the python script {0} {1}", metaPythonExePath, arguments);
                throw new FileNotFoundException(message);
            }

            this.SetComponents_detailed(modelicaModelDetails);

            if (this.debugLoadLibraries == false)
            {
                try
                {
                    File.Delete(jsonToImport);
                }
                catch (Exception)
                {
                    this.m_Logger.WriteInfo(
                        "Was not able to delete {0} from the project directory; it might be open in another program.",
                        jsonToImport);
                }
            }
        }

        private void backgroundWorkerTreeImport_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Cancelled == true)
            {
                MessageBox.Show("Canceled!");
            }
            else if (e.Error != null)
            {
                MessageBox.Show("Error: " + e.Error.Message);
            }
            else
            {
                this.lbExternalLibList.DataSource = successfullyLoadedPackages;
                this.lbExternalLibList.Update();

                this.UpdateModelPickerDataSource_detailed();
                this.btnImportSelected.Enabled = true;
                this.btnViewWebDoc.Enabled = true;
                this.pictureBoxLoader.Visible = false;
            }
        }

        private void backgroundWorkerComponentImport_DoWork(object sender, DoWorkEventArgs e)
        {
            string componentsToImport_argument = string.Empty;

            List<string> componentsToImport = this.GetMultipleChosenComponents_detailed();
            componentsToImport_argument = string.Join(";", componentsToImport);

            var metaPythonExePath = META.VersionInfo.PythonVEnvExe;
            string arguments = string.Format("-m py_modelica_exporter --components {0} --json", componentsToImport_argument);

            if (externalPackagePaths.Count != 0)
            {
                arguments = string.Format(
                    "-m py_modelica_exporter --components {0} --packages \"{1}\" --json",
                    componentsToImport_argument,
                    string.Join(";", externalPackagePaths));
            }

            if (this.debugImportModels)
            {
                // The idea is to allow the user to edit the py_modelica_exporter's command-line arguments at runtime.
                this.CallCommand(metaPythonExePath, arguments, Environment.CurrentDirectory);
            }
            else
            {
                this.CallCommand(metaPythonExePath, arguments, Environment.CurrentDirectory);
            }
        }

        private void backgroundWorkerComponentImport_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Cancelled == true)
            {
                MessageBox.Show("Canceled!");
            }
            else if (e.Error != null)
            {
                MessageBox.Show("Error: " + e.Error.Message);
                // Maybe allow the User to try a different Component...?
                this.btnImportSelected.Enabled = true;
                this.btnViewWebDoc.Enabled = true;
                this.btnAddCustomLibrary.Enabled = true;
                this.btnLoadLibraries.Enabled = true;
                this.pictureBoxLoader.Visible = false;
            }
            else
            {
                // py_modelica_exporter has finished in the other thread, read the generated file and import components
                string jsonToImport = "modelica_components.json";

                if (File.Exists(jsonToImport))
                {
                    ReadFileAndImportComponents_Flat(jsonToImport);

                    if (this.debugImportModels == false)
                    {
                        try
                        {
                            File.Delete(jsonToImport);
                        }
                        catch (Exception)
                        {
                            this.m_Logger.WriteInfo(
                                "Was not able to delete {0} from the project directory; it might be open in another program.",
                                jsonToImport);
                        }
                    }
                }
                else
                {
                    var message = string.Format("Component description {0} was not generated by the py_modelica_exporter.", jsonToImport);
                    throw new FileNotFoundException(message);
                }

                this.pictureBoxLoader.Visible = false;
                this.btnLoadLibraries.Enabled = true;
                this.btnImportSelected.Enabled = true;
                this.btnViewWebDoc.Enabled = true;
                this.DialogResult = System.Windows.Forms.DialogResult.OK;
                this.Close();
            }
        }

        #endregion

        private void ReadFileAndImportComponents_Flat(string jsonToImport)
        {
            List<Modelica.ExportedComponent> jsonComponentList =
                Newtonsoft.Json.JsonConvert.DeserializeObject<List<Modelica.ExportedComponent>>(File.ReadAllText(jsonToImport));

            int imported = 0;
            
            if (this.m_createMultipleComponents)
            {
                string folderName = "ModelicaImports";
                CyPhy.TestComponents newTestingImportsFolder = null;
                CyPhy.Components newImportsFolder = null;

                if (this.rbTestComponents.Checked)
                {
                    var newTestingFolder = CyPhyClasses.Testing.Create(this.rootFolder);
                    newTestingFolder.Name = folderName;
                    newTestingImportsFolder = CyPhyClasses.TestComponents.Create(newTestingFolder);
                    newTestingImportsFolder.Name = folderName;
                }
                else
                {
                    newImportsFolder = CyPhyClasses.Components.Create(this.rootFolder);
                    newImportsFolder.Name = folderName;
                }

                foreach (Modelica.ExportedComponent exportedComponentInfo in jsonComponentList)
                {
                    if (exportedComponentInfo.Components.FirstOrDefault().FullName == "Exception")
                    {
                        this.m_Logger.WriteError(
                            "OpenModelica Compiler Error: Could not parse {0}.",
                            exportedComponentInfo.ExportedComponentClass);
                        continue;
                    }

                    this.m_Logger.WriteDebug(
                        "Importing {0}.",
                        exportedComponentInfo.ExportedComponentClass);

                    var flatComponentInfo = FlattenExportedComponent(exportedComponentInfo);

                    if (this.rbTestComponents.Checked)
                    {
                        this.MakeNewTestComponent(newTestingImportsFolder, flatComponentInfo);
                    }
                    else
                    {
                        this.MakeNewComponent(newImportsFolder, flatComponentInfo);
                    }

                    imported++;
                }
            }
            else if (this.m_CyPhyComponent != null)
            {
                foreach (Modelica.ExportedComponent exportedComponentInfo in jsonComponentList)
                {
                    if (exportedComponentInfo.Components.FirstOrDefault().FullName == "Exception")
                    {
                        this.m_Logger.WriteWarning(
                            "OpenModelica Compiler Error: Could not parse {0}.",
                            exportedComponentInfo.ExportedComponentClass);
                        continue;
                    }

                    if (this.m_CyPhyComponent is CyPhyClasses.TestComponent &&
                        this.m_CyPhyComponent.Children.ModelicaModelCollection.Count() != 0)
                    {
                        this.m_Logger.WriteWarning(
                            "TestComponents can contain only 1 ModelicaModel; Skipping {0}",
                            exportedComponentInfo.ExportedComponentClass);
                        continue;
                    }

                    var flatComponentInfo = FlattenExportedComponent(exportedComponentInfo);

                    this.m_Logger.WriteDebug(
                        "Importing {0}.",
                        exportedComponentInfo.ExportedComponentClass);

                    this.AddToExistingComponent(this.m_CyPhyComponent, flatComponentInfo);

                    imported++;
                }
            }
        }

        private Modelica.ExportedComponent FlattenExportedComponent(Modelica.ExportedComponent componentInfo)
        {
            Modelica.Component flatComponent = new Modelica.Component();
            Modelica.ExportedComponent flatComponentInfo = new Modelica.ExportedComponent();

            string componentClass = componentInfo.ExportedComponentClass;
            flatComponent.FullName = componentClass;
            flatComponentInfo.ExportedComponentClass = componentClass;

            flatComponent = ExtractExtendsInfo(flatComponent, componentInfo.Components, componentClass);

            flatComponentInfo.Components.Add(flatComponent);

            return flatComponentInfo;
        }

        private Modelica.Component ExtractExtendsInfo(
            Modelica.Component flatComponent, 
            List<Modelica.Component> componentList, 
            string componentClass)
        {
            Modelica.Component thisComponent = componentList.FirstOrDefault(x=>x.FullName == componentClass);

            var parameterDict = thisComponent.Parameters.ToDictionary(p => p.Name);
            foreach (Modelica.Parameter parameter in flatComponent.Parameters)
            {
                #region Try to get base class parameter info, and save a 'default' value
                Modelica.Parameter extendedClassParameterInfo = null;

                if (parameterDict.TryGetValue(parameter.Name, out extendedClassParameterInfo))
                {
                    parameter.DefaultValue = extendedClassParameterInfo.Value;
                }
                #endregion

                parameterDict[parameter.Name] = parameter;
            }
            flatComponent.Parameters = parameterDict.Values.ToList();

            var connectorDict = thisComponent.Connectors.ToDictionary(p => p.Name);
            foreach (Modelica.Connector connector in flatComponent.Connectors)
            {
                connectorDict[connector.Name] = connector;
            }
            flatComponent.Connectors = connectorDict.Values.ToList();

            var packageDict = thisComponent.Packages.ToDictionary(p => p.Name);
            foreach (Modelica.Package package in flatComponent.Packages)
            {
                #region Try to get base class package info, and save a 'default' value
                Modelica.Package extendedClassPackageInfo = null;

                if (packageDict.TryGetValue(package.Name, out extendedClassPackageInfo))
                {
                    package.DefaultValue = extendedClassPackageInfo.Value;
                }
                #endregion

                packageDict[package.Name] = package;
            }
            flatComponent.Packages = packageDict.Values.ToList();

            foreach (Modelica.Extend extend in thisComponent.Extends)
            {
                parameterDict = extend.Parameters.ToDictionary(p => p.Name);
                foreach (Modelica.Parameter parameter in flatComponent.Parameters)
                {
                    #region Try to get base class parameter info, and save a 'default' value
                    Modelica.Parameter extendedClassParameterInfo = null;

                    if (parameterDict.TryGetValue(parameter.Name, out extendedClassParameterInfo))
                    {
                        parameter.DefaultValue = extendedClassParameterInfo.Value;
                    }
                    #endregion

                    parameterDict[parameter.Name] = parameter;
                }
                flatComponent.Parameters = parameterDict.Values.ToList();

                ExtractExtendsInfo(flatComponent, componentList, extend.FullName);
            }

            return flatComponent;
        }

        private void MakeNewComponent(CyPhy.Components cFolder, Modelica.ExportedComponent flatExportedComponentInfo)
        {
            var newCyPhyComponent = CyPhyClasses.Component.Create(cFolder);
            string cName = flatExportedComponentInfo.ExportedComponentClass.Split('.').LastOrDefault();
            newCyPhyComponent.Name = cName;

            var newModelicaModel = CyPhyClasses.ModelicaModel.Create(newCyPhyComponent);
            newModelicaModel.Name = cName.ToLower();
            newModelicaModel.Attributes.Class = flatExportedComponentInfo.ExportedComponentClass;
            newModelicaModel.Preferences.PortLabelLength = 0;

            //this.placeFco(newModelicaModel as MgaFCO, MODELICA_MODEL_START_X, MODELICA_MODEL_ADJUST_Y);

            foreach (MgaPart item in (newModelicaModel.Impl as MgaFCO).Parts)
            {
                item.SetGmeAttrs(
                    null, 
                    MODELICA_MODEL_START_X,
                    MODELICA_MODEL_ADJUST_Y);
            }

            Modelica.Component modelicaComponentInfo = flatExportedComponentInfo.Components.FirstOrDefault(c => 
                c.FullName == flatExportedComponentInfo.ExportedComponentClass);

            this.m_Logger.WriteDebug("Creating Component '{0}' and ModelicaModel '{1}'", newCyPhyComponent.Name, newModelicaModel.Name);
            AddComponentParts_Flat(newCyPhyComponent, newModelicaModel, modelicaComponentInfo);
        }

        private void MakeNewTestComponent(CyPhy.TestComponents tcFolder, Modelica.ExportedComponent flatExportedComponentInfo)
        {
            var newCyPhyTestComponent = CyPhyClasses.TestComponent.Create(tcFolder);
            string cName = flatExportedComponentInfo.ExportedComponentClass.Split('.').LastOrDefault();
            newCyPhyTestComponent.Name = cName;

            var newModelicaModel = CyPhyClasses.ModelicaModel.Create(newCyPhyTestComponent);
            newModelicaModel.Name = cName.ToLower();
            newModelicaModel.Attributes.Class = flatExportedComponentInfo.ExportedComponentClass;
            newModelicaModel.Preferences.PortLabelLength = 0;

            foreach (MgaPart item in (newModelicaModel.Impl as MgaFCO).Parts)
            {
                item.SetGmeAttrs(
                    null,
                    MODELICA_MODEL_START_X,
                    MODELICA_MODEL_ADJUST_Y);
            }

            Modelica.Component modelicaComponentInfo = flatExportedComponentInfo.Components.FirstOrDefault(c =>
                c.FullName == flatExportedComponentInfo.ExportedComponentClass);

            this.m_Logger.WriteDebug("Creating {0} and {1}.", newCyPhyTestComponent.Name, newModelicaModel.Name);
            AddComponentParts_Flat(newCyPhyTestComponent, newModelicaModel, modelicaComponentInfo);
        }

        private void AddToExistingComponent(CyPhy.ComponentType cyPhyComponentType, Modelica.ExportedComponent flatExportedComponentInfo)
        {
            string cName = flatExportedComponentInfo.ExportedComponentClass.Split('.').LastOrDefault();
            this.numModelicaModels = cyPhyComponentType.Children.ModelicaModelCollection.Count();
            CyPhy.ModelicaModel newModelicaModel = null;

            if (cyPhyComponentType is CyPhy.TestComponent)
            {
                newModelicaModel = CyPhyClasses.ModelicaModel.Create(cyPhyComponentType as CyPhy.TestComponent);
            }
            else if (cyPhyComponentType is CyPhy.Component)
            {
                newModelicaModel = CyPhyClasses.ModelicaModel.Create(cyPhyComponentType as CyPhy.Component);
            }
            else
            {
                return;
            }

            this.numModelicaModels++;

            newModelicaModel.Name = cName.ToLower();
            newModelicaModel.Attributes.Class = flatExportedComponentInfo.ExportedComponentClass;
            newModelicaModel.Preferences.PortLabelLength = 0;

            foreach (MgaPart item in (newModelicaModel.Impl as MgaFCO).Parts)
            {
                item.SetGmeAttrs(
                    null, 
                    MODELICA_MODEL_START_X, 
                    this.numModelicaModels * MODELICA_MODEL_ADJUST_Y);
            } 

            Modelica.Component modelicaComponentInfo = flatExportedComponentInfo.Components.FirstOrDefault(c =>
                c.FullName == flatExportedComponentInfo.ExportedComponentClass);

            this.m_Logger.WriteDebug("Adding {0} to {1}", newModelicaModel.Name, cyPhyComponentType.Name);
            AddComponentParts_Flat(cyPhyComponentType, newModelicaModel, modelicaComponentInfo);
        }

        private void AddComponentParts_Flat(
            CyPhy.ComponentType cyphyComponent,
            CyPhy.ModelicaModel modelicaModel,
            Modelica.Component modelicaComponentInfo)
        {
            this.m_Logger.WriteDebug("Adding properties/parameters for {0}", modelicaModel.Name);
            AddParameters_Flat(cyphyComponent, modelicaModel, modelicaComponentInfo.Parameters);

            this.m_Logger.WriteDebug("Adding connectors for {0}", modelicaModel.Name);
            AddConnectors_Flat(cyphyComponent, modelicaModel, modelicaComponentInfo);
        }

        private void AddParameters_Flat(
            CyPhy.ComponentType cyphyComponent, 
            CyPhy.ModelicaModel modelicaModel, 
            List<Modelica.Parameter> parameters)
        {
            var propCollection = cyphyComponent.Children.PropertyCollection;

            var existingPropertyMap = propCollection.ToDictionary(p => p.Name);

            this.numModelicaParams = 0;
            this.numComponentParams = propCollection.Count();

            parameters.Sort((p1, p2) => p1.Name.CompareTo(p2.Name));

            foreach (Modelica.Parameter p in parameters)
            {
                # region Skip non-public parameters

                if (p.isPublic == false)
                {
                    continue;
                }

                #endregion

                string pValueString;
                CyPhyClasses.Property.AttributesClass.DataType_enum pDataType;

                var makeProperty = this.GetParameterInfo(p, out pValueString, out pDataType);

                if (string.IsNullOrWhiteSpace(pValueString))
                {
                    this.m_Logger.WriteWarning(
                        "Property/Parameter has no value ({0} in {1}, {2}). Please specify a valid value before you include this component in design.",
                        p.Name,
                        (cyphyComponent.Impl as IMgaFCO).ToMgaHyperLink(useName: true),
                        modelicaModel.Name);
                }

                #region Make new Modelica parameter

                CyPhy.ModelicaParameter newModelicaParameter = CyPhyClasses.ModelicaParameter.Create(modelicaModel);
                newModelicaParameter.Name = p.Name;
                newModelicaParameter.Attributes.DefaultValue = pValueString;
                newModelicaParameter.Attributes.Value = pValueString;

                foreach (MgaPart item in (newModelicaParameter.Impl as MgaFCO).Parts)
                {
                    item.SetGmeAttrs(null, PARAMETER_START_X, PARAMETER_START_Y + (this.numModelicaParams * PARAMETER_ADJUST_Y));
                }

                this.numModelicaParams++;

                #endregion

                #region Make or get existing Property or Parameter, and connect to Modelica parameter

                if (makeProperty)
                {
                    CyPhy.Property cyPhyProperty = null;
                    CyPhy.Parameter cyPhyParameter = null;
                    existingPropertyMap.TryGetValue(p.Name, out cyPhyProperty);

                    if (cyPhyProperty == null)
                    {
                        this.m_Logger.WriteDebug("Could not find existing property named {0}", p.Name);

                        if (cyphyComponent is CyPhy.Component)
                        {
                            cyPhyProperty = CyPhyClasses.Property.Create(cyphyComponent as CyPhy.Component);
                            cyPhyProperty.Name = p.Name;
                            cyPhyProperty.Attributes.DefaultValue = pValueString;
                            cyPhyProperty.Attributes.Value = pValueString;
                            cyPhyProperty.Attributes.DataType = pDataType;

                            foreach (MgaPart item in (cyPhyProperty.Impl as MgaFCO).Parts)
                            {
                                item.SetGmeAttrs(null, PARAMETER_START_X, PARAMETER_START_Y + (this.numComponentParams * PARAMETER_ADJUST_Y));
                            }

                            this.numComponentParams++;

                            CyPhy.ModelicaParameterPortMap ppmap = CyPhyClasses.ModelicaParameterPortMap.Connect(cyPhyProperty, newModelicaParameter);
                            ppmap.Name = typeof(CyPhy.ModelicaParameterPortMap).Name;
                        }
                        else if (cyphyComponent is CyPhy.TestComponent)
                        {
                            cyPhyParameter = CyPhyClasses.Parameter.Create(cyphyComponent as CyPhy.TestComponent);
                            cyPhyParameter.Name = p.Name;
                            cyPhyParameter.Attributes.DefaultValue = pValueString;
                            cyPhyParameter.Attributes.Value = pValueString;

                            switch (pDataType)
                            {
                                case CyPhyClasses.Property.AttributesClass.DataType_enum.Float:
                                    cyPhyParameter.Attributes.DataType =
                                        CyPhyClasses.Parameter.AttributesClass.DataType_enum.Float;
                                    break;
                                case CyPhyClasses.Property.AttributesClass.DataType_enum.Integer:
                                    cyPhyParameter.Attributes.DataType =
                                        CyPhyClasses.Parameter.AttributesClass.DataType_enum.Integer;
                                    break;
                                case CyPhyClasses.Property.AttributesClass.DataType_enum.Boolean:
                                    cyPhyParameter.Attributes.DataType =
                                        CyPhyClasses.Parameter.AttributesClass.DataType_enum.Boolean;
                                    break;
                                case CyPhyClasses.Property.AttributesClass.DataType_enum.String:
                                    cyPhyParameter.Attributes.DataType =
                                        CyPhyClasses.Parameter.AttributesClass.DataType_enum.String;
                                    break;
                                default:
                                    break;
                            }


                            foreach (MgaPart item in (cyPhyParameter.Impl as MgaFCO).Parts)
                            {
                                item.SetGmeAttrs(null, PARAMETER_START_X, PARAMETER_START_Y + (this.numComponentParams * PARAMETER_ADJUST_Y));
                            }

                            this.numComponentParams++;

                            CyPhy.ModelicaParameterPortMap ppmap = CyPhyClasses.ModelicaParameterPortMap.Connect(cyPhyParameter, newModelicaParameter);
                            ppmap.Name = typeof(CyPhy.ModelicaParameterPortMap).Name;
                        }
                    }
                    else
                    {
                        CyPhy.ModelicaParameterPortMap ppmap = CyPhyClasses.ModelicaParameterPortMap.Connect(cyPhyProperty, newModelicaParameter);
                        ppmap.Name = typeof(CyPhy.ModelicaParameterPortMap).Name;
                    }
                }
                else
                {
                    this.m_Logger.WriteDebug(
                        "Skipping Property/Parameter {0} ({1})", 
                        p.Name, 
                        pValueString.Replace("\n", string.Empty).Replace("\r", string.Empty));
                }

                #endregion
            }
        }

        private bool GetParameterInfo(
            Modelica.Parameter mParam,
            out string valueString, 
            out CyPhyClasses.Property.AttributesClass.DataType_enum dataType)
        {
            valueString = mParam.Value.ToString();

            switch (mParam.FullName)
            {
                case "Real":
                    dataType = CyPhyClasses.Property.AttributesClass.DataType_enum.Float;
                    return this.GetParameterValueString(mParam, out valueString);
                case "Boolean":
                    dataType = CyPhyClasses.Property.AttributesClass.DataType_enum.Boolean;
                    valueString = valueString.ToLower();
                    return true;
                case "Integer":
                    dataType = CyPhyClasses.Property.AttributesClass.DataType_enum.Integer;
                    return this.GetParameterValueString(mParam, out valueString);
                case "String":
                    dataType = CyPhyClasses.Property.AttributesClass.DataType_enum.String;
                    valueString = string.Format("\"{0}\"", valueString);
                    return true;
                default:
                    dataType = CyPhyClasses.Property.AttributesClass.DataType_enum.String;
                    return false;
            }
        }

        private bool GetParameterValueString(
            Modelica.Parameter mParam, 
            out string valueString)
        {
            valueString = mParam.Value.ToString();
            bool shouldMakeProperty = true;

            int[] safeSet = {0, 1};

            if (safeSet.Contains(mParam.Dimension) == false)
            {
                shouldMakeProperty = false;

                if (mParam.Value is Newtonsoft.Json.Linq.JArray)
                {
                    valueString = valueString.Replace('[', '{');
                    valueString = valueString.Replace(']', '}');
                    valueString = valueString.Replace(" ", "");
                }
            }
            else
            {
                // Could be a dimension-1 array...
                if (valueString.Contains('['))
                {
                    shouldMakeProperty = false;
                }
            }

            return shouldMakeProperty;
        }

        private void AddConnectors_Flat(
            CyPhy.ComponentType cyphyComponent,
            CyPhy.ModelicaModel modelicaModel,
            Modelica.Component modelicaCompInfo) 
        {
            this.numModelicaPorts = 0;
            this.numComponentPorts = cyphyComponent.Children.ConnectorCollection.Count();

            var connectors = modelicaCompInfo.Connectors;
            var packageMap = modelicaCompInfo.Packages.ToDictionary(p => p.Name);

            var existingConnectorMap = cyphyComponent.Children.ConnectorCollection.ToDictionary(c => c.Name);

            connectors.Sort((c1, c2) => c1.Name.CompareTo(c2.Name));

            var modelicaConnectorWithinModelicaModelRoleString = 
                ISIS.GME.Dsml.CyPhyML.Classes.ModelicaConnector.Roles.ModelicaModel.ModelicaConnector.ToString();

            GME.MGA.Meta.MgaMetaRole roleModelicaConnectorInMM =
                ((modelicaModel.Impl as MgaModel).Meta as GME.MGA.Meta.MgaMetaModel).
                RoleByName[modelicaConnectorWithinModelicaModelRoleString];

            foreach (Modelica.Connector connInfo in connectors)
            {
                #region Make new ModelicaConnector in modelicaModel

                CyPhy.ModelicaConnector modelicaConnectorInLibrary = null;
                CyPhy.ModelicaConnector newModelicaConnectorInMM = null;

                if (!this.localPortMap.TryGetValue(connInfo.FullName, out modelicaConnectorInLibrary))
                {
                    string lastName = connInfo.FullName.Split('.').LastOrDefault();

                    modelicaConnectorInLibrary = CyPhyClasses.ModelicaConnector.Create(this.localPortLibrary);
                    modelicaConnectorInLibrary.Name = lastName;
                    modelicaConnectorInLibrary.Attributes.Class = connInfo.FullName; 
                    modelicaConnectorInLibrary.Attributes.Locator = lastName;

                    #region Add parameters/redeclares to the 'library' port

                    foreach (Modelica.RedeclareParameter rp in connInfo.RedeclareParameters)
                    {
                        CyPhy.ModelicaRedeclare newRedeclare =
                            modelicaConnectorInLibrary.Children.ModelicaRedeclareCollection.FirstOrDefault(x => x.Name == rp.Name);

                        if (newRedeclare == null)
                        {
                            newRedeclare = CyPhyClasses.ModelicaRedeclare.Create(modelicaConnectorInLibrary);
                            newRedeclare.Name = rp.Name;
                        }

                        Modelica.Package referredPackage = null;

                        if (packageMap.TryGetValue(rp.Value, out referredPackage))
                        {
                            newRedeclare.Attributes.Value = 
                                referredPackage.DefaultValue == null ? 
                                referredPackage.Value : 
                                referredPackage.DefaultValue;
                            newRedeclare.Attributes.DefaultValue = 
                                referredPackage.DefaultValue == null ? 
                                referredPackage.Value : 
                                referredPackage.DefaultValue;
                        }
                        else
                        {
                            newRedeclare.Attributes.Value = rp.Value;
                        }
                    }

                    string modificationType = null;

                    if (connInfo.Modifiers.TryGetValue("modifications", out modificationType) && modificationType == "public")
                    {
                        foreach (var kvp in connInfo.Modifiers)
                        {
                            if (kvp.Key == "modifications")
                            {
                                continue;
                            }
                            else
                            {
                                CyPhy.ModelicaParameter newPortParameter =
                                    modelicaConnectorInLibrary.Children.ModelicaParameterCollection.FirstOrDefault(x => x.Name == kvp.Key);

                                if (newPortParameter == null)
                                {
                                    newPortParameter = CyPhyClasses.ModelicaParameter.Create(modelicaConnectorInLibrary);
                                    newPortParameter.Name = kvp.Key;
                                }

                                newPortParameter.Attributes.DefaultValue = kvp.Value;
                                newPortParameter.Attributes.Value = kvp.Value;
                            }
                        }
                    }

                    #endregion

                    this.localPortMap[connInfo.FullName] = modelicaConnectorInLibrary;
                }

                // Make a copy in modelicaModel
                newModelicaConnectorInMM = CyPhyClasses.ModelicaConnector.Cast(
                    (modelicaModel.Impl as MgaModel).CopyFCODisp(
                    modelicaConnectorInLibrary.Impl as MgaFCO,
                    roleModelicaConnectorInMM));

                newModelicaConnectorInMM.Name = connInfo.Name;
                newModelicaConnectorInMM.Attributes.Locator = connInfo.Name;

                #region create/overwrite port parameters and redeclares

                foreach (Modelica.RedeclareParameter rp in connInfo.RedeclareParameters)
                {
                    CyPhy.ModelicaRedeclare newRedeclare = 
                        newModelicaConnectorInMM.Children.ModelicaRedeclareCollection.FirstOrDefault(x => x.Name == rp.Name);

                    if (newRedeclare == null)
                    {
                        newRedeclare = CyPhyClasses.ModelicaRedeclare.Create(newModelicaConnectorInMM);
                        newRedeclare.Name = rp.Name;
                    }

                    Modelica.Package referredPackage = null;

                    if (packageMap.TryGetValue(rp.Value, out referredPackage))
                    {
                        newRedeclare.Attributes.Value = referredPackage.Value;
                    }
                    else
                    {
                        newRedeclare.Attributes.Value = rp.Value;
                    }
                }

                string modType = null;

                if (connInfo.Modifiers.TryGetValue("modifications", out modType) && modType == "public")
                {
                    foreach (var kvp in connInfo.Modifiers)
                    {
                        if (kvp.Key == "modifications")
                        {
                            continue;
                        }
                        else
                        {
                            CyPhy.ModelicaParameter newPortParameter = 
                                newModelicaConnectorInMM.Children.ModelicaParameterCollection.FirstOrDefault(x => x.Name == kvp.Key);

                            if (newPortParameter == null)
                            {
                                newPortParameter = CyPhyClasses.ModelicaParameter.Create(newModelicaConnectorInMM);
                                newPortParameter.Name = kvp.Key;
                            }

                            newPortParameter.Attributes.DefaultValue = kvp.Value;
                            newPortParameter.Attributes.Value = kvp.Value;
                        }
                    }
                }

                #endregion

                this.placeFco(newModelicaConnectorInMM.Impl as MgaFCO, PORT_START_X, PORT_START_Y + (this.numModelicaPorts * PORT_ADJUST_Y));

                this.numModelicaPorts++;

                #endregion

                #region Make or get existing Connector & ModelicaConnector

                CyPhy.Connector cyphyConnector = null;
                CyPhy.ModelicaConnector modelicaConnectorWithinCyPhyConnector = null;

                var existingModelicaModels = cyphyComponent.Children.ModelicaModelCollection;

                if (existingConnectorMap.TryGetValue(connInfo.Name, out cyphyConnector))
                {
                    modelicaConnectorWithinCyPhyConnector = 
                        cyphyConnector.Children.ModelicaConnectorCollection.FirstOrDefault(x => 
                            x.Attributes.Class == connInfo.FullName);
                }

                if (modelicaConnectorWithinCyPhyConnector == null)
                {
                    var similarModelicaModel = existingModelicaModels.FirstOrDefault(m => 
                        m.Attributes.Class == modelicaModel.Attributes.Class && 
                        m.ID != modelicaModel.ID);

                    if (similarModelicaModel != null)
                    {
                        modelicaConnectorWithinCyPhyConnector = searchModelicaModelForMatchingModelicaConnector(similarModelicaModel, connInfo);
                    }
                }

                if (modelicaConnectorWithinCyPhyConnector == null && existingModelicaModels.Count() != 0)
                {
                    modelicaConnectorWithinCyPhyConnector = searchModelicaModelsForMatchingModelicaConnector(existingModelicaModels, connInfo);
                }

                if (modelicaConnectorWithinCyPhyConnector == null)
                {
                    modelicaConnectorWithinCyPhyConnector = searchComponentForMatchingModelicaConnector(cyphyComponent, connInfo);
                }

                if (modelicaConnectorWithinCyPhyConnector == null)
                {
                    this.m_Logger.WriteDebug("Could not find existing Connector with name {0}", connInfo.Name);

                    if (cyphyComponent is CyPhy.Component)
                    {
                        cyphyConnector = CyPhyClasses.Connector.Create(cyphyComponent as CyPhy.Component);
                    }
                    else if (cyphyComponent is CyPhy.TestComponent)
                    {
                        cyphyConnector = CyPhyClasses.Connector.Create(cyphyComponent as CyPhy.TestComponent);
                    }

                    foreach (MgaPart item in (cyphyConnector.Impl as MgaFCO).Parts)
                    {
                        item.SetGmeAttrs(null, PORT_START_X, PORT_START_Y + (this.numComponentPorts * PORT_ADJUST_Y));
                    }

                    this.numComponentPorts++;

                    cyphyConnector.Name = connInfo.Name;

                    // Look in the "local" port library for this type; if it don't exist, make it.

                    modelicaConnectorWithinCyPhyConnector = CyPhyClasses.ModelicaConnector.Create(cyphyConnector);
                    modelicaConnectorWithinCyPhyConnector.Name = connInfo.Name;
                    modelicaConnectorWithinCyPhyConnector.Attributes.Class = connInfo.FullName;
                    modelicaConnectorWithinCyPhyConnector.Attributes.Locator = connInfo.Name;
                }

                #endregion

                #region Make connection

                if (modelicaConnectorWithinCyPhyConnector != null &&
                    newModelicaConnectorInMM != null)
                {
                    CyPhy.PortComposition pc = CyPhyClasses.PortComposition.Connect(
                        modelicaConnectorWithinCyPhyConnector, 
                        newModelicaConnectorInMM, 
                        null, 
                        null, 
                        cyphyComponent);
                    pc.Name = typeof(CyPhy.PortComposition).Name;
                }

                #endregion
            }
        }

        private void placeFco(MgaFCO newModConnAsFco, int placementX, int placementY)
        {
            foreach (MgaPart item in newModConnAsFco.Parts)
            {
                item.SetGmeAttrs(null, placementX, placementY);
            }
        }

        private static CyPhy.ModelicaConnector searchModelicaModelForMatchingModelicaConnector(
            CyPhy.ModelicaModel modelicaModel, 
            Modelica.Connector mo_connector)
        {
            CyPhy.ModelicaConnector mc_in_connector = null;

            // get a port in the modelicaModel with matching Name && Class
            CyPhy.ModelicaConnector correspondingModelicaConnector =
                modelicaModel.Children.ModelicaConnectorCollection.FirstOrDefault(x =>
                    x.Name == mo_connector.Name &&
                    x.Attributes.Class == mo_connector.FullName);

            if (correspondingModelicaConnector != null)
            {
                var dstPortCompositions = correspondingModelicaConnector.DstConnections.PortCompositionCollection;
                foreach (var conn in dstPortCompositions)
                {
                    if (conn.DstEnd != null)
                    {
                        mc_in_connector = CyPhyClasses.ModelicaConnector.Cast(conn.DstEnd.Impl);
                        break; // language allows several connections going out, but one is sufficient (?)
                    }
                }

                if (mc_in_connector == null)
                {
                    var srcPortCompositions = correspondingModelicaConnector.SrcConnections.PortCompositionCollection;
                    foreach (var conn in srcPortCompositions)
                    {
                        if (conn.SrcEnd != null)
                        {
                            mc_in_connector = CyPhyClasses.ModelicaConnector.Cast(conn.SrcEnd.Impl);
                            break; // language allows several connections going out, but one is sufficient (?)
                        }
                    }
                }
            }

            return mc_in_connector;
        }

        private static CyPhy.ModelicaConnector searchModelicaModelsForMatchingModelicaConnector(
            IEnumerable<CyPhy.ModelicaModel> existingModelicaModels,
            Modelica.Connector mo_connector)
        {
            CyPhy.ModelicaConnector mc_in_connector = null;

            foreach (CyPhy.ModelicaModel mm in existingModelicaModels)
            {
                mc_in_connector = searchModelicaModelForMatchingModelicaConnector(mm, mo_connector);

                if (mc_in_connector != null)
                {
                    break;
                }
            }

            return mc_in_connector;
        }

        private CyPhy.ModelicaConnector searchComponentForMatchingModelicaConnector(
            CyPhy.ComponentType component, 
            Modelica.Connector mo_connector)
        {
            CyPhy.ModelicaConnector mcWithSameClass = null;

            CyPhy.Connector connectorWithSameName =
                component.Children.ConnectorCollection.FirstOrDefault(x => x.Name == mo_connector.Name);
            if (connectorWithSameName != null)
            {
                mcWithSameClass =
                    connectorWithSameName.Children.ModelicaConnectorCollection.FirstOrDefault(x => x.Attributes.Class == mo_connector.FullName);
            }

            return mcWithSameClass;
        }

        private void AddModifiers(
            CyPhy.ComponentType cyphyComponent, 
            CyPhy.ModelicaModel modelicaModel, 
            Modelica.Component mo_component, 
            Dictionary<string, string> modifiers)
        {
            this.m_Logger.WriteDebug("AddModifiers({0})", mo_component.FullName);

            foreach (var kvp in modifiers)
            {
                if (modelicaModel.Children.ModelicaParameterCollection.Any(x => x.Name == kvp.Key))
                {
                    // if it was already created because of extends do not create it again.
                    continue;
                }

                CyPhy.ModelicaParameter cyphy_mo_parameter_modifier = CyPhyClasses.ModelicaParameter.Create(modelicaModel);
                cyphy_mo_parameter_modifier.Name = kvp.Key;
                cyphy_mo_parameter_modifier.Attributes.DefaultValue = kvp.Value;
                cyphy_mo_parameter_modifier.Attributes.Value = kvp.Value;

                foreach (MgaPart item in (cyphy_mo_parameter_modifier.Impl as MgaFCO).Parts)
                {
                    item.SetGmeAttrs(null, PARAMETER_START_X, PARAMETER_START_Y + (this.numModelicaParams * PARAMETER_ADJUST_Y));
                }

                this.numModelicaParams++;
            }
        }

        private void AddParameters(
            CyPhy.ComponentType cyphyComponent,
            CyPhy.ModelicaModel modelicaModel,
            Modelica.Component mo_component,
            List<Modelica.Parameter> parameters)
        {
            this.m_Logger.WriteDebug("AddParameters({0})", mo_component.FullName);

            foreach (var mo_parameter in parameters)
            {
                if (mo_parameter.isPublic == false)
                {
                    continue;
                }

                #region HandleQudtUnits
                //foreach (var kvp in mo_parameter.Modifications)
                //{
                //    if (kvp.Key == "\"unit\"")
                //    {
                //        ;// TODO: get the QUDT library object
                //    }

                //    //string name = mo_parameter.Name + "." + kvp.Key;
                //    //if (modelicaModel.Children.ModelicaParameterCollection.Any(x => x.Name == name))
                //    //{
                //    //    // if it was already created because of extends do not create it again.
                //    //    continue;
                //    //}

                //    //CyPhy.ModelicaParameter cyphy_mo_parameter_modifier = CyPhyClasses.ModelicaParameter.Create(modelicaModel);
                //    //cyphy_mo_parameter_modifier.Name = name;
                //    //cyphy_mo_parameter_modifier.Attributes.DefaultValue = kvp.Value;
                //    //cyphy_mo_parameter_modifier.Attributes.Value = kvp.Value;

                //    //foreach (MgaPart item in (cyphy_mo_parameter_modifier.Impl as MgaFCO).Parts)
                //    //{
                //    //    item.SetGmeAttrs(null, PARAMETER_START_X, PARAMETER_START_Y + (this.numModelicaParams * PARAMETER_ADJUST_Y));
                //    //}

                //    //this.numModelicaParams++;
                //}
                #endregion

                if (modelicaModel.Children.ModelicaParameterCollection.Any(x => x.Name == mo_parameter.Name))
                {
                    // if it was already created because of extends do not create it again.
                    continue;
                }

                bool makePropertyOrParameter = false;

                CyPhy.ModelicaParameter cyphy_mo_parameter = CyPhyClasses.ModelicaParameter.Create(modelicaModel);
                cyphy_mo_parameter.Name = mo_parameter.Name;
                // TODO: cyphy_mo_parameter.Referred = QUDT_Library_Object_from_above;

                if (mo_parameter.Value is bool)
                {
                    mo_parameter.Value = mo_parameter.Value.ToString().ToLower();
                    makePropertyOrParameter = true;
                }
                else if (
                    mo_parameter.Value is double ||
                    mo_parameter.Value is long ||
                    mo_parameter.Value is short)
                {
                    makePropertyOrParameter = true;
                }
                else if (mo_parameter.Value is string)
                {
                    string stringValue = mo_parameter.Value.ToString();

                    if (string.IsNullOrWhiteSpace(stringValue))
                    {
                        this.m_Logger.WriteWarning(
                            "Property/Parameter has no value ({0} in {1}). Please specify a valid value before you include this component in design.",
                            mo_parameter.Name,
                            (cyphyComponent.Impl as IMgaFCO).ToMgaHyperLink(useName: true));

                        makePropertyOrParameter = true;
                    }
                }
                else if (mo_parameter.Value is Newtonsoft.Json.Linq.JArray)
                {
                    string stringArray = mo_parameter.Value.ToString();

                    stringArray = stringArray.Replace('[', '{');
                    stringArray = stringArray.Replace(']', '}');
                    mo_parameter.Value = stringArray;
                }

                cyphy_mo_parameter.Attributes.DefaultValue = mo_parameter.Value == null ? "" : mo_parameter.Value.ToString();
                cyphy_mo_parameter.Attributes.Value = mo_parameter.Value == null ? "" : mo_parameter.Value.ToString();

                foreach (MgaPart item in (cyphy_mo_parameter.Impl as MgaFCO).Parts)
                {
                    item.SetGmeAttrs(null, PARAMETER_START_X, PARAMETER_START_Y + (this.numModelicaParams * PARAMETER_ADJUST_Y));
                }

                this.numModelicaParams++;

                // if the value cannot be parsed as a 'real' 'bool' or "" (empty string), we shouldn't make a Property/Parameter
                if (!makePropertyOrParameter)
                {
                    continue;
                }

                CyPhyClasses.Property.AttributesClass.DataType_enum propDataType = 
                    CyPhyClasses.Property.AttributesClass.DataType_enum.String;
                CyPhyClasses.Parameter.AttributesClass.DataType_enum paramDataType =
                    CyPhyClasses.Parameter.AttributesClass.DataType_enum.String;

                if (mo_parameter.FullName == "Real")
                {
                    propDataType = CyPhyClasses.Property.AttributesClass.DataType_enum.Float;
                    paramDataType = CyPhyClasses.Parameter.AttributesClass.DataType_enum.Float;
                }
                else if (mo_parameter.FullName == "Boolean")
                {
                    propDataType = CyPhyClasses.Property.AttributesClass.DataType_enum.Boolean;
                    paramDataType = CyPhyClasses.Parameter.AttributesClass.DataType_enum.Boolean;
                }

                if (cyphyComponent is CyPhy.Component)
                {
                    CyPhy.Property property =
                        cyphyComponent.Children.PropertyCollection.FirstOrDefault(x => x.Name == mo_parameter.Name);

                    if (property == null)
                    {
                        property = CyPhyClasses.Property.Create(cyphyComponent as CyPhy.Component);
                        property.Name = mo_parameter.Name;
                        property.Attributes.Description = mo_parameter.Description;
                        property.Attributes.DataType = propDataType;
                        property.Attributes.DefaultValue = mo_parameter.Value == null ? "" : mo_parameter.Value.ToString();
                        property.Attributes.Value = mo_parameter.Value == null ? "" : mo_parameter.Value.ToString();
                        // TODO: property.Referred = QUDT_Library_Object_from_above;

                        foreach (MgaPart item in (property.Impl as MgaFCO).Parts)
                        {
                            item.SetGmeAttrs(null, PARAMETER_START_X, PARAMETER_START_Y + (this.numComponentParams * PARAMETER_ADJUST_Y));
                        }

                        this.numComponentParams++;
                    }

                    CyPhy.ModelicaParameterPortMap ppmap = CyPhyClasses.ModelicaParameterPortMap.Connect(property, cyphy_mo_parameter);
                    ppmap.Name = typeof(CyPhy.ModelicaParameterPortMap).Name;
                }
                else if (cyphyComponent is CyPhy.TestComponent)
                {
                    CyPhy.Parameter parameter =
                        cyphyComponent.Children.ParameterCollection.FirstOrDefault(x => x.Name == mo_parameter.Name);

                    if (parameter == null)
                    {
                        parameter = CyPhyClasses.Parameter.Create(cyphyComponent as CyPhy.TestComponent);
                        parameter.Name = mo_parameter.Name;
                        parameter.Attributes.Description = mo_parameter.Description;
                        parameter.Attributes.DataType = paramDataType;
                        parameter.Attributes.DefaultValue = mo_parameter.Value == null ? "" : mo_parameter.Value.ToString();
                        parameter.Attributes.Value = mo_parameter.Value == null ? "" : mo_parameter.Value.ToString();

                        foreach (MgaPart item in (parameter.Impl as MgaFCO).Parts)
                        {
                            item.SetGmeAttrs(null, PARAMETER_START_X, PARAMETER_START_Y + (this.numComponentParams * PARAMETER_ADJUST_Y));
                        }

                        this.numComponentParams++;
                    }

                    CyPhy.ModelicaParameterPortMap ppmap = CyPhyClasses.ModelicaParameterPortMap.Connect(parameter, cyphy_mo_parameter);
                    ppmap.Name = typeof(CyPhy.ModelicaParameterPortMap).Name;
                }

                // TODO: redeclares
            }
        }

        private void CallCommand(string filename, string arguments, string workingDirectory)
        {
            ProcessStartInfo psi = new ProcessStartInfo()
            {
                Arguments = arguments,
                CreateNoWindow = true,
                FileName = filename,
                WorkingDirectory = workingDirectory,
                RedirectStandardError = true,
                RedirectStandardOutput = true,
                UseShellExecute = false,
            };

            StringBuilder stderr = new StringBuilder();
            StringBuilder stdout = new StringBuilder();

            Process process = new Process()
            {
                StartInfo = psi
            };

            process.OutputDataReceived += ((sender, e) =>
            {
                if (e.Data != null)
                {
                    stdout.Append(e.Data);
                    stdout.Append(Environment.NewLine);
                }
            });

            process.ErrorDataReceived += ((sender, e) =>
            {
                if (e.Data != null)
                {
                    stderr.Append(e.Data);
                    stderr.Append(Environment.NewLine);
                }
            });

            this.m_Logger.WriteDebug("Calling {0} {1} in {2} directory.", filename, arguments, workingDirectory);

            System.Windows.Forms.Application.DoEvents();

            process.Start();

            if (psi.RedirectStandardOutput)
            {
                process.BeginOutputReadLine();
            }
            if (psi.RedirectStandardError)
            {
                process.BeginErrorReadLine();
            }

            process.WaitForExit();

            if (process.ExitCode != 0)
            {
                var msg = System.Security.SecurityElement.Escape(stderr.ToString()).Replace("\n", "<br>");

                if (process.ExitCode == 99)
                {
                    lock (this.m_Logger)
                    {
                        this.m_Logger.WriteError("py_modelica_exporter failed. ExitCode: {0}", process.ExitCode);
                        this.m_Logger.WriteDebug(msg);
                    }

                    throw new Exception(
                        "The current Python parser (pyparser) cannot parse the selected " +
                        "Modelica component. With improvements, this may change in the future. " +
                        "Please select a different component.");
                }
                else
                {
                    lock (this.m_Logger)
                    {
                        this.m_Logger.WriteError(msg);
                        this.m_Logger.WriteError("Python script exporter failed! ExitCode: {0}", process.ExitCode);
                    }

                    throw new Exception("Python script exporter failed!");
                }
            }
        }

        private void cbIncludeMSL_CheckedChanged(object sender, EventArgs e)
        {
            if (cbIncludeMSL.Checked == false)
            {
                lbExternalLibList.Items.Remove("ModelicaStandardLibrary");
            }
            else if (cbIncludeMSL.Checked == true)
            {
                lbExternalLibList.Items.Add("ModelicaStandardLibrary");
            }
        }
    }
}
