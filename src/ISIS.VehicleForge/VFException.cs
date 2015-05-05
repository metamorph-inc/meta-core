namespace ISIS.VehicleForge

{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
using System.Runtime.Serialization;

    public class VFException : System.Exception
    {
        public VFException()
            :base()
        {
        }

        public VFException(string message)
            : base(message)
        {
        }

        protected VFException(SerializationInfo info, StreamingContext context)
            : base(info, context)
        {
        }

        public VFException(string message, Exception innerException)
            : base(message, innerException)
        {
        }


        public virtual string Header { get { return "Unknown Error Occurred"; } }
        public override string Message { get { return "VehicleForge Communication Error"; } }
    }

}
