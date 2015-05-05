using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Web;
using System.Threading;

namespace ISIS.Web
{
    public class WebClient : IWebClient
    {
        public struct ContentType
        {
            /// <summary>
            /// HTTP Content-Type header value: application/x-www-form-urlencoded
            /// </summary>
            public static string FormEncoded = "application/x-www-form-urlencoded";
        }

        private bool _AllowAutoRedirect = false;
        public bool AllowAutoRedirect
        {
            get { return _AllowAutoRedirect; }
            set { _AllowAutoRedirect = value; }
        }

        protected string m_serverUrl { get; set; }

        public virtual int HTTP_WEB_REQUEST_TIMEOUT { get; set; }
        public virtual int HTTP_WEB_REQUEST_ASYNC_TIMEOUT { get; set; }

        public bool LoggingEnabled { get; set; }

        public CookieContainer AuthCookies { get; set; }

        public delegate void UploadPercent(int percent);
        public delegate void DownloadPercent(int percent);

        public string ServerUrl
        {
            get { return this.m_serverUrl; }
        }

        //public string RestUrl { get; set; }

        public WebClient(string serverUrl, bool loggingEnabled = true)
            : base()
        {
            this.m_serverUrl = serverUrl;
            this.AuthCookies = new CookieContainer();
            this.LoggingEnabled = loggingEnabled;
            this.HTTP_WEB_REQUEST_TIMEOUT = (int)TimeSpan.FromSeconds(20).TotalMilliseconds;
            this.HTTP_WEB_REQUEST_ASYNC_TIMEOUT = (int)TimeSpan.FromMinutes(10).TotalMilliseconds;
        }

        public virtual bool UploadFile(string fileName, string url, UploadPercent callback)
        {
            string fullUrl = this.GetFullUrl(url);

            try
            {
                try
                {
                    using (CookieAwareWebClient webClient = new CookieAwareWebClient())
                    {
                        webClient.AuthCookies = this.AuthCookies;
                        webClient.Timeout = this.HTTP_WEB_REQUEST_TIMEOUT;

                        Semaphore progress = new Semaphore(0, int.MaxValue);
                        Semaphore completed = new Semaphore(0, 1);

                        UploadFileCompletedEventArgs completedData = null;

                        int maxProgress = 0;
                        object maxProgressLock = new object();
                        webClient.UploadFileCompleted += delegate(object sender, UploadFileCompletedEventArgs e)
                        {
                            completedData = e;
                            completed.Release();
                        };
                        webClient.UploadProgressChanged += delegate(object sender, UploadProgressChangedEventArgs e)
                        {
                            lock (maxProgressLock)
                            {
                                int percentProgress = (int)(((double)e.BytesSent / e.TotalBytesToSend) * 99.0);
                                if (percentProgress > maxProgress)
                                {
                                    // e.ProgressPercentage is not correct, as (almost) all the time is spent uploading
                                    maxProgress = percentProgress;
                                    callback(maxProgress);
                                }
                            }
                            progress.Release();
                        };

                        webClient.UploadFileAsync(new Uri(fullUrl), "PUT", fileName);

                        while (true)
                        {
                            WaitHandle[] handles = new WaitHandle[] { completed, progress };
                            int timeout = webClient.Timeout;
                            lock (maxProgressLock)
                            {
                                // Workaround for Fiddler proxy: Fiddler accepts all the data at once, then uploads to VF
                                if (maxProgress > 98)
                                {
                                    handles = new WaitHandle[] { completed };
                                    timeout = this.HTTP_WEB_REQUEST_ASYNC_TIMEOUT;
                                }
                            }
                            int handle = WaitHandle.WaitAny(handles, timeout);
                            if (handle == 0)
                            {
                                lock (maxProgressLock)
                                {
                                    maxProgress = 100;
                                    callback(maxProgress);
                                }
                                break;
                            }
                            if (handle == System.Threading.WaitHandle.WaitTimeout)
                            {
                                webClient.CancelAsync();
                                throw new WebException("Connection timed out");
                            }
                            if (handle != 1)
                                throw new Exception();
                        }

                        Trace.TraceInformation("Upload to " + Encoding.UTF8.GetString(completedData.Result));
                        return true;
                    }
                }
                catch (System.Reflection.TargetInvocationException ex)
                {
                    throw ex.InnerException;
                }

            }
            catch (WebException ex)
            {
                Trace.TraceError("Error uploading to {0}. Exception follows", fullUrl);
                Trace.TraceError(ex.ToString());
            }
            return false;
        }

        public virtual bool DownloadFile(string fileName, string url, DownloadPercent callback)
        {
            Func<bool> action = () => DownloadFileAppropriately(fileName, url, callback);
            try
            {
                var result = action.BeginInvoke(null, null);
                return action.EndInvoke(result);
            }
            catch (System.Reflection.TargetInvocationException ex)
            {
                throw ex.InnerException;
            }
        }

