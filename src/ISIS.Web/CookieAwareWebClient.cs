using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;

namespace ISIS.Web
{
    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class CookieAwareWebClient : System.Net.WebClient
    {
        public CookieContainer AuthCookies { get; set; }
        public int Timeout { get; set; }
        public bool AllowWriteStreamBuffering { get; set; }
        public bool AllowReadStreamBuffering { get; set; } 

        public CookieAwareWebClient()
            : base()
        {
            this.AuthCookies = new CookieContainer();
        }

        protected override WebRequest GetWebRequest(Uri address)
        {
            WebRequest request = base.GetWebRequest(address);
            HttpWebRequest webRequest = request as HttpWebRequest;
            if (webRequest != null)
            {
                webRequest.KeepAlive = false;
                webRequest.CookieContainer = this.AuthCookies;
                webRequest.Timeout = this.Timeout;
                webRequest.AllowWriteStreamBuffering = this.AllowWriteStreamBuffering;
                //webRequest.AllowReadStreamBuffering = this.AllowReadStreamBuffering; // .NET 4.5 only!
            }
            return request;
        }


    }
}
