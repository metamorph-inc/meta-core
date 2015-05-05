using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2Modelica_v2.Modelica
{
    public class Limit : IComparable<Limit>
    {
        public Limit(CyPhy.LimitCheck impl)
        {
            Impl = impl;
        }

        public CyPhy.LimitCheck Impl { get; set; }
        
        public string Name {
            get
            {
                return Impl.Name;
            }
        }
        public CyPhyClasses.LimitCheck.AttributesClass.LimitType_enum LimitType { 
            get      
            {
                return Impl.Attributes.LimitType;
            }
        }
        public string Value
        {
            get
            {
                return Impl.Attributes.Value.ToString();
            }
        }
        public string VariableName
        {
            get
            {
                return Impl.Attributes.VariableName;
            }
        }
        public string ComponentModelicaUri { get; set; }
        public string VariablePath
        {
            get
            {
                return ComponentModelicaUri + "." + VariableName;
            }
        }

        public int CompareTo(Limit other)
        {
            return (this.Name + this.VariablePath).CompareTo(other.Name + other.VariablePath);
        }
    }
}
