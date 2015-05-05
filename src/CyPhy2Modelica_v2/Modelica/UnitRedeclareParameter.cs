using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2Modelica_v2.Modelica
{
    public class UnitRedeclareParameter: UnitParameter
    {
        public UnitRedeclareParameter(CyPhy.ModelicaRedeclare impl)
        {
            this._impl = impl;
        }

        private CyPhy.ModelicaRedeclare _impl { get; set; }

        public override string Name {
            get
            {
                return string.Format("redeclare {0} {1}", this._impl.Attributes.ModelicaRedeclareType.ToString().ToLowerInvariant(), this._impl.Name); 
            }
        }

        public override int CompareTo(UnitParameter other)
        {
            if (other is UnitRedeclareParameter)
            {
                return this._impl.Name.CompareTo((other as UnitRedeclareParameter)._impl.Name);
            }
            else
            {
                return this._impl.Name.CompareTo(other.Name);
            }
        }
    }
}
