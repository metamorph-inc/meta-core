namespace CyPhyMasterInterpreter
{
    using System;
    using System.Runtime.InteropServices;
    using GME.MGA;

    [Guid("4C0558BE-3482-48EE-88D5-E578FB4BC8A4"),
    ProgId("CyPhyMasterInterpreter.MasterInterpreterResult"),
    ClassInterface(ClassInterfaceType.AutoDual),
    ComVisible(true)]
    public class MasterInterpreterResult
    {
        public IMgaModel Context { get; set; }
        public IMgaFCO Configuration { get; set; }
        public bool Success { get; set; }
        public string Message { get; set; }
        public string Exception { get; set; }
        public string OutputDirectory { get; set; }
    }
}
