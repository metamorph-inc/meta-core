using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using Microsoft.Win32;
using System.Collections;

namespace CyPhy2Modelica_v2
{
    public partial class MainForm : Form
    {
        public CyPhy2Modelica_v2Settings ModelicaSettings { get; set; }
        public string ProjectDir { get; set; }
        public string ProgId { get; set; }
        public IList libData { get; set; }

        public MainForm(CyPhy2Modelica_v2Settings settings, string projectDir)
        {
            InitializeComponent();
            this.ProjectDir = projectDir;
            this.ModelicaSettings = settings;
            libData = ((System.Windows.Forms.BindingSource)dataGridView1.DataSource).List;
            var libraries = ModelicaLibrary.GetLibrariesFromSettings(this.ModelicaSettings, this.ProjectDir);
            foreach (var lib in libraries)
            {
                var selectedPath = lib.GetCurrentLibraryPath();
                if (Path.IsPathRooted(selectedPath) == false)
                {
                    selectedPath = Path.Combine(this.ProjectDir, selectedPath);
                }

                if (File.Exists(selectedPath) || Directory.Exists(selectedPath))
                {
                    libData.Add(lib);
                }
            }

            if (string.IsNullOrEmpty(DymolaExe))
            {
                chbCheckWithDymola.Enabled = false;
                chbCheckWithDymola.Checked = false;
            }
            else
            {
                chbCheckWithDymola.Enabled = true;
                chbCheckWithDymola.Checked = this.ModelicaSettings.CheckWithDymola;
            }

            if (string.IsNullOrEmpty(OMCExe))
            {
                chbCheckWithOM.Enabled = false;
                chbCheckWithOM.Checked = false;
            }
            else
            {
                chbCheckWithOM.Enabled = true;
                chbCheckWithOM.Checked = this.ModelicaSettings.CheckWithOpenModelica;
            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = System.Windows.Forms.DialogResult.Cancel;
            Close();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            SaveSettings();
            DialogResult = System.Windows.Forms.DialogResult.OK;
            Close();
        }

        private void SaveSettings()
        {
            ModelicaLibrary.UpdateSettingsFromModelicaLibraries(this.ModelicaSettings, libData.Cast<ModelicaLibrary>().ToList());
            this.ModelicaSettings.CheckWithDymola = chbCheckWithDymola.Checked && chbCheckWithDymola.Enabled;
            this.ModelicaSettings.CheckWithOpenModelica = chbCheckWithOM.Checked && chbCheckWithOM.Enabled;
            this.ModelicaSettings.GenerateConstrainedBys = chbConstrained.Checked;
        }

        private void btnBrowseIncludeDir_Click(object sender, EventArgs e)
        {
            string startupDir = "";
            var lastItem = libData.Cast<ModelicaLibrary>().LastOrDefault();

            if (lastItem != null)
            {
                startupDir = (new DirectoryInfo(Path.GetDirectoryName(lastItem.GetCurrentLibraryPath()))).Parent.FullName;

                if (Path.IsPathRooted(startupDir) == false)
                {
                    startupDir = Path.Combine(this.ProjectDir, startupDir);
                }
            }
            else
            {
                startupDir = this.ProjectDir; 
            }

            var fbd = new OpenFileDialog()
            {
                Multiselect = false,
                Filter = "Modelica files (*.mo)|*.mo",
                InitialDirectory = startupDir,
                Title = "Select Modelica package. If multi-file package; select the package.mo highest up in the file hierarchy."
            };

            var dr = fbd.ShowDialog();

            if (dr == System.Windows.Forms.DialogResult.OK)
            {
                var pathToSave = fbd.FileName;
                // offer relative path if it is available
                Uri uri1 = new Uri(pathToSave);
                Uri uri2 = new Uri(this.ProjectDir + "\\");
                string relativePath = Uri.UnescapeDataString(uri2.MakeRelativeUri(uri1).ToString().Replace('/', '\\'));

                if (relativePath != pathToSave &&
                    Path.GetFullPath(relativePath) == pathToSave)
                {
                    var saveRelative = MessageBox.Show(
                        "Would you like to save the path as relative?",
                        "Save as relative path",
                        MessageBoxButtons.YesNo,
                        MessageBoxIcon.Information);

                    if (saveRelative == System.Windows.Forms.DialogResult.Yes)
                    {
                        pathToSave = relativePath;
                    }
                }

                var modelicaLibrary = ModelicaLibrary.GetLibraryFromPath(pathToSave);

                if (modelicaLibrary == null)
                {
                    MessageBox.Show(pathToSave + "\ndoes not have a valid Modelica package name!",
                        "Invalid package name",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Error);
                    return;
                }

                var existingPackage = libData.Cast<ModelicaLibrary>().Where(lib => lib.Name == modelicaLibrary.Name).FirstOrDefault();
                if (existingPackage == null)
                {
                    libData.Add(modelicaLibrary);
                }
                else
                {
                    existingPackage.UpdateVersionPath(modelicaLibrary.Version, pathToSave);
                    existingPackage.Version = modelicaLibrary.Version;
                }

                dataGridView1.Refresh();
            }
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            SaveSettings();
        }

        private void btnRemoveLib_Click(object sender, EventArgs e)
        {
            var selectedRow = dataGridView1.SelectedRows.Cast<DataGridViewRow>().FirstOrDefault();
            if (selectedRow != null)
            {
                libData.Remove(selectedRow.DataBoundItem as ModelicaLibrary);
                dataGridView1.Refresh();
            }
            //int index = clbIncludeLibs.SelectedIndex;
            //if (index > -1)
            //{
            //    clbIncludeLibs.Items.RemoveAt(index);
            //}
        }

        private static string _dymoleExe = null;

        public static string DymolaExe
        {
            get
            {
                if (_dymoleExe == null)
                {
                    // TODO: get it from the registry
                    string path = @"C:\Program Files (x86)\Dymola 2014\bin64\Dymola.exe";
                    
                    var uninstallKey = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall");

                    SortedDictionary<string, string> dymolaInstallLocations = new SortedDictionary<string, string>();

                    foreach (string subkeyname in uninstallKey.GetSubKeyNames())
                    {
                        var subkey = uninstallKey.OpenSubKey(subkeyname);
                        string name = subkey.GetValue("DisplayName") as string;
                        if (name == null)
                        {
                            continue;
                        }
                        if (name.StartsWith("Dymola"))
                        {
                            try
                            {
                                dymolaInstallLocations.Add(subkey.GetValue("DisplayVersion") as string, subkey.GetValue("InstallLocation") as string);
                            }
                            catch (Exception)
                            {
                                continue;
                            }
                        }
                    }

                    if (dymolaInstallLocations.Count > 0)
                    {
                        path = Path.Combine(dymolaInstallLocations.LastOrDefault().Value, "bin64", "Dymola.exe");
                    }

                    if (File.Exists(path))
                    {
                        _dymoleExe = path;
                    }
                    else
                    {
                        _dymoleExe = string.Empty;
                    }
                }

                return _dymoleExe;
            }
        }

        private static string _omcExe = null;

        public static string OMCExe { 
            get 
            {
                if (_omcExe == null)
                {
                    var omHome = "";
                    try
                    {
                        omHome = Environment.GetEnvironmentVariable("OPENMODELICAHOME");
                    }
                    catch (System.Security.SecurityException)
                    {
                        // User cannot check with OMC in this case
                    }

                    if (string.IsNullOrWhiteSpace(omHome) == false)
                    {
                        var path = Path.Combine(omHome, "bin", "omc.exe");
                        if (File.Exists(path))
                        {
                            _omcExe = path;
                        }
                        else
                        {
                            _omcExe = string.Empty;
                        }
                    }
                    else
                    {
                        _omcExe = string.Empty;
                    }
                }

                return _omcExe;
            }
        }

        private ContextMenuStrip versionMenu;
        private ModelicaLibrary clickedLib;

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

            if (e.ColumnIndex == 0)
            {
                if (e.RowIndex < 0)
                {
                    //Header was clicked.
                    var listModelicaLibrary = libData.Cast<ModelicaLibrary>().ToList();
                    libData.Clear();
                    foreach (var lib in listModelicaLibrary.OrderBy(lib => lib.IsSelected).Reverse())
                    {
                        libData.Add(lib);
                    }
                }
                else
                {
                    var lib = this.dataGridView1.Rows[e.RowIndex].DataBoundItem as ModelicaLibrary;
                    lib.IsSelected = !lib.IsSelected;
                }
            }

            if (e.ColumnIndex == 1)
            {
                if (e.RowIndex < 0)
                {
                    //Header was clicked.
                    var listModelicaLibrary = libData.Cast<ModelicaLibrary>().ToList();
                    libData.Clear();
                    foreach (var lib in listModelicaLibrary.OrderBy(lib => lib.Name))
                    {
                        libData.Add(lib);
                    }
                }
            }

            if (e.ColumnIndex == 2)
            {
                this.clickedLib = this.dataGridView1.Rows[e.RowIndex].DataBoundItem as ModelicaLibrary;
                this.versionMenu = new ContextMenuStrip();
                foreach (var ver in this.clickedLib.Versions.OrderBy(tup => tup.Item1))
                {
                    var item = new ToolStripMenuItem(ver.Item1 + " - " + ver.Item2)
                    {
                        Checked = false,
                        Name = ver.Item1,
                    };

                    item.Click += new EventHandler(toolStripMenuItem_Click);

                    if (ver.Item1 == this.clickedLib.Version)
                    {
                        item.Checked = true;
                    }
                    this.versionMenu.Items.Add(item);
                }

                var cellRectangle = dataGridView1.GetCellDisplayRectangle(2, e.RowIndex, true);
                this.versionMenu.Show(this.dataGridView1, cellRectangle.Location);
            }
        }

        private void toolStripMenuItem_Click(object sender, EventArgs e)
        {
            var menuItem = sender as ToolStripMenuItem;
            if (menuItem.Checked == false)
            {
                foreach (var item in this.versionMenu.Items.Cast<ToolStripMenuItem>())
                {
                    item.Checked = false;
                }
                
                menuItem.Checked = true;
                this.clickedLib.Version = menuItem.Name;
                this.dataGridView1.Refresh();
            }

        }

    }
}
