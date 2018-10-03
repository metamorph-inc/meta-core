using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
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
            dataGridView1.CellValidating += DataGridView1_CellValidating;
            dataGridView1.Columns[0].ReadOnly = true;
            dataGridView1.Columns[1].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            configurationParameterBindingSource.AllowNew = parameters.keywords != null;
        }

        [DllImport("CyPhyPython.dll", CallingConvention = CallingConvention.StdCall)]
        [return: MarshalAs(UnmanagedType.BStr)]
        public static extern string GetExpressionParseError([MarshalAs(UnmanagedType.BStr)] string expression);

        private void DataGridView1_CellValidating(object sender, DataGridViewCellValidatingEventArgs e)
        {
            string headerText = dataGridView1.Columns[e.ColumnIndex].HeaderText;

            if (headerText.Equals("Value"))
            {
                string userInput = dataGridView1.Rows[e.RowIndex].Cells[e.ColumnIndex].EditedFormattedValue.ToString();
                string error = GetExpressionParseError(userInput);
                dataGridView1.Rows[e.RowIndex].ErrorText = error;
                if (error != null)
                {
                    e.Cancel = true;
                }
            }

        }

        private void ConfigurationParameterEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            // remove focus from dataGridView1 to save in-progress changes
            try
            {
                dataGridView1.CurrentCell = null;
            }
            catch (InvalidOperationException)
            {
                // validation failed
                e.Cancel = true;
            }
        }
    }
}
