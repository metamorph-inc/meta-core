using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ISIS.Web
{
    public interface IJob
    {
        string name { get; set; }
        string id { get; set; }
        //Enum enum_status { get; set; }
    }
}
