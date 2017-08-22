using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace JobManager
{
    public partial class RemoteServiceStatusForm : Form
    {
        public RemoteServiceStatusForm()
        {
            InitializeComponent();

            this.InitNodesList();
        }

        private void InitNodesList()
        {
            this.lvRemoteNodes.HeaderStyle = ColumnHeaderStyle.Nonclickable;
            this.lvRemoteNodes.View = View.Details;

            this.lvRemoteNodes.Columns.Add("Status", "Status");
            this.lvRemoteNodes.Columns["Status"].Width = 50;

            this.lvRemoteNodes.Columns.Add("Busy", "Busy");
            this.lvRemoteNodes.Columns["Busy"].Width = 45;

            this.lvRemoteNodes.Columns.Add("Available", "Avail.");
            this.lvRemoteNodes.Columns["Available"].Width = 45;

            this.lvRemoteNodes.Columns.Add("Total", "Total");
            this.lvRemoteNodes.Columns["Total"].Width = 45;

            this.lvRemoteNodes.Columns.Add("Name", "Name");
            this.lvRemoteNodes.Columns["Name"].Width = 80;

            this.lvRemoteNodes.Columns.Add("Description", "Description");
            this.lvRemoteNodes.Columns["Description"].Width = 600;

            //lvJobQueue.AutoResizeColumns(ColumnHeaderAutoResizeStyle.ColumnContent);
        }

    }
}
