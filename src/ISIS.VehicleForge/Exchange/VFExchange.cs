using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using ISIS.Web;
using System.Web;
using System.Diagnostics;

namespace ISIS.VehicleForge
{

    public class VFExchange : IComponentLibrary<VFComponent>
    {
        private IWebClient m_vfWebClient { get; set; }

        private string m_defaultDownloadDirectory { get; set; }

        public VFExchange(VFWebClient vfWebClient)
        {
            this.m_vfWebClient = vfWebClient;
            this.m_defaultDownloadDirectory = Path.Combine(Environment.CurrentDirectory, "VFExchangeDownloads");
            vfWebClient.HTTP_WEB_REQUEST_TIMEOUT = 5000;
        }

        private bool searchTreeRecursive(VFOntologyTreeNode treeNode, string category)
        {
            // Check 'this' label, return true if match
            if (treeNode.label == category)
            {
                return true;
            }

            // search child nodes
            foreach (VFOntologyTreeNode childNode in treeNode.children)
            {
                if (searchTreeRecursive(childNode, category))
                {
                    return true;
                }
            }

            return false;
        }

        public bool CheckTreeForCategory(IComponentLibraryFilter filter)
        {
            bool CategoryExists = false;

            try
            {
                VFOntologyTreeNode vfOntologyTree = this.m_vfWebClient.SendGetRequest<VFOntologyTreeNode>("/rest/exchange/ontology/domain/root/json_tree");

                CategoryExists = this.searchTreeRecursive(vfOntologyTree, filter.Category);
            }
            catch (Exception)
            {
                throw;
            }

            return CategoryExists;
        }


        public bool CheckForCategory(IComponentLibraryFilter filter)
        {
            bool CategoryExists = false;

            try
            {
                VFOntologyList vfOntologyList = this.m_vfWebClient.SendGetRequest<VFOntologyList>("/rest/exchange/ontology/term/");

                foreach (VFOntologyInstance instance in vfOntologyList.instances)
                {
                    if (instance.label == filter.Category)
                    {
                        CategoryExists = true;
                    }
                }
            }
            catch (Exception)
            {
                throw;
            }

            return CategoryExists;
        }
        
        public IComponentLibrarySearchResult<VFComponent> Search(IComponentLibraryFilter filter)
        {
            IComponentLibrarySearchResult<VFComponent> result = null;

            if (!this.CheckTreeForCategory(filter))
            {
                return result;
            }

            Dictionary<string, object> searchQuery = new Dictionary<string, object>();
            searchQuery["category_name"] = filter.Category;
            searchQuery["rows"] = filter.NumberOfResults;
            searchQuery["startPos"] = filter.StartPosition;

            var content = "filters=" +
                Newtonsoft.Json.JsonConvert.SerializeObject(
                (filter as VFExchangeFilter).ParameterList, 
                new Newtonsoft.Json.JsonSerializerSettings() { NullValueHandling = Newtonsoft.Json.NullValueHandling.Ignore});

            string contentType = WebClient.ContentType.FormEncoded;

            try
            {
                result = this.m_vfWebClient.SendPostRequest<VFExchangeSearchResult>("/rest/exchange/search/components", searchQuery, content, contentType);
            }
            catch (System.Net.WebException ex)
            {
                // TODO: response/exception is already disposed; need to figure out how to get the type of HTTP error, e.g. 404, 500, ...
                throw new VehicleForge.VFExchangeSearchException();

                //switch ((ex.Response as System.Net.HttpWebResponse).StatusCode)
                //{
                //    case System.Net.HttpStatusCode.NotFound:
                //        throw new VehicleForge.VFExchangeSearchException();
                //        //throw new VehicleForge.VFInvalidURLException();
                //    case System.Net.HttpStatusCode.InternalServerError:
                //        throw new VehicleForge.VFExchangeSearchException();
                //        //throw new VehicleForge.VFInvalidURLException();
                //    default:
                //        throw ex;
                //}
            }

            return result;
        }

        public string ViewComponent(VFComponent component)
        {
            throw new NotImplementedException();
        }

        // should return the local path to the .zip file
        public string DownloadComponent(VFComponent component, string downloadDirectory = null, WebClient.DownloadPercent downloadCallback = null)
        {
            if (string.IsNullOrWhiteSpace(downloadDirectory))
            {
                downloadDirectory = this.m_defaultDownloadDirectory;
            }

            Directory.CreateDirectory(downloadDirectory);

            string zipFileName = Path.Combine(downloadDirectory, component.name + ".zip");

            if (downloadCallback == null)
            {
                downloadCallback = new WebClient.DownloadPercent((percent) => { });
            }

            try
            {
                if (this.m_vfWebClient.DownloadFile(zipFileName, component.zip_url, downloadCallback))
                {
                    // downloaded successfully
                    return zipFileName;
                }
                else
                {
                    // failed to download
                    return null;
                }
            }
            catch (System.Net.WebException webEx)
            {
                Trace.TraceError("WebException thrown when attempting component download.");
                Trace.TraceError("{0}", webEx.Message);
                Trace.TraceError("{0}", webEx.StackTrace);
                throw new VehicleForge.VFDownloadFailedException();
            }
        }
    }

}
