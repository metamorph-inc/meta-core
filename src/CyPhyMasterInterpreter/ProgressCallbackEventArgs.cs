using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using System.Runtime.InteropServices;

namespace CyPhyMasterInterpreter
{
    [ComVisible(true)]
    [ClassInterface(ClassInterfaceType.AutoDispatch)]
    [Guid("B601188E-7F7E-4C20-9A52-745440B18926")]
    public class ProgressCallbackEventArgs
    {
        public double Percent { get; set; }
        public string Context { get; set; }
        public string Configuration { get; set; }
        public string Title { get; set; }
        //public MasterInterpreterResult CurrentResult { get; set; }
    }
}
