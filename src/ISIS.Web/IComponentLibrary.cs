using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;

namespace ISIS.Web
{
    public interface IComponentLibrary<T> where T : IComponent
    {
        IComponentLibrarySearchResult<T> Search(IComponentLibraryFilter filter);

        string ViewComponent(T component);

        string DownloadComponent(T component, string downloadLocation = null, WebClient.DownloadPercent downloadCallback = null);
    }

}
