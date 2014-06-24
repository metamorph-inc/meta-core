// -----------------------------------------------------------------------
// <copyright file="Jenkins.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace JobManagerFramework.Jenkins
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Net;
    using System.IO;
    using System.Web;
    using System.Runtime.InteropServices;
    using System.Threading.Tasks;
    using System.Threading;
    using System.Diagnostics;

    public class WebExceptionWithStatusCode : WebException
    {
        public int StatusCode { get; private set; }
        public WebExceptionWithStatusCode(string message, int statusCode)
            : base(message)
        {
            StatusCode = statusCode;
        }
    }

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class Jenkins
    {
        private static int HTTP_WEB_TIMEOUT_BASE = 1000 * 30;
        private static int HTTP_WEB_REQUEST_TIMEOUT_BASE = 1000 * 30;
        public static int HTTP_WEB_REQUEST_TIMEOUT = 1000 * 60 * 10;
        private static TimeSpan VF_SESSION_TIMEOUT = TimeSpan.FromMinutes(9);

        /// <summary>
        /// Number of miliseconds for the JobManager timers that monitors jobs through the VF API.
        /// </summary>
        public const int VF_JOB_POLL_FREQUENCY = 30000;

        /// <summary>
        /// Test VF connection and see if the user is logged in or not.
        /// </summary>
        /// <returns>True if VF connection is alive and we have a logged in session, otherwise false</returns>
        public bool PingVF()
        {
            UserProfile.UserProfile info = null;
            return this.PingVF(out info);
        }

        /// <summary>
        /// Test VF connection and see if the user is logged in or not.
        /// </summary>
        /// <param name="info">Gets details about the user.</param>
        /// <returns>True if VF connection is alive and we have a logged in session, otherwise false</returns>
        public bool PingVF(out UserProfile.UserProfile info)
        {
            bool result = false;
            info = null;
            
            try
            {
                info = this.GetUserProfile(rethrow:true);
                result = true;
            }
            catch (Exception ex)
            {
                // TODO: refine it to webexception and error codes
                Trace.TraceError(ex.ToString());
                result = false;
            }

            return result;
        }

        /// <summary>
        /// Tests the remote slaves/executors.
        /// </summary>
        /// <returns>True if Jenkins master is alive and accesible and it is not detected as DOWN by VF, otherwise false</returns>
        public bool PingJenkins()
        {
            StatusInfo.StatusInfo statusInfo = null;
            return this.PingJenkins(out statusInfo);
        }

        /// <summary>
        /// Tests the remote slaves/executors.
        /// </summary>
        /// <param name="statusInfo">Detailed status information about the Jenkins service.</param>
        /// <returns>True if Jenkins master is alive and accesible and it is not detected as DOWN by VF, otherwise false</returns>
        public bool PingJenkins(out StatusInfo.StatusInfo statusInfo)
        {
            bool result = false;
            statusInfo = null;

            try
            {
                statusInfo = this.GetStatusInfo(rethrow: true);
                result = statusInfo.status != "DOWN";
                Trace.TraceInformation(
                    "Test bench service status: {0}",
                    statusInfo.status);
            }
            catch (Exception ex)
            {
                // TODO: refine it to webexception and error codes
                Trace.TraceError(ex.ToString());
                Trace.TraceInformation("Test bench service status: DISCONNECTED");
                result = false;
            }
            
            return result;
        }


        private string SendPostRequest(
            string url,
            string content = "",
            string contentType = null)
        {
            HttpWebResponse response;

            string ret;
            try
            {
                ret = SendPostRequest(url, content, contentType, out response);
                if (response != null)
                    (response as IDisposable).Dispose();
            }
            catch (Exception ex)
            {
                Trace.TraceError(ex.ToString());
                return null;
            }

            return ret;
        }
        private string SendPostRequest(
            string url,
            string content,
            string contentType,
            out HttpWebResponse response,
            bool isLogging = true)
        {
            string logMessage = string.Empty;
            string responseFromServer = null;
            response = null;

            // Create a request using a URL that can receive a post. 
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
            using (request as IDisposable)
            {
                request.Timeout = HTTP_WEB_REQUEST_TIMEOUT;
                request.KeepAlive = false;
                request.Method = "POST";

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
                    if (isLogging)
                    {
                        Trace.TraceError(e.ToString());

                        using (response = e.Response as HttpWebResponse)
                        {
                            logMessage = response.Method + " " + request.Address + " " + (int)response.StatusCode;
                            DebugLog.WriteLine(logMessage);
                            Trace.TraceWarning(logMessage);

                            using (Stream data = response.GetResponseStream())
                            {
                                string text = new StreamReader(data).ReadToEnd();
                                logMessage = "\t" + text.Replace("\n", "");
                                DebugLog.WriteLine(logMessage);
                                Trace.TraceWarning(logMessage);
                            }
                        }
                        throw;
                    }
                    else
                    {
                        using (response = e.Response as HttpWebResponse)
                            Trace.TraceError("Exception occured (" + (int)response.StatusCode + ") but logging is disabled");
                        throw;
                    }
                }

                if (isLogging)
                {
                    logMessage = response.Method + " " + request.Address + " " + (int)response.StatusCode;
                    DebugLog.WriteLine(logMessage);
                    Trace.TraceInformation(logMessage);
                }

                try
                {
                    // Display the status.
                    //Console.WriteLine(((HttpWebResponse)response).StatusDescription);
                    // Get the stream containing content returned by the server.
                    using (Stream dataStream = response.GetResponseStream())
                    {
                        // Open the stream using a StreamReader for easy access.
                        using (StreamReader reader = new StreamReader(dataStream))
                        {
                            // Read the content.
                            responseFromServer = reader.ReadToEnd();
                            // Display the content.
                            //Console.WriteLine(responseFromServer);
                        }
                    }
                }
                catch (Exception ex)
                {
                    if (isLogging)
                    {
                        Trace.TraceError(ex.ToString());
                    }
                    else
                    {
                        Trace.TraceError("Exception occured but logging is disabled");
                    }

                    (response as IDisposable).Dispose();
                    response = null;
                    throw;
                }
            }
            return responseFromServer;
        }

        private void SendGetRequest(string url, out HttpWebResponse response, bool isLogging = true)
        {
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
            using (request as IDisposable)
            {
                request.Timeout = HTTP_WEB_REQUEST_TIMEOUT_BASE;
                request.KeepAlive = false;
                request.Method = "GET";
                request.CookieContainer = this.AuthCookies;

                // Get the response.
                try
                {
                    response = (HttpWebResponse)request.GetResponse();
                }
                catch (WebException e)
                {
                    int statusCode = 0;
                    if (e.Response != null && e.Response is HttpWebResponse)
                    {
                        statusCode = (int)((HttpWebResponse)e.Response).StatusCode;
                    }
                    IDisposable disp = e.Response as IDisposable;
                    if (disp != null)
                        disp.Dispose();

                    throw new WebExceptionWithStatusCode(e.Message, statusCode);
                }
            }
        }

        /// <summary>
        /// Sends a get request to a given url.
        /// </summary>
        /// <param name="url">Server's url</param>
        /// <param name="isLogging">Puts all exceptions and requests into the log file.</param>
        /// <param name="rethrow">Rethrows the exception that occured.</param>
        /// <returns></returns>
        private string SendGetRequest(string url, bool isLogging = true, bool rethrow = false)
        {
            string responseFromServer = null;
            HttpWebResponse response;

            try
            {
                SendGetRequest(url, out response, isLogging);
                using (response)
                using (Stream dataStream = response.GetResponseStream())
                {
                    // Open the stream using a StreamReader for easy access.
                    using (StreamReader reader = new StreamReader(dataStream))
                    {
                        // Read the content.
                        responseFromServer = reader.ReadToEnd();
                        // Display the content.
                        //Console.WriteLine(responseFromServer);
                    }
                }
            }
            catch (Exception ex)
            {
                if (isLogging)
                {
                    Trace.TraceInformation("GET " + url);
                    Trace.TraceError(ex.ToString());
                }

                if (rethrow)
                {
                    throw;
                }
            }

            return responseFromServer;
        }


        public struct Queries
        {
            /// <summary>
            /// User profile details, we may use its content in the future.
            /// Now it is being used to ping vehicleforge and making sure the user is logged in.
            /// </summary>
            public const string USER_PROFILE = "/rest/user/get_user_profile";

            /// <summary>
            /// Detailed status information about the Jenkins master and slaves.
            /// </summary>
            public const string STATUS_INFO = "/testbench/info.json";

            public const string INFO = "/testbench/api/json";
            public const string JOB_INFO = "/testbench/job/{0}/api/json?depth=0";
            public const string BUILD_INFO = "/testbench/job/{0}/{1}/api/json";
            public const string Q_INFO = "/testbench/queue/api/json?depth=0";
            public const string CREATE_JOB = "/testbench/createItem?name={0}"; //also post config.xml
            public const string CONFIG_JOB = "/testbench/job/{0}/config.xml";
            public const string DELETE_JOB = "/testbench/job/{0}/doDelete";
            public const string ENABLE_JOB = "/testbench/job/{0}/enable";
            public const string DISABLE_JOB = "/testbench/job/{0}/disable";
            public const string STOP_JOB = "/testbench/job/{0}/stop";
            public const string COPY_JOB = "/testbench/createItem?name={0}&mode=copy&from={1}";
            public const string BUILD_JOB = "/testbench/job/{0}/build?filename={1}&{2}";
            public const string WIPE_OUT_WORKSPACE = "/testbench/job/{0}/doWipeOutWorkspace";
            public const string MARK_HIGH_PRIORITY = "/testbench/job/{0}/make_high_priority";

            public const string LAST_BUILD_CONSOLE_TEXT = "/testbench/job/{0}/lastBuild/consoleText";

            //public const string CREATE_NODE = "computer/doCreateItem?{0}";
            //public const string DELETE_NODE = "computer/{0}/doDelete";
            //public const string NODE_INFO = "computer/{0}/api/json?depth=0";
            //public const string NODE_TYPE = "hudson.slaves.DumbSlave$DescriptorImpl";
        }

        public Job.Job GetJobInfo(string name, bool rethrow=false)
        {
            Login();
            string response = SendGetRequest(ServerUrl + String.Format(Queries.JOB_INFO, HttpUtility.UrlEncode(name)), rethrow: rethrow);
            //Console.WriteLine(response);
            Job.Job jobInfo = null;
            if (response != null)
            {
                try
                {
                    jobInfo = Newtonsoft.Json.JsonConvert.DeserializeObject<Job.Job>(response);
                }
                catch (Newtonsoft.Json.JsonException ex)
                {
                    Trace.TraceError(ex.ToString());
                    if (rethrow)
                    {
                        throw;
                    }
                }
            }
            return jobInfo;
        }

        public dynamic GetBuildInfo(string name, string buildNumberOrLastBuild)
        {
            Login();
            string response = SendGetRequest(ServerUrl + String.Format(Queries.BUILD_INFO, name, HttpUtility.UrlEncode(buildNumberOrLastBuild)));
            //Console.WriteLine(response);
            if (response != null)
            {
                try
                {
                    dynamic buildInfo = Newtonsoft.Json.Linq.JObject.Parse(response);
                    return buildInfo;
                }
                catch (Newtonsoft.Json.JsonException ex)
                {
                    Trace.TraceError(ex.ToString());
                }
            }
            return null;
        }

        //public object Open(string url)
        //{
        //    // for authentication...
        //    throw new NotImplementedException();
        //}


        public object GetQueueInfo()
        {
            Login();
            string response = SendGetRequest(ServerUrl + Queries.Q_INFO);
            //Console.WriteLine(response);
            return response;
        }

        public StatusInfo.StatusInfo GetStatusInfo(bool rethrow = false)
        {
            Login();
            string response = SendGetRequest(ServerUrl + Queries.STATUS_INFO, rethrow:rethrow);
            //Console.WriteLine(response);
            StatusInfo.StatusInfo result = null;
            if (response != null)
            {
                try
                {
                    result = Newtonsoft.Json.JsonConvert.DeserializeObject<StatusInfo.StatusInfo>(response);
                }
                catch (Newtonsoft.Json.JsonException ex)
                {
                    Trace.TraceError(ex.ToString());
                }
            }
            return result;
        }

        public UserProfile.UserProfile GetUserProfile(bool rethrow = false)
        {
            Login();
            string response = SendGetRequest(ServerUrl + Queries.USER_PROFILE, rethrow: rethrow);
            //Console.WriteLine(response);
            UserProfile.UserProfile result = null;
            if (response != null)
            {
                try
                {
                    result = Newtonsoft.Json.JsonConvert.DeserializeObject<UserProfile.UserProfile>(response);
                }
                catch (Newtonsoft.Json.JsonException ex)
                {
                    Trace.TraceError(ex.ToString());
                }
            }
            return result;
        }


        public Info.Info GetInfo(bool rethrow = false)
        {
            Login();
            string response = SendGetRequest(ServerUrl + Queries.INFO, rethrow: rethrow);
            //Console.WriteLine(response);
            Info.Info info = null;
            if (response != null)
            {
                try
                {
                    info = Newtonsoft.Json.JsonConvert.DeserializeObject<Info.Info>(response);
                }
                catch (Newtonsoft.Json.JsonException ex)
                {
                    Trace.TraceError(ex.ToString());
                }
            }
            return info;
        }

        public List<Info.Job> GetJobs()
        {
            var info = GetInfo();
            if (info != null)
            {
                return info.jobs;
            }
            else
            {
                return new List<Info.Job>();
            }
        }


        public object CopyJob(
            string from_name,
            string to_name)
        {
            throw new NotImplementedException();
        }

        public bool DeleteJob(string name)
        {
            Login();
            var jobinfo = GetJobInfo(name);
            if (jobinfo != null)
            {
                Cookie session_id = AuthCookies.GetCookies(new Uri(ServerUrl)).Cast<Cookie>().First(x => x.Name == "_session_id");
                string response = SendPostRequest(
                    ServerUrl + String.Format(String.Format(Queries.DELETE_JOB, HttpUtility.UrlEncode(name))),
                    String.Format("_session_id={0}", HttpUtility.UrlEncode(session_id.Value)),
                    "application/x-www-form-urlencoded");
                //Console.WriteLine(response);
            }
            return true;
        }

        public void WipeWorkSpace(string name)
        {
            Login();
            var jobinfo = GetJobInfo(name);
            if (jobinfo != null)
            {
                Cookie session_id = AuthCookies.GetCookies(new Uri(ServerUrl)).Cast<Cookie>().First(x => x.Name == "_session_id");
                string response = SendPostRequest(
                    ServerUrl + String.Format(String.Format(Queries.WIPE_OUT_WORKSPACE, HttpUtility.UrlEncode(name))),
                    String.Format("_session_id={0}", HttpUtility.UrlEncode(session_id.Value)),
                    "application/x-www-form-urlencoded");
                //Console.WriteLine(response);
            }
        }

        public void MakeHighPriority(string name)
        {
            Login();
            var jobinfo = GetJobInfo(name);
            if (jobinfo != null)
            {
                Cookie session_id = AuthCookies.GetCookies(new Uri(ServerUrl)).Cast<Cookie>().First(x => x.Name == "_session_id");
                string response = SendPostRequest(
                    ServerUrl + String.Format(String.Format(Queries.MARK_HIGH_PRIORITY, HttpUtility.UrlEncode(name))),
                    String.Format("_session_id={0}", HttpUtility.UrlEncode(session_id.Value)),
                    "application/x-www-form-urlencoded");
                //Console.WriteLine(response);
            }
        }

        public void EnableJob(string name)
        {
            Login();
            var jobinfo = GetJobInfo(name);
            if (jobinfo != null)
            {
                string response = SendPostRequest(ServerUrl + String.Format(Queries.ENABLE_JOB, HttpUtility.UrlEncode(name)));
                //Console.WriteLine(response);
            }
        }

        public void DisableJob(string name)
        {
            Login();
            var jobinfo = GetJobInfo(name);
            if (jobinfo != null)
            {
                string response = SendPostRequest(ServerUrl + String.Format(Queries.DISABLE_JOB, HttpUtility.UrlEncode(name)));
                //Console.WriteLine(response);
            }
        }

        /// <summary>
        /// Stops the last build on the server
        /// </summary>
        /// <param name="name">Name of the job</param>
        public void StopJob(string name)
        {
            Login();
            var jobinfo = GetJobInfo(name);
            if (jobinfo != null)
            {
                Cookie session_id = AuthCookies.GetCookies(new Uri(ServerUrl)).Cast<Cookie>().First(x => x.Name == "_session_id");
                string response = SendPostRequest(
                    ServerUrl + String.Format(String.Format(Queries.STOP_JOB, HttpUtility.UrlEncode(name))),
                    String.Format("_session_id={0}", HttpUtility.UrlEncode(session_id.Value)),
                    "application/x-www-form-urlencoded");
                //Console.WriteLine(response);
            }
        }

        public bool JobExists(string name)
        {
            Login();
            var info = GetJobInfo(name);
            return info == null ? false : true;
        }

        public string Username { get; set; }
        public string Email { get; set; }
        public string Password { get; set; }
        public CookieContainer AuthCookies { get; set; }
        public StreamWriter DebugLog { get; set; }

        DateTime LastLogin = DateTime.MinValue;

        public void Login()
        {
            if (LastLogin.Add(VF_SESSION_TIMEOUT) > DateTime.Now)
            {
                return;
            }

            // META-447: trash our cookies to get a new session id
            AuthCookies = new CookieContainer();
            Trace.TraceInformation("Logging in: {0} as {1}", ServerUrl, this.Username);

            HttpWebResponse response;
            // DO NOT LOG PASSWORD!

            // Get session id
            SendGetRequest(ServerUrl + "/auth/", out response);
            (response as IDisposable).Dispose();

            SendPostRequest(ServerUrl + "/auth/do_login", String.Format("username={0}&password={1}",
                HttpUtility.UrlEncode(Username), HttpUtility.UrlEncode(Password)),
                "application/x-www-form-urlencoded", out response, isLogging: false);

            using (response)
            {
                Trace.TraceInformation("Login response: {0} {1}", ServerUrl, response.StatusCode);

                if (response.ResponseUri != null && response.ResponseUri.ToString().EndsWith("/auth/do_login"))
                {
                    Trace.TraceInformation("Credentials incorrect", ServerUrl, response.StatusCode);
                    throw new Exception("Authentication credentials are incorrect");
                }

                LastLogin = DateTime.Now;
            }
        }

        public Job.Job CreateJob(
            string name,
            string config_xml)
        {
            Trace.TraceInformation("Creating job: {0} with Config xml: {2}{1}{2}",
                name,
                config_xml,
                Environment.NewLine);

            Login();
            Job.Job jobinfo = null; // GetJobInfo(name);
            if (jobinfo == null)
            {
                Cookie session_id = AuthCookies.GetCookies(new Uri(ServerUrl)).Cast<Cookie>().First(x => x.Name == "_session_id");
                string response = SendPostRequest(
                    ServerUrl + String.Format(Queries.CREATE_JOB, HttpUtility.UrlEncode(name)),
                    String.Format("data={0}&_session_id={1}", HttpUtility.UrlEncode(config_xml), HttpUtility.UrlEncode(session_id.Value)),
                    "application/x-www-form-urlencoded");
                //Console.WriteLine(response);
                if (response != null)
                    jobinfo = GetJobInfo(name);
            }
            return jobinfo;
        }

        public string GetJobConfig(string name)
        {
            Login();
            string response = SendGetRequest(ServerUrl + String.Format(Queries.CONFIG_JOB, HttpUtility.UrlEncode(name)));
            //Console.WriteLine(response);
            return response;
        }

        public string BuildJob(
            string name,
            string swiftFilename,
            string buildQuery = "")
        {
            Login();

            Cookie session_id = AuthCookies.GetCookies(new Uri(ServerUrl)).Cast<Cookie>().First(x => x.Name == "_session_id");
            string response = SendPostRequest(
                ServerUrl + String.Format(Queries.BUILD_JOB, name, HttpUtility.UrlEncode(swiftFilename), buildQuery),
                String.Format("_session_id={0}", HttpUtility.UrlEncode(session_id.Value)),
                "application/x-www-form-urlencoded");
            return response;
        }

        public class CookieAwareWebClient : WebClient
        {
            public CookieContainer cookies { get; set; }
            public int Timeout { get; set; }
            public bool AllowWriteStreamBuffering { get; set; }

            public CookieAwareWebClient()
            {
                Timeout = HTTP_WEB_REQUEST_TIMEOUT;
            }

            protected override WebRequest GetWebRequest(Uri address)
            {
                WebRequest request = base.GetWebRequest(address);
                HttpWebRequest webRequest = request as HttpWebRequest;
                if (webRequest != null)
                {
                    webRequest.KeepAlive = false;
                    webRequest.CookieContainer = cookies;
                    webRequest.Timeout = Timeout;
                    webRequest.AllowWriteStreamBuffering = AllowWriteStreamBuffering;
                }
                return request;
            }
        }

        public void SaveLastConsole(
            string name,
            string path)
        {
            Login();
            // http://129.59.105.75:8080/job/[jobName]/lastBuild/consoleText
            if (JobExists(name))
            {
                try
                {
                    // TODO: HEAD it and set Timeout appropriately
                    using (CookieAwareWebClient webClient = new CookieAwareWebClient())
                    {
                        webClient.cookies = AuthCookies;
                        webClient.DownloadFile(ServerUrl + String.Format(Queries.LAST_BUILD_CONSOLE_TEXT, HttpUtility.UrlEncode(name)), path);
                    }
                }
                catch (WebException ex)
                {
                    Console.WriteLine(ex.ToString());
                }
            }
        }

        // TODO: implement node functions

        // TODO: implement view functions
        // https://issues.jenkins-ci.org/browse/JENKINS-8927?page=com.atlassian.jira.plugin.system.issuetabpanels:all-tabpanel
        // wget --post-data='name=ViewX&mode=hudson.model.ListView&json={"name": "ViewX", "mode": "hudson.model.ListView"}' http://.../jenkins/createView

        //TODO: implement settings mechanism
        public string ServerUrl { get { return ""; } }

        public Jenkins(string logDir = null)
        {
            if (string.IsNullOrEmpty(logDir) ||
                Directory.Exists(logDir) == false)
            {
                logDir = System.IO.Path.GetTempPath();
            }

            AuthCookies = new CookieContainer();
            try
            {
                // FIXME: What about closing the file stream? Should we do it somewhere?
                this.DebugLog = new StreamWriter(new FileStream(Path.Combine(logDir, "JobManagerLog.txt"), FileMode.Create));
            }
            catch (IOException e)
            {
                if (Marshal.GetHRForException(e) == /* E_SHARING_VIOLATION*/ unchecked((int)0x80070020L))
                {
                    this.DebugLog = new StreamWriter(new FileStream(Path.Combine(logDir, "JobManagerLog-" + Process.GetCurrentProcess().Id + ".txt"), FileMode.Create));
                }
                else
                {
                    throw;
                }
            }
            DebugLog.AutoFlush = true;
        }

        internal delegate void DownloadPercent(int percent);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="zipFile"></param>
        /// <param name="jenkinsJobName"></param>
        /// <param name="callback"></param>
        /// <returns>true on success</returns>
        internal bool DownloadFileFromVF(string jenkinsJobName, ref string clientDownloadUrl)
        {
            Login();
            string downloadUrl = this.ServerUrl + "/testbench/client_download_url?filename=" + HttpUtility.UrlEncode(jenkinsJobName);
            string fileUploadInfo = SendGetRequest(downloadUrl);
            if (fileUploadInfo == null)
            {
                return false;
            }
            dynamic fileUploadInfoJson = Newtonsoft.Json.Linq.JObject.Parse(fileUploadInfo);
            clientDownloadUrl = fileUploadInfoJson["url"].Value;

            return true;
        }

        internal bool DownloadFileFromSwift(string zipFile, string clientDownloadUrl, DownloadPercent callback)
        {
            Login();
            string cleanClientDownloadUrl = clientDownloadUrl;
            if (clientDownloadUrl.Contains("temp_url_sig"))
            {
                cleanClientDownloadUrl = clientDownloadUrl.Substring(0, clientDownloadUrl.IndexOf("temp_url_sig")) + "[redacted]";
            }
            Trace.TraceInformation("Downloading artifacts from {0} to {1}", cleanClientDownloadUrl, zipFile);

            try
            {
                try
                {
                    string status = "connecting";

                    HttpWebRequest request = (HttpWebRequest)WebRequest.Create(clientDownloadUrl);
                    HttpWebResponse response = null;
                    request.Timeout = Jenkins.HTTP_WEB_REQUEST_TIMEOUT_BASE;
                    // webRequest.ReadWriteTimeout = Jenkins.HTTP_WEB_REQUEST_TIMEOUT_BASE;
                    request.KeepAlive = false;
                    request.CookieContainer = this.AuthCookies;

                    FileStream output = new FileStream(zipFile, FileMode.Append, FileAccess.Write);
                    // test resume download:
                    //  output.Seek(0, SeekOrigin.Begin);
                    //  output.Write(new byte[] { (byte)'P', (byte)'K', 3, 4 }, 0, 4);
                    using (output)
                    using (request as IDisposable)
                    {
                        long BytesRead = output.Position;
                        if (BytesRead > 0)
                        {
                            request.AddRange(BytesRead);
                        }

                        try
                        {
                            response = (HttpWebResponse)request.GetResponse();
                            //create network stream
                            var ns = response.GetResponseStream();
                            var acceptRanges = String.Compare(response.Headers["Accept-Ranges"], "bytes", true) == 0;
                            if (acceptRanges == false)
                            {
                                output.Seek(0, SeekOrigin.Begin);
                                BytesRead = 0;
                            }
                            byte[] buffer = new byte[8 * 1024];
                            while (true)
                            {
                                var res = ns.BeginRead(buffer, 0, buffer.Length, null, null);
                                bool readSucceeded = res.AsyncWaitHandle.WaitOne(Jenkins.HTTP_WEB_REQUEST_TIMEOUT_BASE);
                                if (readSucceeded == false)
                                {
                                    throw new WebException(String.Format("Connection timed out {0}", status));
                                }
                                int bytesRead = ns.EndRead(res);
                                if (bytesRead == 0)
                                    break;
                                output.Write(buffer, 0, bytesRead);
                                int percentProgress = (int)(((double)output.Position) / (BytesRead + response.ContentLength) * 99.0);
                                status = String.Format("at {0}%", percentProgress);
                                callback(percentProgress);
                            }
                        }
                        catch (WebException e)
                        {
                            IDisposable disp = e.Response as IDisposable;
                            if (disp != null)
                                disp.Dispose();
                            throw;
                        }
                        callback(100);
                    }
                    Trace.TraceInformation("Download from S3 succeeded {0}", zipFile);
                    return true;
                }
                catch (System.Reflection.TargetInvocationException ex)
                {
                    // FIXME: I don't think this is reachable anymore
                    throw ex.InnerException;
                }

            }
            catch (WebException ex)
            {
                Trace.TraceError(String.Format("Error downloading {0}. Exception follows", cleanClientDownloadUrl));
                Trace.TraceError(ex.ToString());
            }
            return false;
        }


        internal delegate void UploadPercent(int percent);

        internal string UploadFileToVF(string zipFile, string swiftFilename, UploadPercent callback)
        {
            Login();
            string fileUploadInfo = SendGetRequest(ServerUrl + "/testbench/client_upload_url?filename=" + HttpUtility.UrlEncode(swiftFilename));
            if (fileUploadInfo == null)
            {
                // job creation failed
                //job.Status = Job.StatusEnum.Failed;
                return null;
            }
            int maxProgress = 0;
            object maxProgressLock = new object();
            dynamic fileUploadInfoJson = Newtonsoft.Json.Linq.JObject.Parse(fileUploadInfo);
            string fileUploadUrl = fileUploadInfoJson["url"].Value;
            try
            {
                try
                {
                    using (CookieAwareWebClient webClient = new CookieAwareWebClient())
                    {
                        webClient.cookies = this.AuthCookies;
                        webClient.Timeout = HTTP_WEB_TIMEOUT_BASE;
                        Semaphore progress = new Semaphore(0, int.MaxValue);
                        Semaphore completed = new Semaphore(0, 1);
                        UploadFileCompletedEventArgs completedData = null;
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
                        webClient.UploadFileAsync(new Uri(fileUploadUrl), "PUT", zipFile);
                        while (true)
                        {
                            WaitHandle[] handles = new WaitHandle[] { completed, progress };
                            int timeout = HTTP_WEB_TIMEOUT_BASE;
                            lock (maxProgressLock)
                            {
                                // Workaround for Fiddler proxy: Fiddler accepts all the data at once, then uploads to VF
                                if (maxProgress > 98)
                                {
                                    handles = new WaitHandle[] { completed };
                                    timeout = HTTP_WEB_REQUEST_TIMEOUT;
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

                        Trace.TraceInformation("Upload to S3 " + Encoding.UTF8.GetString(completedData.Result));
                        return fileUploadUrl;
                    }
                }
                catch (System.Reflection.TargetInvocationException ex)
                {
                    throw ex.InnerException;
                }

            }
            catch (WebException ex)
            {
                Trace.TraceError("Error uploading to {0}. Exception follows", fileUploadUrl);
                Trace.TraceError(ex.ToString());
            }
            return null;
        }
    }
}
