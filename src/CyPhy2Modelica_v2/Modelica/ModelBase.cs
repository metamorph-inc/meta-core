using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CyPhy2Modelica_v2.Modelica
{
    public abstract class ModelBase<T> : IComparable<ModelBase<T>> where T : ISIS.GME.Common.Interfaces.FCO
    {

        private string _name = null;

        public string Name
        {
            get
            {
                if (string.IsNullOrWhiteSpace(_name))
                {
                    this._name = this.Impl.Name.Replace(' ', '_');
                }

                return this._name;
            }
            set 
            {
                this._name = value;
            }
        }
        public T Impl { get; set; }
        public int CanvasX { get; set; }
        public int CanvasY { get; set; }

        public ModelBase(T impl)
        {
            this.Impl = impl;
            CanvasX = 0;
            CanvasY = 0;
        }

        public int CompareTo(ModelBase<T> other)
        {
            return this.Name.CompareTo(other.Name);
        }
    }
}