        public virtual bool DownloadFileAppropriately(string fileName, string url, DownloadPercent callback)
        {
            string fullUrl = this.GetFullUrl(url);
            Trace.TraceInformation("Downloading artifacts from {0} to {1}", fullUrl, fileName);

            int maxProgress = 0;
            object maxProgressLock = new object();

            try
            {
                string status = "Connecting ...";
                using (CookieAwareWebClient webClient = new CookieAwareWebClient())
                {
                    webClient.AuthCookies = this.AuthCookies;
                    webClient.Timeout = this.HTTP_WEB_REQUEST_TIMEOUT;
                    //webClient.AllowReadStreamBuffering = false;

                    Semaphore progress = new Semaphore(0, int.MaxValue);
                    Semaphore completed = new Semaphore(0, 1);
                    Exception downloadError = null;
                    webClient.DownloadFileCompleted += delegate(object sender, System.ComponentModel.AsyncCompletedEventArgs e)
                    {
                        downloadError = e.Error;
                        completed.Release();
                    };
                    webClient.DownloadProgressChanged += delegate(object sender, DownloadProgressChangedEventArgs e)
                    {
                        lock (maxProgressLock)
                        {
                            int percentProgress = (int)(((double)e.BytesReceived / e.TotalBytesToReceive) * 99.0);
                            if (percentProgress > maxProgress)
                            {
                                // e.ProgressPercentage is not correct, as (almost) all the time is spent uploading
                                maxProgress = percentProgress;
                                callback(maxProgress);
                                status = String.Format("at {0}%", percentProgress);
                            }
                        }
                        progress.Release();
                    };

                    webClient.DownloadFileAsync(new Uri(fullUrl), fileName);

                    while (true)
                    {
                        WaitHandle[] handles = new WaitHandle[] { completed, progress };
                        int timeout = webClient.Timeout;
                        lock (maxProgressLock)
                        {
                            // Workaround for Fiddler proxy: Fiddler downloads all the data at once, then passes it on to clients
                            if (maxProgress < 2)
                            {
                                handles = new WaitHandle[] { completed };
                                timeout = this.HTTP_WEB_REQUEST_ASYNC_TIMEOUT;
                            }
                        }
                        int handle = WaitHandle.WaitAny(handles, timeout);
                        if (handle == 0)
                        {
                            lock (maxProgressLock)
                            {
                                maxProgress = 100;
                                callback(maxProgress);
                            }
                            break;
                        }
                        if (handle == System.Threading.WaitHandle.WaitTimeout)
                        {
                            webClient.CancelAsync();
                            throw new WebException(String.Format("Connection timed out {0}", status));
                        }
                        if (handle != 1)
                        {
                            //TODO: Kevin, please specify the failure condition. Not clear what state we are in.
                            throw new WebException();
                        }
                    }
                    if (downloadError != null)
                    {
                        throw downloadError;
                    }

                    Trace.TraceInformation("Download from {0} succeeded {1}", fullUrl, fileName);
                    return true;
                }
            }
            catch (System.Reflection.TargetInvocationException ex)
            {
                throw ex.InnerException;
            }
        }

        public virtual T SendGetRequest<T>(string url, Dictionary<string, object> query = null) where T : new()
        {
            string response = this.SendGetRequest(url, query);
            T result = Newtonsoft.Json.JsonConvert.DeserializeObject<T>(response);
            return result;
        }

        public virtual T SendPostRequest<T>(string url, Dictionary<string, object> query = null, string content = null, string contentType = null) where T : new()
        {
            string response = this.SendPostRequest(url, query, content, contentType);
            T result = Newtonsoft.Json.JsonConvert.DeserializeObject<T>(response);
            return result;
        }

        public virtual string SendGetRequest(string url, Dictionary<string, object> query = null)
        {
            return this.SendRawGetRequest(url, query);
        }

        protected string SendRawGetRequest(string url, Dictionary<string, object> query = null)
        {
            string fullUrl = this.GetFullUrl(url, query);
            HttpWebResponse response = null;
            string responseFromServer = string.Empty;

            Trace.TraceInformation(String.Format("SendRawGetRequest to: [{0}]", url));

            try
            {
                HttpWebRequest request = (HttpWebRequest)WebRequest.Create(fullUrl);
                request.AllowAutoRedirect = false;
                using (request as IDisposable)
                {
                    request.Timeout = this.HTTP_WEB_REQUEST_TIMEOUT;
                    request.KeepAlive = false;
                    request.Method = "GET";
                    request.AllowAutoRedirect = this.AllowAutoRedirect;
                    request.CookieContainer = this.AuthCookies;

                    // Get the response.
                    try
                    {
                        response = (HttpWebResponse)request.GetResponse();
                    }
                    catch (WebException e)
                    {
                        IDisposable disp = e.Response as IDisposable;
                        if (disp != null)
                            disp.Dispose();
                        throw;
                    }
                }

                using (response)
                using (Stream dataStream = response.GetResponseStream())
                {
                    // Open the stream using a StreamReader for easy access.
                    using (StreamReader reader = new StreamReader(dataStream))
                    {
                        // Read the content.
                        responseFromServer = reader.ReadToEnd();
                    }
                }
            }
            catch (Exception ex)
            {
                if (this.LoggingEnabled)
                {
                    Trace.TraceInformation("GET " + fullUrl);
                    Trace.TraceError(ex.ToString());
                }

                throw ex;
            }

            return responseFromServer;
        }

