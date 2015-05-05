using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CADAddOn
{
    public partial class CADPropertySelection : Form
    {
        public CADPropertySelection()
        {
            InitializeComponent();
            doneInitializing = false;
        }

        public void SetInitItemToBeChecked(List<string> itemsToCheck)
        {
            if (itemsToCheck.Count > 0)
            {
  
                for (int i = 0; i < itemsToCheck.Count; i++)
                {
                    string name = itemsToCheck[i];

                    if (name == "Volume")
                    {
                        VolumeCB.Enabled = false;
                        VolumeCB.Checked = true;
                    }                    
                    if (name == "Mass")
                    {
                        MassCB.Enabled = false;
                        MassCB.Checked = true;
                    }
                    if (name == "SurfaceArea")
                    {
                        SurfaceAreaCB.Enabled = false;
                        SurfaceAreaCB.Checked = true;
                    }
                    if (name == "CG")
                    {
                        CGCB.Enabled = false;
                        CGCB.Checked = true;
                    }
                    if (name == "Density")
                    {
                        DensityCB.Enabled = false;
                        DensityCB.Checked = true;
                    }
                }
            }

            doneInitializing = true;
        }
                
        public string selectedProperty { get; set; }
        bool doneInitializing;

        private void CGCB_CheckedChanged(object sender, EventArgs e)
        {
            if (doneInitializing)
            {
                if (CGCB.Checked == false)
                {
                    // renable stuff
                    if (VolumeCB.Checked == false)
                    {
                        VolumeCB.Enabled = true;
                    }

                    if (MassCB.Checked == false)
                    {
                        MassCB.Enabled = true;
                    }

                    if (SurfaceAreaCB.Checked == false)
                    {
                        SurfaceAreaCB.Enabled = true;
                    }

                    if (DensityCB.Checked == false)
                    {
                        DensityCB.Enabled = true;
                    }
                    selectedProperty = "";
                }
                else
                {
                    // disable stuff
                    VolumeCB.Enabled = false;
                    MassCB.Enabled = false;
                    SurfaceAreaCB.Enabled = false;
                    DensityCB.Enabled = false;

                    selectedProperty = "CG";
                }
            }
        }

        private void DensityCB_CheckedChanged(object sender, EventArgs e)
        {
            if (doneInitializing)
            {
                if (DensityCB.Checked == false)
                {
                    // renable stuff
                    if (VolumeCB.Checked == false)
                    {
                        VolumeCB.Enabled = true;
                    }

                    if (MassCB.Checked == false)
                    {
                        MassCB.Enabled = true;
                    }

                    if (SurfaceAreaCB.Checked == false)
                    {
                        SurfaceAreaCB.Enabled = true;
                    }

                    if (CGCB.Checked == false)
                    {
                        CGCB.Enabled = true;
                    }

                    selectedProperty = "";
                }
                else
                {
                    // disable stuff
                    VolumeCB.Enabled = false;
                    MassCB.Enabled = false;
                    SurfaceAreaCB.Enabled = false;
                    CGCB.Enabled = false;

                    selectedProperty = "Density";
                }
            }
        }

        private void MassCB_CheckedChanged(object sender, EventArgs e)
        {
            if (doneInitializing)
            {
                if (MassCB.Checked == false)
                {
                    // renable stuff
                    if (VolumeCB.Checked == false)
                    {
                        VolumeCB.Enabled = true;
                    }

                    if (DensityCB.Checked == false)
                    {
                        DensityCB.Enabled = true;
                    }

                    if (SurfaceAreaCB.Checked == false)
                    {
                        SurfaceAreaCB.Enabled = true;
                    }

                    if (CGCB.Checked == false)
                    {
                        CGCB.Enabled = true;
                    }

                    selectedProperty = "";
                }
                else
                {
                    // disable stuff
                    VolumeCB.Enabled = false;
                    CGCB.Enabled = false;
                    SurfaceAreaCB.Enabled = false;
                    DensityCB.Enabled = false;

                    selectedProperty = "Mass";
                }
            }
        }

        private void SurfaceAreaCB_CheckedChanged(object sender, EventArgs e)
        {
            if (doneInitializing)
            {
                if (SurfaceAreaCB.Checked == false)
                {
                    // renable stuff
                    if (VolumeCB.Checked == false)
                    {
                        VolumeCB.Enabled = true;
                    }

                    if (MassCB.Checked == false)
                    {
                        MassCB.Enabled = true;
                    }

                    if (DensityCB.Checked == false)
                    {
                        DensityCB.Enabled = true;
                    }

                    if (CGCB.Checked == false)
                    {
                        CGCB.Enabled = true;
                    }

                    selectedProperty = "";
                }
                else
                {
                    // disable stuff
                    VolumeCB.Enabled = false;
                    MassCB.Enabled = false;
                    CGCB.Enabled = false;
                    DensityCB.Enabled = false;

                    selectedProperty = "SurfaceArea";
                }
            }
        }

        private void VolumeCB_CheckedChanged(object sender, EventArgs e)
        {
            if (doneInitializing)
            {
                if (VolumeCB.Checked == false)
                {
                    // renable stuff
                    if (DensityCB.Checked == false)
                    {
                        DensityCB.Enabled = true;
                    }

                    if (MassCB.Checked == false)
                    {
                        MassCB.Enabled = true;
                    }

                    if (SurfaceAreaCB.Checked == false)
                    {
                        SurfaceAreaCB.Enabled = true;
                    }

                    if (CGCB.Checked == false)
                    {
                        CGCB.Enabled = true;
                    }

                    selectedProperty = "";
                }
                else
                {
                    // disable stuff
                    CGCB.Enabled = false;
                    MassCB.Enabled = false;
                    SurfaceAreaCB.Enabled = false;
                    DensityCB.Enabled = false;

                    selectedProperty = "Volume";
                }
            }
        }

        private void cancelBT_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void okBT_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
