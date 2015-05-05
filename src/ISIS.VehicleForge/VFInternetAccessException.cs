namespace ISIS.VehicleForge

{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    public class VFInternetAccessException : VehicleForge.VFException
    {
        public override string Header
        {
            get
            {
                return "Login Error";
            }
        }

        public override string Message
        {
            get
            {
                return "Login Failed; Please check your Internet Connection.";
            }
        }
    }
}
