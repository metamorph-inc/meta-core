using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Newtonsoft.Json;

namespace WorkflowDecorator
{
    public partial class ParameterSettingsForm : Form
    {
        public class Parameter
        {
            public string Name { get; set; }
            public string Value { get; set; }
        }

        Parameter paramAnalysis { get; set; }
        public List<Parameter> parameters;

        public ParameterSettingsForm(List<Parameter> list, string taskProgId)
        {
            InitializeComponent();

            paramAnalysis = list.FirstOrDefault(x => x.Name == META.AnalysisTool.ParameterNameInWorkflow);
            if (paramAnalysis != null)
            {
                list.Remove(paramAnalysis);
            }
            else
            {
                paramAnalysis = new Parameter()
                    {
                        Name = META.AnalysisTool.ParameterNameInWorkflow,
                        Value = "Default"
                    };
            }

            parameters = list;

            this.parameterGridView.DataSource = parameters;
            foreach (var item in this.parameterGridView.Columns)
            {
                (item as DataGridViewColumn).SortMode = DataGridViewColumnSortMode.Automatic;
                (item as DataGridViewColumn).HeaderCell.SortGlyphDirection = SortOrder.None;
            }
            parameterGridView.Columns[0].ReadOnly = true;
            (parameterGridView.Columns[0] as DataGridViewColumn).HeaderCell.SortGlyphDirection = SortOrder.Ascending;
            (parameterGridView.Columns[1] as DataGridViewColumn).AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            parameters.Sort(new ParameterComparor() { columnName = "Name" });

            List<string> AnalysisTools = new List<string>() { "Default" };

            // add the name of the saved tool in case we do not have it installed user still can select it.
            AnalysisTools.Add(paramAnalysis.Value);

            var installedTools = META.AnalysisTool.GetFromProgId(taskProgId);
            installedTools.ForEach(x => AnalysisTools.Add(x.Name));

            // list should contain unique tool names
            AnalysisTools = AnalysisTools.Distinct().ToList();

            this.cmbAnalysisToolSelection.DataSource = AnalysisTools;
            this.cmbAnalysisToolSelection.SelectedItem = paramAnalysis.Value;
        }

        private void parameterGridView_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            string columnName = parameterGridView.Columns[e.ColumnIndex].Name;
            parameters.Sort(new ParameterComparor() { columnName = columnName });
            foreach (DataGridViewColumn column in parameterGridView.Columns)
            {
                column.HeaderCell.SortGlyphDirection = SortOrder.None;
            }
            parameterGridView.Columns[e.ColumnIndex].HeaderCell.SortGlyphDirection = SortOrder.Ascending;
            parameterGridView.DataSource = null;
            parameterGridView.DataSource = parameters;
            (parameterGridView.Columns[1] as DataGridViewColumn).AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
        }

        class ParameterComparor : IComparer<Parameter>
        {
            public string columnName { get; set; }
            public int Compare(Parameter x, Parameter y)
            {
                return ((IComparable)x.GetType().GetProperty(columnName).GetValue(x, null))
                    .CompareTo(y.GetType().GetProperty(columnName).GetValue(y, null));
            }
        }

        private void ParameterSettingsForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.paramAnalysis.Value = this.cmbAnalysisToolSelection.SelectedItem.ToString();
            this.parameters.Add(this.paramAnalysis);
        }
    }
}
