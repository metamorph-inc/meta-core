using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2Modelica_v2.Modelica
{
    public class RedeclareParameter: Parameter
    {
        private CyPhy.ModelicaRedeclare _impl { get; set; }

        public override string Name {
            get
            {
                return string.Format("redeclare {0} {1}", this._impl.Attributes.ModelicaRedeclareType.ToString().ToLowerInvariant(), this._impl.Name); 
            }
        } 

        public RedeclareParameter(CyPhy.ModelicaRedeclare impl)
        {
            this._impl = impl;
        }

        public override int CompareTo(Parameter other)
        {
            if (other is RedeclareParameter)
            {
                return this._impl.Name.CompareTo((other as RedeclareParameter)._impl.Name);
            }
            else
            {
                return this._impl.Name.CompareTo(other.Name);
            }
        }
    }
}
