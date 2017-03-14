using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CyPhyMasterInterpreter
{
    public class InterpreterProgressEventArgs : EventArgs
    {
        public int Percent { get; set; }
        public bool Success { get; set; }
        public META.ComComponent Interpreter { get; set; }
    }
}
