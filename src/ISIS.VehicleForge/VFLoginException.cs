namespace ISIS.VehicleForge

{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Runtime.Serialization;

    public class VFLoginException : VehicleForge.VFException
    {
        public VFLoginException()
            :base()
        {
        }

        public VFLoginException(string message)
            : base(message)
        {
        }

        protected VFLoginException(SerializationInfo info, StreamingContext context)
            : base(info, context)
        {
        }

        public VFLoginException(string message, Exception innerException)
            : base(message, innerException)
        {
        }


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
                return "Login Failed; Please verify Username and Password.";
            }
        }
    }

}
