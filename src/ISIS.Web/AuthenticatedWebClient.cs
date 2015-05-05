using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.IO;
using System.Net;

namespace ISIS.Web
{
    public abstract class AuthenticatedWebClient : WebClient, IAuthentication
    {
        protected TimeSpan m_SessionTimeOut { get; set; }
        protected Credentials m_credentials { get; set; }
        protected DateTime m_lastLogin = DateTime.MinValue;
        protected bool m_loggedIn { get; set; }

        public AuthenticatedWebClient(string serverUrl, Credentials credentials)
            : base(serverUrl, loggingEnabled: true)
        {
            this.m_SessionTimeOut = TimeSpan.FromMinutes(9);
            this.m_credentials = credentials;
            this.m_loggedIn = false;
        }

        #region IAuthentication

        public abstract void Login();

        public virtual void Logout()
        {
            if (this.m_loggedIn)
            {
                base.SendGetRequest(this.LogoutUrl);
                this.m_loggedIn = false;
            }
        }

        public void Dispose()
        {
            this.Logout();
        }

        public abstract string LoginUrl { get; }

        public abstract string LogoutUrl { get; }

        #endregion

        public override bool DownloadFile(string fileName, string url, WebClient.DownloadPercent callback)
        {
            this.Login();
            return base.DownloadFile(fileName, url, callback);
        }

        public override bool UploadFile(string fileName, string url, WebClient.UploadPercent callback)
        {
            this.Login();
            return base.UploadFile(fileName, url, callback);
        }

        public override string SendDeleteRequest(string url)
        {
            this.Login();
            return base.SendDeleteRequest(url);
        }

        public override T SendGetRequest<T>(string url, Dictionary<string, object> query = null)
        {
            this.Login();
            return base.SendGetRequest<T>(url, query);
        }

        public override string SendGetRequest(string url, Dictionary<string, object> query = null)
        {
            this.Login();
            return base.SendGetRequest(url, query);
        }

        public override T SendPostRequest<T>(string url, Dictionary<string, object> query = null, string content = null, string contentType = null)
        {
            this.Login();
            return base.SendPostRequest<T>(url, query, content, contentType);
        }

        public override string SendPostRequest(string url, Dictionary<string, object> query = null, string content = null, string contentType = null)
        {
            this.Login();
            return base.SendPostRequest(url, query, content, contentType);
        }

        public override string SendPutRequest(string url)
        {
            this.Login();
            return base.SendPutRequest(url);
        }

        public TimeSpan SessionTimeOut
        {
            get { return this.m_SessionTimeOut; }
        }

        public abstract bool TryPing();

    }
}
