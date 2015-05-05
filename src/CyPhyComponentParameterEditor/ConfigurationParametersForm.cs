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
    public partial class ConfigurationParametersForm : Form
    {
        private MgaFCO currentObj;
        public ConfigurationParametersForm(MgaFCO currentobj)
        {
            this.currentObj = currentobj;
            InitializeComponent();
            Activated += new EventHandler(ConfigurationParametersForm_Activated);
        }

        void ConfigurationParametersForm_Activated(object sender, EventArgs e)
        {
            foreach (var template in ConfigurationParameterTemplate.Templates)
            {
                if (template.AppliesTo.Contains(currentObj.MetaBase.Name))
                {
                    AddTemplate(template);
                }
            }
            SendKeys.Send("^{+}"); // Trick: auto-resize columns in teh listview
        }

        private void AddTemplate(ConfigurationParameterTemplate template)
        {
            ListViewItem item = new ListViewItem(){
                Text = template.Name,
                Tag = template
            };
            item.SubItems.Add(new ListViewItem.ListViewSubItem(){
                Text = template.HelpText
            });
            listView_Items.Items.Add(item);
        }

        public IEnumerable<ConfigurationParameterTemplate> SelectedTemplates
        {
            get
            {
                foreach (ListViewItem lvitem in listView_Items.SelectedItems)
                {
                    yield return lvitem.Tag as ConfigurationParameterTemplate;
                }
            }
        }

        private void btnInsert_Click(object sender, EventArgs e)
        {
        }



    }
}
