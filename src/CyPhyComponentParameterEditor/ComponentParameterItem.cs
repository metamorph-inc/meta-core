// -----------------------------------------------------------------------
// <copyright file="ComponentParameterItem.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace CyPhyComponentParameterEditor
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
using GME.MGA;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class ComponentParameterItem
    {
        private MgaFCO m_fco { get; set; }

        public string Name
        {
            get { return this.m_fco.Name; }
            set { this.m_fco.Name = value; }
        }

        public string Value
        {
            get { return this.m_fco.StrAttrByName["Value"]; }
            set { this.m_fco.StrAttrByName["Value"] = value; }
        }

        public string Unit
        {
            get
            {
                if (this.m_fco is MgaReference)
                {
                    var reference = this.m_fco as MgaReference;
                    if (reference.Referred != null)
                    {
                        return reference.Referred.StrAttrByName["Symbol"];
                    }
                }
                return "";
            }
            set
            {
            }
        }

        public List<string> Category { get; set; }

        public string Description
        {
            get { return this.m_fco.StrAttrByName["Description"]; }
            set { this.m_fco.StrAttrByName["Description"] = value; }
        }

        public ComponentParameterItem(MgaFCO fco)
        {
            this.m_fco = fco;

            this.Category = new List<string>();
        }
    }
}
