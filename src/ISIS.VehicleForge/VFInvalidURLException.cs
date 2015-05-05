namespace ISIS.VehicleForge

{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Runtime.Serialization;

    public class VFInvalidURLException : VehicleForge.VFException
    {
        public VFInvalidURLException()
            :base()
        {
        }

        public VFInvalidURLException(string message)
            : base(message)
        {
        }

        protected VFInvalidURLException(SerializationInfo info, StreamingContext context)
            : base(info, context)
        {
        }

        public VFInvalidURLException(string message, Exception innerException)
            : base(message, innerException)
        {
        }

        public override string Header
        {
            get
            {
                return "VehicleForge Communication Error";
            }
        }

    }

}
