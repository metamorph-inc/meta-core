using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using GME.MGA;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;


namespace CyPhyComponentParameterEditor
{
    public partial class ComponentParameterForm : Form
    {
        private MgaFCO currentObj;
        public ComponentParameterForm(MgaFCO currentobj)
        {
            this.currentObj = currentobj;
            InitializeComponent();
        }

        private void btnUpdate_Click(object sender, EventArgs e)
        {
            this.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.Close();
        }

        private void btnConfigParams_Click(object sender, EventArgs e)
        {
            ConfigurationParametersForm confparams = new ConfigurationParametersForm(currentObj);
            if (confparams.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            foreach (var param in confparams.SelectedTemplates)
            {
                try
                {
                    CyPhy.Parameter cyphyparam = param.Create(currentObj);
                    ((BindingList<ComponentParameterItem>)dgvParameters.DataSource).Add(new ComponentParameterItem((MgaFCO)cyphyparam.Impl));
                }
                catch (Exception)
                {
                    // Log something here
                }
            }
        }
    }
}
