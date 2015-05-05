using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;

namespace ISIS.Web
{
    public interface IAuthentication : IDisposable
    {
        TimeSpan SessionTimeOut { get; }

        string LoginUrl { get; }

        string LogoutUrl { get; }

        void Login();

        void Logout();

        bool TryPing();
    }

}
