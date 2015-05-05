namespace ISIS.VehicleForge

{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    public class VFDownloadFailedException : VehicleForge.VFException
    {
        public override string Header
        {
            get
            {
                return "VehicleForge Download Error";
            }
        }

        public override string Message
        {
            get
            {
                return "Could not download component.";
            }
        }
    }

}
