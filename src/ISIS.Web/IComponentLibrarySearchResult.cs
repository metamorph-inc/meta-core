using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ISIS.Web
{
    public interface IComponentLibrarySearchResult<T> where T : IComponent
    {
        int hits { get; set; }               // total number of results from search
        List<T> components { get; set; }     // actual component descriptions returned by filtered search
    }
}



