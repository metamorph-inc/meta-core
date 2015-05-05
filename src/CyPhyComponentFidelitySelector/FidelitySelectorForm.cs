using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CyPhyComponentFidelitySelector
{
    public partial class FidelitySelectorForm : Form
    {
        private const string Classification = "Classification";
        private const string ModelicaModelOptions = "ModelicaModelOptions";

        // List for the data grid view control
        public List<ComponentItem> componentItems = new List<ComponentItem>();
        public string FidelitySettings { get; set; }

        public List<string> consoleMessages { get; set; }

        public FidelitySelectorForm()
        {
            this.consoleMessages = new List<string>();

            InitializeComponent();
        }

        public void PopulateDgv()
        {
            var fidelity = new Dictionary<string, Dictionary<string, string>>();

            try
            {
                fidelity = Newtonsoft.Json.JsonConvert.DeserializeObject<Dictionary<string, Dictionary<string, string>>>(this.FidelitySettings);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
            }

            dgvData.AutoGenerateColumns = false;
            dgvData.DataSource = componentItems;
            dgvData.DataBindingComplete += (o, e) =>
            {
                foreach (DataGridViewRow row in dgvData.Rows)
                {
                    var cboCell = new DataGridViewComboBoxCell
                    {
                        DataSource = ((ComponentItem)row.DataBoundItem).ModelicaModelOptions,
                        ValueMember = "Key",
                        DisplayMember = "Name",
                    };
                    row.Cells[0] = cboCell;
                    row.Cells[0].Value = ((ComponentItem)row.DataBoundItem).ModelicaModelOptions.FirstOrDefault().Key;

                    // update value based on saved settings from FidelitySettings
                    var currentSettings = new Dictionary<string, string>();
                    if (fidelity.TryGetValue(((ComponentItem)row.DataBoundItem).Classification, out currentSettings))
                    {
                        string savedValue = ((ComponentItem)row.DataBoundItem).ModelicaModelOptions.FirstOrDefault().Name;
                        if (currentSettings.TryGetValue("ModelicaModel", out savedValue))
                        {
                            var opt = ((ComponentItem)row.DataBoundItem).ModelicaModelOptions.FirstOrDefault(x => x.Name == savedValue);
                            if (opt != null)
                            {
                                row.Cells[0].Value = opt.Key;
                            }
                        }
                    }
                }
            };

            dgvData.Columns.Add(
                new DataGridViewComboBoxColumn
                {
                    HeaderText = ModelicaModelOptions,
                    Name = ModelicaModelOptions
                });
            dgvData.Columns[0].Width = 200;

            dgvData.Columns.Add(
                new DataGridViewTextBoxColumn
                {
                    DataPropertyName = Classification,
                    HeaderText = Classification,
                    Name = Classification
                });
            dgvData.Columns[1].Width = 200;
            dgvData.Columns[1].ReadOnly = true;

            //dgvData.AutoResizeRows(DataGridViewAutoSizeRowsMode.AllCells);
            //dgvData.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.AllCells);
            //dgvData.Update();
        }

        private void btnSaveAndClose_Click(object sender, EventArgs e)
        {
            Dictionary<string, Dictionary<string, string>> settings =
                new Dictionary<string, Dictionary<string, string>>();

            foreach (DataGridViewRow row in dgvData.Rows)
            {
                var selection = new Dictionary<string, string>();

                selection.Add("ModelicaModel", row.Cells[ModelicaModelOptions].FormattedValue as string);
                // TODO: add CAD

                settings.Add(row.Cells[Classification].Value as string, selection);

                string settingInfo = string.Format(
                    "Components with Classification '{0}' will use ModelicaModel '{1}'",
                    row.Cells[Classification].Value as string,
                    row.Cells[ModelicaModelOptions].FormattedValue as string);

                this.consoleMessages.Add(settingInfo);
            }

            this.FidelitySettings = Newtonsoft.Json.JsonConvert.SerializeObject(settings, Newtonsoft.Json.Formatting.Indented);
            this.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.Close();
        }
    }
}
