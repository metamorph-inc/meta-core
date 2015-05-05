using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AVM.META
{
    public partial class NamedValue : Value
    {
        public virtual String DataSource
        {
            get;
            set;
        }
    }
}
