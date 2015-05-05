using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ISIS.Web
{
    public interface IComponent
    {
        string name { get; set; }
        string id { get; set; }
        string category_name { get; set; }
    }
}
