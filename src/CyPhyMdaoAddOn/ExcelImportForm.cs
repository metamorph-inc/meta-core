using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CyPhyMdaoAddOn
{
	public partial class ExcelImportForm : Form
	{
		public ExcelImportForm()
		{
			InitializeComponent();
		}

		private void btnAddParameter_Click(object sender, EventArgs e)
		{
			List<string> names = new List<string>();
			foreach (string item in lbNamedCells.SelectedItems)
			{
				names.Add(item);
			}
			foreach (var item in names)
			{
				lbParameters.Items.Add(item);
				lbNamedCells.Items.Remove(item);
			}
		}

		private void btnAddMetric_Click(object sender, EventArgs e)
		{
			List<string> names = new List<string>();
			foreach (string item in lbNamedCells.SelectedItems)
			{
				names.Add(item);
			}
			foreach (var item in names)
			{
				lbMetrics.Items.Add(item);
				lbNamedCells.Items.Remove(item);
			}
		}

		private void btnOK_Click(object sender, EventArgs e)
		{
			DialogResult = System.Windows.Forms.DialogResult.OK;
			Close();
		}

		private void btnRemoveParameter_Click(object sender, EventArgs e)
		{
			List<string> names = new List<string>();
			foreach (string item in lbParameters.SelectedItems)
			{
				names.Add(item);
			}
			foreach (var item in names)
			{
				lbNamedCells.Items.Add(item);
				lbParameters.Items.Remove(item);
			}
		}

		private void btnRemoveMetric_Click(object sender, EventArgs e)
		{
			List<string> names = new List<string>();
			foreach (string item in lbMetrics.SelectedItems)
			{
				names.Add(item);
			}
			foreach (var item in names)
			{
				lbNamedCells.Items.Add(item);
				lbMetrics.Items.Remove(item);
			}
		}

		private void SelectAll(ListBox listBox)
		{
			for (int i = 0; i < listBox.Items.Count; ++i)
			{
				listBox.SetSelected(i, true);
			}
		}

		private void lbNamedCells_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.A &&
				e.Modifiers == Keys.Control)
			{
				SelectAll(lbNamedCells);
			}
		}

		private void lbParameters_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.A &&
				e.Modifiers == Keys.Control)
			{
				SelectAll(lbParameters);
			}
		}

		private void lbMetrics_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.A &&
				e.Modifiers == Keys.Control)
			{
				SelectAll(lbMetrics);
			}
		}

	}
}
