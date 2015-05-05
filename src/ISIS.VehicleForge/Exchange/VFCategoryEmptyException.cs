using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ISIS.VehicleForge
{
    public class VFCategoryEmptyException : VehicleForge.VFException
    {
        public override string Header
        {
            get
            {
                return "VehicleForgeException";
            }
        }

        public override string Message
        {
            get
            {
                return "Selected Component does not have a defined Classfication.";
            }
        }
    }

}
