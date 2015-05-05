using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;

namespace ISIS.Web
{
    public interface IComponentLibraryFilter
    {
        string Category { get; set; }
        int StartPosition { get; set; }
        int NumberOfResults { get; set; }
    }
}
