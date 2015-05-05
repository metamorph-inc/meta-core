using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CyPhy2Modelica_v2.Modelica 
{
    public class UnitParameter : IComparable<UnitParameter>
    {
        public virtual string Name { get; set; }
        public ModelicaValue Value { get; set; }
        public string Description { get; set; }
        private string _modifer;
        public string Modifier
        {
            get
            {
                if (this._modifer == null)
                {
                    this._modifer = "";
                }

                return this._modifer;
            }
            set
            {
                _modifer = value;
            }
        }

        public virtual int CompareTo(UnitParameter other)
        {
            return this.Name.CompareTo(other.Name);
        }
    }
}