        public virtual string SendPostRequest(string url, Dictionary<string, object> query = null, string content = null, string contentType = null)
        {
            return this.SendRawPostRequest(url, query, content, contentType);
        }

        protected string SendRawPostRequest(string url, Dictionary<string, object> query = null, string content = null, string contentType = null)
        {

            string responseFromServer = null;

            using (HttpWebResponse response = SendRawPostRequestGetResponse(url, query, content, contentType))
            {

                try
                {
                    // Get the stream containing content returned by the server.
                    using (Stream dataStream = response.GetResponseStream())
                    {
                        // Open the stream using a StreamReader for easy access.
                        using (StreamReader reader = new StreamReader(dataStream))
                        {
                            // Read the content.
                            responseFromServer = reader.ReadToEnd();
                        }
                    }

                }
                catch (Exception ex)
                {
                    if (this.LoggingEnabled)
                    {
                        Trace.TraceError(ex.ToString());
                    }
                    else
                    {
                        Trace.TraceError("Exception occured but logging is disabled");
                    }

                    throw;
                }
            }
            return responseFromServer;

        }

        protected HttpWebResponse SendRawPostRequestGetResponse(
            string url,
            Dictionary<string, object> query = null,
            string content = null,
            string contentType = null)
        {
            Trace.TraceInformation(String.Format("SendRawPostRequestGetResponse: [{0}]", url));

            // TODO: review this method!
            string fullUrl = this.GetFullUrl(url, query);
            Console.WriteLine(String.Format("Post from {0}", fullUrl));
            if (string.IsNullOrEmpty(content))
            {
                content = "";
            }

            string logMessage = string.Empty;

            // Create a request using a URL that can receive a post. 
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(fullUrl);
            HttpWebResponse response = null;

            using (request as IDisposable)
            {
                request.Timeout = this.HTTP_WEB_REQUEST_TIMEOUT;
                request.KeepAlive = false;
                request.Method = "POST";
                request.AllowAutoRedirect = this.AllowAutoRedirect;

                request.CookieContainer = this.AuthCookies;
                // Create POST data and convert it to a byte array.
                byte[] byteArray = Encoding.UTF8.GetBytes(content);
                // Set the ContentType property of the WebRequest.
                request.ContentType = contentType;
                // Set the ContentLength property of the WebRequest.
                request.ContentLength = byteArray.Length;
                // Get the request stream.
                using (Stream dataStream = request.GetRequestStream())
                {
                    // Write the data to the request stream.
                    dataStream.Write(byteArray, 0, byteArray.Length);
                }


                // Get the response.
                try
                {
                    response = request.GetResponse() as HttpWebResponse;
                }
                catch (WebException e)
                {
                    if (this.LoggingEnabled)
                    {
                        Trace.TraceError(e.ToString());

                        using (response = e.Response as HttpWebResponse)
                        {
                            logMessage = response.Method + " " + request.Address + " " + (int)response.StatusCode;
                            Trace.TraceWarning(logMessage);

                            using (Stream data = response.GetResponseStream())
                            {
                                string text = new StreamReader(data).ReadToEnd();
                                logMessage = "\t" + text.Replace("\n", "");
                                Trace.TraceWarning(logMessage);
                            }
                        }
                        throw;
                    }
                    else
                    {
                        using (response = e.Response as HttpWebResponse)
                        {
                            Trace.TraceError("Exception occured (" + (int)response.StatusCode + ") but logging is disabled");
                        }
                        throw;
                    }
                }

                if (this.LoggingEnabled)
                {
                    logMessage = response.Method + " " + request.Address + " " + (int)response.StatusCode;
                    Trace.TraceInformation(logMessage);
                }
            }

            return response;
        }

        private string GetFullUrl(string url, Dictionary<string, object> query = null)
        {
            StringBuilder fullUrl = new StringBuilder(this.m_serverUrl);

            fullUrl.Append(url);

            // dictionary key value pair to query string
            StringBuilder encodedQuery = new StringBuilder();
            if (query != null)
            {
                foreach (var kvp in query)
                {
                    if (query.FirstOrDefault().Key == kvp.Key)
                    {
                        fullUrl.Append("?");
                    }
                    fullUrl.AppendFormat("{0}={1}", kvp.Key, HttpUtility.UrlEncode(kvp.Value.ToString()));
                    if (query.LastOrDefault().Key != kvp.Key)
                    {
                        fullUrl.Append("&");
                    }
                }
            }

            return fullUrl.ToString();
        }

        public virtual string SendPutRequest(string url)
        {
            throw new NotImplementedException();
        }

        public virtual string SendDeleteRequest(string url)
        {
            throw new NotImplementedException();
        }
    }
}
