using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;

namespace CyPhyMasterInterpreter
{
    public class ProgressCallbackEventArgs : EventArgs
    {
        public double Percent { get; set; }
        public string Context { get; set; }
        public string Configuration { get; set; }
        public string Title { get; set; }
        //public MasterInterpreterResult CurrentResult { get; set; }
    }
}
