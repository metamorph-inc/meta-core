using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.IO;
using GME.Util;
using GME.MGA;
using META;

namespace CyPhyDecoratorAddon
{
    public partial class InterpreterSelectionForm : Form
    {
        public InterpreterSelectionForm()
        {
            InitializeComponent();
        }

        internal CyPhyDecoratorAddon addon { get; set; }

        internal void Init()
        {
            LoadInterpreters();
        }

        private void LoadInterpreters()
        {
            List<ComComponent> ComComponents = new List<ComComponent>();

            MgaRegistrar registrar = new MgaRegistrar();
            regaccessmode_enum r = regaccessmode_enum.REGACCESS_BOTH;
            IEnumerable components = (IEnumerable)(object)registrar.GetComponentsDisp(r);

            string paradigm = "CyPhyML";

            foreach (string comProgId in components)
            {
                try
                {
                    bool isAssociated;
                    bool canAssociate;

                    registrar.IsAssociated(comProgId, paradigm, out isAssociated, out canAssociate, r);
                    string DllPath = registrar.LocalDllPath[comProgId];

                    componenttype_enum Type;
                    string desc;
                    registrar.QueryComponent(
                        comProgId,
                        out Type,
                        out desc,
                        regaccessmode_enum.REGACCESS_BOTH);

                    bool isInterpreter = false;
                    isInterpreter = (Type == componenttype_enum.COMPONENTTYPE_INTERPRETER);

                    if (canAssociate &&
                        File.Exists(DllPath) &&
                        isInterpreter)
                    {
                        ComComponent component = new ComComponent(comProgId);
                        if (component.isValid)
                        {
                            ComComponents.Add(component);
                        }
                    }
                }
                catch (System.Runtime.InteropServices.COMException)
                {
                    // don't add to list
                }
            }

            lbInterpreters.Items.Clear();
            ComComponents.Sort((Comparison<ComComponent>)((a, b) => a.ToString().CompareTo(b.ToString())));
            foreach (ComComponent c in ComComponents)
            {
                lbInterpreters.Items.Add(c);
            }
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
            Close();
        }


    }
}
