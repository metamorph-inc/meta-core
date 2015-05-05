using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ISIS.Web
{
    public interface IWebClient
    {
        string ServerUrl { get; }

        //string RestUrl { get; }

        // TODO: make is void and raise exception on failure
        bool UploadFile(string fileName, string url, WebClient.UploadPercent callback);

        // TODO: make is void and raise exception on failure
        bool DownloadFile(string fileName, string url, WebClient.DownloadPercent callback);

        T SendGetRequest<T>(string url, Dictionary<string, object> query = null) where T : new();

        T SendPostRequest<T>(string url, Dictionary<string, object> query = null, string content = null, string contentType = null) where T : new();

        string SendGetRequest(string url, Dictionary<string, object> query = null);
        
        string SendPostRequest(string url, Dictionary<string, object> query = null, string content = null, string contentType = null);
    }
}
