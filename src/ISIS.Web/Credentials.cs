using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;

namespace ISIS.Web
{
    public class Credentials
    {
        //public Uri Url {get; set;}
        public string Url { get; set; }
        public string Username { get; set; }
        public string Password { get; set; }

        public Credentials(string url, string username, string password)
        {
            this.Url = url;
            this.Username = username;
            this.Password = password;
        }


        public Credentials(Credentials otherCredentials)
            : this(otherCredentials.Url, otherCredentials.Username, otherCredentials.Password) 
        { 
        } 
    }

}
