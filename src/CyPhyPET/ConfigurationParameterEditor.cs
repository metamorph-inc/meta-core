using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using static CyPhyPET.CyPhyPETInterpreter;

namespace CyPhyPET
{
    public partial class ConfigurationParameterEditor : Form
    {
        public ConfigurationParameterEditor(CyPhyPETInterpreter.PythonComponentParameters parameters)
        {
            InitializeComponent();
            this.FormClosing += ConfigurationParameterEditor_FormClosing;
            dataGridView1.UserDeletingRow += (sender, e) =>
            {
                e.Cancel = parameters.requiredArgs.Contains(((ConfigurationParameter)e.Row.DataBoundItem).Name);
            };
            dataGridView1.Columns[0].ReadOnly = true;
            dataGridView1.Columns[1].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            configurationParameterBindingSource.AllowNew = parameters.keywords != null;
            dataGridView1.CellParsing += DataGridView1_CellParsing;
        }

        private void DataGridView1_CellParsing(object sender, DataGridViewCellParsingEventArgs e)
        {
            string headerText = dataGridView1.Columns[e.ColumnIndex].HeaderText;

            if (headerText.Equals("Value"))
            {
                double value;
                if (Double.TryParse((string)e.Value, out value) == false)
                {
                    string strValue = e.Value.ToString();
                    if (strValue[0] != '"')
                    {
                        e.Value = strValue = "\"" + strValue;
                        e.ParsingApplied = true;
                    }
                    if (strValue.Last() != '"')
                    {
                        e.Value = strValue = strValue + "\"";
                        e.ParsingApplied = true;
                    }
                }
            }

        }

        private void ConfigurationParameterEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            // remove focus from dataGridView1 to save in-progress changes
            dataGridView1.CurrentCell = null;
        }
    }
}
