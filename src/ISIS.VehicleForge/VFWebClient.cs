using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Diagnostics;
using ISIS.Web;

namespace ISIS.VehicleForge
{
    /// <summary>
    /// Handles authentication and request to a VehicleFORGE deployment
    /// </summary>
    public class VFWebClient : AuthenticatedWebClient
    {
        private Dictionary<string, object> m_loginQuery { get; set; }

        public VFWebClient(string serverUrl, Credentials credentials)
            : base(serverUrl, credentials)
        {
            // META-2204: Login information MUST not be sent as query parameters!
            this.m_loginQuery = new Dictionary<string, object>();
        }

        public string AuthUrl
        {
            get { return "/auth/"; }
        }

        public override string LoginUrl
        {
            get { return "/auth/do_login"; }
        }

        public override string LogoutUrl
        {
            get { return "/auth/logout"; }
        }

        public override T SendPostRequest<T>(string url, Dictionary<string, object> query = null, string content = null, string contentType = null)
        {
            query = this.AppendSessionId(query);

            return base.SendPostRequest<T>(url, query, content, contentType);
        }

        public override string SendPostRequest(string url, Dictionary<string, object> query = null, string content = null, string contentType = null)
        {
            query = this.AppendSessionId(query);

            return base.SendPostRequest(url, query, content, contentType);

            //try
            //{
            //    query = this.AppendSessionId(query);

            //    return base.SendPostRequest(url, query, content, contentType);
            //}
            //catch (WebException webEx)
            //{
            //    if (webEx.Status == WebExceptionStatus.Timeout)
            //    {
            //        System.Windows.Forms.MessageBox.Show(
            //            "Timeout WebException in VFWebClient's SendPostRequest...",
            //            "VF Timeout",
            //            System.Windows.Forms.MessageBoxButtons.OK,
            //            System.Windows.Forms.MessageBoxIcon.Error);

            //        throw new VehicleForge.VFLoginException();
            //    }
            //    else
            //    {
            //        throw webEx;
            //    }
            //}
        }

        private Dictionary<string, object> AppendSessionId(Dictionary<string, object> query)
        {
            this.Login();

            if (query == null)
            {
                query = new Dictionary<string, object>();
            }

            Cookie session_id = AuthCookies.GetCookies(new Uri(ServerUrl)).Cast<Cookie>().First(x => x.Name == "_session_id");
            query["_session_id"] = session_id.Value;
            return query;
        }

        //public override string RestUrl
        //{
        //    get { return"/rest"; }
        //}

        public override void Login()
        {
            if (this.m_lastLogin.Add(this.SessionTimeOut) > DateTime.Now)
            {
                return;
            }

            this.m_loggedIn = false;

            Trace.TraceInformation("[VFAuthentication][Login] - Logging in: {0} as {1}", ServerUrl, this.m_credentials.Username);

            // META-447: trash our cookies to get a new session id
            this.AuthCookies = new CookieContainer();

            // get new session id
            // send request without authentication
            try
            {
                this.SendRawGetRequest(this.AuthUrl);
            }
            catch (WebException webEx)
            {
                var status = webEx.Status;

                string message = string.Format("{0}{1}{2}", webEx.Message, Environment.NewLine, webEx.Status);

                throw new VehicleForge.VFInvalidURLException(message, webEx);
            }

            try
            {
                // DO NOT LOG PASSWORD!
                this.LoggingEnabled = false;

                string credentialsUrlEncoded = String.Format(
                    "username={0}&password={1}",
                    System.Web.HttpUtility.UrlEncode(this.m_credentials.Username),
                    System.Web.HttpUtility.UrlEncode(this.m_credentials.Password));

                // TODO: How to detect invalid credentials?
                // send request without authentication
                using (var respone = this.SendRawPostRequestGetResponse(
                    this.LoginUrl,                                // url
                    null,                                         // META-2204 query must not have credential info
                    credentialsUrlEncoded,                            // put credential information into the content
                    ISIS.Web.WebClient.ContentType.FormEncoded))
                {

                    // VehicleForge returns '200' ONLY if failure occurs, so we check what url we are pointed to (login url)
                    if ((int)respone.StatusCode == 200)
                    {
                        string message = string.Format("Cannot login to VehicleForge {0} as {1}", this.ServerUrl, this.m_credentials.Username);
                        throw new VFLoginException(message);
                    }

                    Trace.WriteLine(string.Format("{0} {1} {2}", respone.ResponseUri, respone.StatusCode, respone.IsMutuallyAuthenticated));

                    this.m_lastLogin = DateTime.Now;
                    this.m_loggedIn = true;
                }
            }
            finally
            {
                // enable logging again
                this.LoggingEnabled = true;
            }
        }

        public override bool TryPing()
        {
            bool success = false;
            try
            {
                this.SendGetRequest(this.ServerUrl + "/rest/user/get_user_profile");
                success = true;
            }
            catch (Exception)
            {

                success = false;
            }
            return success;
        }

        public string GetProfile()
        {
            return this.SendGetRequest("/rest/user/get_user_profile");
        }

    }

}
