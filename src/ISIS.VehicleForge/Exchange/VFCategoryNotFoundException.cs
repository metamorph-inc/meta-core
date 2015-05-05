using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
namespace ISIS.VehicleForge
{
    public class VFCategoryNotFoundException : VehicleForge.VFException
    {
        public override string Header
        {
            get
            {
                return "VehicleForge Communication Error";
            }
        }

        public override string Message
        {
            get
            {
                return "No Components found matching this Classification.";
            }
        }

    }
}
